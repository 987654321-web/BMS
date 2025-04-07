#ifndef SOLF_I2C_H
#define SOLF_I2C_H

#include <stdint.h>
#include <stdbool.h>

// I2C 起始信号
void I2C_Start(void);

// I2C 停止信号
void I2C_Stop(void);

// I2C 发送一个字节
bool I2C_SendByte(uint8_t data);

// I2C 接收一个字节
uint8_t I2C_ReceiveByte(bool ack);

#endif