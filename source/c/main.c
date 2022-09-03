#include "stm32f103xe.h"
#include "main.h"
#include "rcc.h"
#include "afio.h"
#include "gpio.h"
#include "tim.h"
#include "usart.h"
#include "etr.h"
#include "dma.h"
#include "adc.h"
#include "iwdg.h"
#include "flash.h"
#include "ModbusRTU.h"
#include "oven.h"


uint8_t *PACK_input_Master;
uint8_t *PACK_input_Slave;

uint8_t PACK_out_Master[50];
uint8_t PACK_out_Slave[50];

uint8_t id_next_device;
uint8_t Enable_USART1_Send;

uint8_t IN_start_reg = 0;
uint8_t IN_count_reg = 0;

uint16_t ADC_VALUE_Chanel[2];

uint8_t lenght_pack_in_master = 0;
uint8_t lenght_pack_out_master = 0;

uint8_t lenght_pack_in_slave = 0;
uint8_t lenght_pack_out_slave = 0;

uint8_t PUSH_Button_DRV1 = True;
uint8_t PUSH_Button_DRV2 = True;

uint8_t Enable_Timer_Button_DRV1 = False;
uint8_t Enable_Timer_Button_DRV2 = False;

uint8_t Timer_Button_DRV1 = 0;
uint8_t Timer_Button_DRV2 = 0;

uint8_t Enable_Timer = True;

uint8_t msec = 0;

uint8_t Enable_count_alarm_drv1 = False;	
uint8_t Enable_count_alarm_drv2 = False;
uint8_t Enable_LED_ALARM_DRV1 = False;
uint8_t Enable_LED_ALARM_DRV2 = False;
uint16_t count_alarm_drv1 = 0;
uint16_t count_alarm_drv2 = 0;

uint8_t Mask_Pack_DRV1 = 0;
uint8_t Mask_Pack_DRV2 = 0;
uint8_t Enable_Led_Work_DRV1 = False;
uint8_t Enable_Led_Work_DRV2 = False;
uint8_t Led_ON = True;
uint8_t Enable_count = False;
uint32_t count = 0;
uint8_t count_led_work = 0;

uint8_t Enable_delay = False;
uint8_t delay_ = 0;

float Uadc_1 = 0;
float Uadc_2 = 0;

float R_ptc_1 = 0;
float R_ptc_2 = 0;

float DRV1_MAX_RotFreqGen_1 = 0;
float DRV1_MIN_RotFreqGen_1 = 0;

float DRV1_MAX_RotFreqGen_2 = 0;
float DRV1_MIN_RotFreqGen_2 = 0;

float DRV2_MAX_RotFreqGen_1 = 0;
float DRV2_MIN_RotFreqGen_1 = 0;

float DRV2_MAX_RotFreqGen_2 = 0;
float DRV2_MIN_RotFreqGen_2 = 0;



struct VALUE{
	uint16_t Error;
	uint16_t CodeAlarm;
	float R_ptc;
	float RotFreqMotor;
	float RotFreqGen_1;
	float RotFreqGen_2;
	float CalcRotFreqGen_1;
	float CalcRotFreqGen_2;
	float VibSpeed_1X;
	float VibSpeed_2X;		
};

typedef union {
	uint16_t ModbusRTU_IN_REG[36];
	struct {
		struct VALUE DRV1;
		struct VALUE DRV2;		
	}REG;
}TypeValue;
		
struct LIMIT{
	uint32_t Command;
	float R_ptc;
	float Ratio_1;
	float Ratio_2;
	float VibSpeed_1X;
	float VibSpeed_2X;
};
	
typedef union {
	uint16_t ModbusRTU_HOL_REG[24];
	struct {
		struct LIMIT DRV1;
		struct LIMIT DRV2;		
	}REG;
}TyteLimit;


TypeValue INPUT;
TyteLimit HOLDING;

void SysTick_Handler(void);

