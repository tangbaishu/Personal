#include "tcp_driver.h"
#include "uart_debug_driver.h"
#include "lwip.h"
#include "arch.h"
#include "string.h"

const char ETH_Err_Event[][30]={
"NO ERR",						/*0 没有错误事件*/		\
"Out of memory error",			/*1 内存不足错误*/		\
"Buffer error",					/*2 缓冲区错误*/		\
"Timeout",						/*3 超时*/				\
"Routing problem",				/*4 路径问题*/			\
"Operation in progress",		/*5 作业进行中*/		\
"Illegal value",				/*6 非法值*/			\
"Operation would block",		/*7 操作会阻塞*/		\
"Address in use",				/*8 使用中的地址*/		\
"Already connecting",			/*9 已经连接*/			\
"Conn already established",		/*10 连接已经建立*/		\
"Not connected",				/*11 未连接*/			\
"Low-level netif error",		/*12 低级 netif 错误*/	\
"Connection aborted",			/*13 连接失败*/			\
"Connection reset",				/*14 连接复位*/			\
"Connection closed",			/*15 连接关闭*/			\
"Illegal argument"				/*16 非法参数*/			\
};	
const char ETH_State[][12]={
"CLOSED",						/*0 关闭的*/			\
"LISTEN",						/*1 倾听*/				\
"SYN_SENT",						/*2 发送SYN*/			\
"SYN_RCVD",						/*3 接收SYN*/			\
"ESTABLISHED",					/*4 TCP连接成功*/		\
"FIN_WAIT_1",					/*5 发送FIN(TCP结束帧) 1，下一步等待被关闭方确认信息*/					\
"FIN_WAIT_2",					/*6 发送FIN(TCP结束帧) 2*/					\
"CLOSE_WAIT",					/*7 结束等待、TCP连接断开*/		\
"CLOSING",						/*8 */					\
"LAST_ACK",						/*9 */					\
"TIME_WAIT"						/*10 */					\
};		

TCP_Driver_Data_t 	TCP_Driver_Data;

/**
 * PD3  -> Ethernet_PHY_LAN8720_Reset
 * Reset:0  Normal:1
 * 初始化并控制IO口实现 LAN8720复位
 */
void Ethernet_GPIO_Init(void)
{
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef gpio_config;
	gpio_config.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_config.Pin = GPIO_PIN_3;
	gpio_config.Pull = GPIO_NOPULL;
	gpio_config.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET);
	HAL_GPIO_Init(GPIOD, &gpio_config);
	HAL_Delay(50);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET);
	HAL_Delay(50);
}

void ETH_RSTN_IO_Init(void)
{
	GPIO_InitTypeDef eth_rstn_io_config;
	__HAL_RCC_GPIOC_CLK_ENABLE();
	eth_rstn_io_config.Mode = GPIO_MODE_OUTPUT_PP;
	eth_rstn_io_config.Pin = GPIO_PIN_3;
	eth_rstn_io_config.Pull = GPIO_NOPULL;
	eth_rstn_io_config.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_RESET);	// 先写入IO口输出电平寄存器，确保GPIO初始化电平的稳定性
	HAL_GPIO_Init(GPIOC,&eth_rstn_io_config);
	HAL_Delay(2);										// 输出低电平，2ms使PHY芯片复位（规格书要求低电平持续1us）
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_SET);	
	HAL_Delay(2);										// 输出高电平，2ms使PHY芯片启动（规格书要求高电平持续3us）
}

Return_Status TCP_Driver_Init(void)
{
	// ETH_RSTN_IO_Init();
	// MX_LWIP_Init();		// 已在stm32 ETH驱动任务中执行
	if(TCP_Driver_Bind() == RETURN_ERROR)
	{
		return RETURN_ERROR;
	}
	return RETURN_SUCCESS;
}

