/****************************************************************************
 * @copyright Copyright(C) 2020-2023 Ismartware Limited. All rights reserved.
 * @file zr_i2c.h
 * @brief Functions interfaces for the I2C firmware library
 * @author David
 ****************************************************************************/
#ifndef ZR_I2C_H
#define ZR_I2C_H

#include "system.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// clang-format off
/* I2C master interrupt enable */

#define I2C_M_FINISH_INT_EN         (0x00000001)
#define I2C_M_ERROR_INT_EN          (0x00000002)
#define I2C_M_BUS_LOST_INT_EN       (0x00000004)
#define I2C_M_SCL_TIMEOUT_INT_EN    (0x00000008)

/* I2C master control */
#define I2C_M_START_BIT_EN          (0x00010000)
#define I2C_M_DEV_ADDR0_EN          (0x00020000)
#define I2C_M_WR_DATA_EN            (0x00040000)
#define I2C_M_RE_START_BIT_EN       (0x00080000)
#define I2C_M_DEV_ADDR1_EN          (0x00100000)
#define I2C_M_RD_DATA_EN            (0x00200000)
#define I2C_M_STOP_BIT_EN           (0x00400000)
#define I2C_M_SEND_SCL_EN           (0x00800000)
#define I2C_M_SCL_PULLDOWN_EN       (0x01000000)
#define I2C_M_START                 (0x80000000)

#define I2C_M_EN                    (0x80000000)  

/* I2C master interrupt pending flag */
#define I2C_M_FINISH_PENGING        (0x00000001)
#define I2C_M_NACK_PENGING          (0x00000002)
#define I2C_M_BUS_LOST_PENGING      (0x00000004)
#define I2C_M_SCL_TIMEOUT_PENGING   (0x00000008)

#define I2C_M_ALL_PENGING           (0xF)

/* I2C slave interrupt enable */
#define I2C_S_RX_DATA_INT_EN        (0x00000002)
#define I2C_S_TX_DATA_INT_EN        (0x00000004)
#define I2C_S_FINISH_INT_EN         (0x00000008)
#define I2C_S_RX_OVERFLOW_INT_EN    (0x00000010)
#define I2C_S_TX_UNDERFLOW_INT_EN   (0x00000020)
#define I2C_S_RX_FIFO_INT_EN        (0x00000040)
#define I2C_S_TX_FIFO_INT_EN        (0x00000080)
#define I2C_S_BUS_ERROR_INT_EN      (0x80000000)

/* I2C slave control */
#define I2C_S_EN                    (0x00000001)  

/* I2C slave interrupt pending flag */
#define I2C_S_RX_DATA_PENGING       (0x00000001)
#define I2C_S_TX_DATA_PENGING       (0x00000002)
#define I2C_S_FINISH_PENGING        (0x00000004)
#define I2C_S_RX_OVERFLOW_PENGING   (0x00000008)
#define I2C_S_TX_UNDERFLOW_PENGING  (0x00000010)
#define I2C_S_RX_FIFO_PENGING       (0x00000020)
#define I2C_S_TX_FIFO_PENGING       (0x00000040)
#define I2C_S_BUS_ERROR_PENGING     (0x80000000)


/* I2C slave fifo status & control  */
#define I2C_S_RX_FIFO_EMPTY         (0x00008000)
#define I2C_S_TX_FIFO_FULL          (0x80000000)
#define I2C_S_RX_FIFO_RESET         (0x00000100)
#define I2C_S_TX_FIFO_RESET         (0x01000000)


//!< force pulldown disable
#define I2C_S_FORCE_SCL_DIS (0x00000000)
//!< force pulldown enable
#define I2C_S_FORCE_SCL_EN (0x00010000)

//!< wait disable
#define I2C_S_WAIT_DIS (0x00000000)
//!< wait enable
#define I2C_S_WAIT_EN (0x00008000)

// clang-format on

