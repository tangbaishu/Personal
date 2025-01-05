#ifndef TCP_DRIVER_H_
#define TCP_DRIVER_H_

#include "stm32f4xx_hal.h"
#include "base_function.h"
#include "tcp.h"
// #include "tcpbase.h"

#define TCP_DRIVER_ENABLE

#define PHY_IC_REST() 				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_RESET);


#define	TCP_POLL_CALLBACK_TIME		1000	//	调用poll_callback()函数，查询TCP连接状态时间 \
												注：该值需要是 ONCE_POLL_TIME 的整数倍
#define TCP_ONCE_POLL_TIME			500		//	单次轮询耗时

#define TCP_STATE_CONTINUE_COUNT	2		//	进入poll_callback()函数时，存在连续相同状态的次数		

#define TCP_STATE_GET(tpcb)			tcp_dbg_get_tcp_state(tpcb)

#define ETH_PULLOUT					1
#define ETH_INSERT					2


typedef struct
{
	uint8 ETH_Insert_State;	 // ETH_PULLOUT 网线未插入 / ETH_INSERT 网线插入
}ETH_Hardware_State_t;



/**
 *	enum tcp_state {
		CLOSED      = 0,
		LISTEN      = 1,
		SYN_SENT    = 2,		
		SYN_RCVD    = 3,
		ESTABLISHED = 4,
		FIN_WAIT_1  = 5,
		FIN_WAIT_2  = 6,
		CLOSE_WAIT  = 7,
		CLOSING     = 8,
		LAST_ACK    = 9,
		TIME_WAIT   = 10
	};
 * 
 */
typedef struct
{
	enum tcp_state	Previous_state;	// 上一个状态
	int8	Unusual_State;			// 长时间/连续多次处于通过一个状态
	int 	State_Continue_Time;	// 状态持续时间 单位：ms
	int8	State_Continue_Count;	// 进入poll_callback()函数时，状态持续次数
	int8	Error_State;			// 由tcp底层驱动 TCP_Error_Callback() 返回的错误状态
	uint8	Tcp_Pcb_Clear_Flag;		// tcp_pcb 清除标志 0清除、1 已为 tcp_pcb 变量分配空间
}TCP_Driver_State_t;

#pragma pack(1)
typedef struct
{
	uint8				Slef_IP[4];			// 自身IP地址数据
	uint8				Slef_Mask[4];		// 自身IP地址数据
	uint8				Slef_Gateway[4];	// 自身IP地址数据

	uint8				Server_IP[4];		// 服务器IP地址数据
}IP_List_t;									// IP数据列表


typedef struct
{
	IP_List_t			IP_List;			// IP数据列表
	ip4_addr_t			Client_IP_Addr;		// 自身IP地址
	uint16				Client_Port;		// 自身业务端口号

	ip4_addr_t 			Server_IP_Addr;		// 服务器IP地址
	uint16				Server_Port;		// 服务器端口号
}IP_Config_t;

typedef struct
{
	IP_Config_t			IP_Config;				// IP地址、掩码、网关配置
}TCP_Driver_Config_t;

typedef struct
{
	uint8_t		Rec_Count;						// 接收次数
	uint16_t	Rec_Data_Len;					// 接收数据长度/剩余接收未处理数据长度
	uint8		Rec_buff[1536];					// 接收数据缓存区
}TCP_Receive_Info_t;
#pragma pack()

typedef struct
{
	ETH_Hardware_State_t	ETH_Hardware_State;	// ETH外设硬件状态 （网线插入状态）
	TCP_Driver_Config_t		Config;
	struct tcp_pcb 			*TCP_Driver_Pcb;	// TCP 协议控制块
	struct netif 			TCP_Driver_netif;	// 所有LWIP网络接口的通用数据结构（在ETH初始化时，完成实例化）
	TCP_Driver_State_t		TCP_Driver_State;
	TCP_Receive_Info_t		TCP_Receive_Info;	// TCP接收信息
}TCP_Driver_Data_t;

extern TCP_Driver_Data_t TCP_Driver_Data;

/**
 * PD3  -> Ethernet_PHY_LAN8720_Reset
 * Reset:0  Normal:1
 * 初始化并控制IO口实现 LAN8720复位
 */
void Ethernet_GPIO_Init(void);

Return_Status TCP_Driver_Init(void);
Return_Status TCP_Driver_Bind(void);
Return_Status TCP_Driver_Connect(uint8_t *dest_ip, uint16 dest_port, tcp_connected_fn call_back_fun);
void TCP_Send_Massage(const char *data_pointer, int data_len, uint8 now_send);
void TCP_Receive_Massage(void);
void TCP_Error_Printf(err_t err);	// 错误状态打印
void TCP_State_Printf(uint8 state_addr);

/**
 * TCP底层库函数解析：
 * 函数名称：void tcp_arg(struct tcp_pcb *pcb, void *arg)
 * 作用：用于给回调函数( tcp_sent_fn sent 、 tcp_recv_fn recv 、tcp_connected_fn connected
 * tcp_poll_fn poll、 tcp_err_fn errf )传递参数，使在调用多个函数的过程中，共享参数。
 * 函数名称：void tcp_abort(struct tcp_pcb *pcb)
 * 作用：通过向远程发送RST
 */
#endif

