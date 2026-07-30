#ifndef __IIC_H
#define __IIC_H

#include "main.h"

#define IIC_SCL_PORT GPIOB
#define IIC_SCL_PIN  GPIO_PIN_6

#define IIC_SDA_PORT GPIOB
#define IIC_SDA_PIN  GPIO_PIN_7

#define SCL_Write(x)     HAL_GPIO_WritePin(IIC_SCL_PORT, IIC_SCL_PIN, (GPIO_PinState)x)

#define SDA_Write(x)      HAL_GPIO_WritePin(IIC_SDA_PORT, IIC_SDA_PIN, (GPIO_PinState)x)

void IIC_Init();
void IIC_Start();
void IIC_Send_Byte(uint8_t data);
void IIC_End();

#endif