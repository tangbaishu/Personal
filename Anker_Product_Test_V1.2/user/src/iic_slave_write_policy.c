/****************************************************************************
 * @copyright Copyright(C) 2024-2024 Ismartware Limited. All rights reserved.
 * @file iic_slave_write_policy.c
 * @brief write_policy
 * @author SZ team
 ****************************************************************************/

#include "iic_slave_write_policy.h"

#define WRITE_POLICY		
#define NORMAL_FUNCTION
#define CONFIG_REG			//0x01-0x2F
#define PD_PDO_REG			//0x30-0x4F
#define UFCS_PDO_REG		//0x50-0x64
#define ABN_CONFIG_REG	//0x65-0x6F
#define API_REG					//0x70-0x7E
#define CUSTOM_FUN_REG	//0x7F-0x84

//策略全局变量
extern policy_data_t policyGlobalData;


#ifdef CONFIG_REG	//0x01-0x2F

/*	端口配置
* @name   Execute_Register01_Function
* bit7-5：端口模式 (0：单A、1：单C、2：双A、3：A+C、4：)
* bit4-3：port1限流点 (0：1.5A、1：2.0A、2：2.4A、3：3.0A)
* bit2-1：port2限流点 (0：1.5A、1：2.0A、2：2.4A、3：3.0A)
* bit0：	使能主程序运行
* @param  regData
* @retval None      
*/
void Execute_Register01_Function(uint8_t regData)
{
	config_data_t* config 		= 	Config_Get();
	config->portMode					= 	(regData & (0x07 << 5)) >> 5;
	config->p1CurrentLimit		= 	(regData & (0x03 << 3)) >> 3;
	config->p2CurrentLimit		= 	(regData & (0x03 << 1)) >> 1;	
	policyGlobalData.startRun = 	(regData & (0x01 << 0)) >> 0;	
}

/*	协议使能1
* @name   Execute_Register02_Function
* bit7：  PD 协议(0：不使能、1：使能)
* bit6：  UFCS 协议(0：不使能、1：使能)
* bit5：  QC2 协议(0：不使能、1：使能)
* bit4：  QC3 协议(0：不使能、1：使能)
* bit3：  QC3+ 协议(0：不使能、1：使能)
* bit2：  FCP 协议(0：不使能、1：使能)
* bit1：  H_SCP 协议(0：不使能、1：使能)
* bit0：  L_SCP 协议(0：不使能、1：使能)
* @param  regData
* @retval None      
*/
void Execute_Register02_Function(uint8_t regData)
{
	config_data_t* config 		= 	Config_Get();
	config->isPdEnabled 		  = 	(regData & (0x01 << 7)) >> 7;
	config->isUfcsEnabled 		= 	(regData & (0x01 << 6)) >> 6;
	config->isQc2p0Enabled 		= 	(regData & (0x01 << 5)) >> 5;
	config->isQc3p0Enabled 		= 	(regData & (0x01 << 4)) >> 4;
	config->isQc3plusEnabled	= 	(regData & (0x01 << 3)) >> 3;
	config->isFcpEnabled  		= 	(regData & (0x01 << 2)) >> 2;
	config->isHvScpEnabled 		= 	(regData & (0x01 << 1)) >> 1;
	config->isLvScpEnabled 		= 	(regData & (0x01 << 0)) >> 0;	
}

/*	协议使能2
* @name   Execute_Register03_Function
* bit7：  vooc 1.0 协议(0：不使能、1：使能)
* bit6：  vooc 4.0 协议(0：不使能、1：使能)
* bit5：  suppervooc 2.0 协议(0：不使能、1：使能)
* bit4：  SFCP 协议(0：不使能、1：使能)
* bit3：  TFCP 协议(0：不使能、1：使能)
* bit2：  AFC 协议(0：不使能、1：使能)
* bit1：  PE 协议(0：不使能、1：使能)
* bit0：	
* @param  regData
* @retval None      
*/
void Execute_Register03_Function(uint8_t regData)
{
	config_data_t* config 				= 	Config_Get();
	config->isVooc1p0Enabled 			= 	(regData & (0x01 << 7)) >> 7;
	config->isVooc4p0Enabled 			= 	(regData & (0x01 << 6)) >> 6;
	config->isSuperVooc2p0Enabled = 	(regData & (0x01 << 5)) >> 5;
	config->isSfcpEnabled 				= 	(regData & (0x01 << 4)) >> 4;
	config->isTfcpEnabled 				= 	(regData & (0x01 << 3)) >> 3;
	config->isAfcEnabled   				= 	(regData & (0x01 << 2)) >> 2;
	config->isPeEnabled 					= 	(regData & (0x01 << 1)) >> 1;
	//reserved
}


/*	PD最大功率
* @name   Execute_Register04_Function
* bit7-0：PD功率，单位：W
* @param  regData
* @retval None      
*/
void Execute_Register04_Function(uint8_t regData)
{
	config_data_t* config			= 	Config_Get();
	config->pdMaxPower				= 	regData;
}

/*	pd 版本
* @name   Execute_Register05_Function
* bit7-0：pd 版本
* @param  regData
* @retval None      
*/
void Execute_Register05_Function(uint8_t regData)
{
	config_data_t* config			= 	Config_Get();
	config->pdVersionMinor		= 	regData;
}


/*	pd vdm 版本
* @name   Execute_Register06_Function
* bit7-0：pd vdm 版本
* pdVdmVer = 0 -> vdm 2.0版本，pdVdmVer = 1 -> vdm 2.1版本
* @param  regData
* @retval None      
*/
void Execute_Register06_Function(uint8_t regData)
{
	config_data_t* config			= 	Config_Get();
	config->pdVdmVer					= 	regData;
}

/*	PPS保护阈值电压
* @name   Execute_Register07_Function
* bit7-0：pps shutdown  50mv/step
* @param  regData
* @retval None      
*/
void Execute_Register07_Function(uint8_t regData)
{
	config_data_t* config			= 	Config_Get();
	config->vPpsShutDownInMv	= 	regData * 50;
}

/*	PD协议相关操作
* @name   Execute_Register08_Function
* bit7：  PD Gating Vooc(0：不使能、1：使能)
* bit6：  PD Gating Scp(0：不使能、1：使能)
* bit5：  支持vconn swap(0：不使能、1：使能)
* bit4：  响应discover id(0：不使能、1：使能)
* bit3：  cc 切换到 cv 时是否发送警报(0：不使能、1：使能)
* bit2：  读取线缆后关闭vconn(0：不使能、1：使能)
* bit1：  非法pps请求发送hardreset(0：不使能、1：使能)
* bit0：  广播5v2a(0：不使能、1：使能)
* @param  regData
* @retval None      
*/
void Execute_Register08_Function(uint8_t regData)
{
	config_data_t* config 							= 	Config_Get();
	config->isPdGatingVoocEnabled 			= 	(regData & (0x01 << 7)) >> 7;
	config->isPdGatingScpEnabled 				= 	(regData & (0x01 << 6)) >> 6;
	config->isVconnSwapEnabled 					= 	(regData & (0x01 << 5)) >> 5;
	config->isResponseDiscoveryId				= 	(regData & (0x01 << 4)) >> 4;	
	config->isPdAlertEnabled						= 	(regData & (0x01 << 3)) >> 3;
	config->isVconnLowPowerModeEnabled	= 	(regData & (0x01 << 2)) >> 2;
	config->isInvalidPpsEnabled 				= 	(regData & (0x01 << 1)) >> 1;
	config->isPdo5v2aEnabled						= 	(regData & (0x01 << 0)) >> 0;		
}


/*	xid 31bit~24bit
* @name   Execute_Register09_Function
* bit7-0：xid 31bit~24bit
* @param  regData
* @retval None      
*/
void Execute_Register09_Function(uint8_t regData)
{
	config_data_t* config			= 	Config_Get();
	config->xid &= ~(0xFF << 24);
	config->xid	|= (regData << 24);
}

/*	xid 23bit~16bit
* @name   Execute_Register0A_Function
* bit7-0：xid 23bit~16bit
* @param  regData
* @retval None      
*/
void Execute_Register0A_Function(uint8_t regData)
{
	config_data_t* config			= 	Config_Get();
	config->xid &= ~(0xFF << 16);
	config->xid	|= (regData << 16);
}

