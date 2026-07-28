#include "key.h"

// 按键引脚定义
#define KEY1_PIN   GPIO_PIN_4
#define KEY1_PORT  GPIOB

// 时间阈值（单位：ms）
#define DEBOUNCE_TIME   20     // 防抖延时
#define LONG_PRESS_TIME 1000   // 长按判定时间（1秒）

typedef enum {
    KEY_IDLE = 0,      // 空闲
    KEY_PRESSED,       // 已按下，未稳定
    KEY_DEBOUNCE,      // 防抖等待
    KEY_HOLD           // 长按已触发
} KeyState_t;

typedef struct {
    KeyState_t state;
    uint32_t press_time;      // 按下时刻（ms）
    uint8_t short_press_flag; // 短按事件标志
    uint8_t long_press_flag;  // 长按事件标志
} Key_t;

Key_t key1 = {0};


// 获取当前按键电平（按下返回0，释放返回1）
uint8_t KEY_GetLevel(void)
{
    return HAL_GPIO_ReadPin(KEY1_PORT, KEY1_PIN);
}

// 按键扫描函数，需要在主循环中周期性调用（建议每10ms调用一次）
void KEY_Scan(void)
{
    uint32_t now = HAL_GetTick();
    uint8_t level = KEY_GetLevel();
    
    switch(key1.state)
    {
        case KEY_IDLE:
            if(level == 0) { // 检测到按下
                key1.state = KEY_PRESSED;
                key1.press_time = now;
            }
            break;
              
        case KEY_PRESSED:
            if(level == 0) {
                // 仍按住，检查是否达到长按时间
                if(now - key1.press_time >= LONG_PRESS_TIME) {
                    key1.long_press_flag = 1;
                    key1.state = KEY_HOLD; // 防止多次触发长按
                }
            } else { // 松开了
                // 防抖：如果按下时间太短（小于防抖时间），认为是噪声
                if(now - key1.press_time >= DEBOUNCE_TIME) {
                    key1.short_press_flag = 1; // 有效短按
                }
                key1.state = KEY_IDLE;
            }
            break;
            
        case KEY_HOLD:
            // 一直按住时，等待释放
            if(level == 1) {
                key1.state = KEY_IDLE; // 释放后回到空闲
            }
            break;
            
        default:
            key1.state = KEY_IDLE;
            break;
    }
}

// 获取短按事件（并清除标志）
uint8_t KEY_GetShortPress(void)
{
    if(key1.short_press_flag) {
        key1.short_press_flag = 0;
        return 1;
    }
    return 0;
}

// 获取长按事件（并清除标志）
uint8_t KEY_GetLongPress(void)
{
    if(key1.long_press_flag) {
        key1.long_press_flag = 0;
        return 1;
    }
    return 0;
}