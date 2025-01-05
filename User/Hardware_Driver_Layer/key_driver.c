#include "key_driver.h"
#include "stm32f4xx_hal.h"
#include "uart_debug_driver.h"

uint8_t 					Key_Press_Filter[KEY_NUMBER];		// 按键按下过滤器
static KEY_Type_BSP 		Key_Press_Type[KEY_NUMBER];			// 按键按下类型
QueueHandle_t 				G_key_Press_Queue = NULL;			// 按键按下键值 队列
static QueueHandle_t 		Key_Press_Semaphore = NULL;			// 按键按下信号
static KEY_Press_Type_e 	Key_Press_Value;					// 按键按下键值

static EXTI_HandleTypeDef	KEY0_Exti4_IRQn;					// 按键0 外部中断
static EXTI_HandleTypeDef 	KEY1_Exti3_IRQn;					// 按键1 外部中断
static EXTI_HandleTypeDef 	KEY2_Exti2_IRQn;					// 按键2 外部中断
static EXTI_HandleTypeDef 	KEY3_Exti0_IRQn;					// 按键3 外部中断
const static KEY_BSP_Init_Config_t	KEY_BSP_Config[]={	\
		{ GPIOE, GPIO_PIN_4, GPIO_PULLUP, EXTI_GPIOE, EXTI_LINE_4, EXTI4_IRQn, 5 },	\
		{ GPIOE, GPIO_PIN_3, GPIO_PULLUP, EXTI_GPIOE, EXTI_LINE_3, EXTI3_IRQn, 5},	\
		{ GPIOE, GPIO_PIN_2, GPIO_PULLUP, EXTI_GPIOE, EXTI_LINE_2, EXTI2_IRQn, 5},	\
		{ GPIOA, GPIO_PIN_0, GPIO_PULLDOWN, EXTI_GPIOA, EXTI_LINE_0, EXTI0_IRQn, 5}	\
													};		// 按键初始化配置

/**
 * @brief Construct a new key gpio init object
 * 注：未使能相关GPIO组时钟！！！
 * 初始化 与按键连接的 gpio口 
 * @param gpio_group 
 * @param gpio_pin 
 */
static void KEY_GPIO_Init(GPIO_TypeDef* gpio_group, uint32_t gpio_pin, uint32_t gpio_pull)
{
	GPIO_InitTypeDef key_init_config;
	key_init_config.Mode = GPIO_MODE_INPUT;
	key_init_config.Pin = gpio_pin;
	key_init_config.Pull = gpio_pull;
	HAL_GPIO_Init(gpio_group, &key_init_config);	
}

#if KEY_DRIVER_TYPE == KEY_DEIVER_INPUTER
static void KEY_EXTI_Init(	uint32_t exti_group, uint32_t exti_line, EXTI_HandleTypeDef* exti_handle, \
							IRQn_Type exti_irqn, uint32_t priority)
{
	EXTI_ConfigTypeDef	key_exti_config;
	key_exti_config.GPIOSel = exti_group;
	key_exti_config.Line = exti_line;
	key_exti_config.Mode = EXTI_MODE_INTERRUPT;
	key_exti_config.Trigger = EXTI_TRIGGER_RISING_FALLING;
	HAL_EXTI_SetConfigLine(exti_handle, &key_exti_config);
	HAL_NVIC_SetPriority(exti_irqn, priority, 0);
	HAL_NVIC_EnableIRQ(exti_irqn);
}

static void KEY_Press_Enable_ISR()
{
	uint32_t num=0;
	EXTI_HandleTypeDef* exti_handle;
	KEY0_Exti4_IRQn.Line = KEY_BSP_Config[0].KEY_Exti_Line;
	KEY1_Exti3_IRQn.Line = KEY_BSP_Config[1].KEY_Exti_Line;
	KEY2_Exti2_IRQn.Line = KEY_BSP_Config[2].KEY_Exti_Line;
	KEY3_Exti0_IRQn.Line = KEY_BSP_Config[3].KEY_Exti_Line;
	for(;num<sizeof(KEY_BSP_Config)/sizeof(KEY_BSP_Init_Config_t);num++)
	{
		exti_handle = (EXTI_HandleTypeDef*) &(KEY_BSP_Config[num].KEY_Exti_Line);
		KEY_EXTI_Init(	KEY_BSP_Config[num].KEY_Exti_group, KEY_BSP_Config[num].KEY_Exti_Line, exti_handle,\
						KEY_BSP_Config[num].KEY_IRQn_Type, KEY_BSP_Config[num].KEY_IRQ_Priority);
	}
	
}
#endif

