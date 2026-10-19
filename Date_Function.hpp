/*-------------------------------------------------------------------------------------------------------------
@file		: Date_Function.hpp
@author  	: Reza Farahani (nmmco2015@gmail.com)
@brief     	: Header for Date_Function.cpp file.
-------------------------------------------------------------------------------------------------------------*/

#ifndef __DATE_FUNCTION_H
#define __DATE_FUNCTION_H

//**********************************************************************************************************

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

//**********************************************************************************************************

typedef struct
			{
				int32_t iYear;
				int32_t iMonth;
				int32_t iDay;
			}ShamsiDate_TypeDef;

//**********************************************************************************************************

ShamsiDate_TypeDef MiladiToShamsi(int32_t iMiladiMonth, int32_t iMiladiDay, int32_t iMiladiYear);
ShamsiDate_TypeDef ShamsiToMiladi(int32_t y, int32_t m, int32_t d);

//**********************************************************************************************************

#ifdef __cplusplus
}
#endif

#endif /* __DATE_FUNCTION_H */

