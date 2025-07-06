/****************************************************************************
 * @copyright Copyright(C) 2020-2023 Ismartware Limited. All rights reserved.
 * @file zr_systick.h
 * @brief Functions interfaces for system tick
 * @author David
 ****************************************************************************/
#ifndef ZR_SYSTICK_H
#define ZR_SYSTICK_H

#include "system.h"
#include "sys_clock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FAC_US (FCLK / 1000000)
#define FAC_MS (FCLK / 1000)

/**
 * @brief  N us hard delay with system tick, blocked operation
 * @param  nus  The number of us for delay
 *         This parameter can be:
 *         ticks value should not more than 419430
 */
void Systick_Delay_Us(u32 nus);

/**
 * @brief  N ms hard delay with system tick, blocked operation
 * @param  nms The number of ms for delay
 *         This parameter can be:
 *         ticks value should not more than 419
 */
void Systick_Delay_Ms(u16 nms);

#ifdef __cplusplus
}
#endif

#endif
