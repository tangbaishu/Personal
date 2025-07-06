/****************************************************************************
 * @copyright Copyright(C) 2020-2022 Ismartware Limited. All rights reserved.
 * @file user_pd_policy.h
 * @brief  user pd policy header
 * @author 005
 ****************************************************************************/
#ifndef _USER_PD_POLICY_H_
#define _USER_PD_POLICY_H_

#include "system.h"

// clang-format off

typedef struct
{
    u32 startTime;        //!< the start time of the timer
    u32 interval;         //!< the interval of delay  
} soft_timer_t;

/**
 * @brief  start a soft timer
 * @param[in]  self soft timer, allocate memory at consumer
 * @param[in]  timeInMs the interval in Ms
 */
void Soft_Timer_Start(soft_timer_t* self, u32 timeInMs);

/**
 * @brief  stop a soft timer, to avoid miss used, a timer shall be stop if it has been started
 * @param[in]  self soft timer, allocate memory at consumer
 */
void Soft_Timer_Stop(soft_timer_t* self);

/**
 * @brief  check whether soft timer is expired
 * @param[in]  self the soft timer
 * @return true when it is expired
 */
bool Soft_Timer_Is_Expired(soft_timer_t* self);


typedef enum
{
    USER_PD_POLICY_IDLE = 0,
	
	USER_PD_POLICY_READY,
    USER_PD_POLICY_SEND,
	USER_PD_POLICY_WAIT_DR_SWAP,
	USER_PD_POLICY_DISCID_REQ,
	USER_PD_POLICY_WAIT_DISCID_REQ,
    
    USER_PD_POLICY_NUM,
} user_pd_policy_state_e;

typedef struct sm_state sm_state_t;

/// Pointer functions for entry of state
typedef void (*entry_func)(void);
/// Pointer functions for run of state
typedef user_pd_policy_state_e (*run_func)(void);

/**
 * @brief The generic state definition
 * entryFunc is the first call when enter a new state
 * runFun is shall be scheduled to implement the logic of state
 */
struct sm_state
{
    entry_func entryFunc;        //!< entry funcation, set to 0 if not needed
    run_func runFunc;            //!< run funcation, shall not be NULL
};

/**
 * @brief  init user's pd policy
 */
void User_Pd_Policy_Init(void);

// clang-format on

#endif





