#include "SYS_Prj.h"

uint32_t 	SYS_1ms_CNTR=0;				//sys 1ms计时器
uint32_t	Idle_Cyclecount=0;
BaseType_t TaskSuspend_Record=0;//任务暂停期间调度记录

int fputc(int ch,FILE *p)
{
	TaskSuspend_Record=0;
	vTaskSuspendAll();//暂停调度器
	HAL_UART_Transmit(&huart1,(uint8_t*)&ch,1,1000);
	TaskSuspend_Record = xTaskResumeAll();//恢复调度器 返回值：pdTRUE 表示在暂停期间有更高优先级的任务就绪了
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


