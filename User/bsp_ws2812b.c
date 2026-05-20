/**
 * @file    bsp_ws2812b.c
 * @brief   WS2812B 驱动模块
 */

#include "bsp_ws2812b.h"
#include "spi.h"

#include "log.h"

#define WS2812_SPI_Handle hspi1

ws2812b_rgb_color_t ws2812b_led_data[WS2812B_QUANTITY] = {0};
// uint8_t spi_dma_buffer[SPI_DMA_BUFFER_SIZE];

// 使用 32 字节对齐，确保该变量占据独立的 Cache Line
__ALIGNED(32)
uint8_t spi_dma_buffer[SPI_DMA_BUFFER_SIZE];

/* 在 bsp_ws2812b.c 中修改定义 */
/* 强制定位到 0x30000000，避开 DTCM */
// uint8_t spi_dma_buffer[SPI_DMA_BUFFER_SIZE] __attribute__((section(".bss.ARM.__at_0x24000000"))) __attribute__((aligned(32)));

/**
 * @brief 设置指定灯珠的 RGB 颜色分量
 * @param  index: 灯珠的索引，范围 0~WS2812B_QUANTITY-1
 * @param  r: 红色亮度值 (0 - 255)
 * @param  g: 绿色亮度值 (0 - 255)
 * @param  b: 蓝色亮度值 (0 - 255)
 */
void BSP_WS2812_SetPixel(uint16_t index, uint8_t r, uint8_t g, uint8_t b)
{
    if (index < WS2812B_QUANTITY)
    {
        ws2812b_led_data[index].r = r;
        ws2812b_led_data[index].g = g;
        ws2812b_led_data[index].b = b;
    }
}

/**
 * @brief HSV 转 RGB
 * @param  index: 灯珠的索引，范围 0~WS2812B_QUANTITY-1
 * @param  h: 色调 (0.0 - 360.0)
 * @param  s: 饱和度 (0.0 - 1.0)
 * @param  v: 亮度 (0.0 - 1.0)
 */
void BSP_WS2812_SetHSV(uint16_t index, float h, float s, float v)
{
    float r = 0.0f, g = 0.0f, b = 0.0f;
    int i = (int)(h / 60.0f) % 6;
    float f = (h / 60.0f) - (int)(h / 60.0f);
    float p = v * (1.0f - s);
    float q = v * (1.0f - f * s);
    float t = v * (1.0f - (1.0f - f) * s);

    switch (i)
    {
    case 0:
        r = v, g = t, b = p;
        break;
    case 1:
        r = q, g = v, b = p;
        break;
    case 2:
        r = p, g = v, b = t;
        break;
    case 3:
        r = p, g = q, b = v;
        break;
    case 4:
        r = t, g = p, b = v;
        break;
    case 5:
        r = v, g = p, b = q;
        break;
    }
    /* 转换为 0-255 并写入 */
    BSP_WS2812_SetPixel(index, (uint8_t)(r * 255), (uint8_t)(g * 255), (uint8_t)(b * 255));
}

/**
 * @brief 将 RGB 数据根据协议转换后使用 SPI DMA 发送
 */
void BSP_WS2812_Update(void)
{
    uint32_t dat;
    uint32_t pos = 0;

    for (int i = 0; i < WS2812B_QUANTITY; i++)
    {
        /* WS2812 协议顺序是 g-r-b */
        dat = (ws2812b_led_data[i].g << 16) | (ws2812b_led_data[i].r << 8) | (ws2812b_led_data[i].b);

        /* 协议要求先发送颜色分量的高位 */
        for (int bit = 23; bit >= 0; bit--)
        {
            if ((dat >> bit) & 0x01)
            {
                spi_dma_buffer[pos++] = WS2812_1;
            }
            else
            {
                spi_dma_buffer[pos++] = WS2812_0;
            }
        }
    }

    /* 末尾填充复位低电平信号 */
    for (int i = 0; i < WS2812_RESET_LEN; i++)
    {
        spi_dma_buffer[pos++] = 0x00;
    }

    printf("Buffer Address: 0x%08X\r\n", (uint32_t)spi_dma_buffer);

    /* 2. 在开启 DMA 前，将 DCache 中的内容“刷”入 SRAM */
    /* 注意：地址和长度最好都是 32 的倍数 */
    SCB_CleanDCache_by_Addr((uint32_t *)spi_dma_buffer, SPI_DMA_BUFFER_SIZE);
    // 3. 发送数据
    if (HAL_SPI_Transmit_DMA(&hspi1, spi_dma_buffer, SPI_DMA_BUFFER_SIZE) != HAL_OK)
    {
        printf("DMA Start Failed\r\n");
    }
}

/**
 * @brief WS2812 测试，进入后为死循环
 */
void BSP_WS2812_Test(uint8_t mode)
{
    uint16_t start_hue = 0;

    if (!mode)
    {
        while (1)
        {
            for (int i = 0; i < WS2812B_QUANTITY; i++)
            {
                BSP_WS2812_SetHSV(i, (float)0, 0, 1.0f);
            }
            BSP_WS2812_Update();
            HAL_Delay(30);
        }
    }
    else if (mode == 1)
    {
        while (1)
        {
            for (int i = 0; i < WS2812B_QUANTITY; i++)
            {
                /* 每个灯珠的色调比前一个偏移 20 度 */
                uint16_t hue = (start_hue + (i * 20)) % 360;
                BSP_WS2812_SetHSV(i, (float)hue, 1.0f, 0.1f);
            }
            BSP_WS2812_Update();
            start_hue = (start_hue + 5) % 360; /* 偏移起始色调，产生流动感 */
            HAL_Delay(30);
        }
    }
}
