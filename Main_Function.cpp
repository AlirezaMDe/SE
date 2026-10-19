/*-----------------------------------------------------------------------------------------------------------
@file          	: Main_Function.cpp
@author  		: Reza Farahani (nmmco2015@gmail.com)
C++ source 2024
-----------------------------------------------------------------------------------------------------------*/

#include "Main_Function.h"
#include "GLCD_TextFunction.h"
#include "General_Functions.hpp"
#include "System_Function.hpp"
#include "Printer.hpp"
#include "Panel.hpp"

//**********************************************************************************************************

extern TIM_HandleTypeDef htim2;
extern RTC_HandleTypeDef hrtc;

//**********************************************************************************************************
//**********************************************************************************************************
//**********************************************************************************************************

osMessageQueueId_t KeyboardUSB_QueueHandle;
const osMessageQueueAttr_t KeyboardUSB_Queue_attributes = {
  .name = "KeyboardUSB_Queue"
};

osMessageQueueId_t ConfigParametersScale_01_QueueHandle;
const osMessageQueueAttr_t ConfigParametersScale_01_Queue_attributes = {
  .name = "ConfigParametersScale_01_Queue"
};

osMessageQueueId_t ConfigParametersScale_02_QueueHandle;
const osMessageQueueAttr_t ConfigParametersScale_02_Queue_attributes = {
  .name = "ConfigParametersScale_02_Queue"
};

osMessageQueueId_t RespanceScale_01_QueueHandle;
const osMessageQueueAttr_t RespanceScale_01_Queue_attributes = {
	.name = "RespanceScale_01_Queue"
};

osMessageQueueId_t RespanceScale_02_QueueHandle;
const osMessageQueueAttr_t RespanceScale_02_Queue_attributes = {
	.name = "RespanceScale_02_Queue"
};

//**********************************************************************************************************

void CreateQueuesWeightDisplay(void)
{
	KeyboardUSB_QueueHandle = osMessageQueueNew (1, sizeof(Keyboard_Info_TypeDef), &KeyboardUSB_Queue_attributes);
	ConfigParametersScale_01_QueueHandle = osMessageQueueNew (1, sizeof(ConfigParametersScale_TypeDef), &ConfigParametersScale_01_Queue_attributes);
	ConfigParametersScale_02_QueueHandle = osMessageQueueNew (1, sizeof(ConfigParametersScale_TypeDef), &ConfigParametersScale_02_Queue_attributes);
	RespanceScale_01_QueueHandle = osMessageQueueNew (3, sizeof(RespanceScale_TypeDef), &RespanceScale_01_Queue_attributes);
	RespanceScale_02_QueueHandle = osMessageQueueNew (3, sizeof(RespanceScale_TypeDef), &RespanceScale_02_Queue_attributes);
}

//**********************************************************************************************************
//**********************************************************************************************************
//**********************************************************************************************************

void Timer2_Function(void)
{
	CountAutoPowerOff++;
	if((SystemFunction_GetScaleSettingParameters().AutoPowerOff != 0) && (CountAutoPowerOff >= (SystemFunction_GetScaleSettingParameters().AutoPowerOff * 60)))
		LCD_BLK_RESET;
}

//**********************************************************************************************************
//**********************************************************************************************************
//**********************************************************************************************************

void WeightDisplay_TestFunction(void)
{

}

//**********************************************************************************************************

void WeightDisplay_MainFunction(void)
{
	WeightDisplay_Initalize();
	WeightDisplay_ShowInformation();
	WeightDisplay_TestFunction();
	SystemFunction_AutoZeroAtPowerUp();
	WeightDisplay_BasePageFunction();
}

//**********************************************************************************************************

