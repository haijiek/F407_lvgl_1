// ... existing code ...
#include "24cxx.h"
#include "main.h"
#include "i2c.h"

#define EEPROM_I2C_TIMEOUT 100
#define EEPROM_MAX_RETRIES 3

/**
 * @brief       初始化IIC接口
 * @param       无
 * @retval      无
 */
void at24cxx_init(void)
{
    MX_I2C1_Init();
}

/**
 * @brief       在 AT24CXX 指定地址读出一个数据
 * @param       addr: 开始读数的地址
 * @retval      读到的数据
 */
uint8_t at24cxx_read_one_byte(uint16_t addr)
{
    uint8_t temp = 0;
    uint16_t memadd_size;
    uint16_t device_addr;
    HAL_StatusTypeDef status;

    if (EE_TYPE > AT24C16)
    {
        memadd_size = I2C_MEMADD_SIZE_16BIT;
        device_addr = 0xA0;
    }
    else
    {
        memadd_size = I2C_MEMADD_SIZE_8BIT;
        device_addr = 0xA0 + ((addr >> 8) << 1);
    }

    // 添加重试机制
    for (int retry = 0; retry < EEPROM_MAX_RETRIES; retry++)
    {
        status = HAL_I2C_Mem_Read(&hi2c1,
                                 device_addr,
                                 addr,
                                 memadd_size,
                                 &temp,
                                 1,
                                 EEPROM_I2C_TIMEOUT);

        if (status == HAL_OK)
        {
            break;
        }

        // 如果不是最后一次重试，延时后重试
        if (retry < EEPROM_MAX_RETRIES - 1)
        {
            HAL_Delay(5);
        }
    }

    return temp;
}

// ... existing code ...

/**
 * @brief       在 AT24CXX 指定地址写入一个数据
 * @param       addr: 写入数据的目的地址
 * @param       data: 要写入的数据
 * @retval      无
 */
void at24cxx_write_one_byte(uint16_t addr, uint8_t data)
{
    uint16_t memadd_size;
    uint16_t device_addr;
    HAL_StatusTypeDef status;

    if (EE_TYPE > AT24C16)
    {
        memadd_size = I2C_MEMADD_SIZE_16BIT;
        device_addr = 0xA0;
    }
    else
    {
        memadd_size = I2C_MEMADD_SIZE_8BIT;
        device_addr = 0xA0 + ((addr >> 8) << 1);
    }

    // 添加重试机制
    for (int retry = 0; retry < EEPROM_MAX_RETRIES; retry++)
    {
        status = HAL_I2C_Mem_Write(&hi2c1,
                                  device_addr,
                                  addr,
                                  memadd_size,
                                  &data,
                                  1,
                                  EEPROM_I2C_TIMEOUT);

        if (status == HAL_OK)
        {
            break;
        }

        // 如果不是最后一次重试，延时后重试
        if (retry < EEPROM_MAX_RETRIES - 1)
        {
            HAL_Delay(5);
        }
    }

    // 等待写操作完成
    HAL_Delay(10);
}

// ... existing code ...

/**
 * @brief       检查AT24CXX是否正常
 *   @note      检测原理: 在器件的末地址写如0X55, 然后再读取, 如果读取值为0X55
 *              则表示检测正常. 否则,则表示检测失败.
 *
 * @param       无
 * @retval      检测结果
 *              0: 检测成功
 *              1: 检测失败
 */
uint8_t at24cxx_check(void)
{
    uint8_t temp;
    uint16_t addr = EE_TYPE;
    temp = at24cxx_read_one_byte(addr);     /* 避免每次开机都写AT24CXX */

    if (temp == 0x55)   /* 读取数据正常 */
    {
        return 0;
    }
    else    /* 排除第一次初始化的情况 */
    {
        at24cxx_write_one_byte(addr, 0x55); /* 先写入数据 */
        HAL_Delay(10);
        temp = at24cxx_read_one_byte(addr);  /* 再读取数据 */

        if (temp == 0x55) return 0;
    }

    return 1;
}


/**
 * @brief       在 AT24CXX 指定地址写入多个数据
 * @param       addr: 写入数据的目的地址
 * @param       data: 要写入的数据数组
 * @param       len: 数据长度
 * @retval      无
 */
void at24cxx_write(uint16_t addr, uint8_t *data, uint16_t len)
{
    uint16_t memadd_size;
    uint16_t device_addr;
    HAL_StatusTypeDef status;

    if (EE_TYPE > AT24C16)
    {
        memadd_size = I2C_MEMADD_SIZE_16BIT;
        device_addr = 0xA0;
    }
    else
    {
        memadd_size = I2C_MEMADD_SIZE_8BIT;
        device_addr = 0xA0 + ((addr >> 8) << 1);
    }

    // 添加重试机制
    for (int retry = 0; retry < EEPROM_MAX_RETRIES; retry++)
    {
        status = HAL_I2C_Mem_Write(&hi2c1,
                                   device_addr,
                                   addr,
                                   memadd_size,
                                   data,
                                   len,
                                   EEPROM_I2C_TIMEOUT);

        if (status == HAL_OK)
        {
            break;
        }

        // 如果不是最后一次重试，延时后重试
        if (retry < EEPROM_MAX_RETRIES - 1)
        {
            HAL_Delay(5);
        }
    }

    // 等待写操作完成
    HAL_Delay(10);
}

/**
 * @brief       在 AT24CXX 指定地址读取多个数据
 * @param       addr: 开始读数的地址
 * @param       data: 用于存储读取数据的缓冲区
 * @param       len: 读取数据的长度
 * @retval      无
 */
void at24cxx_read(uint16_t addr, uint8_t *data, uint16_t len)
{
    uint16_t memadd_size;
    uint16_t device_addr;
    HAL_StatusTypeDef status;

    if (EE_TYPE > AT24C16)
    {
        memadd_size = I2C_MEMADD_SIZE_16BIT;
        device_addr = 0xA0;
    }
    else
    {
        memadd_size = I2C_MEMADD_SIZE_8BIT;
        device_addr = 0xA0 + ((addr >> 8) << 1);
    }

    // 添加重试机制
    for (int retry = 0; retry < EEPROM_MAX_RETRIES; retry++)
    {
        status = HAL_I2C_Mem_Read(&hi2c1,
                                  device_addr,
                                  addr,
                                  memadd_size,
                                  data,
                                  len,
                                  EEPROM_I2C_TIMEOUT);

        if (status == HAL_OK)
        {
            break;
        }

        // 如果不是最后一次重试，延时后重试
        if (retry < EEPROM_MAX_RETRIES - 1)
        {
            HAL_Delay(5);
        }
    }
}
