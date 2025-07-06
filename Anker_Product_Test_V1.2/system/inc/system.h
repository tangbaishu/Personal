/****************************************************************************
 * @copyright Copyright(C) 2020-2023 Ismartware Limited. All rights reserved.
 * @file system.h
 * @brief The system registers definition
 * @author SD1 team
 ****************************************************************************/

#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// clang-format off

/* ---------------------------- Interrupt Number Definition --------------------------------------*/
typedef enum IRQn
{
    /*Cortex-M0 Processor Exceptions Numbers */
    NonMaskableInt_IRQn           = -14,      /*!<  2 Cortex-M0 Non Maskable Interrupt            */
    HardFault_IRQn                = -13,      /*!<  3 Cortex-M0 Hard Fault Interrupt              */
    SVCall_IRQn                   = -5,       /*!< 11 Cortex-M0 SV Call Interrupt                 */
    PendSV_IRQn                   = -2,       /*!< 14 Cortex-M0 Pend SV Interrupt                 */
    SysTick_IRQn                  = -1,       /*!< 15 Cortex-M0 System Tick Interrupt             */

    /* System Specific Interrupt Numbers */
    UART_IRQn                     = 0,       /*!< UART Interrupt                                  */
    TYPEC0_IRQn                   = 1,       /*!< TYPE C 0 Interrupt                              */
    I2CM_IRQn                     = 2,       /*!< I2C Master Interrupt                            */
    PD_IRQn                       = 3,       /*!< PD Interrupt                                    */
    DPDM_IRQn                     = 4,       /*!< DPDM Interrupt                                  */
    I2CS_IRQn                     = 5,       /*!< I2C Slave Interrupt                              */
    GPIO_IRQn                     = 6,       /*!< GPIO Interrupt                                  */
    POWER_IRQn                    = 7,       /*!< POWER Interrupt                                 */
    TYPEC1_IRQn                   = 8,       /*!< TYPE C 1 Interrupt                              */
    TFCP_IRQn                     = 9,       /*!< TFCP Interrupt                                  */
    SCP_IRQn                      = 10,      /*!< SCP Interrupt                                   */    
    ADC_IRQn                      = 11,      /*!< ADC Interrupt                                  */
    UFCS_IRQn                     = 12,      /*!< UFCS Interrupt                                  */
    BTM0_IRQn                     = 13,      /*!< BTM0 Interrupt                                  */
    BTM1_IRQn                     = 14,      /*!< BTM1 Interrupt                                  */
    BTM2_IRQn                     = 15,      /*!< BTM2 Interrupt                                  */
    BTM3_IRQn                     = 16,      /*!< BTM3 Interrupt                                  */
    BTM4_IRQn                     = 17,      /*!< BTM4 Interrupt                                  */
    Reserved0_IRQn                = 18,      /*!< Reserved                                        */
    Reserved1_IRQn                = 19,      /*!< Reserved                                        */
    Reserved2_IRQn                = 20,      /*!< Reserved                                        */
    ATM0_IRQn                     = 21,      /*!< ATM0 Interrupt                                  */
    ATM1_IRQn                     = 22,      /*!< ATM1 Interrupt                                  */
    PLUG_IRQn                     = 23,      /*!< PLUG Interrupt                                  */    
    ABN_IRQn                      = 24,      /*!< ABN Interrupt                                   */        
    VOOC_IRQn                     = 25,      /*!< VOOC Interrupt                                  */    
}IRQn_Type;

/* ---------------------------- Interrupt Priority Definition --------------------------------------*/
typedef enum PRI
{
    PRIORITY_0                    = 0,    
    PRIORITY_1                    = 1,      
    PRIORITY_2                    = 2,      
    PRIORITY_3                    = 3,       
}Priority_Type;

/* -------------------------- Processor and Core Peripheral Section ------------------------------*/
/* Configuration of the Cortex-M0 Processor and Core Peripherals */
#define __CM0_REV                 0x0000    /*!< Core Revision r0p0                               */
#define __NVIC_PRIO_BITS          2         /*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used     */
#define __MPU_PRESENT             0         /*!< MPU present or not                               */

/*@}*/ /* end of group CMSDK_CMSIS */

#include "core_cm0.h"                       /* Cortex-M0 processor and core peripherals           */

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

