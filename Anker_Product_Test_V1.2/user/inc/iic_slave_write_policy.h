/****************************************************************************
 * @copyright Copyright(C) 2024-2024 Ismartware Limited. All rights reserved.
 * @file iic_slave_write_policy.h
 * @brief write_policy
 * @author SZ team
 ****************************************************************************/
#ifndef IIC_Slave_Write_Policy_H
#define IIC_Slave_Write_Policy_H

#include "api.h"
#include "pdo.h"
#include "ufcs_caps.h"
#include "zr_abn.h"
#include "zr_adc.h"
#include "iic_slave_global_variables_policy.h"

#define PD_PDO_DATA 
#define UFCS_PDO_DATA


//获取5VFlag参数
u32 Get_5VFlag_Data(uint8_t regData);
//获取PPSFlag参数
u32 Get_PPSFlag_data(uint8_t regData);

#ifdef PD_PDO_DATA
//PD_PDO数据包封装---如果与客户要求广播精度不够请联系
#define FIX9V_ENABLE 	(Get_RegAddr_Data(0x31) & (0x01 << 7)) >> 7
#define FIX12V_ENABLE (Get_RegAddr_Data(0x31) & (0x01 << 6)) >> 6
#define FIX15V_ENABLE (Get_RegAddr_Data(0x31) & (0x01 << 5)) >> 5
#define FIX20V_ENABLE (Get_RegAddr_Data(0x31) & (0x01 << 4)) >> 4
#define PPS0_ENABLE 	(Get_RegAddr_Data(0x31) & (0x01 << 3)) >> 3
#define PPS1_ENABLE 	(Get_RegAddr_Data(0x31) & (0x01 << 2)) >> 2
#define PPS2_ENABLE 	(Get_RegAddr_Data(0x31) & (0x01 << 1)) >> 1
#define FIX28V_ENABLE (Get_RegAddr_Data(0x31) & (0x01 << 0)) >> 0
#define AVS0_ENABLE 	(Get_RegAddr_Data(0x32) & (0x01 << 7)) >> 7

#define FIX5V_CUR 		Get_RegAddr_Data(0x34) * 50
#define FIX9V_VOL 		Get_RegAddr_Data(0x35) * 100
#define FIX9V_CUR 		Get_RegAddr_Data(0x36) * 50
#define FIX12V_VOL 		Get_RegAddr_Data(0x37) * 100
#define FIX12V_CUR 		Get_RegAddr_Data(0x38) * 50
#define FIX15V_VOL 		Get_RegAddr_Data(0x39) * 100
#define FIX15V_CUR 		Get_RegAddr_Data(0x3A) * 50
#define FIX20V_VOL 		Get_RegAddr_Data(0x3B) * 100
#define FIX20V_CUR 		Get_RegAddr_Data(0x3C) * 50
#define PPS0_VOL_MIN 	Get_RegAddr_Data(0x3D) * 100
#define PPS0_VOL_MAX 	Get_RegAddr_Data(0x3E) * 100
#define PPS0_CUR_MAX 	Get_RegAddr_Data(0x3F) * 50
#define PPS1_VOL_MIN 	Get_RegAddr_Data(0x40) * 100
#define PPS1_VOL_MAX 	Get_RegAddr_Data(0x41) * 100
#define PPS1_CUR_MAX 	Get_RegAddr_Data(0x42) * 50
#define PPS2_VOL_MIN 	Get_RegAddr_Data(0x43) * 100
#define PPS2_VOL_MAX 	Get_RegAddr_Data(0x44) * 100
#define PPS2_CUR_MAX 	Get_RegAddr_Data(0x45) * 50
#define FIX28V_VOL 		Get_RegAddr_Data(0x46) * 200
#define FIX28V_CUR 		Get_RegAddr_Data(0x47) * 50
#define AVS0_VOL_MIN 	Get_RegAddr_Data(0x48) * 200
#define AVS0_VOL_MAX 	Get_RegAddr_Data(0x49) * 200

