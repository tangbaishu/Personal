#include "SYS_Prj.h"

void Task1_Function(void *params)
{
	uint8_t num=0,sys_time=xTaskGetTickCount();
	while(1)
	{
		num++;
		vTaskDelay(500/portTICK_PERIOD_MS);
		printf("task2_time=%d ms\r\n",xTaskGetTickCount() - sys_time);
		printf("num=%d\r\n",num);
		sys_time = xTaskGetTickCount();
	}
}

