#ifndef MODBUSRTU_H
#define MODBUSRTU_H
#include "stm32f103xe.h"

#define ID_DEVICE         	 77

#define INDEX_ADDR           0
#define INDEX_COMMAND        1
#define INDEX_START_REG_HI   2
#define INDEX_START_REG_LO   3
#define INDEX_COUNT_REG_HI   4
#define INDEX_COUNT_REG_LO   5
#define INDEX_COUNT_BYTE     6
#define INDEX_START_DATA     3

#define COMMAND_READ_HOLDING_REG    0x03
#define COMMAND_READ_INPUT_REG      0x04
#define COMMAND_WRITE_MULTIPLE_REG  0x10    

void ModbusRTU_init(uint16_t *hol_reg, uint16_t *in_reg);
void ModbusRTU_READ_INPUT_REG_Pack(uint8_t *buff, uint8_t *len_buff, uint8_t device_id, uint16_t start_reg, uint16_t count_reg);
void ModbusRTU_PRESET_MULTIPLE_REG_Pack(uint8_t *buff, uint8_t *len_buff, uint8_t device_id, uint16_t start_reg, uint16_t count_reg);
void ModbusRTU_Heandler_Slave(uint8_t *buff_in, uint8_t lenght_buff_in, uint8_t *buff_out, uint8_t *lenght_buff_out);
void ModbusRTU_Heandler_Master(uint8_t *buff_in, uint8_t lenght_buff_in, uint8_t strat_reg, uint8_t count_reg);
uint8_t ModbusRTU_Return_Result(void);


#endif