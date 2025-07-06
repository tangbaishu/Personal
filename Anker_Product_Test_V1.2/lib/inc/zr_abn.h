/****************************************************************************
 * @copyright Copyright(C) 2020-2023 Ismartware Limited. All rights reserved.
 * @file zr_abn.h
 * @brief Functions interfaces for the Abn firmware library
 * @author Zeng Jianbo
 ****************************************************************************/
#ifndef ZR_ABN_H
#define ZR_ABN_H

#include "system.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @page abnormal Abnormal
 * - Provide interface to set and get abnormal configuration, e.g. dpdm short threshold, vin ovp threshold, etc.
 * - Abn_Get_Config() to get the current abnormal configuration, client can be customized based on this configuration
 * - Abn_Set_Config() to set the desired configuration
 * 
 * @par Example below shows Abn config set and get
 *
@code

int main(void)
{
    /// system init shall be called firstly
    Device_Init();
    /// abnormal config
    abn_config_t abnCfg = {0};
    Abn_Get_Config(&abnCfg);
    abnCfg.bitFields.dpdmShortTh = DPDM_SHORT_TH_4P2V;
    Abn_Set_Config(&abnCfg);

    while (1)
    {
        // the following sequence shall NOT change
        Device_Run();
        // device policy process, secondary development

        // sleep process
        Low_Power_Sleep();
    }
}

}
@endcode
*/

/** LPC Adc current compare threshold */
typedef enum
{
    LPS_ADC_CMP_TH_0P2A = 0,        //!< 0.2A
    LPS_ADC_CMP_TH_0P6A,            //!< 0.6A
    LPS_ADC_CMP_TH_1P0A,            //!< 1.0A
    LPS_ADC_CMP_TH_1P4A,            //!< 1.4A
    LPS_ADC_CMP_TH_1P8A,            //!< 1.8A
    LPS_ADC_CMP_TH_2P2A,            //!< 2.2A
    LPS_ADC_CMP_TH_2P6A,            //!< 2.6A
    LPS_ADC_CMP_TH_3P0A,            //!< 3.0A
} lps_adc_current_threshold_e;

/**
 * LPC hmos current compare threshold
 * iL_avg / iL_peak
 */
typedef enum
{
    LPS_ADC_CMP_TH_0P60A_0P98A = 0,        //!< 0.60A / 0.98A
    LPS_ADC_CMP_TH_1P16A_1P65A,            //!< 1.16A / 1.65A
    LPS_ADC_CMP_TH_1P72A_2P32A,            //!< 1.72A / 2.32A
    LPS_ADC_CMP_TH_2P28A_2P99A,            //!< 2.28A / 2.99A
    LPS_ADC_CMP_TH_2P84A_3P66A,            //!< 2.84A / 3.66A
    LPS_ADC_CMP_TH_3P40A_4P33A,            //!< 3.40A / 4.33A
    LPS_ADC_CMP_TH_3P96A_5P00A,            //!< 3.96A / 5.00A
    LPS_ADC_CMP_TH_4P52A_5P67A,            //!< 4.52A / 5.67A
} lps_hmos_current_threshold_e;

/**
 * Vout scp threshold
 */
typedef enum
{
    VOUT_SCP_TH_3P0V = 0,        //!< 3.0V
    VOUT_SCP_TH_2P7V             //!< 2.7V
} vout_scp_threshold_e;

/**
 * Vout UVP threshold
 */
typedef enum
{
    VOUT_UVP_TH_PER70 = 0,        //!< 70%
    VOUT_UVP_TH_3P1V              //!< 3.1V
} vout_uvp_threshold_e;

/**
 * Vin UVLO threshold
 */
typedef enum
{
    VIN_UVLP_TH_26_27V = 0,        //!< 26/27V
    VIN_UVLP_TH_19_20V,            //!< 19/20V
    VIN_UVLP_TH_18_19V,            //!< 18/19V
    VIN_UVLP_TH_14_15V,            //!< 14/15V
    VIN_UVLP_TH_13_14V,            //!< 13/14V
    VIN_UVLP_TH_10_11V,            //!< 10/11V
    VIN_UVLP_TH_7_8V,              //!< 7/8V
    VIN_UVLP_TH_5_6V,              //!< 5/6V
} vin_uvlp_threshold_e;

/**
 * Vin OVP threshold
 */
typedef enum
{
    VIN_OVP_TH_42_40V = 0,        //!< 42V/40V
    VIN_OVP_TH_40_38V,            //!< 40V/38V
    VIN_OVP_TH_38_36V,            //!< 38V/36V
    VIN_OVP_TH_36_34V,            //!< 36V/34V
    VIN_OVP_TH_34_32V,            //!< 34V/32V
    VIN_OVP_TH_32_30V,            //!< 32V/30V
    VIN_OVP_TH_24_22V,            //!< 24V/22V
    VIN_OVP_TH_26_24V,            //!< 26V/24V
} vin_ovp_threshold_e;

