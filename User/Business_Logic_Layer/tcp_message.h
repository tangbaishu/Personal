#ifndef TCP_MESSAGE_H_
#define TCP_MESSAGE_H_

#include "base_function.h"

#define TCP_MESSAGE_PACK_HEAD	0x0f0e0d0c
// #define TCP_MESSAGE_REC_BUFF	100			/**TCP消息接收数据长度（不包含包头部分） */
#define	MODE_REV_16BIT(bit16)			__REVSH(bit16)
#define MODE_REV_INT32(bit32)			__REV(bit32)
#define MODE_REV_SHORT16(word16)		__REV16(word16)

typedef enum
{
	PASS = 0,
	FAIL = 1
}TCP_Return_e;

typedef enum
{
	MESSAGE_UNSEND,						// 未完成设备注册 / 未执行测距距离上传
	MESSAGE_SENDING,					// 已发送设备注册请求 / 测距距离上传中
	MESSAGE_FINISH,						// 已接收设备注册应答 / 测距距离上传完成
	MESSAGE_ACK							// 发送消息被应答
}Course_e;

typedef enum { 							/** 设备类型 */
    TYPE_DEFAULT = 0, 					/** 备类型定义为0，这类设备没有意义 */
    TYPE_TAG_UWB = 1, 					/** 标签设备类新定义为 1 */
    TYPE_ANCHOR_UWB = 1 << 1, 			/** 基站设备类新定义为 2 */
    TYPE_CRANE_CONTROL_LER = 1 << 2 	/** 天车控制器设备类型定义为 4 */
}DeviceType_e;

typedef enum { 							/** 传输协议中的命令，即协议所包含的数据的业务类型 */
    CMD_DEFAULT = 0,  					/** 默认命令，即无效命令 */
    CMD_DEVICE_REGIST = 1,  			/** 仅由基站/标签发送 	数据结构 RadarBaseCfg_t			设备注册命令 */
    CMD_DEVICE_REGIST_RESPONSE = 2,		/** 仅接收处理			数据结构 DeviceRegistResponse_t	控制器应答命令 */
    CMD_TAGCFG = 3,  					/** 仅接收处理			数据结构 RadarBaseCfg_t			标签配置命令 */
    CMD_ANCHOR_CFG = 4,					/** 仅接收处理			数据结构 RadarBaseCfg_t			基站配置命令 */
    CMD_TIMESTAMP_SYNC = 5,				/** 仅接收处理			数据结构 TimeStampSync_t		时间同步命令 */
    CMD_DISTANCE_DATA = 6,				/** 仅由基站发送		数据结构 DistanceData_t			测距数据命令 */
}TCP_Cmd_Type_e;

#pragma pack(1)
typedef struct
{							 			/** 通讯协议对象头 */
	int Head;				 			/** 协议头，固定为 0x0f0e0d0c **/
	int DateLen;			 			/** 除协议头之外的数据长队，即对应的Playload 数据长度 */
	int Version;			 			/** 协议版本，该协议的版本，初始为 0x01000001, 对应为1.0.0.1，每一个位最大值为255*/
	int Crc;				 			/** 除协议头之外的数据Crc，即对应的Playload 数据Crc */
	int Cmd;				 			/** 命令，即具体协议，每个协议头之后的Playload 部分所包含的数据必须是同一类型的数据，不允许多种数据混杂*/
	short DeviceId;			 			/** 设备id,该消息来源的设备id值*/
	int DeviceType; 					/** 设备类型，太消息来源的设备类型 */
	uint8 *Payload;		 				/** 数据区，该定义用于占位，指向数据区的首地址 */
}TCP_Message_Header_t;

#define TCP_MESSAGE_HEADER_LEN			(sizeof(TCP_Message_Header_t) - sizeof(uint8 *))

