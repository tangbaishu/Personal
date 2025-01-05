#include "base_function.h"
#include "uart_debug_driver.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lwip/mem.h"

uint32 Get_100CNTR = 0;

/**
 * Data_Buff_Stack初始化
 * Data_Buff_Queue_t** p_stack  Data_Buff_Queue_t*类型指针
 * uint32 p_stack_len  Data_Buff_Queue_t->stack_data 长度
 */
Return_Status Data_Buff_Queue_Init(Data_Buff_Queue_t *p_stack, uint32 p_stack_len)
{
    // 测距数据缓存区初始化
    p_stack->Data = NULL;
    p_stack->Data = malloc(p_stack_len);
    if (p_stack->Data == NULL)
    {
        DMA_Sprintf("Error!!!\t Data_Buff_Queue_Init Error:malloc return NULL\r\n");
        return RETURN_ERROR;
    }
    memset(p_stack->Data, 0, p_stack_len);  // 队列元素清零
    (*p_stack).Top = 0;
    (*p_stack).Len = p_stack_len;
    return RETURN_SUCCESS;
} // end void Data_Buff_Queue_t(void)

Return_Status Push_Stack(Data_Buff_Queue_t *p_stack,
                         uint8 *p_data, uint32 push_len)
{
    uint8 num = 0;
    // 判断栈内剩余空间是否满足带入栈数据长度
    if (push_len > (p_stack->Len - p_stack->Top))
    {
        return RETURN_ERROR;
    }
    for (; num < push_len; num++)
    {
        p_stack->Data[p_stack->Top] = p_data[num];
    }
    return RETURN_SUCCESS;
}

/**
 * 将数据压入栈中
 * void* p_stack      需要压栈的栈区地址
 * uint64* src_data   需要压栈的栈中的数据首地址
 * uint8 push_len     栈内数据类别
 */
Return_Status bit64_Data_Push_Stack(Data_Buff_Queue_t *p_stack,
                                    uint64 *src_data, uint32 push_len)
{
    uint8 num = 0;
    // 判断栈内剩余空间是否满足带入栈数据长度
    if (push_len > (p_stack->Len - p_stack->Top))
    {
        return RETURN_ERROR;
    }
    // 将数据依次压入栈中
    for (; num <= push_len; num++)
    {
        p_stack->Data[++p_stack->Top] =
            (uint8)(src_data[num / 8] >> (num * 8));
    }
    return RETURN_SUCCESS;
} // end void bit64_Data_Push_Stack(uint8 *p_stack, uint8 *p_data, uint8 data_len)

/**
 * @brief Buf_Queue_t 栈清除
 *
 * @param p_stack
 */
void Stack_CLR(Data_Buff_Queue_t *p_stack)
{
    p_stack->Top = 0;
}

// /**
//  * *src_data指向的地址数据区内，地址偏移量为 src_start_adr下的数据  复制??
//  * *p_data 指向 地址偏移量为 start_adr下的地址空间内，复制长度?? copy_len
//  * uint8 *p_data        待???制的目标数??区的首地址
//  * uint16 start_adr     复制的目标数??区的地址偏移??
//  * uint16 p_data_len    复制??标区域溢出长??
//  * uint8 *src_data      源地址的目标数??区的首地址
//  * uint8 src_start_adr  源地址的目标数??区的地址偏移量
//  * uint16 src_data_len  源地址目标区域溢出长度
//  * uint16 copy_len      复制数据长度
//  */
// Return_Status Copy_data(uint8 *p_data, uint16 start_adr, uint16 p_data_len,
//                         uint8 *src_data, uint8 src_start_adr,
//                         uint16 src_data_len, uint16 copy_len)
// {
//     // 判断待Copy数据和原??数据长度 ???? 在copy长度??
//     if (((p_data_len - start_adr) < copy_len) ||
//         ((src_data_len - src_start_adr) < copy_len))
//     {
//         printf("Copy_data() Error!!!");
//         return RETURN_ERROR;
//     }
//     for (uint16 num = 0; num < copy_len; num++)
//     {
//         p_data[start_adr++] = src_data[src_start_adr++];
//     }
//     return RETURN_SUCCESS;
// } //Return_Status Copy_data( uint8* p_data, uint16 start_adr, uint16 p_data_len,\
//                         uint8* src_data, uint16 src_data_len, uint16 copy_len);

/**
 * 对传入的数据进???打??
 * uint8* p_data    待打印数??的???地址
 * uint16 data_len  待打印数??长度
 */
void Uint8_Data_Printf(uint8 *p_data, uint16 data_len)
{
    for (int i = 0; i < data_len; i++)
    {
        DMA_Sprintf("0x%x ", p_data[i]);
        // DMA_Sprintf("%c ", p_data[i]);
    }
    DMA_Sprintf("\r\n");
}

/**
 * 4字节数据合并成 32位类型数据（实际有效位为40位）
 * 逻辑:p_uint8[4]对应 p_uint64类型变量的0~7
 */
void Uint8_Convert_Uint32(uint8 *p_uint8, uint32 *p_uint32)
{
    int8 num = 3;
    *p_uint32 = 0;
    for (; num >= 0; num--)
    {
        *p_uint32 = (*p_uint32 << 8) + p_uint8[num];
    }
} // end void Uint8_Convert_Uint64(uint8 *p_uint8, uint64 *p_uint64)

/**
 * 5字节数据合并成 64位类型数据（实际有效位为40位）
 * 逻辑:p_uint8[5]对应 p_uint64类型变量的0~7
 */
void Uint8_Convert_Uint64(uint8 *p_uint8, uint64 *p_uint64)
{
    int8 num = 4;
    *p_uint64 = 0;
    for (; num >= 0; num--)
    {
        *p_uint64 = (*p_uint64 << 8) + p_uint8[num];
    }
} // end void Uint8_Convert_Uint64(uint8 *p_uint8, uint64 *p_uint64)

/**
 * 提取 64位类型数据前40位 拆解为 5字节数据
 * p_uint64类型变量的0~7位对应 p_uint8[0]
 */
void Uint64_Convert_Uint8(uint64 p_uint64, uint8 *p_uint8)
{
    int8 num = 0;
    for (; num <= 4; num++)
    {
        p_uint8[num] = p_uint64 >> (num * 8);
    }
} // end void Uint64_Convert_Uint8(uint64 *p_uint64, uint8 *p_uint8)

/**
 * 对传入的字节类型数据uint8 清零
 * uint8*p_data 待清零数据首地址
 * uint8 data_len 待清零数据长度（从1开始记数）
 */
void Uint8_CLR(uint8 *p_data, uint8 data_len)
{
    for (; data_len > 0; data_len--)
    {
        *p_data++ = 0;
    }
}

/**
 * 对传入的uint64数据类型变量 清零
 * uint64 *p_data 待清零数据首地址
 * uint8 data_len 待清零数据长度（从1开始记数）
 */
void Uint64_CLR(uint64 *p_data, uint8 data_len)
{
    for (; data_len > 0; data_len--)
    {
        *p_data++ = 0;
    }
}

void Take_IP_String_Data(const char *string, uint8 *save_p)
{
    int ram_data[4] = {0};
    uint8 data_offset = 0, num = 0;
    sscanf(string, "%d", &ram_data[data_offset]);
    save_p[data_offset] = (uint8)ram_data[data_offset];
    data_offset++;

    for (; num < strlen(string); num++)
    {
        if ('.' == string[num])
        {
            sscanf(&string[num], ".%d", &ram_data[data_offset]);
            save_p[data_offset] = (uint8)ram_data[data_offset];
            data_offset++;
        }
    }
}
