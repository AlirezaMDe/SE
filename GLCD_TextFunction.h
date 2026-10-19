/*-------------------------------------------------------------------------------------------------------------
@file           : GLCD_TextFunction.h
@author  		: Reza Farahani (nmmco2015@gmail.com)
@brief          : Header for GLCD_TextFunction.cpp file.
-------------------------------------------------------------------------------------------------------------*/

#ifndef __GLCD_TEXT_FUNCTION_H
#define __GLCD_TEXT_FUNCTION_H

#ifdef __cplusplus
extern "C" {
#endif

//**********************************************************************************************************

#include "GLCD_Function.h"
#include <string.h>

//**********************************************************************************************************

#define FONT_HEADER_SIZE					7	// header size of fonts
#define PutPixel(x, y, clr)					GLCD_SetPixel(x, y, clr)

//**********************************************************************************************************

typedef enum
{
	ENGLISH_LETTER,
	PERSIAN_LETTER
}FontLetter_TypeDef;

//**********************************************************************************************************

#define STYLE_NONE							0
#define STYLE_NO_ZERO   					1
#define STYLE_NO_SPACE  					2

#define ALINE_LEFT							0
#define ALINE_CENTER						1
#define ALINE_RIGHT							2
#define ALINE_MARK							(ALINE_LEFT | ALINE_CENTER | ALINE_RIGHT)

#define BORDER_NONE							0x00
#define BORDER_RECT							0x04
#define BORDER_FILL							0x08
#define BORDER_BEVEL						0xF0	// bevel radius 0x00(rectangle), 0x10-0xF0(radius size)
#define BORDER_MASK							(BORDER_FILL | BORDER_RECT)
#define bvl(x)								(x << 4)

//**********************************************************************************************************

extern uint8_t			Rot;
extern uint8_t			Reverse;
extern uint8_t			Horizontal;
extern GLCD_Color_TypeDef			FontColor;
extern GLCD_Color_TypeDef			BackgroundColor;
extern uint8_t			FontFixed;
extern uint8_t			NonTransparence;
extern uint8_t			FontWidth;
extern uint8_t			FontHeight;
extern uint8_t			FontXScale;
extern uint8_t			FontYScale;
extern uint8_t			FontSpaceX;
extern uint8_t			FontSpaceY;
extern uint32_t 		cursorX;
extern uint32_t 		cursorY;
extern FontLetter_TypeDef		peLETTER;

//**********************************************************************************************************

#define GetLetter() 					peLETTER

#define GetCursorX() 					cursorX
#define GetCursorY() 					cursorY
#define SetCursorX(x) 					cursorX = x
#define SetCursorY(y) 					cursorY = y
#define SetCursor(x,y) 					{cursorX = x; cursorY = y;}

#define LcdRot(n)						Rot = n
#define LcdReverse(n)					Reverse = n
#define LcdHorizontal(n)				Horizontal = n

#define LcdFontFixed(n)					FontFixed = n
#define LcdNonTransparence(n)			NonTransparence = n

#define SetFontColor(c)					FontColor = c
#define SetBackgroundColor(c)			BackgroundColor = c
#define GetFontColor()					FontColor
#define GetbackgroundColor()			BackgroundColor

#define LcdFontXScale(n)				FontXScale = n
#define LcdFontYScale(n)				FontYScale = n

#define LcdFontSpaceX(n)				FontSpaceX  = n
#define LcdFontSpaceY(n)				FontSpaceY  = n

#define LcdFontWidth() 	 				FontWidth
#define LcdFontHeight()					FontHeight

//**********************************************************************************************************

uint8_t Bit_Is_Set(uint8_t Byte, uint8_t Mask);

void PutCharEnglish(uint8_t CharEnglish);
void PutCharPersian(uint8_t CharPersian);
void PutChar(uint8_t Char);

void PutsStringEnglish(char *strEnglish);
void PutsStringPersian(char *strPersian);
void PutsString(char *Str);

uint32_t CalcTextWidthEnglish(char *str);
uint32_t CalcTextWidthPersian(char *str);
uint32_t CalcTextWidth(char *Str);
uint32_t CalcTextHeight(void);

char UTF8_To_ASCII(char Ascii);

void SetFont(const uint8_t *pointer);
void SetFontLetter(FontLetter_TypeDef FontLetter);
void SetTextPixelPosition(uint8_t X, uint8_t Y);
void SetTextPosition(uint8_t Line, uint8_t Column);
void SetLinePixelPosition_X(uint8_t Line, uint8_t X);

void GetTextPixelPosition(uint8_t *X, uint8_t *Y);

void DrawStringAt(uint8_t Line, uint8_t Column, char *Str, GLCD_Color_TypeDef FontColor, GLCD_Color_TypeDef backgroundColor);

//**********************************************************************************************************

#ifdef __cplusplus
}
#endif
#endif /* __GLCD_TEXT_FUNCTION_H */
