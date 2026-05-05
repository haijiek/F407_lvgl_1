/**
 * ui_custom.c
 * 独立自定义 UI 更新模块
 *
 * 线程安全设计：
 * - Key_Task / Sensor_Task（高优先级）负责接收数据、解析状态，只写缓冲区和标志位
 * - lvgl_Task（Normal优先级）在 lv_timer_handler() 后调用 apply_ui_updates()，
 *   在 LVGL 线程安全地刷新控件
 */

#include "ui_custom.h"
#include "ui.h"
#include "screens/ui_Screen1.h"
#include <string.h>
#include <stdio.h>
#include "cmsis_os.h"

/* ========== WiFi 状态刷新（线程安全：Key_Task 写，lvgl_Task 读） ========== */
static char wifi_status_str[64] = "Disconnected";
static volatile uint8_t wifi_status_dirty = 0;

/* ========== WiFi 列表刷新（线程安全） ========== */
static char wifi_dropdown_options[512] = {0};
static volatile uint8_t wifi_dropdown_dirty = 0;

/* ========== 传感器数据刷新（线程安全：Sensor_Task 写，lvgl_Task 读） ========== */
static float sensor_temp = 0.0f;
static float sensor_humi = 0.0f;
static uint16_t sensor_light = 0;
static volatile uint8_t sensor_dirty = 0;

/* ========== WiFi 扫描状态 ========== */
static uint32_t wifi_scan_start_time = 0;
static uint8_t wifi_scanning = 0;

/* 外部声明：Send_AT_Command 定义在 freertos.c 中 */
extern void Send_AT_Command(const char *cmd);

/* ========== WiFi UI 更新接口 ========== */

void update_wifi_status(const char *status)
{
    strncpy(wifi_status_str, status, sizeof(wifi_status_str) - 1);
    wifi_status_str[sizeof(wifi_status_str) - 1] = '\0';
    wifi_status_dirty = 1;
}

void parse_wifi_list(const char *response)
{
    const char *p = strstr(response, "WIFI_LIST:");
    if(p == NULL) return;
    p += 10;

    char options[512] = {0};

    while(*p != '\0' && strncmp(p, "END", 3) != 0) {
        char ssid[33] = {0};
        int rssi, enc;

        if(sscanf(p, "%32[^,],%d,%d", ssid, &rssi, &enc) >= 1) {
            if(strlen(options) > 0) strcat(options, "\n");
            strncat(options, ssid, 32);
        }

        p = strchr(p, '|');
        if(p == NULL) break;
        p++;
    }

    if(strlen(options) > 0) {
        strncpy(wifi_dropdown_options, options, sizeof(wifi_dropdown_options) - 1);
        wifi_dropdown_options[sizeof(wifi_dropdown_options) - 1] = '\0';
        wifi_dropdown_dirty = 1;
    }
    wifi_scanning = 0;
}

/* ========== 传感器 UI 更新接口 ========== */

void update_sensor_display(float temperature, float humidity, uint16_t light)
{
    sensor_temp = temperature;
    sensor_humi = humidity;
    sensor_light = light;
    sensor_dirty = 1;
}

/* 在 lvgl_Task 中调用，将标记的 UI 更新实际应用到屏幕 */
void apply_ui_updates(void)
{
    /* --- WiFi 状态 --- */
    if(wifi_status_dirty) {
        wifi_status_dirty = 0;
        lv_textarea_set_text(ui_Wifi_state, wifi_status_str);
    }

    /* --- WiFi 列表 --- */
    if(wifi_dropdown_dirty) {
        wifi_dropdown_dirty = 0;
        lv_dropdown_set_options(ui_Cmd, wifi_dropdown_options);
    }

    /* --- 传感器数据（Parameter 选项卡）--- */
    if(sensor_dirty) {
        sensor_dirty = 0;
        char buf[32];

        snprintf(buf, sizeof(buf), "%.1f C", sensor_temp);
        lv_textarea_set_text(ui_TemperatureText, buf);

        snprintf(buf, sizeof(buf), "%.1f %%", sensor_humi);
        lv_textarea_set_text(ui_HumidityText, buf);

        snprintf(buf, sizeof(buf), "%d", sensor_light);
        lv_textarea_set_text(ui_LightText, buf);
    }
}

/* ========== WiFi 扫描控制 ========== */

void wifi_scan_start(void)
{
    Send_AT_Command("WIFI_SCAN\n");
    wifi_scanning = 1;
    wifi_scan_start_time = osKernelGetTickCount();
}

uint8_t is_wifi_scanning(void)
{
    return wifi_scanning;
}

uint32_t get_wifi_scan_start_time(void)
{
    return wifi_scan_start_time;
}

void clear_wifi_scanning(void)
{
    wifi_scanning = 0;
}
