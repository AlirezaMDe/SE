/*-----------------------------------------------------------------------------------------------------------
 @file          : Scale_Function.cpp
 @author  		: Reza Farahani (nmmco2015@gmail.com)
 C++ source 2024
 -----------------------------------------------------------------------------------------------------------*/

#include "Scale_Function.hpp"

//**********************************************************************************************************

Scale_Function::Scale_Function(osMessageQueueId_t ConfigParametersScale_QueueHandle, osMessageQueueId_t RespanceScale_QueueHandle)
{
	ConfigParameters_QueueHandle = ConfigParametersScale_QueueHandle;
	Respance_QueueHandle = RespanceScale_QueueHandle;
}

//**********************************************************************************************************

StatusScale_Function_TypeDef Scale_Function::ConfigParametersConfiguration(ConfigParametersScale_TypeDef ConfigParametersScale)
{
	CommunicationParameters_AD7780_TypeDef CommunicationParameters_AD7780;

	CommunicationParameters_AD7780.SPI_Port = ConfigParametersScale.SPI_Port;
	CommunicationParameters_AD7780.GPIO_Port_DataReady = ConfigParametersScale.GPIO_Port_DataReady;
	CommunicationParameters_AD7780.GPIO_Pin_DataReady = ConfigParametersScale.GPIO_Pin_DataReady;
	CommunicationParameters_AD7780.GPIO_Port_Reset = ConfigParametersScale.GPIO_Port_Reset;
	CommunicationParameters_AD7780.GPIO_Pin_Reset = ConfigParametersScale.GPIO_Pin_Reset;

	Analog.CommunicationParametersConfiguration(CommunicationParameters_AD7780);

	return OK_SCALE_FUNCTION;
}

//**********************************************************************************************************

StatusScale_Function_TypeDef Scale_Function::Read_ScaleFunction(int32_t *ADCLoadcell)
{
	Status_AD7780_TypeDef Status_AD7780;
	int32_t ADC_AD7780;
	uint8_t Count_NoReady = 0;

	do
	{
		Status_AD7780 = Analog.Read_ContinuousConversionMode(&ADC_AD7780);
		if(Status_AD7780 == NO_READY_DATA_AD7780)
		{
			if(++Count_NoReady >= 110)
				return ERROR_SCALE_FUNCTION;

			DELAY_MS(1);
		}
	}
	while(Status_AD7780 == NO_READY_DATA_AD7780);

	if(Status_AD7780 == OK_AD7780)
	{
		*ADCLoadcell = ADC_AD7780;
		return OK_SCALE_FUNCTION;
	}
	else if(Status_AD7780 == ERROR_PORT_AD7780)
		return ERROR_AD7780_SCALE_FUNCTION;
	else
		return ERROR_SCALE_FUNCTION;
}

//**********************************************************************************************************

void Scale_Function::MainScaleFunction(void)
{
	ConfigParametersScale_TypeDef ConfigParametersScale;
	RespanceScale_TypeDef RespanceScale;

	if(osMessageQueueGet(ConfigParameters_QueueHandle, (uint8_t *) &ConfigParametersScale, NULL, portMAX_DELAY) == osOK)
		ConfigParametersConfiguration(ConfigParametersScale);

	while(1)
	{
		RespanceScale.Status = Read_ScaleFunction(&RespanceScale.Respance);
		osMessageQueuePut(Respance_QueueHandle, (uint8_t *) &RespanceScale, osPriorityNormal, 0);
		DELAY_MS(100);
	}
}

//**********************************************************************************************************


