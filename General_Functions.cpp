/*-----------------------------------------------------------------------------------------------------------
 @file          : General_Functions.cpp
 @author  		: Reza Farahani (nmmco2015@gmail.com)
 C++ source 2024
 -----------------------------------------------------------------------------------------------------------*/

#include "General_Functions.hpp"
#include "GLCD_TextFunction.h"
#include "Date_Function.hpp"
#include "Panel.hpp"

//**********************************************************************************************************

extern RTC_HandleTypeDef hrtc;

//**********************************************************************************************************

ErrorList_TypeDef ErrorList;

//**********************************************************************************************************

string IntegerToString(int32_t Integer, uint8_t NumberOfDigits, bool FillZero)
{
	char StringInteger[20] = "";
	char Format[5] = "";
	char StringNumberOfDigits[3] = "";

	sprintf(StringNumberOfDigits, "%d", NumberOfDigits);
	strcat(Format, "%");

	if(FillZero)
		strcat(Format, "0");

	strcat(Format, StringNumberOfDigits);
	strcat(Format, "d");
	sprintf(StringInteger, Format, Integer);

	return StringInteger;
}

//**********************************************************************************************************

string FloatToString(float Float, uint8_t NumberOfDigits, uint8_t Decimal_Point, bool FillZero)
{
	char StringFloat[20] = "";
	char Format[8] = "";
	char StringNumberOfDigits[3] = "";
	char StringDecimal_Point[3] = "";

	sprintf(StringNumberOfDigits, "%d", NumberOfDigits);
	sprintf(StringDecimal_Point, "%d", Decimal_Point);
	strcat(Format, "%");

	if(FillZero)
		strcat(Format, "0");

	strcat(Format, StringNumberOfDigits);
	strcat(Format, ".");
	strcat(Format, StringDecimal_Point);
	strcat(Format, "f");
	sprintf(StringFloat, Format, Float);

	return StringFloat;
}

//**********************************************************************************************************

string WeightToString(int32_t Weight, uint8_t NumberOfDigits, uint8_t Decimal_Point, bool FillZero)
{
	string stringWeight = "";

	if(Decimal_Point >= NumberOfDigits)
		NumberOfDigits = 6;

	if(Decimal_Point > 0)
		FillZero = true;

	if(Weight >= 0)
		stringWeight = " ";
	else
		NumberOfDigits++;

	stringWeight += IntegerToString(Weight, NumberOfDigits, FillZero);

	if(Decimal_Point > 0)
	{
		if(Weight < 0)
			stringWeight.insert((NumberOfDigits - Decimal_Point),".", 1);
		else
			stringWeight.insert((NumberOfDigits - Decimal_Point + 1),".", 1);
	}

	return stringWeight;
}

//**********************************************************************************************************

string GetStringInteger(PanelKeypadButton_TypeDef *LastPressButton, const string StringNumberInput, bool IsPassword)
{
	PanelKeypadButton_TypeDef Button;
	Blinker_TypeDef Blinker;
	string StringNumber = StringNumberInput;
	uint8_t NumberOfDigits = StringNumberInput.length();
	uint8_t X;
	uint8_t Y;
	uint8_t Last_Char_X;
	uint8_t Last_Char_Y;

	GetTextPixelPosition(&X, &Y);
	Blinker.AmountBlinker = AMOUNT_BLINKER_GET_NUMBER;

	while(1)
	{
		Button = Panel_ReadPanel();

		if(Button != NO_PUSH)
		{
			switch((uint8_t)Button)
			{
				case KEYPAD_00:
				{
					StringNumber += "0";
					break;
				}
				case KEYPAD_01:
				{
					StringNumber += "1";
					break;
				}
				case KEYPAD_02:
				{
					StringNumber += "2";
					break;
				}
				case KEYPAD_03:
				{
					StringNumber += "3";
					break;
				}
				case KEYPAD_04:
				{
					StringNumber += "4";
					break;
				}
				case KEYPAD_05:
				{
					StringNumber += "5";
					break;
				}
				case KEYPAD_06:
				{
					StringNumber += "6";
					break;
				}
				case KEYPAD_07:
				{
					StringNumber += "7";
					break;
				}
				case KEYPAD_08:
				{
					StringNumber += "8";
					break;
				}
				case KEYPAD_09:
				{
					StringNumber += "9";
					break;
				}

				case KEYPAD_CE:
				{
					StringNumber.clear();
					break;
				}
				case KEYPAD_ESC:
				{
					*LastPressButton = KEYPAD_ESC;
					return StringNumberInput;
				}
				case KEYPAD_ENTER:
				case KEYPAD_UP:
				case KEYPAD_DOWN:
				{
					*LastPressButton = Button;
					return StringNumber;
				}
			}
		}

		if(StringNumber.length() > NumberOfDigits)
			StringNumber.erase(0,(StringNumber.length() - NumberOfDigits));

		if(IsPassword)
		{
			Blinker.String = "";
			Blinker.String.insert(0,"********************", StringNumber.length());
			Blinker.String.insert(0,"                    ", (NumberOfDigits - StringNumber.length()));
		}
		else
		{
			StringNumber.insert(0,"00000000000000000000", (NumberOfDigits - StringNumber.length()));
			Blinker.String = StringNumber;
		}

		Blinker.Counter++;
		if(Blinker.Counter > Blinker.AmountBlinker)
		{
			Blinker.String = "";
			Blinker.String.insert(0,"                    ", NumberOfDigits);

			if(Blinker.Counter > (1.5 * Blinker.AmountBlinker))
				Blinker.Counter = 0;
		}

		GetTextPixelPosition(&Last_Char_X, &Last_Char_Y);

		if(GetLetter() == PERSIAN_LETTER)
			GLCD_FillRectangle((GET_MAX_X() - Last_Char_X), Y, (GET_MAX_X() - X - 1), (Last_Char_Y + LcdFontHeight() - 1), GetbackgroundColor());
		else
			GLCD_FillRectangle(X, Y, (Last_Char_X - 1), (Last_Char_Y + LcdFontHeight() - 1), GetbackgroundColor());

		SetTextPixelPosition(X, Y);
		PutsString((char *)Blinker.String.c_str());
		GLCD_Screen_Update();
	}
}

