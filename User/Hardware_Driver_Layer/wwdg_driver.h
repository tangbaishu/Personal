#ifndef _WWDG_DRIVER_H_
#define _WWDG_DRIVER_H_

#include "stm32f4xx_hal.h"
#include "iwdg_driver.h"
#define WWDG_REAL_OVERFLOW		10		// WWDG 溢出次数超过x次后，系统真实复位重启

extern uint8_t	WWDG_Full_Count;	// WWDG 溢出次数，若该值 ≥ WWDG_REAL_OVERFLOW，系统将复位重启

#define WDG_CLEAR()				WWDG->CR = 0xFE;WWDG_Full_Count=0;IWDG->KR = 0XAAAA

extern WWDG_HandleTypeDef WWDG_Config;

void WWDG_Init(void);
void WWDG_DeInit(void);

#endif
