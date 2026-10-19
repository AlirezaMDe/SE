/*-----------------------------------------------------------------------------------------------------------
 @file          : Printer.cpp
 @author  		: Reza Farahani (nmmco2015@gmail.com)
 C++ source 2024
 -----------------------------------------------------------------------------------------------------------*/

#include "Printer.hpp"
#include "LPT_DotMatrixPrinter_24h.h"
#include "LPT_DotMatrixPrinterPersian_Map.h"
#include "LPT_LaserPrinter.h"
#include "LPT_LaserPrinterPersian_Map.h"
#include "Thermal_Printer.hpp"
#include "Thermal_PrinterPersian_Map.h"

//**********************************************************************************************************

extern UART_HandleTypeDef huart1;		//Rs232_01

//**********************************************************************************************************

Thermal_Printer Printer;

uint8_t TypePrinter = 0;
bool TypePaper = true;

//**********************************************************************************************************

void Delay_us(uint32_t Delay)
{
	for(uint32_t i = (Delay * SYSTEM_CORE_CLOCK); i > 0; i--)
	{
		__ASM("NOP");
	}
}

//**********************************************************************************************************

void LPT_Printer_SendData(uint8_t Data)
{
#if(LPT_PRINTER_CUSTOM_PIN)
{
	if(Data & 0X01)
		LPT_D0_SET;
	else
		LPT_D0_RESET;
	if(Data & 0X02)
		LPT_D1_SET;
	else
		LPT_D1_RESET;
	if(Data & 0X04)
		LPT_D2_SET;
	else
		LPT_D2_RESET;
	if(Data & 0X08)
		LPT_D3_SET;
	else
		LPT_D3_RESET;
	if(Data & 0X10)
		LPT_D4_SET;
	else
		LPT_D4_RESET;
	if(Data & 0X20)
		LPT_D5_SET;
	else
		LPT_D5_RESET;
	if(Data & 0X40)
		LPT_D6_SET;
	else
		LPT_D6_RESET;
	if(Data & 0X80)
		LPT_D7_SET;
	else
		LPT_D7_RESET;
}
#else
{
	HAL_GPIO_WritePin(LPT_D0_GPIO_Port, 0xFF, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LPT_D0_GPIO_Port, Data, GPIO_PIN_SET);
}
#endif
}

//**********************************************************************************************************

Status_Printer_TypeDef LPT_Printer_SendChar(uint8_t Char)
{
	uint32_t LastTickCount = 0;

	if(IS_RESET_LPT_BUSY)
	{
		LPT_Printer_SendData(Char);
		Delay_us(1);
		LPT_STROB_RESET;
		Delay_us(1);
		LPT_STROB_SET;

		LastTickCount = osKernelGetTickCount();

		do
		{
			if((osKernelGetTickCount() - LastTickCount) >= LPT_PRINTER_TIME_OUT)
				return TIME_OUT_PRINTER;
		}
		while(!IS_RESET_LPT_BUSY);

		return OK_PRINTER;
	}
	else
		return BUSY_PRINTER;
}

//**********************************************************************************************************

Status_Printer_TypeDef LPT_Printer_SendString(string String)
{
	Status_Printer_TypeDef Status_Printer;
	uint8_t i = 0;

	while(String[i])
	{
		Status_Printer = LPT_Printer_SendChar(String[i]);
		if(Status_Printer != OK_PRINTER)
			return Status_Printer;
		i++;
	}

	return OK_PRINTER;
}

//**********************************************************************************************************

Status_Printer_TypeDef LPT_Printer_SendCommand(const uint8_t *Command, uint16_t Size)
{
	Status_Printer_TypeDef Status_Printer;

	for(uint16_t i = 0; i < Size; i++)
	{
	 	Status_Printer = LPT_Printer_SendChar(Command[i]);
		if(Status_Printer != OK_PRINTER)
			return Status_Printer;
	}

	return OK_PRINTER;
}

//**********************************************************************************************************

Status_Printer_TypeDef LPT_Printer_CheckPaper(void)
{
	if(IS_RESET_LPT_PAPER)
		return OK_PRINTER;
	else
		return NO_PAPER_PRINTER;
}

//**********************************************************************************************************
//**********************************************************************************************************
//**********************************************************************************************************

Status_Printer_TypeDef LPT_DotMatrixPrinterInitialize(void)
{
	LPT_STROB_SET;
	LPT_STROB_SET;
	DELAY_MS(100);
	LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_RESET_COMMAND, 2);
	DELAY_MS(100);
	return LPT_DotMatrixPrinterDefiningCharacters();
}

//**********************************************************************************************************

Status_Printer_TypeDef LPT_DotMatrixPrinterDefiningCharacters(void)
{
	Status_Printer_TypeDef Status_Printer;

	Status_Printer = LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_RESET_COMMAND, 2);
	if(Status_Printer != OK_PRINTER)
		return Status_Printer;

//----------------------------------------------------------------------------------------------------------

	Status_Printer = LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_SELECT_DRAFT_COMMAND, 3);
	if(Status_Printer != OK_PRINTER)
		return Status_Printer;

	Status_Printer = LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_CANCELS_SUPER_SUBSCRIPT_COMMAND, 2);
	if(Status_Printer != OK_PRINTER)
		return Status_Printer;

	Status_Printer = LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_CANCEL_PROPORTIONAL_SPACING_COMMAND, 3);
	if(Status_Printer != OK_PRINTER)
		return Status_Printer;

	Status_Printer = LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_CANCEL_ITALIC_FONT_COMMAND, 2);
	if(Status_Printer != OK_PRINTER)
		return Status_Printer;

	Status_Printer = LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_SELECT_FONT_10_CPI_COMMAND, 2);
	if(Status_Printer != OK_PRINTER)
		return Status_Printer;

//----------------------------------------------------------------------------------------------------------

	Status_Printer = LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_COPY_ROM_TO_RAM_COMMAND, 5);
	if(Status_Printer != OK_PRINTER)
		return Status_Printer;

