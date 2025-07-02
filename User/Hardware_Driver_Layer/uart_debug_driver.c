#include "uart_debug_driver.h"
#include "led_driver.h"
#include "base_function.h"
#include "wwdg_driver.h"
#include "stdarg.h"
#include <string.h>


typedef struct
{
	uint8 data[DEBUG_BUFF_LEN]; // 栈数据
	int top;					// 栈顶地址
} Debug_Buff_t;					// 影子存储栈 结构体
static Debug_Buff_t Debug_Buff;
static Debug_Buff_t Shadow_Debug_Buff;

SYS_Uart_Rec_Queue_t SYS_UartDMA_Rx_Queue;

/*UART外设初始化变量*/
UART_HandleTypeDef huart1;
DMA_HandleTypeDef DMA_Uart_TX_Config;
DMA_HandleTypeDef DMA_Uart_RX_Config;

#define UART_IDLE_IRQ_EN()			SET_BIT(huart1.Instance->CR1, USART_CR1_IDLEIE)				//使能UART空闲中断,用于检测UART数据传输结束
#define DMA_UART_RECEIVE_LEN()		(RECEIVE_STACK_BUFF_LEN - huart1.hdmarx->Instance->NDTR) 	//RECEIVE_STACK_BUFF_LEN：设置传输的数据总数、NDTR:剩余要传输的数据项数
#define CLEAR_DMA_UART_IDLE_IRQ()	__HAL_UART_CLEAR_IDLEFLAG(&huart1);CLEAR_BIT(huart1.Instance->CR1, USART_CR1_IDLEIE)

uint8 Init_Finish_Flag=0;

/**
 * @brief 通过DMA 完成uart_tx数据发送
 * 注：在发送过程中未进行 喂狗操作，需要警惕大量数据打印 导致WWDG溢出
 *
 * @param format
 * @param ...
 */
void DMA_Sprintf(const char *format, ...)
{
	static int format_len = 0;
	va_list args;
	
	do
	{
		if (Debug_Buff.top < VALID_DEBUG_BUFF_LEN)
		{
			va_start(args, format);
			format_len = vsnprintf((char *)&(Debug_Buff.data[Debug_Buff.top]), DEBUG_BUFF_LEN - Debug_Buff.top + 1, (char *)format, args);
			Debug_Buff.top += format_len;
			va_end(args);
		}

		if (Debug_Buff.top >= VALID_DEBUG_BUFF_LEN)
		{
			if (READ_BIT(huart1.Instance->SR, USART_SR_TC) == USART_SR_TC)
			{
				memcpy(Shadow_Debug_Buff.data, Debug_Buff.data, Debug_Buff.top);
				Shadow_Debug_Buff.top = Debug_Buff.top;
				Debug_Buff.top = 0;
				HAL_UART_Transmit_DMA(&huart1, Shadow_Debug_Buff.data, Shadow_Debug_Buff.top);
				return;
			}
		}
	} while (Debug_Buff.top >= VALID_DEBUG_BUFF_LEN); // vsnprintf() 返回传入参数地址溢出
	if (READ_BIT(huart1.Instance->SR, USART_SR_TC) == USART_SR_TC)
	{
		memcpy(Shadow_Debug_Buff.data, Debug_Buff.data, Debug_Buff.top);
		Shadow_Debug_Buff.top = Debug_Buff.top;
		Debug_Buff.top = 0;
		HAL_UART_Transmit_DMA(&huart1, Shadow_Debug_Buff.data, Shadow_Debug_Buff.top);
	}
}

/**
 * @brief 
 * 
 * @param format 
 * @param ... 
 */
