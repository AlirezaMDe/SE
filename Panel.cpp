/*-----------------------------------------------------------------------------------------------------------
 @file          : Panel.cpp
 @author  		: Reza Farahani (nmmco2015@gmail.com)
 C++ source 2024
 -----------------------------------------------------------------------------------------------------------*/

#include "GLCD_TextFunction.h"
#include "Panel.hpp"
#include "usbh_hid.h"

//**********************************************************************************************************

extern USBH_HandleTypeDef hUsbHostFS;

//**********************************************************************************************************

extern osMessageQueueId_t KeyboardUSB_QueueHandle;

//**********************************************************************************************************

volatile uint16_t CountAutoPowerOff = 0;

//**********************************************************************************************************

void USBH_HID_EventCallback(USBH_HandleTypeDef *phost)
{
	if(USBH_HID_GetDeviceType(phost) == HID_KEYBOARD)
	{
		HID_KEYBD_Info_TypeDef *Keyboard_Info;
		UnionKeyboard_Info_TypeDef UnionKeyboard_Info;

		Keyboard_Info = USBH_HID_GetKeybdInfo(phost);

		if(Keyboard_Info != NULL)
		{
			UnionKeyboard_Info.Keyboard_Info.Ascii = USBH_HID_GetASCIICode(Keyboard_Info);
			UnionKeyboard_Info.Keyboard_Info.key = Keyboard_Info->keys[0];

			osMessageQueueReset(KeyboardUSB_QueueHandle);
			osMessageQueuePut(KeyboardUSB_QueueHandle, UnionKeyboard_Info.Array, osPriorityISR, 0);
		}
	}
}

//**********************************************************************************************************
//KEYBOARD FUNCTION

Keyboard_Info_TypeDef Panel_ScanKeyboard(void)
{
	UnionKeyboard_Info_TypeDef UnionKeyboard_Info;

	if(osMessageQueueGet(KeyboardUSB_QueueHandle, UnionKeyboard_Info.Array, NULL, 0) == osOK)
		return UnionKeyboard_Info.Keyboard_Info;
	else
	{
		UnionKeyboard_Info.Keyboard_Info.key = 0;
		UnionKeyboard_Info.Keyboard_Info.Ascii = 0;
		return UnionKeyboard_Info.Keyboard_Info;
	}
}

//**********************************************************************************************************

PanelKeypadButton_TypeDef Panel_ReadKeyboard(void)
{
	Keyboard_Info_TypeDef Keyboard_Info;

	Keyboard_Info = Panel_ScanKeyboard();

	switch(Keyboard_Info.key)
	{
		case 58:
			 return KEYPAD_WEIGH_01;
		case 59:
			 return KEYPAD_WEIGH_02;
		case 60:
			 return KEYPAD_PRINT;
		case 61:
			 return KEYPAD_SET;
		case 62:
			 return KEYPAD_PTARE;
		case 89:
		case 30:
			 return KEYPAD_01;
		case 90:
		case 31:
			 return KEYPAD_02;
		case 91:
		case 32:
			 return KEYPAD_03;
		case 92:
		case 33:
			 return KEYPAD_04;
		case 93:
		case 34:
			 return KEYPAD_05;
		case 94:
		case 35:
			 return KEYPAD_06;
		case 95:
		case 36:
			 return KEYPAD_07;
		case 96:
		case 37:
			 return KEYPAD_08;
		case 97:
		case 38:
			 return KEYPAD_09;
		case 98:
		case 39:
			 return KEYPAD_00;
		case 42:
			 return KEYPAD_CE;
		case 41:
			 return KEYPAD_ESC;
		case 40:
		case 88:
			 return KEYPAD_ENTER;
		case 80:
			 return KEYPAD_LEFT;
		case 82:
			 return KEYPAD_UP;
		case 79:
			 return KEYPAD_RIGHT;
		case 69:
			 return KEYPAD_MENU;
		case 81:
			 return KEYPAD_DOWN;
		default:
			 return NO_PUSH;
	}
}

//**********************************************************************************************************
//KEYPAD FUNCTION

