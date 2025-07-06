/****************************************************************************
 * @copyright Copyright(C) 2020-2023 Ismartware Limited. All rights reserved.
 * @file serial.c
 * @brief Function implement for printf
 * @author David
 ****************************************************************************/
#include "serial.h"
#include "zr_rcc.h"
#include "zr_gpio.h"
#include "zr_uart.h"

/**
 * @brief  Initializes the serial for printf
 * @param  baudrate: UART baudrate
 *         This parameter can be:
 *         115200
 *         38400
 *         ....
 */
void Serail_Init(u32 baudrate)
{
    /* uart pinmux */
    Rcc_Module_Clk_Enable(GPIO_ID);
    Gpio_Function_Select(GPIO3, GPIO3_FUNC_SEL_UART_TX); /* tx */
//    Gpio_Function_Select(GPIO1, FUNCTION_SEL1); /* rx */

    /* clk,reset */
    Rcc_Module_Rst(UART_ID);
    Rcc_Module_Clk_Enable(UART_ID);
    Rcc_Module_Rst_Release(UART_ID);

    Uart_Baudrate_Cfg(baudrate);
    Uart_TX_Enable();
//    Uart_RX_Enable();
    Rcc_Module_Clk_Disable(GPIO_ID);
}

/**
 * @brief  Printf function implement
 */
#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)

#else

#pragma import(__use_no_semihosting)
struct __FILE
{
    int handle;
};
#endif

FILE __stdout;
int fputc(int ch, FILE* f)
{
    (void)f;
    Uart_TX_Byte((uint8_t)ch);
    return ch;
}
