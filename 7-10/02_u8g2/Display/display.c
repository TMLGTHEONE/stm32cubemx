#include "display.h"

#define IIC_SCL_PORT GPIOB
#define IIC_SCL_PIN GPIO_PIN_6

#define IIC_SDA_PORT GPIOB
#define IIC_SDA_PIN GPIO_PIN_7

#define SCL_Write(x) HAL_GPIO_WritePin(IIC_SCL_PORT, IIC_SCL_PIN, (GPIO_PinState)x)

#define SDA_Write(x) HAL_GPIO_WritePin(IIC_SDA_PORT, IIC_SDA_PIN, (GPIO_PinState)x)

u8g2_t u8g2;

uint8_t u8x8_gpio_and_delay_stm32f1(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    switch (msg)
    {
    case U8X8_MSG_GPIO_AND_DELAY_INIT: // called once during init phase of u8g2/u8x8

        delay_init(72); // 初始化delay
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

        break;                // can be used to setup pins
    case U8X8_MSG_DELAY_NANO: // delay arg_int * 1 nano second
        __NOP();
        break;
    case U8X8_MSG_DELAY_100NANO: // delay arg_int * 100 nano seconds
        __NOP();
        break;
    case U8X8_MSG_DELAY_10MICRO: // delay arg_int * 10 micro seconds
        delay_us(10 * arg_int);
        break;
    case U8X8_MSG_DELAY_MILLI: // delay arg_int * 1 milli second
        delay_ms(arg_int);
        break;
    case U8X8_MSG_DELAY_I2C: // arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
        if (arg_int == 1)
        {
            delay_us(5);
        }
        if (arg_int == 4)
        {
            delay_us(2);
        }
        break;                    // arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
    case U8X8_MSG_GPIO_I2C_CLOCK: // arg_int=0: Output low at I2C clock pin
        SCL_Write(arg_int);
        break;                   // arg_int=1: Input dir with pullup high for I2C clock pin
    case U8X8_MSG_GPIO_I2C_DATA: // arg_int=0: Output low at I2C data pin
        SDA_Write(arg_int);
        break; // arg_int=1: Input dir with pullup high for I2C data pin
    case U8X8_MSG_GPIO_MENU_SELECT:
        u8x8_SetGPIOResult(u8x8, /* get menu select pin state */ 0);
        break;
    case U8X8_MSG_GPIO_MENU_NEXT:
        u8x8_SetGPIOResult(u8x8, /* get menu next pin state */ 0);
        break;
    case U8X8_MSG_GPIO_MENU_PREV:
        u8x8_SetGPIOResult(u8x8, /* get menu prev pin state */ 0);
        break;
    case U8X8_MSG_GPIO_MENU_HOME:
        u8x8_SetGPIOResult(u8x8, /* get menu home pin state */ 0);
        break;
    default:
        u8x8_SetGPIOResult(u8x8, 1); // default return value
        break;
    }
    return 1;
}

void u8g2_init()
{
    u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_stm32f1);
    u8g2_InitDisplay(&u8g2);     // send init sequence to the display, display is in sleep mode after this,
    u8g2_SetPowerSave(&u8g2, 0); // wake up display
}