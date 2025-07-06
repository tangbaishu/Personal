/****************************************************************************
 * @copyright Copyright(C) 2024-2024 Ismartware Limited. All rights reserved.
 * @file iic_slave_drive_policy.h
 * @brief drive_policy
 * @author SZ team
 ****************************************************************************/
#ifndef IIC_Slave_Drive_Policy_H
#define IIC_Slave_Drive_Policy_H

#include "api.h"
#include "pdo.h"
#include "config.h"
#include "zr_adc.h"
#include "zr_atm.h"
#include "zr_i2c.h"
#include "zr_rcc.h"
#include "zr_gpio.h"
#include "zr_nvic.h"
#include "ufcs_caps.h"
#include "pd_message.h"
#include "iic_slave_global_variables_policy.h"

//ATM0定时器_使能
void Atm0_Custom_Enable(void);
//ATM0定时器_取消使能
void Atm0_Custom_Disable(void);

#endif