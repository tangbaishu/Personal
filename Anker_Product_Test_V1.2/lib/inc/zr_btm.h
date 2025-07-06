/****************************************************************************
 * @copyright Copyright(C) 2020-2023 Ismartware Limited. All rights reserved.
 * @file zr_btm.h
 * @brief Functions interfaces for the BTM4 firmware library
 * @author David
 ****************************************************************************/

#ifndef ZR_BTM_H
#define ZR_BTM_H

#include "system.h"
#include <stdint.h>

/**
 * @brief  Enable BTM4
 */
void Btm4_Enable(void);

/**
 * @brief  Disable BTM4
 */
void Btm4_Disable(void);

/**
 * @brief  Enable BTM4 interrupt
 */
void Btm4_Int_Enable(void);

/**
 * @brief  Disable BTM4 interrupt
 */
void Btm4_Int_Disable(void);

/**
 * @brief  Get BTM4 interrupt pending
 * @return SET when there is a BTM4 interrupt, otherwiser return NOTSET
 */
ITStatus Btm4_Get_Int_Pending(void);

/**
 * @brief  Clear BTM4 interrupt pending
 */
void Btm4_Clear_Int_Pending(void);

/**
 * @brief  Get BTM4 current value
 * @return current value
 */
uint32_t Btm4_Get_Current_Value(void);

/**
 * @brief  Set BTM4 reload value
 * @param value counter reload value(20 bits)
 */
void Btm4_Set_Reload_Value(uint32_t value);

#endif