Return_Status TCP_Driver_Bind(void)
{
	if(TCP_Driver_Data.TCP_Driver_State.Tcp_Pcb_Clear_Flag) 	// 若已为 tcp_pcb 变量分配空间
	{
		//DMA_Sprintf("TCP_Driver_Bind() tcp_pcb != NULL\r\n");
		if( ERR_OK == tcp_close(TCP_Driver_Data.TCP_Driver_Pcb) ) // 释放之前的 tcp_pcb 变量空间
		{
			TCP_Driver_Data.TCP_Driver_State.Tcp_Pcb_Clear_Flag = 0;
		}
		else
		{
			return RETURN_ERROR;
		}
	}
	
	TCP_Driver_Data.TCP_Driver_Pcb = tcp_new();
	if(NULL == TCP_Driver_Data.TCP_Driver_Pcb)
	{
		//DMA_Sprintf("Error!!!\t tcp_new() Error:return NULL\r\n");
		return RETURN_ERROR;
	}
	TCP_Driver_Data.TCP_Driver_State.Tcp_Pcb_Clear_Flag = 1;	// 若已为 tcp_pcb 变量分配空间

	if(ERR_OK != tcp_bind(TCP_Driver_Data.TCP_Driver_Pcb, &TCP_Driver_Data.Config.IP_Config.Client_IP_Addr, TCP_Driver_Data.Config.IP_Config.Client_Port))	// 绑定自身IP地址
	{
		//DMA_Sprintf("Error!!!\t tcp_bind() Error:return !ERR_OK\r\n");
		return RETURN_ERROR;
	}

	tcp_bind_netif(TCP_Driver_Data.TCP_Driver_Pcb, &TCP_Driver_Data.TCP_Driver_netif);
	return RETURN_SUCCESS;
}

Return_Status TCP_Driver_Connect(uint8_t *dest_ip, uint16 dest_port, tcp_connected_fn call_back_fun)
{
	err_t err=0;
	#if 0
	Running_Record.Record_TCP.TCP_Server_Poll_Count++;
	#endif
	IP4_ADDR(&TCP_Driver_Data.Config.IP_Config.Server_IP_Addr, dest_ip[0], dest_ip[1], dest_ip[2], dest_ip[3]);
	err = tcp_connect(TCP_Driver_Data.TCP_Driver_Pcb, &TCP_Driver_Data.Config.IP_Config.Server_IP_Addr, dest_port, call_back_fun);
	if( err != ERR_OK ) // 连接目标IP地址
	{
		//DMA_Sprintf("Error!!!\t TCP_Driver_Connect(){ tcp_connect() }\r\n");
		TCP_Error_Printf(err);
		return RETURN_ERROR;
	}
	return RETURN_SUCCESS;
}

void TCP_Send_Massage(const char *data_pointer, int data_len, uint8 now_send)
{
	if(data_len == -1)
	{
		data_len = strlen(data_pointer);
	}
	tcp_write(TCP_Driver_Data.TCP_Driver_Pcb, data_pointer, data_len, TCP_WRITE_FLAG_MORE);	// 将发送数据加载入缓冲区
	#if 0
	if(now_send)
	{
		Running_Record.Record_TCP.CTL_TCP_Send_Count++;
		tcp_output(TCP_Driver_Data.TCP_Driver_Pcb);	//立即发送缓冲区内的函数
	}
	#endif
}

void TCP_Receive_Massage(void)
{
	
}

void TCP_Error_Printf(err_t err)
{
//	uint8 err_value = (uint8)(0 - (int)err);
	//DMA_Sprintf("TCP_Error_Callback(): err = %d\r\n", err_value);
	//DMA_Sprintf_String((uint8 *)&ETH_Err_Event[err_value][0], strlen(&ETH_Err_Event[err_value][0]));
	//DMA_Sprintf_String("\r\n", strlen("\r\n"));
}

void TCP_State_Printf(uint8 state_addr)
{
	//DMA_Sprintf_String("TCP_State:",strlen("TCP_State:"));
	//DMA_Sprintf_String((uint8 *)&ETH_State[state_addr][0], strlen(&ETH_State[state_addr][0]));
	//DMA_Sprintf_String("\r\n", strlen("\r\n"));
}
