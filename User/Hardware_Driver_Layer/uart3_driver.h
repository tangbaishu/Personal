#ifndef _UART3_DRIVER_H_
#define _UART3_DRIVER_H_

#include "usart.h"

#ifndef uint8
#ifndef _DECA_UINT8_
#define _DECA_UINT8_
typedef unsigned char uint8;
#endif
#endif

#define REC_BUFFER_LEN  100		// unit: uint8_t	 

typedef struct
{
	uint8_t 	Receive_Buffer_Area[REC_BUFFER_LEN];
	uint8_t		Valid_Buffer_Data_Len;
	uint8_t		Rec_Buffer_Overflow;
	uint8_t		Rec_Data_Finish;
	uint32_t	test_1;
	uint32_t	test_2;
}UART3_Driver_Data_t;
extern UART3_Driver_Data_t		UART3_Driver_Data;

void UART3_Driver_Init(void);

/**
 * @brief
 * DMA_USART3_TX  ->DMA1_Stream3	->DMA_CHANNEL_4
 * DMA_USART3_RX  ->DMA1_Stream1	->DMA_CHANNEL_1
 */
void UART3_DMA_Driver_Init(void);

void UART3_Send_Hex_Data(uint8_t *p_hex_data, uint8 data_len);

void UART3_Receive_Data_Wait(uint8_t *p_buff, uint8_t size, uint32_t timeout);
#endif