typedef struct
{									 	/** （基站/标签）基础配置信息 */
	int CraneDeviceType;	 			/** 设备类型 */
	short ID;						 	/** UWB ID，作为基站时是基站ID，作为标签时是标签ID */
	int PanID;						 	/** UWB PanID */
	int TransmitPower;				 	/** 无线发射功率，单位毫瓦（mW） */
	unsigned int IPAddrs;			 	/** 雷达设备硬件地址 */
	unsigned short Port;			 	/** 雷达设备硬件服务端口 */
	unsigned int ServerIPAddrs;		 	/** 对接的控制器服务地址 */
	unsigned short ServerPort;		 	/** 对接的控制器服务端口 */

	int IsMove;	 						/** 该值由控制器提供，默认值为0	该设备在测距中是移动方还是静止方，1表示移动，0表示静止 */
	int IssubCrane; 					/** 该值由控制器提供，默认值为0	属于运动的一方，则不在大车上就应该在小车上，如果在打车上且需要和小车测距是，则看作静止方 */
	int leftWidth;	 					/** 该值由控制器提供，默认值为0 设备部署距离天车左侧的距离，该防线指向平行于轨道的负半轴方向 */
	int X;						 		/** 该功能未添加，保持默认值为0	原点坐标选择天车左侧轨道端点为原点，相对坐标x轴，标识离原点的平行于轨道坐标，单位毫米 */
	int Y;						 		/** 该功能未添加，保持默认值为0	原点坐标选择天车左侧轨道端点为原点，相对坐标y轴，标识离原点的垂直于轨道坐标，单位毫米 */
	int Z;						 		/** 该功能未添加，保持默认值为0	原点坐标选择天车左侧轨道端点为原点，相对坐标z轴，标识离原点的高度，单位毫米 */
	char BTID[16];				 		/** 蓝牙设备名称	蓝牙广播，用于保存备注，比如哪条跨哪台车，用户方便查找设备 */
}RadarBaseCfg_t;

typedef struct 
{										/** 网络时间同步协议 */
    unsigned int Second; 				/** 年、月、日、时、分、秒部分 */
    unsigned int MSecond; 				/** 毫秒秒部分 */
}TimeStampSync_t;

typedef struct 
{/** 注册应答消息协议 */
    int RegistResult; /** 注册结果，1标识成功，0标识失败 */
    //RadarBaseCfg Cfg; /** 成果返回时携带最新的配置信息 */
    char error[128];  /** 错误信息 */
}DeviceRegistResponse_t;

typedef struct
{
	Course_e	Device_Register_Course;		// 0:未完成设备注册, 1:已发送设备注册请求, 2: 已接收设备注册应答
}TCP_Message_Course_t;

typedef struct
{
	RadarBaseCfg_t			DeviceRegist;			// 1 标签/基站首次联网后，上报注册消息包
	DeviceRegistResponse_t	DeviceRegistResponse;	// 2 控制器接收到消息包后，应答消息包
	RadarBaseCfg_t			TAGCfg;					// 3 控制器下发的 标签配置数据
	RadarBaseCfg_t			BaseStationCfg;			// 4 控制器下发的 基站配置数据
	TimeStampSync_t			TimeStampSync;			// 5 控制器下发的时间同步命令
}TCP_Message_Data_t;

typedef struct
{
	TCP_Message_Header_t	Rec_Header;		// 所有数据接收 TCP包头
	uint8					*Data_Buff;			// 接收数据缓存区
}TCP_Message_Rec_t;

typedef struct
{
	int		SYS_Version;	// 系统版本号
}TCP_Device_Info_t;


typedef struct
{
	TCP_Message_Header_t	Transfer_Header;	// 所有数据发送 TCP包头
	TCP_Message_Data_t		Message_Group;		// 所有消息实例的集合
	TCP_Device_Info_t		TCP_Device_Info;	// 与TCP相关的设备信息
	TCP_Message_Course_t	TCP_Message_Course;	// TCP业务进程
	TCP_Message_Rec_t		TCP_Message_Rec;	// 接收消息缓存区
}TCP_Message_Group_t;
#pragma pack()

extern volatile TCP_Message_Group_t 	TCP_Message_Group;	// 与控制器进行TCP交互的 消息集

TCP_Return_e TCP_Header_Check(TCP_Message_Header_t *tcp_header);						// 对 TCP 包头校验
void TCP_Header_Init(void);																// 基于设备数据，生成TCP 包头部分,在组包时，不在对包头进行全面修改。
TCP_Return_e TCP_Device_Regist(RadarBaseCfg_t *deviceregist);							// 标签/基站首次联网后，上报注册消息包
TCP_Return_e TCP_Device_Regist_Response(DeviceRegistResponse_t *deviceregist_response);	// 控制器接收到消息包后，应答消息包
TCP_Return_e TCP_Device_Config(RadarBaseCfg_t *config);									// 控制器下发的 基站/标签 配置数据
TCP_Return_e TCP_TimeStamp_Sync(TimeStampSync_t *timestamp_sync);						// 控制器下发的时间同步命令
void TCP_Message_Pack_Printf(TCP_Message_Header_t *tcp_header, uint8 pack_type);
void TCP_ETH_Header_Rev(TCP_Message_Header_t *tcp_header);
void TCP_ETH_Config_Rev(RadarBaseCfg_t *config_data);
#endif
