#ifndef __OLED_MODE_H__
#define __OLED_MODE_H__

#include "main.h"

/* 光照值 */
#define TH_BRIGHT  800
#define TH_MID     1500
#define TH_DARK    2500

/* LED亮度水平（CCR值） */
#define LEVEL_OFF   0U
#define LEVEL_LOW   2000U
#define LEVEL_MID   10000U
#define LEVEL_HIGH  20000U

/* 模式变量（在oled_mode. c中定义） */
extern uint8_t current_mode; /* 0 = manual, 1 = auto */
extern uint8_t manual_level;
extern uint8_t auto_level; /* 当前自动确定的级别，用于显示 */

/* APIs */
uint8_t Get_LightLevel(void);
void Set_LED_Level(uint8_t level);

#endif /* __OLED_MODE_H__ */
