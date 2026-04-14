/**
 * @file lv_port_disp_templ.c
 *
 */
#if 1

#include "lv_port_disp.h"
#include <stdbool.h>
#include "lcd.h"
/*********************
 *      DEFINES
 *********************/
#ifndef MY_DISP_HOR_RES
    #warning Please define or replace the macro MY_DISP_HOR_RES with the actual screen width, default value 320 is used for now.
    #define MY_DISP_HOR_RES    800
#endif

#ifndef MY_DISP_VER_RES
    #warning Please define or replace the macro MY_DISP_HOR_RES with the actual screen height, default value 240 is used for now.
    #define MY_DISP_VER_RES    480
#endif

static void disp_init(void);

static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);

void lv_port_disp_init(void)
{
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    disp_init();

    /* 使用更大的缓冲区以提升刷新速度
     * 建议: 至少分配1/10屏幕大小的缓冲区
     * 800*480 / 10 = 38400 像素, 约 75KB (16bit色深)
     * 如果内存足够,可以分配更大如: MY_DISP_HOR_RES * 100 (约150KB)
     */
    static lv_disp_draw_buf_t draw_buf_dsc_1;
    static lv_color_t buf_1[MY_DISP_HOR_RES * 20] __attribute__((section(".ccmram")));  /* 20行缓冲 ~ 32KB，放入CCMRAM */
    static lv_color_t buf_2[MY_DISP_HOR_RES * 20] __attribute__((section(".ccmram")));  /* 双缓冲 */
    lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, buf_2, MY_DISP_HOR_RES * 20);

    static lv_disp_drv_t disp_drv;                         /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);                    /*Basic initialization*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = MY_DISP_HOR_RES;
    disp_drv.ver_res = MY_DISP_VER_RES;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = disp_flush;

    /*Set a display buffer*/
    disp_drv.draw_buf = &draw_buf_dsc_1;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
    HAL_GPIO_WritePin(GPIOF,GPIO_PIN_10,GPIO_PIN_RESET);

}

/*Initialize your display and the required peripherals.*/
static void disp_init(void)
{
    /*You code here*/
    lcd_init();
    lcd_display_dir(1);

}
volatile bool disp_flush_enabled = true;

/* Enable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_enable_update(void)
{
    disp_flush_enabled = true;
}
/* Disable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_disable_update(void)
{
    disp_flush_enabled = false;
}

void lcd_draw_fast_rgb_color(int16_t sx, int16_t sy,int16_t ex, int16_t ey, uint16_t *color)
{
    uint16_t w = ex-sx+1;
    uint16_t h = ey-sy+1;

    lcd_set_window(sx, sy, w, h);
    uint32_t draw_size = w * h;
    lcd_write_ram_prepare();

    /* 使用 uint32_t 双字批量写入，减少一半循环次数，显著提升刷新速度 */
    volatile uint32_t *ram32 = (volatile uint32_t *)&LCD->LCD_RAM;
    uint32_t *p32 = (uint32_t *)color;
    uint32_t len32 = draw_size / 2;

    for(uint32_t i = 0; i < len32; i++)
    {
        *ram32 = p32[i];
    }

    if(draw_size & 1)  /* 奇数个像素，补写最后一个 */
    {
        LCD->LCD_RAM = color[draw_size - 1];
    }
}

static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    lcd_draw_fast_rgb_color(area->x1,area->y1,area->x2,area->y2,(uint16_t*)color_p);

    lv_disp_flush_ready(disp_drv);
}

#endif