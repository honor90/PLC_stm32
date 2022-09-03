#ifndef USART_H
#define USART_H
#include "stm32f103xe.h"

#define Transmit_Complite 0x01
#define Pack_Is_Received  0x02

void USART1_init(void);
uint8_t *USART1_get(uint8_t *len_buff);
uint8_t USART1_status(void);
void USART1_send(uint8_t *buff, uint8_t offset, uint8_t len);
uint8_t USART1_status(void);
void USART1_Clear_status(void);

void USART3_init(void);
uint8_t *USART3_get(uint8_t *len_buff);
uint8_t USART3_status(void);
void USART3_send(uint8_t *buff, uint8_t offset, uint8_t len);
uint8_t USART3_status(void);
void USART3_Clear_status(void);

#define RS485_U1_RSE_ON()  GPIOB->BSRR |= GPIO_BSRR_BS5
#define RS485_U1_RSE_OFF() GPIOB->BRR |= GPIO_BRR_BR5

#define RS485_U3_RSE_ON()  GPIOB->BSRR |= GPIO_BSRR_BS4
#define RS485_U3_RSE_OFF() GPIOB->BRR |= GPIO_BRR_BR4

#endif