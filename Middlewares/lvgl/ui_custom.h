/**
 * ui_custom.h
 * 独立自定义 UI 更新模块
 * 作用：把 SquareLine 生成的 UI 代码与业务逻辑分离，避免 SquareLine 重新导出时覆盖自定义代码
 *
 * 该文件放在 Middlewares/lvgl/ 下，SquareLine 不会触碰它
 */

#ifndef _UI_CUSTOM_H
#define _UI_CUSTOM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========== UI 状态更新（线程安全：Key_Task 标记，lvgl_Task 执行） ========== */
void update_wifi_status(const char *status);
void parse_wifi_list(const char *response);
void apply_ui_updates(void);

/* ========== 传感器 UI 更新（线程安全：Sensor_Task 标记，lvgl_Task 执行） ========== */
void update_sensor_display(float temperature, float humidity, uint16_t light);

/* ========== WiFi 扫描控制 ========== */
void wifi_scan_start(void);
uint8_t is_wifi_scanning(void);
uint32_t get_wifi_scan_start_time(void);
void clear_wifi_scanning(void);

#ifdef __cplusplus
}
#endif

#endif
