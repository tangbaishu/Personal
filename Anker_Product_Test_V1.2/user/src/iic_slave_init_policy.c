	/****************************************************************************
 * @copyright Copyright(C) 2024-2024 Ismartware Limited. All rights reserved.
 * @file iic_slave_init_policy.c
 * @brief init_policy
 * @author SZ team
 ****************************************************************************/
#include "iic_slave_init_policy.h"

//硬件初始化
#define HARD_INIT
#define CONFIG_REG			//0x01-0x2F
#define PD_PDO_REG			//0x30-0x4F
#define UFCS_PDO_REG		//0x50-0x64
#define ABN_CONFIG_REG	//0x65-0x6F
#define API_REG					//0x70-0x7E
#define CUSTOM_FUN_REG	//0x7F-0x84

//策略全局变量
extern policy_data_t policyGlobalData;

//总功能初始化
void Init_Config_Policy(void)
{
	Nvic_Irq_Enable_All();
	//定时器初始化
	Atm0_Config_Init();
	//ADC初始化
	Adc_Config_Init();
	//IIC地址初始化-需要使用到定时器功能、ADC功能
	IICSlave_Address_Init();
	//设备地址初始化-
	I2c_Slave_Config_Init();
	//FIFO初始化
	Init_SqQueue(&policyGlobalData.regAddrFifo);
	//Anker队列初始化
	Anker_Init_SqQueue(&policyGlobalData.curAnkerSqQueue);
	//强制关闭芯片温度检测
	Adc_Die_Force_Close(true);
	//初始化回调接口
	Init_CallBack();
	//寄存器数据及功能初始化
	Reg_Data_Init();	
	//IRQ引脚配置为拉高,初始化完成
	Gpio_Config_Init();	
}

#ifdef HARD_INIT
//ATM0定时器初始化
void Atm0_Config_Init(void)
{
	Rcc_Module_Clk_Enable(ATM_ID);
	Rcc_Module_Rst_Release(ATM_ID);
	Atm0_Set_Reload_Value(50000);//12500*80ns=1ms //50000*80=4ms
	Atm0_Int_Enable();
	Nvic_Irq_Enable(ATM0_IRQn, PRIORITY_3);
	Atm0_Enable();	
}

//IIC_Slave配置初始化
void I2c_Slave_Config_Init(void)
{
	I2c_Master_Disable();
	Rcc_Module_Rst(I2C_ID);
	Rcc_Module_Clk_Enable(GPIO_ID | I2C_ID);
	Rcc_Module_Rst_Release(I2C_ID);
	//将GPIO引脚设置为IIC通道引脚
	Gpio_Function_Select(I2CS_SDA_GPIO1, I2CS_SDA_FUNC);
	Gpio_Function_Select(I2CS_SCK_GPIO0, I2CS_SCK_FUNC);
	//将GPIO引脚设置为开漏输出
	Gpio_Mos_Opendrain_Cfg(I2CS_SDA_GPIO1, PMOS_OPDRAIN_DIS, NMOS_OPDRAIN_EN);
	Gpio_Mos_Opendrain_Cfg(I2CS_SCK_GPIO0, PMOS_OPDRAIN_DIS, NMOS_OPDRAIN_EN);
	Gpio_Pulldown_Pullup_Cfg(I2CS_SDA_GPIO1, PULLDOWN_DIS, PULLUP_EN);
	Gpio_Pulldown_Pullup_Cfg(I2CS_SCK_GPIO0, PULLDOWN_DIS, PULLUP_EN);
	//IIC从机初始化
	I2c_Slave_Device_Addr_Cfg(policyGlobalData.iicDeviceAddr);
	I2c_Slave_TX_FIFO_Reset();
	I2c_Slave_RX_FIFO_Reset();
	I2c_Slave_Int_Enable(I2C_S_TX_DATA_INT_EN | I2C_S_RX_DATA_INT_EN);
	Nvic_Irq_Enable(I2CS_IRQn, PRIORITY_0);
	I2c_Slave_Enable();
	I2c_Slave_Clear_Pending(I2C_S_RX_DATA_PENGING | I2C_S_TX_DATA_PENGING | I2C_S_FINISH_PENGING
													| I2C_S_RX_OVERFLOW_PENGING | I2C_S_TX_UNDERFLOW_PENGING | I2C_S_RX_FIFO_PENGING
													| I2C_S_TX_FIFO_PENGING | I2C_S_BUS_ERROR_PENGING);	
}


