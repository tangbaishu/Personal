#include "json_dispose.h"
#include "uart_debug_driver.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"

void JSON_Task(void)
{
	cJSON *Json_data = NULL;
	char *print_str = NULL;
	Json_data = cJSON_CreateObject();
	if(Json_data == NULL)
	{
		vTaskDelete(NULL);
		return;
	}
//				vTaskSuspendAll();
			DMA_Sprintf("cJSON_AddStringToObject()\r\n");
//			xTaskResumeAll();
//			cJSON_AddNullToObject(Json_data, (char *)"Json_data");
			cJSON_AddStringToObject(Json_data, (char *)"sbdsdgas", (char *)"zengyan");
	while (1)
	{
		// if(Json_data->next == NULL)
		// {

		// }
		// else
		// {
		// 	vTaskSuspendAll();
		// 	DMA_Sprintf("Json_data_Init->next != NULL\r\n");
		// 	xTaskResumeAll();
		// }
//		vTaskSuspendAll();
		print_str = cJSON_Print(Json_data);
		
		printf("%s\r\n", print_str);
		HAL_Delay(100);
//		xTaskResumeAll();
		// if(Json_data->next != NULL)
		// {
//			vTaskSuspendAll();
			DMA_Sprintf("Json_data->type = %ld\r\n", Json_data->type);
			DMA_Sprintf("Json_data->valuedouble = %lld\r\n", Json_data->valuedouble);
			DMA_Sprintf("son_data->valueint = %lld\r\n", Json_data->valueint);
			HAL_Delay(100);
			printf("%s\r\n",Json_data->string);
			HAL_Delay(100);
			printf("%s\r\n",(*Json_data).valuestring);
			HAL_Delay(100);
			// DMA_Sprintf_String((uint8 *)Json_data->string, strlen(Json_data->string));
			// DMA_Sprintf_String((uint8 *)Json_data->valuestring, strlen(Json_data->valuestring));
			// DMA_Sprintf(&);
//			xTaskResumeAll();
//			vTaskDelay(1000);
		// }
		// else
		// {
		// 	DMA_Sprintf("Json_data->next = NULL\r\n");
		// }
	}
	
}

void JSON_Info(void)
{
//	char json_data[]={"name:tanglibo\r\n"};
}


