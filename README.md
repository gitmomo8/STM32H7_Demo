STM32H7 芯片（Cortex-M7 内核）和 Keil MDK (armclang) 环境

## 下载

[FreeRTOS-Kernel V12.2.0](https://github.com/FreeRTOS/FreeRTOS-Kernel/releases/tag/V11.2.0)

[FreeRTOS](https://github.com/FreeRTOS/FreeRTOS)

[FreeRTOS 主页下载 FreeRTOS 202406.04 LTS](https://www.freertos.org/zh-cn-cmn-s/Documentation/02-Kernel/01-About-the-FreeRTOS-kernel/03-Download-freeRTOS/01-DownloadFreeRTOS)

## 精简拷贝源码文件

在工程目录下新建 `FreeRTOS` 文件夹，按以下路径拷贝文件：

### 核心文件

目标路径：FreeRTOS-LTS\FreeRTOS\FreeRTOS-Kernel\

拷贝文件：tasks.c, list.c, queue.c, timers.c, event_groups.c, stream_buffer.c

### 头文件

目标路径：FreeRTOS-LTS\FreeRTOS\FreeRTOS-Kernel\include\

拷贝文件：拷贝整个 include 文件夹

### 移植层文件

#### 接口文件

目标路径：FreeRTOS-LTS\FreeRTOS\FreeRTOS-Kernel\portable\

拷贝文件：拷贝 GCC\ARM_CM7\r0p1\ 整个文件夹（Keil 的 armclang 也是基于 LLVM/GCC 语法的，用这个路径最稳）

#### 内存管理

拷贝文件：MemMang\heap_4.c

### 配置文件

拷贝文件：从 FreeRTOS-LTS\FreeRTOS\Demo\CORTEX_M7_M4_AMP_STM32H745I_Discovery_IAR\CM7\include（或其他 H7 Demo）中拷贝一个 FreeRTOSConfig.h

## 在 keil 中组织项目

### 新建分组

在 Keil 的 Project 窗口新建 FreeRTOS/Core 和 FreeRTOS/Port

### 添加文件

Core 组：添加 tasks.c, list.c, queue.c 等。

Port 组：添加 heap_4.c 和 port.c。

### 配置包含路径（Include Paths）

.\FreeRTOS\include

.\FreeRTOS\portable\GCC\ARM_CM7\r0p1

以及 FreeRTOSConfig.h 所在的目录

## 修改关键配置

### 中断映射

去 stm32h7xx_it.c 里把“SVC_Handler、PendSV_Handler、SysTick_Handler”这三个函数的空壳注释掉，否则会报错

也可以在 CubeMX 中的 NVIC 点击 Code generation 选项卡，在表格中找到以下三行，取消勾选这三行对应的 Generate IRQ Handler 复选框，重新生成代码即可

System service call via SWI instruction (对应 SVC_Handler)

Pendable request for system service (对应 PendSV_Handler)

System tick timer (对应 SysTick_Handler)

### 优先级配置

H7 必须使用组 4 优先级，main.c 初始化时确保调用 

```C
HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
```

也可以在 CubeMX 中的 NVIC 中的 Priority Group 选择 4 bits for pre-emption priority 0 bits for subpriority

## 编写第一个测试任务

在 main.c 中：

``` C
#include "FreeRTOS.h"
#include "task.h"

void StartTask(void *pvParameters) {
    for(;;) {
        // 这里放你的 LED 翻转代码
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

int main(void) {
    HAL_Init();
    SystemClock_Config(); 
    // ... 硬件初始化 ...

    xTaskCreate(StartTask, "StartTask", 256, NULL, 1, NULL);
    vTaskStartScheduler(); // 启动调度

    while(1);
}
```

## 编译报错

如果报错 vApplicationGetIdleTaskMemory 找不到：说明你开启了静态分配。在 FreeRTOSConfig.h 中搜 configSUPPORT_STATIC_ALLOCATION 设为 0，或者直接提供这个回调函数，同时确保 `#define configSUPPORT_DYNAMIC_ALLOCATION` 是 1

## STM32 的 HAL 库时基

当 FreeRTOS 占用了 SysTick 之后，首选 TIM6 / TIM7 (基本定时器)，次选 TIM1 (高级定时器)

HAL Timebase (TIM6)： 建议设为 0（最高），确保 HAL 库的计数器永不停止。

FreeRTOS (SysTick)： 由 FreeRTOS 管理，通常保持为 15（最低）。