/** i2cm config */
typedef struct
{
    uint8_t devAddr;           //!< devAddr, I2C master acknowledge device address
    uint8_t clockSpeed;        //!< clockSpeed, the clock frequency ,step of 1KHz and can not higher than 130KHz
} i2cm_config_t;

/** i2cs config */
typedef struct
{
    uint8_t devAddr;                        //!< devAddr, I2C slave own device address
    bool isSclPulldown;                     //!< SCL Pulldown, whether force SCL pulldown
    bool isWait;                            //!< wait, whether enable slave wait
} i2cs_config_t;

/**
 * @brief  Init I2C Master and enable it
 * @param  cfg see i2cm_config_t
 */
void I2c_Master_Init(i2cm_config_t* cfg);

/**
 * @brief  Deinit I2C
 */
void I2c_Master_Deinit(void);

/**
 * @brief  I2C master transfer start
 */
void I2c_Master_Start(void);

/**
 * @brief  I2C master enable
 */
void I2c_Master_Enable(void);

/**
 * @brief  I2C master disable
 */
void I2c_Master_Disable(void);

/**
 * @brief  I2C master interrupt enable
 * @param  interrupt I2C master interrupt
 *         This parameter can be any combination of the following values:
 *         I2C_M_FINISH_INT_EN
 *         I2C_M_ERROR_INT_EN
 *         I2C_M_BUS_LOST_INT_EN
 *         I2C_M_SCL_TIMEOUT_INT_EN
 */
void I2c_Master_Int_Enable(u32 interrupt);

/**
 * @brief  I2C master interrupt disable
 * @param  interrupt I2C master interrupt
 *         This parameter can be any combination of the following values:
 *         I2C_M_FINISH_INT_EN
 *         I2C_M_ERROR_INT_EN
 *         I2C_M_BUS_LOST_INT_EN
 *         I2C_M_SCL_TIMEOUT_INT_EN
 */
void I2c_Master_Int_Disable(u32 interrupt);

/**
 * @brief  Set I2C master frequency
 * @param  freq I2C master frequency, step of 1KHz
 */
void I2c_Master_Freq_Set(uint8_t freq);

/**
 * @brief  Set I2C device address
 * @param  device_addr I2C device address(7 bits)
 */
void I2c_Master_Device_Addr_Set(u8 device_addr);

/**
 * @brief  Get I2C master pending flag set or not
 * @param  pending I2C master pending
 *         This parameter can be:
 *         I2C_M_FINISH_PENGING
 *         I2C_M_NACK_PENGING
 *         I2C_M_BUS_LOST_PENGING
 *         I2C_M_SCL_TIMEOUT_PENGING
 * @return The I2C master flag (SET or NOTSET)
 */
FlagStatus I2c_Master_Get_Pending(u32 pending);

/**
 * @brief  Clear I2C master pending flag
 * @param  pending I2C master pending
 *         This parameter can be:
 *         I2C_M_FINISH_PENGING
 *         I2C_M_NACK_PENGING
 *         I2C_M_BUS_LOST_PENGING
 *         I2C_M_SCL_TIMEOUT_PENGING
 */
void I2c_Master_Clear_Pending(u32 pending);

/**
 * @brief  Get I2C master busy or idle flag
 * @return The I2C master busy flag (SET or NOTSET)
 *         SET busy   NOTSET:idle
 */
FlagStatus I2c_Master_Busy_Flag(void);

/**
 * @brief  I2C master clock pulldown timer enable and config
 * @param  n 4bits,SCL pull down timer, timer=(n+1)*8ms
 */
void I2c_Master_Clock_Pulldown_Timer_Enable(u8 n);

/**
 * @brief  I2C master clock pulldown timer disable
 */
void I2c_Master_Clock_Pulldown_Timer_Disable(void);

/**
 * @brief  Configure I2C master write byte length
 * @param  len write byte length
 *         This parameter can be:
 *         1~32 byte length
 */
void I2c_Master_WR_Byte_Length_Cfg(u8 len);

/**
 * @brief  Configure I2C master read byte length
 * @param  len read byte length
 *         This parameter can be:
 *         1~32 byte length
 */
