#include "stm32f103xe.h"
#include "etr.h"

uint16_t TIM1_impulse = 0;
uint16_t TIM2_impulse = 0;

void ETR_init(void)
{
	
	TIM1->PSC = 1 - 1;
	TIM1->CNT = 0;
	TIM1->ARR = 60000 - 1;	
	TIM1->CR1 &= ~TIM_CR1_DIR;
	
  TIM1->SMCR &= ~TIM_SMCR_ETF; 
	TIM1->SMCR &= ~TIM_SMCR_ETPS;
	TIM1->SMCR &= ~TIM_SMCR_ETP;
	TIM1->SMCR |= TIM_SMCR_ECE; 
	

	TIM2->PSC = 1 - 1;
	TIM2->CNT = 0;
	TIM2->ARR = 60000 - 1;	
	TIM2->CR1 &= ~TIM_CR1_DIR;
	
	TIM2->SMCR &= ~TIM_SMCR_ETF;
	TIM2->SMCR &= ~TIM_SMCR_ETPS;
	TIM2->SMCR &= ~TIM_SMCR_ETP;
	TIM2->SMCR |= TIM_SMCR_ECE; 	


  TIM4->PSC = 32000-1;
	TIM4->CNT = 0;
  TIM4->ARR = 1000;	
	TIM4->DIER |= TIM_DIER_UIE;
	
	NVIC_EnableIRQ(TIM4_IRQn);
	
	TIM1->CR1 |= TIM_CR1_CEN;
	TIM2->CR1 |= TIM_CR1_CEN;
	TIM4->CR1 |= TIM_CR1_CEN;
}

void TIM4_IRQHandler(void)
{
	TIM1->CR1 &= ~TIM_CR1_CEN;
	TIM2->CR1 &= ~TIM_CR1_CEN;
	
	TIM1_impulse = TIM1->CNT;
	TIM2_impulse = TIM2->CNT;
	
	TIM1->CNT = 0;
	TIM2->CNT = 0;
	TIM4->CNT = 0;
	TIM4->SR &= ~TIM_SR_UIF; 
	
	TIM1->CR1 |= TIM_CR1_CEN;
	TIM2->CR1 |= TIM_CR1_CEN;
	TIM4->CR1 |= TIM_CR1_CEN;
}	

uint16_t ETR_TIM1_Impulse_get(void)
{
	return TIM1_impulse;
}

uint16_t ETR_TIM2_Impulse_get(void)
{
	return TIM2_impulse;
}
