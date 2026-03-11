//
// Created by lzn16 on 2026/3/9.
//

#include "delay.h"

void delay_ms(uint32_t ms) {
    if (ms > 0) {
        HAL_Delay(ms);
    }
}

void delay_us(uint32_t us) {
    uint32_t start, current, elapsed;
    uint32_t ticks = us * (SystemCoreClock / 1000000);

    start = SysTick->VAL;

    do {
        current = SysTick->VAL;
        if (current > start) {
            elapsed = (start + (SysTick->LOAD + 1) - current);
        } else {
            elapsed = start - current;
        }
    } while (elapsed < ticks);
}
