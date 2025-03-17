#include "SYS_Prj.h"

void task2_function(void *params)
{
	uint8_t num=0;
//	uint8_t sys_time=xTaskGetTickCount();
	while(1)
	{ 
//		printf("task2_time=%d ms\r\n",xTaskGetTickCount() - sys_time);
//		sys_time = xTaskGetTickCount();
		num++;
		printf("num=%d\r\n",num);
	}
}