//----------------------------------------------------------------------------------------------------------

	for(uint32_t i = 0; i < sizeof(LPT_DOT_MATRIX_PRINTER_DATA); i++)
	{
		Status_Printer = LPT_Printer_SendChar(LPT_DOT_MATRIX_PRINTER_DATA[i]);
		if(Status_Printer != OK_PRINTER)
			return Status_Printer;
	}

//----------------------------------------------------------------------------------------------------------

	Status_Printer = LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_DEFINE_POSITIONING_UNIT_COMMAND, 6);
	if(Status_Printer != OK_PRINTER)
		return Status_Printer;

//----------------------------------------------------------------------------------------------------------

	LPT_DotMatrixPrinterRestInitializeFont();

//----------------------------------------------------------------------------------------------------------

	return OK_PRINTER;
}

//**********************************************************************************************************

void LPT_DotMatrixPrinterRestInitializeFont(void)
{
	LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_CANCEL_ITALIC_FONT_COMMAND, 2);
	LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_CANCEL_BOLD_FONT_COMMAND, 2);
	LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_CANCEL_CONDENSED_PRINTING_COMMAND);
	LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_TURN_OFF_DOUBLE_WIDTH_PRINTING_COMMAND, 3);
	LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_TURN_OFF_DOUBLE_HEIGHT_PRINTING_COMMAND, 3);
	LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_CANCEL_DOUBLE_STRIKE_PRINTING_COMMAND, 2);
	LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_TURN_OFF_UNDERLINE_PRINTING_COMMAND, 3);
	LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_SELECT_FONT_10_CPI_COMMAND, 2);
	LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_CARRIAGE_RETURN_COMMAND);
}

//**********************************************************************************************************

uint8_t LPT_DotMatrixPrinter_UTF8_To_Map(uint8_t Character)
{
	static uint8_t LastCharacter = 0;
	uint8_t Last;

    if(Character < 128)
    {
    	LastCharacter = 0;
        return Character;
    }

    Last = LastCharacter;
    LastCharacter = Character;

    switch(Last)
    {
		case 0xCA:
    	{
    		switch(Character)
			{
    			case 0xBE:
    				return 0xBF;
			}
    	}
    	case 0xD8:
    	{
			switch(Character)
			{
				case 0xA2:
    				return 0xC0;
				case 0xA7:
    				return 0xC1;
				case 0xA8:
    				return 0xC2;
				case 0xAA:
    				return 0xC4;
				case 0xAB:
    				return 0xC5;
				case 0xAC:
    				return 0xC6;
				case 0xAD:
    				return 0xC8;
				case 0xAE:
    				return 0xC9;
				case 0xAF:
    				return 0xCA;
				case 0xB0:
    				return 0xCB;
				case 0xB1:
    				return 0xCC;
				case 0xB2:
    				return 0xCD;
				case 0xB3:
    				return 0xCF;
				case 0xB4:
    				return 0xD0;
				case 0xB5:
    				return 0xD1;
				case 0xB6:
    				return 0xD2;
				case 0xB7:
    				return 0xD3;
				case 0xB8:
    				return 0xD4;
				case 0xB9:
    				return 0xD5;
				case 0xBA:
    				return 0xD6;
			}
    	}
    	case 0xD9:
    	{
			switch(Character)
			{
    			case 0x81:
    				return 0xD7;
				case 0x82:
    				return 0xD8;
				case 0x84:
    				return 0xDB;
				case 0x85:
    				return 0xDC;
				case 0x86:
    				return 0xDD;
				case 0x87:
    				return 0xDF;
				case 0x88:
    				return 0xDE;
				case 0xBE:
    				return 0xC3;
			}
    	}
    	case 0xDA:
    	{
			switch(Character)
			{
    			case 0x86:
    				return 0xC7;
				case 0x98:
    				return 0xCE;
				case 0xA9:
    				return 0xD9;
				case 0xAF:
    				return 0xDA;
			}
    	}
    	case 0xDB:
    	{
    		switch(Character)
			{
    			case 0x8C:
    				return 0xE0;
			}
    	}
	}

	return 0;
}

//**********************************************************************************************************

Status_Printer_TypeDef LPT_DotMatrixPrinterPrintStringPersian(char *strPersian)
{
	Status_Printer_TypeDef Status_Printer;
	uint8_t Character;
	uint8_t CurrentCharacter = 0;
	uint8_t NextCharacter = 0;
	uint8_t PreviousCharacter = 0;
	uint8_t CurrentStat;
	uint8_t NextStat;
	uint8_t PreviousStat;
	uint8_t IndexString = 0;
	uint8_t String[100];
	uint8_t CountNumber = 0;
	uint8_t StringNumber[100];

	for (int i = 0; i < strlen(strPersian); i++)
	{
		Character = LPT_DotMatrixPrinter_UTF8_To_Map(strPersian[i]);
		if (Character != 0)
			String[IndexString++] = Character;
	}

	String[IndexString] = 0;
	IndexString = 0;

//----------------------------------------------------------------------------------------------------------

 	while(String[IndexString])
	{
 		CurrentCharacter = String[IndexString++];
		NextCharacter = String[IndexString];

		if((CurrentCharacter >= 0x30) && (CurrentCharacter <= 0x39))
		{
			CountNumber++;
			StringNumber[CountNumber] = CurrentCharacter;
			
			if(!((NextCharacter >= 0x30) && (NextCharacter <= 0x39)))
			{
				for (int i = CountNumber; i > 0; i--)
				{
					Status_Printer = LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_USER_DEFINED_SET_COMMAND, 3);
					if(Status_Printer != OK_PRINTER)
						return Status_Printer;

					Status_Printer = LPT_DotMatrixPrinterPrintCharacterPersian(StringNumber[i]);
					if(Status_Printer != OK_PRINTER)
						return Status_Printer;	
				}
				
				CountNumber = 0;
			}
			
			PreviousCharacter = 0;
			continue;
		}
		else if(CurrentCharacter == 0x3F)
		{
			Status_Printer = LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_USER_DEFINED_SET_COMMAND, 3);
			if(Status_Printer != OK_PRINTER)
				return Status_Printer;

			Status_Printer = LPT_DotMatrixPrinterPrintCharacterPersian(0x3A);
			if(Status_Printer != OK_PRINTER)
				return Status_Printer;

			PreviousCharacter = 0;
			continue;
		}
		else if(CurrentCharacter == 0x2C)
		{
			Status_Printer = LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_USER_DEFINED_SET_COMMAND, 3);
			if(Status_Printer != OK_PRINTER)
				return Status_Printer;

			Status_Printer = LPT_DotMatrixPrinterPrintCharacterPersian(CurrentCharacter);
			if(Status_Printer != OK_PRINTER)
				return Status_Printer;

			PreviousCharacter = 0;
			continue;
		}
		else if(CurrentCharacter <= 0x7F)
		{
			Status_Printer = LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_NORMAL_SET_COMMAND, 3);
			if(Status_Printer != OK_PRINTER)
				return Status_Printer;

			Status_Printer = LPT_DotMatrixPrinterPrintCharacterPersian(CurrentCharacter);
			if(Status_Printer != OK_PRINTER)
				return Status_Printer;

			PreviousCharacter = 0;
			continue;
		}
		else if(CurrentCharacter == 0xDB)
		{
			if(String[IndexString] == 0xC1)
			{
				Status_Printer = LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_USER_DEFINED_SET_COMMAND, 3);
				if(Status_Printer != OK_PRINTER)
					return Status_Printer;

				Status_Printer = LPT_DotMatrixPrinterPrintCharacterPersian(0x3C);
				if(Status_Printer != OK_PRINTER)
					return Status_Printer;

				PreviousCharacter = 0;
				IndexString++;
				continue;
			}
		}

