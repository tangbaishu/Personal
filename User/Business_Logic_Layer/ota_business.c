#include "OTA_Business.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_crc.h"
#include "uart_debug_driver.h"

Data_Transfer_t Data_Transfer;

void OTA_Business_Init(void)
{
//	Data_Transfer.Uart_Init = DMA_UART_Debug_Driver_Init;
//	Data_Transfer.Uart_Init();
}
