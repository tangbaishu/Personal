#ifndef _FLASH_DRIVER_H
#define _FLASH_DRIVER_H

#include "stm32f4xx_hal.h"
#include "base_function.h"

#define ADDR_FLASH_SECTOR_0         ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1         ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2         ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3         ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4         ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5         ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6         ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7         ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8         ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9         ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10        ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11        ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */

#define FLASH_PARAM_SECTOR_NUMBER	(FLASH_SECTOR_5)
#define FLASH_PARAM_START_ADDR      (ADDR_FLASH_SECTOR_5)
#define FLASH_PARAM_BACKUP_ADDR		(ADDR_FLASH_SECTOR_2)
#define BOOT0_START_ADDRESS			(ADDR_FLASH_SECTOR_0)
#define BOOT1_START_ADDRESS			(ADDR_FLASH_SECTOR_5)
#define APP_START_ADDRESS			(ADDR_FLASH_SECTOR_6)
#define FLASH_PARAM_SEC_TOTAL       (1)

/**
 * @brief MCU flash扇区擦除
 * 
 * @param sector_err 
 * @param sector_number FLASH_SECTOR_0 ~ FLASH_SECTOR_11
 * @return Return_Status 
 */
Return_Status Flash_Sector_Erase(uint32_t sector_err, uint32_t sector_number);

Return_Status Flash_Write_Uint8(const uint32_t write_addr, const uint8 *write_p, uint16 write_len);
Return_Status Flash_Write_Uint32(const uint32_t write_addr, const uint32 *write_p, uint16 write_len);
void Flash_Read_Uint8(uint32_t base_address, uint8_t *read_buff, uint32_t r_char_len);
void Flash_Read_Uint32(uint32_t base_address, uint32_t *read_buff, uint32_t r_32bit_len);
void Test_Flash(void);
#endif
