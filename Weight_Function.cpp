/*-----------------------------------------------------------------------------------------------------------
@file          : Weight_Function.cpp
@author  	   : Reza Farahani (nmmco2015@gmail.com)
C++ source 2023
-----------------------------------------------------------------------------------------------------------*/

#include "Weight_Function.h"
#include "General_Functions.hpp"
#include "GLCD_TextFunction.h"
#include "Panel.hpp"

//**********************************************************************************************************

void Weight_Function::ConfigParametersConfiguration(CalibrationParameters_TypeDef Scale_CalibrationParameters, osMessageQueueId_t Scale_RespanceScale_01_QueueHandle, osMessageQueueId_t Scale_RespanceScale_02_QueueHandle)
{
	CalibrationParameters = Scale_CalibrationParameters;
	RespanceScale_01_QueueHandle = Scale_RespanceScale_01_QueueHandle;
	RespanceScale_02_QueueHandle = Scale_RespanceScale_02_QueueHandle;
	DigitalFilter = pow(2.0, (double) CalibrationParameters.ScaleSettingParameters.DIGFilter);
	OverRange = CalibrationParameters.ScaleSettingParameters.WEIGRange;

	if(CalibrationParameters.ScaleSettingParameters.ZeroRange)
		ZeroRange = OverRange * 0.02;
	else
		ZeroRange = OverRange * 0.1;
}

//**********************************************************************************************************

StatusWeight_Function_TypeDef Weight_Function::GetADCScale(int32_t *ADC_Scale)
{
	RespanceScale_TypeDef RespanceScale_01;
	RespanceScale_TypeDef RespanceScale_02;
	int32_t Total_ADC = 0;

	if(osMessageQueueGet(RespanceScale_01_QueueHandle, (uint8_t *) &RespanceScale_01, NULL, 100) == osOK)
	{
		if(RespanceScale_01.Status == OK_SCALE_FUNCTION)
		{
			CountErrorAD7780_Scal_01_Function = 0;
			CountErrorScal_01_Function = 0;

			if(++Counter_ADC_Sample >= DigitalFilter)
				Counter_ADC_Sample = 0;

			ADC_Sample[Counter_ADC_Sample] = RespanceScale_01.Respance;

			for(int i = 0; i < DigitalFilter; i++)
				Total_ADC += ADC_Sample[i];

			*ADC_Scale = Total_ADC / DigitalFilter;
			return OK_WEIGHT_FUNCTION;
		}
		else if(RespanceScale_01.Status == ERROR_AD7780_SCALE_FUNCTION)
		{
			if(++CountErrorAD7780_Scal_01_Function > 10)
				return ERROR_AD7780_SCALE_01_WEIGHT_FUNCTION;
		}
		else if(RespanceScale_01.Status == ERROR_SCALE_FUNCTION)
		{
			if(++CountErrorScal_01_Function > 10)
				return ERROR_SCALE_01_WEIGHT_FUNCTION;
		}
	}

	if(CalibrationParameters.ScaleSettingParameters.SCALE_02_ENABLE)
	{
		if(osMessageQueueGet(RespanceScale_02_QueueHandle, (uint8_t *) &RespanceScale_02, NULL, 100) == osOK)
		{
			if(RespanceScale_02.Status == OK_SCALE_FUNCTION)
			{
				CountErrorAD7780_Scal_02_Function = 0;
				CountErrorScal_02_Function = 0;

				if(++Counter_ADC_Sample >= DigitalFilter)
					Counter_ADC_Sample = 0;

				ADC_Sample[Counter_ADC_Sample] = RespanceScale_02.Respance;

				for(int i = 0; i < DigitalFilter; i++)
					Total_ADC += ADC_Sample[i];

				*ADC_Scale = Total_ADC / DigitalFilter;
				return OK_WEIGHT_FUNCTION;
			}
			else if(RespanceScale_02.Status == ERROR_AD7780_SCALE_FUNCTION)
			{
				if(++CountErrorAD7780_Scal_02_Function > 10)
					return ERROR_AD7780_SCALE_02_WEIGHT_FUNCTION;
			}
			else if(RespanceScale_02.Status == ERROR_SCALE_FUNCTION)
			{
				if(++CountErrorScal_02_Function > 10)
					return ERROR_SCALE_02_WEIGHT_FUNCTION;
			}
		}
	}

	return NON_WEIGHT_FUNCTION;
}

//**********************************************************************************************************

