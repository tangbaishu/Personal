/* USER CODE BEGIN Header */
#include "driver_led.h"
#include "driver_lcd.h"
#include "driver_mpu6050.h"
#include "driver_timer.h"
#include "driver_ds18b20.h"
#include "driver_dht11.h"
#include "driver_active_buzzer.h"
#include "driver_passive_buzzer.h"
#include "driver_color_led.h"
#include "driver_ir_receiver.h"
#include "driver_ir_sender.h"
#include "driver_light_sensor.h"
#include "driver_ir_obstacle.h"
#include "driver_ultrasonic_sr04.h"
#include "driver_spiflash_w25q64.h"
#include "driver_rotary_encoder.h"
#include "driver_motor.h"
#include "driver_key.h"
#include "driver_uart.h"
#include "FreeRTOS.h"                   // ARM.FreeRTOS::RTOS:Core
#include "task.h"                       // ARM.FreeRTOS::RTOS:Core
#include "event_groups.h"               // ARM.FreeRTOS::RTOS:Event Groups
#include "semphr.h"                     // ARM.FreeRTOS::RTOS:Core

/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
static StackType_t g_pucStackOfLightTask[80];
static StaticTask_t g_TCBofLightTask;
static TaskHandle_t xLightTaskHandle;

static StackType_t g_pucStackOfColorTask[128];
static StaticTask_t g_TCBofColorTask;
static TaskHandle_t xColorTaskHandle;
void game1_task(void *params);
static SemaphoreHandle_t g_xI2CMutex; 
static signed char pcWriteBuffer[200];

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void GetI2C(void)
{
	/* ç­ĺžä¸?ä¸ŞäşćĽé */
	xSemaphoreTake(g_xI2CMutex, portMAX_DELAY);
}

void PutI2C(void)
{
	/* éćžäşćĽé? */
	xSemaphoreGive(g_xI2CMutex);
}



/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationIdleHook(void);

/* USER CODE BEGIN 2 */
void vApplicationIdleHook( void )
{
   /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
   to 1 in FreeRTOSConfig.h. It will be called on each iteration of the idle
   task. It is essential that code added to this hook function never attempts
   to block in any way (for example, call xQueueReceive() with a block time
   specified, or call vTaskDelay()). If the application makes use of the
   vTaskDelete() API function (as this demo application does) then it is also
   important that vApplicationIdleHook() is permitted to return to its calling
   function, because it is the responsibility of the idle task to clean up
   memory allocated by the kernel to any task that has since been deleted. */
	int i;
	vTaskList(pcWriteBuffer);
	for (i = 0; i < 16; i++)
		printf("-");
	printf("\n\r");
	printf("%s\n\r", pcWriteBuffer);
}
/* USER CODE END 2 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
	extern void car_test(void);
  g_xI2CMutex = xSemaphoreCreateMutex();

  LCD_Init();
  LCD_Clear();
  
  MPU6050_Init();
  IRReceiver_Init();
  RotaryEncoder_Init();
  LCD_PrintString(0, 0, "Starting");
  //car_test();
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* ĺĺťşäťťĺĄ */
    extern void PlayMusic(void *params);
	extern void MPU6050_Task(void *params);

	xTaskCreate(game1_task, "GameTask", 128, NULL, osPriorityNormal, NULL);
	extern void car_game(void);
	//car_game();
    //xTaskCreate(PlayMusic, "MusicTask", 128, NULL, osPriorityNormal, NULL);

  /* ĺĺťşäťťĺĄ:ç? */
  xLightTaskHandle = xTaskCreateStatic(Led_Test, "LightTask", 80, NULL, osPriorityNormal, g_pucStackOfLightTask, &g_TCBofLightTask);

  /* ĺĺťşäťťĺĄ:ĺ? */
  xColorTaskHandle = xTaskCreateStatic(ColorLED_Test, "ColorTask", 128, NULL, osPriorityNormal, g_pucStackOfColorTask, &g_TCBofColorTask);

  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */

/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  uint8_t dev, data;
	int bRunning;
	
	TaskHandle_t xSoundTaskHandle = NULL;
	BaseType_t ret;
	
	vTaskDelete(NULL);
	
	LCD_Init();
	LCD_Clear();

	
    IRReceiver_Init();
	LCD_PrintString(0, 0, "Waiting control");
	
	while (1)
	{
		MPU6050_Test();
	}

    while (1)
    {
        /* čŻťĺçş˘ĺ¤éĽć§ */
		if (0 == IRReceiver_Read(&dev, &data))
		{		
			if (data == 0xa8) /* play */
			{
				/* ĺĺťşć­ćžéłäšçäťťĺ? */
			  extern void PlayMusic(void *params);
			  if (xSoundTaskHandle == NULL)
			  {
					LCD_ClearLine(0, 0);
					LCD_PrintString(0, 0, "Create Task");
					ret = xTaskCreate(PlayMusic, "SoundTask", 128, NULL, osPriorityNormal+1, &xSoundTaskHandle);
					bRunning = 1;
			  }
			  else
			  {
				  /* čŚäšsuspendčŚäšresume */
				  if (bRunning)
				  {
					  LCD_ClearLine(0, 0);
					  LCD_PrintString(0, 0, "Suspend Task");
					  vTaskSuspend(xSoundTaskHandle);
					  PassiveBuzzer_Control(0); /* ĺć­˘čé¸Łĺ? */
					  bRunning = 0;
				  }
				  else
				  {
					  LCD_ClearLine(0, 0);
					  LCD_PrintString(0, 0, "Resume Task");
					  vTaskResume(xSoundTaskHandle);
					  bRunning = 1;
				  }
			  }
			}
			
			else if (data == 0xa2) /* power */
			{
				/* ĺ é¤ć­ćžéłäšçäťťĺ? */
				if (xSoundTaskHandle != NULL)
				{
					LCD_ClearLine(0, 0);
					LCD_PrintString(0, 0, "Delete Task");
					vTaskDelete(xSoundTaskHandle);
					PassiveBuzzer_Control(0); /* ĺć­˘čé¸Łĺ? */
					xSoundTaskHandle = NULL;
				}
			}
		}
    }
	
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

