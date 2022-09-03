#include "stm32f103xe.h"
#include "gpio.h"

void GPIO_init(void)
{

	GPIOA->CRL = 0;
	GPIOA->CRH = 0;	
	GPIOB->CRL = 0;
	GPIOB->CRH = 0;
	GPIOC->CRL = 0;
	GPIOC->CRH = 0;	
	GPIOD->CRL = 0;
	GPIOD->CRH = 0;


	//USART1
	//(PA9) TX
	GPIOA->CRH &= ~GPIO_CRH_CNF9;	
	GPIOA->CRH |=  GPIO_CRH_CNF9_1;	// Set CNF bit 2 to 10 - AFIO Push-Pull
	GPIOA->CRH |=  GPIO_CRH_MODE9_0;	// Set MODE bit 2 to Mode 01 = 10MHz 

	//(PA10) RX
  GPIOA->CRH |=  GPIO_CRH_CNF10_0;// Set CNF bit 3 to 01 HiZ
  GPIOA->CRH &= ~GPIO_CRH_CNF10_1;
  GPIOA->CRH &= ~GPIO_CRH_MODE10;// Set MODE bit 3 to Mode 01 = 10MHz 
	
	//(PB5) RS485_RSE
	GPIOB->CRL |= GPIO_CRL_MODE5; //Output mode, max speed 50 MHz.		
	
	
	
	//USART3
	//(PB10) TX
	GPIOB->CRH &= ~GPIO_CRH_CNF10;	
	GPIOB->CRH |=  GPIO_CRH_CNF10_1;	// Set CNF bit 2 to 10 - AFIO Push-Pull
	GPIOB->CRH |=  GPIO_CRH_MODE10_0;	// Set MODE bit 2 to Mode 01 = 10MHz 

	//(PB11) RX
  GPIOB->CRH |=  GPIO_CRH_CNF11_0;// Set CNF bit 3 to 01 HiZ
  GPIOB->CRH &= ~GPIO_CRH_CNF11_1;
  GPIOB->CRH &= ~GPIO_CRH_MODE11;// Set MODE bit 3 to Mode 01 = 10MHz 
	
	//(PB4) RS485_RSE
	GPIOB->CRL |= GPIO_CRL_MODE4; //Output mode, max speed 50 MHz.	
	
	

	//(PD2) Button Left
	GPIOD->CRL |= GPIO_CRL_CNF2_1;
	GPIOD->BRR |= GPIO_BRR_BR2;
	
	//(PA4) Button Right
	GPIOA->CRL |= GPIO_CRL_CNF4_1;
	GPIOA->BRR |= GPIO_BRR_BR4;
	
	//(PC6) Sensor Smoke Left
	GPIOC->CRL |= GPIO_CRL_CNF6_1;
	GPIOC->BRR |= GPIO_BRR_BR6;		
	
	//(PC8) Sensor Smoke right
	GPIOC->CRH |= GPIO_CRH_CNF8_1;
	GPIOC->BRR |= GPIO_BRR_BR8;		
	
	


	//(PB8) RELE_Left
	GPIOB->CRH |= GPIO_CRH_MODE8; //Output mode, max speed 50 MHz.
	
	//(PB9) RELE_Right
	GPIOB->CRH |= GPIO_CRH_MODE9; //Output mode, max speed 50 MHz.
	
	
	
	//(PC10) LED_NORM_Left
	GPIOC->CRH |= GPIO_CRH_MODE10; //Output mode, max speed 50 MHz.
	
	//(PC12) LED_ALARM_Fire_Left
	GPIOC->CRH |= GPIO_CRH_MODE12; //Output mode, max speed 50 MHz.
	
	//(PA14) LED_ALARM_Temperature_Left
	GPIOA->CRH |= GPIO_CRH_MODE14; //Output mode, max speed 50 MHz.
	
	//(PC11) LED_ALARM_Destruction_Left
	GPIOC->CRH |= GPIO_CRH_MODE11; //Output mode, max speed 50 MHz.
	
	//(PA13) LED_ALARM_Vibration_Left
	GPIOA->CRH |= GPIO_CRH_MODE13; //Output mode, max speed 50 MHz.
	
	
	
	//(PB7) LED_NORM_Right
	GPIOB->CRL |= GPIO_CRL_MODE7; //Output mode, max speed 50 MHz.	
	
	//(PB0) LED_ALARM_Fire_Right
	GPIOB->CRL |= GPIO_CRL_MODE0; //Output mode, max speed 50 MHz.
	
	//(PC3) ED_ALARM_Temperature_Right
	GPIOC->CRL |= GPIO_CRL_MODE3; //Output mode, max speed 50 MHz.	
	
	//(PC13) LED_ALARM_Destruction_Right
	GPIOC->CRH |= GPIO_CRH_MODE13; //Output mode, max speed 50 MHz.
	
	//(PC2) LED_ALARM_Vibration_Right
	GPIOC->CRL |= GPIO_CRL_MODE2; //Output mode, max speed 50 MHz.	
	
	
	
	//LED2 (PA5)
	GPIOA->CRL |= GPIO_CRL_MODE5; //Output mode, max speed 50 MHz.
	
	
	
	//ADC1
	//GPIOA->CRL &= ~ (GPIO_CRL_MODE0 | GPIO_CRL_CNF0); // PA0 - analod mode
	//GPIOA->CRL &= ~ (GPIO_CRL_MODE1 | GPIO_CRL_CNF1); // PA1 - analod mode
	
	GPIOA->CRL |=  GPIO_CRL_CNF0_1; // PA0 - pull down
	GPIOA->CRL |=  GPIO_CRL_CNF1_1; // PA1 - pull down
}