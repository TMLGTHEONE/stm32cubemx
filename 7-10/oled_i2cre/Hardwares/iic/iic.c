#include "iic.h"



void IIC_INIT(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_Init_Struct = {0};
    GPIO_Init_Struct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_Init_Struct.Pull = GPIO_NOPULL;
    GPIO_Init_Struct.Pin = IIC_SCL_PIN;
    GPIO_Init_Struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(IIC_SCL_PORT, &GPIO_Init_Struct);

    GPIO_Init_Struct.Pin = IIC_SDA_PIN;
    HAL_GPIO_Init(IIC_SDA_PORT, &GPIO_Init_Struct);

    SCL_HIGH;
    SDA_HIGH;
}

void IIC_START(void)
{
    SDA_HIGH;
    SCL_HIGH;
    SDA_LOW;
    SCL_LOW;
}

void SDA_Write(uint8_t data)
{
    if (data > 0)
    {
        HAL_GPIO_WritePin(IIC_SDA_PORT, IIC_SDA_PIN, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(IIC_SDA_PORT, IIC_SDA_PIN, GPIO_PIN_RESET);
    }
}

/**
 * @brief IIC发送一个字节
 * @param data 发送的数据
 */
void IIC_Send_Byte(uint8_t data)
{
    for (int i = 0; i < 8; i++)
    {
        uint8_t result = data << i;
        result >>= 7;
        SDA_Write(result);
        SCL_HIGH;
        SCL_LOW;
    }
    // 释放 SDA
    SDA_HIGH;
    // 应答位
    SCL_LOW;
    SCL_HIGH;
  
}


void IIC_END(void)
{
    SDA_LOW;
    SCL_HIGH;
    IIC_Delay();
    SDA_HIGH;
    IIC_Delay();
}