/* exact-width signed integer types */
typedef int32_t   s32;
typedef int16_t   s16;
typedef int8_t    s8;

/* exact-width unsigned integer types */
typedef uint32_t  u32;
typedef uint16_t  u16;
typedef uint8_t   u8;

typedef enum 
{
    NOTSET = 0, 
    SET = !NOTSET
}FlagStatus, ITStatus;

#ifndef TRUE
#define TRUE true
#endif

#ifndef FALSE
#define FALSE false
#endif

typedef enum 
{
    DISABLE = 0,
    ENABLE = !DISABLE
}FunctionalState;

typedef enum 
{
    ERROR = 0,
    SUCCESS = !ERROR
}ErrorStatus;

#if defined ( __CC_ARM   )
#pragma no_anon_unions
#endif

/*------------------------------------ Return Types ---------------------------------- */
#define RET_OK      (0)
#define RET_FAIL    (-1)
#define RET_TIMEOUT (-2)

/*----------------  UART(Universal Asynchronous Receiver Transmitter) -----------------*/
typedef struct
{
    __IO  u8   DATA;             /*!< Offset: 0x000 Data Register             (R/W)    */
    __IO  u32  STATUS;           /*!< Offset: 0x004 Status Register           (R/W)    */
    __IO  u32  CTRL;             /*!< Offset: 0x008 Control Register          (R/W)    */
    __IO  u32  INTSTATUS;        /*!< Offset: 0x00C Interrupt Status Register (R/WC)   */
    __IO  u32  BAUDDIV;          /*!< Offset: 0x010 Baudrate Divider Register (R/W)    */
}ZR_UART_TypeDef;

/*---------------------------------- ATM(Advanced Timer) ------------------------------*/
typedef struct
{
    __IO  u32  CTRL;             /*!< Offset: 0x000 Control Register           (R/W)   */
    __I   u32  CURRENT;          /*!< Offset: 0x004 Current Value Register     (R)     */
    __IO  u32  RELOAD;           /*!< Offset: 0x008 Reload Value Register      (R/W)   */
    __IO  u32  INTSTATUS;        /*!< Offset: 0x00C Interrupt Status Register  (R/WC)  */
}ZR_ATM_TypeDef;

/*------------------------- GPIO(General Purpose Input Output) ------------------------*/
typedef struct
{
    __I   u32  DATAIN;           /*!< Offset: 0x000 DATA Input Register        (R)     */
    __IO  u32  DATAOUT;          /*!< Offset: 0x004 Data Output Latch Register (R/W)   */
    __IO  u32  OUTPUT_EN;        /*!< Offset: 0x008 Output Enable Register     (R/W)   */
          u32  RESERVED0;  
    __IO  u32  PULL_EN;          /*!< Offset: 0x010 Pullup Pulldown Enable Register(R/W)*/
    __IO  u32  OPDRAIN_EN;       /*!< Offset: 0x014 Open Drain Enable Register (R/W)   */
    __IO  u32  RESISTANCE;       /*!< Offset: 0x018 Pullup Resistance Register (R/W)   */
          u32  RESERVED1;     
    __IO  u32  INT_EN;           /*!< Offset: 0x020 Interrupt Enable Register  (R/W)   */
    __IO  u32  INT_TYPE;         /*!< Offset: 0x024 Interrupt Mode Register    (R/W)   */
    __IO  u32  INT_POL;          /*!< Offset: 0x028 Interrupt Status Register  (R/W)   */
    __IO  u32  INTSTATUS;        /*!< Offset: 0x02C Interrupt Status Register  (R/WC)  */
    __IO  u32  MFP_CTRL0;        /*!< Offset: 0x030 Function Select 0 Register (R/W)   */
    __IO  u32  MFP_CTRL1;        /*!< Offset: 0x034 Function Select 1 Register (R/W)   */
    __IO  u32  DBG_CTRL;         /*!< Offset: 0x038 Debug Control Register     (R/W)   */      
}ZR_GPIO_TypeDef;

