#include "running_record.h"
#include "sys_uart_driver.h"
#include "wwdg_driver.h"
#include <string.h>



#define STRING_LEN	40
const char Runing_Record_Name[][STRING_LEN]={\
"DW1000_IRQ_Trigger_Count",\
"CTL_Tx_Count",\
"Tx_Finish_Count",\
"Tx_Timeout_Count",\
"CTL_Rx_Count",\
"Rx_Good_Frame_Count",\
"Rx_Pass_Frame_Count",\
"Rx_Timeout_Count",\
"Rx_Fail_Count",\
"DW1000_Reset_Count",\
"Send_Poll_NoEvent_Count",\
"Send_Poll_RTC_Count",\
"Send_Poll_TimeSYNC_Count",\
"Rec_Resp_Ranging_Count",\
"Rec_Resp_RTC_Count",\
"Send_Final_Ranging_Count",\
"Send_Final_RTC_Count",\
"Rec_Poll_NoEvent_Count",\
"Rec_Poll_RTC_Count",\
"Rec_Poll_TimeSYNC_Count",\
"Send_Resp_Ranging_Count",\
"Send_Resp_RTC_Count",\
"Rec_Final_Ranging_Count",\
"Rec_Final_RTC_Count",\
"ETH_Link_State",\
"TCP_Server_Connect_State",\
"TCP_Server_Poll_Count",\
"TCP_Server_Poll_Success_Count",\
"TCP_Server_Connect_Error_Count",\
"TCP_Server_Reconnection_Count",\
"CTL_TCP_Send_Count",\
"TCP_Send_Finish_Count",\
"TCP_Rec_Count",\
"TCP_Rec_Pass",
};

Runing_Record_t Running_Record;
Ranging_Time_t	Ranging_Time;

void Running_Record_Read(void)
{
	uint32 *record_pointer = &Running_Record.Dw1000_Driver.DW1000_IRQ_Trigger_Count;
	uint8 ram_addr=0;
	for(uint16 num=0; num < sizeof(Runing_Record_t)/sizeof(uint32); num++)
	{
		WDG_CLEAR();
		for(ram_addr=0; ram_addr < STRING_LEN; ram_addr++)
		{
			if(Runing_Record_Name[num][ram_addr] == '\0')
			{
				break;
			}
		}
		DMA_Sprintf_String((const uint8*)&Runing_Record_Name[num][0], ram_addr);
		DMA_Sprintf("=%ld\r\n", record_pointer[num]);
	}
	DMA_Sprintf("DW1000_Reset_Count = %ld\r\n", Running_Record.Dw1000_Driver.DW1000_Reset_Count);
}


//void Ranging_Module_Record(Ranging_Business_t ranging_record_data)
//{
//	uint8 record_buff[50];

//	ranging_record_data.Ranging_Config.Anchor_Init_Addr
//}
