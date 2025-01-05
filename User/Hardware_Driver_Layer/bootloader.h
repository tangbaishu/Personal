#ifndef BootLoader_H_
#define	BootLoader_H_

//MCU底层寄存器映射&外设配置驱动层
#include "stm32f407xx.h"
#include "stm32f4xx_hal.h"

#define	BOOT_CODE			0
#define	APP_CODE			1
#define CODE_TYPE			APP

#define BOOT_START_ADDR		0x08000000						// boot固件运行地址
#define BOOT_SIZE			0x19000							// boot固件区大小
#define APP_START_ADDR		(BOOT_START_ADDR + BOOT_SIZE) 	// APP应用程序区起始地址

void Jump_Code_District(uint32_t en_jump_app);





#endif