//**********************************************************************************************************

string GetStringFloat(PanelKeypadButton_TypeDef *LastPressButton, const string StringFloatInput)
{
	PanelKeypadButton_TypeDef Button;
	Blinker_TypeDef Blinker;
	string StringFloat = StringFloatInput;
	uint8_t DotPosition = StringFloat.find(".");
	string StringNumber = StringFloat.erase(StringFloat.find("."), 1);
	uint8_t NumberOfDigits = StringNumber.length();
	uint8_t X;
	uint8_t Y;
	uint8_t Last_Char_X;
	uint8_t Last_Char_Y;

	GetTextPixelPosition(&X, &Y);
	Blinker.AmountBlinker = AMOUNT_BLINKER_GET_NUMBER;

	while(1)
	{
		Button = Panel_ReadPanel();

		if(Button != NO_PUSH)
		{
			switch((uint8_t)Button)
			{
				case KEYPAD_00:
				{
					StringNumber += "0";
					break;
				}
				case KEYPAD_01:
				{
					StringNumber += "1";
					break;
				}
				case KEYPAD_02:
				{
					StringNumber += "2";
					break;
				}
				case KEYPAD_03:
				{
					StringNumber += "3";
					break;
				}
				case KEYPAD_04:
				{
					StringNumber += "4";
					break;
				}
				case KEYPAD_05:
				{
					StringNumber += "5";
					break;
				}
				case KEYPAD_06:
				{
					StringNumber += "6";
					break;
				}
				case KEYPAD_07:
				{
					StringNumber += "7";
					break;
				}
				case KEYPAD_08:
				{
					StringNumber += "8";
					break;
				}
				case KEYPAD_09:
				{
					StringNumber += "9";
					break;
				}

				case KEYPAD_CE:
				{
					StringNumber.clear();
					break;
				}
				case KEYPAD_ESC:
				{
					*LastPressButton = KEYPAD_ESC;
					return StringFloatInput;
				}
				case KEYPAD_ENTER:
				case KEYPAD_UP:
				case KEYPAD_DOWN:
				{
					*LastPressButton = Button;
					return StringFloat;
				}
			}
		}

		if(StringNumber.length() > NumberOfDigits)
			StringNumber.erase(0, (StringNumber.length() - NumberOfDigits));

		StringNumber.insert(0,"00000000000000000000", (NumberOfDigits - StringNumber.length()));
		StringFloat = StringNumber;
		StringFloat.insert(DotPosition,"." , 1);
		Blinker.String = StringFloat;

		Blinker.Counter++;
		if(Blinker.Counter > Blinker.AmountBlinker)
		{
			Blinker.String = "";
			Blinker.String.insert(0,"                    ", (NumberOfDigits + 1));

			if(Blinker.Counter > (1.5 * Blinker.AmountBlinker))
				Blinker.Counter = 0;
		}

		GetTextPixelPosition(&Last_Char_X, &Last_Char_Y);

		if(GetLetter() == PERSIAN_LETTER)
			GLCD_FillRectangle((GET_MAX_X() - Last_Char_X), Y, (GET_MAX_X() - X - 1), (Last_Char_Y + LcdFontHeight() - 1), GetbackgroundColor());
		else
			GLCD_FillRectangle(X, Y, (Last_Char_X - 1), (Last_Char_Y + LcdFontHeight() - 1), GetbackgroundColor());

		SetTextPixelPosition(X, Y);
		PutsString((char *)Blinker.String.c_str());
		GLCD_Screen_Update();
	}
}

//**********************************************************************************************************

string GetStringWeight(PanelKeypadButton_TypeDef *LastPressButton, int32_t Weight, uint8_t NumberOfDigits, uint8_t Decimal_Point)
{
	PanelKeypadButton_TypeDef Button;
	Blinker_TypeDef Blinker;
	string StringWeight;
	uint8_t X;
	uint8_t Y;
	uint8_t Last_Char_X;
	uint8_t Last_Char_Y;

	GetTextPixelPosition(&X, &Y);
	StringWeight = IntegerToString(Weight, NumberOfDigits);
	Blinker.AmountBlinker = AMOUNT_BLINKER_GET_NUMBER;

	while(1)
	{
		Button = Panel_ReadPanel();

		if(Button != NO_PUSH)
		{
			switch((uint8_t)Button)
			{
				case KEYPAD_00:
				{
					StringWeight += "0";
					break;
				}
				case KEYPAD_01:
				{
					StringWeight += "1";
					break;
				}
				case KEYPAD_02:
				{
					StringWeight += "2";
					break;
				}
				case KEYPAD_03:
				{
					StringWeight += "3";
					break;
				}
				case KEYPAD_04:
				{
					StringWeight += "4";
					break;
				}
				case KEYPAD_05:
				{
					StringWeight += "5";
					break;
				}
				case KEYPAD_06:
				{
					StringWeight += "6";
					break;
				}
				case KEYPAD_07:
				{
					StringWeight += "7";
					break;
				}
				case KEYPAD_08:
				{
					StringWeight += "8";
					break;
				}
				case KEYPAD_09:
				{
					StringWeight += "9";
					break;
				}

				case KEYPAD_CE:
				{
					StringWeight.clear();
					break;
				}
				case KEYPAD_ESC:
				{
					*LastPressButton = KEYPAD_ESC;
					return IntegerToString(Weight, NumberOfDigits);
				}
				case KEYPAD_ENTER:
				case KEYPAD_UP:
				case KEYPAD_DOWN:
				{
					*LastPressButton = Button;
					return StringWeight;
				}
			}
		}

		if(StringWeight.length() > NumberOfDigits)
			StringWeight.erase(0, (StringWeight.length() - NumberOfDigits));

		StringWeight.insert(0,"00000000000000000000", (NumberOfDigits - StringWeight.length()));
		Blinker.String = StringWeight;

		if(Decimal_Point != 0)
			Blinker.String.insert((NumberOfDigits - Decimal_Point),"." , 1);

		Blinker.Counter++;
		if(Blinker.Counter > Blinker.AmountBlinker)
		{
			Blinker.String = "";
			if(Decimal_Point != 0)
				Blinker.String.insert(0,"                    ", (NumberOfDigits + 1));
			else
				Blinker.String.insert(0,"                    ", NumberOfDigits);

			if(Blinker.Counter > (1.5 * Blinker.AmountBlinker))
				Blinker.Counter = 0;
		}

		GetTextPixelPosition(&Last_Char_X, &Last_Char_Y);

		if(GetLetter() == PERSIAN_LETTER)
			GLCD_FillRectangle((GET_MAX_X() - Last_Char_X), Y, (GET_MAX_X() - X - 1), (Last_Char_Y + LcdFontHeight() - 1), GetbackgroundColor());
		else
			GLCD_FillRectangle(X, Y, (Last_Char_X - 1), (Last_Char_Y + LcdFontHeight() - 1), GetbackgroundColor());

		SetTextPixelPosition(X, Y);
		PutsString((char *)Blinker.String.c_str());
		GLCD_Screen_Update();
	}
}

