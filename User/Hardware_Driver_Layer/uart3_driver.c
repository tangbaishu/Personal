#include "uart3_driver.h"
#include "uart_debug_driver.h"
#include "led_driver.h"
#include "stdarg.h"
#include <stdio.h>
#include <string.h>



static UART_HandleTypeDef 		UART3_Handle;
static DMA_HandleTypeDef 		DMA_USART3_TX_Config;
static DMA_HandleTypeDef 		DMA_USART3_RX_Config;

UART3_Driver_Data_t		UART3_Driver_Data;

#define UART3_IDLE_IRQ_EN()			SET_BIT(UART3_Handle.Instance->CR1, USART_CR1_IDLEIE)				//使能UART空闲中断,用于检测UART数据传输结束
#define DMA_UART3_RECEIVE_LEN()		(UART3_Handle.hdmarx->Instance->NDTR) 	//RECEIVE_STACK_BUFF_LEN：设置传输的数据总数、NDTR:剩余要传输的数据项数
#define CLEAR_DMA_UART3_IDLE_IRQ()	__HAL_UART_CLEAR_IDLEFLAG(&UART3_Handle);CLEAR_BIT(UART3_Handle.Instance->CR1, USART_CR1_IDLEIE)

#ifndef FALSE
	#define FALSE 0
#endif

#ifndef TRUE
	#define TRUE  1
#endif

void UART3_Driver_Init(void)
{
	__HAL_RCC_USART3_CLK_ENABLE();
	UART3_Handle.Instance = USART3;
	UART3_Handle.Init.BaudRate = 115200;
	UART3_Handle.Init.Mode = UART_MODE_TX_RX;
	UART3_Handle.Init.OverSampling = UART_OVERSAMPLING_16;
	UART3_Handle.Init.Parity = UART_PARITY_NONE;
	UART3_Handle.Init.StopBits = UART_STOPBITS_1;
	UART3_Handle.Init.WordLength = UART_WORDLENGTH_8B;
	UART3_Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;		// UART硬件流控制

	HAL_UART_Init(&UART3_Handle);
	HAL_NVIC_SetPriority(USART3_IRQn, 9, 0);
	HAL_NVIC_EnableIRQ(USART3_IRQn);
}


/**
 * @brief
 * DMA_USART3_TX  ->DMA1_Stream3	->DMA_CHANNEL_4
 * DMA_USART3_RX  ->DMA1_Stream1	->DMA_CHANNEL_1
 */
void UART3_DMA_Driver_Init(void)
{
	__HAL_RCC_DMA1_CLK_ENABLE();
	UART3_Driver_Init();
	DMA_USART3_TX_Config.Instance = DMA1_Stream3;
	DMA_USART3_TX_Config.Init.Channel = DMA_CHANNEL_4;
	DMA_USART3_TX_Config.Init.Direction = DMA_MEMORY_TO_PERIPH;
	DMA_USART3_TX_Config.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
	DMA_USART3_TX_Config.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	DMA_USART3_TX_Config.Init.MemBurst = DMA_MBURST_SINGLE;
	DMA_USART3_TX_Config.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	DMA_USART3_TX_Config.Init.MemInc = DMA_MINC_ENABLE;
	DMA_USART3_TX_Config.Init.Mode = DMA_NORMAL;
	DMA_USART3_TX_Config.Init.PeriphBurst = DMA_PBURST_SINGLE;
	DMA_USART3_TX_Config.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	DMA_USART3_TX_Config.Init.PeriphInc = DMA_PINC_DISABLE;
	DMA_USART3_TX_Config.Init.Priority = DMA_PRIORITY_HIGH;
	HAL_DMA_Init(&DMA_USART3_TX_Config);
	__HAL_LINKDMA(&UART3_Handle, hdmatx, DMA_USART3_TX_Config);

	DMA_USART3_RX_Config.Instance = DMA1_Stream1;
	DMA_USART3_RX_Config.Init.Channel = DMA_CHANNEL_1;
	DMA_USART3_RX_Config.Init.Direction = DMA_PERIPH_TO_MEMORY;
	DMA_USART3_RX_Config.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
	DMA_USART3_RX_Config.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	DMA_USART3_RX_Config.Init.MemBurst = DMA_MBURST_SINGLE;
	DMA_USART3_RX_Config.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	DMA_USART3_RX_Config.Init.MemInc = DMA_MINC_ENABLE;
	DMA_USART3_RX_Config.Init.Mode = DMA_NORMAL;
	DMA_USART3_RX_Config.Init.PeriphBurst = DMA_PBURST_SINGLE;
	DMA_USART3_RX_Config.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	DMA_USART3_RX_Config.Init.PeriphInc = DMA_PINC_DISABLE;
	DMA_USART3_RX_Config.Init.Priority = DMA_PRIORITY_HIGH;
	HAL_DMA_Init(&DMA_USART3_RX_Config);
	__HAL_LINKDMA(&UART3_Handle, hdmarx, DMA_USART3_RX_Config);

	HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 9, 1);
	HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);

	HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 9, 2);
	HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
	// __HAL_UART_ENABLE_IT(&UART3_Handle, UART_IT_RXNE);	// uart 非空中断
	HAL_UART_DMAStop(&UART3_Handle, 0); // 仅停止RX传输
	HAL_UART_DMAStop(&UART3_Handle, 1); // 仅停止RX传输


}

