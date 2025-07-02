#include "my_base_function.h"


/**
 * @brief 
 * 4字节数据合并成 32位类型数据
 * 逻辑:p_uint8[4]Byte对应 p_uint64类型变量的0~7bit
 * @param p_uint8 
 * @param p_uint32 
 */
void Uint8_Convert_Uint32(uint8 *p_uint8, uint32 *p_uint32)
{
    int8 num = 3;
    *p_uint32 = 0;
    for (; num >= 0; num--)
    {
        *p_uint32 = (*p_uint32 << 8) + p_uint8[num];
    }
} // end void Uint8_Convert_Uint32(uint8 *p_uint8, uint32 *p_uint32)

/**
 * 5字节数据合并成 64位类型数据
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

