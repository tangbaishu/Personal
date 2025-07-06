/****************************************************************************
 * @copyright Copyright(C) 2020-2022 Ismartware Limited. All rights reserved.
 * @file user_pd_policy.c
 * @brief  user pd policy,this code can be reprogramming base actual,just attention hook function init!!!!!!!!!!!!!!
 * @author 005
 ****************************************************************************/
#include "zr_nvic.h"
#include "zr_rcc.h"
#include "zr_atm.h"
#include "api.h"
#include "user_pd_policy.h"
#include "pd_message.h"

#define TIME_INTERVAL  (2)        // 2ms
#define HARDWARE_TIMER_RELOAD (12500 * TIME_INTERVAL - 1)
#define HARDWARE_TIMER_IRQ (ATM0_IRQn)

static const pd_mesg_summ_t* pdRxMessageSummary;
static const u8* pdRxBuff;
static pd_mesg_summ_t* pdTxMessageSummary;
static u8* pdTxBuff;
static user_pd_policy_state_e currentState;
static user_pd_policy_state_e nextSuccessState;
static user_pd_policy_state_e nextFailState;
static bool discIdReqWork = FALSE;

static soft_timer_t userTime = {0};
static volatile u32 gSysTick = 0;
static volatile u8 gSoftTimerNumber = 0;

static void User_Typec_Attach_Hook(u8 portIndex);
static void User_Typec_Detach_Hook(u8 portIndex);
static void User_Pd_Request_Info_Hook(u8 objIndex, u16 volInMv, u16 currInMa);
static bool User_Pd_Policy_Hook(void);


// clang-format off
//user pd policy state machine
static void User_Pd_Policy_State_Change(user_pd_policy_state_e state);
// user idle
static void User_Pd_Policy_idle_Entry(void);
// user ready
static void User_Pd_Policy_Ready_Entry(void);
static user_pd_policy_state_e User_Pd_Policy_Ready_Run(void);
// user send message
static user_pd_policy_state_e User_Pd_Policy_Send_Run(void);
// user wait dr swap
static void User_Pd_Policy_Wait_Dr_Swap_Entry(void);
static user_pd_policy_state_e User_Pd_Policy_Wait_Dr_Swap_Run(void);
// user discid req
static user_pd_policy_state_e User_Pd_Policy_DiscID_Req_Run(void);
// user wait discid req
static void User_Pd_Policy_Wait_DiscId_Req_Entry(void);
static user_pd_policy_state_e User_Pd_Policy_Wait_DiscId_Req_Run(void);

// global user pd policy engines
const sm_state_t gUserPdPolicies[USER_PD_POLICY_NUM] = {
    [USER_PD_POLICY_IDLE]                                 = {User_Pd_Policy_idle_Entry, 0},

    [USER_PD_POLICY_READY]                                = {User_Pd_Policy_Ready_Entry, User_Pd_Policy_Ready_Run},
    [USER_PD_POLICY_SEND]                                 = {0, User_Pd_Policy_Send_Run}, 
    [USER_PD_POLICY_WAIT_DR_SWAP]                         = {User_Pd_Policy_Wait_Dr_Swap_Entry, User_Pd_Policy_Wait_Dr_Swap_Run},
    [USER_PD_POLICY_DISCID_REQ]                           = {0, User_Pd_Policy_DiscID_Req_Run},
    [USER_PD_POLICY_WAIT_DISCID_REQ]                      = {User_Pd_Policy_Wait_DiscId_Req_Entry, User_Pd_Policy_Wait_DiscId_Req_Run},     
};
// clang-format on

void ATM0_Handler(void)
{       
    Low_Power_Not_Sleep();  
    Atm0_Clear_Int_Pending();    
    gSysTick += TIME_INTERVAL;
}

/**
 * @brief  start a soft timer
 * @param[in]  self soft timer, allocate memory at consumer
 * @param[in]  timeInMs the interval in Ms
 */
void Soft_Timer_Start(soft_timer_t* self, u32 timeInMs)
{
    if (timeInMs == 0)
    {
        return;
    }

    if (gSoftTimerNumber == 0)
    {
        Atm0_Enable();  //enable 
    }
    
    // shall be here after clock enabled
    gSoftTimerNumber++;

    self->startTime = gSysTick;
    self->interval = timeInMs;
}

/**
 * @brief  stop a soft timer
 * @param[in]  self soft timer, allocate memory at consumer
 */