static void KEY_Driver_Init(void)
{
	uint8_t	num=0;
	KEY0_CLOCK_ENABLE();
	KEY1_CLOCK_ENABLE();
	KEY2_CLOCK_ENABLE();
	KEY3_CLOCK_ENABLE();
	for(;num<sizeof(KEY_BSP_Config)/sizeof(KEY_BSP_Init_Config_t);num++)
	{
		KEY_GPIO_Init(KEY_BSP_Config[num].KEY_Group, KEY_BSP_Config[num].KEY_Gpio_Pin, KEY_BSP_Config[num].KEY_Pull_Type);
	}
	#if KEY_DRIVER_TYPE == KEY_DEIVER_INPUTER
		KEY_Press_Enable_ISR();
	#endif
}

static void KEY_Press_Filter(void)
{
	if(GET_KEY0_STATE() == KEY_PRESS)
	{
		Key_Press_Filter[0]++;
		if(Key_Press_Filter[0] == KEY_SHORT_FILTER_COUNT)// 多次扫描，均连续处于按下状态
		{
			if((Key_Press_Type[0] != KEY0) && (Key_Press_Type[0] != KEY0_LONG))
			{
				Key_Press_Type[0] = KEY0;
				xSemaphoreGive(Key_Press_Semaphore);
				if(xQueueSend(G_key_Press_Queue, (KEY_Press_Type_e *)&Key_Press_Type[0], 10) == errQUEUE_FULL)	// 从队列末尾插入
				{
					DMA_Sprintf("Error!!!\t G_key_Press_Queue Full\r\n");
				}
			}
		}
		else if(Key_Press_Filter[0] == KEY_LONG_FILTER_COUNT)
		{
			if(Key_Press_Type[0] != KEY0_LONG)
			{
				Key_Press_Type[0] = KEY0_LONG;
				xSemaphoreGive(Key_Press_Semaphore);
				if(xQueueSend(G_key_Press_Queue, (KEY_Press_Type_e *)&Key_Press_Type[0], 10) == errQUEUE_FULL)	// 从队列末尾插入
				{
					DMA_Sprintf("Error!!!\t G_key_Press_Queue Full\r\n");
				}
			}	
		}
	}
	else
	{
		Key_Press_Type[0] = LOOSEN;
		Key_Press_Filter[0] = 0;
	}

	if(GET_KEY1_STATE() == KEY_PRESS)
	{
		Key_Press_Filter[1]++;
		if(Key_Press_Filter[1] == KEY_SHORT_FILTER_COUNT)// 多次扫描，均连续处于按下状态
		{
			if((Key_Press_Type[1] != KEY1) && (Key_Press_Type[1] != KEY1_LONG))
			{
				Key_Press_Type[1] = KEY1;
				xSemaphoreGive(Key_Press_Semaphore);
				if(xQueueSend(G_key_Press_Queue, (KEY_Press_Type_e *)&Key_Press_Type[1], 10) == errQUEUE_FULL)	// 从队列末尾插入
				{
						DMA_Sprintf("Error!!!\t G_key_Press_Queue Full\r\n");
				}
			}
			
		}
		else if(Key_Press_Filter[1] == KEY_LONG_FILTER_COUNT)
		{
			if(Key_Press_Type[1] != KEY1_LONG)
			{
				Key_Press_Type[1] = KEY1_LONG;
				xSemaphoreGive(Key_Press_Semaphore);
				if(xQueueSend(G_key_Press_Queue, (KEY_Press_Type_e *)&Key_Press_Type[1], 10) == errQUEUE_FULL)	// 从队列末尾插入
				{
					DMA_Sprintf("Error!!!\t G_key_Press_Queue Full\r\n");
				}
			}	
		}
	}
	else
	{
		Key_Press_Type[1] = LOOSEN;
		Key_Press_Filter[1] = 0;
	}

	if(GET_KEY2_STATE() == KEY_PRESS)
	{
		Key_Press_Filter[2]++;
		if(Key_Press_Filter[2] == KEY_SHORT_FILTER_COUNT)// 多次扫描，均连续处于按下状态
		{
			if((Key_Press_Type[2] != KEY2) && (Key_Press_Type[2] != KEY2_LONG))
			{
				Key_Press_Type[2] = KEY2;
				xSemaphoreGive(Key_Press_Semaphore);
				if(xQueueSend(G_key_Press_Queue, (KEY_Press_Type_e *)&Key_Press_Type[2], 10)  == errQUEUE_FULL)	// 从队列末尾插入
				{
						DMA_Sprintf("Error!!!\t G_key_Press_Queue Full\r\n");
				}
			}
		}
		else if(Key_Press_Filter[2] == KEY_LONG_FILTER_COUNT)
		{
			if(Key_Press_Type[2] != KEY2_LONG)
			{
				Key_Press_Type[2] = KEY2_LONG;
				xSemaphoreGive(Key_Press_Semaphore);
				if(xQueueSend(G_key_Press_Queue, (KEY_Press_Type_e *)&Key_Press_Type[2], 10) == errQUEUE_FULL)	// 从队列末尾插入
				{
					DMA_Sprintf("Error!!!\t G_key_Press_Queue Full\r\n");
				}
			}	
		}
	}
	else
	{
		Key_Press_Type[2] = LOOSEN;
		Key_Press_Filter[2] = 0;
	}

	if(GET_KEY3_STATE() == KEY_PRESS)
	{
		Key_Press_Filter[3]++;
		if(Key_Press_Filter[3] == KEY_SHORT_FILTER_COUNT)// 多次扫描，均连续处于按下状态
		{
			if((Key_Press_Type[3] != KEY3) && (Key_Press_Type[3] != KEY3_LONG))
			{
				Key_Press_Type[3] = KEY3;
				xSemaphoreGive(Key_Press_Semaphore);
				if(xQueueSend(G_key_Press_Queue, (KEY_Press_Type_e *)&Key_Press_Type[3], 10) == errQUEUE_FULL)	// 从队列末尾插入;
				{
						DMA_Sprintf("Error!!!\t G_key_Press_Queue Full\r\n");
				}
			}
		}
		else if(Key_Press_Filter[3] == KEY_LONG_FILTER_COUNT)
		{
			if(Key_Press_Type[3] != KEY3_LONG)
			{
				Key_Press_Type[3] = KEY3_LONG;
				xSemaphoreGive(Key_Press_Semaphore);
				if(xQueueSend(G_key_Press_Queue, (KEY_Press_Type_e *)&Key_Press_Type[3], 10) == errQUEUE_FULL)	// 从队列末尾插入
				{
					DMA_Sprintf("Error!!!\t G_key_Press_Queue Full\r\n");
				}
			}	
		}
	}
	else
	{
		Key_Press_Type[3] = LOOSEN;
		Key_Press_Filter[3] = 0;
	}
}

