/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "ui.h"
#include  "key.h"
#include "led.h"
#include "usart.h"
#include "stdio.h"
#include "ui_events.h"
#include "ui_custom.h"
#include "dht11.h"
#include "adc.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint16_t t;
int flag ;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define WIFI_SCAN_TIMEOUT 8000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
/* WiFi variables moved to wifi_control.c */
/* USER CODE END Variables */
/* Definitions for lvgl_Task */
osThreadId_t lvgl_TaskHandle;
const osThreadAttr_t lvgl_Task_attributes = {
  .name = "lvgl_Task",
  .stack_size = 2048 * 4,
  .priority = (osPriority_t) osPriorityRealtime,
};
/* Definitions for touch_Task02 */
osThreadId_t touch_Task02Handle;
const osThreadAttr_t touch_Task02_attributes = {
  .name = "touch_Task02",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for Key_Task03 */
osThreadId_t Key_Task03Handle;
const osThreadAttr_t Key_Task03_attributes = {
  .name = "Key_Task03",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Senor_Task04 */
osThreadId_t Senor_Task04Handle;
const osThreadAttr_t Senor_Task04_attributes = {
  .name = "Senor_Task04",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for lvgl_Timer01 */
osTimerId_t lvgl_Timer01Handle;
const osTimerAttr_t lvgl_Timer01_attributes = {
  .name = "lvgl_Timer01"
};
/* Definitions for my_Printf_Mutex01 */
osMutexId_t my_Printf_Mutex01Handle;
const osMutexAttr_t my_Printf_Mutex01_attributes = {
  .name = "my_Printf_Mutex01"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Start_lvgl_Task01(void *argument);
void Start_touch_Task02(void *argument);
void Start_Key_Task03(void *argument);
void Start_Senor_Task04(void *argument);
void lvgl_Callback01(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of my_Printf_Mutex01 */
  my_Printf_Mutex01Handle = osMutexNew(&my_Printf_Mutex01_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of lvgl_Timer01 */
  lvgl_Timer01Handle = osTimerNew(lvgl_Callback01, osTimerPeriodic, NULL, &lvgl_Timer01_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of lvgl_Task */
  lvgl_TaskHandle = osThreadNew(Start_lvgl_Task01, NULL, &lvgl_Task_attributes);

  /* creation of touch_Task02 */
  touch_Task02Handle = osThreadNew(Start_touch_Task02, NULL, &touch_Task02_attributes);

  /* creation of Key_Task03 */
  Key_Task03Handle = osThreadNew(Start_Key_Task03, NULL, &Key_Task03_attributes);

  /* creation of Senor_Task04 */
  Senor_Task04Handle = osThreadNew(Start_Senor_Task04, NULL, &Senor_Task04_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_Start_lvgl_Task01 */
/**
  * @brief  Function implementing the lvgl_Task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Start_lvgl_Task01 */
void Start_lvgl_Task01(void *argument)
{
  /* USER CODE BEGIN Start_lvgl_Task01 */
  lv_init();
  lv_port_disp_init();
  lv_port_indev_init();

  ui_init();
  osTimerStart(lvgl_Timer01Handle, 5);  /* 5ms tick timer */

  /* Infinite loop */
  for(;;)
  {
    /* lv_timer_handler() 会处理所有到期的LVGL任务
     * 返回下一次需要调用的时间(ms)
     * 使用osDelay让出CPU,避免阻塞其他任务
     */
    uint32_t time_till_next = lv_timer_handler();

    /* 将 Key_Task 中标记的 UI 更新应用到屏幕（必须在 LVGL 任务中执行） */
    apply_ui_updates();

    /* 限制最小延时1ms,避免过于频繁调度 */
    if(time_till_next < 1) time_till_next = 1;
    /* 限制最大延时33ms,保证约30FPS的刷新率 */
    if(time_till_next > 33) time_till_next = 33;

    osDelay(time_till_next);
  }
  /* USER CODE END Start_lvgl_Task01 */
}

/* USER CODE BEGIN Header_Start_touch_Task02 */
/**
* @brief Function implementing the touch_Task02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_touch_Task02 */
void Start_touch_Task02(void *argument)
{
  /* USER CODE BEGIN Start_touch_Task02 */
  char GBD_buf[32] = {0};
  char last_tx_buf[64] = {0};
  char last_rx_buf[64] = {0};

  /* Infinite loop - 更新串口数据显示到UI */
  for(;;)
  {
    /* 只在数据变化时才更新UI,减少不必要的刷新 */

    /* 100ms刷新一次足够显示串口数据 */
    osDelay(100);
  }
  /* USER CODE END Start_touch_Task02 */
}

/* USER CODE BEGIN Header_Start_Key_Task03 */
/**
* @brief Function implementing the Key_Task03 thread.
* @param argument: Not used
* @retval None
*/

/* WiFi functions moved to wifi_control.c */
void Send_AT_Command(const char *cmd)
{
    printf("%s\r\n", cmd);
}
/* USER CODE END Header_Start_Key_Task03 */
void Start_Key_Task03(void *argument)
{
  /* USER CODE BEGIN Start_Key_Task03 */
  LED_Init();
  USART3_Init_IO();

  uint8_t key0_cnt = 0, key0_trig = 0;
  uint8_t key1_cnt = 0, key1_trig = 0;
  uint8_t key2_cnt = 0, key2_trig = 0;

  // 上电自动扫描：等待 ESP-01 启动完成（2秒）
  osDelay(2000);
  update_wifi_status("Disconnected");
  wifi_scan_start();

  for(;;)
  {
    // 统一处理串口接收帧（只消费一次 frame_ready）
    if(usart3_rx_handler.frame_ready) {
      usart3_rx_handler.frame_ready = 0;
      char *rx = (char*)usart3_rx_handler.rx_buf;

      // 1. 如果在扫描期间，尝试解析 WiFi 列表
      if(is_wifi_scanning()) {
        parse_wifi_list(rx);
      }

      // 2. 无论是否在扫描，都检查 WiFi 状态响应
      if(strstr(rx, "STATUS:CONNECTED")) {
        update_wifi_status("Connected");
      }
      else if(strstr(rx, "STATUS:FAILED")) {
        update_wifi_status("Password Error");
      }
      else if(strstr(rx, "STATUS:CONNECTING")) {
        update_wifi_status("Connecting...");
      }
      else if(strstr(rx, "STATUS:DISCONNECTED")) {
        update_wifi_status("Disconnected");
      }
    }

    // 扫描超时处理
    if(is_wifi_scanning() && (osKernelGetTickCount() - get_wifi_scan_start_time()) > WIFI_SCAN_TIMEOUT) {
      clear_wifi_scanning();
    }

    // KEY_0: 手动刷新 WiFi 列表
    if(Key_GetState(KEY_0) == KEY_PRESSED) {
      if(key0_cnt < 3) key0_cnt++;
      else if(!key0_trig) {
        key0_trig = 1;
        wifi_scan_start();
        LED1_Green_TOGGLE();
      }
    } else {
      key0_cnt = 0;
      key0_trig = 0;
    }

    // KEY_1: 连接 WiFi
    if(Key_GetState(KEY_1) == KEY_PRESSED) {
      if(key1_cnt < 3) key1_cnt++;
      else if(!key1_trig) {
        key1_trig = 1;
        do_wifi_connect(NULL);
      }
    } else {
      key1_cnt = 0;
      key1_trig = 0;
    }

    // KEY_2: 断开 WiFi
    if(Key_GetState(KEY_2) == KEY_PRESSED) {
      if(key2_cnt < 3) key2_cnt++;
      else if(!key2_trig) {
        key2_trig = 1;
        do_wifi_disconnect(NULL);
      }
    } else {
      key2_cnt = 0;
      key2_trig = 0;
    }

    // 每 3 秒轮询一次 WiFi 状态
    static uint32_t last_status_query = 0;
    if(osKernelGetTickCount() - last_status_query > 3000) {
      last_status_query = osKernelGetTickCount();
      Send_AT_Command("WIFI_STATUS\n");
    }

    osDelay(10);
  }
  /* USER CODE END Start_Key_Task03 */
}

/* USER CODE BEGIN Header_Start_Senor_Task04 */
/**
* @brief Function implementing the Senor_Task04 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_Senor_Task04 */
void Start_Senor_Task04(void *argument)
{
  /* USER CODE BEGIN Start_Senor_Task04 */
    DHT11_Init();

    float temperature = 0.0f;
    float humidity = 0.0f;
    uint16_t light_raw = 0;

    /* Infinite loop */
    for(;;)
    {
        uint8_t dht11_ok = 1;

        vTaskSuspendAll();
        if(DHT11_Read_Data(&temperature, &humidity) != 0) {
            dht11_ok = 0;
        }
        xTaskResumeAll();

        uint32_t adc_sum = 0;
        for(int i = 0; i < 10; i++) {
            HAL_ADC_Start(&hadc3);
            if(HAL_ADC_PollForConversion(&hadc3, 10) == HAL_OK) {
                adc_sum += HAL_ADC_GetValue(&hadc3);
            }
            HAL_ADC_Stop(&hadc3);
        }
        light_raw = (uint16_t)(adc_sum / 10);

        update_sensor_display(temperature, humidity, light_raw);

        osDelay(2000);
    }
  /* USER CODE END Start_Senor_Task04 */
}

/* lvgl_Callback01 function */
void lvgl_Callback01(void *argument)
{
  /* USER CODE BEGIN lvgl_Callback01 */
  lv_tick_inc(5);

  /* USER CODE END lvgl_Callback01 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

