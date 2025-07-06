/****************************************************************************
 * @copyright Copyright(C) 2020-2023 Ismartware Limited. All rights reserved.
 * @file zr_gpio.h
 * @brief Functions interfaces for the GPIO firmware library
 * @author David
 ****************************************************************************/
#ifndef ZR_GPIO_H
#define ZR_GPIO_H

#include "system.h"

#ifdef __cplusplus
extern "C" {
#endif

// clang-format off

/** gpio_id 0~14 */
#define GPIO0                (0x00000001)
#define GPIO1                (0x00000002)
#define GPIO2                (0x00000004)
#define GPIO3                (0x00000008)
#define GPIO4                (0x00000010)
#define GPIO5                (0x00000020)
#define GPIO6                (0x00000040)
#define GPIO7                (0x00000080)
#define GPIO8                (0x00000100)
#define GPIO9                (0x00000200)
#define GPIO10               (0x00000400)
#define GPIO11               (0x00000800)
#define GPIO12               (0x00001000)
#define GPIO13               (0x00002000)
#define GPIO14               (0x00004000)

// clang-format on

/** gpio function */
typedef enum
{
    FUNCTION_SEL0   = (uint8_t)(0x00000000),
    FUNCTION_SEL1   = (uint8_t)(0x00000001),
    FUNCTION_SEL2   = (uint8_t)(0x00000002),
    FUNCTION_SEL3   = (uint8_t)(0x00000003),
    FUNCTION_SEL4   = (uint8_t)(0x00000004),
    FUNCTION_SEL5   = (uint8_t)(0x00000005),
    FUNCTION_SEL6   = (uint8_t)(0x00000006),
    FUNCTION_SEL7   = (uint8_t)(0x00000007),
    FUNCTION_SEL8   = (uint8_t)(0x00000008),
    FUNCTION_SEL9   = (uint8_t)(0x00000009),
    FUNCTION_SELA   = (uint8_t)(0x0000000A),
    FUNCTION_SELB   = (uint8_t)(0x0000000B),
    FUNCTION_SELC   = (uint8_t)(0x0000000C),
    FUNCTION_SELD   = (uint8_t)(0x0000000D),
    FUNCTION_SELE   = (uint8_t)(0x0000000E),
    FUNCTION_SELF   = (uint8_t)(0x0000000F),
}gpio_func_e;

/** gpio direction */
typedef enum
{
    GPIO_DIRECTION_INPUT = 0,        //!< input mode
    GPIO_DIRECTION_OUTPUT = 1        //!< output mode
} gpio_direction_e;

/** gpio pulldown */
typedef enum
{
    PULLDOWN_DIS = 0,        //!< pulldown disable
    PULLDOWN_EN = 1          //!< pulldown enable
} gpio_pulldown_e;

/** gpio pullup */
typedef enum
{
    PULLUP_DIS = 0,        //!< pullup disable
    PULLUP_EN = 1          //!< pullup enable
} gpio_pullup_e;

/** gpio pmos open drain */
typedef enum
{
    PMOS_OPDRAIN_DIS = 0,        //!< pmos opendrain disable
    PMOS_OPDRAIN_EN = 1          //!< pmos opendrain enable
} gpio_pmos_opendrain_e;

/** gpio nmos open drain */
typedef enum
{
    NMOS_OPDRAIN_DIS = 0,        //!< nmos opendrain disable
    NMOS_OPDRAIN_EN = 1          //!< nmos opendrain enable
} gpio_nmos_opendrain_e;

/** pullup resistance */
typedef enum
{
    PULLUP_RES_10K = (uint8_t)(0x00000000),        //!< pullup resistance 10k
    PULLUP_RES_4K = (uint8_t)(0x00000001),         //!< pullup resistance 4k
} gpio_pullup_resistance_e;

/** gpio config */
typedef struct
{
    gpio_func_e gpioFunc;                   //!< gpioFunc, see gpio_func_e
    gpio_direction_e direction;             //!< direction, see gpio_direction_e
    gpio_pulldown_e pulldown;               //!< pulldown, see gpio_pulldown_e
    gpio_pullup_e pullup;                   //!< pullup, see gpio_pullup_e
    gpio_pmos_opendrain_e pmosOd;           //!< pmos opendrain, see gpio_pmos_opendrain_e
    gpio_nmos_opendrain_e nmosOd;           //!< nmos opendrain, see gpio_nmos_opendrain_e
    gpio_pullup_resistance_e res;           //!< pullup resistance, see gpio_pullup_resistance_e
} gpio_config_t;

/** gpio interrupt type */
typedef enum
{
    LEVEL_TYPE = (uint8_t)(0x00000000),        //!< level trigger
    EDGE_TYPE = (uint8_t)(0x00000001),         //!< edge trigger
} gpio_interrupt_type_e;

/** gpio interrupt polarity */
typedef enum
{
    LOW_NEG_POLAR = (uint8_t)(0x00000000),         //!< low polarity
    HIGH_POS_POLAR = (uint8_t)(0x00000001),        //!< high polarity
    NEG_POS_POLAR = (uint8_t)(0x00000002)          //!< falling edge or rising edge (edge type)
} gpio_interrupt_polarity_e;

/**
 * @brief  Init GPIOx
 * @param  gpioId GPIO0~GPIO14
 * @param  cfg see gpio_config_t
 */
void Gpio_Init(u32 gpioId,const gpio_config_t* cfg);

/**
 * @brief  Deinit GPIOx
 * @param  gpioId GPIO0~GPIO14
 */
void Gpio_Deinit(u32 gpioId);

/** GPIO0 function selection*/
typedef enum
{
    GPIO0_FUNC_SEL_GPIO = 0x0,            //!< function select 0 : GPIO, default
    GPIO0_FUNC_SEL_UART_TX = 0x1,         //!< function select 1 : UART_TX
    GPIO0_FUNC_SEL_ATM = 0x2,             //!< function select 2 : ATM
    GPIO0_FUNC_SEL_UART_RX = 0x3,         //!< function select 3 : UART_RX
    GPIO0_FUNC_SEL_TFCP_SCK = 0x4,        //!< function select 4 : TFCP_SCK
    GPIO0_FUNC_SEL_TFCP_SDA = 0x5,        //!< function select 5 : TFCP_SDA
    GPIO0_FUNC_SEL_I2C_SCK = 0x6,         //!< function select 4 : I2C_SCK
    GPIO0_FUNC_SEL_I2C_SDA = 0x7,         //!< function select 5 : I2C_SDA
    GPIO0_FUNC_SEL_ADC = 0xF,             //!< function select 5 : ADC
} gpio0_func_sel_e;

/** GPIO1 function selection*/
typedef enum
{
    GPIO1_FUNC_SEL_GPIO = 0x0,            //!< function select 0 : GPIO, default
    GPIO1_FUNC_SEL_UART_RX = 0x1,         //!< function select 1 : UART_RX
    GPIO1_FUNC_SEL_ATM = 0x2,             //!< function select 2 : ATM
    GPIO1_FUNC_SEL_UART_TX = 0x3,         //!< function select 3 : UART_TX
    GPIO1_FUNC_SEL_TFCP_SDA = 0x4,        //!< function select 4 : TFCP_SDA
    GPIO1_FUNC_SEL_TFCP_SCK = 0x5,        //!< function select 5 : TFCP_SCK
    GPIO1_FUNC_SEL_I2C_SDA = 0x6,         //!< function select 4 : I2C_SDA
    GPIO1_FUNC_SEL_I2C_SCK = 0x7,         //!< function select 5 : I2C_SCK
    GPIO1_FUNC_SEL_ADC = 0xF,             //!< function select 5 : ADC
} gpio1_func_sel_e;

/** GPIO2 function selection*/
typedef enum
{
    GPIO2_FUNC_SEL_GPIO = 0x0,            //!< function select 0 : GPIO, default
    GPIO2_FUNC_SEL_I2C_SDA = 0x1,         //!< function select 1 : I2C_SDA
    GPIO2_FUNC_SEL_I2C_SCK = 0x2,         //!< function select 2 : I2C_SCK
    GPIO2_FUNC_SEL_TFCP_SDA = 0x3,        //!< function select 3 : TFCP_SDA
    GPIO2_FUNC_SEL_TFCP_SCK = 0x4,        //!< function select 4 : TFCP_SCK
    GPIO2_FUNC_SEL_UART_TX = 0x5,         //!< function select 5 : UART_TX
    GPIO2_FUNC_SEL_UART_RX = 0x6,         //!< function select 4 : UART_RX
    GPIO2_FUNC_SEL_ATM = 0x7,             //!< function select 5 : ATM
    GPIO2_FUNC_SEL_ADC = 0xF,             //!< function select 5 : ADC
} gpio2_func_sel_e;

/** GPIO3 function selection*/
typedef enum
{
    GPIO3_FUNC_SEL_GPIO = 0x0,            //!< function select 0 : GPIO, default
    GPIO3_FUNC_SEL_I2C_SCK = 0x1,         //!< function select 1 : I2C_SCK
    GPIO3_FUNC_SEL_I2C_SDA = 0x2,         //!< function select 2 : I2C_SDA
    GPIO3_FUNC_SEL_TFCP_SCK = 0x3,        //!< function select 3 : TFCP_SCK
    GPIO3_FUNC_SEL_TFCP_SDA = 0x4,        //!< function select 4 : TFCP_SDA
    GPIO3_FUNC_SEL_UART_RX = 0x5,         //!< function select 5 : UART_TX
    GPIO3_FUNC_SEL_UART_TX = 0x6,         //!< function select 4 : UART_RX
    GPIO3_FUNC_SEL_ATM = 0x7,             //!< function select 5 : ATM
    GPIO3_FUNC_SEL_ADC = 0xF,             //!< function select 5 : ADC
} gpio3_func_sel_e;

/** GPIO4 function selection*/
typedef enum
{
    GPIO4_FUNC_SEL_GPIO = 0x0,           //!< function select 0 : GPIO
    GPIO4_FUNC_SEL_I2C_SCK = 0x1,        //!< function select 1 : I2C_SCK
    GPIO4_FUNC_SEL_I2C_SDA = 0x2,        //!< function select 2 : I2C_SDA
    GPIO4_FUNC_SEL_UART_TX = 0x3,        //!< function select 3 : UART_TX
    GPIO4_FUNC_SEL_UART_RX = 0x4,        //!< function select 4 : UART_RX
    GPIO4_FUNC_SEL_SWD_SCK = 0x5,        //!< function select 5 : SWD_SCK
    GPIO4_FUNC_SEL_SWD_DIO = 0x6,        //!< function select 4 : SWD_DIO
    GPIO4_FUNC_SEL_CC11 = 0xF,           //!< function select 5 : CC11, default
} gpio4_func_sel_e;

/** GPIO5 function selection*/
typedef enum
{
    GPIO5_FUNC_SEL_GPIO = 0x0,           //!< function select 0 : GPIO
    GPIO5_FUNC_SEL_I2C_SDA = 0x1,        //!< function select 1 : I2C_SDA
    GPIO5_FUNC_SEL_I2C_SCK = 0x2,        //!< function select 2 : I2C_SCK
    GPIO5_FUNC_SEL_UART_RX = 0x3,        //!< function select 3 : UART_RX
    GPIO5_FUNC_SEL_UART_TX = 0x4,        //!< function select 4 : UART_TX
    GPIO5_FUNC_SEL_SWD_DIO = 0x5,        //!< function select 5 : SWD_DIO
    GPIO5_FUNC_SEL_SWD_SCK = 0x6,        //!< function select 4 : SWD_SCK
    GPIO5_FUNC_SEL_CC21 = 0xF,           //!< function select 5 : CC21, default
} gpio5_func_sel_e;

/** GPIO6 function selection*/
typedef enum
{
    GPIO6_FUNC_SEL_GPIO = 0x0,            //!< function select 0 : GPIO
    GPIO6_FUNC_SEL_I2C_SCK = 0x1,         //!< function select 1 : I2C_SCK
    GPIO6_FUNC_SEL_I2C_SDA = 0x2,         //!< function select 2 : I2C_SDA
    GPIO6_FUNC_SEL_TFCP_SCK = 0x3,        //!< function select 3 : TFCP_SCK
    GPIO6_FUNC_SEL_SWD_SCK = 0x4,         //!< function select 4 : SWD_SCK, default
    GPIO6_FUNC_SEL_TFCP_SDA = 0x5,        //!< function select 5 : TFCP_SDA
    GPIO6_FUNC_SEL_UART_TX = 0x6,         //!< function select 4 : UART_TX
    GPIO6_FUNC_SEL_UART_RX = 0x7,         //!< function select 4 : UART_RX
    GPIO6_FUNC_SEL_ATM = 0x8,             //!< function select 4 : ATM
    GPIO6_FUNC_SEL_ADC = 0xF,             //!< function select 5 : ADC
} gpio6_func_sel_e;

/** GPIO7 function selection*/
typedef enum
{
    GPIO7_FUNC_SEL_GPIO = 0x0,            //!< function select 0 : GPIO
    GPIO7_FUNC_SEL_I2C_SDA = 0x1,         //!< function select 1 : I2C_SDA
    GPIO7_FUNC_SEL_I2C_SCK = 0x2,         //!< function select 2 : I2C_SCK
    GPIO7_FUNC_SEL_TFCP_SDA = 0x3,        //!< function select 3 : TFCP_SDA
    GPIO7_FUNC_SEL_SWD_SDA = 0x4,         //!< function select 4 : SWD_SDA, default
    GPIO7_FUNC_SEL_TFCP_SCK = 0x5,        //!< function select 5 : TFCP_SCK
    GPIO7_FUNC_SEL_UART_RX = 0x6,         //!< function select 4 : UART_RX
    GPIO7_FUNC_SEL_UART_TX = 0x7,         //!< function select 4 : UART_TX
    GPIO7_FUNC_SEL_ATM = 0x8,             //!< function select 4 : ATM
    GPIO7_FUNC_SEL_ADC = 0xF,             //!< function select 5 : ADC
} gpio7_func_sel_e;

/** GPIO8 function selection*/
typedef enum
{
    GPIO8_FUNC_SEL_GPIO = 0x0,            //!< function select 0 : GPIO
    GPIO8_FUNC_SEL_TFCP_SCK = 0x1,        //!< function select 1 : TFCP_SCK
    GPIO8_FUNC_SEL_TFCP_SDA = 0x2,        //!< function select 2 : TFCP_SDA
    GPIO8_FUNC_SEL_UART_TX = 0x3,         //!< function select 3 : UART_TX
    GPIO8_FUNC_SEL_UART_RX = 0x4,         //!< function select 4 : UART_RX
    GPIO8_FUNC_SEL_SWD_SCK = 0x5,         //!< function select 5 : SWD_SCK
    GPIO8_FUNC_SEL_SWD_DIO = 0x6,         //!< function select 4 : SWD_DIO
    GPIO8_FUNC_SEL_DP1 = 0xF,             //!< function select 5 : DP1, default
} gpio8_func_sel_e;

/** GPIO9 function selection*/
typedef enum
{
    GPIO9_FUNC_SEL_GPIO = 0x0,            //!< function select 0 : GPIO
    GPIO9_FUNC_SEL_TFCP_SDA = 0x1,        //!< function select 1 : TFCP_SDA
    GPIO9_FUNC_SEL_TFCP_SCK = 0x2,        //!< function select 2 : TFCP_SCK
    GPIO9_FUNC_SEL_UART_RX = 0x3,         //!< function select 3 : UART_RX
    GPIO9_FUNC_SEL_UART_TX = 0x4,         //!< function select 4 : UART_TX
    GPIO9_FUNC_SEL_SWD_DIO = 0x5,         //!< function select 5 : SWD_DIO
    GPIO9_FUNC_SEL_SWD_SCK = 0x6,         //!< function select 4 : SWD_SCK
    GPIO9_FUNC_SEL_DM1 = 0xF,             //!< function select 5 : DM1, default
} gpio9_func_sel_e;

/** GPIO10 function selection*/
typedef enum
{
    GPIO10_FUNC_SEL_GPIO = 0x0,            //!< function select 0 : GPIO
    GPIO10_FUNC_SEL_I2C_SCK = 0x1,         //!< function select 1 : I2C_SCK
    GPIO10_FUNC_SEL_I2C_SDA = 0x2,         //!< function select 2 : I2C_SDA
    GPIO10_FUNC_SEL_TFCP_SCK = 0x3,        //!< function select 3 : TFCP_SCK
    GPIO10_FUNC_SEL_TFCP_SDA = 0x4,        //!< function select 4 : TFCP_SDA
    GPIO10_FUNC_SEL_UART_TX = 0x5,         //!< function select 5 : UART_TX
    GPIO10_FUNC_SEL_UART_RX = 0x6,         //!< function select 4 : UART_RX
    GPIO10_FUNC_SEL_SWD_SCK = 0x7,         //!< function select 4 : SWD_SCK
    GPIO10_FUNC_SEL_SWD_DIO = 0x8,         //!< function select 5 : SWD_DIO
    GPIO10_FUNC_SEL_ID1 = 0x9,             //!< function select 5 : ID1
    GPIO10_FUNC_SEL_CC12 = 0xF,            //!< function select 5 : CC12, default
} gpio10_func_sel_e;

/** GPIO11 function selection*/
typedef enum
{
    GPIO11_FUNC_SEL_GPIO = 0x0,            //!< function select 0 : GPIO
    GPIO11_FUNC_SEL_I2C_SDA = 0x1,         //!< function select 2 : I2C_SDA
    GPIO11_FUNC_SEL_I2C_SCK = 0x2,         //!< function select 1 : I2C_SCK
    GPIO11_FUNC_SEL_TFCP_SDA = 0x3,        //!< function select 4 : TFCP_SDA
    GPIO11_FUNC_SEL_TFCP_SCK = 0x4,        //!< function select 3 : TFCP_SCK
    GPIO11_FUNC_SEL_UART_RX = 0x5,         //!< function select 4 : UART_RX
    GPIO11_FUNC_SEL_UART_TX = 0x6,         //!< function select 5 : UART_TX
    GPIO11_FUNC_SEL_SWD_DIO = 0x7,         //!< function select 5 : SWD_DIO
    GPIO11_FUNC_SEL_SWD_SCK = 0x8,         //!< function select 4 : SWD_SCK
    GPIO11_FUNC_SEL_ID2 = 0x9,             //!< function select 5 : ID2
    GPIO11_FUNC_SEL_CC22 = 0xF,            //!< function select 5 : CC22, default
} gpio11_func_sel_e;

/** GPIO12 function selection*/
typedef enum
{
    GPIO12_FUNC_SEL_GPIO = 0x0,            //!< function select 0 : GPIO
    GPIO12_FUNC_SEL_TFCP_SCK = 0x1,        //!< function select 2 : TFCP_SCK
    GPIO12_FUNC_SEL_TFCP_SDA = 0x2,        //!< function select 1 : TFCP_SDA
    GPIO12_FUNC_SEL_UART_TX = 0x3,         //!< function select 4 : UART_TX
    GPIO12_FUNC_SEL_UART_RX = 0x4,         //!< function select 3 : UART_RX
    GPIO12_FUNC_SEL_SWD_SCK = 0x5,         //!< function select 4 : SWD_SCK
    GPIO12_FUNC_SEL_SWD_DIO = 0x6,         //!< function select 5 : SWD_DIO
    GPIO12_FUNC_SEL_DP2 = 0xF,             //!< function select 5 : DP2, default
} gpio12_func_sel_e;

/** GPIO13 function selection*/
typedef enum
{
    GPIO13_FUNC_SEL_GPIO = 0x0,            //!< function select 0 : GPIO
    GPIO13_FUNC_SEL_TFCP_SDA = 0x1,        //!< function select 1 : TFCP_SDA
    GPIO13_FUNC_SEL_TFCP_SCK = 0x2,        //!< function select 2 : TFCP_SCK
    GPIO13_FUNC_SEL_UART_RX = 0x3,         //!< function select 3 : UART_RX
    GPIO13_FUNC_SEL_UART_TX = 0x4,         //!< function select 4 : UART_TX
    GPIO13_FUNC_SEL_SWD_DIO = 0x5,         //!< function select 5 : SWD_DIO
    GPIO13_FUNC_SEL_SWD_SCK = 0x6,         //!< function select 4 : SWD_SCK
    GPIO13_FUNC_SEL_DM2 = 0xF,             //!< function select 5 : DM2, default
} gpio13_func_sel_e;

/** GPIO14 function selection*/
typedef enum
{
    GPIO14_FUNC_SEL_GPIO = 0x0,            //!< function select 0 : GPIO
    GPIO14_FUNC_SEL_I2C_SCK = 0x1,         //!< function select 1 : I2C_SCK
    GPIO14_FUNC_SEL_I2C_SDA = 0x2,         //!< function select 2 : I2C_SDA
    GPIO14_FUNC_SEL_TFCP_SCK = 0x3,        //!< function select 3 : TFCP_SCK
    GPIO14_FUNC_SEL_TFCP_SDA = 0x4,        //!< function select 5 : TFCP_SDA
    GPIO14_FUNC_SEL_UART_TX = 0x5,         //!< function select 4 : UART_TX
    GPIO14_FUNC_SEL_UART_RX = 0x6,         //!< function select 4 : UART_RX
    GPIO14_FUNC_SEL_ATM = 0x7,             //!< function select 4 : ATM
    GPIO14_FUNC_SEL_VD = 0xF,              //!< function select 5 : VD, default
} gpio14_func_sel_e;

/**
 * @brief  GPIOx function select
 * @param  gpioId GPIO0~GPIO14
 * @param  funSel function select, difference GPIO has different selection, see gpiox_func_sel_e or FUNCTION_SEL0~F
 */
void Gpio_Function_Select(u32 gpioId, u32 funSel);

/**
 * @brief  Set GPIOx direction
 * @param  gpioId GPIO0~GPIO14
 * @param  direction input or output direction mode
 */
void Gpio_Direction_Cfg(u32 gpioId, gpio_direction_e direction);

/**
 * @brief  Get GPIOx input value
 * @param  gpioId GPIO0~GPIO14
 * @return GPIOx input value:1 or 0
 */
u8 Gpio_Get_Input_Value(u32 gpioId);

/**
 * @brief  Set GPIOx output value 0 or 1
 * @param  gpioId GPIO0~GPIO14
 * @param  isHighLevel TRUE to set output is high level, otherwise to set low level
 */
void Gpio_Set_Output_Value(u32 gpioId, bool isHighLevel);

/**
 * @brief  Toggle GPIOx output value
 * @param  gpioId GPIO0~GPIO14
 */
void Gpio_Toggle_Output(u32 gpioId);

/**
 * @brief  GPIOx pulldown/pullup enable
 * @param  gpioId GPIO0~GPIO14
 * @param  pulldown:
 *         This parameter can be:
 *         PULLDOWN_EN
 *         PULLDOWN_DIS
 * @param  pullup:
 *         This parameter can be:
 *         PULLUP_EN
 *         PULLUP_DIS
 */
void Gpio_Pulldown_Pullup_Cfg(u32 gpioId, gpio_pulldown_e pulldown, gpio_pullup_e pullup);

/**
 * @brief  GPIOx pmos/nmos opendrain enable
 * @param  gpioId GPIO0~GPIO14
 * @param  pmos:
 *         This parameter can be:
 *         PMOS_OPDRAIN_EN
 *         PMOS_OPDRAIN_DIS
 * @param  nmos:
 *         This parameter can be:
 *         NMOS_OPDRAIN_EN
 *         NMOS_OPDRAIN_DIS
 */
void Gpio_Mos_Opendrain_Cfg(u32 gpioId, gpio_pmos_opendrain_e pmos, gpio_nmos_opendrain_e nmos);

/**
 * @brief  GPIOx pullup resistance config
 * @param  gpioId GPIO0~GPIO14
 * @param  res:
 *         This parameter can be:
 *         PULLUP_RES_4K
 *         PULLUP_RES_10K
 */
void Gpio_Pullup_Resistance_Cfg(u32 gpioId, gpio_pullup_resistance_e res);

/**
 * @brief  GPIOx interrupt enable
 * @param  gpioId GPIO0~GPIO14
 */
void Gpio_Interrupt_Enable(u32 gpioId);

/**
 * @brief  GPIOx interrupt disable
 * @param  gpioId GPIO0~GPIO14
 */
void Gpio_Interrupt_Disable(u32 gpioId);

/**
 * @brief  GPIOx interrupt mode configure
 * @param  gpioId GPIO0~GPIO14
 * @param  type interrupt type
 *         This parameter can be:
 *         EDGE_TYPE
 *         LEVEL_TYPE
 * @param  polarity interrupt polarity
 *         This parameter can be:
 *         HIGH_POS_POLAR high level or rising edge
 *         LOW_NEG_POLAR  low level or falling edge
 *         NEG_POS_POLAR  rising edge or falling edge
 */
void Gpio_Interrupt_Mode_Cfg(u32 gpioId, gpio_interrupt_type_e type, gpio_interrupt_polarity_e polarity);

/**
 * @brief  Get GPIOx interrupt pending
 * @param  gpioId GPIO0~GPIO14
 * @return The GPIOx interrupt pending(SET or NOTSET)
 *         SET:pending set    NOTSET:pending is not set
 */
ITStatus Gpio_Get_Int_Pending(u32 gpioId);

/**
 * @brief  GPIOx interrupt pending clear
 * @param  gpioId GPIO0~GPIO14
 */
void Gpio_Clear_Pending(u32 gpioId);

#ifdef __cplusplus
}
#endif

#endif