int main(void)
{	
	
	RCC_init();
	AFIO_init();
	GPIO_init();
	USART1_init();
	USART3_init();
	TIM3_init();
	//ETR_init();
	DMA_init(ADC_VALUE_Chanel, sizeof(ADC_VALUE_Chanel));
	ADC_init();

	IWDG_init();
	
	ModbusRTU_init(HOLDING.ModbusRTU_HOL_REG, INPUT.ModbusRTU_IN_REG);
	OVEN_init();

	SysTick_Config(32000000/20);
	
	
	FLASH_unlock();
	
	if(FLASH_read(ADDR_DRV1_LIMIT_TEMP) == 0xFFFFFFFF) HOLDING.REG.DRV1.R_ptc = 500;
	else HOLDING.REG.DRV1.R_ptc = FLASH_read(ADDR_DRV1_LIMIT_TEMP);
	if(FLASH_read(ADDR_DRV1_LIMIT_RATIO_1) == 0xFFFFFFFF) HOLDING.REG.DRV1.Ratio_1 = 3.73;
	else HOLDING.REG.DRV1.Ratio_1 = FLASH_read(ADDR_DRV1_LIMIT_RATIO_1);
	if(FLASH_read(ADDR_DRV1_LIMIT_RATIO_2) == 0xFFFFFFFF) HOLDING.REG.DRV1.Ratio_2 = 2;
	else HOLDING.REG.DRV1.Ratio_2 = FLASH_read(ADDR_DRV1_LIMIT_RATIO_2);
	if(FLASH_read(ADDR_DRV1_LIMIT_VIB_SPEED_1_X) == 0xFFFFFFFF) HOLDING.REG.DRV1.VibSpeed_1X = 30;
	else HOLDING.REG.DRV1.VibSpeed_1X = FLASH_read(ADDR_DRV1_LIMIT_VIB_SPEED_1_X);
	if(FLASH_read(ADDR_DRV1_LIMIT_VIB_SPEED_2_X) == 0xFFFFFFFF) HOLDING.REG.DRV1.VibSpeed_2X = 30;
	else HOLDING.REG.DRV1.VibSpeed_2X = FLASH_read(ADDR_DRV1_LIMIT_VIB_SPEED_2_X);

	if(FLASH_read(ADDR_DRV2_LIMIT_TEMP) == 0xFFFFFFFF) HOLDING.REG.DRV2.R_ptc = 500;
	else HOLDING.REG.DRV2.R_ptc = FLASH_read(ADDR_DRV2_LIMIT_TEMP);
	if(FLASH_read(ADDR_DRV2_LIMIT_RATIO_1) == 0xFFFFFFFF) HOLDING.REG.DRV2.Ratio_1 = 3.73;
	else HOLDING.REG.DRV2.Ratio_1 = FLASH_read(ADDR_DRV2_LIMIT_RATIO_1);
	if(FLASH_read(ADDR_DRV2_LIMIT_RATIO_2) == 0xFFFFFFFF) HOLDING.REG.DRV2.Ratio_2 = 2;
	else HOLDING.REG.DRV2.Ratio_2 = FLASH_read(ADDR_DRV2_LIMIT_RATIO_2);
	if(FLASH_read(ADDR_DRV2_LIMIT_VIB_SPEED_1_X) == 0xFFFFFFFF) HOLDING.REG.DRV2.VibSpeed_1X = 30;
	else HOLDING.REG.DRV2.VibSpeed_1X = FLASH_read(ADDR_DRV2_LIMIT_VIB_SPEED_1_X);
	if(FLASH_read(ADDR_DRV2_LIMIT_VIB_SPEED_2_X) == 0xFFFFFFFF) HOLDING.REG.DRV2.VibSpeed_2X = 30;
	else HOLDING.REG.DRV2.VibSpeed_2X = FLASH_read(ADDR_DRV2_LIMIT_VIB_SPEED_2_X);


	FLASH_lock();
	
	DRV1_RELE_OFF();
	DRV2_RELE_OFF();

	DRV1_LED_WORK_ON();
	DRV1_LED_ALARM_Fire_ON();
	DRV1_LED_ALARM_Temperature_ON();
	DRV1_LED_ALARM_Destruction_ON();
	DRV1_LED_ALARM_Vibration_ON();
	
	DRV2_LED_WORK_ON();
	DRV2_LED_ALARM_Fire_ON();
	DRV2_LED_ALARM_Temperature_ON();
	DRV2_LED_ALARM_Destruction_ON();
	DRV2_LED_ALARM_Vibration_ON();
	
	delay_ = 20;
	Enable_delay = True;
	while(delay_)
	{
		
	IWDG_reset();
	}
	
	DRV1_LED_WORK_OFF();
	DRV1_LED_ALARM_Fire_OFF();
	DRV1_LED_ALARM_Temperature_OFF();
	DRV1_LED_ALARM_Destruction_OFF();
	DRV1_LED_ALARM_Vibration_OFF();
	
	DRV2_LED_WORK_OFF();
	DRV2_LED_ALARM_Fire_OFF();
	DRV2_LED_ALARM_Temperature_OFF();
	DRV2_LED_ALARM_Destruction_OFF();
	DRV2_LED_ALARM_Vibration_OFF();
	
	delay_ = 20;
	Enable_delay = True;	
	while(delay_)
	{
		IWDG_reset();
	}
	
	DRV1_LED_WORK_ON();
	DRV2_LED_WORK_ON();
	
	DRV1_LED_WORK_ON();
	DRV1_LED_ALARM_Fire_OFF();
	DRV1_LED_ALARM_Temperature_OFF();
	DRV1_LED_ALARM_Destruction_OFF();
	DRV1_LED_ALARM_Vibration_OFF();
	
	DRV2_LED_WORK_ON();
	DRV2_LED_ALARM_Fire_OFF();
	DRV2_LED_ALARM_Temperature_OFF();
	DRV2_LED_ALARM_Destruction_OFF();
	DRV2_LED_ALARM_Vibration_OFF();
	
	LED2_OFF();
	
	Enable_USART1_Send = True;
	id_next_device = ID_DRV1_SENSOR_ROT_FREQ_1;
	
	
	DRV1_MIN_RotFreqGen_1 = (HOLDING.REG.DRV1.Ratio_1 / HOLDING.REG.DRV1.Ratio_2) * 0.5;
	DRV1_MAX_RotFreqGen_1 = (HOLDING.REG.DRV1.Ratio_1 / HOLDING.REG.DRV1.Ratio_2) * 1.5;
		
		
	DRV2_MIN_RotFreqGen_1 = (HOLDING.REG.DRV2.Ratio_1 / HOLDING.REG.DRV2.Ratio_2) * 0.5;
	DRV2_MAX_RotFreqGen_1 = (HOLDING.REG.DRV2.Ratio_1 / HOLDING.REG.DRV2.Ratio_2) * 1.5;
	
	
	while(1) 
	{
		
		//INPUT.REG.DRV1.RotFreqMotor = (float)(ETR_TIM1_Impulse_get())/2500;
		//INPUT.REG.DRV2.RotFreqMotor = (float)(ETR_TIM2_Impulse_get())/2500;
		
		
		ADC_convert_U((uint32_t *)&Uadc_1, ADC_VALUE_Chanel[0]);
		ADC_convert_U((uint32_t *)&Uadc_2, ADC_VALUE_Chanel[1]);
		
		ADC_calculation_R((uint32_t *)&R_ptc_1, Uadc_1);
		ADC_calculation_R((uint32_t *)&R_ptc_2, Uadc_2);
		
		if(R_ptc_1 < 0)	INPUT.REG.DRV1.R_ptc = 0;	
		else INPUT.REG.DRV1.R_ptc = R_ptc_1;
		if(R_ptc_2 < 0)INPUT.REG.DRV2.R_ptc = 0;
		else INPUT.REG.DRV2.R_ptc = R_ptc_2;		
		
		
		if(INPUT.REG.DRV1.R_ptc > HOLDING.REG.DRV1.R_ptc) 
		{
			DRV1_LED_ALARM_Temperature_ON();
			INPUT.REG.DRV1.CodeAlarm |= ALARM_TEMP;
		}
		else 
		{
			DRV1_LED_ALARM_Temperature_OFF();	
			INPUT.REG.DRV1.CodeAlarm &=~ ALARM_TEMP;
		}			
		
		
		if(INPUT.REG.DRV2.R_ptc > HOLDING.REG.DRV2.R_ptc) 
		{
			DRV2_LED_ALARM_Temperature_ON();
			INPUT.REG.DRV2.CodeAlarm |= ALARM_TEMP;
		}
		else 
		{
			DRV2_LED_ALARM_Temperature_OFF();
			INPUT.REG.DRV2.CodeAlarm &=~ ALARM_TEMP;
		}
		

		
		
	
		

		if(INPUT.REG.DRV1.RotFreqGen_1 >=10 || INPUT.REG.DRV1.RotFreqGen_2 >=10)
		{

			if((INPUT.REG.DRV1.RotFreqGen_1>10)&&(INPUT.REG.DRV1.RotFreqGen_2>10))INPUT.REG.DRV1.CalcRotFreqGen_1 = INPUT.REG.DRV1.RotFreqGen_1 / INPUT.REG.DRV1.RotFreqGen_2;
			if(INPUT.REG.DRV1.RotFreqGen_1<10) INPUT.REG.DRV1.CalcRotFreqGen_1 = 1/INPUT.REG.DRV1.RotFreqGen_2;
			if(INPUT.REG.DRV1.RotFreqGen_2<10) INPUT.REG.DRV1.CalcRotFreqGen_1 = INPUT.REG.DRV1.RotFreqGen_1/1;
			
			if((INPUT.REG.DRV1.CalcRotFreqGen_1 < DRV1_MIN_RotFreqGen_1) || (INPUT.REG.DRV1.CalcRotFreqGen_1 > DRV1_MAX_RotFreqGen_1)) 
			{
				Enable_count_alarm_drv1 = True;
				Enable_LED_ALARM_DRV1 = True;
			}
			else 
			{
				Enable_count_alarm_drv1 = False;	
				Enable_LED_ALARM_DRV1 = False;					
				DRV1_LED_ALARM_Destruction_OFF();
			}
		}
		else 
		{
			Enable_count_alarm_drv1 = False;	
			Enable_LED_ALARM_DRV1 = False;				
			DRV1_LED_ALARM_Destruction_OFF();
		}
			
		
		if(INPUT.REG.DRV2.RotFreqGen_1 >=10 || INPUT.REG.DRV2.RotFreqGen_2 >=10)
		{
			if((INPUT.REG.DRV2.RotFreqGen_1>10)&&(INPUT.REG.DRV2.RotFreqGen_2>10))INPUT.REG.DRV2.CalcRotFreqGen_1 = INPUT.REG.DRV2.RotFreqGen_1 / INPUT.REG.DRV2.RotFreqGen_2;
			if(INPUT.REG.DRV2.RotFreqGen_1<10) INPUT.REG.DRV2.CalcRotFreqGen_1 = 1/INPUT.REG.DRV2.RotFreqGen_2;
			if(INPUT.REG.DRV2.RotFreqGen_2<10) INPUT.REG.DRV2.CalcRotFreqGen_1 = INPUT.REG.DRV2.RotFreqGen_1/1;

			if((INPUT.REG.DRV2.CalcRotFreqGen_1 < DRV2_MIN_RotFreqGen_1) || (INPUT.REG.DRV2.CalcRotFreqGen_1 > DRV2_MAX_RotFreqGen_1)) 
			{
				Enable_count_alarm_drv2 = True;	
				Enable_LED_ALARM_DRV2 = True;				
			}
			else 
			{
				Enable_count_alarm_drv2 = False;	
				Enable_LED_ALARM_DRV2 = False;				
				DRV2_LED_ALARM_Destruction_OFF();
			}
		}
		else
		{
			Enable_count_alarm_drv2 = False;	
			Enable_LED_ALARM_DRV2 = False;			
			DRV2_LED_ALARM_Destruction_OFF();
		}

		

		
		
		if((INPUT.REG.DRV1.VibSpeed_1X > HOLDING.REG.DRV1.VibSpeed_1X) || (INPUT.REG.DRV1.VibSpeed_2X > HOLDING.REG.DRV1.VibSpeed_2X)) 
		{
			DRV1_LED_ALARM_Vibration_ON();
		}
		else 
		{
			DRV1_LED_ALARM_Vibration_OFF();
		}
		if(INPUT.REG.DRV1.VibSpeed_1X > HOLDING.REG.DRV1.VibSpeed_1X) 
		{
			INPUT.REG.DRV1.CodeAlarm |= ALARM_VIB_1;
		}
		else 
		{
			INPUT.REG.DRV1.CodeAlarm &=~ ALARM_VIB_1;
		}		
		if(INPUT.REG.DRV1.VibSpeed_2X > HOLDING.REG.DRV1.VibSpeed_2X) 
		{
			INPUT.REG.DRV1.CodeAlarm |= ALARM_VIB_2;
		}
		else 
		{
			INPUT.REG.DRV1.CodeAlarm &=~ ALARM_VIB_2;
		}


		if((INPUT.REG.DRV2.VibSpeed_1X > HOLDING.REG.DRV2.VibSpeed_1X) || (INPUT.REG.DRV2.VibSpeed_2X > HOLDING.REG.DRV2.VibSpeed_2X)) 
		{
			DRV2_LED_ALARM_Vibration_ON();
		}
		else 
		{
			DRV2_LED_ALARM_Vibration_OFF();
		}
		if(INPUT.REG.DRV2.VibSpeed_1X > HOLDING.REG.DRV2.VibSpeed_1X) 
		{
			INPUT.REG.DRV2.CodeAlarm |= ALARM_VIB_1;
		}
		else 
		{
			INPUT.REG.DRV2.CodeAlarm &=~ ALARM_VIB_1;
		}		
		if(INPUT.REG.DRV2.VibSpeed_2X > HOLDING.REG.DRV2.VibSpeed_2X) 
		{
			INPUT.REG.DRV2.CodeAlarm |= ALARM_VIB_2;
		}
		else 
		{
			INPUT.REG.DRV2.CodeAlarm &=~ ALARM_VIB_2;
		}
		
		
		
		
		if(DRV1_SENSOR_SMOKE_ALARM())
		{
			DRV1_LED_ALARM_Fire_ON();
			INPUT.REG.DRV1.CodeAlarm |= ALARM_SMOKE;
		}
		
		if(DRV2_SENSOR_SMOKE_ALARM())
		{
			DRV2_LED_ALARM_Fire_ON();
			INPUT.REG.DRV2.CodeAlarm |= ALARM_SMOKE;
		}
		
		
		if(DRV1_BUTTON_PUSH())
		{
			if(PUSH_Button_DRV1)
			{
				PUSH_Button_DRV1 = False;
				TIM3->CR1 |= TIM_CR1_CEN;
				DRV1_RELE_ON();			
				Timer_Button_DRV1 = 0;
				Enable_Timer_Button_DRV1 = True;
			}				
		}
		else
		{
			if(PUSH_Button_DRV1 == False) 
			{
				PUSH_Button_DRV1 = True;
				Enable_Timer_Button_DRV1 = False;
			}
		}
		
		if(DRV2_BUTTON_PUSH())
		{
			if(PUSH_Button_DRV2)
			{			
				PUSH_Button_DRV2 = False;
				TIM3->CR1 |= TIM_CR1_CEN;
				DRV2_RELE_ON();					
				Timer_Button_DRV2 = 0;
				Enable_Timer_Button_DRV2 = True;	
			}
		}
		else
		{
			if(PUSH_Button_DRV2 == False)
			{
				PUSH_Button_DRV2 = True;
				Enable_Timer_Button_DRV2 = False;
			}
		}		
		
		
		if(Enable_USART1_Send == True)
		{
			Enable_USART1_Send = False;
			LED2_ON();
			switch(id_next_device)
			{				
				case ID_DRV1_SENSOR_ROT_FREQ_1: 
					OVEN_Send_Pack_DSPD(PACK_out_Master, ID_DRV1_SENSOR_ROT_FREQ_1);
					USART1_send(PACK_out_Master, 0, 14);
					Mask_Pack_DRV1 |= MASK_PACK_1;
					id_next_device = ID_DRV1_SENSOR_ROT_FREQ_2;
					break;					
				case ID_DRV1_SENSOR_ROT_FREQ_2: 
					OVEN_Send_Pack_DSPD(PACK_out_Master, ID_DRV1_SENSOR_ROT_FREQ_2);
					USART1_send(PACK_out_Master, 0, 14);	
					Mask_Pack_DRV1 |= MASK_PACK_2;				
					id_next_device = ID_DRV1_SENSOR_VIBRATION_1;
					break;									
				case ID_DRV1_SENSOR_VIBRATION_1: 
					ModbusRTU_READ_INPUT_REG_Pack(PACK_out_Master, &lenght_pack_out_master, ID_DRV1_SENSOR_VIBRATION_1, 1008, 2);
					IN_start_reg = DRV1_VIB_SPEED_1_X_Lo;
					IN_count_reg = 2;
					USART1_send(PACK_out_Master, 0, lenght_pack_out_master);
					Mask_Pack_DRV1 |= MASK_PACK_3;
					id_next_device = ID_DRV1_SENSOR_VIBRATION_2;
					break;									
				case ID_DRV1_SENSOR_VIBRATION_2: 
					ModbusRTU_READ_INPUT_REG_Pack(PACK_out_Master, &lenght_pack_out_master, ID_DRV1_SENSOR_VIBRATION_2, 1008, 2);
					IN_start_reg = DRV1_VIB_SPEED_2_X_Lo;
					IN_count_reg = 2;
					USART1_send(PACK_out_Master, 0, lenght_pack_out_master);
					Mask_Pack_DRV1 |= MASK_PACK_4;
					id_next_device = ID_DRV2_SENSOR_ROT_FREQ_1;
					break;				
				case ID_DRV2_SENSOR_ROT_FREQ_1: 
					OVEN_Send_Pack_DSPD(PACK_out_Master, ID_DRV2_SENSOR_ROT_FREQ_1);
					USART1_send(PACK_out_Master, 0, 14);		
					Mask_Pack_DRV2 |= MASK_PACK_1;				
					id_next_device = ID_DRV2_SENSOR_ROT_FREQ_2;	
					break;					
				case ID_DRV2_SENSOR_ROT_FREQ_2: 
					OVEN_Send_Pack_DSPD(PACK_out_Master, ID_DRV2_SENSOR_ROT_FREQ_2);
					USART1_send(PACK_out_Master, 0, 14);			
					Mask_Pack_DRV2 |= MASK_PACK_2;				
					id_next_device = ID_DRV2_SENSOR_VIBRATION_1;	
					break;									
				case ID_DRV2_SENSOR_VIBRATION_1:
					ModbusRTU_READ_INPUT_REG_Pack(PACK_out_Master, &lenght_pack_out_master, ID_DRV2_SENSOR_VIBRATION_1, 1008, 2);
					IN_start_reg = DRV2_VIB_SPEED_1_X_Lo;
					IN_count_reg = 2;
					USART1_send(PACK_out_Master, 0, lenght_pack_out_master);
					Mask_Pack_DRV2 |= MASK_PACK_3;
					id_next_device = ID_DRV2_SENSOR_VIBRATION_2;		
					break;									
				case ID_DRV2_SENSOR_VIBRATION_2: 
					ModbusRTU_READ_INPUT_REG_Pack(PACK_out_Master, &lenght_pack_out_master, ID_DRV2_SENSOR_VIBRATION_2, 1008, 2);
					IN_start_reg = DRV2_VIB_SPEED_2_X_Lo;
					IN_count_reg = 2;
					USART1_send(PACK_out_Master, 0, lenght_pack_out_master);
					Mask_Pack_DRV2 |= MASK_PACK_4;
					id_next_device = ID_DRV1_SENSOR_ROT_FREQ_1;	
					break;			
			}
		}

		if(USART3_status() == Pack_Is_Received)
		{
			USART3_Clear_status();
			PACK_input_Slave = USART3_get(&lenght_pack_in_slave);
			ModbusRTU_Heandler_Slave(PACK_input_Slave, lenght_pack_in_slave, PACK_out_Slave, &lenght_pack_out_slave);
			USART3_send(PACK_out_Slave, 0, lenght_pack_out_slave);
		}

		if(USART1_status() == Pack_Is_Received)
		{
			USART1_Clear_status();
			PACK_input_Master = USART1_get(&lenght_pack_in_master);
			
			if(PACK_input_Master[0] == '#')
			{
				OVEN_Heandler_Master(PACK_input_Master, lenght_pack_in_master);
				
				if(OVEN_Status() == OVEN_Pack_Is_Received)
				{
					OVEN_Clear_Status();
					switch(OVEN_Get_Adress_SI8())
					{
						case ID_DRV1_SENSOR_ROT_FREQ_1:
							INPUT.REG.DRV1.RotFreqGen_1 = OVEN_Get_FlowMeter();
							break;
						case ID_DRV1_SENSOR_ROT_FREQ_2:
							INPUT.REG.DRV1.RotFreqGen_2 = OVEN_Get_FlowMeter();
							break;
						case ID_DRV2_SENSOR_ROT_FREQ_1:
							INPUT.REG.DRV2.RotFreqGen_1 = OVEN_Get_FlowMeter();
							break;
						case ID_DRV2_SENSOR_ROT_FREQ_2:
							INPUT.REG.DRV2.RotFreqGen_2 = OVEN_Get_FlowMeter();
							break;
					}
				}
			}
			else
			{
				ModbusRTU_Heandler_Master(PACK_input_Master, lenght_pack_in_master, IN_start_reg, IN_count_reg);
			}
			
			switch(ModbusRTU_Return_Result())
			{
				case ID_DRV1_SENSOR_VIBRATION_1: Mask_Pack_DRV1 &=~ MASK_PACK_3; break;
				case ID_DRV1_SENSOR_VIBRATION_2: Mask_Pack_DRV1 &=~ MASK_PACK_4; break;
				case ID_DRV2_SENSOR_VIBRATION_1: Mask_Pack_DRV2 &=~ MASK_PACK_3; break;
				case ID_DRV2_SENSOR_VIBRATION_2: Mask_Pack_DRV2 &=~ MASK_PACK_4; break;
			}
			switch(OVEN_Return_Result())
			{
				case ID_DRV1_SENSOR_ROT_FREQ_1:  Mask_Pack_DRV1 &=~ MASK_PACK_1; break;
				case ID_DRV1_SENSOR_ROT_FREQ_2:  Mask_Pack_DRV1 &=~ MASK_PACK_2; break;
				case ID_DRV2_SENSOR_ROT_FREQ_1:  Mask_Pack_DRV2 &=~ MASK_PACK_1; break;
				case ID_DRV2_SENSOR_ROT_FREQ_2:  Mask_Pack_DRV2 &=~ MASK_PACK_2; break;		
			}
			
			msec=3;
			Enable_Timer = True;

		}
		
		if(Mask_Pack_DRV1 == 0 && Mask_Pack_DRV2 == 0) Enable_count = False;
		else Enable_count = True;
		
		if(Mask_Pack_DRV1==0)
		{
			Enable_Led_Work_DRV1 = False;
			DRV1_LED_WORK_ON();
			INPUT.REG.DRV1.Error |= ERROR_NET;
		}
		
		if(Mask_Pack_DRV2==0)
		{
			Enable_Led_Work_DRV2 = False;
			DRV2_LED_WORK_ON();
			INPUT.REG.DRV2.Error |= ERROR_NET;
		}		
		
		if(USART1_status() == Transmit_Complite)
		{
			USART1_Clear_status();
			msec=20;
			Enable_Timer = True;
			LED2_OFF();
		}
		
		switch(HOLDING.REG.DRV1.Command)
		{
			case COMMAND_WRITE_FLASH:
				FLASH_unlock();
				FLASH_erase_page(ADDR_DRV1_LIMIT_TEMP);
				FLASH_write(ADDR_DRV1_LIMIT_TEMP, HOLDING.REG.DRV1.R_ptc);
				FLASH_write(ADDR_DRV1_LIMIT_RATIO_1, HOLDING.REG.DRV1.Ratio_1);
			  FLASH_write(ADDR_DRV1_LIMIT_RATIO_2, HOLDING.REG.DRV1.Ratio_2);
				FLASH_write(ADDR_DRV1_LIMIT_VIB_SPEED_1_X, HOLDING.REG.DRV1.VibSpeed_1X);
				FLASH_write(ADDR_DRV1_LIMIT_VIB_SPEED_2_X, HOLDING.REG.DRV1.VibSpeed_2X);
				FLASH_lock();
				HOLDING.REG.DRV1.Command &=~ COMMAND_WRITE_FLASH;
				break;
		}
		
		switch(HOLDING.REG.DRV2.Command)
		{
			case COMMAND_WRITE_FLASH:
				FLASH_unlock();
				FLASH_erase_page(ADDR_DRV2_LIMIT_TEMP);
				FLASH_write(ADDR_DRV2_LIMIT_TEMP, HOLDING.REG.DRV2.R_ptc);
				FLASH_write(ADDR_DRV2_LIMIT_RATIO_1, HOLDING.REG.DRV2.Ratio_1);
			  FLASH_write(ADDR_DRV2_LIMIT_RATIO_2, HOLDING.REG.DRV2.Ratio_2);
				FLASH_write(ADDR_DRV2_LIMIT_VIB_SPEED_1_X, HOLDING.REG.DRV2.VibSpeed_1X);
				FLASH_write(ADDR_DRV2_LIMIT_VIB_SPEED_2_X, HOLDING.REG.DRV2.VibSpeed_2X);
				FLASH_lock();
				HOLDING.REG.DRV2.Command &=~ COMMAND_WRITE_FLASH;
				break;
		}

		IWDG_reset();
	}
}

