#ifndef _LED_DRIVER_H_
#define _LED_DRIVER_H_

#include "stm32f4xx_hal.h"

#define LED_NUMBER		2

#define LED0_GPIO_CLOCK_EN()	__HAL_RCC_GPIOF_CLK_ENABLE()
#define LED0_GPIO_PORT			GPIOF
#define LED0_GPIO_PIN			GPIO_PIN_9

#define LED1_GPIO_CLOCK_EN()	__HAL_RCC_GPIOF_CLK_ENABLE()
#define LED1_GPIO_PORT			GPIOF
#define LED1_GPIO_PIN			GPIO_PIN_10

#ifndef uint8
#ifndef _DECA_UINT8_
#define _DECA_UINT8_
typedef unsigned char uint8;
#endif
#endif

typedef enum
{
	LED0 = 0,
	LED1,
}LED_BSP_Num_e;

void LED_Driver_Check(void);

void LED_Driver_Init(void);

/**
 * @brief LED_Driver_ON
 * 
 * @param led_num:	LED_BSP_Num_e
 */
void LED_Driver_ON(uint8 led_num);

/**
 * @brief LED_Driver_OFF
 * 
 * @param led_num:	LED_BSP_Num_e
 */
void LED_Driver_OFF(uint8 led_num);

/**
 * @brief LED_Driver_Turn
 * 
 * @param led_num:	LED_BSP_Num_e
 */
void LED_Driver_Turn(uint8 led_num);

#endif
