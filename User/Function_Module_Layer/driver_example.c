#include "driver_example.h"
#include "func_hardware_api.h"
#include "uart_driver.h"
#include "wwdg_driver.h"
#include "string.h"

void UART1_Example(void)
{
	static const char tx_data[]={"hello stm32\r\n"};
	static const char tx_data_it[]={"hello stm32 IT\r\n"};
	static const char tx_data_dma[]={"hello stm32 DMA\r\n"};
	static char rx_data[50];
	volatile uint8_t log_data=0;
	UARTx_DMA_Init(&UART1_DMA_Driver);
	UART_Driver_Tx_Data(&UART1_DMA_Driver, (uint8_t* )tx_data, strlen(tx_data), 0xefffffff);
	UART_Driver_Tx_Data_IT(&UART1_DMA_Driver, (uint8_t* )tx_data_it, strlen(tx_data_it));
	while(READ_BIT(UART1_DMA_Driver.Derivant.UART_Config.Instance->SR, USART_SR_TC) != USART_SR_TC){WDG_CLEAR();};	//等待串口空闲
	UART_Driver_Tx_Data_DMA(&UART1_DMA_Driver, (uint8_t* )tx_data_dma, strlen(tx_data_dma));
	while(1)
	{
		WDG_CLEAR();
		// UART_Driver_Rx_Data(&UART1_DMA_Driver, (uint8_t* )rx_data, sizeof(rx_data), 0xefffffff);
		// printf("wait\t %s\r\n",rx_data);
		// memset(rx_data, 0, sizeof(rx_data));
		// HAL_Delay(500);
		UART_Driver_Rx_Data_IT(&UART1_DMA_Driver, (uint8_t* )rx_data, sizeof(rx_data));
		while(1)
		{
			log_data = UART1_DMA_Driver.Derivant.UART_Config.RxXferCount;
			printf("log_data = %d\r\n",log_data);
			log_data = UART_Rx_Process(&UART1_DMA_Driver);
			printf("log_data = %d\r\n",log_data);
			if(log_data == UART_RX_FINISH)
			{
				break;
			}
			WDG_CLEAR();
		}
		printf("state = %d\r\n", UART1_DMA_Driver.Derivant.UART_Config.RxState);
		printf("Rxcount = %d\r\n", UART1_DMA_Driver.Derivant.UART_Config.RxXferCount);
		printf("IT\t %s\r\n",rx_data);
		memset(rx_data, 0, sizeof(rx_data));

		UART_Driver_Rx_Data_DMA(&UART1_DMA_Driver, (uint8_t* )rx_data, sizeof(rx_data));
		while(UART1_DMA_Driver.Derivant.UART_Config.RxState == HAL_UART_STATE_BUSY_RX){WDG_CLEAR();}
		printf("DMA\t %s\r\n",rx_data);
		memset(rx_data, 0, sizeof(rx_data));

	}
}
