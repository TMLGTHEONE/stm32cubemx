#include "key.h"

/*
@brief  按键按下电平状态
*/
#define KEY_GPIO_PRESS_STATE GPIO_PIN_RESET

/*
 * @brief  消抖采样次数(定时器中断周期建议为1ms，消抖时间=KEY_DEBOUNCE_COUNT*1ms)
 */
#define KEY_DEBOUNCE_COUNT 5

/*
 * @brief  按键状态枚举
 */
typedef enum {
    KEY_IDLE = 0,// 按键空闲
    KEY_DETECTED,// 按键检测到
    KEY_CONFIRMED// 按键确认按下
} Key_StateTypeDef;

/*
 * @brief  按键状态变量和消抖计数器
 */
static volatile Key_StateTypeDef key_state = KEY_IDLE;// 按键状态变量
static volatile uint8_t key_debounce_count = 0;// 消抖计数器
static volatile uint8_t key_press_flag = 0;// 按键按下标志
 // 读取当前按键状态
static uint8_t current_state = 0;
/*
 * @brief  按键定时器中断扫描函数(在TIM中断中调用)
 */
void key_timer_scan(void)
{
    // 读取当前按键状态
current_state = (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == KEY_GPIO_PRESS_STATE);
    
    switch (key_state)
    {
        case KEY_IDLE:
            // 检测到按键按下
            if (current_state)
            {
                key_state = KEY_DETECTED;
                key_debounce_count = 0;
            }
            break;
            
        case KEY_DETECTED:
            // 连续检测到按键按下
            if (current_state)
            {
                key_debounce_count++;
                // 消抖完成，确认按键按下
                if (key_debounce_count >= KEY_DEBOUNCE_COUNT)
                {
                    key_state = KEY_CONFIRMED;
                    key_press_flag = 1;
                }
            }
             else
             {
                 // 抖动，回到空闲状态
                 key_state = KEY_IDLE;
             }
            break;
            
        case KEY_CONFIRMED:
            // 检测按键松开
            if (!current_state)
            {
                key_state = KEY_IDLE;
            }
            break;
            
        default:
            key_state = KEY_IDLE;
            break;
    }
}

/*
 * @brief  获取按键按下标志(单次触发)
 * @retval uint8_t 0 未按下 1 已按下
 */
uint8_t key_get_press(void)
{
    if (key_press_flag)
    {
        key_press_flag = 0;
        return 1;
    }
    return 0;
}

/*
 * @brief  获取按键当前状态
 * @retval uint8_t 0 未按下 1 已按下
 */
uint8_t key_get_state(void)
{
    return (key_state == KEY_CONFIRMED) ? 1 : 0;
}

/*
 * @brief  按键扫描函数(兼容原有接口，非阻塞式)
 * @param  mode 0 非连续 按键松开是一 1 连续 按键按下都是一
 * @retval uint8_t 0 未按下 1 已按下
 */
uint8_t key_scan(uint8_t mode)
{
    if (mode == 0)
    {
        // 非连续模式：单次按键触发
        return key_get_press();
    }
    else
    {
        // 连续模式：按键按下期间一直返回1
        return key_get_state();
    }
}