//**********************************************************************************************************

string GetStringCharacter(PanelKeypadButton_TypeDef *LastPressButton, const string StringCharInput, uint8_t LengthString, uint8_t KeyInTime)
{
	PanelKeypadButton_TypeDef Button;
	PanelKeypadButton_TypeDef LastButton;
	Keyboard_Info_TypeDef Keyboard_Info;
	string StringChar = StringCharInput;
	string LastString = "";
	uint8_t KeypadButton = 0;
	uint8_t IndexString = 0;
	static uint32_t TickCount = 0;
	static uint32_t LastTickCount = 0;
	Blinker_TypeDef Blinker;
	uint8_t X;
	uint8_t Y;
	uint8_t Last_Char_X;
	uint8_t Last_Char_Y;
	bool Is_PushButton = false;

	GetTextPixelPosition(&X, &Y);
	Blinker.AmountBlinker = AMOUNT_BLINKER_GET_NUMBER;

	if(StringCharInput.length() > 0)
		IndexString = StringCharInput.length();

	while(1)
	{
		if(Is_PushButton)
		{
			TickCount = osKernelGetTickCount();
			if((TickCount - LastTickCount) >= (KeyInTime * 100))
			{
				if(IndexString < (LengthString - 2))
					IndexString += 2;

				Is_PushButton = false;
				KeypadButton = 0;
			}
		}

		Keyboard_Info = Panel_ScanKeyboard();		//ScanKeyboard

		if(Keyboard_Info.key != 0)
		{
			if(Is_PushButton)
			{
				if(IndexString < (LengthString - 2))
					IndexString += 2;

				Is_PushButton = false;
				KeypadButton = 0;
			}

			switch(Keyboard_Info.key)
			{
				case 42:		//Key_CE
				{
					if((IndexString > 0) && (StringChar.at((IndexString - 1)) <= 0x7F))
						StringChar.erase((--IndexString), 1);
					else if((IndexString > 1) && (StringChar.at((IndexString - 2)) > 0x7F))
					{
						StringChar.erase((--IndexString), 1);
						StringChar.erase((--IndexString), 1);
					}

					break;
				}
				case 41:		//Key_ESC
				{
					*LastPressButton = KEYPAD_ESC;
					return StringCharInput;
				}
				case 40:		//Key_ENTER
				case 88:
				{
					*LastPressButton = KEYPAD_ENTER;
					return StringChar;
				}
				case 53:		//Key_~
				{
					if(IndexString < (LengthString - 2))
					{
						StringChar.replace(IndexString++, 1, 1, 0xD9);
						StringChar.replace(IndexString++, 1, 1, 0xBE);
					}
					break;
				}
				case 49:		//Key_|
				{
					if(IndexString < (LengthString - 2))
					{
						StringChar.replace(IndexString++, 1, 1, 0xDA);
						StringChar.replace(IndexString++, 1, 1, 0x98);
					}
					break;
				}
				case 20:		//Key_Q
				{
					if(IndexString < (LengthString - 2))
					{
						StringChar.replace(IndexString++, 1, 1, 0xD8);
						StringChar.replace(IndexString++, 1, 1, 0xB6);
					}
					break;
				}
				case 26:		//Key_W
				{
					if(IndexString < (LengthString - 2))
					{
						StringChar.replace(IndexString++, 1, 1, 0xD8);
						StringChar.replace(IndexString++, 1, 1, 0xB5);
					}
					break;
				}
				case 8:		//Key_E
				{
					if(IndexString < (LengthString - 2))
					{
						StringChar.replace(IndexString++, 1, 1, 0xD8);
						StringChar.replace(IndexString++, 1, 1, 0xAB);
					}
					break;
				}
				case 21:		//Key_R
				{
					if(IndexString < (LengthString - 2))
					{
						StringChar.replace(IndexString++, 1, 1, 0xD9);
						StringChar.replace(IndexString++, 1, 1, 0x82);
					}
					break;
				}
				case 23:		//Key_T
				{
					if(IndexString < (LengthString - 2))
					{
						StringChar.replace(IndexString++, 1, 1, 0xD9);
						StringChar.replace(IndexString++, 1, 1, 0x81);
					}
					break;
				}
				case 28:		//Key_Y
				{
					if(IndexString < (LengthString - 2))
					{
						StringChar.replace(IndexString++, 1, 1, 0xD8);
						StringChar.replace(IndexString++, 1, 1, 0xBA);
					}
					break;
				}
				case 24:		//Key_U
				{
					if(IndexString < (LengthString - 2))
					{
						StringChar.replace(IndexString++, 1, 1, 0xD8);
						StringChar.replace(IndexString++, 1, 1, 0xB9);
					}
					break;
				}
				case 12:		//Key_I
				{
					if(IndexString < (LengthString - 2))
					{
						StringChar.replace(IndexString++, 1, 1, 0xD9);
						StringChar.replace(IndexString++, 1, 1, 0x87);
					}
					break;
				}
				case 18:		//Key_O
				{
					if(IndexString < (LengthString - 2))
					{
						StringChar.replace(IndexString++, 1, 1, 0xD8);
						StringChar.replace(IndexString++, 1, 1, 0xAE);
					}
					break;
				}
				case 19:		//Key_P
				{
					if(IndexString < (LengthString - 2))
					{
						StringChar.replace(IndexString++, 1, 1, 0xD8);
						StringChar.replace(IndexString++, 1, 1, 0xAD);
					}
					break;
				}
				case 47:		//Key_{
				{
					if(IndexString < (LengthString - 2))
					{
						StringChar.replace(IndexString++, 1, 1, 0xD8);
						StringChar.replace(IndexString++, 1, 1, 0xAC);
					}
					break;
				}
				case 48:		//Key_}
				{
					if(IndexString < (LengthString - 2))
					{
						StringChar.replace(IndexString++, 1, 1, 0xDA);
						StringChar.replace(IndexString++, 1, 1, 0x86);
					}
					break;
				}
				case 4:			//Key_A
				{
					if(IndexString < (LengthString - 2))
					{
						StringChar.replace(IndexString++, 1, 1, 0xD8);
						StringChar.replace(IndexString++, 1, 1, 0xB4);
					}
					break;
				}
				case 22:		//Key_S
				{
					if(IndexString < (LengthString - 2))
					{
						StringChar.replace(IndexString++, 1, 1, 0xD8);
						StringChar.replace(IndexString++, 1, 1, 0xB3);
					}
					break;
				}
				case 7:			//Key_D
				{
					if(IndexString < (LengthString - 2))
					{
						StringChar.replace(IndexString++, 1, 1, 0xDB);
						StringChar.replace(IndexString++, 1, 1, 0x8C);
					}
					break;
				}
				case 9:			//Key_F
				{
					if(IndexString < (LengthString - 2))
					{
						StringChar.replace(IndexString++, 1, 1, 0xD8);
						StringChar.replace(IndexString++, 1, 1, 0xA8);
					}
					break;
				}
				case 10:		//Key_G
				{
					if(IndexString < (LengthString - 2))
					{
						StringChar.replace(IndexString++, 1, 1, 0xD9);
						StringChar.replace(IndexString++, 1, 1, 0x84);
					}
					break;
				}
				case 11:		//Key_H
				{
					if(IndexString < (LengthString - 2))
					{
						StringChar.replace(IndexString++, 1, 1, 0xD8);
						StringChar.replace(IndexString++, 1, 1, 0xA7);
					}
					break;
				}
				case 13:		//Key_J
				{
					if(IndexString < (LengthString - 2))
					{
						StringChar.replace(IndexString++, 1, 1, 0xD8);
						StringChar.replace(IndexString++, 1, 1, 0xAA);
					}
					break;
				}
				case 14:		//Key_K
				{
					if(IndexString < (LengthString - 2))
					{
						StringChar.replace(IndexString++, 1, 1, 0xD9);
						StringChar.replace(IndexString++, 1, 1, 0x86);
					}
					break;
				}
				case 15:		//Key_L
				{
					if(IndexString < (LengthString - 2))
					{
						StringChar.replace(IndexString++, 1, 1, 0xD9);
						StringChar.replace(IndexString++, 1, 1, 0x85);
					}
					break;
				}
				case 51:		//Key_:
				{
					if(IndexString < (LengthString - 2))
					{
						StringChar.replace(IndexString++, 1, 1, 0xDA);
						StringChar.replace(IndexString++, 1, 1, 0xA9);
					}
					break;
				}
				case 52:		//Key_"
				{
					if(IndexString < (LengthString - 2))
					{
						StringChar.replace(IndexString++, 1, 1, 0xDA);
						StringChar.replace(IndexString++, 1, 1, 0xAF);
					}
					break;
				}
				case 29:		//Key_Z
				{
					if(IndexString < (LengthString - 2))
					{
						StringChar.replace(IndexString++, 1, 1, 0xD8);
						StringChar.replace(IndexString++, 1, 1, 0xB8);
					}
					break;
				}
				case 27:		//Key_X
				{
					if(IndexString < (LengthString - 2))
					{
						StringChar.replace(IndexString++, 1, 1, 0xD8);
						StringChar.replace(IndexString++, 1, 1, 0xB7);
					}
					break;
				}
				case 6:		//Key_C
				{
					if(IndexString < (LengthString - 2))
					{
						StringChar.replace(IndexString++, 1, 1, 0xD8);
						StringChar.replace(IndexString++, 1, 1, 0xB2);
					}
					break;
				}
				case 25:		//Key_V
				{
					if(IndexString < (LengthString - 2))
					{
						StringChar.replace(IndexString++, 1, 1, 0xD8);
						StringChar.replace(IndexString++, 1, 1, 0xB1);
					}
					break;
				}
				case 5:			//Key_B
				{
					if(IndexString < (LengthString - 2))
					{
						StringChar.replace(IndexString++, 1, 1, 0xD8);
						StringChar.replace(IndexString++, 1, 1, 0xB0);
					}
					break;
				}
				case 17:		//Key_N
				{
					if(IndexString < (LengthString - 2))
					{
						StringChar.replace(IndexString++, 1, 1, 0xD8);
						StringChar.replace(IndexString++, 1, 1, 0xAF);
					}
					break;
				}
				case 16:		//Key_M
				{
					if(IndexString < (LengthString - 2))
					{
						StringChar.replace(IndexString++, 1, 1, 0xCA);
						StringChar.replace(IndexString++, 1, 1, 0xBE);
					}
					break;
				}
				case 54:		//Key_<
				{
					if(IndexString < (LengthString - 2))
					{
						StringChar.replace(IndexString++, 1, 1, 0xD9);
						StringChar.replace(IndexString++, 1, 1, 0x88);
					}
					break;
				}
				default:
				{
					if(Keyboard_Info.Ascii != 0)
					{
						if(IndexString < (LengthString - 1))
							StringChar.replace(IndexString++, 1, 1, Keyboard_Info.Ascii);
					}
					break;
				}
			}
		}
		else
		{
			Button = Panel_ReadKeypad();

			if(Button != NO_PUSH)
			{
				if((Button > NO_PUSH) && (Button < KEYPAD_TARE))
				{
					if(LastButton != Button)
					{
						LastButton = Button;
						if(Is_PushButton)
						{
							if(IndexString < (LengthString - 2))
								IndexString+= 2;

							Is_PushButton = false;
							KeypadButton = 0;
						}
					}
					
					LastTickCount = osKernelGetTickCount();
				}

				switch((uint8_t)Button)
				{
					case KEYPAD_00:
					{
						if(IndexString < (LengthString - 1))
							StringChar.replace(IndexString++, 1, 1, '0');

						break;
					}
					case KEYPAD_01:
					{
						if(IndexString < (LengthString - 1))
							StringChar.replace(IndexString++, 1, 1, '1');

						break;
					}
					case KEYPAD_02:
					{
						if(IndexString < (LengthString - 1))
							StringChar.replace(IndexString++, 1, 1, '2');

						break;
					}
					case KEYPAD_03:
					{
						if(IndexString < (LengthString - 1))
							StringChar.replace(IndexString++, 1, 1, '3');

						break;
					}
					case KEYPAD_04:
					{
						if(IndexString < (LengthString - 1))
							StringChar.replace(IndexString++, 1, 1, '4');

						break;
					}
					case KEYPAD_05:
					{
						if(IndexString < (LengthString - 1))
							StringChar.replace(IndexString++, 1, 1, '5');

						break;
					}
					case KEYPAD_06:
					{
						if(IndexString < (LengthString - 1))
							StringChar.replace(IndexString++, 1, 1, '6');

						break;
					}
					case KEYPAD_07:
					{
						if(IndexString < (LengthString - 1))
							StringChar.replace(IndexString++, 1, 1, '7');

						break;
					}
					case KEYPAD_08:
					{
						if(IndexString < (LengthString - 1))
							StringChar.replace(IndexString++, 1, 1, '8');

						break;
					}
					case KEYPAD_09:
					{
						if(IndexString < (LengthString - 1))
							StringChar.replace(IndexString++, 1, 1, '9');

						break;
					}
					case KEYPAD_ALPHABET_01:
					{
						Is_PushButton = true;

						if(IndexString < (LengthString - 2))
						{
							if(KeypadButton == 0)
							{
								if((IndexString == 0) || ((IndexString != 0) && (StringChar.at((IndexString - 1)) == 0x20)))
								{
									StringChar.replace(IndexString, 1, 1, 0xD8);
									StringChar.replace((IndexString + 1), 1, 1, 0xA2);
								}
								else
								{
									StringChar.replace(IndexString, 1, 1, 0xD8);
									StringChar.replace((IndexString + 1), 1, 1, 0xA7);
									KeypadButton++;
								}
								
								KeypadButton++;
							}
							else if(KeypadButton == 1)
							{
								StringChar.replace(IndexString, 1, 1, 0xD8);
								StringChar.replace((IndexString + 1), 1, 1, 0xA7);
								KeypadButton++;
							}
							else if(KeypadButton == 2)
							{
								StringChar.replace(IndexString, 1, 1, 0xD8);
								StringChar.replace((IndexString + 1), 1, 1, 0xA8);
								KeypadButton++;
							}
							else if(KeypadButton == 3)
							{
								StringChar.replace(IndexString, 1, 1, 0xD9);
								StringChar.replace((IndexString + 1), 1, 1, 0xBE);
								KeypadButton++;
							}
							else if(KeypadButton == 4)
							{
								StringChar.replace(IndexString, 1, 1, 0xD8);
								StringChar.replace((IndexString + 1), 1, 1, 0xAA);
								KeypadButton = 0;
							}
						}
						break;
					}
					case KEYPAD_ALPHABET_02:
					{
						Is_PushButton = true;

						if(IndexString < (LengthString - 2))
						{
							if(KeypadButton == 0)
							{
								StringChar.replace(IndexString, 1, 1, 0xD8);
								StringChar.replace((IndexString + 1), 1, 1, 0xAC);
								KeypadButton++;
							}
							else if(KeypadButton == 1)
							{
								StringChar.replace(IndexString, 1, 1, 0xDA);
								StringChar.replace((IndexString + 1), 1, 1, 0x86);
								KeypadButton++;
							}
							else if(KeypadButton == 2)
							{
								StringChar.replace(IndexString, 1, 1, 0xD8);
								StringChar.replace((IndexString + 1), 1, 1, 0xAD);
								KeypadButton++;
							}
							else if(KeypadButton == 3)
							{
								StringChar.replace(IndexString, 1, 1, 0xD8);
								StringChar.replace((IndexString + 1), 1, 1, 0xAE);
								KeypadButton = 0;
							}
						}

						break;
					}
					case KEYPAD_ALPHABET_03:
					{
						Is_PushButton = true;

						if(IndexString < (LengthString - 2))
						{
							if(KeypadButton == 0)
							{
								StringChar.replace(IndexString, 1, 1, 0xD8);
								StringChar.replace((IndexString + 1), 1, 1, 0xAF);
								KeypadButton++;
							}
							else if(KeypadButton == 1)
							{
								StringChar.replace(IndexString, 1, 1, 0xD8);
								StringChar.replace((IndexString + 1), 1, 1, 0xB0);
								KeypadButton++;
							}
							else if(KeypadButton == 2)
							{
								StringChar.replace(IndexString, 1, 1, 0xD8);
								StringChar.replace((IndexString + 1), 1, 1, 0xB1);
								KeypadButton++;
							}
							else if(KeypadButton == 3)
							{
								StringChar.replace(IndexString, 1, 1, 0xD8);
								StringChar.replace((IndexString + 1), 1, 1, 0xB2);
								KeypadButton = 0;
							}
						}

						break;
					}
					case KEYPAD_ALPHABET_04:
					{
						Is_PushButton = true;

						if(IndexString < (LengthString - 2))
						{
							if(KeypadButton == 0)
							{
								StringChar.replace(IndexString, 1, 1, 0xD8);
								StringChar.replace((IndexString + 1), 1, 1, 0xB3);
								KeypadButton++;
							}
							else if(KeypadButton == 1)
							{
								StringChar.replace(IndexString, 1, 1, 0xD8);
								StringChar.replace((IndexString + 1), 1, 1, 0xB4);
								KeypadButton++;
							}
							else if(KeypadButton == 2)
							{
								StringChar.replace(IndexString, 1, 1, 0xD8);
								StringChar.replace((IndexString + 1), 1, 1, 0xB5);
								KeypadButton++;
							}
							else if(KeypadButton == 3)
							{
								StringChar.replace(IndexString, 1, 1, 0xD8);
								StringChar.replace((IndexString + 1), 1, 1, 0xB6);
								KeypadButton = 0;
							}
						}

						break;
					}
					case KEYPAD_ALPHABET_05:
					{
						Is_PushButton = true;

						if(IndexString < (LengthString - 2))
						{
							if(KeypadButton == 0)
							{
								StringChar.replace(IndexString, 1, 1, 0xD8);
								StringChar.replace((IndexString + 1), 1, 1, 0xB7);
								KeypadButton++;
							}
							else if(KeypadButton == 1)
							{
								StringChar.replace(IndexString, 1, 1, 0xD8);
								StringChar.replace((IndexString + 1), 1, 1, 0xB8);
								KeypadButton++;
							}
							else if(KeypadButton == 2)
							{
								StringChar.replace(IndexString, 1, 1, 0xD8);
								StringChar.replace((IndexString + 1), 1, 1, 0xB9);
								KeypadButton++;
							}
							else if(KeypadButton == 3)
							{
								StringChar.replace(IndexString, 1, 1, 0xD8);
								StringChar.replace((IndexString + 1), 1, 1, 0xBA);
								KeypadButton = 0;
							}
						}

						break;
					}
					case KEYPAD_ALPHABET_06:
					{
						Is_PushButton = true;

						if(IndexString < (LengthString - 2))
						{
							if(KeypadButton == 0)
							{
								StringChar.replace(IndexString, 1, 1, 0xD9);
								StringChar.replace((IndexString + 1), 1, 1, 0x81);
								KeypadButton++;
							}
							else if(KeypadButton == 1)
							{
								StringChar.replace(IndexString, 1, 1, 0xD9);
								StringChar.replace((IndexString + 1), 1, 1, 0x82);
								KeypadButton++;
							}
							else if(KeypadButton == 2)
							{
								StringChar.replace(IndexString, 1, 1, 0xDA);
								StringChar.replace((IndexString + 1), 1, 1, 0xA9);
								KeypadButton++;
							}
							else if(KeypadButton == 3)
							{
								StringChar.replace(IndexString, 1, 1, 0xDA);
								StringChar.replace((IndexString + 1), 1, 1, 0xAF);
								KeypadButton = 0;
							}
						}

						break;
					}
					case KEYPAD_ALPHABET_07:
					{
						Is_PushButton = true;

						if(IndexString < (LengthString - 2))
						{
							if(KeypadButton == 0)
							{
								StringChar.replace(IndexString, 1, 1, 0xD9);
								StringChar.replace((IndexString + 1), 1, 1, 0x84);
								KeypadButton++;
							}
							else if(KeypadButton == 1)
							{
								StringChar.replace(IndexString, 1, 1, 0xD9);
								StringChar.replace((IndexString + 1), 1, 1, 0x85);
								KeypadButton++;
							}
							else if(KeypadButton == 2)
							{
								StringChar.replace(IndexString, 1, 1, 0xD9);
								StringChar.replace((IndexString + 1), 1, 1, 0x86);
								KeypadButton++;
							}
							else if(KeypadButton == 3)
							{
								StringChar.replace(IndexString, 1, 1, 0xD9);
								StringChar.replace((IndexString + 1), 1, 1, 0x88);
								KeypadButton = 0;
							}
						}

						break;
					}
					case KEYPAD_ALPHABET_08:
					{
						Is_PushButton = true;

						if(IndexString < (LengthString - 2))
						{
							if(KeypadButton == 0)
							{
								StringChar.replace(IndexString, 1, 1, 0xD9);
								StringChar.replace((IndexString + 1), 1, 1, 0x87);
								KeypadButton++;
							}
							else if(KeypadButton == 1)
							{
								StringChar.replace(IndexString, 1, 1, 0xDB);
								StringChar.replace((IndexString + 1), 1, 1, 0x8C);
								KeypadButton++;
							}
							else if(KeypadButton == 2)
							{
								StringChar.replace(IndexString, 1, 1, 0xD8);
								StringChar.replace((IndexString + 1), 1, 1, 0xAB);
								KeypadButton++;
							}
							else if(KeypadButton == 3)
							{
								StringChar.replace(IndexString, 1, 1, 0xDA);
								StringChar.replace((IndexString + 1), 1, 1, 0x98);
								KeypadButton = 0;
							}
						}
						
						break;
					}
					case KEYPAD_SPACE:
					{
						if(IndexString < (LengthString - 1))
							StringChar.replace(IndexString++, 1, 1, ' ');

						break;
					}
					case KEYPAD_CE:
					{
						if((IndexString > 0) && (StringChar.at((IndexString - 1)) <= 0x7F))
							StringChar.erase((--IndexString), 1);
						else if((IndexString > 1) && (StringChar.at((IndexString - 2)) > 0x7F))
						{
							StringChar.erase((--IndexString), 1);
							StringChar.erase((--IndexString), 1);
						}

						break;
					}
					case KEYPAD_ESC:
					{
						*LastPressButton = Button;
						return StringCharInput;
					}
					case KEYPAD_ENTER:
					{
						*LastPressButton = Button;
						return StringChar;
					}
				}
			}
		}

		GetTextPixelPosition(&Last_Char_X, &Last_Char_Y);

		if(GetLetter() == PERSIAN_LETTER)
			GLCD_FillRectangle((GET_MAX_X() - Last_Char_X), Y, (GET_MAX_X() - X - 1), (Last_Char_Y + LcdFontHeight() - 1), GetbackgroundColor());
		else
			GLCD_FillRectangle(X, Y, (Last_Char_X - 1), (Last_Char_Y + LcdFontHeight() - 1), GetbackgroundColor());

		SetTextPixelPosition(X, Y);
		PutsString((char *)StringChar.c_str());

		Blinker.Counter++;
		if(Blinker.Counter > Blinker.AmountBlinker)
		{
			PutChar(' ');

			if(Blinker.Counter > (1.5 * Blinker.AmountBlinker))
				Blinker.Counter = 0;
		}
		else
			PutChar('|');

		GLCD_Screen_Update();
	}
}

