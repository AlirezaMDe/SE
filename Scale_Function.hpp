/*-------------------------------------------------------------------------------------------------------------
@file           : Scale_Function.hpp
@author  		: Reza Farahani (nmmco2015@gmail.com)
@brief          : Header for Scale_Function.cpp file.
-------------------------------------------------------------------------------------------------------------*/

#ifndef __SCALE_FUNCTION_H
#define __SCALE_FUNCTION_H

//**********************************************************************************************************

#include "User_Defined_Type.hpp"
#include "AD7780.hpp"

//**********************************************************************************************************

class Scale_Function
{
	osMessageQueueId_t ConfigParameters_QueueHandle;
	osMessageQueueId_t Respance_QueueHandle;
	AD7780 Analog;

//----------------------------------------------------------------------------------------------------------

	StatusScale_Function_TypeDef ConfigParametersConfiguration(ConfigParametersScale_TypeDef ConfigParametersScale);
	StatusScale_Function_TypeDef Read_ScaleFunction(int32_t *ADCLoadcell);

public:

//----------------------------------------------------------------------------------------------------------

	Scale_Function(osMessageQueueId_t ConfigParametersScale_QueueHandle, osMessageQueueId_t RespanceScale_QueueHandle);

	void MainScaleFunction(void);
};

//**********************************************************************************************************

#endif /* __SCALE_FUNCTION_H */
