#include "my_queue.h"

static Queue_Return_e Round_Queue_IsFull(My_Round_Queue_t *round_queue);
static Queue_Return_e Round_Queue_IsEmpty(My_Round_Queue_t *round_queue);

void My_Queue_Example(void)
{
	#include <stdio.h>
	My_Round_Queue_t My_Round_Queue;
	uint8_t data[8]={0};
	My_Round_Queue_Init(&My_Round_Queue, data, sizeof(data));

}

Queue_Return_e My_Round_Queue_Init(My_Round_Queue_t *round_queue, uint8_t *data_area, uint8_t data_area_size)
{
	if(round_queue && data_area)
	{
		round_queue->Front = 0;
		round_queue->Rear = 0;
		round_queue->Data_Area = data_area;
		round_queue->Data_Area_Size = data_area_size;
		return QUEUE_OK;
	}
	return QUEUE_FAIL;
}

static Queue_Return_e Round_Queue_IsFull(My_Round_Queue_t *round_queue)
{
	if(round_queue)
	{
		if((round_queue->Rear+1) % round_queue->Data_Area_Size == round_queue->Front)
		{
			return QUEUE_FULL;
		}
		else
		{
			return QUEUE_OK;
		}
	}
	return QUEUE_FAIL;
}

static Queue_Return_e Round_Queue_IsEmpty(My_Round_Queue_t *round_queue)
{
	if(round_queue)
	{
		if(round_queue->Front == round_queue->Rear)
		{
			return QUEUE_EMPTY;
		}
		else
		{
			return QUEUE_OK;
		}
	}
	return QUEUE_FAIL;
}

Queue_Return_e My_Round_Enqueue(My_Round_Queue_t *round_queue, uint8_t push_data)
{
	Queue_Return_e  queue_return;
	queue_return = Round_Queue_IsFull(round_queue);
	if(QUEUE_OK != queue_return)
	{
		return queue_return;
	}
	else
	{
		round_queue->Data_Area[round_queue->Rear] = push_data;
		round_queue->Rear = (round_queue->Rear + 1) % round_queue->Data_Area_Size;
	}
	return QUEUE_OK;
}

Queue_Return_e My_Round_Dequeue(My_Round_Queue_t *round_queue, uint8_t pull_data)
{
	Queue_Return_e  queue_return;
	queue_return = Round_Queue_IsEmpty(round_queue);
	if(QUEUE_OK != queue_return)
	{
		return queue_return;
	}
	else
	{
		pull_data = round_queue->Data_Area[round_queue->Front];
		round_queue->Front = (round_queue->Front + 1) % round_queue->Data_Area_Size;;
	}
	return QUEUE_OK;
}

Queue_Return_e My_Round_Queue_Peek_Front(My_Round_Queue_t *round_queue, uint8_t peek_data)
{
	Queue_Return_e  queue_return;
	queue_return = Round_Queue_IsEmpty(round_queue);
	if(QUEUE_OK != queue_return)
	{
		return queue_return;
	}
	else
	{
		peek_data = round_queue->Data_Area[round_queue->Front];
	}
	return QUEUE_OK;
}

Queue_Return_e My_Round_Queue_Peek_Any_Data(My_Round_Queue_t *round_queue, uint8_t peek_data_sub, uint8_t peek_data)
{
	if(round_queue)
	{
		if(round_queue->Data_Area_Size < peek_data)
		{
			return QUEUE_FAIL;
		}
		peek_data = round_queue->Data_Area[peek_data_sub];
		return QUEUE_OK;
	}
	return QUEUE_FAIL;
}

Queue_Return_e My_Round_Queue_Clear(My_Round_Queue_t *round_queue)
{
	if(round_queue)
	{
		round_queue->Front = round_queue->Rear;
		return QUEUE_OK;
	}
	return QUEUE_FAIL;
}

/**
 * @brief 从队列内拷贝数据出来
 * 
 * @param round_queue 	待拷贝的队列
 * @param copy_data 	需要拷贝提取数据的起始地址
 * @param copy_len 		拷贝数据长度 (从0开始计数)
 */
Queue_Return_e My_Round_Queue_Copy_Data(My_Round_Queue_t *round_queue, uint8_t *copy_data, uint8_t copy_len)
{
	Queue_Return_e  queue_return;
	uint8_t	num = 0;
	queue_return = Round_Queue_IsEmpty(round_queue);
	if(QUEUE_OK != queue_return)
	{
		return queue_return;
	}
	else
	{
		if(round_queue->Front < round_queue->Rear)
		{
			if(((round_queue->Front + copy_len - 1) % round_queue->Data_Area_Size) > round_queue->Rear)
			{
				return QUEUE_FAIL;
			}
		}
		else
		{
			if((copy_len - 1) > (round_queue->Data_Area_Size - (round_queue->Front - round_queue->Rear)))
			{
				return QUEUE_FAIL;
			}
		}
		for(; num < copy_len; num++)
		{
			copy_data[num] = round_queue->Data_Area[round_queue->Front];
		}
		round_queue->Front = (round_queue->Front + copy_len - 1) % round_queue->Data_Area_Size;
	}
	return QUEUE_OK;
}

/**
 * @brief 向队列粘贴数据进去
 * 
 * @param round_queue 	待粘贴数据的队列
 * @param paste_data 	待粘贴数据起始地址
 * @param paste_len 	粘贴数据长度
 */
Queue_Return_e My_Round_Queue_Paste_Data(My_Round_Queue_t *round_queue, uint8_t *paste_data, uint8_t paste_len)
{
	Queue_Return_e  queue_return;
	uint8_t num = 0;
	queue_return = Round_Queue_IsFull(round_queue);
	if(QUEUE_OK != queue_return)
	{
		return queue_return;
	}
	else
	{
		if(round_queue->Rear < round_queue->Front)
		{
			if(((round_queue->Rear + paste_len - 1) % round_queue->Data_Area_Size) > round_queue->Front)
			{
				return QUEUE_FAIL;
			}
		}
		else
		{
			if((paste_len - 1) > (round_queue->Data_Area_Size - (round_queue->Rear - round_queue->Front)))
			{
				return QUEUE_FAIL;
			}
		}
		
		for(; num < paste_data; num++)
		{
			round_queue->Data_Area[round_queue->Rear] = paste_data[num];
		}
		round_queue->Rear = (round_queue->Rear + paste_len - 1) % round_queue->Data_Area_Size;
	}
	return QUEUE_OK;
}