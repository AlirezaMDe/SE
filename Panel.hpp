/*-------------------------------------------------------------------------------------------------------------
@file           : Panel.hpp
@author  		: Reza Farahani (nmmco2015@gmail.com)
@brief          : Header for Panel.cpp file.
-------------------------------------------------------------------------------------------------------------*/

#ifndef __PANEL_H
#define __PANEL_H

//**********************************************************************************************************

#include "User_Defined_Type.hpp"

//**********************************************************************************************************

#define DELAY_MS(Delay_mS)		osDelay(Delay_mS)
#define KEYBOARD_USB			hUsbHostFS

//**********************************************************************************************************

extern volatile uint16_t CountAutoPowerOff;

//**********************************************************************************************************
//KEYPAD

#define KEY_COL01_SET				HAL_GPIO_WritePin(KEY_COL01_GPIO_Port, KEY_COL01_Pin, GPIO_PIN_SET)
#define KEY_COL01_RESET    			HAL_GPIO_WritePin(KEY_COL01_GPIO_Port, KEY_COL01_Pin, GPIO_PIN_RESET)

#define KEY_COL02_SET				HAL_GPIO_WritePin(KEY_COL02_GPIO_Port, KEY_COL02_Pin, GPIO_PIN_SET)
#define KEY_COL02_RESET    			HAL_GPIO_WritePin(KEY_COL02_GPIO_Port, KEY_COL02_Pin, GPIO_PIN_RESET)

#define KEY_COL03_SET				HAL_GPIO_WritePin(KEY_COL03_GPIO_Port, KEY_COL03_Pin, GPIO_PIN_SET)
#define KEY_COL03_RESET    			HAL_GPIO_WritePin(KEY_COL03_GPIO_Port, KEY_COL03_Pin, GPIO_PIN_RESET)

#define KEY_COL04_SET				HAL_GPIO_WritePin(KEY_COL04_GPIO_Port, KEY_COL04_Pin, GPIO_PIN_SET)
#define KEY_COL04_RESET    			HAL_GPIO_WritePin(KEY_COL04_GPIO_Port, KEY_COL04_Pin, GPIO_PIN_RESET)

#define KEY_COL05_SET				HAL_GPIO_WritePin(KEY_COL05_GPIO_Port, KEY_COL05_Pin, GPIO_PIN_SET)
#define KEY_COL05_RESET    			HAL_GPIO_WritePin(KEY_COL05_GPIO_Port, KEY_COL05_Pin, GPIO_PIN_RESET)

#define KEY_COL06_SET				HAL_GPIO_WritePin(KEY_COL06_GPIO_Port, KEY_COL06_Pin, GPIO_PIN_SET)
#define KEY_COL06_RESET    			HAL_GPIO_WritePin(KEY_COL06_GPIO_Port, KEY_COL06_Pin, GPIO_PIN_RESET)

#define KEY_COL07_SET				HAL_GPIO_WritePin(KEY_COL07_GPIO_Port, KEY_COL07_Pin, GPIO_PIN_SET)
#define KEY_COL07_RESET    			HAL_GPIO_WritePin(KEY_COL07_GPIO_Port, KEY_COL07_Pin, GPIO_PIN_RESET)

#define KEY_COL08_SET				HAL_GPIO_WritePin(KEY_COL08_GPIO_Port, KEY_COL08_Pin, GPIO_PIN_SET)
#define KEY_COL08_RESET    			HAL_GPIO_WritePin(KEY_COL08_GPIO_Port, KEY_COL08_Pin, GPIO_PIN_RESET)

#define IS_RESET_KEY_ROW01			HAL_GPIO_ReadPin(KEY_ROW01_GPIO_Port, KEY_ROW01_Pin) == GPIO_PIN_RESET
#define IS_RESET_KEY_ROW02			HAL_GPIO_ReadPin(KEY_ROW02_GPIO_Port, KEY_ROW02_Pin) == GPIO_PIN_RESET
#define IS_RESET_KEY_ROW03			HAL_GPIO_ReadPin(KEY_ROW03_GPIO_Port, KEY_ROW03_Pin) == GPIO_PIN_RESET
#define IS_RESET_KEY_ROW04			HAL_GPIO_ReadPin(KEY_ROW04_GPIO_Port, KEY_ROW04_Pin) == GPIO_PIN_RESET

//**********************************************************************************************************

#define BUZZER_SET					HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET)
#define BUZZER_RESET    			HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET)

//**********************************************************************************************************

//KEYBOARD FUNCTION
Keyboard_Info_TypeDef Panel_ScanKeyboard(void);
PanelKeypadButton_TypeDef Panel_ReadKeyboard(void);

//KEYPAD FUNCTION
void ResetColemnKeyboard(uint8_t Colemn);
bool Panel_IsPushKeypad(void);
uint8_t Panel_ScanKeypad(void);

PanelKeypadButton_TypeDef Panel_ReadKeypad(void);

//PUBLIC FUNCTIONS
PanelKeypadButton_TypeDef Panel_ReadPanel(void);

//**********************************************************************************************************

#endif /* __PANEL_H */

