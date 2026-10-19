/*-----------------------------------------------------------------------------------------------------------
@file     	: Date_Function.cpp
@author  	: Reza Farahani (nmmco2015@gmail.com)
C++ source 2024
-----------------------------------------------------------------------------------------------------------*/

#include"Date_Function.hpp"
#include<stdio.h>
#include<math.h>

//**********************************************************************************************************

bool MiladiIsLeap(int32_t miladiYear)
{
	if(((miladiYear % 100) != 0 && (miladiYear % 4) == 0)
		|| ((miladiYear % 100) == 0 && (miladiYear % 400) == 0))
		return true;
	else
		return false;
}

//**********************************************************************************************************

int32_t DoomsDay(int32_t year)
{
	int32_t a = 0;

	if(year >= 1800 && year <= 1899)
		a = 5;
	else if(year >= 1900 && year <= 1999)
		a = 3;
	else if(year >= 2000 && year <= 2099)
		a = 2;
	else if(year >= 2100 && year <= 2199)
		a = 0;
	else
		return -1;

	int32_t y = year % 100;
	int32_t z = (y + floor(y / 4) + a);
	z = z % 7;

	return(z);
}

//**********************************************************************************************************

int32_t GetDayOfWeek(int32_t y, int32_t m, int32_t d)
{
  int32_t doom;

  doom = DoomsDay(y);
  int32_t AllDoomsy[13] = {-1,3,7,7,4,2,6,4,1,5,3,7,5};
  int32_t c = AllDoomsy[m];

  if((m == 1 || m == 2) && MiladiIsLeap(y) == 1)
	  c++;

  int32_t r = d - c + doom;
  if(r < 0)
	  r = r + 7;

  return(r%7);
}

//**********************************************************************************************************

ShamsiDate_TypeDef MiladiToShamsi(int32_t iMiladiMonth, int32_t iMiladiDay, int32_t iMiladiYear)
{
    int32_t shamsiDay, shamsiMonth, shamsiYear;
    int32_t dayCount, farvardinDayDiff, deyDayDiff ;
    int32_t sumDayMiladiMonth[] = {0,31,59,90,120,151,181,212,243,273,304,334};
    int32_t sumDayMiladiMonthLeap[] = {0,31,60,91,121,152,182,213,244,274,305,335};
    ShamsiDate_TypeDef shamsidate;

    farvardinDayDiff = 79;

    if (MiladiIsLeap(iMiladiYear))
    	dayCount = sumDayMiladiMonthLeap[iMiladiMonth-1] + iMiladiDay;
    else
    	dayCount = sumDayMiladiMonth[iMiladiMonth-1] + iMiladiDay;

    if((MiladiIsLeap(iMiladiYear - 1)))
    	deyDayDiff = 11;
    else
    	deyDayDiff = 10;

    if (dayCount > farvardinDayDiff)
    {
    	dayCount = dayCount - farvardinDayDiff;
    	if (dayCount <= 186)
    	{
    		switch (dayCount%31)
    		{
    			case 0:
    				shamsiMonth = dayCount / 31;
    				shamsiDay = 31;
    				break;
    			default:
    				shamsiMonth = (dayCount / 31) + 1;
    				shamsiDay = (dayCount%31);
    				break;
    		}

    		shamsiYear = iMiladiYear - 621;
    	}
    	else
    	{
    		dayCount = dayCount - 186;
    		switch (dayCount%30)
    		{
    			case 0:
    				shamsiMonth = (dayCount / 30) + 6;
    				shamsiDay = 30;
    				break;
    			default:
    				shamsiMonth = (dayCount / 30) + 7;
    				shamsiDay = (dayCount%30);
    				break;
    		}

    		shamsiYear = iMiladiYear - 621;
    	}
    }
    else
    {
    	dayCount = dayCount + deyDayDiff;

    	switch (dayCount%30)
    	{
    		case 0 :
    			shamsiMonth = (dayCount / 30) + 9;
    			shamsiDay = 30;
    			break;
    		default:
    			shamsiMonth = (dayCount / 30) + 10;
    			shamsiDay = (dayCount%30);
    			break;
    	}

    	shamsiYear = iMiladiYear - 622;

    }

    shamsidate.iYear = shamsiYear;
    shamsidate.iMonth = shamsiMonth;
    shamsidate.iDay = shamsiDay;

    return shamsidate ;
}

//**********************************************************************************************************

ShamsiDate_TypeDef ShamsiToMiladi(int32_t y, int32_t m, int32_t d)
{
	int32_t sumshamsi[] = { 31, 62, 93, 124, 155, 186, 216, 246, 276, 306, 336, 365 };
	int32_t miladidays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int32_t yy, mm, daycount;
	daycount = d;

	if(m > 1)
		daycount = daycount + sumshamsi[m - 2];

	yy = y + 621;
	daycount = daycount + 79;

	if(MiladiIsLeap(yy))
	{
		if (daycount > 366)
        {
            daycount -= 366;
            yy++;
        }
	}
	else if(daycount > 365)
	{
		daycount -= 365;
        yy++;
	}

	if (MiladiIsLeap(yy))
		miladidays[1] = 29;

	mm = 0;

	while (daycount > miladidays[mm])
	{
		daycount = daycount - miladidays[mm];
		mm++;
	}

	ShamsiDate_TypeDef miladiDate;
	miladiDate.iYear= yy;
	miladiDate.iMonth=(mm + 1);
	miladiDate.iDay=daycount;

	return miladiDate;
}

//**********************************************************************************************************

