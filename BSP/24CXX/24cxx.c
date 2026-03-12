/**
 ****************************************************************************************************
 * @file        24cxx.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-23
 * @brief       24CXX 驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32F407开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20211023
 * 第一次发布
 *
 ****************************************************************************************************
 */

#include "24cxx.h"
#include "main.h"
#include "i2c.h"


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
    
    HAL_I2C_Mem_Read(&hi2c1, 
                     device_addr, 
                     addr, 
                     memadd_size, 
                     &temp, 
                     1, 
                     100);
    
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
    
    HAL_I2C_Mem_Write(&hi2c1, 
                      device_addr, 
                      addr, 
                      memadd_size, 
                      &data, 
                      1, 
                      100);
    
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
        temp = at24cxx_read_one_byte(255);  /* 再读取数据 */

        if (temp == 0x55)return 0;
    }

    return 1;
}

/**
 * @brief       在AT24CXX里面的指定地址开始读出指定个数的数据
 * @param       addr    : 开始读出的地址 对24c02为0~255
 * @param       pbuf    : 数据数组首地址
 * @param       datalen : 要读出数据的个数
 * @retval      无
 */
void at24cxx_read(uint16_t addr, uint8_t *pbuf, uint16_t datalen)
{
    while (datalen--)
    {
        *pbuf++ = at24cxx_read_one_byte(addr++);
    }
}

/**
 * @brief       在AT24CXX里面的指定地址开始写入指定个数的数据
 * @param       addr    : 开始写入的地址 对24c02为0~255
 * @param       pbuf    : 数据数组首地址
 * @param       datalen : 要写入数据的个数
 * @retval      无
 */
void at24cxx_write(uint16_t addr, uint8_t *pbuf, uint16_t datalen)
{
    while (datalen--)
    {
        at24cxx_write_one_byte(addr, *pbuf);
        addr++;
        pbuf++;
    }
}