static void KEY_Scan_Timer(TimerHandle_t xtimer)
{
	KEY_Press_Filter();
}

static void KEY_Press_Debug(void)
{
	xQueuePeek(G_key_Press_Queue, (KEY_Press_Type_e *)&Key_Press_Value, portMAX_DELAY);		// 窥探按键 按下键值/类别 队列数据 
	switch (Key_Press_Value)
	{
		case KEY_LEFT:			printf("KEY_LEFT\r\n");break;
		case KEY_LEFT_LONG:		printf("KEY_LEFT_LONG\r\n");break;

		case KEY_RIGHT:			printf("KEY_RIGHT\r\n");break;
		case KEY_RIGHT_LONG:	printf("KEY_RIGHT_LONG\r\n");break;

		case KEY_DOWN:			printf("KEY_DOWN\r\n");break;
		case KEY_DOWN_LONG:		printf("KEY_DOWN_LONG\r\n");break;

		case KEY_UP:			printf("KEY_UP\r\n");break;
		case KEY_UP_LONG:		printf("KEY_UP_LONG\r\n");break;
		default: break;
	}
}

void KEY_Scan(void* param)
{
	TickType_t blocktime = portMAX_DELAY;
	#if KEY_DRIVER_TYPE == KEY_DEIVER_TIMER
		static TimerHandle_t key_scan_timer = NULL;
		key_scan_timer = xTimerCreate("KEY_Scan", KEY_SCAN_PERIOD, pdTRUE, (void *) 0,  KEY_Scan_Timer);
		if(key_scan_timer == NULL)
		{
			goto error_Init_Fail;
		}
		else
		{
			xTimerStart(key_scan_timer, 0);
		}
	#endif
	G_key_Press_Queue = xQueueCreate(KEY_NUMBER, sizeof(KEY_Press_Type_e));
	if(G_key_Press_Queue == NULL)
	{
		goto error_Init_Fail;
	}
	
	Key_Press_Semaphore = xSemaphoreCreateCounting(1u<<(KEY_NUMBER * 2+1), 0);
	if(Key_Press_Semaphore == NULL)
	{
		goto error_Init_Fail;
	}
	KEY_Driver_Init();
	// xSemaphoreCreateBinary()		// 新版本创建二进制信号量，创建初始值为空，可直接使用
	// vSemaphoreCreateBinary();	// 老版本创建二进制信号量，创建初始值为满，需要先“take”为空
	while (1)
	{
		// DMA_Sprintf("semaphore_count=%ld\r\n", uxSemaphoreGetCount(Key_Press_Semaphore));				// 获取按键按下信号量次数
		DMA_Sprintf("xSemaphoreTake =%d\r\n", (uint16_t)xSemaphoreTake(Key_Press_Semaphore, blocktime));	// 等待按键按下信号量
		#if KEY_DRIVER_TYPE == KEY_DEIVER_INPUTER
			KEY_Press_Filter();
			if(xQueuePeek(G_key_Press_Queue, (KEY_Press_Type_e *)&Key_Press_Value, 10) == pdTRUE)
			{
				KEY_Press_Debug();	
				blocktime = portMAX_DELAY;
			}
		#else
			KEY_Press_Debug();
		#endif
		
		// xQueueReceive(G_key_Press_Queue, (KEY_Press_Type_e *)&key_press_value, portMAX_DELAY);				// 获取后删除 按键键值/类别 队列数据
	
	}
error_Init_Fail:	
	DMA_Sprintf("Error!!!\t KEY_Scan() Init Fail\r\n");
	vTaskDelete(NULL);
}

