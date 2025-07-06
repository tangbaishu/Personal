/****************************************************************************
 * @copyright Copyright(C) 2024-2024 Ismartware Limited. All rights reserved.
 * @file iic_slave_drive_policy.c
 * @brief driver_policy
 * @author SZ team
 ****************************************************************************/
 
#include "iic_slave_drive_policy.h"


//策略全局变量
extern policy_data_t policyGlobalData;


//ATM0中断
void ATM0_Handler(void)
{
	 Atm0_Clear_Int_Pending();	
	 policyGlobalData.systemTimeCounter += 4;//根据定时器配置中断进入时间进行累加
}

//ATM0定时器_使能
void Atm0_Custom_Enable(void)
{
	Atm0_Clear_Int_Pending();	
	Atm0_Int_Enable();
	Atm0_Enable();	
}

//ATM0定时器_取消使能
void Atm0_Custom_Disable(void)
{
	Atm0_Clear_Int_Pending();	
	Atm0_Int_Disable();
	Atm0_Disable();		
}

//IICSlave中断
void I2CS_Handler()
{
	volatile u8 tempRegAddr = 0;
	volatile u8 tempRegData[3] = { 0 , 0 , 0 };
	volatile static u8 finishEnableFlag = 0;
	
	Low_Power_Not_Sleep();
	//IIC读操作中断
	if(I2c_Slave_Get_Pending(I2C_S_TX_DATA_PENGING))
	{
		//获取设备地址
		tempRegAddr = I2c_Slave_Get_Register_Address();
		//更新填充数据
		tempRegData[0] = tempRegAddr;
		tempRegData[1] = policyGlobalData.regDataArray[tempRegAddr];
		tempRegData[2] = ~(tempRegData[0] + tempRegData[1]);
		//填充数据到Fifo
		I2c_Slave_Write_TX_Data((u8 *)&tempRegData, 3);
		//清除TX中断信号
		I2c_Slave_Clear_Pending(I2C_S_TX_DATA_PENGING);
		//使能Finish中断
		finishEnableFlag = 1;
		//清除FINISH中断信号
		I2c_Slave_Clear_Pending(I2C_S_FINISH_PENGING);
		//使能Finish中断
		I2c_Slave_Int_Enable(I2C_S_FINISH_INT_EN);		
	}
	//IIC写操作中断
	else if(I2c_Slave_Get_Pending(I2C_S_RX_DATA_PENGING))
	{
		//获取设备地址
		tempRegAddr = I2c_Slave_Get_Register_Address();
		//获取接收数据
		I2c_Slave_Get_RX_Data((u8 *)&tempRegData, 1);
		//只更新显示寄存器数据
		Set_RegAddr_Data(tempRegAddr, tempRegData[0]);
		//进入队列
		Enter_SqQueue(&policyGlobalData.regAddrFifo, tempRegAddr);		
		//接收数据后进行复位
		I2c_Slave_RX_FIFO_Reset();
		//清除RX中断信号
		I2c_Slave_Clear_Pending(I2C_S_RX_DATA_PENGING);
	}
	//IIC_Finish中断
	else if(I2c_Slave_Get_Pending(I2C_S_FINISH_PENGING))
	{
		//判断是否开启Finish中断
		if(finishEnableFlag)
		{
			//发送完数据进行复位
			I2c_Slave_TX_FIFO_Reset();		
			//关闭Finish中断
			I2c_Slave_Int_Disable(I2C_S_FINISH_INT_EN);
			//清除Finish中断
			I2c_Slave_Clear_Pending(I2C_S_FINISH_PENGING);
			//使能Finish中断使能标志清除
			finishEnableFlag = 0;
		}
		
	}
}







