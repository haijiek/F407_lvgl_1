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
    if (!(DWT->CTRL & DWT_CTRL_CYCCNTENA_Msk)) {
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
        DWT->CYCCNT = 0;
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    }
    uint32_t start = DWT->CYCCNT;
    uint32_t ticks = us * (HAL_RCC_GetSysClockFreq() / 1000000);
    while ((DWT->CYCCNT - start) < ticks);
}
