#include "tcp_business.h"
#include "tcp_message.h"
#include "running_record.h"
#include "uart_debug_driver.h"
#include "tcp_priv.h"
#include "lwip.h"
#include "netif.h"
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"


TCP_Business_t TCP_Business;
volatile uint32 	TCP_Timer=0;
uint8 error_payload=0;	// 用于存放错误的head->payload

static err_t TCP_Connect_Success_Callback(void *arg, struct tcp_pcb *tpcb, err_t err);			// TCP连接成功回调函数
static err_t TCP_Poll_Callback(void *arg, struct tcp_pcb *tpcb);

static void TCP_Unusual_Dispose(uint8 unusual_dispose);		// 异常状态处理
static void TCP_Rate_Change(void);			// 改变TCP轮询速率
static void TCP_Callback_Interface(void);	// TCP回调函数接口
static Return_Status TCP_Connect_Server(uint8_t *dest_ip, uint16 dest_port, tcp_connected_fn call_back_fun);
Return_Status TCP_Message_Send_Manage(void);

void TCP_Task(void *param)
{
	TCP_Business_Pointer_Init();
	TCP_Business.Driver_Data->Config.IP_Config.Client_Port = 1000;
	TCP_Business.Driver_Data->Config.IP_Config.IP_List.Server_IP[0] = 192;
	TCP_Business.Driver_Data->Config.IP_Config.IP_List.Server_IP[1] = 168;
	TCP_Business.Driver_Data->Config.IP_Config.IP_List.Server_IP[2] = 1;
	TCP_Business.Driver_Data->Config.IP_Config.IP_List.Server_IP[3] = 1;
	TCP_Business.Driver_Data->Config.IP_Config.Server_Port = 12345;
	TCP_Business_Running();
	while(1)
	{
		TCP_Business_Logic();
		osDelay(10);
//		LED0_OVERTURN();
	}
}

void TCP_Business_Pointer_Init(void)
{
	TCP_Business.Driver_Data = &TCP_Driver_Data;
}

void TCP_Business_Running(void)
{
	// 初始化TCP(ETH)外设接口
	if (RETURN_ERROR == TCP_Driver_Init())
	{
		DMA_Sprintf("Error\t TCP_Business_Running() -> TCP_Driver_Init() RETURN_ERROR");
		Running_Record.Record_TCP.TCP_Server_Connect_State= 0;	//TCP 连接失败
		TCP_Business.TCP_Business_State = TCP_CLOSE;
		return;
	}

	// 连接设定的服务器IP和端口，并初始化TCP运行过程中的回调函数接口、Poll轮询速率
	TCP_Connect_Server(TCP_Business.Driver_Data->Config.IP_Config.IP_List.Server_IP, TCP_Business.Driver_Data->Config.IP_Config.Server_Port, TCP_Connect_Success_Callback);
	TCP_Header_Init();
}