void Soft_Timer_Stop(soft_timer_t* self)
{
    if (gSoftTimerNumber && self->interval)
    {
        gSoftTimerNumber--;
    }
    self->interval = 0;

    if (!gSoftTimerNumber)
    {
        Atm0_Disable();   //disable
    }
}

/**
 * @brief  check whether soft timer is expired
 * @param[in]  self the soft timer
 * @return true when it is expired
 */
bool Soft_Timer_Is_Expired(soft_timer_t* self)
{
    return (self->interval && ((gSysTick - self->startTime) >= self->interval));
}

/**
 * @brief  change user pd policy state
 * @param state,set to specified state
 */
static void User_Pd_Policy_State_Change(user_pd_policy_state_e state)
{
    currentState = state;
    if (gUserPdPolicies[state].entryFunc)
    {
        gUserPdPolicies[state].entryFunc();
    }
}

/**
 * @brief  entry function of USER_PD_POLICY_IDLE
 */
static void User_Pd_Policy_idle_Entry(void)
{
    //clear flag
    discIdReqWork = FALSE;  
    Soft_Timer_Stop(&userTime);
}

/**
 * @brief  entry function of USER_PD_POLICY_READY
 */
static void User_Pd_Policy_Ready_Entry(void)
{
    if (discIdReqWork)
    {
        Soft_Timer_Start(&userTime, 1000);  //1s delay
    }
}

/**
 * @brief  run function of USER_PD_POLICY_READY
 */
static user_pd_policy_state_e User_Pd_Policy_Ready_Run(void)
{
    u32 vdmHeader;
    user_pd_policy_state_e returnState = USER_PD_POLICY_READY;
    
    if (Pd_Is_Event_Happen(PD_RX_MESSAGE_SUCCESS_EVENT))
    {
        if (PD_SOP == pdRxMessageSummary->packet_start &&
            PD_Vendor_Defined == pdRxMessageSummary->mesg_type)
        {
            pdRxBuff = Pd_Get_Rx_Buff_Start_Address(FALSE);
            vdmHeader = *(u32*)(pdRxBuff + 2);
            if (0xff008001 == (vdmHeader & 0xffff801f))  //discid req
            {
                Pd_Clear_Event(PD_RX_MESSAGE_SUCCESS_EVENT);  //user process discid req
                
                //construct tx message connect
                pdTxMessageSummary->packet_start = PD_SOP;
                pdTxMessageSummary->mesg_class = PD_DATA_CLASS;
                pdTxMessageSummary->mesg_type = PD_Vendor_Defined;
                pdTxBuff = Pd_Get_Tx_Buff_Start_Address(FALSE);
                *(u32*)(pdTxBuff + 2) = 0xff008041;  //vdm header
                *(u32*)(pdTxBuff + 6) = 0x01c02f16;  //id header
                *(u32*)(pdTxBuff + 10) = 0;  //cert vdo
                *(u32*)(pdTxBuff + 14) = 0;  //product vdo
                *(u32*)(pdTxBuff + 18) = 0x40000000;  //product type vdo                
                Pd_Tx_Message(5, pdTxBuff);
                
                nextSuccessState = USER_PD_POLICY_READY;
                nextFailState = USER_PD_POLICY_READY;
                
                returnState = USER_PD_POLICY_SEND;
            }
        }
        //others message is ingored
    }
    
    if (discIdReqWork && Soft_Timer_Is_Expired(&userTime))
    {
        if (Pd_Is_Dfp())  //if dfp,swap to ufp
        {
            //construct tx message connect
            pdTxMessageSummary->packet_start = PD_SOP;
            pdTxMessageSummary->mesg_class = PD_CONTROL_CLASS;
            pdTxMessageSummary->mesg_type = PD_DR_Swap;
            pdTxBuff = Pd_Get_Tx_Buff_Start_Address(FALSE);         
            Pd_Tx_Message(0, pdTxBuff);
            
            nextSuccessState = USER_PD_POLICY_WAIT_DR_SWAP;
            nextFailState = USER_PD_POLICY_READY;
            
            returnState = USER_PD_POLICY_SEND;      
        }
        else
        {
            returnState = USER_PD_POLICY_DISCID_REQ;    
        }
    }
    
    if (USER_PD_POLICY_READY != returnState)
    {
        Soft_Timer_Stop(&userTime);
    }
    return returnState;
}

/**
 * @brief  run function of USER_PD_POLICY_SEND
 */