/*	xid 15bit~8bit
* @name   Execute_Register0B_Function
* bit7-0：xid 15bit~8bit
* @param  regData
* @retval None      
*/
void Execute_Register0B_Function(uint8_t regData)
{
	config_data_t* config			= 	Config_Get();
	config->xid &= ~(0xFF << 8);
	config->xid	|= (regData << 8);
}

/*	xid 7bit~0bit
* @name   Execute_Register0C_Function
* bit7-0：xid 7bit~0bit
* @param  regData
* @retval None      
*/
void Execute_Register0C_Function(uint8_t regData)
{
	config_data_t* config			= 	Config_Get();
	config->xid &= ~(0xFF << 0);
	config->xid	|= (regData << 0);
}

/*	vid 15bit~8bit
* @name   Execute_Register0D_Function
* bit7-0：vid 高八位
* @param  regData
* @retval None      
*/
void Execute_Register0D_Function(uint8_t regData)
{
	config_data_t* config			= 	Config_Get();
	config->vid &= ~(0xFF << 8);
	config->vid	|= (regData << 8);
}

/*	vid 7bit~0bit
* @name   Execute_Register0E_Function
* bit7-0：vid 低八位
* @param  regData
* @retval None      
*/
void Execute_Register0E_Function(uint8_t regData)
{
	config_data_t* config			= 	Config_Get();
	config->vid &= ~(0xFF << 0);
	config->vid	|= (regData << 0);
}

/*	pid 15bit~8bit
* @name   Execute_Register0F_Function
* bit7-0：pid 高八位
* @param  regData
* @retval None      
*/
void Execute_Register0F_Function(uint8_t regData)
{
	config_data_t* config			= 	Config_Get();
	config->pid &= ~(0xFF << 8);
	config->pid	|= (regData << 8);
}

/*	pid 7bit~0bit
* @name   Execute_Register10_Function
* bit7-0：pid 低八位
* @param  regData
* @retval None      
*/
void Execute_Register10_Function(uint8_t regData)
{
	config_data_t* config			= 	Config_Get();
	config->pid &= ~(0xFF << 0);
	config->pid	|= (regData << 0);
}


/*	bcdDevice 15bit~8bit
* @name   Execute_Register11_Function
* bit7-0：bcdDevice 高八位
* @param  regData
* @retval None      
*/
void Execute_Register11_Function(uint8_t regData)
{
	config_data_t* config			= 	Config_Get();
	config->bcdDevice &= ~(0xFF << 8);
	config->bcdDevice	|= (regData << 8);
}

/*	bcdDevice 7bit~0bit
* @name   Execute_Register12_Function
* bit7-0：bcdDevice 低八位
* @param  regData
* @retval None      
*/
void Execute_Register12_Function(uint8_t regData)
{
	config_data_t* config			= 	Config_Get();
	config->bcdDevice &= ~(0xFF << 0);
	config->bcdDevice	|= (regData << 0);
}


/*	ufcs设备温度
* @name   Execute_Register13_Function
* bit7-0：ufcs DevTemp	单位:度
* @param  regData
* @retval None      
*/
void Execute_Register13_Function(uint8_t regData)
{
	config_data_t* config			= 	Config_Get();
	config->ufcsDevTemp				= 	regData;
}


/*	ufcs usb端口温度
* @name   Execute_Register14_Function
* bit7-0：ufcs UsbTemp	单位:度
* @param  regData
* @retval None      
*/
void Execute_Register14_Function(uint8_t regData)
{
	config_data_t* config			= 	Config_Get();
	config->ufcsUsbTemp				= 	regData;
}


/*	协议参数配置1
* @name   Execute_Register15_Function
* bit7-6：QC2.0最高电压(0:5V、1:9V、2:12V、3:20V)
* bit5-4：QC3.0最高电压(0:5V、1:9V、2:12V、3:20V)    
* bit3-2：QC3.0最大功率(0:18W、1:27W、2:45W、3:60W)   
* bit1-0：PE 最高电压(0:5V、1:9V、2:12V、3:20V)
* @param  regData
* @retval None      
*/
void Execute_Register15_Function(uint8_t regData)	
{
	config_data_t* config 			= 	Config_Get();
	config->qc2p0MaxVoltage 		= 	(regData & (0x03 << 6)) >> 6;
	config->qc3p0MaxVoltage			= 	(regData & (0x03 << 4)) >> 4;
	config->qc3MaxPower					= 	(regData & (0x03 << 2)) >> 2;
	config->peMaxVoltage 				= 	(regData & (0x03 << 0)) >> 0;
}

/*	协议参数配置2 
* @name   Execute_Register16_Function
* bit7：  AFC最高电压(0:9V、1:12V)
* bit6：  SFCP最高电压(0:9V、1:12V)
* bit5：  FCP最高电压(0:9V、1:12V)
* bit4：  
* bit3：
* bit2：
* bit1：
* bit0：
* @param  regData
* @retval None      
*/
void Execute_Register16_Function(uint8_t regData)
{
	config_data_t* config 		= 	Config_Get();
	config->afcMaxVoltage			=		(regData & (0x01 << 7)) >> 7;
	config->sfcpMaxVoltage		=   (regData & (0x01 << 6)) >> 6;
	config->fcpMaxVoltage			=   (regData & (0x01 << 5)) >> 5;
}



/*	SCP协议最大电流-5A
* @name   Execute_Register17_Function
* bit7-0：SCP协议最大电流(100mA/Step)10-63
* @param  regData
* @retval None      
*/
void Execute_Register17_Function(uint8_t regData)
{
	config_data_t* config 	= 	Config_Get();
	config->scpMaxCurrent		= 	regData;
}

/*	SCP协议最大电压-12V
* @name   Execute_Register18_Function
* bit7-0：SCP协议最大电压(100mV/Step)100-630
* @param  regData
* @retval None      
*/
void Execute_Register18_Function(uint8_t regData)
{
	config_data_t* config 	= 	Config_Get();
	config->scpMaxVoltage		= 	regData;
}


/*	SCP协议最大功率-23
* @name   Execute_Register19_Function
* bit7-0：SCP协议最大功率(W)1-127
* @param  regData
* @retval None      
*/
void Execute_Register19_Function(uint8_t regData)
{
	config_data_t* config 	= 	Config_Get();
	config->scpMaxPower			= 	regData;
}


/*	SCP适配器类型
* @name   Execute_Register1A_Function
* bit7-0：SCP适配器类型
* @param  regData
* @retval None      
*/
void Execute_Register1A_Function(uint8_t regData)
{
	config_data_t* config 	= 	Config_Get();
	config->scpAdapterType	= 	regData;
}


/*	DPDM协议最大功率
* @name   Execute_Register1B_Function
* bit7-0：DPDM协议最大功率，afc/fcp/sfcp/pe/qc2.0 power, 单位 : W   
* @param  regData
* @retval None      
*/
void Execute_Register1B_Function(uint8_t regData)
{
	config_data_t* config 	= 	Config_Get();
	config->dpdmFixPower		= 	regData;
}


/*	VOOC1.0 VOOC4.0 Max Power
* @name   Execute_Register1C_Function
* bit7-0：Vooc最大功率，单位：W
* @param  regData
* @retval None      
*/
void Execute_Register1C_Function(uint8_t regData)
{
	config_data_t* config 	= 	Config_Get();
	config->voocMaxPower		= 	regData;
}


/*	superVooc最大功率
* @name   Execute_Register1D_Function
* bit7-0：superVooc最大功率,单位：W
* @param  regData
* @retval None      
*/
void Execute_Register1D_Function(uint8_t regData)
{
	config_data_t* config 			= 	Config_Get();
	config->superVoocMaxPower		= 	regData;
}


/*	vooc正品线缆Type150阻抗
* @name   Execute_Register1E_Function
* bit7-0：vooc正品线缆Type150阻抗
* @param  regData
* @retval None      
*/
void Execute_Register1E_Function(uint8_t regData)
{
	config_data_t* config 												= 	Config_Get();
	config->voocPathImpedanceGenuineCableTyp150		= 	regData;
}


/*	vooc正品线缆Type180阻抗
* @name   Execute_Register1F_Function
* bit7-0：vooc正品线缆Type180阻抗
* @param  regData
* @retval None      
*/
void Execute_Register1F_Function(uint8_t regData)
{
	config_data_t* config 												= 	Config_Get();
	config->voocPathImpedanceGenuineCableTyp180		= 	regData;
}

