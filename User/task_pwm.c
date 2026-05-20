/**
 * @file task_pwm.c
 * @brief PWM渐变任务实现
 */

#include "task_pwm.h"
#include "log.h"
#include "tim.h"

/**
 * @brief 设置PWM占空比
 * @param pwm 占空比百分比（0-100）
 * @note 将百分比转换为定时器CCR值，设置TIM1通道1的PWM输出
 */
static void Set_Pwm(uint8_t pwm)
{
    uint32_t ccr_value = 0;

    if (pwm > 100)
    {
        pwm = 100;
    }


    
    ccr_value = pwm * 10;

    if (ccr_value > 999)
    {
        ccr_value = 999;
    }

    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, ccr_value);
}

/**
 * @brief PWM渐变任务
 * @param pvParameters 任务参数（未使用）
 * @note 以50ms间隔从0%渐变到100%，再从100%渐变到0%，循环执行
 */
void Pwm_Task(void *pvParameters)
{
    for (;;)
    {
        for (uint8_t i = 0; i <= 100; i++)
        {
            Set_Pwm(i);
            LOG_D("占空比为 %d", i);
            vTaskDelay(pdMS_TO_TICKS(50));
        }
        for (uint8_t i = 100; i >= 1; i--)
        {
            Set_Pwm(i);
            LOG_D("占空比为 %d", i);
            vTaskDelay(pdMS_TO_TICKS(50));
        }
    }
}

/**
 * @brief PWM任务初始化
 * @retval None
 * @note 创建PWM渐变任务，优先级为2，栈深度为256
 */
void Task_Pwm_Init(void)
{
    xTaskCreate(Pwm_Task, "Pwm_Task", 256, NULL, 2, NULL);
}
