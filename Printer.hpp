/*-------------------------------------------------------------------------------------------------------------
@file           : Printer.hpp
@author  		: Reza Farahani (nmmco2015@gmail.com)
@brief          : Header for Printer.cpp file.
-------------------------------------------------------------------------------------------------------------*/

#ifndef __PRINTER_H
#define __PRINTER_H

//**********************************************************************************************************

#include "User_Defined_Type.hpp"

//**********************************************************************************************************

#define SYSTEM_CORE_CLOCK       			168  	//168 Mhz
#define LPT_PRINTER_CUSTOM_PIN     			0 		//0 = LPT_Printer DATA PORT CONFIG ONLY PORT / 1 = LPT_Printer DATA PORT CONFIG ANY PIN
#define LPT_PRINTER_TIME_OUT				1000

//**********************************************************************************************************

#define DELAY_MS(Delay_mS)			osDelay(Delay_mS)

//**********************************************************************************************************

#define LPT_D0_SET					HAL_GPIO_WritePin(LPT_D0_GPIO_Port, LPT_D0_Pin, GPIO_PIN_SET)
#define LPT_D0_RESET    			HAL_GPIO_WritePin(LPT_D0_GPIO_Port, LPT_D0_Pin, GPIO_PIN_RESET)

#define LPT_D1_SET					HAL_GPIO_WritePin(LPT_D1_GPIO_Port, LPT_D1_Pin, GPIO_PIN_SET)
#define LPT_D1_RESET    			HAL_GPIO_WritePin(LPT_D1_GPIO_Port, LPT_D1_Pin, GPIO_PIN_RESET)

#define LPT_D2_SET					HAL_GPIO_WritePin(LPT_D2_GPIO_Port, LPT_D2_Pin, GPIO_PIN_SET)
#define LPT_D2_RESET    			HAL_GPIO_WritePin(LPT_D2_GPIO_Port, LPT_D2_Pin, GPIO_PIN_RESET)

#define LPT_D3_SET					HAL_GPIO_WritePin(LPT_D3_GPIO_Port, LPT_D3_Pin, GPIO_PIN_SET)
#define LPT_D3_RESET    			HAL_GPIO_WritePin(LPT_D3_GPIO_Port, LPT_D3_Pin, GPIO_PIN_RESET)

#define LPT_D4_SET					HAL_GPIO_WritePin(LPT_D4_GPIO_Port, LPT_D4_Pin, GPIO_PIN_SET)
#define LPT_D4_RESET    			HAL_GPIO_WritePin(LPT_D4_GPIO_Port, LPT_D4_Pin, GPIO_PIN_RESET)

#define LPT_D5_SET					HAL_GPIO_WritePin(LPT_D5_GPIO_Port, LPT_D5_Pin, GPIO_PIN_SET)
#define LPT_D5_RESET    			HAL_GPIO_WritePin(LPT_D5_GPIO_Port, LPT_D5_Pin, GPIO_PIN_RESET)

#define LPT_D6_SET					HAL_GPIO_WritePin(LPT_D6_GPIO_Port, LPT_D6_Pin, GPIO_PIN_SET)
#define LPT_D6_RESET    			HAL_GPIO_WritePin(LPT_D6_GPIO_Port, LPT_D6_Pin, GPIO_PIN_RESET)

#define LPT_D7_SET					HAL_GPIO_WritePin(LPT_D7_GPIO_Port, LPT_D7_Pin, GPIO_PIN_SET)
#define LPT_D7_RESET    			HAL_GPIO_WritePin(LPT_D7_GPIO_Port, LPT_D7_Pin, GPIO_PIN_RESET)

#define LPT_STROB_SET				HAL_GPIO_WritePin(LPT_STROB_GPIO_Port, LPT_STROB_Pin, GPIO_PIN_SET)
#define LPT_STROB_RESET    			HAL_GPIO_WritePin(LPT_STROB_GPIO_Port, LPT_STROB_Pin, GPIO_PIN_RESET)

//----------------------------------------------------------------------------------------------------------

#define IS_RESET_LPT_BUSY			HAL_GPIO_ReadPin(LPT_BUSY_GPIO_Port, LPT_BUSY_Pin) == GPIO_PIN_RESET
#define IS_RESET_LPT_PAPER			HAL_GPIO_ReadPin(LPT_PAPER_GPIO_Port, LPT_PAPER_Pin) == GPIO_PIN_RESET