/*	vooc正品线缆Type250阻抗
* @name   Execute_Register20_Function
* bit7-0：vooc正品线缆Type250阻抗
* @param  regData
* @retval None      
*/
void Execute_Register20_Function(uint8_t regData)
{
	config_data_t* config 												= 	Config_Get();
	config->voocPathImpedanceGenuineCableTyp250		= 	regData;
}


/*	vooc普通线缆Type150阻抗
* @name   Execute_Register21_Function
* bit7-0：vooc普通线缆Type150阻抗
* @param  regData
* @retval None      
*/
void Execute_Register21_Function(uint8_t regData)
{
	config_data_t* config 												= 	Config_Get();
	config->voocPathImpedanceCommonCableTyp150		= 	regData;
}

/*	vooc普通线缆Type180阻抗
* @name   Execute_Register22_Function
* @brief  参数配置15
* bit7-0：vooc普通线缆Type180阻抗
* @param  regData
* @retval None      
*/
void Execute_Register22_Function(uint8_t regData)
{
	config_data_t* config 												= 	Config_Get();
	config->voocPathImpedanceCommonCableTyp180		= 	regData;
}


/*	vooc普通线缆Type250阻抗
* @name   Execute_Register23_Function
* bit7-0：vooc普通线缆Type250阻抗
* @param  regData
* @retval None      
*/
void Execute_Register23_Function(uint8_t regData)
{
	config_data_t* config 												= 	Config_Get();
	config->voocPathImpedanceCommonCableTyp250		= 	regData;
}


/*	TFCP最大功率
* @name   Execute_Register24_Function
* bit7-0：TFCP协议最大功率，单位：W
* @param  regData
* @retval None      
*/
void Execute_Register24_Function(uint8_t regData)
{
	config_data_t* config 	= 	Config_Get();
	config->tfcpMaxPower		= 	regData;
}



/*	type-c连接时间
* @name   Execute_Register25_Function
* bit7-0：typecConnectTime，1ms/step
* typec的连接时间，这次只是typec的时间，真正的vbus_on时间将>这个
* @param  regData
* @retval None      
*/
void Execute_Register25_Function(uint8_t regData)
{
	config_data_t* config			= 	Config_Get();
	config->typecConnectTime	= 	regData;
}

/*	设备参数配置1
* @name   Execute_Register26_Function
* bit7-6：Typec CC 配置(0:RP_RESERVED、1:RP_3A、2:RP_1_5A、3:RP_DEFAULT_USB)
* bit5：  Typec Vconn使能(0:不使能、1:使能)
* bit4：  Typec 认证模式(是否开启typec认证模式，区别在于断开认证的时间，0:断开时间为200ms, 1:断开时间为12ms)
* bit3：  C口空载(0:不使能、1:使能)
* bit2：  lps使能(0:不使能、1:使能)
* bit1：  
* bit0：	
* @param  regData
* @retval None      
*/
void Execute_Register26_Function(uint8_t regData)
{
	config_data_t* config			= 	Config_Get();
	config->typecCcTerm 					= 	(regData & (0x03 << 6)) >> 6;
	config->isTypecVconnEnabled 	= 	(regData & (0x01 << 5)) >> 5;
	config->isTypecCertEnabled		= 	(regData & (0x01 << 4)) >> 4;	
	config->isPortCNoLoad					= 	(regData & (0x01 << 3)) >> 3;
	config->isAbnLpsEnabled				= 	(regData & (0x01 << 2)) >> 2;
	
}

/*	设备参数配置2
* @name   Execute_Register27_Function
* bit7：  线补使能(0:不使能、1:使能)
* bit6：  线补值(0:100 mohm、1:65 mohm)
* bit5-4：固定挡位电压偏置(0:0mV、1:100mV、2:150mV、3:200mV) 
* bit3-2：固定挡位电流偏置(0:300mA、1:350mA、2:400mA、3:450mA)
* bit1：
* bit0：
* @param  regData
* @retval None      
*/
void Execute_Register27_Function(uint8_t regData)
{
	config_data_t* config 					= 	Config_Get();
	config->isCableCompensaved			= 	(regData & (0x01 << 7)) >> 7;
	config->cableCompensationValue 	= 	(regData & (0x01 << 6)) >> 6;
	config->fixVoltageOffset				= 	(regData & (0x03 << 4)) >> 4;
	config->fixCurrentOffset 				= 	(regData & (0x03 << 2)) >> 2;	
	Config_Apply();
	policyGlobalData.notAllowAdjustVolCur = true;
}

/*	设备参数配置3
* @name   Execute_Register28_Function
* bit7：  Apple 模式(0:不使能、1:使能)
* bit6：  Samsung 模式(0:不使能、1:使能)
* bit5：  UFCS 认证(0:不开启入住、1：开启认证，dp<1.05v，apple->samsung)
* bit4：  PD 认证(0:不使能、1:开启pd认证模式，DPDM短接，DPDM协议不会运行)
* bit3：  PD Apple 认证(0:不使能、1:使能)
* bit2：  读emark使能(0:不使能、1:使能)
* bit1：  直接广播140W(0:不发送、1:发送140w EPR SRC)
* bit0：  vooc 线缆严格模式(0:不使能、1:使能)
* @param  regData
* @retval None      
*/
void Execute_Register28_Function(uint8_t regData)
{
	config_data_t* config 				= 	Config_Get();
	config->isAppleEnabled 				= 	(regData & (0x01 << 7)) >> 7;
	config->isSamsungEnabled 			= 	(regData & (0x01 << 6)) >> 6;
	config->isUfcsCertEnabled 		= 	(regData & (0x01 << 5)) >> 5;
	config->isPdCertEnabled				= 	(regData & (0x01 << 4)) >> 4;	
	config->isPdCertWithApple			= 	(regData & (0x01 << 3)) >> 3;
	config->isEmarkEnabled  			= 	(regData & (0x01 << 2)) >> 2;
	config->isIgnoreEprCable 			= 	(regData & (0x01 << 1)) >> 1;
	config->isVoocCableStricMode	= 	(regData & (0x01 << 0)) >> 0;		
}

#endif



#ifdef PD_PDO_REG	//0x30-0x4F
/*	PD附加信息
* @name   Execute_Register30_Function
* bit7~bit3：fix5v_flags
* bit7：PDO_FIXED_DUAL_POWER_ROLE
* bit6：PDO_FIXED_UNCONSTRAINED_POWER
* bit5：PDO_FIXED_COMM_CAP
* bit4：PDO_FIXED_DUAL_DATA_ROLE
* bit3：PDO_FIXED_EPR_MODE
* bit2：
* bit1：
* bit0：
* @param  regData
* @retval None      
*/

void Execute_Register30_Function(uint8_t regData)
{

}


/*	PD参数配置1
* @name   Execute_Register31_Function
* bit7：9V使能位，0：不使能、1：使能
* bit6：12V使能位，0：不使能、1：使能
* bit5：15V使能位，0：不使能、1：使能
* bit4：20V使能位，0：不使能、1：使能
* bit3：PPS0使能位，0：不使能、1：使能
* bit2：PPS1使能位，0：不使能、1：使能
* bit1：PPS2使能位，0：不使能、1：使能
* bit0：28V使能位，0：不使能、1：使能
* @param  regData
* @retval None      
*/
void Execute_Register31_Function(uint8_t regData)
{

}


/*	PD参数配置2
* @name   Execute_Register32_Function
* 0:no peak current、1:peak current mode1、2:current mode2、3:peak current mode3
* bit7：AVS0使能位
* bit6：PPS1恒功率使能位
* bit5：PPS2恒功率使能位
* bit4：PPS0恒功率使能位
* bit3~bit2：fix5v peakCurrent
* bit1~bit0：fix9v peakCurrent
* @param  regData
* @retval None      
*/

void Execute_Register32_Function(uint8_t regData)
{
	
}

/*	PD参数配置3
* @name   Execute_Register33_Function
* 0:no peak current、1:peak current mode1、2:current mode2、3:peak current mode3
* bit7~bit6：fix12v peakCurrent
* bit5~bit4：fix15v peakCurrent
* bit3~bit2：fix20v peakCurrent
* bit1~bit0：fix28v peakCurrent
* @param  regData
* @retval None      
*/
void Execute_Register33_Function(uint8_t regData)
{
	
}


