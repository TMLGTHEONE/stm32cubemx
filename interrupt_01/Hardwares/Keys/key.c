#include "key.h"

/*
@brief  按键按下电平状态
*/
#define KEY_GPIO_PRESS_STATE GPIO_PIN_RESET

/*
 * @brief  消抖时间(ms)
 */
#define KEY_DEBOUNCE_TIME 5

/*
 * @brief  按键状态枚举
 */
typedef enum {
    KEY_IDLE = 0,// 按键空闲
    KEY_DETECTED,// 按键检测到
    KEY_CONFIRMED// 按键确认按下
} Key_StateTypeDef;

/*
 * @brief  按键状态变量和时间戳
 */
static volatile Key_StateTypeDef key_state = KEY_IDLE;// 按键状态变量
static volatile uint32_t key_time_stamp = 0;// 记录按键检测时间戳

/*
 * @brief  按键标志位设置函数(在中断中调用)
 */
void key_set_flag(void)
{
    if (key_state == KEY_IDLE)
    {
        key_state = KEY_DETECTED;
        key_time_stamp = HAL_GetTick();
    }
}

/*
 * @brief  按键扫描函数(非阻塞式)
 * @param  mode 0 非连续 按键松开是一 1 连续 按键按下都是一
 * @retval uint8_t 0 未按下 1 已按下
 */
uint8_t key_scan(uint8_t mode)
{
    // 检测到按键，等待消抖时间
    if (key_state == KEY_DETECTED)
    {
        if ((HAL_GetTick() - key_time_stamp) >= KEY_DEBOUNCE_TIME)
        {
            // 二次判断，排除抖动噪声
            if (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == KEY_GPIO_PRESS_STATE)
            {
                key_state = KEY_CONFIRMED;
            }
            else
            {
                key_state = KEY_IDLE;
            }
        }
    }
    
    // 按键是否松开
    if (key_state == KEY_CONFIRMED && HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) != KEY_GPIO_PRESS_STATE)
    {
        key_state = KEY_IDLE;
        return !mode;
    }
    
    return mode ? (key_state == KEY_CONFIRMED) : 0;
}