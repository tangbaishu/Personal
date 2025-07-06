/****************************************************************************
 * @copyright Copyright(C) 2020-2022 Ismartware Limited. All rights reserved.
 * @file api.h
 * @brief ZR2067 Application development interfaces
 * @author SD1 & SW team
 ****************************************************************************/
#ifndef API_H
#define API_H

#include <stdint.h>
#include "config.h"
#include "config_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  Device initialization, it shall be call firstly
 */
void Device_Init(void);

/**
 * @brief  Device inside logic process, e.g. protocols, it shall be scheduled in while loop
 */
void Device_Run(void);

/**
 * @brief  Get device port state
 * @return The port state \n
 *  bit 0: is to indicate whether there is a device in PORT 1 , 0: no device, 1: has device \n
 *  bit 1: is to indicate whether there is a device in PORT 2, 0: no device, 1: has device \n
 @code
    while (1)
    {
        // the following sequence shall NOT change
        Device_Run();
        // device policy process, secondary development
        uint8_t state = Device_Get_Port_State();
        // handle Single A mode
        if (gConfig.portMode == SINGLE_A_MODE)
        {
            if (state == 0)
            {
                /// there is no device attached
            }
            else if (state == 1)
            {
                /// there is a device attached
            }
        }
        // handle other mode
        if (gConfig.portMode == SINGLE_C_MODE)
        {

        }
    }

 @endcode
 */
uint8_t Device_Get_Port_State(void);

/**
 * @brief  Get device fast charge status
 * @return true when it is in fast charge, otherwise return false
 @code
    while (1)
    {
        // the following sequence shall NOT change
        Device_Run();
        // device policy process, secondary development
        bool state = Device_Is_In_Fast_Charge();
        if (state)
        {
            uint8_t port = Device_Get_Fast_Charge_Port();
            fast_charge_protocol_e protocol = Device_Get_Protocol(port);
            /// to show protocol signal
            display_protocol(protocol);
        }
        else
        {
            // to show empty protocol
            display_protocol(empty);
        }
    }

 @endcode
 *
 */
bool Device_Is_In_Fast_Charge(void);

/**
 * @brief  Get fast charge port while it is in fast charge
 * @return port number, 1 or 2 when it is in fast charge, otherwise return 0
 */
uint8_t Device_Get_Fast_Charge_Port(void);

/**
 * @brief  Get current working protocol, only when Device_Is_In_Fast_Charge is true
 * @param portNumber port number, can be 1 or 2
 * @return fast_charge_protocol_e current working protocol
 */
fast_charge_protocol_e Device_Get_Protocol(uint8_t portNumber);

/**
 * @brief  Get current requesting voltage
 * @return the current requesing voltage without offset, unit : mV
 */
uint16_t Device_Get_Requesting_Voltage(void);

/**
 * @brief  Set buck clock
 * @param div the buck clock divider, a valid range is 2-32, the clock is 2000/clock = n KHz, the default value is 11,
 * clock is about 180Khz
 */
void Device_Set_Buck_Clock(uint8_t div);

/**
 * @brief  Set mos resistance
 * @param res resistance value
 */
void Device_Cfg_Mos_Resistance(mos_res_e res);

/**
 * @brief  Get mos resistance
 * @return the configured mos resistance
 */
mos_res_e Device_Get_Mos_Resistance(void);

/**
 * @brief  Set ocp threshold
 * @param ocp The ocp threshold, see vr_ocp_e
 */
void Device_Set_Vr_Ocp(vr_ocp_e ocp);

/**
 * @brief  Get ocp threshold
 * @return the configured ocp threshold
 */
vr_ocp_e Device_Get_Vr_Ocp(void);

/**
 * @brief  Set buck config
 * @param iType buck config
 * @Note
 * 1) For chip version A, system will config it automatically when Device_Init, this function call is not needed
 * 2) For chip version B, system will config it as BUCK_CFG_181KHZ_10UH automatically when Device_Init,
 * the developer shall call this function in case of a different configuration
 */
void Device_Buck_Config(buck_cfg_type_e iType);

/**
 * @brief configure power buck temperature control loop 
 * @param slewType constant temperature control loop slew
 * @param startType constant temperature control loop start
 */
