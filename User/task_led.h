/**
 * @file task_led.h
 * @brief LED任务头文件
 */

#ifndef __TASK_LED_H__
#define __TASK_LED_H__

#include "FreeRTOS.h"
#include "task.h"

/**
 * @brief LED任务初始化
 * @retval None
 */
void Task_Led_Init(void);

#endif
