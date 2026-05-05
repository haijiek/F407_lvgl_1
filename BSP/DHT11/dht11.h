#ifndef __DHT11_H
#define __DHT11_H

#include "main.h"

#define DHT11_DQ_PORT   GPIOG
#define DHT11_DQ_PIN    GPIO_PIN_9

uint8_t DHT11_Init(void);
uint8_t DHT11_Read_Data(float *temperature, float *humidity);

#endif
