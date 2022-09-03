#include "stm32f103xe.h"
#include "adc.h"

void ADC_init(void)
{
	ADC1->CR1 = 0;
	ADC1->CR2 = 0;
	ADC1->SMPR2 = 0;
	ADC1->SQR1 = 0;
	ADC1->SQR2 = 0;
	ADC1->SQR3 = 0;
	
	ADC1->CR2 |= ADC_CR2_ADON;
	
	ADC1->CR2 |= ADC_CR2_RSTCAL;
	while ((ADC1->CR2 & ADC_CR2_RSTCAL) == ADC_CR2_RSTCAL){}
	
	ADC1->CR2 |= ADC_CR2_CAL;
	while ((ADC1->CR2 & ADC_CR2_RSTCAL) == ADC_CR2_CAL){}


	//ADC1->SMPR2 |= (ADC_SMPR2_SMP0_2 | ADC_SMPR2_SMP0_1 | ADC_SMPR2_SMP0_0); 
  //ADC1->SMPR2 |= (ADC_SMPR2_SMP1_2 | ADC_SMPR2_SMP1_1 | ADC_SMPR2_SMP1_0); 
	
	ADC1->SMPR2 |= ADC_SMPR2_SMP0_0 | ADC_SMPR2_SMP0_2 ; // 55.5 cycles
	ADC1->SMPR2 &= ~ADC_SMPR2_SMP0_1 ;
	ADC1->SMPR2 |= ADC_SMPR2_SMP1_0 | ADC_SMPR2_SMP1_2 ; // 55.5 cycles
	ADC1->SMPR2 &= ~ADC_SMPR2_SMP1_1;
	
		
	ADC1->CR1 |= ADC_CR1_DISCNUM_0;	
	ADC1->CR1 |= ADC_CR1_SCAN;	
		
	
  ADC1->CR2 |= ADC_CR2_EXTSEL;
                                                   
  ADC1->CR2 |= ADC_CR2_EXTTRIG;  
	
  ADC1->CR2 |= ADC_CR2_DMA | ADC_CR2_CONT; 

	
	ADC1->SQR1 |= ADC_SQR1_L_0;
	
	ADC1->SQR3 |= ADC_SQR3_SQ2_0;

  ADC1->CR2 |= ADC_CR2_SWSTART;
}

void ADC_convert_U(uint32_t *Uadc, uint16_t ADC_value)
{
	float var = 0;
	
	var = ((ADC_value*3.3)/4095);
	*Uadc = *(uint32_t *)&var;
}

void ADC_calculation_R(uint32_t *R2, float Uadc)
{
	float var = 0;
	
	var = 300/((Uin/Uadc)-4);
	*R2 = *(uint32_t *)&var;
}
