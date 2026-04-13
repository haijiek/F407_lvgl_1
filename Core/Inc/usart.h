/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "usart.h"
/* USER CODE END Includes */

extern UART_HandleTypeDef huart3;

/* USER CODE BEGIN Private defines */
#define USART3_RX_BUF_SIZE 256
#define USART3_TX_BUF_SIZE 256
#define DBG(fmt, ...) Debug_Print(fmt "\r\n", ##__VA_ARGS__)


typedef struct {
    uint8_t rx_buf[USART3_RX_BUF_SIZE];
    uint16_t rx_len;
    uint8_t frame_ready;
} USART3_RxHandler_t;

typedef struct {
    uint8_t tx_buf[USART3_TX_BUF_SIZE];
    uint16_t tx_len;
    uint8_t tx_done;
} USART3_TxHandler_t;

extern USART3_RxHandler_t usart3_rx_handler;
extern USART3_TxHandler_t usart3_tx_handler;
/* USER CODE END Private defines */

void MX_USART3_UART_Init(void);

/* USER CODE BEGIN Prototypes */
void USART3_Init_IO(void);
void Send_AT_Command(const char *cmd);
void Debug_Print(const char *fmt, ...);


/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

