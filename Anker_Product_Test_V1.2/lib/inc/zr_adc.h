/****************************************************************************
 * @copyright Copyright(C) 2020-2023 Ismartware Limited. All rights reserved.
 * @file zr_adc.h
 * @brief Functions interfaces for the Adc firmware library
 * @author Zeng jianbo
 ****************************************************************************/
#ifndef ZR_ADC_H
#define ZR_ADC_H

#include "system.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @page adc Adc
 * - Adc will be enabled automatically when there is a device attached, you can enable/disable it used
Adc_Enable()/Adc_Disable() too.
 * - Adc provides interfaces to get vin/vbus/die, etc.
 * - Adc provides generic 4 Adc channels interfaces
 * - Adc provides 4 channel comparators interfaces, the according Adc channel shall be enabled before.
 *
 * @par Example below shows how to use the generic Adc sampling and comparator
 *
@code

void Adc_Example(void)
{
    if (Adc_Is_Enabled())
    {
        Rcc_Module_Clk_Enable(GPIO_ID);
        Gpio_Function_Select(GPIO0, GPIO0_FUNC_SEL_ADC);
        Gpio_Function_Select(GPIO1, GPIO1_FUNC_SEL_ADC);
        Gpio_Function_Select(GPIO2, GPIO2_FUNC_SEL_ADC);
        Gpio_Function_Select(GPIO3, GPIO3_FUNC_SEL_ADC);

        adc_config_t adcCfg;
        Adc_Get_Config(&adcCfg);
        adcCfg.adc2Source = ADC2_SIG_SRC_GPIO2;
        adcCfg.adc3Source = ADC3_SIG_SRC_GPIO3;
        Adc_Set_Config(&adcCfg);

        Adc_Start_Adc_Channel(ADC_CHL_0);
        Adc_Start_Adc_Channel(ADC_CHL_1);
        Adc_Start_Adc_Channel(ADC_CHL_2);
        Adc_Start_Adc_Channel(ADC_CHL_3);
        adc_cmp_param_t param;
        param.mode = ADC_CMP_TRIGGER_MODE_POLL;
        param.op = ADC_CMP_OP_GREATER_OR_EQU;
        param.threshold = 1200/0.8;

        Adc_Start_Comparator(ADC_CHL_3, &param);

        while (1)
        {
            Systick_Delay_Ms(10);
            uint16_t data0 = Adc_Get_Channel_Data(ADC_CHL_0);
            uint16_t data1 = Adc_Get_Channel_Data(ADC_CHL_1);
            uint16_t data2 = Adc_Get_Channel_Data(ADC_CHL_2);
            uint16_t data3 = Adc_Get_Channel_Data(ADC_CHL_3);
            bool cmpResult = Adc_Get_Compartor_Result(ADC_CHL_3);
            printf("%d, %d, %d, %d, %d\n", data0, data1, data2, data3, cmpResult);
            Systick_Delay_Ms(10);
        }
    }
}
@endcode
*/

/**
 * @brief Adc enable
 */
void Adc_Enabled(void);

/**
 * @brief Adc disable
 */
void Adc_Disabled(void);

/**
 * @brief get Adc enable state
 * @return TRUE when Adc is enabled, otherwise return FALSE
 */
bool Adc_Is_Enabled(void);

/**
 * @brief  Get output voltage (move average)
 * @return output move average voltage in mV unit
 */
uint16_t Adc_Vout_Get(void);

/**
 * @brief  Get output voltage (raw)
 * @return output raw voltage, unit mV
 */
uint16_t Adc_Vout_Raw_Get(void);

/**
 * @brief  Get output current (move average)
 * @param portNum port number 0 or 1
 * @return output move average current in mA unit
 */
uint16_t Adc_Iout_Get(u8 portNum);

/**
 * @brief  Get output current (raw)
 * @param portNum port number 0 or 1
 * @return output raw current in mA unit
 */
uint16_t Adc_Iout_Raw_Get(u8 portNum);

/**
 * @brief  Get die temperature
 * @return die temperature, degree
 */
uint16_t Adc_Die_Get(void);

/**
 * @brief  Adc die channel force close enable or not 
 * @param isEnable, bool true is to enable force close otherwise not
 */
void Adc_Die_Force_Close(bool isEnable);

/**
 * @brief  Adc die channel is force closed or not
 * @param true die channel is force closed otherwise not
 */
bool Adc_Die_Is_Force_Closed(void);

/**
 * @brief  Get vin voltage, it is disabled in default, use Adc_Vin_Enable/Adc_Vin_Disable to control
 * @return vin voltage, mV
 */
uint16_t Adc_Vin_Get(void);

/**
 * @brief  enable vin sampling
 */
void Adc_Vin_Enable(void);

/**
 * @brief  disable vin sampling
 */
void Adc_Vin_Disable(void);

/// adc pullup current type
typedef enum
{
    ADC_PULLUP_CURRENT_DISABLE,        //!< disable
    ADC_PULLUP_CURRENT_5UA,            //!< 5 uA
    ADC_PULLUP_CURRENT_10UA,           //!< 10 uA
    ADC_PULLUP_CURRENT_20UA,           //!< 20 uA
    ADC_PULLUP_CURRENT_40UA,           //!< 40 uA
    ADC_PULLUP_CURRENT_60UA,           //!< 60 uA
    ADC_PULLUP_CURRENT_80UA,           //!< 80 uA
    ADC_PULLUP_CURRENT_100UA,          //!< 100 uA
} adc_pullup_current_e;

/// adc pullup current type
typedef enum
{
    ADC_SAMPLE_RATE_1KH,         //!< sample rate 1KH
    ADC_SAMPLE_RATE_750H,        //!< sample rate 750Hz
    ADC_SAMPLE_RATE_500H,        //!< sample rate 500Hz
    ADC_SAMPLE_RATE_250H,        //!< sample rate 250Hz
} adc_sample_rate_e;

