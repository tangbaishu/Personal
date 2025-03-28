/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：beep.h
 * 摘要：
 *  
 * 修改历史     版本号        Author       修改内容
 *--------------------------------------------------
 * 2023.9.08      v01         百问科技      创建文件
 *--------------------------------------------------
*/

#ifndef BEEP_H_
#define BEEP_H_

/**********************************************************************
 * 函数名称： buzzer_init
 * 功能描述： 初始化蜂鸣器并创建定时器(用于停止声音)
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 * 修改日期：      版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2023/09/08	     V1.0	  韦东山	      创建
 ***********************************************************************/
int buzzer_init(void);

/**********************************************************************
 * 函数名称： buzzer_buzz
 * 功能描述： 启动蜂鸣器发出一段声音
 * 输入参数： freq - 声音频率
 *            time_ms - 持续时间
 * 输出参数： 无
 * 返 回 值： 无
 * 修改日期：      版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2023/09/08	     V1.0	  韦东山	      创建
 ***********************************************************************/
void buzzer_buzz(int freq, int time_ms);


#endif /* BEEP_H_ */
