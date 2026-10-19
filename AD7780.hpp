/*-------------------------------------------------------------------------------------------------------------
@file           : AD7780.hpp
@author  		: Reza Farahani (nmmco2015@gmail.com)
@brief          : Header for AD7780.cpp file.
-------------------------------------------------------------------------------------------------------------*/

#ifndef __AD7780_H
#define __AD7780_H

//**********************************************************************************************************

#include "main.h"

//**********************************************************************************************************

#define DELAY_MS(Delay_mS)			osDelay(Delay_mS)

//**********************************************************************************************************

#define TIME_OUT_AD7780    		1000

//**********************************************************************************************************

typedef struct
{
	SPI_HandleTypeDef *SPI_Port;
	GPIO_TypeDef *GPIO_Port_Reset;
	uint16_t GPIO_Pin_Reset;
	GPIO_TypeDef *GPIO_Port_DataReady;
	uint16_t GPIO_Pin_DataReady;
}CommunicationParameters_AD7780_TypeDef;

typedef enum
{
	ERROR_PORT_AD7780,
	ERROR_AD7780,
	NO_READY_DATA_AD7780,
	OK_AD7780
}Status_AD7780_TypeDef;

//**********************************************************************************************************

class AD7780
{
	CommunicationParameters_AD7780_TypeDef CommunicationParameters;

//----------------------------------------------------------------------------------------------------------

	void Reset(void);
	bool IsReadyData(void);

//----------------------------------------------------------------------------------------------------------

public:

	void CommunicationParametersConfiguration(CommunicationParameters_AD7780_TypeDef CommunicationParameters_AD7780);

	Status_AD7780_TypeDef Read_ContinuousConversionMode(int32_t *ADCValue);
};

//**********************************************************************************************************

#endif /* __AD7780_H */