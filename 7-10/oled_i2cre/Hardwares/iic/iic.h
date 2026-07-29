#ifndef __IIC_H__
#define __IIC_H__
#include "main.h"

#define IIC_SCL_PORT GPIOB
#define IIC_SCL_PIN  GPIO_PIN_6

#define IIC_SDA_PORT GPIOB
#define IIC_SDA_PIN  GPIO_PIN_7

#define SCL_HIGH HAL_GPIO_WritePin(IIC_SCL_PORT,IIC_SCL_PIN,1)
#define SCL_LOW HAL_GPIO_WritePin(IIC_SCL_PORT,IIC_SCL_PIN,0)

#define SDA_HIGH HAL_GPIO_WritePin(IIC_SDA_PORT,IIC_SDA_PIN,1)
#define SDA_LOW HAL_GPIO_WritePin(IIC_SDA_PORT,IIC_SDA_PIN,0)

void IIC_INIT(void);
void IIC_START(void);
void IIC_END(void);
void IIC_Send_Byte(uint8_t data);

#endif