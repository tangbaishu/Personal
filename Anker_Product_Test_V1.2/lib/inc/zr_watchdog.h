/****************************************************************************
 * @copyright Copyright(C) 2020-2023 Ismartware Limited. All rights reserved.
 * @file zr_watchdog.h
 * @brief Functions interfaces for the WDG firmware library
 * @author David
 ****************************************************************************/
#ifndef ZR_WATCHDOG_H
#define ZR_WATCHDOG_H

#include "system.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * @page watchdog Watchdog
 * 
 * - provides interfaces for watchdog features.
 * - watchdog can working in interrupt mode and poll mode.
 *  - in interrupt mode, NMI_Handler IRQ is used, calls Wdg_Clear_Pending() to feed watchdog in NMI_Handler().
 *  - in poll mode, call Wdg_Clear_Pending() to feed watchdog in while loop.
 * - watchdog also works while system is in sleep
 * - reset mode can be configured to NONE, RESET_CPU, RESET_SYSTEM.
 * - reset time is based on 8us unit and can be configured by reload value.
 * - system will do reset action based on configuration if there are NOT watchdog feeding TWICE.
 *
 * @note in interrupt & RESET_CPU mode, disable watchdog interrupt before stop feeding watchdog to make CPU reset workable
 * @par Example below shows how to use watchdog in interrupt mode
 * @code


int main(void)
{
    /// system init shall be called firstly
    Device_Init();

    Rcc_Module_Rst(WATCHDOG_ID);
    Rcc_Module_Clk_Enable(WATCHDOG_ID);
    Rcc_Module_Rst_Release(WATCHDOG_ID);
    Systick_Delay_Us(50);
    wdg_config_t wdgCfg = {0};
    // the watchdog timer is based 125Khz clock
    wdgCfg.interruptEn = WDG_INT_EN;
    // 10ms
    wdgCfg.reload = 10000/8;
    wdgCfg.resetMode = WDG_RST_SYSTEM;
    wdgCfg.resetState = WDG_TIMEOUT_RST_EN;
    Wdg_Init(&wdgCfg);

    while(1)
    {

    }
}

void NMI_Handler(void)
{
    Wdg_Clear_Pending();
}

 * @endcode
 */ 

/** wdg interrupt state */
typedef enum
{
    WDG_INT_DIS = (uint8_t)(0x00000000),        //!< watchdog timeout reset enable
    WDG_INT_EN = (uint8_t)(0x00000002),         //!< watchdog timeout reset disable
} wdg_interrupt_e;

/** wdg reset state */
typedef enum
{
    WDG_TIMEOUT_RST_DIS = (uint8_t)(0x00000000),        //!< watchdog timeout reset enable
    WDG_TIMEOUT_RST_EN = (uint8_t)(0x00000004),         //!< watchdog timeout reset disable
} wdg_reset_state_e;

/** wdg reset mode */
typedef enum
{
    WDG_RST_NOTHING = (uint8_t)(0x00000000),        //!< reset nothing
    WDG_RST_CPU = (uint8_t)(0x00000001),            //!< reset CPU only
    WDG_RST_SYSTEM = (uint8_t)(0x00000002),         //!< reset all system
} wdg_reset_mode_e;

/** wdg config */
typedef struct
{
    uint32_t reload;                     //!< reload, watchdog counter reload , max counter is 0xFFFFFF
    wdg_reset_state_e resetState;        //!< resetState, see wdg_reset_state_e
    wdg_reset_mode_e resetMode;          //!< resetMode, see wdg_reset_mode_e
    wdg_interrupt_e interruptEn;         //!< interruptEn, see wdg_int_e
} wdg_config_t;

/**
 * @brief  Init Watchdog
 * @param  cfg: see wdg_config_t
 */
void Wdg_Init(const wdg_config_t* cfg);

/**
 * @brief  Deinit Watchdog
 */
void Wdg_Deinit(void);

/**
 * @brief  Watchdog function enable
 */
void Wdg_Enable(void);

/**
 * @brief  Watchdog function disable
 */
void Wdg_Disable(void);

/**
 * @brief  Watchdog interrupt enable
 */
void Wdg_Int_Enable(void);

/**
 * @brief  Watchdog interrupt disable
 */
void Wdg_Int_Disable(void);

/**
 * @brief  Watchdog timeout reset enable
 */
void Wdg_Rst_Enable(void);

/**
 * @brief  Watchdog timeout reset disable
 */
void Wdg_Rst_Disable(void);

/**
 * @brief  Get Watchdog current counter value
 * @retval current counter value, it can be
 *         0x00~0xFFFFFF
 */
u32 Wdg_Get_Current_Value(void);

/**
 * @brief  Set Watchdog reload value
 * @param  reload_value
 *         This parameter can be:
 *         0x00~0xFFFFFF
 */
void Wdg_Set_Reload_Value(u32 reload_value);

/**
 * @brief  Get Watchdog interrupt pending
 * @retval The Watchdog interrupt pending(SET or NOTSET)
 *         SET:pending set    NOTSET:pending is not set
 */
ITStatus Wdg_Get_Int_Pending(void);

/**
 * @brief  Clear Watchdog interrupt pending
 */
void Wdg_Clear_Pending(void);

#ifdef __cplusplus
}
#endif

#endif