void TCP_Business_Logic(void)
{
	// 上电 / 网线拔出
	if((ETH_PULLOUT == TCP_Business.Driver_Data->ETH_Hardware_State.ETH_Insert_State) || (0 == TCP_Business.Driver_Data->ETH_Hardware_State.ETH_Insert_State))
	{
		if((TCP_Business.Driver_Data->TCP_Driver_State.Tcp_Pcb_Clear_Flag) && (TCP_Business.Driver_Data->TCP_Driver_Pcb->state == TCP_CLOSE))
        {
            int8 ram_err = 0;
            ram_err = tcp_close(TCP_Driver_Data.TCP_Driver_Pcb);
			if( ram_err == ERR_OK)
			{
				TCP_Business.Driver_Data->TCP_Driver_State.Tcp_Pcb_Clear_Flag = 0;
				TCP_Business.TCP_Business_State = TCP_OFF;
			}
            DMA_Sprintf("tcp_close() return ram_err = %d\r\n", ram_err);
        }
		return;
	}
	else if(TCP_OFF == TCP_Business.TCP_Business_State) // 网线插入后，准备发起TCP连接请求
	{
		if(ETH_INSERT == TCP_Business.Driver_Data->ETH_Hardware_State.ETH_Insert_State)
		{
			TCP_Business.TCP_Business_State = TCP_INIT;
		}
	}

	if(TCP_CONNECT_SERVER_OK == TCP_Business.TCP_Business_State) // 通过TCP协议连接到服务器
	{
		TCP_Business.TCP_Business_State = TCP_IDLE;
	}
	else if(TCP_CLOSE == TCP_Business.TCP_Business_State)		// 由于异常状态或错误，TCP业务逻辑 决定 关闭TCP业务
	{
		int8 ram_err=0;
		ram_err = tcp_close(TCP_Business.Driver_Data->TCP_Driver_Pcb);
		DMA_Sprintf("ram_err = %d\r\n",ram_err);
		TCP_Business.TCP_Business_State = TCP_INIT;
	}

	if(TCP_IDLE == TCP_Business.TCP_Business_State) 			// TCP连接服务器成功 / TCP数据发送成功，目前处于空闲状态。可在该状态下写入待发送数据
	{
		if(RETURN_SUCCESS == TCP_Message_Send_Manage())
		{
			TCP_Timer = HAL_GetTick();
			TCP_Business.TCP_Business_State = TCP_SEND_BUSY;
		}
	}
	else if(TCP_INIT == TCP_Business.TCP_Business_State)		// TCP业务逻辑，要求初始化TCP协议
	{
		DMA_Sprintf("TCP Business: Init_TCP\r\n");
		if(	( CLOSED == TCP_STATE_GET(TCP_Business.Driver_Data->TCP_Driver_Pcb)) )
		{
			Running_Record.Record_TCP.TCP_Server_Reconnection_Count++;
			DMA_Sprintf("TCP_Driver_Bind Reset\r\n");
			if(TCP_Driver_Bind() == RETURN_ERROR)
			{
				DMA_Sprintf("Error!!!\t TCP_Unusual_Dispose() -> TCP_Driver_Bind()\r\n");
				return;
			}
			TCP_Connect_Server(TCP_Business.Driver_Data->Config.IP_Config.IP_List.Server_IP, TCP_Business.Driver_Data->Config.IP_Config.Server_Port, TCP_Connect_Success_Callback);
			TCP_Message_Group.TCP_Message_Course.Device_Register_Course = MESSAGE_UNSEND;	// 上电 / 网线 将设备注册进程复位，待后续重新发送

		}
		else if( TIME_WAIT == TCP_STATE_GET(TCP_Business.Driver_Data->TCP_Driver_Pcb) )
		{
			DMA_Sprintf("TCP_Connect_Server\r\n");
			TCP_Connect_Server(TCP_Business.Driver_Data->Config.IP_Config.IP_List.Server_IP, TCP_Business.Driver_Data->Config.IP_Config.Server_Port, TCP_Connect_Success_Callback);
			TCP_Message_Group.TCP_Message_Course.Device_Register_Course = MESSAGE_UNSEND;	// 上电 / 网线 将设备注册进程复位，待后续重新发送

		}
		else
		{
			DMA_Sprintf("Error!!!\t TCP_Unusual_Dispose() -> tcp_state no closed\r\n");
			TCP_State_Printf((uint8)TCP_STATE_GET(TCP_Business.Driver_Data->TCP_Driver_Pcb));
		}
	}
	
}

Return_Status TCP_Message_Send_Manage(void)
{
	if(TCP_Message_Group.TCP_Message_Course.Device_Register_Course == MESSAGE_UNSEND)
	{
		TCP_Message_Group.TCP_Message_Course.Device_Register_Course = MESSAGE_SENDING;
		TCP_Device_Regist((RadarBaseCfg_t *)&TCP_Message_Group.Message_Group.DeviceRegist);
		TCP_Message_Pack_Printf((TCP_Message_Header_t *)&TCP_Message_Group.Transfer_Header, 1);
		return RETURN_SUCCESS;
	}
	if((MESSAGE_FINISH == TCP_Message_Group.TCP_Message_Course.Device_Register_Course) || (MESSAGE_ACK == TCP_Message_Group.TCP_Message_Course.Device_Register_Course))	// 若消息注册未发送完成
	{

	}
	return RETURN_ERROR;	// 默认发送失败，避免回调“发送成功”的错误数据 
}

