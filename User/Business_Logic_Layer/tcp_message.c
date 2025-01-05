#include "tcp_message.h"
#include "tcp_business.h"
#include "uart_debug_driver.h"
#include "base_function.h"
#include "string.h"

#include "wwdg_driver.h"
volatile TCP_Message_Group_t 	TCP_Message_Group;											// 与控制器进行TCP交互的 消息集
static void TCP_Header_UpData(int dataLen, int cmd, uint8 *payload);					// 基于发送包类型，进行局部修改

/**
 * @brief 
 * 基于设备数据，生成TCP 包头部分
 * 上电初始化过程中，执行该函数
 */
void TCP_Header_Init(void)
{
	TCP_Message_Group.Transfer_Header.Head = MODE_REV_INT32(TCP_MESSAGE_PACK_HEAD);

	TCP_Message_Group.Transfer_Header.Version = MODE_REV_INT32(TCP_Message_Group.TCP_Device_Info.SYS_Version);
	TCP_Message_Group.Transfer_Header.Crc = MODE_REV_INT32(0);

	TCP_Message_Group.Transfer_Header.DeviceId = MODE_REV_SHORT16(0x45);
	TCP_Message_Group.Transfer_Header.DeviceType = MODE_REV_INT32(2);
}

/**
 * @brief 
 * 
 * @param DataLen 
 * @param cmd 
 * @param payload 
 */
void TCP_Header_UpData(int dataLen, int cmd, uint8 *payload)						// 基于发送包类型，进行局部修改
{
	TCP_Message_Group.Transfer_Header.DateLen = MODE_REV_INT32(dataLen);
	TCP_Message_Group.Transfer_Header.Cmd = MODE_REV_INT32(cmd);
	TCP_Message_Group.Transfer_Header.Payload = payload;
}


/**
 * @brief 对 TCP 包头校验
 * 
 * @param tcp_header 
 * @return TCP_Return_e 
 */
TCP_Return_e TCP_Header_Check(TCP_Message_Header_t *tcp_header)						
{
	if(TCP_MESSAGE_PACK_HEAD != tcp_header->Head)
	{
		return FAIL;
	}
	return PASS;
}// end TCP_Return_e TCP_Header_Check(TCP_Message_Header_t *tcp_header)

/**
 * @brief 标签/基站首次联网后，上报注册消息包
 * 
 * @param deviceregist 
 * @return TCP_Return_e 
 */
TCP_Return_e TCP_Device_Regist(RadarBaseCfg_t *deviceregist)
{
	TCP_Header_UpData(sizeof(RadarBaseCfg_t), CMD_DEVICE_REGIST,(uint8 *)deviceregist);

	deviceregist->CraneDeviceType = MODE_REV_INT32(2);
	
	deviceregist->ID = MODE_REV_SHORT16((short)0x45);

	deviceregist->PanID = MODE_REV_INT32(0x7890);

	deviceregist->TransmitPower = MODE_REV_INT32(3);

	deviceregist->IPAddrs = MODE_REV_INT32(TCP_Business.Driver_Data->Config.IP_Config.Client_IP_Addr.addr);

	deviceregist->Port = MODE_REV_SHORT16(TCP_Business.Driver_Data->Config.IP_Config.Client_Port);

	deviceregist->ServerIPAddrs = MODE_REV_INT32(TCP_Business.Driver_Data->Config.IP_Config.Server_IP_Addr.addr);

	deviceregist->ServerPort= MODE_REV_SHORT16(TCP_Business.Driver_Data->Config.IP_Config.Server_Port);

	TCP_Send_Massage((char *)&TCP_Message_Group.Transfer_Header, TCP_MESSAGE_HEADER_LEN, 0);	// 不包含*Payload 占位符地址
	TCP_Send_Massage((char *)&TCP_Message_Group.Message_Group.DeviceRegist, sizeof(RadarBaseCfg_t), 1 );
	DMA_Sprintf("TCP_Device_Regist:\r\n");
	Uint8_Data_Printf((uint8 *)&TCP_Message_Group.Transfer_Header, TCP_MESSAGE_HEADER_LEN + sizeof(RadarBaseCfg_t));
	return PASS;
}// end TCP_Return_e TCP_Device_Regist(RadarBaseCfg_t *deviceregist)

/**
 * @brief 控制器接收到消息包后，应答消息包
 * 
 * @param deviceregist_response 
 * @return TCP_Return_e 
 */
TCP_Return_e TCP_Device_Regist_Response(DeviceRegistResponse_t *deviceregist_response)
{
	DMA_Sprintf("TCP_Device_Regist_Response()\r\n");
	deviceregist_response->RegistResult = MODE_REV_INT32(deviceregist_response->RegistResult);
	Uint8_Data_Printf((uint8 *)deviceregist_response, TCP_Message_Group.TCP_Message_Rec.Rec_Header.DateLen);
	if(deviceregist_response->RegistResult)
	{
		DMA_Sprintf("TCP_Device_Regist_Success\r\n");
	}
	else
	{
		DMA_Sprintf("TCP_Device_Regist_Fail\r\n");
		return FAIL;
	}
	TCP_Message_Group.TCP_Message_Course.Device_Register_Course = MESSAGE_ACK;
	return PASS;
}// end TCP_Return_e TCP_Device_Regist_Response(RadarBaseCfg_t *deviceregist_response)

/**
 * @brief 控制器下发的 基站/标签 配置数据
 * 
 * @param config 
 * @return TCP_Return_e 
 */
