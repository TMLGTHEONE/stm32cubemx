#include "iic.h"

/**
 * @brief   初始化IIC对应的引脚
 *
 */
void IIC_Init()
{
    // 打开对应的时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();
    // 初始化SCL
    GPIO_InitTypeDef GPIO_Init_Struct = {};
    GPIO_Init_Struct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_Init_Struct.Pull = GPIO_NOPULL;
    GPIO_Init_Struct.Pin = IIC_SCL_PIN;
    GPIO_Init_Struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(IIC_SCL_PORT, &GPIO_Init_Struct);

    // 初始化SDA
    GPIO_Init_Struct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_Init_Struct.Pull = GPIO_NOPULL;
    GPIO_Init_Struct.Pin = IIC_SDA_PIN;
    GPIO_Init_Struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(IIC_SDA_PORT, &GPIO_Init_Struct);

    SDA_Write(1);
    SCL_Write(1);
}

/**
 * @brief   IIC起始信号生成
 *
 */
void IIC_Start()
{
    // SDA 置1
    SDA_Write(1);

    // SCL 置1
    SCL_Write(1);

    // SDA 置0
    SDA_Write(0);

    SCL_Write(0);
}

/**
 * @brief   通过IIC协议发送数据
 *
 * @param   data 一个字节的数据
 */
void IIC_Send_Byte(uint8_t data)
{
    for (uint8_t i = 0; i < 8; i++)
    {

        // 从高位到地位以此选出某一位数据进行传输
        SDA_Write(data & (0x80 >> i) ? 1 : 0);

        SCL_Write(1);

        SCL_Write(0);
    }
    // 松开SDA
    SDA_Write(1);

    // 假装处理应答位
    SCL_Write(1);
    SCL_Write(0);
}

/**
 * @brief   IIC结束信号生成
 *
 */
void IIC_End()
{
    SDA_Write(0);
    SCL_Write(1);

    SDA_Write(1);
}