//**********************************************************************************************************

//General operation
const uint8_t LPT_DOT_MATRIX_PRINTER_RESET_COMMAND[] = {0x1B, 0x40};
const uint8_t LPT_DOT_MATRIX_PRINTER_TURN_UNIDIRECTIONAL_MODE_ON_COMMAND[] = {0x1B, 0x55, 0x31};
const uint8_t LPT_DOT_MATRIX_PRINTER_TURN_UNIDIRECTIONAL_MODE_OFF_COMMAND[] = {0x1B, 0x55, 0x30};

//----------------------------------------------------------------------------------------------------------
//Paper feeding
const uint8_t LPT_DOT_MATRIX_PRINTER_BACKSPACE_COMMAND[] = {0x08};
const uint8_t LPT_DOT_MATRIX_PRINTER_LINE_FEED_COMMAND[] = {0x0A};
const uint8_t LPT_DOT_MATRIX_PRINTER_CARRIAGE_RETURN_COMMAND[] = {0x0D};
const uint8_t LPT_DOT_MATRIX_PRINTER_EJECT_PAPER_COMMAND[] = {0x0C};

//----------------------------------------------------------------------------------------------------------
//Font selection
const uint8_t LPT_DOT_MATRIX_PRINTER_SELECT_FONT_10_CPI_COMMAND[] = {0x1B, 0x50};
const uint8_t LPT_DOT_MATRIX_PRINTER_SELECT_FONT_12_CPI_COMMAND[] = {0x1B, 0x4D};
const uint8_t LPT_DOT_MATRIX_PRINTER_SELECT_FONT_15_CPI_COMMAND[] = {0x1B, 0x67};

const uint8_t LPT_DOT_MATRIX_PRINTER_SELECTS_PROPORTIONAL_SPACING_COMMAND[] = {0x1B, 0x70, 0x31};
const uint8_t LPT_DOT_MATRIX_PRINTER_CANCEL_PROPORTIONAL_SPACING_COMMAND[] = {0x1B, 0x70, 0x30};

const uint8_t LPT_DOT_MATRIX_PRINTER_SELECT_DRAFT_COMMAND[] = {0x1B, 0x78, 0x30};
const uint8_t LPT_DOT_MATRIX_PRINTER_SELECT_LETTER_QUALITY_COMMAND[] = {0x1B, 0x78, 0x31};

const uint8_t LPT_DOT_MATRIX_PRINTER_SELECT_ITALIC_FONT_COMMAND[] = {0x1B, 0x34};
const uint8_t LPT_DOT_MATRIX_PRINTER_CANCEL_ITALIC_FONT_COMMAND[] = {0x1B, 0x35};

const uint8_t LPT_DOT_MATRIX_PRINTER_SELECT_BOLD_FONT_COMMAND[] = {0x1B, 0x45};
const uint8_t LPT_DOT_MATRIX_PRINTER_CANCEL_BOLD_FONT_COMMAND[] = {0x1B, 0x46};

//----------------------------------------------------------------------------------------------------------
//Font enhancement
const uint8_t LPT_DOT_MATRIX_PRINTER_SELECT_CONDENSED_PRINTING_COMMAND[] = {0x0F};
const uint8_t LPT_DOT_MATRIX_PRINTER_CANCEL_CONDENSED_PRINTING_COMMAND[] = {0x12};

const uint8_t LPT_DOT_MATRIX_PRINTER_TURN_ON_DOUBLE_WIDTH_PRINTING_COMMAND[] = {0x1B, 0x57, 0x31};
const uint8_t LPT_DOT_MATRIX_PRINTER_TURN_OFF_DOUBLE_WIDTH_PRINTING_COMMAND[] = {0x1B, 0x57, 0x30};

const uint8_t LPT_DOT_MATRIX_PRINTER_TURN_ON_DOUBLE_HEIGHT_PRINTING_COMMAND[] = {0x1B, 0x77, 0x31};
const uint8_t LPT_DOT_MATRIX_PRINTER_TURN_OFF_DOUBLE_HEIGHT_PRINTING_COMMAND[] = {0x1B, 0x77, 0x30};