void TCP_Message_Receive_Manage(void)
{
	int16_t residue_data_len=TCP_Business.Driver_Data->TCP_Receive_Info.Rec_Data_Len;	// 剩余数据长度
	uint16_t dispose_data_len=0;														// 已处理数据长度
	memcpy((uint8 *)&TCP_Message_Group.TCP_Message_Rec.Rec_Header.Head, TCP_Business.Driver_Data->TCP_Receive_Info.Rec_buff, TCP_MESSAGE_HEADER_LEN);
	TCP_Message_Group.TCP_Message_Rec.Rec_Header.Payload = (uint8 *)&TCP_Business.Driver_Data->TCP_Receive_Info.Rec_buff[TCP_MESSAGE_HEADER_LEN];
	if(TCP_Business.Driver_Data->TCP_Receive_Info.Rec_Count != 0)
	{
		do
		{
			if(TCP_Business.Driver_Data->TCP_Receive_Info.Rec_Count != 0)
			{
				TCP_Business.Driver_Data->TCP_Receive_Info.Rec_Count--;
			}
			DMA_Sprintf("Rec_Count = %d\r\n", TCP_Business.Driver_Data->TCP_Receive_Info.Rec_Count);

			TCP_ETH_Header_Rev((TCP_Message_Header_t *)&TCP_Message_Group.TCP_Message_Rec.Rec_Header);
			TCP_Message_Pack_Printf((TCP_Message_Header_t *)&TCP_Message_Group.TCP_Message_Rec.Rec_Header, 0);
			if(TCP_Message_Group.TCP_Message_Rec.Rec_Header.Cmd != 0)
			{
				if(TCP_Message_Group.TCP_Message_Rec.Rec_Header.Cmd == CMD_DEVICE_REGIST_RESPONSE)
				{
					Running_Record.Record_TCP.TCP_Rec_Pass++;
					dispose_data_len = TCP_MESSAGE_HEADER_LEN + sizeof(DeviceRegistResponse_t);
					if(residue_data_len < dispose_data_len)
					{
						DMA_Sprintf("data_len error\r\n");
						return;
					}
					TCP_Device_Regist_Response((DeviceRegistResponse_t *)TCP_Message_Group.TCP_Message_Rec.Rec_Header.Payload);
				}
				else if((TCP_Message_Group.TCP_Message_Rec.Rec_Header.Cmd == CMD_TAGCFG) || (TCP_Message_Group.TCP_Message_Rec.Rec_Header.Cmd == CMD_ANCHOR_CFG))
				{
					Running_Record.Record_TCP.TCP_Rec_Pass++;
					dispose_data_len = TCP_MESSAGE_HEADER_LEN + sizeof(RadarBaseCfg_t);
					if(residue_data_len < dispose_data_len)
					{
						DMA_Sprintf("data_len error\r\n");
						return;
					}
					TCP_Device_Config((RadarBaseCfg_t *)TCP_Message_Group.TCP_Message_Rec.Rec_Header.Payload);
				}
				else if(TCP_Message_Group.TCP_Message_Rec.Rec_Header.Cmd == CMD_TIMESTAMP_SYNC)
				{
					Running_Record.Record_TCP.TCP_Rec_Pass++;
					dispose_data_len = TCP_MESSAGE_HEADER_LEN + sizeof(TimeStampSync_t);
					if(residue_data_len < dispose_data_len)
					{
						DMA_Sprintf("data_len error\r\n");
						return;
					}
					TCP_TimeStamp_Sync((TimeStampSync_t *)TCP_Message_Group.TCP_Message_Rec.Rec_Header.Payload);
				}
				else
				{
					break;
				}
				// memset((char *)TCP_Message_Group.TCP_Message_Rec.Data_Buff, 0, TCP_Message_Group.TCP_Message_Rec.Rec_Header->DateLen);
				// memset((char *)&TCP_Message_Group.TCP_Message_Rec.Rec_Header->Head, 0 , TCP_MESSAGE_HEADER_LEN);
			}
			else
			{
				break;
			}
			residue_data_len -=  dispose_data_len;
			if(residue_data_len > TCP_MESSAGE_HEADER_LEN)
			{
				DMA_Sprintf("continue\r\n");
				memcpy((uint8 *)&TCP_Message_Group.TCP_Message_Rec.Rec_Header.Head, (uint8 *)&TCP_Business.Driver_Data->TCP_Receive_Info.Rec_buff[dispose_data_len], TCP_MESSAGE_HEADER_LEN);
				TCP_Message_Group.TCP_Message_Rec.Rec_Header.Payload = (uint8 *)&TCP_Business.Driver_Data->TCP_Receive_Info.Rec_buff[dispose_data_len + TCP_MESSAGE_HEADER_LEN];
			}
			else
			{
				break;
			}
		}while(1);
	}
}

