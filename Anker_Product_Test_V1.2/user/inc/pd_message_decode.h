#ifndef _PD_MESSAGE_DECODE_H_
#define _PD_MESSAGE_DECODE_H_

#include "system.h"

/**
 * @brief 发送PD消息数据，例如UVDM等
 * 
 * @param data 指向 紧跟在 message_header 后的32位数据的低8位地址
 * @param object_num (Number_of_Data_Objects) message_header 后面跟的 32位数据对象 的数量
 * @param is_extend_mesg  true/1 为拓展消息 false/0 为控制类或数据类消息 
 */
void PD_Send_Message_Data(uint8_t *data, uint8_t object_num, uint8_t is_extend_mesg);

void PD_Send_Not_Suppertd(void);

void Transition_Bytes_Bit16(uint8_t* start_addr);

void Transition_Bytes_Bit32(uint8_t* start_addr);

/**
 * @brief 内存字节数据拷贝
 * 
 * @param p_dest 	拷贝数据存放的目的地址
 * @param p_source 	拷贝原始数据地址
 * @param copy_len  拷贝字节长度，单位：Byte
 */
void My_Memory_Byte_Copy(uint8_t *p_dest, const uint8_t* p_source, uint8_t copy_len);

#endif