void WeightDisplay_Initalize(void)
{
	GLCD_Initialization();
	GLCD_Clear();

//----------------------------------------------------------------------------------------------------------

	BUZZER_SET;
	DELAY_MS(500);
	BUZZER_RESET;
	DELAY_MS(1000);

//----------------------------------------------------------------------------------------------------------

	SystemFunction_InitializeBackupMemory();

//----------------------------------------------------------------------------------------------------------

	SystemFunction_InitializeSystemParameters();

//----------------------------------------------------------------------------------------------------------

	HAL_TIM_Base_Start_IT(&htim2);
}

//**********************************************************************************************************

void WeightDisplay_ShowInformation(void)
{
//	GLCD_Clear();
//	GLCD_DrawBitmap(Logo_Image, 21, 0,219, 64);
//	GLCD_Screen_Update();
//	DELAY_MS(3000);

//----------------------------------------------------------------------------------------------------------

	GLCD_Clear();
	SetFontLetter(ENGLISH_LETTER);
	SetFontColor(BLACK);
	SetBackgroundColor(WHITE);
	LcdFontSpaceX(1);
	LcdFontSpaceY(0);

	SetFont(Font_En_Verdana_16X16);
	SetTextPixelPosition(5, 5);
	PutsString((char*)SYSTEM_MODEL);
	SetTextPixelPosition(5, 25);
	PutsString((char*)SYSTEM_VERSION);
	SetTextPixelPosition(5, 45);
	PutsString((char*)SYSTEM_SERIAL_NUMBER);

	GLCD_Screen_Update();
	DELAY_MS(3000);

//----------------------------------------------------------------------------------------------------------

	GLCD_Clear();
}

//**********************************************************************************************************