//GPIO配置初始化
void Gpio_Config_Init(void)
{
		gpio_config_t gpioConfig;
		Rcc_Module_Clk_Enable(GPIO_ID);
		Gpio_Deinit(IRQ_GPIO2);
		gpioConfig .direction = GPIO_DIRECTION_OUTPUT;
		gpioConfig .gpioFunc = (gpio_func_e)IRQ_FUNC;
		//将引脚配置为推挽输出
		gpioConfig .pmosOd   = PMOS_OPDRAIN_DIS;
		gpioConfig .nmosOd   = NMOS_OPDRAIN_DIS;
		gpioConfig .pullup   = PULLUP_EN;
		gpioConfig .pulldown = PULLDOWN_DIS;
		gpioConfig .res = PULLUP_RES_10K;
		Gpio_Init(IRQ_GPIO2, &gpioConfig);
		IRQ_GPIO2_0;
	
		Gpio_Deinit(TYPEC_GPIO3);
		gpioConfig .direction = GPIO_DIRECTION_OUTPUT;
		gpioConfig .gpioFunc = (gpio_func_e)TYPEC_FUNC;
		//将引脚配置为推挽输出
		gpioConfig .pmosOd   = PMOS_OPDRAIN_DIS;
		gpioConfig .nmosOd   = NMOS_OPDRAIN_DIS;
		gpioConfig .pullup   = PULLUP_EN;
		gpioConfig .pulldown = PULLDOWN_DIS;
		gpioConfig .res = PULLUP_RES_10K;
		Gpio_Init(TYPEC_GPIO3, &gpioConfig);
		TYPEC_GPIO3_1;	
	
}


//ADC配置初始化
void Adc_Config_Init(void)
{
	Adc_Enabled();
	//Adc_vin使能
	Adc_Vin_Enable();
	adc_config_t adcCfg;
	Adc_Get_Config(&adcCfg);
	//配置VDPin为ADC功能
	adcCfg.adc3Pullup = ADC_PULLUP_CURRENT_20UA;//100KΩ（≈1.9V-2V）
	adcCfg.adc3Source = ADC3_SIG_SRC_VD;//singal source should equal channel	
	Adc_Set_Config(&adcCfg);
	Adc_Start_Adc_Channel(ADC_CHL_3);
}


//IICSlave地址初始化
void IICSlave_Address_Init(void)
{
	//防止Vin电压不稳导致ADC功能异常,设备地址初始化异常
	bool isVinNormal = false;
	u8 checkVinCount = 0;
	
	u16 tempBuf[10];
	u32 tempVlaue = 0;
	
	while(!isVinNormal)
	{
		//判断输入电压是否达到6V
		if(Adc_Vin_Get() > 6000)
		{
			checkVinCount ++;
			if(checkVinCount > 6)
			{
				//连续检测到输入电压满足要求
				isVinNormal = true;
			}
		}
		else
		{
			checkVinCount = 0;
		}
		//死延时
		Systick_Delay_Ms(6);
	}
	
	//获取VDPin电阻
	for(u8 i = 0; i < 10; i++)
	{
		tempBuf[i] = Adc_Get_Channel_Data(ADC_CHL_3);
		Systick_Delay_Ms(6);
	}
	
	//冒泡排序
	for(int i = 0 ; i < 10 ; i++)
	{
		for(int j = 0 ; j <9 - i ; j++ )
		{
				if(tempBuf[j] > tempBuf[j+1])
				{
					 tempVlaue = tempBuf[j];
					 tempBuf[j] = tempBuf[j+1];
					 tempBuf[j+1] = tempVlaue;                   
				}
		 }
	}
	//电流值设置为20ua
	//VDPin电压值为3.3V
	//理论获取电阻值最大为 R = U/I = 3300 / (20 / 1000) = 165000 = 165k
	//换算电阻值
	tempVlaue = tempBuf[4] * 50;
	//每20k差异对应一个地址
	//00k  - 20k --0x3D---引脚推荐电阻10k ± 5%
	//20k  - 40k --0x3E---引脚推荐电阻30k ± 5%
	//40k  - 60k --0x3F---引脚推荐电阻50k ± 5%
	//60k  - 80k --0x40---引脚推荐电阻70k ± 5%
	//80k  - 100k--0x41---引脚推荐电阻90k ± 5%
	//100k - 120k--0x42---引脚推荐电阻110k ± 5%
	//120k - 140k--0x43---引脚推荐电阻130k ± 5%
	//140k - 160k--0x44---引脚推荐电阻150k ± 5%
	//160k - ∞  --0x45---引脚悬空
	policyGlobalData.iicDeviceAddr = 0x3D + (tempVlaue / 20000);
	//检测完成，关闭vd_adc检测
	Adc_Stop_Adc_Channel(ADC_CHL_3);
	//检测完成，关闭vin检测
	Adc_Vin_Disable();
	Adc_Disabled();
}

