/****************************************************************************
 * @copyright Copyright(C) 2020-2023 Ismartware Limited. All rights reserved.
 * @file zr_rcc.h
 * @brief Functions interfaces for the RCC firmware library
 * @author David
 ****************************************************************************/
#ifndef ZR_RCC_H
#define ZR_RCC_H

#include "system.h"

#ifdef __cplusplus
extern "C" {
#endif

// clang-format off
/** clk reset id */
#define WATCHDOG_ID          (0x00000001)
#define I2C_ID               (0x00000002)
#define UART_ID              (0x00000004)
#define ATM_ID               (0x00000008)
#define BTM_ID	             (0x00000010)
#define GPIO_ID              (0x00000020)
#define ADC_ID	             (0x00000800)

/** clock config */
#define SOURCE_CLK_RC1       (0x00000000)
#define SOURCE_CLK_125K      (0x00000001)
#define APBCLK_GATE_DIS      (0x00000002)
#define AHBCLK_GATE_DIS      (0x00000004)
// clang-format on

/**
 * @brief  Module clock enable
 * @param  id: Module id
 *         This parameter can be any combination of the following values:
 *         WATCHDOG_ID
 *         I2C_ID
 *         UART_ID
 *         ATM_ID
 *         GPIO_ID
 */
void Rcc_Module_Clk_Enable(u32 id);

/**
 * @brief  Module clock disable
 * @param  id: Module id
 *         This parameter can be any combination of the following values:
 *         WATCHDOG_ID
 *         I2C_ID
 *         UART_ID
 *         ATM_ID
 *         GPIO_ID
 */
void Rcc_Module_Clk_Disable(u32 id);

/**
 * @brief  Module reset
 * @param  id: Module id
 *         This parameter can be any combination of the following values:
 *         WATCHDOG_ID
 *         I2C_ID
 *         UART_ID
 *         ATM_ID
 *         GPIO_ID
 */
void Rcc_Module_Rst(u32 id);

/**
 * @brief  Module reset release
 * @param  id: Module id
 *         This parameter can be any combination of the following values:
 *         WATCHDOG_ID
 *         I2C_ID
 *         UART_ID
 *         ATM_ID
 *         GPIO_ID
 */
void Rcc_Module_Rst_Release(u32 id);

/**
 * @brief  System clock source configure
 * @param  clk_source: clock source
 *         This parameter can be:
 *         SOURCE_CLK_RC1
 *         SOURCE_CLK_125K
 */
void Rcc_Sysclk_Source_Cfg(u32 clk_source);

/**
 * @brief  APB clock gating enable
 */
void Rcc_APB_Clock_Gate_Enable(void);

/**
 * @brief  APB clock gating disable
 */
void Rcc_APB_Clock_Gate_Disable(void);

/**
 * @brief  AHB clock gating enable
 */
void Rcc_AHB_Clock_Gate_Enable(void);

/**
 * @brief  AHB clock gating disable
 */
void Rcc_AHB_Clock_Gate_Disable(void);

#ifdef __cplusplus
}
#endif

#endif
