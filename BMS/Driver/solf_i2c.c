#include "solf_i2c.h"
#include <stdint.h>
#include <stdbool.h>
#include <main.h>
// 定义 I2C 引脚操作的宏
#define SDA_HIGH() HAL_GPIO_WritePin(I2C_SDA_GPIO_Port, I2C_SDA_Pin, GPIO_PIN_SET)  // 设置 SDA 引脚为高电平
#define SDA_LOW() HAL_GPIO_WritePin(I2C_SDA_GPIO_Port, I2C_SDA_Pin, GPIO_PIN_RESET) // 设置 SDA 引脚为低电平
#define SCL_HIGH() HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin, GPIO_PIN_SET)  // 设置 SCL 引脚为高电平
#define SCL_LOW() HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin, GPIO_PIN_RESET) // 设置 SCL 引脚为低电平
#define SDA_READ() HAL_GPIO_ReadPin(I2C_SDA_GPIO_Port, I2C_SDA_Pin)                 // 读取 SDA 引脚的电平

// 延时函数（根据具体硬件调整）
static void I2C_Delay(void)
{
    for (volatile int i = 0; i < 100; i++)
        ;
}

// I2C 起始信号
void I2C_Start(void)
{
    SDA_HIGH();
    SCL_HIGH();
    I2C_Delay();
    SDA_LOW();
    I2C_Delay();
    SCL_LOW();
}

// I2C 停止信号
void I2C_Stop(void)
{
    SDA_LOW();
    SCL_HIGH();
    I2C_Delay();
    SDA_HIGH();
    I2C_Delay();
}

// I2C 发送一个字节
bool I2C_SendByte(uint8_t data)
{
    for (int i = 0; i < 8; i++)
    {
        if (data & 0x80)
        {
            SDA_HIGH();
        }
        else
        {
            SDA_LOW();
        }
        data <<= 1;
        I2C_Delay();
        SCL_HIGH();
        I2C_Delay();
        SCL_LOW();
    }

    // 检查应答信号
    SDA_HIGH(); // 释放 SDA
    I2C_Delay();
    SCL_HIGH();
    bool ack = !SDA_READ(); // 读取应答信号
    I2C_Delay();
    SCL_LOW();
    return ack;
}

// I2C 接收一个字节
uint8_t I2C_ReceiveByte(bool ack)
{
    uint8_t data = 0;
    SDA_HIGH(); // 释放 SDA
    for (int i = 0; i < 8; i++)
    {
        data <<= 1;
        SCL_HIGH();
        I2C_Delay();
        if (SDA_READ())
        {
            data |= 0x01;
        }
        SCL_LOW();
        I2C_Delay();
    }

    // 发送应答信号
    if (ack)
    {
        SDA_LOW();
    }
    else
    {
        SDA_HIGH();
    }
    I2C_Delay();
    SCL_HIGH();
    I2C_Delay();
    SCL_LOW();
    SDA_HIGH(); // 释放 SDA
    return data;
}
