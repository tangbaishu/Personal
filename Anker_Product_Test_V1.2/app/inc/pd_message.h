/****************************************************************************
 * @copyright Copyright(C) 2020-2023 Ismartware Limited. All rights reserved.
 * @file pd_message.h
 * @brief pd message define and api function
 * @author SD1 & SW team
 ****************************************************************************/
#ifndef _PD_MESSAGE_H_
#define _PD_MESSAGE_H_

#include "system.h"

// clang-format off
#define PD_SOP                           (u8)0x01
#define PD_SOP1                          (u8)0x02
#define PD_SOP2                          (u8)0x03

#define PD_CONTROL_CLASS     (u8)0x00
#define PD_DATA_CLASS        (u8)0x40
#define PD_EXTENDED_CLASS    (u8)0x80

//pd control message
#define PD_GoodCRC                  (u8)(0x1 | PD_CONTROL_CLASS)
#define PD_GotoMin                  (u8)(0x2 | PD_CONTROL_CLASS)
#define PD_Accept                   (u8)(0x3 | PD_CONTROL_CLASS)
#define PD_Reject                   (u8)(0x4 | PD_CONTROL_CLASS)
#define PD_Ping                     (u8)(0x5 | PD_CONTROL_CLASS)
#define PD_PS_RDY                   (u8)(0x6 | PD_CONTROL_CLASS)
#define PD_Get_Source_Cap           (u8)(0x7 | PD_CONTROL_CLASS)
#define PD_Get_Sink_Cap             (u8)(0x8 | PD_CONTROL_CLASS)
#define PD_DR_Swap                  (u8)(0x9 | PD_CONTROL_CLASS)
#define PD_PR_Swap                  (u8)(0xa | PD_CONTROL_CLASS)
#define PD_VCONN_Swap               (u8)(0xb | PD_CONTROL_CLASS)
#define PD_Wait                     (u8)(0xc | PD_CONTROL_CLASS)
#define PD_Soft_Reset               (u8)(0xd | PD_CONTROL_CLASS)
#define PD_Not_Supported            (u8)(0x10 | PD_CONTROL_CLASS)
#define PD_Get_Source_Cap_Extended  (u8)(0x11 | PD_CONTROL_CLASS)
#define PD_Get_Status               (u8)(0x12 | PD_CONTROL_CLASS)
#define PD_FR_Swap                  (u8)(0x13 | PD_CONTROL_CLASS)
#define PD_Get_PPS_Status           (u8)(0x14 | PD_CONTROL_CLASS)
#define PD_Get_Country_Codes        (u8)(0x15 | PD_CONTROL_CLASS)
#define PD_Get_Sink_Cap_Extended    (u8)(0x16 | PD_CONTROL_CLASS)
#define PD_Get_Source_Info          (u8)(0x17 | PD_CONTROL_CLASS)
#define PD_Get_Revision             (u8)(0x18 | PD_CONTROL_CLASS)

//pd data message
#define PD_Source_Capabilities      (u8)(0x1 | PD_DATA_CLASS)
#define PD_Request                  (u8)(0x2 | PD_DATA_CLASS)
#define PD_BIST_Message             (u8)(0x3 | PD_DATA_CLASS)
#define PD_Sink_Capabilities        (u8)(0x4 | PD_DATA_CLASS)
#define PD_Battery_Status           (u8)(0x5 | PD_DATA_CLASS)
#define PD_Alert                    (u8)(0x6 | PD_DATA_CLASS)
#define PD_Get_Country_Info         (u8)(0x7 | PD_DATA_CLASS)
#define PD_Enter_USB                (u8)(0x8 | PD_DATA_CLASS)
#define PD_EPR_Request              (u8)(0x9 | PD_DATA_CLASS)
#define PD_EPR_Mode                 (u8)(0xa | PD_DATA_CLASS)
#define PD_Source_Info              (u8)(0xb | PD_DATA_CLASS)
#define PD_Revision                 (u8)(0xc | PD_DATA_CLASS)
#define PD_Vendor_Defined           (u8)(0xf | PD_DATA_CLASS)

//pd extended message
#define PD_Source_Capabilities_Extended         (u8)(0x1 | PD_EXTENDED_CLASS)
#define PD_Status                               (u8)(0x2 | PD_EXTENDED_CLASS)
#define PD_Get_Battery_Cap                      (u8)(0x3 | PD_EXTENDED_CLASS)
#define PD_Get_Battery_Status                   (u8)(0x4 | PD_EXTENDED_CLASS)
#define PD_Battery_Capabilities                 (u8)(0x5 | PD_EXTENDED_CLASS)
#define PD_Get_Manufacturer_Info                (u8)(0x6 | PD_EXTENDED_CLASS)
#define PD_Manufacturer_Info                    (u8)(0x7 | PD_EXTENDED_CLASS)
#define PD_Security_Request                     (u8)(0x8 | PD_EXTENDED_CLASS)
#define PD_Security_Response                    (u8)(0x9 | PD_EXTENDED_CLASS)
#define PD_Firmware_Update_Request              (u8)(0xa | PD_EXTENDED_CLASS)
#define PD_Firmware_Update_Response             (u8)(0xb | PD_EXTENDED_CLASS)
#define PD_PPS_Status                           (u8)(0xc | PD_EXTENDED_CLASS)
#define PD_Country_Info                         (u8)(0xd | PD_EXTENDED_CLASS)
#define PD_Country_Codes                        (u8)(0xe | PD_EXTENDED_CLASS)
#define PD_Sink_Capabilities_Extended           (u8)(0xf | PD_EXTENDED_CLASS)
#define PD_Extended_Control                     (u8)(0x10 | PD_EXTENDED_CLASS)
#define PD_EPR_Source_Capabilities              (u8)(0x11 | PD_EXTENDED_CLASS)
#define PD_EPR_Sink_Capabilities                (u8)(0x12 | PD_EXTENDED_CLASS)
#define PD_Vendor_Defined_Extended              (u8)(0x1e | PD_EXTENDED_CLASS)

//Structured VDM Command
#define PD_Discover_Identity              (u8)0x1
#define PD_Discover_SVIDs                 (u8)0x2
#define PD_Discover_Modes                 (u8)0x3
#define PD_Enter_Mode                     (u8)0x4
#define PD_Exit_Mode                      (u8)0x5
#define PD_Attention                      (u8)0x6
//Structured VDM Command Type
#define PD_VDM_REQ           (u8)0x0
#define PD_VDM_ACK           (u8)0x1
#define PD_VDM_NAK           (u8)0x2
#define PD_VDM_BUSY          (u8)0x3

// clang-format on

#endif





