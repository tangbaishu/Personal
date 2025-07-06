/****************************************************************************
 * @copyright Copyright(C) 2024-2024 Ismartware Limited. All rights reserved.
 * @file anker_policy.h
 * @brief 
 * @author SZ team
 ****************************************************************************/
#ifndef Anker_Policy_H
#define Anker_Policy_H


#include "iic_slave_global_variables_policy.h"
#include "pd_message_variable.h"

//安克创新VID
#define ANKER_VID		0x291A

//Anker_Uart引脚定义
#define ANKER_BAUDRATE 115200
#define DEMO_BOARD 0
#if DEMO_BOARD
	#define UART_TX_GPIO10 (GPIO1) 
	#define UART_RX_GPI011 (GPIO2)
	#define UART_TX_FUNC 	(GPIO1_FUNC_SEL_UART_TX)
	#define UART_RX_FUNC 	(GPIO2_FUNC_SEL_UART_RX)
#else
	#define UART_TX_GPIO10 (GPIO10) 
	#define UART_RX_GPI011 (GPIO11)
	#define UART_TX_FUNC 	(GPIO10_FUNC_SEL_UART_TX)
	#define UART_RX_FUNC 	(GPIO11_FUNC_SEL_UART_RX)
#endif


extern PD_UVDM_Anker_Package_t PD_UVDM_Anker_Package;

//anker产测功能
void Anker_Product_Test_Policy(void);

bool Anker_UVDM_Decode(void);

#endif