static user_pd_policy_state_e User_Pd_Policy_Send_Run(void)
{
    if (Pd_Is_Event_Happen(PD_TX_MESSAGE_SUCCESS_EVENT))
    {
        Pd_Clear_Event(PD_TX_MESSAGE_SUCCESS_EVENT);
        return nextSuccessState;            
    }
    if (Pd_Is_Event_Happen(PD_TX_MESSAGE_FAIL_EVENT))
    {
        Pd_Clear_Event(PD_TX_MESSAGE_FAIL_EVENT);
        Pd_Send_Soft_Reset();  //send soft reset
        return nextFailState;           
    }
    return USER_PD_POLICY_SEND; 
}

/**
 * @brief  entry function of USER_PD_POLICY_WAIT_DR_SWAP
 */
static void User_Pd_Policy_Wait_Dr_Swap_Entry(void)
{
    Soft_Timer_Start(&userTime, 30);  //tSenderResponse
}

/**
 * @brief  run function of USER_PD_POLICY_WAIT_DR_SWAP
 */
static user_pd_policy_state_e User_Pd_Policy_Wait_Dr_Swap_Run(void)
{
    user_pd_policy_state_e returnState = USER_PD_POLICY_WAIT_DR_SWAP;
    
    if (Pd_Is_Event_Happen(PD_RX_MESSAGE_SUCCESS_EVENT))
    {
        Pd_Clear_Event(PD_RX_MESSAGE_SUCCESS_EVENT);
        if (PD_SOP == pdRxMessageSummary->packet_start)
        {
            if (PD_Accept == pdRxMessageSummary->mesg_type)
            {
                Pd_Set_Data_Role(FALSE);  //to ufp
                returnState = USER_PD_POLICY_DISCID_REQ;
            }
            else if (PD_Reject != pdRxMessageSummary->mesg_type && PD_Not_Supported != pdRxMessageSummary->mesg_type)   
            {
                Pd_Send_Soft_Reset();  //send soft reset
                returnState = USER_PD_POLICY_READY;
            }               
        }
    }
    
    if (Soft_Timer_Is_Expired(&userTime))
    {
        returnState = USER_PD_POLICY_READY; 
    }
    
    if (USER_PD_POLICY_WAIT_DR_SWAP != returnState)
    {
        Soft_Timer_Stop(&userTime);
    }

    return returnState; 
}

/**
 * @brief  run function of USER_PD_POLICY_DISCID_REQ
 */
static user_pd_policy_state_e User_Pd_Policy_DiscID_Req_Run(void)
{
    //construct tx message connect
    pdTxMessageSummary->packet_start = PD_SOP;
    pdTxMessageSummary->mesg_class = PD_DATA_CLASS;
    pdTxMessageSummary->mesg_type = PD_Vendor_Defined;
    pdTxBuff = Pd_Get_Tx_Buff_Start_Address(FALSE);
    *(u32*)(pdTxBuff + 2) = 0xff008001;  //vdm header       
    Pd_Tx_Message(1, pdTxBuff);
    
    nextSuccessState = USER_PD_POLICY_WAIT_DISCID_REQ;
    nextFailState = USER_PD_POLICY_READY;
    
    return USER_PD_POLICY_SEND;
}

/**
 * @brief  entry function of USER_PD_POLICY_WAIT_DISCID_REQ
 */
static void User_Pd_Policy_Wait_DiscId_Req_Entry(void)
{
    Soft_Timer_Start(&userTime, 30);  //tSenderResponse
    discIdReqWork = FALSE;
}

/**
 * @brief  run function of USER_PD_POLICY_WAIT_DISCID_REQ
 */
static user_pd_policy_state_e User_Pd_Policy_Wait_DiscId_Req_Run(void)
{
    user_pd_policy_state_e returnState = USER_PD_POLICY_WAIT_DISCID_REQ;
    
    if (Pd_Is_Event_Happen(PD_RX_MESSAGE_SUCCESS_EVENT))
    {
        Pd_Clear_Event(PD_RX_MESSAGE_SUCCESS_EVENT);
        if (PD_SOP == pdRxMessageSummary->packet_start &&
            PD_Vendor_Defined == pdRxMessageSummary->mesg_type)
        {
                
        }
        returnState = USER_PD_POLICY_READY; 
    }
    
    if (Soft_Timer_Is_Expired(&userTime))
    {
        returnState = USER_PD_POLICY_READY; 
    }
    
    if (USER_PD_POLICY_WAIT_DISCID_REQ != returnState)
    {
        Soft_Timer_Stop(&userTime);
    }   
    
    return returnState;
}


