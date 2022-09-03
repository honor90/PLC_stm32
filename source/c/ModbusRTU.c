#include "stm32f103xe.h"
#include "ModbusRTU.h"
#include "CRC16.h"


uint16_t *buff_hol_reg;
uint16_t *buff_in_reg;

void ModbusRTU_init(uint16_t *hol_reg, uint16_t *in_reg)
{
	buff_hol_reg = hol_reg;
	buff_in_reg = in_reg;
}

void ModbusRTU_Write_REG(uint16_t *buff_reg, uint8_t *buff, uint8_t start_reg, uint8_t count_reg)
{
	uint16_t i = 0;
	uint16_t j = 0;
	
	switch(*(buff+INDEX_COMMAND))
	{
		case COMMAND_READ_HOLDING_REG:
			j = 3;
			break;
		case COMMAND_READ_INPUT_REG:
			j = 3;
			break;
		case COMMAND_WRITE_MULTIPLE_REG:
			j = 7;
			break;
	}

	
	for(i = start_reg; i < start_reg+count_reg; i++)
	{
		*(buff_reg+i) = (*(buff+j)<<8)|(*(buff+j+1));
		j+=2;
	}
}

void ModbusRTU_Read_REG(uint16_t *buff_reg, uint8_t *buff, uint16_t start_reg, uint16_t count_reg)
{
	uint8_t i = 0;
	uint8_t j = INDEX_START_DATA;
	for(i = start_reg; i < start_reg+count_reg; i++)
	{
		*(buff+j) = *(buff_reg+i)>>8;
		j++;
		*(buff+j) = *(buff_reg+i)&0xFF;
		j++;
	}
}


void ModbusRTU_READ_INPUT_REG_Pack(uint8_t *buff, uint8_t *len_buff, uint8_t device_id, uint16_t start_reg, uint16_t count_reg)
{
	*buff = device_id;
	*(buff+1) = COMMAND_READ_INPUT_REG;
	*(buff+2) = start_reg>>8;
	*(buff+3) = start_reg&0xFF;
	*(buff+4) = count_reg>>8;
	*(buff+5) = count_reg&0xFF;	
	*(buff+6) = CRC16(buff, 6)>>8;
	*(buff+7) = CRC16(buff, 6)&0xFF;
	
	*len_buff = 8;
}


void ModbusRTU_PRESET_MULTIPLE_REG_Pack(uint8_t *buff, uint8_t *len_buff, uint8_t device_id, uint16_t start_reg, uint16_t count_reg)
{
	uint8_t count_byte = count_reg*2;
	
	*buff = device_id;
	*(buff+1) = COMMAND_WRITE_MULTIPLE_REG;
	*(buff+2) = start_reg>>8;
	*(buff+3) = start_reg&0xFF;
	*(buff+4) = count_reg>>8;
	*(buff+5) = count_reg&0xFF;	
	*(buff+6) = count_byte;
	ModbusRTU_Read_REG(buff_hol_reg, buff, start_reg, count_reg);
	*(buff+count_byte+7) = CRC16(buff, count_byte+7)>>8;
	*(buff+count_byte+8) = CRC16(buff, count_byte+7)&0xFF;
	
	*len_buff = count_byte + 9;
}

