/****************************************************************************
 * @copyright Copyright(C) 2024-2024 Ismartware Limited. All rights reserved.
 * @file iic_slave_global_variables_policy.c
 * @brief main_policy
 * @author SZ team
 ****************************************************************************/
#include "iic_slave_global_variables_policy.h"

//策略全局变量
policy_data_t policyGlobalData;


//获取与时间标志相差的时间
u32 My_GetSystemTimePass(u32 tm)
{
	u32 otm = (TIMEMARKMASK & policyGlobalData.systemTimeCounter);
	if(otm > tm)
		return (otm-tm);
	else
		return (TIMEMARKMASK&((TIMEMARKMASK & ~tm) + 1 + otm));
}

//获取当前时间标志
u32 My_GetSystemTimeMark(void)
{
	return (TIMEMARKMASK & policyGlobalData.systemTimeCounter);
}


//初始化队列
void Init_SqQueue(SqQueue *AddrFifo)
{
    AddrFifo->front = AddrFifo->rear = 0;
}

//判断队列是否为空
bool SqQueue_IsEmpty(SqQueue *AddrFifo)
{
    return AddrFifo->front == AddrFifo->rear;
}

//队列元素入队
void Enter_SqQueue(SqQueue *AddrFifo, u8 regAddr)
{
    if((AddrFifo->rear + 1) % FIFO_MAXSIZE == AddrFifo->front)
		{
			//printf("队列已满\n");
			return;
    }
    AddrFifo->data[AddrFifo->rear] = regAddr;               		 //在队尾指向的地址赋值
    AddrFifo->rear = (AddrFifo->rear + 1) % FIFO_MAXSIZE;  //队尾指针进1
}

//队列元素出队
void Depart_SqQueue(SqQueue *AddrFifo, u8 *regAddr)
{
    if(SqQueue_IsEmpty(AddrFifo))
		{
			//printf("队列中无元素\n");
			return;
    }
    *regAddr = AddrFifo->data[AddrFifo->front];             		 //取出队首指针指向的地址元素
    AddrFifo->front = (AddrFifo->front + 1) % FIFO_MAXSIZE;//队首指针进1
}




//Anker初始化队列
void Anker_Init_SqQueue(AnkerSqQueue *AddrFifo)
{
    AddrFifo->front = AddrFifo->rear = 0;
		AddrFifo->length = 0;
}

//Anker判断队列是否为空
bool Anker_SqQueue_IsEmpty(AnkerSqQueue *AddrFifo)
{
    return AddrFifo->front == AddrFifo->rear;
}

//Anker队列元素入队
void Anker_Enter_SqQueue(AnkerSqQueue *AddrFifo, u8 regAddr)
{
    if((AddrFifo->rear + 1) % FIFO_MAXSIZE == AddrFifo->front)
		{
			//printf("队列已满\n");
			return;
    }
		AddrFifo->length ++;
    AddrFifo->data[AddrFifo->rear] = regAddr;               		 //在队尾指向的地址赋值
    AddrFifo->rear = (AddrFifo->rear + 1) % FIFO_MAXSIZE;  //队尾指针进1
}

//Anker队列元素出队
void Anker_Depart_SqQueue(AnkerSqQueue *AddrFifo, u8 *regAddr)
{
    if(Anker_SqQueue_IsEmpty(AddrFifo))
		{
			//printf("队列中无元素\n");
			return;
    }
		AddrFifo->length --;
    *regAddr = AddrFifo->data[AddrFifo->front];             		 //取出队首指针指向的地址元素
    AddrFifo->front = (AddrFifo->front + 1) % FIFO_MAXSIZE;//队首指针进1
}

//Anker队列元素数量
uint8_t Anker_SqQueue_Length(AnkerSqQueue *AddrFifo)
{
	return AddrFifo->length;
}




//获取寄存器地址值
u8 Get_RegAddr_Data(u8 regAddr)
{
	return policyGlobalData.regDataArray[regAddr];
}

//设置寄存器的值，不填充FiFO
void Set_RegAddr_Data(u8 regAddr,u8 regData)
{
	policyGlobalData.regDataArray[regAddr] = regData;
}