/**
 * @brief  init user's pd policy
 */
void User_Pd_Policy_Init(void)  //when vin power on,call this function to init user pd process
{
    atm_config_t cfg;
    cfg.mode = ATM_SOURCE_COUNTER;
    cfg.reload = HARDWARE_TIMER_RELOAD;     
    Rcc_Module_Clk_Enable(ATM_ID);
    Atm0_Init(&cfg);
    Atm0_Int_Enable();
    Atm0_Disable();  //init timer
    Nvic_Clear_Irq_Pending(HARDWARE_TIMER_IRQ);
    Nvic_Irq_Enable(HARDWARE_TIMER_IRQ, PRIORITY_3);   //en irq channel     

    pdRxMessageSummary = Pd_Init_Rx_Message_Summary();   //get rx message summary 
    pdTxMessageSummary = Pd_Init_Tx_Message_Summary();   //get tx message summary 
    User_Pd_Policy_State_Change(USER_PD_POLICY_IDLE);       //init state to idle
    
    #define USER_PD_POLICY
    #ifdef USER_PD_POLICY
    //must init valid fuction pointer,then this fuction can be called
    Typec_Init_Hook_Func(User_Typec_Attach_Hook, User_Typec_Detach_Hook);
    Pd_Init_Hook_Func(User_Pd_Request_Info_Hook, User_Pd_Policy_Hook);
    #else
    //if want hook function not be called,init to 0 
    User_Init_Typec_Hook_Func(0, 0);
    User_Init_Pd_Hook_Func(0, 0);
    #endif
}

/*************************************hook function******************************************************/
/**
 * @brief  it is a hook function, called only once at typec attach
           it mean when attach event happen,this function will be called, only once
 * @param portIndex,which port's typec attach, 0:port1  1:port2
 */
static void User_Typec_Attach_Hook(u8 portIndex)
{
/**
    modify code according to the actual situation
 */
    if (!portIndex)  //in port1
    {
        discIdReqWork = TRUE;  //when typec attach ,excute:1) drswap to ufp 2)discid req    
    }
}

/**
 * @brief  it is a hook function, called only once at typec detach
           it mean when detach event happen,this function will be called, only once
 * @param portIndex,which port's typec attach, 0:port1  1:port2
 */
static void User_Typec_Detach_Hook(u8 portIndex)
{
/**
    modify code according to the actual situation
 */
	if (Typec_Is_Cc1_Connect(portIndex))
	{
		//portIndex's cc1 connect device
		//to do...
	}
    if (!portIndex)  //in port1
    {
		
        User_Pd_Policy_State_Change(USER_PD_POLICY_IDLE);       //init state to idle    
    }
}

/**
 * @brief  rx pd request message information.it is a hook function, called only once at rx request message
           it mean as long as rx valid request message,this function will be called, only once
 * @param objIndex,which object data in src cap
 * @param volInMv,request vol in mv
 * @param currInMa,request current in ma
 */
static void User_Pd_Request_Info_Hook(u8 objIndex, u16 volInMv, u16 currInMa)
{
/**
    modify code according to the actual situation
 */
}

/**
 * @brief  policy of user handler pd work,eg rx/tx vdm message.it is a hook function, called by pd main process
           it mean as long as main loop execute to pd main process,this function will be called once
 * @return when true ,user is processing
 */
static bool User_Pd_Policy_Hook(void)
{
/**
    modify code according to the actual situation,
    but return value must have!!!!!!!!!!!!!!!!
    this example implements the functions:
    1)response ack for discid req
    2)after attach 1s,if dfp,swap to ufp
    3)disc id req in ufp
 */
    user_pd_policy_state_e stateTemp;
    
    if (USER_PD_POLICY_IDLE == currentState)
    {
        if (Pd_Is_Ready())  //pd main process is ready
        {
            User_Pd_Policy_State_Change(USER_PD_POLICY_READY);   
        }
    }
    else
    {
        if (!Pd_Is_Ready())
        {
            User_Pd_Policy_State_Change(USER_PD_POLICY_IDLE);   
        }   
    }
    
    if (gUserPdPolicies[currentState].runFunc)
    {
        stateTemp = gUserPdPolicies[currentState].runFunc();
        if (stateTemp != currentState)
        {
            User_Pd_Policy_State_Change(stateTemp);    
        }
    } 
    
    if (currentState > USER_PD_POLICY_READY)
    {
        return TRUE;  //in process
    }
    return FALSE; 
}


