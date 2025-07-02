/**
 * @file uart_driver.h
 * @author tanglibo
 * @brief 
 * 通过如下文件组合实现UART驱动功能
 * uart_msp_init 文件 声明 UART GPIO引脚映射关系和软硬件绑定
 * 
 * V0.1：实现基础的UART接收、发送功能
 * @version 0.1
 * @date 2025-01-05
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef _UART_DRIVER_H_
#define _UART_DRIVER_H_

#include "stm32f4xx_hal.h"
#include "my_base_function.h"
#include <stdio.h>

#define PRINTF_DRIVER	USART1	// 串口 printf 打印驱动外设基地址

typedef enum
{
	Name_DMA1_Stream1 = 1,
	Name_DMA1_Stream2,
	Name_DMA1_Stream3,
	Name_DMA1_Stream4,
	Name_DMA1_Stream5,
	Name_DMA1_Stream6,
	Name_DMA1_Stream7,
	Name_DMA2_Stream0 = 8,
	Name_DMA2_Stream1,
	Name_DMA2_Stream2,
	Name_DMA2_Stream3,
	Name_DMA2_Stream4,
	Name_DMA2_Stream5,
	Name_DMA2_Stream6,
	Name_DMA2_Stream7,
}DMA_Stream_Name_e;	//	UART_DMA_Tx_Stream_Name_e

typedef enum
{
	UART_RX_BUSY,	
	UART_TX_BUSY,
	UART_RX_FINISH,
	UART_TX_FINISH,
}UART_State_e;


typedef struct
{
	DMA_Stream_Name_e				Tx_Stream_Name;
	DMA_Stream_TypeDef*         	Tx_Stream;    	// Register base address
	uint32_t 						Tx_DMA_Stream_Channel;
	DMA_Stream_Name_e				Rx_Instance_Name;
	DMA_Stream_TypeDef*         	Rx_Stream;    	// Register base address
	uint32_t 						Rx_DMA_Stream_Channel;
}UART_DMA_Config_t;

typedef struct
{
	USART_TypeDef 					*Instance;			// USART 对应实际外设寄存器基地址 USART1~USART3、UART4、UART5、USART6
	uint32_t						Baud_Rate;			// UART	波特率		
}UART_Config_t;

typedef struct
{
	IRQn_Type					UARTx_IRQ;					// UART 外设中断号 @ IRQn_Type
	uint8_t						UARTx_Int_PeremptPriority;	// UART 抢占优先级
	uint8_t						UARTx_Int_SubPriority;		// UART 子优先级

	IRQn_Type					DMA_Tx_Streamx_IRQ;			// DMA发送 数据流中断号	
	uint8_t						DMA_Tx_Int_PeremptPriority;	// DMA发送 中断抢占优先级
	uint8_t						DMA_Tx_Int_SubPriority;		// DMA发送 子优先级

	IRQn_Type					DMA_Rx_Streamx_IRQ;			// DMA接收 数据流中断号
	uint8_t						DMA_RX_Int_PeremptPriority;	// DMA接收 中断抢占优先级
	uint8_t						DMA_Rx_Int_SubPriority;		// DMA接收 子优先级
}UART_Interrupt_Config_t;

typedef struct
{
	UART_HandleTypeDef			UART_Config;				// UART 配置后 衍生数据,已确保HAL底层驱动运行
	DMA_HandleTypeDef			UART_Tx_DMA_Config;			// UART_Tx_DMA 配置后 衍生数据,已确保HAL底层驱动运行
	DMA_HandleTypeDef			UART_Rx_DMA_Config;			// UART_Rx_DMA 配置后 衍生数据,已确保HAL底层驱动运行
}Derivant_t;					// 衍生物



typedef struct
{
	UART_Config_t					UART_Config;
	UART_DMA_Config_t				UART_DMA_Config;
	UART_Interrupt_Config_t			UART_Int_Config;
	struct Byte_Buffer_Queue_t		TX_Queue_Config;	// 发送数据缓存区，需要提供数据缓存空间
	struct Byte_Buffer_Queue_t		RX_Queue_Config;	// 接收数据缓存区，需要提供数据缓存空间
	Derivant_t						Derivant;			// 衍生物，经过初始化配置后参数的数据
}UART_Driver_t;											

void UART_Driver_Init(UART_Driver_t *driver_config);

void UART_Driver_DMA_Init(UART_Driver_t *driver_config);

UART_State_e UART_Rx_Process(UART_Driver_t* handle);

/**
 * @brief 阻塞式发送数据
 * @param handle 	对应UART外设驱动句柄
 * @param tx_data 	待发送数据首地址
 * @param data_len 	发送数据长度
 * @param wait_time 等待超时时间
 */
void UART_Driver_Tx_Data(UART_Driver_t* handle, uint8_t* tx_data, uint8_t data_len, uint32_t wait_time);	

/**
 * @brief 阻塞式接收数据
 * @param handle 	对应UART外设驱动句柄
 * @param rx_data 	待存储接收数据首地址
 * @param data_len 	接收数据长度
 * @param wait_time 等待超时时间
 */
void UART_Driver_Rx_Data(UART_Driver_t* handle, uint8_t* rx_data, uint8_t data_len, uint32_t wait_time);

/**
 * @brief 非阻塞式接收数据
 * @param handle 	对应UART外设驱动句柄
 * @param p_data 	待发送数据首地址
 * @param data_len 	发送数据长度
 */
void UART_Driver_Tx_Data_IT(UART_Driver_t* handle, uint8_t *p_data, uint8_t data_len);

/**
 * @brief 非阻塞式接收数据
 * @param handle 	对应UART外设驱动句柄
 * @param rx_data 	待存储接收数据首地址
 * @param data_len 	接收数据长度
 */
void UART_Driver_Rx_Data_IT(UART_Driver_t* handle, uint8_t *p_data, uint8_t data_len);

/**
 * @brief DMA通道自动完成数据发送
 * @param handle 	对应UART外设驱动句柄
 * @param p_data 	待发送数据首地址
 * @param data_len 	发送数据长度
 */
void UART_Driver_Tx_Data_DMA(UART_Driver_t* handle, uint8_t *p_data, uint8_t data_len);

/**
 * @brief DMA通道自动完成数据发送
 * @param handle 	对应UART外设驱动句柄
 * @param rx_data 	待存储接收数据首地址
 * @param data_len 	接收数据长度
 */
void UART_Driver_Rx_Data_DMA(UART_Driver_t* handle, uint8_t *p_data, uint8_t data_len);

#endif

