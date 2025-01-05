#include "freertos_debug.h"
#include "key_driver.h"
#include "uart_debug_driver.h"
#include "basetimer_driver.h"
#include "stm32f4xx_hal.h"
#include "freertos.h"
#include "task.h"
#include "semphr.h"

void FreeRTOS_Debug_BaseTime_Init(void)
{
	extern uint32_t FreeRTOS_10usRun_Tick;				// 单位：ms
	TIM7_Base_Init();
	FreeRTOS_10usRun_Tick=0;
}

extern QueueHandle_t G_key_Press_Queue;

void FreeRTOS_Debug_Task(void *param)
{
	BaseType_t task_count = 0;
	TaskStatus_t *task_status = NULL;
	TaskStatus_t *task_runtime = NULL;
	size_t heap_size=0;
	KEY_Press_Type_e left_press_flag;
	while(1)
	{
		xQueueReceive(G_key_Press_Queue, (KEY_Press_Type_e *)&left_press_flag, portMAX_DELAY);
		if(left_press_flag == KEY_LEFT)
		{
			task_count = uxTaskGetNumberOfTasks();	//获取任务数量

			task_status = (TaskStatus_t *)pvPortMalloc(sizeof(TaskStatus_t) * task_count);
			DMA_Sprintf("/*********TaskList*********/\r\n");
			vTaskList((char *)task_status);
			DMA_Sprintf("name\tstate\tpriority\tresidue_stack\tnumber\r\n");
			DMA_Sprintf("%s", task_status);

			task_runtime = (TaskStatus_t *)pvPortMalloc(sizeof(TaskStatus_t) * task_count);
			DMA_Sprintf("/*********Task_Runtime*********/\r\n");
			vTaskGetRunTimeStats((char *)task_runtime);
			DMA_Sprintf("task_name\truning_time\tpercentage\r\n");
			DMA_Sprintf("%s", task_runtime);
			// vTaskGetInfo(task_status->pcTaskName);
			heap_size = xPortGetFreeHeapSize();
			DMA_Sprintf("heap_size=%ld\r\n",heap_size);
			vPortFree(task_status);
			vPortFree(task_runtime);
			
			vTaskDelay(1000);
		}
	}
}