void Device_Buck_Config_Temp_Parameter(constant_temp_slew_e slewType, constant_temp_start_e startType);

/**
 * @brief get power buck temperature control loop setting
 * @param slewType constant_temp_slew_e*, pointed to constant temperature control loop slew 
 * @param startType constant_temp_start_e*,pointed to temperature control loop start 
 */
void Device_Buck_Get_Temp_Parameter(constant_temp_slew_e* slewType, constant_temp_start_e* startType);

/**
 * @brief  Compensate hook while voltage adjusting
 * @param[in] volInMv the requesting voltage without offset, unit : mv
 * @param[in,out] isEnabled true to enable cable compensation, the value will be applied, otherwise disable cable
 * compensation
 * @param[in,out] value the compensave value want to set
 * @return true to apply compensation isEnabled & value, false to skip
 */
typedef bool (*CompensateHook)(uint16_t volInMv, bool* isEnabled, cable_compensave_value_e* value);

/**
 * @brief  Register voltage adjusting compensate hook while voltage adjusting
 * @param callback the callback
 */
void Device_Register_Cable_Compensate_Hook(CompensateHook callback);

/**
 * @brief  power change hook while voltage adjusting for dedicated port
 * @param[in,out] vol the requesting voltage with offset, unit : 10 mv
 * @param[in,out] port1Curr the requesting port1 current with offset, unit : 25 ma, port1Curr is 0 when it is for port2
 * voltage adjustment
 * @param[in,out] port2Curr the requesting port2 current with offset, unit : 25 ma, port2Curr is 0 when it is for port1
 * voltage adjustment
 * @return true when to change the original values
 * @note port1Curr or port2Curr is NULL means the power change is not for port1/port2.
 */
typedef bool (*PowerAdjustHook)(uint16_t* vol, uint16_t* port1Curr, uint16_t* port2Curr);

/**
 * @brief  Register power adjusting  hook
 * @param callback the callback
 */
void Device_Register_Power_Adjust_Hook(PowerAdjustHook callback);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief  Get system config
 * @return system config instance
 */
config_data_t* Config_Get(void);

/**
 * @brief  Apply config immediately e.g. cable compensation, voltage/current offset
 * @note: It is recommenderred to call this function after changed cable compensation, voltage & current offset config
* @code
    /// get system config, it shall be after Device_Init()
    config_data_t* config = Config_Get();
    config->cableCompensationValue = CABLE_COMPENSAVE_VALUE_60MOHM;
    config->isCableCompensaved = true;
    Config_Apply();
 *@endcode
 */
void Config_Apply(void);

/**
 * @brief  Set No load threshold
 * @param portNum port number, can be 1 or 2
 * @param value noload threshold, see noload_threshold_e  
 */
void Config_Set_Noload_Threshold(uint8_t portNum, noload_threshold_e value);

/**
 * @brief  Get No load threshold setting presently
 * @param portNum port number, can be 1 or 2 
 */
noload_threshold_e Config_Get_Noload_Threshold(uint8_t portNum);

/**
 * @brief  Set lps open/close
 * @param isEnabled true to open lps
 * @note: It only can be call after buck on,after buck off,should not call this function
 */
void Config_Set_Lps(bool isEnabled);

/**
 * @brief  Get IUM SN
 * @return the start address of SN
 */
const uint32_t* Config_Get_Sn(void);

/**
 * @brief  Noload trigger callback
 * @param isDisableNoLoad[in, out] whether to disable noload
 */
typedef void (*noload_trigger_callback)(bool* isDisableNoLoad);

/**
 * @brief  Register No load callback for runtime disable noload setup
 * @param callback[in] the callback of noload trigger
 * @Note it is only apply to A+C mode A and C attached scenario currently
 * @Note it is only apply to A+A mode has device attached scenario currently
 */
void Register_Noload_Trigger_Callback(noload_trigger_callback callback);


/**
 * @brief  Check whether the port1 is plug-off only used for noload_trigger_callback & A+A mode
 * @return true when the port1 is plug-off
 */
bool Is_Port1_Plugoff(void);


/**
 * @brief  Check whether the port2 is plug-off only used for noload_trigger_callback & A+A mode
 * @return true when the port2 is plug-off
 */
bool Is_Port2_Plugoff(void);