//----------------------------------------------------------------------------------------------------------

		if(PreviousCharacter >= 0xBF)
			PreviousStat = LPT_DOT_MATRIX_PRINTER_PERSIAN_MAP[(PreviousCharacter - 0xBF)][5];
		else
			PreviousStat = 0;

		if(NextCharacter >= 0xBF)
			NextStat = LPT_DOT_MATRIX_PRINTER_PERSIAN_MAP[(NextCharacter - 0xBF)][4];
		else
			NextStat = 0;

		CurrentStat = (PreviousStat << 1) | NextStat;
		Status_Printer = LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_USER_DEFINED_SET_COMMAND, 3);
		if(Status_Printer != OK_PRINTER)
			return Status_Printer;

		Status_Printer = LPT_DotMatrixPrinterPrintCharacterPersian(LPT_DOT_MATRIX_PRINTER_PERSIAN_MAP[(CurrentCharacter - 0xBF)][CurrentStat]);
		if(Status_Printer != OK_PRINTER)
			return Status_Printer;

		PreviousCharacter = CurrentCharacter;
	}

	return OK_PRINTER;
}

//**********************************************************************************************************

Status_Printer_TypeDef LPT_DotMatrixPrinterPrintNumbersPersian(char *NumbersPersian)
{
	Status_Printer_TypeDef Status_Printer;
	uint8_t CurrentCharacter = 0;
	uint8_t IndexString = 0;

 	while(NumbersPersian[IndexString])
	{
 		CurrentCharacter = NumbersPersian[IndexString++];

		if((CurrentCharacter >= 0x30) && (CurrentCharacter <= 0x39))
		{
			Status_Printer = LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_USER_DEFINED_SET_COMMAND, 3);
			if(Status_Printer != OK_PRINTER)
				return Status_Printer;

			Status_Printer = LPT_Printer_SendChar(CurrentCharacter);
			if(Status_Printer != OK_PRINTER)
				return Status_Printer;
		}
		else if(CurrentCharacter <= 0x7F)
		{
			Status_Printer = LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_NORMAL_SET_COMMAND, 3);
			if(Status_Printer != OK_PRINTER)
				return Status_Printer;

			Status_Printer = LPT_Printer_SendChar(CurrentCharacter);
			if(Status_Printer != OK_PRINTER)
				return Status_Printer;
		}
	}

	return OK_PRINTER;
}

//**********************************************************************************************************

Status_Printer_TypeDef LPT_DotMatrixPrinterSetHorizontalCursorPositioning(uint8_t Columns)
{
	uint16_t HorizontalPrintPosition;
	uint8_t Command[4] = {0x1B, 0x24, 0x00, 0x00};

	HorizontalPrintPosition = ((float)Columns * 450) / 80;
	
	Command[2] = HorizontalPrintPosition & 0xFF;
	Command[3] = (HorizontalPrintPosition >> 8) & 0xFF;

	return LPT_Printer_SendCommand(Command, 4);
}

//**********************************************************************************************************

Status_Printer_TypeDef LPT_DotMatrixPrinterBackspace(uint8_t Backspace)
{
	Status_Printer_TypeDef Status_Printer;

	for(int i = 0; i < Backspace; i++)
	{
		Status_Printer = LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_BACKSPACE_COMMAND);
		if(Status_Printer != OK_PRINTER)
			return Status_Printer;
	}

	return OK_PRINTER;
}

//**********************************************************************************************************

Status_Printer_TypeDef LPT_DotMatrixPrinterSpace(uint8_t Space)
{
	Status_Printer_TypeDef Status_Printer;

	Status_Printer = LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_USER_DEFINED_SET_COMMAND, 3);
	if(Status_Printer != OK_PRINTER)
		return Status_Printer;

	for(int i = 0; i < Space; i++)
	{
		Status_Printer = LPT_Printer_SendChar(0x20);
		if(Status_Printer != OK_PRINTER)
			return Status_Printer;
	}

	return OK_PRINTER;
}

//**********************************************************************************************************

Status_Printer_TypeDef LPT_DotMatrixPrinterPrintCharacterPersian(uint8_t CharacterPersian)
{
	Status_Printer_TypeDef Status_Printer;

	Status_Printer = LPT_DotMatrixPrinterBackspace();
	if(Status_Printer != OK_PRINTER)
		return Status_Printer;

	Status_Printer = LPT_Printer_SendChar(CharacterPersian);
	if(Status_Printer != OK_PRINTER)
		return Status_Printer;

	Status_Printer = LPT_DotMatrixPrinterBackspace();
	if(Status_Printer != OK_PRINTER)
		return Status_Printer;

	return OK_PRINTER;
}

