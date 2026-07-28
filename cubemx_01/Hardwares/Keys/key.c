#include "key.h"
/*
@brief  按键按下电平状态
*/
#define KEY_GPIO_STATE GPIO_PIN_RESET
/*
 * @brief  按键扫描函数
 * @param  mode 0 非连续 按键松开是一 1 连续 按键按下都是一
 * @retval uint8_t 0 未按下 1 已按下
 */
uint8_t key_scan(uint8_t mode)
{
    static uint8_t key_state = 0;
    if (!key_state)
    { // 第一段波动消抖只执行一次
        if (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == KEY_GPIO_STATE)
        {
            HAL_Delay(5);
            // 二次判断，排除高电平抖动噪声
            if (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == KEY_GPIO_STATE)
                key_state = 1;
        }
    }
    // 摁键是否松开
    if (key_state && HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) != KEY_GPIO_STATE)
    {
        key_state = 0;
        return !mode;
    }
    return mode ? key_state : 0;
}