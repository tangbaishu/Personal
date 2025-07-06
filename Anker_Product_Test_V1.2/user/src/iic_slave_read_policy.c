/****************************************************************************
 * @copyright Copyright(C) 2024-2024 Ismartware Limited. All rights reserved.
 * @file iic_slave_read_policy.c
 * @brief read_policy
 * @author SZ team
 ****************************************************************************/
#include "iic_slave_read_policy.h"

//策略全局变量
extern policy_data_t policyGlobalData;

#define READ_POLICY
#define WRITE_REG				//0x82
#define API_STATE_REG		//0x85-0x8F
#define DATA_REG				//0x90-0x9F
#define EXTEND_REG			//0xA0

//0x82
#ifdef WRITE_REG
/*	IRQ相关功能
* @name   Execute_Register82_Function
* bit7-6：GPIO2_IRQ: 0、设备插入  1、发生调流调压（发送中断拉低IRQ引脚）
* bit5：发生中断信号，写1清0
* bit4：掉电标志-写入1之后，只有在初始化时才会清零。
* bit3-0：
* @param  regData
* @retval None      
*/
void Execute_Register82_Write_Function(void)
{
	//设备插入中断步骤
	static u8 deviceAttachStep = 0;
	//调流调压中断
	static u8 adjustCurVolStep = 0;	
	//获取状态
	u8 regData = Get_RegAddr_Data(0x82);
	//获取设备在线状态
	u8 portOnlineState = (Get_RegAddr_Data(0x85) & (0x01 << 7)) | (Get_RegAddr_Data(0x86) & (0x01 << 7));
	//判断当前中断信号是否触发--防止未清除中断，导致功能失效
	if((regData & (0x01 << 5)) >> 5)
	{
		//检测设备都离线
		if(!portOnlineState)
		{
			//中断标志恢复
			Set_RegAddr_Data(0x82, (regData & ~(0x01 << 5)));
			//IRQ引脚恢复
			IRQ_GPIO2_0;
			//重新开始检测
			deviceAttachStep = 0;
			adjustCurVolStep = 0;
			//清空标志
			policyGlobalData.isAdjustCurVol = false;			
		}
	}
	
	switch((regData & (0x03 << 6)) >> 6)
	{
		case 0:	//端口插入IRQ
			switch(deviceAttachStep)
			{
				case 0:
					if(portOnlineState)
					{
						//发生中断标志
						Set_RegAddr_Data(0x82, (regData | (0x01 << 5)));
						//拉低IRQ引脚
						IRQ_GPIO2_1;	
						//进入下一步
						deviceAttachStep = 1;
					}
					break;
				case 1:
					//检测当前中断信号是否清除
					if(!((regData & (0x01 << 5)) >> 5))
					{
						//恢复IRQ引脚
						IRQ_GPIO2_0;	
						//进入下一步
						deviceAttachStep = 2;				
					}
					break;
				case 2:
					//检测设备都离线
					if(!portOnlineState)
					{
						//重新开始检测
						deviceAttachStep = 0;							
					}					
					break;
				default:
					break;
				
			}
		
			break;
		case 1: //调流调压中断-
			switch(adjustCurVolStep)
			{
				case 0:
					if(policyGlobalData.isAdjustCurVol)
					{
						//发生中断标志
						Set_RegAddr_Data(0x82, (regData | (0x01 << 5)));
						//拉低IRQ引脚
						IRQ_GPIO2_1;	
						//进入下一步
						adjustCurVolStep = 1;		
					}
					break;
				case 1:
					//检测当前中断信号是否清除
					if(!((regData & (0x01 << 5)) >> 5))
					{
						//恢复IRQ引脚
						IRQ_GPIO2_0;	
						//进入下一步
						adjustCurVolStep = 0;		
						//清空标志
						policyGlobalData.isAdjustCurVol = false;						
					}					
					break;
			}
			break;
		default:
			break;
	}	
}


#endif