void ResetColemnKeyboard(uint8_t Colemn)
{
	switch(Colemn)
	{
		case 0:
		{
			KEY_COL01_RESET;
			KEY_COL02_SET;
			KEY_COL03_SET;
			KEY_COL04_SET;
			KEY_COL05_SET;
			KEY_COL06_SET;
			KEY_COL07_SET;
			KEY_COL08_SET;
			break;
		}
		case 1:
		{
			KEY_COL01_SET;
			KEY_COL02_RESET;
			KEY_COL03_SET;
			KEY_COL04_SET;
			KEY_COL05_SET;
			KEY_COL06_SET;
			KEY_COL07_SET;
			KEY_COL08_SET;
			break;
		}
		case 2:
		{
			KEY_COL01_SET;
			KEY_COL02_SET;
			KEY_COL03_RESET;
			KEY_COL04_SET;
			KEY_COL05_SET;
			KEY_COL06_SET;
			KEY_COL07_SET;
			KEY_COL08_SET;
			break;
		}
		case 3:
		{
			KEY_COL01_SET;
			KEY_COL02_SET;
			KEY_COL03_SET;
			KEY_COL04_RESET;
			KEY_COL05_SET;
			KEY_COL06_SET;
			KEY_COL07_SET;
			KEY_COL08_SET;
			break;
		}
		case 4:
		{
			KEY_COL01_SET;
			KEY_COL02_SET;
			KEY_COL03_SET;
			KEY_COL04_SET;
			KEY_COL05_RESET;
			KEY_COL06_SET;
			KEY_COL07_SET;
			KEY_COL08_SET;
			break;
		}
		case 5:
		{
			KEY_COL01_SET;
			KEY_COL02_SET;
			KEY_COL03_SET;
			KEY_COL04_SET;
			KEY_COL05_SET;
			KEY_COL06_RESET;
			KEY_COL07_SET;
			KEY_COL08_SET;
			break;
		}
		case 6:
		{
			KEY_COL01_SET;
			KEY_COL02_SET;
			KEY_COL03_SET;
			KEY_COL04_SET;
			KEY_COL05_SET;
			KEY_COL06_SET;
			KEY_COL07_RESET;
			KEY_COL08_SET;
			break;
		}
		case 7:
		{
			KEY_COL01_SET;
			KEY_COL02_SET;
			KEY_COL03_SET;
			KEY_COL04_SET;
			KEY_COL05_SET;
			KEY_COL06_SET;
			KEY_COL07_SET;
			KEY_COL08_RESET;
			break;
		}
	}
}

//**********************************************************************************************************

bool Panel_IsPushKeypad(void)
{
	for(uint8_t i = 0 ; i < 8 ; i++)
	{
		ResetColemnKeyboard(i);
		DELAY_MS(1);

		if((IS_RESET_KEY_ROW01) || (IS_RESET_KEY_ROW02) || (IS_RESET_KEY_ROW03) || (IS_RESET_KEY_ROW04))
			return true;
	}

	return false;
}

//**********************************************************************************************************

uint8_t Panel_ScanKeypad(void)
{
	if(Panel_IsPushKeypad())
	{
		DELAY_MS(10);

		for(uint8_t i = 0 ; i < 8 ; i++)
		{
			ResetColemnKeyboard(i);
			DELAY_MS(1);

			if(IS_RESET_KEY_ROW01)
				return (10 + i);
			else if(IS_RESET_KEY_ROW02)
				return (20 + i);
			else if(IS_RESET_KEY_ROW03)
				return (30 + i);
			else if(IS_RESET_KEY_ROW04)
				return (40 + i);
		}
	}
	
	return 0;
}

//**********************************************************************************************************

PanelKeypadButton_TypeDef Panel_ReadKeypad(void)
{
	uint8_t KeypadButton;

	KeypadButton = Panel_ScanKeypad();

	if(KeypadButton == 0)
		return NO_PUSH;

	BUZZER_SET;
	DELAY_MS(150);
	BUZZER_RESET;

	switch(KeypadButton)
	{
		case 33:
			 return KEYPAD_01;
		case 34:
			 return KEYPAD_02;
		case 35:
			 return KEYPAD_03;
		case 25:
			 return KEYPAD_04;
		case 26:
			 return KEYPAD_05;
		case 27:
			 return KEYPAD_06;
		case 15:
			 return KEYPAD_07;
		case 16:
			 return KEYPAD_08;
		case 17:
			 return KEYPAD_09;
		case 37:
			 return KEYPAD_ESC;
		case 36:
			 return KEYPAD_00;
		case 24:
			 return KEYPAD_ENTER;
		case 14:
			 return KEYPAD_WEIGH_02;
		case 13:
			 return KEYPAD_WEIGH_01;
		case 22:
			 return KEYPAD_SPACE;
		case 12:
			 return KEYPAD_SET;
		case 23:
			 return KEYPAD_PRINT;
		case 45:
			 return KEYPAD_CE;
		case 42:
			 return KEYPAD_LEFT;
		case 32:
			 return KEYPAD_UP;
		case 44:
			 return KEYPAD_RIGHT;
		case 43:
			 return KEYPAD_MENU;
		case 46:
			 return KEYPAD_DOWN;
		case 11:
			 return KEYPAD_ALPHABET_01;
		case 10:
			 return KEYPAD_ALPHABET_02;
		case 21:
			 return KEYPAD_ALPHABET_03;
		case 20:
			 return KEYPAD_ALPHABET_04;
		case 31:
			 return KEYPAD_ALPHABET_05;
		case 30:
			 return KEYPAD_ALPHABET_06;
		case 41:
			 return KEYPAD_ALPHABET_07;
		case 40:
			 return KEYPAD_ALPHABET_08;
		default:
			 return NO_PUSH;
	}
}

//**********************************************************************************************************
//PUBLIC FUNCTIONS

PanelKeypadButton_TypeDef Panel_ReadPanel(void)
{
	PanelKeypadButton_TypeDef KeypadButton = NO_PUSH;

	KeypadButton = Panel_ReadKeypad();

	if(KeypadButton == NO_PUSH)
		KeypadButton = Panel_ReadKeyboard();

	if(KeypadButton != NO_PUSH)
	{
		LCD_BLK_SET;
		CountAutoPowerOff = 0;
	}

	return KeypadButton;
}

//**********************************************************************************************************

