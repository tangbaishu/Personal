/****************************************************************************
 * @copyright Copyright(C) 2024-2024 Ismartware Limited. All rights reserved.
 * @file anker_policy.c
 * @brief
 * @author SZ team
 ****************************************************************************/

#include "anker_policy.h"
#include "pd_message_decode.h"
#include "api.h"


PD_UVDM_Anker_Package_t PD_UVDM_Anker_Package;

// 策略全局变量
extern policy_data_t policyGlobalData;

/**
 * @brief 应答安克产测设备发送的UVDM消息
 */
static void Anker_Response_UVDM(void);

// 串口使能
void Anker_Uart_Enable()
{
	/* uart pinmux */
	Rcc_Module_Clk_Enable(GPIO_ID);

	Gpio_Function_Select(UART_TX_GPIO10, UART_TX_FUNC); /* tx */
	Gpio_Function_Select(UART_RX_GPI011, UART_RX_FUNC); /* rx */

	/* clk,reset */
	Rcc_Module_Rst(UART_ID);
	Rcc_Module_Clk_Enable(UART_ID);
	Rcc_Module_Rst_Release(UART_ID);

	Uart_Mode_Cfg(MODE_TXRX);
	Uart_Baudrate_Cfg(ANKER_BAUDRATE);
	Uart_TX_Enable();
	Uart_RX_Enable();

	Nvic_Irq_Enable(UART_IRQn, PRIORITY_1);
	Uart_Interrupt_Enable(UART_TX_FINISH_INT_EN | UART_RX_FINISH_INT_EN);
	Uart_Clear_Pending(UART_TX_FINISH_INT_PENDING | UART_RX_FINISH_INT_PENDING);
}

// 串口使能
void Anker_Uart_Disable()
{
	Rcc_Module_Clk_Disable(UART_ID);
	Uart_Deinit();
	Uart_TX_Disable();
	Uart_RX_Disable();
	Uart_Interrupt_Disable(UART_TX_FINISH_INT_EN | UART_RX_FINISH_INT_EN);
	Uart_Clear_Pending(UART_TX_FINISH_INT_PENDING | UART_RX_FINISH_INT_PENDING);
}

// 串口中断
void UART_Handler()
{
	static u8 tx_send_count = 1;
	// 接收数据
	if (Uart_Get_Int_Pending(UART_RX_FINISH_INT_PENDING))
	{
		if (PD_UVDM_Anker_Package.Product_Test_Step == 0x02)	// 若当前处于 待接收 安克产测数据阶段
		{
			if(PD_UVDM_Anker_Package.Memory_Sub == 0)		// 互斥锁是否解锁
			{
				PD_UVDM_Anker_Package.Product_Test_Step = 0x03;	// 接收UART数据
			}
		}
		if (PD_UVDM_Anker_Package.Product_Test_Step == 0x03)
		{
			PD_UVDM_Anker_Package.Memory_Area[PD_UVDM_Anker_Package.Memory_Sub++] = Uart_RX_Byte();
		}
		Uart_Clear_Pending(UART_RX_FINISH_INT_PENDING);
	}
	if (Uart_Get_Int_Pending(UART_TX_FINISH_INT_PENDING))
	{
		Uart_Clear_Pending(UART_TX_FINISH_INT_PENDING);
		if(tx_send_count < PD_UVDM_Anker_Package.Memory_Sub)
		{
			Uart_TX_Byte(PD_UVDM_Anker_Package.Memory_Area[tx_send_count++]);
		}
		else
		{
			PD_UVDM_Anker_Package.Memory_Sub = 0;			// 释放互斥锁
			tx_send_count = 1;
		}
	}
}

bool Anker_Uart_Rec_Check(void)
{
	static bool is_rec_mes_header = false;
	if (PD_UVDM_Anker_Package.Product_Test_Step != 0x03)	// 当前未接收到 任何UART数据
	{
		is_rec_mes_header = false;
		return false;
	}
	if((PD_UVDM_Anker_Package.Memory_Sub) > 1 && (is_rec_mes_header == 0)) // 接收到 Message_Header
	{
		Transition_Bytes_Bit16((uint8_t *)&PD_UVDM_Anker_Package.PD_Message_Header);
		is_rec_mes_header = true;
	}
	if (PD_UVDM_Anker_Package.Memory_Sub >= (PD_UVDM_Anker_Package.PD_Message_Header.Number_of_Data_Objects * 4 + 2))
	{
		is_rec_mes_header = false;
		return true;
	}
	return false;
}