/**
 * @brief  Noload x second callback
 * @param xSecond[in, out] the value of x second, shall be range 0-7
 */
typedef void (*noload_x_second_config_callback)(uint8_t* xSecond);

/**
 * @brief  Register x second config callback for customize x second config before port A noload checking
 * @param callback[in] the callback of noload trigger
 * @Note it is only apply to A+C mode and only A attached scenario currently
 */
void Register_Noload_X_Second_Config_Callback(noload_x_second_config_callback callback);

/**
 * @brief  Noload state customize
 * @param isPort1Offline whether port1 is offline, true is offline, otherwise is online
 * @param isPort2Offline whether port2 is offline, true is offline, otherwise is online
 */
typedef void (*noload_state_customize_callback)(bool* isPort1Offline, bool* isPort2Offline);

/**
 * @brief  Register noload state customized callback
 * @param callback[in] the callback of noload state customization
 * @Note it is only apply to A+C mode and both A+C device attached
 * @Note it is only apply to A+A mode and has device attached
 */
void Register_Noload_State_Customize_Callback(noload_state_customize_callback callback);

/**
 * @brief  buck off callback prototype
 * @return true to do buck off operation, otherwise no buck off operation
 */
typedef bool (*buck_off_callback)(void);

/**
 * @brief  Register buck off operation callback
 * @param callback[in] the callback of buck off customization
 * @Note it applies to all scenarios except Power_Buck_Ctrl() call
 */
void Register_Buck_Off_Callback(buck_off_callback callback);

/**
 * @brief  gate state callback prototype
 * @return gate state, bit 0 : port1 gate state, bit 1: port2 gate state 
 */
typedef uint8_t (*mos_state_callback)(void);

/**
 * @brief  mos/gate on callback prototype
 * @return true to do mos on operation, otherwise no mos on operation
 */
typedef bool (*mos_on_callback)(void);

/**
 * @brief  Register mos/gate on operation callback
 * @param callback[in] the callback of  mos on customization
 * @Note it applies to all scenarios except Power_Gate_Ctrl() direct call
 */
void Register_Mos_On_Callback(mos_on_callback callback);

/**
 * @brief  Register mos/gate on operation callback
 * @param callback[in] the callback of  mos on customization
 * @Note it applies to all scenarios except Power_Gate_Ctrl() direct call
 */
void Register_Mos_State_Callback(mos_state_callback callback);


/**
 * @brief  Set gate ctrl force mode, in the force mode, the gate open state which based on Register_Mos_State_Callback callback
 * @param isEnabled true to enter the gate force mode
 */
void Gate_Ctrl_Force_Mode_Set(bool isEnabled);

/**
 * @brief  power buck control
 * @param isOn[in] true to turn buck on, false to turn buck off
 * @Note please careful using this function, it will affect to whole system operation
 */
void Power_Buck_Ctrl(bool isOn);

/**
 * @brief  get power buck state
 * @return true when buck is on, otherwise return false
 */ 
bool Power_Is_Buck_On(void);

/**
 * @brief  power gate control
 * @param portNum[in] port number, could be 1 or 2
 * @param isOn[in] true to turn gate on, false to close gate
 * @Note please careful using this function, it will affect to the port operation
 */
void Power_Gate_Ctrl(uint8_t portNum, bool isOn);

/**
 * @brief  get port gate state
 * @param portNum[in] port number, could be 1 or 2
 * @return true when gate is on, otherwise return false
 */ 
bool Power_Is_Gate_On(uint8_t portNum);

/**
 * @brief  power adjust, the voltage and current offset will be added based on configuration
 * @param volInMv[in] voltage in 10mV
 * @param currInMv[in] current in 25mA
 * @ Note this api ignores the port state, e.g. C + C port1 & port2 are online, using this api will direct adjust 
 * voltage & current
 */
void Power_Adjust(uint16_t volInMv, uint16_t currInMv);

/**
 * @brief  check whether the power adjust is finished
 * @return true when the power adjust is finished, otherwise return false
 */
bool Power_Is_Adjust_Finished(void);

/**
 * @brief  current limitation prototype
 * @param port1Current[in/out] port1 current in 25mA
 * @param port2Current[in/out] port2 current in 25mA
 * @return true to apply current limitation customization
 */