/// adc channel 2 source
typedef enum
{
    ADC2_SIG_SRC_NONE,         //!< no source
    ADC2_SIG_SRC_GPIO2,        //!< GPIO2
    ADC2_SIG_SRC_GPIO3,        //!< GPIO3
    ADC2_SIG_SRC_GPIO6,        //!< GPIO6
    ADC2_SIG_SRC_GPIO7,        //!< GPIO7
    ADC2_SIG_SRC_CC12,         //!< CC12
    ADC2_SIG_SRC_CC22,         //!< CC22
    ADC2_SIG_SRC_VD,           //!< VD
} adc2_signal_source_e;

/// adc channel 3 source
typedef enum
{
    ADC3_SIG_SRC_NONE,         //!< no source
    ADC3_SIG_SRC_GPIO2,        //!< GPIO2
    ADC3_SIG_SRC_GPIO3,        //!< GPIO3
    ADC3_SIG_SRC_GPIO6,        //!< GPIO6
    ADC3_SIG_SRC_GPIO7,        //!< GPIO7
    ADC3_SIG_SRC_CC12,         //!< CC12
    ADC3_SIG_SRC_CC22,         //!< CC22
    ADC3_SIG_SRC_VD,           //!< VD
} adc3_signal_source_e;

/// adc channel
typedef enum
{
    ADC_CHL_0 = 0,        //!< adc channel 0
    ADC_CHL_1,            //!< adc channel 1
    ADC_CHL_2,            //!< adc channel 2
    ADC_CHL_3,            //!< adc channel 3
    ADC_VOUT,             //!< adc vout channel
    ADC_IOUT2,            //!< adc iout2 channel    
} adc_channel_e;

/// adc comparator operator
typedef enum
{
    ADC_CMP_OP_LESS = 0,              //!< less threshold
    ADC_CMP_OP_GREATER_OR_EQU,        //!< greater or equal threshold
} adc_cmp_op_e;

/// adc comparator trigger mode
typedef enum
{
    ADC_CMP_TRIGGER_MODE_POLL,             //!< poll mode, call Get_Compartor_Result() to get the result
    ADC_CMP_TRIGGER_MODE_INTERRUPT,        //!< interrupt mode, the callback will called when interrupt happen
} adc_cmp_trigger_mode_e;

/// callback for interrupt happen
typedef void (*AdcComparatorCallback)(adc_channel_e channel);

/// adc comparator parameter
typedef struct
{
    adc_cmp_op_e op : 1;                    //!< compartor operator
    adc_cmp_trigger_mode_e mode : 1;        //!< work mode
    uint16_t threshold : 12;                //!< threshold, 0.8 mv/bit
    uint32_t reserved : 18;                 //!< reserved
} adc_cmp_param_t;

/// high-end or low-end shunt resistance
typedef enum
{
    ADC_SHUT_RES_3MOHM,        //<! 3 mOhm
    ADC_SHUT_RES_5MOHM,        //!< 5 mOhm
} adc_shunt_resistance_e;

/// adc config
typedef struct
{
    adc_pullup_current_e adc0Pullup : 3;
    adc_pullup_current_e adc1Pullup : 3;
    adc_pullup_current_e adc2Pullup : 3;
    adc_pullup_current_e adc3Pullup : 3;

    adc_sample_rate_e adc0SampleRate : 2;
    adc_sample_rate_e adc1SampleRate : 2;
    adc_sample_rate_e adc2SampleRate : 2;
    adc_sample_rate_e adc3SampleRate : 2;

    adc2_signal_source_e adc2Source : 3;
    adc3_signal_source_e adc3Source : 3;

    adc_shunt_resistance_e adcRes : 1;
} adc_config_t;

/**
 * @brief get Adc config
 * @param[in] value The config see adc_config_t
 */
void Adc_Get_Config(adc_config_t* value);

/**
 * @brief set Adc config
 * @param[in] value The config see adc_config_t
 */
void Adc_Set_Config(const adc_config_t* value);

/**
 * @brief register a comparator callback, needed only for ADC_CMP_TRIGGER_MODE_INTERRUPT mode
 * @param[in] callback The callback function
 */
void Adc_Register_Comparator_Callback(AdcComparatorCallback callback);

/**
 * @brief Start a comparator, the according adc channel shall be started before
 * @param[in] channel The Adc channel see adc_channel_e
 * @param[in] paramter The paramter for channel
 */
void Adc_Start_Comparator(adc_channel_e channel, const adc_cmp_param_t* paramter);

/**
 * @brief Stop a comparator
 * @param[in] channel The Adc channel see adc_channel_e
 */
void Adc_Stop_Comparator(adc_channel_e channel);

/**
 * @brief Get comparator result for poll mode
 * @param[in] channel The Adc channel see adc_channel_e
 * @return true when the comparasion is met, otherwise return false
 */
bool Adc_Get_Compartor_Result(adc_channel_e channel);

/**
 * @brief Start Adc sampling, the according gpio pin mux ADC feature shall be selected
 * @param[in] channel The Adc channel see adc_channel_e
 */
void Adc_Start_Adc_Channel(adc_channel_e channel);

/**
 * @brief Stop Adc sampling
 * @param[in] channel The Adc channel see adc_channel_e
 */
void Adc_Stop_Adc_Channel(adc_channel_e channel);

/**
 * @brief Get Adc sampling result, unit mV
 * @param[in] channel The Adc channel see adc_channel_e
 * @return The adc sampling result
 */
uint16_t Adc_Get_Channel_Data(adc_channel_e channel);

#ifdef __cplusplus
}
#endif

#endif