// anker产测功能
void Anker_Product_Test_Policy(void)
{
	static u8 curStep = 0;
	static uint32_t data_life_cycle; // Anker产测数据存在时间
	static u8 start_send_data = 1;
	switch (curStep)
	{
		case 0x00:
			// 等待接收VDM消息
			if (PD_UVDM_Anker_Package.Product_Test_Step == 0x01)
			{
				// 串口功能使能
				Anker_Uart_Enable();
				curStep = 0x01;
				start_send_data = 1;
			}
			break;
		case 0x01:
			// 发送串口数据
			if(start_send_data)
			{
				start_send_data = 0;
				Uart_TX_Byte(PD_UVDM_Anker_Package.Memory_Area[0]);
				return;
			}
			if(PD_UVDM_Anker_Package.Memory_Sub == 0)
			{
				// 发送数据完成
				PD_UVDM_Anker_Package.Product_Test_Step = 0x02;	// 等待接收 UART数据
				curStep = 0x02;
			}
			break;
		case 0x02:
			// 接收串口数据
			if (Anker_Uart_Rec_Check())
			{
				curStep = 0x03;
			}
			break;
		case 0x03:
			// 发送VDM消息
			Anker_Response_UVDM();
			PD_UVDM_Anker_Package.Memory_Sub = 0; // 释放互斥锁
			curStep = 0x04;
			break;
		case 0x04:
			curStep = 0x00;
			Anker_Uart_Disable();
			break;
		default:
			// 复位清空数据
			break;
	}
	if(PD_UVDM_Anker_Package.Product_Test_Step)
	{
		Low_Power_Not_Sleep();
	}
		
	if(PD_UVDM_Anker_Package.Memory_Sub != 0)
	{
		if(data_life_cycle == 0)
		{
			data_life_cycle = My_GetSystemTimeMark();
		}
		if(My_GetSystemTimePass(data_life_cycle) >= 100)
		{
			PD_Send_Not_Suppertd();
			PD_UVDM_Anker_Package.Product_Test_Step = 4;
			PD_UVDM_Anker_Package.Memory_Sub = 0;
			curStep = 0x04;
		}
	}
	else
	{
		data_life_cycle = 0;
	}
}

bool Anker_UVDM_Decode(void)
{
	uint8_t* buff_start_addr;

	PD_UVDM_Anker_Package.Memory_Sub = 2;	// 互斥锁
	buff_start_addr = (uint8_t *)(Pd_Get_Rx_Buff_Start_Address(FALSE));		// 获取 Message_Header 数据
	// memcpy((uint8_t *)&PD_UVDM_Anker_Package.PD_Message_Header, buff_start_addr, 2);
	My_Memory_Byte_Copy((uint8_t *)&PD_UVDM_Anker_Package.PD_Message_Header, buff_start_addr, 2);

	buff_start_addr = (uint8_t *)(Pd_Get_Rx_Buff_Start_Address(FALSE) + 2);	// 获取 VDM_Header 数据
	// memcpy((uint8_t *)&PD_UVDM_Anker_Package.VDM_Header, buff_start_addr, 4);
	My_Memory_Byte_Copy((uint8_t *)&PD_UVDM_Anker_Package.VDM_Header, buff_start_addr, 4);
	PD_UVDM_Anker_Package.Memory_Sub += 4;			

	buff_start_addr = (uint8_t *)(Pd_Get_Rx_Buff_Start_Address(FALSE) + 6);	// 获取MagicNumber数据
	// memcpy((uint8_t *)&(PD_UVDM_Anker_Package.MagicNumber), buff_start_addr, 4); 
	My_Memory_Byte_Copy((uint8_t *)&(PD_UVDM_Anker_Package.MagicNumber), buff_start_addr, 4);
	PD_UVDM_Anker_Package.Memory_Sub += 4;

	if(PD_UVDM_Anker_Package.MagicNumber.Data_sub2 == 0x6B && PD_UVDM_Anker_Package.MagicNumber.Data_sub3 == 0x41)
	{
		uint8_t vdo_num = PD_UVDM_Anker_Package.PD_Message_Header.Number_of_Data_Objects;
		Transition_Bytes_Bit16((uint8_t *)&PD_UVDM_Anker_Package.PD_Message_Header);
		Transition_Bytes_Bit32((uint8_t *)&PD_UVDM_Anker_Package.VDM_Header);
		Transition_Bytes_Bit32((uint8_t *)&PD_UVDM_Anker_Package.MagicNumber);
		
		vdo_num -= 2;	// 去除 VDM_Header、MagicNumber
		for(uint8_t num=0; num < vdo_num; num++)
		{
			buff_start_addr = (uint8_t *)(Pd_Get_Rx_Buff_Start_Address(FALSE) + 10 + num * 4);	// 获取 VDO[0~4] 数据
			// memcpy((uint8_t *)&(PD_UVDM_Anker_Package.VDO_Array[num]), buff_start_addr, 4); 
			My_Memory_Byte_Copy((uint8_t *)&(PD_UVDM_Anker_Package.VDO_Array[num]), buff_start_addr, 4); 
			PD_UVDM_Anker_Package.Memory_Sub += 4;
			Transition_Bytes_Bit32((uint8_t *)&(PD_UVDM_Anker_Package.VDO_Array[num]));
		}
		return TRUE;
	}
	else	// 无效数据
	{	
		PD_UVDM_Anker_Package.Memory_Sub = 0;	// 释放互斥锁
		return FALSE;
	}
	return TRUE;
}

/**
 * @brief 应答安克产测设备发送的UVDM消息
 */
static void Anker_Response_UVDM(void)
{
	Transition_Bytes_Bit32((uint8_t *)&PD_UVDM_Anker_Package.VDM_Header);	// VDM_Header
	Transition_Bytes_Bit32((uint8_t *)&PD_UVDM_Anker_Package.MagicNumber);	// MagicNumber
	for (uint8_t num = 0; num < (PD_UVDM_Anker_Package.PD_Message_Header.Number_of_Data_Objects - 2); num++)
	{
		Transition_Bytes_Bit32((uint8_t *)&(PD_UVDM_Anker_Package.VDO_Array[num]));
	}
	PD_Send_Message_Data((uint8_t*)&PD_UVDM_Anker_Package.VDM_Header,\
							PD_UVDM_Anker_Package.PD_Message_Header.Number_of_Data_Objects,\
							0);
	PD_UVDM_Anker_Package.Product_Test_Step = 0x04;
}