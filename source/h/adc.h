#ifndef ADC_H
#define ADC_H
#include "stm32f103xe.h"

#define R1 300
#define Uin 5

void ADC_init(void);
void ADC_convert_U(uint32_t *Uadc, uint16_t ADC_value);
void ADC_calculation_R(uint32_t *R, float Uadc);

#endif