/****************************************************************************
 * @copyright Copyright(C) 2020-2022 Ismartware Limited. All rights reserved.
 * @file i2cm_async.h
 * @brief i2c async master interface, the target is to create a service for
 * one master hardware to support mutiple i2c slave devices to avoid communication
 * confliction. The communication is asynchronization with rule first in first run.
 * @author SD1 team
 ****************************************************************************/
#ifndef I2CM_ASYNC_H
#define I2CM_ASYNC_H
#include "list_head.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct i2cs_device i2cs_device_t;
/// the callback function for on begining or on ending , they can be for pin mux preparion
typedef void (*on_begin_event)(i2cs_device_t* device);
typedef void (*on_end_event)(i2cs_device_t* device);

/**
 *  i2cs device struct to hold the slave private data,
 *  it is used by i2cm_async, the memory is alloacted the user to avoid dynamic memory allocation
 *  The Pin mux shall be guranted in outside see OnBeginEvent/OnEndEvent
 *  DON'T DIRECT CHANGE MEMBERS' VALUE
 */
struct i2cs_device
{
    list_head_t head;              //!< the head to hold next or prev i2c slave device
    on_begin_event onBegin;        //!< the event for start reading or writing, it can be used i2c pin mux selection
    on_end_event onEnd;        //!< the envent for end reading or writing, it can be used fallback to pin mux selection
    uint8_t* buffer;           //!< the buffer to hold read & write memory
    uint8_t bufferLen;         //!< the length of buffer
    uint8_t address;           //!< the i2c slave device address
    uint8_t writeLen;          //!< the number registers to read or write
    uint8_t readLen;           //!< the number registers to read or write
    uint8_t flag;              //!< the flag of operation runtime state
};

/** i2cm result */
typedef enum
{
    I2CM_RESULT_UNKNOWN,        //!< unknown
    I2CM_RESULT_SUCCESS,        //!< reading or writing successful
    I2CM_RESULT_FAILURE,        //!< reading or writing finish but failure
} i2cm_result_e;

/**
 * @brief init i2cm async
 * @param[in] freq the frequency of I2c master, unit : 1KHz  e.g. 100 -> 100KHz
 */
void I2cm_Async_Init(uint32_t freq);

/**
 * @brief deinit i2cm async
 */
void I2cm_Async_Deinit(void);

/**
 * @brief run i2cm async, shall be scheduled in main loop
 */
void I2cm_Async_Run(void);

/**
 * @brief Attach a slave device
 * @param[in,out] dev The slave device instance
 * @param[in] onBegin The on begin event callback
 * @param[in] onEnd The on end event callback
 * @param[in] address The slave device address, 7bits address only
 * @param[in] buffer The buffer memory to hold read and write data
 * @param[in] bufferLen The length of buffer memory
 */
i2cs_device_t* I2cm_Async_Attach(i2cs_device_t* dev, on_begin_event onBegin, on_end_event onEnd, uint8_t address,
    uint8_t* buffer, uint8_t bufferLen);

/**
 * @brief Detach a slave device
 * @param[in] dev The slave device instance
 */
void I2cm_Async_Detach(i2cs_device_t* dev);

/**
 * @brief Read data from slave device in specific address
 * @param[in] dev The slave device instance
 * @param[in] reg The start register address
 * @param[in] len The length of registers want to read
 * @return true when success to start reading, otherwise return false
 * @note The operation is asynchronization, repeated call I2cm_Async_Get_And_Clear_Finished() to get the operation
 * state, until I2CM_RESULT_SUCCESS is return, then call I2cm_Async_Get_Read_Data() to get read data.
 */
bool I2cm_Async_Read(i2cs_device_t* dev, uint8_t reg, uint8_t len);

/**
 * @brief Read data from slave device withtout specific address
 * @param[in] dev The slave device instance
 * @param[in] len The length of registers want to read
 * @return true when success to start reading, otherwise return false
 * @note The operation is asynchronization, repeated call I2cm_Async_Get_And_Clear_Finished() to get the operation
 * state, until I2CM_RESULT_SUCCESS is return, then call I2cm_Async_Get_Read_Data() to get read data.
 */
bool I2cm_Async_Read_Without_Register(i2cs_device_t* dev, uint8_t len);

/**
 * @brief Write data to slave device
 * @param[in] dev The slave device instance
 * @param[in] reg The start register address
 * @param[in] data The data want to write
 * @param[in] len The length of data, the max len is bufferLen - 1
 * @return true when success to start wrting, otherwise return false
 * @note The operation is asynchronization, repeated call I2cm_Async_Get_And_Clear_Finished() to get the operation
 * state, until I2CM_RESULT_SUCCESS is return.
 */
bool I2cm_Async_Write(i2cs_device_t* dev, uint8_t reg, const uint8_t* data, uint8_t len);

/**
 * @brief Get finish state and clear it
 * @param[in] dev The slave device instance
 * @return the read or write result see i2cm_result_e
 */
i2cm_result_e I2cm_Async_Get_And_Clear_Finished(i2cs_device_t* dev);

/**
 * @brief Get read data
 * @param[in] dev The slave device instance
 * @param[out] len The read data length
 * @return the pointer to read data
 */
uint8_t* I2cm_Async_Get_Read_Data(i2cs_device_t* dev, uint8_t* len);

#ifdef __cplusplus
}
#endif

#endif        // end of I2CM_ASYNC_H
