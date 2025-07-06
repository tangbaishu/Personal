/****************************************************************************
 * @copyright Copyright(C) 2020-2023 Ismartware Limited. All rights reserved.
 * @file zr_uart.h
 * @brief Functions interfaces for the UART firmware library
 * @author David
 ****************************************************************************/
#ifndef ZR_UART_H
#define ZR_UART_H

#include "system.h"
#include "sys_clock.h"

#ifdef __cplusplus
extern "C" {
#endif

/* uart status */
#define UART_TX_BUF_FULL    (0x00000001)
#define UART_RX_BUF_FULL    (0x00000002)
#define UART_TX_BUF_OVERRUN (0x00000004)
#define UART_RX_BUF_OVERRUN (0x00000008)

/* uart ctrl */
#define UART_TX_EN             (0x00000001)
#define UART_RX_EN             (0x00000002)
#define UART_TX_FINISH_INT_EN  (0x00000004)
#define UART_RX_FINISH_INT_EN  (0x00000008)
#define UART_TX_OVERRUN_INT_EN (0x00000010)
#define UART_RX_OVERRUN_INT_EN (0x00000020)

/* interrupt status */
#define UART_TX_FINISH_INT_PENDING  (0x00000001)
#define UART_RX_FINISH_INT_PENDING  (0x00000002)
#define UART_TX_OVERRUN_INT_PENDING (0x00000004)
#define UART_RX_OVERRUN_INT_PENDING (0x00000008)

/** uart mode*/
typedef enum
{
    MODE_TX = (uint8_t)(0x00000001),          //!< tx mode
    MODE_RX = (uint8_t)(0x00000002),          //!< rx mode
    MODE_TXRX = (uint8_t)(0x00000003),        //!< tx and rx mode;
} uart_mode_e;

/** uart config */
typedef struct
{
    uint32_t baud;           //!< baud
    uart_mode_e mode;        //!< mode, see uart_mode_e
} uart_config_t;

/**
 * @brief  Init UART
 * @param  cfg: see uart_config_t
 */
void Uart_Init(const uart_config_t* cfg);

/**
 * @brief  Deinit UART
 */
void Uart_Deinit(void);

/**
 * @brief  UART baudrate configure for NON-SLEEP or Normal sleep
 * @param  baudrate: UART baudrate
 *         This parameter can be:
 *         115200
 *         38400
 *         ....
 */
void Uart_Baudrate_Cfg(u32 baudrate);

/**
 * @brief  UART baudrate configure for deep sleep
 * it shall change clock source when go to deep sleep when uart as the wakeup source, and shall turn back to clock source
 * too when in normal run or normal sleep
 * @param  baudrate: UART baudrate
 *         This parameter can be:
 *         115200
 *         38400
 *         ....
 */
void Uart_Baudrate_Cfg_On_Deep_Sleep(u32 baudrate);

/**
 * @brief  UART mode select
 * @param  mode: see uart_mode_e
 */
void Uart_Mode_Cfg(uart_mode_e mode);

/**
 * @brief  UART tx function enable
 */
void Uart_TX_Enable(void);

/**
 * @brief  UART tx function disable
 */
void Uart_TX_Disable(void);

/**
 * @brief  UART rx function enable
 */
void Uart_RX_Enable(void);

/**
 * @brief  UART rx function disable
 */
void Uart_RX_Disable(void);

/**
 * @brief  UART tx a data function, this function will wait all the time until the tx buffer is not full before sending
 * a data
 * @param  data: UART tx data
 */
void Uart_TX_Byte(u8 data);

/**
 * @brief  UART rx a data function, this function will wait all the time until the rx buffer is full before receive a
 * data
 * @retval UART rx data
 */
u8 Uart_RX_Byte(void);

/**
 * @brief  UART tx data function,  this function will wait all the time until the tx buffer is not full before sending
 * each data
 * @param  data: UART tx data
 * @param  n: number of tx data
 */
void Uart_TX_Bytes(u8* data, u32 n);

/**
 * @brief  UART rx data function, this function will wait all the time until the rx buffer is full before receive each
 * data
 * @param  data: UART rx data
 * @param  n: number of rx data
 */
void Uart_RX_Bytes(u8* data, u32 n);

/**
 * @brief  UART interrupt enable
 * @param  interrupt: UART interrupt
 *         This parameter can be any combination of the following values:
 *         UART_TX_FINISH_INT_EN
 *         UART_RX_FINISH_INT_EN
 *         UART_TX_OVERRUN_INT_EN
 *         UART_RX_OVERRUN_INT_EN
 */
void Uart_Interrupt_Enable(u32 interrupt);

/**
 * @brief  UART interrupt disable
 * @param  interrupt: UART interrupt
 *         This parameter can be any combination of the following values:
 *         UART_TX_FINISH_INT_EN
 *         UART_RX_FINISH_INT_EN
 *         UART_TX_OVERRUN_INT_EN
 *         UART_RX_OVERRUN_INT_EN
 */
void Uart_Interrupt_Disable(u32 interrupt);

/**
 * @brief  Get UART interrupt pending
 * @param  int_pending: UART interrupt pending bit
 *         This parameter can be:
 *         UART_TX_FINISH_INT_PENDING
 *         UART_RX_FINISH_INT_PENDING
 *         UART_TX_OVERRUN_INT_PENDING
 *         UART_RX_OVERRUN_INT_PENDING
 * @retval The UART interrupt pending(SET or NOTSET)
 *         SET:pending set    NOTSET:pending is not set
 */
ITStatus Uart_Get_Int_Pending(u32 int_pending);

/**
 * @brief  Clear UART interrupt pending
 * @param  int_pending: UART interrupt pending bit
 *         This parameter can be:
 *         UART_TX_FINISH_INT_PENDING
 *         UART_RX_FINISH_INT_PENDING
 *         UART_TX_OVERRUN_INT_PENDING
 *         UART_RX_OVERRUN_INT_PENDING
 */
void Uart_Clear_Pending(u32 int_pending);

/**
 * @brief  Get UART status set or not set flag
 * @param  status: UART status
 *         This parameter can be:
 *         UART_TX_BUF_FULL
 *         UART_RX_BUF_FULL
 *         UART_TX_BUF_OVERRUN
 *         UART_RX_BUF_OVERRUN
 * @retval The UART status (SET or NOTSET)
 */
FlagStatus Uart_Get_Status(u32 status);

/**
 * @brief  Clear UART status
 * @param  status: UART status
 *         This parameter can be:
 *         UART_TX_BUF_OVERRUN
 *         UART_RX_BUF_OVERRUN
 */
void Uart_Clear_Status(u32 status);

#ifdef __cplusplus
}
#endif

#endif