/*	fix5V电流
* @name   Execute_Register34_Function
* bit7~bit0：fix5V电流，50mA/Step
* @param  regData
* @retval None      
*/
void Execute_Register34_Function(uint8_t regData)
{
	
}

/*	fix9V电压
* @name   Execute_Register35_Function
* bit7~bit0：fix9V电压，100mV/Step
* @param  regData
* @retval None      
*/
void Execute_Register35_Function(uint8_t regData)
{
	
}

/*	fix9V电流
* @name   Execute_Register36_Function
* bit7~bit0：fix9V电流，50mA/Step
* @param  regData
* @retval None      
*/
void Execute_Register36_Function(uint8_t regData)
{
	
}

/*	fix12V电压
* @name   Execute_Register37_Function
* bit7~bit0：fix12V电压，100mV/Step
* @param  regData
* @retval None      
*/
void Execute_Register37_Function(uint8_t regData)
{
	
}

/*	fix12V电流
* @name   Execute_Register38_Function
* bit7~bit0：fix12V电流，50mA/Step
* @param  regData
* @retval None      
*/
void Execute_Register38_Function(uint8_t regData)
{
	
}

/*	fix15V电压
* @name   Execute_Register39_Function
* bit7~bit0：fix15V电压，100mV/Step
* @param  regData
* @retval None      
*/
void Execute_Register39_Function(uint8_t regData)
{
	
}

/*	fix15V电流
* @name   Execute_Register3A_Function
* bit7~bit0：fix15V电流，50mA/Step
* @param  regData
* @retval None      
*/
void Execute_Register3A_Function(uint8_t regData)
{
	
}

/*	fix20V电压
* @name   Execute_Register3B_Function
* bit7~bit0：fix20V电压，100mV/Step
* @param  regData
* @retval None      
*/
void Execute_Register3B_Function(uint8_t regData)
{
	
}

/*	fix20V电流
* @name   Execute_Register3C_Function
* bit7~bit0：fix20V电流，50mA/Step
* @param  regData
* @retval None      
*/
void Execute_Register3C_Function(uint8_t regData)
{
	
}


/*	PPS0最小电压
* @name   Execute_Register3D_Function
* bit7~bit0：PPS0最小电压，100mV/Step
* @param  regData
* @retval None      
*/
void Execute_Register3D_Function(uint8_t regData)
{
	
}

/*	PPS0最大电压
* @name   Execute_Register3E_Function
* bit7~bit0：PPS0最大电压，100mV/Step
* @param  regData
* @retval None      
*/
void Execute_Register3E_Function(uint8_t regData)
{
	
}

/*	PPS0最大电流
* @name   Execute_Register3F_Function
* bit7~bit0：PPS0最大电流，50mA/Step
* @param  regData
* @retval None      
*/
void Execute_Register3F_Function(uint8_t regData)
{
	
}


/*	PPS1最小电压
* @name   Execute_Register40_Function
* bit7~bit0：PPS1最小电压，100mV/Step
* @param  regData
* @retval None      
*/
void Execute_Register40_Function(uint8_t regData)
{
	
}

/*	PPS1最大电压
* @name   Execute_Register41_Function
* bit7~bit0：PPS1最大电压，100mV/Step
* @param  regData
* @retval None      
*/
void Execute_Register41_Function(uint8_t regData)
{
	
}

/*	PPS1最大电流
* @name   Execute_Register42_Function
* bit7~bit0：PPS1最大电流，50mA/Step
* @param  regData
* @retval None      
*/
void Execute_Register42_Function(uint8_t regData)
{
	
}

/*	PPS2最小电压
* @name   Execute_Register43_Function
* bit7~bit0：PPS2最小电压，100mV/Step
* @param  regData
* @retval None      
*/
void Execute_Register43_Function(uint8_t regData)
{
	
}

/*	PPS2最大电压
* @name   Execute_Register44_Function
* bit7~bit0：PPS2最大电压，100mV/Step
* @param  regData
* @retval None      
*/
void Execute_Register44_Function(uint8_t regData)
{
	
}

/*	PPS2最大电流
* @name   Execute_Register45_Function
* bit7~bit0：PPS2最大电流，50mA/Step
* @param  regData
* @retval None      
*/
void Execute_Register45_Function(uint8_t regData)
{
	
}

/*	28V档电压
* @name   Execute_Register46_Function
* bit7~bit0：28V档电压，200mV/Step
* @param  regData
* @retval None      
*/
void Execute_Register46_Function(uint8_t regData)
{
	
}

/*	28V档电流
* @name   Execute_Register47_Function
* bit7~bit0：28V档电流，50mA/Step
* @param  regData
* @retval None      
*/
void Execute_Register47_Function(uint8_t regData)
{
	
}

/*	AVS0最小电压
* @name   Execute_Register48_Function
* bit7~bit0：AVS0最小电压，200mV/Step
* @param  regData
* @retval None      
*/
void Execute_Register48_Function(uint8_t regData)
{
	
}

/*	AVS0最大电压
* @name   Execute_Register49_Function
* bit7~bit0：AVS0最大电压，200mA/Step
* @param  regData
* @retval None      
*/
void Execute_Register49_Function(uint8_t regData)
{
	
}



#endif


#ifdef UFCS_PDO_REG	//0x50-0x64
/*	UFCS挡位使能
* @name   Execute_Register50_Function
* bit7：ID1使能位
* bit6：ID2使能位
* bit5：ID3使能位
* bit4：
* bit3：
* bit2：
* bit1：
* bit0：
* @param  regData
* @retval None      
*/

void Execute_Register50_Function(uint8_t regData)
{
	
}

/*	UFCS_CAP_HEAD设置0
* @name   Execute_Register51_Function
* bit7~bit5：UFCS_ID1电流步进设置(0:10mA,1:20mA,2:30mA,3:40mA,4:50mA,5:保留)
* bit4：UFCS_ID1电压步进设置(0:10mV,1:20mV)
* bit3~bit1：UFCS_ID2电流步进设置(0:10mA,1:20mA,2:30mA,3:40mA,4:50mA,5:保留)
* bit0：UFCS_ID2电压步进设置(0:10mV,1:20mV)
* @param  regData
* @retval None      
*/
void Execute_Register51_Function(uint8_t regData)
{
	
}

/*  UFCS_CAP_HEAD设置1
* @name   Execute_Register52_Function
* bit7~bit5：UFCS_ID3电流步进设置(0:10mA,1:20mA,2:30mA,3:40mA,4:50mA,5:保留)
* bit4：UFCS_ID3电压步进设置(0:10mV,1:20mV)
* @param  regData
* @retval None      
*/
void Execute_Register52_Function(uint8_t regData)
{
	
}


/*  UFCS_ID1最大电压
* @name   Execute_Register53_Function
* bit7~bit0：UFCS_ID1最大电压，100mV/Step
* @param  regData
* @retval None      
*/
void Execute_Register53_Function(uint8_t regData)
{
	
}

/*  UFCS_ID1最小电压
* @name   Execute_Register54_Function
* bit7~bit0：UFCS_ID1最小电压，100mV/Step
* @param  regData
* @retval None      
*/
void Execute_Register54_Function(uint8_t regData)
{
	
}

/*  UFCS_ID1最大电流
* @name   Execute_Register55_Function
* bit7~bit0：UFCS_ID1最大电流，50mA/Step
* @param  regData
* @retval None      
*/
void Execute_Register55_Function(uint8_t regData)
{
	
}

/*  UFCS_ID1最小电流
* @name   Execute_Register56_Function
* bit7~bit0：UFCS_ID1最小电流，50mA/Step
* @param  regData
* @retval None      
*/
void Execute_Register56_Function(uint8_t regData)
{
	
}

/*  UFCS_ID2最大电压
* @name   Execute_Register57_Function
* bit7~bit0：UFCS_ID2最大电压，100mV/Step
* @param  regData
* @retval None      
*/
void Execute_Register57_Function(uint8_t regData)
{
	
}

/*  UFCS_ID2最小电压
* @name   Execute_Register58_Function
* bit7~bit0：UFCS_ID2最小电压，100mV/Step
* @param  regData
* @retval None      
*/
void Execute_Register58_Function(uint8_t regData)
{
	
}

