#include "pd_message_decode.h"
#include "pd_message_variable.h"
#include "pd_message.h"
#include "api.h"

/**
 * @brief 发送PD消息数据，例如UVDM等
 * 
 * @param data 指向 紧跟在 message_header 后的32位数据的低8位地址
 * @param object_num (Number_of_Data_Objects) message_header 后面跟的 32位数据对象 的数量
 * @param is_extend_mesg  true/1 为拓展消息 false/0 为控制类或数据类消息 
 */
void PD_Send_Message_Data(uint8_t *data, uint8_t object_num, uint8_t is_extend_mesg)
{
	uint8_t *txbuff_start_addr = Pd_Get_Tx_Buff_Start_Address(is_extend_mesg);//txbuff_start_addr;
	uint8_t *pd_txbuff = txbuff_start_addr;
	pd_mesg_summ_t *pd_mesg_summ = Pd_Init_Tx_Message_Summary();   //get tx message summary 	
	uint8_t	transfer_32bit = 4;
	uint8_t transfer_data_len = object_num;
	
	pd_mesg_summ->packet_start = PD_SOP;
	pd_mesg_summ->mesg_class = PD_DATA_CLASS;
	pd_mesg_summ->mesg_type = PD_Vendor_Defined;

	pd_txbuff += 2;		// 跳过 Message_Header
	while(object_num)
	{
		transfer_32bit = 4;
		while(transfer_32bit)
		{
			*pd_txbuff = *data;
			data++;
			pd_txbuff++;
			transfer_32bit--;
		}
		object_num--;
	}	
	Pd_Tx_Message(transfer_data_len, txbuff_start_addr);
}

void PD_Send_Not_Suppertd(void)
{
	pd_mesg_summ_t *pd_mesg_summ = Pd_Init_Tx_Message_Summary();   //get tx message summary 
	uint8_t *txbuff_start_addr = Pd_Get_Tx_Buff_Start_Address(0);	//txbuff_start_addr;
	pd_mesg_summ->packet_start = PD_SOP;
	pd_mesg_summ->mesg_class = PD_CONTROL_CLASS;
	pd_mesg_summ->mesg_type = PD_Not_Supported;
	Pd_Tx_Message(0, txbuff_start_addr);
}

void Transition_Bytes_Bit16(uint8_t* start_addr)
{
	uint8_t ram_data = *(start_addr + 1);
	*(start_addr + 1) = *start_addr;
	*start_addr = ram_data;
}

void Transition_Bytes_Bit32(uint8_t* start_addr)
{
	uint8_t ram_data[4] = {0};
	ram_data[0] = *start_addr;
	ram_data[1] = *(start_addr + 1);
	ram_data[2] = *(start_addr + 2);
	ram_data[3] = *(start_addr + 3);
	*(start_addr + 3) = ram_data[0];
	*(start_addr + 2) = ram_data[1];
	*(start_addr + 1) = ram_data[2];
	*start_addr = ram_data[3];
}

/**
 * @brief 内存字节数据拷贝
 * 
 * @param p_dest 	拷贝数据存放的目的地址
 * @param p_source 	拷贝原始数据地址
 * @param copy_len  拷贝字节长度，单位：Byte
 */
void My_Memory_Byte_Copy(uint8_t *p_dest, const uint8_t* p_source, uint8_t copy_len)
{
	while(copy_len)
	{
		*p_dest = *p_source;
		p_source++;
		p_dest++;
		copy_len--;
	}
}