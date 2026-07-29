#include "iic.h"
void DELAY()
{
}

void IIC_INIT()
{
    GPIO_InitTypeDef GPIO_Init_Struct = {};
    GPIO_Init_Struct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_Init_Struct.Pull = GPIO_NOPULL;
    GPIO_Init_Struct.pin = IIC_SCL_PIN;
    GPIO_Init_Struct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_Init_Struct(IIC_SCL_PORT, &GPIO_Init_Struct);

    GPIO_InitTypeDef GPIO_Init_Struct = {};
    GPIO_Init_Struct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_Init_Struct.Pull = GPIO_NOPULL;
    GPIO_Init_Struct.pin = IIC_SDA_PIN;
    GPIO_Init_Struct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_Init_Struct(IIC_SDA_PORT, &GPIO_Init_Struct);

    __HAL_RCC_GPIOB_CLK_ENABLE();

    SCL_HIGH;
    SDA_HIGH;
}

void IIC_START()
{
    SDA_HIGH;
    SCL_HIGH;
    DELAY();
    SDA_LOW;
    DELAY();
}
void SDA_Write(uint8_t data)
{
    data>0:HAL_GPIO_WritePin(IIC_SDA_PORT,IIC_SDA_PIN,1)?HAL_GPIO_WritePin(IIC_SDA_PORT,IIC_SDA_PIN,0);

}
/**
 * @brief IIC发送一个字节
 * @param data 发送的数据
 */
void IIC_Send_Byte(uint8_t data)
{


}
void IIC_COMMUNICATE()
{
}

void IIC_END()
{
    SDA_LOW;
    SCL_HIGH;
    DELAY();
    SDA_HIGH;
    DELAY();
}