void WeightDisplay_BasePageFunction(void)
{
	PanelKeypadButton_TypeDef Button;
	PanelKeypadButton_TypeDef LastPressButton = NO_PUSH;
	WeightOperation_TypeDef Operation = DISPLAY_WEIGHT;
	WeightScale_TypeDef WeightScale = {0};
	uint32_t Stable_LastTickCount = 0;
	bool HighResolution = false;
	uint32_t DisplayExternal_LastTickCount = 0;
	uint8_t DisplayExternal_Index = 0;
	uint32_t TextWidth;
	string Str;
	uint8_t PixelPositionWeekDay;
	RTC_DateTypeDef sDate = {0};

//----------------------------------------------------------------------------------------------------------

	GLCD_Clear();

//----------------------------------------------------------------------------------------------------------

	SystemFunction_StartReadScale();

//----------------------------------------------------------------------------------------------------------

	while(1)
	{
//----------------------------------------------------------------------------------------------------------

		Button = Panel_ReadPanel();

		if(Button == KEYPAD_MENU)
		{
			WeightDisplay_MenuFunction();
			SystemFunction_StartReadScale();
			continue;
		}
		else if((Button != NO_PUSH) && (GetErrorSystem() == NON_ERROR))
		{
			switch((uint8_t)Button)
			{
				case KEYPAD_00:
				{
					Operation = ZERO_WEIGHT;
					break;
				}
				case KEYPAD_01:
				{
					HighResolution = !HighResolution;
					break;
				}
				case KEYPAD_SET:
				{
					Operation = PERSIANIZING_PRINTER;
					break;
				}
				case KEYPAD_TARE:
				{
					Operation = TARE_WEIGHT;
					break;
				}
				case KEYPAD_PTARE:
				{
					Operation = PRESET_TARE;
					break;
				}
				case KEYPAD_WEIGH_01:
				{
					Operation = PRINT_WEIGHT_01;
					break;
				}
				case KEYPAD_WEIGH_02:
				{
					Operation = PRINT_WEIGHT_02;
					break;
				}
				case KEYPAD_PRINT:
				{
					Operation = PRINTING;
					break;
				}
				case KEYPAD_ESC:
				{
					SystemFunction_UnTaringScale();
					break;
				}
			}
		}

//----------------------------------------------------------------------------------------------------------

		SystemFunction_GetWeightScale(&WeightScale);

//----------------------------------------------------------------------------------------------------------

		if((!WeightScale.IsError) && (WeightScale.HighResolution != 0) && (!WeightScale.IsUnderRange) && (!WeightScale.IsOverRange) && (!WeightScale.IsNet) && (WeightScale.IsStable))
		{
			if((osKernelGetTickCount() - Stable_LastTickCount) >= 5000)
				SystemFunction_AutoZeroTracking();
		}
		else
			Stable_LastTickCount = osKernelGetTickCount();

//----------------------------------------------------------------------------------------------------------

		if((osKernelGetTickCount() - DisplayExternal_LastTickCount) >= 200)
		{
			if(DisplayExternal_Index == 0)
				SystemFunction_SendWeightToDisplayExternal01(WeightScale);
			else if(DisplayExternal_Index == 1)
				SystemFunction_SendWeightToDisplayExternal02(WeightScale);

			if(++DisplayExternal_Index > 1)
				DisplayExternal_Index = 0;

			DisplayExternal_LastTickCount = osKernelGetTickCount();
		}

//----------------------------------------------------------------------------------------------------------

		switch(Operation)
		{
			case DISPLAY_WEIGHT:
			{
				GLCD_Clear();

				SetFontLetter(PERSIAN_LETTER);
				SetFont(Font_Pe_14X16);
				SetFontColor(BLACK);
				SetBackgroundColor(WHITE);
				LcdFontSpaceX(1);
				LcdFontSpaceY(0);
				SetTextPixelPosition(10, 50);
				PutsString((char *)GetStringPersianTimeSystem(true).c_str());

				HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
					
				switch(sDate.WeekDay)
				{
					case RTC_WEEKDAY_MONDAY:
					{
						PixelPositionWeekDay = 94;
						Str = "دوشنبه";
						break;
					}
					case RTC_WEEKDAY_TUESDAY:
					{
						PixelPositionWeekDay = 90;
						Str = "سه شنبه";
						break;
					}
					case RTC_WEEKDAY_WEDNESDAY:
					{
						PixelPositionWeekDay = 86;
						Str = "چهارشنبه";
						break;
					}
					case RTC_WEEKDAY_THURSDAY:
					{
						PixelPositionWeekDay = 90;
						Str = "پنج شنبه";
						break;
					}
					case RTC_WEEKDAY_FRIDAY:
					{
						PixelPositionWeekDay = 99;
						Str = "جمعه";
						break;
					}
					case RTC_WEEKDAY_SATURDAY:
					{
						PixelPositionWeekDay = 101;
						Str = "شنبه";
						break;
					}
					case RTC_WEEKDAY_SUNDAY:
					{
						PixelPositionWeekDay = 92;
						Str = "یک شنبه";
						break;
					}
				}	
				
				SetFont(Font_Pe_13X12);
				SetTextPixelPosition(PixelPositionWeekDay, 52);
				PutsString((char *)Str.c_str());
			
				SetFont(Font_Pe_14X16);
				Str = GetStringPersianDateSystem();
				TextWidth = CalcTextWidth((char *)Str.c_str());
				SetTextPixelPosition((229 - TextWidth), 50);
				PutsString((char *)Str.c_str());

				SystemFunction_ShowWeightScale(&LastPressButton, WeightScale, HighResolution);

				if(LastPressButton == KEYPAD_MENU)
				{
					WeightDisplay_MenuFunction();
					SystemFunction_StartReadScale();
					continue;
				}

				break;
			}
			case ZERO_WEIGHT:
			{
				if(SystemFunction_ZeroingScale() == OK_ZERO_TRACK)
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
					DELAY_MS(1000);
				}

				Operation = DISPLAY_WEIGHT;
				break;
			}
			case TARE_WEIGHT:
			{
				if(WeightScale.IsStable)
					SystemFunction_TaringScale();

				Operation = DISPLAY_WEIGHT;
				break;
			}
			case PERSIANIZING_PRINTER:
			{
				if(Printer_Initialize() != OK_PRINTER)
					 ErrorSystemReport(E12_PRINTER);

				Operation = DISPLAY_WEIGHT;
				break;
			}
			case PRESET_TARE:
			{
				if(WeightScale.IsNet)
					SystemFunction_UnTaringScale();

				Operation = DISPLAY_WEIGHT;
				break;
			}
			case PRINT_WEIGHT_01:
			{
				if(WeightScale.IsStable)
				{
					if(WeightScale.Normally < 0)
						ErrorSystemReport(E11_NEGATIVE_WEIGHTS);
					else if(WeightScale.IsNet)
						ErrorSystemReport(E10_SCALE_TARED);
					else
						SystemFunction_PrintWeight01Function(WeightScale);
				}

				Operation = DISPLAY_WEIGHT;
				break;
			}
			case PRINT_WEIGHT_02:
			{
				if(WeightScale.IsStable)
				{
					if(WeightScale.Normally < 0)
						ErrorSystemReport(E11_NEGATIVE_WEIGHTS);
					else if(WeightScale.IsNet)
						ErrorSystemReport(E10_SCALE_TARED);
					else
						SystemFunction_PrintWeight02Function(WeightScale);
				}

				Operation = DISPLAY_WEIGHT;
				break;
			}
			case PRINTING:
			{
				SystemFunction_PrintFunction();
				Operation = DISPLAY_WEIGHT;
				break;
			}
			default:
			{
				Operation = DISPLAY_WEIGHT;
				break;
			}
		}

		GLCD_Screen_Update();
	}
}