/*----------------------------- RCC(Reset Clock Controller) ---------------------------*/
typedef struct
{
    __IO  u32  CLK_EN;           /*!< Offset: 0x000 Clock Enable Register     (R/W)    */
    __IO  u32  CLK_CFG;          /*!< Offset: 0x004 Clock Configure Register  (R/W)    */
          u32  RESERVED0[2];
    __IO  u32  RST_CFG0;         /*!< Offset: 0x010 Reset Configure 0 Register(R/W)    */
    __IO  u32  RST_CFG1;         /*!< Offset: 0x014 Reset Configure 1Register (R/W)    */
    __IO  u32  ANA_TEST_CFG0;    /*!< Offset: 0x018 Analog test Configure Register0(R/W)*/  
    __IO  u32  ANA_TEST_CFG1;    /*!< Offset: 0x01C Analog test Configure Register1(R/W)*/  
    __IO  u32  MEM_TEST_CFG;     /*!< Offset: 0x020 Memory test Configure Register(R/W)*/
    __IO  u32  SYS_CFG;          /*!< Offset: 0x024 System Configure Register (R/W)    */  
    __IO  u32  ANA_CFG;          /*!< Offset: 0x028 Analog Configure Register (R/W)    */   
          u32  RESERVED1;
    __I   u32  CHIP_VERSION;     /*!< Offset: 0x030 Chip Version Register     (R/W)    */     
}ZR_RCC_TypeDef;

/*------------------------------------- I2C -------------------------------------------*/
typedef struct
{
    __IO  u32  M_CTRL;           /*!< Offset: 0x000 Master Control Register   (R/W)    */
    __IO  u32  M_STATUS;         /*!< Offset: 0x004 Master Status Register    (R/WC)   */    
    __IO  u32  M_WR_DATA;        /*!< Offset: 0x008 Master Write Data Register(R/W)    */
    __I   u32  M_RD_DATA;        /*!< Offset: 0x00C Master Read Data Register (R)      */   
    __IO  u32  M_CFG;            /*!< Offset: 0x010 Master Configure Register (R/W)    */
          u32  RESERVED0[3];    
    __IO  u32  S_CTRL;           /*!< Offset: 0x020 Slave Control Register    (R/W)    */
    __IO  u32  S_STATUS;         /*!< Offset: 0x024 Slave Status Register     (R/WC)   */   
    __IO  u32  S_FIFO;           /*!< Offset: 0x028 Slave FIFO Register       (R/WC)   */       
    __IO  u32  S_TX_DATA;        /*!< Offset: 0x02C Slave TX Data Register    (R/W)    */
    __I   u32  S_RX_DATA;        /*!< Offset: 0x030 Slave RX Data Register    (R)      */   
}ZR_I2C_TypeDef;

/*-------------------------------- WDG(Watchdog) --------------------------------------*/
typedef struct
{
    __IO  u32  CTRL;             /*!< Offset: 0x000 Control Register          (R/W)    */
    __IO  u32  CURRENT;          /*!< Offset: 0x004 Current Value Register    (R/W)    */
    __IO  u32  RELOAD;           /*!< Offset: 0x008 Reload Value Register     (R/W)    */
    __IO  u32  INTSTATUS;        /*!< Offset: 0x00C Interrupt Status Register (R/WC)   */
}ZR_WDG_TypeDef;

// clang-format on

/*-------------------------------- Peripheral Memory Map-------------------------------*/
/* Peripheral and Memory base address */
#define APB_BASE (0x40000000UL)
#define AHB_BASE (0x40010000UL)

/* APB peripherals                 */
#define ATM0_BASE     (APB_BASE + 0x0000UL)
#define UART_BASE     (APB_BASE + 0x4000UL)
#define WATCHDOG_BASE (APB_BASE + 0x8000UL)
#define I2C_BASE      (APB_BASE + 0xC000UL)
#define RCC_BASE      (APB_BASE + 0xF000UL)

/* AHB peripherals                 */
#define GPIO_BASE (AHB_BASE + 0x0000UL)

/*------------------------------- Peripheral Declaration ------------------------------*/
#define ZR_ATM0 ((ZR_ATM_TypeDef*)ATM0_BASE)
#define ZR_UART ((ZR_UART_TypeDef*)UART_BASE)
#define ZR_WDG  ((ZR_WDG_TypeDef*)WATCHDOG_BASE)
#define ZR_I2C  ((ZR_I2C_TypeDef*)I2C_BASE)
#define ZR_RCC  ((ZR_RCC_TypeDef*)RCC_BASE)
#define ZR_GPIO ((ZR_GPIO_TypeDef*)GPIO_BASE)

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_H */
