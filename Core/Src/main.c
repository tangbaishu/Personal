/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "uart_debug_driver.h"
#include "uart3_driver.h"
#include "crc_driver.h"
#include "base_function.h"
#include "flash_driver.h"
#include "wwdg_driver.h"
#include "LED_Driver.h"

#include "BootLoader.h"

#include "lwip.h"

void SystemClock_Config(void);

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
    HAL_Init();
	SystemClock_Config();
	HAL_Delay(1000);	// 上电延时
	
	LED_Driver_Init();
	LED_Driver_Check();
    DMA_UART_Debug_Driver_Init();
    // CRC_Driver_Init();
    // MX_LWIP_Init();
	// IWDG_Init();
	// WWDG_Init();
    DMA_Sprintf("BOOT\t STM32F407ZGT6_Embedded_Init_Success\r\n");
	UART3_DMA_Driver_Init();
//	Test_Flash();
    //  Jump_Code_District(1);
    // DMA_Sprintf("Jump Fail!!!\r\n");
    uint8_t test_data[]={0x12,0x23,0x34};
    UART3_Send_Hex_Data(test_data, sizeof(test_data));
    // CRC_Start();
	// UART3_Driver_Data.Receive_Buffer_Area[0] = 0xa1;
	// UART3_Driver_Data.Receive_Buffer_Area[1] = 0xa2;
	// UART3_Driver_Data.Receive_Buffer_Area[2] = 0xa3;
	// UART3_Driver_Data.Receive_Buffer_Area[3] = 0xa4;
	// DMA_Sprintf_String((uint8*)&UART3_Driver_Data.Receive_Buffer_Area[0], 3);
    UART3_Receive_Data_Wait(UART3_Driver_Data.Receive_Buffer_Area, 50, 0xefffffff);
    printf("UART3_Receiv_Finish\r\n");
    while (1)
    {
		WDG_CLEAR();
		if(SYS_UartDMA_Rx_Queue.Rec_Finish_Flag)
        {
            SYS_UartDMA_Rx_Queue.Rec_Finish_Flag = 0;
            DMA_Sprintf_String(SYS_UartDMA_Rx_Queue.Data_Buff.Data, SYS_UartDMA_Rx_Queue.Data_Buff.Len);
            
        }
        if(UART3_Driver_Data.Rec_Data_Finish == 1)
        {
            printf("UART3_Receiv_Data\r\n");
            UART3_Driver_Data.Rec_Data_Finish=0;
            DMA_Sprintf_String((uint8*)&UART3_Driver_Data.Receive_Buffer_Area[0], UART3_Driver_Data.Valid_Buffer_Data_Len);
            UART3_Send_Hex_Data(UART3_Driver_Data.Receive_Buffer_Area, UART3_Driver_Data.Valid_Buffer_Data_Len);
        }
        if(UART3_Driver_Data.test_1 != 0 && UART3_Driver_Data.test_2 != 0)
        {
            HAL_Delay(500);
            printf("test_1 = %d, test_2 = %d \r\n", UART3_Driver_Data.test_1, UART3_Driver_Data.test_2);
        }
		
        //        MX_LWIP_Process();
        //        DMA_UART1_Buff_ckeck();
        //        HAL_Delay(100);
    }

}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
     */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 168;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
