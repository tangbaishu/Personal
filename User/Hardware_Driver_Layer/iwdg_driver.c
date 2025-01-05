#include "iwdg_driver.h"
#include "stm32f4xx_hal.h"
#include "uart_debug_driver.h"

void IWDG_Init(void)
{	
    IWDG_HandleTypeDef IWDG_Config;
    IWDG_Config.Instance = IWDG;
    IWDG_Config.Init.Prescaler = IWDG_PRESCALER_256;
	IWDG_Config.Init.Reload = 375;
    HAL_IWDG_Init(&IWDG_Config);//??????? 4s ???????1 / (32000 / 256) * Reload
	if(READ_BIT(RCC->CSR,RCC_CSR_LSIRDY)==0)
	{
		printf("LSI_RC OFF\r\n");
	}
}


