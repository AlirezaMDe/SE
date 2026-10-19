/*-----------------------------------------------------------------------------------------------------------
@file          	: LCD_WG24064.cpp
@author  		: Reza Farahani (nmmco2015@gmail.com)
C++ source 2024
-----------------------------------------------------------------------------------------------------------*/

#include "LCD_WG24064.h"

//**********************************************************************************************************

void Delay_us(uint32_t Delay)
{
	for(uint32_t i = (Delay * SYSTEM_CORE_CLOCK); i > 0; i--)
	{
		__ASM("NOP");
	}
}

//**********************************************************************************************************

void WG24064_Send(uint8_t Command)
{
#if(WG24064_CUSTOM_PIN)
{
	if(command & 0X01)
		LCD_DB0_SET;
	else
		LCD_DB0_RESET;
	if(command & 0X02)
		LCD_DB1_SET;
	else
		LCD_DB1_RESET;
	if(command & 0X04)
		LCD_DB2_SET;
	else
		LCD_DB2_RESET;
	if(command & 0X08)
		LCD_DB3_SET;
	else
		LCD_DB3_RESET;
	if(command & 0X10)
		LCD_DB4_SET;
	else
		LCD_DB4_RESET;
	if(command & 0X20)
		LCD_DB5_SET;
	else
		LCD_DB5_RESET;
	if(command & 0X40)
		LCD_DB6_SET;
	else
		LCD_DB6_RESET;
	if(command & 0X80)
		LCD_DB7_SET;
	else
		LCD_DB7_RESET;
}
#else
{
	HAL_GPIO_WritePin(LCD_DB0_GPIO_Port, 0xFF, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_DB0_GPIO_Port, Command, GPIO_PIN_SET);
}
#endif
}

//**********************************************************************************************************

void WG24064_WriteCommand(uint8_t Command)
{
	WG24064_Send(Command);

	LCD_WR_RESET;
	LCD_CE_RESET;
	Delay_us(1);
	LCD_CE_SET;
	LCD_WR_SET;
}

//**********************************************************************************************************

void WG24064_WriteData(uint8_t Data)
{
	WG24064_Send(Data);

	LCD_CD_RESET;
	LCD_WR_RESET;
	LCD_CE_RESET;
	Delay_us(1);
	LCD_CE_SET;
	LCD_WR_SET;
	LCD_CD_SET;
}

//**********************************************************************************************************

void WG24064_SetAddressPointer(uint16_t Address)
{
	WG24064_WriteData(Address & 0xFF);
	WG24064_WriteData(Address >> 8);
	WG24064_WriteCommand(WG24064_SET_ADDRESS_POINTER);
}

//**********************************************************************************************************

void WG24064_WriteDisplayData(uint8_t DisplayData)
{
	WG24064_WriteData(DisplayData);
	WG24064_WriteCommand(WG24064_DATA_WRITE_AND_INCREMENT);
}

//**********************************************************************************************************

void WG24064_Initialization(void)
{
	LCD_BLK_RESET;
	LCD_RD_SET;
	LCD_CD_SET;
	LCD_WR_SET;
	LCD_CE_SET;

	LCD_RST_RESET;
	DELAY_MS(500);
	LCD_RST_SET;

	WG24064_WriteData(WG24064_GRAPHIC_HOME & 0xFF);
	WG24064_WriteData(WG24064_GRAPHIC_HOME >> 8);
	WG24064_WriteCommand(WG24064_SET_GRAPHIC_HOME_ADDRESS);

	WG24064_WriteData(WG24064_GRAPHIC_AREA);
	WG24064_WriteData(0x00);
	WG24064_WriteCommand(WG24064_SET_GRAPHIC_AREA);

	WG24064_WriteData(WG24064_TEXT_HOME);
	WG24064_WriteData(WG24064_TEXT_HOME >> 8);
	WG24064_WriteCommand(WG24064_SET_TEXT_HOME_ADDRESS);

	WG24064_WriteData(WG24064_TEXT_AREA);
	WG24064_WriteData(0x00);
	WG24064_WriteCommand(WG24064_SET_TEXT_AREA);

	WG24064_WriteData(WG24064_OFFSET_REGISTER);
	WG24064_WriteData(0x00);
	WG24064_WriteCommand(WG24064_SET_OFFSET_REGISTER);

	WG24064_WriteCommand( WG24064_DISPLAY_MODE | WG24064_GRAPHIC_DISPLAY_ON |	WG24064_TEXT_DISPLAY_ON );

	WG24064_WriteCommand(WG24064_MODE_SET | 0x00);

	WG24064_Clear();

	LCD_BLK_SET;
}

//**********************************************************************************************************

void WG24064_Clear_CG(void)
{
	WG24064_SetAddressPointer(WG24064_EXTERNAL_CG_HOME);

	for(int  i = 0; i < (256 * 8); i++)
		WG24064_WriteDisplayData(0x00);
}

//**********************************************************************************************************

void WG24064_Clear_Text(void)
{
	WG24064_SetAddressPointer(WG24064_TEXT_HOME);

	for(int i = 0; i < WG24064_TEXT_SIZE; i++)
		WG24064_WriteDisplayData(0x00);
}

//**********************************************************************************************************

void WG24064_Clear_Graphic(void)
{
	WG24064_SetAddressPointer(WG24064_GRAPHIC_HOME);

	for(int i = 0; i < WG24064_GRAPHIC_SIZE; i++)
		WG24064_WriteDisplayData(0x00);
}

//**********************************************************************************************************

void WG24064_Clear(void)
{
	WG24064_Clear_CG();
	WG24064_Clear_Text();
	WG24064_Clear_Graphic();
}

//**********************************************************************************************************

void WG24064_TextGoTo(uint8_t X, uint8_t Y)
{
	uint16_t Address;

	Address = (WG24064_TEXT_HOME + X) + (WG24064_TEXT_AREA * Y);
	WG24064_SetAddressPointer(Address);
}

//**********************************************************************************************************

void WG24064_GraphicGoTo(uint8_t X, uint8_t Y)
{
	uint16_t Address;

	Address = WG24064_GRAPHIC_HOME +(X / WG24064_FONT_WIDTH) + (WG24064_GRAPHIC_AREA *Y);
	WG24064_SetAddressPointer(Address);
}

//**********************************************************************************************************

void WG24064_GoToPixel(uint8_t X, uint8_t Y)
{
	uint16_t Address;
	if((X < WG24064_SCREEN_WIDTH) && (Y < WG24064_SCREEN_HEIGHT))
	{
		Address = WG24064_GRAPHIC_HOME + (X / WG24064_FONT_WIDTH) + (WG24064_GRAPHIC_AREA * Y);
		WG24064_SetAddressPointer(Address);
	}
}

//**********************************************************************************************************

void WG24064_PutChar(char Char)
{
	WG24064_WriteDisplayData(Char - 32);
}

//**********************************************************************************************************

void WG24064_PutString(char *str)
{
	while(*str)
		WG24064_PutChar(*str++);
}

//**********************************************************************************************************
