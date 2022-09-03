#include "stm32f103xe.h"
#include "oven.h"
#include "math.h"



uint8_t addr_si8;
uint8_t status_si8;
uint32_t data;
uint16_t crc;
uint8_t oven_crc_result = 0;
float flow_meter = 0;


void OVEN_init(void)
{

}

uint16_t c;
uint8_t v[10];
void OVEN_Heandler_Master(uint8_t *buff_in, const uint8_t lenght_buff_in)
{
	uint8_t count = (lenght_buff_in-1)/2;
	uint8_t k = 0;
	uint8_t j = 1;
	
	if(*buff_in == '#')
	{
		if(*(buff_in+lenght_buff_in) == 0x0D)
		{
			for(k = 0; k < 25; k++)
			{
				v[k] = (OVEN_Convert_ASCII_byte(*(buff_in+j))<<4) | ((OVEN_Convert_ASCII_byte(*(buff_in+j+1)))&0xF);
				j+=2;
			}
			crc = (v[8]<<8)|v[9];
			c = OVEN_CRC16(v, 8);
			if(c == crc)
			{
				oven_crc_result = 1;
				status_si8 = OVEN_Pack_Is_Received;
				addr_si8 = v[0];
				data = (v[4]<<24)|(v[5]<<16)|(v[6]<<8)|v[7];
				flow_meter = OVEN_Convert_DEC_dot0(data);
			}	
			else oven_crc_result = 0;
		}
	}
}


void OVEN_Send_Pack_DSPD(uint8_t *buff, uint8_t addr)
{
	switch(addr)
	{
		case 1:
			// #GHHGOVSITLVL\r
			*buff 		= 0x23;
			*(buff+1) = 0x47;
			*(buff+2) = 0x48;
			*(buff+3) = 0x48;
			*(buff+4) = 0x47;
			*(buff+5) = 0x4F;
			*(buff+6) = 0x56;
			*(buff+7) = 0x53;
			*(buff+8) = 0x49;
			*(buff+9) = 0x54;
			*(buff+10) = 0x4C;
			*(buff+11) = 0x56;
			*(buff+12) = 0x4C;
			*(buff+13) = 0x0D; 	
			break;		
		case 2:
			// #GIHGOVSIPQVN\r 	
			*buff 		= 0x23;
			*(buff+1) = 0x47;
			*(buff+2) = 0x49;
			*(buff+3) = 0x48;
			*(buff+4) = 0x47;
			*(buff+5) = 0x4F;
			*(buff+6) = 0x56;
			*(buff+7) = 0x53;
			*(buff+8) = 0x49;
			*(buff+9) = 0x50;
			*(buff+10) = 0x51;
			*(buff+11) = 0x56;
			*(buff+12) = 0x4E;
			*(buff+13) = 0x0D;
			break;
		case 3:
			// #GJHGOVSIQGGP\r
			*buff 		= 0x23;
			*(buff+1) = 0x47;
			*(buff+2) = 0x4A;
			*(buff+3) = 0x48;
			*(buff+4) = 0x47;
			*(buff+5) = 0x4F;
			*(buff+6) = 0x56;
			*(buff+7) = 0x53;
			*(buff+8) = 0x49;
			*(buff+9) = 0x51;
			*(buff+10) = 0x47;
			*(buff+11) = 0x47;
			*(buff+12) = 0x50;
			*(buff+13) = 0x0D; 
			break;
		case 4:
			// #GKHGOVSIGKVJ\r
			*buff 		= 0x23;
			*(buff+1) = 0x47;
			*(buff+2) = 0x4B;
			*(buff+3) = 0x48;
			*(buff+4) = 0x47;
			*(buff+5) = 0x4F;
			*(buff+6) = 0x56;
			*(buff+7) = 0x53;
			*(buff+8) = 0x49;
			*(buff+9) = 0x47;
			*(buff+10) = 0x4B;
			*(buff+11) = 0x56;
			*(buff+12) = 0x4A;
			*(buff+13) = 0x0D; 
			break;
	}
}

