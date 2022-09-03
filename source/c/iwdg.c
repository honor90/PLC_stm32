#include "stm32f103xe.h"
#include "iwdg.h"

void IWDG_init(void)
{
	IWDG->KR = 0x5555;
	IWDG->PR = 0;
	IWDG->RLR = 0xFFF;	
	IWDG->KR = 0xCCCC; //start
}

void IWDG_reset(void)
{
	IWDG->KR = 0xAAAA;
}