//**********************************************************************************************************

string GetStringDateSystem(void)
{
	RTC_DateTypeDef sDate = {0};
	ShamsiDate_TypeDef ShamsiDate;
	string StringDate;

	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

	ShamsiDate = MiladiToShamsi(sDate.Month, sDate.Date, (2000 + sDate.Year));
	StringDate = IntegerToString(ShamsiDate.iYear, 4);
	StringDate += "/";
	StringDate += IntegerToString(ShamsiDate.iMonth, 2);
	StringDate += "/";
	StringDate += IntegerToString(ShamsiDate.iDay, 2);

	return StringDate;
}

//**********************************************************************************************************

string GetStringTimeSystem(bool WithSeconds)
{
	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef sDate = {0};
	string StringTime;

	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

	StringTime = IntegerToString(sTime.Hours, 2);
	StringTime += ":";
	StringTime += IntegerToString(sTime.Minutes, 2);

	if(WithSeconds)
	{
		StringTime += ":";
		StringTime += IntegerToString(sTime.Seconds, 2);
	}

	return StringTime;
}

//**********************************************************************************************************

string GetStringPersianDateSystem(void)
{
	RTC_DateTypeDef sDate = {0};
	ShamsiDate_TypeDef ShamsiDate;
	string StringDate;

	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

	ShamsiDate = MiladiToShamsi(sDate.Month, sDate.Date, (2000 + sDate.Year));

	StringDate = IntegerToString(ShamsiDate.iDay, 2);
	StringDate += "/";
	StringDate += IntegerToString(ShamsiDate.iMonth, 2);
	StringDate += "/";
	StringDate += IntegerToString(ShamsiDate.iYear, 4);

	return StringDate;
}