void I2c_Master_RD_Byte_Length_Cfg(u8 len);

/**
 * @brief  I2C master start bit enable
 */
void I2c_Master_Start_Bit_Enable(void);

/**
 * @brief  I2C master start bit disable
 */
void I2c_Master_Start_Bit_Disable(void);

/**
 * @brief  I2C master device address 0 enable
 */
void I2c_Master_Device_Addr0_Enable(void);

/**
 * @brief  I2C master device address 0 disable
 */
void I2c_Master_Device_Addr0_Disable(void);

/**
 * @brief  I2C master device address 1 enable
 */
void I2c_Master_Device_Addr1_Enable(void);

/**
 * @brief  I2C master device address 1 disable
 */
void I2c_Master_Device_Addr1_Disable(void);

/**
 * @brief  I2C master write data enable
 */
void I2c_Master_WR_Data_Enable(void);

/**
 * @brief  I2C master write data disable
 */
void I2c_Master_WR_Data_Disable(void);

/**
 * @brief  I2C master read data enable
 */
void I2c_Master_RD_Data_Enable(void);

/**
 * @brief  I2C master read data disable
 */
void I2c_Master_RD_Data_Disable(void);

/**
 * @brief  I2C master re-start bit enable
 */
void I2c_Master_Restart_Bit_Enable(void);

/**
 * @brief  I2C master re-start bit disable
 */
void I2c_Master_Restart_Bit_Disable(void);

/**
 * @brief  I2C master stop bit enable
 */
void I2c_Master_Stop_Bit_Enable(void);

/**
 * @brief  I2C master stop bit disable
 */
void I2c_Master_Stop_Bit_Disable(void);

/**
 * @brief  I2C master send I2C clock enable
 */
void I2c_Master_Send_Clock_Enable(void);

/**
 * @brief  I2C master send I2C clock disable
 */
void I2c_Master_Send_Clock_Disable(void);

/**
 * @brief  Get I2C master write data fifo data left
 * @return Write data fifo data left
 *         This parameter can be:
 *         0~8 byte
 */
u8 I2c_Master_Get_WR_FIFO_Left(void);

/**
 * @brief  Get I2C master read data fifo data left
 * @return Read data fifo data left
 *         This parameter can be:
 *         0~8 byte
 */
u8 I2c_Master_Get_RD_FIFO_Left(void);

/**
 * @brief  I2c master prepare async read
 * @param[in]  rdBytes The bytes want to read
 * @param[in]  wrBytes The bytes want to write
 * @return I2C bus busy status (SET or NOTSET)
 *         SET: busy
 *         NOTSET: idle
 */
FlagStatus I2c_Master_Prepare_Async_Read(uint8_t rdBytes, uint8_t wrBytes);

/**
 * @brief  I2c master prepare async write
 * @param[in]  bytes The bytes want to write
 * @return I2C bus busy status (SET or NOTSET)
 *         SET: busy
 *         NOTSET: idle
 */
FlagStatus I2c_Master_Prepare_Async_Write(uint8_t bytes);

/**
 * @brief  I2c master async write
 * @param[in]  wrData The data want to write
 * @param[in]  bytes The bytes want to write
 * @return uint8_t the written bytes
 */
uint8_t I2c_Master_Async_Write(uint8_t* wrData, uint8_t bytes);

/**
 * @brief  I2c master async read
 * @param[in]  rdData The data to hold the reading
 * @param[in]  bytes The bytes want to read
 * @return uint8_t the read bytes
 */
uint8_t I2c_Master_Async_Read(uint8_t* rdData, uint8_t bytes);

/**
 * @brief  Write data to write data fifo
 * @param  data The write data
 */
void I2c_Master_WR_Data(u8 data);

/**
 * @brief  Get read data from read data fifo
 * @return uint8_t, The read data
 */
u8 I2c_Master_RD_Data(void);

/**
 * @brief  Get I2C bus busy status
 * @return I2C bus busy status (SET or NOTSET)
 *         SET busy
 *         NOTSET idle
 */
