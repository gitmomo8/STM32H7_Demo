/**
 * @file task_usart.h
 * @brief 串口任务头文件
 */

#ifndef __TASK_USART_H__
#define __TASK_USART_H__

#include "FreeRTOS.h"
#include "task.h"

/**
 * @brief 串口任务初始化
 * @retval None
 */
void Task_Usart_Init(void);

#endif