/*  UFCS_ID2最大电流
* @name   Execute_Register59_Function
* bit7~bit0：UFCS_ID2最大电流，50mA/Step
* @param  regData
* @retval None      
*/
void Execute_Register59_Function(uint8_t regData)
{
	
}

/*  UFCS_ID2最小电流
* @name   Execute_Register5A_Function
* bit7~bit0：UFCS_ID2最小电流，50mA/Step
* @param  regData
* @retval None      
*/
void Execute_Register5A_Function(uint8_t regData)
{
	
}

/*  UFCS_ID3最大电压
* @name   Execute_Register5B_Function
* bit7~bit0：UFCS_ID3最大电压，100mV/Step
* @param  regData
* @retval None      
*/
void Execute_Register5B_Function(uint8_t regData)
{
	
}

/*  UFCS_ID3最小电压
* @name   Execute_Register5C_Function
* bit7~bit0：UFCS_ID3最小电压，100mV/Step
* @param  regData
* @retval None      
*/
void Execute_Register5C_Function(uint8_t regData)
{
	
}

/*  UFCS_ID3最大电流
* @name   Execute_Register5D_Function
* bit7~bit0：UFCS_ID3最大电流，50mA/Step
* @param  regData
* @retval None      
*/
void Execute_Register5D_Function(uint8_t regData)
{
	
}

/*  UFCS_ID3最小电流
* @name   Execute_Register5E_Function
* bit7~bit0：UFCS_ID3最小电流，50mA/Step
* @param  regData
* @retval None      
*/
void Execute_Register5E_Function(uint8_t regData)
{
	
}


#endif

#ifdef ABN_CONFIG_REG	//0x65-0x6F
/*	异常保护参数设置0
* @name   Execute_Register65_Function
* bit7-5：lps adc电流比较阈值(0:0.2A,1:0.6A,2:1.0A,3:1.4A,4:1.8A,5:2.2A,6:2.6A,7:3.0A)
* bit4-2：lps hmos电流比较阈值(0:0.60A / 0.98A,1:1.16A / 1.65A,2:1.72A / 2.32A,3:2.28A / 2.99A
						      4:2.84A / 3.66A,5:3.40A / 4.33A,6:3.96A / 5.00A,7:4.52A / 5.67A)
* bit1-0：
* @param  regData
* @retval None      
*/
void Execute_Register65_Function(uint8_t regData)
{
	abn_config_t abnCfg = {0};
	Abn_Get_Config(&abnCfg);
	abnCfg.bitFields.lpsAdcCurrTh = (regData & (0x07 << 5)) >> 5;
	abnCfg.bitFields.lpsHmosCurrTh = (regData & (0x07 << 2)) >> 2;
	Abn_Set_Config(&abnCfg);
}

/*	异常保护参数设置1
* @name   Execute_Register66_Function
* bit7：vout_scp电流比较阈值(0:3.0V,1:2.7V)
* bit6：vout_uvp电流比较阈值(0:70%,1:3.1V)
* bit5~bit3：vin_uvlp电流比较阈值(0:26/27V,1:19/20V,2:18/19V,3:14/15V,4:13/14V,5:10/11V,6:7/8V,7:5/6V)
* bit2~bit0：vin_ovp电流比较阈值(0:42V/40V,1:40V/38V,2:38V/36V,3:36V/34V,4:34V/32V,5:32V/30V,6:24V/22V,7:26V/24V)
* @param  regData
* @retval None      
*/
void Execute_Register66_Function(uint8_t regData)
{
	abn_config_t abnCfg = {0};
	Abn_Get_Config(&abnCfg);
	abnCfg.bitFields.voutScpTh = (regData & (0x01 << 7)) >> 7;
	abnCfg.bitFields.voutUvpTh = (regData & (0x01 << 6)) >> 6;	
	abnCfg.bitFields.vinUvlpTh = (regData & (0x07 << 3)) >> 3;
	abnCfg.bitFields.vinOvpTh = (regData & (0x07 << 0)) >> 0;	
	Abn_Set_Config(&abnCfg);
}

/*	异常保护参数设置2
* @name   Execute_Register67_Function
* bit7：vin ovp debounce time(0:30us,1:4ms)
* bit6：vout sovp debounce time(0:8ms,1:4ms)
* bit5：vout fovp debounce time(0:50us,1:30us)
* bit4：vin uvlp debounce time(0:4ms,1:128us)
* @param  regData
* @retval None      
*/
void Execute_Register67_Function(uint8_t regData)
{
	abn_config_t abnCfg = {0};
	Abn_Get_Config(&abnCfg);
	abnCfg.bitFields.vinOvpDbcTime = (regData & (0x01 << 7)) >> 7;
	abnCfg.bitFields.voutSovpDbcTime = (regData & (0x01 << 6)) >> 6;	
	abnCfg.bitFields.voutFovpDbcTime = (regData & (0x01 << 5)) >> 5;
	abnCfg.bitFields.vinUvloDbcTime = (regData & (0x01 << 4)) >> 4;	
	Abn_Set_Config(&abnCfg);
}


/*	异常保护参数设置3
* @name   Execute_Register68_Function
* bit7~bit5：DPDM弱短阈值(0:3.6V,1:3.7V,2:3.8V,3:3.9V,4:4.0V,5:4.1V,6:4.2V,7:4.3V)
* bit4~bit3：Ovp关闭阈值(0:130 degree,1:140 degree,2:150 degree,3:160 degree)
* bit2~bit1：Ovp报警阈值(0:110 degree,1:120 degree,2:130 degree,3:140 degree)
* @param  regData
* @retval None      
*/
void Execute_Register68_Function(uint8_t regData)
{
	abn_config_t abnCfg = {0};
	Abn_Get_Config(&abnCfg);
	abnCfg.bitFields.dpdmShortTh = (regData & (0x07 << 5)) >> 5;
	abnCfg.bitFields.ovpShutdownTh = (regData & (0x03 << 3)) >> 3;	
	abnCfg.bitFields.ovpAlarmTh = (regData & (0x03 << 1)) >> 1;
	Abn_Set_Config(&abnCfg);
}

#endif






#ifdef API_REG	//0x70-0x7E
/*	PD命令操作
* @name   Execute_Register70_Function
* bit7：  不检测线缆信息，直接广播
* bit6：  重新广播PD_PDO
* bit5：  重新广播UFCS_PDO
* bit4：  PD 软件复位
* bit3：  PD 硬件复位
* bit2：  退出EPR模式
* bit1：  
* bit0：  
* @param  regData
* @retval None      
*/
void Execute_Register70_Function(uint8_t regData)
{
	if((regData & (0x01 << 7)) >> 7)
	{
		Pd_Src_Cap_Ignore_Emarker(true);
	}
	
	if((regData & (0x01 << 6)) >> 6)
	{
		Update_PD_PDO();
		Pd_Rebroadcast_Src_Cap();
		policyGlobalData.isOccBroadcast = true;
	}
	
	if((regData & (0x01 << 5)) >> 5)
	{
		Update_UFCS_PDO();
		Ufcs_Set_Power_Change();
		policyGlobalData.isOccBroadcast = true;
	}
	
	if((regData & (0x01 << 4)) >> 4)
	{
		Pd_Send_Soft_Reset();
	}
	
	if((regData & (0x01 << 3)) >> 3)
	{
		Pd_Src_Send_Hard_Reset();
	}		
	
	if((regData & (0x01 << 2)) >> 2)
	{	
		Pd_Exit_Epr_Mode();
	}
	Set_RegAddr_Data(0x70, 0x00);
}


/*	BUCK参数配置
* @name   Execute_Register71_Function
* bit7-6：  buck频率：0：125K 1：181K 2：333K  
* bit5-4：  恒温环斜率
* bit3-2：  恒温环起始温度
* bit1：  	
* bit0：  
* @param  regData
* @retval None      
*/
void Execute_Register71_Function(uint8_t regData)
{
	switch((regData & (0x03 << 6)) >> 6)
	{
		case 0:
			Device_Set_Buck_Clock(2000 / 125);
			Device_Buck_Config(BUCK_CFG_125KHZ_22UH);
			break;
		case 1:
			Device_Set_Buck_Clock(2000 / 181);
			Device_Buck_Config(BUCK_CFG_181KHZ_10UH);			
			break;
		case 2:
			Device_Set_Buck_Clock(2000 / 333);
			Device_Buck_Config(BUCK_CFG_333KHZ_4UH7);					
			break;
		default:
			Device_Set_Buck_Clock(2000 / 181);
			Device_Buck_Config(BUCK_CFG_181KHZ_10UH);					
			break;
	}
	Device_Buck_Config_Temp_Parameter((regData & (0x03 << 4)) >> 4,(regData & (0x03 << 2)) >> 2);
}

