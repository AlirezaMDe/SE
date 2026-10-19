/*-------------------------------------------------------------------------------------------------------------
@file           : Main_Function.h
@author  		: Reza Farahani (nmmco2015@gmail.com)
@brief          : Header for Main_Function.cpp file.
-------------------------------------------------------------------------------------------------------------*/

#ifndef __MAIN_FUNCTION_H
#define __MAIN_FUNCTION_H

#ifdef __cplusplus
extern "C" {
#endif

//**********************************************************************************************************

#include "main.h"

//**********************************************************************************************************

void WeightDisplay_MainFunction(void);
void CreateQueuesWeightDisplay(void);
void Timer2_Function(void);

//----------------------------------------------------------------------------------------------------------

void WeightDisplay_TestFunction(void);
void WeightDisplay_Initalize(void);
void WeightDisplay_ShowInformation(void);

void WeightDisplay_BasePageFunction(void);
void WeightDisplay_MenuFunction(void);
void WeightDisplay_Memory_Function(void);

//**********************************************************************************************************

#ifdef __cplusplus
}
#endif
#endif /* __MAIN_FUNCTION_H */
