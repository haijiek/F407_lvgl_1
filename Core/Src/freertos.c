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
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint16_t t;
int flag ;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for lvgl_Task */
osThreadId_t lvgl_TaskHandle;
const osThreadAttr_t lvgl_Task_attributes = {
  .name = "lvgl_Task",
  .stack_size = 1536 * 4,
  .priority = (osPriority_t) osPriorityNormal,
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
  .priority = (osPriority_t) osPriorityAboveNormal,
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
    if(memcmp(last_tx_buf, usart3_tx_handler.tx_buf, sizeof(last_tx_buf)) != 0)
    {
      memcpy(last_tx_buf, usart3_tx_handler.tx_buf, sizeof(last_tx_buf));
      lv_textarea_set_text(ui_SendText, (char *)usart3_tx_handler.tx_buf);
    }

    if(memcmp(last_rx_buf, usart3_rx_handler.rx_buf, sizeof(last_rx_buf)) != 0)
    {
      memcpy(last_rx_buf, usart3_rx_handler.rx_buf, sizeof(last_rx_buf));
      lv_textarea_set_text(ui_ReceiText, (char *)usart3_rx_handler.rx_buf);
    }

    /* 显示系统时钟信息 */
    snprintf(GBD_buf, sizeof(GBD_buf), "SysClk:%luMHz", HAL_RCC_GetSysClockFreq()/1000000);
    lv_textarea_set_text(ui_GDBText, GBD_buf);

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

void Send_AT_Command(const char *cmd)
{
  uint16_t len = strlen(cmd);

  // memcpy(usart3_tx_handler.tx_buf, cmd, len);//cmd字符复制到usart3_tx_handler.tx_buf里面
  // usart3_tx_handler.tx_len = len;            //发送命令的长度

  printf("%s\r\n", cmd);
}
/* USER CODE END Header_Start_Key_Task03 */
void Start_Key_Task03(void *argument)
{
  /* USER CODE BEGIN Start_Key_Task03 */
  LED_Init();
  USART3_Init_IO();
  char AT_CMD_1[] = "AT";
  char AT_CMD_2[] = "AT+CWMODE=1";
  char AT_CMD_3[] = "AT+CWJAP=\"IQOO15\",\"12345678\"";
  char AT_CMD_4[] = "AT+ATKCLDSTA=\"35050997308880666791\",\"12345678\"";

  /* 按键消抖计数器与边沿触发标志 */
  uint8_t key0_cnt = 0, key1_cnt = 0, key2_cnt = 0, keyup_cnt = 0;
  uint8_t key0_trig = 0, key1_trig = 0, key2_trig = 0, keyup_trig = 0;

  char buf[50];

  /* Infinite loop */
  for(;;)
  {
    lv_dropdown_get_selected_str(ui_Cmd, usart3_tx_handler.tx_buf, sizeof(usart3_tx_handler.tx_buf));
    const char * text = lv_textarea_get_text(ui_SendText);
    /* KEY_0: 消抖 + 边沿检测，防止漏检和连发 */
    if (Key_GetState(KEY_0) == KEY_PRESSED) {
      if (key0_cnt < 3) key0_cnt++;
      else if (!key0_trig) {
        key0_trig = 1;
        Send_AT_Command(text);
        LED1_Green_TOGGLE();
      }
    } else {
      key0_cnt = 0;
      key0_trig = 0;
    }

    /* KEY_1 */
    if (Key_GetState(KEY_1) == KEY_PRESSED) {
      if (key1_cnt < 3) key1_cnt++;
      else if (!key1_trig) {
        key1_trig = 1;
        Send_AT_Command(AT_CMD_4);
        LED1_Green_TOGGLE();
      }
    } else {
      key1_cnt = 0;
      key1_trig = 0;
    }

    /* KEY_2 */
    if (Key_GetState(KEY_2) == KEY_PRESSED) {
      if (key2_cnt < 3) key2_cnt++;
      else if (!key2_trig) {
        key2_trig = 1;
        LED1_Green_TOGGLE();
      }
    } else {
      key2_cnt = 0;
      key2_trig = 0;
    }

    /* KEY_UP */
    if (Key_GetState(KEY_UP) == KEY_PRESSED) {
      if (keyup_cnt < 3) keyup_cnt++;
      else if (!keyup_trig) {
        keyup_trig = 1;
        LED1_Green_TOGGLE();
      }
    } else {
      keyup_cnt = 0;
      keyup_trig = 0;
    }

    osDelay(10);
  }
  /* USER CODE END Start_Key_Task03 */
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

