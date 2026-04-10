#include "key.h"
#include "main.h"

// 定义按键GPIO
#define KEY_UP_GPIO_PORT  GPIOA
#define KEY_UP_GPIO_PIN   GPIO_PIN_0

#define KEY_0_GPIO_PORT   GPIOE
#define KEY_0_GPIO_PIN    GPIO_PIN_4

#define KEY_1_GPIO_PORT   GPIOE
#define KEY_1_GPIO_PIN    GPIO_PIN_3

#define KEY_2_GPIO_PORT   GPIOE
#define KEY_2_GPIO_PIN    GPIO_PIN_2

// // 定义LEDGPIO（根据实际硬件修改）
// #define LED_GPIO_PORT     GPIOB
// #define LED_GPIO_PIN      GPIO_PIN_5



// 获取指定按键的状态（非阻塞）
uint8_t Key_GetState(uint8_t key)
{
    switch (key)
    {
        case KEY_0:
            return (HAL_GPIO_ReadPin(KEY_0_GPIO_PORT, KEY_0_GPIO_PIN) == GPIO_PIN_RESET) ? KEY_PRESSED : KEY_RELEASED;

        case KEY_1:
            return (HAL_GPIO_ReadPin(KEY_1_GPIO_PORT, KEY_1_GPIO_PIN) == GPIO_PIN_RESET) ? KEY_PRESSED : KEY_RELEASED;

        case KEY_2:
            return (HAL_GPIO_ReadPin(KEY_2_GPIO_PORT, KEY_2_GPIO_PIN) == GPIO_PIN_RESET) ? KEY_PRESSED : KEY_RELEASED;

        case KEY_UP:
            return (HAL_GPIO_ReadPin(KEY_UP_GPIO_PORT, KEY_UP_GPIO_PIN) == GPIO_PIN_RESET) ? KEY_PRESSED : KEY_RELEASED;

        default:
            return KEY_RELEASED;
    }
}

// 按键模块初始化（可用于LED等初始化）
// void Key_Init(void)
// {
//     GPIO_InitTypeDef GPIO_InitStruct = {0};
//
//     // 使能LED GPIO时钟
//     __HAL_RCC_GPIOB_CLK_ENABLE();
//
//     GPIO_InitStruct.Pin = LED_GPIO_PIN;
//     GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//     GPIO_InitStruct.Pull = GPIO_NOPULL;
//     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//
//     HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);
//
//     HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_SET); // 初始熄灭
// }
