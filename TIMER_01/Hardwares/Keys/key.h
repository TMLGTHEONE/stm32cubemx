#ifndef __KEY_H
#define __KEY_H
#include "main.h"
uint8_t key_scan(uint8_t mode);
void key_timer_scan(void);
uint8_t key_get_press(void);
uint8_t key_get_state(void);
#endif /* __KEY_H */