/*	HSMOS电阻值设置
* @name   Execute_Register72_Function
* bit7-5：  设置mos电阻
* bit4-2：  设置ocp阈值  
* bit1：  
* bit0：  
* @param  regData
* @retval None      
*/
void Execute_Register72_Function(uint8_t regData)
{
	Device_Cfg_Mos_Resistance((regData & (0x07 << 5)) >> 5);
	Device_Set_Vr_Ocp((regData & (0x07 << 2)) >> 2);
}

/*	端口空载门限
* @name   Execute_Register73_Function
* bit7-4：  设置Port1空载阈值
* bit3-0：  设置Port2空载阈值
* @param  regData
* @retval None      
*/
void Execute_Register73_Function(uint8_t regData)
{
	Config_Set_Noload_Threshold(1, (regData & (0x0F << 4)) >> 4);
	Config_Set_Noload_Threshold(2, (regData & (0x0F << 0)) >> 0);
}


/*	Power驱动强度
* @name   Execute_Register74_Function
* bit7-5：  下拉强度
* bit4-2：  上拉强度  
* bit1：  
* bit0： 
* @param  regData
* @retval None      
*/
void Execute_Register74_Function(uint8_t regData)
{
	Power_Hsmos_Strength_Cfg((regData & (0x07 << 5)) >> 5, (regData & (0x07 << 2)) >> 2);
}


/*	调压调流斜率
* @name   Execute_Register75_Function
* bit7-6：设置调压斜率
* bit5-4：设置调流斜率
* bit3：
* bit2：
* bit1：  
* bit0： 
* @param  regData
* @retval None      
*/
void Execute_Register75_Function(uint8_t regData)
{
	Power_Set_Voltage_Slew((regData & (0x03 << 6)) >> 6);
	Power_Set_Current_Slew((regData & (0x03 << 4)) >> 4);
}


/*	强制异常使能操作
* @name   Execute_Register76_Function
* bit7：  打开LPS
* bit6：  关闭LPS
* bit5：  打开Vin-Ovp
* bit4：  关闭Vin-Ovp
* bit3：  打开Vout-Ovp
* bit2：  关闭Vout-Ovp
* bit1：  主动触发异常,关闭buck
* bit0：  关闭触发异常,打开buck
* @param  regData
* @retval None      
*/
void Execute_Register76_Function(uint8_t regData)
{
	//在未关闭buck状态下，操作打开buck会出现异常
	static bool isCloseBuck;
	
	if((regData & (0x01 << 7)) >> 7)
	{
		Config_Set_Lps(true);
	}
	if((regData & (0x01 << 6)) >> 6)
	{
		Config_Set_Lps(false);
	}
	if((regData & (0x01 << 5)) >> 5)
	{
		Vin_Ovp_Ctrl(true);
	}
	if((regData & (0x01 << 4)) >> 4)
	{
		Vin_Ovp_Ctrl(false);
	}
	if((regData & (0x01 << 3)) >> 3)
	{
		Vout_Ovp_Ctrl(true);
	}		
	if((regData & (0x01 << 2)) >> 2)
	{	
		Vout_Ovp_Ctrl(false);
	}
	if((regData & (0x01 << 1)) >> 1)
	{
		isCloseBuck = true;
		Vout_Ovp_Ctrl(false);
		Abn_Force_Ctrl(true);
	}		
	if((regData & (0x01 << 0)) >> 0)
	{
		if(isCloseBuck)
		{
			isCloseBuck = false;
			Vout_Ovp_Ctrl(true);
			Abn_Force_Ctrl(false);			
		}
	}
	//功能执行完寄存器数据清零
	Set_RegAddr_Data(0x76, 0x00);
}


/*	强制操作
* @name   Execute_Register77_Function
* bit7：  Port1_DPDM短接
* bit6：  Port1_DPDM不短接
* bit5：  Port2_DPDM短接
* bit4：  Port2_DPDM不短接
* bit3：  Flash_休眠
* bit2：  Flash_不休眠
* bit1：  关闭输出(C口)
* bit0：  打开输出(C口)
* @param  regData
* @retval None      
*/
void Execute_Register77_Function(uint8_t regData)
{
	//在未关闭buck状态下，操作打开buck会出现异常
	static bool isCloseBuck;
	
	if((regData & (0x01 << 7)) >> 7)
	{
		Dpdm_Short_Ctrl(1, true);
	}
	if((regData & (0x01 << 6)) >> 6)
	{
		Dpdm_Short_Ctrl(1, false);
	}
	if((regData & (0x01 << 5)) >> 5)
	{
		Dpdm_Short_Ctrl(2, true);
	}
	if((regData & (0x01 << 4)) >> 4)
	{
		Dpdm_Short_Ctrl(2, false);
	}
	if((regData & (0x01 << 3)) >> 3)
	{
		Low_Power_Flash_Ctrl(true);
	}		
	if((regData & (0x01 << 2)) >> 2)
	{	
		Low_Power_Flash_Ctrl(false);
	}
	if((regData & (0x01 << 1)) >> 1)
	{
		isCloseBuck = true;
		Vout_Ovp_Ctrl(false);
		Power_Gate_Ctrl(1,false);
		Power_Gate_Ctrl(2,false);
		Power_Buck_Ctrl(false);
		Port_Force_Disconnect(0x00);
		Port_Force_Disconnect(0x01);			
	}		
	if((regData & (0x01 << 0)) >> 0)
	{
		if(isCloseBuck)
		{
			isCloseBuck = false;
			Vout_Ovp_Ctrl(true);
			Port_Reconnect(0x00);
			Port_Reconnect(0x01);	
		}
	}
	//功能执行完寄存器数据清零
	Set_RegAddr_Data(0x77, 0x00);
}


/*	ADC使能
* @name   Execute_Register78_Function
* bit7：  ADC使能
* bit6：  ADC不使能
* bit5：  ADC_Vin使能
* bit4：  ADC_Vin不使能
* bit3：  ADC_Die强制关闭
* bit2：  ADC_Die强制打开
* bit1：  
* bit0：  
* @param  regData
* @retval None      
*/
void Execute_Register78_Function(uint8_t regData)
{
	//bit7
	if((regData & (0x01 << 7)) >> 7)
	{
		Adc_Enabled();
	}
	//bit6
	if((regData & (0x01 << 6)) >> 6)
	{
		Adc_Disabled();
	}
	//bit5
	if((regData & (0x01 << 5)) >> 5)
	{
		Adc_Vin_Enable();
	}
	//bit4
	if((regData & (0x01 << 4)) >> 4)
	{
		Adc_Vin_Disable();
	}
	//bit3
	if((regData & (0x01 << 3)) >> 3)
	{
		Adc_Die_Force_Close(true);
	}		
	//bit2
	if((regData & (0x01 << 2)) >> 2)
	{	
		Adc_Die_Force_Close(false);
	}

	//功能执行完寄存器数据清零
	Set_RegAddr_Data(0x78, 0x00);
}




#endif



#ifdef CUSTOM_FUN_REG	//0x7F-0x84

/*	休眠相关功能
* @name   Execute_Register7F_Function
* bit0：	写1退出休眠
* @param  regData
* @retval None      
*/
void Execute_Register7F_Function(uint8_t regData)
{
	
}

/*	Port1 PPS/UFCS电流偏置
* @name   Execute_Register80_Function
* bit7-4：  PPS电流偏置		-500 + 100 * bit[7-4]  ma
* bit3-0：  UFCS电流偏置  	-500 + 100 * bit[3-0]  ma
* @param  regData
* @retval None      
*/
void Execute_Register80_Function(uint8_t regData)
{

}