#endif


//寄存器数据及功能初始化
void Reg_Data_Init(void)
{
	//FiFo最多填充数据为64个
	#ifdef CONFIG_REG	//0x01-0x2F (40-8)
	
	//单C、port1/port2限流3A、主程序不使能
	Set_RegAddr_Data(0x01, (0x01 << 5) | (0x03 << 3) | (0x03 << 1) | (0x00 << 0));
	Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x01);	
	
	//PD使能、UFCS使能、QC2.0使能、QC3.0使能、QC3+使能、FCP使能、高压SCP、低压SCP
	Set_RegAddr_Data(0x02, (0x01 << 7) | (0x01 << 6) | (0x01 << 5) | (0x01 << 4) | (0x01 << 3) | (0x01 << 2) | (0x01 << 1) | (0x01 << 0));
	Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x02);	
	
	//VOOC1.0不使能、VOOC4.0不使能、Supper VOOC2.0不使能、SFCP不使能、TFCP不使能、AFC不使能、PE不使能、Reserved
	Set_RegAddr_Data(0x03, (0x00 << 7) | (0x00 << 6) | (0x00 << 5) | (0x00 << 4) | (0x00 << 3) | (0x00 << 2) | (0x00 << 1) | (0x00 << 0));
	Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x03);		
	
	//PD最大100W
	Set_RegAddr_Data(0x04, 100 << 0);
	Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x04);		
	
	//PD版本 0x06
	Set_RegAddr_Data(0x05, 6 << 0);
	Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x05);		
	
	//PD_VDM版本 v2.0
	Set_RegAddr_Data(0x06, 0 << 0);
	Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x06);		
	
	//PPS保护阈值电压-3100mV
	Set_RegAddr_Data(0x07, (3100 / 50) << 0);
	Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x07);		
	
	//PD Gating Vooc不使能、PD Gating SCP不使能、Vconn_Swap不使能、Discover_Id使能、PdAlert不使能、VconnLowPowerMode使能、非法PPS请求使能、广播5V2A不使能
	Set_RegAddr_Data(0x08, (0x00 << 7) | (0x00 << 6) | (0x00 << 5) | (0x01 << 4) | (0x00 << 3) | (0x01 << 2) | (0x01 << 1) | (0x00 << 0));
	Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x08);	

	//PD XID[31:24] 0x00
	Set_RegAddr_Data(0x09, 0 << 0);
	//Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x09);		
	
	//PD XID[23:16] 0x00
	Set_RegAddr_Data(0x0A, 0 << 0);
	//Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x0A);		
	
	//PD XID[15:8] 0x00
	Set_RegAddr_Data(0x0B, 0 << 0);
	//Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x0B);		
	
	//PD XID[7:0] 0x00
	Set_RegAddr_Data(0x0C, 0 << 0);
	//Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x0C);	
	
	//PD VID[15:8] 0x2F
	Set_RegAddr_Data(0x0D, 0x2F << 0);
	Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x0D);		
	
	//PD VID[7:0] 0xE6
	Set_RegAddr_Data(0x0E, 0xE6 << 0);
	Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x0E);	
	
	//PD PID[15:8] 0x00
	Set_RegAddr_Data(0x0F, 0 << 0);
	//Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x0F);		
	
	//PD PID[7:0] 0x00
	Set_RegAddr_Data(0x10, 0 << 0);
	//Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x10);	
	
	//PD BCD_DEVICE[15:8] 0x00
	Set_RegAddr_Data(0x11, 0 << 0);
	//Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x11);		
	
	//PD BCD_DEVICE[7:0] 0x00
	Set_RegAddr_Data(0x12, 0 << 0);
	//Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x12);	
	
	//UFCS 设备温度 25℃
	Set_RegAddr_Data(0x13, 25 << 0);
	Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x13);	
	
	//UFCS 端口温度 25℃
	Set_RegAddr_Data(0x14, 25 << 0);
	Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x14);
	
	//QC2最高电压12V、QC3最高电压12V、QC3最大功率45W、PE最高电压12V
	Set_RegAddr_Data(0x15, (0x02 << 6) | (0x02 << 4) | (0x02 << 2) | (0x02 << 0));
	Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x15);

	//AFC最高电压12V、SFCP最高电压12V、FCP最大电压12V
	Set_RegAddr_Data(0x16, (0x01 << 7) | (0x01 << 6) | (0x01 << 5));
	Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x16);
	
	//SCP协议最大电流 5A
	Set_RegAddr_Data(0x17, (5000/100) << 0);
	Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x17);
	
	//SCP协议最大电压 12V
	Set_RegAddr_Data(0x18, (12000/100) << 0);
	Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x18);
	
	//SCP协议最大功率 23W
	Set_RegAddr_Data(0x19, 23 << 0);
	Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x19);
	
	//SCP协议适配器类型 0x0A
	Set_RegAddr_Data(0x1A, 0x0A << 0);
	Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x1A);
	
	//DPDM协议最大功率 18W 
	Set_RegAddr_Data(0x1B, 18 << 0);
	Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x1B);
	
	//VOOC1.0/VOOC4.0最大功率 30W 
	Set_RegAddr_Data(0x1C, 30 << 0);
	Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x1C);
	
	//SuperVOOC最大功率 65W 
	Set_RegAddr_Data(0x1D, 65 << 0);
	Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x1D);
	
	//VOOC正品线缆Type150阻抗 149
	Set_RegAddr_Data(0x1E, 149 << 0);
	Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x1E);
	
	//VOOC正品线缆Type180阻抗 180
	Set_RegAddr_Data(0x1F, 180 << 0);
	Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x1F);
	
	//VOOC正品线缆Type250阻抗 250
	Set_RegAddr_Data(0x20, 250 << 0);
	Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x20);

	//VOOC普通线缆Type150阻抗 149
	Set_RegAddr_Data(0x21, 149 << 0);
	Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x21);
	
	//VOOC普通线缆Type180阻抗 180
	Set_RegAddr_Data(0x22, 180 << 0);
	Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x22);
	
	//VOOC普通线缆Type250阻抗 250
	Set_RegAddr_Data(0x23, 250 << 0);
	Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x23);

	//TFCP最大功率 18W
	Set_RegAddr_Data(0x24, 18 << 0);
	Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x24);
	
	//Type-C连接时间 130ms
	Set_RegAddr_Data(0x25, 130 << 0);
	Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x25);
	
	//Typec_CC = RP_3A、Typec_Vconn使能、Typec认证不使能、C口空载使能、LPS使能
	Set_RegAddr_Data(0x26, (0x01 << 6) | (0x01 << 5) | (0x00 << 4) | (0x01 << 3) | (0x01 << 2) | (0x00 << 1) | (0x00 << 0));
	Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x26);
	
	//线补使能、线补值100mohm、固定档位电压配置200mV、固定档位电流配置300mA
	Set_RegAddr_Data(0x27, (0x01 << 7) | (0x00 << 6) | (0x03 << 4) | (0x00 << 2));
	Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x27);
	
	//apple 2.7v使能、samsung 1.2v使能、UFCS认证不使能、PD认证不使能、PD Apple认证不使能、读Emaker使能、直接广播140W不使能、VOOC严格线缆不使能
	Set_RegAddr_Data(0x28, (0x01 << 7) | (0x01 << 6) | (0x00 << 5) | (0x00 << 4) | (0x00 << 3) | (0x01 << 2) | (0x00 << 1) | (0x00 << 0));
	Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x28);
	
	//Reg0x29-Reg0x2F Reserved 0x00
	Set_RegAddr_Data(0x29,0x00);
	Set_RegAddr_Data(0x2A,0x00);
	Set_RegAddr_Data(0x2B,0x00);
	Set_RegAddr_Data(0x2C,0x00);
	Set_RegAddr_Data(0x2D,0x00);
	Set_RegAddr_Data(0x2E,0x00);
	Set_RegAddr_Data(0x2F,0x00);
	
	#endif
		
	#ifdef PD_PDO_REG	//0x30-0x4F (0)
	//Fix_5V_Flag：PDO_FIXED_UNCONSTRAINED_POWER使能，其他不使能
	Set_RegAddr_Data(0x30,(0x00 << 7) | (0x01 << 6) | (0x00 << 5) | (0x00 << 4) | (0x00 << 3));
	
	//PD 9V使能、PD 12V使能、PD 15V使能、PD 20V使能、PPS0使能、PPS1使能、PPS2使能、PD 28V不使能、
	Set_RegAddr_Data(0x31,(0x01 << 7) | (0x01 << 6) | (0x00 << 5) | (0x01 << 4) | (0x01 << 3) | (0x01 << 2) | (0x01 << 1) | (0x00 << 0));
	
	//AVS0不使能、PPS0恒功率不使能、PPS1恒功率不使能、PPS2恒功率不使能、5V峰值电流：NO_PEAK_CURRENT、9V峰值电流：NO_PEAK_CURRENT
	Set_RegAddr_Data(0x32,(0x00 << 7) | (0x00 << 6) | (0x00 << 5) | (0x00 << 4) | (0x00 << 2) | (0x00 << 0));
	
	//12V峰值电流：NO_PEAK_CURRENT、15V峰值电流：NO_PEAK_CURRENT、20V峰值电流：NO_PEAK_CURRENT、28V峰值电流：NO_PEAK_CURRENT
	Set_RegAddr_Data(0x33,(0x00 << 6) | (0x00 << 4) | (0x00 << 2) | (0x00 << 0));
	
	//5V档电流 3A
	Set_RegAddr_Data(0x34,((3000/50) << 0));
	
	//9V档电压 9V
	Set_RegAddr_Data(0x35,((9000/100) << 0));
	
	//9V档电流 3A
	Set_RegAddr_Data(0x36,((3000/50) << 0));
	
	//12V档电压 12V
	Set_RegAddr_Data(0x37,((12000/100) << 0));
	
	//12V档电流 3A
	Set_RegAddr_Data(0x38,((3000/50) << 0));
	
	//15V档电压 15V
	Set_RegAddr_Data(0x39,((15000/100) << 0));
	
	//15V档电流 3A
	Set_RegAddr_Data(0x3A,((3000/50) << 0));
	
	//20V档电压 15V
	Set_RegAddr_Data(0x3B,((20000/100) << 0));
	
	//20V档电流 5A
	Set_RegAddr_Data(0x3C,((5000/50) << 0));
	
	//PPS0最小电压 5V
	Set_RegAddr_Data(0x3D,((5000/100) << 0));
	
	//PPS0最大电压 11V
	Set_RegAddr_Data(0x3E,((11000/100) << 0));
	
	//PPS0最大电流 3A
	Set_RegAddr_Data(0x3F,((3000/50) << 0));
	
	//PPS1最小电压 5V
	Set_RegAddr_Data(0x40,((5000/100) << 0));
	
	//PPS1最大电压 16V
	Set_RegAddr_Data(0x41,((16000/100) << 0));
	
	//PPS1最大电流 3A
	Set_RegAddr_Data(0x42,((3000/50) << 0));
	
	//PPS2最小电压 5V
	Set_RegAddr_Data(0x43,((5000/100) << 0));
	
	//PPS2最大电压 21V
	Set_RegAddr_Data(0x44,((21000/100) << 0));
	
	//PPS2最大电流 5A
	Set_RegAddr_Data(0x45,((5000/50) << 0));
	
	//28V档电压 28V
	Set_RegAddr_Data(0x46,((28000/200) << 0));
	
	//28V档电流 5A
	Set_RegAddr_Data(0x47,((5000/50) << 0));
	
	//AVS0最小电压 15V
	Set_RegAddr_Data(0x48,((15000/200) << 0));
	
	//AVS0最大电压 28V
	Set_RegAddr_Data(0x49,((28000/200) << 0));

	//Reg0x4A-Reg0x4F Reserved 0x00
	Set_RegAddr_Data(0x4A,0x00);
	Set_RegAddr_Data(0x4B,0x00);
	Set_RegAddr_Data(0x4C,0x00);
	Set_RegAddr_Data(0x4D,0x00);
	Set_RegAddr_Data(0x4E,0x00);
	Set_RegAddr_Data(0x4F,0x00);

	#endif
	
	#ifdef UFCS_PDO_REG		//0x50-0x64	(0)
	
	//UFCS_PPS0使能、UFCS_PPS1使能、UFCS_PPS2使能
	Set_RegAddr_Data(0x50,(0x01 << 7) | (0x01 << 6) | (0x01 << 5));
	
	//UFCS_PPS0电流步进50mA、UFCS_PPS0电压步进10mV、UFCS_PPS1电流步进50mA、UFCS_PPS1电压步进10mV
	Set_RegAddr_Data(0x51,(0x04 << 5) | (0x00 << 4) | (0x04 << 1) | (0x00 << 0));
	
	//UFCS_PPS2电流步进50mA、UFCS_PPS2电压步进10mV
	Set_RegAddr_Data(0x52,(0x04 << 5) | (0x00 << 4));
	
	//UFCS_PPS0最大电压 5.5V
	Set_RegAddr_Data(0x53,((5500/100) << 0));
	
	//UFCS_PPS0最小电压 3.4V
	Set_RegAddr_Data(0x54,((3400/100) << 0));
	
	//UFCS_PPS0最大电流 3A
	Set_RegAddr_Data(0x55,((3000/50) << 0));
	
	//UFCS_PPS0最小电流 100mA
	Set_RegAddr_Data(0x56,((100/50) << 0));
	
	//UFCS_PPS1最大电压 11V
	Set_RegAddr_Data(0x57,((11000/100) << 0));
	
	//UFCS_PPS1最小电压 5.5V
	Set_RegAddr_Data(0x58,((5500/100) << 0));
	
	//UFCS_PPS1最大电流 3A
	Set_RegAddr_Data(0x59,((3000/50) << 0));
	
	//UFCS_PPS1最小电流 100mA
	Set_RegAddr_Data(0x5A,((100/50) << 0));
	
	//UFCS_PPS2最大电压 21V
	Set_RegAddr_Data(0x5B,((21000/100) << 0));
	
	//UFCS_PPS2最小电压 11V
	Set_RegAddr_Data(0x5C,((11000/100) << 0));
	
	//UFCS_PPS2最大电流 5A
	Set_RegAddr_Data(0x5D,((5000/50) << 0));
	
	//UFCS_PPS2最小电流 100mA
	Set_RegAddr_Data(0x5E,((100/50) << 0));
	
	//Reg0x5F-Reg0x64 Reserved 0x00
	Set_RegAddr_Data(0x5F,0x00);
	Set_RegAddr_Data(0x60,0x00);
	Set_RegAddr_Data(0x61,0x00);
	Set_RegAddr_Data(0x62,0x00);
	Set_RegAddr_Data(0x63,0x00);
	Set_RegAddr_Data(0x64,0x00);
	
	#endif
	
	
	#ifdef ABN_CONFIG_REG	//0x65-0x6F	(0)
	
	//LPS ADC电流比较阈值：1.4A、LPS异常上管电流比较门限：LPS_ADC_CMP_TH_4P52A_5P67A
	Set_RegAddr_Data(0x65,(0x03 << 5) | (0x07 << 2));
	Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x65);
	
	//VOUT SCP阈值：3.0V、VOUT UVP阈值：70%、VIN UVLP阈值：5.6V、VIN OVP阈值：VIN_OVP_TH_38_36V
	Set_RegAddr_Data(0x66,(0x00 << 7) | (0x00 << 6) | (0x07 << 3) | (0x02 << 0));
	
	//VIN OVP消抖时间30us、VOUT SOVP消抖时间8ms、VIN FOVP消抖时间50us、VIN UVLO消抖时间4ms
	Set_RegAddr_Data(0x67,(0x00 << 7) | (0x00 << 6) | (0x00 << 3) | (0x01 << 0));
	
	//DPDM弱短路阈值4.2V、过温关机阈值150℃、过温报警阈值130℃
	Set_RegAddr_Data(0x68,(0x06 << 5) | (0x02 << 3) | (0x02 << 1));
	
	//Reg0x69-Reg0x6F Reserved 0x00
	Set_RegAddr_Data(0x69,0x00);
	Set_RegAddr_Data(0x6A,0x00);
	Set_RegAddr_Data(0x6B,0x00);
	Set_RegAddr_Data(0x6C,0x00);
	Set_RegAddr_Data(0x6D,0x00);
	Set_RegAddr_Data(0x6E,0x00);
	Set_RegAddr_Data(0x6F,0x00);
	
	#endif
	
	
	#ifdef API_REG				//0x70-0x7E (0)
	
	//PD操作自动清零，默认0x00
	Set_RegAddr_Data(0x70,0x01 << 6 | 0x01 << 5);
	Enter_SqQueue(&policyGlobalData.regAddrFifo, 0x70);	
	
	//Buck频率180KHZ、恒温环斜率：CONST_TEMP_11DEG_V、恒温环起始温度：CONST_TEMP_START_120_DEG
	Set_RegAddr_Data(0x71,(0x01 << 6) | (0x03 << 4) | (0x02 << 2));
	
	//HSMOS电阻值：8MOHM、OCP保护门限：15A
	Set_RegAddr_Data(0x72,(0x05 << 5) | (0x04 << 2));
	
	//port1空载电流15mA、port2空载电流15mA
	Set_RegAddr_Data(0x73,(0x04 << 4) | (0x04 << 0));
	
	//下拉强度：HSMOS_PD_TH6、上拉强度：HSMOS_PU_TH3
	Set_RegAddr_Data(0x74,(0x02 << 5) | (0x05 << 2));
	
	//调压斜率：VOLTAGE_SLEW_64US_10MV、调流斜率：CURRENT_SLEW_192US_25MA
	Set_RegAddr_Data(0x75,(0x02 << 6) | (0x03 << 4));
	
	//强制异常使能操作，自动清零
	Set_RegAddr_Data(0x76,0x00);
	
	//强制操作，自动清零
	Set_RegAddr_Data(0x77,0x00);
	
	//ADC使能，自动清零
	Set_RegAddr_Data(0x78,0x00);
	
	//Reg0x79-Reg0x7E Reserved 0x00
	Set_RegAddr_Data(0x79,0x00);
	Set_RegAddr_Data(0x7A,0x00);
	Set_RegAddr_Data(0x7B,0x00);
	Set_RegAddr_Data(0x7C,0x00);
	Set_RegAddr_Data(0x7D,0x00);
	Set_RegAddr_Data(0x7E,0x00);
	
	#endif
	
	
	#ifdef  CUSTOM_FUN_REG	//0x7F-0x84(0)
	//退出系统休眠
	Set_RegAddr_Data(0x7F,0x01);
	
	//Port1 PPS电流偏置0mA、UFCS电流偏置0mA
	Set_RegAddr_Data(0x80,(0x05 << 4) | (0x05 << 0));
	
	//Port2 PPS电流偏置0mA、UFCS电流偏置0mA
	Set_RegAddr_Data(0x81,(0x05 << 4) | (0x05 << 0));
	
	//Reg0x82-Reg0x84 Reserved 0x00
	Set_RegAddr_Data(0x82,0x00);
	Set_RegAddr_Data(0x83,0x00);
	Set_RegAddr_Data(0x84,0x00);
	
	#endif

}





