/*-----------------------------------------------------------------------------------------------------------
 @file          : AD7780.cpp
 @author  		: Reza Farahani (nmmco2015@gmail.com)
 C++ source 2024
 -----------------------------------------------------------------------------------------------------------*/

#include "AD7780.hpp"
#include "cmsis_os.h"

//**********************************************************************************************************

void AD7780::CommunicationParametersConfiguration(CommunicationParameters_AD7780_TypeDef CommunicationParameters_AD7780)
{
	CommunicationParameters.SPI_Port = CommunicationParameters_AD7780.SPI_Port;
	CommunicationParameters.GPIO_Port_DataReady = CommunicationParameters_AD7780.GPIO_Port_DataReady;
	CommunicationParameters.GPIO_Pin_DataReady = CommunicationParameters_AD7780.GPIO_Pin_DataReady;
	CommunicationParameters.GPIO_Port_Reset = CommunicationParameters_AD7780.GPIO_Port_Reset;
	CommunicationParameters.GPIO_Pin_Reset = CommunicationParameters_AD7780.GPIO_Pin_Reset;

	Reset();
}

//**********************************************************************************************************

void AD7780::Reset(void)
{
	HAL_GPIO_WritePin(CommunicationParameters.GPIO_Port_Reset, CommunicationParameters.GPIO_Pin_Reset, GPIO_PIN_RESET);
	DELAY_MS(100);
	HAL_GPIO_WritePin(CommunicationParameters.GPIO_Port_Reset, CommunicationParameters.GPIO_Pin_Reset, GPIO_PIN_SET);
	DELAY_MS(1000);
}

//**********************************************************************************************************

bool AD7780::IsReadyData(void)
{
	if(HAL_GPIO_ReadPin(CommunicationParameters.GPIO_Port_DataReady, CommunicationParameters.GPIO_Pin_DataReady) == GPIO_PIN_RESET)
		return true;
	else
		return false;
}

//**********************************************************************************************************

Status_AD7780_TypeDef AD7780::Read_ContinuousConversionMode(int32_t *ADCValue)
{
	uint32_t AD7780_ADC;
	uint8_t Data[4];

	if(IsReadyData())
	{
		if (HAL_SPI_Receive(CommunicationParameters.SPI_Port, Data, 4, TIME_OUT_AD7780) != HAL_OK)
			return ERROR_PORT_AD7780;

		if((Data[3] & 0x80) == 0x80)
			return NO_READY_DATA_AD7780;
		else if((Data[3] & 0xFB) == 0x49)
		{
			AD7780_ADC = (uint32_t)Data[0] << 16;
			AD7780_ADC |= (uint32_t)Data[1] << 8;
			AD7780_ADC |= (uint32_t)Data[2];

			*ADCValue = (float)AD7780_ADC - 0x800000;
			return OK_AD7780;
		}
		else
			return ERROR_AD7780;
	}
	else
		return NO_READY_DATA_AD7780;
}

//**********************************************************************************************************