//**********************************************************************************************************

void WeightDisplay_MenuFunction(void)
{
	PanelKeypadButton_TypeDef Button;
	PanelKeypadButton_TypeDef LastPressButton;
	string String_Password;
	uint8_t ParameterNumber = 0;
	uint8_t IndexPage = 0;

    while(1)
	{
		GLCD_Clear();

//----------------------------------------------------------------------------------------------------------

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);

		if(IndexPage == 0)
		{
			SetFont(Font_Pe_12X11);
			SetTextPixelPosition(5, 2);
			PutsString("1-تنظیم تاریخ )etaD(");
			SetTextPixelPosition(5, 18);
			PutsString("2-تنظیم ساعت )emiT(");
			SetTextPixelPosition(5, 34);
			PutsString("3-پیکربندی )noitarugifnoC(");
			SetTextPixelPosition(5, 50);
			PutsString("4-کالیبراسیون )noitarbilaC(");
			SetFont(Symbol_16);
			SetTextPixelPosition(230, 47);
			PutChar(0x01);
		}
		else if(IndexPage == 1)
		{
			SetFont(Font_Pe_12X11);
			SetTextPixelPosition(5, 2);
			PutsString("5-تغییر پسورد سیستم )ssaP metsyS(");
			SetTextPixelPosition(5, 18);
			PutsString("6-تنظیمات پورت سریال 1 )232SR(");
			SetTextPixelPosition(5, 34);
			PutsString("7-تنظیمات پورت سریال 2 )485SR(");
			SetTextPixelPosition(5, 50);
			PutsString("8-فرمت چاپ )tamroF tnirP(");
			SetFont(Symbol_16);
			SetTextPixelPosition(230, 1);
			PutChar(0x00);
			SetTextPixelPosition(230, 47);
			PutChar(0x01);
		}
		else if(IndexPage == 2)
		{
			SetFont(Font_Pe_12X11);
			SetTextPixelPosition(5, 2);
			PutsString("9-حافظه )yromeM(");
			SetFont(Symbol_16);
			SetTextPixelPosition(230, 1);
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
		else if(Button == KEYPAD_ESC)
			return;

//----------------------------------------------------------------------------------------------------------

		if((IndexPage == 0) && ((Button == KEYPAD_01)))
		{
			SystemFunction_SetDate();
			ParameterNumber = 0;
		}
		else if((IndexPage == 0) && ((Button == KEYPAD_02)))
		{
			SystemFunction_SetTime();
			ParameterNumber = 0;
		}
		else if((IndexPage == 0) && ((Button == KEYPAD_03)))
		{
			String_Password = SystemFunction_GetPassword(&LastPressButton);

			if(LastPressButton == KEYPAD_ENTER)
			{
				if((String_Password == SystemFunction_GetPinSystem()) || (String_Password == BREAK_PASSWORD))
					SystemFunction_SetScaleSettingParameters();
				else
					ShowFillPassword();
			}

			ParameterNumber = 0;
		}
		else if((IndexPage == 0) && ((Button == KEYPAD_04)))
		{
			String_Password = SystemFunction_GetPassword(&LastPressButton);

			if(LastPressButton == KEYPAD_ENTER)
			{
				if((String_Password == SystemFunction_GetPinSystem()) || (String_Password == BREAK_PASSWORD))
					SystemFunction_CalibrationScale();
				else
					ShowFillPassword();
			}

			ParameterNumber = 0;
		}
		else if((IndexPage == 1) && ((Button == KEYPAD_05)))
		{
			String_Password = SystemFunction_GetPassword(&LastPressButton);

			if(LastPressButton == KEYPAD_ENTER)
			{
				if((String_Password == SystemFunction_GetPinSystem()) || (String_Password == BREAK_PASSWORD))
					SystemFunction_SetSystemPassword();
				else
					ShowFillPassword();
			}

			ParameterNumber = 0;
		}
		else if((IndexPage == 1) && ((Button == KEYPAD_06)))
		{
			SystemFunction_SetCommunicationParametersCOM1();
			SystemFunction_InitializeCommunicationParameters(GetCommunicationParameters_COM1());
			ParameterNumber = 0;
		}
		else if((IndexPage == 1) && ((Button == KEYPAD_07)))
		{
			SystemFunction_SetCommunicationParametersCOM2();
			SystemFunction_InitializeCommunicationParameters(GetCommunicationParameters_COM2());
			ParameterNumber = 0;
		}
		else if((IndexPage == 1) && ((Button == KEYPAD_08)))
		{
			SystemFunction_SetPrintFormat();
			ParameterNumber = 0;
		}
		else if((IndexPage == 2) && ((Button == KEYPAD_09)))
		{
			WeightDisplay_Memory_Function();
			ParameterNumber = 0;
		}
	}
}

