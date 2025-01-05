#ifndef _RUNING_RECORD_H_
#define _RUNING_RECORD_H_

#include "base_function.h"

#pragma pack(32)
typedef struct
{
	uint32	DW1000_IRQ_Trigger_Count;	// 记录MCU 因DW1000_IRQ引脚上升沿，进入中断的次数

	uint32	CTL_Tx_Count;				// 控制DW1000 进入发送模式次数
	uint32	Tx_Finish_Count;			// DW1000 发送完成次数
	uint32  Tx_Timeout_Count;			// DW1000 发送超时次数

	uint32	CTL_Rx_Count;				// 控制DW1000 进入接收模式次数
	uint32	Rx_Good_Frame_Count;		// DW1000 接收好的帧次数 ( 通过DW1000帧过滤 ) 次数
	uint32	Rx_Pass_Frame_Count;		// DW1000 完成协议校验通过的帧次数（发送方的目的地址与自身匹配 ）次数
	uint32	Rx_Timeout_Count;			// DW1000 接收超时次数
	uint32	Rx_Fail_Count;				// DW1000 接收失败次数

	uint32	DW1000_Reset_Count;			// DW1000 重启次数
}Record_Dw1000_Driver_t;

typedef struct
{
	// 标签设备
	uint32	Send_Poll_NoEvent_Count;	// 发送 Poll_NoEvent 帧次数
	uint32	Send_Poll_RTC_Count;		// 发送 Poll_RTC 帧次数
	uint32	Send_Poll_TimeSYNC_Count;	// 发送 Poll_TimeSYNC 帧次数
	uint32	Rec_Resp_Ranging_Count;		// 接收 Resp_Ranging 帧次数
	uint32	Rec_Resp_RTC_Count;			// 接收 Resp_RTC 帧次数
	uint32	Send_Final_Ranging_Count;	// 发送 Final_Ranging 帧次数
	uint32	Send_Final_RTC_Count;		// 发送 Final_RTC 帧次数

	// 基站设备
	uint32	Rec_Poll_NoEvent_Count;		// 接收 Poll_NoEvent 帧次数
	uint32	Rec_Poll_RTC_Count;			// 接收 Poll_RTC 帧次数
	uint32	Rec_Poll_TimeSYNC_Count;	// 接收 Poll_TimeSYNC 帧次数
	uint32	Send_Resp_Ranging_Count;	// 发送 Resp_Ranging 帧次数
	uint32	Send_Resp_RTC_Count;		// 发送 Resp_RTC 帧次数
	uint32	Rec_Final_Ranging_Count;	// 接收 Final_Ranging 帧次数
	uint32	Rec_Final_RTC_Count;		// 接收 Final_Ranging 帧次数
}Record_Ranging_t;

typedef struct
{
	uint32	Poll_Send_Time;
	uint32	Resp_Send_Time;
	uint32	Final_Send_Time;
}Ranging_Time_t;

typedef struct
{

	uint32	ETH_Link_State;					// 仅赋值 ETH_PULLOUT = 1、ETH_INSERT = 2
	uint32	TCP_Server_Connect_State;		// 服务器连接状态 0：失败、1：成功
	uint32	TCP_Server_Poll_Count;			// 请求/发起服务器连接次数（仅记录发起）
	uint32	TCP_Server_Poll_Success_Count;	// 连接服务器请求成功次数
	uint32	TCP_Server_Connect_Error_Count;	// 服务器连接异常次数
	uint32	TCP_Server_Reconnection_Count;	// 服务器重连次数
	uint32	CTL_TCP_Send_Count;				// 控制TCP发送次数（仅调用发送指令，不考虑发送是否成功）
	uint32	TCP_Send_Finish_Count;			// TCP发送完成次数
	uint32	TCP_Rec_Count;					// TCP接收数据次数（任何数据接收都会计次，不考虑是否符合协议要求）
	uint32	TCP_Rec_Pass;					// TCP接收数据并校验通过
}Record_TCP_t;


typedef struct
{
	Record_Dw1000_Driver_t	Dw1000_Driver;
	Record_Ranging_t		Ranging_Frame;
	Record_TCP_t			Record_TCP;
}Runing_Record_t;

#pragma pack()
extern Ranging_Time_t	Ranging_Time;	
extern Runing_Record_t	Running_Record;



void Running_Record_Read(void);

#endif
