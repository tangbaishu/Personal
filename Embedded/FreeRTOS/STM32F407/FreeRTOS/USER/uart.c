#include "SYS_Prj.h"

QueueHandle_t uartsend_queue = NULL;


void task_uart_send(void *params)
{
	taskENTER_CRITICAL();//�����ٽ�� ( �� configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY��������ж����ȼ�ȫ���رգ���ʵ�������л����ܵ�PendSV�жϺ͵δ�ʱ���ж���������ȼ���\
	//ȷ��������ٽ�δ����ڼ䲻�ᱻ���������ȼ�������)
	uartsend_queue = xQueueCreate(200,sizeof(uint8_t));
	if(uartsend_queue != NULL)
	{
		printf("Create uartsend_queue message success!!!\r\n");
	}
	while(1)
	{
		
	}
}