//**********************************************************************************************************

void WeightDisplay_Memory_Function(void)
{
	PanelKeypadButton_TypeDef Button;
	uint8_t ParameterNumber = 0;
	uint8_t IndexPage = 0;

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
		SetTextPixelPosition(74, 2);
		PutsString("حافظه )yromeM(");

//----------------------------------------------------------------------------------------------------------

		SetFontLetter(PERSIAN_LETTER);
		SetFontColor(BLACK);
		SetBackgroundColor(WHITE);
		LcdFontSpaceX(1);
		LcdFontSpaceY(0);

		if(IndexPage == 0)
		{
			SetFont(Font_Pe_12X11);
			SetTextPixelPosition(5, 18);
			PutsString("1-تعریف کد کالا و قیمت واحد");
			SetTextPixelPosition(5, 33);
			PutsString("2-پاک کردن حافظه کد کالا و قیمت واحد");
			SetTextPixelPosition(5, 48);
			PutsString("3-چاپ کد کالا و قیمت واحد");
			SetFont(Symbol_16);
			SetTextPixelPosition(230, 47);
			PutChar(0x01);
		}
		else if(IndexPage == 1)
		{
			SetFont(Font_Pe_12X11);
			SetTextPixelPosition(5, 18);
			PutsString("4-تعریف کد مشتری");
			SetTextPixelPosition(5, 33);
			PutsString("5-پاک کردن حافظه کد مشتری");
			SetTextPixelPosition(5, 48);
			PutsString("6-چاپ کد مشتری");

			SetFont(Symbol_16);
			SetTextPixelPosition(230, 14);
			PutChar(0x00);
			SetTextPixelPosition(230, 47);
			PutChar(0x01);
		}
		else if(IndexPage == 2)
		{
			SetFont(Font_Pe_12X11);
			SetTextPixelPosition(5, 18);
			PutsString("7-تعریف وزن خالی کامیونهای خاص");
			SetTextPixelPosition(5, 33);
			PutsString("8-پاک کردن حافظه وزن خالی کامیونهای خاص");
			SetTextPixelPosition(5, 48);
			PutsString("9-چاپ وزن خالی کامیونهای خاص");

			SetFont(Symbol_16);
			SetTextPixelPosition(230, 14);
			PutChar(0x00);
			SetTextPixelPosition(230, 47);
			PutChar(0x01);
		}
		else if(IndexPage == 3)
		{
			SetFont(Font_Pe_12X11);
			SetTextPixelPosition(5, 18);
			PutsString("10-تعریف حق توزین");
			SetTextPixelPosition(5, 33);
			PutsString("11-پاک کردن حافظه توزین");

			SetFont(Symbol_16);
			SetTextPixelPosition(230, 14);
			PutChar(0x00);
		}

//----------------------------------------------------------------------------------------------------------

		GLCD_Screen_Update();

//----------------------------------------------------------------------------------------------------------

		Button = Panel_ReadPanel();

		if(Button == KEYPAD_DOWN)
		{
			if(IndexPage < 3)
				IndexPage++;
		}
		else if(Button == KEYPAD_UP)
		{
			if(IndexPage > 0)
				IndexPage--;
		}
		else if(Button == KEYPAD_ESC)
			return;

//----------------------------------------------------------------------------------------------------------

		if((IndexPage == 0) && ((Button == KEYPAD_01)))
		{
			SystemFunction_DefinitionProductCode();
			ParameterNumber = 0;
		}
		else if((IndexPage == 0) && ((Button == KEYPAD_02)))
		{
			SystemFunction_DeleteProductCode();
			ParameterNumber = 0;
		}
		else if((IndexPage == 0) && ((Button == KEYPAD_03)))
		{
			SystemFunction_ShowProductCode();
			ParameterNumber = 0;
		}
		else if((IndexPage == 1) && ((Button == KEYPAD_04)))
		{
			SystemFunction_DefinitionCustomerCode();
			ParameterNumber = 0;
		}
		else if((IndexPage == 1) && ((Button == KEYPAD_05)))
		{
			SystemFunction_DeleteCustomerCode();
			ParameterNumber = 0;
		}
		else if((IndexPage == 1) && ((Button == KEYPAD_06)))
		{
			SystemFunction_ShowCustomerCode();
			ParameterNumber = 0;
		}
		else if((IndexPage == 2) && ((Button == KEYPAD_07)))
		{
			SystemFunction_DefinitionEmptyWeightCertainTrucks();
			ParameterNumber = 0;
		}
		else if((IndexPage == 2) && ((Button == KEYPAD_08)))
		{
			SystemFunction_DeleteEmptyWeightCertainTrucks();
			ParameterNumber = 0;
		}
		else if((IndexPage == 2) && ((Button == KEYPAD_09)))
		{
			SystemFunction_ShowEmptyWeightCertainTrucks();
			ParameterNumber = 0;
		}
		else if((IndexPage == 3) && ((Button == KEYPAD_00)))
		{
			SystemFunction_DefinitionWeighingCost();
			ParameterNumber = 0;
		}
		else if((IndexPage == 3) && ((Button == KEYPAD_01)))
		{
			SystemFunction_DeleteWeightInformationMemory();
			ParameterNumber = 0;
		}
	}
}

//**********************************************************************************************************