/**
 * @brief 连接设定的服务器IP和端口，并初始化TCP运行过程中的回调函数接口、Poll轮询速率
 * 用于 TCP业务初始化( TCP_Business_Running() ) 或 TCP连接错误，tcp_Close()后的重连机制
 * @param dest_ip 
 * @param dest_port 
 * @param call_back_fun 
 * @return Return_Status 
 */
Return_Status TCP_Connect_Server(uint8_t *dest_ip, uint16 dest_port, tcp_connected_fn call_back_fun)
{
	if(ETH_PULLOUT == TCP_Business.Driver_Data->ETH_Hardware_State.ETH_Insert_State) // 若网线拔出
	{
		TCP_Business.TCP_Business_State = TCP_OFF;
		return RETURN_ERROR;
	}
	if(TCP_Driver_Connect(dest_ip, dest_port, call_back_fun) == RETURN_ERROR)
	{
		return RETURN_ERROR;
	}
	TCP_Business.TCP_Business_State = TCP_CONNECT_UNDERWAY;
	TCP_Rate_Change(); // 初始化/改变 Poll轮询速率、 指定tcp_poll()回调函数接口 
	TCP_Callback_Interface();	// 初始化TCP运行过程中的回调函数接口
	return RETURN_SUCCESS;
}

void TCP_Anew_Connect_Server(uint8_t *dest_ip, uint16 dest_port,uint8 self_updating)
{
	uint32 tcp_abort_timer=0;
	tcp_abort(TCP_Business.Driver_Data->TCP_Driver_Pcb);	// 断开当前连接的TCP

	if(self_updating)
	{
		tcp_close(TCP_Business.Driver_Data->TCP_Driver_Pcb);
		TCP_Business.TCP_Business_State = TCP_CLOSE;
		return;
	}

	tcp_abort_timer = HAL_GetTick();
	while(1)
	{
		if(TCP_Business.Driver_Data->TCP_Driver_Pcb->state == CLOSED)
		{
			break;
		}
		if((HAL_GetTick() - tcp_abort_timer) > 1000)
		{
			DMA_Sprintf("Warning!!\t TCP_Anew_Connect_Server()->tcp_abort() fail\r\n");
			return;
		}
	}
	TCP_Connect_Server(dest_ip, dest_port, TCP_Connect_Success_Callback);
}

/**
 * @brief 
 * TCP异常处理函数
 * 禁止在函数内，执行释放 tcp_pcb 变量空间的操作，否则会导致TCP底层驱动异常。看门狗复位
 * 
 */
