/****************************************************************************
 * @copyright Copyright(C) 2024-2024 Ismartware Limited. All rights reserved.
 * @file iic_slave_callback_policy.h
 * @brief main_policy
 * @author SZ team
 ****************************************************************************/
#include "iic_slave_callback_policy.h"
#include "anker_policy.h"
#include "pd_message_decode.h"

#include "log.h"
#include "string.h"

#define PPS_UFCS_CUR_OFFSET

static const pd_mesg_summ_t* pdRxMessageSummary;
static const u8* pdRxBuff;
pd_mesg_summ_t* pdTxMessageSummary;
static u8* pdTxBuff;

//策略全局变量
extern policy_data_t policyGlobalData;

//异常发生
void Abnormal_Occurred(u32 pending)
{
	Set_RegAddr_Data(0x89, (pending & (0xFF << 0)) >> 0);
	Set_RegAddr_Data(0x8A, (pending & (0xFF << 8)) >> 8);
	Set_RegAddr_Data(0x8B, (pending & (0xFF << 16)) >> 16);
	Set_RegAddr_Data(0x8C, (pending & (0xFF << 24)) >> 24);		
}

//异常恢复
void Abnormal_Release(u32 pending)
{
	
}

//调流调压
bool Power_Adjust_Fun(uint16_t* vol, uint16_t* port1Curr, uint16_t* port2Curr)
{
	//不进行调压调流操作
	if(policyGlobalData.notAllowAdjustVolCur)
	{
		policyGlobalData.notAllowAdjustVolCur = false;
		return false;
	}
	//发生调压调流
	policyGlobalData.isAdjustCurVol = true;
	
	#ifdef PPS_UFCS_CUR_OFFSET
	
	//Port_PPS_UFCS_强制限流,根据UFCS_PPS0最大电流
	bool forceCurUfcs = (Get_RegAddr_Data(0x82) & (1 << 3)) >> 3;	
	//ufcs_pps0最大电流
	u8 ufcsMaxCur = Get_RegAddr_Data(0x55);

	//Port1_端口状态
	u8 port1State = Get_RegAddr_Data(0x85);
	//Port1_PPS_UFCS电流偏置
	u8 port1Offest = Get_RegAddr_Data(0x80);
	
	//Port2_端口状态
	u8 port2State = Get_RegAddr_Data(0x86);
	//Port2_PPS_UFCS电流偏置
	u8 port2Offest = Get_RegAddr_Data(0x81);
	
	if(port1State & (0x01 << 7))
	{
		switch((port1State & (0x1F << 1)) >> 1)
		{
			case FC_PD_PPS:
				*port1Curr = *port1Curr + ((((port1Offest & (0x0F << 4)) >> 4) * 100 - 500 ) / 25);
				return true;				
				break;
			case FC_UFCS:
				if(forceCurUfcs)
				{
					*port1Curr = ufcsMaxCur * 50;
				}
				*port1Curr = *port1Curr + ((((port1Offest & (0x0F << 0)) >> 0) * 100 - 500 ) / 25);
				return true;					
				break;
			default :
				break;
		}
	}
	else if(port2State & (0x01 << 7))
	{
		switch((port2State & (0x1F << 1)) >> 1)
		{
			case FC_PD_PPS:
				*port2Curr = *port2Curr + ((((port2Offest & (0x0F << 4)) >> 4) * 100 - 500 ) / 25);
				return true;				
				break;
			case FC_UFCS:
				if(forceCurUfcs)
				{
					*port2Curr = ufcsMaxCur * 50;
				}				
				*port2Curr = *port2Curr + ((((port2Offest & (0x0F << 0)) >> 0) * 100 - 500 ) / 25);
				return true;					
				break;
			default :
				break;
		}
	}
	#endif
	
	return false;
}

//Typec插入
void User_Typec_Attach_Hook(u8 portIndex)
{

}

//Typec拔出
void User_Typec_Detach_Hook(u8 portIndex)
{
	Set_RegAddr_Data(0xA0, 0x00);
	Set_RegAddr_Data(0xA1, 0x00);
}

//PD请求挡位、电压、电流
static void User_Pd_Request_Info_Hook(u8 objIndex, u16 volInMv, u16 currInMa)
{
	//电压
	Set_RegAddr_Data(0x9C, (volInMv & (0xFF << 8)) >> 8);
	Set_RegAddr_Data(0x9D, (volInMv & (0xFF << 0)) >> 0);	
	//电流
	Set_RegAddr_Data(0x9E, (currInMa & (0xFF << 8)) >> 8);
	Set_RegAddr_Data(0x9F, (currInMa & (0xFF << 0)) >> 0);		
}

