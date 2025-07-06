/****************************************************************************
 * @copyright Copyright(C) 2024-2024 Ismartware Limited. All rights reserved.
 * @file iic_slave_main_policy.c
 * @brief main_policy
 * @author SZ team
 ****************************************************************************/
#include "iic_slave_main_policy.h"

//策略全局变量
extern policy_data_t policyGlobalData;


//是否开始运行主程序
void Main_Start_Run_Policy(void)
{
	u32 tempTime = My_GetSystemTimeMark();
	while(!policyGlobalData.startRun)
	{
		//写策略
		Write_Policy();
		//2s内未操作开buck就恢复默认输出
		if(My_GetSystemTimePass(tempTime) > 2000)
		{
			policyGlobalData.startRun = true;
		}
	}
}


//GPIO策略
void Gpio_Policy(void)
{
	//获取寄存器值
	static u8 oldTypeC = 0;
	u8 typeCPort1CC1 = (Get_RegAddr_Data(0x88) & (0x01 << 7)) >> 7;
	u8 typeCPort1CC2 = (Get_RegAddr_Data(0x88) & (0x01 << 5)) >> 5;
	u8 newTypeC = typeCPort1CC1 | (typeCPort1CC2 << 1);
	if(newTypeC != oldTypeC)
	{
		oldTypeC = newTypeC;
		switch(oldTypeC)
		{
			case 0://CC都不在线
				break;
			case 1://CC1在线
				if(policyGlobalData.iicDeviceAddr == 0x3E)
				{
					TYPEC_GPIO3_0;
				}
				else
				{
					TYPEC_GPIO3_1;
				}
				break;
			case 2://CC2在线
				if(policyGlobalData.iicDeviceAddr == 0x3E)
				{
					TYPEC_GPIO3_1;
				}
				else
				{
					TYPEC_GPIO3_0;
				}				
				break;
			default:
				break;
		}
	}
}

//睡眠策略
void Sleep_Policy(void)
{
	//睡眠策略-0x8F-bit0-写1退出休眠
	//1是不休眠-运行Low_Power_Not_Sleep()
	//0是休眠-不运行Low_Power_Not_Sleep()
	static u32 delaySleepTime;
	static u8 sleepStep = 0;
	u8 regSleepState = Get_RegAddr_Data(0x7F);
	//退出休眠逻辑
	if(regSleepState)
	{
		sleepStep = 0;
		Low_Power_Not_Sleep();
	}	
	switch(sleepStep)
	{
		case 0:
			//不休眠-运行Low_Power_Not_Sleep()
			Low_Power_Not_Sleep();
			//开始进入休眠逻辑
			if(!regSleepState)
			{
				sleepStep = 1;
			}
			break;
		case 1:
			Low_Power_Not_Sleep();
			delaySleepTime = My_GetSystemTimeMark();
			sleepStep = 2;
			break;
		case 2:
			Low_Power_Not_Sleep();
			if(My_GetSystemTimePass(delaySleepTime) > 20000)
			{
				sleepStep = 3;
			}
			break;
		case 3:
			//进入休眠-不运行Low_Power_Not_Sleep()
			break;
		default:
			break;
	}
	// sleep process
	Low_Power_Sleep();
}


//从机主策略
void Main_Policy(void)
{
	//写策略
	Write_Policy();
	//读策略
	Read_Policy();
	//兼容性策略
	Device_Compatibility_Policy();
	//Anker产测功能
	Anker_Product_Test_Policy();
	//GPIO策略
	Gpio_Policy();
	//睡眠策略
	Sleep_Policy();
}





