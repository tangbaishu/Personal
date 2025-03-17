#include "SYS_Prj.h"

TaskHandle_t	Task1_handle = NULL;	
uint32_t task1_cyclecount=0;

void Task1_Function(void *params)
{
	uint8_t 	num=0;
	uint32_t 	sys_time=xTaskGetTickCount();
	printf("create_task1\r\n");
	while(1)
	{
		if(Task1_handle != NULL)
		{
			num++;
			sys_time = xTaskGetTickCount();
			vTaskDelay(10/portTICK_PERIOD_MS);
			task1_cyclecount++;
			printf("task1_time=%d ms\r\n",xTaskGetTickCount() - sys_time);
			printf("tasl1_num=%d\r\n",num);
		}
	}
}



