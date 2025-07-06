/****************************************************************************
 * @copyright Copyright(C) 2024-2024 Ismartware Limited. All rights reserved.
 * @file main.c
 * @brief main
 * @author SZ team
 ****************************************************************************/
#include "api.h"
#include "serial.h"
#include <stdio.h>
#include "iic_slave_init_policy.h"
#include "iic_slave_main_policy.h"
#include "anker_policy.h"
extern void Anker_Uart_Enable();

int main(void)
{
    /// device init shall be called firstly
    Device_Init();
    Init_Config_Policy();
    Main_Start_Run_Policy();

    while (1)
    {
        // the following sequence shall NOT change
        Device_Run();
        Main_Policy();
    }
}