//PD二次开发
static bool User_Pd_Policy_Hook(void)
{
	uint8_t* buff_start_addr;
	VDM_Header_u vdm_header;

	u8 sop1ProductType = 0;//bit29-27   3
	u8 cableLatency = 0;//bit16-13      4
	u8 maxVol = 0;//bit10-9             2
	u8 maxCur = 0;//bit6-5              2
	u8 usbHighestSpeed = 0;//bit2-0     3
	
	//判断属于PD接收命令
	if(Pd_Is_Event_Happen(PD_RX_MESSAGE_SUCCESS_EVENT))
	{
		// 判断属于线缆
		if(PD_SOP1 == pdRxMessageSummary->packet_start || PD_SOP2 == pdRxMessageSummary->packet_start)
		{

		}
		if(PD_SOP == pdRxMessageSummary->packet_start)
		{
			//判断属于VDM信息类型
			// discoverid 信息模板：Message_Header(2Byte) + VDM_Header(32bit 4Byte) + ID_Header_VDO(32bit 4Byte) +\
			// Cert_Start_VDO(32bit 4Byte) + Product_VDO(32bit 4Byte) + Cable VDO1(32bit 4Byte) + Cable VD02(32bit 4Byte)
			if(PD_Vendor_Defined == pdRxMessageSummary->mesg_type)
			{
				buff_start_addr = (uint8_t *)(Pd_Get_Rx_Buff_Start_Address(FALSE) + 2);	// 获取 VDM_Header 数据
				memcpy((uint8_t *)&vdm_header, buff_start_addr, 4);
				
				if(vdm_header.SVID == ANKER_VID)	// 是否为安克创新的VID
				{
					if(PD_UVDM_Anker_Package.Memory_Sub != 0)	// 互斥，已存在数据不可进行写操作
																// 可能项：已经获取到PD数据，并通过UART透传给MCU / UART_RX 接收到 MCU数据
																// 后续会在该处加超时机制
					{
						return false;
					}
					if(Anker_UVDM_Decode())			// 安克UVDM解码完成
					{
						PD_UVDM_Anker_Package.Product_Test_Step = 0x01;
						Pd_Clear_Event(PD_RX_MESSAGE_SUCCESS_EVENT);
					}
				}
			}			
		}
		#if 0
		//判断属于Sink设备
		else if(PD_SOP == pdRxMessageSummary->packet_start)
		{
			//判断属于Requset
			if(PD_Request == pdRxMessageSummary->mesg_type || PD_EPR_Request == pdRxMessageSummary->mesg_type)
			{
				tempData = *(u32*)(Pd_Get_Rx_Buff_Start_Address(FALSE) + 2);
				Set_RegAddr_Data(0xA2, (tempData & (0xFF << 24)) >> 24);
				Set_RegAddr_Data(0xA3, (tempData & (0xFF << 16)) >> 16);
				Set_RegAddr_Data(0xA4, (tempData & (0xFF << 8)) >> 8);
				Set_RegAddr_Data(0xA5, (tempData & (0xFF << 0)) >> 0);
			}
			//判断属于VDM消息
			else if(PD_Vendor_Defined == pdRxMessageSummary->mesg_type)
			{
				pdRxBuff = Pd_Get_Rx_Buff_Start_Address(FALSE);
				msgHeader = *(u16*)(pdRxBuff);
				msgDataObjectNum = (msgHeader & (0x07 << 12)) >> 12;
				//判断数据长度
				if(msgDataObjectNum >= 2)
				{
					vdmHeader = *(u32*)(pdRxBuff + 2);
					//判断Anker产测功能(Anker_VID)
					if(((vdmHeader & 0xFFFF << 16) >> 16) == 0x291A)
					{
						tempData = *(u32*)(pdRxBuff + 6);	
						//Anker_产测指令
						if(((tempData & 0xFFFF << 16) >> 16) == 0x416B)
						{
							//填充数据头
							Anker_Enter_SqQueue(&policyGlobalData.curAnkerSqQueue, (msgHeader & (0xFF << 8)) >> 8);
							Anker_Enter_SqQueue(&policyGlobalData.curAnkerSqQueue, (msgHeader & (0xFF << 0)) >> 0);
							//获取全部数据
							for(uint8_t i = 0; i < msgDataObjectNum; i++)
							{
								//填充数据
								tempData = *(u32*)(pdRxBuff + (2 + (i * 4)));
								Anker_Enter_SqQueue(&policyGlobalData.curAnkerSqQueue, (tempData & (0xFF << 24)) >> 24);
								Anker_Enter_SqQueue(&policyGlobalData.curAnkerSqQueue, (tempData & (0xFF << 16)) >> 16);								
								Anker_Enter_SqQueue(&policyGlobalData.curAnkerSqQueue, (tempData & (0xFF << 8)) >> 8);
								Anker_Enter_SqQueue(&policyGlobalData.curAnkerSqQueue, (tempData & (0xFF << 0)) >> 0);									
							}
							//接收到产测数据
							policyGlobalData.ankerProductTestStep = 1;
						}
					}
				}
			}
		}
		#endif
	}
	else if(Pd_Is_Event_Happen(PD_TX_MESSAGE_SUCCESS_EVENT))
	{
		if(PD_UVDM_Anker_Package.Product_Test_Step == 0x04)
		{
			PD_UVDM_Anker_Package.Product_Test_Step = 0;
			Pd_Clear_Event(PD_TX_MESSAGE_SUCCESS_EVENT);
		}

	}
  return FALSE; 
}


//初始化回调接口
void Init_CallBack(void)
{
	pdRxMessageSummary = Pd_Init_Rx_Message_Summary();   //get rx message summary 
	pdTxMessageSummary = Pd_Init_Tx_Message_Summary();   //get tx message summary 	
	//调流调压
	Device_Register_Power_Adjust_Hook(Power_Adjust_Fun);
	//异常
	Abnormal_Init_Hook_Func(Abnormal_Occurred, Abnormal_Release);
	//Typc插入拔出
	Typec_Init_Hook_Func(User_Typec_Attach_Hook, User_Typec_Detach_Hook);
	//pd请求电压电流
	Pd_Init_Hook_Func(User_Pd_Request_Info_Hook, User_Pd_Policy_Hook);	
}