StatusWeight_Function_TypeDef Weight_Function::GetADCCalibration(int32_t *ADCCalibration)
{
	int32_t ADC_ScaleSample[123] = {0};
	int32_t Total_ADC = 0;
	uint8_t Counter_ADC_Scale = 0;
	int32_t ADC_Scale = 0;
	StatusWeight_Function_TypeDef StatusWeight_Function;

//----------------------------------------------------------------------------------------------------------

	while(1)
	{
		StatusWeight_Function = GetADCScale(&ADC_Scale);

		if(StatusWeight_Function == NON_WEIGHT_FUNCTION)
			continue;
		else if(StatusWeight_Function != OK_WEIGHT_FUNCTION)
			return StatusWeight_Function;

//----------------------------------------------------------------------------------------------------------

		GLCD_FillRectangle(45, 14, 194, 58, BLACK);
		GLCD_FillRectangle(47, 16, 192, 56, WHITE);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_12X11);
		SetTextPixelPosition(85, 25);
		PutsString("لطفا صبر کنید...");

		GLCD_FillRectangle(57, 42, 182, 51, BLACK);
		GLCD_FillRectangle((58 + Counter_ADC_Scale), 43, 181, 50, WHITE);

		GLCD_Screen_Update();

//----------------------------------------------------------------------------------------------------------

		ADC_ScaleSample[Counter_ADC_Scale] = ADC_Scale;
		Counter_ADC_Scale++;

		if(Counter_ADC_Scale >= 123)
		{
			for(int i = 0; i < 123; i++)
				Total_ADC += ADC_ScaleSample[i];

			*ADCCalibration = Total_ADC / 123;
			return OK_WEIGHT_FUNCTION;
		}
	}
}

//**********************************************************************************************************

StatusWeight_Function_TypeDef Weight_Function::GetWeightScale(WeightScale_TypeDef *WeightScale)
{
	StatusWeight_Function_TypeDef StatusWeight_Function;
	int32_t ADC_Scale = 0;
	float RawWeight;
	float GrossWeight;
	float HighResolution;
	uint8_t ScaleDivision;
	float WeightLoadCell;

	StatusWeight_Function = GetADCScale(&ADC_Scale);

	if(StatusWeight_Function != OK_WEIGHT_FUNCTION)
	{
		if(StatusWeight_Function != NON_WEIGHT_FUNCTION)
			WeightScale->IsError = true;

		return StatusWeight_Function;
	}
	else
		WeightScale->IsError = false;

//----------------------------------------------------------------------------------------------------------

	if(CalibrationParameters.ScaleSettingParameters.AutoZTrack != 0)
		WeightScale->IsAutoZero = true;
	else
		WeightScale->IsAutoZero = false;

//----------------------------------------------------------------------------------------------------------

	WeightScale->DISPDigits = CalibrationParameters.ScaleSettingParameters.DISPDigits;
	WeightScale->DECPoint = CalibrationParameters.ScaleSettingParameters.DECPoint;
	WeightScale->LeadZBlank = CalibrationParameters.ScaleSettingParameters.LeadZBlank;

//----------------------------------------------------------------------------------------------------------

	WeightScale->ADC_Scale = ADC_Scale;
	RawWeight = (WeightScale->ADC_Scale - CalibrationParameters.DeadLoad) / CalibrationParameters.Alpha;

//----------------------------------------------------------------------------------------------------------
//Zero Scale

	GrossWeight = RawWeight - ZeroWeight;

//----------------------------------------------------------------------------------------------------------

	WeightScale->Raw = round(RawWeight);
	WeightScale->Gross = round(GrossWeight);

//----------------------------------------------------------------------------------------------------------
//Taring

	if(TareWeight == 0)
	{
		HighResolution = GrossWeight;
		WeightScale->IsNet = false;
	}
	else
	{
		HighResolution = GrossWeight - TareWeight;
		WeightScale->IsNet = true;
	}

//----------------------------------------------------------------------------------------------------------

	WeightScale->HighResolution = round(HighResolution);

//----------------------------------------------------------------------------------------------------------
//Scale Division

	ScaleDivision = GetScaleDivision((uint8_t)CalibrationParameters.ScaleSettingParameters.ScaleDIV);
	WeightScale->WeightRange = 0;

	if(abs(LastHighResolution - HighResolution) < (ScaleDivision / 5))
		HighResolution = LastHighResolution;
	else
		LastHighResolution = HighResolution;

	HighResolution = HighResolution / ScaleDivision;
	HighResolution = round(HighResolution);
	HighResolution *= ScaleDivision;

//----------------------------------------------------------------------------------------------------------

	WeightScale->Normally = HighResolution;

//----------------------------------------------------------------------------------------------------------
//Motion Samples

	if(LastWeightMotionSamples == WeightScale->Normally)
	{
		if(MotionSamples < 8)
			MotionSamples++;
	}
	else
	{
		MotionSamples = 0;
		LastWeightMotionSamples = WeightScale->Normally;
	}

	if(MotionSamples >= (uint8_t)CalibrationParameters.ScaleSettingParameters.MotionSamples)
		WeightScale->IsStable = true;
	else
		WeightScale->IsStable = false;

//----------------------------------------------------------------------------------------------------------
//Under Range

//	if(ADC_Scale < 0)
//		WeightScale->IsUnderRange = true;
//	else
		WeightScale->IsUnderRange = false;

//----------------------------------------------------------------------------------------------------------
//Over Range

	if(WeightScale->Normally > OverRange)
		WeightScale->IsOverRange = true;
	else
		WeightScale->IsOverRange = false;

//----------------------------------------------------------------------------------------------------------
//Weight Zero

	if((WeightScale->IsStable) && (!WeightScale->IsUnderRange) && (WeightScale->Normally == 0))
		WeightScale->IsZero = true;
	else
		WeightScale->IsZero = false;

//----------------------------------------------------------------------------------------------------------

	return OK_WEIGHT_FUNCTION;
}

