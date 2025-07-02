#ifndef _MY_QUEUE_H_
#define _MY_QUEUE_H_

#ifndef uint8_t
#ifndef _DECA_UINT8_
#define _DECA_UINT8_
typedef unsigned char uint8_t;
#endif
#endif

#ifndef uint16_t
#ifndef _DECA_UINT16_
#define _DECA_UINT16_
typedef unsigned char uint16_t;
#endif
#endif

#ifndef uint32_t
#ifndef _DECA_UINT32_
#define _DECA_UINT32_
typedef unsigned char uint32_t;
#endif
#endif

#ifndef uint64_t
#ifndef _DECA_UINT64_
#define _DECA_UINT64_
typedef unsigned char uint64_t;
#endif
#endif

typedef enum
{
	QUEUE_OK,
	QUEUE_FAIL,
	QUEUE_EMPTY,
	QUEUE_FULL,
}Queue_Return_e;

typedef struct
{
	uint8_t *Data_Area;
	uint8_t	Front;
	uint8_t Rear;
	uint8_t	Data_Area_Size;
}My_Round_Queue_t;	// 环形队列

Queue_Return_e My_Round_Queue_Init(My_Round_Queue_t *round_queue, uint8_t *data_area, uint8_t data_area_size);

Queue_Return_e My_Round_Enqueue(My_Round_Queue_t *round_queue, uint8_t push_data);

Queue_Return_e My_Round_Dequeue(My_Round_Queue_t *round_queue, uint8_t pull_data);

Queue_Return_e My_Round_Queue_Peek_Front(My_Round_Queue_t *round_queue, uint8_t peek_data);

Queue_Return_e My_Round_Queue_Peek_Any_Data(My_Round_Queue_t *round_queue, uint8_t peek_data_sub, uint8_t peek_data);

Queue_Return_e My_Round_Queue_Clear(My_Round_Queue_t *round_queue);

/**
 * @brief 从队列内拷贝数据出来
 * 
 * @param round_queue 	待拷贝的队列
 * @param copy_data 	需要拷贝提取数据的起始地址
 * @param copy_len 		拷贝数据长度
 */
Queue_Return_e My_Round_Queue_Copy_Data(My_Round_Queue_t *round_queue, uint8_t *copy_data, uint8_t copy_len);

/**
 * @brief 向队列粘贴数据进去
 * 
 * @param round_queue 	待粘贴数据的队列
 * @param paste_data 	待粘贴数据起始地址
 * @param paste_len 		粘贴数据长度
 */
Queue_Return_e My_Round_Queue_Paste_Data(My_Round_Queue_t *round_queue, uint8_t *paste_data, uint8_t paste_len);

#endif

