#ifndef KEY_DRIVER_H_
#define KEY_DRIVER_H_

#include "stm32f4xx_hal.h"
#include "base_function.h"

/**********KEY BSP START**********/
#define KEY_DEIVER_TIMER	0
#define KEY_DEIVER_INPUTER	1
#define	KEY_DRIVER_TYPE		KEY_DEIVER_TIMER
typedef struct
{
	GPIO_TypeDef* 	KEY_Group;			//	GPIOA 、 GPIOB ~ GPIOE
	uint32_t		KEY_Gpio_Pin;		//	GPIO_PIN_0 ~ GPIO_PIN_15
	uint32_t		KEY_Pull_Type;		//	GPIO_NOPULL 、 GPIO_PULLUP 、 GPIO_PULLDOWN
	uint32_t		KEY_Exti_group;		//	EXTI_GPIOA 、EXTI_GPIOB ~ EXTI_GPIOE
	uint32_t		KEY_Exti_Line;		//	EXTI_LINE_0 ~ EXTI_LINE_15
	IRQn_Type		KEY_IRQn_Type;		//	EXTI0_IRQn ~ EXTI4_IRQn 、 EXTI9_5_IRQn 、 EXTI15_10_IRQn
	uint32_t		KEY_IRQ_Priority;	//	< 0x10U(16)
}KEY_BSP_Init_Config_t;

#define KEY0_CLOCK_ENABLE()		__HAL_RCC_GPIOE_CLK_ENABLE()
#define KEY1_CLOCK_ENABLE()		__HAL_RCC_GPIOE_CLK_ENABLE()
#define KEY2_CLOCK_ENABLE()		__HAL_RCC_GPIOE_CLK_ENABLE()
#define KEY3_CLOCK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE()

typedef enum
{
	KEY_PRESS = 1,
	KEY_LOOSEN
}KEY_State_e;

#define GET_KEY0_STATE()		HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == 0?KEY_PRESS:KEY_LOOSEN
#define GET_KEY1_STATE()		HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == 0?KEY_PRESS:KEY_LOOSEN
#define GET_KEY2_STATE()		HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2) == 0?KEY_PRESS:KEY_LOOSEN
#define GET_KEY3_STATE()		HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 1?KEY_PRESS:KEY_LOOSEN
/**********KEY BSP END**********/

#define KEY_NUMBER				4			// 按键数量
#define	KEY_SCAN_PERIOD			(10)		// 单位：ms	按键扫描周期
#define KEY_SHORT_FILTER_TIME	(100)			// 单位：ms 按键短按滤波时间
#define KEY_LONG_FILTER_TIME	(500)			// 单位：ms 按键长按滤波时间
#define KEY_SHORT_FILTER_COUNT	((KEY_SHORT_FILTER_TIME) / (KEY_SCAN_PERIOD))	// 按键短按滤波，连续按下次数
#define KEY_LONG_FILTER_COUNT	((KEY_LONG_FILTER_TIME) / (KEY_SCAN_PERIOD))		// 按键长按滤波，连续按下次数

typedef enum
{
	NO_PRESS = 0,
	KEY_UP = 1,
	KEY_UP_LONG,
	KEY_UP_LOOSEN,

	KEY_DOWN,
	KEY_DOWN_LONG,
	KEY_DOWN_LOOSEN,

	KEY_LEFT,
	KEY_LEFT_LONG,
	KEY_LEFT_LOOSEN,
	
	KEY_RIGHT,
	KEY_RIGHT_LONG,
	KEY_RIGHT_LOOSEN
}KEY_Press_Type_e;

typedef enum
{
	KEY0 		= KEY_LEFT,
	KEY0_LONG 	= KEY_LEFT_LONG,
	KEY1 		= KEY_DOWN,
	KEY1_LONG 	= KEY_DOWN_LONG,
	KEY2 		= KEY_RIGHT,
	KEY2_LONG 	= KEY_RIGHT_LONG,
	KEY3 		= KEY_UP,
	KEY3_LONG 	= KEY_UP_LONG,
	LOOSEN		= 0
}KEY_Type_BSP;

void KEY_Scan(void* param);

#endif //  KEY_DRIVER_H_
