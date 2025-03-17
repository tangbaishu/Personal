#include "SYS_Prj.h"

void Task2_Function(void *p)
{
	uint8_t num=0;
	uint32_t sys_time=SYS_1ms_CNTR;
	printf("create_task2\r\n");
	while(1)
	{ 
		sys_time = xTaskGetTickCount();
		num++;
//		vTaskDelay(10);
//		printf("task2_vtaskdelay=%d\r\n", xTaskGetTickCount() - sys_time);
//		printf("task2_num=%d\r\n",num);
//		printf("task1_cyclecount=%d\r\n",task1_cyclecount);
//		printf("RAM_Freeheap=%d\r\n", xPortGetFreeHeapSize());
		printf("%d\r\n", xPortGetFreeHeapSize());
		if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13))
		{
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
		}
		if(num > 10)
		{
			num=0;
			if(Task1_handle != NULL)
			{
		//		vTaskDelay(10/portTICK_PERIOD_MS);
				vTaskDelete(Task1_handle);
				printf("Task1 Delete\r\n");
				Task1_handle = NULL;
			}
			else
			{
				xTaskCreate(Task1_Function,"Task1",128,NULL,osPriorityNormal,&Task1_handle);
				printf("Task1 Create \r\n");
			}
		}
		if(TaskSuspend_Record)
		{
			printf("TaskSuspend_Record=%d\r\n",(uint32_t)TaskSuspend_Record);
		}
	}
}