/**
 * Vin OVP debounce time
 */
typedef enum
{
    VIN_OVP_DBC_TIME_30US = 0,        //!< 30us
    VIN_OVP_DBC_TIME_4MS,             //!< 4ms
} vin_ovp_debounce_time_e;

/**
 * Vout Slow OVP debounce time
 */
typedef enum
{
    VOUT_SOVP_DBC_TIME_8MS = 0,        //!< 8ms
    VOUT_SOVP_DBC_TIME_4MS,            //!< 4ms
} vout_sovp_debounce_time_e;

/**
 * Vout Fast OVP debounce time
 */
typedef enum
{
    VOUT_FOVP_DBC_TIME_50US = 0,        //!< 50us
    VOUT_FOVP_DBC_TIME_30US,            //!< 30us
} vout_fovp_debounce_time_e;

/**
 * Vin UVLO debounce time
 */
typedef enum
{
    VIN_UVLO_DBC_TIME_4MS = 0,        //!< 4ms
    VIN_UVLO_DBC_TIME_128US,          //!< 128us
} vin_uvlo_debounce_time_e;

/**
 * DPDM weak short threshold
 */
typedef enum
{
    DPDM_SHORT_TH_3P6V = 0,        //!< 3.6V
    DPDM_SHORT_TH_3P7V,            //!< 3.7V
    DPDM_SHORT_TH_3P8V,            //!< 3.8V
    DPDM_SHORT_TH_3P9V,            //!< 3.9V
    DPDM_SHORT_TH_4P0V,            //!< 4.0V
    DPDM_SHORT_TH_4P1V,            //!< 4.1V
    DPDM_SHORT_TH_4P2V,            //!< 4.2V
    DPDM_SHORT_TH_4P3V,            //!< 4.3V
} dpdm_weak_short_threshold_e;

/**
 * over temperature shutdown threshold
 */
typedef enum
{
    OVP_SHUTDOWN_TH_130 = 0,        //!< 130 degree
    OVP_SHUTDOWN_TH_140,            //!< 140 degree
    OVP_SHUTDOWN_TH_150,            //!< 150 degree
    OVP_SHUTDOWN_TH_160,            //!< 160 degree
} ovp_shutdown_threshold_e;

/**
 * over temperature alarm threshold
 */
typedef enum
{
    OVP_ALARM_TH_110 = 0,        //!< 110 degree
    OVP_ALARM_TH_120,            //!< 120 degree
    OVP_ALARM_TH_130,            //!< 130 degree
    OVP_ALARM_TH_140,            //!< 140 degree
} ovp_alarm_threshold_e;

typedef union 
{
    uint32_t value;
    struct
    {
        lps_adc_current_threshold_e lpsAdcCurrTh : 3;           // lps adc current compare threshold
        lps_hmos_current_threshold_e lpsHmosCurrTh : 3;         // lps hmos current compare threshold
        vout_scp_threshold_e voutScpTh : 1;                     // vout scp threshold
        vout_uvp_threshold_e voutUvpTh : 1;                     // vout uvp threshold
        vin_uvlp_threshold_e vinUvlpTh : 3;                     // vin uvlp threshold
        vin_ovp_threshold_e vinOvpTh : 3;                       // vin ovp threshold
        vin_ovp_debounce_time_e vinOvpDbcTime : 1;              // vin ovp debounce time
        vout_sovp_debounce_time_e voutSovpDbcTime : 1;          // vout sovp debounce time
        vout_fovp_debounce_time_e voutFovpDbcTime: 1;           // vout fovp debounce time
        vin_uvlo_debounce_time_e vinUvloDbcTime: 1;             // vin uvlp debounce time
        uint32_t reserved0: 1;                                  // reserved
        dpdm_weak_short_threshold_e dpdmShortTh: 3;             // dpdm weak short threshold
        ovp_shutdown_threshold_e ovpShutdownTh: 2;              // ovp shutdown threshold
        ovp_alarm_threshold_e ovpAlarmTh: 2;                    // ovp alarm threshold
        uint32_t reserver1: 6;                                  // reserved
    } bitFields;
} abn_config_t;


/**
 * @brief Get Abnormal config
 * @param config the config instance to retrive
 */
void Abn_Get_Config(abn_config_t* config);

/**
 * @brief Set Abnormal config
 * @param config the config instance want to set 
 */
void Abn_Set_Config(const abn_config_t* config);


#ifdef __cplusplus
}
#endif

#endif
