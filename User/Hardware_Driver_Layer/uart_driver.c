#include "uart_driver.h"

DMA_HandleTypeDef DMA_Uart_TX_Config;

void UART_Driver_Init(UART_Driver_t *driver_config)
{
	driver_config->Derivant.UART_Config.Instance = driver_config->UART_Config.Instance;
	driver_config->Derivant.UART_Config.Init.BaudRate = driver_config->UART_Config.Baud_Rate;
	if( (driver_config->UART_Config.Instance != UART4) || (driver_config->UART_Config.Instance != UART5) )
	{
		driver_config->Derivant.UART_Config.Init.HwFlowCtl = UART_HWCONTROL_NONE;	// USART外设启动硬件流控制
	}
	driver_config->Derivant.UART_Config.Init.Mode = UART_MODE_TX_RX;
	driver_config->Derivant.UART_Config.Init.OverSampling = UART_OVERSAMPLING_16;
	driver_config->Derivant.UART_Config.Init.Parity = UART_PARITY_NONE;
	driver_config->Derivant.UART_Config.Init.StopBits = UART_STOPBITS_1;
	driver_config->Derivant.UART_Config.Init.WordLength = UART_WORDLENGTH_8B;
	HAL_UART_Init(&driver_config->Derivant.UART_Config);
	HAL_NVIC_SetPriority(driver_config->UART_Int_Config.UARTx_IRQ,\
			 driver_config->UART_Int_Config.UARTx_Int_PeremptPriority, driver_config->UART_Int_Config.UARTx_Int_SubPriority);
	HAL_NVIC_EnableIRQ(driver_config->UART_Int_Config.UARTx_IRQ);
}

void UART_Driver_DMA_Init(UART_Driver_t *driver_config)
{
	if( (driver_config->UART_DMA_Config.Tx_Stream_Name < Name_DMA2_Stream0) ||\
		 (driver_config->UART_DMA_Config.Rx_Instance_Name < Name_DMA2_Stream0) )
	{
		__HAL_RCC_DMA1_CLK_ENABLE();
	}

	if( (driver_config->UART_DMA_Config.Tx_Stream_Name >= Name_DMA2_Stream0) ||\
		 (driver_config->UART_DMA_Config.Rx_Instance_Name >= Name_DMA2_Stream0) )
	{
		__HAL_RCC_DMA2_CLK_ENABLE();
	}
	driver_config->Derivant.UART_Tx_DMA_Config.Instance = driver_config->UART_DMA_Config.Tx_Stream;					// 设置DMA外设寄存器基地址
	driver_config->Derivant.UART_Tx_DMA_Config.Init.Channel = driver_config->UART_DMA_Config.Tx_DMA_Stream_Channel;	// 设置UART_DMA 发送数据流通道
	driver_config->Derivant.UART_Tx_DMA_Config.Init.Direction = DMA_MEMORY_TO_PERIPH;								// 数据传输方向： 存储器到外设
	driver_config->Derivant.UART_Tx_DMA_Config.Init.FIFOMode = DMA_FIFOMODE_DISABLE;								// 禁用FIFI 缓存模式
	driver_config->Derivant.UART_Tx_DMA_Config.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;						// FIFI 缓存阈值配置： 满
	driver_config->Derivant.UART_Tx_DMA_Config.Init.MemBurst = DMA_MBURST_SINGLE;									// DMA存储器突发传输配置：单次突发
	driver_config->Derivant.UART_Tx_DMA_Config.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;							// 存储器数据对齐方式：字节对齐
	driver_config->Derivant.UART_Tx_DMA_Config.Init.MemInc = DMA_MINC_ENABLE;										// 存储器地址递增使能：使能
	driver_config->Derivant.UART_Tx_DMA_Config.Init.Mode = DMA_NORMAL;												// DMA工作模式： 正常模式
	driver_config->Derivant.UART_Tx_DMA_Config.Init.PeriphBurst = DMA_PBURST_SINGLE;								// DMA外设突发传输配置: 单次突发
	driver_config->Derivant.UART_Tx_DMA_Config.Init.PeriphDataAlignment =DMA_PDATAALIGN_BYTE;						// 外设数据对齐方式：字节对齐
	driver_config->Derivant.UART_Tx_DMA_Config.Init.PeriphInc = DMA_PINC_DISABLE;									// 外设地址递增使能：禁用
	driver_config->Derivant.UART_Tx_DMA_Config.Init.Priority = DMA_PRIORITY_HIGH;									// DMA传输优先级：高
	HAL_DMA_Init(&driver_config->Derivant.UART_Tx_DMA_Config);														// DMA外设初始化
	__HAL_LINKDMA(&driver_config->Derivant.UART_Config, hdmatx, driver_config->Derivant.UART_Tx_DMA_Config);								// UART外设与DMA外设链接
	HAL_NVIC_SetPriority(driver_config->UART_Int_Config.DMA_Tx_Streamx_IRQ,\
			 driver_config->UART_Int_Config.DMA_Tx_Int_PeremptPriority, driver_config->UART_Int_Config.DMA_Tx_Int_SubPriority);
	HAL_NVIC_EnableIRQ(driver_config->UART_Int_Config.DMA_Tx_Streamx_IRQ);

	driver_config->Derivant.UART_Rx_DMA_Config.Instance = driver_config->UART_DMA_Config.Rx_Stream;					// 设置DMA外设寄存器基地址
	driver_config->Derivant.UART_Rx_DMA_Config.Init.Channel = driver_config->UART_DMA_Config.Rx_DMA_Stream_Channel;	// 设置UART_DMA 发送数据流通道
	driver_config->Derivant.UART_Rx_DMA_Config.Init.Direction = DMA_PERIPH_TO_MEMORY;								// 数据传输方向： 存储器到外设
	driver_config->Derivant.UART_Rx_DMA_Config.Init.FIFOMode = DMA_FIFOMODE_DISABLE;								// 禁用FIFI 缓存模式
	driver_config->Derivant.UART_Rx_DMA_Config.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;						// FIFI 缓存阈值配置： 满
	driver_config->Derivant.UART_Rx_DMA_Config.Init.MemBurst = DMA_MBURST_SINGLE;									// DMA存储器突发传输配置：单次突发
	driver_config->Derivant.UART_Rx_DMA_Config.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;							// 存储器数据对齐方式：字节对齐
	driver_config->Derivant.UART_Rx_DMA_Config.Init.MemInc = DMA_MINC_ENABLE;										// 存储器地址递增使能：使能
	driver_config->Derivant.UART_Rx_DMA_Config.Init.Mode = DMA_NORMAL;												// DMA工作模式： 正常模式
	driver_config->Derivant.UART_Rx_DMA_Config.Init.PeriphBurst = DMA_PBURST_SINGLE;								// DMA外设突发传输配置: 单次突发
	driver_config->Derivant.UART_Rx_DMA_Config.Init.PeriphDataAlignment =DMA_PDATAALIGN_BYTE;						// 外设数据对齐方式：字节对齐
	driver_config->Derivant.UART_Rx_DMA_Config.Init.PeriphInc = DMA_PINC_DISABLE;									// 外设地址递增使能：禁用
	driver_config->Derivant.UART_Rx_DMA_Config.Init.Priority = DMA_PRIORITY_HIGH;									// DMA传输优先级：高
	HAL_DMA_Init(&driver_config->Derivant.UART_Rx_DMA_Config);														// DMA外设初始化
	__HAL_LINKDMA(&driver_config->Derivant.UART_Config, hdmarx, driver_config->Derivant.UART_Rx_DMA_Config);								// UART外设与DMA外设链接
	HAL_NVIC_SetPriority(driver_config->UART_Int_Config.DMA_Rx_Streamx_IRQ,\
			 driver_config->UART_Int_Config.DMA_RX_Int_PeremptPriority, driver_config->UART_Int_Config.DMA_Rx_Int_SubPriority);
	HAL_NVIC_EnableIRQ(driver_config->UART_Int_Config.DMA_Rx_Streamx_IRQ);
}

