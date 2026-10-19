/*-------------------------------------------------------------------------------------------------------------
@file           : GLCD_Function.h
@author  		: Reza Farahani (nmmco2015@gmail.com)
@brief          : Header for GLCD_Function.cpp file.
-------------------------------------------------------------------------------------------------------------*/

#ifndef __GLCD_FUNCTION_H
#define __GLCD_FUNCTION_H

#ifdef __cplusplus
extern "C" {
#endif

//**********************************************************************************************************

#include "LCD_WG24064.h"

//**********************************************************************************************************

#define DELAY_MS(Delay_mS)		            osDelay(Delay_mS)
  
//**********************************************************************************************************

#define GLCD_SCREEN_WIDTH					WG24064_SCREEN_WIDTH
#define GLCD_SCREEN_HEIGHT					WG24064_SCREEN_HEIGHT
#define	GLCD_SCREEN_SIZE_PAGE				WG24064_SCREEN_SIZE_PAGE
#define GLCD_SCREEN_PAGES					WG24064_SCREEN_PAGES

//----------------------------------------------------------------------------------------------------------

#define GET_MAX_X()							((uint32_t)GLCD_SCREEN_WIDTH - 1)
#define GET_MAX_Y()							((uint32_t)GLCD_SCREEN_HEIGHT - 1)

//**********************************************************************************************************

typedef enum
{
	NON_INVERTED,
	INVERTED
}GLCD_OperatingMode_TypeDef;

typedef enum
{
	WHITE = 0x00,
	BLACK = 0xFF
}GLCD_Color_TypeDef;

typedef struct
{
	uint8_t X;
	uint8_t Y;
	uint8_t Page;
	GLCD_OperatingMode_TypeDef Mode;
}GLCD_Parameters_TypeDef;

//**********************************************************************************************************

uint8_t GLCD_ReadBuffer(uint8_t Page, uint8_t Column);
void GLCD_WriteBuffer(uint8_t Page, uint8_t Column, uint8_t Data);

void GLCD_Initialization(void);
void GLCD_SetMode(GLCD_OperatingMode_TypeDef GLCD_OperatingMode);

void GLCD_Clear(void);
void GLCD_ClearPage(uint8_t Page);
void GLCD_FillScreen(GLCD_Color_TypeDef Color);

void GLCD_SetX(uint8_t X);
void GLCD_SetY(uint8_t Y);
void GLCD_SetXY(uint8_t X, uint8_t Y);

void GLCD_GoToPage(uint8_t Page);

uint8_t GLCD_GetX(void);
uint8_t GLCD_GetY(void);
uint8_t GLCD_GetPage(void);

void GLCD_SetPixel(uint8_t X, uint8_t Y, GLCD_Color_TypeDef Color);
void GLCD_SetPixels(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2, GLCD_Color_TypeDef Color);

void GLCD_Draw_H_Line(uint8_t X1, uint8_t X2, uint8_t Y, GLCD_Color_TypeDef Color);
void GLCD_Draw_V_Line(uint8_t Y1, uint8_t Y2, uint8_t X, GLCD_Color_TypeDef Color);

void GLCD_DrawBitmap(const uint8_t *Bitmap, uint8_t X, uint8_t Y,uint8_t Width, uint8_t Height);
void GLCD_DrawLine(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2, GLCD_Color_TypeDef Color);
void GLCD_DrawRectangle(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2, GLCD_Color_TypeDef Color);
void GLCD_DrawRoundRectangle(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2, uint8_t Radius, GLCD_Color_TypeDef Color);
void GLCD_DrawTriangle(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2, uint8_t X3, uint8_t Y3, GLCD_Color_TypeDef Color);
void GLCD_DrawCircle(uint8_t CenterX, uint8_t CenterY, uint8_t Radius, GLCD_Color_TypeDef Color);
void GLCD_FillRectangle(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2, GLCD_Color_TypeDef Color);
void GLCD_FillRoundRectangle(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2, uint8_t Radius, GLCD_Color_TypeDef Color);
void GLCD_FillTriangle(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t y2, uint8_t X3, uint8_t Y3, GLCD_Color_TypeDef Color);
void GLCD_FillCircle(uint8_t CenterX, uint8_t CenterY, uint8_t Radius, GLCD_Color_TypeDef Color);
void GLCD_InvertRect(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2);

void GLCD_Screen_Update(void);

//**********************************************************************************************************

#ifdef __cplusplus
}
#endif
#endif /* __GLCD_FUNCTION_H */