/**
 * @brief KEY3 = KEY_UP
 * 
 */
void EXTI0_IRQHandler(void)
{
//    if ((uint8)GET_KEY0_STATE() == KEY_PRESS)
//    {
//        Key_Press_Filter[0] = HAL_GetTick();
//    }
//    else
//    {
//        Key_Press_Filter[0] = 0;
//    }
	DMA_Sprintf_ISR("key3_irq KEY_UP isr\r\n");
    HAL_EXTI_IRQHandler(&KEY3_Exti0_IRQn);
}

/**
 * @brief KEY2 = KEY_RIGHT
 * 
 */
void EXTI2_IRQHandler(void)
{
//    if ((uint8)GET_KEY0_STATE() == KEY_PRESS)
//    {
//        Key_Press_Filter[0] = HAL_GetTick();
//    }
//    else
//    {
//        Key_Press_Filter[0] = 0;
//    }
DMA_Sprintf_ISR("key2_irq KEY_RIGHT isr\r\n");
    HAL_EXTI_IRQHandler(&KEY2_Exti2_IRQn);
}

/**
 * @brief KEY1 = KEY_DOWN
 * 
 */
void EXTI3_IRQHandler(void)
{
//    if ((uint8)GET_KEY0_STATE() == KEY_PRESS)
//    {
//        Key_Press_Filter[0] = HAL_GetTick();
//    }
//    else
//    {
//        Key_Press_Filter[0] = 0;
//    }
DMA_Sprintf_ISR("key1_irq KEY_DOWN isr\r\n");
    HAL_EXTI_IRQHandler(&KEY1_Exti3_IRQn);
}

/**
 * @brief KEY0 = KEY_LEFT
 * 
 */
void EXTI4_IRQHandler(void)
{
//    if ((uint8)GET_KEY0_STATE() == KEY_PRESS)
//    {
//        Key_Press_Filter[0] = HAL_GetTick();
//    }
//    else
//    {
//        Key_Press_Filter[0] = 0;
//    }
DMA_Sprintf_ISR("key0_irq KEY_LEFT isr\r\n");
    HAL_EXTI_IRQHandler(&KEY0_Exti4_IRQn);
}
