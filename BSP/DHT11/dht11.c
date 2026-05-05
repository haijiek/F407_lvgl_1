#include "dht11.h"
#include "delay.h"

static void DHT11_DQ_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DHT11_DQ_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DHT11_DQ_PORT, &GPIO_InitStruct);
}

static void DHT11_DQ_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DHT11_DQ_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(DHT11_DQ_PORT, &GPIO_InitStruct);
}

static uint8_t DHT11_Check_Response(void)
{
    uint8_t retry = 0;
    DHT11_DQ_IN();
    while (HAL_GPIO_ReadPin(DHT11_DQ_PORT, DHT11_DQ_PIN) == GPIO_PIN_SET && retry < 200) {
        retry++;
        delay_us(1);
    }
    if(retry >= 200) return 1;

    retry = 0;
    while (HAL_GPIO_ReadPin(DHT11_DQ_PORT, DHT11_DQ_PIN) == GPIO_PIN_RESET && retry < 200) {
        retry++;
        delay_us(1);
    }
    if(retry >= 200) return 1;

    return 0;
}

static uint8_t DHT11_Read_Bit(void)
{
    uint8_t retry = 0;
    while (HAL_GPIO_ReadPin(DHT11_DQ_PORT, DHT11_DQ_PIN) == GPIO_PIN_SET && retry < 200) {
        retry++; delay_us(1);
    }
    retry = 0;
    while (HAL_GPIO_ReadPin(DHT11_DQ_PORT, DHT11_DQ_PIN) == GPIO_PIN_RESET && retry < 200) {
        retry++; delay_us(1);
    }
    delay_us(40);
    if (HAL_GPIO_ReadPin(DHT11_DQ_PORT, DHT11_DQ_PIN) == GPIO_PIN_SET) {
        retry = 0;
        while (HAL_GPIO_ReadPin(DHT11_DQ_PORT, DHT11_DQ_PIN) == GPIO_PIN_SET && retry < 200) {
            retry++; delay_us(1);
        }
        return 1;
    }
    return 0;
}

static uint8_t DHT11_Read_Byte(void)
{
    uint8_t i, dat = 0;
    for (i = 0; i < 8; i++) {
        dat <<= 1;
        dat |= DHT11_Read_Bit();
    }
    return dat;
}

uint8_t DHT11_Init(void)
{
    __HAL_RCC_GPIOG_CLK_ENABLE();
    DHT11_DQ_OUT();
    HAL_GPIO_WritePin(DHT11_DQ_PORT, DHT11_DQ_PIN, GPIO_PIN_SET);
    HAL_Delay(1000);
    return 0;
}

uint8_t DHT11_Read_Data(float *temperature, float *humidity)
{
    uint8_t buf[5] = {0};
    uint8_t i;

    DHT11_DQ_OUT();
    HAL_GPIO_WritePin(DHT11_DQ_PORT, DHT11_DQ_PIN, GPIO_PIN_RESET);
    HAL_Delay(18);
    HAL_GPIO_WritePin(DHT11_DQ_PORT, DHT11_DQ_PIN, GPIO_PIN_SET);
    delay_us(30);
    DHT11_DQ_IN();

    if (DHT11_Check_Response() != 0) return 1;

    for (i = 0; i < 5; i++) {
        buf[i] = DHT11_Read_Byte();
    }

    DHT11_DQ_OUT();
    HAL_GPIO_WritePin(DHT11_DQ_PORT, DHT11_DQ_PIN, GPIO_PIN_SET);

    if ((uint8_t)(buf[0] + buf[1] + buf[2] + buf[3]) != buf[4]) return 2;

    *humidity = buf[0] + buf[1] * 0.1f;
    *temperature = buf[2] + buf[3] * 0.1f;

    return 0;
}