//**********************************************************************************************************

string GetStringPersianTimeSystem(bool WithSeconds)
{
	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef sDate = {0};
	string StringTime = "";

	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

	if(WithSeconds)
	{
		StringTime = IntegerToString(sTime.Seconds, 2);
		StringTime += ":";
	}

	StringTime += IntegerToString(sTime.Minutes, 2);
	StringTime += ":";
	StringTime += IntegerToString(sTime.Hours, 2);

	return StringTime;
}

//**********************************************************************************************************

void ErrorSystemReport(ErrorSystem_TypeDef ErrorSystem)
{
	switch((uint8_t)ErrorSystem)
	{
		case E01_READ_SCALE01:
		{
			ErrorList.E01_READ_SCALE01 = true;
			break;
		}
		case E02_READ_SCALE02:
		{
			ErrorList.E02_READ_SCALE02 = true;
			break;
		}
		case E03_AD7780_SCALE01:
		{
			ErrorList.E03_AD7780_SCALE01 = true;
			break;
		}
		case E04_AD7780_SCALE02:
		{
			ErrorList.E04_AD7780_SCALE02 = true;
			break;
		}
		case E05_EEPROM:
		{
			ErrorList.E05_EEPROM = true;
			break;
		}
		case E06_COM01:
		{
			ErrorList.E06_COM01 = true;
			break;
		}
		case E07_COM02:
		{
			ErrorList.E07_COM02 = true;
			break;
		}
		case E08_COM03:
		{
			ErrorList.E08_COM03 = true;
			break;
		}
		case E09_POWER_UP_ZERO:
		{
			ErrorList.E09_POWER_UP_ZERO = true;
			break;
		}
		case E10_SCALE_TARED:
		{
			ErrorList.E10_SCALE_TARED = true;
			break;
		}
		case E11_NEGATIVE_WEIGHTS:
		{
			ErrorList.E11_NEGATIVE_WEIGHTS = true;
			break;
		}
		case E12_PRINTER:
		{
			ErrorList.E12_PRINTER = true;
			break;
		}
		case E13_BACKUP_MEMORY:
		{
			ErrorList.E13_BACKUP_MEMORY = true;
			break;
		}
		case E14_LIMIT_BACKUP_MEMORY:
		{
			ErrorList.E14_LIMIT_BACKUP_MEMORY = true;
			break;
		}
		case E15_SCALE_NOT_TARED:
		{
			ErrorList.E15_SCALE_NOT_TARED = true;
			break;
		}
		case E16_USB_PRINTER:
		{
			ErrorList.E16_USB_PRINTER = true;
			break;
		}
	}
}