void ModbusRTU_Heandler_Slave(uint8_t *buff_in, uint8_t lenght_buff_in, uint8_t *buff_out, uint8_t *lenght_buff_out)
{
	uint16_t Start_reg = 0;
	uint16_t Count_reg = 0;
	uint16_t Count_byte = 0;
	uint16_t crc_calc = 0;
	uint16_t crc_pack = 0;
	
	crc_calc = CRC16(buff_in, lenght_buff_in-1);
	crc_pack = ((*(buff_in+lenght_buff_in-1))<<8)|(*(buff_in+(lenght_buff_in)));
	
	if(crc_calc == crc_pack)
	{
		if(*(buff_in+INDEX_ADDR) == ID_DEVICE)
		{
			switch(*(buff_in+INDEX_COMMAND))
			{
				case COMMAND_READ_HOLDING_REG:
					Start_reg = ((*(buff_in+INDEX_START_REG_HI))<<8)|(*(buff_in+INDEX_START_REG_LO));
					Count_reg = ((*(buff_in+INDEX_COUNT_REG_HI))<<8)|(*(buff_in+INDEX_COUNT_REG_LO));
					Count_byte = Count_reg*2;
					*(buff_out+INDEX_ADDR) = ID_DEVICE;
					*(buff_out+INDEX_COMMAND) = COMMAND_READ_HOLDING_REG;
					*(buff_out+2) = Count_byte;
					ModbusRTU_Read_REG(buff_hol_reg, buff_out, Start_reg, Count_reg);
					*(buff_out+Count_byte+3) = CRC16(buff_out, Count_byte+3)>>8;
					*(buff_out+Count_byte+4) = CRC16(buff_out, Count_byte+3)&0xFF;
					*lenght_buff_out = Count_byte + 5;
					break;
				case COMMAND_READ_INPUT_REG:
					Start_reg = ((*(buff_in+INDEX_START_REG_HI))<<8)|(*(buff_in+INDEX_START_REG_LO));
					Count_reg = ((*(buff_in+INDEX_COUNT_REG_HI))<<8)|(*(buff_in+INDEX_COUNT_REG_LO));
					Count_byte = Count_reg*2;
					*(buff_out+INDEX_ADDR) = ID_DEVICE;
					*(buff_out+INDEX_COMMAND) = COMMAND_READ_INPUT_REG;
					*(buff_out+2) = Count_byte;
					ModbusRTU_Read_REG(buff_in_reg, buff_out, Start_reg, Count_reg);
					*(buff_out+Count_byte+3) = CRC16(buff_out, Count_byte+3)>>8;
					*(buff_out+Count_byte+4) = CRC16(buff_out, Count_byte+3)&0xFF;
					*lenght_buff_out = Count_byte + 5;
					break;
				case COMMAND_WRITE_MULTIPLE_REG:
					Start_reg = ((*(buff_in+INDEX_START_REG_HI))<<8)|(*(buff_in+INDEX_START_REG_LO));
					Count_reg = ((*(buff_in+INDEX_COUNT_REG_HI))<<8)|(*(buff_in+INDEX_COUNT_REG_LO));	
					Count_byte = *(buff_in+INDEX_COUNT_BYTE);	
					ModbusRTU_Write_REG(buff_hol_reg, buff_in, Start_reg, Count_reg);				
					break;
			}		
		} 
	}		
}

uint8_t addr_slave = 0;
uint8_t crc_result = 0;

void ModbusRTU_Heandler_Master(uint8_t *buff_in, uint8_t lenght_buff_in, uint8_t strat_reg, uint8_t count_reg)
{
	uint16_t crc_calc = 0;
	uint16_t crc_pack = 0;
	addr_slave = (*(buff_in+INDEX_ADDR));
	crc_calc = CRC16(buff_in, lenght_buff_in-1);
	crc_pack = ((*(buff_in+lenght_buff_in-1))<<8)|(*(buff_in+(lenght_buff_in)));
	
	if(crc_calc == crc_pack)
	{
		crc_result = 1;
		switch(*(buff_in+INDEX_COMMAND))
		{
			case COMMAND_READ_INPUT_REG:
					ModbusRTU_Write_REG(buff_in_reg, buff_in, strat_reg, count_reg);		
				break;
			case COMMAND_WRITE_MULTIPLE_REG:
				break;
		}
	}
	else crc_result = 0;
}

uint8_t ModbusRTU_Return_Result(void)
{
	if(crc_result)
	{
		crc_result = 0;
		return addr_slave;
	}
	else return 0;
}

