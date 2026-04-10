//
// Created by lzn16 on 2026/4/10.
//

#ifndef F407_LVGL_1_KEY_H
#define F407_LVGL_1_KEY_H


#include "stm32f4xx_hal.h"

// 按键定义
#define KEY_UP    1
#define KEY_0     2
#define KEY_1     3
#define KEY_2     4
#define KEY_RELEASED      1
#define KEY_PRESSED       0
// 按键状态获取函数
uint8_t Key_GetState(uint8_t key);

// 按键初始化函数（用于LED等初始化）
void Key_Init(void);

#endif