void DMA_Sprintf_ISR(const char *format, ...)
{
	static int format_len = 0;
	va_list args;
	do
	{
		if (Debug_Buff.top < VALID_DEBUG_BUFF_LEN)
		{
			va_start(args, format);
			format_len = vsnprintf((char *)&(Debug_Buff.data[Debug_Buff.top]), DEBUG_BUFF_LEN - Debug_Buff.top + 1, (char *)format, args);
			Debug_Buff.top += format_len;
			va_end(args);
		}

		if (Debug_Buff.top >= VALID_DEBUG_BUFF_LEN)
		{
			if (READ_BIT(huart1.Instance->SR, USART_SR_TC) == USART_SR_TC)
			{
				memcpy(Shadow_Debug_Buff.data, Debug_Buff.data, Debug_Buff.top);
				Shadow_Debug_Buff.top = Debug_Buff.top;
				Debug_Buff.top = 0;
				HAL_UART_Transmit_DMA(&huart1, Shadow_Debug_Buff.data, Shadow_Debug_Buff.top);
				return;
			}
		}
	} while (Debug_Buff.top >= VALID_DEBUG_BUFF_LEN); // vsnprintf() 返回传入参数地址溢出

	while(READ_BIT(huart1.Instance->SR, USART_SR_TC) != USART_SR_TC){WDG_CLEAR();};	//等待串口空闲
	memcpy(Shadow_Debug_Buff.data, Debug_Buff.data, Debug_Buff.top);
	Shadow_Debug_Buff.top = Debug_Buff.top;
	Debug_Buff.top = 0;
	HAL_UART_Transmit_DMA(&huart1, Shadow_Debug_Buff.data, Shadow_Debug_Buff.top);
	while(READ_BIT(huart1.Instance->SR, USART_SR_TC) != USART_SR_TC){WDG_CLEAR();};	//等待串口发送完成
}

/**
 * @brief 打印字符串数据
 * 
 * @param string_data 
 * @param string_len 
 */
void DMA_Sprintf_String(const uint8 *string_data, uint16 string_len)
{
	while(READ_BIT(huart1.Instance->SR, USART_SR_TC) != USART_SR_TC){WDG_CLEAR();};	//等待串口空闲
	HAL_UART_Transmit_DMA(&huart1, string_data, string_len);
	while(READ_BIT(huart1.Instance->SR, USART_SR_TC) != USART_SR_TC){WDG_CLEAR();};	//等待串口发送完成
}

/**
 * 检查 DMA_Debug_Buff内的数据是否传输完毕
 * 若未传输完毕，在UART_DMATransmitCplt()函数内再次开启DMA传输
 */
void DMA_UART1_Buff_ckeck(void)
{
	if (Debug_Buff.top != 0 && READ_BIT(huart1.Instance->SR, USART_SR_TC) == USART_SR_TC)
	{
		memcpy(Shadow_Debug_Buff.data, Debug_Buff.data, Debug_Buff.top);
		Shadow_Debug_Buff.top = Debug_Buff.top;
		Debug_Buff.top = 0;
		HAL_UART_Transmit_DMA(&huart1, Shadow_Debug_Buff.data, Shadow_Debug_Buff.top);
	}
}

/**
 * @brief printf() 重定向函数
 * 用于立即且实时打印函数
 * 若要在中断内使用，需确保调用的 UART外设中断优先级 高于 打印中断优先级
 * 注：STM32 mcu 数字越小，中断优先级越高
 * @param ch
 * @param _File
 * @return int
 */
int fputc(int ch, FILE *_File)
{
	// 等待UART外设完成数据发送
	while (READ_BIT(huart1.Instance->SR, USART_SR_TC) != USART_SR_TC){};
	huart1.Instance->DR = (uint8_t)ch;
	// HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xff);
	// HAL_UART_Transmit_IT(&huart1, (uint8_t *)&ch, 1);
	return ch;
} // end int fputc(int _Ch,FILE *_File)
/* USART1 init function */

static void UART1_Driver_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 921600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
	HAL_NVIC_SetPriority(USART1_IRQn, 7, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE END USART1_Init 2 */
}

/**
 * @brief
 * DMA_USART1_TX  ->DMA2_Stream7	->DMA_CHANNEL_4
 * DMA_USART1_RX  ->DMA2_Stream5	->DMA_CHANNEL_4
 */
