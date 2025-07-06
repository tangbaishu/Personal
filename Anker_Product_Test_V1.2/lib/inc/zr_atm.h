/****************************************************************************
 * @copyright Copyright(C) 2020-2023 Ismartware Limited. All rights reserved.
 * @file zr_atm.h
 * @brief Functions interfaces for the ATM firmware library
 * @author David
 ****************************************************************************/
#ifndef ZR_ATM_H
#define ZR_ATM_H

#include "system.h"

#ifdef __cplusplus
extern "C" {
#endif

/** atm source mode */
typedef enum
{
    ATM_LEVEL_MODE = (uint8_t)(0x00000000),        //!< level mode
    ATM_CLOCK_MODE = (uint8_t)(0x00000004),        //!< clock mode
} atm_capture_mode_e;

/** atm source select */
typedef enum
{
    ATM_SEL_GPIO = (uint8_t)(0x00000000),            //!< gpio
    ATM_SEL_GPIO_INV = (uint8_t)(0x00000010),        //!< invert of gpio
} atm_capture_source_e;

/** atm source state */
typedef enum
{
    ATM_SOURCE_COUNTER = (uint8_t)(0x00000000),        //!< source counter mode
    ATM_SOURCE_CAPTURE = (uint8_t)(0x00000002),        //!< source capture mode
} atm_mode_e;

/** atm config */
typedef struct
{
    atm_capture_mode_e capture_mode;            //!< capture_mode, see atm_capture_mode_e
    atm_capture_source_e capture_source;        //!< capture_source, see atm_capture_source_e
    atm_mode_e mode;                            //!< source_mode, see atm_source_mode_e
    uint32_t reload;                            //!< atm counter reload , max counter is 0xFFFFFF
} atm_config_t;

/**
 * @brief  Init ATM0
 * @param  cfg see atm_config_t
 */
void Atm0_Init(const atm_config_t* cfg);

/**
 * @brief  Deinit ATM0
 */
void Atm0_Deinit(void);

/**
 * @brief  ATM0 enable
 */
void Atm0_Enable(void);

/**
 * @brief  ATM0 disable
 */
void Atm0_Disable(void);

/**
 * @brief  ATM0 source enable
 */
void Atm0_Source_Enable(void);

/**
 * @brief  ATM0 source disable
 */
void Atm0_Source_Disable(void);

/**
 * @brief  ATM0 mode config
 * @param  mode:
 *         This parameter can be:
 *         ATM_SOURCE_COUNTER
 *         ATM_SOURCE_CAPTURE
 */
void Atm0_Mode_Config(atm_mode_e mode);

/**
 * @brief  ATM0 source config
 * @param  captureMode:
 *         This parameter can be:
 *         ATM_CLOCK_MODE
 *         ATM_LEVEL_MODE
 * @param  sourceSel source select
 *         This parameter can be:
 *         ATM_SEL_GPIO
 *         ATM_SEL_GPIO_INV
 */
void Atm0_Source_Config(atm_capture_mode_e captureMode, atm_capture_source_e sourceSel);

/**
 * @brief  ATM0 interrupt enable
 */
void Atm0_Int_Enable(void);

/**
 * @brief  ATM0 interrupt disable
 */
void Atm0_Int_Disable(void);

/**
 * @brief  Get ATM0 current counter value
 * @return ATM0 current counter value
 *         current value should not more than 0xFFFFFF
 */
u32 Atm0_Get_Current_Value(void);

/**
 * @brief  Set ATM0 reload value
 * @param  reloadValue ATMx reload value
 *         reload value should not more than 0xFFFFFF
 */
void Atm0_Set_Reload_Value(u32 reloadValue);

/**
 * @brief  Get ATM0 interrupt pending
 * @return The ATMx interrupt pending(SET or NOTSET)
 *         SET:pending set    NOTSET:pending not set
 */
ITStatus Atm0_Get_Int_Pending(void);

/**
 * @brief  Clear ATMx interrupt pending
 */
void Atm0_Clear_Int_Pending(void);

#ifdef __cplusplus
}
#endif

#endif
