#include "SYS_Prj.h"

uint32_t 	SYS_1ms_CNTR=0;				//sys 1ms��ʱ��
uint32_t	Idle_Cyclecount=0;
BaseType_t TaskSuspend_Record=0;//������ͣ�ڼ���ȼ�¼

int fputc(int ch,FILE *p)
{
	TaskSuspend_Record=0;
	vTaskSuspendAll();//��ͣ������
	HAL_UART_Transmit(&huart1,(uint8_t*)&ch,1,1000);
	TaskSuspend_Record = xTaskResumeAll();//�ָ������� ����ֵ��pdTRUE ��ʾ����ͣ�ڼ��и������ȼ������������
	return ch;
}


#if configUSE_IDLE_HOOK

	void vApplicationIdleHook(void)
	{
		Idle_Cyclecount++;
	//	printf("Idle_Cyclecount=%d\r\n",Idle_Cyclecount);
	//	vTaskDelay(1/portTICK_PERIOD_MS);
	}

#endif