UART_State_e UART_Rx_Process(UART_Driver_t* handle)
{
	if(handle->Derivant.UART_Config.RxXferCount)
	{
		return UART_RX_BUSY;
	}
	return UART_RX_FINISH;
}

/**
 * @brief 阻塞式发送数据
 * @param handle 	对应UART外设驱动句柄
 * @param tx_data 	待发送数据首地址
 * @param data_len 	发送数据长度
 * @param wait_time 等待超时时间
 */
void UART_Driver_Tx_Data(UART_Driver_t* handle, uint8_t* tx_data, uint8_t data_len, uint32_t wait_time)
{
	HAL_UART_Transmit(&handle->Derivant.UART_Config, tx_data, data_len, wait_time);
}

/**
 * @brief 阻塞式接收数据
 * @param handle 	对应UART外设驱动句柄
 * @param rx_data 	待存储接收数据首地址
 * @param data_len 	接收数据长度
 * @param wait_time 等待超时时间
 */
void UART_Driver_Rx_Data(UART_Driver_t* handle, uint8_t* rx_data, uint8_t data_len, uint32_t wait_time)
{
	HAL_UART_Receive(&handle->Derivant.UART_Config, rx_data, data_len, wait_time);
}

/**
 * @brief 非阻塞式接收数据
 * @param handle 	对应UART外设驱动句柄
 * @param tx_data 	待发送数据首地址
 * @param data_len 	发送数据长度
 */
void UART_Driver_Tx_Data_IT(UART_Driver_t* handle, uint8_t *tx_data, uint8_t data_len)
{
	HAL_UART_Transmit_IT(&handle->Derivant.UART_Config, tx_data, data_len);
}

/**
 * @brief 非阻塞式接收数据
 * @param handle 	对应UART外设驱动句柄
 * @param rx_data 	待存储接收数据首地址
 * @param data_len 	接收数据长度
 */
void UART_Driver_Rx_Data_IT(UART_Driver_t* handle, uint8_t *rx_data, uint8_t data_len)
{
	printf("return %d\r\n", (uint8_t)HAL_UART_Receive_IT(&handle->Derivant.UART_Config, rx_data, data_len));
}

/**
 * @brief DMA通道自动完成数据发送
 * @param handle 	对应UART外设驱动句柄
 * @param tx_data 	待发送数据首地址
 * @param data_len 	发送数据长度
 */
void UART_Driver_Tx_Data_DMA(UART_Driver_t* handle, uint8_t *tx_data, uint8_t data_len)
{
	HAL_UART_Transmit_DMA(&handle->Derivant.UART_Config, tx_data, data_len);
}

/**
 * @brief DMA通道自动完成数据发送
 * @param handle 	对应UART外设驱动句柄
 * @param rx_data 	待存储接收数据首地址
 * @param data_len 	接收数据长度
 */
void UART_Driver_Rx_Data_DMA(UART_Driver_t* handle, uint8_t *rx_data, uint8_t data_len)
{
	HAL_UART_Receive_DMA(&handle->Derivant.UART_Config, rx_data, data_len);
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
	while (READ_BIT(PRINTF_DRIVER->SR, USART_SR_TC) != USART_SR_TC){};
	PRINTF_DRIVER->DR = (uint8_t)ch;
	// HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xff);
	// HAL_UART_Transmit_IT(&huart1, (uint8_t *)&ch, 1);
	return ch;
} // end int fputc(int _Ch,FILE *_File)

