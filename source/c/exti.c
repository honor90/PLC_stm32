#include "stm32f103xe.h"
#include "exti.h"

void EXTI_init(void)
{
	//Button Left
	EXTI->RTSR |= EXTI_RTSR_TR2; 
  EXTI->FTSR |= EXTI_FTSR_TR2; 	
	
	//Button Right
	EXTI->RTSR |= EXTI_RTSR_TR4; 
  EXTI->FTSR |= EXTI_FTSR_TR4; 	
	
	
	EXTI->PR |= EXTI_PR_PR2 | EXTI_PR_PR4 ;		
	
	EXTI->IMR |= EXTI_IMR_MR2 | EXTI_IMR_MR4 ; //On interup	

	NVIC_EnableIRQ(EXTI2_IRQn); 
	NVIC_EnableIRQ(EXTI4_IRQn); 
}
