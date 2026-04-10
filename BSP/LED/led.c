#include "led.h"
#include "main.h"

// 初始化LED GPIO
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // 配置LED0 (PF9)
    GPIO_InitStruct.Pin = LED0_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED0_GPIO_PORT, &GPIO_InitStruct);

    // 配置LED1 (PF10)
    GPIO_InitStruct.Pin = LED1_GPIO_PIN;
    HAL_GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStruct);

    // 初始状态：关闭两个LED
    LED0_Red_OFF();
    LED1_Green_OFF();
}
#include "led.h"