typedef bool (*current_limitation_callback)(uint16_t* port1Current, uint16_t* port2Current);

/**
 * @brief  Register current limitation callback
 * @param callback[in] the callback of current limitation
 * @Note this callback will be executed after PowerAdjustHook, 
 * it is used for forcing change port1 and port2 current whatever portMode
 * e.g. it can used for single C mode to control 2 ports
 */
void Register_Current_Limitation_Callback(current_limitation_callback callback);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Low power hook for before enter sleep and after sleep wakeup
 */
typedef void (*LowPowerHook)(void);

/**
 * @brief Regiter low power before sleeping and after sleeping hook
 * @param onBefore the callback before enter sleep
 * @param onAfter the callback after sleep wakeup
 * @note The hooks are NULL default, when one of them is NULL, which means there is no callback
 * @code
 int main()
 {
    Device_Init();
    Low_Power_Register_Hook(lowPowerOnBefore, lowPowerOnAfter);

    while (1)
    {
        // the following sequence shall NOT change
        Device_Run();
        // device policy process, secondary development

        // sleep process
        Low_Power_Sleep();
    }
}

static void lowPowerOnBefore(void)
{
    printf("before enter low power\n");
}

static void lowPowerOnAfter(void)
{
    printf("low power waken\n");
}

 * @endcode
 */
void Low_Power_Register_Hook(LowPowerHook onBefore, LowPowerHook onAfter);

/**
 * @brief Enter low power process, the result can be deep sleep or normal sleep or not sleep based on conditions
 */
void Low_Power_Sleep(void);

/**
 * @brief  Tell system NOT sleep in this round of while loop
 */
void Low_Power_Not_Sleep(void);

/**
 * @brief  low power flash control is to set whether flash enter to low power while sleeping
 * @param isLowPower true to set flash to enter low power while sleeping, otherwise not enter low power
 */
void Low_Power_Flash_Ctrl(bool isLowPower);

/**
 * @brief  Get current sleep whether is deep sleep or normal sleep
 * @return true when system is going to deep sleep, otherwise return false
 */
