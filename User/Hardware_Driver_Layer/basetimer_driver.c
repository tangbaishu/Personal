#include "basetimer_driver.h"

/**TIM时钟源频率：84MHZ */
static const TIM_Base_Config_t TIM_Base_Config_Array[]={ {TIM7, 100-1, 84-1, TIM7_IRQn, 4} };
TIM_HandleTypeDef 	TIM7_Handle;
uint32_t FreeRTOS_10usRun_Tick;				// 单位：ms

void TIM7_Base_Init(void)


{
	__HAL_RCC_TIM7_CLK_ENABLE();
	TIMx_Base_Config_Init(TIM7_Handle, TIM_Base_Config_Array[0]);
	FreeRTOS_10usRun_Tick = 0;
}

void TIMx_Base_Config_Init(TIM_HandleTypeDef tim_handle, TIM_Base_Config_t TIM_Base_Config)
{
	tim_handle.Instance = TIM_Base_Config.instance;
	tim_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	tim_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	tim_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim_handle.Init.Period = TIM_Base_Config.period;
	tim_handle.Init.Prescaler = TIM_Base_Config.prescaler;
	tim_handle.Init.RepetitionCounter = 0;
	HAL_TIM_Base_Init(&tim_handle);
	HAL_TIM_Base_Start_IT(&tim_handle);
	HAL_NVIC_SetPriority(TIM_Base_Config.tim_irqn, TIM_Base_Config.tim_preempt_priority, 0);
	HAL_NVIC_EnableIRQ(TIM_Base_Config.tim_irqn);
}

void TIM7_IRQHandler(void)
{
	if(READ_BIT(TIM7->SR, TIM_SR_UIF))
	{
		FreeRTOS_10usRun_Tick++;
		CLEAR_BIT(TIM7->SR,TIM_SR_UIF);
	}
}
