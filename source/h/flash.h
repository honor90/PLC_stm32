#ifndef FLASH_H
#define FLASH_H
#include "stm32f103xe.h"

void FLASH_unlock(void);
void FLASH_lock(void);
uint32_t FLASH_read(uint32_t address);
void FLASH_write(uint32_t address,uint32_t data);
void FLASH_erase_page(uint32_t address);

#endif

