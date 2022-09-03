#include "stm32f103xe.h"
#include "flash.h"


void FLASH_unlock(void) 
{
	FLASH->KEYR = FLASH_KEY1;
	FLASH->KEYR = FLASH_KEY2;
}

void FLASH_lock(void) 
{
	FLASH->CR |= FLASH_CR_LOCK;
}

uint32_t FLASH_read(uint32_t address) 
{
	return (*(__IO uint32_t*) address);
}
	
void FLASH_write(uint32_t address, uint32_t data) 
{
	FLASH->CR |= FLASH_CR_PG;
	while(FLASH->SR & FLASH_SR_BSY);
	
	*(__IO uint16_t*)address = (uint16_t)data;
  while(FLASH->SR & FLASH_SR_BSY);
	
	address+=2;
	data>>=16;
	
	*(__IO uint16_t*)address = (uint16_t)data;
  while(FLASH->SR & FLASH_SR_BSY);
	
  FLASH->CR &= ~(FLASH_CR_PG);
}

void FLASH_erase_page(uint32_t address) 
{
	FLASH->CR|= FLASH_CR_PER;
	FLASH->AR = address;
	FLASH->CR|= FLASH_CR_STRT;
	while(FLASH->SR & FLASH_SR_BSY);
	FLASH->CR&= ~FLASH_CR_PER;
}
	
