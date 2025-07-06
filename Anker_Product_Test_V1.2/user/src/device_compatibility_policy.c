/****************************************************************************
 * @copyright Copyright(C) 2024-2024 Ismartware Limited. All rights reserved.
 * @file device_compatibility_policy.c
 * @brief device_compatibility_policy
 * @author SZ team
 ****************************************************************************/
 
 #include "device_compatibility_policy.h"
 
 //策略全局变量
extern policy_data_t policyGlobalData;


#define PC_POLICY					//笔记本策略
#define PHONE_POLICY			//手机策略
#define OTHER_POLICY			//其他策略


#ifdef PHONE_POLICY
/*	Samsung_Galaxy_S23
* 设备兼容性问题说明
* 原因：设备在直充类协议下重新广播PD，会触发芯片Vout_OVP保护
* 方案：在重新广播PD时，关闭OVP保护5s
* 结果：
*/
void Samsung_Galaxy_S23_Policy(void)
{
	static u8 policyStep = 0;
	static u32 tempTime = 0;
	
	if(policyGlobalData.isOccBroadcast)
	{
		policyGlobalData.isOccBroadcast = false;
		policyStep = 1;
	}
		
	switch(policyStep)
	{
		case 0:
			//不执行操作
			break;
		case 1:
			Vout_Ovp_Ctrl(false);
			tempTime = My_GetSystemTimeMark();
			policyStep = 2;
			break;
		case 2:
			if(My_GetSystemTimePass(tempTime) > 5000)
			{
				Vout_Ovp_Ctrl(true);
				policyStep = 0;
			}
			break;
		default:
			break;
	}
}

#endif
 
#ifdef OTHER_POLICY
/*	Thunderbolt 4 (USB-C) Pro Cable (1.8M / 3M)
* 设备兼容性问题说明
* 原因：线缆会触发芯片VCONN_OCP
* 方案：当触发线缆时不使能Vconn
* 结果：
*/
void Thunderbolt_Cable_Policy(void)
{
	static u8 policyStep = 0;
	config_data_t* config = Config_Get();
	
	switch(policyStep)
	{
		case 0:
			//检测异常状态 VCONN_OCP
			if((Get_RegAddr_Data(0x89) & (0x03 << 5)))
			{
				policyStep = 1;
			}
			break;
		case 1:
			//关闭VCONN检测使能
			config->isTypecVconnEnabled = false;		
			policyStep = 2;
			break;
		case 2:
			//检测设备离线，重新打开VCONN检测使能
			if(!((Get_RegAddr_Data(0x85) & (0x01 << 7)) && (Get_RegAddr_Data(0x86) & (0x01 << 7))))
			{
				config->isTypecVconnEnabled = true;		
				policyStep = 0;				
			}
			break;
		default:
			break;
	}	
}

#endif

 
//设备兼容性策略
void Device_Compatibility_Policy(void)
{
	Samsung_Galaxy_S23_Policy();
	Thunderbolt_Cable_Policy();
}
 
 
 