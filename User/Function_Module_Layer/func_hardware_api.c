#include "func_hardware_api.h"
#include "uart_driver.h"
#include "string.h"

#define USART1_TX_BUFFER_LEN	100
#define USART1_RX_BUFFER_LEN	100

static uint8_t USART1_TX_Buffer_Area[USART1_TX_BUFFER_LEN];
static uint8_t USART1_RX_Buffer_Area[USART1_RX_BUFFER_LEN];

UART_Driver_t UART1_DMA_Driver={
	.UART_Config.Instance = USART1,
	.UART_Config.Baud_Rate = 921600,

	.UART_DMA_Config.Tx_Stream_Name = Name_DMA2_Stream7,
	.UART_DMA_Config.Tx_Stream = DMA2_Stream7,
	.UART_DMA_Config.Tx_DMA_Stream_Channel = DMA_CHANNEL_4,

	.UART_DMA_Config.Rx_Instance_Name = Name_DMA2_Stream5,
	.UART_DMA_Config.Rx_Stream = DMA2_Stream5,
	.UART_DMA_Config.Rx_DMA_Stream_Channel = DMA_CHANNEL_4,

	.UART_Int_Config.UARTx_IRQ = USART1_IRQn,
	.UART_Int_Config.UARTx_Int_PeremptPriority = 10,
	.UART_Int_Config.UARTx_Int_SubPriority = 0,

	.UART_Int_Config.DMA_Tx_Streamx_IRQ = DMA2_Stream7_IRQn,
	.UART_Int_Config.DMA_Tx_Int_PeremptPriority = 10,
	.UART_Int_Config.DMA_Tx_Int_SubPriority = 1,

	.UART_Int_Config.DMA_Rx_Streamx_IRQ = DMA2_Stream5_IRQn,
	.UART_Int_Config.DMA_RX_Int_PeremptPriority = 10,
	.UART_Int_Config.DMA_Rx_Int_SubPriority = 2,

	.TX_Queue_Config.Buffer_Area = &USART1_TX_Buffer_Area[0],
	.TX_Queue_Config.Buffer_Area_Len =  USART1_TX_BUFFER_LEN,

	.RX_Queue_Config.Buffer_Area = &USART1_RX_Buffer_Area[0],
	.RX_Queue_Config.Buffer_Area_Len = USART1_RX_BUFFER_LEN,
};

void UARTx_DMA_Init(UART_Driver_t *UART_Driver)
{
	UART_Driver_Init(UART_Driver);
	UART_Driver_DMA_Init(UART_Driver);
}

