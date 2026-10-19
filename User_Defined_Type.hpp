/*-------------------------------------------------------------------------------------------------------------
@file 			: User_Defined_Type.hpp
@author  		: Reza Farahani (nmmco2015@gmail.com)
-------------------------------------------------------------------------------------------------------------*/

#ifndef __USER_DEFINED_TYPE_H
#define __USER_DEFINED_TYPE_H

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include <string>

using namespace std;

//**********************************************************************************************************

#ifdef __cplusplus
extern "C" {
#endif

#include "User_System_Config.h"

#include "Font_En_Num_8X10.h"
#include "Font_En_Num_16X20.h"
#include "Font_En_Num_27X37.h"

#include "Font_En_Verdana_16X16.h"

#include "Font_Pe_9X8.h"
#include "Font_Pe_12X11.h"
#include "Font_Pe_13X12.h"
#include "Font_Pe_12X16.h"
#include "Font_Pe_14X16.h"

#include "Symbol_16.h"

#include "Logo_Image.h"

#include <string.h>
#include <stdlib.h>
#include <math.h>

//**********************************************************************************************************

#define MAX_TICKET_SERIAL_NUMBER    		20000
#define MAX_CHECK_EXIST_NUMBER_TRUCK		1000
#define LENGTH_PIN_SYSTEM					7
#define MAX_PRODUCT_NAME                	50
#define MAX_PRODUCT_CODE                	300
#define MAX_CUSTOMER_NAME                	50
#define MAX_CUSTOMER_CODE                	300
#define MAX_NUMBER_PLATES					8
#define MAX_CERTAIN_TRUCKS                  300
#define MAX_TITLE							50
#define TIMEOUT_ERROR						250

//**********************************************************************************************************

typedef enum
{
	DISPLAY_WEIGHT,
	ZERO_WEIGHT,
	TARE_WEIGHT,
	PRESET_TARE,
	PERSIANIZING_PRINTER,
	PRINT_WEIGHT_01,
	PRINT_WEIGHT_02,
	PRINTING,
}WeightOperation_TypeDef;

//**********************************************************************************************************
//PANEL

typedef enum
{
	NO_PUSH,

	KEYPAD_01,
	KEYPAD_02,
	KEYPAD_03,
	KEYPAD_04,
	KEYPAD_05,
	KEYPAD_06,
	KEYPAD_07,
	KEYPAD_08,
	KEYPAD_09,
	KEYPAD_00,

	KEYPAD_ALPHABET_01,
	KEYPAD_ALPHABET_02,
	KEYPAD_ALPHABET_03,
	KEYPAD_ALPHABET_04,
	KEYPAD_ALPHABET_05,
	KEYPAD_ALPHABET_06,
	KEYPAD_ALPHABET_07,
	KEYPAD_ALPHABET_08,
	
	KEYPAD_SPACE,

	KEYPAD_TARE,
	KEYPAD_PTARE,
	KEYPAD_WEIGH_01,
	KEYPAD_WEIGH_02,
	KEYPAD_PRINT,

	KEYPAD_MENU,
	KEYPAD_LEFT,
	KEYPAD_RIGHT,
	KEYPAD_UP,
	KEYPAD_DOWN,

	KEYPAD_SET,
	KEYPAD_CE,
	KEYPAD_ESC,
	KEYPAD_ENTER
}PanelKeypadButton_TypeDef;

typedef struct
{
	uint8_t AmountBlinker;
	uint8_t Counter;
	string String;
}Blinker_TypeDef;

typedef struct
{
	uint8_t Ascii;
	uint8_t key;
}Keyboard_Info_TypeDef;

typedef union
{
	Keyboard_Info_TypeDef Keyboard_Info;
	uint8_t Array[sizeof(Keyboard_Info_TypeDef)];
}UnionKeyboard_Info_TypeDef;

//----------------------------------------------------------------------------------------------------------
//Scale_Parameters

typedef enum
{
	ERROR_AD7780_SCALE_FUNCTION,
	ERROR_SCALE_FUNCTION,
	OK_SCALE_FUNCTION
}StatusScale_Function_TypeDef;

typedef struct
{
	SPI_HandleTypeDef *SPI_Port;
	GPIO_TypeDef *GPIO_Port_Reset;
	uint16_t GPIO_Pin_Reset;
	GPIO_TypeDef *GPIO_Port_DataReady;
	uint16_t GPIO_Pin_DataReady;
}ConfigParametersScale_TypeDef;

typedef struct
{
	StatusScale_Function_TypeDef Status;
	int32_t Respance;
}RespanceScale_TypeDef;

//----------------------------------------------------------------------------------------------------------
//Weight_Function

typedef enum
{
	NON_WEIGHT_FUNCTION,
	ERROR_AD7780_SCALE_01_WEIGHT_FUNCTION,
	ERROR_SCALE_01_WEIGHT_FUNCTION,
	ERROR_AD7780_SCALE_02_WEIGHT_FUNCTION,
	ERROR_SCALE_02_WEIGHT_FUNCTION,
	OK_WEIGHT_FUNCTION
}StatusWeight_Function_TypeDef;

typedef enum
{
	OK_ZERO_TRACK,
	ERROR_ZERO_TRACK
}StatusZeroTrack_TypeDef;

typedef enum
{
	OK_WEIGHT_TARING,
	ERROR_WEIGHT_TARING
}StatusWeightTaring_TypeDef;

typedef struct
{
	unsigned DISPDigits : 3;
	unsigned DECPoint : 3;
	unsigned WeightRange : 2;
	unsigned LeadZBlank : 1;
	unsigned IsStable : 1;
	unsigned IsZero : 1;
	unsigned IsNet : 1;
	unsigned IsUnderRange : 1;
	unsigned IsOverRange : 1;
	unsigned IsError : 1;
	unsigned IsAutoZero : 1;
	int32_t Raw;
	int32_t Gross;
	int32_t Normally;
	int32_t HighResolution;
	int32_t ADC_Scale;
}WeightScale_TypeDef;

//----------------------------------------------------------------------------------------------------------
//System_Parameters

typedef enum
{
	BUSY_PRINTER,
	TIME_OUT_PRINTER,
	NO_PAPER_PRINTER,
	ERROR_PRINTER,
	OK_PRINTER
}Status_Printer_TypeDef;

typedef enum
{
	PRINTER_LINE_FEED_COMMAND,
	PRINTER_EJECT_PAPER_COMMAND,
	PRINTER_USER_DEFINED_SET_COMMAND,
	PRINTER_NORMAL_SET_COMMAND
}Command_Printer_TypeDef;

typedef enum
{
	NON_ERROR,
	E01_READ_SCALE01,
	E02_READ_SCALE02,
	E03_AD7780_SCALE01,
	E04_AD7780_SCALE02,
	E05_EEPROM,
	E06_COM01,
	E07_COM02,
	E08_COM03,
	E09_POWER_UP_ZERO,
	E10_SCALE_TARED,
	E11_NEGATIVE_WEIGHTS,
	E12_PRINTER,
	E13_BACKUP_MEMORY,
	E14_LIMIT_BACKUP_MEMORY,
	E15_SCALE_NOT_TARED,
	E16_USB_PRINTER
}ErrorSystem_TypeDef;

typedef struct
{
	unsigned E01_READ_SCALE01 : 1;
	unsigned E02_READ_SCALE02 : 1;
	unsigned E03_AD7780_SCALE01 : 1;
	unsigned E04_AD7780_SCALE02 : 1;
	unsigned E05_EEPROM : 1;
	unsigned E06_COM01 : 1;
	unsigned E07_COM02 : 1;
	unsigned E08_COM03 : 1;
	unsigned E09_POWER_UP_ZERO : 1;
	unsigned E10_SCALE_TARED : 1;
	unsigned E11_NEGATIVE_WEIGHTS : 1;
	unsigned E12_PRINTER : 1;
	unsigned E13_BACKUP_MEMORY : 1;
	unsigned E14_LIMIT_BACKUP_MEMORY : 1;
	unsigned E15_SCALE_NOT_TARED : 1;
	unsigned E16_USB_PRINTER : 1;
}ErrorList_TypeDef;

typedef struct
{
	unsigned DISPDigits : 3;
	unsigned DECPoint : 3;
	unsigned ScaleDIV : 3;
	unsigned DIGFilter : 3;
	unsigned MotionSamples : 3;
	unsigned AutoZTrack : 7;
	unsigned INITZero : 1;
	unsigned LeadZBlank : 1;
	unsigned ZeroRange : 1;
	unsigned SCALE_02_ENABLE : 1;
	unsigned AutoPowerOff : 7;
	uint32_t WEIGRange;
}ScaleSettingParameters_TypeDef;

typedef struct
{
	//PARAMETERS
	ScaleSettingParameters_TypeDef ScaleSettingParameters;
	//DeadLoad
	int32_t DeadLoad;
	//Alpha
	float Alpha;
}CalibrationParameters_TypeDef;

typedef struct
{
	UART_HandleTypeDef *COM_Port;
	uint32_t BaudRate;
	uint8_t Parity;
	uint8_t DataBits;
	bool StopBits;
	uint8_t Operation;
}CommunicationParameters_TypeDef;

typedef struct
{
	uint32_t Trolley;
	uint32_t Ten_Wheels;
	uint32_t Six_Wheels;
	uint32_t Khavar;
	uint32_t Nissan;
	uint32_t PickupTruck;
	uint32_t Other;
}WeighingCost_TypeDef;

typedef struct
{
	uint8_t Line;
	uint8_t Column;
}FieldParameters_TypeDef;

typedef struct
{
	unsigned PrintTitle : 1;
	unsigned EjectSheet : 1;
	unsigned TypePrinter : 2;
	unsigned PrintFont : 2;
	unsigned NumberOfPaperPrint : 3;
	unsigned NumberOfPrint : 3;
	unsigned PrintType : 2;
	unsigned TopLineEmpty : 5;
	unsigned BottomLineEmpty : 5;
	unsigned SecondWeighingPrint : 1;
	unsigned TypePaper : 1;
	char Title_01[2][MAX_TITLE];
	char Title_02[2][MAX_TITLE];
	FieldParameters_TypeDef TitleNumberPlates;
	FieldParameters_TypeDef TitleProduct;
	FieldParameters_TypeDef TitleTime;
	FieldParameters_TypeDef TitleDate;
	FieldParameters_TypeDef TitleWeight;
	FieldParameters_TypeDef TitleTicketSerialNumber;
	FieldParameters_TypeDef TitleCustomer;
	FieldParameters_TypeDef TitleWeighingCost;
	FieldParameters_TypeDef NumberPlates;
	FieldParameters_TypeDef Product;
	FieldParameters_TypeDef FirstWeightDate;
	FieldParameters_TypeDef SecondWeightDate;
	FieldParameters_TypeDef FirstWeightTime;
	FieldParameters_TypeDef SecondWeightTime;
	FieldParameters_TypeDef FirstWeight;
	FieldParameters_TypeDef SecondWeight;
	FieldParameters_TypeDef NetWeight;
	FieldParameters_TypeDef TicketSerialNumber;
	FieldParameters_TypeDef Customer;
	FieldParameters_TypeDef WeighingCost;
	FieldParameters_TypeDef ProductPrice;
}PrintFormatParameters_TypeDef;

typedef struct
{
	char Name[MAX_PRODUCT_NAME];
	uint32_t Price;
}ProductParameters_TypeDef;

typedef struct
{
	char Name[MAX_CUSTOMER_NAME];
}CustomerParameters_TypeDef;

typedef struct
{
	char NumberPlates[MAX_NUMBER_PLATES];
	uint32_t EmptyWeight;
}CertainTrucksParameters_TypeDef;

typedef struct
{
	char PinSystem[LENGTH_PIN_SYSTEM];
	char PinOperator[LENGTH_PIN_SYSTEM];
	CalibrationParameters_TypeDef CalibrationParameters;
	CommunicationParameters_TypeDef COM1;
	CommunicationParameters_TypeDef COM2;
	WeighingCost_TypeDef WeighingCost;
	PrintFormatParameters_TypeDef PrintFormatParameters;
}SystemParameters_TypeDef;

//----------------------------------------------------------------------------------------------------------
//Print

typedef struct
{
	uint8_t Hours;
	uint8_t Minutes;
	uint8_t Seconds;
}TimeWeightInformation_TypeDef;

typedef struct
{
	uint16_t Year;
	uint8_t Month;
	uint8_t Date;
}DateWeightInformation_TypeDef;

typedef struct
{
	unsigned DISPDigits : 3;
	unsigned DECPoint : 3;
	unsigned LeadZBlank : 1;
	unsigned TypeFirstWeight : 1;    //Certain Trucks = 1
	unsigned TypeWeighingInformation : 2;    //NetWeight = 0, FirstWeight = 1, SecondWeight = 2, FirstWeight_SecondWeight_Used = 3
	uint32_t TicketSerialNumber;
	ProductParameters_TypeDef ProductParameters;
	CustomerParameters_TypeDef CustomerParameters;
	char NumberTruck[MAX_NUMBER_PLATES];
	uint32_t WeighingCost;
	DateWeightInformation_TypeDef FirstWeightDate;
	TimeWeightInformation_TypeDef FirstWeightTime;
	int32_t FirstWeight;
	DateWeightInformation_TypeDef SecondWeightDate;
	TimeWeightInformation_TypeDef SecondWeightTime;
	int32_t SecondWeight;
	int32_t NetWeight;
	uint32_t ProductPrice;
}WeighingInformation_TypeDef;

//**********************************************************************************************************

#ifdef __cplusplus
}
#endif

#endif /* __USER_DEFINED_TYPE_H */

