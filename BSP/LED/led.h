#ifndef __LED_H
#define __LED_H

#include "stm32f4xx_hal.h"

// LED端口和引脚定义
#define LED0_GPIO_PORT  GPIOF
#define LED0_GPIO_PIN   GPIO_PIN_9

#define LED1_GPIO_PORT  GPIOF
#define LED1_GPIO_PIN   GPIO_PIN_10

// LED控制宏（高电平点亮）
#define LED0_Red_ON()       HAL_GPIO_WritePin(LED0_GPIO_PORT, LED0_GPIO_PIN, GPIO_PIN_RESET)
#define LED0_Red_OFF()      HAL_GPIO_WritePin(LED0_GPIO_PORT, LED0_GPIO_PIN, GPIO_PIN_SET)
#define LED0_Red_TOGGLE()   HAL_GPIO_TogglePin(LED0_GPIO_PORT, LED0_GPIO_PIN)

#define LED1_Green_ON()       HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_GPIO_PIN, GPIO_PIN_RESET)
#define LED1_Green_OFF()      HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_GPIO_PIN, GPIO_PIN_SET)
#define LED1_Green_TOGGLE()   HAL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_GPIO_PIN)

void LED_Init(void);

#endif
