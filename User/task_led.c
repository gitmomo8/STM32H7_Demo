/**
 * @file task_led.c
 * @brief LED闪烁任务实现
 */

#include "task_led.h"
#include "log.h"
#include "gpio.h"

/**
 * @brief LED闪烁任务
 * @param pvParameters 任务参数（未使用）
 * @note 以500ms间隔切换LED状态
 */
void Led_Task(void *pvParameters)
{
    for (;;)
    {
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

/**
 * @brief LED任务初始化
 * @retval None
 * @note 创建LED闪烁任务，优先级为1，栈深度为256
 */
void Task_Led_Init(void)
{
    xTaskCreate(Led_Task, "Led_Task", 256, NULL, 1, NULL);
}
