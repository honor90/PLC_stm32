#ifndef MAIN_H
#define MAIN_H
#include "stm32f103xe.h"


#define True 	1
#define False 0

#define MASK_PACK_1 (1<<0)
#define MASK_PACK_2 (1<<1)
#define MASK_PACK_3 (1<<2)
#define MASK_PACK_4 (1<<3)


#define ID_DRV1_SENSOR_ROT_FREQ_1 	1
#define ID_DRV1_SENSOR_ROT_FREQ_2 	2
#define ID_DRV2_SENSOR_ROT_FREQ_1 	3
#define ID_DRV2_SENSOR_ROT_FREQ_2 	4
#define ID_DRV1_SENSOR_VIBRATION_1  20
#define ID_DRV1_SENSOR_VIBRATION_2  21
#define ID_DRV2_SENSOR_VIBRATION_1 	22
#define ID_DRV2_SENSOR_VIBRATION_2 	23

//Holding Registers
#define DRV1_COMMAND_Lo              0
#define DRV1_COMMAND_Hi              1
#define DRV1_LIMIT_R_PTC_Lo          2
#define DRV1_LIMIT_R_PTC_Hi          3
#define DRV1_LIMIT_RATIO_1_Lo        4
#define DRV1_LIMIT_RATIO_1_Hi        5
#define DRV1_LIMIT_RATIO_2_Lo        6
#define DRV1_LIMIT_RATIO_2_Hi        7
#define DRV1_LIMIT_VIB_SPEED_1_X_Lo  8
#define DRV1_LIMIT_VIB_SPEED_1_X_Hi  9
#define DRV1_LIMIT_VIB_SPEED_2_X_Lo  10
#define DRV1_LIMIT_VIB_SPEED_2_X_Hi  11

#define DRV2_COMMAND_Lo              12
#define DRV2_COMMAND_Hi              13
#define DRV2_LIMIT_R_PTC_Lo          14
#define DRV2_LIMIT_R_PTC_Hi          15
#define DRV2_LIMIT_RATIO_1_Lo        16
#define DRV2_LIMIT_RATIO_1_Hi        17
#define DRV2_LIMIT_RATIO_2_Lo        18
#define DRV2_LIMIT_RATIO_2_Hi        19
#define DRV2_LIMIT_VIB_SPEED_1_X_Lo  20
#define DRV2_LIMIT_VIB_SPEED_1_X_Hi  21
#define DRV2_LIMIT_VIB_SPEED_2_X_Lo  22
#define DRV2_LIMIT_VIB_SPEED_2_X_Hi  23

//Input Registers
#define DRV1_ERROR                   0
#define DRV1_CODE_ALARM              1
#define DRV1_R_PTC_Lo                2
#define DRV1_R_PTC_Hi                3
#define DRV1_ROT_FREQ_MOTOR_Lo       4
#define DRV1_ROT_FREQ_MOTOR_Hi       5
#define DRV1_ROT_FREQ_GEN_1_Lo       6
#define DRV1_ROT_FREQ_GEN_1_Hi       7
#define DRV1_ROT_FREQ_GEN_2_Lo       8
#define DRV1_ROT_FREQ_GEN_2_Hi       9
#define DRV1_CALC_ROT_FREQ_GEN_1_Lo  10
#define DRV1_CALC_ROT_FREQ_GEN_1_Hi  11
#define DRV1_CALC_ROT_FREQ_GEN_2_Lo  12
#define DRV1_CALC_ROT_FREQ_GEN_2_Hi  13
#define DRV1_VIB_SPEED_1_X_Lo        14
#define DRV1_VIB_SPEED_1_X_Hi        15
#define DRV1_VIB_SPEED_2_X_Lo        16
#define DRV1_VIB_SPEED_2_X_Hi        17

#define DRV2_ERROR                   18
#define DRV2_CODE_ALARM              19
#define DRV2_R_PTC_Lo                20
#define DRV2_R_PTC_Hi                21
#define DRV2_ROT_FREQ_MOTOR_Lo       22
#define DRV2_ROT_FREQ_MOTOR_Hi       23
#define DRV2_ROT_FREQ_GEN_1_Lo       24
#define DRV2_ROT_FREQ_GEN_1_Hi       25
#define DRV2_ROT_FREQ_GEN_2_Lo       26
#define DRV2_ROT_FREQ_GEN_2_Hi       27
#define DRV2_CALC_ROT_FREQ_GEN_1_Lo  28
#define DRV2_CALC_ROT_FREQ_GEN_1_Hi  29
#define DRV2_CALC_ROT_FREQ_GEN_2_Lo  30
#define DRV2_CALC_ROT_FREQ_GEN_2_Hi  31
#define DRV2_VIB_SPEED_1_X_Lo        32
#define DRV2_VIB_SPEED_1_X_Hi        33
#define DRV2_VIB_SPEED_2_X_Lo        34
#define DRV2_VIB_SPEED_2_X_Hi        35