void TCP_Unusual_Dispose(uint8 unusual_dispose)
{
	DMA_Sprintf("TCP_Business.Driver_Data->TCP_Driver_State.Unusual_State = %d\r\n", TCP_Business.Driver_Data->TCP_Driver_State.Unusual_State);
	DMA_Sprintf("TCP_Business.Driver_Data->TCP_Driver_State.Error_State = %d\r\n", TCP_Business.Driver_Data->TCP_Driver_State.Error_State);
	if(unusual_dispose)
	{
		switch (TCP_Business.Driver_Data->TCP_Driver_State.Unusual_State)
		{
			case CLOSED: 			// 关闭的
				break;
			case LISTEN: 			// 倾听

				break;
			case SYN_SENT: 			// MCU主动发送TCP连接请求，若长时间处于该状态。意味着服务器/客户端无应答
				TCP_Rate_Change();	// 初始化/改变 Poll轮询速率
				break;
			case SYN_RCVD: 			// 接收SYN

				break;
			case ESTABLISHED:		// TCP连接成功
			
				break;
			case FIN_WAIT_1:		// 发送FIN(TCP结束帧) 1，下一步等待被关闭方确认信息
				Running_Record.Record_TCP.TCP_Server_Connect_State= 0;	//TCP 连接失败
				TCP_Business.TCP_Business_State = TCP_CLOSE;
				break;
			case FIN_WAIT_2: 		// 发送FIN(TCP结束帧) 2

				break;
			case CLOSE_WAIT:		// 结束等待、TCP连接断开
				Running_Record.Record_TCP.TCP_Server_Connect_State= 0;	//TCP 连接失败
				TCP_Business.TCP_Business_State = TCP_CLOSE;
				break;
			case CLOSING: 			// 双方几乎同时发送TCP结束请求

				break;
			case LAST_ACK: 			// 关闭接收放的主动TCP接收报文后，回应ACK后，并同时发送FIN结束报文，等待主动方发送回应ACK
				
				break;
			case TIME_WAIT: 		// TIME_WAIT状态发生在主动关闭方发送最后一个ACK响应被动关闭方的FIN报文后。
									// 此时，主动关闭方等待足够的时间以确保被动关闭方接收到了最终的ACK
				Running_Record.Record_TCP.TCP_Server_Connect_State= 0;	//TCP 连接失败
				TCP_Business.TCP_Business_State = TCP_CLOSE;
				break;
			default:
				break;
		}
	}
	switch (TCP_Business.Driver_Data->TCP_Driver_State.Error_State)
	{
		case ERR_OK:
			break;
		case ERR_MEM:
			break;
		case ERR_BUF:
			break;
		case ERR_TIMEOUT:
			break;
		case ERR_RTE: 
			break;
		case ERR_INPROGRESS: 
			break;
		case ERR_VAL: 
			break;
		case ERR_WOULDBLOCK:
			break;
		case ERR_USE:
			break;
		case ERR_ALREADY: 
			break;
		case ERR_ISCONN:
			break;
		case ERR_CONN: 
			break;
		case ERR_IF:
			break;
		case ERR_ABRT:
			Running_Record.Record_TCP.TCP_Server_Connect_State= 0;	//TCP 连接失败
			TCP_Business.TCP_Business_State = TCP_CLOSE;
			break;
		case ERR_RST: 
			Running_Record.Record_TCP.TCP_Server_Connect_State= 0;	//TCP 连接失败
			TCP_Business.TCP_Business_State = TCP_CLOSE;
			break;
		case ERR_CLSD:
			break;     
		case ERR_ARG:
			break;
		default:
			break;
	}
	TCP_Business.Driver_Data->TCP_Driver_State.Unusual_State = 0;
	TCP_Business.Driver_Data->TCP_Driver_State.Error_State = 0;
}

/**
 * @brief TCP错误回调函数 (错误事件集: ETH_Err_Event[] )
 * 
 * @param arg 
 * @param err 
 */
void  TCP_Error_Callback(void *arg, err_t err)
{
	Running_Record.Record_TCP.TCP_Server_Connect_Error_Count++;
	TCP_Business.Driver_Data->TCP_Driver_State.Error_State = err;
	DMA_Sprintf("TCP_Business_State = %d\r\n",TCP_Business.TCP_Business_State);
	TCP_Unusual_Dispose(0);
	TCP_Error_Printf(err);
}// void  TCP_Error_Callback(void *arg, err_t err)

/**
 * @brief TCP连接成功回调函数，若连接失败调用TCP_Error_Callback()函数。
 * 在TCP连接过程中，当发送 SYN后，会基于tcp_poll()函数设定的 Poll周期数（500ms每周期）
 * 当达到设定周期后，会进入TCP_Poll_Callback()函数。
 * @param arg 
 * @param tpcb 
 * @param err 
 * @return err_t 
 */
err_t TCP_Connect_Success_Callback(void *arg, struct tcp_pcb *tpcb, err_t err)
{
	Running_Record.Record_TCP.TCP_Server_Poll_Success_Count++;
	Running_Record.Record_TCP.TCP_Server_Connect_State = 1;	//TCP 连接成功

	DMA_Sprintf("TCP_Connect_Success_Callback(): \r\n");
	if(TCP_Business.TCP_Business_State == TCP_CONNECT_UNDERWAY)
	{
		TCP_Business.TCP_Business_State = TCP_CONNECT_SERVER_OK;
	}
	return 0;
}// err_t TCP_Connect_Success_Callback(void *arg, struct tcp_pcb *tpcb, err_t err)

