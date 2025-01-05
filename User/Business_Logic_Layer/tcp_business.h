#ifndef TCP_BUSINESS_H_
#define TCP_BUSINESS_H_

#include "tcp_driver.h"

typedef enum
{
	TCP_OFF,					// 因网线拔出，导致 tcp_pcb变量空间释放，关闭TCP业务功能
	TCP_CLOSE,					// TCP关闭，因异常/错误事件（服务器关闭/未应答），软件关闭TCP
	TCP_INIT,					// 准备 初始化TCP协议
	TCP_BIND_OK,				// 本地ip、端口号已在TCP上绑定
	TCP_CONNECT_UNDERWAY,		// TCP处于服务器进程中
	TCP_CONNECT_SERVER_OK,		// TCP连接服务器成功
	TCP_CONNECT_BREAK,			// TCP连接断开
	TCP_IDLE,					// TCP发送完成/处于空闲状态（接收状态不考虑）
	TCP_SEND_BUSY				// TCP数据发送中
}TCP_Business_State_e;			// 用于管理TCP业务处理状态，但未包含接收部分

typedef enum
{
	FAST_ADD = 1,				// 快速速率，每失败一次，下次连接时间增加500ms,总时间最高不超过5s + 初始值（TCP_FAST_CONNECT / TCP_NORMAL_CONNECT）
	NORMAL_ADD = 4,				// 正常速率，每失败一次，下次连接时间增加2s，总时间最高不超过15s + 初始值（TCP_FAST_CONNECT / TCP_NORMAL_CONNECT）
	SLOW_ADD = 	10				// 缓慢速率，每失败一次，下次连接时间增加5s，总时间最高不超过60s + 初始值（TCP_FAST_CONNECT / TCP_NORMAL_CONNECT）
}TCP_Rate_Mode_e;

typedef struct
{
	uint8			Real_Rate;			// 具体连接速率（由Fast_Add、Normal_Add确定速率增长值）
	TCP_Rate_Mode_e	Rate_Add_Mode;		// 速率连接模式
}TCP_Connect_Rate_t;					// TCP连接/轮询速率，基本单位：500ms

typedef struct
{
	TCP_Driver_Data_t		*Driver_Data;
	TCP_Business_State_e 	TCP_Business_State;
	TCP_Connect_Rate_t		Connect_Rate;	
}TCP_Business_t;
#define TCP_FAST_CONNECT		2;TCP_Business.Connect_Rate.Rate_Add_Mode = FAST_ADD;		// 基础值=1s 基本单位：500ms
#define TCP_NORMAL_CONNECT		10;TCP_Business.Connect_Rate.Rate_Add_Mode = NORMAL_ADD;	// 基础值=5s
#define TCP_SLOW_CONNECT		20;TCP_Business.Connect_Rate.Rate_Add_Mode = SLOW_ADD;		// 基础值=10s

extern TCP_Business_t TCP_Business;
void TCP_Task(void *param);
void TCP_Business_Pointer_Init(void);
void TCP_Business_Running(void);
void TCP_Business_Logic(void);
void TCP_Message_Receive_Manage(void);
void TCP_Anew_Connect_Server(uint8_t *dest_ip, uint16 dest_port,uint8 self_updating);

#endif
