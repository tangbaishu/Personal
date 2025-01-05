#include "LED_Driver.h"

#define LED0_ON_LEVEL()		HAL_GPIO_WritePin(LED0_GPIO_PORT,LED0_GPIO_PIN,GPIO_PIN_RESET)
#define LED1_ON_LEVEL()		HAL_GPIO_WritePin(LED1_GPIO_PORT,LED1_GPIO_PIN,GPIO_PIN_RESET)


#define LED0_OFF_LEVEL()	HAL_GPIO_WritePin(LED0_GPIO_PORT,LED0_GPIO_PIN,GPIO_PIN_SET)
#define LED1_OFF_LEVEL()	HAL_GPIO_WritePin(LED1_GPIO_PORT,LED1_GPIO_PIN,GPIO_PIN_SET)

#define LED0_TURN()			HAL_GPIO_TogglePin(LED0_GPIO_PORT,LED0_GPIO_PIN);
#define LED1_TURN()			HAL_GPIO_TogglePin(LED1_GPIO_PORT,LED1_GPIO_PIN);

void LED_Driver_Check(void)
{
	LED_Driver_Init();
	LED_Driver_ON(0);LED_Driver_ON(1);
	HAL_Delay(1000);
	LED_Driver_OFF(0);LED_Driver_OFF(1);
	HAL_Delay(1000);
	LED_Driver_Turn(0);LED_Driver_Turn(1);
	HAL_Delay(1000);
	LED_Driver_OFF(0);LED_Driver_OFF(1);
	
}

void LED_Driver_Init(void)
{
	GPIO_InitTypeDef led_gpio_config;
	
	LED0_GPIO_CLOCK_EN();	// 先初始化外设时钟，在配置外设。
	LED1_GPIO_CLOCK_EN();
	
	led_gpio_config.Mode 	= GPIO_MODE_OUTPUT_PP;
	led_gpio_config.Pin 	= LED0_GPIO_PIN | LED1_GPIO_PIN;
	led_gpio_config.Pull 	= GPIO_NOPULL;
	led_gpio_config.Speed 	= GPIO_SPEED_HIGH;
	HAL_GPIO_Init(LED0_GPIO_PORT, &led_gpio_config);

	LED_Driver_OFF(0);
	LED_Driver_OFF(1);
}

/**
 * @brief LED_Driver_ON
 * 
 * @param led_num:	LED_BSP_Num_e
 */
void LED_Driver_ON(uint8 led_num)
{
	if(LED0 == led_num)
	{
		LED0_ON_LEVEL();
	}
	else if(LED1 == led_num)
	{
		LED1_ON_LEVEL();
	}
}

/**
 * @brief LED_Driver_OFF
 * 
 * @param led_num:	LED_BSP_Num_e
 */
void LED_Driver_OFF(uint8 led_num)
{
	if(LED0 == led_num)
	{
		LED0_OFF_LEVEL();
	}
	else if(LED1 == led_num)
	{
		LED1_OFF_LEVEL();
	}
}

/**
 * @brief LED_Driver_Turn
 * 
 * @param led_num:	LED_BSP_Num_e
 */
void LED_Driver_Turn(uint8 led_num)
{
	if(LED0 == led_num)
	{
		LED0_TURN();
	}
	else if(LED1 == led_num)
	{
		LED1_TURN();
	}
}