//**********************************************************************************************************

ErrorSystem_TypeDef GetErrorSystem(void)
{
	if(ErrorList.E01_READ_SCALE01)
		return E01_READ_SCALE01;
	else if(ErrorList.E02_READ_SCALE02)
		return E02_READ_SCALE02;
	else if(ErrorList.E03_AD7780_SCALE01)
		return E03_AD7780_SCALE01;
	else if(ErrorList.E04_AD7780_SCALE02)
		return E04_AD7780_SCALE02;
	else if(ErrorList.E05_EEPROM)
		return E05_EEPROM;
	else if(ErrorList.E06_COM01)
		return E06_COM01;
	else if(ErrorList.E07_COM02)
		return E07_COM02;
	else if(ErrorList.E08_COM03)
		return E08_COM03;
	else if(ErrorList.E09_POWER_UP_ZERO)
		return E09_POWER_UP_ZERO;
	else if(ErrorList.E10_SCALE_TARED)
		return E10_SCALE_TARED;
	else if(ErrorList.E11_NEGATIVE_WEIGHTS)
		return E11_NEGATIVE_WEIGHTS;
	else if(ErrorList.E12_PRINTER)
		return E12_PRINTER;
	else if(ErrorList.E13_BACKUP_MEMORY)
		return E13_BACKUP_MEMORY;
	else if(ErrorList.E14_LIMIT_BACKUP_MEMORY)
		return E14_LIMIT_BACKUP_MEMORY;
	else if(ErrorList.E15_SCALE_NOT_TARED)
		return E15_SCALE_NOT_TARED;
	else if(ErrorList.E16_USB_PRINTER)
		return E16_USB_PRINTER;
	else
		return NON_ERROR;
}