const uint8_t LPT_DOT_MATRIX_PRINTER_SELECT_DOUBLE_STRIKE_PRINTING_COMMAND[] = {0x1B, 0x47};
const uint8_t LPT_DOT_MATRIX_PRINTER_CANCEL_DOUBLE_STRIKE_PRINTING_COMMAND[] = {0x1B, 0x48};

const uint8_t LPT_DOT_MATRIX_PRINTER_SELECT_SUPERSCRIPT_COMMAND[] = {0x1B, 0x53, 0x30};
const uint8_t LPT_DOT_MATRIX_PRINTER_SELECT_SUBSCRIPT_COMMAND[] = {0x1B, 0x53, 0x31};
const uint8_t LPT_DOT_MATRIX_PRINTER_CANCELS_SUPER_SUBSCRIPT_COMMAND[] = {0x1B, 0x54};

const uint8_t LPT_DOT_MATRIX_PRINTER_TURN_ON_UNDERLINE_PRINTING_COMMAND[] = {0x1B, 0x2D, 0x31};
const uint8_t LPT_DOT_MATRIX_PRINTER_TURN_OFF_UNDERLINE_PRINTING_COMMAND[] = {0x1B, 0x2D, 0x30};

//----------------------------------------------------------------------------------------------------------

const uint8_t LPT_DOT_MATRIX_PRINTER_SELECT_CHARACTER_TABLE_00_COMMAND[] = {0x1B, 0x74, 0x30};
const uint8_t LPT_DOT_MATRIX_PRINTER_SELECT_CHARACTER_TABLE_01_COMMAND[] = {0x1B, 0x74, 0x31};
const uint8_t LPT_DOT_MATRIX_PRINTER_SELECT_CHARACTER_TABLE_02_COMMAND[] = {0x1B, 0x74, 0x32};
const uint8_t LPT_DOT_MATRIX_PRINTER_SELECT_CHARACTER_TABLE_03_COMMAND[] = {0x1B, 0x74, 0x33};

const uint8_t LPT_DOT_MATRIX_PRINTER_COPY_ROM_TO_RAM_COMMAND[] = {0x1B, 0x3A, 0x00, 0x00, 0x00};

const uint8_t LPT_DOT_MATRIX_PRINTER_NORMAL_SET_COMMAND[] = {0x1B, 0x25, 0x00};
const uint8_t LPT_DOT_MATRIX_PRINTER_USER_DEFINED_SET_COMMAND[] = {0x1B, 0x25, 0x01};

const uint8_t LPT_DOT_MATRIX_PRINTER_ENABLE_PRINTING_UPPER_CONTROL_CODES_COMMAND[] = {0x1B, 0x36};
const uint8_t LPT_DOT_MATRIX_PRINTER_DEFINE_POSITIONING_UNIT_COMMAND[] = {0x1B, 0x28, 0x55, 0x01, 0x00, 0x3C};

//**********************************************************************************************************
//**********************************************************************************************************
//**********************************************************************************************************

//General operation
const uint8_t PCL_PRINTER_RESET_COMMAND[] = {0x1B, 0x45};		//"\eE"

//----------------------------------------------------------------------------------------------------------
//Paper Control
const uint8_t PCL_PRINTER_NUMBER_OF_COPIES_COMMAND[] = {0x1B, 0x26, 0x6C, 0x31, 0x58};		//"\e&l#X"
const uint8_t PCL_PRINTER_SIMPLEX_COMMAND[] = {0x1B, 0x26, 0x6C, 0x30, 0x53};		//"\e&l0S"

//----------------------------------------------------------------------------------------------------------
//Paper feeding
const uint8_t PCL_PRINTER_BACKSPACE_COMMAND[] = {0x08};
const uint8_t PCL_PRINTER_LINE_FEED_COMMAND[] = {0x0A, 0x0D};
const uint8_t PCL_PRINTER_CARRIAGE_RETURN_COMMAND[] = {0x0D};
const uint8_t PCL_PRINTER_EJECT_PAPER_COMMAND[] = {0x1B, 0x45};		//"\eE"

//----------------------------------------------------------------------------------------------------------
//Margins and Text length
const uint8_t PCL_PRINTER_LEFT_MARGIN_COMMAND[] = {0x1B, 0x26, 0x61, 0x31, 0x35, 0x4C};		//"\e&a#L"