//**********************************************************************************************************

uint8_t LPT_DotMatrixPrinterSelectPrintFont_01(void)
{
	uint8_t MaxCharacterToLine = 113;

	LPT_DotMatrixPrinterRestInitializeFont();
	LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_SELECT_BOLD_FONT_COMMAND, 2);
	LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_SELECT_CONDENSED_PRINTING_COMMAND);

	return MaxCharacterToLine;
}

//**********************************************************************************************************

uint8_t LPT_DotMatrixPrinterSelectPrintFont_02(void)
{
	uint8_t MaxCharacterToLine = 66;

	LPT_DotMatrixPrinterRestInitializeFont();
	LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_SELECT_BOLD_FONT_COMMAND, 2);

	return MaxCharacterToLine;
}

//**********************************************************************************************************

uint8_t LPT_DotMatrixPrinterSelectPrintFont_03(void)
{
	uint8_t MaxCharacterToLine = 56;

	LPT_DotMatrixPrinterRestInitializeFont();
	LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_SELECT_BOLD_FONT_COMMAND, 2);
	LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_SELECT_CONDENSED_PRINTING_COMMAND);
	LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_TURN_ON_DOUBLE_WIDTH_PRINTING_COMMAND, 3);

	return MaxCharacterToLine;
}

//**********************************************************************************************************

uint8_t LPT_DotMatrixPrinterSelectPrintFont_04(void)
{
	uint8_t MaxCharacterToLine = 56;

	LPT_DotMatrixPrinterRestInitializeFont();
	LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_SELECT_BOLD_FONT_COMMAND, 2);
	LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_SELECT_CONDENSED_PRINTING_COMMAND);
	LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_TURN_ON_DOUBLE_WIDTH_PRINTING_COMMAND, 3);
	LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_TURN_ON_DOUBLE_HEIGHT_PRINTING_COMMAND, 3);

	return MaxCharacterToLine;
}

//**********************************************************************************************************
//**********************************************************************************************************
//**********************************************************************************************************

Status_Printer_TypeDef LPT_LaserPrinterInitialize(void)
{
	LPT_STROB_SET;
	LPT_STROB_SET;
	DELAY_MS(100);
	LPT_Printer_SendCommand(PCL_PRINTER_RESET_COMMAND, 2);
	DELAY_MS(100);
	return LPT_LaserPrinterDefiningCharacters();
}

//**********************************************************************************************************

Status_Printer_TypeDef LPT_LaserPrinterDefiningCharacters(void)
{
	Status_Printer_TypeDef Status_Printer;

	Status_Printer = LPT_Printer_SendCommand(PCL_PRINTER_RESET_COMMAND, 2);
	if(Status_Printer != OK_PRINTER)
		return Status_Printer;

//----------------------------------------------------------------------------------------------------------

	Status_Printer = LPT_Printer_SendCommand(PCL_PRINTER_DEFINE_FONT_ID_01_COMMAND, 5);
	if(Status_Printer != OK_PRINTER)
		return Status_Printer;
	
	Status_Printer = LPT_Printer_SendCommand(PCL_PRINTER_CREATE_FONT_HEADER_OF_64_BYTES_COMMAND, 6);
	if(Status_Printer != OK_PRINTER)
		return Status_Printer;
	
	Status_Printer = LPT_Printer_SendCommand(PCL_PRINTER_FONT_HEADER_BITMAPPED_FONTS, 65);
	if(Status_Printer != OK_PRINTER)
		return Status_Printer;
	
//----------------------------------------------------------------------------------------------------------
	
	for(uint32_t i = 0; i < sizeof(LPT_LASER_PRINTER_DATA); i++)
	{
		Status_Printer = LPT_Printer_SendChar(LPT_LASER_PRINTER_DATA[i]);
		if(Status_Printer != OK_PRINTER)
			return Status_Printer;
	}

//----------------------------------------------------------------------------------------------------------

	Status_Printer = LPT_Printer_SendCommand(PCL_PRINTER_MAKE_FONT_PERMANENT_COMMAND, 5);
	if(Status_Printer != OK_PRINTER)
		return Status_Printer;

	Status_Printer = LPT_Printer_SendCommand(PCL_PRINTER_SELECT_PRIMARY_FONT_ID_01_COMMAND, 4);
	if(Status_Printer != OK_PRINTER)
		return Status_Printer;
	
//----------------------------------------------------------------------------------------------------------
	
	LPT_LaserPrinterRestInitializeFont();

//----------------------------------------------------------------------------------------------------------

	return OK_PRINTER;
}

//**********************************************************************************************************

void LPT_LaserPrinterRestInitializeFont(void)
{
	LPT_Printer_SendCommand(PCL_PRINTER_LINE_TERMINATION_COMMAND, 5);
	
	if(TypePaper)
	{
		LPT_Printer_SendCommand(PCL_PRINTER_PAGE_SIZE_A4_COMMAND, 6);
		LPT_Printer_SendCommand(PCL_PRINTER_PORTRAIT_COMMAND, 5);
	}
	else
	{
		LPT_Printer_SendCommand(PCL_PRINTER_PAGE_SIZE_A5_COMMAND, 6);
		LPT_Printer_SendCommand(PCL_PRINTER_LANDSCAPE_COMMAND, 5);
	}
	
	LPT_Printer_SendCommand(PCL_PRINTER_CARRIAGE_RETURN_COMMAND);	
}

//**********************************************************************************************************

