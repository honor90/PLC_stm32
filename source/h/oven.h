#ifndef OVEN_H
#define OVEN_H
#include "stm32f103xe.h"


#define OVEN_Pack_Is_Received  0x02


void OVEN_init(void);
void OVEN_Heandler_Master(uint8_t *buff_in, uint8_t lenght_buff_in);
void OVEN_Send_Pack_DSPD(uint8_t *buff, uint8_t addr);
uint8_t OVEN_Convert_ASCII_byte(uint8_t ch);
float OVEN_Convert_DEC_dot0(uint32_t data);
uint16_t OVEN_CRC16(uint8_t* packet, uint8_t length);
uint8_t OVEN_Status(void);
void OVEN_Clear_Status(void);
uint8_t OVEN_Get_Adress_SI8(void);
float OVEN_Get_FlowMeter(void);
uint8_t OVEN_Return_Result(void);


#endif