void UART3_Send_Hex_Data(uint8_t *p_hex_data, uint8 data_len)
{
	HAL_UART_Transmit_DMA(&UART3_Handle, p_hex_data, data_len);
}

void UART3_Receive_Data_Wait(uint8_t *p_buff, uint8_t size, uint32_t timeout)
{
	HAL_UART_Receive(&huart1, p_buff, size, timeout );
	UART3_Driver_Data.Rec_Data_Finish = 1;
}

void USART3_IRQHandler(void)
{

	// if (READ_BIT(UART3_Handle.Instance->SR, USART_SR_RXNE) == USART_SR_RXNE)	// 检测到Rx数据，开启DMA传输
    // {
	// 	UART3_Driver_Data.Receive_Buffer_Area[0] = (uint8)(UART3_Handle.Instance->DR & (uint8)0x00FF);// DR读取数据时, USART_SR_RXNE清零
	// 	UART3_Driver_Data.Valid_Buffer_Data_Len = 1;
    //     UART3_IDLE_IRQ_EN();
	// 	__HAL_UART_DISABLE_IT(&UART3_Handle, UART_IT_RXNE);	// 暂时关闭UART 接收中断，通过DMA接收剩余数据
        HAL_UART_Receive_DMA(&UART3_Handle, &UART3_Driver_Data.Receive_Buffer_Area[1], REC_BUFFER_LEN-1);
    //     // DMA_Sprintf_ISR("USART3_SR_RXNE\r\n");
	// 	UART3_Driver_Data.test_1++;
    // }
	HAL_UART_IRQHandler(&UART3_Handle);
    // if (READ_BIT(UART3_Handle.Instance->SR, USART_SR_IDLE) == USART_SR_IDLE) // 检测到空闲中断，DMA传输完毕
    // {
	// 	CLEAR_BIT(UART3_Handle.Instance->SR, USART_SR_IDLE);
    //     UART3_Driver_Data.Valid_Buffer_Data_Len += DMA_UART3_RECEIVE_LEN();
	// 	if(UART3_Driver_Data.Valid_Buffer_Data_Len >= REC_BUFFER_LEN)
	// 	{
	// 		UART3_Driver_Data.Rec_Buffer_Overflow = TRUE;
	// 	}
    //     CLEAR_DMA_UART3_IDLE_IRQ();
    //     HAL_UART_DMAStop(&UART3_Handle, 0); // 仅停止RX传输
    //     __HAL_UART_ENABLE_IT(&UART3_Handle, UART_IT_RXNE);
	// 	UART3_Driver_Data.Rec_Data_Finish = TRUE;
	// 	// DMA_Sprintf_ISR("USART3_SR_IDLE\r\n");
	// 	UART3_Driver_Data.test_2++;
    // }

	

	
}

void DMA1_Stream3_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&DMA_USART3_TX_Config);
}

// 中断
void DMA1_Stream1_IRQHandler(void)
{
    // UART3_Driver_Data.Valid_Buffer_Data_Len += UART3_Handle.hdmarx->Instance->NDTR;
   	DMA_Sprintf_ISR("USART3_NDTR = %d\r\n", UART3_Handle.hdmarx->Instance->NDTR); // 显示进入DMA_UART_Rx中断使，DMA接收数据剩余数值
    HAL_DMA_IRQHandler(&DMA_USART3_RX_Config);
}