void DMA_UART_Debug_Driver_Init(void)
{
	if(Init_Finish_Flag)
	{
		return;
	}
	UART1_Driver_Init();
	__HAL_RCC_DMA2_CLK_ENABLE();

	DMA_Uart_TX_Config.Instance = DMA2_Stream7;
	DMA_Uart_TX_Config.Init.Channel = DMA_CHANNEL_4;
	DMA_Uart_TX_Config.Init.Direction = DMA_MEMORY_TO_PERIPH;
	DMA_Uart_TX_Config.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
	DMA_Uart_TX_Config.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	DMA_Uart_TX_Config.Init.MemBurst = DMA_MBURST_SINGLE;
	DMA_Uart_TX_Config.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	DMA_Uart_TX_Config.Init.MemInc = DMA_MINC_ENABLE;
	DMA_Uart_TX_Config.Init.Mode = DMA_NORMAL;
	DMA_Uart_TX_Config.Init.PeriphBurst = DMA_PBURST_SINGLE;
	DMA_Uart_TX_Config.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	DMA_Uart_TX_Config.Init.PeriphInc = DMA_PINC_DISABLE;
	DMA_Uart_TX_Config.Init.Priority = DMA_PRIORITY_HIGH;
	HAL_DMA_Init(&DMA_Uart_TX_Config);
	__HAL_LINKDMA(&huart1, hdmatx, DMA_Uart_TX_Config);
	HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 10, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);

	DMA_Uart_RX_Config.Instance = DMA2_Stream5;
	DMA_Uart_RX_Config.Init.Channel = DMA_CHANNEL_4;
	DMA_Uart_RX_Config.Init.Direction = DMA_PERIPH_TO_MEMORY;
	DMA_Uart_RX_Config.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
	DMA_Uart_RX_Config.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	DMA_Uart_RX_Config.Init.MemBurst = DMA_MBURST_SINGLE;
	DMA_Uart_RX_Config.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	DMA_Uart_RX_Config.Init.MemInc = DMA_MINC_ENABLE;
	DMA_Uart_RX_Config.Init.Mode = DMA_NORMAL;
	DMA_Uart_RX_Config.Init.PeriphBurst = DMA_PBURST_SINGLE;
	DMA_Uart_RX_Config.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	DMA_Uart_RX_Config.Init.PeriphInc = DMA_PINC_DISABLE;
	DMA_Uart_RX_Config.Init.Priority = DMA_PRIORITY_HIGH;
	HAL_DMA_Init(&DMA_Uart_RX_Config);
	__HAL_LINKDMA(&huart1, hdmarx, DMA_Uart_RX_Config);
	HAL_NVIC_SetPriority(DMA2_Stream5_IRQn, 10, 1);
	HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);
	Data_Buff_Queue_Init( &SYS_UartDMA_Rx_Queue.Data_Buff, RECEIVE_STACK_BUFF_LEN);
	// SET_BIT(huart1.Instance->CR1, USART_CR1_WAKE | USART_CR1_RE);
	// __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
	Init_Finish_Flag = 1;
} // end void DMA_UART_Debug_Driver_Init(void)

void USART1_IRQHandler(void)
{
    if (READ_BIT(huart1.Instance->SR, USART_SR_RXNE) == USART_SR_RXNE)	// 检测到Rx数据，开启DMA传输
    {
        SYS_UartDMA_Rx_Queue.Data_Buff.Data[0] = (uint8)(huart1.Instance->DR & (uint8)0x00FF);
        SYS_UartDMA_Rx_Queue.Data_Buff.Top = 1;
        SYS_UartDMA_Rx_Queue.Rec_Start_Flag = 1;
        UART_IDLE_IRQ_EN();
        HAL_UART_Receive_DMA(&huart1, &SYS_UartDMA_Rx_Queue.Data_Buff.Data[1], RECEIVE_STACK_BUFF_LEN);
        // DMA_Sprintf_ISR("USART_SR_RXNE\r\n");
    }
	LED_Driver_Turn(0);
    HAL_UART_IRQHandler(&huart1);
    if (READ_BIT(huart1.Instance->SR, USART_SR_IDLE) == USART_SR_IDLE) // 检测到空闲中断，DMA传输完毕
    {
        SYS_UartDMA_Rx_Queue.Data_Buff.Top += DMA_UART_RECEIVE_LEN();

        CLEAR_DMA_UART_IDLE_IRQ();
        HAL_UART_DMAStop(&huart1, 0); // 仅停止RX传输
        __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
        SYS_UartDMA_Rx_Queue.Rec_Finish_Flag = 1;
        SYS_UartDMA_Rx_Queue.Rec_Start_Flag = 0;
        // DMA_Sprintf_ISR("USART_SR_IDLE\r\n");
    }
}

void DMA2_Stream7_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&DMA_Uart_TX_Config);
}

// 中断
void DMA2_Stream5_IRQHandler(void)
{
    SYS_UartDMA_Rx_Queue.Data_Buff.Top = huart1.hdmarx->Instance->NDTR;
//    DMA_Sprintf_ISR("NDTR = %d\r\n", SYS_UartDMA_Rx_Queue.Data_Buff.Top); // 显示进入DMA_UART_Rx中断使，DMA接收数据剩余数值
    HAL_DMA_IRQHandler(&DMA_Uart_RX_Config);
}
