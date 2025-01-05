#include "communication_module.h"
#include "stm32f4xx_hal.h"
#include "uart_debug_driver.h"

#define PRINTF_EN	ENABLE

#define LOG		"communciation_module.c	:"

Communication_Hardware_API_t Communication_Hardware_API={DMA_UART_Debug_Driver_Init,};

void Communciation_Module_Init(Communication_Hardware_API_t *pointer_api)
{
	#undef PRINTF_EN
	#define PRINTF_EN	DISABLE
	if(pointer_api->Uart_Init != NULL)
	{
		pointer_api->Uart_Init();
		#if PRINTF_EN == ENABLE
			DMA_Sprintf(LOG"Communciation_Module_Init -> Uart_Init()\r\n");
		#endif
	}
	#if PRINTF_EN == ENABLE
	else
	{
		DMA_Sprintf(LOG"Communciation_Module_Init -> Uart_Init() NULL\r\n");
	}
	#endif

	if(pointer_api->ETH_Init != NULL)
	{
		pointer_api->Uart_Init();
		#if PRINTF_EN == ENABLE
			DMA_Sprintf(LOG"Communciation_Module_Init -> ETH_Init()\r\n");
		#endif
	}
	#if PRINTF_EN == ENABLE
	else
	{
		DMA_Sprintf(LOG"Communciation_Module_Init -> ETH_Init() NULL\r\n");
	}
	#endif

	// if(pointer_api->Uart_Init != NULL)
	// {
	// 	pointer_api->Uart_Init();
	// }
	// #if PRINTF_EN == ENABLE
	// else
	// {
	// 	DMA_Sprintf(LOG"Communciation_Module_Init -> Uart_Init() NULL\r\n");
	// }
	// #endif
	#ifndef 
	
	#endifndef 
	#define PRINTF_EN	ENABLE
}