//**********************************************************************************************************

void Weight_Function::SetZeroWeight(int32_t Weight)
{
	ZeroWeight = Weight;
}

//**********************************************************************************************************

void Weight_Function::SetTareWeight(int32_t Weight)
{
	TareWeight = Weight;
}

//**********************************************************************************************************

uint8_t Weight_Function::GetScaleDivision(uint8_t ScaleDivision)
{
	switch(ScaleDivision)
	{
		case 0:
			return 1;
		case 1:
			return 2;
		case 2:
			return 5;
		case 3:
			return 10;
		case 4:
			return 20;
		case 5:
			return 50;
		case 6:
			return 100;
		case 7:
			return 200;
	}

	return 1;
}

//**********************************************************************************************************

StatusZeroTrack_TypeDef Weight_Function::ZeroTracking(void)
{
	StatusWeight_Function_TypeDef StatusWeight_Function;
	WeightScale_TypeDef WeightScale = {0};

	StatusWeight_Function = GetWeightScale(&WeightScale);

	if(StatusWeight_Function != OK_WEIGHT_FUNCTION)
		return ERROR_ZERO_TRACK;

	if((!WeightScale.IsError) && (!WeightScale.IsZero) && (!WeightScale.IsUnderRange) && (!WeightScale.IsOverRange) && (!WeightScale.IsNet) && (WeightScale.IsStable) && (abs(WeightScale.HighResolution) <= ZeroRange))
	{
		SetZeroWeight(WeightScale.Raw);
		return OK_ZERO_TRACK;
	}
	else
		return ERROR_ZERO_TRACK;
}

//**********************************************************************************************************

void Weight_Function::AutoZeroTracking(void)
{
	StatusWeight_Function_TypeDef StatusWeight_Function;
	WeightScale_TypeDef WeightScale = {0};

	StatusWeight_Function = GetWeightScale(&WeightScale);

	if(StatusWeight_Function != OK_WEIGHT_FUNCTION)
		return;

	if((!WeightScale.IsError) && (!WeightScale.IsUnderRange) && (!WeightScale.IsNet) && (WeightScale.IsStable) && (abs(WeightScale.HighResolution) <= CalibrationParameters.ScaleSettingParameters.AutoZTrack))
		SetZeroWeight(WeightScale.Raw);
}

//**********************************************************************************************************

StatusWeightTaring_TypeDef Weight_Function::WeightTaring(void)
{
	StatusWeight_Function_TypeDef StatusWeight_Function;
	WeightScale_TypeDef WeightScale = {0};

	StatusWeight_Function = GetWeightScale(&WeightScale);

	if(StatusWeight_Function != OK_WEIGHT_FUNCTION)
		return ERROR_WEIGHT_TARING;

	if((!WeightScale.IsError) && (!WeightScale.IsZero) && (!WeightScale.IsUnderRange) && (!WeightScale.IsOverRange) && (!WeightScale.IsNet) && (WeightScale.IsStable) && (WeightScale.Gross > 0))
	{
		SetTareWeight(WeightScale.Gross);
		return OK_WEIGHT_TARING;
	}
	else
		return ERROR_WEIGHT_TARING;
}

//**********************************************************************************************************

int32_t Weight_Function::GetTareValue(void)
{
	return TareWeight;
}

//**********************************************************************************************************


