/****************************************************************************
 * @copyright Copyright(C) 2024-2024 Ismartware Limited. All rights reserved.
 * @file iic_slave_global_variables_policy.h
 * @brief main_policy
 * @author SZ team
 ****************************************************************************/
#ifndef IIC_Slave_Global_Variables_Policy_H
#define IIC_Slave_Global_Variables_Policy_H

#include "system.h"
#include "ufcs_caps.h"
#include "zr_uart.h"
#include "zr_gpio.h"
#include "zr_nvic.h"
#include "zr_rcc.h"
#include <stdint.h>

// 寄存器缓存大小
#define FIFO_MAXSIZE 64
// 寄存器大小
#define REG_MAXSIZE 0x100

#define TIMEMARKMASK 0X7FFFFFFF

typedef u8 Elemtype;

typedef struct
{
	Elemtype data[FIFO_MAXSIZE];
	int front; // 队列前指针
	int rear;  // 队列后指针
} SqQueue;

typedef struct
{
	Elemtype data[FIFO_MAXSIZE];
	int front; // 队列前指针
	int rear;  // 队列后指针
	u8 length; // 队列当前数据
} AnkerSqQueue;

// 策略层变量
typedef struct
{
	// 实际存储寄存器数据数组
	u8 regDataArray[REG_MAXSIZE];
	// 临时存储IIC接收寄存器地址，防止主MCU操作IIC过于频繁的缓冲区Fifo
	SqQueue regAddrFifo;
	// PD_PDO
	volatile u32 pdSrcCapArry[9];
	// UFCS_PDO
	volatile ufcs_output_cap_t ufcsOutputCap[3];
	// IIC设备地址
	u8 iicDeviceAddr : 8;
	// 系统定时器变量
	volatile u32 systemTimeCounter;
	// 主循环开始运行
	bool startRun : 1;
	// 不允许调压调流,当主动调压config_apply时
	bool notAllowAdjustVolCur : 1;
	// 发生重新广播PD_PDO,需要关闭ovp保护，防止直充类协议
	bool isOccBroadcast : 1;
	// 开始休眠标志
	bool isStartSleep : 1;
	// 发生调压调流
	bool isAdjustCurVol : 1;

	// Anker产测功能数据
	AnkerSqQueue curAnkerSqQueue;
	uint8_t ankerProductTestStep;

} policy_data_t;

// 初始化队列
void Init_SqQueue(SqQueue *AddrFifo);
// 判断队列是否为空
bool SqQueue_IsEmpty(SqQueue *AddrFifo);
// 队列元素入队
void Enter_SqQueue(SqQueue *AddrFifo, u8 regAddr);
// 队列元素出队
void Depart_SqQueue(SqQueue *AddrFifo, u8 *regAddr);

// Anker初始化队列
void Anker_Init_SqQueue(AnkerSqQueue *AddrFifo);
// Anker判断队列是否为空
bool Anker_SqQueue_IsEmpty(AnkerSqQueue *AddrFifo);
// Anker队列元素入队
void Anker_Enter_SqQueue(AnkerSqQueue *AddrFifo, u8 regAddr);
// Anker队列元素出队
void Anker_Depart_SqQueue(AnkerSqQueue *AddrFifo, u8 *regAddr);
// Anker队列元素数量
uint8_t Anker_SqQueue_Length(AnkerSqQueue *AddrFifo);

// 获取寄存器地址值
u8 Get_RegAddr_Data(u8 regAddr);
// 设置寄存器的值
void Set_RegAddr_Data(u8 regAddr, u8 regData);

// 获取与时间标志相差的时间
u32 My_GetSystemTimePass(u32 tm);
// 获取当前时间标志
u32 My_GetSystemTimeMark(void);

#endif
