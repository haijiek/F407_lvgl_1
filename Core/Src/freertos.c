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
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

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
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for touch_Task02 */
osThreadId_t touch_Task02Handle;
const osThreadAttr_t touch_Task02_attributes = {
  .name = "touch_Task02",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Start_lvgl_Task01(void *argument);
void Start_touch_Task02(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

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
  uint8_t x = 0;
  uint8_t lcd_id[12];
  lcd_init();
  /* Infinite loop */
  for(;;)
  {
    lcd_show_string(10, 40, 240, 32, 32, "STM32", RED);
    lcd_show_string(10, 80, 240, 24, 24, "TFTLCD TEST", RED);
    lcd_show_string(10, 110, 240, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(10, 230, 240, 16, 16, (char *)lcd_id, RED); /* 显示LCD ID */
    x++;
    lcd_show_num(10,160,x,20,32,RED);
    if (lcddev.id == 0x9486) {
      HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_9);
      HAL_Delay(500);
    }
    // lcd_show_num(10,190,aa,20,32,RED);
    // lcd_show_num(10,220,(uint32_t)&aa,20,32,RED);
    osDelay(1);
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
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Start_touch_Task02 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

