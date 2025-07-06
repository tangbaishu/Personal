/****************************************************************************
 * @copyright Copyright(C) 2020-2023 Ismartware Limited. All rights reserved.
 * @file i2c_slave.c
 * @brief i2c slave read demo
 * @author Zeng jianbo
 ****************************************************************************/

#include "zr_i2c.h"
#include "zr_gpio.h"
#include "zr_rcc.h"
#include "system.h"
#include "zr_nvic.h"
#include "api.h"

/**
 *  This demo demostarates how to use i2c slave reading in sleep mode (zr2067 as slave)
 *  As the MCU performance is not stronger, it need time to waken system , so when system in sleep, the i2c slave has not
 *  enough time to prepare data. it is recommended to design a mechanism to make system has enough time to prepare time.
 *  below example shows a mechanism -> there is not sleep when there is a slave reading
 *  1) master write reg to tell slave to wakeup; (reg 0x8, data: 1)
 *  2) slave will waken and not go to sleep;
 *  3) master read slave data operations;  (reg 0x1, data length : 8)
 *  4) master write reg to tell slave can go to sleep;  (reg 0x8, data: 0)
 *  5) slave can go to sleep;
 */

void I2c_Slave_Policy_Init(void)
{
    Rcc_Module_Rst(I2C_ID);
    Rcc_Module_Clk_Enable(GPIO_ID | I2C_ID);
    Rcc_Module_Rst_Release(I2C_ID);
    Gpio_Function_Select(GPIO2, GPIO2_FUNC_SEL_I2C_SCK);
    Gpio_Function_Select(GPIO3, GPIO3_FUNC_SEL_I2C_SDA);

    Gpio_Mos_Opendrain_Cfg(GPIO2, PMOS_OPDRAIN_DIS, NMOS_OPDRAIN_EN);
    Gpio_Mos_Opendrain_Cfg(GPIO3, PMOS_OPDRAIN_DIS, NMOS_OPDRAIN_EN);
    Gpio_Pulldown_Pullup_Cfg(GPIO2, PULLDOWN_DIS, PULLUP_EN);
    Gpio_Pulldown_Pullup_Cfg(GPIO3, PULLDOWN_DIS, PULLUP_EN);

    I2c_Slave_Device_Addr_Cfg(0x3C);

    I2c_Slave_TX_FIFO_Reset();
    I2c_Slave_Int_Enable(I2C_S_TX_DATA_INT_EN | I2C_S_RX_DATA_INT_EN);
    // set to highest priority - > PRIORITY_0
    Nvic_Irq_Enable(I2CS_IRQn, PRIORITY_0);
    I2c_Slave_Enable();
    I2c_Slave_Clear_Pending(I2C_S_RX_DATA_PENGING | I2C_S_TX_DATA_PENGING | I2C_S_FINISH_PENGING
                            | I2C_S_RX_OVERFLOW_PENGING | I2C_S_TX_UNDERFLOW_PENGING | I2C_S_RX_FIFO_PENGING
                            | I2C_S_TX_FIFO_PENGING | I2C_S_BUS_ERROR_PENGING);
}

static const uint8_t gReg0x1Data[8] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8};
static uint8_t gReg0x8Data = 0;

void I2c_Slave_Policy_Run(void)
{
    if (gReg0x8Data)
    {
        Low_Power_Not_Sleep();
    } 
}

void I2CS_Handler()
{

    if (I2c_Slave_Get_Pending(I2C_S_TX_DATA_PENGING))
    {
        uint8_t regAddr = I2c_Slave_Get_Register_Address();
        if (regAddr == 0x1)
        {
            for (volatile uint8_t i = 0; i < 8; i++)
            {
                ZR_I2C->S_TX_DATA = gReg0x1Data[i];
            }
            I2c_Slave_Clear_Pending(I2C_S_TX_DATA_PENGING);
        }        
    }

    if (I2c_Slave_Get_Pending(I2C_S_RX_DATA_PENGING))
    {
        uint8_t regAddr = I2c_Slave_Get_Register_Address();
        uint8_t regData = 0x00;
        I2c_Slave_Get_RX_Data((u8*)&regData, 1);
        if (regAddr == 0x08)
        {
            gReg0x8Data = regData;
        }
        I2c_Slave_Clear_Pending(I2C_S_RX_DATA_PENGING);
    }

    // put to end of code
    Low_Power_Not_Sleep();
}