/**
 * @brief TCP数据发送完成后的回调函数
 * @param arg 
 * @param tpcb 
 * @param len 	// 发送数据成功长度
 * @return err_t 
 */
err_t TCP_Send_Finish_Callback(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
	Running_Record.Record_TCP.TCP_Send_Finish_Count++;
	DMA_Sprintf("TCP_Timer = %d ms\r\n", HAL_GetTick() - TCP_Timer);
	if(TCP_Message_Group.TCP_Message_Course.Device_Register_Course == MESSAGE_SENDING)
	{
		TCP_Message_Group.TCP_Message_Course.Device_Register_Course = MESSAGE_FINISH;
	}

	if(TCP_Business.TCP_Business_State == TCP_SEND_BUSY)
	{
		TCP_Business.TCP_Business_State = TCP_IDLE;
	}
	pbuf_free(tpcb->unacked->p);
	return 0;
}// err_t TCP_Send_Finish_Callback(void *arg, struct tcp_pcb *tpcb, u16_t len)

/**
 * @brief TCP接收数据后的回调函数
 * @param arg 
 * @param tpcb 
 * @param p 
 * @param err 
 * @return err_t 
 */
err_t TCP_Receive_Callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
	uint16 copy_len=0;
	struct pbuf *ram_p = p;
	Running_Record.Record_TCP.TCP_Rec_Count++;
	DMA_Sprintf("TCP_Receive_Callback() len = %d\r\n", p->tot_len);
	if(tpcb == NULL)
	{
		return RETURN_ERROR;
	}
	if(p->tot_len > ETH_RX_BUF_SIZE)
	{
		DMA_Sprintf("TCP_Rec_Data_Len OverFlow\r\n");
		return 0;
	}
	Uint8_Data_Printf(p->payload,p->len);		//打印接收到的 数据原文字符
	if(p->len > TCP_MESSAGE_HEADER_LEN)
	{
		// memcpy((uint8_t *)&TCP_Message_Group.TCP_Message_Rec.Rec_Header->Head, (uint8_t *)(*p).payload, TCP_MESSAGE_HEADER_LEN);	// 拷贝TCP消息头部段
		while(1)
		{
			copy_len += ram_p->len;
			DMA_Sprintf("ram_p->len=%d\r\n", copy_len);
			if(ram_p->next == NULL)
			{
				// DMA_Sprintf("p->next=NULL\r\n");
				memcpy(TCP_Business.Driver_Data->TCP_Receive_Info.Rec_buff,(uint8 *)p->payload, ram_p->len);
				DMA_Sprintf("copy_len = %ld\r\n", copy_len);
				TCP_Business.Driver_Data->TCP_Receive_Info.Rec_Data_Len = copy_len;
				DMA_Sprintf("data_len = %ld\r\n", copy_len - TCP_MESSAGE_HEADER_LEN);
				Uint8_Data_Printf((uint8 *)&TCP_Business.Driver_Data->TCP_Receive_Info.Rec_buff[TCP_MESSAGE_HEADER_LEN], copy_len - TCP_MESSAGE_HEADER_LEN);
				break;
			}
			ram_p = ram_p->next;
		}
		TCP_Business.Driver_Data->TCP_Receive_Info.Rec_Count++;	// 接收TCP消息次数 ++
	}
	TCP_Message_Receive_Manage();
	pbuf_free(p);	// 释放当前接收的消息数据
	return 0;
}// err_t TCP_Receive_Callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)

/**
 * @brief 通过 TCP_STATE_GET(tpcb) 获取TCP连接状态
 * 该函数在正常发送和接收过程中，不会触发。
 * 在发送SYN时，若长时间无应答，会触发该函数
 * @param arg 
 * @param tpcb 
 * @return err_t 
 */
