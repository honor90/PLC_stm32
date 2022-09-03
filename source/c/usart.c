#include "stm32f103xe.h"
#include "usart.h"


uint8_t U1_status;

uint8_t USART1_PACK_in[50];
uint8_t *USART1_PACK_out;

uint8_t U1_len_pack_out=0;
uint8_t U1_len_pack_in=0;
uint8_t U1_j;

uint8_t U3_status;

uint8_t USART3_PACK_in[50];
uint8_t *USART3_PACK_out;

uint8_t U3_len_pack_out=0;
uint8_t U3_len_pack_in=0;
uint8_t U3_j;

uint32_t DR;
uint32_t SR;

void USART1_init(void)
{
	U1_len_pack_in = 0;

  USART1->BRR = (uint16_t)(32000000/9600);
	
  USART1->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE | USART_CR1_UE | USART_CR1_IDLEIE; 
	USART1->CR2=0;
	USART1->CR3=0;	
	
	NVIC_EnableIRQ(USART1_IRQn);
	
	RS485_U1_RSE_OFF();
}


void USART3_init(void)
{
	U3_len_pack_in = 0;

  USART3->BRR = (uint16_t)(32000000/9600);
	
  USART3->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE | USART_CR1_UE | USART_CR1_IDLEIE; 
	USART3->CR2=0;
	USART3->CR3=0;	
	
	NVIC_EnableIRQ(USART3_IRQn);
	
	RS485_U3_RSE_OFF();
}

void USART1_IRQHandler(void)
{	
	if(USART1->SR & USART_SR_RXNE)
	{
		USART1_PACK_in[U1_len_pack_in] = USART1->DR;
		U1_len_pack_in++;	
	} 	
	
	if(USART1->SR & USART_SR_IDLE)
	{
		SR = USART1->SR;
		DR = USART1->DR;
    
		USART1->CR1 &=~ USART_CR1_RXNEIE;
		U1_status = Pack_Is_Received;
	}	

	if(USART1->SR & USART_SR_TXE && USART1->CR1 & USART_CR1_TXEIE)
	{
		USART1->DR = *(USART1_PACK_out+U1_j);
		U1_j++;
		if(U1_j == U1_len_pack_out)
		{
			U1_j=0;
			USART1->CR1 &=~ USART_CR1_TXEIE;
		}				
	}
	
	if(USART1->SR & USART_SR_TC && USART1->CR1 & USART_CR1_TCIE)
	{
		USART1->SR &=~ USART_SR_TC;
		USART1->CR1 &=~ USART_CR1_TCIE;
		USART1->CR1 |= USART_CR1_RXNEIE;
		RS485_U1_RSE_OFF();
		U1_status = Transmit_Complite;
	}
}

void USART3_IRQHandler(void)
{	
	if(USART3->SR & USART_SR_RXNE)
	{
		USART3_PACK_in[U3_len_pack_in] = USART3->DR;
		U3_len_pack_in++;	
	} 	
	
	if(USART3->SR & USART_SR_IDLE)
	{
		SR = USART3->SR;
		DR = USART3->DR;
    
		USART3->CR1 &=~ USART_CR1_RXNEIE;
		U3_status = Pack_Is_Received;
	}

	if(USART3->SR & USART_SR_TXE && USART3->CR1 & USART_CR1_TXEIE)
	{
		USART3->DR = *(USART3_PACK_out+U3_j);
		U3_j++;
		if(U3_j == U3_len_pack_out)
		{
			U3_j=0;
			USART3->CR1 &=~ USART_CR1_TXEIE;
		}				
	}
	
	if(USART3->SR & USART_SR_TC && USART3->CR1 & USART_CR1_TCIE)
	{
		
		USART3->SR &=~ USART_SR_TC;
		USART3->CR1 &=~ USART_CR1_TCIE;
		USART3->CR1 |= USART_CR1_RXNEIE;
		RS485_U3_RSE_OFF();
		U3_status = Transmit_Complite;
	}
}

uint8_t *USART1_get(uint8_t *len_buff)
{
	*len_buff = U1_len_pack_in-1;
	U1_len_pack_in = 0;
	return USART1_PACK_in;
}

uint8_t *USART3_get(uint8_t *len_buff)
{
	*len_buff = U3_len_pack_in-1;
	U3_len_pack_in = 0;
	return USART3_PACK_in;
}

void USART1_send(uint8_t *buff, uint8_t offset, uint8_t len)
{
	USART1_PACK_out = buff;
	U1_j = offset;
	U1_len_pack_out = len;
	
	RS485_U1_RSE_ON();
	USART1->CR1 |= USART_CR1_TXEIE;
	USART1->CR1 &=~ USART_CR1_RXNEIE;
	USART1->CR1 |= USART_CR1_TCIE;
}


void USART3_send(uint8_t *buff, uint8_t offset, uint8_t len)
{
	USART3_PACK_out = buff;
	U3_j = offset;
	U3_len_pack_out = len;
	
	RS485_U3_RSE_ON();
	USART3->CR1 |= USART_CR1_TXEIE;
	USART3->CR1 &=~ USART_CR1_RXNEIE;
	USART3->CR1 |= USART_CR1_TCIE;
}

uint8_t USART1_status(void)
{
	return U1_status;
}

uint8_t USART3_status(void)
{
	return U3_status;
}

void USART1_Clear_status(void)
{
	U1_status = 0;
}

void USART3_Clear_status(void)
{
	U3_status = 0;
}

