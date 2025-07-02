#ifndef _FUNC_HARDWARE_API_H_
#define _FUNC_HARDWARE_API_H_

#include "stm32f4xx.h"
#include "uart_driver.h"

extern UART_Driver_t UART1_DMA_Driver;

void UARTx_DMA_Init(UART_Driver_t *UART_Driver);

#endif