//0x85-0x8F
#ifdef API_STATE_REG
/*	端口状态 0x85-0x86
* @name   Execute_Register85_And_Register86_Function
* bit7：	设备是否在线
* bit6：	设备是否处于快充
* bit5-1: 端口协议	
* bit0：	
* @param  regData
* @retval None      
*/
void Execute_Register85_And_Register86_Function(void)
{
	//当前异常步骤
	static u8 abnOccurStep = 0;
	static u32 curTime = 0;
	u8 tempData = 0;
	
	//出现异常状态不更新设备状态
	if(Get_RegAddr_Data(0x89) | Get_RegAddr_Data(0x8A) | Get_RegAddr_Data(0x8B) | Get_RegAddr_Data(0x8C))
	{
		abnOccurStep = 1;
	}
	
	switch(abnOccurStep)
	{
		case 0:
			tempData = Device_Get_Port_State();
			switch(tempData)
			{
				case 1:
					//Port1
					Set_RegAddr_Data(0x85, (0x01 << 7) | Device_Is_In_Fast_Charge() << 6 | Device_Get_Protocol(0x01) << 1);
					Set_RegAddr_Data(0x86, 0x00);
					break;
				case 2:
					//Port2
					Set_RegAddr_Data(0x85, 0x00);
					Set_RegAddr_Data(0x86, (0x01 << 7) | Device_Is_In_Fast_Charge() << 6 | Device_Get_Protocol(0x02) << 1);
					break;
				case 3:
					//双口在线
					Set_RegAddr_Data(0x85, (0x01 << 7) | Device_Is_In_Fast_Charge() << 6 | Device_Get_Protocol(0x01) << 1);
					Set_RegAddr_Data(0x86, (0x01 << 7) | Device_Is_In_Fast_Charge() << 6 | Device_Get_Protocol(0x02) << 1);
					break;
				default:
					Set_RegAddr_Data(0x85, 0x00);
					Set_RegAddr_Data(0x86, 0x00);
					break;
			}
			break;
		case 1:
			curTime = My_GetSystemTimeMark();
			abnOccurStep = 2;
			break;
		case 2:
			//异常状态解除后，5s内不更新设备状态
			if(My_GetSystemTimePass(curTime) > 5000)
			{
				abnOccurStep = 0;
			}
			break;
		default:
			break;
	}
}




/*	设备状态 0x87
* @name   Execute_Register87_Function
* bit7：	Buck_State
* bit6：	Port1_Gate_State
* bit5：	Port2_Gate_State
* bit4： 	调压是否完成
* bit3：	处于深睡还是正常睡眠
* bit2：	Port1_DPDM是否短接
* bit1：	Port2_DPDM是否短接
* bit0：
* @param  regData
* @retval None      
*/
void Execute_Register87_Function(void)
{
	u8 regData = Power_Is_Buck_On() << 7 | Power_Is_Gate_On(0x01) << 6 | Power_Is_Gate_On(0x02) << 5 | Power_Is_Adjust_Finished() << 4 | Low_Power_Is_Deep_Sleep() << 3 | Dpdm_Is_Short(0x01) << 2 | Dpdm_Is_Short(0x02) << 1;
	Set_RegAddr_Data(0x87, regData);
}
/*	PD状态 0x88
* @name   Execute_Register88_Function
* bit7：	Port1_CC1是否连接
* bit6：	Port2_CC1是否连接
* bit5：	Port1_CC2是否连接
* bit4： 	Port2_CC2是否连接
* bit3：	是否为5A线缆
* bit2：	PD_Is_Ready
* bit1：	PD_Is_3.0
* bit0：	PD_Is_Dfp
* @param  regData
* @retval None      
*/
void Execute_Register88_Function(void)
{
	u8 regData = Typec_Is_Cc1_Connect(0x00) << 7 | Typec_Is_Cc1_Connect(0x01) << 6 | Typec_Is_Cc2_Connect(0x00) << 5 | Typec_Is_Cc2_Connect(0x01) << 4 | Pd_Is_5a_Cable() << 3 | Pd_Is_Ready() << 2 | Pd_Is_Rev3_0() << 1 | Pd_Is_Dfp() << 0;
	Set_RegAddr_Data(0x88, regData);
}

/*	异常状态- 0x89-0x8C-写0清除
* @name   Execute_Register89_And_Register8C_Function
 * 0x89
 * bit7：DIE OTP
 * bit6：VCONN1 OCP
 * bit5：VCONN2 OCP
 * bit4：CC1 OVP
 * bit3：CC2 OVP
 * bit2：DP OVP
 * bit1：DM OVP 
 * bit0：LPS
 * -------------
 * 0x8A
 * bit7：DM2 Weak Short
 * bit6：DIE Warn
 * bit5：VOUT UVP
 * bit4：VIN OVP
 * bit3：VIN UVLO
 * bit2：VOUT Fast OVP 
 * bit1：VOUT Slow OVP
 * bit0：VOUT SCP
 * -------------
 * 0x8B
 * bit7：
 * bit6：
 * bit5：
 * bit4：
 * bit3：
 * bit2：DP1 Weak Short
 * bit1：DM1 Weak Short,
 * bit0：DP2 Weak Short
 * -------------
 * 0x8C	
 * bit7-0： 
* @param  regData
* @retval None      
*/
void Execute_Register89_And_Register8C_Function(void)
{
	
}


