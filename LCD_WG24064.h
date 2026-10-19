/*-------------------------------------------------------------------------------------------------------------
@file           : LCD_WG24064.h
@author  		: Reza Farahani (nmmco2015@gmail.com)
@brief          : Header for LCD_WG24064.cpp file.
-------------------------------------------------------------------------------------------------------------*/

#ifndef __LCD_WG24064_H
#define __LCD_WG24064_H

#ifdef __cplusplus
extern "C" {
#endif

//**********************************************************************************************************

#include "main.h"
#include "cmsis_os.h"

//**********************************************************************************************************

#define DELAY_MS(Delay_mS)		        osDelay(Delay_mS)  
  
//**********************************************************************************************************

#define SYSTEM_CORE_CLOCK       		168  	//168 Mhz
#define WG24064_CUSTOM_PIN     			0 		//0 = GLCD DATA PORT CONFIG ONLY PORT / 1 = GLCD DATA PORT CONFIG ANY PIN

//**********************************************************************************************************

#define WG24064_SCREEN_WIDTH          	240
#define WG24064_SCREEN_HEIGHT         	64
#define	WG24064_SCREEN_SIZE_PAGE		8
#define WG24064_SCREEN_PAGES			(WG24064_SCREEN_HEIGHT / WG24064_SCREEN_SIZE_PAGE)

//**********************************************************************************************************

#define WG24064_FONT_WIDTH								8

#define WG24064_GRAPHIC_AREA							(WG24064_SCREEN_WIDTH / WG24064_FONT_WIDTH)
#define WG24064_TEXT_AREA								(WG24064_SCREEN_WIDTH / WG24064_FONT_WIDTH)
#define WG24064_GRAPHIC_SIZE							(WG24064_GRAPHIC_AREA * WG24064_SCREEN_HEIGHT)
#define WG24064_TEXT_SIZE								(WG24064_TEXT_AREA * (WG24064_SCREEN_HEIGHT / 8))

#define WG24064_TEXT_HOME								0
#define WG24064_OFFSET_REGISTER							2

#define WG24064_GRAPHIC_HOME							(WG24064_TEXT_HOME + WG24064_TEXT_SIZE)
#define WG24064_EXTERNAL_CG_HOME						(WG24064_OFFSET_REGISTER <<11)

#define WG24064_SET_CURSOR_POINTER						0x21
#define WG24064_SET_OFFSET_REGISTER						0x22
#define WG24064_SET_ADDRESS_POINTER						0x24

#define WG24064_SET_TEXT_HOME_ADDRESS					0x40
#define WG24064_SET_TEXT_AREA							0x41
#define WG24064_SET_GRAPHIC_HOME_ADDRESS				0x42
#define WG24064_SET_GRAPHIC_AREA						0x43

#define WG24064_BUSY_FLAG								0x03

#define WG24064_MODE_SET								0x80

#define WG24064_DISPLAY_MODE							0x90
#define WG24064_CURSOR_BLINK_ON							0x01
#define WG24064_CURSOR_DISPLAY_ON						0x02
#define WG24064_TEXT_DISPLAY_ON							0x04
#define WG24064_GRAPHIC_DISPLAY_ON						0x08

#define WG24064_CURSOR_PATTERN_SELECT					0xA0
#define WG24064_CURSOR_1_LINE							0x00
#define WG24064_CURSOR_2_LINE							0x01
#define WG24064_CURSOR_3_LINE							0x02
#define WG24064_CURSOR_4_LINE							0x03
#define WG24064_CURSOR_5_LINE							0x04
#define WG24064_CURSOR_6_LINE							0x05
#define WG24064_CURSOR_7_LINE							0x06
#define WG24064_CURSOR_8_LINE							0x07

#define WG24064_SET_DATA_AUTO_WRITE						0xB0
#define WG24064_SET_DATA_AUTO_READ						0xB1
#define WG24064_AUTO_RESET								0xB2

#define WG24064_DATA_WRITE_AND_INCREMENT				0xC0
#define WG24064_DATA_READ_AND_INCREMENT					0xC1
#define WG24064_DATA_WRITE_AND_DECREMENT				0xC2
#define WG24064_DATA_READ_AND_DECREMENT					0xC3
#define WG24064_DATA_WRITE_AND_NONVARIALBE				0xC4
#define WG24064_DATA_READ_AND_NONVARIABLE				0xC5

#define WG24064_SCREEN_PEEK								0xE0
#define WG24064_SCREEN_COPY								0xE8

//**********************************************************************************************************

#define LCD_DB0_SET					HAL_GPIO_WritePin(LCD_DB0_GPIO_Port, LCD_DB0_Pin, GPIO_PIN_SET)
#define LCD_DB0_RESET    			HAL_GPIO_WritePin(LCD_DB0_GPIO_Port, LCD_DB0_Pin, GPIO_PIN_RESET)

#define LCD_DB1_SET					HAL_GPIO_WritePin(LCD_DB1_GPIO_Port, LCD_DB1_Pin, GPIO_PIN_SET)
#define LCD_DB1_RESET    			HAL_GPIO_WritePin(LCD_DB1_GPIO_Port, LCD_DB1_Pin, GPIO_PIN_RESET)

#define LCD_DB2_SET					HAL_GPIO_WritePin(LCD_DB2_GPIO_Port, LCD_DB2_Pin, GPIO_PIN_SET)
#define LCD_DB2_RESET    			HAL_GPIO_WritePin(LCD_DB2_GPIO_Port, LCD_DB2_Pin, GPIO_PIN_RESET)

#define LCD_DB3_SET					HAL_GPIO_WritePin(LCD_DB3_GPIO_Port, LCD_DB3_Pin, GPIO_PIN_SET)
#define LCD_DB3_RESET    			HAL_GPIO_WritePin(LCD_DB3_GPIO_Port, LCD_DB3_Pin, GPIO_PIN_RESET)

#define LCD_DB4_SET					HAL_GPIO_WritePin(LCD_DB4_GPIO_Port, LCD_DB4_Pin, GPIO_PIN_SET)
#define LCD_DB4_RESET    			HAL_GPIO_WritePin(LCD_DB4_GPIO_Port, LCD_DB4_Pin, GPIO_PIN_RESET)

#define LCD_DB5_SET					HAL_GPIO_WritePin(LCD_DB5_GPIO_Port, LCD_DB5_Pin, GPIO_PIN_SET)
#define LCD_DB5_RESET    			HAL_GPIO_WritePin(LCD_DB5_GPIO_Port, LCD_DB5_Pin, GPIO_PIN_RESET)

#define LCD_DB6_SET					HAL_GPIO_WritePin(LCD_DB6_GPIO_Port, LCD_DB6_Pin, GPIO_PIN_SET)
#define LCD_DB6_RESET    			HAL_GPIO_WritePin(LCD_DB6_GPIO_Port, LCD_DB6_Pin, GPIO_PIN_RESET)

#define LCD_DB7_SET					HAL_GPIO_WritePin(LCD_DB7_GPIO_Port, LCD_DB7_Pin, GPIO_PIN_SET)
#define LCD_DB7_RESET    			HAL_GPIO_WritePin(LCD_DB7_GPIO_Port, LCD_DB7_Pin, GPIO_PIN_RESET)

#define LCD_RD_SET					HAL_GPIO_WritePin(LCD_RD_GPIO_Port, LCD_RD_Pin, GPIO_PIN_SET)
#define LCD_RD_RESET    			HAL_GPIO_WritePin(LCD_RD_GPIO_Port, LCD_RD_Pin, GPIO_PIN_RESET)

#define LCD_CD_SET					HAL_GPIO_WritePin(LCD_CD_GPIO_Port, LCD_CD_Pin, GPIO_PIN_SET)
#define LCD_CD_RESET    			HAL_GPIO_WritePin(LCD_CD_GPIO_Port, LCD_CD_Pin, GPIO_PIN_RESET)

#define LCD_RST_SET					HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET)
#define LCD_RST_RESET    			HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET)

#define LCD_WR_SET					HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, GPIO_PIN_SET)
#define LCD_WR_RESET    			HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, GPIO_PIN_RESET)

#define LCD_CE_SET					HAL_GPIO_WritePin(LCD_CE_GPIO_Port, LCD_CE_Pin, GPIO_PIN_SET)
#define LCD_CE_RESET    			HAL_GPIO_WritePin(LCD_CE_GPIO_Port, LCD_CE_Pin, GPIO_PIN_RESET)

#define LCD_BLK_SET					HAL_GPIO_WritePin(LCD_BLK_GPIO_Port, LCD_BLK_Pin, GPIO_PIN_SET)
#define LCD_BLK_RESET    			HAL_GPIO_WritePin(LCD_BLK_GPIO_Port, LCD_BLK_Pin, GPIO_PIN_RESET)

//**********************************************************************************************************

void Delay_us(uint32_t Delay);
void WG24064_Send(uint8_t Command);

void WG24064_WriteCommand(uint8_t Command);
void WG24064_WriteData(uint8_t Data);

void WG24064_SetAddressPointer(uint16_t Address);
void WG24064_WriteDisplayData(uint8_t DisplayData);

void WG24064_Clear_CG(void);
void WG24064_Clear_Text(void);
void WG24064_Clear_Graphic(void);
void WG24064_Clear(void);

void WG24064_Initialization(void);

void WG24064_TextGoTo(uint8_t X, uint8_t Y);
void WG24064_GraphicGoTo(uint8_t X, uint8_t Y);

void WG24064_GoToPixel(uint8_t X, uint8_t Y);

void WG24064_PutChar(char Char);
void WG24064_PutString(char *str);

//**********************************************************************************************************

#ifdef __cplusplus
}
#endif
#endif /* __LCD_WG24064_H */
