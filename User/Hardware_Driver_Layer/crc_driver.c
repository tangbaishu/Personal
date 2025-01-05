#include "crc_driver.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_crc.h"
#include "base_function.h"
#include "uart_debug_driver.h"

CRC_HandleTypeDef crc_config;

void CRC_Driver_Init(void)
{
	__HAL_RCC_CRC_CLK_ENABLE();
	crc_config.Instance = CRC;
	crc_config.Lock = HAL_UNLOCKED;
	HAL_CRC_Init(&crc_config);
}

uint32_t CRC_Start(void)
{
	uint8_t test[] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x20,0x21};
	uint32_t calculate = 0;
	uint32_t crc_check_len=0;

	crc_check_len = sizeof(test)/4;
	if((sizeof(test) % 4) != 0)
	{
		crc_check_len += 1;
	}
	calculate = HAL_CRC_Calculate(&crc_config, (uint32_t *)&test[0], crc_check_len);
	DMA_Sprintf("CRC_Calaulate = %#x\r\n", calculate);
	return calculate;
}
