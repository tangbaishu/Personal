/****************************************************************************
 * @copyright Copyright(C) 2020-2023 Ismartware Limited. All rights reserved.
 * @file zr_nvic.h
 * @brief Functions interfaces for the NVIC firmware library
 * @author David
 ****************************************************************************/
#ifndef ZR_NVIC_H
#define ZR_NVIC_H

#include "system.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  NVIC irq enable 
 * @param  IRQn: interrupt number 
 *         This parameter can be:
 *         see IRQn_Type Definition   
 * @param  priority: interrupt priority 
 *         This parameter can be:
 *         PRIORITY_0      
 *         PRIORITY_1              
 *         PRIORITY_2              
 *         PRIORITY_3                                  
 */
void Nvic_Irq_Enable(IRQn_Type IRQn, Priority_Type priority);

/**
 * @brief  NVIC irq disable 
 * @param  IRQn: interrupt number 
 *         This parameter can be:
 *         see IRQn_Type Definition                                    
 */
void Nvic_Irq_Disable(IRQn_Type IRQn);

/**
 * @brief  Get NVIC IRQn pending 
 * @param  IRQn: interrupt number 
 *         This parameter can be:
 *         see IRQn_Type Definition                                    
 * @return NVIC pending(SET or NOTSET)
 *         SET:pending set    NOTSET:pending not set
 */
FlagStatus Nvic_Get_Irq_Pending(IRQn_Type IRQn);

/**
 * @brief  Set NVIC IRQn pending 
 * @param  IRQn: interrupt number 
 *         This parameter can be:
 *         see IRQn_Type Definition                                    
 */
void Nvic_Set_Irq_Pending(IRQn_Type IRQn);

/**
 * @brief  Clear NVIC IRQn pending 
 * @param  IRQn: interrupt number 
 *         This parameter can be:
 *         see IRQn_Type Definition                                    
 */
void Nvic_Clear_Irq_Pending(IRQn_Type IRQn);

/**
 * @brief  NVIC all irq enable 
 */
void Nvic_Irq_Enable_All(void);

/**
 * @brief  NVIC all irq disable 
 */
void Nvic_Irq_Disable_All(void);

#ifdef __cplusplus
}
#endif

#endif
