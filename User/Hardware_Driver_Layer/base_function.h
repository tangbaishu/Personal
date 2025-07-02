#ifndef BASE_FUNCTON_H_
#define BASE_FUNCTON_H_

#define AGAIN_TYPEDEF	1	//常用类型 重定义
#if (AGAIN_TYPEDEF==1)
  #ifndef uint8
  #ifndef _DECA_UINT8_
  #define _DEAL_UINT8_
  typedef unsigned char uint8;
  #endif
  #endif

  #ifndef int8
  #ifndef _DECA_INT8_
  #define _DEAL_INT8_
  typedef signed char int8;
  #endif
  #endif

  #ifndef uint16
  #ifndef _DECA_UINT16_
  #define _DEAL_UINT16_
  typedef unsigned short uint16;
  #endif
  #endif

  #ifndef int16
  #ifndef _DECA_INT16_
  #define _DEAL_INT16_
  typedef signed short int16;
  #endif
  #endif

  #ifndef uint32
  #ifndef _DECA_UINT32_
  #define _DEAL_UINT32_
  typedef unsigned int uint32;
  #endif
  #endif

  #ifndef int32
  #ifndef _DECA_INT32_
  #define _DEAL_INT32_
  typedef signed int int32;
  #endif
  #endif

  #ifndef uint64
  #ifndef _DECA_UINT64_
  #define _DEAL_UINT64_
  typedef unsigned long long uint64;
  #endif
  #endif

  #ifndef int64
  #ifndef _DECA_UINT64_
  #define _DEAL_UINT64_
  typedef signed long long int64;
  #endif
  #endif
#endif
typedef enum
{
    RETURN_SUCCESS = 0,
    RETURN_ERROR
} Return_Status;

typedef struct
{
  uint8   *Data;            //栈内数据指针  全局变量首次初始化为 NULL 
  uint8   Top;              //栈顶数据下标
  uint8   Len;              //栈长度
}Data_Buff_Queue_t;

extern uint32 Get_100CNTR;

Return_Status Push_Stack(Data_Buff_Queue_t *p_stack,
                         uint8 *p_data, uint32 push_len);

Return_Status Data_Buff_Queue_Init(Data_Buff_Queue_t *p_stack,uint32 p_stack_len);

/**
 * 将数据压入栈中
 * void* p_stack      需要压栈的栈区地址
 * uint64* src_data   需要压栈的栈中的数据首地址
 * uint8 push_len     栈内数据类别
 */
Return_Status Push_Stack_bit64_Data(Data_Buff_Queue_t *p_stack,
                                    uint64 *src_data, uint32 push_len);

/**
 * @brief Data_Buff_Queue_t 栈清除
 * 
 * @param p_stack 
 */
void Stack_CLR(Data_Buff_Queue_t *p_stack);

// /**
//  * 将 *src_data指向的地址数据区内，地址偏移量为 src_start_adr下的数据  复制到
//  * *p_data 指向 地址偏移量为 start_adr下的地址空间内，复制长度为 copy_len
//  * uint8 *p_data        待复制的目标数据区的首地址
//  * uint16 start_adr     复制的目标数据区的地址偏移量
//  * uint16 p_data_len    复制目标区域长度
//  * uint8 *src_data      源地址的目标数据区的首地址
//  * uint8 src_start_adr  源地址的目标数据区的地址偏移量
//  * uint16 src_data_len  源地址目标区域长度
//  * uint16 copy_len      复制数据长度
//  */
// Return_Status Copy_data(uint8 *p_data, uint16 start_adr, uint16 p_data_len,
//                         uint8 *src_data, uint8 src_start_adr,
//                         uint16 src_data_len, uint16 copy_len);

/**
 * 对传入的数据长度进行打印
 * uint8* p_data    待打印数据的首地址
 * uint16 data_len  待打印数据长度
 */
void Uint8_Data_Printf(uint8 *p_data, uint16 data_len);

/**
 * 4字节数据合并成 32位类型数据（实际有效位为40位）
 * 逻辑:p_uint8[4]对应 p_uint64类型变量的0~7
 */
void Uint8_Convert_Uint32(uint8 *p_uint8, uint32 *p_uint32);

/**
 * 将4个字节合并成一个64位数据
 */
void Uint8_Convert_Uint64(uint8 *p_uint8, uint64 *p_uint64);

/**
 * 提取 64位数据前 40位并转换成4个字节
 * 64位0~7位为 &p_uint8[0]
 */
void Uint64_Convert_Uint8(uint64 p_uint64, uint8 *p_uint8);

/**
 * 对传入的字节类型数据uint8 清零
 * uint8*p_data 待清零数据首地址
 * uint8 data_len 待清零数据长度
 */
void Uint8_CLR(uint8 *p_data, uint8 data_len);

/**
 * 对传入的uint64数据类型变量 清零
 * uint64 *p_data 待清零数据首地址
 * uint8 data_len 待清零数据个数/长度（从1开始计数）
 */
void Uint64_CLR(uint64 *p_data, uint8 data_len);

void Take_IP_String_Data(const char *string, uint8 *save_p);
#endif
