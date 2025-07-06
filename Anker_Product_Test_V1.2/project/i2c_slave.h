/****************************************************************************
 * @copyright Copyright(C) 2020-2023 Ismartware Limited. All rights reserved.
 * @file i2c_slave.h
 * @brief Functions interfaces for the I2C firmware library
 * @author David
 ****************************************************************************/
#ifndef I2C_SLAVE_H
#define I2C_SLAVE_H

#ifdef __cplusplus
extern "C" {
#endif

// This demo demostarates how to use i2c slave reading in sleep mode
void I2c_Slave_Policy_Init(void);
void I2c_Slave_Policy_Run();

#ifdef __cplusplus
}
#endif

#endif

