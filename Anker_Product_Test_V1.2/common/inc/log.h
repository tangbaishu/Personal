/****************************************************************************
 * @copyright Copyright(C) 2020-2022 Ismartware Limited. All rights reserved.
 * @file log.h
 * @brief  Functions prototypes for log
 * @author SW team
 ****************************************************************************/
#ifndef LOG_H
#define LOG_H

// clang-format off
#include "system.h"
#include "zr_gpio.h"
#include "zr_rcc.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

// gpio clock shall be enabled outside

#ifdef LOG_GPIO

#define LOG_PIN (GPIO0)

#define Log_Pin_Init() do {                                \
    Gpio_Function_Select(LOG_PIN, FUNCTION_SEL0);          \
    Gpio_Direction_Cfg(LOG_PIN, GPIO_DIRECTION_OUTPUT);    \
    Gpio_Interrupt_Disable(LOG_PIN);                       \
    Gpio_Set_Output_Value(LOG_PIN, false);                 \
} while (0);

#define Log_Pin_On() do{ZR_GPIO->DATAOUT |= LOG_PIN; } while (0);
#define Log_Pin_Off() do{ZR_GPIO->DATAOUT &= (uint32_t)(~LOG_PIN); } while (0);
#define Log_Pin_Toggle() do{ZR_GPIO->DATAOUT ^= LOG_PIN; } while (0);
#else
#define Log_Pin_Init()
#define Log_Pin_On()
#define Log_Pin_Off()
#define Log_Pin_Toggle()
#endif

// clang-format on

#ifdef __cplusplus
}
#endif

#endif        // end of LOG_H
