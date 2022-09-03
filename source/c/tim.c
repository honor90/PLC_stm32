#include "stm32f103xe.h"
#include "tim.h"

void TIM3_init(void)
{
	TIM3->PSC = 32000 - 1;
	TIM3->CNT = 0;
  TIM3->ARR = 100;	
	
	TIM3->DIER |= TIM_DIER_UIE;
	//TIM3->CR1  |= TIM_CR1_CEN;
	
	NVIC_EnableIRQ(TIM3_IRQn);
}


