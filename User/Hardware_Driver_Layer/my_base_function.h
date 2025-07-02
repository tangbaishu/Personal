#ifndef _MY_BASE_FUNCTION_H_
#define _MY_BASE_FUNCTION_H_

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

struct Byte_Buffer_Queue_t
{	
	uint8 	*Buffer_Area;		// 缓存区首地址
	uint8	top;				// 当前队列数据顶部
	uint8	Buffer_Area_Len;	// 缓存区数据长度	
};



/**
 * 4字节数据合并成 32位类型数据（实际有效位为40位）
 * 逻辑:p_uint8[4]对应 p_uint64类型变量的0~7
 */
void Uint8_Convert_Uint32(uint8 *p_uint8, uint32 *p_uint32);


#endif