/*	Port2 PPS/UFCS电流偏置
* @name   Execute_Register81_Function
* bit7-4：  PPS电流偏置		-500 + 100 * bit[7-4]
* bit3-0：  UFCS电流偏置  	-500 + 100 * bit[3-0]
* @param  regData
* @retval None      
*/
void Execute_Register81_Function(uint8_t regData)
{

}



/*	IRQ相关功能
* @name   Execute_Register82_Function
* bit7-6：GPIO2_IRQ: 0、设备插入  1、发生调流调压
* bit5：发生中断信号，写1清0
* bit4：掉电标志-写入1之后，只有在初始化时才会清零。
* bit3-0：
* @param  regData
* @retval None      
*/
void Execute_Register82_Function(uint8_t regData)
{
	//bit7-6
	//---
	static u8 powerDownSign = 0;
	
	//bit5-中断信号
	if((regData & (0x01 << 5)) >> 5)
	{
		//写1清0
		Set_RegAddr_Data(0x82, (regData & ~(0x01 << 5)));		
	}
	
	//bit4-掉电标志
	if((powerDownSign == 0) && ((regData & (0x01 << 4)) >> 4))
	{
		powerDownSign = 1;
	}
	
	if(powerDownSign)
	{
		//写1后该bit不允许修改
		Set_RegAddr_Data(0x82, (regData | (0x01 << 4)));
	}
}










#endif



//写主策略
#ifdef WRITE_POLICY
//写执行寄存器功能
void Write_Policy(void)
{
	u8 regAddr = 0;
	u8 regData = 0;
	
	//判读FIFO是否为空
	if(!SqQueue_IsEmpty(&policyGlobalData.regAddrFifo))
	{
		//获取Fifo内寄存器地址
		Depart_SqQueue(&policyGlobalData.regAddrFifo, &regAddr);
		//PD_PDO和UFCS_PDO都不需要执行
		if((!(regAddr >= 0x30 && regAddr <= 0x64)) && regAddr < 0x85)
		{
			regData = Get_RegAddr_Data(regAddr);
			switch(regAddr)
			{
				#ifdef CONFIG_REG	//0x01-0x2F
				case 0x01:
					Execute_Register01_Function(regData);
					break;
				case 0x02:
					Execute_Register02_Function(regData);
					break;
				case 0x03:
					Execute_Register03_Function(regData);
					break;
				case 0x04:
					Execute_Register04_Function(regData);
					break;
				case 0x05:
					Execute_Register05_Function(regData);
					break;
				case 0x06:
					Execute_Register06_Function(regData);
					break;
				case 0x07:
					Execute_Register07_Function(regData);
					break;
				case 0x08:
					Execute_Register08_Function(regData);
					break;
				case 0x09:
					Execute_Register09_Function(regData);
					break;
				case 0x0A:
					Execute_Register0A_Function(regData);
					break;
				case 0x0B:
					Execute_Register0B_Function(regData);
					break;
				case 0x0C:
					Execute_Register0C_Function(regData);
					break;
				case 0x0D:
					Execute_Register0D_Function(regData);
					break;
				case 0x0E:
					Execute_Register0E_Function(regData);
					break;
				case 0x0F:
					Execute_Register0F_Function(regData);
					break;
				case 0x10:
					Execute_Register10_Function(regData);
					break;
				case 0x11:
					Execute_Register11_Function(regData);
					break;
				case 0x12:
					Execute_Register12_Function(regData);
					break;
				case 0x13:
					Execute_Register13_Function(regData);
					break;
				case 0x14:
					Execute_Register14_Function(regData);
					break;
				case 0x15:
					Execute_Register15_Function(regData);
					break;
				case 0x16:
					Execute_Register16_Function(regData);
					break;
				case 0x17:
					Execute_Register17_Function(regData);
					break;
				case 0x18:
					Execute_Register18_Function(regData);
					break;
				case 0x19:
					Execute_Register19_Function(regData);
					break;
				case 0x1A:
					Execute_Register1A_Function(regData);
					break;
				case 0x1B:
					Execute_Register1B_Function(regData);
					break;
				case 0x1C:
					Execute_Register1C_Function(regData);
					break;
				case 0x1D:
					Execute_Register1D_Function(regData);
					break;
				case 0x1E:
					Execute_Register1E_Function(regData);
					break;
				case 0x1F:
					Execute_Register1F_Function(regData);
					break;
				case 0x20:
					Execute_Register20_Function(regData);
					break;
				case 0x21:
					Execute_Register21_Function(regData);
					break;
				case 0x22:
					Execute_Register22_Function(regData);
					break;
				case 0x23:
					Execute_Register23_Function(regData);
					break;
				case 0x24:
					Execute_Register24_Function(regData);
					break;
				case 0x25:
					Execute_Register25_Function(regData);
					break;
				case 0x26:
					Execute_Register26_Function(regData);
					break;
				case 0x27:
					Execute_Register27_Function(regData);
					break;
				case 0x28:
					Execute_Register28_Function(regData);
					break;
				#endif
				
				#ifdef ABN_CONFIG_REG	//0x65-0x6F
				case 0x65:
					Execute_Register65_Function(regData);
					break;
				case 0x66:
					Execute_Register66_Function(regData);
					break;
				case 0x67:
					Execute_Register67_Function(regData);
					break;
				case 0x68:
					Execute_Register68_Function(regData);
					break;
				#endif
				
				
				#ifdef API_REG					//0x70-0x7E
				case 0x70:
					Execute_Register70_Function(regData);
					break;
				case 0x71:
					Execute_Register71_Function(regData);
					break;
				case 0x72:
					Execute_Register72_Function(regData);
					break;
				case 0x73:
					Execute_Register73_Function(regData);
					break;
				case 0x74:
					Execute_Register74_Function(regData);
					break;
				case 0x75:
					Execute_Register75_Function(regData);
					break;
				case 0x76:
					Execute_Register76_Function(regData);
					break;
				case 0x77:
					Execute_Register77_Function(regData);
					break;
				case 0x78:
					Execute_Register78_Function(regData);
					break;
				#endif
				#ifdef CUSTOM_FUN_REG	//0x7F-0x84
				
				#endif
				
				default:
					break;
			}
		}
	}
}

#endif


//正常接口
#ifdef NORMAL_FUNCTION
//获取PPSFlag参数
u32 Get_PPSFlag_data(uint8_t regData)
{
	u32 tempData = 0;
	if(regData)
	{
		tempData |= PDO_SPR_PPS_POWER_LIMITED;
	}
	return tempData;
}

//获取5VFlag参数
u32 Get_5VFlag_Data(uint8_t regData)
{
	//默认参数
	u32 tempData = PDO_FIXED_UNCONSTRAINED_POWER;
	
	if((regData & (0x01 << 7)) >> 7)
	{
		tempData |= PDO_FIXED_DUAL_POWER_ROLE;
	}

	if((regData & (0x01 << 6)) >> 6)
	{
		tempData |= PDO_FIXED_UNCONSTRAINED_POWER;
	}
	
	if((regData & (0x01 << 5)) >> 5)
	{
		tempData |= PDO_FIXED_COMM_CAP;
	}
	
	if((regData & (0x01 << 4)) >> 4)
	{
		tempData |= PDO_FIXED_DUAL_DATA_ROLE;
	}
	
	if((regData & (0x01 << 3)) >> 3)
	{
		tempData |= PDO_FIXED_EPR_MODE;
	}
	
	return tempData;
}