void SysTick_Handler()       
{
	
	if(Enable_delay) 
	{
		delay_--;
		if(delay_ == 0) Enable_delay = False;
	}
		
	
	if(Enable_Timer)
	{
		msec--;
		if(msec==0)
		{
			//msec=20;
			Enable_Timer = False;
			Enable_USART1_Send = True; 
		}
	}
	
	if(Enable_count)
	{
		count++;
		if(count == 750)
		{
			count=0;
			if(Mask_Pack_DRV1) Enable_Led_Work_DRV1 = True;
			if(Mask_Pack_DRV2) Enable_Led_Work_DRV2 = True;
			/*
			if(Mask_Pack_DRV1&MASK_PACK_1) INPUT.REG.DRV1.RotFreqGen_1 = 0;
			if(Mask_Pack_DRV1&MASK_PACK_2) INPUT.REG.DRV1.RotFreqGen_2 = 0;
			if(Mask_Pack_DRV1&MASK_PACK_3) INPUT.REG.DRV1.VibSpeed_1X = 0;
			if(Mask_Pack_DRV1&MASK_PACK_4) INPUT.REG.DRV1.VibSpeed_2X = 0;
				
			if(Mask_Pack_DRV2&MASK_PACK_1) INPUT.REG.DRV2.RotFreqGen_1 = 0;
			if(Mask_Pack_DRV2&MASK_PACK_2) INPUT.REG.DRV2.RotFreqGen_2 = 0;
			if(Mask_Pack_DRV2&MASK_PACK_3) INPUT.REG.DRV2.VibSpeed_1X = 0;
			if(Mask_Pack_DRV2&MASK_PACK_4) INPUT.REG.DRV2.VibSpeed_2X = 0;				
		*/
		}
	}
	else count=0;
	
	if(Enable_count_alarm_drv1)
	{
		count_alarm_drv1++;
		if(count_alarm_drv1 == 500)
		{
			count_alarm_drv1 = 0;
			if(Enable_LED_ALARM_DRV1) DRV1_LED_ALARM_Destruction_ON();
		}
	}
	else 
	{
		count_alarm_drv1 = 0;
		//DRV1_LED_ALARM_Destruction_OFF();
	}
	
	
	if(Enable_count_alarm_drv2)
	{
		count_alarm_drv2++;
		if(count_alarm_drv2 == 500)
		{
			count_alarm_drv2 = 0;
			if(Enable_LED_ALARM_DRV2) DRV2_LED_ALARM_Destruction_ON();
		}
	}
	else 
	{
		count_alarm_drv2 = 0;
		//DRV2_LED_ALARM_Destruction_OFF();
	}
	
		
	
	count_led_work++;
	if(count_led_work == 10)
	{
		count_led_work = 0;
		if(Led_ON) Led_ON = False;	
		else Led_ON = True;
		/*if(Enable_Led_Work_DRV1)
		{
			if(Led_ON) DRV1_LED_WORK_ON();
			else DRV1_LED_WORK_OFF();
		}
		if(Enable_Led_Work_DRV2)
		{
			if(Led_ON) DRV2_LED_WORK_ON();
			else DRV2_LED_WORK_OFF();
		}	*/
	}
	
	if(Enable_Timer_Button_DRV1)
	{
		Timer_Button_DRV1++;
		if(Timer_Button_DRV1 == 20)
		{
			Timer_Button_DRV1 = 0;
			TIM3->CR1 |= TIM_CR1_CEN;
			DRV1_RELE_ON();			
		}
	}
	
	if(Enable_Timer_Button_DRV2)
	{
		Timer_Button_DRV2++;
		if(Timer_Button_DRV2 == 20)
		{
			Timer_Button_DRV2 = 0;
			TIM3->CR1 |= TIM_CR1_CEN;
			DRV2_RELE_ON();		
		}
	}
}


void TIM3_IRQHandler(void)
{
	TIM3->SR &= ~ TIM_SR_UIF;
	TIM3->CR1 &= ~TIM_CR1_CEN;
	TIM3->CNT = 0;
	DRV1_RELE_OFF();
	DRV2_RELE_OFF();
}