TCP_Return_e TCP_Device_Config(RadarBaseCfg_t *config)
{
	// 该部分需要确认是否需要由基站接收标签的配置信息，并通过DW1000转发给标签
	DMA_Sprintf("Warning!\t Dispose Logic Unconfirmed\r\n");

	TCP_Business.Driver_Data->Config.IP_Config.Client_IP_Addr.addr = MODE_REV_INT32(config->IPAddrs);
	TCP_Business.Driver_Data->Config.IP_Config.Client_Port = MODE_REV_SHORT16(config->Port);
	TCP_Business.Driver_Data->Config.IP_Config.Server_IP_Addr.addr = MODE_REV_INT32(config->ServerIPAddrs);
	TCP_Business.Driver_Data->Config.IP_Config.Server_Port = MODE_REV_INT32(config->ServerPort);
	TCP_Message_Pack_Printf((TCP_Message_Header_t *)&TCP_Message_Group.TCP_Message_Rec.Rec_Header,1);
	// TCP_Message_Group.Message_Group.BaseStationCfg
	return PASS;
}// end TCP_Return_e TCP_Device_Config(RadarBaseCfg_t *config)

/**
 * @brief 控制器下发的时间同步命令
 * 
 * @param TCP_TimeStamp_Sync 
 * @return TCP_Return_e 
 */
TCP_Return_e TCP_TimeStamp_Sync(TimeStampSync_t *timestamp_sync)
{
	DMA_Sprintf("TCP_TimeStamp_Sync()\r\n");
	timestamp_sync->Second = MODE_REV_INT32(timestamp_sync->Second);
	timestamp_sync->MSecond = MODE_REV_INT32(timestamp_sync->MSecond);
	DMA_Sprintf("Second = %ld\r\n", timestamp_sync->Second);
	DMA_Sprintf("MSecond = %ld\r\n", timestamp_sync->MSecond);
	HAL_Delay(1000);
	return PASS;
}// end TCP_Return_e TCP_TimeStamp_Sync(RadarBaseCfg_t *timestamp_sync)


void TCP_Message_Data_Printf(RadarBaseCfg_t *config)
{
	DMA_Sprintf("config->CraneDeviceType =0x%lx\r\n",config->CraneDeviceType);
	DMA_Sprintf("config->ID =0x%lx\r\n",config->ID);
	DMA_Sprintf("config->PanID =0x%lx\r\n",config->PanID);
	DMA_Sprintf("config->TransmitPower =0x%lx\r\n",config->TransmitPower);
	DMA_Sprintf("config->IPAddrs =0x%lx\r\n",config->IPAddrs);
	DMA_Sprintf("config->Port =0x%ld\r\n",config->Port);
	DMA_Sprintf("config->ServerIPAddrs =0x%lx\r\n",config->ServerIPAddrs);
	DMA_Sprintf("config->ServerPort =0x%ld\r\n",config->ServerPort);
	Uint8_Data_Printf((uint8 *)&config->IsMove, 40);
}

void TCP_Message_Pack_Printf(TCP_Message_Header_t *tcp_header, uint8 pack_type)
{
	DMA_Sprintf("tcp_header->Head =0x%lx\r\n",tcp_header->Head);
	DMA_Sprintf("tcp_header->DateLen =%ld\r\n",tcp_header->DateLen);
	DMA_Sprintf("tcp_header->Version =%ld\r\n",tcp_header->Version);
	DMA_Sprintf("tcp_header->Crc =%ld\r\n",tcp_header->Crc);
	DMA_Sprintf("tcp_header->Cmd =%ld\r\n",tcp_header->Cmd);
	DMA_Sprintf("tcp_header->DeviceId =0x%lx\r\n",tcp_header->DeviceId);
	DMA_Sprintf("tcp_header->DeviceType =%ld\r\n",tcp_header->DeviceType);
	// Uint8_Data_Printf(tcp_header->Payload, (uint16)tcp_header->DateLen);
	Uint8_Data_Printf((uint8 *)&TCP_Business.Driver_Data->TCP_Receive_Info.Rec_buff[TCP_MESSAGE_HEADER_LEN], (uint16)tcp_header->DateLen);
	Uint8_Data_Printf((uint8 *)&TCP_Driver_Data.TCP_Receive_Info.Rec_buff[TCP_MESSAGE_HEADER_LEN], (uint16)tcp_header->DateLen);
	if(pack_type == 1)
	{
		TCP_Message_Data_Printf((RadarBaseCfg_t *)(*tcp_header).Payload);
	}
}

void TCP_ETH_Header_Rev(TCP_Message_Header_t *tcp_header)
{
	tcp_header->Head = MODE_REV_INT32(tcp_header->Head);
	tcp_header->DateLen = MODE_REV_INT32(tcp_header->DateLen);
	tcp_header->Version = MODE_REV_INT32(tcp_header->Version);
	tcp_header->Crc = MODE_REV_INT32(tcp_header->Crc);
	tcp_header->Cmd = MODE_REV_INT32(tcp_header->Cmd);
	tcp_header->DeviceId = MODE_REV_SHORT16(tcp_header->DeviceId);
	tcp_header->DeviceType = MODE_REV_INT32(tcp_header->DeviceType);
}

void TCP_ETH_Config_Rev(RadarBaseCfg_t *config_data)
{
	config_data->CraneDeviceType = MODE_REV_INT32(config_data->CraneDeviceType);
	config_data->ID = MODE_REV_SHORT16(config_data->ID);
	config_data->PanID = MODE_REV_INT32(config_data->PanID);
	config_data->TransmitPower = MODE_REV_INT32(config_data->TransmitPower);
	config_data->IPAddrs = MODE_REV_INT32(config_data->IPAddrs);
	config_data->Port = MODE_REV_SHORT16(config_data->Port);
	config_data->ServerIPAddrs = MODE_REV_INT32(config_data->ServerIPAddrs);
	config_data->ServerPort = MODE_REV_SHORT16(config_data->ServerPort);
}
