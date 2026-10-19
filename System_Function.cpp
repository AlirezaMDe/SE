/*-----------------------------------------------------------------------------------------------------------
 @file          : System_Function.cpp
 @author  		: Reza Farahani (nmmco2015@gmail.com)
 C++ source 2024
 -----------------------------------------------------------------------------------------------------------*/

#include "System_Function.hpp"
#include "General_Functions.hpp"
#include "Memory_Functions.hpp"
#include "Weight_Function.h"
#include "Date_Function.hpp"
#include "GLCD_TextFunction.h"
#include "Printer.hpp"
#include "Panel.hpp"
#include "w25qxx.h"

//**********************************************************************************************************

extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;

extern UART_HandleTypeDef huart2; 		//Rs232_02
extern UART_HandleTypeDef huart3; 		//Rs485

extern RTC_HandleTypeDef hrtc;

//**********************************************************************************************************

extern osMessageQueueId_t ConfigParametersScale_01_QueueHandle;
extern osMessageQueueId_t ConfigParametersScale_02_QueueHandle;
extern osMessageQueueId_t RespanceScale_01_QueueHandle;
extern osMessageQueueId_t RespanceScale_02_QueueHandle;

//**********************************************************************************************************

SystemParameters_TypeDef SystemParameters = {0};

//----------------------------------------------------------------------------------------------------------

Weight_Function Weight_Scale;

//**********************************************************************************************************

void SystemFunction_InitializeSystemParameters(void)
{
	if(ReadInitializeValue() != 0x20)
	{
		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_9X8);
		SetTextPixelPosition(81, 2);
		PutsString("پیکربندی سیستم");

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
		GLCD_FillRectangle((58 + 0), 43, 181, 50, WHITE);

		GLCD_Screen_Update();

//----------------------------------------------------------------------------------------------------------

		SaveTicketSerialNumber(1);
		DELAY_MS(100);

		GLCD_FillRectangle(57, 42, 182, 51, BLACK);
		GLCD_FillRectangle((58 + 20), 43, 181, 50, WHITE);

		GLCD_Screen_Update();

//----------------------------------------------------------------------------------------------------------

		strcpy(SystemParameters.PinSystem, DEFAULT_PASSWORD);
		strcpy(SystemParameters.PinOperator, DEFAULT_PASSWORD);

		SystemFunction_ResetScaleSettingParameters();
		SystemParameters.CalibrationParameters.DeadLoad = 0;
		SystemParameters.CalibrationParameters.Alpha = 1000.0;

		SystemParameters.COM1.COM_Port = &huart2;
		SystemParameters.COM2.COM_Port = &huart3;
		SystemFunction_ResetCommunicationParameters(&SystemParameters.COM1);
		SystemFunction_ResetCommunicationParameters(&SystemParameters.COM2);

		SystemFunction_ResetWeighingCost();
		SystemFunction_ResetPrintFormatParameters();

		SaveSystemParameters(SystemParameters);
		DELAY_MS(100);

		GLCD_FillRectangle(57, 42, 182, 51, BLACK);
		GLCD_FillRectangle((58 + 40), 43, 181, 50, WHITE);

		GLCD_Screen_Update();

//----------------------------------------------------------------------------------------------------------

		ProductParameters_TypeDef ProductParameters;
		strcpy(ProductParameters.Name, "");
		ProductParameters.Price = 0;

		for(int i = 0; i < MAX_PRODUCT_CODE; i++)
			SaveProductCode(ProductParameters , i);

		GLCD_FillRectangle(57, 42, 182, 51, BLACK);
		GLCD_FillRectangle((58 + 60), 43, 181, 50, WHITE);

		GLCD_Screen_Update();

//----------------------------------------------------------------------------------------------------------

		CustomerParameters_TypeDef CustomerParameters;
		strcpy(CustomerParameters.Name, "");

		for(int i = 0; i < MAX_CUSTOMER_CODE; i++)
			SaveCustomerParameters(CustomerParameters , i);

		GLCD_FillRectangle(57, 42, 182, 51, BLACK);
		GLCD_FillRectangle((58 + 80), 43, 181, 50, WHITE);

		GLCD_Screen_Update();

//----------------------------------------------------------------------------------------------------------

		CertainTrucksParameters_TypeDef CertainTrucksParameters;
		strcpy(CertainTrucksParameters.NumberPlates, "");
		CertainTrucksParameters.EmptyWeight = 0;

		for(int i = 0; i < MAX_CERTAIN_TRUCKS; i++)
			SaveCertainTrucksParameters(CertainTrucksParameters, i);

		GLCD_FillRectangle(57, 42, 182, 51, BLACK);
		GLCD_FillRectangle((58 + 100), 43, 181, 50, WHITE);

		GLCD_Screen_Update();

//----------------------------------------------------------------------------------------------------------

		W25qxx_EraseChip();
		GLCD_FillRectangle(57, 42, 182, 51, BLACK);
		GLCD_FillRectangle((58 + 123), 43, 181, 50, WHITE);

		GLCD_Screen_Update();

//----------------------------------------------------------------------------------------------------------

		SaveInitializeValue(0x20);
	}

//----------------------------------------------------------------------------------------------------------

	SystemParameters = ReadSystemParameters();

//----------------------------------------------------------------------------------------------------------

	SystemFunction_InitializeCommunicationParameters(SystemParameters.COM1);
	SystemFunction_InitializeCommunicationParameters(SystemParameters.COM2);

//----------------------------------------------------------------------------------------------------------

	Printer_SetPrinterType(SystemParameters.PrintFormatParameters.TypePrinter , SystemParameters.PrintFormatParameters.TypePaper);
	
//----------------------------------------------------------------------------------------------------------

	SystemFunction_SetConfigParametersScale_01();
	SystemFunction_SetConfigParametersScale_02();

//----------------------------------------------------------------------------------------------------------

	Weight_Scale.ConfigParametersConfiguration(SystemParameters.CalibrationParameters, RespanceScale_01_QueueHandle , RespanceScale_02_QueueHandle);
}

//**********************************************************************************************************

void SystemFunction_ResetScaleSettingParameters(void)
{
	SystemParameters.CalibrationParameters.ScaleSettingParameters.DISPDigits = 5;
	SystemParameters.CalibrationParameters.ScaleSettingParameters.DECPoint  = 0;
	SystemParameters.CalibrationParameters.ScaleSettingParameters.ScaleDIV = 5;
	SystemParameters.CalibrationParameters.ScaleSettingParameters.WEIGRange = 60000;
	SystemParameters.CalibrationParameters.ScaleSettingParameters.DIGFilter = 3;
	SystemParameters.CalibrationParameters.ScaleSettingParameters.MotionSamples = 3;
	SystemParameters.CalibrationParameters.ScaleSettingParameters.AutoZTrack = 0;
	SystemParameters.CalibrationParameters.ScaleSettingParameters.INITZero = true;
	SystemParameters.CalibrationParameters.ScaleSettingParameters.LeadZBlank = true;
	SystemParameters.CalibrationParameters.ScaleSettingParameters.ZeroRange = false;
	SystemParameters.CalibrationParameters.ScaleSettingParameters.AutoPowerOff = 0;
	SystemParameters.CalibrationParameters.ScaleSettingParameters.SCALE_02_ENABLE = false;
}

//**********************************************************************************************************

void SystemFunction_ResetCommunicationParameters(CommunicationParameters_TypeDef *CommunicationParameters)
{
	CommunicationParameters->BaudRate = 9600;
	CommunicationParameters->Parity = 0;
	CommunicationParameters->DataBits = 1;
	CommunicationParameters->StopBits = true;
	CommunicationParameters->Operation = 0;
}

//**********************************************************************************************************

void SystemFunction_ResetWeighingCost(void)
{
	SystemParameters.WeighingCost.Trolley = 0;
	SystemParameters.WeighingCost.Ten_Wheels = 0;
	SystemParameters.WeighingCost.Six_Wheels = 0;
	SystemParameters.WeighingCost.Khavar = 0;
	SystemParameters.WeighingCost.Nissan = 0;
	SystemParameters.WeighingCost.PickupTruck = 0;
	SystemParameters.WeighingCost.Other = 0;
}

//**********************************************************************************************************

void SystemFunction_ResetPrintFormatParameters(void)
{
	SystemParameters.PrintFormatParameters.PrintTitle = true;
	SystemParameters.PrintFormatParameters.EjectSheet = true;
	SystemParameters.PrintFormatParameters.TypePrinter = 1;
	SystemParameters.PrintFormatParameters.PrintFont = 1;
	SystemParameters.PrintFormatParameters.NumberOfPrint = 1;
	SystemParameters.PrintFormatParameters.NumberOfPaperPrint = 1;
	SystemParameters.PrintFormatParameters.PrintType = 0;
	SystemParameters.PrintFormatParameters.TopLineEmpty = 1;
	SystemParameters.PrintFormatParameters.BottomLineEmpty = 1;
	SystemParameters.PrintFormatParameters.SecondWeighingPrint = true;
	SystemParameters.PrintFormatParameters.TypePaper = true;
	strcpy(SystemParameters.PrintFormatParameters.Title_01[0], "سپهر سازه");
	strcpy(SystemParameters.PrintFormatParameters.Title_01[1], "");
	strcpy(SystemParameters.PrintFormatParameters.Title_02[0], "استان تهران شهر رباط کریم");
	strcpy(SystemParameters.PrintFormatParameters.Title_02[1], "");

	SystemParameters.PrintFormatParameters.TitleNumberPlates.Line = 0;
	SystemParameters.PrintFormatParameters.TitleNumberPlates.Column = 0;

	SystemParameters.PrintFormatParameters.TitleProduct.Line = 0;
	SystemParameters.PrintFormatParameters.TitleProduct.Column = 0;

	SystemParameters.PrintFormatParameters.TitleDate.Line = 0;
	SystemParameters.PrintFormatParameters.TitleDate.Column = 0;

	SystemParameters.PrintFormatParameters.TitleTime.Line = 0;
	SystemParameters.PrintFormatParameters.TitleTime.Column = 0;

	SystemParameters.PrintFormatParameters.TitleWeight.Line = 0;
	SystemParameters.PrintFormatParameters.TitleWeight.Column = 0;

	SystemParameters.PrintFormatParameters.TitleTicketSerialNumber.Line = 0;
	SystemParameters.PrintFormatParameters.TitleTicketSerialNumber.Column = 0;

	SystemParameters.PrintFormatParameters.TitleCustomer.Line = 0;
	SystemParameters.PrintFormatParameters.TitleCustomer.Column = 0;

	SystemParameters.PrintFormatParameters.TitleWeighingCost.Line = 0;
	SystemParameters.PrintFormatParameters.TitleWeighingCost.Column = 0;

	SystemParameters.PrintFormatParameters.NumberPlates.Line = 5;
	SystemParameters.PrintFormatParameters.NumberPlates.Column = 25;

	SystemParameters.PrintFormatParameters.Product.Line = 5;
	SystemParameters.PrintFormatParameters.Product.Column = 35;

	SystemParameters.PrintFormatParameters.FirstWeightDate.Line = 5;
	SystemParameters.PrintFormatParameters.FirstWeightDate.Column = 40;

	SystemParameters.PrintFormatParameters.SecondWeightDate.Line = 7;
	SystemParameters.PrintFormatParameters.SecondWeightDate.Column = 40;

	SystemParameters.PrintFormatParameters.FirstWeightTime.Line = 5;
	SystemParameters.PrintFormatParameters.FirstWeightTime.Column = 55;

	SystemParameters.PrintFormatParameters.SecondWeightTime.Line = 7;
	SystemParameters.PrintFormatParameters.SecondWeightTime.Column = 55;

	SystemParameters.PrintFormatParameters.FirstWeight.Line = 5;
	SystemParameters.PrintFormatParameters.FirstWeight.Column = 65;

	SystemParameters.PrintFormatParameters.SecondWeight.Line = 7;
	SystemParameters.PrintFormatParameters.SecondWeight.Column = 65;

	SystemParameters.PrintFormatParameters.NetWeight.Line = 9;
	SystemParameters.PrintFormatParameters.NetWeight.Column = 65;

	SystemParameters.PrintFormatParameters.TicketSerialNumber.Line = 1;
	SystemParameters.PrintFormatParameters.TicketSerialNumber.Column = 18;

	SystemParameters.PrintFormatParameters.Customer.Line = 11;
	SystemParameters.PrintFormatParameters.Customer.Column = 60;

	SystemParameters.PrintFormatParameters.WeighingCost.Line = 11;
	SystemParameters.PrintFormatParameters.WeighingCost.Column = 15;

	SystemParameters.PrintFormatParameters.ProductPrice.Line = 0;
	SystemParameters.PrintFormatParameters.ProductPrice.Column = 0;
}

//**********************************************************************************************************

void SystemFunction_InitializeCommunicationParameters(CommunicationParameters_TypeDef CommunicationParameters)
{
	CommunicationParameters.COM_Port->Init.BaudRate = CommunicationParameters.BaudRate;

	if(CommunicationParameters.DataBits == 0)	//7Bit
	{
		CommunicationParameters.COM_Port->Init.WordLength = UART_WORDLENGTH_8B;

		if(CommunicationParameters.Parity == 0)
			CommunicationParameters.COM_Port->Init.Parity = UART_PARITY_NONE;
		else if(CommunicationParameters.Parity == 1)
			CommunicationParameters.COM_Port->Init.Parity = UART_PARITY_EVEN;
		else if(CommunicationParameters.Parity == 2)
			CommunicationParameters.COM_Port->Init.Parity = UART_PARITY_ODD;
	}
	else if(CommunicationParameters.DataBits == 1)	//8Bit
	{
		if(CommunicationParameters.Parity == 0)
		{
			CommunicationParameters.COM_Port->Init.WordLength = UART_WORDLENGTH_8B;
			CommunicationParameters.COM_Port->Init.Parity = UART_PARITY_NONE;
		}
		else if(CommunicationParameters.Parity == 1)
		{
			CommunicationParameters.COM_Port->Init.WordLength = UART_WORDLENGTH_9B;
			CommunicationParameters.COM_Port->Init.Parity = UART_PARITY_EVEN;
		}
		else if(CommunicationParameters.Parity == 2)
		{
			CommunicationParameters.COM_Port->Init.WordLength = UART_WORDLENGTH_9B;
			CommunicationParameters.COM_Port->Init.Parity = UART_PARITY_ODD;
		}
	}
	else if(CommunicationParameters.DataBits == 2)	//9Bit
	{
		CommunicationParameters.COM_Port->Init.WordLength = UART_WORDLENGTH_9B;

		if(CommunicationParameters.Parity == 0)
			CommunicationParameters.COM_Port->Init.Parity = UART_PARITY_NONE;
		else if(CommunicationParameters.Parity == 1)
			CommunicationParameters.COM_Port->Init.Parity = UART_PARITY_EVEN;
		else if(CommunicationParameters.Parity == 2)
			CommunicationParameters.COM_Port->Init.Parity = UART_PARITY_ODD;
	}

	if(CommunicationParameters.StopBits)
		CommunicationParameters.COM_Port->Init.StopBits = UART_STOPBITS_1;
	else
		CommunicationParameters.COM_Port->Init.StopBits = UART_STOPBITS_2;

	if (HAL_UART_Init(CommunicationParameters.COM_Port) != HAL_OK)
		Error_Handler();
}

//**********************************************************************************************************

void SystemFunction_SetConfigParametersScale_01(void)
{
	ConfigParametersScale_TypeDef ConfigParametersScale_01;

	ConfigParametersScale_01.SPI_Port = &hspi1;
	ConfigParametersScale_01.GPIO_Port_Reset = ADC1_RST_GPIO_Port;
	ConfigParametersScale_01.GPIO_Pin_Reset = ADC1_RST_Pin;
	ConfigParametersScale_01.GPIO_Port_DataReady = ADC1_DOUT_GPIO_Port;
	ConfigParametersScale_01.GPIO_Pin_DataReady = ADC1_DOUT_Pin;

	osMessageQueuePut(ConfigParametersScale_01_QueueHandle, (uint8_t *) &ConfigParametersScale_01, osPriorityNormal, 0);
}

//**********************************************************************************************************

void SystemFunction_SetConfigParametersScale_02(void)
{
	ConfigParametersScale_TypeDef ConfigParametersScale_02;

	ConfigParametersScale_02.SPI_Port = &hspi2;
	ConfigParametersScale_02.GPIO_Port_Reset = ADC2_RST_GPIO_Port;
	ConfigParametersScale_02.GPIO_Pin_Reset = ADC2_RST_Pin;
	ConfigParametersScale_02.GPIO_Port_DataReady = ADC2_DOUT_GPIO_Port;
	ConfigParametersScale_02.GPIO_Pin_DataReady = ADC2_DOUT_Pin;

	osMessageQueuePut(ConfigParametersScale_02_QueueHandle, (uint8_t *) &ConfigParametersScale_02, osPriorityNormal, 0);
}

//**********************************************************************************************************

void SystemFunction_SetDate(void)
{
	PanelKeypadButton_TypeDef Button;
	PanelKeypadButton_TypeDef LastPressButton;
	RTC_DateTypeDef sDate = {0};
	uint8_t WeekDay = 1;
	ShamsiDate_TypeDef ShamsiDate;
	uint32_t TextWidth;
	uint8_t StartText;
	uint8_t PixelPositionWeekDay;
	uint8_t StartFillRectangle;
	uint8_t ParameterNumber = 0;
	string Str;
	Blinker_TypeDef Blinker;

	Blinker.AmountBlinker = AMOUNT_BLINKER_SET_PARAMETERS;

    while(1)
	{
		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_9X8);
		Str = "تنظیم تاریخ )etaD(";
		SetTextPixelPosition(76, 2);
		PutsString((char *)Str.c_str());

//----------------------------------------------------------------------------------------------------------

		SetFontLetter(ENGLISH_LETTER);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(2);
		LcdFontSpaceY(0);
		SetFont(Font_En_Num_16X20);

		HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
		ShamsiDate = MiladiToShamsi(sDate.Month, sDate.Date, (2000 + sDate.Year));

		Str = IntegerToString(ShamsiDate.iYear, 4);
		Str += "/";
		Str += IntegerToString(ShamsiDate.iMonth, 2);
		Str += "/";
		Str += IntegerToString(ShamsiDate.iDay, 2);

		TextWidth = CalcTextWidth((char *)Str.c_str());
		StartText = 120 - (TextWidth / 2);
		SetTextPixelPosition(StartText, 20);
		PutsString((char *)Str.c_str());

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_14X16);
		
		if(sDate.WeekDay >= 8)
			sDate.WeekDay = 1;
		
		if(ParameterNumber != 3)
			WeekDay = sDate.WeekDay;
			
		switch(sDate.WeekDay)
		{
			case RTC_WEEKDAY_MONDAY:
			{
				PixelPositionWeekDay = 99;
				Str = "دوشنبه";
				break;
			}
			case RTC_WEEKDAY_TUESDAY:
			{
				PixelPositionWeekDay = 95;
				Str = "سه شنبه";
				break;
			}
			case RTC_WEEKDAY_WEDNESDAY:
			{
				PixelPositionWeekDay = 91;
				Str = "چهارشنبه";
				break;
			}
			case RTC_WEEKDAY_THURSDAY:
			{
				PixelPositionWeekDay = 95;
				Str = "پنج شنبه";
				break;
			}
			case RTC_WEEKDAY_FRIDAY:
			{
				PixelPositionWeekDay = 104;
				Str = "جمعه";
				break;
			}
			case RTC_WEEKDAY_SATURDAY:
			{
				PixelPositionWeekDay = 106;
				Str = "شنبه";
				break;
			}
			case RTC_WEEKDAY_SUNDAY:
			{
				PixelPositionWeekDay = 97;
				Str = "یک شنبه";
				break;
			}
		}	
		
		SetTextPixelPosition(PixelPositionWeekDay, 45);
		PutsString((char *)Str.c_str());
				
//----------------------------------------------------------------------------------------------------------

		if(ParameterNumber == 0)
		{
			Str = IntegerToString(ShamsiDate.iYear, 4);
			Str += "/";
			Str += IntegerToString(ShamsiDate.iMonth, 2);
			Str += "/";

			SetFontLetter(ENGLISH_LETTER);
			LcdFontSpaceX(2);
			LcdFontSpaceY(0);
			SetFont(Font_En_Num_16X20);
			TextWidth = CalcTextWidth((char *)Str.c_str());
			StartFillRectangle = StartText + TextWidth;
			TextWidth = CalcTextWidth((char *)IntegerToString(ShamsiDate.iDay, 2).c_str());
			GLCD_FillRectangle((StartFillRectangle - 2), 18, (StartFillRectangle + TextWidth - 1), 41, BLACK);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(StartFillRectangle, 20);
			Str = GetStringInteger(&LastPressButton, IntegerToString(ShamsiDate.iDay, 2));

			if(LastPressButton == KEYPAD_ENTER)
			{
				ShamsiDate.iDay = atoi(Str.c_str());

				ShamsiDate = ShamsiToMiladi(ShamsiDate.iYear, ShamsiDate.iMonth, ShamsiDate.iDay);
				sDate.Year = ShamsiDate.iYear - 2000;
				sDate.Month = ShamsiDate.iMonth;
				sDate.Date = ShamsiDate.iDay;

				HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
				ParameterNumber++;
			}
			else if(LastPressButton == KEYPAD_ESC)
				return;
		}
		else if(ParameterNumber == 1)
		{
			Str = IntegerToString(ShamsiDate.iYear, 4);
			Str += "/";

			SetFontLetter(ENGLISH_LETTER);
			LcdFontSpaceX(2);
			LcdFontSpaceY(0);
			SetFont(Font_En_Num_16X20);
			TextWidth = CalcTextWidth((char *)Str.c_str());
			StartFillRectangle = StartText + TextWidth;
			TextWidth = CalcTextWidth((char *)IntegerToString(ShamsiDate.iMonth, 2).c_str());
			GLCD_FillRectangle((StartFillRectangle - 2), 18, (StartFillRectangle + TextWidth - 1), 41, BLACK);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(StartFillRectangle, 20);
			Str = GetStringInteger(&LastPressButton, IntegerToString(ShamsiDate.iMonth, 2));

			if(LastPressButton == KEYPAD_ENTER)
			{
				ShamsiDate.iMonth = atoi(Str.c_str());

				ShamsiDate = ShamsiToMiladi(ShamsiDate.iYear, ShamsiDate.iMonth, ShamsiDate.iDay);
				sDate.Year = ShamsiDate.iYear - 2000;
				sDate.Month = ShamsiDate.iMonth;
				sDate.Date = ShamsiDate.iDay;

				HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
				ParameterNumber++;
			}
			else if(LastPressButton == KEYPAD_ESC)
				return;
		}
		else if(ParameterNumber == 2)
		{
			SetFontLetter(ENGLISH_LETTER);
			LcdFontSpaceX(2);
			LcdFontSpaceY(0);
			SetFont(Font_En_Num_16X20);
			StartFillRectangle = StartText;
			TextWidth = CalcTextWidth((char *)IntegerToString(ShamsiDate.iYear, 4).c_str());
			GLCD_FillRectangle((StartFillRectangle - 2), 18, (StartFillRectangle + TextWidth - 1), 41, BLACK);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(StartFillRectangle, 20);
			Str = GetStringInteger(&LastPressButton, IntegerToString(ShamsiDate.iYear, 4));

			if(LastPressButton == KEYPAD_ENTER)
			{
				ShamsiDate.iYear = atoi(Str.c_str());

				ShamsiDate = ShamsiToMiladi(ShamsiDate.iYear, ShamsiDate.iMonth, ShamsiDate.iDay);
				sDate.Year = ShamsiDate.iYear - 2000;
				sDate.Month = ShamsiDate.iMonth;
				sDate.Date = ShamsiDate.iDay;

				HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
				ParameterNumber++;
			}
			else if(LastPressButton == KEYPAD_ESC)
				return;
		}
		else if(ParameterNumber == 3)
		{
			switch(WeekDay)
			{
				case RTC_WEEKDAY_MONDAY:
				{
					PixelPositionWeekDay = 99;
					Str = "دوشنبه";
					break;
				}
				case RTC_WEEKDAY_TUESDAY:
				{
					PixelPositionWeekDay = 95;
					Str = "سه شنبه";
					break;
				}
				case RTC_WEEKDAY_WEDNESDAY:
				{
					PixelPositionWeekDay = 91;
					Str = "چهارشنبه";
					break;
				}
				case RTC_WEEKDAY_THURSDAY:
				{
					PixelPositionWeekDay = 95;
					Str = "پنج شنبه";
					break;
				}
				case RTC_WEEKDAY_FRIDAY:
				{
					PixelPositionWeekDay = 104;
					Str = "جمعه";
					break;
				}
				case RTC_WEEKDAY_SATURDAY:
				{
					PixelPositionWeekDay = 106;
					Str = "شنبه";
					break;
				}
				case RTC_WEEKDAY_SUNDAY:
				{
					PixelPositionWeekDay = 97;
					Str = "یک شنبه";
					break;
				}
			}	
			
			GLCD_FillRectangle(80, 43, 160, 63, BLACK);
			
			Blinker.Counter++;
			if(Blinker.Counter > Blinker.AmountBlinker)
			{
				SetFontLetter(PERSIAN_LETTER);
				SetFontColor(WHITE);
				SetBackgroundColor(BLACK);
				LcdFontSpaceX(1);
				LcdFontSpaceY(0);
				SetFont(Font_Pe_14X16);
				SetTextPixelPosition(PixelPositionWeekDay, 45);
				PutsString((char *)Str.c_str());

				if(Blinker.Counter > (1.5 * Blinker.AmountBlinker))
					Blinker.Counter = 0;
			}
			
			GLCD_Screen_Update();
			
			Button = Panel_ReadPanel();
			
			if(Button == KEYPAD_ENTER)
			{
				sDate.WeekDay = WeekDay;
				HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
				ParameterNumber++;
			}
			else if(Button == KEYPAD_LEFT)
			{
				if(WeekDay > 1)
					WeekDay--;
				else
					WeekDay = 7;
			}
			else if(Button == KEYPAD_RIGHT)
			{
				if(WeekDay < 7)
					WeekDay++;
				else
					WeekDay = 1;
			}
			else if(Button == KEYPAD_ESC)
				return;
		}
		else
			return;
	}
}

//**********************************************************************************************************

void SystemFunction_SetTime(void)
{
	PanelKeypadButton_TypeDef LastPressButton;
	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef sDate = {0};
	uint32_t TextWidth;
	uint8_t StartText;
	uint8_t StartFillRectangle;
	uint8_t ParameterNumber = 0;
	string Str;

	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

    while(1)
	{
		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_9X8);
		Str = "تنظیم ساعت )emiT(";
		SetTextPixelPosition(73, 2);
		PutsString((char *)Str.c_str());

//----------------------------------------------------------------------------------------------------------

		SetFontLetter(ENGLISH_LETTER);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(2);
		LcdFontSpaceY(0);
		SetFont(Font_En_Num_16X20);

		Str = IntegerToString(sTime.Hours, 2);
		Str += ":";
		Str += IntegerToString(sTime.Minutes, 2);
		Str += ":";
		Str += IntegerToString(sTime.Seconds, 2);

		TextWidth = CalcTextWidth((char *)Str.c_str());
		StartText = 120 - (TextWidth / 2);
		SetTextPixelPosition(StartText, 27);
		PutsString((char *)Str.c_str());

//----------------------------------------------------------------------------------------------------------

		if(ParameterNumber == 0)
		{
			Str = IntegerToString(sTime.Hours, 2);
			Str += ":";
			Str += IntegerToString(sTime.Minutes, 2);
			Str += ":";

			TextWidth = CalcTextWidth((char *)Str.c_str());
			StartFillRectangle = StartText + TextWidth;
			TextWidth = CalcTextWidth((char *)IntegerToString(sTime.Seconds, 2).c_str());
			GLCD_FillRectangle((StartFillRectangle - 2), 25, (StartFillRectangle + TextWidth - 1), 48, BLACK);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(StartFillRectangle, 27);
			Str = GetStringInteger(&LastPressButton, IntegerToString(sTime.Seconds, 2));

			if(LastPressButton == KEYPAD_ENTER)
			{
				sTime.Seconds = atoi(Str.c_str());

				HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
				ParameterNumber++;
			}
			else if(LastPressButton == KEYPAD_ESC)
				return;
		}
		else if(ParameterNumber == 1)
		{
			Str = IntegerToString(sTime.Hours, 2);
			Str += ":";

			TextWidth = CalcTextWidth((char *)Str.c_str());
			StartFillRectangle = StartText + TextWidth;
			TextWidth = CalcTextWidth((char *)IntegerToString(sTime.Minutes, 2).c_str());
			GLCD_FillRectangle((StartFillRectangle - 2), 25, (StartFillRectangle + TextWidth - 1), 48, BLACK);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(StartFillRectangle, 27);
			Str = GetStringInteger(&LastPressButton, IntegerToString(sTime.Minutes, 2));

			if(LastPressButton == KEYPAD_ENTER)
			{
				sTime.Minutes = atoi(Str.c_str());

				HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
				ParameterNumber++;
			}
			else if(LastPressButton == KEYPAD_ESC)
				return;
		}
		else if(ParameterNumber == 2)
		{
			StartFillRectangle = StartText;
			TextWidth = CalcTextWidth((char *)IntegerToString(sTime.Hours, 2).c_str());
			GLCD_FillRectangle((StartFillRectangle - 2), 25, (StartFillRectangle + TextWidth - 1), 48, BLACK);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(StartFillRectangle, 27);
			Str = GetStringInteger(&LastPressButton, IntegerToString(sTime.Hours, 2));

			if(LastPressButton == KEYPAD_ENTER)
			{
				sTime.Hours = atoi(Str.c_str());

				HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
				ParameterNumber++;
			}
			else if(LastPressButton == KEYPAD_ESC)
				return;
		}
		else
			return;
	}
}

//**********************************************************************************************************

void SystemFunction_SetScaleSettingParameters(void)
{
	uint8_t ParameterNumber = 0;
	PanelKeypadButton_TypeDef Button;
	PanelKeypadButton_TypeDef LastPressButton = NO_PUSH;
	Blinker_TypeDef Blinker;
	uint8_t IndexPage = 0;
	string Str = "";

	Blinker.AmountBlinker = AMOUNT_BLINKER_SET_PARAMETERS;

//----------------------------------------------------------------------------------------------------------

	if(IS_CALIBRATION_PIN_JUMPER)
	{
		GLCD_FillRectangle(25, 14, 214, 58, BLACK);
		GLCD_FillRectangle(27, 16, 212, 56, WHITE);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_12X11);
		SetTextPixelPosition(42, 31);
		PutsString("این قسمت توسط جامپر قفل شده است");

		GLCD_Screen_Update();
		DELAY_MS(3000);
		return;
	}

//----------------------------------------------------------------------------------------------------------

	while(1)
	{
		Button = Panel_ReadPanel();

		if((ParameterNumber == 0) && (Button == KEYPAD_DOWN))
		{
			if(IndexPage < 3)
				IndexPage++;
		}
		else if((ParameterNumber == 0) && (Button == KEYPAD_UP))
		{
			if(IndexPage > 0)
				IndexPage--;
		}
		else if(Button == KEYPAD_00)
		{
			if(IndexPage == 3)
				ParameterNumber = 10;
		}
		else if(Button == KEYPAD_01)
		{
			if(IndexPage == 0)
				ParameterNumber = 1;
			else if(IndexPage == 3)
				ParameterNumber = 11;
		}
		else if(Button == KEYPAD_02)
		{
			if(IndexPage == 0)
				ParameterNumber = 2;
			else if(IndexPage == 3)
				ParameterNumber = 12;
		}
		else if(Button == KEYPAD_03)
		{
			if(IndexPage == 0)
				ParameterNumber = 3;
		}
		else if(Button == KEYPAD_04)
		{
			if(IndexPage == 1)
				ParameterNumber = 4;
		}
		else if(Button == KEYPAD_05)
		{
			if(IndexPage == 1)
				ParameterNumber = 5;
		}
		else if(Button == KEYPAD_06)
		{
			if(IndexPage == 1)
				ParameterNumber = 6;
		}
		else if(Button == KEYPAD_07)
		{
			if(IndexPage == 2)
				ParameterNumber = 7;
		}
		else if(Button == KEYPAD_08)
		{
			if(IndexPage == 2)
				ParameterNumber = 8;
		}
		else if(Button == KEYPAD_09)
		{
			if(IndexPage == 2)
				ParameterNumber = 9;
		}
		else if((Button == KEYPAD_ENTER) || (Button == KEYPAD_ESC))
		{
			if(ParameterNumber != 0)
				ParameterNumber = 0;
			else
			{
				ShowSaveParameters();
				SaveSystemParameters(SystemParameters);
				return;
			}
		}

//----------------------------------------------------------------------------------------------------------

		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_9X8);
		SetTextPixelPosition(54, 2);
		PutsString("پیکربندی )noitarugifnoC(");

//----------------------------------------------------------------------------------------------------------

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);

		if(IndexPage == 0)
		{
			SetFont(Font_Pe_12X11);
			SetTextPixelPosition(10, 18);
			PutsString("1-نمایش ارقام )stigiD yalpsiD(");
			SetFontLetter(ENGLISH_LETTER);
			SetTextPixelPosition(55, 18);
			PutChar(':');
			SetTextPixelPosition(10, 18);
			Str = IntegerToString(SystemParameters.CalibrationParameters.ScaleSettingParameters.DISPDigits, 1);
			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetTextPixelPosition(10, 33);
			PutsString("2-نقطه اعشار )tnioP lamiceD(");
			SetFontLetter(ENGLISH_LETTER);
			SetTextPixelPosition(55, 33);
			PutChar(':');
			SetTextPixelPosition(10, 33);
			Str = IntegerToString(SystemParameters.CalibrationParameters.ScaleSettingParameters.DECPoint, 1);
			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetTextPixelPosition(10, 48);
			PutsString("3-زینه )noisiviD elacS(");
			SetFontLetter(ENGLISH_LETTER);
			SetTextPixelPosition(55, 48);
			PutChar(':');
			SetTextPixelPosition(10, 48);
			if(SystemParameters.CalibrationParameters.ScaleSettingParameters.ScaleDIV == 0)
				Str = "1";
			else if(SystemParameters.CalibrationParameters.ScaleSettingParameters.ScaleDIV == 1)
				Str = "2";
			else if(SystemParameters.CalibrationParameters.ScaleSettingParameters.ScaleDIV == 2)
				Str = "5";
			else if(SystemParameters.CalibrationParameters.ScaleSettingParameters.ScaleDIV == 3)
				Str = "10";
			else if(SystemParameters.CalibrationParameters.ScaleSettingParameters.ScaleDIV == 4)
				Str = "20";
			else if(SystemParameters.CalibrationParameters.ScaleSettingParameters.ScaleDIV == 5)
				Str = "50";
			else if(SystemParameters.CalibrationParameters.ScaleSettingParameters.ScaleDIV == 6)
				Str = "100";
			else if(SystemParameters.CalibrationParameters.ScaleSettingParameters.ScaleDIV == 7)
				Str = "200";

			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetFont(Symbol_16);
			SetTextPixelPosition(0, 47);
			PutChar(0x01);
		}
		else if(IndexPage == 1)
		{
			SetFont(Font_Pe_12X11);
			SetTextPixelPosition(10, 18);
			PutsString("4-محدوده وزن )yticapaC(");
			SetFontLetter(ENGLISH_LETTER);
			SetTextPixelPosition(55, 18);
			PutChar(':');
			SetTextPixelPosition(10, 18);
			Str = IntegerToString(SystemParameters.CalibrationParameters.ScaleSettingParameters.WEIGRange, 6, true);
			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetTextPixelPosition(10, 33);
			PutsString("5-فیلتر دیجیتال )retliF latigiD(");
			SetFontLetter(ENGLISH_LETTER);
			SetTextPixelPosition(55, 33);
			PutChar(':');
			SetTextPixelPosition(10, 33);
			Str = IntegerToString(SystemParameters.CalibrationParameters.ScaleSettingParameters.DIGFilter, 1);
			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetTextPixelPosition(10, 48);
			PutsString("6-محدوده صفر )egnaR oreZ(");
			SetFontLetter(ENGLISH_LETTER);
			SetTextPixelPosition(55, 48);
			PutChar(':');
			SetTextPixelPosition(10, 48);
			if(SystemParameters.CalibrationParameters.ScaleSettingParameters.ZeroRange)
				Str = "2%";
			else
				Str = "10%";

			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetFont(Symbol_16);
			SetTextPixelPosition(0, 14);
			PutChar(0x00);
			SetTextPixelPosition(0, 47);
			PutChar(0x01);
		}
		else if(IndexPage == 2)
		{
			SetFont(Font_Pe_12X11);
			SetTextPixelPosition(10, 18);
			PutsString("7-صفر کردن خودکار )oreZ otuA(");
			SetFontLetter(ENGLISH_LETTER);
			SetTextPixelPosition(55, 18);
			PutChar(':');
			SetTextPixelPosition(10, 18);
			Str = IntegerToString(SystemParameters.CalibrationParameters.ScaleSettingParameters.AutoZTrack, 2);
			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetTextPixelPosition(10, 33);
			PutsString("8-صفر کردن هنگام روشن شدن");
			SetFontLetter(ENGLISH_LETTER);
			SetTextPixelPosition(55, 33);
			PutChar(':');
			SetTextPixelPosition(10, 33);
			if(SystemParameters.CalibrationParameters.ScaleSettingParameters.INITZero)
				Str = "Yes";
			else
				Str = "No";

			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetTextPixelPosition(10, 48);
			PutsString("9- نمایش صفرهای وزن");
			SetFontLetter(ENGLISH_LETTER);
			SetTextPixelPosition(55, 48);
			PutChar(':');
			SetTextPixelPosition(10, 48);
			if(SystemParameters.CalibrationParameters.ScaleSettingParameters.LeadZBlank)
				Str = "Yes";
			else
				Str = "No";

			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetFont(Symbol_16);
			SetTextPixelPosition(0, 14);
			PutChar(0x00);
			SetTextPixelPosition(0, 47);
			PutChar(0x01);
		}
		else if(IndexPage == 3)
		{
			SetFont(Font_Pe_12X11);
			SetTextPixelPosition(10, 18);
			PutsString("10-فعال شدن صفحه دو");
			SetFontLetter(ENGLISH_LETTER);
			SetTextPixelPosition(55, 18);
			PutChar(':');
			SetTextPixelPosition(10, 18);
			if(SystemParameters.CalibrationParameters.ScaleSettingParameters.SCALE_02_ENABLE)
				Str = "Yes";
			else
				Str = "No";

			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetTextPixelPosition(10, 33);
			PutsString("11-تعداد نمونه بدون حرکت");
			SetFontLetter(ENGLISH_LETTER);
			SetTextPixelPosition(55, 33);
			PutChar(':');
			SetTextPixelPosition(10, 33);
			Str = IntegerToString(((uint8_t)SystemParameters.CalibrationParameters.ScaleSettingParameters.MotionSamples), 1);
			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetTextPixelPosition(10, 48);
			PutsString("12-زمان خاموش شدن نمایشگر");
			SetFontLetter(ENGLISH_LETTER);
			SetTextPixelPosition(55, 48);
			PutChar(':');
			SetTextPixelPosition(10, 48);
			Str = IntegerToString((uint8_t)SystemParameters.CalibrationParameters.ScaleSettingParameters.AutoPowerOff, 2);
			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetFont(Symbol_16);
			SetTextPixelPosition(0, 14);
			PutChar(0x00);
		}

//----------------------------------------------------------------------------------------------------------

		if(ParameterNumber == 1)
		{
			GLCD_FillRectangle(5, 16, 50, 30, BLACK);

			if(Button == KEYPAD_LEFT)
			{
				if(SystemParameters.CalibrationParameters.ScaleSettingParameters.DISPDigits > 4)
					SystemParameters.CalibrationParameters.ScaleSettingParameters.DISPDigits --;
				else
					SystemParameters.CalibrationParameters.ScaleSettingParameters.DISPDigits = 6;
			}
			else if(Button == KEYPAD_RIGHT)
			{
				if(SystemParameters.CalibrationParameters.ScaleSettingParameters.DISPDigits < 6)
					SystemParameters.CalibrationParameters.ScaleSettingParameters.DISPDigits ++;
				else
					SystemParameters.CalibrationParameters.ScaleSettingParameters.DISPDigits = 4;
			}

			Blinker.Counter++;
			if(Blinker.Counter > Blinker.AmountBlinker)
			{
				SetFontLetter(ENGLISH_LETTER);
				SetFont(Font_Pe_12X11);
				SetFontColor(WHITE);
				SetBackgroundColor(BLACK);
				SetTextPixelPosition(10, 18);
				Str = IntegerToString(SystemParameters.CalibrationParameters.ScaleSettingParameters.DISPDigits, 1);
				PutsString((char *)Str.c_str());

				if(Blinker.Counter > (1.5 * Blinker.AmountBlinker))
					Blinker.Counter = 0;
			}
		}
		else if(ParameterNumber == 2)
		{
			GLCD_FillRectangle(5, 31, 50, 45, BLACK);

			if(Button == KEYPAD_LEFT)
			{
				if(SystemParameters.CalibrationParameters.ScaleSettingParameters.DECPoint > 0)
					SystemParameters.CalibrationParameters.ScaleSettingParameters.DECPoint --;
				else
					SystemParameters.CalibrationParameters.ScaleSettingParameters.DECPoint = 5;
			}
			else if(Button == KEYPAD_RIGHT)
			{
				if(SystemParameters.CalibrationParameters.ScaleSettingParameters.DECPoint < 5)
					SystemParameters.CalibrationParameters.ScaleSettingParameters.DECPoint ++;
				else
					SystemParameters.CalibrationParameters.ScaleSettingParameters.DECPoint = 0;
			}

			Blinker.Counter++;
			if(Blinker.Counter > Blinker.AmountBlinker)
			{
				SetFontLetter(ENGLISH_LETTER);
				SetFont(Font_Pe_12X11);
				SetFontColor(WHITE);
				SetBackgroundColor(BLACK);
				SetTextPixelPosition(10, 33);
				Str = IntegerToString(SystemParameters.CalibrationParameters.ScaleSettingParameters.DECPoint, 1);
				PutsString((char *)Str.c_str());

				if(Blinker.Counter > (1.5 * Blinker.AmountBlinker))
					Blinker.Counter = 0;
			}
		}
		else if(ParameterNumber == 3)
		{
			GLCD_FillRectangle(5, 46, 50, 60, BLACK);

			if(Button == KEYPAD_LEFT)
			{
				if(SystemParameters.CalibrationParameters.ScaleSettingParameters.ScaleDIV > 0)
					SystemParameters.CalibrationParameters.ScaleSettingParameters.ScaleDIV --;
				else
					SystemParameters.CalibrationParameters.ScaleSettingParameters.ScaleDIV = 7;
			}
			else if(Button == KEYPAD_RIGHT)
			{
				if(SystemParameters.CalibrationParameters.ScaleSettingParameters.ScaleDIV < 7)
					SystemParameters.CalibrationParameters.ScaleSettingParameters.ScaleDIV ++;
				else
					SystemParameters.CalibrationParameters.ScaleSettingParameters.ScaleDIV = 0;
			}

			Blinker.Counter++;
			if(Blinker.Counter > Blinker.AmountBlinker)
			{
				SetFontLetter(ENGLISH_LETTER);
				SetFont(Font_Pe_12X11);
				SetFontColor(WHITE);
				SetBackgroundColor(BLACK);
				SetTextPixelPosition(10, 48);

				if(SystemParameters.CalibrationParameters.ScaleSettingParameters.ScaleDIV == 0)
				Str = "1";
				else if(SystemParameters.CalibrationParameters.ScaleSettingParameters.ScaleDIV == 1)
					Str = "2";
				else if(SystemParameters.CalibrationParameters.ScaleSettingParameters.ScaleDIV == 2)
					Str = "5";
				else if(SystemParameters.CalibrationParameters.ScaleSettingParameters.ScaleDIV == 3)
					Str = "10";
				else if(SystemParameters.CalibrationParameters.ScaleSettingParameters.ScaleDIV == 4)
					Str = "20";
				else if(SystemParameters.CalibrationParameters.ScaleSettingParameters.ScaleDIV == 5)
					Str = "50";
				else if(SystemParameters.CalibrationParameters.ScaleSettingParameters.ScaleDIV == 6)
					Str = "100";
				else if(SystemParameters.CalibrationParameters.ScaleSettingParameters.ScaleDIV == 7)
					Str = "200";

				PutsString((char *)Str.c_str());

				if(Blinker.Counter > (1.5 * Blinker.AmountBlinker))
					Blinker.Counter = 0;
			}
		}
		else if(ParameterNumber == 4)
		{
			GLCD_FillRectangle(5, 16, 50, 30, BLACK);
			SetFontLetter(ENGLISH_LETTER);
			SetFont(Font_Pe_12X11);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(10, 18);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.CalibrationParameters.ScaleSettingParameters.WEIGRange, 6));

			if(LastPressButton == KEYPAD_ENTER)
			{
				SystemParameters.CalibrationParameters.ScaleSettingParameters.WEIGRange = atoi(Str.c_str());
				ParameterNumber = 0;
			}
			else if(LastPressButton == KEYPAD_ESC)
				ParameterNumber = 0;
		}
		else if(ParameterNumber == 5)
		{
			GLCD_FillRectangle(5, 31, 50, 45, BLACK);

			if(Button == KEYPAD_LEFT)
			{
				if(SystemParameters.CalibrationParameters.ScaleSettingParameters.DIGFilter > 0)
					SystemParameters.CalibrationParameters.ScaleSettingParameters.DIGFilter --;
				else
					SystemParameters.CalibrationParameters.ScaleSettingParameters.DIGFilter = 5;
			}
			else if(Button == KEYPAD_RIGHT)
			{
				if(SystemParameters.CalibrationParameters.ScaleSettingParameters.DIGFilter < 5)
					SystemParameters.CalibrationParameters.ScaleSettingParameters.DIGFilter ++;
				else
					SystemParameters.CalibrationParameters.ScaleSettingParameters.DIGFilter = 0;
			}

			Blinker.Counter++;
			if(Blinker.Counter > Blinker.AmountBlinker)
			{
				SetFontLetter(ENGLISH_LETTER);
				SetFont(Font_Pe_12X11);
				SetFontColor(WHITE);
				SetBackgroundColor(BLACK);
				SetTextPixelPosition(10, 33);
				Str = IntegerToString(SystemParameters.CalibrationParameters.ScaleSettingParameters.DIGFilter, 1);
				PutsString((char *)Str.c_str());

				if(Blinker.Counter > (1.5 * Blinker.AmountBlinker))
					Blinker.Counter = 0;
			}
		}
		else if(ParameterNumber == 6)
		{
			GLCD_FillRectangle(5, 46, 50, 60, BLACK);

			if((Button == KEYPAD_LEFT) || (Button == KEYPAD_RIGHT))
				SystemParameters.CalibrationParameters.ScaleSettingParameters.ZeroRange = !SystemParameters.CalibrationParameters.ScaleSettingParameters.ZeroRange;

			Blinker.Counter++;
			if(Blinker.Counter > Blinker.AmountBlinker)
			{
				SetFontLetter(ENGLISH_LETTER);
				SetFont(Font_Pe_12X11);
				SetFontColor(WHITE);
				SetBackgroundColor(BLACK);
				SetTextPixelPosition(10, 48);

				if(SystemParameters.CalibrationParameters.ScaleSettingParameters.ZeroRange)
					Str = "2%";
				else
					Str = "10%";

				PutsString((char *)Str.c_str());

				if(Blinker.Counter > (1.5 * Blinker.AmountBlinker))
					Blinker.Counter = 0;
			}
		}
		else if(ParameterNumber == 7)
		{
			GLCD_FillRectangle(5, 16, 50, 30, BLACK);
			SetFontLetter(ENGLISH_LETTER);
			SetFont(Font_Pe_12X11);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(10, 18);
			Str = GetStringInteger(&LastPressButton, IntegerToString((uint8_t)SystemParameters.CalibrationParameters.ScaleSettingParameters.AutoZTrack, 2));

			if(LastPressButton == KEYPAD_ENTER)
			{
				SystemParameters.CalibrationParameters.ScaleSettingParameters.AutoZTrack = atoi(Str.c_str());
				ParameterNumber = 0;
			}
			else if(LastPressButton == KEYPAD_ESC)
				ParameterNumber = 0;
		}
		else if(ParameterNumber == 8)
		{
			GLCD_FillRectangle(5, 31, 50, 45, BLACK);

			if((Button == KEYPAD_LEFT) || (Button == KEYPAD_RIGHT))
				SystemParameters.CalibrationParameters.ScaleSettingParameters.INITZero = !SystemParameters.CalibrationParameters.ScaleSettingParameters.INITZero;

			Blinker.Counter++;
			if(Blinker.Counter > Blinker.AmountBlinker)
			{
				SetFontLetter(ENGLISH_LETTER);
				SetFont(Font_Pe_12X11);
				SetFontColor(WHITE);
				SetBackgroundColor(BLACK);
				SetTextPixelPosition(10, 33);

				if(SystemParameters.CalibrationParameters.ScaleSettingParameters.INITZero)
					Str = "Yes";
				else
					Str = "No";

				PutsString((char *)Str.c_str());

				if(Blinker.Counter > (1.5 * Blinker.AmountBlinker))
					Blinker.Counter = 0;
			}
		}
		else if(ParameterNumber == 9)
		{
			GLCD_FillRectangle(5, 46, 50, 60, BLACK);

			if((Button == KEYPAD_LEFT) || (Button == KEYPAD_RIGHT))
				SystemParameters.CalibrationParameters.ScaleSettingParameters.LeadZBlank = !SystemParameters.CalibrationParameters.ScaleSettingParameters.LeadZBlank;

			Blinker.Counter++;
			if(Blinker.Counter > Blinker.AmountBlinker)
			{
				SetFontLetter(ENGLISH_LETTER);
				SetFont(Font_Pe_12X11);
				SetFontColor(WHITE);
				SetBackgroundColor(BLACK);
				SetTextPixelPosition(10, 48);

				if(SystemParameters.CalibrationParameters.ScaleSettingParameters.LeadZBlank)
					Str = "Yes";
				else
					Str = "No";

				PutsString((char *)Str.c_str());

				if(Blinker.Counter > (1.5 * Blinker.AmountBlinker))
					Blinker.Counter = 0;
			}
		}
		else if(ParameterNumber == 10)
		{
			GLCD_FillRectangle(5, 16, 50, 30, BLACK);

			if((Button == KEYPAD_LEFT) || (Button == KEYPAD_RIGHT))
				SystemParameters.CalibrationParameters.ScaleSettingParameters.SCALE_02_ENABLE = !SystemParameters.CalibrationParameters.ScaleSettingParameters.SCALE_02_ENABLE;

			Blinker.Counter++;
			if(Blinker.Counter > Blinker.AmountBlinker)
			{
				SetFontLetter(ENGLISH_LETTER);
				SetFont(Font_Pe_12X11);
				SetFontColor(WHITE);
				SetBackgroundColor(BLACK);
				SetTextPixelPosition(10, 18);

				if(SystemParameters.CalibrationParameters.ScaleSettingParameters.SCALE_02_ENABLE)
					Str = "Yes";
				else
					Str = "No";

				PutsString((char *)Str.c_str());

				if(Blinker.Counter > (1.5 * Blinker.AmountBlinker))
					Blinker.Counter = 0;
			}
		}
		else if(ParameterNumber == 11)
		{
			GLCD_FillRectangle(5, 31, 50, 45, BLACK);

			if(Button == KEYPAD_LEFT)
			{
				if(SystemParameters.CalibrationParameters.ScaleSettingParameters.MotionSamples > 1)
					SystemParameters.CalibrationParameters.ScaleSettingParameters.MotionSamples --;
				else
					SystemParameters.CalibrationParameters.ScaleSettingParameters.MotionSamples = 7;
			}
			else if(Button == KEYPAD_RIGHT)
			{
				if(SystemParameters.CalibrationParameters.ScaleSettingParameters.MotionSamples < 7)
					SystemParameters.CalibrationParameters.ScaleSettingParameters.MotionSamples ++;
				else
					SystemParameters.CalibrationParameters.ScaleSettingParameters.MotionSamples = 1;
			}

			Blinker.Counter++;
			if(Blinker.Counter > Blinker.AmountBlinker)
			{
				SetFontLetter(ENGLISH_LETTER);
				SetFont(Font_Pe_12X11);
				SetFontColor(WHITE);
				SetBackgroundColor(BLACK);
				SetTextPixelPosition(10, 33);
				Str = IntegerToString(((uint8_t)SystemParameters.CalibrationParameters.ScaleSettingParameters.MotionSamples), 1);
				PutsString((char *)Str.c_str());

				if(Blinker.Counter > (1.5 * Blinker.AmountBlinker))
					Blinker.Counter = 0;
			}
		}
		else if(ParameterNumber == 12)
		{
			GLCD_FillRectangle(5, 46, 50, 60, BLACK);
			SetFontLetter(ENGLISH_LETTER);
			SetFont(Font_Pe_12X11);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(10, 48);
			Str = GetStringInteger(&LastPressButton, IntegerToString((uint8_t)SystemParameters.CalibrationParameters.ScaleSettingParameters.AutoPowerOff, 2));

			if(LastPressButton == KEYPAD_ENTER)
			{
				SystemParameters.CalibrationParameters.ScaleSettingParameters.AutoPowerOff = atoi(Str.c_str());
				ParameterNumber = 0;
			}
			else if(LastPressButton == KEYPAD_ESC)
				ParameterNumber = 0;
		}

//----------------------------------------------------------------------------------------------------------

		GLCD_Screen_Update();
	}
}

//**********************************************************************************************************

void SystemFunction_CalibrationScale(void)
{
	StatusWeight_Function_TypeDef StatusWeight_Function;
	WeightScale_TypeDef WeightScale = {0};
	int32_t ADCCalibration;
	int32_t Weight;
	PanelKeypadButton_TypeDef Button;
	PanelKeypadButton_TypeDef LastPressButton = NO_PUSH;
	string Str = "";

//----------------------------------------------------------------------------------------------------------

	if(IS_CALIBRATION_PIN_JUMPER)
	{
		GLCD_FillRectangle(25, 14, 214, 58, BLACK);
		GLCD_FillRectangle(27, 16, 212, 56, WHITE);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_12X11);
		SetTextPixelPosition(42, 31);
		PutsString("این قسمت توسط جامپر قفل شده است");

		GLCD_Screen_Update();
		DELAY_MS(3000);
		return;
	}

//----------------------------------------------------------------------------------------------------------

	Weight_Scale.ConfigParametersConfiguration(SystemParameters.CalibrationParameters, RespanceScale_01_QueueHandle , RespanceScale_02_QueueHandle);

	while(1)
	{
		Button = Panel_ReadPanel();

		if(Button == KEYPAD_01)
		{
			StatusWeight_Function = Weight_Scale.GetADCCalibration(&ADCCalibration);

			if(StatusWeight_Function == OK_WEIGHT_FUNCTION)
			{
				SystemParameters.CalibrationParameters.DeadLoad = ADCCalibration;
				Weight_Scale.ConfigParametersConfiguration(SystemParameters.CalibrationParameters, RespanceScale_01_QueueHandle , RespanceScale_02_QueueHandle);
			}
			else if(StatusWeight_Function == ERROR_AD7780_SCALE_01_WEIGHT_FUNCTION)
			ErrorSystemReport(E03_AD7780_SCALE01);
			else if(StatusWeight_Function == ERROR_AD7780_SCALE_02_WEIGHT_FUNCTION)
				ErrorSystemReport(E04_AD7780_SCALE02);
			else if(StatusWeight_Function == ERROR_SCALE_01_WEIGHT_FUNCTION)
				ErrorSystemReport(E01_READ_SCALE01);
			else if(StatusWeight_Function == ERROR_SCALE_02_WEIGHT_FUNCTION)
				ErrorSystemReport(E02_READ_SCALE02);
		}
		else if(Button == KEYPAD_02)
		{
			GLCD_FillRectangle(25, 14, 214, 58, BLACK);
			GLCD_FillRectangle(27, 16, 212, 56, WHITE);

			SetFontLetter(PERSIAN_LETTER);
			SetFontColor(BLACK);
			SetBackgroundColor(WHITE);
			LcdFontSpaceX(1);
			LcdFontSpaceY(0);
			SetFont(Font_Pe_12X11);
			SetTextPixelPosition(30, 31);
			PutsString("وزن کالیبره )thgieW( :");

			GLCD_FillRectangle(30, 30, 80, 41, BLACK);
			SetFontLetter(ENGLISH_LETTER);
			SetFont(Font_En_Num_8X10);
			LcdFontSpaceX(0);
			LcdFontSpaceY(0);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(32, 31);

			Str = GetStringWeight(&LastPressButton, SystemParameters.CalibrationParameters.ScaleSettingParameters.WEIGRange,
								  SystemParameters.CalibrationParameters.ScaleSettingParameters.DISPDigits,
								  SystemParameters.CalibrationParameters.ScaleSettingParameters.DECPoint);

			if(LastPressButton == KEYPAD_ENTER)
			{
				 Weight = atol(Str.c_str());

				 if((Weight > 0) && (Weight <= SystemParameters.CalibrationParameters.ScaleSettingParameters.WEIGRange))
				 {
					StatusWeight_Function = Weight_Scale.GetADCCalibration(&ADCCalibration);

					if(StatusWeight_Function == OK_WEIGHT_FUNCTION)
					{
						SystemParameters.CalibrationParameters.Alpha = ((float)ADCCalibration - SystemParameters.CalibrationParameters.DeadLoad) / Weight;
						Weight_Scale.ConfigParametersConfiguration(SystemParameters.CalibrationParameters, RespanceScale_01_QueueHandle , RespanceScale_02_QueueHandle);
					}
					else if(StatusWeight_Function == ERROR_AD7780_SCALE_01_WEIGHT_FUNCTION)
					ErrorSystemReport(E03_AD7780_SCALE01);
					else if(StatusWeight_Function == ERROR_AD7780_SCALE_02_WEIGHT_FUNCTION)
						ErrorSystemReport(E04_AD7780_SCALE02);
					else if(StatusWeight_Function == ERROR_SCALE_01_WEIGHT_FUNCTION)
						ErrorSystemReport(E01_READ_SCALE01);
					else if(StatusWeight_Function == ERROR_SCALE_02_WEIGHT_FUNCTION)
						ErrorSystemReport(E02_READ_SCALE02);
				 }
			}
		}
		else if(Button == KEYPAD_03)
		{
			GLCD_FillRectangle(3, 46, 82, 58, BLACK);
			SetFontLetter(ENGLISH_LETTER);
			SetFont(Font_En_Num_8X10);
			LcdFontSpaceX(0);
			LcdFontSpaceY(0);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(6, 48);
			Str = GetStringFloat(&LastPressButton, FloatToString(SystemParameters.CalibrationParameters.Alpha, 11, 3, true));

			if(LastPressButton == KEYPAD_ENTER)
			{
				if(atof(Str.c_str()) > 0)
				{
					SystemParameters.CalibrationParameters.Alpha = atof(Str.c_str());
					Weight_Scale.ConfigParametersConfiguration(SystemParameters.CalibrationParameters, RespanceScale_01_QueueHandle , RespanceScale_02_QueueHandle);
				}
			}
		}
		else if((Button == KEYPAD_ENTER) || (Button == KEYPAD_ESC))
		{
			ShowSaveParameters();
			SaveSystemParameters(SystemParameters);
			return;
		}

//----------------------------------------------------------------------------------------------------------

		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFont(Font_Pe_9X8);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetTextPixelPosition(54, 2);
		PutsString("کالیبراسیون )noitarbilaC(");

		SetFontLetter(ENGLISH_LETTER);
		SetFont(Font_Pe_12X11);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);

//----------------------------------------------------------------------------------------------------------

		Weight_Scale.SetZeroWeight(0);
		Weight_Scale.SetTareWeight(0);
		StatusWeight_Function = Weight_Scale.GetWeightScale(&WeightScale);

		if(StatusWeight_Function == ERROR_AD7780_SCALE_01_WEIGHT_FUNCTION)
			ErrorSystemReport(E03_AD7780_SCALE01);
		else if(StatusWeight_Function == ERROR_AD7780_SCALE_02_WEIGHT_FUNCTION)
			ErrorSystemReport(E04_AD7780_SCALE02);
		else if(StatusWeight_Function == ERROR_SCALE_01_WEIGHT_FUNCTION)
			ErrorSystemReport(E01_READ_SCALE01);
		else if(StatusWeight_Function == ERROR_SCALE_02_WEIGHT_FUNCTION)
			ErrorSystemReport(E02_READ_SCALE02);


		if(!SystemFunction_ShowErrorSystem(&LastPressButton))
		{
			SetTextPixelPosition(0, 18);
			PutsString("ADC:");
			Str = IntegerToString(WeightScale.ADC_Scale, 1, false);
			if(WeightScale.ADC_Scale >= 0)
				PutChar(' ');

			PutsString((char *)Str.c_str());
			SetTextPixelPosition(0, 33);
			PutsString("Weight:");
			Str = WeightToString(WeightScale.HighResolution, 1, WeightScale.DECPoint, WeightScale.LeadZBlank);
			PutsString((char *)Str.c_str());
			PutsString("Kg");
		}
		else
		{
			if(Button == KEYPAD_ESC)
				return;

			continue;
		}

//----------------------------------------------------------------------------------------------------------

		SetFontLetter(PERSIAN_LETTER);
		SetFont(Font_Pe_12X11);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);

		SetTextPixelPosition(5, 18);
		PutsString("1-حذف وزن مرده )oreZ(");
		SetTextPixelPosition(5, 33);
		PutsString("2-وزن کالیبره )napS(");
		SetTextPixelPosition(5, 48);
		PutsString("3-ضریب کالیبراسیون )ahplA( : ");

		SetFontLetter(ENGLISH_LETTER);
		SetTextPixelPosition(7, 48);
		Str = FloatToString(SystemParameters.CalibrationParameters.Alpha, 11, 3, true);
		PutsString((char *)Str.c_str());

//----------------------------------------------------------------------------------------------------------

		GLCD_Screen_Update();
	}
}

//**********************************************************************************************************

void SystemFunction_SetSystemPassword(void)
{
	PanelKeypadButton_TypeDef Button;
	PanelKeypadButton_TypeDef LastPressButton = NO_PUSH;
	string String_Password = "000000";
	string String_RepeatPassword = "000000";

	while(1)
	{
		Button = Panel_ReadPanel();

		if(Button == KEYPAD_01)
		{
			GLCD_FillRectangle(58, 24, 101, 35, BLACK);
			SetFontLetter(ENGLISH_LETTER);
			LcdFontSpaceX(1);
			LcdFontSpaceY(0);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(60, 25);
			String_Password = GetStringInteger(&LastPressButton, String_Password);
		}
		else if(Button == KEYPAD_02)
		{
			GLCD_FillRectangle(58, 39, 101, 50, BLACK);
			SetFontLetter(ENGLISH_LETTER);
			LcdFontSpaceX(1);
			LcdFontSpaceY(0);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(60, 40);
			String_RepeatPassword = GetStringInteger(&LastPressButton, String_RepeatPassword);
		}
		else if((Button == KEYPAD_ENTER) || (Button == KEYPAD_ESC))
		{
			if(String_Password == String_RepeatPassword)
			{
				strcpy(SystemParameters.PinSystem, String_Password.c_str());
				SaveSystemParameters(SystemParameters);

				GLCD_FillRectangle(25, 14, 214, 58, BLACK);
				GLCD_FillRectangle(27, 16, 212, 56, WHITE);

				SetFontLetter(PERSIAN_LETTER);
				SetFontColor(BLACK);
				SetBackgroundColor(WHITE);
				LcdFontSpaceX(1);
				LcdFontSpaceY(0);
				SetFont(Font_Pe_12X11);
				SetTextPixelPosition(74, 31);
				PutsString("پسورد جدید ذخیره شد");
				GLCD_Screen_Update();
				DELAY_MS(3000);

				return;
			}
			else
			{
				GLCD_FillRectangle(25, 14, 214, 58, BLACK);
				GLCD_FillRectangle(27, 16, 212, 56, WHITE);

				SetFontLetter(PERSIAN_LETTER);
				SetFontColor(BLACK);
				SetBackgroundColor(WHITE);
				LcdFontSpaceX(1);
				LcdFontSpaceY(0);
				SetFont(Font_Pe_12X11);
				SetTextPixelPosition(65, 31);
				PutsString("عدم تطبیق مقادیر ورودی");
				GLCD_Screen_Update();
				DELAY_MS(3000);
			}
		}

//----------------------------------------------------------------------------------------------------------

		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFont(Font_Pe_9X8);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetTextPixelPosition(31, 2);
		PutsString("تغییر پسورد سیستم )ssaP metsyS(");

//----------------------------------------------------------------------------------------------------------

		SetFontLetter(PERSIAN_LETTER);
		SetFont(Font_Pe_12X11);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);

		SetTextPixelPosition(5, 25);
		PutsString("1-وارد کردن پسورد جدید");
		SetFontLetter(ENGLISH_LETTER);
		SetTextPixelPosition(110, 25);
		PutChar(':');
		SetTextPixelPosition(60, 25);
		PutsString((char *)String_Password.c_str());

		SetFontLetter(PERSIAN_LETTER);
		SetTextPixelPosition(5, 40);
		PutsString("2-وارد کردن مجدد پسورد");
		SetFontLetter(ENGLISH_LETTER);
		SetTextPixelPosition(110, 40);
		PutChar(':');
		SetTextPixelPosition(60, 40);
		PutsString((char *)String_RepeatPassword.c_str());

//----------------------------------------------------------------------------------------------------------

		GLCD_Screen_Update();
	}
}

//**********************************************************************************************************

void SystemFunction_SetOperatorPassword(void)
{
	PanelKeypadButton_TypeDef Button;
	PanelKeypadButton_TypeDef LastPressButton = NO_PUSH;
	string String_Password = "000000";
	string String_RepeatPassword = "000000";

	while(1)
	{
		Button = Panel_ReadPanel();

		if(Button == KEYPAD_01)
		{
			GLCD_FillRectangle(58, 24, 101, 35, BLACK);
			SetFontLetter(ENGLISH_LETTER);
			LcdFontSpaceX(1);
			LcdFontSpaceY(0);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(60, 25);
			String_Password = GetStringInteger(&LastPressButton, String_Password);
		}
		else if(Button == KEYPAD_02)
		{
			GLCD_FillRectangle(58, 39, 101, 50, BLACK);
			SetFontLetter(ENGLISH_LETTER);
			LcdFontSpaceX(1);
			LcdFontSpaceY(0);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(60, 40);
			String_RepeatPassword = GetStringInteger(&LastPressButton, String_RepeatPassword);
		}
		else if((Button == KEYPAD_ENTER) || (Button == KEYPAD_ESC))
		{
			if(String_Password == String_RepeatPassword)
			{
				strcpy(SystemParameters.PinOperator, String_Password.c_str());
				SaveSystemParameters(SystemParameters);

				GLCD_FillRectangle(25, 14, 214, 58, BLACK);
				GLCD_FillRectangle(27, 16, 212, 56, WHITE);

				SetFontLetter(PERSIAN_LETTER);
				SetFontColor(BLACK);
				SetBackgroundColor(WHITE);
				LcdFontSpaceX(1);
				LcdFontSpaceY(0);
				SetFont(Font_Pe_12X11);
				SetTextPixelPosition(74, 31);
				PutsString("پسورد جدید ذخیره شد");
				GLCD_Screen_Update();
				DELAY_MS(3000);
				return;
			}
			else
			{
				GLCD_FillRectangle(25, 14, 214, 58, BLACK);
				GLCD_FillRectangle(27, 16, 212, 56, WHITE);

				SetFontLetter(PERSIAN_LETTER);
				SetFontColor(BLACK);
				SetBackgroundColor(WHITE);
				LcdFontSpaceX(1);
				LcdFontSpaceY(0);
				SetFont(Font_Pe_12X11);
				SetTextPixelPosition(65, 31);
				PutsString("عدم تطبیق مقادیر ورودی");
				GLCD_Screen_Update();
				DELAY_MS(3000);
			}
		}

//----------------------------------------------------------------------------------------------------------

		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFont(Font_Pe_9X8);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetTextPixelPosition(29, 2);
		PutsString("تغییر پسورد کاربر )ssaP rotarepO(");

//----------------------------------------------------------------------------------------------------------

		SetFontLetter(PERSIAN_LETTER);
		SetFont(Font_Pe_12X11);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);

		SetTextPixelPosition(5, 25);
		PutsString("1-وارد کردن پسورد جدید");
		SetFontLetter(ENGLISH_LETTER);
		SetTextPixelPosition(110, 25);
		PutChar(':');
		SetTextPixelPosition(60, 25);
		PutsString((char *)String_Password.c_str());

		SetFontLetter(PERSIAN_LETTER);
		SetTextPixelPosition(5, 40);
		PutsString("2-وارد کردن مجدد پسورد");
		SetFontLetter(ENGLISH_LETTER);
		SetTextPixelPosition(110, 40);
		PutChar(':');
		SetTextPixelPosition(60, 40);
		PutsString((char *)String_RepeatPassword.c_str());

//----------------------------------------------------------------------------------------------------------

		GLCD_Screen_Update();
	}
}

//**********************************************************************************************************

void SystemFunction_SetCommunicationParametersCOM1(void)
{
	uint8_t ParameterNumber = 0;
	PanelKeypadButton_TypeDef Button;
	PanelKeypadButton_TypeDef LastPressButton = NO_PUSH;
	Blinker_TypeDef Blinker;
	uint8_t IndexPage = 0;
	string Str = "";

	Blinker.AmountBlinker = AMOUNT_BLINKER_SET_PARAMETERS;

	SystemParameters.COM1.COM_Port = &huart2;

	while(1)
	{
		Button = Panel_ReadPanel();

		if((ParameterNumber == 0) && (Button == KEYPAD_DOWN))
		{
			if(IndexPage < 1)
				IndexPage++;
		}
		else if((ParameterNumber == 0) && (Button == KEYPAD_UP))
		{
			if(IndexPage > 0)
				IndexPage--;
		}
		else if(Button == KEYPAD_01)
		{
			if(IndexPage == 0)
				ParameterNumber = 1;
		}
		else if(Button == KEYPAD_02)
		{
			if(IndexPage == 0)
				ParameterNumber = 2;
		}
		else if(Button == KEYPAD_03)
		{
			if(IndexPage == 0)
				ParameterNumber = 3;
		}
		else if(Button == KEYPAD_04)
		{
			if(IndexPage == 1)
				ParameterNumber = 4;
		}
		else if(Button == KEYPAD_05)
		{
			if(IndexPage == 1)
				ParameterNumber = 5;
		}
		else if((Button == KEYPAD_ENTER) || (Button == KEYPAD_ESC))
		{
			if(ParameterNumber != 0)
				ParameterNumber = 0;
			else
			{
				ShowSaveParameters();
				SaveSystemParameters(SystemParameters);
				return;
			}
		}

//----------------------------------------------------------------------------------------------------------

		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_9X8);
		SetTextPixelPosition(44, 2);
		PutsString("تنظیمات پورت سریال 1 )232SR(");

//----------------------------------------------------------------------------------------------------------

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);

		if(IndexPage == 0)
		{
			SetFont(Font_Pe_12X11);
			SetTextPixelPosition(10, 18);
			PutsString("1-نرخ ارسال )etaR duaB(");
			SetFontLetter(ENGLISH_LETTER);
			SetTextPixelPosition(55, 18);
			PutChar(':');
			SetTextPixelPosition(10, 18);
			Str = IntegerToString(SystemParameters.COM1.BaudRate, 6);
			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetTextPixelPosition(10, 33);
			PutsString("2-بیت برابری )ytiraP(");
			SetFontLetter(ENGLISH_LETTER);
			SetTextPixelPosition(55, 33);
			PutChar(':');
			SetTextPixelPosition(10, 33);

			if(SystemParameters.COM1.Parity == 0)
				Str = "NONE";
			else if(SystemParameters.COM1.Parity == 1)
				Str = "EVEN";
			else if(SystemParameters.COM1.Parity == 2)
				Str = "ODD";

			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetTextPixelPosition(10, 48);
			PutsString("3-طول داده )stiB ataD(");
			SetFontLetter(ENGLISH_LETTER);
			SetTextPixelPosition(55, 48);
			PutChar(':');
			SetTextPixelPosition(10, 48);

			if(SystemParameters.COM1.DataBits == 0)
				Str = "7BIT";
			else if(SystemParameters.COM1.DataBits == 1)
				Str = "8BIT";
			else if(SystemParameters.COM1.DataBits == 2)
				Str = "9BIT";

			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetFont(Symbol_16);
			SetTextPixelPosition(0, 47);
			PutChar(0x01);
		}
		else if(IndexPage == 1)
		{
			SetFont(Font_Pe_12X11);
			SetTextPixelPosition(10, 18);
			PutsString("4-بیت توقف )stiB potS(");
			SetFontLetter(ENGLISH_LETTER);
			SetTextPixelPosition(55, 18);
			PutChar(':');
			SetTextPixelPosition(10, 18);

			if(SystemParameters.COM1.StopBits)
				Str = "1BIT";
			else
				Str = "2BIT";

			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetTextPixelPosition(10, 33);
			PutsString("5-فرمت خروجی )tamroF(");
			SetFontLetter(ENGLISH_LETTER);
			SetTextPixelPosition(80, 33);
			PutChar(':');
			SetTextPixelPosition(10, 33);

			if(SystemParameters.COM1.Operation == 0)
				Str = "GAMMA";
			else if(SystemParameters.COM1.Operation == 1)
				Str = "ALPHA";
			else if(SystemParameters.COM1.Operation == 2)
				Str = "TB";
			else if(SystemParameters.COM1.Operation == 3)
				Str = "LD5218";
			else if(SystemParameters.COM1.Operation == 4)
				Str = "YAOHUA";
			else if(SystemParameters.COM1.Operation == 5)
				Str = "OMEGA";
			else if(SystemParameters.COM1.Operation == 6)
				Str = "TEC1500";
			else if(SystemParameters.COM1.Operation == 7)
				Str = "TSM100";
			else if(SystemParameters.COM1.Operation == 8)
				Str = "CARDINAL";
			else if(SystemParameters.COM1.Operation == 9)
				Str = "SCHENCK";
			else if(SystemParameters.COM1.Operation == 10)
				Str = "PU800";
			else if(SystemParameters.COM1.Operation == 11)
				Str = "TT ARYA";

			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetFont(Symbol_16);
			SetTextPixelPosition(0, 14);
			PutChar(0x00);
		}

//----------------------------------------------------------------------------------------------------------

		if(ParameterNumber == 1)
		{
			GLCD_FillRectangle(5, 16, 50, 30, BLACK);

			SetFontLetter(ENGLISH_LETTER);
			SetFont(Font_Pe_12X11);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(10, 18);

			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.COM1.BaudRate, 6));

			if(LastPressButton == KEYPAD_ENTER)
			{
				SystemParameters.COM1.BaudRate = atoi(Str.c_str());
				ParameterNumber = 0;
			}
			else if(LastPressButton == KEYPAD_ESC)
				ParameterNumber = 0;
		}
		else if(ParameterNumber == 2)
		{
			GLCD_FillRectangle(5, 31, 50, 45, BLACK);

			if(Button == KEYPAD_LEFT)
			{
				if(SystemParameters.COM1.Parity > 0)
					SystemParameters.COM1.Parity --;
				else
					SystemParameters.COM1.Parity = 2;
			}
			else if(Button == KEYPAD_RIGHT)
			{
				if(SystemParameters.COM1.Parity < 2)
					SystemParameters.COM1.Parity ++;
				else
					SystemParameters.COM1.Parity = 0;
			}

			Blinker.Counter++;
			if(Blinker.Counter > Blinker.AmountBlinker)
			{
				SetFontLetter(ENGLISH_LETTER);
				SetFont(Font_Pe_12X11);
				SetFontColor(WHITE);
				SetBackgroundColor(BLACK);
				SetTextPixelPosition(10, 33);

				if(SystemParameters.COM1.Parity == 0)
					Str = "NONE";
				else if(SystemParameters.COM1.Parity == 1)
					Str = "EVEN";
				else if(SystemParameters.COM1.Parity == 2)
					Str = "ODD";

				PutsString((char *)Str.c_str());

				if(Blinker.Counter > (1.5 * Blinker.AmountBlinker))
					Blinker.Counter = 0;
			}
		}
		else if(ParameterNumber == 3)
		{
			GLCD_FillRectangle(5, 46, 50, 60, BLACK);

			if(Button == KEYPAD_LEFT)
			{
				if(SystemParameters.COM1.DataBits > 0)
					SystemParameters.COM1.DataBits --;
				else
					SystemParameters.COM1.DataBits = 2;
			}
			else if(Button == KEYPAD_RIGHT)
			{
				if(SystemParameters.COM1.DataBits < 2)
					SystemParameters.COM1.DataBits ++;
				else
					SystemParameters.COM1.DataBits = 0;
			}

			Blinker.Counter++;
			if(Blinker.Counter > Blinker.AmountBlinker)
			{
				SetFontLetter(ENGLISH_LETTER);
				SetFont(Font_Pe_12X11);
				SetFontColor(WHITE);
				SetBackgroundColor(BLACK);
				SetTextPixelPosition(10, 48);

				if(SystemParameters.COM1.DataBits == 0)
					Str = "7BIT";
				else if(SystemParameters.COM1.DataBits == 1)
					Str = "8BIT";
				else if(SystemParameters.COM1.DataBits == 2)
					Str = "9BIT";

				PutsString((char *)Str.c_str());

				if(Blinker.Counter > (1.5 * Blinker.AmountBlinker))
					Blinker.Counter = 0;
			}
		}
		else if(ParameterNumber == 4)
		{
			GLCD_FillRectangle(5, 16, 50, 30, BLACK);

			if((Button == KEYPAD_LEFT) || (Button == KEYPAD_RIGHT))
				SystemParameters.COM1.StopBits = !SystemParameters.COM1.StopBits;

			Blinker.Counter++;
			if(Blinker.Counter > Blinker.AmountBlinker)
			{
				SetFontLetter(ENGLISH_LETTER);
				SetFont(Font_Pe_12X11);
				SetFontColor(WHITE);
				SetBackgroundColor(BLACK);
				SetTextPixelPosition(10, 18);

				if(SystemParameters.COM1.StopBits)
					Str = "1BIT";
				else
					Str = "2BIT";

				PutsString((char *)Str.c_str());

				if(Blinker.Counter > (1.5 * Blinker.AmountBlinker))
					Blinker.Counter = 0;
			}
		}
		else if(ParameterNumber == 5)
		{
			GLCD_FillRectangle(5, 31, 75, 45, BLACK);

			if(Button == KEYPAD_LEFT)
			{
				if(SystemParameters.COM1.Operation > 0)
					SystemParameters.COM1.Operation --;
				else
					SystemParameters.COM1.Operation = 11;
			}
			else if(Button == KEYPAD_RIGHT)
			{
				if(SystemParameters.COM1.Operation < 11)
					SystemParameters.COM1.Operation ++;
				else
					SystemParameters.COM1.Operation = 0;
			}

			Blinker.Counter++;
			if(Blinker.Counter > Blinker.AmountBlinker)
			{
				SetFontLetter(ENGLISH_LETTER);
				SetFont(Font_Pe_12X11);
				SetFontColor(WHITE);
				SetBackgroundColor(BLACK);
				SetTextPixelPosition(10, 33);

				if(SystemParameters.COM1.Operation == 0)
					Str = "GAMMA";
				else if(SystemParameters.COM1.Operation == 1)
					Str = "ALPHA";
				else if(SystemParameters.COM1.Operation == 2)
					Str = "TB";
				else if(SystemParameters.COM1.Operation == 3)
					Str = "LD5218";
				else if(SystemParameters.COM1.Operation == 4)
					Str = "YAOHUA";
				else if(SystemParameters.COM1.Operation == 5)
					Str = "OMEGA";
				else if(SystemParameters.COM1.Operation == 6)
					Str = "TEC1500";
				else if(SystemParameters.COM1.Operation == 7)
					Str = "TSM100";
				else if(SystemParameters.COM1.Operation == 8)
					Str = "CARDINAL";
				else if(SystemParameters.COM1.Operation == 9)
					Str = "SCHENCK";
				else if(SystemParameters.COM1.Operation == 10)
					Str = "PU800";
				else if(SystemParameters.COM1.Operation == 11)
					Str = "TT ARYA";

				PutsString((char *)Str.c_str());

				if(Blinker.Counter > (1.5 * Blinker.AmountBlinker))
					Blinker.Counter = 0;
			}
		}

//----------------------------------------------------------------------------------------------------------

		GLCD_Screen_Update();
	}
}

//**********************************************************************************************************

void SystemFunction_SetCommunicationParametersCOM2(void)
{
	uint8_t ParameterNumber = 0;
	PanelKeypadButton_TypeDef Button;
	PanelKeypadButton_TypeDef LastPressButton = NO_PUSH;
	Blinker_TypeDef Blinker;
	uint8_t IndexPage = 0;
	string Str = "";

	Blinker.AmountBlinker = AMOUNT_BLINKER_SET_PARAMETERS;

	SystemParameters.COM2.COM_Port = &huart3;

	while(1)
	{
		Button = Panel_ReadPanel();

		if((ParameterNumber == 0) && (Button == KEYPAD_DOWN))
		{
			if(IndexPage < 1)
				IndexPage++;
		}
		else if((ParameterNumber == 0) && (Button == KEYPAD_UP))
		{
			if(IndexPage > 0)
				IndexPage--;
		}
		else if(Button == KEYPAD_01)
		{
			if(IndexPage == 0)
				ParameterNumber = 1;
		}
		else if(Button == KEYPAD_02)
		{
			if(IndexPage == 0)
				ParameterNumber = 2;
		}
		else if(Button == KEYPAD_03)
		{
			if(IndexPage == 0)
				ParameterNumber = 3;
		}
		else if(Button == KEYPAD_04)
		{
			if(IndexPage == 1)
				ParameterNumber = 4;
		}
		else if(Button == KEYPAD_05)
		{
			if(IndexPage == 1)
				ParameterNumber = 5;
		}
		else if((Button == KEYPAD_ENTER) || (Button == KEYPAD_ESC))
		{
			if(ParameterNumber != 0)
				ParameterNumber = 0;
			else
			{
				ShowSaveParameters();
				SaveSystemParameters(SystemParameters);
				return;
			}
		}

//----------------------------------------------------------------------------------------------------------

		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_9X8);
		SetTextPixelPosition(41, 2);
		PutsString("تنظیمات پورت سریال 2 )485SR(");

//----------------------------------------------------------------------------------------------------------

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);

		if(IndexPage == 0)
		{
			SetFont(Font_Pe_12X11);
			SetTextPixelPosition(10, 18);
			PutsString("1-نرخ ارسال )etaR duaB(");
			SetFontLetter(ENGLISH_LETTER);
			SetTextPixelPosition(55, 18);
			PutChar(':');
			SetTextPixelPosition(10, 18);
			Str = IntegerToString(SystemParameters.COM2.BaudRate, 6);
			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetTextPixelPosition(10, 33);
			PutsString("2-بیت برابری )ytiraP(");
			SetFontLetter(ENGLISH_LETTER);
			SetTextPixelPosition(55, 33);
			PutChar(':');
			SetTextPixelPosition(10, 33);

			if(SystemParameters.COM2.Parity == 0)
				Str = "NONE";
			else if(SystemParameters.COM2.Parity == 1)
				Str = "EVEN";
			else if(SystemParameters.COM2.Parity == 2)
				Str = "ODD";

			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetTextPixelPosition(10, 48);
			PutsString("3-طول داده )stiB ataD(");
			SetFontLetter(ENGLISH_LETTER);
			SetTextPixelPosition(55, 48);
			PutChar(':');
			SetTextPixelPosition(10, 48);

			if(SystemParameters.COM2.DataBits == 0)
				Str = "7BIT";
			else if(SystemParameters.COM2.DataBits == 1)
				Str = "8BIT";
			else if(SystemParameters.COM2.DataBits == 2)
				Str = "9BIT";

			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetFont(Symbol_16);
			SetTextPixelPosition(0, 47);
			PutChar(0x01);
		}
		else if(IndexPage == 1)
		{
			SetFont(Font_Pe_12X11);
			SetTextPixelPosition(10, 18);
			PutsString("4-بیت توقف )stiB potS(");
			SetFontLetter(ENGLISH_LETTER);
			SetTextPixelPosition(55, 18);
			PutChar(':');
			SetTextPixelPosition(10, 18);

			if(SystemParameters.COM2.StopBits)
				Str = "1BIT";
			else
				Str = "2BIT";

			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetTextPixelPosition(10, 33);
			PutsString("5-فرمت خروجی )tamroF(");
			SetFontLetter(ENGLISH_LETTER);
			SetTextPixelPosition(80, 33);
			PutChar(':');
			SetTextPixelPosition(10, 33);

			if(SystemParameters.COM2.Operation == 0)
				Str = "GAMMA";
			else if(SystemParameters.COM2.Operation == 1)
				Str = "ALPHA";
			else if(SystemParameters.COM2.Operation == 2)
				Str = "TB";
			else if(SystemParameters.COM2.Operation == 3)
				Str = "LD5218";
			else if(SystemParameters.COM2.Operation == 4)
				Str = "YAOHUA";
			else if(SystemParameters.COM2.Operation == 5)
				Str = "OMEGA";
			else if(SystemParameters.COM2.Operation == 6)
				Str = "TEC1500";
			else if(SystemParameters.COM2.Operation == 7)
				Str = "TSM100";
			else if(SystemParameters.COM2.Operation == 8)
				Str = "CARDINAL";
			else if(SystemParameters.COM2.Operation == 9)
				Str = "SCHENCK";
			else if(SystemParameters.COM2.Operation == 10)
				Str = "PU800";
			else if(SystemParameters.COM2.Operation == 11)
				Str = "TT ARYA";

			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetFont(Symbol_16);
			SetTextPixelPosition(0, 14);
			PutChar(0x00);
		}

//----------------------------------------------------------------------------------------------------------

		if(ParameterNumber == 1)
		{
			GLCD_FillRectangle(5, 16, 50, 30, BLACK);

			SetFontLetter(ENGLISH_LETTER);
			SetFont(Font_Pe_12X11);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(10, 18);

			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.COM2.BaudRate, 6));

			if(LastPressButton == KEYPAD_ENTER)
			{
				SystemParameters.COM2.BaudRate = atoi(Str.c_str());
				ParameterNumber = 0;
			}
			else if(LastPressButton == KEYPAD_ESC)
				ParameterNumber = 0;
		}
		else if(ParameterNumber == 2)
		{
			GLCD_FillRectangle(5, 31, 50, 45, BLACK);

			if(Button == KEYPAD_LEFT)
			{
				if(SystemParameters.COM2.Parity > 0)
					SystemParameters.COM2.Parity --;
				else
					SystemParameters.COM2.Parity = 2;
			}
			else if(Button == KEYPAD_RIGHT)
			{
				if(SystemParameters.COM2.Parity < 2)
					SystemParameters.COM2.Parity ++;
				else
					SystemParameters.COM2.Parity = 0;
			}

			Blinker.Counter++;
			if(Blinker.Counter > Blinker.AmountBlinker)
			{
				SetFontLetter(ENGLISH_LETTER);
				SetFont(Font_Pe_12X11);
				SetFontColor(WHITE);
				SetBackgroundColor(BLACK);
				SetTextPixelPosition(10, 33);

				if(SystemParameters.COM2.Parity == 0)
					Str = "NONE";
				else if(SystemParameters.COM2.Parity == 1)
					Str = "EVEN";
				else if(SystemParameters.COM2.Parity == 2)
					Str = "ODD";

				PutsString((char *)Str.c_str());

				if(Blinker.Counter > (1.5 * Blinker.AmountBlinker))
					Blinker.Counter = 0;
			}
		}
		else if(ParameterNumber == 3)
		{
			GLCD_FillRectangle(5, 46, 50, 60, BLACK);

			if(Button == KEYPAD_LEFT)
			{
				if(SystemParameters.COM2.DataBits > 0)
					SystemParameters.COM2.DataBits --;
				else
					SystemParameters.COM2.DataBits = 2;
			}
			else if(Button == KEYPAD_RIGHT)
			{
				if(SystemParameters.COM2.DataBits < 2)
					SystemParameters.COM2.DataBits ++;
				else
					SystemParameters.COM2.DataBits = 0;
			}

			Blinker.Counter++;
			if(Blinker.Counter > Blinker.AmountBlinker)
			{
				SetFontLetter(ENGLISH_LETTER);
				SetFont(Font_Pe_12X11);
				SetFontColor(WHITE);
				SetBackgroundColor(BLACK);
				SetTextPixelPosition(10, 48);

				if(SystemParameters.COM2.DataBits == 0)
					Str = "7BIT";
				else if(SystemParameters.COM2.DataBits == 1)
					Str = "8BIT";
				else if(SystemParameters.COM2.DataBits == 2)
					Str = "9BIT";

				PutsString((char *)Str.c_str());

				if(Blinker.Counter > (1.5 * Blinker.AmountBlinker))
					Blinker.Counter = 0;
			}
		}
		else if(ParameterNumber == 4)
		{
			GLCD_FillRectangle(5, 16, 50, 30, BLACK);

			if((Button == KEYPAD_LEFT) || (Button == KEYPAD_RIGHT))
				SystemParameters.COM2.StopBits = !SystemParameters.COM2.StopBits;

			Blinker.Counter++;
			if(Blinker.Counter > Blinker.AmountBlinker)
			{
				SetFontLetter(ENGLISH_LETTER);
				SetFont(Font_Pe_12X11);
				SetFontColor(WHITE);
				SetBackgroundColor(BLACK);
				SetTextPixelPosition(10, 18);

				if(SystemParameters.COM2.StopBits)
					Str = "1BIT";
				else
					Str = "2BIT";

				PutsString((char *)Str.c_str());

				if(Blinker.Counter > (1.5 * Blinker.AmountBlinker))
					Blinker.Counter = 0;
			}
		}
		else if(ParameterNumber == 5)
		{
			GLCD_FillRectangle(5, 31, 75, 45, BLACK);

			if(Button == KEYPAD_LEFT)
			{
				if(SystemParameters.COM2.Operation > 0)
					SystemParameters.COM2.Operation --;
				else
					SystemParameters.COM2.Operation = 11;
			}
			else if(Button == KEYPAD_RIGHT)
			{
				if(SystemParameters.COM2.Operation < 11)
					SystemParameters.COM2.Operation ++;
				else
					SystemParameters.COM2.Operation = 0;
			}

			Blinker.Counter++;
			if(Blinker.Counter > Blinker.AmountBlinker)
			{
				SetFontLetter(ENGLISH_LETTER);
				SetFont(Font_Pe_12X11);
				SetFontColor(WHITE);
				SetBackgroundColor(BLACK);
				SetTextPixelPosition(10, 33);

				if(SystemParameters.COM2.Operation == 0)
					Str = "GAMMA";
				else if(SystemParameters.COM2.Operation == 1)
					Str = "ALPHA";
				else if(SystemParameters.COM2.Operation == 2)
					Str = "TB";
				else if(SystemParameters.COM2.Operation == 3)
					Str = "LD5218";
				else if(SystemParameters.COM2.Operation == 4)
					Str = "YAOHUA";
				else if(SystemParameters.COM2.Operation == 5)
					Str = "OMEGA";
				else if(SystemParameters.COM2.Operation == 6)
					Str = "TEC1500";
				else if(SystemParameters.COM2.Operation == 7)
					Str = "TSM100";
				else if(SystemParameters.COM2.Operation == 8)
					Str = "CARDINAL";
				else if(SystemParameters.COM2.Operation == 9)
					Str = "SCHENCK";
				else if(SystemParameters.COM2.Operation == 10)
					Str = "PU800";
				else if(SystemParameters.COM2.Operation == 11)
					Str = "TT ARYA";

				PutsString((char *)Str.c_str());

				if(Blinker.Counter > (1.5 * Blinker.AmountBlinker))
					Blinker.Counter = 0;
			}
		}

//----------------------------------------------------------------------------------------------------------

		GLCD_Screen_Update();
	}
}

//**********************************************************************************************************

ScaleSettingParameters_TypeDef SystemFunction_GetScaleSettingParameters(void)
{
	return SystemParameters.CalibrationParameters.ScaleSettingParameters;
}

//**********************************************************************************************************

void SystemFunction_DefinitionProductCode(void)
{
	PanelKeypadButton_TypeDef Button;
	PanelKeypadButton_TypeDef LastPressButton = NO_PUSH;
	ProductParameters_TypeDef ProductParameters = {0};
	string Str = "";
	uint16_t ProductCode = 0;

	while(1)
	{
		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_9X8);
		SetTextPixelPosition(58, 2);
		PutsString("تعریف کد کالا و قیمت واحد");

//----------------------------------------------------------------------------------------------------------

		SetFontLetter(PERSIAN_LETTER);
		SetFont(Font_Pe_12X11);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);

		SetTextPixelPosition(5, 18);
		PutsString("کد کالا : ");
		SetTextPixelPosition(5, 33);
		PutsString("1-نام کالا : ");
		SetTextPixelPosition(5, 48);
		PutsString("2-قیمت واحد )تومان(");
		SetFontLetter(ENGLISH_LETTER);
		SetTextPixelPosition(66, 48);
		PutChar(':');

//----------------------------------------------------------------------------------------------------------

		GLCD_FillRectangle(68, 14, 172, 58, BLACK);
		GLCD_FillRectangle(70, 16, 170, 56, WHITE);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_12X11);
		SetTextPixelPosition(75, 31);
		PutsString("کد کالا : ");

		GLCD_FillRectangle(75, 30, 110, 41, BLACK);
		SetFontLetter(ENGLISH_LETTER);
		SetFont(Font_En_Num_8X10);
		LcdFontSpaceX(0);
		LcdFontSpaceY(0);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		SetTextPixelPosition(83, 31);

		Str = GetStringInteger(&LastPressButton, IntegerToString(ProductCode, 3));

		if(LastPressButton == KEYPAD_ENTER)
		{
			if(atol(Str.c_str()) < MAX_PRODUCT_CODE)
			{
			 	ProductCode = atol(Str.c_str());
				ProductParameters = ReadProductCode(ProductCode);
				break;
			}
			else
			{
				GLCD_FillRectangle(18, 14, 222, 58, BLACK);
				GLCD_FillRectangle(20, 16, 220, 56, WHITE);

				SetFontLetter(PERSIAN_LETTER);
				SetFontColor(BLACK);
				SetBackgroundColor(WHITE);
				LcdFontSpaceX(1);
				LcdFontSpaceY(0);
				SetFont(Font_Pe_12X11);
				SetTextPixelPosition(30, 31);
				PutsString("مقدار ورودی بیشتر از رنج مجاز می باشد");
				GLCD_Screen_Update();
				DELAY_MS(3000);

				continue;
			}
		}
		else
			return;
	}

//----------------------------------------------------------------------------------------------------------

	while(1)
	{
		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_9X8);
		SetTextPixelPosition(58, 2);
		PutsString("تعریف کد کالا و قیمت واحد");

//----------------------------------------------------------------------------------------------------------

		SetFontLetter(PERSIAN_LETTER);
		SetFont(Font_Pe_12X11);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);

		SetTextPixelPosition(5, 18);
		PutsString("کد کالا : ");
		Str = IntegerToString(ProductCode, 3);
		PutsString((char *)Str.c_str());

		SetTextPixelPosition(5, 33);
		PutsString("1-نام کالا : ");
		PutsString(ProductParameters.Name);

		SetTextPixelPosition(5, 48);
		PutsString("2-قیمت واحد )تومان(");
		SetFontLetter(ENGLISH_LETTER);
		SetTextPixelPosition(66, 48);
		PutChar(':');
		SetTextPixelPosition(10, 48);
		Str = IntegerToString(ProductParameters.Price, 7);
		PutsString((char *)Str.c_str());

//----------------------------------------------------------------------------------------------------------

		Button = Panel_ReadPanel();

		if(Button == KEYPAD_01)
		{
			GLCD_FillRectangle(5, 31, 181, 45, BLACK);

			SetFontLetter(PERSIAN_LETTER);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(60, 33);
			strcpy(ProductParameters.Name,
				   (char *)GetStringCharacter(&LastPressButton, ProductParameters.Name, MAX_PRODUCT_NAME).c_str());
		}
		else if(Button == KEYPAD_02)
		{
			GLCD_FillRectangle(5, 46, 61, 60, BLACK);

			SetFontLetter(ENGLISH_LETTER);
			SetFont(Font_Pe_12X11);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(10, 48);

			Str = GetStringInteger(&LastPressButton, IntegerToString(ProductParameters.Price, 7));

			if(LastPressButton == KEYPAD_ENTER)
				ProductParameters.Price = atoi(Str.c_str());
		}
		else if((Button == KEYPAD_ENTER) || (Button == KEYPAD_ESC))
		{
			ShowSaveParameters();
			SaveProductCode(ProductParameters , ProductCode);
			return;
		}

//----------------------------------------------------------------------------------------------------------

		GLCD_Screen_Update();
	}
}

//**********************************************************************************************************

void SystemFunction_DefinitionCustomerCode(void)
{
	PanelKeypadButton_TypeDef Button;
	PanelKeypadButton_TypeDef LastPressButton = NO_PUSH;
	CustomerParameters_TypeDef CustomerParameters = {0};
	string Str = "";
	uint16_t CustomerCode = 0;

	while(1)
	{
		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_9X8);
		SetTextPixelPosition(81, 2);
		PutsString("تعریف کد مشتری");

//----------------------------------------------------------------------------------------------------------

		SetFontLetter(PERSIAN_LETTER);
		SetFont(Font_Pe_12X11);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);

		SetTextPixelPosition(5, 25);
		PutsString("کد مشتری : ");
		SetTextPixelPosition(5, 40);
		PutsString("1-نام مشتری : ");

//----------------------------------------------------------------------------------------------------------

		GLCD_FillRectangle(68, 14, 172, 58, BLACK);
		GLCD_FillRectangle(70, 16, 170, 56, WHITE);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_12X11);
		SetTextPixelPosition(70, 31);
		PutsString("کد مشتری : ");

		GLCD_FillRectangle(73, 30, 108, 41, BLACK);
		SetFontLetter(ENGLISH_LETTER);
		SetFont(Font_En_Num_8X10);
		LcdFontSpaceX(0);
		LcdFontSpaceY(0);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		SetTextPixelPosition(81, 31);

		Str = GetStringInteger(&LastPressButton, IntegerToString(CustomerCode, 3));

		if(LastPressButton == KEYPAD_ENTER)
		{
			if(atol(Str.c_str()) < MAX_CUSTOMER_CODE)
			{
			 	CustomerCode = atol(Str.c_str());
				CustomerParameters = ReadCustomerCode(CustomerCode);
				break;
			}
			else
			{
				GLCD_FillRectangle(18, 14, 222, 58, BLACK);
				GLCD_FillRectangle(20, 16, 220, 56, WHITE);

				SetFontLetter(PERSIAN_LETTER);
				SetFontColor(BLACK);
				SetBackgroundColor(WHITE);
				LcdFontSpaceX(1);
				LcdFontSpaceY(0);
				SetFont(Font_Pe_12X11);
				SetTextPixelPosition(30, 31);
				PutsString("مقدار ورودی بیشتر از رنج مجاز می باشد");
				GLCD_Screen_Update();
				DELAY_MS(3000);

				continue;
			}
		}
		else
			return;
	}

//----------------------------------------------------------------------------------------------------------

	while(1)
	{
		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_9X8);
		SetTextPixelPosition(81, 2);
		PutsString("تعریف کد مشتری");

//----------------------------------------------------------------------------------------------------------

		SetFontLetter(PERSIAN_LETTER);
		SetFont(Font_Pe_12X11);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);

		SetTextPixelPosition(5, 25);
		PutsString("کد مشتری : ");
		Str = IntegerToString(CustomerCode, 3);
		PutsString((char *)Str.c_str());

		SetTextPixelPosition(5, 40);
		PutsString("1-نام مشتری : ");
		PutsString(CustomerParameters.Name);

//----------------------------------------------------------------------------------------------------------

		Button = Panel_ReadPanel();

		if(Button == KEYPAD_01)
		{
			GLCD_FillRectangle(5, 38, 168, 52, BLACK);

			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(72, 40);
			strcpy(CustomerParameters.Name,
				   (char *)GetStringCharacter(&LastPressButton, CustomerParameters.Name, MAX_CUSTOMER_NAME).c_str());
		}
		else if((Button == KEYPAD_ENTER) || (Button == KEYPAD_ESC))
		{
			ShowSaveParameters();
			SaveCustomerParameters(CustomerParameters , CustomerCode);
			return;
		}

//----------------------------------------------------------------------------------------------------------

		GLCD_Screen_Update();
	}
}

//**********************************************************************************************************

void SystemFunction_DefinitionWeighingCost(void)
{
	uint8_t ParameterNumber = 0;
	PanelKeypadButton_TypeDef Button;
	PanelKeypadButton_TypeDef LastPressButton = NO_PUSH;
	uint8_t IndexPage = 0;
	string Str = "";

	while(1)
	{
		Button = Panel_ReadPanel();

		if((ParameterNumber == 0) && (Button == KEYPAD_DOWN))
		{
			if(IndexPage < 2)
				IndexPage++;
		}
		else if((ParameterNumber == 0) && (Button == KEYPAD_UP))
		{
			if(IndexPage > 0)
				IndexPage--;
		}
		else if(Button == KEYPAD_01)
		{
			if(IndexPage == 0)
				ParameterNumber = 1;
		}
		else if(Button == KEYPAD_02)
		{
			if(IndexPage == 0)
				ParameterNumber = 2;
		}
		else if(Button == KEYPAD_03)
		{
			if(IndexPage == 0)
				ParameterNumber = 3;
		}
		else if(Button == KEYPAD_04)
		{
			if(IndexPage == 1)
				ParameterNumber = 4;
		}
		else if(Button == KEYPAD_05)
		{
			if(IndexPage == 1)
				ParameterNumber = 5;
		}
		else if(Button == KEYPAD_06)
		{
			if(IndexPage == 1)
				ParameterNumber = 6;
		}
		else if(Button == KEYPAD_07)
		{
			if(IndexPage == 2)
				ParameterNumber = 7;
		}
		else if((Button == KEYPAD_ENTER) || (Button == KEYPAD_ESC))
		{
			if(ParameterNumber != 0)
				ParameterNumber = 0;
			else
			{
				ShowSaveParameters();
				SaveSystemParameters(SystemParameters);
				return;
			}
		}

//----------------------------------------------------------------------------------------------------------

		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_9X8);
		SetTextPixelPosition(82, 2);
		PutsString("تعریف حق توزین");

//----------------------------------------------------------------------------------------------------------

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);

		if(IndexPage == 0)
		{
			SetFont(Font_Pe_12X11);
			SetTextPixelPosition(10, 18);
			PutsString("1-تریلی )تومان(");
			SetFontLetter(ENGLISH_LETTER);
			SetTextPixelPosition(66, 18);
			PutChar(':');
			SetTextPixelPosition(10, 18);
			Str = IntegerToString(SystemParameters.WeighingCost.Trolley, 7);
			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetTextPixelPosition(10, 33);
			PutsString("2-10 چرخ )تومان(");
			SetFontLetter(ENGLISH_LETTER);
			SetTextPixelPosition(66, 33);
			PutChar(':');
			SetTextPixelPosition(10, 33);
			Str = IntegerToString(SystemParameters.WeighingCost.Ten_Wheels, 7);
			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetTextPixelPosition(10, 48);
			PutsString("3-6 چرخ )تومان(");
			SetFontLetter(ENGLISH_LETTER);
			SetTextPixelPosition(66, 48);
			PutChar(':');
			SetTextPixelPosition(10, 48);
			Str = IntegerToString(SystemParameters.WeighingCost.Six_Wheels, 7);
			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetFont(Symbol_16);
			SetTextPixelPosition(0, 47);
			PutChar(0x01);
		}
		else if(IndexPage == 1)
		{
			SetFont(Font_Pe_12X11);
			SetTextPixelPosition(10, 18);
			PutsString("4-خاور )تومان(");
			SetFontLetter(ENGLISH_LETTER);
			SetTextPixelPosition(66, 18);
			PutChar(':');
			SetTextPixelPosition(10, 18);
			Str = IntegerToString(SystemParameters.WeighingCost.Khavar, 7);
			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetTextPixelPosition(10, 33);
			PutsString("5-نیسان )تومان(");
			SetFontLetter(ENGLISH_LETTER);
			SetTextPixelPosition(66, 33);
			PutChar(':');
			SetTextPixelPosition(10, 33);
			Str = IntegerToString(SystemParameters.WeighingCost.Nissan, 7);
			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetTextPixelPosition(10, 48);
			PutsString("6-وانت )تومان(");
			SetFontLetter(ENGLISH_LETTER);
			SetTextPixelPosition(66, 48);
			PutChar(':');
			SetTextPixelPosition(10, 48);
			Str = IntegerToString(SystemParameters.WeighingCost.PickupTruck, 7);
			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetFont(Symbol_16);
			SetTextPixelPosition(0, 14);
			PutChar(0x00);
			SetTextPixelPosition(0, 47);
			PutChar(0x01);
		}
		else if(IndexPage == 2)
		{
			SetFont(Font_Pe_12X11);
			SetTextPixelPosition(10, 18);
			PutsString("7-متفرقه )تومان(");
			SetFontLetter(ENGLISH_LETTER);
			SetTextPixelPosition(66, 18);
			PutChar(':');
			SetTextPixelPosition(10, 18);
			Str = IntegerToString(SystemParameters.WeighingCost.Other, 7);
			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetFont(Symbol_16);
			SetTextPixelPosition(0, 14);
			PutChar(0x00);
		}

//----------------------------------------------------------------------------------------------------------

		if(ParameterNumber == 1)
		{
			GLCD_FillRectangle(5, 16, 61, 30, BLACK);

			SetFontLetter(ENGLISH_LETTER);
			SetFont(Font_Pe_12X11);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(10, 18);

			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.WeighingCost.Trolley, 7));

			if(LastPressButton == KEYPAD_ENTER)
			{
				SystemParameters.WeighingCost.Trolley = atoi(Str.c_str());
				ParameterNumber = 0;
			}
			else if(LastPressButton == KEYPAD_ESC)
				ParameterNumber = 0;
		}
		else if(ParameterNumber == 2)
		{
			GLCD_FillRectangle(5, 31, 61, 45, BLACK);

			SetFontLetter(ENGLISH_LETTER);
			SetFont(Font_Pe_12X11);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(10, 33);

			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.WeighingCost.Ten_Wheels, 7));

			if(LastPressButton == KEYPAD_ENTER)
			{
				SystemParameters.WeighingCost.Ten_Wheels = atoi(Str.c_str());
				ParameterNumber = 0;
			}
			else if(LastPressButton == KEYPAD_ESC)
				ParameterNumber = 0;
		}
		else if(ParameterNumber == 3)
		{
			GLCD_FillRectangle(5, 46, 61, 60, BLACK);

			SetFontLetter(ENGLISH_LETTER);
			SetFont(Font_Pe_12X11);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(10, 48);

			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.WeighingCost.Six_Wheels, 7));

			if(LastPressButton == KEYPAD_ENTER)
			{
				SystemParameters.WeighingCost.Six_Wheels = atoi(Str.c_str());
				ParameterNumber = 0;
			}
			else if(LastPressButton == KEYPAD_ESC)
				ParameterNumber = 0;
		}
		else if(ParameterNumber == 4)
		{
			GLCD_FillRectangle(5, 16, 61, 30, BLACK);

			SetFontLetter(ENGLISH_LETTER);
			SetFont(Font_Pe_12X11);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(10, 18);

			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.WeighingCost.Khavar, 7));

			if(LastPressButton == KEYPAD_ENTER)
			{
				SystemParameters.WeighingCost.Khavar = atoi(Str.c_str());
				ParameterNumber = 0;
			}
			else if(LastPressButton == KEYPAD_ESC)
				ParameterNumber = 0;
		}
		else if(ParameterNumber == 5)
		{
			GLCD_FillRectangle(5, 31, 61, 45, BLACK);

			SetFontLetter(ENGLISH_LETTER);
			SetFont(Font_Pe_12X11);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(10, 33);

			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.WeighingCost.Nissan, 7));

			if(LastPressButton == KEYPAD_ENTER)
			{
				SystemParameters.WeighingCost.Nissan = atoi(Str.c_str());
				ParameterNumber = 0;
			}
			else if(LastPressButton == KEYPAD_ESC)
				ParameterNumber = 0;
		}
		else if(ParameterNumber == 6)
		{
			GLCD_FillRectangle(5, 46, 61, 60, BLACK);

			SetFontLetter(ENGLISH_LETTER);
			SetFont(Font_Pe_12X11);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(10, 48);

			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.WeighingCost.PickupTruck, 7));

			if(LastPressButton == KEYPAD_ENTER)
			{
				SystemParameters.WeighingCost.PickupTruck = atoi(Str.c_str());
				ParameterNumber = 0;
			}
			else if(LastPressButton == KEYPAD_ESC)
				ParameterNumber = 0;
		}
		else if(ParameterNumber == 7)
		{
			GLCD_FillRectangle(5, 16, 61, 30, BLACK);

			SetFontLetter(ENGLISH_LETTER);
			SetFont(Font_Pe_12X11);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(10, 18);

			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.WeighingCost.Other, 7));

			if(LastPressButton == KEYPAD_ENTER)
			{
				SystemParameters.WeighingCost.Other = atoi(Str.c_str());
				ParameterNumber = 0;
			}
			else if(LastPressButton == KEYPAD_ESC)
				ParameterNumber = 0;
		}

//----------------------------------------------------------------------------------------------------------

		GLCD_Screen_Update();
	}
}

//**********************************************************************************************************

void SystemFunction_DefinitionEmptyWeightCertainTrucks(void)
{
	PanelKeypadButton_TypeDef Button;
	PanelKeypadButton_TypeDef LastPressButton = NO_PUSH;
	CertainTrucksParameters_TypeDef CertainTrucksParameters = {0};
	string Str = "";
	int16_t Index = -1;

	while(1)
	{
		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_9X8);
		SetTextPixelPosition(44, 2);
		PutsString("تعریف وزن خالی کامیونهای خاص");

//----------------------------------------------------------------------------------------------------------

		SetFontLetter(PERSIAN_LETTER);
		SetFont(Font_Pe_12X11);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);

		SetTextPixelPosition(5, 25);
		PutsString("1-شماره پلاک : ");
		SetTextPixelPosition(5, 40);
		PutsString("2-وزن خالی");
		SetFontLetter(ENGLISH_LETTER);
		SetTextPixelPosition(60, 40);
		PutChar(':');

//----------------------------------------------------------------------------------------------------------

		GLCD_FillRectangle(38, 14, 202, 58, BLACK);
		GLCD_FillRectangle(40, 16, 200, 56, WHITE);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_12X11);
		SetTextPixelPosition(43, 31);
		PutsString("شماره پلاک : ");

		GLCD_FillRectangle(43, 29, 128, 43, BLACK);

		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		SetTextPixelPosition(115, 31);
		Str = GetStringCharacter(&LastPressButton, "", MAX_NUMBER_PLATES).c_str();

		if(LastPressButton == KEYPAD_ENTER)
		{
			for(int i = 0; i < MAX_CERTAIN_TRUCKS; i++)
			{
				CertainTrucksParameters = ReadCertainTrucksParameters(i);
				if(Str == (string)CertainTrucksParameters.NumberPlates)
				{
					Index = i;
					break;
				}
			}

			if(Index == -1)
			{
				for(int i = 0; i < MAX_CERTAIN_TRUCKS; i++)
				{
					CertainTrucksParameters = ReadCertainTrucksParameters(i);
					if((string)CertainTrucksParameters.NumberPlates == "")
					{
						Index = i;
						break;
					}
				}

				if(Index < MAX_CERTAIN_TRUCKS)
				{
					strcpy(CertainTrucksParameters.NumberPlates, (char *)Str.c_str());
					CertainTrucksParameters.EmptyWeight = 0;
				}
				else
				{
					GLCD_FillRectangle(38, 14, 202, 58, BLACK);
					GLCD_FillRectangle(40, 16, 200, 56, WHITE);

					SetFontLetter(PERSIAN_LETTER);
					SetFontColor(BLACK);
					SetBackgroundColor(WHITE);
					LcdFontSpaceX(1);
					LcdFontSpaceY(0);
					SetFont(Font_Pe_12X11);
					SetTextPixelPosition(77, 31);
					PutsString("حافظه پر شده است");
					GLCD_Screen_Update();
					DELAY_MS(3000);
					return;
				}
			}

			break;
		}
		else
			return;
	}

//----------------------------------------------------------------------------------------------------------

	while(1)
	{
		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_9X8);
		SetTextPixelPosition(44, 2);
		PutsString("تعریف وزن خالی کامیونهای خاص");

//----------------------------------------------------------------------------------------------------------

		SetFontLetter(PERSIAN_LETTER);
		SetFont(Font_Pe_12X11);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);

		SetTextPixelPosition(5, 25);
		PutsString("1-شماره پلاک : ");
		PutsString(CertainTrucksParameters.NumberPlates);
		SetTextPixelPosition(5, 40);
		PutsString("2-وزن خالی");
		SetFontLetter(ENGLISH_LETTER);
		SetTextPixelPosition(60, 40);
		PutChar(':');

		SetFontLetter(ENGLISH_LETTER);
		SetFont(Font_En_Num_8X10);
		LcdFontSpaceX(0);
		LcdFontSpaceY(0);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		SetTextPixelPosition(1, 40);

		Str = WeightToString(CertainTrucksParameters.EmptyWeight,
							 SystemParameters.CalibrationParameters.ScaleSettingParameters.DISPDigits,
							 SystemParameters.CalibrationParameters.ScaleSettingParameters.DECPoint,
							 SystemParameters.CalibrationParameters.ScaleSettingParameters.LeadZBlank);

		PutsString((char *)Str.c_str());

//----------------------------------------------------------------------------------------------------------

		Button = Panel_ReadPanel();

		if(Button == KEYPAD_01)
		{
			GLCD_FillRectangle(5, 23, 167, 37, BLACK);

			SetFontLetter(PERSIAN_LETTER);
			SetFont(Font_Pe_12X11);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			LcdFontSpaceX(1);
			LcdFontSpaceY(0);

			SetTextPixelPosition(73, 25);
			strcpy(CertainTrucksParameters.NumberPlates,
				   (char *)GetStringCharacter(&LastPressButton, CertainTrucksParameters.NumberPlates, MAX_NUMBER_PLATES).c_str());
		}
		else if(Button == KEYPAD_02)
		{
			GLCD_FillRectangle(3, 38, 57, 52, BLACK);
			SetFontLetter(ENGLISH_LETTER);
			SetFont(Font_En_Num_8X10);
			LcdFontSpaceX(0);
			LcdFontSpaceY(0);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(8, 40);

			Str = GetStringWeight(&LastPressButton, CertainTrucksParameters.EmptyWeight,
								  SystemParameters.CalibrationParameters.ScaleSettingParameters.DISPDigits,
								  SystemParameters.CalibrationParameters.ScaleSettingParameters.DECPoint);

			if(LastPressButton == KEYPAD_ENTER)
				 CertainTrucksParameters.EmptyWeight = atol(Str.c_str());
		}
		else if((Button == KEYPAD_ENTER) || (Button == KEYPAD_ESC))
		{
			ShowSaveParameters();
			SaveCertainTrucksParameters(CertainTrucksParameters, Index);
			return;
		}

//----------------------------------------------------------------------------------------------------------

		GLCD_Screen_Update();
	}
}

//**********************************************************************************************************

void SystemFunction_SetPrintFormat(void)
{
	uint8_t ParameterNumber = 0;
	PanelKeypadButton_TypeDef Button;
	Blinker_TypeDef Blinker;
	uint8_t IndexPage = 0;
	string Str = "";

	Blinker.AmountBlinker = AMOUNT_BLINKER_SET_PARAMETERS;

	while(1)
	{
		Button = Panel_ReadPanel();

		if((ParameterNumber == 0) && (Button == KEYPAD_DOWN))
		{
			if(IndexPage < 4)
				IndexPage++;
		}
		else if((ParameterNumber == 0) && (Button == KEYPAD_UP))
		{
			if(IndexPage > 0)
				IndexPage--;
		}
		else if(Button == KEYPAD_00)
		{
			if(IndexPage == 3)
				ParameterNumber = 10;
		}
		else if(Button == KEYPAD_01)
		{
			if(IndexPage == 0)
				ParameterNumber = 1;
			else if(IndexPage == 3)
				ParameterNumber = 11;
		}
		else if(Button == KEYPAD_02)
		{
			if(IndexPage == 0)
				ParameterNumber = 2;
			else if(IndexPage == 3)
				ParameterNumber = 12;
		}
		else if(Button == KEYPAD_03)
		{
			if(IndexPage == 0)
				ParameterNumber = 3;
			else if(IndexPage == 4)
				ParameterNumber = 13;
		}
		else if(Button == KEYPAD_04)
		{
			if(IndexPage == 1)
				ParameterNumber = 4;
		}
		else if(Button == KEYPAD_05)
		{
			if(IndexPage == 1)
				ParameterNumber = 5;
		}
		else if(Button == KEYPAD_06)
		{
			if(IndexPage == 1)
				ParameterNumber = 6;
		}
		else if(Button == KEYPAD_07)
		{
			if(IndexPage == 2)
				ParameterNumber = 7;
		}
		else if(Button == KEYPAD_08)
		{
			if(IndexPage == 2)
				ParameterNumber = 8;
		}
		else if(Button == KEYPAD_09)
		{
			if(IndexPage == 2)
				ParameterNumber = 9;
		}
		else if((Button == KEYPAD_ENTER) || (Button == KEYPAD_ESC))
		{
			if(ParameterNumber != 0)
				ParameterNumber = 0;
			else
			{
				ShowSaveParameters();
				SaveSystemParameters(SystemParameters);
				Printer_SetPrinterType(SystemParameters.PrintFormatParameters.TypePrinter , SystemParameters.PrintFormatParameters.TypePaper);
				return;
			}
		}

//----------------------------------------------------------------------------------------------------------

		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_9X8);
		SetTextPixelPosition(51, 2);
		PutsString("فرمت چاپ )tamroF tnirP(");

//----------------------------------------------------------------------------------------------------------

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);

		if(IndexPage == 0)
		{
			SetFont(Font_Pe_12X11);
			SetTextPixelPosition(10, 18);
			PutsString("1-چاپ تیتر در قبض باسکول");
			SetTextPixelPosition(170, 18);
			PutChar(':');
			SetTextPixelPosition(180, 18);

			if(SystemParameters.PrintFormatParameters.PrintTitle)
				Str = "فعال";
			else
				Str = "غیر فعال";

			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetTextPixelPosition(10, 33);
			PutsString("2-فونت چاپ");
			SetTextPixelPosition(170, 33);
			PutChar(':');
			SetTextPixelPosition(180, 33);

			if(SystemParameters.PrintFormatParameters.PrintFont == 0)
				Str = "فونت 1";
			else if(SystemParameters.PrintFormatParameters.PrintFont == 1)
				Str = "فونت 2";
			else if(SystemParameters.PrintFormatParameters.PrintFont == 2)
				Str = "فونت 3";
			else if(SystemParameters.PrintFormatParameters.PrintFont == 3)
				Str = "فونت 4";

			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetTextPixelPosition(10, 48);
			PutsString("3-تعداد دفعات چاپ قبض");
			SetTextPixelPosition(170, 48);
			PutChar(':');
			SetTextPixelPosition(180, 48);
			Str = IntegerToString((uint8_t) SystemParameters.PrintFormatParameters.NumberOfPrint, 1);
			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetFont(Symbol_16);
			SetTextPixelPosition(0, 47);
			PutChar(0x01);
		}
		else if(IndexPage == 1)
		{
			SetFont(Font_Pe_12X11);
			SetTextPixelPosition(10, 18);
			PutsString("4-فرمت قبض باسکول");
			SetTextPixelPosition(170, 18);
			PutChar(':');
			SetTextPixelPosition(180, 18);

			if(SystemParameters.PrintFormatParameters.PrintType == 0)
				Str = "استاندارد 1";
			else if(SystemParameters.PrintFormatParameters.PrintType == 1)
				Str = "استاندارد 2";
			else if(SystemParameters.PrintFormatParameters.PrintType == 2)
				Str = "کاربر";

			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetTextPixelPosition(10, 33);
			PutsString("5-تعریف فرمت دلخواه");

			SetFontLetter(PERSIAN_LETTER);
			SetTextPixelPosition(10, 48);
			PutsString("6-تعریف مشخصات واحد تولیدی");

			SetFontLetter(PERSIAN_LETTER);
			SetFont(Symbol_16);
			SetTextPixelPosition(0, 14);
			PutChar(0x00);
			SetTextPixelPosition(0, 47);
			PutChar(0x01);
		}
		else if(IndexPage == 2)
		{
			SetFont(Font_Pe_12X11);
			SetTextPixelPosition(10, 18);
			PutsString("7-فاصله خالی بالای قبض");
			SetTextPixelPosition(170, 18);
			PutChar(':');
			SetTextPixelPosition(180, 18);

			Str = IntegerToString((uint8_t) SystemParameters.PrintFormatParameters.TopLineEmpty, 1);
			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetTextPixelPosition(10, 33);
			PutsString("8-نحوه چاپ توزین دوم");
			SetTextPixelPosition(170, 33);
			PutChar(':');
			SetTextPixelPosition(180, 33);

			if(SystemParameters.PrintFormatParameters.SecondWeighingPrint)
				Str = "کامل";
			else
				Str = "ناقص";

			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetTextPixelPosition(10, 48);
			PutsString("9-خروج قبض");
			SetTextPixelPosition(170, 48);
			PutChar(':');
			SetTextPixelPosition(180, 48);

			if(SystemParameters.PrintFormatParameters.EjectSheet)
				Str = "فعال";
			else
				Str = "غیر فعال";

			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetFont(Symbol_16);
			SetTextPixelPosition(0, 14);
			PutChar(0x00);
			SetTextPixelPosition(0, 47);
			PutChar(0x01);
		}
		else if(IndexPage == 3)
		{
			SetFont(Font_Pe_12X11);
			SetTextPixelPosition(10, 18);
			PutsString("10-نوع چاپگر");
			SetTextPixelPosition(170, 18);
			PutChar(':');
			SetTextPixelPosition(180, 18);

			if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
				Str = "سوزنی";
			else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
				Str = "لیزریTPL";
			else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
				Str = "لیزریBSU";
			else if(SystemParameters.PrintFormatParameters.TypePrinter == 3)
				Str = "حرارتی";

			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetTextPixelPosition(10, 33);
			PutsString("11-تعداد برگ چاپ قبض");
			SetTextPixelPosition(170, 33);
			PutChar(':');
			SetTextPixelPosition(180, 33);
			Str = IntegerToString((uint8_t) SystemParameters.PrintFormatParameters.NumberOfPaperPrint, 1);
			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetTextPixelPosition(10, 48);
			PutsString("12-ابعاد کاغذ برای پرینتر لیزری");
			SetTextPixelPosition(170, 48);
			PutChar(':');
			SetFontLetter(ENGLISH_LETTER);
			SetTextPixelPosition(45, 48);

			if(SystemParameters.PrintFormatParameters.TypePaper)
				Str = "A4";
			else
				Str = "A5";

			PutsString((char *)Str.c_str());
			
			SetFontLetter(PERSIAN_LETTER);
			SetFont(Symbol_16);
			SetTextPixelPosition(0, 14);
			PutChar(0x00);
			SetTextPixelPosition(0, 47);
			PutChar(0x01);
		}
		else if(IndexPage == 4)
		{
			SetFont(Font_Pe_12X11);
			SetTextPixelPosition(10, 18);
			PutsString("13-فاصله خالی پایین قبض");
			SetTextPixelPosition(170, 18);
			PutChar(':');
			SetTextPixelPosition(180, 18);

			Str = IntegerToString((uint8_t) SystemParameters.PrintFormatParameters.BottomLineEmpty, 1);
			PutsString((char *)Str.c_str());

			SetFontLetter(PERSIAN_LETTER);
			SetFont(Symbol_16);
			SetTextPixelPosition(0, 14);
			PutChar(0x00);
		}

//----------------------------------------------------------------------------------------------------------

		if(ParameterNumber == 1)
		{
			GLCD_FillRectangle(5, 16, 60, 30, BLACK);

			if((Button == KEYPAD_LEFT)  || (Button == KEYPAD_RIGHT))
				SystemParameters.PrintFormatParameters.PrintTitle = !SystemParameters.PrintFormatParameters.PrintTitle;

			Blinker.Counter++;
			if(Blinker.Counter > Blinker.AmountBlinker)
			{
				SetFontLetter(PERSIAN_LETTER);
				SetFont(Font_Pe_12X11);
				SetFontColor(WHITE);
				SetBackgroundColor(BLACK);
				SetTextPixelPosition(180, 18);

				if(SystemParameters.PrintFormatParameters.PrintTitle)
					Str = "فعال";
				else
					Str = "غیر فعال";

				PutsString((char *)Str.c_str());

				if(Blinker.Counter > (1.5 * Blinker.AmountBlinker))
					Blinker.Counter = 0;
			}
		}
		else if(ParameterNumber == 2)
		{
			GLCD_FillRectangle(5, 31, 60, 45, BLACK);

			if(Button == KEYPAD_LEFT)
			{
				if(SystemParameters.PrintFormatParameters.PrintFont > 0)
					SystemParameters.PrintFormatParameters.PrintFont --;
				else
					SystemParameters.PrintFormatParameters.PrintFont = 3;
			}
			else if(Button == KEYPAD_RIGHT)
			{
				if(SystemParameters.PrintFormatParameters.PrintFont < 3)
					SystemParameters.PrintFormatParameters.PrintFont ++;
				else
					SystemParameters.PrintFormatParameters.PrintFont = 0;
			}

			Blinker.Counter++;
			if(Blinker.Counter > Blinker.AmountBlinker)
			{
				SetFontLetter(PERSIAN_LETTER);
				SetFont(Font_Pe_12X11);
				SetFontColor(WHITE);
				SetBackgroundColor(BLACK);
				SetTextPixelPosition(180, 33);

				if(SystemParameters.PrintFormatParameters.PrintFont == 0)
					Str = "فونت 1";
				else if(SystemParameters.PrintFormatParameters.PrintFont == 1)
					Str = "فونت 2";
				else if(SystemParameters.PrintFormatParameters.PrintFont == 2)
					Str = "فونت 3";
				else if(SystemParameters.PrintFormatParameters.PrintFont == 3)
					Str = "فونت 4";

				PutsString((char *)Str.c_str());

				if(Blinker.Counter > (1.5 * Blinker.AmountBlinker))
					Blinker.Counter = 0;
			}
		}
		else if(ParameterNumber == 3)
		{
			GLCD_FillRectangle(5, 46, 60, 60, BLACK);

			if(Button == KEYPAD_LEFT)
			{
				if(SystemParameters.PrintFormatParameters.NumberOfPrint > 1)
					SystemParameters.PrintFormatParameters.NumberOfPrint --;
				else
					SystemParameters.PrintFormatParameters.NumberOfPrint = 5;
			}
			else if(Button == KEYPAD_RIGHT)
			{
				if(SystemParameters.PrintFormatParameters.NumberOfPrint < 5)
					SystemParameters.PrintFormatParameters.NumberOfPrint ++;
				else
					SystemParameters.PrintFormatParameters.NumberOfPrint = 1;
			}

			Blinker.Counter++;
			if(Blinker.Counter > Blinker.AmountBlinker)
			{
				SetFontLetter(PERSIAN_LETTER);
				SetFont(Font_Pe_12X11);
				SetFontColor(WHITE);
				SetBackgroundColor(BLACK);
				SetTextPixelPosition(180, 48);
				Str = IntegerToString((uint8_t) SystemParameters.PrintFormatParameters.NumberOfPrint, 1);
				PutsString((char *)Str.c_str());

				if(Blinker.Counter > (1.5 * Blinker.AmountBlinker))
					Blinker.Counter = 0;
			}
		}
		else if(ParameterNumber == 4)
		{
			GLCD_FillRectangle(5, 16, 60, 30, BLACK);

			if(Button == KEYPAD_LEFT)
			{
				if(SystemParameters.PrintFormatParameters.PrintType > 0)
					SystemParameters.PrintFormatParameters.PrintType --;
				else
					SystemParameters.PrintFormatParameters.PrintType = 2;
			}
			else if(Button == KEYPAD_RIGHT)
			{
				if(SystemParameters.PrintFormatParameters.PrintType < 2)
					SystemParameters.PrintFormatParameters.PrintType ++;
				else
					SystemParameters.PrintFormatParameters.PrintType = 0;
			}

			Blinker.Counter++;
			if(Blinker.Counter > Blinker.AmountBlinker)
			{
				SetFontLetter(PERSIAN_LETTER);
				SetFont(Font_Pe_12X11);
				SetFontColor(WHITE);
				SetBackgroundColor(BLACK);
				SetTextPixelPosition(180, 18);

				if(SystemParameters.PrintFormatParameters.PrintType == 0)
					Str = "استاندارد 1";
				else if(SystemParameters.PrintFormatParameters.PrintType == 1)
					Str = "استاندارد 2";
				else if(SystemParameters.PrintFormatParameters.PrintType == 2)
					Str = "کاربر";

				PutsString((char *)Str.c_str());

				if(Blinker.Counter > (1.5 * Blinker.AmountBlinker))
					Blinker.Counter = 0;
			}
		}
		else if(ParameterNumber == 5)
		{
			DefinitionPrintFormat();
			ParameterNumber = 0;
		}
		else if(ParameterNumber == 6)
		{
			SystemFunction_DefinitionProductFactoryInformation();
			ParameterNumber = 0;
		}
		else if(ParameterNumber == 7)
		{
			GLCD_FillRectangle(5, 16, 60, 30, BLACK);

			if(Button == KEYPAD_LEFT)
			{
				if(SystemParameters.PrintFormatParameters.TopLineEmpty > 0)
					SystemParameters.PrintFormatParameters.TopLineEmpty --;
				else
					SystemParameters.PrintFormatParameters.TopLineEmpty = 9;
			}
			else if(Button == KEYPAD_RIGHT)
			{
				if(SystemParameters.PrintFormatParameters.TopLineEmpty < 9)
					SystemParameters.PrintFormatParameters.TopLineEmpty ++;
				else
					SystemParameters.PrintFormatParameters.TopLineEmpty = 0;
			}

			Blinker.Counter++;
			if(Blinker.Counter > Blinker.AmountBlinker)
			{
				SetFontLetter(PERSIAN_LETTER);
				SetFont(Font_Pe_12X11);
				SetFontColor(WHITE);
				SetBackgroundColor(BLACK);
				SetTextPixelPosition(180, 18);
				Str = IntegerToString((uint8_t) SystemParameters.PrintFormatParameters.TopLineEmpty, 1);
				PutsString((char *)Str.c_str());

				if(Blinker.Counter > (1.5 * Blinker.AmountBlinker))
					Blinker.Counter = 0;
			}
		}
		else if(ParameterNumber == 8)
		{
			GLCD_FillRectangle(5, 31, 60, 45, BLACK);

			if((Button == KEYPAD_LEFT)  || (Button == KEYPAD_RIGHT))
				SystemParameters.PrintFormatParameters.SecondWeighingPrint = !SystemParameters.PrintFormatParameters.SecondWeighingPrint;

			Blinker.Counter++;
			if(Blinker.Counter > Blinker.AmountBlinker)
			{
				SetFontLetter(PERSIAN_LETTER);
				SetFont(Font_Pe_12X11);
				SetFontColor(WHITE);
				SetBackgroundColor(BLACK);
				SetTextPixelPosition(180, 33);

				if(SystemParameters.PrintFormatParameters.SecondWeighingPrint)
					Str = "کامل";
				else
					Str = "ناقص";

				PutsString((char *)Str.c_str());

				if(Blinker.Counter > (1.5 * Blinker.AmountBlinker))
					Blinker.Counter = 0;
			}
		}
		else if(ParameterNumber == 9)
		{
			GLCD_FillRectangle(5, 46, 60, 60, BLACK);

			if((Button == KEYPAD_LEFT)  || (Button == KEYPAD_RIGHT))
				SystemParameters.PrintFormatParameters.EjectSheet = !SystemParameters.PrintFormatParameters.EjectSheet;

			Blinker.Counter++;
			if(Blinker.Counter > Blinker.AmountBlinker)
			{
				SetFontLetter(PERSIAN_LETTER);
				SetFont(Font_Pe_12X11);
				SetFontColor(WHITE);
				SetBackgroundColor(BLACK);
				SetTextPixelPosition(180, 48);

				if(SystemParameters.PrintFormatParameters.EjectSheet)
					Str = "فعال";
				else
					Str = "غیر فعال";

				PutsString((char *)Str.c_str());

				if(Blinker.Counter > (1.5 * Blinker.AmountBlinker))
					Blinker.Counter = 0;
			}
		}
		else if(ParameterNumber == 10)
		{
			GLCD_FillRectangle(5, 16, 60, 30, BLACK);

			if(Button == KEYPAD_LEFT)
			{
				if(SystemParameters.PrintFormatParameters.TypePrinter > 0)
					SystemParameters.PrintFormatParameters.TypePrinter --;
				else
					SystemParameters.PrintFormatParameters.TypePrinter = 3;
			}
			else if(Button == KEYPAD_RIGHT)
			{
				if(SystemParameters.PrintFormatParameters.TypePrinter < 3)
					SystemParameters.PrintFormatParameters.TypePrinter ++;
				else
					SystemParameters.PrintFormatParameters.TypePrinter = 0;
			}
			
			Blinker.Counter++;
			if(Blinker.Counter > Blinker.AmountBlinker)
			{
				SetFontLetter(PERSIAN_LETTER);
				SetFont(Font_Pe_12X11);
				SetFontColor(WHITE);
				SetBackgroundColor(BLACK);
				SetTextPixelPosition(180, 18);

				if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
					Str = "سوزنی";
				else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
					Str = "لیزریTPL";
				else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
					Str = "لیزریBSU";
				else if(SystemParameters.PrintFormatParameters.TypePrinter == 3)
					Str = "حرارتی";

				PutsString((char *)Str.c_str());

				if(Blinker.Counter > (1.5 * Blinker.AmountBlinker))
					Blinker.Counter = 0;
			}
		}
		else if(ParameterNumber == 11)
		{
			GLCD_FillRectangle(5, 31, 60, 45, BLACK);

			if(Button == KEYPAD_LEFT)
			{
				if(SystemParameters.PrintFormatParameters.NumberOfPaperPrint > 1)
					SystemParameters.PrintFormatParameters.NumberOfPaperPrint --;
				else
					SystemParameters.PrintFormatParameters.NumberOfPaperPrint = 5;
			}
			else if(Button == KEYPAD_RIGHT)
			{
				if(SystemParameters.PrintFormatParameters.NumberOfPaperPrint < 5)
					SystemParameters.PrintFormatParameters.NumberOfPaperPrint ++;
				else
					SystemParameters.PrintFormatParameters.NumberOfPaperPrint = 1;
			}

			Blinker.Counter++;
			if(Blinker.Counter > Blinker.AmountBlinker)
			{
				SetFontLetter(PERSIAN_LETTER);
				SetFont(Font_Pe_12X11);
				SetFontColor(WHITE);
				SetBackgroundColor(BLACK);
				SetTextPixelPosition(180, 33);
				Str = IntegerToString((uint8_t) SystemParameters.PrintFormatParameters.NumberOfPaperPrint, 1);
				PutsString((char *)Str.c_str());

				if(Blinker.Counter > (1.5 * Blinker.AmountBlinker))
					Blinker.Counter = 0;
			}
		}
		else if(ParameterNumber == 12)
		{
			GLCD_FillRectangle(5, 46, 60, 60, BLACK);

			if((Button == KEYPAD_LEFT)  || (Button == KEYPAD_RIGHT))
				SystemParameters.PrintFormatParameters.TypePaper = !SystemParameters.PrintFormatParameters.TypePaper;

			Blinker.Counter++;
			if(Blinker.Counter > Blinker.AmountBlinker)
			{
				SetFontLetter(ENGLISH_LETTER);
				SetFont(Font_Pe_12X11);
				SetFontColor(WHITE);
				SetBackgroundColor(BLACK);
				SetTextPixelPosition(45, 48);

				if(SystemParameters.PrintFormatParameters.TypePaper)
					Str = "A4";
				else
					Str = "A5";

				PutsString((char *)Str.c_str());

				if(Blinker.Counter > (1.5 * Blinker.AmountBlinker))
					Blinker.Counter = 0;
			}
		}
		else if(ParameterNumber == 13)
		{
			GLCD_FillRectangle(5, 16, 60, 30, BLACK);

			if(Button == KEYPAD_LEFT)
			{
				if(SystemParameters.PrintFormatParameters.BottomLineEmpty > 0)
					SystemParameters.PrintFormatParameters.BottomLineEmpty --;
				else
					SystemParameters.PrintFormatParameters.BottomLineEmpty = 9;
			}
			else if(Button == KEYPAD_RIGHT)
			{
				if(SystemParameters.PrintFormatParameters.BottomLineEmpty < 9)
					SystemParameters.PrintFormatParameters.BottomLineEmpty ++;
				else
					SystemParameters.PrintFormatParameters.BottomLineEmpty = 0;
			}

			Blinker.Counter++;
			if(Blinker.Counter > Blinker.AmountBlinker)
			{
				SetFontLetter(PERSIAN_LETTER);
				SetFont(Font_Pe_12X11);
				SetFontColor(WHITE);
				SetBackgroundColor(BLACK);
				SetTextPixelPosition(180, 18);
				Str = IntegerToString((uint8_t) SystemParameters.PrintFormatParameters.BottomLineEmpty, 1);
				PutsString((char *)Str.c_str());

				if(Blinker.Counter > (1.5 * Blinker.AmountBlinker))
					Blinker.Counter = 0;
			}
		}

//----------------------------------------------------------------------------------------------------------

		GLCD_Screen_Update();
	}
}

//**********************************************************************************************************

void SystemFunction_InitializeBackupMemory(void)
{
	if(!W25qxx_Init())
		ErrorSystemReport(E13_BACKUP_MEMORY);
}

//**********************************************************************************************************

string SystemFunction_GetPassword(PanelKeypadButton_TypeDef *LastPressButton)
{
	GLCD_FillRectangle(25, 14, 214, 58, BLACK);
	GLCD_FillRectangle(27, 16, 212, 56, WHITE);

	SetFontLetter(PERSIAN_LETTER);
	SetFontColor(BLACK);
	SetBackgroundColor(WHITE);
	LcdFontSpaceX(1);
	LcdFontSpaceY(0);
	SetFont(Font_Pe_12X11);
	SetTextPixelPosition(30, 31);
	PutsString("پسورد )drowssaP(  :");

	GLCD_FillRectangle(30, 29, 80, 43, BLACK);
	SetFontLetter(ENGLISH_LETTER);
	LcdFontSpaceX(0);
	LcdFontSpaceY(0);
	SetFontColor(WHITE);
	SetBackgroundColor(BLACK);
	SetTextPixelPosition(43, 31);

	return GetStringInteger(LastPressButton, DEFAULT_PASSWORD);
}

//**********************************************************************************************************

string SystemFunction_GetPinSystem(void)
{
	return SystemParameters.PinSystem;
}

//**********************************************************************************************************

string SystemFunction_GetPinOperator(void)
{
	return SystemParameters.PinOperator;
}

//**********************************************************************************************************

CommunicationParameters_TypeDef GetCommunicationParameters_COM1(void)
{
	return SystemParameters.COM1;
}

//**********************************************************************************************************

CommunicationParameters_TypeDef GetCommunicationParameters_COM2(void)
{
	return SystemParameters.COM2;
}

//**********************************************************************************************************

void SystemFunction_DeleteProductCode(void)
{
	PanelKeypadButton_TypeDef Button;
	ProductParameters_TypeDef ProductParameters = {0};

	while(1)
	{
		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_9X8);
		SetTextPixelPosition(31, 2);
		PutsString("پاک کردن حافظه کد کالا و قیمت واحد");

//----------------------------------------------------------------------------------------------------------

		GLCD_FillRectangle(25, 14, 214, 58, BLACK);
		GLCD_FillRectangle(27, 16, 212, 56, WHITE);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_12X11);
		SetTextPixelPosition(77, 31);
		PutsString("آیا مطمعن هستید?");

//----------------------------------------------------------------------------------------------------------

		GLCD_Screen_Update();

//----------------------------------------------------------------------------------------------------------

	   	Button = Panel_ReadPanel();

		if(Button == KEYPAD_ENTER)
		{
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

			strcpy(ProductParameters.Name, "");
			ProductParameters.Price = 0;

			for(int i = 0; i < MAX_PRODUCT_CODE;i++)
			{
				SaveProductCode(ProductParameters, i);
				GLCD_FillRectangle(57, 42, 182, 51, BLACK);
				GLCD_FillRectangle((58 + ((i * 124) / MAX_PRODUCT_CODE)), 43, 181, 50, WHITE);
				GLCD_Screen_Update();
			}

			ShowSaveParameters();
			return;
		}
		else if(Button == KEYPAD_ESC)
			return;
	}
}

//**********************************************************************************************************

void SystemFunction_DeleteCustomerCode(void)
{
	PanelKeypadButton_TypeDef Button;
	CustomerParameters_TypeDef CustomerParameters = {0};

	while(1)
	{
		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_9X8);
		SetTextPixelPosition(55, 2);
		PutsString("پاک کردن حافظه کد مشتری");

//----------------------------------------------------------------------------------------------------------

		GLCD_FillRectangle(25, 14, 214, 58, BLACK);
		GLCD_FillRectangle(27, 16, 212, 56, WHITE);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_12X11);
		SetTextPixelPosition(77, 31);
		PutsString("آیا مطمعن هستید?");

//----------------------------------------------------------------------------------------------------------

		GLCD_Screen_Update();

//----------------------------------------------------------------------------------------------------------

	   	Button = Panel_ReadPanel();

		if(Button == KEYPAD_ENTER)
		{
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

			strcpy(CustomerParameters.Name, "");

			for(int i = 0; i < MAX_CUSTOMER_CODE;i++)
			{
				SaveCustomerParameters(CustomerParameters, i);
				GLCD_FillRectangle(57, 42, 182, 51, BLACK);
				GLCD_FillRectangle((58 + ((i * 124) / MAX_CUSTOMER_CODE)), 43, 181, 50, WHITE);
				GLCD_Screen_Update();
			}

			ShowSaveParameters();
			return;
		}
		else if(Button == KEYPAD_ESC)
			return;
	}
}

//**********************************************************************************************************

void SystemFunction_DeleteEmptyWeightCertainTrucks(void)
{
	PanelKeypadButton_TypeDef Button;
	CertainTrucksParameters_TypeDef CertainTrucksParameters = {0};

	while(1)
	{
		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_9X8);
		SetTextPixelPosition(17, 2);
		PutsString("پاک کردن حافظه وزن خالی کامیونهای خاص");

//----------------------------------------------------------------------------------------------------------

		GLCD_FillRectangle(25, 14, 214, 58, BLACK);
		GLCD_FillRectangle(27, 16, 212, 56, WHITE);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_12X11);
		SetTextPixelPosition(77, 31);
		PutsString("آیا مطمعن هستید?");

//----------------------------------------------------------------------------------------------------------

		GLCD_Screen_Update();

//----------------------------------------------------------------------------------------------------------

	   	Button = Panel_ReadPanel();

		if(Button == KEYPAD_ENTER)
		{
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

			strcpy(CertainTrucksParameters.NumberPlates, "");
			CertainTrucksParameters.EmptyWeight = 0;

			for(int i = 0; i < MAX_CERTAIN_TRUCKS;i++)
			{
				SaveCertainTrucksParameters(CertainTrucksParameters, i);
				GLCD_FillRectangle(57, 42, 182, 51, BLACK);
				GLCD_FillRectangle((58 + ((i * 124) / MAX_CERTAIN_TRUCKS)), 43, 181, 50, WHITE);
				GLCD_Screen_Update();
			}

			ShowSaveParameters();
			return;
		}
		else if(Button == KEYPAD_ESC)
			return;
	}
}

//**********************************************************************************************************

void SystemFunction_DeleteWeightInformationMemory(void)
{
	PanelKeypadButton_TypeDef Button;

	while(1)
	{
		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_9X8);
		SetTextPixelPosition(65, 2);
		PutsString("پاک کردن حافظه توزین");

//----------------------------------------------------------------------------------------------------------

		GLCD_FillRectangle(25, 14, 214, 58, BLACK);
		GLCD_FillRectangle(27, 16, 212, 56, WHITE);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_12X11);
		SetTextPixelPosition(77, 31);
		PutsString("آیا مطمعن هستید?");

//----------------------------------------------------------------------------------------------------------

		GLCD_Screen_Update();

//----------------------------------------------------------------------------------------------------------

	   	Button = Panel_ReadPanel();

		if(Button == KEYPAD_ENTER)
		{
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

			SaveTicketSerialNumber(1);
			GLCD_FillRectangle(57, 42, 182, 51, BLACK);
			GLCD_FillRectangle((58 + 62), 43, 181, 50, WHITE);
			GLCD_Screen_Update();

			W25qxx_EraseChip();
			GLCD_FillRectangle(57, 42, 182, 51, BLACK);
			GLCD_FillRectangle((58 + 123), 43, 181, 50, WHITE);
			
			ShowSaveParameters();
			return;
		}
		else if(Button == KEYPAD_ESC)
			return;
	}
}

//**********************************************************************************************************

void SystemFunction_StartReadScale(void)
{
	Weight_Scale.ConfigParametersConfiguration(SystemParameters.CalibrationParameters, RespanceScale_01_QueueHandle , RespanceScale_02_QueueHandle);
}

//**********************************************************************************************************

StatusWeightTaring_TypeDef SystemFunction_TaringScale(void)
{
	return Weight_Scale.WeightTaring();
}

//**********************************************************************************************************

void SystemFunction_UnTaringScale(void)
{
	Weight_Scale.SetTareWeight(0);
}

//**********************************************************************************************************

void SystemFunction_GetWeightScale(WeightScale_TypeDef *WeightScale)
{
	StatusWeight_Function_TypeDef StatusWeight_Function;

	StatusWeight_Function = Weight_Scale.GetWeightScale(WeightScale);

	if(StatusWeight_Function == ERROR_AD7780_SCALE_01_WEIGHT_FUNCTION)
		ErrorSystemReport(E03_AD7780_SCALE01);
	else if(StatusWeight_Function == ERROR_SCALE_01_WEIGHT_FUNCTION)
		ErrorSystemReport(E01_READ_SCALE01);
	else if(StatusWeight_Function == ERROR_AD7780_SCALE_02_WEIGHT_FUNCTION)
		ErrorSystemReport(E04_AD7780_SCALE02);
	else if(StatusWeight_Function == ERROR_SCALE_02_WEIGHT_FUNCTION)
		ErrorSystemReport(E02_READ_SCALE02);
}

//**********************************************************************************************************

void SystemFunction_AutoZeroTracking(void)
{
	if(SystemParameters.CalibrationParameters.ScaleSettingParameters.AutoZTrack != 0)
		Weight_Scale.AutoZeroTracking();
}

//**********************************************************************************************************

void SystemFunction_SendWeightToDisplayExternal01(WeightScale_TypeDef WeightScale)
{
	string StringDisplayExternal;

	StringDisplayExternal = GetStringDisplayExternal(WeightScale, SystemParameters.COM1.Operation);

	if(StringDisplayExternal == "")
		return;

	if (HAL_UART_Transmit(SystemParameters.COM1.COM_Port, (uint8_t *)StringDisplayExternal.c_str(), StringDisplayExternal.length(), 1000) != HAL_OK)
		ErrorSystemReport(E06_COM01);
}

//**********************************************************************************************************

void SystemFunction_SendWeightToDisplayExternal02(WeightScale_TypeDef WeightScale)
{
	string StringDisplayExternal;

	StringDisplayExternal = GetStringDisplayExternal(WeightScale, SystemParameters.COM2.Operation);

	if(StringDisplayExternal == "")
		return;

	if (HAL_UART_Transmit(SystemParameters.COM2.COM_Port, (uint8_t *)StringDisplayExternal.c_str(), StringDisplayExternal.length(), 1000) != HAL_OK)
		ErrorSystemReport(E07_COM02);
}

//**********************************************************************************************************

void SystemFunction_ShowWeightScale(PanelKeypadButton_TypeDef *LastPressButton, WeightScale_TypeDef WeightScale, bool HighResolution)
{
	if(SystemFunction_ShowErrorSystem(LastPressButton))
		return;

//----------------------------------------------------------------------------------------------------------

	if(WeightScale.IsOverRange)
	{
		GLCD_FillRectangle(25, 14, 214, 58, BLACK);
		GLCD_FillRectangle(27, 16, 212, 56, WHITE);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_12X11);
		SetTextPixelPosition(48, 31);
		PutsString("مقدار وزن بیش از ظرفیت باسکول");

		return;
	}

//----------------------------------------------------------------------------------------------------------

	if(WeightScale.IsUnderRange)
	{
    	GLCD_FillRectangle(25, 14, 214, 58, BLACK);
		GLCD_FillRectangle(27, 16, 212, 56, WHITE);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_12X11);
		SetTextPixelPosition(56, 31);
		PutsString("هیچ فشاری روی لودسل نیست");

        return;
	}

//----------------------------------------------------------------------------------------------------------

	GLCD_FillRectangle(0, 0, 239, 51, WHITE);
	SetFontLetter(ENGLISH_LETTER);
	SetFontColor(BLACK);
	SetBackgroundColor(WHITE);

//----------------------------------------------------------------------------------------------------------

	if(HighResolution)
	{
		SetFont(Font_En_Num_8X10);
		SetTextPixelPosition(1, 41);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);

		PutsString((char *)WeightToString(WeightScale.HighResolution, WeightScale.DISPDigits, WeightScale.DECPoint, WeightScale.LeadZBlank).c_str());
	}

//----------------------------------------------------------------------------------------------------------

	if(WeightScale.IsStable)
	{
		SetFont(Symbol_16);
		SetTextPixelPosition(0, 5);
		PutChar(0x02);
	}

//----------------------------------------------------------------------------------------------------------

	if(WeightScale.IsNet)
	{
		SetFont(Symbol_16);
		SetTextPixelPosition(0, 15);
		PutChar(0x03);
	}

//----------------------------------------------------------------------------------------------------------

	SetFont(Font_En_Num_27X37);
	LcdFontSpaceX(1);
	LcdFontSpaceY(0);

	if(HighResolution)
	{
		if(WeightScale.DECPoint == 0)
			SetTextPixelPosition(27, 3);
		else
			SetTextPixelPosition(15, 3);
	}
	else
	{
		if(WeightScale.DECPoint == 0)
			SetTextPixelPosition(27, 7);
		else
			SetTextPixelPosition(15, 7);
	}

	if((WeightScale.DISPDigits == 4) && (WeightScale.DISPDigits > WeightScale.DECPoint))
    	PutsString("  ");
    else if((WeightScale.DISPDigits == 5) && (WeightScale.DISPDigits > WeightScale.DECPoint))
    	PutsString(" ");

//    if(HighResolution)
//    	PutsString((char *)WeightToString(WeightScale.HighResolution, WeightScale.DISPDigits, WeightScale.DECPoint, WeightScale.LeadZBlank).c_str());
//    else
    	PutsString((char *)WeightToString(WeightScale.Normally, WeightScale.DISPDigits, WeightScale.DECPoint, WeightScale.LeadZBlank).c_str());

//----------------------------------------------------------------------------------------------------------

	SetFontLetter(ENGLISH_LETTER);
	SetFont(Font_Pe_12X16);
	SetFontColor(BLACK);
	SetBackgroundColor(WHITE);
	LcdFontSpaceX(0);
	LcdFontSpaceY(0);
	SetTextPixelPosition(222, 30);
	PutsString("Kg");
}

//**********************************************************************************************************

bool SystemFunction_ShowErrorSystem(PanelKeypadButton_TypeDef *LastPressButton)
{
	PanelKeypadButton_TypeDef Button;
	ErrorSystem_TypeDef ErrorSystem;

	ErrorSystem = GetErrorSystem();

	if(ErrorSystem == NON_ERROR)
	{
		*LastPressButton = NO_PUSH;
		return false;
	}

	switch((uint8_t)ErrorSystem)
	{
		case E01_READ_SCALE01:
		{
			ShowMessageError("خطا 1: لودسل متصل نیست");
			break;
		}
		case E02_READ_SCALE02:
		{
			ShowMessageError("خطا 2: لودسل متصل نیست");
			break;
		}
		case E03_AD7780_SCALE01:
		{
			ShowMessageError("خطا 3: مبدل آنالوگ کفه 1");
			break;
		}
		case E04_AD7780_SCALE02:
		{
			ShowMessageError("خطا 4: مبدل آنالوگ کفه 2");
			break;
		}
		case E05_EEPROM:
		{
			ShowMessageError("خطا 5: حافظه سیستم");
			break;
		}
		case E06_COM01:
		{
			ShowMessageError("خطا 6: پورت سریال 1");
			break;
		}
		case E07_COM02:
		{
			ShowMessageError("خطا 7: پورت سریال 2");
			break;
		}
		case E08_COM03:
		{
			ShowMessageError("خطا 8: پورت سریال 3");
			break;
		}
		case E09_POWER_UP_ZERO:
		{
			ShowMessageError("خطا 9: صفرکردن وزن هنگام روشن شدن");
			break;
		}
		case E10_SCALE_TARED:
		{
			ShowMessageError("خطا 10: deraT elacS");
			break;
		}
		case E11_NEGATIVE_WEIGHTS:
		{
			ShowMessageError("خطا 11: وزن منفی است");
			break;
		}
		case E12_PRINTER:
		{
			ShowMessageError("خطا 12: پرینتر");
			break;
		}
		case E13_BACKUP_MEMORY:
		{
			ShowMessageError("خطا 13: حافظه توزین");
			break;
		}
		case E14_LIMIT_BACKUP_MEMORY:
		{
			ShowMessageError("خطا 14: حافظه توزین پر شده است");
			break;
		}
		case E15_SCALE_NOT_TARED:
		{
			ShowMessageError("خطا 15: deraT toN elacS");
			break;
		}
		case E16_USB_PRINTER:
		{
			ShowMessageError("خطا 16: پرینتر BSU");
			break;
		}
	}

	while(1)
	{
		Button = Panel_ReadPanel();

		if(Button == KEYPAD_CE)
		{
			ErrorSystemReset(ErrorSystem);
			*LastPressButton = Button;
			return true;
		}
		else
		{
			*LastPressButton = Button;
			return true;
		}

		DELAY_MS(100);
	}
}

//**********************************************************************************************************

void SystemFunction_AutoZeroAtPowerUp(void)
{
	uint32_t LastTickCount = 0;
	StatusWeight_Function_TypeDef StatusWeight_Function;
	WeightScale_TypeDef WeightScale = {0};

	if(SystemParameters.CalibrationParameters.ScaleSettingParameters.INITZero)
	{
		GLCD_FillRectangle(25, 14, 214, 58, BLACK);
		GLCD_FillRectangle(27, 16, 212, 56, WHITE);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_12X11);
		SetTextPixelPosition(74, 31);
		PutsString("صفر کردن وزن صفحه");
		GLCD_Screen_Update();

		Weight_Scale.ConfigParametersConfiguration(SystemParameters.CalibrationParameters, RespanceScale_01_QueueHandle , RespanceScale_02_QueueHandle);
		LastTickCount = osKernelGetTickCount();

		do
		{
			StatusWeight_Function = Weight_Scale.GetWeightScale(&WeightScale);
			if((osKernelGetTickCount() - LastTickCount) >= 3000)
				break;
		}while((StatusWeight_Function != OK_WEIGHT_FUNCTION) || (WeightScale.IsError) || (!WeightScale.IsStable));

		Weight_Scale.ZeroTracking();
		DELAY_MS(1000);
	}
}

//**********************************************************************************************************

StatusZeroTrack_TypeDef SystemFunction_ZeroingScale(void)
{
	return Weight_Scale.ZeroTracking();
}

//**********************************************************************************************************

void DefinitionPrintFormat(void)
{
	PanelKeypadButton_TypeDef Button;
	PanelKeypadButton_TypeDef LastPressButton;
	uint8_t ParameterNumber = 0;
	uint8_t IndexPage = 0;
	string Str = "";

	while(1)
	{
		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_9X8);
		SetTextPixelPosition(77, 2);
		PutsString("تعریف فرمت دلخواه");

		GLCD_FillRectangle(119, 13, 120, 63, BLACK);

//----------------------------------------------------------------------------------------------------------

		SetFontLetter(ENGLISH_LETTER);
		SetFont(Font_Pe_12X11);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);

//----------------------------------------------------------------------------------------------------------

		if(IndexPage == 0)
		{
			SetTextPixelPosition(5, 14);
			PutsString("1-Truck");
			SetTextPixelPosition(59, 14);
			PutChar(':');
			SetTextPixelPosition(70, 14);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.NumberPlates.Line, 2, true).c_str());
			SetTextPixelPosition(88, 14);
			PutChar('-');
			SetTextPixelPosition(97, 14);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.NumberPlates.Column, 2, true).c_str());

			SetTextPixelPosition(5, 26);
			PutsString("2-Produ");
			SetTextPixelPosition(59, 26);
			PutChar(':');
			SetTextPixelPosition(70, 26);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.Product.Line, 2, true).c_str());
			SetTextPixelPosition(88, 26);
			PutChar('-');
			SetTextPixelPosition(97, 26);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.Product.Column, 2, true).c_str());

			SetTextPixelPosition(5, 38);
			PutsString("3-Ticket");
			SetTextPixelPosition(59, 38);
			PutChar(':');
			SetTextPixelPosition(70, 38);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.TicketSerialNumber.Line, 2, true).c_str());
			SetTextPixelPosition(88, 38);
			PutChar('-');
			SetTextPixelPosition(97, 38);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.TicketSerialNumber.Column, 2, true).c_str());

			SetTextPixelPosition(5, 50);
			PutsString("4-Time_F");
			SetTextPixelPosition(59, 50);
			PutChar(':');
			SetTextPixelPosition(70, 50);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.SecondWeightTime.Line, 2, true).c_str());
			SetTextPixelPosition(88, 50);
			PutChar('-');
			SetTextPixelPosition(97, 50);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.SecondWeightTime.Column, 2, true).c_str());

			SetTextPixelPosition(125, 14);
			PutsString("5-Time_E");
			SetTextPixelPosition(179, 14);
			PutChar(':');
			SetTextPixelPosition(190, 14);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.FirstWeightTime.Line, 2, true).c_str());
			SetTextPixelPosition(208, 14);
			PutChar('-');
			SetTextPixelPosition(217, 14);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.FirstWeightTime.Column, 2, true).c_str());

			SetTextPixelPosition(125, 26);
			PutsString("6-W_F");
			SetTextPixelPosition(179, 26);
			PutChar(':');
			SetTextPixelPosition(190, 26);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.SecondWeight.Line, 2, true).c_str());
			SetTextPixelPosition(208, 26);
			PutChar('-');
			SetTextPixelPosition(217, 26);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.SecondWeight.Column, 2, true).c_str());

			SetTextPixelPosition(125, 38);
			PutsString("7-W_E");
			SetTextPixelPosition(179, 38);
			PutChar(':');
			SetTextPixelPosition(190, 38);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.FirstWeight.Line, 2, true).c_str());
			SetTextPixelPosition(208, 38);
			PutChar('-');
			SetTextPixelPosition(217, 38);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.FirstWeight.Column, 2, true).c_str());

			SetTextPixelPosition(125, 50);
			PutsString("8-W_N");
			SetTextPixelPosition(179, 50);
			PutChar(':');
			SetTextPixelPosition(190, 50);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.NetWeight.Line, 2, true).c_str());
			SetTextPixelPosition(208, 50);
			PutChar('-');
			SetTextPixelPosition(217, 50);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.NetWeight.Column, 2, true).c_str());

			SetFont(Symbol_16);
			SetTextPixelPosition(234, 47);
			PutChar(0x01);
		}
		else if(IndexPage == 1)
		{
			SetTextPixelPosition(5, 14);
			PutsString("1-Date_F");
			SetTextPixelPosition(59, 14);
			PutChar(':');
			SetTextPixelPosition(70, 14);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.SecondWeightDate.Line, 2, true).c_str());
			SetTextPixelPosition(88, 14);
			PutChar('-');
			SetTextPixelPosition(97, 14);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.SecondWeightDate.Column, 2, true).c_str());

			SetTextPixelPosition(5, 26);
			PutsString("2-Date_E");
			SetTextPixelPosition(59, 26);
			PutChar(':');
			SetTextPixelPosition(70, 26);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.FirstWeightDate.Line, 2, true).c_str());
			SetTextPixelPosition(88, 26);
			PutChar('-');
			SetTextPixelPosition(97, 26);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.FirstWeightDate.Column, 2, true).c_str());

			SetTextPixelPosition(5, 38);
			PutsString("3-Custo");
			SetTextPixelPosition(59, 38);
			PutChar(':');
			SetTextPixelPosition(70, 38);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.Customer.Line, 2, true).c_str());
			SetTextPixelPosition(88, 38);
			PutChar('-');
			SetTextPixelPosition(97, 38);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.Customer.Column, 2, true).c_str());

			SetTextPixelPosition(5, 50);
			PutsString("4-W_Cos");
			SetTextPixelPosition(59, 50);
			PutChar(':');
			SetTextPixelPosition(70, 50);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.WeighingCost.Line, 2, true).c_str());
			SetTextPixelPosition(88, 50);
			PutChar('-');
			SetTextPixelPosition(97, 50);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.WeighingCost.Column, 2, true).c_str());

			SetTextPixelPosition(125, 14);
			PutsString("5-T_Truc");
			SetTextPixelPosition(179, 14);
			PutChar(':');
			SetTextPixelPosition(190, 14);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.TitleNumberPlates.Line, 2, true).c_str());
			SetTextPixelPosition(208, 14);
			PutChar('-');
			SetTextPixelPosition(217, 14);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.TitleNumberPlates.Column, 2, true).c_str());

			SetTextPixelPosition(125, 26);
			PutsString("6-T_Prod");
			SetTextPixelPosition(179, 26);
			PutChar(':');
			SetTextPixelPosition(190, 26);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.TitleProduct.Line, 2, true).c_str());
			SetTextPixelPosition(208, 26);
			PutChar('-');
			SetTextPixelPosition(217, 26);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.TitleProduct.Column, 2, true).c_str());

			SetTextPixelPosition(125, 38);
			PutsString("7-T_Date");
			SetTextPixelPosition(179, 38);
			PutChar(':');
			SetTextPixelPosition(190, 38);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.TitleDate.Line, 2, true).c_str());
			SetTextPixelPosition(208, 38);
			PutChar('-');
			SetTextPixelPosition(217, 38);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.TitleDate.Column, 2, true).c_str());

			SetTextPixelPosition(125, 50);
			PutsString("8-T_Time");
			SetTextPixelPosition(179, 50);
			PutChar(':');
			SetTextPixelPosition(190, 50);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.TitleTime.Line, 2, true).c_str());
			SetTextPixelPosition(208, 50);
			PutChar('-');
			SetTextPixelPosition(217, 50);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.TitleTime.Column, 2, true).c_str());

			SetFont(Symbol_16);
			SetTextPixelPosition(234, 14);
			PutChar(0x00);
			SetTextPixelPosition(234, 47);
			PutChar(0x01);
		}
		else if(IndexPage == 2)
		{
			SetTextPixelPosition(5, 14);
			PutsString("1-T_Weig");
			SetTextPixelPosition(59, 14);
			PutChar(':');
			SetTextPixelPosition(70, 14);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.TitleWeight.Line, 2, true).c_str());
			SetTextPixelPosition(88, 14);
			PutChar('-');
			SetTextPixelPosition(97, 14);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.TitleWeight.Column, 2, true).c_str());

			SetTextPixelPosition(5, 26);
			PutsString("2-T_Tick");
			SetTextPixelPosition(59, 26);
			PutChar(':');
			SetTextPixelPosition(70, 26);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.TitleTicketSerialNumber.Line, 2, true).c_str());
			SetTextPixelPosition(88, 26);
			PutChar('-');
			SetTextPixelPosition(97, 26);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.TitleTicketSerialNumber.Column, 2, true).c_str());

			SetTextPixelPosition(5, 38);
			PutsString("3-T_Cust");
			SetTextPixelPosition(59, 38);
			PutChar(':');
			SetTextPixelPosition(70, 38);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.TitleCustomer.Line, 2, true).c_str());
			SetTextPixelPosition(88, 38);
			PutChar('-');
			SetTextPixelPosition(97, 38);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.TitleCustomer.Column, 2, true).c_str());

			SetTextPixelPosition(5, 50);
			PutsString("4-T_Cost");
			SetTextPixelPosition(59, 50);
			PutChar(':');
			SetTextPixelPosition(70, 50);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.TitleWeighingCost.Line, 2, true).c_str());
			SetTextPixelPosition(88, 50);
			PutChar('-');
			SetTextPixelPosition(97, 50);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.TitleWeighingCost.Column, 2, true).c_str());

			SetTextPixelPosition(125, 14);
			PutsString("5-Price");
			SetTextPixelPosition(179, 14);
			PutChar(':');
			SetTextPixelPosition(190, 14);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.ProductPrice.Line, 2, true).c_str());
			SetTextPixelPosition(208, 14);
			PutChar('-');
			SetTextPixelPosition(217, 14);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.ProductPrice.Column, 2, true).c_str());

			SetFont(Symbol_16);
			SetTextPixelPosition(234, 14);
			PutChar(0x00);
		}

//----------------------------------------------------------------------------------------------------------

		GLCD_Screen_Update();

//----------------------------------------------------------------------------------------------------------

	   	Button = Panel_ReadPanel();

		if(Button == KEYPAD_DOWN)
		{
			if(IndexPage < 2)
				IndexPage++;
		}
		else if(Button == KEYPAD_UP)
		{
			if(IndexPage > 0)
				IndexPage--;
		}
		else if((Button == KEYPAD_ENTER) || (Button == KEYPAD_ESC))
		{
			ShowSaveParameters();
			SaveSystemParameters(SystemParameters);
			return;
		}
		else if(Button == KEYPAD_01)
			ParameterNumber = (IndexPage * 8) + 1;
		else if(Button == KEYPAD_02)
			ParameterNumber = (IndexPage * 8) + 2;
		else if(Button == KEYPAD_03)
			ParameterNumber = (IndexPage * 8) + 3;
		else if(Button == KEYPAD_04)
			ParameterNumber = (IndexPage * 8) + 4;
		else if(Button == KEYPAD_05)
			ParameterNumber = (IndexPage * 8) + 5;
		else if(Button == KEYPAD_06)
			ParameterNumber = (IndexPage * 8) + 6;
		else if(Button == KEYPAD_07)
			ParameterNumber = (IndexPage * 8) + 7;
		else if(Button == KEYPAD_08)
			ParameterNumber = (IndexPage * 8) + 8;

//----------------------------------------------------------------------------------------------------------

		if(ParameterNumber == 1)
		{
			GLCD_FillRectangle(67, 14, 84, 24, BLACK);

			SetFontLetter(ENGLISH_LETTER);
			SetFont(Font_Pe_12X11);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(70, 14);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.NumberPlates.Line, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.NumberPlates.Line = atoi(Str.c_str());
			else if(LastPressButton == KEYPAD_ESC)
			{
				ParameterNumber = 0;
				continue;
			}

			GLCD_FillRectangle(67, 14, 84, 24, WHITE);
			SetFontColor(BLACK);
			SetBackgroundColor(WHITE);
			SetTextPixelPosition(70, 14);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.NumberPlates.Line, 2, true).c_str());

			GLCD_FillRectangle(94, 14, 111, 24, BLACK);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(97, 14);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.NumberPlates.Column, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.NumberPlates.Column = atoi(Str.c_str());

			ParameterNumber = 0;
		}
		else if(ParameterNumber == 2)
		{
			GLCD_FillRectangle(67, 26, 84, 36, BLACK);

			SetFontLetter(ENGLISH_LETTER);
			SetFont(Font_Pe_12X11);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(70, 26);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.Product.Line, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.Product.Line = atoi(Str.c_str());
			else if(LastPressButton == KEYPAD_ESC)
			{
				ParameterNumber = 0;
				continue;
			}

			GLCD_FillRectangle(67, 26, 84, 36, WHITE);
			SetFontColor(BLACK);
			SetBackgroundColor(WHITE);
			SetTextPixelPosition(70, 26);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.Product.Line, 2, true).c_str());

			GLCD_FillRectangle(94, 26, 111, 36, BLACK);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(97, 26);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.Product.Column, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.Product.Column = atoi(Str.c_str());

			ParameterNumber = 0;
		}
		else if(ParameterNumber == 3)
		{
			GLCD_FillRectangle(67, 38, 84, 48, BLACK);

			SetFontLetter(ENGLISH_LETTER);
			SetFont(Font_Pe_12X11);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(70, 38);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.TicketSerialNumber.Line, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.TicketSerialNumber.Line = atoi(Str.c_str());
			else if(LastPressButton == KEYPAD_ESC)
			{
				ParameterNumber = 0;
				continue;
			}

			GLCD_FillRectangle(67, 38, 84, 48, WHITE);
			SetFontColor(BLACK);
			SetBackgroundColor(WHITE);
			SetTextPixelPosition(70, 38);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.TicketSerialNumber.Line, 2, true).c_str());

			GLCD_FillRectangle(94, 38, 111, 48, BLACK);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(97, 38);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.TicketSerialNumber.Column, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.TicketSerialNumber.Column = atoi(Str.c_str());

			ParameterNumber = 0;
		}
		else if(ParameterNumber == 4)
		{
			GLCD_FillRectangle(67, 50, 84, 60, BLACK);

			SetFontLetter(ENGLISH_LETTER);
			SetFont(Font_Pe_12X11);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(70, 50);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.SecondWeightTime.Line, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.SecondWeightTime.Line = atoi(Str.c_str());
			else if(LastPressButton == KEYPAD_ESC)
			{
				ParameterNumber = 0;
				continue;
			}

			GLCD_FillRectangle(67, 50, 84, 60, WHITE);
			SetFontColor(BLACK);
			SetBackgroundColor(WHITE);
			SetTextPixelPosition(70, 50);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.SecondWeightTime.Line, 2, true).c_str());

			GLCD_FillRectangle(94, 50, 111, 60, BLACK);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(97, 50);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.SecondWeightTime.Column, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.SecondWeightTime.Column = atoi(Str.c_str());

			ParameterNumber = 0;
		}
		else if(ParameterNumber == 5)
		{
			GLCD_FillRectangle(187, 14, 204, 24, BLACK);

			SetFontLetter(ENGLISH_LETTER);
			SetFont(Font_Pe_12X11);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(190, 14);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.FirstWeightTime.Line, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.FirstWeightTime.Line = atoi(Str.c_str());
			else if(LastPressButton == KEYPAD_ESC)
			{
				ParameterNumber = 0;
				continue;
			}

			GLCD_FillRectangle(187, 14, 204, 24, WHITE);
			SetFontColor(BLACK);
			SetBackgroundColor(WHITE);
			SetTextPixelPosition(190, 14);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.FirstWeightTime.Line, 2, true).c_str());

			GLCD_FillRectangle(214, 14, 231, 24, BLACK);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(217, 14);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.FirstWeightTime.Column, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.FirstWeightTime.Column = atoi(Str.c_str());

			ParameterNumber = 0;
		}
		else if(ParameterNumber == 6)
		{
			GLCD_FillRectangle(187, 26, 204, 36, BLACK);

			SetFontLetter(ENGLISH_LETTER);
			SetFont(Font_Pe_12X11);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(190, 26);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.SecondWeight.Line, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.SecondWeight.Line = atoi(Str.c_str());
			else if(LastPressButton == KEYPAD_ESC)
			{
				ParameterNumber = 0;
				continue;
			}

			GLCD_FillRectangle(187, 26, 204, 36, WHITE);
			SetFontColor(BLACK);
			SetBackgroundColor(WHITE);
			SetTextPixelPosition(190, 26);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.SecondWeight.Line, 2, true).c_str());

			GLCD_FillRectangle(214, 26, 231, 36, BLACK);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(217, 26);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.SecondWeight.Column, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.SecondWeight.Column = atoi(Str.c_str());

			ParameterNumber = 0;
		}
		else if(ParameterNumber == 7)
		{
			GLCD_FillRectangle(187, 38, 204, 48, BLACK);

			SetFontLetter(ENGLISH_LETTER);
			SetFont(Font_Pe_12X11);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(190, 38);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.FirstWeight.Line, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.FirstWeight.Line = atoi(Str.c_str());
			else if(LastPressButton == KEYPAD_ESC)
			{
				ParameterNumber = 0;
				continue;
			}

			GLCD_FillRectangle(187, 38, 204, 48, WHITE);
			SetFontColor(BLACK);
			SetBackgroundColor(WHITE);
			SetTextPixelPosition(190, 38);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.FirstWeight.Line, 2, true).c_str());

			GLCD_FillRectangle(214, 38, 231, 48, BLACK);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(217, 38);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.FirstWeight.Column, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.FirstWeight.Column = atoi(Str.c_str());

			ParameterNumber = 0;
		}
		else if(ParameterNumber == 8)
		{
			GLCD_FillRectangle(187, 50, 204, 60, BLACK);

			SetFontLetter(ENGLISH_LETTER);
			SetFont(Font_Pe_12X11);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(190, 50);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.NetWeight.Line, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.NetWeight.Line = atoi(Str.c_str());
			else if(LastPressButton == KEYPAD_ESC)
			{
				ParameterNumber = 0;
				continue;
			}

			GLCD_FillRectangle(187, 50, 204, 60, WHITE);
			SetFontColor(BLACK);
			SetBackgroundColor(WHITE);
			SetTextPixelPosition(190, 50);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.NetWeight.Line, 2, true).c_str());

			GLCD_FillRectangle(214, 50, 231, 60, BLACK);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(217, 50);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.NetWeight.Column, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.NetWeight.Column = atoi(Str.c_str());

			ParameterNumber = 0;
		}
		else if(ParameterNumber == 9)
		{
			GLCD_FillRectangle(67, 14, 84, 24, BLACK);

			SetFontLetter(ENGLISH_LETTER);
			SetFont(Font_Pe_12X11);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(70, 14);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.SecondWeightDate.Line, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.SecondWeightDate.Line = atoi(Str.c_str());
			else if(LastPressButton == KEYPAD_ESC)
			{
				ParameterNumber = 0;
				continue;
			}

			GLCD_FillRectangle(67, 14, 84, 24, WHITE);
			SetFontColor(BLACK);
			SetBackgroundColor(WHITE);
			SetTextPixelPosition(70, 14);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.SecondWeightDate.Line, 2, true).c_str());

			GLCD_FillRectangle(94, 14, 111, 24, BLACK);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(97, 14);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.SecondWeightDate.Column, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.SecondWeightDate.Column = atoi(Str.c_str());

			ParameterNumber = 0;
		}
		else if(ParameterNumber == 10)
		{
			GLCD_FillRectangle(67, 26, 84, 36, BLACK);

			SetFontLetter(ENGLISH_LETTER);
			SetFont(Font_Pe_12X11);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(70, 26);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.FirstWeightDate.Line, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.FirstWeightDate.Line = atoi(Str.c_str());
			else if(LastPressButton == KEYPAD_ESC)
			{
				ParameterNumber = 0;
				continue;
			}

			GLCD_FillRectangle(67, 26, 84, 36, WHITE);
			SetFontColor(BLACK);
			SetBackgroundColor(WHITE);
			SetTextPixelPosition(70, 26);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.FirstWeightDate.Line, 2, true).c_str());

			GLCD_FillRectangle(94, 26, 111, 36, BLACK);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(97, 26);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.FirstWeightDate.Column, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.FirstWeightDate.Column = atoi(Str.c_str());

			ParameterNumber = 0;
		}
		else if(ParameterNumber == 11)
		{
			GLCD_FillRectangle(67, 38, 84, 48, BLACK);

			SetFontLetter(ENGLISH_LETTER);
			SetFont(Font_Pe_12X11);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(70, 38);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.Customer.Line, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.Customer.Line = atoi(Str.c_str());
			else if(LastPressButton == KEYPAD_ESC)
			{
				ParameterNumber = 0;
				continue;
			}

			GLCD_FillRectangle(67, 38, 84, 48, WHITE);
			SetFontColor(BLACK);
			SetBackgroundColor(WHITE);
			SetTextPixelPosition(70, 38);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.Customer.Line, 2, true).c_str());

			GLCD_FillRectangle(94, 38, 111, 48, BLACK);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(97, 38);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.Customer.Column, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.Customer.Column = atoi(Str.c_str());

			ParameterNumber = 0;
		}
		else if(ParameterNumber == 12)
		{
			GLCD_FillRectangle(67, 50, 84, 60, BLACK);

			SetFontLetter(ENGLISH_LETTER);
			SetFont(Font_Pe_12X11);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(70, 50);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.WeighingCost.Line, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.WeighingCost.Line = atoi(Str.c_str());
			else if(LastPressButton == KEYPAD_ESC)
			{
				ParameterNumber = 0;
				continue;
			}

			GLCD_FillRectangle(67, 50, 84, 60, WHITE);
			SetFontColor(BLACK);
			SetBackgroundColor(WHITE);
			SetTextPixelPosition(70, 50);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.WeighingCost.Line, 2, true).c_str());

			GLCD_FillRectangle(94, 50, 111, 60, BLACK);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(97, 50);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.WeighingCost.Column, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.WeighingCost.Column = atoi(Str.c_str());

			ParameterNumber = 0;
		}
		else if(ParameterNumber == 13)
		{
			GLCD_FillRectangle(187, 14, 204, 24, BLACK);

			SetFontLetter(ENGLISH_LETTER);
			SetFont(Font_Pe_12X11);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(190, 14);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.TitleNumberPlates.Line, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.TitleNumberPlates.Line = atoi(Str.c_str());
			else if(LastPressButton == KEYPAD_ESC)
			{
				ParameterNumber = 0;
				continue;
			}

			GLCD_FillRectangle(187, 14, 204, 24, WHITE);
			SetFontColor(BLACK);
			SetBackgroundColor(WHITE);
			SetTextPixelPosition(190, 14);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.TitleNumberPlates.Line, 2, true).c_str());

			GLCD_FillRectangle(214, 14, 231, 24, BLACK);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(217, 14);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.TitleNumberPlates.Column, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.TitleNumberPlates.Column = atoi(Str.c_str());

			ParameterNumber = 0;
		}
		else if(ParameterNumber == 14)
		{
			GLCD_FillRectangle(187, 26, 204, 36, BLACK);

			SetFontLetter(ENGLISH_LETTER);
			SetFont(Font_Pe_12X11);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(190, 26);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.TitleProduct.Line, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.TitleProduct.Line = atoi(Str.c_str());
			else if(LastPressButton == KEYPAD_ESC)
			{
				ParameterNumber = 0;
				continue;
			}

			GLCD_FillRectangle(187, 26, 204, 36, WHITE);
			SetFontColor(BLACK);
			SetBackgroundColor(WHITE);
			SetTextPixelPosition(190, 26);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.TitleProduct.Line, 2, true).c_str());

			GLCD_FillRectangle(214, 26, 231, 36, BLACK);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(217, 26);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.TitleProduct.Column, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.TitleProduct.Column = atoi(Str.c_str());

			ParameterNumber = 0;
		}
		else if(ParameterNumber == 15)
		{
			GLCD_FillRectangle(187, 38, 204, 48, BLACK);

			SetFontLetter(ENGLISH_LETTER);
			SetFont(Font_Pe_12X11);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(190, 38);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.TitleDate.Line, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.TitleDate.Line = atoi(Str.c_str());
			else if(LastPressButton == KEYPAD_ESC)
			{
				ParameterNumber = 0;
				continue;
			}

			GLCD_FillRectangle(187, 38, 204, 48, WHITE);
			SetFontColor(BLACK);
			SetBackgroundColor(WHITE);
			SetTextPixelPosition(190, 38);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.TitleDate.Line, 2, true).c_str());

			GLCD_FillRectangle(214, 38, 231, 48, BLACK);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(217, 38);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.TitleDate.Column, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.TitleDate.Column = atoi(Str.c_str());

			ParameterNumber = 0;
		}
		else if(ParameterNumber == 16)
		{
			GLCD_FillRectangle(187, 50, 204, 60, BLACK);

			SetFontLetter(ENGLISH_LETTER);
			SetFont(Font_Pe_12X11);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(190, 50);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.TitleTime.Line, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.TitleTime.Line = atoi(Str.c_str());
			else if(LastPressButton == KEYPAD_ESC)
			{
				ParameterNumber = 0;
				continue;
			}

			GLCD_FillRectangle(187, 50, 204, 60, WHITE);
			SetFontColor(BLACK);
			SetBackgroundColor(WHITE);
			SetTextPixelPosition(190, 50);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.TitleTime.Line, 2, true).c_str());

			GLCD_FillRectangle(214, 50, 231, 60, BLACK);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(217, 50);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.TitleTime.Column, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.TitleTime.Column = atoi(Str.c_str());

			ParameterNumber = 0;
		}
		else if(ParameterNumber == 17)
		{
			GLCD_FillRectangle(67, 14, 84, 24, BLACK);

			SetFontLetter(ENGLISH_LETTER);
			SetFont(Font_Pe_12X11);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(70, 14);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.TitleWeight.Line, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.TitleWeight.Line = atoi(Str.c_str());
			else if(LastPressButton == KEYPAD_ESC)
			{
				ParameterNumber = 0;
				continue;
			}

			GLCD_FillRectangle(67, 14, 84, 24, WHITE);
			SetFontColor(BLACK);
			SetBackgroundColor(WHITE);
			SetTextPixelPosition(70, 14);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.TitleWeight.Line, 2, true).c_str());

			GLCD_FillRectangle(94, 14, 111, 24, BLACK);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(97, 14);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.TitleWeight.Column, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.TitleWeight.Column = atoi(Str.c_str());

			ParameterNumber = 0;
		}
		else if(ParameterNumber == 18)
		{
			GLCD_FillRectangle(67, 26, 84, 36, BLACK);

			SetFontLetter(ENGLISH_LETTER);
			SetFont(Font_Pe_12X11);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(70, 26);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.TitleTicketSerialNumber.Line, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.TitleTicketSerialNumber.Line = atoi(Str.c_str());
			else if(LastPressButton == KEYPAD_ESC)
			{
				ParameterNumber = 0;
				continue;
			}

			GLCD_FillRectangle(67, 26, 84, 36, WHITE);
			SetFontColor(BLACK);
			SetBackgroundColor(WHITE);
			SetTextPixelPosition(70, 26);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.TitleTicketSerialNumber.Line, 2, true).c_str());

			GLCD_FillRectangle(94, 26, 111, 36, BLACK);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(97, 26);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.TitleTicketSerialNumber.Column, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.TitleTicketSerialNumber.Column = atoi(Str.c_str());

			ParameterNumber = 0;
		}
		else if(ParameterNumber == 19)
		{
			GLCD_FillRectangle(67, 38, 84, 48, BLACK);

			SetFontLetter(ENGLISH_LETTER);
			SetFont(Font_Pe_12X11);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(70, 38);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.TitleCustomer.Line, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.TitleCustomer.Line = atoi(Str.c_str());
			else if(LastPressButton == KEYPAD_ESC)
			{
				ParameterNumber = 0;
				continue;
			}

			GLCD_FillRectangle(67, 38, 84, 48, WHITE);
			SetFontColor(BLACK);
			SetBackgroundColor(WHITE);
			SetTextPixelPosition(70, 38);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.TitleCustomer.Line, 2, true).c_str());

			GLCD_FillRectangle(94, 38, 111, 48, BLACK);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(97, 38);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.TitleCustomer.Column, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.TitleCustomer.Column = atoi(Str.c_str());

			ParameterNumber = 0;
		}
		else if(ParameterNumber == 20)
		{
			GLCD_FillRectangle(67, 50, 84, 60, BLACK);

			SetFontLetter(ENGLISH_LETTER);
			SetFont(Font_Pe_12X11);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(70, 50);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.TitleWeighingCost.Line, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.TitleWeighingCost.Line = atoi(Str.c_str());
			else if(LastPressButton == KEYPAD_ESC)
			{
				ParameterNumber = 0;
				continue;
			}

			GLCD_FillRectangle(67, 50, 84, 60, WHITE);
			SetFontColor(BLACK);
			SetBackgroundColor(WHITE);
			SetTextPixelPosition(70, 50);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.TitleWeighingCost.Line, 2, true).c_str());

			GLCD_FillRectangle(94, 50, 111, 60, BLACK);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(97, 50);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.TitleWeighingCost.Column, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.TitleWeighingCost.Column = atoi(Str.c_str());

			ParameterNumber = 0;
		}
		else if(ParameterNumber == 21)
		{
			GLCD_FillRectangle(187, 14, 204, 24, BLACK);

			SetFontLetter(ENGLISH_LETTER);
			SetFont(Font_Pe_12X11);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(190, 14);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.ProductPrice.Line, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.ProductPrice.Line = atoi(Str.c_str());
			else if(LastPressButton == KEYPAD_ESC)
			{
				ParameterNumber = 0;
				continue;
			}

			GLCD_FillRectangle(187, 14, 204, 24, WHITE);
			SetFontColor(BLACK);
			SetBackgroundColor(WHITE);
			SetTextPixelPosition(190, 14);
			PutsString((char *)IntegerToString(SystemParameters.PrintFormatParameters.ProductPrice.Line, 2, true).c_str());

			GLCD_FillRectangle(214, 14, 231, 24, BLACK);
			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(217, 14);
			Str = GetStringInteger(&LastPressButton, IntegerToString(SystemParameters.PrintFormatParameters.ProductPrice.Column, 2));

			if(LastPressButton == KEYPAD_ENTER)
				SystemParameters.PrintFormatParameters.ProductPrice.Column = atoi(Str.c_str());

			ParameterNumber = 0;
		}
		else
			ParameterNumber = 0;
	}
}

//**********************************************************************************************************

void SystemFunction_DefinitionProductFactoryInformation(void)
{
	PanelKeypadButton_TypeDef Button;

    while(1)
	{
		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_9X8);
		SetTextPixelPosition(57, 2);
		PutsString("تعریف مشخصات واحد تولیدی");

//----------------------------------------------------------------------------------------------------------

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);

		SetFont(Font_Pe_12X11);
		SetTextPixelPosition(5, 25);
		PutsString("1-تعریف نام واحد تولیدی");
		SetTextPixelPosition(5, 40);
		PutsString("2-تعریف آدرس واحد تولیدی");

		GLCD_Screen_Update();

//----------------------------------------------------------------------------------------------------------

	   	Button = Panel_ReadPanel();

		if(Button == KEYPAD_01)
			SystemFunction_DefinitionNameProductFactory();
		else if(Button == KEYPAD_02)
			SystemFunction_DefinitionAddressProductFactory();
		else if(Button == KEYPAD_ESC)
			return;
	}
}

//**********************************************************************************************************

void SystemFunction_DefinitionNameProductFactory(void)
{
  	PanelKeypadButton_TypeDef Button;
	PanelKeypadButton_TypeDef LastPressButton = NO_PUSH;

    while(1)
	{
		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_9X8);
		SetTextPixelPosition(68, 2);
		PutsString("تعریف نام واحد تولیدی");

//----------------------------------------------------------------------------------------------------------

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);

		SetFont(Font_Pe_12X11);
		SetTextPixelPosition(5, 25);
		PutsString("1-سطر اول : ");
		PutsString(SystemParameters.PrintFormatParameters.Title_01[0]);
		SetTextPixelPosition(5, 40);
		PutsString("2-سطر دوم : ");
		PutsString(SystemParameters.PrintFormatParameters.Title_01[1]);

		GLCD_Screen_Update();

//----------------------------------------------------------------------------------------------------------

	   	Button = Panel_ReadPanel();

		if(Button == KEYPAD_01)
		{
			GLCD_FillRectangle(5, 23, 178, 37, BLACK);

			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(62, 25);
			strcpy(SystemParameters.PrintFormatParameters.Title_01[0],
				   (char *)GetStringCharacter(&LastPressButton, SystemParameters.PrintFormatParameters.Title_01[0], MAX_TITLE).c_str());
		}
		else if(Button == KEYPAD_02)
		{
			GLCD_FillRectangle(5, 38, 175, 52, BLACK);

			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(65, 40);
			strcpy(SystemParameters.PrintFormatParameters.Title_01[1],
				   (char *)GetStringCharacter(&LastPressButton, SystemParameters.PrintFormatParameters.Title_01[1], MAX_TITLE).c_str());
		}
		else if((Button == KEYPAD_ENTER) || (Button == KEYPAD_ESC))
		{
			ShowSaveParameters();
			SaveSystemParameters(SystemParameters);
			return;
		}
	}
}

//**********************************************************************************************************

void SystemFunction_DefinitionAddressProductFactory(void)
{
  	PanelKeypadButton_TypeDef Button;
	PanelKeypadButton_TypeDef LastPressButton = NO_PUSH;

    while(1)
	{
		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_9X8);
		SetTextPixelPosition(63, 2);
		PutsString("تعریف آدرس واحد تولیدی");

//----------------------------------------------------------------------------------------------------------

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);

		SetFont(Font_Pe_12X11);
		SetTextPixelPosition(5, 25);
		PutsString("1-سطر اول : ");
		PutsString(SystemParameters.PrintFormatParameters.Title_02[0]);
		SetTextPixelPosition(5, 40);
		PutsString("2-سطر دوم : ");
		PutsString(SystemParameters.PrintFormatParameters.Title_02[1]);

		GLCD_Screen_Update();

//----------------------------------------------------------------------------------------------------------

	   	Button = Panel_ReadPanel();

		if(Button == KEYPAD_01)
		{
			GLCD_FillRectangle(5, 23, 178, 37, BLACK);

			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(62, 25);
			strcpy(SystemParameters.PrintFormatParameters.Title_02[0],
				   (char *)GetStringCharacter(&LastPressButton, SystemParameters.PrintFormatParameters.Title_02[0], MAX_TITLE).c_str());
		}
		else if(Button == KEYPAD_02)
		{
			GLCD_FillRectangle(5, 38, 175, 52, BLACK);

			SetFontColor(WHITE);
			SetBackgroundColor(BLACK);
			SetTextPixelPosition(65, 40);
			strcpy(SystemParameters.PrintFormatParameters.Title_02[1],
				   (char *)GetStringCharacter(&LastPressButton, SystemParameters.PrintFormatParameters.Title_02[1], MAX_TITLE).c_str());
		}
		else if((Button == KEYPAD_ENTER) || (Button == KEYPAD_ESC))
		{
			ShowSaveParameters();
			SaveSystemParameters(SystemParameters);
			return;
		}
	}
}

//**********************************************************************************************************

void SystemFunction_PrintWeight01Function(WeightScale_TypeDef WeightScale)
{
	PanelKeypadButton_TypeDef Button;
	PanelKeypadButton_TypeDef LastPressButton = NO_PUSH;
	WeighingInformation_TypeDef WeighingInformation = {0};
	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef sDate = {0};
	ShamsiDate_TypeDef ShamsiDate;
	uint8_t IndexPage = 0;
	string Str = "";
	bool IsAvailable = false;
	uint32_t TicketSerialNumberAvailable = 0;
	uint32_t LastTicketSerialNumber = 0;
	uint32_t EndTicketSerialNumber = 0;

	while(1)
	{
		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_9X8);
		SetTextPixelPosition(75, 2);
		PutsString("توزین کامیون خالی");

//----------------------------------------------------------------------------------------------------------

		GLCD_FillRectangle(38, 14, 202, 58, BLACK);
		GLCD_FillRectangle(40, 16, 200, 56, WHITE);

		SetFontLetter(PERSIAN_LETTER);
		SetFont(Font_Pe_12X11);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetTextPixelPosition(43, 31);
		PutsString("شماره پلاک : ");

		GLCD_FillRectangle(43, 29, 128, 43, BLACK);

		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		SetTextPixelPosition(115, 31);
		Str = GetStringCharacter(&LastPressButton, "", MAX_NUMBER_PLATES).c_str();

		if(LastPressButton == KEYPAD_ENTER)
		{
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
			GLCD_FillRectangle((58 + 0), 43, 181, 50, WHITE);

			GLCD_Screen_Update();
			LastTicketSerialNumber = ReadTicketSerialNumber();

			if(LastTicketSerialNumber > MAX_CHECK_EXIST_NUMBER_TRUCK)
				EndTicketSerialNumber = LastTicketSerialNumber - MAX_CHECK_EXIST_NUMBER_TRUCK;
			else
				EndTicketSerialNumber = 0;

			for(uint32_t i = LastTicketSerialNumber; i > EndTicketSerialNumber; i--)
			{		
			  	WeighingInformation = SystemFunction_ReadWeighingInformation(i);
				
				if((WeighingInformation.TicketSerialNumber == i) 
				&& ((WeighingInformation.TypeWeighingInformation == 1) || (WeighingInformation.TypeWeighingInformation == 2))
				&& (Str == (string)WeighingInformation.NumberTruck))
				{
					TicketSerialNumberAvailable = i;
					IsAvailable = true;
					break;
				}
			}

			if(IsAvailable)
			{
				if(WeighingInformation.TypeWeighingInformation == 1)
				{
					while(1)
					{
						GLCD_FillRectangle(38, 14, 202, 58, BLACK);
						GLCD_FillRectangle(40, 16, 200, 56, WHITE);

						SetFontLetter(PERSIAN_LETTER);
						SetFont(Font_Pe_12X11);
						SetFontColor(BLACK);
						SetBackgroundColor(WHITE);
						LcdFontSpaceX(1);
						LcdFontSpaceY(0);
						SetTextPixelPosition(69, 24);
						PutsString("موجود در حافظه توزین");
						SetTextPixelPosition(73, 37);
						PutsString("آیا ادامه می دهید?");
						GLCD_Screen_Update();

						Button = Panel_ReadPanel();

						if(Button == KEYPAD_ENTER)
						{
						  	IsAvailable = false;
							break;
						}
						else if(Button == KEYPAD_ESC)
							return;
					}
				}
			}
			
			if(!IsAvailable)
			{
				while(1)
				{
					GLCD_Clear();
					GLCD_FillRectangle(0, 0, 239, 12, BLACK);

					SetFontLetter(PERSIAN_LETTER);
					SetFontColor(WHITE);
					SetBackgroundColor(BLACK);
					LcdFontSpaceX(1);
					LcdFontSpaceY(0);
					SetFont(Font_Pe_9X8);
					SetTextPixelPosition(75, 2);
					PutsString("توزین کامیون خالی");

					SetFontLetter(PERSIAN_LETTER);
					SetFontColor(BLACK);
					SetBackgroundColor(WHITE);
					LcdFontSpaceX(1);
					LcdFontSpaceY(0);
					SetFont(Font_Pe_12X11);
					SetTextPixelPosition(5, 14);
					PutsString("1-تریلی");
					SetTextPixelPosition(5, 26);
					PutsString("2-10 چرخ");
					SetTextPixelPosition(5, 38);
					PutsString("3-6 چرخ");
					SetTextPixelPosition(5, 50);
					PutsString("4-خاور");
					SetTextPixelPosition(125, 14);
					PutsString("5-نیسان");
					SetTextPixelPosition(125, 26);
					PutsString("6-وانت");
					SetTextPixelPosition(125, 38);
					PutsString("7-متفرقه");

					Button = Panel_ReadPanel();

					if(Button == KEYPAD_01)
					{
						WeighingInformation.WeighingCost = SystemParameters.WeighingCost.Trolley;
						break;
					}
					else if(Button == KEYPAD_02)
					{
						WeighingInformation.WeighingCost = SystemParameters.WeighingCost.Ten_Wheels;
						break;
					}
					else if(Button == KEYPAD_03)
					{
						WeighingInformation.WeighingCost = SystemParameters.WeighingCost.Six_Wheels;
						break;
					}
					else if(Button == KEYPAD_04)
					{
						WeighingInformation.WeighingCost = SystemParameters.WeighingCost.Khavar;
						break;
					}
					else if(Button == KEYPAD_05)
					{
						WeighingInformation.WeighingCost = SystemParameters.WeighingCost.Nissan;
						break;
					}
					else if(Button == KEYPAD_06)
					{
						WeighingInformation.WeighingCost = SystemParameters.WeighingCost.PickupTruck;
						break;
					}
					else if(Button == KEYPAD_07)
					{
						WeighingInformation.WeighingCost = SystemParameters.WeighingCost.Other;
						break;
					}
					else if(Button == KEYPAD_ENTER)
					{
						WeighingInformation.WeighingCost = 0;
						break;
					}
					else if(Button == KEYPAD_ESC)
						return;

					GLCD_Screen_Update();
				}

				strcpy(WeighingInformation.NumberTruck, (char *)Str.c_str());
				strcpy(WeighingInformation.CustomerParameters.Name, "");
				strcpy(WeighingInformation.ProductParameters.Name, "");
				WeighingInformation.ProductParameters.Price = 0;
				WeighingInformation.SecondWeightDate.Year = 0;
				WeighingInformation.SecondWeightDate.Month = 0;
				WeighingInformation.SecondWeightDate.Date = 0;
				WeighingInformation.SecondWeightTime.Hours = 0;
				WeighingInformation.SecondWeightTime.Minutes = 0;
				WeighingInformation.SecondWeightTime.Seconds = 0;
				WeighingInformation.SecondWeight = 0;
			}

			HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
			HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
			ShamsiDate = MiladiToShamsi(sDate.Month, sDate.Date, (2000 + sDate.Year));

			WeighingInformation.DISPDigits = WeightScale.DISPDigits;
			WeighingInformation.DECPoint  = WeightScale.DECPoint;
			WeighingInformation.LeadZBlank  = WeightScale.LeadZBlank;
			WeighingInformation.TypeFirstWeight = 0;
			WeighingInformation.FirstWeightTime.Hours = sTime.Hours;
			WeighingInformation.FirstWeightTime.Minutes = sTime.Minutes;
			WeighingInformation.FirstWeightTime.Seconds = sTime.Seconds;
			WeighingInformation.FirstWeightDate.Year = ShamsiDate.iYear;
			WeighingInformation.FirstWeightDate.Month = ShamsiDate.iMonth;
			WeighingInformation.FirstWeightDate.Date = ShamsiDate.iDay;
			WeighingInformation.FirstWeight = WeightScale.Normally;

			if((WeighingInformation.SecondWeight != 0) && (WeighingInformation.FirstWeight != 0))
			{
				WeighingInformation.NetWeight = abs(WeighingInformation.SecondWeight - WeighingInformation.FirstWeight);
				WeighingInformation.TypeWeighingInformation = 0;
			}
			else
			{
			  	WeighingInformation.NetWeight = 0;
			  	WeighingInformation.TypeWeighingInformation = 1;	
			}

			WeighingInformation.ProductPrice = WeighingInformation.NetWeight * WeighingInformation.ProductParameters.Price;
			
			while(1)
			{
				GLCD_Clear();
				GLCD_FillRectangle(0, 0, 239, 12, BLACK);

				SetFontLetter(PERSIAN_LETTER);
				SetFontColor(WHITE);
				SetBackgroundColor(BLACK);
				LcdFontSpaceX(1);
				LcdFontSpaceY(0);
				SetFont(Font_Pe_9X8);
				SetTextPixelPosition(75, 2);
				PutsString("توزین کامیون خالی");

//----------------------------------------------------------------------------------------------------------

				if(IndexPage == 0)
				{
					SetFontLetter(PERSIAN_LETTER);
					SetFont(Font_Pe_12X11);
					SetFontColor(BLACK);
					SetBackgroundColor(WHITE);
					LcdFontSpaceX(1);
					LcdFontSpaceY(0);
					SetTextPixelPosition(5, 14);
					PutsString("شماره پلاک : ");
					PutsString(WeighingInformation.NumberTruck);

					if(WeighingInformation.SecondWeight != 0)
					{
						SetFontLetter(PERSIAN_LETTER);
						SetTextPixelPosition(5, 26);
						PutsString("وزن پر");
						SetFontLetter(ENGLISH_LETTER);
						SetTextPixelPosition(70, 26);
						PutsString((char *)IntegerToString(WeighingInformation.SecondWeightDate.Year, 4).c_str());
						PutsString("/");
						PutsString((char *)IntegerToString(WeighingInformation.SecondWeightDate.Month, 2).c_str());
						PutsString("/");
						PutsString((char *)IntegerToString(WeighingInformation.SecondWeightDate.Date, 2).c_str());
						SetTextPixelPosition(150, 26);
						PutsString((char *)IntegerToString(WeighingInformation.SecondWeightTime.Hours, 2).c_str());
						PutsString(":");
						PutsString((char *)IntegerToString(WeighingInformation.SecondWeightTime.Minutes, 2).c_str());
						SetTextPixelPosition(60, 26);
						PutChar(':');
						SetTextPixelPosition(5, 26);
						PutsString((char *)WeightToString(WeighingInformation.SecondWeight,
												  WeighingInformation.DISPDigits,
												  WeighingInformation.DECPoint,
												  WeighingInformation.LeadZBlank).c_str());

						SetFontLetter(PERSIAN_LETTER);
						SetTextPixelPosition(5, 38);
						PutsString("وزن خالی");
						SetFontLetter(ENGLISH_LETTER);
						SetTextPixelPosition(70, 38);
						PutsString((char *)IntegerToString(WeighingInformation.FirstWeightDate.Year, 4).c_str());
						PutsString("/");
						PutsString((char *)IntegerToString(WeighingInformation.FirstWeightDate.Month, 2).c_str());
						PutsString("/");
						PutsString((char *)IntegerToString(WeighingInformation.FirstWeightDate.Date, 2).c_str());
						SetTextPixelPosition(150, 38);
						PutsString((char *)IntegerToString(WeighingInformation.FirstWeightTime.Hours, 2).c_str());
						PutsString(":");
						PutsString((char *)IntegerToString(WeighingInformation.FirstWeightTime.Minutes, 2).c_str());
						SetTextPixelPosition(60, 38);
						PutChar(':');
						SetTextPixelPosition(5, 38);
						PutsString((char *)WeightToString(WeighingInformation.FirstWeight,
												  WeighingInformation.DISPDigits,
												  WeighingInformation.DECPoint,
												  WeighingInformation.LeadZBlank).c_str());

						SetFontLetter(PERSIAN_LETTER);
						SetTextPixelPosition(5, 50);
						PutsString("وزن خالص");
						SetFontLetter(ENGLISH_LETTER);
						SetTextPixelPosition(60, 50);
						PutChar(':');
						SetTextPixelPosition(5, 50);
						PutsString((char *)WeightToString(WeighingInformation.NetWeight,
												  WeighingInformation.DISPDigits,
												  WeighingInformation.DECPoint,
												  WeighingInformation.LeadZBlank).c_str());
					}
					else
					{
						SetFontLetter(PERSIAN_LETTER);
						SetTextPixelPosition(5, 26);
						PutsString("وزن خالی");
						SetFontLetter(ENGLISH_LETTER);
						SetTextPixelPosition(70, 26);
						PutsString((char *)IntegerToString(WeighingInformation.FirstWeightDate.Year, 4).c_str());
						PutsString("/");
						PutsString((char *)IntegerToString(WeighingInformation.FirstWeightDate.Month, 2).c_str());
						PutsString("/");
						PutsString((char *)IntegerToString(WeighingInformation.FirstWeightDate.Date, 2).c_str());
						SetTextPixelPosition(150, 26);
						PutsString((char *)IntegerToString(WeighingInformation.FirstWeightTime.Hours, 2).c_str());
						PutsString(":");
						PutsString((char *)IntegerToString(WeighingInformation.FirstWeightTime.Minutes, 2).c_str());
						SetTextPixelPosition(60, 26);
						PutChar(':');
						SetTextPixelPosition(5, 26);
						PutsString((char *)WeightToString(WeighingInformation.FirstWeight,
												  WeighingInformation.DISPDigits,
												  WeighingInformation.DECPoint,
												  WeighingInformation.LeadZBlank).c_str());
					}

					SetFont(Symbol_16);
					SetTextPixelPosition(234, 47);
					PutChar(0x01);
				}
				else if(IndexPage == 1)
				{
					SetFontLetter(PERSIAN_LETTER);
					SetFont(Font_Pe_12X11);
					SetFontColor(BLACK);
					SetBackgroundColor(WHITE);
					LcdFontSpaceX(1);
					LcdFontSpaceY(0);
					SetTextPixelPosition(5, 14);
					PutsString("نام کالا : ");
					PutsString(WeighingInformation.ProductParameters.Name);

					SetFontLetter(PERSIAN_LETTER);
					SetTextPixelPosition(5, 26);
					PutsString("نام مشتری : ");
					PutsString(WeighingInformation.CustomerParameters.Name);

					SetFontLetter(PERSIAN_LETTER);
					SetTextPixelPosition(5, 38);
					PutsString("حق توزین");
					SetFontLetter(ENGLISH_LETTER);
					SetTextPixelPosition(180, 38);
					PutChar(':');
					SetTextPixelPosition(125, 38);
					PutsString((char *)IntegerToString(WeighingInformation.WeighingCost, 7).c_str());

					SetFontLetter(ENGLISH_LETTER);
					SetTextPixelPosition(5, 38);
					PutsString("Enter->Save");
					SetTextPixelPosition(5, 50);
					PutsString("Print->Print");

					SetFont(Symbol_16);
					SetTextPixelPosition(234, 14);
					PutChar(0x00);
				}

//----------------------------------------------------------------------------------------------------------

				GLCD_Screen_Update();

//----------------------------------------------------------------------------------------------------------

				Button = Panel_ReadPanel();
				if(Button == KEYPAD_DOWN)
				{
					if(IndexPage < 1)
						IndexPage++;
				}
				else if(Button == KEYPAD_UP)
				{
					if(IndexPage > 0)
						IndexPage--;
				}
				else if(Button == KEYPAD_ENTER)
				{
					ShowSaveParameters();

					WeighingInformation.TicketSerialNumber = ReadTicketSerialNumber();
					SystemFunction_IncreaseTicketSerialNumber();
					SystemFunction_SaveWeighingInformation(WeighingInformation, WeighingInformation.TicketSerialNumber);

					if(TicketSerialNumberAvailable != 0)
					{
						WeighingInformation = SystemFunction_ReadWeighingInformation(TicketSerialNumberAvailable);
						WeighingInformation.TypeWeighingInformation = 3;
						SystemFunction_SaveWeighingInformation(WeighingInformation, WeighingInformation.TicketSerialNumber);
					}
					
					return;
				}
				else if(Button == KEYPAD_PRINT)
				{
					WeighingInformation.TicketSerialNumber = ReadTicketSerialNumber();
					SystemFunction_IncreaseTicketSerialNumber();
					SystemFunction_SaveWeighingInformation(WeighingInformation, WeighingInformation.TicketSerialNumber);
					SystemFunction_PrintWeightTicket(WeighingInformation);
				
					if(TicketSerialNumberAvailable != 0)
					{
						WeighingInformation = SystemFunction_ReadWeighingInformation(TicketSerialNumberAvailable);
						WeighingInformation.TypeWeighingInformation = 3;
						SystemFunction_SaveWeighingInformation(WeighingInformation, WeighingInformation.TicketSerialNumber);
					}
					
					return;
				}
				else if(Button == KEYPAD_ESC)
					return;
			}
		}
		else if(LastPressButton == KEYPAD_ESC)
			return;
	}
}

//**********************************************************************************************************

void SystemFunction_SaveWeighingInformation(WeighingInformation_TypeDef WeighingInformation, uint32_t IndexWeighingInformation)
{
	uint32_t Add_Sector = W25qxx_PageToSector(IndexWeighingInformation);
	uint32_t Add_PageToSector = (IndexWeighingInformation - W25qxx_SectorToPage(Add_Sector)) * W25qxx_GetPageSize();
	uint8_t BufferSector[0x1000];
	uint8_t *BufferPrintInformation = (uint8_t *) &WeighingInformation;

	if(IndexWeighingInformation >= W25qxx_GetPageCount())
	{
		ErrorSystemReport(E13_BACKUP_MEMORY);
		return;
	}

	W25qxx_ReadSector(BufferSector, Add_Sector, 0, W25qxx_GetSectorSize());
	W25qxx_EraseSector(Add_Sector);

	for(uint32_t i = 0; i < SIZE_WEIGHING_INFORMATION; i++)
	{
		BufferSector[(Add_PageToSector + i)] = *BufferPrintInformation;
		BufferPrintInformation++;
	}

	W25qxx_WriteSector(BufferSector, Add_Sector, 0, W25qxx_GetSectorSize());
}

//**********************************************************************************************************

WeighingInformation_TypeDef SystemFunction_ReadWeighingInformation(uint32_t IndexWeighingInformation)
{
	WeighingInformation_TypeDef WeighingInformation;

	W25qxx_ReadPage((uint8_t *) &WeighingInformation, IndexWeighingInformation, 0, SIZE_WEIGHING_INFORMATION);

	return WeighingInformation;
}

//**********************************************************************************************************

void SystemFunction_PrintWeight02Function(WeightScale_TypeDef WeightScale)
{
	PanelKeypadButton_TypeDef Button;
	PanelKeypadButton_TypeDef LastPressButton = NO_PUSH;
	WeighingInformation_TypeDef WeighingInformation = {0};
	CertainTrucksParameters_TypeDef CertainTrucksParameters = {0};
	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef sDate = {0};
	ShamsiDate_TypeDef ShamsiDate;
	uint8_t IndexPage = 0;
	uint16_t ProductCode = 0;
	uint16_t CustomerCode = 0;
	string Str = "";
	bool IsAvailable = false;
	uint32_t TicketSerialNumberAvailable = 0;
	uint32_t LastTicketSerialNumber = 0;
	uint32_t EndTicketSerialNumber = 0;

	while(1)
	{
		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_9X8);
		SetTextPixelPosition(82, 2);
		PutsString("توزین کامیون پر");

//----------------------------------------------------------------------------------------------------------

		GLCD_FillRectangle(38, 14, 202, 58, BLACK);
		GLCD_FillRectangle(40, 16, 200, 56, WHITE);

		SetFontLetter(PERSIAN_LETTER);
		SetFont(Font_Pe_12X11);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetTextPixelPosition(43, 31);
		PutsString("شماره پلاک : ");

		GLCD_FillRectangle(43, 29, 128, 43, BLACK);

		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		SetTextPixelPosition(115, 31);
		Str = GetStringCharacter(&LastPressButton, "", MAX_NUMBER_PLATES).c_str();

		if(LastPressButton == KEYPAD_ENTER)
		{
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
			GLCD_FillRectangle((58 + 0), 43, 181, 50, WHITE);

			GLCD_Screen_Update();
			LastTicketSerialNumber = ReadTicketSerialNumber();

			if(LastTicketSerialNumber > MAX_CHECK_EXIST_NUMBER_TRUCK)
				EndTicketSerialNumber = LastTicketSerialNumber - MAX_CHECK_EXIST_NUMBER_TRUCK;
			else
				EndTicketSerialNumber = 0;

			for(uint32_t i = LastTicketSerialNumber; i > EndTicketSerialNumber; i--)
			{		
			  	WeighingInformation = SystemFunction_ReadWeighingInformation(i);
				
				if((WeighingInformation.TicketSerialNumber == i) 
					&& ((WeighingInformation.TypeWeighingInformation == 1) || (WeighingInformation.TypeWeighingInformation == 2))
					&& (Str == (string)WeighingInformation.NumberTruck))
				{
					TicketSerialNumberAvailable = i;
					IsAvailable = true;
					break;
				}
			}

			if(IsAvailable)
			{
				if(WeighingInformation.TypeWeighingInformation == 2)
				{
					while(1)
					{
						GLCD_FillRectangle(38, 14, 202, 58, BLACK);
						GLCD_FillRectangle(40, 16, 200, 56, WHITE);

						SetFontLetter(PERSIAN_LETTER);
						SetFont(Font_Pe_12X11);
						SetFontColor(BLACK);
						SetBackgroundColor(WHITE);
						LcdFontSpaceX(1);
						LcdFontSpaceY(0);
						SetTextPixelPosition(69, 24);
						PutsString("موجود در حافظه توزین");
						SetTextPixelPosition(73, 37);
						PutsString("آیا ادامه می دهید?");
						GLCD_Screen_Update();

						Button = Panel_ReadPanel();

						if(Button == KEYPAD_ENTER)
						{
						  	IsAvailable = false;
							break;
						}
						else if(Button == KEYPAD_ESC)
							return;
					}
				}
			}
			
			if(!IsAvailable)
			{
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
				GLCD_FillRectangle((58 + 62), 43, 181, 50, WHITE);

				GLCD_Screen_Update();

				for(int i = 0; i < MAX_CERTAIN_TRUCKS; i++)
				{
					CertainTrucksParameters = ReadCertainTrucksParameters(i);
					if(Str == (string)CertainTrucksParameters.NumberPlates)
					{
						IsAvailable = true;

						strcpy(WeighingInformation.NumberTruck, CertainTrucksParameters.NumberPlates);
						WeighingInformation.TypeFirstWeight = 1;
						WeighingInformation.FirstWeightTime.Hours = 0;
						WeighingInformation.FirstWeightTime.Minutes = 0;
						WeighingInformation.FirstWeightTime.Seconds = 0;
						WeighingInformation.FirstWeightDate.Year = 0;
						WeighingInformation.FirstWeightDate.Month = 0;
						WeighingInformation.FirstWeightDate.Date = 0;
						
						WeighingInformation.FirstWeight = CertainTrucksParameters.EmptyWeight;

						break;
					}
				}
			}

//----------------------------------------------------------------------------------------------------------

			if(!IsAvailable)
			{
				strcpy(WeighingInformation.NumberTruck, (char *)Str.c_str());
				WeighingInformation.TypeFirstWeight = 0;
				WeighingInformation.FirstWeightTime.Hours = 0;
				WeighingInformation.FirstWeightTime.Minutes = 0;
				WeighingInformation.FirstWeightTime.Seconds = 0;
				WeighingInformation.FirstWeightDate.Year = 0;
				WeighingInformation.FirstWeightDate.Month = 0;
				WeighingInformation.FirstWeightDate.Date = 0;
				WeighingInformation.FirstWeight = 0;
			}

			HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
			HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
			ShamsiDate = MiladiToShamsi(sDate.Month, sDate.Date, (2000 + sDate.Year));

			WeighingInformation.DISPDigits = WeightScale.DISPDigits;
			WeighingInformation.DECPoint  = WeightScale.DECPoint;
			WeighingInformation.LeadZBlank  = WeightScale.LeadZBlank;
			WeighingInformation.SecondWeight = WeightScale.Normally;
			WeighingInformation.SecondWeightTime.Hours = sTime.Hours;
			WeighingInformation.SecondWeightTime.Minutes = sTime.Minutes;
			WeighingInformation.SecondWeightTime.Seconds = sTime.Seconds;
			WeighingInformation.SecondWeightDate.Year = ShamsiDate.iYear;
			WeighingInformation.SecondWeightDate.Month = ShamsiDate.iMonth;
			WeighingInformation.SecondWeightDate.Date = ShamsiDate.iDay;

			if((WeighingInformation.SecondWeight != 0) && (WeighingInformation.FirstWeight != 0))
			{
				WeighingInformation.NetWeight = abs(WeighingInformation.SecondWeight - WeighingInformation.FirstWeight);
				WeighingInformation.TypeWeighingInformation = 0;
			}
			else
			{
				WeighingInformation.NetWeight = 0;
				WeighingInformation.TypeWeighingInformation = 2;
			}

			break;
		}
		else if(LastPressButton == KEYPAD_ESC)
			return;

//----------------------------------------------------------------------------------------------------------

		GLCD_Screen_Update();
	}

//----------------------------------------------------------------------------------------------------------

	while(1)
	{
		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_9X8);
		SetTextPixelPosition(82, 2);
		PutsString("توزین کامیون پر");

		GLCD_FillRectangle(68, 14, 172, 58, BLACK);
		GLCD_FillRectangle(70, 16, 170, 56, WHITE);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_12X11);
		SetTextPixelPosition(75, 31);
		PutsString("کد کالا : ");

		GLCD_FillRectangle(75, 30, 110, 41, BLACK);
		SetFontLetter(ENGLISH_LETTER);
		SetFont(Font_En_Num_8X10);
		LcdFontSpaceX(0);
		LcdFontSpaceY(0);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		SetTextPixelPosition(83, 31);

		Str = GetStringInteger(&LastPressButton, IntegerToString(ProductCode, 3));

		if(LastPressButton == KEYPAD_ENTER)
		{
			if(atol(Str.c_str()) < MAX_PRODUCT_CODE)
			{
				ProductCode = atol(Str.c_str());
				WeighingInformation.ProductParameters = ReadProductCode(ProductCode);
				WeighingInformation.ProductPrice = WeighingInformation.NetWeight * WeighingInformation.ProductParameters.Price;
				break;
			}
			else
			{
				GLCD_FillRectangle(18, 14, 222, 58, BLACK);
				GLCD_FillRectangle(20, 16, 220, 56, WHITE);

				SetFontLetter(PERSIAN_LETTER);
				SetFontColor(BLACK);
				SetBackgroundColor(WHITE);
				LcdFontSpaceX(1);
				LcdFontSpaceY(0);
				SetFont(Font_Pe_12X11);
				SetTextPixelPosition(30, 31);
				PutsString("مقدار ورودی بیشتر از رنج مجاز می باشد");
				GLCD_Screen_Update();
				DELAY_MS(3000);

				continue;
			}
		}
		else if(LastPressButton == KEYPAD_ESC)
			return;

//----------------------------------------------------------------------------------------------------------

		GLCD_Screen_Update();
	}

//----------------------------------------------------------------------------------------------------------

	while(1)
	{
		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_9X8);
		SetTextPixelPosition(82, 2);
		PutsString("توزین کامیون پر");

		GLCD_FillRectangle(68, 14, 172, 58, BLACK);
		GLCD_FillRectangle(70, 16, 170, 56, WHITE);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_12X11);
		SetTextPixelPosition(70, 31);
		PutsString("کد مشتری : ");

		GLCD_FillRectangle(73, 30, 108, 41, BLACK);
		SetFontLetter(ENGLISH_LETTER);
		SetFont(Font_En_Num_8X10);
		LcdFontSpaceX(0);
		LcdFontSpaceY(0);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		SetTextPixelPosition(81, 31);

		Str = GetStringInteger(&LastPressButton, IntegerToString(CustomerCode, 3));

		if(LastPressButton == KEYPAD_ENTER)
		{
			if(atol(Str.c_str()) < MAX_CUSTOMER_CODE)
			{
			 	CustomerCode = atol(Str.c_str());
				WeighingInformation.CustomerParameters = ReadCustomerCode(CustomerCode);
				break;
			}
			else
			{
				GLCD_FillRectangle(18, 14, 222, 58, BLACK);
				GLCD_FillRectangle(20, 16, 220, 56, WHITE);

				SetFontLetter(PERSIAN_LETTER);
				SetFontColor(BLACK);
				SetBackgroundColor(WHITE);
				LcdFontSpaceX(1);
				LcdFontSpaceY(0);
				SetFont(Font_Pe_12X11);
				SetTextPixelPosition(30, 31);
				PutsString("مقدار ورودی بیشتر از رنج مجاز می باشد");
				GLCD_Screen_Update();
				DELAY_MS(3000);

				continue;
			}
		}
		else if(LastPressButton == KEYPAD_ESC)
			return;

//----------------------------------------------------------------------------------------------------------

		GLCD_Screen_Update();
	}

//----------------------------------------------------------------------------------------------------------

	while(1)
	{
		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_9X8);
		SetTextPixelPosition(82, 2);
		PutsString("توزین کامیون پر");

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_12X11);
		SetTextPixelPosition(5, 14);
		PutsString("1-تریلی");
		SetTextPixelPosition(5, 26);
		PutsString("2-10 چرخ");
		SetTextPixelPosition(5, 38);
		PutsString("3-6 چرخ");
		SetTextPixelPosition(5, 50);
		PutsString("4-خاور");
		SetTextPixelPosition(125, 14);
		PutsString("5-نیسان");
		SetTextPixelPosition(125, 26);
		PutsString("6-وانت");
		SetTextPixelPosition(125, 38);
		PutsString("7-متفرقه");

		Button = Panel_ReadPanel();

		if(Button == KEYPAD_01)
		{
			WeighingInformation.WeighingCost = SystemParameters.WeighingCost.Trolley;
			break;
		}
		else if(Button == KEYPAD_02)
		{
			WeighingInformation.WeighingCost = SystemParameters.WeighingCost.Ten_Wheels;
			break;
		}
		else if(Button == KEYPAD_03)
		{
			WeighingInformation.WeighingCost = SystemParameters.WeighingCost.Six_Wheels;
			break;
		}
		else if(Button == KEYPAD_04)
		{
			WeighingInformation.WeighingCost = SystemParameters.WeighingCost.Khavar;
			break;
		}
		else if(Button == KEYPAD_05)
		{
			WeighingInformation.WeighingCost = SystemParameters.WeighingCost.Nissan;
			break;
		}
		else if(Button == KEYPAD_06)
		{
			WeighingInformation.WeighingCost = SystemParameters.WeighingCost.PickupTruck;
			break;
		}
		else if(Button == KEYPAD_07)
		{
			WeighingInformation.WeighingCost = SystemParameters.WeighingCost.Other;
			break;
		}
		else if(Button == KEYPAD_ENTER)
		{
			WeighingInformation.WeighingCost = 0;
			break;
		}
		else if(Button == KEYPAD_ESC)
			return;

//----------------------------------------------------------------------------------------------------------

		GLCD_Screen_Update();
	}

//----------------------------------------------------------------------------------------------------------

	while(1)
	{
		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_9X8);
		SetTextPixelPosition(82, 2);
		PutsString("توزین کامیون پر");

		if(IndexPage == 0)
		{
			SetFontLetter(PERSIAN_LETTER);
			SetFont(Font_Pe_12X11);
			SetFontColor(BLACK);
			SetBackgroundColor(WHITE);
			LcdFontSpaceX(1);
			LcdFontSpaceY(0);
			SetTextPixelPosition(5, 14);
			PutsString("شماره پلاک : ");
			PutsString(WeighingInformation.NumberTruck);

			SetFontLetter(PERSIAN_LETTER);
			SetTextPixelPosition(5, 26);
			PutsString("وزن پر");
			SetFontLetter(ENGLISH_LETTER);
			SetTextPixelPosition(70, 26);
			PutsString((char *)IntegerToString(WeighingInformation.SecondWeightDate.Year, 4).c_str());
			PutsString("/");
			PutsString((char *)IntegerToString(WeighingInformation.SecondWeightDate.Month, 2).c_str());
			PutsString("/");
			PutsString((char *)IntegerToString(WeighingInformation.SecondWeightDate.Date, 2).c_str());
			SetTextPixelPosition(150, 26);
			PutsString((char *)IntegerToString(WeighingInformation.SecondWeightTime.Hours, 2).c_str());
			PutsString(":");
			PutsString((char *)IntegerToString(WeighingInformation.SecondWeightTime.Minutes, 2).c_str());
			SetTextPixelPosition(60, 26);
			PutChar(':');
			SetTextPixelPosition(5, 26);
			PutsString((char *)WeightToString(WeighingInformation.SecondWeight,
									  WeighingInformation.DISPDigits,
									  WeighingInformation.DECPoint,
									  WeighingInformation.LeadZBlank).c_str());

			if(WeighingInformation.FirstWeight != 0)
			{
				SetFontLetter(PERSIAN_LETTER);
				SetTextPixelPosition(5, 38);
				
				if(WeighingInformation.TypeFirstWeight == 1)
					PutsString("وزن خالی کامیون خاص");
				else
				{
					PutsString("وزن خالی");
					SetFontLetter(ENGLISH_LETTER);
					SetTextPixelPosition(70, 38);
					PutsString((char *)IntegerToString(WeighingInformation.FirstWeightDate.Year, 4).c_str());
					PutsString("/");
					PutsString((char *)IntegerToString(WeighingInformation.FirstWeightDate.Month, 2).c_str());
					PutsString("/");
					PutsString((char *)IntegerToString(WeighingInformation.FirstWeightDate.Date, 2).c_str());
					SetTextPixelPosition(150, 38);
					PutsString((char *)IntegerToString(WeighingInformation.FirstWeightTime.Hours, 2).c_str());
					PutsString(":");
					PutsString((char *)IntegerToString(WeighingInformation.FirstWeightTime.Minutes, 2).c_str());
				}
						
				SetFontLetter(ENGLISH_LETTER);
				SetTextPixelPosition(60, 38);
				PutChar(':');
				SetTextPixelPosition(5, 38);
				PutsString((char *)WeightToString(WeighingInformation.FirstWeight,
										  WeighingInformation.DISPDigits,
										  WeighingInformation.DECPoint,
										  WeighingInformation.LeadZBlank).c_str());

				SetFontLetter(PERSIAN_LETTER);
				SetTextPixelPosition(5, 50);
				PutsString("وزن خالص");
				SetFontLetter(ENGLISH_LETTER);
				SetTextPixelPosition(60, 50);
				PutChar(':');
				SetTextPixelPosition(5, 50);
				PutsString((char *)WeightToString(WeighingInformation.NetWeight,
										  WeighingInformation.DISPDigits,
										  WeighingInformation.DECPoint,
										  WeighingInformation.LeadZBlank).c_str());
			}
			
			SetFont(Symbol_16);
			SetTextPixelPosition(234, 47);
			PutChar(0x01);
		}
		else if(IndexPage == 1)
		{
			SetFontLetter(PERSIAN_LETTER);
			SetFont(Font_Pe_12X11);
			SetFontColor(BLACK);
			SetBackgroundColor(WHITE);
			LcdFontSpaceX(1);
			LcdFontSpaceY(0);
			SetTextPixelPosition(5, 14);
			PutsString("نام کالا : ");
			PutsString(WeighingInformation.ProductParameters.Name);

			SetFontLetter(PERSIAN_LETTER);
			SetTextPixelPosition(5, 26);
			PutsString("نام مشتری : ");
			PutsString(WeighingInformation.CustomerParameters.Name);

			SetFontLetter(PERSIAN_LETTER);
			SetTextPixelPosition(5, 38);
			PutsString("حق توزین");
			SetFontLetter(ENGLISH_LETTER);
			SetTextPixelPosition(180, 38);
			PutChar(':');
			SetTextPixelPosition(125, 38);
			PutsString((char *)IntegerToString(WeighingInformation.WeighingCost, 7).c_str());

			SetFontLetter(ENGLISH_LETTER);
			SetTextPixelPosition(5, 38);
			PutsString("Enter->Save");
			SetTextPixelPosition(5, 50);
			PutsString("Print->Print");

			SetFont(Symbol_16);
			SetTextPixelPosition(234, 14);
			PutChar(0x00);
		}

//----------------------------------------------------------------------------------------------------------

		GLCD_Screen_Update();

//----------------------------------------------------------------------------------------------------------

		Button = Panel_ReadPanel();
		if(Button == KEYPAD_DOWN)
		{
			if(IndexPage < 1)
				IndexPage++;
		}
		else if(Button == KEYPAD_UP)
		{
			if(IndexPage > 0)
				IndexPage--;
		}
		else if(Button == KEYPAD_WEIGH_01)
		{
			if(IndexPage == 0)
			{
				SetFontLetter(PERSIAN_LETTER);
				SetFont(Font_Pe_12X11);
				SetFontColor(BLACK);
				SetBackgroundColor(WHITE);
				LcdFontSpaceX(1);
				LcdFontSpaceY(0);
				GLCD_FillRectangle(0, 37, 239, 48, WHITE);
				SetTextPixelPosition(5, 38);
				PutsString("وزن خالی کامیون خاص");
				GLCD_FillRectangle(5, 37, 57, 48, BLACK);
				SetFontLetter(ENGLISH_LETTER);
				SetTextPixelPosition(60, 38);
				PutChar(':');
				SetFont(Font_Pe_12X11);
				LcdFontSpaceX(1);
				LcdFontSpaceY(0);
				SetFontColor(WHITE);
				SetBackgroundColor(BLACK);
				SetTextPixelPosition(9, 38);

				Str = GetStringWeight(&LastPressButton, WeighingInformation.FirstWeight,
									  WeighingInformation.DISPDigits,
									  WeighingInformation.DECPoint);

				if(LastPressButton == KEYPAD_ENTER)
				{
					WeighingInformation.TypeFirstWeight = 1;
					WeighingInformation.FirstWeightTime.Hours = 0;
					WeighingInformation.FirstWeightTime.Minutes = 0;
					WeighingInformation.FirstWeightTime.Seconds = 0;
					WeighingInformation.FirstWeightDate.Year = 0;
					WeighingInformation.FirstWeightDate.Month = 0;
					WeighingInformation.FirstWeightDate.Date = 0;
					
					 WeighingInformation.FirstWeight = atol(Str.c_str());

					 if(WeighingInformation.FirstWeight != 0)
					 {
					 	WeighingInformation.NetWeight = abs(WeighingInformation.SecondWeight - WeighingInformation.FirstWeight);
						WeighingInformation.ProductPrice = WeighingInformation.NetWeight * WeighingInformation.ProductParameters.Price;
						WeighingInformation.TypeWeighingInformation = 0;
					 }	 
				}
			}
		}
		else if(Button == KEYPAD_ENTER)
		{
			ShowSaveParameters();

			WeighingInformation.TicketSerialNumber = ReadTicketSerialNumber();
			SystemFunction_IncreaseTicketSerialNumber();
			SystemFunction_SaveWeighingInformation(WeighingInformation, WeighingInformation.TicketSerialNumber);
			
			if(TicketSerialNumberAvailable != 0)
			{
				WeighingInformation = SystemFunction_ReadWeighingInformation(TicketSerialNumberAvailable);
				WeighingInformation.TypeWeighingInformation = 3;
				SystemFunction_SaveWeighingInformation(WeighingInformation, WeighingInformation.TicketSerialNumber);
			}

			return;
		}
		else if(Button == KEYPAD_PRINT)
		{
			WeighingInformation.TicketSerialNumber = ReadTicketSerialNumber();
			SystemFunction_IncreaseTicketSerialNumber();
			SystemFunction_SaveWeighingInformation(WeighingInformation, WeighingInformation.TicketSerialNumber);
			SystemFunction_PrintWeightTicket(WeighingInformation);

			if(TicketSerialNumberAvailable != 0)
			{
				WeighingInformation = SystemFunction_ReadWeighingInformation(TicketSerialNumberAvailable);
				WeighingInformation.TypeWeighingInformation = 3;
				SystemFunction_SaveWeighingInformation(WeighingInformation, WeighingInformation.TicketSerialNumber);
			}
			
			return;
		}
		else if(Button == KEYPAD_ESC)
			return;
	}
}

//**********************************************************************************************************

void SystemFunction_PrintWeightTicket(WeighingInformation_TypeDef WeighingInformation)
{
	bool SelectPrintFont;

	for(int i = 0; i < SystemParameters.PrintFormatParameters.NumberOfPaperPrint; i++)
	{
		SelectPrintFont = true;

		for(int j = 0; j < SystemParameters.PrintFormatParameters.NumberOfPrint; j++)
		{
			if(SystemParameters.PrintFormatParameters.TypePrinter == 3)
			{
				if(SystemFunction_Thermal_PrinterPrintWeightTicket(WeighingInformation) != OK_PRINTER)
					ErrorSystemReport(E12_PRINTER);
			}
			else
			{
				if(SystemParameters.PrintFormatParameters.PrintType == 0)
				{
					if(SystemFunction_PrintWeightTicketFormatStandard_01(WeighingInformation, SelectPrintFont) != OK_PRINTER)
						ErrorSystemReport(E12_PRINTER);
				}
				else if(SystemParameters.PrintFormatParameters.PrintType == 1)
				{
					if(SystemFunction_PrintWeightTicketFormatStandard_02(WeighingInformation, SelectPrintFont) != OK_PRINTER)
						ErrorSystemReport(E12_PRINTER);
				}
				else if(SystemParameters.PrintFormatParameters.PrintType == 2)
				{
					if(SystemFunction_PrintWeightTicketFormatUserDefined(WeighingInformation, SelectPrintFont) != OK_PRINTER)
						ErrorSystemReport(E12_PRINTER);
				}

				SelectPrintFont = false;
			}
		}

		if(SystemParameters.PrintFormatParameters.TypePrinter == 3)
		{
			Thermal_PrinterEject();
		}
		else
		{
			if(SystemParameters.PrintFormatParameters.EjectSheet)
			{
				if(Printer_SendCommand(PRINTER_EJECT_PAPER_COMMAND) != OK_PRINTER)
					ErrorSystemReport(E12_PRINTER);
			}
		}
	}
}

//**********************************************************************************************************

void SystemFunction_IncreaseTicketSerialNumber(void)
{
	uint32_t TicketSerialNumber = ReadTicketSerialNumber();

	if(TicketSerialNumber < (MAX_TICKET_SERIAL_NUMBER - 1))
		TicketSerialNumber++;
	else
		TicketSerialNumber = 1;

	SaveTicketSerialNumber(TicketSerialNumber);
}

//**********************************************************************************************************

Status_Printer_TypeDef SystemFunction_PrintWeightTicketFormatStandard_01(WeighingInformation_TypeDef WeighingInformation, bool SelectPrintFont)
{
	Status_Printer_TypeDef Status_Printer;
	uint8_t MaxCharacterToLine;

	while(1)
	{
		if(Printer_CheckPaper() == NO_PAPER_PRINTER)
		{
			GLCD_FillRectangle(10, 14, 229, 58, BLACK);
			GLCD_FillRectangle(12, 16, 227, 56, WHITE);

			SetFontLetter(PERSIAN_LETTER);
			SetFontColor(BLACK);
			SetBackgroundColor(WHITE);
			LcdFontSpaceX(1);
			LcdFontSpaceY(0);
			SetFont(Font_Pe_12X11);
			SetTextPixelPosition(110, 27);
			PutsString("خطا");
			SetTextPixelPosition(87, 41);
			PutsString("عدم وجود کاغذ");
			GLCD_Screen_Update();

			if(Panel_ReadPanel() == KEYPAD_ESC)
				return NO_PAPER_PRINTER;
		}
		else
			break;
	}

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
	GLCD_FillRectangle((58 + 62), 43, 181, 50, WHITE);

	GLCD_Screen_Update();

//----------------------------------------------------------------------------------------------------------

	if(SelectPrintFont)
	{
		if(SystemParameters.PrintFormatParameters.PrintFont == 0)
			MaxCharacterToLine = Printer_SelectPrintFont_01();
		else if(SystemParameters.PrintFormatParameters.PrintFont == 1)
			MaxCharacterToLine = Printer_SelectPrintFont_02();
		else if(SystemParameters.PrintFormatParameters.PrintFont == 2)
			MaxCharacterToLine = Printer_SelectPrintFont_03();
		else if(SystemParameters.PrintFormatParameters.PrintFont == 3)
			MaxCharacterToLine = Printer_SelectPrintFont_04();
	}

//----------------------------------------------------------------------------------------------------------

	for(int i = 0; i < SystemParameters.PrintFormatParameters.TopLineEmpty; i++)
	{
		Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

//----------------------------------------------------------------------------------------------------------

	if(SystemParameters.PrintFormatParameters.PrintTitle)
	{
		Status_Printer = Printer_SetHorizontalCursorPositioning(84);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Printer_PrintStringPersian(SystemParameters.PrintFormatParameters.Title_01[0]);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		if(SystemParameters.PrintFormatParameters.PrintFont == 3)
		{
			Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}

		Status_Printer = Printer_SetHorizontalCursorPositioning(84);
		 if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		 Status_Printer = Printer_PrintStringPersian(SystemParameters.PrintFormatParameters.Title_01[1]);
		 if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

//----------------------------------------------------------------------------------------------------------

	Status_Printer = Printer_SetHorizontalCursorPositioning(15);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.TicketSerialNumber, 5).c_str());
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_Space(5);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_PrintStringPersian("قبض: ");
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

//----------------------------------------------------------------------------------------------------------

	Status_Printer = Printer_SendCommand(PRINTER_USER_DEFINED_SET_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(13);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x21);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xDA);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xDA);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	for(int i = 0; i < MaxCharacterToLine; i++)
	{
		if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
		{
			Status_Printer = Printer_SendChar(0x2F);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}
		else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
		{
			Status_Printer = Printer_SendChar(0xC4);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}
		else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
		{
			Status_Printer = Printer_SendChar(0xC4);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}
	}

	Status_Printer = Printer_SetHorizontalCursorPositioning(84);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x22);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xBF);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xBF);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(13);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x25);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	
//----------------------------------------------------------------------------------------------------------

	Status_Printer = Printer_SetHorizontalCursorPositioning(13);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_Space(8);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

 	Status_Printer = Printer_PrintStringPersian("ش. پلاک");
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(41);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

 	Status_Printer = Printer_PrintStringPersian("کالا");
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(43);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_Space(8);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

 	Status_Printer = Printer_PrintStringPersian("تاریخ");
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(56);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_Space(5);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

 	Status_Printer = Printer_PrintStringPersian("ساعت");
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(62);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_Space(6);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

 	Status_Printer = Printer_PrintStringPersian("وزن");
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

//----------------------------------------------------------------------------------------------------------

	Status_Printer = Printer_SendCommand(PRINTER_USER_DEFINED_SET_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(84);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x25);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(13);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x28);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xC3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xC3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	for(int i = 0; i < MaxCharacterToLine; i++)
	{
		if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
		{
			Status_Printer = Printer_SendChar(0x2F);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}
		else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
		{
			Status_Printer = Printer_SendChar(0xC4);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}
		else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
		{
			Status_Printer = Printer_SendChar(0xC4);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}
	}

	Status_Printer = Printer_SetHorizontalCursorPositioning(84);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x29);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB4);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB4);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(13);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x25);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

//----------------------------------------------------------------------------------------------------------

	Status_Printer = Printer_SendCommand(PRINTER_USER_DEFINED_SET_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(13);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x25);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	Status_Printer = Printer_SetHorizontalCursorPositioning(84);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x25);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(13);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x25);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

//----------------------------------------------------------------------------------------------------------

	Status_Printer = Printer_SetHorizontalCursorPositioning(13);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_Space(8);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

 	Status_Printer = Printer_PrintStringPersian(WeighingInformation.NumberTruck);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(41);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

 	Status_Printer = Printer_PrintStringPersian(WeighingInformation.ProductParameters.Name);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(43);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_Space();
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.SecondWeightDate.Year, 4).c_str());
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SendCommand(PRINTER_NORMAL_SET_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SendChar('/');
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.SecondWeightDate.Month, 2).c_str());
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SendCommand(PRINTER_NORMAL_SET_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SendChar('/');
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.SecondWeightDate.Date, 2).c_str());
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(56);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_Space();
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.SecondWeightTime.Hours, 2).c_str());
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SendCommand(PRINTER_NORMAL_SET_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SendChar(':');
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.SecondWeightTime.Minutes, 2).c_str());
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(62);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_Space();
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_PrintNumbersPersian((char *)WeightToString(WeighingInformation.SecondWeight, WeighingInformation.DISPDigits, WeighingInformation.DECPoint, WeighingInformation.LeadZBlank).c_str());
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(84);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

 	Status_Printer = Printer_PrintStringPersian("پر");
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

//----------------------------------------------------------------------------------------------------------

	Status_Printer = Printer_SendCommand(PRINTER_USER_DEFINED_SET_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(84);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x25);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(13);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x28);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xC3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xC3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	for(int i = 0; i < MaxCharacterToLine; i++)
	{
		if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
		{
			Status_Printer = Printer_SendChar(0x2F);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}
		else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
		{
			Status_Printer = Printer_SendChar(0xC4);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}
		else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
		{
			Status_Printer = Printer_SendChar(0xC4);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}
	}

	Status_Printer = Printer_SetHorizontalCursorPositioning(84);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x29);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB4);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB4);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(13);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x25);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

//----------------------------------------------------------------------------------------------------------

	Status_Printer = Printer_SendCommand(PRINTER_USER_DEFINED_SET_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(13);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x25);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	Status_Printer = Printer_SetHorizontalCursorPositioning(84);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x25);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(13);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x25);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

//----------------------------------------------------------------------------------------------------------

	Status_Printer = Printer_SetHorizontalCursorPositioning(43);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_Space();
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.FirstWeightDate.Year, 4).c_str());
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SendCommand(PRINTER_NORMAL_SET_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SendChar('/');
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.FirstWeightDate.Month, 2).c_str());
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SendCommand(PRINTER_NORMAL_SET_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SendChar('/');
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.FirstWeightDate.Date, 2).c_str());
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;
	
	Status_Printer = Printer_SetHorizontalCursorPositioning(56);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_Space();
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.FirstWeightTime.Hours, 2).c_str());
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SendCommand(PRINTER_NORMAL_SET_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SendChar(':');
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.FirstWeightTime.Minutes, 2).c_str());
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(62);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_Space();
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_PrintNumbersPersian((char *)WeightToString(WeighingInformation.FirstWeight, WeighingInformation.DISPDigits, WeighingInformation.DECPoint, WeighingInformation.LeadZBlank).c_str());
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(WeighingInformation.TypeFirstWeight == 1)
	{
		Status_Printer = Printer_SendCommand(PRINTER_NORMAL_SET_COMMAND);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	
		Status_Printer = Printer_SendString("  *");
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	
	Status_Printer = Printer_SetHorizontalCursorPositioning(84);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

 	Status_Printer = Printer_PrintStringPersian("خالی");
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

//----------------------------------------------------------------------------------------------------------

	Status_Printer = Printer_SendCommand(PRINTER_USER_DEFINED_SET_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(84);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x25);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(13);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x28);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xC3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xC3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	for(int i = 0; i < MaxCharacterToLine; i++)
	{
		if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
		{
			Status_Printer = Printer_SendChar(0x2F);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}
		else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
		{
			Status_Printer = Printer_SendChar(0xC4);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}
		else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
		{
			Status_Printer = Printer_SendChar(0xC4);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}
	}

	Status_Printer = Printer_SetHorizontalCursorPositioning(84);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x29);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB4);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB4);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(13);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x25);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

//----------------------------------------------------------------------------------------------------------

	Status_Printer = Printer_SendCommand(PRINTER_USER_DEFINED_SET_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(13);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x25);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	Status_Printer = Printer_SetHorizontalCursorPositioning(84);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x25);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(13);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x25);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

//----------------------------------------------------------------------------------------------------------

	if(WeighingInformation.NetWeight != 0)
	{
		if(WeighingInformation.ProductPrice != 0)
		{
			Status_Printer = Printer_SetHorizontalCursorPositioning(35);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.ProductPrice, 1).c_str());
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_SetHorizontalCursorPositioning(34);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintStringPersian("تومان");
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}
		
		Status_Printer = Printer_SetHorizontalCursorPositioning(62);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Printer_Space();
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Printer_PrintNumbersPersian((char *)WeightToString(WeighingInformation.NetWeight, WeighingInformation.DISPDigits, WeighingInformation.DECPoint, WeighingInformation.LeadZBlank).c_str());
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Printer_SetHorizontalCursorPositioning(84);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Printer_PrintStringPersian("خالص");
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

//----------------------------------------------------------------------------------------------------------

	Status_Printer = Printer_SendCommand(PRINTER_USER_DEFINED_SET_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(84);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x25);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(13);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x23);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xC0);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xC0);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}


	for(int i = 0; i < MaxCharacterToLine; i++)
	{
		if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
		{
			Status_Printer = Printer_SendChar(0x2F);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}
		else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
		{
			Status_Printer = Printer_SendChar(0xC4);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}
		else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
		{
			Status_Printer = Printer_SendChar(0xC4);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}
	}

	Status_Printer = Printer_SetHorizontalCursorPositioning(84);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x24);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xD9);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xD9);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

//----------------------------------------------------------------------------------------------------------

	Status_Printer = Printer_SetHorizontalCursorPositioning(84);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_PrintStringPersian("نام مشتری : ");
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(66);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_PrintStringPersian(WeighingInformation.CustomerParameters.Name);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

//----------------------------------------------------------------------------------------------------------

	if(WeighingInformation.WeighingCost != 0)
	{
		Status_Printer = Printer_SetHorizontalCursorPositioning(40);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Printer_PrintStringPersian("حق توزین)تومان( : ");
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Printer_Backspace(7);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.WeighingCost, 7, false).c_str());
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.PrintFont == 3)
	{
		Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

//----------------------------------------------------------------------------------------------------------

	if(SystemParameters.PrintFormatParameters.PrintTitle)
	{
		Status_Printer = Printer_SetHorizontalCursorPositioning(84);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Printer_PrintStringPersian(SystemParameters.PrintFormatParameters.Title_02[0]);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		if(SystemParameters.PrintFormatParameters.PrintFont == 3)
		{
			Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}

		Status_Printer = Printer_SetHorizontalCursorPositioning(84);
		 if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		 Status_Printer = Printer_PrintStringPersian(SystemParameters.PrintFormatParameters.Title_02[1]);
		 if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		if(SystemParameters.PrintFormatParameters.PrintFont == 3)
		{
			Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}
	}

//----------------------------------------------------------------------------------------------------------

	for(int i = 0; i < SystemParameters.PrintFormatParameters.BottomLineEmpty; i++)
	{
		Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

//----------------------------------------------------------------------------------------------------------

	return OK_PRINTER;
}

//**********************************************************************************************************

Status_Printer_TypeDef SystemFunction_PrintWeightTicketFormatStandard_02(WeighingInformation_TypeDef WeighingInformation, bool SelectPrintFont)
{
	Status_Printer_TypeDef Status_Printer;
	uint8_t MaxCharacterToLine;

	while(1)
	{
		if(Printer_CheckPaper() == NO_PAPER_PRINTER)
		{
			GLCD_FillRectangle(10, 14, 229, 58, BLACK);
			GLCD_FillRectangle(12, 16, 227, 56, WHITE);

			SetFontLetter(PERSIAN_LETTER);
			SetFontColor(BLACK);
			SetBackgroundColor(WHITE);
			LcdFontSpaceX(1);
			LcdFontSpaceY(0);
			SetFont(Font_Pe_12X11);
			SetTextPixelPosition(110, 27);
			PutsString("خطا");
			SetTextPixelPosition(87, 41);
			PutsString("عدم وجود کاغذ");
			GLCD_Screen_Update();

			if(Panel_ReadPanel() == KEYPAD_ESC)
				return NO_PAPER_PRINTER;
		}
		else
			break;
	}

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
	GLCD_FillRectangle((58 + 62), 43, 181, 50, WHITE);

	GLCD_Screen_Update();

//----------------------------------------------------------------------------------------------------------

	if(SelectPrintFont)
	{
		if(SystemParameters.PrintFormatParameters.PrintFont == 0)
			MaxCharacterToLine = Printer_SelectPrintFont_01();
		else if(SystemParameters.PrintFormatParameters.PrintFont == 1)
			MaxCharacterToLine = Printer_SelectPrintFont_02();
		else if(SystemParameters.PrintFormatParameters.PrintFont == 2)
			MaxCharacterToLine = Printer_SelectPrintFont_03();
		else if(SystemParameters.PrintFormatParameters.PrintFont == 3)
			MaxCharacterToLine = Printer_SelectPrintFont_04();
	}

//----------------------------------------------------------------------------------------------------------

	for(int i = 0; i < SystemParameters.PrintFormatParameters.TopLineEmpty; i++)
	{
		Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

//----------------------------------------------------------------------------------------------------------

	if(SystemParameters.PrintFormatParameters.PrintTitle)
	{
		Status_Printer = Printer_SetHorizontalCursorPositioning(84);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Printer_PrintStringPersian(SystemParameters.PrintFormatParameters.Title_01[0]);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		if(SystemParameters.PrintFormatParameters.PrintFont == 3)
		{
			Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}

		Status_Printer = Printer_SetHorizontalCursorPositioning(84);
		 if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		 Status_Printer = Printer_PrintStringPersian(SystemParameters.PrintFormatParameters.Title_01[1]);
		 if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		 Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		if(SystemParameters.PrintFormatParameters.PrintFont == 3)
		{
			Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}
	}

//----------------------------------------------------------------------------------------------------------

	Status_Printer = Printer_SetHorizontalCursorPositioning(84);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_PrintStringPersian("نام مشتری : ");
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(66);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_PrintStringPersian(WeighingInformation.CustomerParameters.Name);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

//----------------------------------------------------------------------------------------------------------

	if(WeighingInformation.WeighingCost != 0)
	{
		Status_Printer = Printer_SetHorizontalCursorPositioning(40);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Printer_PrintStringPersian("حق توزین)تومان( : ");
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Printer_Backspace(7);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.WeighingCost, 7, false).c_str());
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.PrintFont == 3)
	{
		Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

//----------------------------------------------------------------------------------------------------------

	Status_Printer = Printer_SetHorizontalCursorPositioning(84);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_PrintStringPersian("قبض : ");
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_Backspace(5);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.TicketSerialNumber, 5).c_str());
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.PrintFont == 3)
	{
		Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

//----------------------------------------------------------------------------------------------------------

	Status_Printer = Printer_SetHorizontalCursorPositioning(84);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_PrintStringPersian("ش. پلاک : ");
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(69);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_PrintStringPersian(WeighingInformation.NumberTruck);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.PrintFont == 3)
	{
		Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

//----------------------------------------------------------------------------------------------------------

	Status_Printer = Printer_SetHorizontalCursorPositioning(84);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_PrintStringPersian("کالا : ");
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(71);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_PrintStringPersian(WeighingInformation.ProductParameters.Name);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;
	
	if(WeighingInformation.ProductPrice != 0)
	{
		Status_Printer = Printer_SetHorizontalCursorPositioning(35);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.ProductPrice, 1).c_str());
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Printer_SetHorizontalCursorPositioning(34);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Printer_PrintStringPersian("تومان");
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

//----------------------------------------------------------------------------------------------------------

	Status_Printer = Printer_SendCommand(PRINTER_USER_DEFINED_SET_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(13);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x21);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xDA);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xDA);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	for(int i = 0; i < MaxCharacterToLine; i++)
	{
		if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
		{
			Status_Printer = Printer_SendChar(0x2F);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}
		else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
		{
			Status_Printer = Printer_SendChar(0xC4);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}
		else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
		{
			Status_Printer = Printer_SendChar(0xC4);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}
	}

	Status_Printer = Printer_SetHorizontalCursorPositioning(84);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x22);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xBF);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xBF);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(13);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x25);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

//----------------------------------------------------------------------------------------------------------

	Status_Printer = Printer_SetHorizontalCursorPositioning(13);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_Space(8);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

 	Status_Printer = Printer_PrintStringPersian("تاریخ");
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(33);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_Space(5);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

 	Status_Printer = Printer_PrintStringPersian("ساعت");
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(53);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_Space(6);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

 	Status_Printer = Printer_PrintStringPersian("وزن");
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

//----------------------------------------------------------------------------------------------------------

	Status_Printer = Printer_SendCommand(PRINTER_USER_DEFINED_SET_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(84);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x25);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(13);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x28);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xC3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xC3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	for(int i = 0; i < MaxCharacterToLine; i++)
	{
		if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
		{
			Status_Printer = Printer_SendChar(0x2F);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}
		else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
		{
			Status_Printer = Printer_SendChar(0xC4);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}
		else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
		{
			Status_Printer = Printer_SendChar(0xC4);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}
	}

	Status_Printer = Printer_SetHorizontalCursorPositioning(84);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x29);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB4);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB4);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(13);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x25);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

//----------------------------------------------------------------------------------------------------------

	Status_Printer = Printer_SendCommand(PRINTER_USER_DEFINED_SET_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(13);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x25);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	Status_Printer = Printer_SetHorizontalCursorPositioning(84);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x25);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(13);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x25);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

//----------------------------------------------------------------------------------------------------------

	Status_Printer = Printer_SetHorizontalCursorPositioning(13);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_Space();
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.SecondWeightDate.Year, 4).c_str());
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SendCommand(PRINTER_NORMAL_SET_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SendChar('/');
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.SecondWeightDate.Month, 2).c_str());
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SendCommand(PRINTER_NORMAL_SET_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SendChar('/');
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.SecondWeightDate.Date, 2).c_str());
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(33);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_Space();
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.SecondWeightTime.Hours, 2).c_str());
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SendCommand(PRINTER_NORMAL_SET_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SendChar(':');
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.SecondWeightTime.Minutes, 2).c_str());
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(53);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_Space();
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_PrintNumbersPersian((char *)WeightToString(WeighingInformation.SecondWeight, WeighingInformation.DISPDigits, WeighingInformation.DECPoint, WeighingInformation.LeadZBlank).c_str());
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(84);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

 	Status_Printer = Printer_PrintStringPersian("پر");
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

//----------------------------------------------------------------------------------------------------------

	Status_Printer = Printer_SendCommand(PRINTER_USER_DEFINED_SET_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(84);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x25);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(13);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x28);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xC3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xC3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	for(int i = 0; i < MaxCharacterToLine; i++)
	{
		if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
		{
			Status_Printer = Printer_SendChar(0x2F);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}
		else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
		{
			Status_Printer = Printer_SendChar(0xC4);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}
		else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
		{
			Status_Printer = Printer_SendChar(0xC4);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}
	}

	Status_Printer = Printer_SetHorizontalCursorPositioning(84);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x29);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB4);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB4);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(13);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x25);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

//----------------------------------------------------------------------------------------------------------

	Status_Printer = Printer_SendCommand(PRINTER_USER_DEFINED_SET_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(13);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x25);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	Status_Printer = Printer_SetHorizontalCursorPositioning(84);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x25);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(13);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x25);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

//----------------------------------------------------------------------------------------------------------

	Status_Printer = Printer_SetHorizontalCursorPositioning(13);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_Space();
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.FirstWeightDate.Year, 4).c_str());
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SendCommand(PRINTER_NORMAL_SET_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SendChar('/');
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.FirstWeightDate.Month, 2).c_str());
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SendCommand(PRINTER_NORMAL_SET_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SendChar('/');
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.FirstWeightDate.Date, 2).c_str());
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;
	
	Status_Printer = Printer_SetHorizontalCursorPositioning(33);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_Space();
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.FirstWeightTime.Hours, 2).c_str());
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SendCommand(PRINTER_NORMAL_SET_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SendChar(':');
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.FirstWeightTime.Minutes, 2).c_str());
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(53);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_Space();
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_PrintNumbersPersian((char *)WeightToString(WeighingInformation.FirstWeight, WeighingInformation.DISPDigits, WeighingInformation.DECPoint, WeighingInformation.LeadZBlank).c_str());
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;
	
	if(WeighingInformation.TypeFirstWeight == 1)
	{
		Status_Printer = Printer_SendCommand(PRINTER_NORMAL_SET_COMMAND);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	
		Status_Printer = Printer_SendString("  *");
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	Status_Printer = Printer_SetHorizontalCursorPositioning(84);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

 	Status_Printer = Printer_PrintStringPersian("خالی");
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

//----------------------------------------------------------------------------------------------------------

	Status_Printer = Printer_SendCommand(PRINTER_USER_DEFINED_SET_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(84);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x25);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(13);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x28);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xC3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xC3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	for(int i = 0; i < MaxCharacterToLine; i++)
	{
		if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
		{
			Status_Printer = Printer_SendChar(0x2F);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}
		else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
		{
			Status_Printer = Printer_SendChar(0xC4);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}
		else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
		{
			Status_Printer = Printer_SendChar(0xC4);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}
	}

	Status_Printer = Printer_SetHorizontalCursorPositioning(84);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x29);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB4);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB4);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(13);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x25);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

//----------------------------------------------------------------------------------------------------------

	Status_Printer = Printer_SendCommand(PRINTER_USER_DEFINED_SET_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(13);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x25);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	Status_Printer = Printer_SetHorizontalCursorPositioning(84);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x25);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(13);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x25);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

//----------------------------------------------------------------------------------------------------------

	if(WeighingInformation.NetWeight != 0)
	{
		Status_Printer = Printer_SetHorizontalCursorPositioning(53);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Printer_Space();
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Printer_PrintNumbersPersian((char *)WeightToString(WeighingInformation.NetWeight, WeighingInformation.DISPDigits, WeighingInformation.DECPoint, WeighingInformation.LeadZBlank).c_str());
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Printer_SetHorizontalCursorPositioning(84);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Printer_PrintStringPersian("خالص");
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

//----------------------------------------------------------------------------------------------------------

	Status_Printer = Printer_SendCommand(PRINTER_USER_DEFINED_SET_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(84);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x25);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xB3);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Printer_SetHorizontalCursorPositioning(13);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x23);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xC0);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xC0);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	for(int i = 0; i < MaxCharacterToLine; i++)
	{
		if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
		{
			Status_Printer = Printer_SendChar(0x2F);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}
		else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
		{
			Status_Printer = Printer_SendChar(0xC4);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}
		else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
		{
			Status_Printer = Printer_SendChar(0xC4);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}
	}

	Status_Printer = Printer_SetHorizontalCursorPositioning(84);
 	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	if(SystemParameters.PrintFormatParameters.TypePrinter == 0)
	{
		Status_Printer = Printer_SendChar(0x24);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 1)
	{
		Status_Printer = Printer_SendChar(0xD9);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}
	else if(SystemParameters.PrintFormatParameters.TypePrinter == 2)
	{
		Status_Printer = Printer_SendChar(0xD9);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

//----------------------------------------------------------------------------------------------------------

	if(SystemParameters.PrintFormatParameters.PrintTitle)
	{
		Status_Printer = Printer_SetHorizontalCursorPositioning(84);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Printer_PrintStringPersian(SystemParameters.PrintFormatParameters.Title_02[0]);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		if(SystemParameters.PrintFormatParameters.PrintFont == 3)
		{
			Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}

		Status_Printer = Printer_SetHorizontalCursorPositioning(84);
		 if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		 Status_Printer = Printer_PrintStringPersian(SystemParameters.PrintFormatParameters.Title_02[1]);
		 if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		if(SystemParameters.PrintFormatParameters.PrintFont == 3)
		{
			Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}
	}

//----------------------------------------------------------------------------------------------------------

	for(int i = 0; i < SystemParameters.PrintFormatParameters.BottomLineEmpty; i++)
	{
		Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

//----------------------------------------------------------------------------------------------------------

	return OK_PRINTER;
}

//**********************************************************************************************************

Status_Printer_TypeDef SystemFunction_PrintWeightTicketFormatUserDefined(WeighingInformation_TypeDef WeighingInformation, bool SelectPrintFont)
{
	Status_Printer_TypeDef Status_Printer;
	uint8_t MaxCharacterToLine;
	uint8_t LinePrint = 0;

	while(1)
	{
		if(Printer_CheckPaper() == NO_PAPER_PRINTER)
		{
			GLCD_FillRectangle(10, 14, 229, 58, BLACK);
			GLCD_FillRectangle(12, 16, 227, 56, WHITE);

			SetFontLetter(PERSIAN_LETTER);
			SetFontColor(BLACK);
			SetBackgroundColor(WHITE);
			LcdFontSpaceX(1);
			LcdFontSpaceY(0);
			SetFont(Font_Pe_12X11);
			SetTextPixelPosition(110, 27);
			PutsString("خطا");
			SetTextPixelPosition(87, 41);
			PutsString("عدم وجود کاغذ");
			GLCD_Screen_Update();

			if(Panel_ReadPanel() == KEYPAD_ESC)
				return NO_PAPER_PRINTER;
		}
		else
			break;
	}

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
	GLCD_FillRectangle((58 + 62), 43, 181, 50, WHITE);

	GLCD_Screen_Update();

//----------------------------------------------------------------------------------------------------------

	if(SelectPrintFont)
	{
		if(SystemParameters.PrintFormatParameters.PrintFont == 0)
			MaxCharacterToLine = Printer_SelectPrintFont_01();
		else if(SystemParameters.PrintFormatParameters.PrintFont == 1)
			MaxCharacterToLine = Printer_SelectPrintFont_02();
		else if(SystemParameters.PrintFormatParameters.PrintFont == 2)
			MaxCharacterToLine = Printer_SelectPrintFont_03();
		else if(SystemParameters.PrintFormatParameters.PrintFont == 3)
			MaxCharacterToLine = Printer_SelectPrintFont_04();
	}

//----------------------------------------------------------------------------------------------------------

	for(int i = 0; i < SystemParameters.PrintFormatParameters.TopLineEmpty; i++)
	{
		Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

//----------------------------------------------------------------------------------------------------------

	if(SystemParameters.PrintFormatParameters.PrintTitle)
	{
		Status_Printer = Printer_SetHorizontalCursorPositioning(84);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Printer_PrintStringPersian(SystemParameters.PrintFormatParameters.Title_01[0]);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		if(SystemParameters.PrintFormatParameters.PrintFont == 3)
		{
			Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}

		Status_Printer = Printer_SetHorizontalCursorPositioning(84);
		 if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		 Status_Printer = Printer_PrintStringPersian(SystemParameters.PrintFormatParameters.Title_01[1]);
		 if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

//----------------------------------------------------------------------------------------------------------

	if(SystemParameters.PrintFormatParameters.TitleNumberPlates.Line > LinePrint)
		LinePrint = SystemParameters.PrintFormatParameters.TitleNumberPlates.Line;

	if(SystemParameters.PrintFormatParameters.TitleProduct.Line > LinePrint)
		LinePrint = SystemParameters.PrintFormatParameters.TitleProduct.Line;

	if(SystemParameters.PrintFormatParameters.TitleTime.Line > LinePrint)
		LinePrint = SystemParameters.PrintFormatParameters.TitleTime.Line;

	if(SystemParameters.PrintFormatParameters.TitleDate.Line > LinePrint)
		LinePrint = SystemParameters.PrintFormatParameters.TitleDate.Line;

	if(SystemParameters.PrintFormatParameters.TitleWeight.Line > LinePrint)
		LinePrint = SystemParameters.PrintFormatParameters.TitleWeight.Line;

	if(SystemParameters.PrintFormatParameters.TitleTicketSerialNumber.Line > LinePrint)
		LinePrint = SystemParameters.PrintFormatParameters.TitleTicketSerialNumber.Line;

	if(SystemParameters.PrintFormatParameters.TitleCustomer.Line > LinePrint)
		LinePrint = SystemParameters.PrintFormatParameters.TitleCustomer.Line;

	if(SystemParameters.PrintFormatParameters.NumberPlates.Line > LinePrint)
		LinePrint = SystemParameters.PrintFormatParameters.NumberPlates.Line;

	if(SystemParameters.PrintFormatParameters.Product.Line > LinePrint)
		LinePrint = SystemParameters.PrintFormatParameters.Product.Line;

	if(SystemParameters.PrintFormatParameters.FirstWeightDate.Line > LinePrint)
		LinePrint = SystemParameters.PrintFormatParameters.FirstWeightDate.Line;

	if(SystemParameters.PrintFormatParameters.SecondWeightDate.Line > LinePrint)
		LinePrint = SystemParameters.PrintFormatParameters.SecondWeightDate.Line;

	if(SystemParameters.PrintFormatParameters.FirstWeightTime.Line > LinePrint)
		LinePrint = SystemParameters.PrintFormatParameters.FirstWeightTime.Line;

	if(SystemParameters.PrintFormatParameters.SecondWeightTime.Line > LinePrint)
		LinePrint = SystemParameters.PrintFormatParameters.SecondWeightTime.Line;

	if(SystemParameters.PrintFormatParameters.FirstWeight.Line > LinePrint)
		LinePrint = SystemParameters.PrintFormatParameters.FirstWeight.Line;

	if(SystemParameters.PrintFormatParameters.SecondWeight.Line > LinePrint)
		LinePrint = SystemParameters.PrintFormatParameters.SecondWeight.Line;

	if(SystemParameters.PrintFormatParameters.NetWeight.Line > LinePrint)
		LinePrint = SystemParameters.PrintFormatParameters.NetWeight.Line;

	if(SystemParameters.PrintFormatParameters.TicketSerialNumber.Line > LinePrint)
		LinePrint = SystemParameters.PrintFormatParameters.TicketSerialNumber.Line;

	if(SystemParameters.PrintFormatParameters.Customer.Line > LinePrint)
		LinePrint = SystemParameters.PrintFormatParameters.Customer.Line;

	if(SystemParameters.PrintFormatParameters.WeighingCost.Line > LinePrint)
		LinePrint = SystemParameters.PrintFormatParameters.WeighingCost.Line;

	if(SystemParameters.PrintFormatParameters.TitleWeighingCost.Line > LinePrint)
		LinePrint = SystemParameters.PrintFormatParameters.TitleWeighingCost.Line;

	if(SystemParameters.PrintFormatParameters.ProductPrice.Line > LinePrint)
		LinePrint = SystemParameters.PrintFormatParameters.ProductPrice.Line;

//----------------------------------------------------------------------------------------------------------

	for(int Line = 1; Line <= LinePrint; Line++)
	{
		if(Line == SystemParameters.PrintFormatParameters.TitleNumberPlates.Line)
		{
			Status_Printer = Printer_SetHorizontalCursorPositioning(SystemParameters.PrintFormatParameters.TitleNumberPlates.Column);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintStringPersian("ش. پلاک");
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}

		if(Line == SystemParameters.PrintFormatParameters.TitleProduct.Line)
		{
			Status_Printer = Printer_SetHorizontalCursorPositioning(SystemParameters.PrintFormatParameters.TitleProduct.Column);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintStringPersian("کالا");
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}

		if(Line == SystemParameters.PrintFormatParameters.TitleTime.Line)
		{
			Status_Printer = Printer_SetHorizontalCursorPositioning(SystemParameters.PrintFormatParameters.TitleTime.Column);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintStringPersian("ساعت");
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}

		if(Line == SystemParameters.PrintFormatParameters.TitleDate.Line)
		{
			Status_Printer = Printer_SetHorizontalCursorPositioning(SystemParameters.PrintFormatParameters.TitleDate.Column);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintStringPersian("تاریخ");
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}

		if(Line == SystemParameters.PrintFormatParameters.TitleWeight.Line)
		{
			Status_Printer = Printer_SetHorizontalCursorPositioning(SystemParameters.PrintFormatParameters.TitleWeight.Column);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintStringPersian("وزن");
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}

		if(Line == SystemParameters.PrintFormatParameters.TitleTicketSerialNumber.Line)
		{
			Status_Printer = Printer_SetHorizontalCursorPositioning(SystemParameters.PrintFormatParameters.TitleTicketSerialNumber.Column);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintStringPersian("قبض: ");
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}

		if(Line == SystemParameters.PrintFormatParameters.TitleCustomer.Line)
		{
			Status_Printer = Printer_SetHorizontalCursorPositioning(SystemParameters.PrintFormatParameters.TitleCustomer.Column);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintStringPersian("نام مشتری : ");
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}

		if(Line == SystemParameters.PrintFormatParameters.TitleWeighingCost.Line)
		{
			Status_Printer = Printer_SetHorizontalCursorPositioning(SystemParameters.PrintFormatParameters.TitleWeighingCost.Column);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintStringPersian("حق توزین)تومان( : ");
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}

		if(Line == SystemParameters.PrintFormatParameters.NumberPlates.Line)
		{
			Status_Printer = Printer_SetHorizontalCursorPositioning(SystemParameters.PrintFormatParameters.NumberPlates.Column);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintStringPersian(WeighingInformation.NumberTruck);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}

		if(Line == SystemParameters.PrintFormatParameters.Product.Line)
		{
			Status_Printer = Printer_SetHorizontalCursorPositioning(SystemParameters.PrintFormatParameters.Product.Column);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintStringPersian(WeighingInformation.ProductParameters.Name);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}

		if((Line == SystemParameters.PrintFormatParameters.SecondWeightDate.Line) && (WeighingInformation.SecondWeight != 0))
		{
			Status_Printer = Printer_SetHorizontalCursorPositioning(SystemParameters.PrintFormatParameters.SecondWeightDate.Column);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.SecondWeightDate.Year, 4).c_str());
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_SendCommand(PRINTER_NORMAL_SET_COMMAND);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_SendChar('/');
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.SecondWeightDate.Month, 2).c_str());
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_SendCommand(PRINTER_NORMAL_SET_COMMAND);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_SendChar('/');
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.SecondWeightDate.Date, 2).c_str());
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}

		if((Line == SystemParameters.PrintFormatParameters.SecondWeightTime.Line) && (WeighingInformation.SecondWeight != 0))
		{
			Status_Printer = Printer_SetHorizontalCursorPositioning(SystemParameters.PrintFormatParameters.SecondWeightTime.Column);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.SecondWeightTime.Hours, 2).c_str());
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_SendCommand(PRINTER_NORMAL_SET_COMMAND);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_SendChar(':');
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.SecondWeightTime.Minutes, 2).c_str());
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}

		if((Line == SystemParameters.PrintFormatParameters.SecondWeight.Line) && (WeighingInformation.SecondWeight != 0))
		{
			Status_Printer = Printer_SetHorizontalCursorPositioning(SystemParameters.PrintFormatParameters.SecondWeight.Column);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintNumbersPersian((char *)WeightToString(WeighingInformation.SecondWeight, WeighingInformation.DISPDigits, WeighingInformation.DECPoint, WeighingInformation.LeadZBlank).c_str());
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}

		if((Line == SystemParameters.PrintFormatParameters.FirstWeightDate.Line) && (WeighingInformation.FirstWeight != 0))
		{
			Status_Printer = Printer_SetHorizontalCursorPositioning(SystemParameters.PrintFormatParameters.FirstWeightDate.Column);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.FirstWeightDate.Year, 4).c_str());
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_SendCommand(PRINTER_NORMAL_SET_COMMAND);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_SendChar('/');
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.FirstWeightDate.Month, 2).c_str());
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_SendCommand(PRINTER_NORMAL_SET_COMMAND);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_SendChar('/');
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.FirstWeightDate.Date, 2).c_str());
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}

		if((Line == SystemParameters.PrintFormatParameters.FirstWeightTime.Line) && (WeighingInformation.FirstWeight != 0))
		{
			Status_Printer = Printer_SetHorizontalCursorPositioning(SystemParameters.PrintFormatParameters.FirstWeightTime.Column);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.FirstWeightTime.Hours, 2).c_str());
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_SendCommand(PRINTER_NORMAL_SET_COMMAND);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_SendChar(':');
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.FirstWeightTime.Minutes, 2).c_str());
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}

		if((Line == SystemParameters.PrintFormatParameters.FirstWeight.Line) && (WeighingInformation.FirstWeight != 0))
		{
			Status_Printer = Printer_SetHorizontalCursorPositioning(SystemParameters.PrintFormatParameters.FirstWeight.Column);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintNumbersPersian((char *)WeightToString(WeighingInformation.FirstWeight, WeighingInformation.DISPDigits, WeighingInformation.DECPoint, WeighingInformation.LeadZBlank).c_str());
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
			
			if(WeighingInformation.TypeFirstWeight == 1)
			{
				Status_Printer = Printer_SendCommand(PRINTER_NORMAL_SET_COMMAND);
				if(Status_Printer != OK_PRINTER)
					return ERROR_PRINTER;
	
				Status_Printer = Printer_SendString("  *");
				if(Status_Printer != OK_PRINTER)
					return ERROR_PRINTER;
			}
		}

		if((Line == SystemParameters.PrintFormatParameters.NetWeight.Line) && (WeighingInformation.NetWeight != 0))
		{
			Status_Printer = Printer_SetHorizontalCursorPositioning(SystemParameters.PrintFormatParameters.NetWeight.Column);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintNumbersPersian((char *)WeightToString(WeighingInformation.NetWeight, WeighingInformation.DISPDigits, WeighingInformation.DECPoint, WeighingInformation.LeadZBlank).c_str());
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}

		if(Line == SystemParameters.PrintFormatParameters.TicketSerialNumber.Line)
		{
			Status_Printer = Printer_SetHorizontalCursorPositioning(SystemParameters.PrintFormatParameters.TicketSerialNumber.Column);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.TicketSerialNumber, 5).c_str());
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}

		if(Line == SystemParameters.PrintFormatParameters.Customer.Line)
		{
			Status_Printer = Printer_SetHorizontalCursorPositioning(SystemParameters.PrintFormatParameters.Customer.Column);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintStringPersian(WeighingInformation.CustomerParameters.Name);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}

		if((Line == SystemParameters.PrintFormatParameters.WeighingCost.Line) && (WeighingInformation.WeighingCost != 0))
		{
			Status_Printer = Printer_SetHorizontalCursorPositioning(SystemParameters.PrintFormatParameters.WeighingCost.Column);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.WeighingCost, 7, false).c_str());
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}

		if((Line == SystemParameters.PrintFormatParameters.ProductPrice.Line) && (WeighingInformation.ProductPrice != 0))
		{
			Status_Printer = Printer_SetHorizontalCursorPositioning(SystemParameters.PrintFormatParameters.ProductPrice.Column);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(WeighingInformation.ProductPrice, 1).c_str());
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_SetHorizontalCursorPositioning((SystemParameters.PrintFormatParameters.ProductPrice.Column - 1));
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintStringPersian("تومان");
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}

		Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

//----------------------------------------------------------------------------------------------------------

	if(SystemParameters.PrintFormatParameters.PrintTitle)
	{
		Status_Printer = Printer_SetHorizontalCursorPositioning(84);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Printer_PrintStringPersian(SystemParameters.PrintFormatParameters.Title_02[0]);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		if(SystemParameters.PrintFormatParameters.PrintFont == 3)
		{
			Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}

		Status_Printer = Printer_SetHorizontalCursorPositioning(84);
		 if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		 Status_Printer = Printer_PrintStringPersian(SystemParameters.PrintFormatParameters.Title_02[1]);
		 if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		if(SystemParameters.PrintFormatParameters.PrintFont == 3)
		{
			Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}
	}

//----------------------------------------------------------------------------------------------------------

	for(int i = 0; i < SystemParameters.PrintFormatParameters.BottomLineEmpty; i++)
	{
		Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

//----------------------------------------------------------------------------------------------------------

	return OK_PRINTER;
}

//**********************************************************************************************************

void SystemFunction_ShowProductCode(void)
{
	PanelKeypadButton_TypeDef Button;
	ProductParameters_TypeDef ProductParameters = {0};
	string Str = "";
	uint16_t ProductCode = 0;

	while(1)
	{
		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_9X8);
		SetTextPixelPosition(62, 2);
		PutsString("چاپ کد کالا و قیمت واحد");

//----------------------------------------------------------------------------------------------------------

		SetFontLetter(PERSIAN_LETTER);
		SetFont(Font_Pe_12X11);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);

		ProductParameters = ReadProductCode(ProductCode);
		SetFontLetter(PERSIAN_LETTER);
		SetTextPixelPosition(5, 18);
		Str = IntegerToString(ProductCode, 3);
		PutsString((char *)Str.c_str());
		PutsString("- ");
		PutsString(ProductParameters.Name);
		SetFontLetter(ENGLISH_LETTER);
		SetTextPixelPosition(10, 18);
		Str = IntegerToString(ProductParameters.Price, 7);
		PutsString((char *)Str.c_str());

		ProductParameters = ReadProductCode((ProductCode + 1));
		SetFontLetter(PERSIAN_LETTER);
		SetTextPixelPosition(5, 33);
		Str = IntegerToString((ProductCode + 1), 3);
		PutsString((char *)Str.c_str());
		PutsString("- ");
		PutsString(ProductParameters.Name);
		SetFontLetter(ENGLISH_LETTER);
		SetTextPixelPosition(10, 33);
		Str = IntegerToString(ProductParameters.Price, 7);
		PutsString((char *)Str.c_str());

		ProductParameters = ReadProductCode((ProductCode + 2));
		SetFontLetter(PERSIAN_LETTER);
		SetTextPixelPosition(5, 48);
		Str = IntegerToString((ProductCode + 2), 3);
		PutsString((char *)Str.c_str());
		PutsString("- ");
		PutsString(ProductParameters.Name);
		SetFontLetter(ENGLISH_LETTER);
		SetTextPixelPosition(10, 48);
		Str = IntegerToString(ProductParameters.Price, 7);
		PutsString((char *)Str.c_str());

		SetFontLetter(PERSIAN_LETTER);
		SetFont(Symbol_16);

		if(ProductCode > 2)
		{
			SetTextPixelPosition(230, 14);
			PutChar(0x00);
		}

		if(ProductCode < (MAX_PRODUCT_CODE - 3))
		{
			SetTextPixelPosition(230, 47);
			PutChar(0x01);
		}

//----------------------------------------------------------------------------------------------------------

		GLCD_Screen_Update();

//----------------------------------------------------------------------------------------------------------

		while(1)
		{
			Button = Panel_ReadPanel();

			if(Button == KEYPAD_UP)
			{
				if(ProductCode > 2)
					ProductCode -= 3;

				break;
			}
			else if(Button == KEYPAD_DOWN)
			{
				if(ProductCode < (MAX_PRODUCT_CODE - 3))
					ProductCode += 3;

				break;
			}
			else if(Button == KEYPAD_PRINT)
			{
				if(SystemFunction_PrintProductCode() != OK_PRINTER)
					ErrorSystemReport(E12_PRINTER);
				return;
			}
			else if(Button == KEYPAD_ESC)
				return;
		}
	}
}

//**********************************************************************************************************

Status_Printer_TypeDef SystemFunction_PrintProductCode(void)
{
	Status_Printer_TypeDef Status_Printer;
	ProductParameters_TypeDef ProductParameters = {0};

	while(1)
	{
		if(Printer_CheckPaper() == NO_PAPER_PRINTER)
		{
			GLCD_FillRectangle(10, 14, 229, 58, BLACK);
			GLCD_FillRectangle(12, 16, 227, 56, WHITE);

			SetFontLetter(PERSIAN_LETTER);
			SetFontColor(BLACK);
			SetBackgroundColor(WHITE);
			LcdFontSpaceX(1);
			LcdFontSpaceY(0);
			SetFont(Font_Pe_12X11);
			SetTextPixelPosition(110, 27);
			PutsString("خطا");
			SetTextPixelPosition(87, 41);
			PutsString("عدم وجود کاغذ");
			GLCD_Screen_Update();

			if(Panel_ReadPanel() == KEYPAD_ESC)
				return NO_PAPER_PRINTER;
		}
		else
			break;
	}

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
	GLCD_FillRectangle((58 + 0), 43, 181, 50, WHITE);

	GLCD_Screen_Update();

//----------------------------------------------------------------------------------------------------------

	if(SystemParameters.PrintFormatParameters.PrintFont == 0)
		Printer_SelectPrintFont_01();
	else if(SystemParameters.PrintFormatParameters.PrintFont == 1)
		Printer_SelectPrintFont_02();
	else if(SystemParameters.PrintFormatParameters.PrintFont == 2)
		Printer_SelectPrintFont_03();
	else if(SystemParameters.PrintFormatParameters.PrintFont == 3)
		Printer_SelectPrintFont_04();

//----------------------------------------------------------------------------------------------------------

	Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

//----------------------------------------------------------------------------------------------------------

	for(int i = 0; i < MAX_PRODUCT_CODE; i++)
	{
		ProductParameters = ReadProductCode(i);

		if((string)ProductParameters.Name != "")
		{
			Status_Printer = Printer_SetHorizontalCursorPositioning(84);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_Backspace(3);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(i, 3).c_str());
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_Backspace(3);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintStringPersian(" : ");
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintStringPersian(ProductParameters.Name);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintStringPersian(" --- ");
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_Backspace(7);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(ProductParameters.Price, 7).c_str());
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			if(SystemParameters.PrintFormatParameters.PrintFont == 3)
			{
				Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
				if(Status_Printer != OK_PRINTER)
					return ERROR_PRINTER;
			}
		}

//----------------------------------------------------------------------------------------------------------

		while(1)
		{
			if(Printer_CheckPaper() == NO_PAPER_PRINTER)
			{
				GLCD_FillRectangle(10, 14, 229, 58, BLACK);
				GLCD_FillRectangle(12, 16, 227, 56, WHITE);

				SetFontLetter(PERSIAN_LETTER);
				SetFontColor(BLACK);
				SetBackgroundColor(WHITE);
				LcdFontSpaceX(1);
				LcdFontSpaceY(0);
				SetFont(Font_Pe_12X11);
				SetTextPixelPosition(110, 27);
				PutsString("خطا");
				SetTextPixelPosition(87, 41);
				PutsString("عدم وجود کاغذ");
				GLCD_Screen_Update();

				if(Panel_ReadPanel() == KEYPAD_ESC)
					return NO_PAPER_PRINTER;
			}
			else
				break;
		}

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
		GLCD_FillRectangle((58 + (i * ((float)124 / MAX_PRODUCT_CODE))), 43, 181, 50, WHITE);

		GLCD_Screen_Update();
	}

//----------------------------------------------------------------------------------------------------------

	Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

//----------------------------------------------------------------------------------------------------------

	if(SystemParameters.PrintFormatParameters.EjectSheet)
	{
		Status_Printer = Printer_SendCommand(PRINTER_EJECT_PAPER_COMMAND);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

//----------------------------------------------------------------------------------------------------------

	return OK_PRINTER;
}

//**********************************************************************************************************

void SystemFunction_ShowCustomerCode(void)
{
	PanelKeypadButton_TypeDef Button;
	CustomerParameters_TypeDef CustomerParameters = {0};
	string Str = "";
	uint16_t CustomerCode = 0;

	while(1)
	{
		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_9X8);
		SetTextPixelPosition(84, 2);
		PutsString("چاپ کد مشتری");

//----------------------------------------------------------------------------------------------------------

		SetFontLetter(PERSIAN_LETTER);
		SetFont(Font_Pe_12X11);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);

		CustomerParameters = ReadCustomerCode(CustomerCode);
		SetFontLetter(PERSIAN_LETTER);
		SetTextPixelPosition(5, 18);
		Str = IntegerToString(CustomerCode, 3);
		PutsString((char *)Str.c_str());
		PutsString("- ");
		PutsString(CustomerParameters.Name);

		CustomerParameters = ReadCustomerCode((CustomerCode + 1));
		SetFontLetter(PERSIAN_LETTER);
		SetTextPixelPosition(5, 33);
		Str = IntegerToString((CustomerCode + 1), 3);
		PutsString((char *)Str.c_str());
		PutsString("- ");
		PutsString(CustomerParameters.Name);

		CustomerParameters = ReadCustomerCode((CustomerCode + 2));
		SetFontLetter(PERSIAN_LETTER);
		SetTextPixelPosition(5, 48);
		Str = IntegerToString((CustomerCode + 2), 3);
		PutsString((char *)Str.c_str());
		PutsString("- ");
		PutsString(CustomerParameters.Name);

		SetFontLetter(PERSIAN_LETTER);
		SetFont(Symbol_16);

		if(CustomerCode > 2)
		{
			SetTextPixelPosition(230, 14);
			PutChar(0x00);
		}

		if(CustomerCode < (MAX_CUSTOMER_CODE - 3))
		{
			SetTextPixelPosition(230, 47);
			PutChar(0x01);
		}

//----------------------------------------------------------------------------------------------------------

		GLCD_Screen_Update();

//----------------------------------------------------------------------------------------------------------

		while(1)
		{
			Button = Panel_ReadPanel();

			if(Button == KEYPAD_UP)
			{
				if(CustomerCode > 2)
					CustomerCode -= 3;

				break;
			}
			else if(Button == KEYPAD_DOWN)
			{
				if(CustomerCode < (MAX_CUSTOMER_CODE - 3))
					CustomerCode += 3;

				break;
			}
			else if(Button == KEYPAD_PRINT)
			{
				if(SystemFunction_PrintCustomerCode() != OK_PRINTER)
					ErrorSystemReport(E12_PRINTER);
				return;
			}
			else if(Button == KEYPAD_ESC)
				return;
		}
	}
}

//**********************************************************************************************************

Status_Printer_TypeDef SystemFunction_PrintCustomerCode(void)
{
	Status_Printer_TypeDef Status_Printer;
	CustomerParameters_TypeDef CustomerParameters = {0};

	while(1)
	{
		if(Printer_CheckPaper() == NO_PAPER_PRINTER)
		{
			GLCD_FillRectangle(10, 14, 229, 58, BLACK);
			GLCD_FillRectangle(12, 16, 227, 56, WHITE);

			SetFontLetter(PERSIAN_LETTER);
			SetFontColor(BLACK);
			SetBackgroundColor(WHITE);
			LcdFontSpaceX(1);
			LcdFontSpaceY(0);
			SetFont(Font_Pe_12X11);
			SetTextPixelPosition(110, 27);
			PutsString("خطا");
			SetTextPixelPosition(87, 41);
			PutsString("عدم وجود کاغذ");
			GLCD_Screen_Update();

			if(Panel_ReadPanel() == KEYPAD_ESC)
				return NO_PAPER_PRINTER;
		}
		else
			break;
	}

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
	GLCD_FillRectangle((58 + 0), 43, 181, 50, WHITE);

	GLCD_Screen_Update();

//----------------------------------------------------------------------------------------------------------

	if(SystemParameters.PrintFormatParameters.PrintFont == 0)
		Printer_SelectPrintFont_01();
	else if(SystemParameters.PrintFormatParameters.PrintFont == 1)
		Printer_SelectPrintFont_02();
	else if(SystemParameters.PrintFormatParameters.PrintFont == 2)
		Printer_SelectPrintFont_03();
	else if(SystemParameters.PrintFormatParameters.PrintFont == 3)
		Printer_SelectPrintFont_04();

//----------------------------------------------------------------------------------------------------------

	Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

//----------------------------------------------------------------------------------------------------------

	for(int i = 0; i < MAX_CUSTOMER_CODE; i++)
	{
		CustomerParameters = ReadCustomerCode(i);

		if((string)CustomerParameters.Name != "")
		{
			Status_Printer = Printer_SetHorizontalCursorPositioning(84);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_Backspace(3);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintNumbersPersian((char *)IntegerToString(i, 3).c_str());
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_Backspace(3);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintStringPersian(" : ");
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintStringPersian(CustomerParameters.Name);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			if(SystemParameters.PrintFormatParameters.PrintFont == 3)
			{
				Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
				if(Status_Printer != OK_PRINTER)
					return ERROR_PRINTER;
			}
		}

//----------------------------------------------------------------------------------------------------------

		while(1)
		{
			if(Printer_CheckPaper() == NO_PAPER_PRINTER)
			{
				GLCD_FillRectangle(10, 14, 229, 58, BLACK);
				GLCD_FillRectangle(12, 16, 227, 56, WHITE);

				SetFontLetter(PERSIAN_LETTER);
				SetFontColor(BLACK);
				SetBackgroundColor(WHITE);
				LcdFontSpaceX(1);
				LcdFontSpaceY(0);
				SetFont(Font_Pe_12X11);
				SetTextPixelPosition(110, 27);
				PutsString("خطا");
				SetTextPixelPosition(87, 41);
				PutsString("عدم وجود کاغذ");
				GLCD_Screen_Update();

				if(Panel_ReadPanel() == KEYPAD_ESC)
					return NO_PAPER_PRINTER;
			}
			else
				break;
		}

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
		GLCD_FillRectangle((58 + (i * ((float)124 / MAX_CUSTOMER_CODE))), 43, 181, 50, WHITE);

		GLCD_Screen_Update();
	}

//----------------------------------------------------------------------------------------------------------

	Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

//----------------------------------------------------------------------------------------------------------

	if(SystemParameters.PrintFormatParameters.EjectSheet)
	{
		Status_Printer = Printer_SendCommand(PRINTER_EJECT_PAPER_COMMAND);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

//----------------------------------------------------------------------------------------------------------

	return OK_PRINTER;
}

//**********************************************************************************************************

void SystemFunction_ShowEmptyWeightCertainTrucks(void)
{
	PanelKeypadButton_TypeDef Button;
	CertainTrucksParameters_TypeDef CertainTrucksParameters = {0};
	string Str = "";
	int16_t Index = 0;

	while(1)
	{
		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_9X8);
		SetTextPixelPosition(46, 2);
		PutsString("چاپ وزن خالی کامیونهای خاص");

//----------------------------------------------------------------------------------------------------------

		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);

		CertainTrucksParameters = ReadCertainTrucksParameters(Index);
		SetFontLetter(PERSIAN_LETTER);
		SetFont(Font_Pe_12X11);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetTextPixelPosition(5, 18);
		PutsString(CertainTrucksParameters.NumberPlates);
		SetFontLetter(ENGLISH_LETTER);
		SetFont(Font_En_Num_8X10);
		LcdFontSpaceX(0);
		LcdFontSpaceY(0);
		SetTextPixelPosition(1, 18);

		Str = WeightToString(CertainTrucksParameters.EmptyWeight,
							 SystemParameters.CalibrationParameters.ScaleSettingParameters.DISPDigits,
							 SystemParameters.CalibrationParameters.ScaleSettingParameters.DECPoint,
							 SystemParameters.CalibrationParameters.ScaleSettingParameters.LeadZBlank);

		PutsString((char *)Str.c_str());

		CertainTrucksParameters = ReadCertainTrucksParameters((Index + 1));
		SetFontLetter(PERSIAN_LETTER);
		SetFont(Font_Pe_12X11);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetTextPixelPosition(5, 33);
		PutsString(CertainTrucksParameters.NumberPlates);
		SetFontLetter(ENGLISH_LETTER);
		SetFont(Font_En_Num_8X10);
		LcdFontSpaceX(0);
		LcdFontSpaceY(0);
		SetTextPixelPosition(1, 33);

		Str = WeightToString(CertainTrucksParameters.EmptyWeight,
							 SystemParameters.CalibrationParameters.ScaleSettingParameters.DISPDigits,
							 SystemParameters.CalibrationParameters.ScaleSettingParameters.DECPoint,
							 SystemParameters.CalibrationParameters.ScaleSettingParameters.LeadZBlank);

		PutsString((char *)Str.c_str());

		CertainTrucksParameters = ReadCertainTrucksParameters((Index + 2));
		SetFontLetter(PERSIAN_LETTER);
		SetFont(Font_Pe_12X11);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetTextPixelPosition(5, 48);
		PutsString(CertainTrucksParameters.NumberPlates);
		SetFontLetter(ENGLISH_LETTER);
		SetFont(Font_En_Num_8X10);
		LcdFontSpaceX(0);
		LcdFontSpaceY(0);
		SetTextPixelPosition(1, 48);

		Str = WeightToString(CertainTrucksParameters.EmptyWeight,
							 SystemParameters.CalibrationParameters.ScaleSettingParameters.DISPDigits,
							 SystemParameters.CalibrationParameters.ScaleSettingParameters.DECPoint,
							 SystemParameters.CalibrationParameters.ScaleSettingParameters.LeadZBlank);

		PutsString((char *)Str.c_str());

		SetFontLetter(PERSIAN_LETTER);
		SetFont(Symbol_16);

		if(Index > 2)
		{
			SetTextPixelPosition(230, 14);
			PutChar(0x00);
		}

		if(Index < (MAX_CERTAIN_TRUCKS - 3))
		{
			SetTextPixelPosition(230, 47);
			PutChar(0x01);
		}

//----------------------------------------------------------------------------------------------------------

		GLCD_Screen_Update();

//----------------------------------------------------------------------------------------------------------

		while(1)
		{
			Button = Panel_ReadPanel();

			if(Button == KEYPAD_UP)
			{
				if(Index > 2)
					Index -= 3;

				break;
			}
			else if(Button == KEYPAD_DOWN)
			{
				if(Index < (MAX_CERTAIN_TRUCKS - 3))
					Index += 3;

				break;
			}
			else if(Button == KEYPAD_PRINT)
			{
				if(SystemFunction_PrintEmptyWeightCertainTrucks() != OK_PRINTER)
					ErrorSystemReport(E12_PRINTER);
				return;
			}
			else if(Button == KEYPAD_ESC)
				return;
		}
	}
}

//**********************************************************************************************************

Status_Printer_TypeDef SystemFunction_PrintEmptyWeightCertainTrucks(void)
{
	Status_Printer_TypeDef Status_Printer;
	CertainTrucksParameters_TypeDef CertainTrucksParameters = {0};

	while(1)
	{
		if(Printer_CheckPaper() == NO_PAPER_PRINTER)
		{
			GLCD_FillRectangle(10, 14, 229, 58, BLACK);
			GLCD_FillRectangle(12, 16, 227, 56, WHITE);

			SetFontLetter(PERSIAN_LETTER);
			SetFontColor(BLACK);
			SetBackgroundColor(WHITE);
			LcdFontSpaceX(1);
			LcdFontSpaceY(0);
			SetFont(Font_Pe_12X11);
			SetTextPixelPosition(110, 27);
			PutsString("خطا");
			SetTextPixelPosition(87, 41);
			PutsString("عدم وجود کاغذ");
			GLCD_Screen_Update();

			if(Panel_ReadPanel() == KEYPAD_ESC)
				return NO_PAPER_PRINTER;
		}
		else
			break;
	}

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
	GLCD_FillRectangle((58 + 0), 43, 181, 50, WHITE);

	GLCD_Screen_Update();

//----------------------------------------------------------------------------------------------------------

	if(SystemParameters.PrintFormatParameters.PrintFont == 0)
		Printer_SelectPrintFont_01();
	else if(SystemParameters.PrintFormatParameters.PrintFont == 1)
		Printer_SelectPrintFont_02();
	else if(SystemParameters.PrintFormatParameters.PrintFont == 2)
		Printer_SelectPrintFont_03();
	else if(SystemParameters.PrintFormatParameters.PrintFont == 3)
		Printer_SelectPrintFont_04();

//----------------------------------------------------------------------------------------------------------

	Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

//----------------------------------------------------------------------------------------------------------

	for(int i = 0; i < MAX_CERTAIN_TRUCKS; i++)
	{
		CertainTrucksParameters = ReadCertainTrucksParameters(i);

		if((string)CertainTrucksParameters.NumberPlates != "")
		{
			Status_Printer = Printer_SetHorizontalCursorPositioning(84);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintStringPersian(CertainTrucksParameters.NumberPlates);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintStringPersian(" --- ");
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_Backspace(10);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_PrintNumbersPersian((char *)WeightToString(CertainTrucksParameters.EmptyWeight,
								 SystemParameters.CalibrationParameters.ScaleSettingParameters.DISPDigits,
								 SystemParameters.CalibrationParameters.ScaleSettingParameters.DECPoint,
								 SystemParameters.CalibrationParameters.ScaleSettingParameters.LeadZBlank).c_str());
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			if(SystemParameters.PrintFormatParameters.PrintFont == 3)
			{
				Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
				if(Status_Printer != OK_PRINTER)
					return ERROR_PRINTER;
			}
		}

//----------------------------------------------------------------------------------------------------------

		while(1)
		{
			if(Printer_CheckPaper() == NO_PAPER_PRINTER)
			{
				GLCD_FillRectangle(10, 14, 229, 58, BLACK);
				GLCD_FillRectangle(12, 16, 227, 56, WHITE);

				SetFontLetter(PERSIAN_LETTER);
				SetFontColor(BLACK);
				SetBackgroundColor(WHITE);
				LcdFontSpaceX(1);
				LcdFontSpaceY(0);
				SetFont(Font_Pe_12X11);
				SetTextPixelPosition(110, 27);
				PutsString("خطا");
				SetTextPixelPosition(87, 41);
				PutsString("عدم وجود کاغذ");
				GLCD_Screen_Update();

				if(Panel_ReadPanel() == KEYPAD_ESC)
					return NO_PAPER_PRINTER;
			}
			else
				break;
		}

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
		GLCD_FillRectangle((58 + (i * ((float)124 / MAX_CERTAIN_TRUCKS))), 43, 181, 50, WHITE);

		GLCD_Screen_Update();
	}

//----------------------------------------------------------------------------------------------------------

	Status_Printer = Printer_SendCommand(PRINTER_LINE_FEED_COMMAND);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

//----------------------------------------------------------------------------------------------------------

	if(SystemParameters.PrintFormatParameters.EjectSheet)
	{
		Status_Printer = Printer_SendCommand(PRINTER_EJECT_PAPER_COMMAND);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

//----------------------------------------------------------------------------------------------------------

	return OK_PRINTER;
}

//**********************************************************************************************************

void SystemFunction_PrintFunction(void)
{
	PanelKeypadButton_TypeDef Button;

    while(1)
	{
		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_9X8);
		SetTextPixelPosition(88, 2);
		PutsString("چاپ )tnirP(");

//----------------------------------------------------------------------------------------------------------

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);

		SetFont(Font_Pe_12X11);
		SetTextPixelPosition(5, 18);
		PutsString("1-چاپ توزین کامیون خالی");
		SetTextPixelPosition(5, 33);
		PutsString("2-چاپ توزین کامیون پر");
		SetTextPixelPosition(5, 48);
		PutsString("3-چاپ مجدد قبض");

//----------------------------------------------------------------------------------------------------------

		GLCD_Screen_Update();

//----------------------------------------------------------------------------------------------------------

		Button = Panel_ReadPanel();

		if(Button == KEYPAD_01)
			SystemFunction_PrintWeight01Function();
		else if(Button == KEYPAD_02)
			SystemFunction_PrintWeight02Function();
		else if(Button == KEYPAD_03)
			SystemFunction_PrintWeightTicket();
		else if(Button == KEYPAD_ESC)
			return;
	}
}

//**********************************************************************************************************

void SystemFunction_PrintWeight01Function(void)
{
	PanelKeypadButton_TypeDef Button;
	PanelKeypadButton_TypeDef LastPressButton = NO_PUSH;
	WeighingInformation_TypeDef WeighingInformation = {0};
	uint8_t IndexPage = 0;
	string Str = "";
	bool IsAvailable = false;
	uint32_t LastTicketSerialNumber = 0;
	uint32_t EndTicketSerialNumber = 0;

	while(1)
	{
		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_9X8);
		SetTextPixelPosition(63, 2);
		PutsString("چاپ توزین کامیون خالی");

//----------------------------------------------------------------------------------------------------------

		GLCD_FillRectangle(38, 14, 202, 58, BLACK);
		GLCD_FillRectangle(40, 16, 200, 56, WHITE);

		SetFontLetter(PERSIAN_LETTER);
		SetFont(Font_Pe_12X11);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetTextPixelPosition(43, 31);
		PutsString("شماره پلاک : ");

		GLCD_FillRectangle(43, 29, 128, 43, BLACK);

		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		SetTextPixelPosition(115, 31);
		Str = GetStringCharacter(&LastPressButton, "", MAX_NUMBER_PLATES).c_str();

		if(LastPressButton == KEYPAD_ENTER)
		{
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
			GLCD_FillRectangle((58 + 0), 43, 181, 50, WHITE);

			GLCD_Screen_Update();
			LastTicketSerialNumber = ReadTicketSerialNumber();

			if(LastTicketSerialNumber > MAX_CHECK_EXIST_NUMBER_TRUCK)
				EndTicketSerialNumber = LastTicketSerialNumber - MAX_CHECK_EXIST_NUMBER_TRUCK;
			else
				EndTicketSerialNumber = 0;

			for(uint32_t i = LastTicketSerialNumber; i > EndTicketSerialNumber; i--)
			{		
			  	WeighingInformation = SystemFunction_ReadWeighingInformation(i);
				
				if((WeighingInformation.TicketSerialNumber == i) 
					&& (WeighingInformation.TypeWeighingInformation == 1)
					&& (Str == (string)WeighingInformation.NumberTruck))
				{
					IsAvailable = true;
					break;
				}
			}

			if(!IsAvailable)
			{
				GLCD_FillRectangle(38, 14, 202, 58, BLACK);
				GLCD_FillRectangle(40, 16, 200, 56, WHITE);

				SetFontLetter(PERSIAN_LETTER);
				SetFont(Font_Pe_12X11);
				SetFontColor(BLACK);
				SetBackgroundColor(WHITE);
				LcdFontSpaceX(1);
				LcdFontSpaceY(0);
				SetTextPixelPosition(75, 31);
				PutsString("عدم وجود در حافظه");
				GLCD_Screen_Update();
				DELAY_MS(3000);
				return;
			}
			
//----------------------------------------------------------------------------------------------------------

			while(1)
			{
				GLCD_Clear();
				GLCD_FillRectangle(0, 0, 239, 12, BLACK);

				SetFontLetter(PERSIAN_LETTER);
				SetFontColor(WHITE);
				SetBackgroundColor(BLACK);
				LcdFontSpaceX(1);
				LcdFontSpaceY(0);
				SetFont(Font_Pe_9X8);
				SetTextPixelPosition(63, 2);
				PutsString("چاپ توزین کامیون خالی");

//----------------------------------------------------------------------------------------------------------

				if(IndexPage == 0)
				{
					SetFontLetter(PERSIAN_LETTER);
					SetFont(Font_Pe_12X11);
					SetFontColor(BLACK);
					SetBackgroundColor(WHITE);
					LcdFontSpaceX(1);
					LcdFontSpaceY(0);
					SetTextPixelPosition(5, 14);
					PutsString("شماره پلاک : ");
					PutsString(WeighingInformation.NumberTruck);

					if(WeighingInformation.SecondWeight != 0)
					{
						SetFontLetter(PERSIAN_LETTER);
						SetTextPixelPosition(5, 26);
						PutsString("وزن پر");
						SetFontLetter(ENGLISH_LETTER);
						SetTextPixelPosition(70, 26);
						PutsString((char *)IntegerToString(WeighingInformation.SecondWeightDate.Year, 4).c_str());
						PutsString("/");
						PutsString((char *)IntegerToString(WeighingInformation.SecondWeightDate.Month, 2).c_str());
						PutsString("/");
						PutsString((char *)IntegerToString(WeighingInformation.SecondWeightDate.Date, 2).c_str());
						SetTextPixelPosition(150, 26);
						PutsString((char *)IntegerToString(WeighingInformation.SecondWeightTime.Hours, 2).c_str());
						PutsString(":");
						PutsString((char *)IntegerToString(WeighingInformation.SecondWeightTime.Minutes, 2).c_str());
						SetTextPixelPosition(60, 26);
						PutChar(':');
						SetTextPixelPosition(5, 26);
						PutsString((char *)WeightToString(WeighingInformation.SecondWeight,
												  WeighingInformation.DISPDigits,
												  WeighingInformation.DECPoint,
												  WeighingInformation.LeadZBlank).c_str());

						SetFontLetter(PERSIAN_LETTER);
						SetTextPixelPosition(5, 38);
						PutsString("وزن خالی");
						SetFontLetter(ENGLISH_LETTER);
						SetTextPixelPosition(70, 38);
						PutsString((char *)IntegerToString(WeighingInformation.FirstWeightDate.Year, 4).c_str());
						PutsString("/");
						PutsString((char *)IntegerToString(WeighingInformation.FirstWeightDate.Month, 2).c_str());
						PutsString("/");
						PutsString((char *)IntegerToString(WeighingInformation.FirstWeightDate.Date, 2).c_str());
						SetTextPixelPosition(150, 38);
						PutsString((char *)IntegerToString(WeighingInformation.FirstWeightTime.Hours, 2).c_str());
						PutsString(":");
						PutsString((char *)IntegerToString(WeighingInformation.FirstWeightTime.Minutes, 2).c_str());
						SetTextPixelPosition(60, 38);
						PutChar(':');
						SetTextPixelPosition(5, 38);
						PutsString((char *)WeightToString(WeighingInformation.FirstWeight,
												  WeighingInformation.DISPDigits,
												  WeighingInformation.DECPoint,
												  WeighingInformation.LeadZBlank).c_str());

						SetFontLetter(PERSIAN_LETTER);
						SetTextPixelPosition(5, 50);
						PutsString("وزن خالص");
						SetFontLetter(ENGLISH_LETTER);
						SetTextPixelPosition(60, 50);
						PutChar(':');
						SetTextPixelPosition(5, 50);
						PutsString((char *)WeightToString(WeighingInformation.NetWeight,
												  WeighingInformation.DISPDigits,
												  WeighingInformation.DECPoint,
												  WeighingInformation.LeadZBlank).c_str());
					}
					else
					{
						SetFontLetter(PERSIAN_LETTER);
						SetTextPixelPosition(5, 26);
						PutsString("وزن خالی");
						SetFontLetter(ENGLISH_LETTER);
						SetTextPixelPosition(70, 26);
						PutsString((char *)IntegerToString(WeighingInformation.FirstWeightDate.Year, 4).c_str());
						PutsString("/");
						PutsString((char *)IntegerToString(WeighingInformation.FirstWeightDate.Month, 2).c_str());
						PutsString("/");
						PutsString((char *)IntegerToString(WeighingInformation.FirstWeightDate.Date, 2).c_str());
						SetTextPixelPosition(150, 26);
						PutsString((char *)IntegerToString(WeighingInformation.FirstWeightTime.Hours, 2).c_str());
						PutsString(":");
						PutsString((char *)IntegerToString(WeighingInformation.FirstWeightTime.Minutes, 2).c_str());
						SetTextPixelPosition(60, 26);
						PutChar(':');
						SetTextPixelPosition(5, 26);
						PutsString((char *)WeightToString(WeighingInformation.FirstWeight,
												  WeighingInformation.DISPDigits,
												  WeighingInformation.DECPoint,
												  WeighingInformation.LeadZBlank).c_str());
					}

					SetFont(Symbol_16);
					SetTextPixelPosition(234, 47);
					PutChar(0x01);
				}
				else if(IndexPage == 1)
				{
					SetFontLetter(PERSIAN_LETTER);
					SetFont(Font_Pe_12X11);
					SetFontColor(BLACK);
					SetBackgroundColor(WHITE);
					LcdFontSpaceX(1);
					LcdFontSpaceY(0);
					SetTextPixelPosition(5, 14);
					PutsString("نام کالا : ");
					PutsString(WeighingInformation.ProductParameters.Name);

					SetFontLetter(PERSIAN_LETTER);
					SetTextPixelPosition(5, 26);
					PutsString("نام مشتری : ");
					PutsString(WeighingInformation.CustomerParameters.Name);

					SetFontLetter(PERSIAN_LETTER);
					SetTextPixelPosition(5, 38);
					PutsString("حق توزین");
					SetFontLetter(ENGLISH_LETTER);
					SetTextPixelPosition(180, 38);
					PutChar(':');
					SetTextPixelPosition(125, 38);
					PutsString((char *)IntegerToString(WeighingInformation.WeighingCost, 7).c_str());

					SetFontLetter(PERSIAN_LETTER);
					SetTextPixelPosition(5, 50);
					PutsString("شماره قبض");
					SetFontLetter(ENGLISH_LETTER);
					SetTextPixelPosition(180, 50);
					PutChar(':');
					SetTextPixelPosition(139, 50);
					PutsString((char *)IntegerToString(WeighingInformation.TicketSerialNumber, 5).c_str());

					SetFontLetter(ENGLISH_LETTER);
					SetTextPixelPosition(5, 38);
					PutsString("Enter->Save");
					SetTextPixelPosition(5, 50);
					PutsString("Print->Print");

					SetFont(Symbol_16);
					SetTextPixelPosition(234, 14);
					PutChar(0x00);
				}

//----------------------------------------------------------------------------------------------------------

				GLCD_Screen_Update();

//----------------------------------------------------------------------------------------------------------

				Button = Panel_ReadPanel();
				
				if(Button == KEYPAD_DOWN)
				{
					if(IndexPage < 1)
						IndexPage++;
				}
				else if(Button == KEYPAD_UP)
				{
					if(IndexPage > 0)
						IndexPage--;
				}
				else if(Button == KEYPAD_PRINT)
				{
					SystemFunction_PrintWeightTicket(WeighingInformation);
					return;
				}
				else if(Button == KEYPAD_ESC)
					return;
			}
		}
		else if(LastPressButton == KEYPAD_ESC)
			return;
	}
}

//**********************************************************************************************************

void SystemFunction_PrintWeight02Function(void)
{
	PanelKeypadButton_TypeDef Button;
	PanelKeypadButton_TypeDef LastPressButton = NO_PUSH;
	WeighingInformation_TypeDef WeighingInformation = {0};
	uint8_t IndexPage = 0;
	string Str = "";
	bool IsAvailable = false;
	uint32_t LastTicketSerialNumber = 0;
	uint32_t EndTicketSerialNumber = 0;

	while(1)
	{
		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_9X8);
		SetTextPixelPosition(70, 2);
		PutsString("چاپ توزین کامیون پر");

//----------------------------------------------------------------------------------------------------------

		GLCD_FillRectangle(38, 14, 202, 58, BLACK);
		GLCD_FillRectangle(40, 16, 200, 56, WHITE);

		SetFontLetter(PERSIAN_LETTER);
		SetFont(Font_Pe_12X11);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetTextPixelPosition(43, 31);
		PutsString("شماره پلاک : ");

		GLCD_FillRectangle(43, 29, 128, 43, BLACK);

		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		SetTextPixelPosition(115, 31);
		Str = GetStringCharacter(&LastPressButton, "", MAX_NUMBER_PLATES).c_str();

		if(LastPressButton == KEYPAD_ENTER)
		{
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
			GLCD_FillRectangle((58 + 0), 43, 181, 50, WHITE);

			GLCD_Screen_Update();

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
			GLCD_FillRectangle((58 + 62), 43, 181, 50, WHITE);

			GLCD_Screen_Update();
			LastTicketSerialNumber = ReadTicketSerialNumber();

			if(LastTicketSerialNumber > MAX_CHECK_EXIST_NUMBER_TRUCK)
				EndTicketSerialNumber = LastTicketSerialNumber - MAX_CHECK_EXIST_NUMBER_TRUCK;
			else
				EndTicketSerialNumber = 0;

			for(uint32_t i = LastTicketSerialNumber; i > EndTicketSerialNumber; i--)
			{		
			  	WeighingInformation = SystemFunction_ReadWeighingInformation(i);
				
				if((WeighingInformation.TicketSerialNumber == i) 
					&& (WeighingInformation.TypeWeighingInformation == 2)
					&& (Str == (string)WeighingInformation.NumberTruck))
				{
					IsAvailable = true;
					break;
				}
			}

			if(!IsAvailable)
			{
				GLCD_FillRectangle(38, 14, 202, 58, BLACK);
				GLCD_FillRectangle(40, 16, 200, 56, WHITE);

				SetFontLetter(PERSIAN_LETTER);
				SetFont(Font_Pe_12X11);
				SetFontColor(BLACK);
				SetBackgroundColor(WHITE);
				LcdFontSpaceX(1);
				LcdFontSpaceY(0);
				SetTextPixelPosition(75, 31);
				PutsString("عدم وجود در حافظه");
				GLCD_Screen_Update();
				DELAY_MS(3000);
				return;
			}

//----------------------------------------------------------------------------------------------------------

			while(1)
			{
				GLCD_Clear();
				GLCD_FillRectangle(0, 0, 239, 12, BLACK);

				SetFontLetter(PERSIAN_LETTER);
				SetFontColor(WHITE);
				SetBackgroundColor(BLACK);
				LcdFontSpaceX(1);
				LcdFontSpaceY(0);
				SetFont(Font_Pe_9X8);
				SetTextPixelPosition(70, 2);
				PutsString("چاپ توزین کامیون پر");

//----------------------------------------------------------------------------------------------------------

				if(IndexPage == 0)
				{
					SetFontLetter(PERSIAN_LETTER);
					SetFont(Font_Pe_12X11);
					SetFontColor(BLACK);
					SetBackgroundColor(WHITE);
					LcdFontSpaceX(1);
					LcdFontSpaceY(0);
					SetTextPixelPosition(5, 14);
					PutsString("شماره پلاک : ");
					PutsString(WeighingInformation.NumberTruck);

					SetFontLetter(PERSIAN_LETTER);
					SetTextPixelPosition(5, 26);
					PutsString("وزن پر");
					SetFontLetter(ENGLISH_LETTER);
					SetTextPixelPosition(70, 26);
					PutsString((char *)IntegerToString(WeighingInformation.SecondWeightDate.Year, 4).c_str());
					PutsString("/");
					PutsString((char *)IntegerToString(WeighingInformation.SecondWeightDate.Month, 2).c_str());
					PutsString("/");
					PutsString((char *)IntegerToString(WeighingInformation.SecondWeightDate.Date, 2).c_str());
					SetTextPixelPosition(150, 26);
					PutsString((char *)IntegerToString(WeighingInformation.SecondWeightTime.Hours, 2).c_str());
					PutsString(":");
					PutsString((char *)IntegerToString(WeighingInformation.SecondWeightTime.Minutes, 2).c_str());
					SetTextPixelPosition(60, 26);
					PutChar(':');
					SetTextPixelPosition(5, 26);
					PutsString((char *)WeightToString(WeighingInformation.SecondWeight,
											  WeighingInformation.DISPDigits,
											  WeighingInformation.DECPoint,
											  WeighingInformation.LeadZBlank).c_str());

					if(WeighingInformation.FirstWeight != 0)
					{	
						SetFontLetter(PERSIAN_LETTER);
						SetTextPixelPosition(5, 38);
						
						if(WeighingInformation.TypeFirstWeight == 1)
							PutsString("وزن خالی کامیون خاص");
						else
						{
							PutsString("وزن خالی");
							SetFontLetter(ENGLISH_LETTER);
							SetTextPixelPosition(70, 38);
							PutsString((char *)IntegerToString(WeighingInformation.FirstWeightDate.Year, 4).c_str());
							PutsString("/");
							PutsString((char *)IntegerToString(WeighingInformation.FirstWeightDate.Month, 2).c_str());
							PutsString("/");
							PutsString((char *)IntegerToString(WeighingInformation.FirstWeightDate.Date, 2).c_str());
							SetTextPixelPosition(150, 38);
							PutsString((char *)IntegerToString(WeighingInformation.FirstWeightTime.Hours, 2).c_str());
							PutsString(":");
							PutsString((char *)IntegerToString(WeighingInformation.FirstWeightTime.Minutes, 2).c_str());
						}
						
						SetFontLetter(ENGLISH_LETTER);
						SetTextPixelPosition(60, 38);
						PutChar(':');
						SetTextPixelPosition(5, 38);
						PutsString((char *)WeightToString(WeighingInformation.FirstWeight,
												  WeighingInformation.DISPDigits,
												  WeighingInformation.DECPoint,
												  WeighingInformation.LeadZBlank).c_str());

						SetFontLetter(PERSIAN_LETTER);
						SetTextPixelPosition(5, 50);
						PutsString("وزن خالص");
						SetFontLetter(ENGLISH_LETTER);
						SetTextPixelPosition(60, 50);
						PutChar(':');
						SetTextPixelPosition(5, 50);
						PutsString((char *)WeightToString(WeighingInformation.NetWeight,
												  WeighingInformation.DISPDigits,
												  WeighingInformation.DECPoint,
												  WeighingInformation.LeadZBlank).c_str());
					}
					
					SetFont(Symbol_16);
					SetTextPixelPosition(234, 47);
					PutChar(0x01);
				}
				else if(IndexPage == 1)
				{
					SetFontLetter(PERSIAN_LETTER);
					SetFont(Font_Pe_12X11);
					SetFontColor(BLACK);
					SetBackgroundColor(WHITE);
					LcdFontSpaceX(1);
					LcdFontSpaceY(0);
					SetTextPixelPosition(5, 14);
					PutsString("نام کالا : ");
					PutsString(WeighingInformation.ProductParameters.Name);

					SetFontLetter(PERSIAN_LETTER);
					SetTextPixelPosition(5, 26);
					PutsString("نام مشتری : ");
					PutsString(WeighingInformation.CustomerParameters.Name);

					SetFontLetter(PERSIAN_LETTER);
					SetTextPixelPosition(5, 38);
					PutsString("حق توزین");
					SetFontLetter(ENGLISH_LETTER);
					SetTextPixelPosition(180, 38);
					PutChar(':');
					SetTextPixelPosition(125, 38);
					PutsString((char *)IntegerToString(WeighingInformation.WeighingCost, 7).c_str());

					SetFontLetter(PERSIAN_LETTER);
					SetTextPixelPosition(5, 50);
					PutsString("شماره قبض");
					SetFontLetter(ENGLISH_LETTER);
					SetTextPixelPosition(180, 50);
					PutChar(':');
					SetTextPixelPosition(139, 50);
					PutsString((char *)IntegerToString(WeighingInformation.TicketSerialNumber, 5).c_str());

					SetFontLetter(ENGLISH_LETTER);
					SetTextPixelPosition(5, 38);
					PutsString("Enter->Save");
					SetTextPixelPosition(5, 50);
					PutsString("Print->Print");

					SetFont(Symbol_16);
					SetTextPixelPosition(234, 14);
					PutChar(0x00);
				}

//----------------------------------------------------------------------------------------------------------

				GLCD_Screen_Update();

//----------------------------------------------------------------------------------------------------------

				Button = Panel_ReadPanel();
				
				if(Button == KEYPAD_DOWN)
				{
					if(IndexPage < 1)
						IndexPage++;
				}
				else if(Button == KEYPAD_UP)
				{
					if(IndexPage > 0)
						IndexPage--;
				}
				else if(Button == KEYPAD_PRINT)
				{
					SystemFunction_PrintWeightTicket(WeighingInformation);
					return;
				}
				else if(Button == KEYPAD_ESC)
					return;
			}
		}
		else if(LastPressButton == KEYPAD_ESC)
			return;
	}
}

//**********************************************************************************************************

void SystemFunction_PrintWeightTicket(void)
{
	PanelKeypadButton_TypeDef Button;
	PanelKeypadButton_TypeDef LastPressButton = NO_PUSH;
	WeighingInformation_TypeDef WeighingInformation = {0};
	uint32_t TicketSerialNumber = ReadTicketSerialNumber() - 1;
	uint8_t IndexPage = 0;
	string Str = "";

	while(1)
	{
		GLCD_Clear();
		GLCD_FillRectangle(0, 0, 239, 12, BLACK);

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetFont(Font_Pe_9X8);
		SetTextPixelPosition(85, 2);
		PutsString("چاپ مجدد قبض");

//----------------------------------------------------------------------------------------------------------

		GLCD_FillRectangle(38, 14, 202, 58, BLACK);
		GLCD_FillRectangle(40, 16, 200, 56, WHITE);

		SetFontLetter(PERSIAN_LETTER);
		SetFont(Font_Pe_12X11);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);
		SetTextPixelPosition(68, 31);
		PutsString("شماره قبض : ");

		GLCD_FillRectangle(70, 30, 107, 41, BLACK);
		SetFontLetter(ENGLISH_LETTER);
		SetFont(Font_En_Num_8X10);
		LcdFontSpaceX(0);
		LcdFontSpaceY(0);
		SetFontColor(WHITE);
		SetBackgroundColor(BLACK);
		SetTextPixelPosition(72, 31);

		Str = GetStringInteger(&LastPressButton, IntegerToString(TicketSerialNumber, 5));

		if(LastPressButton == KEYPAD_ENTER)
		{
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
			GLCD_FillRectangle((58 + 0), 43, 181, 50, WHITE);

			GLCD_Screen_Update();

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
			GLCD_FillRectangle((58 + 62), 43, 181, 50, WHITE);

			GLCD_Screen_Update();

			TicketSerialNumber = atol(Str.c_str());
			WeighingInformation = SystemFunction_ReadWeighingInformation(TicketSerialNumber);

			if(WeighingInformation.TicketSerialNumber != TicketSerialNumber)
			{
				GLCD_FillRectangle(38, 14, 202, 58, BLACK);
				GLCD_FillRectangle(40, 16, 200, 56, WHITE);

				SetFontLetter(PERSIAN_LETTER);
				SetFont(Font_Pe_12X11);
				SetFontColor(BLACK);
				SetBackgroundColor(WHITE);
				LcdFontSpaceX(1);
				LcdFontSpaceY(0);
				SetTextPixelPosition(75, 31);
				PutsString("عدم وجود در حافظه");
				GLCD_Screen_Update();
				DELAY_MS(3000);
				return;
			}

//----------------------------------------------------------------------------------------------------------

			while(1)
			{
				GLCD_Clear();
				GLCD_FillRectangle(0, 0, 239, 12, BLACK);

				SetFontLetter(PERSIAN_LETTER);
				SetFontColor(WHITE);
				SetBackgroundColor(BLACK);
				LcdFontSpaceX(1);
				LcdFontSpaceY(0);
				SetFont(Font_Pe_9X8);
				SetTextPixelPosition(85, 2);
				PutsString("چاپ مجدد قبض");

//----------------------------------------------------------------------------------------------------------

				if(IndexPage == 0)
				{
					SetFontLetter(PERSIAN_LETTER);
					SetFont(Font_Pe_12X11);
					SetFontColor(BLACK);
					SetBackgroundColor(WHITE);
					LcdFontSpaceX(1);
					LcdFontSpaceY(0);
					SetTextPixelPosition(5, 14);
					PutsString("شماره پلاک : ");
					PutsString(WeighingInformation.NumberTruck);

					if(WeighingInformation.SecondWeight == 0)
					{
						SetFontLetter(PERSIAN_LETTER);
						SetTextPixelPosition(5, 26);
						PutsString("وزن خالی");
						SetFontLetter(ENGLISH_LETTER);
						SetTextPixelPosition(70, 26);
						PutsString((char *)IntegerToString(WeighingInformation.FirstWeightDate.Year, 4).c_str());
						PutsString("/");
						PutsString((char *)IntegerToString(WeighingInformation.FirstWeightDate.Month, 2).c_str());
						PutsString("/");
						PutsString((char *)IntegerToString(WeighingInformation.FirstWeightDate.Date, 2).c_str());
						SetTextPixelPosition(150, 26);
						PutsString((char *)IntegerToString(WeighingInformation.FirstWeightTime.Hours, 2).c_str());
						PutsString(":");
						PutsString((char *)IntegerToString(WeighingInformation.FirstWeightTime.Minutes, 2).c_str());
						SetTextPixelPosition(60, 26);
						PutChar(':');
						SetTextPixelPosition(5, 26);
						PutsString((char *)WeightToString(WeighingInformation.FirstWeight,
												  WeighingInformation.DISPDigits,
												  WeighingInformation.DECPoint,
												  WeighingInformation.LeadZBlank).c_str());
					}
					else if(WeighingInformation.FirstWeight == 0)
					{
						SetFontLetter(PERSIAN_LETTER);
						SetTextPixelPosition(5, 26);
						PutsString("وزن پر");
						SetFontLetter(ENGLISH_LETTER);
						SetTextPixelPosition(70, 26);
						PutsString((char *)IntegerToString(WeighingInformation.SecondWeightDate.Year, 4).c_str());
						PutsString("/");
						PutsString((char *)IntegerToString(WeighingInformation.SecondWeightDate.Month, 2).c_str());
						PutsString("/");
						PutsString((char *)IntegerToString(WeighingInformation.SecondWeightDate.Date, 2).c_str());
						SetTextPixelPosition(150, 26);
						PutsString((char *)IntegerToString(WeighingInformation.SecondWeightTime.Hours, 2).c_str());
						PutsString(":");
						PutsString((char *)IntegerToString(WeighingInformation.SecondWeightTime.Minutes, 2).c_str());
						SetTextPixelPosition(60, 26);
						PutChar(':');
						SetTextPixelPosition(5, 26);
						PutsString((char *)WeightToString(WeighingInformation.SecondWeight,
												  WeighingInformation.DISPDigits,
												  WeighingInformation.DECPoint,
												  WeighingInformation.LeadZBlank).c_str());
					}
					else
					{
						SetFontLetter(PERSIAN_LETTER);
						SetTextPixelPosition(5, 26);
						PutsString("وزن پر");
						SetFontLetter(ENGLISH_LETTER);
						SetTextPixelPosition(70, 26);
						PutsString((char *)IntegerToString(WeighingInformation.SecondWeightDate.Year, 4).c_str());
						PutsString("/");
						PutsString((char *)IntegerToString(WeighingInformation.SecondWeightDate.Month, 2).c_str());
						PutsString("/");
						PutsString((char *)IntegerToString(WeighingInformation.SecondWeightDate.Date, 2).c_str());
						SetTextPixelPosition(150, 26);
						PutsString((char *)IntegerToString(WeighingInformation.SecondWeightTime.Hours, 2).c_str());
						PutsString(":");
						PutsString((char *)IntegerToString(WeighingInformation.SecondWeightTime.Minutes, 2).c_str());
						SetTextPixelPosition(60, 26);
						PutChar(':');
						SetTextPixelPosition(5, 26);
						PutsString((char *)WeightToString(WeighingInformation.SecondWeight,
												  WeighingInformation.DISPDigits,
												  WeighingInformation.DECPoint,
												  WeighingInformation.LeadZBlank).c_str());

						SetFontLetter(PERSIAN_LETTER);
						SetTextPixelPosition(5, 38);
						
						if(WeighingInformation.TypeFirstWeight == 1)
							PutsString("وزن خالی کامیون خاص");
						else
						{
							PutsString("وزن خالی");
							SetFontLetter(ENGLISH_LETTER);
							SetTextPixelPosition(70, 38);
							PutsString((char *)IntegerToString(WeighingInformation.FirstWeightDate.Year, 4).c_str());
							PutsString("/");
							PutsString((char *)IntegerToString(WeighingInformation.FirstWeightDate.Month, 2).c_str());
							PutsString("/");
							PutsString((char *)IntegerToString(WeighingInformation.FirstWeightDate.Date, 2).c_str());
							SetTextPixelPosition(150, 38);
							PutsString((char *)IntegerToString(WeighingInformation.FirstWeightTime.Hours, 2).c_str());
							PutsString(":");
							PutsString((char *)IntegerToString(WeighingInformation.FirstWeightTime.Minutes, 2).c_str());
						}
						
						SetFontLetter(ENGLISH_LETTER);
						SetTextPixelPosition(60, 38);
						PutChar(':');
						SetTextPixelPosition(5, 38);
						PutsString((char *)WeightToString(WeighingInformation.FirstWeight,
												  WeighingInformation.DISPDigits,
												  WeighingInformation.DECPoint,
												  WeighingInformation.LeadZBlank).c_str());

						SetFontLetter(PERSIAN_LETTER);
						SetTextPixelPosition(5, 50);
						PutsString("وزن خالص");
						SetFontLetter(ENGLISH_LETTER);
						SetTextPixelPosition(60, 50);
						PutChar(':');
						SetTextPixelPosition(5, 50);
						PutsString((char *)WeightToString(WeighingInformation.NetWeight,
												  WeighingInformation.DISPDigits,
												  WeighingInformation.DECPoint,
												  WeighingInformation.LeadZBlank).c_str());
					}
					
					SetFont(Symbol_16);
					SetTextPixelPosition(234, 47);
					PutChar(0x01);
				}
				else if(IndexPage == 1)
				{
					SetFontLetter(PERSIAN_LETTER);
					SetFont(Font_Pe_12X11);
					SetFontColor(BLACK);
					SetBackgroundColor(WHITE);
					LcdFontSpaceX(1);
					LcdFontSpaceY(0);
					SetTextPixelPosition(5, 14);
					PutsString("نام کالا : ");
					PutsString(WeighingInformation.ProductParameters.Name);

					SetFontLetter(PERSIAN_LETTER);
					SetTextPixelPosition(5, 26);
					PutsString("نام مشتری : ");
					PutsString(WeighingInformation.CustomerParameters.Name);

					SetFontLetter(PERSIAN_LETTER);
					SetTextPixelPosition(5, 38);
					PutsString("حق توزین");
					SetFontLetter(ENGLISH_LETTER);
					SetTextPixelPosition(180, 38);
					PutChar(':');
					SetTextPixelPosition(125, 38);
					PutsString((char *)IntegerToString(WeighingInformation.WeighingCost, 7).c_str());

					SetFontLetter(PERSIAN_LETTER);
					SetTextPixelPosition(5, 50);
					PutsString("شماره قبض");
					SetFontLetter(ENGLISH_LETTER);
					SetTextPixelPosition(180, 50);
					PutChar(':');
					SetTextPixelPosition(139, 50);
					PutsString((char *)IntegerToString(WeighingInformation.TicketSerialNumber, 5).c_str());

					SetFontLetter(ENGLISH_LETTER);
					SetTextPixelPosition(5, 38);
					PutsString("Enter->Save");
					SetTextPixelPosition(5, 50);
					PutsString("Print->Print");

					SetFont(Symbol_16);
					SetTextPixelPosition(234, 14);
					PutChar(0x00);
				}

//----------------------------------------------------------------------------------------------------------

				GLCD_Screen_Update();

//----------------------------------------------------------------------------------------------------------

				Button = Panel_ReadPanel();
				
				if(Button == KEYPAD_DOWN)
				{
					if(IndexPage < 1)
						IndexPage++;
				}
				else if(Button == KEYPAD_UP)
				{
					if(IndexPage > 0)
						IndexPage--;
				}
				else if(Button == KEYPAD_LEFT)
				{
					if(TicketSerialNumber > 1)
					{
						TicketSerialNumber--;
						WeighingInformation = SystemFunction_ReadWeighingInformation(TicketSerialNumber);		
					}
				}
				else if(Button == KEYPAD_RIGHT)
				{
					if(TicketSerialNumber < (ReadTicketSerialNumber() - 1))
					{
						TicketSerialNumber++;
						WeighingInformation = SystemFunction_ReadWeighingInformation(TicketSerialNumber);		
					}
				}
				else if(Button == KEYPAD_PRINT)
				{
					SystemFunction_PrintWeightTicket(WeighingInformation);
					return;
				}
				else if(Button == KEYPAD_ESC)
					return;
			}
		}
		else if(LastPressButton == KEYPAD_ESC)
			return;
	}
}

//**********************************************************************************************************

Status_Printer_TypeDef SystemFunction_Thermal_PrinterPrintWeightTicket(WeighingInformation_TypeDef WeighingInformation)
{
	Status_Printer_TypeDef Status_Printer;

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
	GLCD_FillRectangle((58 + 62), 43, 181, 50, WHITE);

	GLCD_Screen_Update();

//----------------------------------------------------------------------------------------------------------

	Thermal_PrinterInitialize();

//----------------------------------------------------------------------------------------------------------

	Thermal_Printerfeed(SystemParameters.PrintFormatParameters.TopLineEmpty);

//----------------------------------------------------------------------------------------------------------

	if(SystemParameters.PrintFormatParameters.PrintTitle)
	{
		Thermal_PrinterJustify('C');
		Thermal_PrinterSelectPrintFont_02();

		Status_Printer = Thermal_PrinterPrintStringPersian(SystemParameters.PrintFormatParameters.Title_01[0]);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Thermal_Printerfeed(1);

		 Status_Printer = Thermal_PrinterPrintStringPersian(SystemParameters.PrintFormatParameters.Title_01[1]);
		 if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		 Thermal_Printerfeed(1);
	}

//----------------------------------------------------------------------------------------------------------

	Thermal_PrinterJustify('R');
	Thermal_PrinterSelectPrintFont_02();

	Status_Printer = Thermal_PrinterPrintNumbersPersian((char *)IntegerToString(WeighingInformation.TicketSerialNumber, 5).c_str());
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Thermal_PrinterPrintStringPersian("شماره قبض: ");
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Thermal_Printerfeed(1);

	Status_Printer = Thermal_PrinterPrintStringPersian(WeighingInformation.NumberTruck);
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Status_Printer = Thermal_PrinterPrintStringPersian("شماره پلاک: ");
	if(Status_Printer != OK_PRINTER)
		return ERROR_PRINTER;

	Thermal_Printerfeed(1);

//----------------------------------------------------------------------------------------------------------

	Thermal_PrinterJustify('C');
	Thermal_PrinterSelectPrintFont_03();

	for(int i = 0; i < 16; i++)
	{
		Status_Printer = Thermal_PrinterPrintCharacterPersian('_');
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;
	}

	Thermal_Printerfeed(1);

//----------------------------------------------------------------------------------------------------------

	if(WeighingInformation.FirstWeight != 0)
	{
		Thermal_PrinterJustify('C');
		Thermal_PrinterSelectPrintFont_01();

		Status_Printer = Thermal_PrinterPrintNumbersPersian((char *)IntegerToString(WeighingInformation.FirstWeightDate.Year, 4).c_str());
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Thermal_PrinterPrintCharacterPersian('/');
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Thermal_PrinterPrintNumbersPersian((char *)IntegerToString(WeighingInformation.FirstWeightDate.Month, 2).c_str());
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Thermal_PrinterPrintCharacterPersian('/');
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Thermal_PrinterPrintNumbersPersian((char *)IntegerToString(WeighingInformation.FirstWeightDate.Date, 2).c_str());
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Thermal_PrinterPrintStringPersian("            ");
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Thermal_PrinterPrintNumbersPersian((char *)IntegerToString(WeighingInformation.FirstWeightTime.Hours, 2).c_str());
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Thermal_PrinterPrintCharacterPersian(':');
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Thermal_PrinterPrintNumbersPersian((char *)IntegerToString(WeighingInformation.FirstWeightTime.Minutes, 2).c_str());
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Thermal_Printerfeed(1);
		Thermal_PrinterJustify('C');
		Thermal_PrinterSelectPrintFont_03();

		Status_Printer = Thermal_PrinterPrintNumbersPersian((char *)WeightToString(WeighingInformation.FirstWeight, WeighingInformation.DISPDigits, WeighingInformation.DECPoint, WeighingInformation.LeadZBlank).c_str());
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		if(WeighingInformation.TypeFirstWeight == 1)
		{
			Status_Printer = Thermal_PrinterPrintCharacterPersian('*');
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}

		Status_Printer = Thermal_PrinterPrintStringPersian("gK ");
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Thermal_PrinterPrintStringPersian("خالی: ");
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Thermal_Printerfeed(1);
		Thermal_PrinterJustify('C');
		Thermal_PrinterSelectPrintFont_03();

		for(int i = 0; i < 16; i++)
		{
			Status_Printer = Thermal_PrinterPrintCharacterPersian('_');
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}

		Thermal_Printerfeed(1);
	}

//----------------------------------------------------------------------------------------------------------

	if(WeighingInformation.SecondWeight != 0)
	{
		Thermal_PrinterJustify('C');
		Thermal_PrinterSelectPrintFont_01();

		Status_Printer = Thermal_PrinterPrintNumbersPersian((char *)IntegerToString(WeighingInformation.SecondWeightDate.Year, 4).c_str());
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Thermal_PrinterPrintCharacterPersian('/');
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Thermal_PrinterPrintNumbersPersian((char *)IntegerToString(WeighingInformation.SecondWeightDate.Month, 2).c_str());
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Thermal_PrinterPrintCharacterPersian('/');
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Thermal_PrinterPrintNumbersPersian((char *)IntegerToString(WeighingInformation.SecondWeightDate.Date, 2).c_str());
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Thermal_PrinterPrintStringPersian("            ");
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Thermal_PrinterPrintNumbersPersian((char *)IntegerToString(WeighingInformation.SecondWeightTime.Hours, 2).c_str());
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Thermal_PrinterPrintCharacterPersian(':');
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Thermal_PrinterPrintNumbersPersian((char *)IntegerToString(WeighingInformation.SecondWeightTime.Minutes, 2).c_str());
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Thermal_Printerfeed(1);
		Thermal_PrinterJustify('C');
		Thermal_PrinterSelectPrintFont_03();

		Status_Printer = Thermal_PrinterPrintNumbersPersian((char *)WeightToString(WeighingInformation.SecondWeight, WeighingInformation.DISPDigits, WeighingInformation.DECPoint, WeighingInformation.LeadZBlank).c_str());
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Thermal_PrinterPrintStringPersian("gK ");
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Thermal_PrinterPrintStringPersian("پر: ");
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Thermal_Printerfeed(1);

		if(strlen(WeighingInformation.ProductParameters.Name) != 0)
		{
			Thermal_PrinterJustify('R');
			Thermal_PrinterSelectPrintFont_02();

			Status_Printer = Thermal_PrinterPrintStringPersian(WeighingInformation.ProductParameters.Name);
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Thermal_PrinterPrintStringPersian("کالا: ");
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Thermal_Printerfeed(1);
		}

		Thermal_PrinterJustify('C');
		Thermal_PrinterSelectPrintFont_03();

		for(int i = 0; i < 16; i++)
		{
			Status_Printer = Thermal_PrinterPrintCharacterPersian('_');
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}

		Thermal_Printerfeed(1);
	}

//----------------------------------------------------------------------------------------------------------

	if(WeighingInformation.NetWeight != 0)
	{
		Thermal_PrinterJustify('C');
		Thermal_PrinterSelectPrintFont_03();

		Status_Printer = Thermal_PrinterPrintNumbersPersian((char *)WeightToString(WeighingInformation.NetWeight, WeighingInformation.DISPDigits, WeighingInformation.DECPoint, WeighingInformation.LeadZBlank).c_str());
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Thermal_PrinterPrintStringPersian("gK ");
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Thermal_PrinterPrintStringPersian("خالص: ");
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Thermal_Printerfeed(1);

		if(WeighingInformation.ProductPrice != 0)
		{
			Thermal_PrinterJustify('C');
			Thermal_PrinterSelectPrintFont_02();

			Status_Printer = Thermal_PrinterPrintStringPersian(" تومان");
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Thermal_PrinterPrintNumbersPersian((char *)IntegerToString(WeighingInformation.ProductPrice, 1).c_str());
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Status_Printer = Thermal_PrinterPrintStringPersian("قیمت :");
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;

			Thermal_Printerfeed(1);
		}

		Thermal_PrinterJustify('C');
		Thermal_PrinterSelectPrintFont_03();

		for(int i = 0; i < 16; i++)
		{
			Status_Printer = Thermal_PrinterPrintCharacterPersian('_');
			if(Status_Printer != OK_PRINTER)
				return ERROR_PRINTER;
		}

		Thermal_Printerfeed(1);
	}

//----------------------------------------------------------------------------------------------------------

	Thermal_PrinterJustify('R');
	Thermal_PrinterSelectPrintFont_02();

	if(strlen(WeighingInformation.CustomerParameters.Name) != 0)
	{
		Status_Printer = Thermal_PrinterPrintStringPersian(WeighingInformation.CustomerParameters.Name);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Thermal_PrinterPrintStringPersian("نام مشتری: ");
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Thermal_Printerfeed(1);
	}

	if(WeighingInformation.WeighingCost != 0)
	{
		Status_Printer = Thermal_PrinterPrintNumbersPersian((char *)IntegerToString(WeighingInformation.WeighingCost, 7, false).c_str());
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Status_Printer = Thermal_PrinterPrintStringPersian("حق توزین)تومان(: ");
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Thermal_Printerfeed(1);
	}

//----------------------------------------------------------------------------------------------------------

	if(SystemParameters.PrintFormatParameters.PrintTitle)
	{
		Thermal_PrinterJustify('C');
		Thermal_PrinterSelectPrintFont_02();

		Status_Printer = Thermal_PrinterPrintStringPersian(SystemParameters.PrintFormatParameters.Title_02[0]);
		if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		Thermal_Printerfeed(1);

		 Status_Printer = Thermal_PrinterPrintStringPersian(SystemParameters.PrintFormatParameters.Title_02[1]);
		 if(Status_Printer != OK_PRINTER)
			return ERROR_PRINTER;

		 Thermal_Printerfeed(1);
	}

//----------------------------------------------------------------------------------------------------------

	Thermal_Printerfeed(SystemParameters.PrintFormatParameters.BottomLineEmpty);

//----------------------------------------------------------------------------------------------------------

	return OK_PRINTER;
}

//**********************************************************************************************************