uint8_t LPT_LaserPrinter_UTF8_To_Map(uint8_t Character)
{
	static uint8_t LastCharacter = 0;
	uint8_t Last;

    if(Character < 128)
    {
    	LastCharacter = 0;
        return Character;
    }

    Last = LastCharacter;
    LastCharacter = Character;

    switch(Last)
    {
		case 0xCA:
    	{
    		switch(Character)
			{
    			case 0xBE:
    				return 0xBF;
			}
    	}
    	case 0xD8:
    	{
			switch(Character)
			{
				case 0xA2:
    				return 0xC0;
				case 0xA7:
    				return 0xC1;
				case 0xA8:
    				return 0xC2;
				case 0xAA:
    				return 0xC4;
				case 0xAB:
    				return 0xC5;
				case 0xAC:
    				return 0xC6;
				case 0xAD:
    				return 0xC8;
				case 0xAE:
    				return 0xC9;
				case 0xAF:
    				return 0xCA;
				case 0xB0:
    				return 0xCB;
				case 0xB1:
    				return 0xCC;
				case 0xB2:
    				return 0xCD;
				case 0xB3:
    				return 0xCF;
				case 0xB4:
    				return 0xD0;
				case 0xB5:
    				return 0xD1;
				case 0xB6:
    				return 0xD2;
				case 0xB7:
    				return 0xD3;
				case 0xB8:
    				return 0xD4;
				case 0xB9:
    				return 0xD5;
				case 0xBA:
    				return 0xD6;
			}
    	}
    	case 0xD9:
    	{
			switch(Character)
			{
    			case 0x81:
    				return 0xD7;
				case 0x82:
    				return 0xD8;
				case 0x84:
    				return 0xDB;
				case 0x85:
    				return 0xDC;
				case 0x86:
    				return 0xDD;
				case 0x87:
    				return 0xDF;
				case 0x88:
    				return 0xDE;
				case 0xBE:
    				return 0xC3;
			}
    	}
    	case 0xDA:
    	{
			switch(Character)
			{
    			case 0x86:
    				return 0xC7;
				case 0x98:
    				return 0xCE;
				case 0xA9:
    				return 0xD9;
				case 0xAF:
    				return 0xDA;
			}
    	}
    	case 0xDB:
    	{
    		switch(Character)
			{
    			case 0x8C:
    				return 0xE0;
			}
    	}
	}

	return 0;
}

//**********************************************************************************************************

Status_Printer_TypeDef LPT_LaserPrinterPrintStringPersian(char *strPersian)
{
	Status_Printer_TypeDef Status_Printer;
	uint8_t Character;
	uint8_t CurrentCharacter = 0;
	uint8_t NextCharacter = 0;
	uint8_t PreviousCharacter = 0;
	uint8_t CurrentStat;
	uint8_t NextStat;
	uint8_t PreviousStat;
	uint8_t IndexString = 0;
	uint8_t String[100];
	uint8_t CountNumber = 0;
	uint8_t StringNumber[100];

	for (int i = 0; i < strlen(strPersian); i++)
	{
		Character = LPT_LaserPrinter_UTF8_To_Map(strPersian[i]);
		if (Character != 0)
			String[IndexString++] = Character;
	}

	String[IndexString] = 0;
	IndexString = 0;

//----------------------------------------------------------------------------------------------------------

 	while(String[IndexString])
	{
 		CurrentCharacter = String[IndexString++];
		NextCharacter = String[IndexString];

		if((CurrentCharacter >= 0x30) && (CurrentCharacter <= 0x39))
		{
			CountNumber++;
			StringNumber[CountNumber] = CurrentCharacter;
			
			if(!((NextCharacter >= 0x30) && (NextCharacter <= 0x39)))
			{
				for (int i = CountNumber; i > 0; i--)
				{
					Status_Printer = LPT_LaserPrinterPrintCharacterPersian((StringNumber[i] + 0x50));
					if(Status_Printer != OK_PRINTER)
						return Status_Printer;	
				}
				
				CountNumber = 0;
			}
			
			PreviousCharacter = 0;
			continue;
		}
		else if(CurrentCharacter == 0x3F)
		{
			Status_Printer = LPT_LaserPrinterPrintCharacterPersian(0x8C);
			if(Status_Printer != OK_PRINTER)
				return Status_Printer;

			PreviousCharacter = 0;
			continue;
		}
		else if(CurrentCharacter == 0x2C)
		{
			Status_Printer = LPT_LaserPrinterPrintCharacterPersian(0x8A);
			if(Status_Printer != OK_PRINTER)
				return Status_Printer;

			PreviousCharacter = 0;
			continue;
		}
		else if(CurrentCharacter <= 0x7F)
		{
			Status_Printer = LPT_LaserPrinterPrintCharacterPersian(CurrentCharacter);
			if(Status_Printer != OK_PRINTER)
				return Status_Printer;

			PreviousCharacter = 0;
			continue;
		}
		else if(CurrentCharacter == 0xDB)
		{
			if(String[IndexString] == 0xC1)
			{
				Status_Printer = LPT_LaserPrinterPrintCharacterPersian(0xF2);
				if(Status_Printer != OK_PRINTER)
					return Status_Printer;

				PreviousCharacter = 0;
				IndexString++;
				continue;
			}
		}

//----------------------------------------------------------------------------------------------------------

		if(PreviousCharacter >= 0xBF)
			PreviousStat = LPT_LASERP_RINTER_PERSIAN_MAP[(PreviousCharacter - 0xBF)][5];
		else
			PreviousStat = 0;

		if(NextCharacter >= 0xBF)
			NextStat = LPT_LASERP_RINTER_PERSIAN_MAP[(NextCharacter - 0xBF)][4];
		else
			NextStat = 0;

		CurrentStat = (PreviousStat << 1) | NextStat;

		Status_Printer = LPT_LaserPrinterPrintCharacterPersian(LPT_LASERP_RINTER_PERSIAN_MAP[(CurrentCharacter - 0xBF)][CurrentStat]);
		if(Status_Printer != OK_PRINTER)
			return Status_Printer;

		PreviousCharacter = CurrentCharacter;
	}

	return OK_PRINTER;
}

//**********************************************************************************************************

