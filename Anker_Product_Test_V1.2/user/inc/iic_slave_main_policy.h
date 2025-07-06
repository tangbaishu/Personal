/****************************************************************************
 * @copyright Copyright(C) 2024-2024 Ismartware Limited. All rights reserved.
 * @file iic_slave_main_policy.h
 * @brief main_policy
 * @author SZ team
 ****************************************************************************/
#ifndef IIC_Slave_Main_Policy_H
#define IIC_Slave_Main_Policy_H

#include "api.h"
#include "config.h"
#include "iic_slave_global_variables_policy.h"
#include "iic_slave_write_policy.h"
#include "iic_slave_read_policy.h"
#include "device_compatibility_policy.h"
#include "anker_policy.h"

//是否开始运行主程序
void Main_Start_Run_Policy(void);
//从机主策略
void Main_Policy(void);

#endif