#endif

//0x90-0x9F
#ifdef DATA_REG
/*	协议获取请求电压(PD、QC、SCP等) 0x90-0x91
* @name   Execute_Register90_And_Register91_Function
* bit7-0：	
* @param  regData
* @retval None      
*/
void Execute_Register90_And_Register91_Function(void)
{
	u16 regData = Device_Get_Requesting_Voltage();
	Set_RegAddr_Data(0x90, (regData & (0xFF << 8)) >> 8);
	Set_RegAddr_Data(0x91, (regData & (0xFF << 0)) >> 0);
}


/*	获取输出电压 0x92-0x93
* @name   Execute_Register92_And_Register93_Function
* bit7-0：	
* @param  regData
* @retval None      
*/
void Execute_Register92_And_Register93_Function(void)
{
	u16 regData = Adc_Vout_Get();
	Set_RegAddr_Data(0x92, (regData & (0xFF << 8)) >> 8);
	Set_RegAddr_Data(0x93, (regData & (0xFF << 0)) >> 0);
}


/*	获取Port1输出电流 0x94-0x95
* @name   Execute_Register94_And_Register95_Function
* bit7-0：	
* @param  regData
* @retval None      
*/
void Execute_Register94_And_Register95_Function(void)
{
	u16 regData = Adc_Iout_Get(0x00);
	Set_RegAddr_Data(0x94, (regData & (0xFF << 8)) >> 8);
	Set_RegAddr_Data(0x95, (regData & (0xFF << 0)) >> 0);
}
/*	获取Port2输出电流 0x96-0x97
* @name   Execute_Register96_And_Register97_Function
* bit7-0：	
* @param  regData
* @retval None      
*/
void Execute_Register96_And_Register97_Function(void)
{
	u16 regData = Adc_Iout_Get(0x01);
	Set_RegAddr_Data(0x96, (regData & (0xFF << 8)) >> 8);
	Set_RegAddr_Data(0x97, (regData & (0xFF << 0)) >> 0);
}
/*	获取Vin电压 0x98-0x99
* @name   Execute_Register98_And_Register99_Function
* bit7-0：	
* @param  regData
* @retval None      
*/
void Execute_Register98_And_Register99_Function(void)
{
	u16 regData = Adc_Vin_Get();
	Set_RegAddr_Data(0x98, (regData & (0xFF << 8)) >> 8);
	Set_RegAddr_Data(0x99, (regData & (0xFF << 0)) >> 0);
}

/*	获取Die温度 0x9A-0x9B
* @name   Execute_Register9A_And_Register9B_Function
* bit7-0：	
* @param  regData
* @retval None      
*/
void Execute_Register9A_And_Register9B_Function(void)
{
	u16 regData = Adc_Die_Get();
	Set_RegAddr_Data(0x9A, (regData & (0xFF << 8)) >> 8);
	Set_RegAddr_Data(0x9B, (regData & (0xFF << 0)) >> 0);
}


/*	PD请求电压电流数据 0x9C-0x9F
* 0x9C-0x9D:PD请求电压
* 0x9E-0x9F:PD请求电流
* @name   Execute_Register9C_And_Register9F_Function
* bit7-0：	
* @param  regData
* @retval None      
*/
void Execute_Register9C_And_Register9F_Function(void)
{
	static u8 curStep = 0;
	static u32 curTime = 0;
	u8 tempData = 0;
	u8 tempPort1Protocol = 0;
	u8 tempPort2Protocol = 0;
	switch(curStep)
	{
		case 0x00:
			//判断是否有数据需要清除
			tempData = Get_RegAddr_Data(0x9C) | Get_RegAddr_Data(0x9D) |  Get_RegAddr_Data(0x9E) |  Get_RegAddr_Data(0x9F);  
			if(tempData != 0)
			{
				curStep = 0x01;
				curTime = My_GetSystemTimeMark();
			}
			break;
		case 0x01:
			//1s后清除数据
			if(My_GetSystemTimePass(curTime) > 1000)
			{
				curStep = 0x02;
			}			
			break;
		case 0x02:
			tempPort1Protocol = (Get_RegAddr_Data(0x85) >> 1) & 0x1F;
			tempPort2Protocol = (Get_RegAddr_Data(0x86) >> 1) & 0x1F;
			if(tempPort1Protocol < 15 && tempPort2Protocol < 15)
			{
				//电压
				Set_RegAddr_Data(0x9C, 0x00);
				Set_RegAddr_Data(0x9D, 0x00);	
				//电流
				Set_RegAddr_Data(0x9E, 0x00);
				Set_RegAddr_Data(0x9F, 0x00);			
			}		
			curStep = 0x00;
			break;
	}
}