Status_Printer_TypeDef LPT_LaserPrinterPrintNumbersPersian(char *NumbersPersian)
{
	Status_Printer_TypeDef Status_Printer;
	uint8_t CurrentCharacter = 0;
	uint8_t IndexString = 0;

 	while(NumbersPersian[IndexString])
	{
 		CurrentCharacter = NumbersPersian[IndexString++];

		if((CurrentCharacter >= 0x30) && (CurrentCharacter <= 0x39))
		{
			Status_Printer = LPT_Printer_SendChar((CurrentCharacter + 0x50));
			if(Status_Printer != OK_PRINTER)
				return Status_Printer;
		}
		else if(CurrentCharacter <= 0x7F)
		{
			Status_Printer = LPT_Printer_SendChar(CurrentCharacter);
			if(Status_Printer != OK_PRINTER)
				return Status_Printer;
		}
	}

	return OK_PRINTER;
}

//**********************************************************************************************************

Status_Printer_TypeDef LPT_LaserPrinterSetHorizontalCursorPositioning(uint8_t Columns)
{
	string Command = {0x1B, 0x26, 0x61, 0x30, 0x30, 0x43, 0x00};
	
	Command[3] = 0x30 + (Columns / 10);
	Command[4] = 0x30 + (Columns % 10);
			
	return LPT_Printer_SendCommand((uint8_t *)Command.c_str(), Command.length());
}

//**********************************************************************************************************

Status_Printer_TypeDef LPT_LaserPrinterBackspace(uint8_t Backspace)
{
	Status_Printer_TypeDef Status_Printer;

	for(int i = 0; i < Backspace; i++)
	{
		Status_Printer = LPT_Printer_SendCommand(PCL_PRINTER_BACKSPACE_COMMAND);
		if(Status_Printer != OK_PRINTER)
			return Status_Printer;
	}

	return OK_PRINTER;
}

//**********************************************************************************************************

Status_Printer_TypeDef LPT_LaserPrinterSpace(uint8_t Space)
{
	Status_Printer_TypeDef Status_Printer;

	for(int i = 0; i < Space; i++)
	{
		Status_Printer = LPT_Printer_SendChar(0x20);
		if(Status_Printer != OK_PRINTER)
			return Status_Printer;
	}

	return OK_PRINTER;
}

//**********************************************************************************************************

Status_Printer_TypeDef LPT_LaserPrinterPrintCharacterPersian(uint8_t CharacterPersian)
{
	Status_Printer_TypeDef Status_Printer;

	Status_Printer = LPT_LaserPrinterBackspace();
	if(Status_Printer != OK_PRINTER)
		return Status_Printer;

	Status_Printer = LPT_Printer_SendChar(CharacterPersian);
	if(Status_Printer != OK_PRINTER)
		return Status_Printer;

	Status_Printer = LPT_LaserPrinterBackspace();
	if(Status_Printer != OK_PRINTER)
		return Status_Printer;

	return OK_PRINTER;
}

//**********************************************************************************************************

uint8_t LPT_LaserPrinterSelectPrintFont_01(void)
{
	uint8_t MaxCharacterToLine = 70;

	LPT_LaserPrinterInitialize();

	return MaxCharacterToLine;
}

//**********************************************************************************************************

uint8_t LPT_LaserPrinterSelectPrintFont_02(void)
{
	uint8_t MaxCharacterToLine = 70;

	LPT_LaserPrinterInitialize();

	return MaxCharacterToLine;
}

//**********************************************************************************************************

uint8_t LPT_LaserPrinterSelectPrintFont_03(void)
{
	uint8_t MaxCharacterToLine = 70;

	LPT_LaserPrinterInitialize();

	return MaxCharacterToLine;
}

//**********************************************************************************************************

uint8_t LPT_LaserPrinterSelectPrintFont_04(void)
{
	uint8_t MaxCharacterToLine = 70;

	LPT_LaserPrinterInitialize();

	return MaxCharacterToLine;
}

//**********************************************************************************************************
//**********************************************************************************************************
//**********************************************************************************************************

Status_Printer_TypeDef Thermal_PrinterInitialize(void)
{
	Printer.SetPort(&huart1);
	Printer.begin();

	Printer.setDefault();
	Printer.justify('R');
	Printer.setCodePage(CODEPAGE_IRAN2);
	Printer.upsideDownOff();
	Printer.setFont('A');

	return OK_PRINTER;
}

//**********************************************************************************************************

uint8_t Thermal_Printer_UTF8_To_Map(uint8_t Character)
{
	static uint8_t LastCharacter = 0;
	uint8_t Last;

    if(Character < 128)
    {
    	LastCharacter = 0;
        return Character;
    }

    Last = LastCharacter;
    LastCharacter = Character;

    switch(Last)
    {
    	case 0xD8:
    	{
			switch(Character)
			{
				case 0xA2:
    				return 0xC0;
				case 0xA7:
    				return 0xC1;
				case 0xA8:
    				return 0xC2;
				case 0xAA:
    				return 0xC4;
				case 0xAB:
    				return 0xC5;
				case 0xAC:
    				return 0xC6;
				case 0xAD:
    				return 0xC8;
				case 0xAE:
    				return 0xC9;
				case 0xAF:
    				return 0xCA;
				case 0xB0:
    				return 0xCB;
				case 0xB1:
    				return 0xCC;
				case 0xB2:
    				return 0xCD;
				case 0xB3:
    				return 0xCF;
				case 0xB4:
    				return 0xD0;
				case 0xB5:
    				return 0xD1;
				case 0xB6:
    				return 0xD2;
				case 0xB7:
    				return 0xD3;
				case 0xB8:
    				return 0xD4;
				case 0xB9:
    				return 0xD5;
				case 0xBA:
    				return 0xD6;
			}
    	}
    	case 0xD9:
    	{
			switch(Character)
			{
    			case 0x81:
    				return 0xD7;
				case 0x82:
    				return 0xD8;
				case 0x84:
    				return 0xDB;
				case 0x85:
    				return 0xDC;
				case 0x86:
    				return 0xDD;
				case 0x87:
    				return 0xDF;
				case 0x88:
    				return 0xDE;
				case 0xBE:
    				return 0xC3;
			}
    	}
    	case 0xDA:
    	{
			switch(Character)
			{
    			case 0x86:
    				return 0xC7;
				case 0x98:
    				return 0xCE;
				case 0xA9:
    				return 0xD9;
				case 0xAF:
    				return 0xDA;
			}
    	}
    	case 0xDB:
    	{
    		switch(Character)
			{
    			case 0x8C:
    				return 0xE0;
			}
    	}
	}

	return 0;
}