#define ERROR_NET    0x0B
#define ALARM_SMOKE  (1<<0)
#define ALARM_TEMP   (1<<1)
#define ALARM_SPEED  (1<<2)
#define ALARM_VIB_1  (1<<3)
#define ALARM_VIB_2  (1<<4)


#define COMMAND_WRITE_FLASH 0x01

#define ADDR_DRV1_LIMIT_TEMP          0x0800A000
#define ADDR_DRV1_LIMIT_RATIO_1       0x0800A004
#define ADDR_DRV1_LIMIT_RATIO_2       0x0800A008
#define ADDR_DRV1_LIMIT_VIB_SPEED_1_X 0x0800A00C
#define ADDR_DRV1_LIMIT_VIB_SPEED_2_X 0x0800A010

#define ADDR_DRV2_LIMIT_TEMP          0x0800A400
#define ADDR_DRV2_LIMIT_RATIO_1       0x0800A404
#define ADDR_DRV2_LIMIT_RATIO_2       0x0800A408
#define ADDR_DRV2_LIMIT_VIB_SPEED_1_X 0x0800A40C
#define ADDR_DRV2_LIMIT_VIB_SPEED_2_X 0x0800A410


#define DRV1_SENSOR_SMOKE_ALARM() GPIOC->IDR & GPIO_IDR_IDR6
#define DRV2_SENSOR_SMOKE_ALARM() GPIOC->IDR & GPIO_IDR_IDR8


#define DRV1_BUTTON_PUSH() GPIOD->IDR & GPIO_IDR_IDR2
#define DRV2_BUTTON_PUSH() GPIOA->IDR & GPIO_IDR_IDR4


#define DRV1_RELE_ON()  GPIOB->BSRR |= GPIO_BSRR_BS8
#define DRV1_RELE_OFF() GPIOB->BRR |= GPIO_BRR_BR8

#define DRV2_RELE_ON()   GPIOB->BSRR |= GPIO_BSRR_BS9 
#define DRV2_RELE_OFF()  GPIOB->BRR |= GPIO_BRR_BR9


#define DRV1_LED_WORK_ON()               GPIOC->BSRR |= GPIO_BSRR_BS10
#define DRV1_LED_WORK_OFF()              GPIOC->BRR |= GPIO_BRR_BR10
#define DRV1_LED_ALARM_Fire_ON()         GPIOC->BSRR |= GPIO_BSRR_BS12
#define DRV1_LED_ALARM_Fire_OFF()        GPIOC->BRR |= GPIO_BRR_BR12
#define DRV1_LED_ALARM_Temperature_ON()  GPIOA->BSRR |= GPIO_BSRR_BS14
#define DRV1_LED_ALARM_Temperature_OFF() GPIOA->BRR |= GPIO_BRR_BR14
#define DRV1_LED_ALARM_Destruction_ON()  GPIOC->BSRR |= GPIO_BSRR_BS11
#define DRV1_LED_ALARM_Destruction_OFF() GPIOC->BRR |= GPIO_BRR_BR11
#define DRV1_LED_ALARM_Vibration_ON()    GPIOA->BSRR |= GPIO_BSRR_BS13
#define DRV1_LED_ALARM_Vibration_OFF()   GPIOA->BRR |= GPIO_BRR_BR13

#define DRV2_LED_WORK_ON()                GPIOB->BSRR |= GPIO_BSRR_BS7
#define DRV2_LED_WORK_OFF()               GPIOB->BRR |= GPIO_BRR_BR7
#define DRV2_LED_ALARM_Fire_ON()          GPIOB->BSRR |= GPIO_BSRR_BS0
#define DRV2_LED_ALARM_Fire_OFF()         GPIOB->BRR |= GPIO_BRR_BR0
#define DRV2_LED_ALARM_Temperature_ON()   GPIOC->BSRR |= GPIO_BSRR_BS3
#define DRV2_LED_ALARM_Temperature_OFF()  GPIOC->BRR |= GPIO_BRR_BR3
#define DRV2_LED_ALARM_Destruction_ON()   GPIOC->BSRR |= GPIO_BSRR_BS13
#define DRV2_LED_ALARM_Destruction_OFF()  GPIOC->BRR |= GPIO_BRR_BR13
#define DRV2_LED_ALARM_Vibration_ON()     GPIOC->BSRR |= GPIO_BSRR_BS2
#define DRV2_LED_ALARM_Vibration_OFF()    GPIOC->BRR |= GPIO_BRR_BR2

#define LED2_ON()  GPIOA->BSRR |= GPIO_BSRR_BS5
#define LED2_OFF() GPIOA->BRR |= GPIO_BRR_BR5


#endif