//----------------------------------------------------------------------------------------------------------
//Page Orientation
const uint8_t PCL_PRINTER_PORTRAIT_COMMAND[] = {0x1B, 0x26, 0x6C, 0x30, 0x4F};		//"\e&l0O"
const uint8_t PCL_PRINTER_LANDSCAPE_COMMAND[] = {0x1B, 0x26, 0x6C, 0x31, 0x4F};		//"\e&l1O"

//----------------------------------------------------------------------------------------------------------
//Page Length and Size
const uint8_t PCL_PRINTER_PAGE_SIZE_A4_COMMAND[] = {0x1B, 0x26, 0x6C, 0x32, 0x36, 0x41};		//"\e&l26A"
const uint8_t PCL_PRINTER_PAGE_SIZE_A5_COMMAND[] = {0x1B, 0x26, 0x6C, 0x32, 0x35, 0x41};		//"\e&l35A"

//----------------------------------------------------------------------------------------------------------
//Line Termination
const uint8_t PCL_PRINTER_LINE_TERMINATION_COMMAND[] = {0x1B, 0x26, 0x6B, 0x32, 0x47};		//"\e&k2G"

//----------------------------------------------------------------------------------------------------------
//Font Selection by identification
const uint8_t PCL_PRINTER_SELECT_PRIMARY_FONT_ID_01_COMMAND[] = {0x1B, 0x28, 0x31, 0x58};		//"\e(#X"

//----------------------------------------------------------------------------------------------------------
//Font Management
const uint8_t PCL_PRINTER_MAKE_FONT_PERMANENT_COMMAND[] = {0x1B, 0x2A, 0x63, 0x35, 0x46};		//"\e*c5F"

//----------------------------------------------------------------------------------------------------------

const uint8_t PCL_PRINTER_DEFINE_FONT_ID_01_COMMAND[] = {0x1B, 0x2A, 0x63, 0x31, 0x44};		//"\e*c#D"
const uint8_t PCL_PRINTER_CREATE_FONT_HEADER_OF_64_BYTES_COMMAND[] = {0x1B, 0x29, 0x73, 0x36, 0x34, 0x57};		//"\e)s#W"	
//const uint8_t PCL_PRINTER_ASCII_CODE_32_COMMAND[] = {0x1B, 0x2A, 0x63, 0x33, 0x32, 0x45};		//"\e*c#E"
//const uint8_t PCL_PRINTER_DOWNLOAD_CHARACTER_172_COMMAND[] = {0x1B, 0x28, 0x73, 0x31, 0x37, 0x32, 0x57};		//"\e(s#W"

//----------------------------------------------------------------------------------------------------------

const uint8_t PCL_PRINTER_FONT_HEADER_BITMAPPED_FONTS[] = {0x00, 0x40, 0x00, 0x02, 0x00, 0x00, 0x00, 0x28, 
															0x00, 0x18, 0x00, 0x50, 0x00, 0x00, 0x01, 0x0D, 
															0x00, 0x60, 0x01, 0x18, 0x00, 0x70, 0x00, 0x00, 
															0x00, 0x03, 0x00, 0x02, 0x02, 0x00, 0x00, 0x00, 
															0x01, 0x18, 0x00, 0x60, 0x00, 0x20, 0x00, 0xFF, 
															0x00, 0x00, 0x8F, 0x69, 0x00, 0x00, 0x00, 0x00, 
															0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 
															0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 
															0x00};

//**********************************************************************************************************

Status_Printer_TypeDef LPT_Printer_SendChar(uint8_t Char);
Status_Printer_TypeDef LPT_Printer_SendString(string String);
Status_Printer_TypeDef LPT_Printer_SendCommand(const uint8_t *Command, uint16_t Size = 1);
Status_Printer_TypeDef LPT_Printer_CheckPaper(void);

//----------------------------------------------------------------------------------------------------------

Status_Printer_TypeDef LPT_DotMatrixPrinterInitialize(void);
Status_Printer_TypeDef LPT_DotMatrixPrinterDefiningCharacters(void);
void LPT_DotMatrixPrinterRestInitializeFont(void);

uint8_t LPT_DotMatrixPrinter_UTF8_To_Map(uint8_t Character);
Status_Printer_TypeDef LPT_DotMatrixPrinterPrintCharacterPersian(uint8_t CharacterPersian);
Status_Printer_TypeDef LPT_DotMatrixPrinterPrintNumbersPersian(char *NumbersPersian);
Status_Printer_TypeDef LPT_DotMatrixPrinterPrintStringPersian(char *strPersian);

