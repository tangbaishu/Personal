#include "SYS_Prj.h"

QueueHandle_t uartsend_queue = NULL;


void task_uart_send(void *params)
{
	taskENTER_CRITICAL();//进入临界断 ( ≤ configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY所定义的中断优先级全部关闭（因实现任务切换功能的PendSV中断和滴答定时器中断是最低优先级）\
	//确保在这段临界段代码期间不会被其它高优先级任务打断)
	uartsend_queue = xQueueCreate(200,sizeof(uint8_t));
	if(uartsend_queue != NULL)
	{
		printf("Create uartsend_queue message success!!!\r\n");
	}
	while(1)
	{
		
	}
}