uint8_t OVEN_Convert_ASCII_byte(uint8_t ch)
{
	uint8_t hash;
	
	int toupper(int c);
	ch=toupper(ch);
	
	switch(ch)
	{
		case '0': hash=ch-'0'; break;
		case '1': hash=ch-'0'; break;
		case '2': hash=ch-'0'; break;
		case '3': hash=ch-'0'; break;
		case '4': hash=ch-'0'; break;
		case '5': hash=ch-'0'; break;
		case '6': hash=ch-'0'; break;
		case '7': hash=ch-'0'; break;
		case '8': hash=ch-'0'; break;
		case '9': hash=ch-'0'; break;
		case 'A': hash=ch-'A'+10; break;
		case 'B': hash=ch-'A'+10; break;
		case 'C': hash=ch-'A'+10; break;
		case 'D': hash=ch-'A'+10; break;
		case 'E': hash=ch-'A'+10; break;
		case 'F': hash=ch-'A'+10; break;
		case 'G': hash=ch-'A'+10; break;
		case 'H': hash=ch-'A'+10; break;
		case 'I': hash=ch-'A'+10; break;
		case 'J': hash=ch-'A'+10; break;
		case 'K': hash=ch-'A'+10; break;
		case 'L': hash=ch-'A'+10; break;
		case 'M': hash=ch-'A'+10; break;
		case 'N': hash=ch-'A'+10; break;
		case 'O': hash=ch-'A'+10; break;
		case 'P': hash=ch-'A'+10; break;
		case 'Q': hash=ch-'A'+10; break;
		case 'R': hash=ch-'A'+10; break;
		case 'S': hash=ch-'A'+10; break;
		case 'T': hash=ch-'A'+10; break;
		case 'U': hash=ch-'A'+10; break;
		case 'V': hash=ch-'A'+10; break;
		case 'W': hash=ch-'A'+10; break;
		case 'X': hash=ch-'A'+10; break;
		case 'Y': hash=ch-'A'+10; break;
		case 'Z': hash=ch-'A'+10; break;
		case '-':	hash=10+26; break;
		case '_':	hash=10+26+1; break;
		case '/':	hash=10+26+2; break;
		case ' ':	hash=10+26+3; break;
	}
	
	//hash*=2;
	
	return hash;
}

float OVEN_Convert_DEC_dot0(uint32_t data)
{
	float result = 0;
	double S = 0;
	double Exponent = 0;
	float Mantissa = 0;
	float s1=0;
	float e1 = 0;
	float m1 = 0;
		
	S = (data&0x80000000)>>31;
	Exponent = (data&0x70000000)>>28;
	Mantissa = data&0xF;
	Mantissa +=((data&0xF0)>>4)*10;
	Mantissa +=((data&0xF00)>>8)*100;
	Mantissa +=((data&0xF000)>>12)*1000;
	Mantissa +=((data&0xF0000)>>16)*10000;
	Mantissa +=((data&0xF00000)>>20)*100000;
	
	s1 = pow(-1, S);
	e1 = pow(10, (-Exponent));
	m1 = Mantissa;
	result = s1 * e1 * m1;
	
	return result;
}

uint16_t OVEN_CRC16(uint8_t* packet, uint8_t length)
{
	uint8_t i, j;
	uint16_t crc;
	uint8_t b;


	crc = 0;
	for (i = 0; i < length; i++)
	{
		b = packet[i];
		for (j = 0; j < 8; j++, b <<= 1)
		{
			if ((b ^ (crc >> 8)) & 0x80)
			{
				crc <<= 1;
				crc ^= 0x8F57;
			}
			else
				crc <<= 1;
		}
	}
	return crc;
}

uint8_t OVEN_Status(void)
{
	return status_si8;
}

void OVEN_Clear_Status(void)
{
	status_si8 = 0;
}

uint8_t OVEN_Get_Adress_SI8(void)
{
	return addr_si8;
}

float OVEN_Get_FlowMeter(void)
{
	return flow_meter;
}

uint8_t OVEN_Return_Result(void)
{
	if(oven_crc_result)
	{
		oven_crc_result = 0;
		return addr_si8;
	}
	else return 0;
}
