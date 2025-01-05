#include "wwdg_driver.h"

WWDG_HandleTypeDef WWDG_Config;
uint8_t	WWDG_Full_Count = 0;	// WWDG 溢出次数，若该值 ≥ WWDG_REAL_OVERFLOW，系统将复位重启


void WWDG_Init(void)
{
	__HAL_RCC_WWDG_CLK_ENABLE();
    WWDG_Config.Instance = WWDG;
	WWDG_Config.Init.Counter = 0x7E;
	WWDG_Config.Init.EWIMode = WWDG_EWI_ENABLE;// 复位前进入中断
	WWDG_Config.Init.Prescaler = WWDG_PRESCALER_8;
	WWDG_Config.Init.Window = 0x7F;
	CLEAR_BIT(WWDG->SR,WWDG_SR_EWIF);
	HAL_WWDG_Init(&WWDG_Config);
	HAL_NVIC_SetPriority(WWDG_IRQn,1,0);
	HAL_NVIC_EnableIRQ(WWDG_IRQn);
}

void WWDG_DeInit(void)
{
	__HAL_RCC_WWDG_CLK_DISABLE();
}
