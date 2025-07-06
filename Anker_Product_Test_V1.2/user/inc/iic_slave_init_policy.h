/****************************************************************************
 * @copyright Copyright(C) 2024-2024 Ismartware Limited. All rights reserved.
 * @file iic_slave_init_policy.h
 * @brief init_policy
 * @author SZ team
 ****************************************************************************/
#ifndef IIC_Slave_Init_Policy_H
#define IIC_Slave_Init_Policy_H

#include "api.h"
#include "pdo.h"
#include "config.h"
#include "zr_adc.h"
#include "zr_atm.h"
#include "zr_i2c.h"
#include "zr_rcc.h"
#include "zr_gpio.h"
#include "zr_nvic.h"
#include "zr_systick.h"
#include "ufcs_caps.h"
#include "pd_message.h"
#include "iic_slave_global_variables_policy.h"
#include "iic_slave_callback_policy.h"


//IIC引脚定义
#define I2CS_SDA_GPIO1 (GPIO1) 
#define I2CS_SCK_GPIO0 (GPIO0)
#define I2CS_SDA_FUNC (GPIO1_FUNC_SEL_I2C_SDA)
#define I2CS_SCK_FUNC (GPIO0_FUNC_SEL_I2C_SCK)

//中断引脚定义
#define IRQ_GPIO2 	(GPIO2)
#define IRQ_FUNC 		(GPIO2_FUNC_SEL_GPIO)
#define IRQ_GPIO2_1 (Gpio_Set_Output_Value(IRQ_GPIO2, 1))
#define IRQ_GPIO2_0 (Gpio_Set_Output_Value(IRQ_GPIO2, 0))

//TypeC正插反插引脚定义
#define TYPEC_GPIO3 	(GPIO3)
#define TYPEC_FUNC 		(GPIO3_FUNC_SEL_GPIO)
#define TYPEC_GPIO3_1 (Gpio_Set_Output_Value(TYPEC_GPIO3, 1))
#define TYPEC_GPIO3_0 (Gpio_Set_Output_Value(TYPEC_GPIO3, 0))

//ATM0定时器初始化
void Atm0_Config_Init(void);

//IIC_Slave配置初始化
void I2c_Slave_Config_Init(void);
	
//GPIO配置初始化
void Gpio_Config_Init(void);

//ADC配置初始化
void Adc_Config_Init(void);

//IICSlave地址初始化
void IICSlave_Address_Init(void);

//寄存器数据及功能初始化
void Reg_Data_Init(void);

//总功能初始化
void Init_Config_Policy();

#endif