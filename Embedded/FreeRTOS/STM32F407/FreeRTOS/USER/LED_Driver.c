#include "SYS_Prj.h"

extern TaskHandle_t * task_handle;

void LED_Init(void)
{
	GPIO_InitTypeDef LED_gpio_config;
	__HAL_RCC_GPIOF_CLK_ENABLE();
	LED_gpio_config.Mode = GPIO_MODE_OUTPUT_PP;
	LED_gpio_config.Pin = GPIO_PIN_9;
	LED_gpio_config.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOF,&LED_gpio_config);
	HAL_GPIO_WritePin(GPIOF,GPIO_PIN_9,GPIO_PIN_RESET);
}

void LED_Control(void *params)
{
	uint8_t count=0;
	while(1)
	{
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_9,GPIO_PIN_RESET);
		HAL_Delay(1000);
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_9,GPIO_PIN_SET);
		HAL_Delay(1000);
		count++;
		printf("LED_Control_count=%d\r\n",count);
		if(count>=2)
		{
			if(task_handle!=NULL)
			{
				vTaskDelete(*task_handle);
				task_handle = NULL;
			}
//			vTaskDelete(NULL);
		}
	}
}