//**********************************************************************************************************

void ErrorSystemReset(ErrorSystem_TypeDef ErrorSystem)
{
	switch((uint8_t)ErrorSystem)
	{
		case E01_READ_SCALE01:
		{
			ErrorList.E01_READ_SCALE01 = false;
			break;
		}
		case E02_READ_SCALE02:
		{
			ErrorList.E02_READ_SCALE02 = false;
			break;
		}
		case E03_AD7780_SCALE01:
		{
			ErrorList.E03_AD7780_SCALE01 = false;
			break;
		}
		case E04_AD7780_SCALE02:
		{
			ErrorList.E04_AD7780_SCALE02 = false;
			break;
		}
		case E05_EEPROM:
		{
			ErrorList.E05_EEPROM = false;
			break;
		}
		case E06_COM01:
		{
			ErrorList.E06_COM01 = false;
			break;
		}
		case E07_COM02:
		{
			ErrorList.E07_COM02 = false;
			break;
		}
		case E08_COM03:
		{
			ErrorList.E08_COM03 = false;
			break;
		}
		case E09_POWER_UP_ZERO:
		{
			ErrorList.E09_POWER_UP_ZERO = false;
			break;
		}
		case E10_SCALE_TARED:
		{
			ErrorList.E10_SCALE_TARED = false;
			break;
		}
		case E11_NEGATIVE_WEIGHTS:
		{
			ErrorList.E11_NEGATIVE_WEIGHTS = false;
			break;
		}
		case E12_PRINTER:
		{
			ErrorList.E12_PRINTER = false;
			break;
		}
		case E13_BACKUP_MEMORY:
		{
			ErrorList.E13_BACKUP_MEMORY = false;
			break;
		}
		case E14_LIMIT_BACKUP_MEMORY:
		{
			ErrorList.E14_LIMIT_BACKUP_MEMORY = false;
			break;
		}
		case E15_SCALE_NOT_TARED:
		{
			ErrorList.E15_SCALE_NOT_TARED = false;
			break;
		}
		case E16_USB_PRINTER:
		{
			ErrorList.E16_USB_PRINTER = false;
			break;
		}
	}
}

