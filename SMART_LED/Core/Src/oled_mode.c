#include "oled_mode.h"
#include "adc.h"
#include "tim.h"

/* Mode/level storage */
uint8_t current_mode = 0;  /* 0 = manual 手动, 1 = auto 自动*/
uint8_t manual_level = 0;  /* manual set level 手动设置*/
uint8_t auto_level = 0;    /* auto-detected level 自动检测*/

uint8_t Get_LightLevel(void)
{
    uint32_t adc_val = 0;
    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK) {
        adc_val = HAL_ADC_GetValue(&hadc1);
    }

    if (adc_val < TH_BRIGHT)      return 0; // high
    else if (adc_val < TH_MID)    return 1; // mid
    else if (adc_val < TH_DARK)   return 2; // low
    else                          return 3; // dark
}

void Set_LED_Level(uint8_t level)
{
    uint32_t ccr = 0;
    switch(level) {
        case 0: ccr = LEVEL_OFF; break;
        case 1: ccr = LEVEL_LOW; break;
        case 2: ccr = LEVEL_MID; break;
        case 3: ccr = LEVEL_HIGH; break;
        default: return;
    }

    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, ccr);
}
