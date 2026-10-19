/*-------------------------------------------------------------------------------------------------------------
@file           : General_Functions.h
@author  		: Reza Farahani (nmmco2015@gmail.com)
@brief          : Header for General_Functions.cpp file.
-------------------------------------------------------------------------------------------------------------*/

#ifndef __GENERAL_FUNCTIONS_H
#define __GENERAL_FUNCTIONS_H

//**********************************************************************************************************

#include "User_Defined_Type.hpp"

//**********************************************************************************************************

#define AMOUNT_BLINKER_GET_NUMBER			10
#define AMOUNT_BLINKER_SET_PARAMETERS		5

//**********************************************************************************************************

string IntegerToString(int32_t Integer, uint8_t NumberOfDigits = 0, bool FillZero = true);
string FloatToString(float Float, uint8_t NumberOfDigits, uint8_t Decimal_Point, bool FillZero = true);
string WeightToString(int32_t Weight, uint8_t NumberOfDigits, uint8_t Decimal_Point, bool FillZero = true);

string GetStringInteger(PanelKeypadButton_TypeDef *LastPressButton, const string StringNumberInput, bool IsPassword = false);
string GetStringFloat(PanelKeypadButton_TypeDef *LastPressButton, const string StringFloatInput);
string GetStringWeight(PanelKeypadButton_TypeDef *LastPressButton, int32_t Weight, uint8_t NumberOfDigits = 0, uint8_t Decimal_Point = 0);
string GetStringCharacter(PanelKeypadButton_TypeDef *LastPressButton, const string StringCharInput = "", uint8_t LengthString = 40, uint8_t KeyInTime = 5);

string GetStringDateSystem(void);
string GetStringPersianDateSystem(void);
string GetStringTimeSystem(bool WithSeconds = false);
string GetStringPersianTimeSystem(bool WithSeconds);

void ErrorSystemReport(ErrorSystem_TypeDef ErrorSystem);
ErrorSystem_TypeDef GetErrorSystem(void);
void ErrorSystemReset(ErrorSystem_TypeDef ErrorSystem);

void ShowFillPassword(void);
void ShowSaveParameters(void);
void ShowMessageError(string MessageError);

string GetStringDisplayExternal(WeightScale_TypeDef WeightScale, uint8_t Format);

//**********************************************************************************************************

#endif /* __GENERAL_FUNCTIONS_H */