//**********************************************************************************************************

Status_Printer_TypeDef Thermal_PrinterPrintStringPersian(char *strPersian)
{
	Status_Printer_TypeDef Status_Printer;
	uint8_t Character = 0;
	uint8_t CurrentCharacter = 0;
	uint8_t NextCharacter = 0;
	uint8_t PreviousCharacter = 0;
	uint8_t CurrentStat = 0;
	uint8_t NextStat = 0;
	uint8_t PreviousStat = 0;
	uint8_t IndexString = 0;
	uint8_t String[100] = {0};
	uint8_t CountNumber = 0;
	uint8_t StringNumber[100] = {0};
	string StringPersian = "";

	for (int i = 0; i < strlen(strPersian); i++)
	{
		Character = Thermal_Printer_UTF8_To_Map(strPersian[i]);
		if (Character != 0)
			String[IndexString++] = Character;
	}

	String[IndexString] = 0;
	IndexString = 0;

//----------------------------------------------------------------------------------------------------------

 	while(String[IndexString])
	{
 		CurrentCharacter = String[IndexString++];
		NextCharacter = String[IndexString];

		if((CurrentCharacter >= 0x30) && (CurrentCharacter <= 0x39))
		{
			CountNumber++;
			StringNumber[CountNumber] = CurrentCharacter;

			if(!((NextCharacter >= 0x30) && (NextCharacter <= 0x39)))
			{
				for (int i = CountNumber; i > 0; i--)
					StringPersian += (0x50 + StringNumber[i]);

				CountNumber = 0;
			}

			PreviousCharacter = 0;
			continue;
		}
		else if(CurrentCharacter == 0x3F)
		{
			StringPersian += 0x8C;
			PreviousCharacter = 0;
			continue;
		}
		else if(CurrentCharacter == 0x2C)
		{
			StringPersian += 0x8A;
			PreviousCharacter = 0;
			continue;
		}
		else if(CurrentCharacter <= 0x7F)
		{
			StringPersian += CurrentCharacter;
			PreviousCharacter = 0;
			continue;
		}

//----------------------------------------------------------------------------------------------------------

		if(PreviousCharacter >= 0xBF)
			PreviousStat = THERMAL_PRINTER_PERSIAN_MAP[(PreviousCharacter - 0xBF)][5];
		else
			PreviousStat = 0;

		if(NextCharacter >= 0xBF)
			NextStat = THERMAL_PRINTER_PERSIAN_MAP[(NextCharacter - 0xBF)][4];
		else
			NextStat = 0;

		CurrentStat = (PreviousStat << 1) | NextStat;
		StringPersian += THERMAL_PRINTER_PERSIAN_MAP[(CurrentCharacter - 0xBF)][CurrentStat];
		PreviousCharacter = CurrentCharacter;
	}

 	for (int i = strlen(StringPersian.c_str()); i >= 0; i--)
 	{
 		Status_Printer = Thermal_PrinterPrintCharacterPersian(StringPersian[i]);
		if(Status_Printer != OK_PRINTER)
			return Status_Printer;
 	}

	return OK_PRINTER;
}

//**********************************************************************************************************

Status_Printer_TypeDef Thermal_PrinterPrintNumbersPersian(char *NumbersPersian)
{
	uint8_t CurrentCharacter = 0;
	uint8_t IndexString = 0;

 	while(NumbersPersian[IndexString])
	{
 		CurrentCharacter = NumbersPersian[IndexString++];

		if((CurrentCharacter >= 0x30) && (CurrentCharacter <= 0x39))
			Printer.write((0x50 + CurrentCharacter));
		else if(CurrentCharacter <= 0x7F)
			Printer.write(CurrentCharacter);
	}

	return OK_PRINTER;
}

//**********************************************************************************************************

Status_Printer_TypeDef Thermal_PrinterPrintCharacterPersian(uint8_t CharacterPersian)
{
	Printer.write(CharacterPersian);
	return OK_PRINTER;
}

//**********************************************************************************************************

void Thermal_PrinterSelectPrintFont_01(void)
{
	Printer.boldOn();
	Printer.doubleHeightOff();
	Printer.doubleWidthOff();
}

//**********************************************************************************************************

void Thermal_PrinterSelectPrintFont_02(void)
{
	Printer.boldOn();
	Printer.doubleHeightOn();
	Printer.doubleWidthOff();
}

//**********************************************************************************************************

void Thermal_PrinterSelectPrintFont_03(void)
{
	Printer.boldOn();
	Printer.doubleHeightOn();
	Printer.doubleWidthOn();
}

//**********************************************************************************************************

void Thermal_PrinterEject(void)
{
	Printer.feed(1);
}

//**********************************************************************************************************

void Thermal_Printerfeed(uint8_t Line)
{
	Printer.feed(Line);
}

//**********************************************************************************************************

void Thermal_PrinterJustify(char Value)
{
	Printer.justify(Value);
}

//**********************************************************************************************************
//**********************************************************************************************************
//**********************************************************************************************************

void Printer_SetPrinterType(uint8_t PrinterType , bool PaperType)
{
	TypePrinter = PrinterType;
	TypePaper = PaperType;
}

//**********************************************************************************************************

Status_Printer_TypeDef Printer_Initialize(void)
{
	if(TypePrinter == 0)
		return LPT_DotMatrixPrinterInitialize();
	else
		return OK_PRINTER;
}

//**********************************************************************************************************

Status_Printer_TypeDef Printer_SendChar(uint8_t Char)
{
	if(TypePrinter == 0)
		return LPT_Printer_SendChar(Char);
	else if(TypePrinter == 1)
		return LPT_Printer_SendChar(Char);
	else if(TypePrinter == 2)
		return ERROR_PRINTER;
}

