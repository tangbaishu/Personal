/**
 * @file ota_business.h
 * @author tangbaishu
 * @brief OTA在线升级业务流程---采用16位指令集(任意设备接收到数据并校验通过后,立即回复"GOOD_CRC"(CRC校验通过)指令后,其后可在 "WAIT_INSTRUCT_TIME"(等待指令回复时间) 内基于发送端指令回复事件指令)
 * 1、客户端上电发送 "CLIENT_INSERT"(客户端启动/请求更新) 指令 或 服务器发送 "SERVER_START"(服务器启动) 指令
 * 2、接收端接收数据通过后立即回复"GOOD_CRC"(CRC校验通过),若失败立即回复"BAD_CRC"(CRC校验失败),发送端在接受到"BAD_CRC" 或 "WAIT_CRC_TIME"(等待CRC校验应答时间)超时后，重新发送该指令
 * 3、服务器随后发送 "HANDSHAKE_REQUEST"(主机(服务器)发送16位握手请求)
 * 4、设备接收指令并回复"GOOD_CRC",随后待空闲时发送 "HANDSHAKE_RESPONSE"(设备(客户端)响应主机握手请求)
 * 5、服务器接收并校验后，将OTA数据拆分成 "communication_module.c" (通讯模块) 定义的数据包格式，逐个发送
 * 6、设备端对接收到的数据包进行校验、处理后，发送 "DATA_RX_FINISH" 数据接收完成
0 * @version 0.1
 * @date 2024-12-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _OTA_BUSINESS_H_
#define _OTA_BUSINESS_H_

#include "communication_module.h"

#define WAIT_CRC_TIME				100		// 等待CRC校验应答时间
#define WAIT_INSTRUCT_RESP_TIME		500		// 等待指令回复时间

typedef struct  
{

	Data_Buff_Queue_t	Rec_Queue;			// 数据接收队列(对外接收数据,统一暂存至该队列)
	Data_Buff_Queue_t 	Send_Queue;			// 数据发送队列(所有待发送数据,统一移动至该队列后发送)
	OAT_Data_Pack_t 	OAT_Data_Pack;		// 数据接受队列 CRC校验通过后,按照 数据包结构逐一提取放置
											// 将待发送数据 通过CRC校验后,移动至 数据发送队列 发送
}Data_Transfer_t;
extern Data_Transfer_t Data_Transfer;



#endif
