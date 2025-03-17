#ifndef _SYS_PRJ_H_
#define _SYS_PRJ_H_

#include "stdio.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "usart.h"
#include "gpio.h"

#include "Task1.h"
#include "Task2.h"


extern uint32_t SYS_1ms_CNTR;		//sys 1ms计时器
extern uint32_t	Idle_Cyclecount;
extern BaseType_t TaskSuspend_Record;//任务暂停期间调度记录

#endif