//**********************************************************************************************************

Status_Printer_TypeDef Printer_SendString(string String)
{
	if(TypePrinter == 0)
		return LPT_Printer_SendString(String);
	else if(TypePrinter == 1)
		return LPT_Printer_SendString(String);
	else if(TypePrinter == 2)
		return ERROR_PRINTER;
}

//**********************************************************************************************************
		
Status_Printer_TypeDef Printer_SendCommand(Command_Printer_TypeDef Command_Printer)
{
	switch(Command_Printer)
	{
	  	case PRINTER_LINE_FEED_COMMAND:
		{
			if(TypePrinter == 0)
				return LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_LINE_FEED_COMMAND);
			else if(TypePrinter == 1)
				return LPT_Printer_SendCommand(PCL_PRINTER_LINE_FEED_COMMAND, 2);
			else if(TypePrinter == 2)
				return ERROR_PRINTER;
	
			break;
		}
		case PRINTER_EJECT_PAPER_COMMAND:
		{
			if(TypePrinter == 0)
				return LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_EJECT_PAPER_COMMAND);
			else if(TypePrinter == 1)
				return LPT_Printer_SendCommand(PCL_PRINTER_SIMPLEX_COMMAND, 5);
			else if(TypePrinter == 2)
				return ERROR_PRINTER;

			break;
		}
		case PRINTER_USER_DEFINED_SET_COMMAND:
		{
			if(TypePrinter == 0)
				return LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_USER_DEFINED_SET_COMMAND, 3);
			else if(TypePrinter == 1)
				return OK_PRINTER;
			else if(TypePrinter == 2)
				return OK_PRINTER;
	
			break;
		}
		case PRINTER_NORMAL_SET_COMMAND:
		{
			if(TypePrinter == 0)
				return LPT_Printer_SendCommand(LPT_DOT_MATRIX_PRINTER_NORMAL_SET_COMMAND, 3);
			else if(TypePrinter == 1)
				return OK_PRINTER;
			else if(TypePrinter == 2)
				return OK_PRINTER;
			
			break;
		}
	}
}
			 
//**********************************************************************************************************
			 
Status_Printer_TypeDef Printer_CheckPaper(void)
{
	if(TypePrinter == 0)
		return LPT_Printer_CheckPaper();	
	else if(TypePrinter == 1)
		return LPT_Printer_CheckPaper();
	else if(TypePrinter == 2)
		return ERROR_PRINTER;
}
			 
//**********************************************************************************************************			 
			 
Status_Printer_TypeDef Printer_PrintNumbersPersian(char *NumbersPersian)
{
	if(TypePrinter == 0)
		return LPT_DotMatrixPrinterPrintNumbersPersian(NumbersPersian);	
	else if(TypePrinter == 1)
		return LPT_LaserPrinterPrintNumbersPersian(NumbersPersian);
	else if(TypePrinter == 2)
		return ERROR_PRINTER;
}

//**********************************************************************************************************

Status_Printer_TypeDef Printer_PrintStringPersian(char *strPersian)
{
	if(TypePrinter == 0)
		return LPT_DotMatrixPrinterPrintStringPersian(strPersian);
	else if(TypePrinter == 1)
		return LPT_LaserPrinterPrintStringPersian(strPersian);
	else if(TypePrinter == 2)
		return ERROR_PRINTER;
}

//**********************************************************************************************************

Status_Printer_TypeDef Printer_SetHorizontalCursorPositioning(uint8_t Columns)
{
	if(TypePrinter == 0)
		return LPT_DotMatrixPrinterSetHorizontalCursorPositioning(Columns);
	else if(TypePrinter == 1)
		return LPT_LaserPrinterSetHorizontalCursorPositioning(Columns);
	else if(TypePrinter == 2)
		return ERROR_PRINTER;
}

//**********************************************************************************************************

Status_Printer_TypeDef Printer_Backspace(uint8_t Backspace)
{
	if(TypePrinter == 0)
		return LPT_DotMatrixPrinterBackspace(Backspace);
	else if(TypePrinter == 1)
		return LPT_LaserPrinterBackspace(Backspace);
	else if(TypePrinter == 2)
		return ERROR_PRINTER;
}

//**********************************************************************************************************

Status_Printer_TypeDef Printer_Space(uint8_t Space)
{
	if(TypePrinter == 0)
		return LPT_DotMatrixPrinterSpace(Space);
	else if(TypePrinter == 1)
		return LPT_LaserPrinterSpace(Space);
	else if(TypePrinter == 2)
		return ERROR_PRINTER;
}

//**********************************************************************************************************

uint8_t Printer_SelectPrintFont_01(void)
{
	if(TypePrinter == 0)
		return LPT_DotMatrixPrinterSelectPrintFont_01();
	else if(TypePrinter == 1)
		return LPT_LaserPrinterSelectPrintFont_01();
	else if(TypePrinter == 2)
		return 0;
}

//**********************************************************************************************************

uint8_t Printer_SelectPrintFont_02(void)
{
	if(TypePrinter == 0)
		return LPT_DotMatrixPrinterSelectPrintFont_02();
	else if(TypePrinter == 1)
		return LPT_LaserPrinterSelectPrintFont_02();
	else if(TypePrinter == 2)
		return 0;
}

//**********************************************************************************************************

uint8_t Printer_SelectPrintFont_03(void)
{
	if(TypePrinter == 0)
		return LPT_DotMatrixPrinterSelectPrintFont_03();
	else if(TypePrinter == 1)
		return LPT_LaserPrinterSelectPrintFont_03();
	else if(TypePrinter == 2)
		return 0;
}

//**********************************************************************************************************

uint8_t Printer_SelectPrintFont_04(void)
{
	if(TypePrinter == 0)
		return LPT_DotMatrixPrinterSelectPrintFont_04();
	else if(TypePrinter == 1)
		return LPT_LaserPrinterSelectPrintFont_04();
	else if(TypePrinter == 2)
		return 0;
}

//**********************************************************************************************************
















