/****************************************************************************
 * @copyright Copyright(C) 2020-2022 Ismartware Limited. All rights reserved.
 * @file pdo.h
 * @brief  Provide macros to assist PDOs generation & decoded
 * @author SD1 team
 ****************************************************************************/
#ifndef PDO_H
#define PDO_H

#ifdef __cplusplus
extern "C" {
#endif

// clang-format off

#define PDO_TYPE_FIXED      (0U)
#define PDO_TYPE_PPS        (12U)
#define PDO_TYPE_AVS        (13U)
#define PDO_TYPE_SPR_AVS    (14U)
#define PDO_TYPE(x) (x << 28)

#define PDO_FIXED_NO_PEAK_CURRENT                       (0U)
#define PDO_FIXED_PEAK_CURRENT_MODE1                    (1U)
#define PDO_FIXED_PEAK_CURRENT_MODE2                    (2U)
#define PDO_FIXED_PEAK_CURRENT_MODE3                    (3U)
#define PDO_FIXED_PEAK_CURRENT_MODE(x)                  (x << 20)

/// Dual-Role Power
#define PDO_FIXED_DUAL_POWER_ROLE                       (1U << 29) 
/// Externally powered  -> unconstrained power
#define PDO_FIXED_UNCONSTRAINED_POWER                   (1U << 27) 
/// USB Communications Capable
#define PDO_FIXED_COMM_CAP                              (1U << 26) 
/// dual Data role
#define PDO_FIXED_DUAL_DATA_ROLE                        (1U << 25)    
/// EPR Mode Capable
#define PDO_FIXED_EPR_MODE                              (1U << 23)
/// Voltage in 50mV units
#define PDO_FIXED_VOLT(mv)              (((mv) / 50U) << 10)
/// Max current in 10mA units
#define PDO_FIXED_CURR(ma)              (((ma) / 10U) << 0) 

#define PDO_SPR_PPS_POWER_LIMITED (1U << 27)
#define PDO_PPS_MAX_CURR(ma) (((ma) / 50U) << 0)
#define PDO_PPS_MIN_VOLT(mv) (((mv) / 100U) << 8)
#define PDO_PPS_MAX_VOLT(mv) (((mv) / 100U) << 17)

#define PDO_EPR_MAX_VOL(mv) (((mv) / 100U) << 17)
#define PDO_EPR_MIN_VOLT(mv) (((mv) / 100U) << 8)
#define PDO_EPR_PDP(w) ((w) << 0)

/** 
 * define vsafe5v fixed power object : 5v 
 * @param ma current in mA unit
 * @param flags the flags,it can be the combineation of : 
    PDO_FIXED_DUAL_POWER_ROLE, dual power role 
    PDO_FIXED_UNCONSTRAINED_POWER, unconstrained power
    PDO_FIXED_COMM_CAP, usb communicate cap
    PDO_FIXED_DUAL_DATA_ROLE, dual data role
    PDO_FIXED_EPR_MODE,  EPR mode capable
 * @param peakCurrent,it can be :
    PDO_FIXED_NO_PEAK_CURRENT,    not support peak current
	PDO_FIXED_PEAK_CURRENT_MODE1, support peak current mode1
	PDO_FIXED_PEAK_CURRENT_MODE2, support peak current mode2
	PDO_FIXED_PEAK_CURRENT_MODE3, support peak current mode3
 */ 
#define PDO_FIXED_VSAFE5V(ma, flags, peakCurrent)      (PDO_FIXED_VOLT(5000) | PDO_FIXED_CURR(ma) | (flags) | PDO_FIXED_PEAK_CURRENT_MODE(peakCurrent))

/** 
 * define other fixed power object
 * @param mv voltage in mV
 * @param ma maximum current in mA
 * @param peakCurrent,it can be :
    PDO_FIXED_NO_PEAK_CURRENT,    not support peak current
	PDO_FIXED_PEAK_CURRENT_MODE1, support peak current mode1
	PDO_FIXED_PEAK_CURRENT_MODE2, support peak current mode2
	PDO_FIXED_PEAK_CURRENT_MODE3, support peak current mode3
 */ 
#define PDO_FIXED_OTHERS(mv, ma, peakCurrent)          (PDO_FIXED_VOLT(mv) | PDO_FIXED_CURR(ma) | PDO_FIXED_PEAK_CURRENT_MODE(peakCurrent))

/** 
 * define spr avs power object
 * @param ma20v max current for 15-20v range
 * @param ma15v max current for 9-15v range
 */ 
#define PDO_SPR_AVS(ma20v, ma15v)          (PDO_FIXED_CURR(ma20v) | (PDO_FIXED_CURR(ma15v) << 10) | PDO_TYPE(PDO_TYPE_SPR_AVS))

/** 
 * define SPR augmented power object 
 * @param max_ma max current in mA unit
 * @param min_mv min voltage in mV unit
 * @param max_mv max voltage in mV unit
 * @param flags the flags, it can be the combineation of : 
    PDO_SPR_PPS_POWER_LIMITED, PPS power limited 
 */
#define PDO_PPS(max_ma, min_mv, max_mv, flags) (PDO_PPS_MAX_CURR(max_ma) | PDO_PPS_MIN_VOLT(min_mv) | PDO_PPS_MAX_VOLT(max_mv) | flags | PDO_TYPE(PDO_TYPE_PPS))
/** 
 * define EPR augmented power object 
 * @param min_mv minimum voltage in mV
 * @param max_mv maximum voltage in mV
 * @param pdp_w PD power in w
 */ 
#define PDO_AVS(min_mv, max_mv, pdp_w)  (PDO_EPR_MIN_VOLT(min_mv) | PDO_EPR_MAX_VOL(max_mv) | PDO_EPR_PDP(pdp_w) | PDO_TYPE(PDO_TYPE_AVS))

/**
 * Example: how to setup 100w SPR pdo
@code
const uint32_t sprPdos[] = 
{
    PDO_FIXED_VSAFE5V(3000, PDO_FIXED_UNCONSTRAINED_POWER | PDO_FIXED_DUAL_DATA_ROLE, PDO_FIXED_NO_PEAK_CURRENT),
    PDO_FIXED_OTHERS(9000, 3000, PDO_FIXED_NO_PEAK_CURRENT),
    PDO_FIXED_OTHERS(12000, 3000, PDO_FIXED_NO_PEAK_CURRENT),
    PDO_FIXED_OTHERS(15000, 3000, PDO_FIXED_NO_PEAK_CURRENT),
    PDO_FIXED_OTHERS(20000, 5000, PDO_FIXED_NO_PEAK_CURRENT),
    PDO_PPS(5000, 3300, 21000, PDO_SPR_PPS_POWER_LIMITED),
};
@endcode
*/

/////////////////////////////////////////////////////////////////////
/// PDO decode

/// Get PDO type
#define PDO_GET_TYPE(x)    (((x) >> 30))
/// Whether support dual role power
#define PDO_IS_SUPPORT_DUAL_ROLE_POWER(x) (((x) >> 29) & 0x1)
/// Whether is a unconstrained power 
#define PDO_IS_SUPPORT_EXTERNAL(x) (((x) >> 27) & 0x1)
/// Whether support dual role data
#define PDO_IS_SUPPORT_DUAL_ROLE_DATA(x) (((x) >> 25) & 0x1)
/// while supoort EPR
#define PDO_IS_SUPPORT_EPR(x) (((x) >> 23) & 0x1)

/// Get fix PDO voltage, unit mv
#define PDO_GET_FIX_VOL(x) ((((x) >> 10) & 0x3FF) * 50)
/// Get fix PDO current, unit ma
#define PDO_GET_FIX_CURR(x) (((x) & 0x3FF) * 10)

/// whether is EPR adjustable voltage supply for a augmented PDO
#define PDO_IS_EPR(x) (((x) >> 28) & 0x1)

/// Get augmented PDO SPR max voltage, unit mv
#define PDO_GET_SPR_MAX_VOL(x) ((((x) >> 17) & 0xFF) * 100)
/// Get augmented PDO SPR min voltage, unit mv
#define PDO_GET_SPR_MIN_VOL(x) ((((x) >> 8) & 0xFF) * 100)
/// Get augmented PDO SPR current, unit ma
#define PDO_GET_SPR_MAX_CURR(x) (((x) & 0x7F) * 50)
/// get augmented PDO pps power limited
#define PDO_SPR_IS_PPS_POWER_LIMIT(x) (((x) >> 27) & 0x1)

/// Get augmented PDO EPR max voltage, unit mv
#define PDO_GET_EPR_MAX_VOL(x) ((((x) >> 17) & 0x1FF) * 100)
/// Get augmented PDO EPR min voltage, unit mv
#define PDO_GET_EPR_MIN_VOL(x) ((((x) >> 8) & 0xFF) * 100)
/// Get augmented PDO PDP, unit w
#define PDO_GET_EPR_PDP(x) ((x) & 0x7F)



/**
 * Example: how to parse pdo

@code
void Display_Pdo(const uint32_t* pdo, uint8_t size)
{
    for (uint8_t i = 0; i < size; i++)
    {
        if (PDO_GET_TYPE(pdo[i]) == PDO_TYPE_FIXED)
        {
            printf("pdo 0x%X, type: %s: vol: %dmV, curr: %dmA\n", pdo[i], CSPdoTypes[PDO_GET_TYPE(pdo[i])], 
                PDO_GET_FIX_VOL(pdo[i]), PDO_GET_FIX_CURR(pdo[i]));
        }
        else if (PDO_GET_TYPE(pdo[i]) == PDO_TYPE_AUGMENTED)
        {
            if (PDO_IS_EPR(pdo[i]))
            {
                printf("pdo 0x%X, type: %s/EPR: max vol: %dmV, min vol: %dmV, Pdp: %dw\n", pdo[i], CSPdoTypes[PDO_GET_TYPE(pdo[i])], 
                    PDO_GET_EPR_MAX_VOL(pdo[i]), PDO_GET_EPR_MIN_VOL(pdo[i]), PDO_GET_EPR_PDP(pdo[i]));
            }
            else
            {
                printf("pdo 0x%X, type: %s/SPR: max vol: %dmV, min vol: %dmV, max curr: %dmA, pps power limited:%s\n", pdo[i], CSPdoTypes[PDO_GET_TYPE(pdo[i])], 
                    PDO_GET_SPR_MAX_VOL(pdo[i]), PDO_GET_SPR_MIN_VOL(pdo[i]), PDO_GET_SPR_MAX_CURR(pdo[i]), 
                    PDO_SPR_IS_PPS_POWER_LIMIT(pdo[i]) ? "YES" : "NO");
            }
        }

        if (i == 0)
        {
            printf("--> support dual role power: %s\n", PDO_IS_SUPPORT_DUAL_ROLE_POWER(pdo[i]) ? "YES" : "NO");
            printf("--> a unconstrained power: %s\n", PDO_IS_SUPPORT_EXTERNAL(pdo[i]) ? "YES" : "NO");
            printf("--> support dual role data: %s\n", PDO_IS_SUPPORT_DUAL_ROLE_DATA(pdo[i]) ? "YES" : "NO");
            printf("--> support EPR: %s\n", PDO_IS_SUPPORT_EPR(pdo[i]) ? "YES" : "NO");
        }
    }
}
@endcode

*/

// clang-format on

#ifdef __cplusplus
}
#endif

#endif        // PDO_H