bool Low_Power_Is_Deep_Sleep(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief version information
 */
typedef struct
{
    uint8_t major;         //!< major version
    uint8_t minor;         //!< minor version
    uint8_t bugfix;        //!< bug fix version
} version_t;

/**
 * @brief Get version information
 * @return The firmware version information
 */
const version_t* Get_Version(void);

////////////////////////////UFCS API/////////////////////////////////////
/**
 * @brief  Set ufcs power change
 */
void Ufcs_Set_Power_Change(void);

/**
 * @brief  Get ufcs is received line reset
 * @return true means received line reset otherwise not
 */
bool Ufcs_Is_Rx_Line_Reset(void);

/**
 * @brief  Clear ufcs rx line reset flag
 */
void Ufcs_Clear_Rx_Line_Reset_Flag(void);

////////////////////////////UFCS API END/////////////////////////////////

////////////////////////////Abnormal API////////////////////////////////////
/**
 * @brief  abnormal hook typedef
 * @param pending, abnormal event irq pending
 * @note  for abnormalOccurredHook, 
 *        bit0  : LPS
 *        bit1  : DM OVP 
 *        bit2  : DP OVP
 *        bit3  : CC2 OVP
 *        bit4  : CC1 OVP
 *        bit5  : VCONN2 OCP
 *        bit6  : VCONN1 OCP
 *        bit7  : DIE OTP
 *        bit8  : VOUT SCP
 *        bit9  : VOUT Slow OVP
 *        bit10 : VOUT Fast OVP 
 *        bit11 : VIN UVLO
 *        bit12 : VIN OVP
 *        bit13 : VOUT UVP
 *        bit14 : DIE Warn
 *        bit15 : DM2 Weak Short
 *        bit16 : DP2 Weak Short
 *        bit17 : DM1 Weak Short,
 *        bit18 : DP1 Weak Short
 *        bit19~bit31  reserved
 * 
 *        for abnormalReleaseHook
 *        bit0  : DM OVP Release
 *        bit1  : DP OVP Release
 *        bit2  : CC2 OVP Release
 *        bit3  : CC1 OVP Release
 *        bit4  : DIE OTP Release
 *        bit5  : VOUT SLOW OVP Release
 *        bit6  : VOUT FAST OVP  Release
 *        bit7  ：VIN UVLO Release
 *        bit8  ：VIN OVP Release
 *        bit9  ：DIE WARN Release
 *        bit10 ：DM2 Weak Short Release
 *        bit11 ：DP2 Weak Short Release
 *        bit12 ：DM1 Weak Short Release
 *        bit13 ：DP1 Weak Short Release
 *        bit14~bit31  reserved
 */
typedef void (*Abnormal_Hook)(u32 pending);

/**
 * @brief  user init abnormal hook func ,only valid func can be called
 * @param abnormalOccurredHook,Pointer functions for abnormal occurred,it will be call once when abnormal occurred
 * @param abnormalReleaseHook,Pointer functions for abnormal release,it will be call once when abnormal release
 */
void Abnormal_Init_Hook_Func(Abnormal_Hook abnormalOccurredHook, Abnormal_Hook abnormalReleaseHook);
////////////////////////////Abnormal API END////////////////////////////////

////////////////////////////TYPEC API////////////////////////////////////
/**
 * @brief  Typec hook protototype
 * @param portIndex port index
 */
typedef void (*Typec_Hook)(u8 portIndex);

/**
 * @brief  Register typec callback
 * @param attachHook Pointer functions for typec attach event
 * @param detachHook Pointer functions for typec detach event
 */
void Typec_Init_Hook_Func(Typec_Hook attachHook, Typec_Hook detachHook);

/**
 * @brief  Check whether cc1 connect device
 * @param portIndex, query specify port  0:port1  1:port2
 * @return true when device connect on specify port's cc1, otherwise return false
 */
bool Typec_Is_Cc1_Connect(u8 portIndex);

/**
 * @brief  Check whether cc2 connect device
 * @param portIndex, query specify port  0:port1  1:port2
 * @return true when device connect on specify port's cc2, otherwise return false
 */
bool Typec_Is_Cc2_Connect(u8 portIndex);

/**
 * @brief  force typec port to disconnect, it is to disable typec hardware
 * @param portIndex, query specify port  0:port1  1:port2
 */
void Port_Force_Disconnect(u8 portIndex);

/**
 * @brief  typec port reconnect
 * @param portIndex, query specify port  0:port1  1:port2
 */
void Port_Reconnect(u8 portIndex);
////////////////////////////TYPEC API END////////////////////////////////

////////////////////////////PD API/////////////////////////////////////
typedef enum
{
    PD_RX_MESSAGE_SUCCESS_EVENT = 0,        //!< success rx message
    PD_RX_HARD_RESET_EVENT,                 //!< rx hard message
    PD_RX_SOFT_RESET_EVENT,                 //!< rx soft message
	PD_RX_CABLE_RESET_EVENT,                //!< rx cable message
    PD_TX_MESSAGE_SUCCESS_EVENT,            //!< success tx message
    PD_TX_MESSAGE_FAIL_EVENT,               //!< fail tx message
    PD_TX_HARD_RESET_EVENT,                 //!< tx hard message
    PD_TX_SOFT_RESET_EVENT,                 //!< tx soft message
	PD_TX_CABLE_RESET_EVENT,                //!< tx cable message
} pd_event_e;

/**  The pd message */
typedef struct
{
    u8 packet_start;        //!< sop/sop1/sop2
    u8 mesg_class;          //!< control/data/extended
    u8 mesg_type;           //!< goodcrc/accept/ps_rdy...
} pd_mesg_summ_t;

/**
 * @brief  Pd request infomation prototype
 * @param objIndex request object index
 * @param volInMv voltage in mV
 * @param currInMa current in mA
 */
typedef void (*Request_Info_Hook)(u8 objIndex, u16 volInMv, u16 currInMa);

/**
 * @brief  Pd policy hook protototype
 * @return true when user need to process it, otherwise return false
 */
typedef bool (*Policy_Hook)(void);

/**
 * @brief  Register pd callback
 * @param requestInfoHook Pointer functions for request infomation
 * @param policyHook Pointer functions for user pd policy
 */
void Pd_Init_Hook_Func(Request_Info_Hook requestInfoHook, Policy_Hook policyHook);

/**
 * @brief  Init user's pd rx message summary
 * @return the instance of the data block of rx message summary
 */
const pd_mesg_summ_t* Pd_Init_Rx_Message_Summary(void);

/**
 * @brief  Get pd rx buff start address
 * @param isExtendedMessage true to get rx extended message buff start address, otherwise to get data message and
 * control message start address
 * @return the rx memory start address, the address is 4 bytes align for extended message, other is 2 bytes align
 * @attention called only when in use, start from low address and store in this order:header...extend header(if
 * have)...data object(if have)
 */
const u8* Pd_Get_Rx_Buff_Start_Address(bool isExtendedMessage);

/**
 * @brief  Init user's pd tx message summary
 * @return the instance of data block of tx message summary
 */
pd_mesg_summ_t* Pd_Init_Tx_Message_Summary(void);

/**
 * @brief  Get pd tx buff start address
 * @param isExtendedMessage true to get tx extended message buff start address, otherwise to get data message and
 * control message start address
 * @return the tx memory start address, address is 4 bytes align for extended message, other is 2 bytes align
 * @attention called only when in use(this function also clear tx buff),start from low address and store in this
 * order:header...extend header(if have)...data object(if have)
 */
u8* Pd_Get_Tx_Buff_Start_Address(bool isExtendedMessage);

/**
 * @brief  User start a tx pd message
 * @param objectNum data object num, control message will be 0,others set value according to the actual situation
 * @param buffStartAddress tx buff start address
 */
void Pd_Tx_Message(u8 objectNum, u8* txBuffStartAddress);

/**
 * @brief  Check whether the specified event is happen
 * @param pdEvent the specified event
 * @return true when the specified event is happen
 */
bool Pd_Is_Event_Happen(pd_event_e pdEvent);

/**
 * @brief  Clear the specified event happen
 * @param pdEvent the specified event
 */
void Pd_Clear_Event(pd_event_e pdEvent);

/**
 * @brief  Check whether pd is in ready
 * @return true when pd is ready, otherwise return false
 */
bool Pd_Is_Ready(void);

/**
 * @brief  Check whether pd is 3.0 revision
 * @return true when pd is 3.0 revision
 */
bool Pd_Is_Rev3_0(void);

/**
 * @brief  Check whether pd is dfp
 * @return true when pd is dfp
 */
bool Pd_Is_Dfp(void);

/**
 * @brief  User change data role to dfp or ufp
 * @param isDfp true to set dfp data role, otherwise is ufp
 */
void Pd_Set_Data_Role(bool isDfp);

/**
 * @brief  User change power role to source or sink
 * @param isSource true to set source power role, otherwise is sink
 */
void Pd_Set_Power_Role(bool isSource);

/**
 * @brief  User change pd rev to pd3.0 or pd2.0
 * @param isPd2 true to set pd2.0, otherwise is pd3.0
 */
void Pd_Set_Pd_Rev(bool isPd2);

/**
 * @brief  User rebroadcast pd src cap
 */
void Pd_Rebroadcast_Src_Cap(void);

/**
 * @brief  User send pd soft reset
 */
void Pd_Send_Soft_Reset(void);

/**
 * @brief  User send pd hard reset as src
 */
void Pd_Src_Send_Hard_Reset(void);

/**
 * @brief  User send pd hard reset as sink
 */
void Pd_Snk_Send_Hard_Reset(void);

/**
 * @brief  User set pd src cap whether ignore emarker info
 * @param isIgnore, when true,ignore emarker and direct broadcast raw src cap
 */
void Pd_Src_Cap_Ignore_Emarker(bool isIgnore);

/**
 * @brief  Check whether pr swap to src and apply 5v complete
 * @return true when complete
 */
bool Pd_Is_Prs_And_Apply_5V_Complete(void);

/**
 * @brief  init pd hardware to sink mode
 * @param portIndex, query specify port  0:port1  1:port2
 * @param isCc2 true to set cc2 for pd communicate
 */
void Pd_Init_Sink(u8 portIndex, bool isCc2);

/**
 * @brief  Check whether 5a cable
 * @return true when is 5a cable
 */
bool Pd_Is_5a_Cable(void);

/**
 * @brief  send exit epr mode
 * @note call this function after send spr src cap
 */
void Pd_Exit_Epr_Mode(void);
////////////////////////////PD API END/////////////////////////////////

/**
 * @brief  get Vd detect result
 * @param[in] timeInMs detection time unit : ms
 * @param[in] debounce debounce [1,15], see vd_debounce_e
 * @param[in] threshold from [0, 0x0f], see vd_compare_threshold_e
 * @param[in] isLowSideResEnabled true to enable 2.1k low side resistance
 * @param[in] isHighSideEnabled true to enable 4.8k high side resistance
 * @return be VD_DETECT_220V when result is 220V or it is VD_DETECT_110V, see vd_result_e
 * @note This function has a hard delay, it is recommended used it before while loop
 */
vd_result_e Vd_Detect_Result_Get(uint16_t timeInMs, vd_debounce_e debounce, vd_compare_threshold_e threshold,
    bool isLowSideResEnabled, bool isHighSideEnabled);

/**
 * @brief  power hsmos strength cfg
 * @param[in] pulldownThreshold hsmos_pulldown_strength_e, hsmos pulldown strength
 * @param[in] pullupThreshold hsmos_pullup_strength_e, hsmos pullup strength
 */
void Power_Hsmos_Strength_Cfg(hsmos_pulldown_strength_e pulldownThreshold, hsmos_pullup_strength_e pullupThreshold);    

/**
 * @brief  get power hsmos strength setting
 * @param[in] pulldownThreshold hsmos_pulldown_strength_e*, pointed to hsmos pulldown strength 
 * @param[in] pullupThreshold hsmos_pullup_strength_e*, pointed to hsmos pullup strength 
 */
void Power_Hsmos_Strength_Get(hsmos_pulldown_strength_e* pulldownThreshold, hsmos_pullup_strength_e* pullupThreshold);

/**
 * @brief  Vin Ovp ctrl
 * @param[in] isEnable true to enable vin ovp or it will disable
 */
void Vin_Ovp_Ctrl(bool isEnable);

/**
 * @brief  Vout Ovp ctrl
 * @param[in] isEnable true to enable vout ovp or it will disable
 */
void Vout_Ovp_Ctrl(bool isEnable);

/**
 * @brief  abnormal force control
 * @param[in] isEnable true to enable an external abnormal, otherwise to restore the external abnormal
 * @note set isEnable = true, will close buck, port1 and port2
 */
void Abn_Force_Ctrl(bool isEnable);

/**
 * @brief  DPDM short operation
 * @param portNumber port number, can be 1 or 2
 * @param isShort true to short dpdm, otherwise not short
 * @Note: the DPDM short state could be changed during the protocol switching.
 */
void Dpdm_Short_Ctrl(uint8_t portNumber, bool isShort);

/**
 * @brief  get DPDM short state
 * @param portNumber port number, can be 1 or 2
 * @return true when dpdm is short, otherwise return false
 */
bool Dpdm_Is_Short(uint8_t portNumber);

/**
 * @brief  Set voltage slew
 * @param slew voltage slew see voltage_slew_e
 */
void Power_Set_Voltage_Slew(voltage_slew_e slew);

/**
 * @brief  Get voltage slew setting
 * @return voltage step slew, see voltage_slew_e
 */
voltage_slew_e Power_Get_Voltage_Slew(void);

/**
 * @brief  Set current slew
 * @param slew current slew see current_slew_e
 */
void Power_Set_Current_Slew(current_slew_e slew);

/**
 * @brief  Get current slew setting
 * @return current step slew, see current_slew_e
 */
current_slew_e Power_Get_Current_Slew(void);


/**
 * @brief  Get UID, only avaliable in SW356R lib
 * @param uid the memory to store the uid, it shall be large or equal than 4 words
 * @Note,
 * 1) The function shall be linked in RAM, see  zr356x_dev.sct demo scatter
 * 2) How to customize scatter in keil (e.g. zr356x_dev.sct), Options -> linker -> unchecked "Use Memory Layout from Target Dialog"
 *   -> Chose the corresponding scatter file in "Scatter file"
 */
void Device_Get_Uid(uint32_t *uid);

/**
 * @brief to check whether is 5A cable
 * @return 0:3A   1:5A
 */
bool Typec_Is_5A_Cable(void);

#ifdef __cplusplus
}
#endif
#endif        // API_H
