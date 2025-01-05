#ifndef BASE_TIMER_DRIVER_H_
#define BASE_TIMER_DRIVER_H_

#include "stm32f4xx_hal.h"

typedef struct
{
	TIM_TypeDef 		*instance;
	uint16_t 			period;
	uint16_t 			prescaler;
	IRQn_Type			tim_irqn;
	uint8_t				tim_preempt_priority;
}TIM_Base_Config_t;

void TIM7_Base_Init(void);
void TIMx_Base_Config_Init(TIM_HandleTypeDef tim_handle, TIM_Base_Config_t TIM_Base_Config);

#endif