Status_Printer_TypeDef LPT_DotMatrixPrinterSetHorizontalCursorPositioning(uint8_t Columns);

Status_Printer_TypeDef LPT_DotMatrixPrinterBackspace(uint8_t Backspace = 1);
Status_Printer_TypeDef LPT_DotMatrixPrinterSpace(uint8_t Space = 1);

uint8_t LPT_DotMatrixPrinterSelectPrintFont_01(void);
uint8_t LPT_DotMatrixPrinterSelectPrintFont_02(void);
uint8_t LPT_DotMatrixPrinterSelectPrintFont_03(void);
uint8_t LPT_DotMatrixPrinterSelectPrintFont_04(void);

//----------------------------------------------------------------------------------------------------------

Status_Printer_TypeDef LPT_LaserPrinterInitialize(void);
Status_Printer_TypeDef LPT_LaserPrinterDefiningCharacters(void);
void LPT_LaserPrinterRestInitializeFont(void);

uint8_t LPT_LaserPrinter_UTF8_To_Map(uint8_t Character);
Status_Printer_TypeDef LPT_LaserPrinterPrintCharacterPersian(uint8_t CharacterPersian);
Status_Printer_TypeDef LPT_LaserPrinterPrintNumbersPersian(char *NumbersPersian);
Status_Printer_TypeDef LPT_LaserPrinterPrintStringPersian(char *strPersian);

Status_Printer_TypeDef LPT_LaserPrinterSetHorizontalCursorPositioning(uint8_t Columns);

Status_Printer_TypeDef LPT_LaserPrinterBackspace(uint8_t Backspace = 1);
Status_Printer_TypeDef LPT_LaserPrinterSpace(uint8_t Space = 1);

uint8_t LPT_LaserPrinterSelectPrintFont_01(void);
uint8_t LPT_LaserPrinterSelectPrintFont_02(void);
uint8_t LPT_LaserPrinterSelectPrintFont_03(void);
uint8_t LPT_LaserPrinterSelectPrintFont_04(void);

//----------------------------------------------------------------------------------------------------------

Status_Printer_TypeDef Thermal_PrinterInitialize(void);

uint8_t Thermal_Printer_UTF8_To_Map(uint8_t Character);
Status_Printer_TypeDef Thermal_PrinterPrintCharacterPersian(uint8_t CharacterPersian);
Status_Printer_TypeDef Thermal_PrinterPrintNumbersPersian(char *NumbersPersian);
Status_Printer_TypeDef Thermal_PrinterPrintStringPersian(char *strPersian);

void Thermal_PrinterSelectPrintFont_01(void);
void Thermal_PrinterSelectPrintFont_02(void);
void Thermal_PrinterSelectPrintFont_03(void);

void Thermal_PrinterEject(void);
void Thermal_Printerfeed(uint8_t Line);
void Thermal_PrinterJustify(char Value);

//----------------------------------------------------------------------------------------------------------

void Printer_SetPrinterType(uint8_t PrinterType , bool PaperType);
Status_Printer_TypeDef Printer_Initialize(void);

Status_Printer_TypeDef Printer_SendChar(uint8_t Char);
Status_Printer_TypeDef Printer_SendString(string String);
Status_Printer_TypeDef Printer_SendCommand(Command_Printer_TypeDef Command_Printer);

Status_Printer_TypeDef Printer_CheckPaper(void);

Status_Printer_TypeDef Printer_PrintNumbersPersian(char *NumbersPersian);
Status_Printer_TypeDef Printer_PrintStringPersian(char *strPersian);

Status_Printer_TypeDef Printer_SetHorizontalCursorPositioning(uint8_t Columns);

Status_Printer_TypeDef Printer_Backspace(uint8_t Backspace = 1);
Status_Printer_TypeDef Printer_Space(uint8_t Space = 1);

uint8_t Printer_SelectPrintFont_01(void);
uint8_t Printer_SelectPrintFont_02(void);
uint8_t Printer_SelectPrintFont_03(void);
uint8_t Printer_SelectPrintFont_04(void);

//**********************************************************************************************************

#endif /* __PRINTER_H */

