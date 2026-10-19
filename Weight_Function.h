/*-------------------------------------------------------------------------------------------------------------
@file           : Weight_Function.h
@author  		: Reza Farahani (nmmco2015@gmail.com)
@brief          : Header for Weight_Function.cpp file.
-------------------------------------------------------------------------------------------------------------*/

#ifndef __WEIGHT_FUNCTION_H
#define __WEIGHT_FUNCTION_H

//**********************************************************************************************************

#include "User_Defined_Type.hpp"

//**********************************************************************************************************

class Weight_Function
{
	CalibrationParameters_TypeDef CalibrationParameters;
	osMessageQueueId_t RespanceScale_01_QueueHandle;
	osMessageQueueId_t RespanceScale_02_QueueHandle;
	int32_t ADC_Sample[32];
	uint8_t Counter_ADC_Sample = 0;
	uint8_t DigitalFilter;
	int32_t FirstRange;
	int32_t SecondRange;
	int32_t OverRange;
	int32_t ZeroRange;
	int32_t ZeroWeight = 0;
	int32_t TareWeight = 0;
	int32_t LastWeightMotionSamples = 0;
	float LastHighResolution = 0;
	uint8_t MotionSamples = 0;
	uint8_t CountErrorScal_01_Function = 0;
	uint8_t CountErrorScal_02_Function = 0;
	uint8_t CountErrorAD7780_Scal_01_Function = 0;
	uint8_t CountErrorAD7780_Scal_02_Function = 0;

//----------------------------------------------------------------------------------------------------------

public:

	void ConfigParametersConfiguration(CalibrationParameters_TypeDef Scale_CalibrationParameters, osMessageQueueId_t Scale_RespanceScale_01_QueueHandle, osMessageQueueId_t Scale_RespanceScale_02_QueueHandle);

	void SetZeroWeight(int32_t Weight);
	void SetTareWeight(int32_t Weight);

	StatusZeroTrack_TypeDef ZeroTracking(void);
	void AutoZeroTracking(void);
	StatusWeightTaring_TypeDef WeightTaring(void);

	StatusWeight_Function_TypeDef GetADCScale(int32_t *ADC_Scale);
	StatusWeight_Function_TypeDef GetADCCalibration(int32_t *ADCCalibration);
	StatusWeight_Function_TypeDef GetWeightScale(WeightScale_TypeDef *WeightScale);

	uint8_t GetScaleDivision(uint8_t ScaleDivision);
	int32_t GetTareValue(void);
};

//**********************************************************************************************************

#endif /* __WEIGHT_FUNCTION_H */