//**********************************************************************************************************

void ShowFillPassword(void)
{
	GLCD_FillRectangle(25, 14, 214, 58, BLACK);
	GLCD_FillRectangle(27, 16, 212, 56, WHITE);

	SetFontLetter(PERSIAN_LETTER);
	SetFontColor(BLACK);
	SetBackgroundColor(WHITE);
	LcdFontSpaceX(1);
	LcdFontSpaceY(0);
	SetFont(Font_Pe_12X11);
	SetTextPixelPosition(81, 31);
	PutsString("پسورد اشتباه است");
	GLCD_Screen_Update();

	DELAY_MS(3000);
}

//**********************************************************************************************************

void ShowSaveParameters(void)
{
	GLCD_FillRectangle(25, 14, 214, 58, BLACK);
	GLCD_FillRectangle(27, 16, 212, 56, WHITE);

	SetFontLetter(PERSIAN_LETTER);
	SetFontColor(BLACK);
	SetBackgroundColor(WHITE);
	LcdFontSpaceX(1);
	LcdFontSpaceY(0);
	SetFont(Font_Pe_12X11);
	SetTextPixelPosition(82, 31);
	PutsString("تغییرات ذخیره شد");
	GLCD_Screen_Update();

	DELAY_MS(3000);
}

//**********************************************************************************************************

void ShowMessageError(string MessageError)
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
	SetTextPixelPosition(15, 41);
	PutsString((char *)MessageError.c_str());
	GLCD_Screen_Update();
}

//**********************************************************************************************************

string GetStringDisplayExternal(WeightScale_TypeDef WeightScale, uint8_t Format)
{
	string StringDisplayExternal = "";

	switch(Format)
	{
		case 0:		//Gamma07
		{
			StringDisplayExternal = "";

			if(WeightScale.Normally >= 0)
				StringDisplayExternal += " ";
			else
				StringDisplayExternal += "-";

			if(WeightScale.DECPoint != 0)
				StringDisplayExternal += WeightToString(labs(WeightScale.Normally), 5, WeightScale.DECPoint);
			else
				StringDisplayExternal += WeightToString(labs(WeightScale.Normally), 5, WeightScale.DECPoint);

			StringDisplayExternal.erase(1, 1);

			StringDisplayExternal += 0x04;
			StringDisplayExternal += 0x0D;

			break;
		}
		case 4:		//YAOHUA
		{
			string String = "";

			String = "=";

			if(WeightScale.Normally >= 0)
				String += "0";
			else
				String += "-";

			if(WeightScale.DECPoint != 0)
				String += WeightToString(labs(WeightScale.Normally), 5, WeightScale.DECPoint);
			else
				String += WeightToString(labs(WeightScale.Normally), 5, WeightScale.DECPoint);

			String.erase(2, 1);

			String += ".";

			StringDisplayExternal = "";

			for(int i = 0; i < String.length(); i++)
				StringDisplayExternal += String[(String.length() - i - 1)];

			break;
		}
		case 7:		//TSM 100
		{
			StringDisplayExternal = "P";

			if(WeightScale.Normally >= 0)
				StringDisplayExternal += "+";
			else
				StringDisplayExternal += "-";

			if(WeightScale.DECPoint != 0)
				StringDisplayExternal += WeightToString(labs(WeightScale.Normally), 5, WeightScale.DECPoint);
			else
				StringDisplayExternal += WeightToString(labs(WeightScale.Normally), 6, WeightScale.DECPoint);

			StringDisplayExternal.erase(2, 1);

			StringDisplayExternal += 0x0D;

			break;
		}
		default:
		{
			StringDisplayExternal = "";
			break;
		}
	}

	return StringDisplayExternal;
}

//**********************************************************************************************************