#define FIX5V_FLAG		Get_5VFlag_Data(Get_RegAddr_Data(0x30))
#define PPS0_FLAG 		Get_PPSFlag_data((Get_RegAddr_Data(0x32) & (0x01 << 6)) >> 6)
#define PPS1_FLAG 		Get_PPSFlag_data((Get_RegAddr_Data(0x32) & (0x01 << 5)) >> 5)
#define PPS2_FLAG 		Get_PPSFlag_data((Get_RegAddr_Data(0x32) & (0x01 << 4)) >> 4)

#define FIX5V_PEAK_CUR 	(Get_RegAddr_Data(0x32) & (0x03 << 2)) >> 2
#define FIX9V_PEAK_CUR 	(Get_RegAddr_Data(0x32) & (0x03 << 0)) >> 0
#define FIX12V_PEAK_CUR (Get_RegAddr_Data(0x33) & (0x03 << 6)) >> 6
#define FIX15V_PEAK_CUR (Get_RegAddr_Data(0x33) & (0x03 << 4)) >> 4
#define FIX20V_PEAK_CUR (Get_RegAddr_Data(0x33) & (0x03 << 2)) >> 2
#define FIX28V_PEAK_CUR (Get_RegAddr_Data(0x33) & (0x03 << 0)) >> 0

#define PD_MAX_POWER 		Get_RegAddr_Data(0x04)

#endif


#ifdef UFCS_PDO_DATA
//UFCS_PDO数据包封装---如果与客户要求广播精度不够请联系

#define UFCS_ID1_ENABLE		(Get_RegAddr_Data(0x50) & (0x01 << 7)) >> 7
#define UFCS_ID2_ENABLE		(Get_RegAddr_Data(0x50) & (0x01 << 6)) >> 6
#define UFCS_ID3_ENABLE		(Get_RegAddr_Data(0x50) & (0x01 << 5)) >> 5

#define UFCS_ID1_CUR_STEP	(Get_RegAddr_Data(0x51) & (0x07 << 5)) >> 5
#define UFCS_ID1_VOL_STEP	(Get_RegAddr_Data(0x51) & (0x01 << 4)) >> 4
#define UFCS_ID2_CUR_STEP	(Get_RegAddr_Data(0x51) & (0x07 << 1)) >> 1
#define UFCS_ID2_VOL_STEP	(Get_RegAddr_Data(0x51) & (0x01 << 0)) >> 0
#define UFCS_ID3_CUR_STEP	(Get_RegAddr_Data(0x52) & (0x07 << 5)) >> 5
#define UFCS_ID3_VOL_STEP	(Get_RegAddr_Data(0x52) & (0x01 << 4)) >> 4

#define UFCS_ID1_MAX_VOL	Get_RegAddr_Data(0x53) * 100
#define UFCS_ID1_MIN_VOL	Get_RegAddr_Data(0x54) * 100
#define UFCS_ID1_MAX_CUR	Get_RegAddr_Data(0x55) * 50
#define UFCS_ID1_MIN_CUR	Get_RegAddr_Data(0x56) * 50
#define UFCS_ID2_MAX_VOL	Get_RegAddr_Data(0x57) * 100
#define UFCS_ID2_MIN_VOL	Get_RegAddr_Data(0x58) * 100
#define UFCS_ID2_MAX_CUR	Get_RegAddr_Data(0x59) * 50
#define UFCS_ID2_MIN_CUR	Get_RegAddr_Data(0x5A) * 50
#define UFCS_ID3_MAX_VOL	Get_RegAddr_Data(0x5B) * 100
#define UFCS_ID3_MIN_VOL	Get_RegAddr_Data(0x5C) * 100
#define UFCS_ID3_MAX_CUR	Get_RegAddr_Data(0x5D) * 50
#define UFCS_ID3_MIN_CUR	Get_RegAddr_Data(0x5E) * 50

#endif


//更新UFCS_PDO
void Update_UFCS_PDO(void);
//更新PD_PDO
void Update_PD_PDO(void);
//写执行寄存器功能
void Write_Policy(void);
#endif