#endif


//0xA0
#ifdef EXTEND_REG
/*	线缆VDM数据 0xA0-0xA1
* @name   Execute_RegisterA0_And_RegisterA1_Function
* 0xA0
* bit7-4：sop1ProductType
* bit3-0：cableLatency
* 0xA1
* bit7-6：maxVol
* bit5-4：maxCur
* bit3-1：usbHighestSpeed
* bit0：
* @param  regData
* @retval None      
*/
void Execute_RegisterA0_And_RegisterA1_Function(void)
{

}


/*	PD_Request请求数据 0xA2-0xA5
* 0xA2-0xA5:
* 数据需要根据EPR还是SPR模式进行区分
* Bit31-28: Object position
* Bit27:		GiveBack flag
* Bit26:    Capability Mismatch
* Bit25:    USB Communications Capable
* Bit24:    No USB Suspend
* Bit23:    Unchunked Extended Messages Supported
* Bit22:    EPR Mode Capable
* Bit21-20: Reserved
* Bit19-10: Operating current in 10mA units
* Bit09-00: Maximum Operating Current 10mA units
* @name   Execute_RegisterA2_And_RegisterA5_Function
* @param  regData
* @retval None      
*/
void Execute_RegisterA2_And_RegisterA5_Function(void)
{
	static u8 curStep = 0;
	static u32 curTime = 0;
	u8 tempData = 0;
	u8 tempPort1Protocol = 0;
	u8 tempPort2Protocol = 0;
	switch(curStep)
	{
		case 0x00:
			//判断是否有数据需要清除
			tempData = Get_RegAddr_Data(0xA2) | Get_RegAddr_Data(0xA3) |  Get_RegAddr_Data(0xA4) |  Get_RegAddr_Data(0xA5);  
			if(tempData != 0)
			{
				curStep = 0x01;
				curTime = My_GetSystemTimeMark();
			}
			break;
		case 0x01:
			//1s后清除数据
			if(My_GetSystemTimePass(curTime) > 1000)
			{
				curStep = 0x02;
			}
			break;
		case 0x02:
			//设备处于非PD协议时，清空当前寄存器
			tempPort1Protocol = (Get_RegAddr_Data(0x85) >> 1) & 0x1F;
			tempPort2Protocol = (Get_RegAddr_Data(0x86) >> 1) & 0x1F;
			if(tempPort1Protocol < 15 && tempPort2Protocol < 15)
			{
				Set_RegAddr_Data(0xA2, 0x00);
				Set_RegAddr_Data(0xA3, 0x00);	
				Set_RegAddr_Data(0xA4, 0x00);
				Set_RegAddr_Data(0xA5, 0x00);			
			}
			curStep = 0x00;
			break;
	}
}


#endif




#ifdef READ_POLICY
//读策略
void Read_Policy(void)
{
	static u32 curCheckTime;
	//IRQ功能
	Execute_Register82_Write_Function();
	//端口状态、快充状态、协议状态
	Execute_Register85_And_Register86_Function();
	//设备状态
	Execute_Register87_Function();
	//PD状态
	Execute_Register88_Function();
	//异常状态
	Execute_Register89_And_Register8C_Function();
	//设备请求电压
	Execute_Register90_And_Register91_Function();
	//100ms更新一次
	if(My_GetSystemTimePass(curCheckTime) > 100)
	{
		curCheckTime = My_GetSystemTimeMark();
		//获取输出电压
		Execute_Register92_And_Register93_Function();
		//Port1输出电流
		Execute_Register94_And_Register95_Function();
		//Port2输出电流
		Execute_Register96_And_Register97_Function();
		//Vin电压
		Execute_Register98_And_Register99_Function();
		//Die温度
		Execute_Register9A_And_Register9B_Function();
	}
	//PD请求电压电流
	Execute_Register9C_And_Register9F_Function();
	//线缆VDM数据
	Execute_RegisterA0_And_RegisterA1_Function();
	//PD_Request请求数据
	Execute_RegisterA2_And_RegisterA5_Function();
}
#endif
