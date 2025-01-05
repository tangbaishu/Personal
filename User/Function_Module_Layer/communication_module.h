#ifndef _COMMUNICATION_MODULE_H_
#define	_COMMUNICATION_MODULE_H_

#include "base_function.h"

#define MAX_VALID_DATA_LEN			1024		// 最大有效数据帧长度 单位：Byte

/***********	PAN ID-5A		***********/
#define CLASS_PANID_NUM				3		// 不同类别 PANID 数量
#define PANID_UART_SERVER_8BH		0x5A	// UART服务器标识符(高8位)	0x5AFF
#define PANID_UART_SERVER_8BL		0xFF	// UART服务器标识符(低8位)
#define PANID_ETH_SERVER_8BH		0x5A	// ETH服务器标识符(高8位)	0x5AFE
#define PANID_ETH_SERVER_8BL		0xFE	// ETH服务器标识符(高8位)
#define PANID_CLIENT_8BH			0x5A	// 客户端标识符(高8位)		0x5A01
#define PANID_CLIENT_8BL			0x01	// 客户端标识符(低8位)

/***********	Device ADDR-0x1A	***********/
#define CLASS_DEVICE_ADDR_NUM		2		// 不同类别 DEVICE_ADDR 数量
#define DEVICE_ADDR_SERVER_8BH		0x1A	// 服务器设备地址(高8位)	0x1A10
#define DEVICE_ADDR_SERVER_8BL	 	0x10	// 服务器设备地址(低8位)
#define DEVICE_ADDR_CLIENT_8BH		0x1A	// 客户端设备地址(高8位)	0x1A01
#define DEVICE_ADDR_CLIENT_8BL	 	0x01	// 客户端设备地址(低8位)

/***********	FRAME TYPE-0x55	***********/
#define CLASS_FRAME_TYPE_NUM		8		// 不同类别 FRAME_TYPE 数量
/*******	 16位指令集 	*******/
#define FRAME_SERVER_START_8BH		0x55	// 服务器启动(高8位)		0x5501
#define FRAME_SERVER_START_8BL		0x01	// 服务器启动(低8位)
#define FRAME_CLIENT_INSERT_8BH		0x55	// 客户端插入/请求更新(高8位)	0x5502
#define FRAME_CLIENT_INSERT_8BL		0x02	// 客户端插入/请求更新(低8位)

#define HANDSHAKE_REQUEST_8BH		0x55	// 主机(服务器)发送16位握手请求(高8位)	0x5503
#define HANDSHAKE_REQUEST_8BL		0x03	// 主机(服务器)发送16位握手请求(低8位)
#define HANDSHAKE_RESPONSE_8BH		0x55	// 设备(客户端)响应主机握手请求(高8位)	0x5504
#define HANDSHAKE_RESPONSE_8BL		0x04	// 设备(客户端)响应主机握手请求(低8位)
	/***********	DATA	***********/
#define FRAME_SERVER_TX_OTA_8BH		0x55	// 服务器发送OTA数据(高8位)	0x5505
#define FRAME_SERVER_TX_OTA_8BL		0x05	// 服务器发送OTA数据(低8位)
#define FRAME_CLIENT_RX_FINISH_8BH	0x55	// 客户端数据接收并处理完成(高8位) {完成数据接收时，已校验并发送CRC}	0x5506
#define FRAME_CLIENT_RX_FINISH_8BL	0x06	// 客户端数据接收并处理完成(低8位)
	/***********	CRC		***********/
#define FRAME_GOOD_CRC_8BH				0x55	// CRC校验通过(高8位) 0x55FF
#define FRAME_GOOD_CRC_8BL				0xFF	// CRC校验通过(低8位)
#define FRAME_BAD_CRC_8BH				0x55	// CRC校验失败(高8位) 0x5500
#define FRAME_BAD_CRC_8BL				0x00	// CRC校验失败(低8位)

#pragma pack(1)
typedef struct
{
	uint16	Sequence_Num;					// 序列号
	uint8	Dest_PANID[2];					// 目标标识符ID号
	uint8 	Dest_Addr[2];					// 目标设备地址
	uint8	Source_PANID[2];				// 源端/发送端标识符ID号
	uint8	Source_Addr[2];					// 源端/发送端设备地址
	uint8	Frame_Type[2];					// 帧类型
	uint16	Frame_Valid_Data_Len;			// 帧内有效数据长度
}OTA_Pack_Hander_t;							// OTA数据包头

typedef struct
{
	OTA_Pack_Hander_t	OTA_Pack_Hander;				// OTA数据包头
	uint8				*P_Valid_Data;					// 有效数据缓冲区首地址
	uint8				CRC[2];							// CRC循环冗余校验
}OAT_Data_Pack_t;										// OTA数据包
#pragma pack()

typedef struct
{
	void (*Uart_Init) (void);				// 串口传输驱动初始化
	void (*ETH_Init) (void);				// 以太网传输驱动初始化
	void (*CRC_Func) (void);				// CRC循环冗余校验函数
	void (*Encrpytion_Func) (void);			// 加密函数
	void (*Decode_Func) (void);				// 解密函数
}Communication_Hardware_API_t;

void Communciation_Module_Init(Communication_Hardware_API_t *pointer_api);


#endif
