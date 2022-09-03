#ifndef ETR_H
#define ETR_H
#include "stm32f103xe.h"

void ETR_init(void);
uint16_t ETR_TIM1_Impulse_get(void);
uint16_t ETR_TIM2_Impulse_get(void);

#endif