FlagStatus I2c_Get_Bus_Busy_Status(void);

/**
 * @brief  I2C master write data to I2C slave, blocked operation
 * @param  wr_data the write data(register address + data) to I2C slave
 * @param  bytes the number byte of write data(register address + data)
 *         This parameter can be:
 *         1~32 byte
 * @return Transfer error flag (SET or NOTSET)
 */
FlagStatus I2c_Master_Write_Data(u8* wr_data, u8 bytes);

/**
 * @brief  I2C master read data from I2C slave, blocked operation
 * @param  wr_data the write data(register address + data) to I2C slave
 * @param  wr_bytes the number byte of write data(register address + data)
 *         This parameter can be:
 *         1~32 byte
 * @param  rd_data read data from I2C slave
 * @param  rd_bytes the number byte of read data
 *         This parameter can be:
 *         1~32 byte
 * @return Transfer error flag (SET or NOTSET)
 */
FlagStatus I2c_Master_Read_Data(u8* wr_data, u8 wr_bytes, u8* rd_data, u8 rd_bytes);

/**
 * @brief  Init I2C Slave and enable it
 * @param  cfg see i2cs_config_t
 */
void I2c_Slave_Init(i2cs_config_t* cfg);

/**
 * @brief  Init I2C Slave
 */
void I2c_Slave_Deinit(void);

/**
 * @brief  I2C slave enable
 */
void I2c_Slave_Enable(void);

/**
 * @brief  I2C slave disable
 */
void I2c_Slave_Disable(void);

/**
 * @brief  I2C slave device address configure
 * @param  device_addr I2C device address(7 bits)
 */
void I2c_Slave_Device_Addr_Cfg(u8 device_addr);

/**
 * @brief  I2C slave interrupt enable
 * @param  interrupt I2C slave interrupt
 *         This parameter can be any combination of the following values:
 *         I2C_S_RX_DATA_INT_EN
 *         I2C_S_TX_DATA_INT_EN
 *         I2C_S_FINISH_INT_EN
 *         I2C_S_RX_OVERFLOW_INT_EN
 *         I2C_S_TX_UNDERFLOW_INT_EN
 *         I2C_S_RX_FIFO_INT_EN
 *         I2C_S_TX_FIFO_INT_EN
 *         I2C_S_BUS_ERROR_INT_EN
 */
void I2c_Slave_Int_Enable(u32 interrupt);

/**
 * @brief  I2C slave interrupt disable
 * @param  interrupt I2C slave interrupt
 *         This parameter can be any combination of the following values:
 *         I2C_S_RX_DATA_INT_EN
 *         I2C_S_TX_DATA_INT_EN
 *         I2C_S_FINISH_INT_EN
 *         I2C_S_RX_OVERFLOW_INT_EN
 *         I2C_S_TX_UNDERFLOW_INT_EN
 *         I2C_S_RX_FIFO_INT_EN
 *         I2C_S_TX_FIFO_INT_EN
 *         I2C_S_BUS_ERROR_INT_EN
 */
void I2c_Slave_Int_Disable(u32 interrupt);

/**
 * @brief  I2C slave wait start enable
 */
void I2c_Slave_Wait_Enable(void);

/**
 * @brief  I2C slave wait start disable
 */
void I2c_Slave_Wait_Disable(void);

/**
 * @brief  I2C slave force scl pulldown enable
 */
void I2c_Slave_Force_SCL_Enable(void);

/**
 * @brief  I2C slave force scl pulldown disable
 */
void I2c_Slave_Force_SCL_Disable(void);

/**
 * @brief  Get I2C slave pending flag set or not
 * @param  pending I2C slave pending
 *         This parameter can be any combination of the following values:
 *         I2C_S_RX_DATA_PENGING
 *         I2C_S_TX_DATA_PENGING
 *         I2C_S_FINISH_PENGING
 *         I2C_S_RX_OVERFLOW_PENGING
 *         I2C_S_TX_UNDERFLOW_PENGING
 *         I2C_S_RX_FIFO_PENGING
 *         I2C_S_TX_FIFO_PENGING
 *         I2C_S_BUS_ERROR_PENGING
 * @return The I2C slave flag (SET or NOTSET)
 */