//更新PD_PDO
void Update_PD_PDO(void)
{
	//PD_PDO
	//数组下标
	u8 arraySub = 0;
	u8 sprFixNum = 0;
	u8 sprNum	= 0;
	u8 eprFixNum = 0;
	u8 eprNum	= 0;
	config_data_t* config			= 	Config_Get();
	//组装PD_PDO
	policyGlobalData.pdSrcCapArry[arraySub] = PDO_FIXED_VSAFE5V(FIX5V_CUR, FIX5V_FLAG, FIX5V_PEAK_CUR);
	arraySub++;
	
	if(FIX9V_ENABLE)
	{
		policyGlobalData.pdSrcCapArry[arraySub] = PDO_FIXED_OTHERS(FIX9V_VOL, FIX9V_CUR, FIX9V_PEAK_CUR);
		arraySub++;		
	}
	if(FIX12V_ENABLE)
	{
		policyGlobalData.pdSrcCapArry[arraySub] = PDO_FIXED_OTHERS(FIX12V_VOL, FIX12V_CUR, FIX12V_PEAK_CUR);
		arraySub++;		
	}
	if(FIX15V_ENABLE)
	{
		policyGlobalData.pdSrcCapArry[arraySub] = PDO_FIXED_OTHERS(FIX15V_VOL, FIX15V_CUR, FIX15V_PEAK_CUR);
		arraySub++;		
	}
	if(FIX20V_ENABLE)
	{
		policyGlobalData.pdSrcCapArry[arraySub] = PDO_FIXED_OTHERS(FIX20V_VOL, FIX20V_CUR, FIX20V_PEAK_CUR);
		arraySub++;		
	}
	
	sprFixNum = arraySub;
	if(PPS0_ENABLE)
	{
		policyGlobalData.pdSrcCapArry[arraySub] = PDO_PPS(PPS0_CUR_MAX, PPS0_VOL_MIN, PPS0_VOL_MAX, PPS0_FLAG);
		arraySub++;	
	}
	if(PPS1_ENABLE)
	{
		policyGlobalData.pdSrcCapArry[arraySub] = PDO_PPS(PPS1_CUR_MAX, PPS1_VOL_MIN, PPS1_VOL_MAX, PPS1_FLAG);
		arraySub++;	
	}
	//SPR数量最多为7个
	if(PPS2_ENABLE && arraySub < 7)
	{
		policyGlobalData.pdSrcCapArry[arraySub] = PDO_PPS(PPS2_CUR_MAX, PPS2_VOL_MIN, PPS2_VOL_MAX, PPS2_FLAG);
		arraySub++;	
	}
	sprNum = arraySub;
	
	//占位符SPR数量不满7个补零
	while((7 - arraySub) > 0)
	{
		policyGlobalData.pdSrcCapArry[arraySub] = 0;
		arraySub ++;		
	}
	
	if(FIX28V_ENABLE)
	{
		policyGlobalData.pdSrcCapArry[arraySub] = PDO_FIXED_OTHERS(FIX28V_VOL, FIX28V_CUR, FIX28V_PEAK_CUR);
		arraySub++;			
	}
	
	eprFixNum = arraySub - 7;
	
	if(AVS0_ENABLE)
	{
		policyGlobalData.pdSrcCapArry[arraySub] = PDO_AVS(AVS0_VOL_MIN, AVS0_VOL_MAX, PD_MAX_POWER);
		arraySub++;			
	}
	
	eprNum = arraySub - 7;
	
	config->pdSrcCap				= policyGlobalData.pdSrcCapArry;
	config->pdMaxPower 			= PD_MAX_POWER;
	config->sprFixPdoNumber = sprFixNum;
	config->sprPdoNumber		= sprNum;
	config->eprFixPdoNumber = eprFixNum;
	config->eprPdoNumber		= eprNum;
	
}


//更新UFCS_PDO
void Update_UFCS_PDO(void)
{
	//数组下标
	u8 arraySub = 0;
	config_data_t* config			= 	Config_Get();

	if(UFCS_ID1_ENABLE)
	{
		policyGlobalData.ufcsOutputCap[arraySub].fields.head = UFCS_CAP_SET_HEAD(UFCS_CAP_ID1, UFCS_ID1_CUR_STEP, UFCS_ID1_VOL_STEP);
		policyGlobalData.ufcsOutputCap[arraySub].fields.hiMaxVolage = UFCS_CAP_SET_HI_BYTE(UFCS_ID1_MAX_VOL / 10);         // unit : 10mV
		policyGlobalData.ufcsOutputCap[arraySub].fields.loMaxVolage = UFCS_CAP_SET_LO_BYTE(UFCS_ID1_MAX_VOL / 10);         // unit : 10mV
		policyGlobalData.ufcsOutputCap[arraySub].fields.hiMinVolage = UFCS_CAP_SET_HI_BYTE(UFCS_ID1_MIN_VOL / 10);         // unit : 10mV
		policyGlobalData.ufcsOutputCap[arraySub].fields.loMinVolage = UFCS_CAP_SET_LO_BYTE(UFCS_ID1_MIN_VOL / 10);         // unit : 10mV
		policyGlobalData.ufcsOutputCap[arraySub].fields.hiMaxCurrent = UFCS_CAP_SET_HI_BYTE(UFCS_ID1_MAX_CUR / 10);        // unit : 10mA
		policyGlobalData.ufcsOutputCap[arraySub].fields.loMaxCurrent = UFCS_CAP_SET_LO_BYTE(UFCS_ID1_MAX_CUR / 10);        // unit : 10mA
		policyGlobalData.ufcsOutputCap[arraySub].fields.minCurrent = UFCS_ID1_MIN_CUR;
		arraySub++;
	}
	
	if(UFCS_ID2_ENABLE)
	{
		policyGlobalData.ufcsOutputCap[arraySub].fields.head = UFCS_CAP_SET_HEAD(UFCS_CAP_ID2, UFCS_ID2_CUR_STEP, UFCS_ID2_VOL_STEP);
		policyGlobalData.ufcsOutputCap[arraySub].fields.hiMaxVolage = UFCS_CAP_SET_HI_BYTE(UFCS_ID2_MAX_VOL / 10);         // unit : 10mV
		policyGlobalData.ufcsOutputCap[arraySub].fields.loMaxVolage = UFCS_CAP_SET_LO_BYTE(UFCS_ID2_MAX_VOL / 10);         // unit : 10mV
		policyGlobalData.ufcsOutputCap[arraySub].fields.hiMinVolage = UFCS_CAP_SET_HI_BYTE(UFCS_ID2_MIN_VOL / 10);         // unit : 10mV
		policyGlobalData.ufcsOutputCap[arraySub].fields.loMinVolage = UFCS_CAP_SET_LO_BYTE(UFCS_ID2_MIN_VOL / 10);         // unit : 10mV
		policyGlobalData.ufcsOutputCap[arraySub].fields.hiMaxCurrent = UFCS_CAP_SET_HI_BYTE(UFCS_ID2_MAX_CUR / 10);        // unit : 10mA
		policyGlobalData.ufcsOutputCap[arraySub].fields.loMaxCurrent = UFCS_CAP_SET_LO_BYTE(UFCS_ID2_MAX_CUR / 10);        // unit : 10mA
		policyGlobalData.ufcsOutputCap[arraySub].fields.minCurrent = UFCS_ID2_MIN_CUR;
		arraySub++;
	}
	
	if(UFCS_ID3_ENABLE)
	{
		policyGlobalData.ufcsOutputCap[arraySub].fields.head = UFCS_CAP_SET_HEAD(UFCS_CAP_ID3, UFCS_ID3_CUR_STEP, UFCS_ID3_VOL_STEP);
		policyGlobalData.ufcsOutputCap[arraySub].fields.hiMaxVolage = UFCS_CAP_SET_HI_BYTE(UFCS_ID3_MAX_VOL / 10);         // unit : 10mV
		policyGlobalData.ufcsOutputCap[arraySub].fields.loMaxVolage = UFCS_CAP_SET_LO_BYTE(UFCS_ID3_MAX_VOL / 10);         // unit : 10mV
		policyGlobalData.ufcsOutputCap[arraySub].fields.hiMinVolage = UFCS_CAP_SET_HI_BYTE(UFCS_ID3_MIN_VOL / 10);         // unit : 10mV
		policyGlobalData.ufcsOutputCap[arraySub].fields.loMinVolage = UFCS_CAP_SET_LO_BYTE(UFCS_ID3_MIN_VOL / 10);         // unit : 10mV
		policyGlobalData.ufcsOutputCap[arraySub].fields.hiMaxCurrent = UFCS_CAP_SET_HI_BYTE(UFCS_ID3_MAX_CUR / 10);        // unit : 10mA
		policyGlobalData.ufcsOutputCap[arraySub].fields.loMaxCurrent = UFCS_CAP_SET_LO_BYTE(UFCS_ID3_MAX_CUR / 10);        // unit : 10mA
		policyGlobalData.ufcsOutputCap[arraySub].fields.minCurrent = UFCS_ID3_MIN_CUR;
		arraySub++;
	}	

	config->ufcsSrcCapNum = arraySub;
	config->ufcsSrcCap		= (const volatile uint8_t*)&policyGlobalData.ufcsOutputCap;
	
	
}



#endif