#include "flash_driver.h"
#include "uart_debug_driver.h"
#include "wwdg_driver.h"
#include "stm32f4xx_hal_flash_ex.h"
#include <string.h>

#define LOG	"Flash: "

void Test_Flash(void)
{
	uint8 test_buff[8]={0x0F,0x0E,0x0D,0x0C,0xA1,0xB2,0xC3,0xD4};
	DMA_Sprintf(LOG"Start Erase Sector\r\n");
	Flash_Sector_Erase(FLASH_PARAM_START_ADDR, FLASH_PARAM_SECTOR_NUMBER);
	DMA_Sprintf(LOG"Erase Sector Finish\r\nStart Write Byte\r\n");
    Flash_Write_Uint8(FLASH_PARAM_START_ADDR, test_buff, 8);
	DMA_Sprintf(LOG"Write Byte Finish\r\nStart Read Byte\r\n");
    Flash_Read_Uint8(FLASH_PARAM_START_ADDR, (uint8 *)test_buff, 8);
	DMA_Sprintf(LOG"Read Byte Finish\r\n");
}

/**
 * @brief MCU flash扇区擦除
 * 
 * @param sector_err 
 * @param sector_number FLASH_SECTOR_0 ~ FLASH_SECTOR_11
 * @return Return_Status 
 */
Return_Status Flash_Sector_Erase(uint32_t sector_err, uint32_t sector_number)
{
	FLASH_EraseInitTypeDef flash_erase_config;
	flash_erase_config.Banks = FLASH_BANK_1;					// 针对所有用户扇区激活擦除操作
	flash_erase_config.NbSectors = 1;							// 待擦除扇区数量
	flash_erase_config.Sector = sector_number;					// 待擦除的扇区
	flash_erase_config.TypeErase = FLASH_TYPEERASE_SECTORS;		// 仅扇区擦除
	flash_erase_config.VoltageRange = FLASH_VOLTAGE_RANGE_3; 	// 2.7~3.6V 32位字节擦除
	WDG_CLEAR();
	if(READ_BIT(WWDG->CR,WWDG_CR_WDGA))
    {
        WWDG_DeInit();
		DMA_Sprintf("WWDG_DeInit\r\n");
    }
	HAL_FLASH_Unlock();
	if( HAL_OK != HAL_FLASHEx_Erase(&flash_erase_config, &sector_err) )
	{
		printf("Error!!!\tFlase_Sector_Erase(): return no HAL_OK\r\n");
		if(READ_BIT(WWDG->CR,WWDG_CR_WDGA))
		{
			WWDG_Init();
			DMA_Sprintf_ISR("WWDG_Init\r\n");
		}
		return RETURN_ERROR;
	}
	HAL_FLASH_Lock();
	if(READ_BIT(WWDG->CR,WWDG_CR_WDGA))
    {
        WWDG_Init();
		DMA_Sprintf_ISR("WWDG_Init\r\n");
    }
	return RETURN_SUCCESS;
}

Return_Status Flash_Write_Uint8(uint32_t write_addr, const uint8 *write_p, uint16 write_len)
{
	volatile uint16 write_p_offset = 0;
	uint32 write_buff=0;
	HAL_FLASH_Unlock();
	while( write_p_offset < write_len )
	{
		WDG_CLEAR();
		memcpy((uint8 *)&write_buff, &write_p[write_p_offset], 4);
		
		if(HAL_OK == HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, write_addr, write_buff))
		{
			DMA_Sprintf("write_p_offset = %ld\r\n",write_p_offset);
			DMA_Sprintf("HAL_FLASH_Write Success\r\n");
		}
		else
		{
			DMA_Sprintf("HAL_FLASH_Write Fail\r\n");
		}
		write_addr += 4;
		write_p_offset += 4;
	}
	FLASH_FlushCaches();
	HAL_FLASH_Lock();
	return RETURN_SUCCESS;
}

void Flash_Read_Uint8(uint32_t base_address, uint8_t *read_buff, uint32_t r_char_len)
{
	uint32_t *read_addr = (uint32_t *)base_address;
	volatile uint32_t r_char_num = 0;
	uint8_t addr_offset = 0;
	uint32_t address = base_address;
	while(1)
	{
		DMA_Sprintf("read_addr = 0x%p, value=0x%x\r\n", read_addr, *read_addr);
		address += 4;
		for(addr_offset = 0; addr_offset < 4; addr_offset++)
		{
			read_buff[addr_offset] = (uint8_t)(*read_addr >> (8 * addr_offset));	// 将指针指向的32位地址上的4个字节逐个赋值给read_buff
			DMA_Sprintf("read_buff[%d] = 0x%x\r\n", addr_offset, read_buff[addr_offset]);
			r_char_num ++;													// 累加读取字节数量
			if(r_char_num >= r_char_len)		
			{
				return;
			}
		}
		read_addr++;	// 指针偏移32位
	}
}

Return_Status Flash_Write_Uint32(uint32_t write_addr, const uint32 *write_p, uint16 write_len)
{
	volatile uint16 write_p_offset = 0;
	HAL_FLASH_Unlock();
	for(; write_p_offset < write_len; write_p_offset++)
	{
		WDG_CLEAR();
		
		if(HAL_OK == HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, write_addr, *write_p))
		{
			DMA_Sprintf("write_p_offset = %ld\r\n",write_p_offset);
			DMA_Sprintf("HAL_FLASH_Write Success\r\n");
		}
		else
		{
			DMA_Sprintf("HAL_FLASH_Write Fail\r\n");
		}
		write_p++;
		write_addr += 4;
	}
	FLASH_FlushCaches();
	HAL_FLASH_Lock();
	return RETURN_SUCCESS;
}

void Flash_Read_Uint32(uint32_t base_address, uint32_t *read_buff, uint32_t r_32bit_len)
{
	uint32_t *read_addr = (uint32_t *)base_address;
	volatile uint32_t r_32bit_num = 0;
	for(; r_32bit_num < r_32bit_len; r_32bit_num++)	// 累加读取字节数量
	{
		*read_buff++ = (uint32_t)*read_addr++;	// 将指针指向的32位地址上的4个字节逐个赋值给read_buff
		DMA_Sprintf_ISR("read_addr = 0x%p, value=0x%x\r\n", read_addr-1, *(read_buff-1));
	}
}

void HAL_FLASH_OperationErrorCallback(uint32_t ReturnValue)
{
	DMA_Sprintf("Error!!!\tHAL_FLASH_OperationErrorCallback()\r\n");
}
