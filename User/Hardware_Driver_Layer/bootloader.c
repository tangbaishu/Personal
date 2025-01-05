#include "BootLoader.h"

typedef void (*Jump_Pointer)(void);//定义 函数指针类型

/**
 * @brief 跳转对应代码区
 * 
 * @param en_jump_app APP_CODE: 跳转到APP区 BOOT_CODE: 跳转到BOOT区
 */
void Jump_Code_District(uint32_t en_jump_app)
{
	Jump_Pointer Jump_Function;//定义 函数指针类型(Jump_Pointer) 变量(Jump_Function)
	
	__disable_irq();	//禁用全局中断
	
	//禁用SysTick中断
	SysTick->CTRL = 0;	//将SysTick CTRL位清零，停止SysTick计时器，并禁止所有中断（SysTick_CTRL_ENABLE_Msk 位用于使能计时器，SysTick_CTRL_TICKINT_Msk 位用于使能中断）
	SysTick->LOAD = 0;	//SysTick重装载值，SysTick_CTRL_ENABLE_Msk 位用于使能计时器，SysTick_CTRL_TICKINT_Msk 位用于使能中断。写0禁用
	SysTick->VAL = 0;	//SysTick当前计时值（VAL）设置为0
	
	// 关闭所有中断，清除所有中断挂起标志
	for (int i = 0; i < 8; i++)
	{
		NVIC->ICER[i]=0xFFFFFFFF;
		NVIC->ICPR[i]=0xFFFFFFFF;
	}
	__enable_irq();		//使能全局中断
	
	if(en_jump_app)
	{
		Jump_Function = (Jump_Pointer) *(__IO uint32_t*)(APP_START_ADDR + 4);//将APP地址赋予函数指针
//		Jump_Function = (Jump_Pointer) *(volatile uint32_t*)(APP_START_ADDR + 4);//将APP地址赋予函数指针(等效用法)
		
		__set_MSP(*(__IO uint32_t *)(APP_START_ADDR)); //将APP首地址赋给主栈指针	
	}
	else
	{
		Jump_Function = (Jump_Pointer) *(__IO uint32_t*)(BOOT_START_ADDR + 4);//将APP地址赋予函数指针
		
		__set_MSP(*(__IO uint32_t *)(BOOT_START_ADDR)); //将APP首地址赋给主栈指针		
	}
	Jump_Function();//通过函数指针调用函数，完成跳转
}
