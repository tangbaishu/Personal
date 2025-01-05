#include "pwm_driver.h"

void PWM_MspInit(void)
{
	GPIO_InitTypeDef buzzer_gpio_config;
	
	__HAL_RCC_GPIOF_CLK_ENABLE();
	
	buzzer_gpio_config.Mode = GPIO_MODE_AF_PP;
	buzzer_gpio_config.Pin = GPIO_PIN_9;
	buzzer_gpio_config.Pull = GPIO_NOPULL;
	buzzer_gpio_config.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOF,&buzzer_gpio_config);
}

void PWM_Init(void)
{
	__HAL_RCC_TIM14_CLK_ENABLE();
	TIM_HandleTypeDef tim_pwm_config;
	TIM_OC_InitTypeDef pwm_channel_config;
	tim_pwm_config.Instance = TIM14;
	tim_pwm_config.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	tim_pwm_config.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	tim_pwm_config.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim_pwm_config.Init.Period = 1000-1;
	tim_pwm_config.Init.Prescaler = 84-1;
	tim_pwm_config.Init.RepetitionCounter = 0;
	HAL_TIM_PWM_Init(&tim_pwm_config);
	PWM_MspInit();
	pwm_channel_config.OCFastMode = TIM_OCFAST_ENABLE;
	pwm_channel_config.OCIdleState = TIM_OCIDLESTATE_RESET;
	pwm_channel_config.OCMode = TIM_OCMODE_PWM1;
	pwm_channel_config.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	pwm_channel_config.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	pwm_channel_config.OCPolarity = TIM_OCPOLARITY_HIGH;
	pwm_channel_config.Pulse = 200-1;
	HAL_TIM_PWM_ConfigChannel(&tim_pwm_config, &pwm_channel_config, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&tim_pwm_config, TIM_CHANNEL_1);
}

void TIM_PWM_Ctl(uint16_t frequency, uint32_t duty_cycle)
{
//	HAL_TIM_PWM_ConfigChannel();
}
