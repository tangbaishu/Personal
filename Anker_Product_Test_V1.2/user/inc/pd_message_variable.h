#ifndef _PD_MESSAGE_VARIABLE_H_
#define _PD_MESSAGE_VARIABLE_H_

#include "system.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef union
{
	uint8_t Memory_Area[2];
	struct
	{
		uint8_t Message_Type : 5;
		uint8_t	Reserved_4_5 : 1;
		uint8_t	Specification_Revision : 2;
		bool	Cable_Plug : 1;
		uint8_t	Message_ID : 3;
		uint8_t Number_of_Data_Objects : 3;
		uint8_t Reserved_15 : 1;
	};
}PD_Message_Header_u;

typedef union
{
	uint8_t	Memory_Area[4];
	struct
	{
		uint8_t		Command : 5;				// 0		保留不得使用
												// 1		Discover Identity
												// 2		Discover SVIDS
												// 3		Discover Modes
												// 4		Enter Mode
												// 5		Exit Mode
												// 6		Attention
												// 7~15 	保留不得使用
												// 16~31	SVID Specific Commands
		bool		Reserved : 1;
		uint8_t		Command_Type : 2;			// 0b		REQ
												// 1b		ACK
												// 2b		NAK
												// 3b		BUSY
		uint8_t		Object_Position : 3;		// 对于进入、退出模式以及注意指令（请求/响应）
												// 0b		保留不得使用
												// 1~6b		从VDO列表中索引，以确定所需的备用模式
												// 7b		退出所有活动模式（相当于开机复位）。
												//			仅能与命令一起使用
												// 当Command == 0~3、7~15时，保留不得使用
		uint8_t		VDM_Version_Minor : 2;		// 次要版本
												// 00b		Version 2.0(适用于PD3.1,V1.6之前的版本)
												// 01b		Version 2.1(适用于PD3.1,V1.6之后的版本)
												// other	保留和不得使用
		uint8_t	 	VDM_Version_Major : 2;		// 主要版本
												// 00b		Version 1.0
												// 01b		Version 2.0
												// other	保留和不得使用
		bool		VDM_Type : 1;				// 默认为1,结构化VDM
		uint16_t	SVID : 16;					// 标准或供应商标识符：由USB-IF分配的唯一16bit标识符
	};
}VDM_Header_u;

typedef union
{
	uint8_t Memory_Area[4];
	struct
	{
		uint16_t	USB_Vendor_ID : 16;				// USB-IF为该线缆分配的VID编号
		uint16_t	Reserved : 10;					// 保留，应该设置为0
		bool		Modal_Operation_Supported : 1;	// 1 	支持模式
		uint8_t		Cable_Plug_Type : 3;			// 100 	活动线缆 SOP' Product Type
		bool		USB_Device : 1;					// 0 	不具备通信/数据处理功能
													// 1	具备数据处理功能
		bool		USB_Host : 1;					// 0	不具备作为USB主机，进行USB通信的功能
	};
}ID_Header_VDO_u;

typedef union
{
	uint8_t Memory_Area[4];							// USB-IF为该线缆分配的XID编号
	struct
	{
		uint8_t Data_sub0;
		uint8_t Data_sub1;
		uint8_t Data_sub2;
		uint8_t Data_sub3;
	};
}Cert_Stat_VDO_u;

typedef union
{
	uint8_t Memory_Area[4];
	struct
	{
		uint16_t DCD_Device : 16;						// 电缆供应商指定的驱动编号
		uint16_t USB_Product_ID : 16;					// 电缆供应商指定的产品编号
	};
}Product_VDO_u;

typedef union
{
	uint8_t Memory_Area[4];
	struct
	{
		uint8_t Data_sub0;
		uint8_t Data_sub1;
		uint8_t Data_sub2;
		uint8_t Data_sub3;
	};
}Cable_VDO1_u;

typedef union
{
	uint8_t Memory_Area[4];
	struct
	{
		uint8_t Data_sub0;
		uint8_t Data_sub1;
		uint8_t Data_sub2;
		uint8_t Data_sub3;
	};
}Cable_VDO2_u;

typedef union
{
	uint8_t Memory_Area[4];
	struct
	{
		uint8_t Data_sub0;
		uint8_t Data_sub1;
		uint8_t Data_sub2;
		uint8_t Data_sub3;
	};
}VDO_u;

#pragma pack(1)
typedef union
{
	PD_Message_Header_u	PD_Message_Header;
	VDM_Header_u		VDM_Header;
	ID_Header_VDO_u		ID_Header_VDO;
	Cert_Stat_VDO_u		Cert_Stat_VDO;
	Product_VDO_u		Product_VDO;
	Cable_VDO1_u		Cable_VDO1;
	Cable_VDO2_u		Cable_VDO2;
}PD_Discover_ID_Packaget_u;					// PD discover identify command response package

typedef union
{
	uint8_t Memory_Area[32];
	struct
	{
		PD_Message_Header_u	PD_Message_Header;	// 2Byte
		VDM_Header_u		VDM_Header;			// 4Byte
		VDO_u				MagicNumber;		// 4Byte
		VDO_u				VDO_Array[5];		// 20Byte
		uint8_t				Memory_Sub;			/* 1Byte	作为数组存储时的数据下标
												 	互斥锁	当该值 == 0 时，可以进行写操作、
													当该值 != 0时，仅能进行读操作（当可以释放互斥状态）
												*/
		uint8_t				Product_Test_Step;	/* 1Byte	
												// 0: 无事件发生
												// 1: PD回调函数 接收到安克UVDM 请求包
												// 2：UVDM数据包解析完成并通过UART-TX发生给MCU
												// 3：接收到MCU返回的PD UVDM应答包
												// 4：已将PD UVDM应答包装载入 PD_Tx_Buff中
												// 0：在PD_TX完成中断事件内将该事件清零
												*/
	};
}PD_UVDM_Anker_Package_t;



#pragma pack()


#ifdef __cplusplus
}
#endif

#endif

