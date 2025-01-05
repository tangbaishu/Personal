/**
 * @file uart_debug_driver.h
 * @author tanglibo
 * @brief 
 * V0.1：新增变量：UART空闲标志；之前采用读取 USART SR状态寄存器，获取空闲状态。现未 防止在等待串口空闲时，概率性进去中断后清除 SR状态寄存器标志。
 * @version 0.1
 * @date 2025-01-05
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef _UART_DEBUG_DRIVER_H_
#define _UART_DEBUG_DRIVER_H_

#include "usart.h"
#include "base_function.h"
#include <stdio.h>

#ifndef uint8
#ifndef _DECA_UINT8_
#define _DECA_UINT8_
typedef unsigned char uint8;
#endif
#endif

extern DMA_HandleTypeDef DMA_Uart_TX_Config;
extern DMA_HandleTypeDef DMA_Uart_RX_Config;

#define DEBUG_BUFF_LEN				1000    //  定义DEBUG_BUFF缓存器数据长度 单位：char
#define VALID_DEBUG_BUFF_LEN		(500)	//  DEBUG_BUFF缓存器数据在实际运行过程中安全溢出值。\
                                    	    //  用于放置单次传输数据 ＞ (DEBUG_BUFF_LEN - VALID_DEBUG_BUFF_LEN)
                                    	    //  导致数据溢出风险
#define RECEIVE_STACK_BUFF_LEN  	100

typedef struct
{
	uint8	Rec_Finish_Flag;		// 当启动 UartDMA_Rx 传输时，该标志位置0； 当UART_Rx 产生空闲中断时，停止 UartDMA_Rx 传输并使该标志位置1
	uint8	Rec_Start_Flag;			// 当启动 UartDMA_Rx 传输时，该标志位置1； 当UART_Rx 产生空闲中断时，停止 UartDMA_Rx 传输并使该标志位置0
	Data_Buff_Queue_t Data_Buff;		// UartDMA_Rx 传输数据缓存区
}SYS_Uart_Rec_Queue_t;
extern SYS_Uart_Rec_Queue_t SYS_UartDMA_Rx_Queue;


extern uint8 Init_Finish_Flag;

/**
 * @brief
 * DMA_USART1_TX  ->DMA2_Stream7	->DMA_CHANNEL_4
 * DMA_USART1_RX  ->DMA2_Stream5	->DMA_CHANNEL_4
 */
void DMA_UART_Debug_Driver_Init(void);

/**
 * @brief 通过DMA 完成uart_tx数据发送
 * 注：在发送过程中未进行 喂狗操作，需要警惕大量数据打印 导致WWDG溢出
 * 
 * @param format 
 * @param ... 
 */
void DMA_Sprintf(const char *format,...);
void DMA_Sprintf_ISR(const char *format, ...);

/**
 * 检查 DMA_Debug_Buff内的数据是否传输完毕
 * 若未传输完毕，在UART_DMATransmitCplt()函数内再次开启DMA传输
 */
void DMA_UART1_Buff_ckeck(void);

void DMA_Sprintf_String(const uint8 *string_data, uint16 string_len);


#endif