FlagStatus I2c_Slave_Get_Pending(u32 pending);

/**
 * @brief  Clear I2C slave pending flag
 * @param  pending I2C slave pending
 *         This parameter can be any combination of the following values:
 *         I2C_S_RX_DATA_PENGING
 *         I2C_S_TX_DATA_PENGING
 *         I2C_S_FINISH_PENGING
 *         I2C_S_RX_OVERFLOW_PENGING
 *         I2C_S_TX_UNDERFLOW_PENGING
 *         I2C_S_RX_FIFO_PENGING
 *         I2C_S_TX_FIFO_PENGING
 *         I2C_S_BUS_ERROR_PENGING
 */
void I2c_Slave_Clear_Pending(u32 pending);

/**
 * @brief  Get I2C slave received byte length
 * @return I2C slave received byte length(5bit)
 *         This parameter can be:
 *         0~31
 */
u8 I2c_Slave_Get_RX_Byte_Length(void);

/**
 * @brief  Get I2C slave transmit byte length
 * @return I2C salve transmit byte length(5bit)
 *         This parameter can be:
 *         0~31
 */
u8 I2c_Slave_Get_TX_Byte_Length(void);

/**
 * @brief  Get I2C slave register address
 * @return I2C slave register address(8bit)
 */
u8 I2c_Slave_Get_Register_Address(void);

/**
 * @brief  Get I2C slave RX fifo empty flag set or not
 * @return The I2C slave RX fifo empty flag (SET or NOTSET)
 */
FlagStatus I2c_Slave_Get_RX_FIFO_Empty_Flag(void);

/**
 * @brief  Get I2C slave TX fifo full flag set or not
 * @return The I2C slave TX fifo full flag (SET or NOTSET)
 */
FlagStatus I2c_Slave_Get_TX_FIFO_Full_Flag(void);

/**
 * @brief  I2C slave RX fifo trigger level configure
 * @param  level RX fifo trigger level(4 bits)
 *         This parameter can be:
 *         1~8
 */
void I2c_Slave_RX_FIFO_Trigger_Level_Cfg(u8 level);

/**
 * @brief  I2C slave TX fifo trigger level configure
 * @param  level TX fifo trigger level(4 bits)
 *         This parameter can be:
 *         1~8
 */
void I2c_Slave_TX_FIFO_Trigger_Level_Cfg(u8 level);

/**
 * @brief  I2C slave RX FIFO reset
 */
void I2c_Slave_RX_FIFO_Reset(void);

/**
 * @brief  I2C slave TX FIFO reset
 */
void I2c_Slave_TX_FIFO_Reset(void);

/**
 * @brief  Get I2C slave received data, blocked operation
 * @param  rx_data rx data
 * @param  byte_length rx data byte length(5bit)
 *         This parameter can be:
 *         0~31
 */
void I2c_Slave_Get_RX_Data(u8* rx_data, u8 byte_length);

/**
 * @brief  Write I2C slave transmit data, blocked operation
 * @param  tx_data tx data
 * @param  byte_length tx data byte length(5bit)
 *         This parameter can be:
 *         0~31
 */
void I2c_Slave_Write_TX_Data(u8* tx_data, u8 byte_length);

/**
 * @brief  Get I2C slave TX fifo data left
 * @return TX fifo data left
 *         This parameter can be:
 *         0~8 byte
 */
u8 I2c_Slave_Get_TX_FIFO_Left(void);

/**
 * @brief  Get I2C slave RX fifo data left
 * @return RX fifo data left
 *         This parameter can be:
 *         0~8 byte
 */
u8 I2c_Slave_Get_RX_FIFO_Left(void);

#ifdef __cplusplus
}
#endif

#endif
