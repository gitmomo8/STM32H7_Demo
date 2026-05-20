/**
 * @file task_usart.c
 * @brief 串口任务状态打印实现
 */

#include "task_usart.h"
#include "log.h"
#include <string.h>
#include <stdio.h>

/**
 * @brief 串口任务状态打印任务
 * @param pvParameters 任务参数（未使用）
 * @note 每2秒打印一次FreeRTOS任务列表和运行时统计信息
 */
void Usart_Task(void *pvParameters)
{
    uint8_t pcWriteBuffer[512];


    
    for (;;)
    {
        memset(pcWriteBuffer, 0, strlen((char *)pcWriteBuffer));
        printf("=================================================\r\n");
        printf("任务名      任务状态 优先级   剩余栈 任务序号\r\n");
        vTaskList((char *)&pcWriteBuffer);
        printf("%s\r\n", pcWriteBuffer);

        memset(pcWriteBuffer, 0, strlen((char *)pcWriteBuffer));
        printf("\r\n任务名       运行计数         使用率\r\n");
        vTaskGetRunTimeStats((char *)&pcWriteBuffer);
        printf("%s\r\n", pcWriteBuffer);

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

/**
 * @brief 串口任务初始化
 * @retval None
 * @note 创建串口任务状态打印任务，优先级为1，栈深度为256
 */
void Task_Usart_Init(void)
{
    xTaskCreate(Usart_Task, "Usart_Task", 256, NULL, 1, NULL);
}