err_t TCP_Poll_Callback(void *arg, struct tcp_pcb *tpcb)
{
	DMA_Sprintf("TCP_Poll_Callback():\r\n");
	
	DMA_Sprintf("TCP_Business_State = %d\r\n",TCP_Business.TCP_Business_State);
	TCP_State_Printf((uint8)tcp_dbg_get_tcp_state(tpcb));
	if(TCP_Business.Driver_Data->TCP_Driver_State.Previous_state == 0)
	{
		TCP_Business.Driver_Data->TCP_Driver_State.Previous_state = TCP_STATE_GET(tpcb);
		TCP_Business.Driver_Data->TCP_Driver_State.State_Continue_Time = 0;
		TCP_Business.Driver_Data->TCP_Driver_State.State_Continue_Count = 0;
	}
	else
	{
		if(TCP_Business.Driver_Data->TCP_Driver_State.Previous_state == TCP_STATE_GET(tpcb))
		{
			TCP_Business.Driver_Data->TCP_Driver_State.State_Continue_Time += TCP_POLL_CALLBACK_TIME;
			TCP_Business.Driver_Data->TCP_Driver_State.State_Continue_Count ++;
		}
		else
		{
			TCP_Business.Driver_Data->TCP_Driver_State.Previous_state = TCP_STATE_GET(tpcb);
			TCP_Business.Driver_Data->TCP_Driver_State.State_Continue_Time = 0;
			TCP_Business.Driver_Data->TCP_Driver_State.State_Continue_Count = 0;
		}
		
	}
	if(TCP_Business.Driver_Data->TCP_Driver_State.State_Continue_Count >= TCP_STATE_CONTINUE_COUNT)
	{
		TCP_Business.Driver_Data->TCP_Driver_State.State_Continue_Count = 0;
		TCP_Business.Driver_Data->TCP_Driver_State.State_Continue_Time = 0;
		TCP_Business.Driver_Data->TCP_Driver_State.Unusual_State = TCP_STATE_GET(tpcb);
		TCP_Unusual_Dispose(1);
	}
	return 0;
}

void TCP_Rate_Change(void)
{
	if(TCP_Business.Connect_Rate.Real_Rate == 0)
	{
		TCP_Business.Connect_Rate.Real_Rate = TCP_FAST_CONNECT;
		tcp_poll(TCP_Business.Driver_Data->TCP_Driver_Pcb, TCP_Poll_Callback, TCP_Business.Connect_Rate.Real_Rate);
		return ;
	}
	TCP_Business.Connect_Rate.Real_Rate += TCP_Business.Connect_Rate.Rate_Add_Mode;
	if(FAST_ADD == TCP_Business.Connect_Rate.Rate_Add_Mode)
	{
		if(TCP_Business.Connect_Rate.Real_Rate > 10)
		{
			TCP_Business.Connect_Rate.Real_Rate = 10; //对应时间5s  =10*500ms
			TCP_Business.TCP_Business_State = TCP_CLOSE;
		}
	}
	else if(NORMAL_ADD == TCP_Business.Connect_Rate.Rate_Add_Mode)
	{
		if(TCP_Business.Connect_Rate.Real_Rate > 30)
		{
			TCP_Business.Connect_Rate.Real_Rate = 30; //对应时间15s  =30*500ms
		}
	}
	else if(SLOW_ADD == TCP_Business.Connect_Rate.Rate_Add_Mode)
	{
		if(TCP_Business.Connect_Rate.Real_Rate > 120)
		{
			TCP_Business.Connect_Rate.Real_Rate = 120; //对应时间60s  =120*500ms
		}
	}
	tcp_poll(TCP_Business.Driver_Data->TCP_Driver_Pcb, TCP_Poll_Callback, TCP_Business.Connect_Rate.Real_Rate);		
}

void TCP_Callback_Interface(void)
{
	
	// 	tcp_poll() 已在 TCP_Rate_Change() 函数内，完成 函数指针赋值
	tcp_sent(TCP_Business.Driver_Data->TCP_Driver_Pcb, TCP_Send_Finish_Callback);				// 指定发送成功后的回调函数
	tcp_recv(TCP_Business.Driver_Data->TCP_Driver_Pcb, TCP_Receive_Callback);					// 指定接收到数据后的回调函数
	tcp_err(TCP_Business.Driver_Data->TCP_Driver_Pcb,  TCP_Error_Callback);					// TCP错误回调函数 (错误事件集: ETH_Err_Event[] )
}
