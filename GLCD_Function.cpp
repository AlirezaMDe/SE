/*-----------------------------------------------------------------------------------------------------------
@file          	: GLCD_Function.cpp
@author  		: Reza Farahani (nmmco2015@gmail.com)
C++ source 2024
-----------------------------------------------------------------------------------------------------------*/

#include "GLCD_Function.h"

//**********************************************************************************************************

uint8_t GLCD_Buffer[GLCD_SCREEN_PAGES][GLCD_SCREEN_WIDTH];
GLCD_Parameters_TypeDef GLCD_Parameters;

//**********************************************************************************************************

#define GLCD_MIN(X, Y)				((X < Y) ? X : Y)
#define GLCD_ABS_DIFF(X, Y)			((X > Y) ? (X - Y) : (Y - X))
#define GLCD_SWAP(X, Y)				do{ typeof(X) t = X; X = Y; Y = t; }while(0)

//**********************************************************************************************************

void GLCD_Initialization(void)
{
	WG24064_Initialization();
	GLCD_SetXY(0, 0);
	GLCD_SetMode(NON_INVERTED);
}

//**********************************************************************************************************
    
void GLCD_Screen_Update(void)
{
    uint8_t Data;
    uint8_t ReadBuffer; 
	uint8_t Maske = 0x01;
	uint8_t WriteMaske = 0x80;
    
	for(int i = 0; i < GLCD_SCREEN_HEIGHT; i++)
	{
	  	for(int j = 0; j < GLCD_SCREEN_WIDTH; j+= 8)
		{
		  	Data = 0;
		  	WG24064_GoToPixel(j, i);
			
		  	for(int k = 0; k < 8; k++)
            {
                WriteMaske = 0x80 >> k;
                ReadBuffer = GLCD_ReadBuffer((i / GLCD_SCREEN_SIZE_PAGE), (j + k));
                Data |= ((ReadBuffer & Maske) != Maske ? WriteMaske : 0x00);
            }
            
            WG24064_WriteDisplayData(Data);
		} 
        
        Maske <<= 1;
        if(Maske == 0)
            Maske = 0x01;
	}
}

//**********************************************************************************************************

uint8_t GLCD_ReadBuffer(uint8_t Page, uint8_t Column)
{
	return GLCD_Buffer[Page][Column];
}

//**********************************************************************************************************

void GLCD_SetMode(GLCD_OperatingMode_TypeDef GLCD_OperatingMode)
{
	GLCD_Parameters.Mode = GLCD_OperatingMode;
}

//**********************************************************************************************************

void GLCD_Clear(void)
{
	GLCD_FillScreen(GLCD_Parameters.Mode == NON_INVERTED ? WHITE : BLACK);
	GLCD_SetXY(0, 0);
}

//**********************************************************************************************************

void GLCD_FillScreen(GLCD_Color_TypeDef Color)
{
	for(int j = 0; j < GLCD_SCREEN_PAGES; j++)
	{
		for(int i = 0; i < GLCD_SCREEN_WIDTH; i++)
			GLCD_WriteBuffer(j, i, Color);
	}

	GLCD_SetXY(0, 0);
}

//**********************************************************************************************************

void GLCD_WriteBuffer(uint8_t Page, uint8_t Column, uint8_t Data)
{
	GLCD_Buffer[Page][Column] = Data;
}

//**********************************************************************************************************

void GLCD_ClearPage(uint8_t Page)
{
	if(Page < GLCD_SCREEN_PAGES)
	{
		GLCD_Color_TypeDef GLCD_Color = GLCD_Parameters.Mode == NON_INVERTED ? WHITE : BLACK;

		for(int i = 0; i < GLCD_SCREEN_WIDTH; i++)
			GLCD_WriteBuffer(Page, i, GLCD_Color);

		GLCD_SetXY(0, (Page * GLCD_SCREEN_SIZE_PAGE));
	}
}

//**********************************************************************************************************

void GLCD_SetX(uint8_t X)
{
	if(X < GLCD_SCREEN_WIDTH)
		GLCD_Parameters.X = X;
}

//**********************************************************************************************************

void GLCD_SetY(uint8_t Y)
{
	if(Y < GLCD_SCREEN_HEIGHT)
	{
		GLCD_Parameters.Y = Y;
		GLCD_Parameters.Page = Y / GLCD_SCREEN_SIZE_PAGE;
	}
}

//**********************************************************************************************************

void GLCD_SetXY(uint8_t X, uint8_t Y)
{
	GLCD_SetX(X);
	GLCD_SetY(Y);
}

//**********************************************************************************************************

void GLCD_GoToPage(uint8_t Page)
{
	if(Page < GLCD_SCREEN_PAGES)
		GLCD_SetXY(0, (Page * GLCD_SCREEN_SIZE_PAGE));
}

//**********************************************************************************************************

uint8_t GLCD_GetX(void)
{
	return GLCD_Parameters.X;
}

//**********************************************************************************************************

uint8_t GLCD_GetY(void)
{
	return GLCD_Parameters.Y;
}

//**********************************************************************************************************

uint8_t GLCD_GetPage(void)
{
	return (GLCD_Parameters.Y / GLCD_SCREEN_SIZE_PAGE);
}

//**********************************************************************************************************

void GLCD_SetPixel(uint8_t X, uint8_t Y, GLCD_Color_TypeDef Color)
{
	if ((X < GLCD_SCREEN_WIDTH) && (Y < GLCD_SCREEN_HEIGHT))
	{
		uint8_t Data = 0;

		GLCD_SetXY(X, Y);
		Data = GLCD_ReadBuffer(GLCD_Parameters.Page, GLCD_Parameters.X);

		if(Color == BLACK)
			Data |= 0x01 << (Y % GLCD_SCREEN_SIZE_PAGE);
		else
			Data &= ~(0x01 << (Y % GLCD_SCREEN_SIZE_PAGE));

		GLCD_WriteBuffer(GLCD_Parameters.Page, GLCD_Parameters.X, Data);
	}
}

//**********************************************************************************************************

void GLCD_SetPixels(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2, GLCD_Color_TypeDef Color)
{
	if ((X1 < GLCD_SCREEN_WIDTH) && (X2 < GLCD_SCREEN_WIDTH) &&
			(Y1 < GLCD_SCREEN_HEIGHT) && (Y2 < GLCD_SCREEN_HEIGHT))
	{
		uint8_t Height, Width, Offset, Mask, h, Data;

		Height = Y2 - Y1 + 1;
		Width = X2 - X1 + 1;
		Offset = Y1 % GLCD_SCREEN_SIZE_PAGE;
		Y1 -= Offset;
		Mask = 0xFF;
		Data = 0;

		if(Height < (GLCD_SCREEN_SIZE_PAGE - Offset))
		{
			Mask >>= (GLCD_SCREEN_SIZE_PAGE - Height);
			h = Height;
		}
		else
			h = GLCD_SCREEN_SIZE_PAGE - Offset;

		Mask <<= Offset;

		GLCD_SetXY(X1, Y1);
		for(int i = 0; i < Width; i++)
		{
			Data = GLCD_ReadBuffer(GLCD_Parameters.Page, GLCD_Parameters.X);
			Data = ((Color == BLACK) ? (Data | Mask) : (Data & ~Mask));
			GLCD_WriteBuffer(GLCD_Parameters.Page, GLCD_Parameters.X++, Data);
		}

		while((h + GLCD_SCREEN_SIZE_PAGE) <= Height)
		{
			h += GLCD_SCREEN_SIZE_PAGE;
			Y1 += GLCD_SCREEN_SIZE_PAGE;
			GLCD_SetXY(X1, Y1);

			for(int i = 0; i < Width; i++)
				GLCD_WriteBuffer(GLCD_Parameters.Page, GLCD_Parameters.X++, Color);
		}

		if(h < Height)
		{
			Mask = ~(0xFF << (Height - h));
			GLCD_SetXY(X1, Y1 + GLCD_SCREEN_SIZE_PAGE);
			for(int i = 0; i < Width; i++)
			{
				Data = GLCD_ReadBuffer(GLCD_Parameters.Page, GLCD_Parameters.X);
				Data = ((Color == BLACK) ? (Data | Mask) : (Data & ~Mask));
				GLCD_WriteBuffer(GLCD_Parameters.Page, GLCD_Parameters.X++, Data);
			}
		}
	}
}

//**********************************************************************************************************

void GLCD_DrawBitmap(const uint8_t *Bitmap, uint8_t X, uint8_t Y,uint8_t Width, uint8_t Height)
{
	uint8_t Mask = 0x01;
	uint16_t Index_Bitmap = 0;
	uint8_t HeightBitmap, WidthBitmap;

	WidthBitmap = Width;
	HeightBitmap = Height;

	if((X + WidthBitmap) > GLCD_SCREEN_WIDTH)
		WidthBitmap = GLCD_SCREEN_WIDTH - X;

	if((Y + HeightBitmap) > GLCD_SCREEN_HEIGHT)
		HeightBitmap = GLCD_SCREEN_HEIGHT - Y;

	for(int i = Y; i < HeightBitmap; i+= GLCD_SCREEN_SIZE_PAGE)
	{
		for(int j = X; j < WidthBitmap; j++)
		{
			for(int Shift = 0; Shift < GLCD_SCREEN_SIZE_PAGE; Shift++)
			{
				GLCD_SetPixel(j, (i + Shift), ((Bitmap[Index_Bitmap] & (Mask << Shift)) ? BLACK : WHITE));
			}
			Index_Bitmap++;
		}
	}
}

//**********************************************************************************************************

void GLCD_DrawLine(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2, GLCD_Color_TypeDef Color)
{
	if ((X1 < GLCD_SCREEN_WIDTH) && (X2 < GLCD_SCREEN_WIDTH) &&
	(Y1 < GLCD_SCREEN_HEIGHT) && (Y2 < GLCD_SCREEN_HEIGHT))
	{
		if (X1 == X2)
			GLCD_Draw_V_Line(Y1, Y2, X1, Color);
		else if (Y1 == Y2)
			GLCD_Draw_H_Line(X1, X2, Y1, Color);
		else
		{
			uint8_t deltax, deltay, x, y, slope;
			int8_t error, ystep;
			slope = ((GLCD_ABS_DIFF(Y1, Y2) > GLCD_ABS_DIFF(X1,X2)) ? 1 : 0);
			if(slope)
			{
				//Swap x1, y1
				GLCD_SWAP(X1, Y1);
				//Swap x2, y2
				GLCD_SWAP(X2, Y2);
			}
			if(X1 > X2)
			{
				//Swap x1, x2
				GLCD_SWAP(X1, X2);
				//Swap y1,y2
				GLCD_SWAP(Y1, Y2);
			}

			deltax = X2 - X1;
			deltay = GLCD_ABS_DIFF(Y2, Y1);
			error = deltax / 2;
			y = Y1;
			ystep = ((Y1 < Y2) ? 1 : -1);

			for (x=X1 ; x<=X2 ; x++)
			{
				if (slope)
					GLCD_SetPixel(y, x, Color);
				else
					GLCD_SetPixel(x, y, Color);

				error -= deltay;
				if (error < 0)
				{
					y = y + ystep;
					error = error + deltax;
				}
			}
		}
	}
}

//**********************************************************************************************************

void GLCD_Draw_H_Line(uint8_t X1, uint8_t X2, uint8_t Y, GLCD_Color_TypeDef Color)
{
	if(X1 > X2)
		GLCD_SWAP(X1, X2);

	while(X1 <= X2)
	{
		GLCD_SetPixel(X1, Y, Color);
		X1++;
	}
}

//**********************************************************************************************************

void GLCD_Draw_V_Line(uint8_t Y1, uint8_t Y2, uint8_t X, GLCD_Color_TypeDef Color)
{
	if (Y1 > Y2)
		GLCD_SWAP(Y1, Y2);

	GLCD_SetPixels(X, Y1, X, Y2, Color);
}

//**********************************************************************************************************

void GLCD_DrawRectangle(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2, GLCD_Color_TypeDef Color)
{
	if((X1 < GLCD_SCREEN_WIDTH) && (X2 < GLCD_SCREEN_WIDTH) &&
			(Y1 < GLCD_SCREEN_HEIGHT) && (Y2 < GLCD_SCREEN_HEIGHT))
	{
		GLCD_Draw_H_Line(X1, X2, Y1, Color);
		GLCD_Draw_H_Line(X1, X2, Y2, Color);
		GLCD_Draw_V_Line(Y1, Y2, X1, Color);
		GLCD_Draw_V_Line(Y1, Y2, X2, Color);
	}
}

//**********************************************************************************************************

void GLCD_DrawRoundRectangle(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2, uint8_t Radius, GLCD_Color_TypeDef Color)
{
	if((X1<GLCD_SCREEN_WIDTH) && (X2<GLCD_SCREEN_WIDTH) &&
			(Y1<GLCD_SCREEN_HEIGHT) && (Y2<GLCD_SCREEN_HEIGHT))
	{
		int16_t tSwitch = 3 - 2 * Radius;
		uint8_t width, height, x, y;
		width = X2-X1;
		height = Y2-Y1;
		x = 0;
		y = Radius;

		//Draw perimeter
		GLCD_Draw_H_Line(X1+Radius, X2-Radius, Y1, Color);	//Top
		GLCD_Draw_H_Line(X1+Radius, X2-Radius, Y2, Color);	//Bottom
		GLCD_Draw_V_Line(Y1+Radius, Y2-Radius, X1, Color);	//Left
		GLCD_Draw_V_Line(Y1+Radius, Y2-Radius, X2, Color);	//Right

		while (x <= y)
		{
			//Upper left corner
			GLCD_SetPixel(X1+Radius-x, Y1+Radius-y, Color);
			GLCD_SetPixel(X1+Radius-y, Y1+Radius-x, Color);

			//Upper right corner
			GLCD_SetPixel(X1+width-Radius+x, Y1+Radius-y, Color);
			GLCD_SetPixel(X1+width-Radius+y, Y1+Radius-x, Color);

			//Lower left corner
			GLCD_SetPixel(X1+Radius-x, Y1+height-Radius+y, Color);
			GLCD_SetPixel(X1+Radius-y, Y1+height-Radius+x, Color);

			//Lower right corner
			GLCD_SetPixel(X1+width-Radius+x, Y1+height-Radius+y, Color);
			GLCD_SetPixel(X1+width-Radius+y, Y1+height-Radius+x, Color);

			if (tSwitch < 0)
				tSwitch += 4 * x + 6;
			else
			{
				tSwitch += 4 * (x - y) + 10;
				y--;
			}
			x++;
		}
	}
}

//**********************************************************************************************************

void GLCD_DrawTriangle(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2, uint8_t X3, uint8_t Y3, GLCD_Color_TypeDef Color)
{
	if(((X1 < GLCD_SCREEN_WIDTH) && (X2 < GLCD_SCREEN_WIDTH) && (X3 < GLCD_SCREEN_WIDTH) &&
			(Y1 < GLCD_SCREEN_HEIGHT) && (Y2 < GLCD_SCREEN_HEIGHT) && (Y3 < GLCD_SCREEN_HEIGHT)))
	{
		GLCD_DrawLine(X1, Y1, X2, Y2, Color);
		GLCD_DrawLine(X2, Y2, X3, Y3, Color);
		GLCD_DrawLine(X3, Y3, X1, Y1, Color);
	}
}

//**********************************************************************************************************

void GLCD_DrawCircle(uint8_t CenterX, uint8_t CenterY, uint8_t Radius, GLCD_Color_TypeDef Color)
{
	if(((CenterX + Radius) < GLCD_SCREEN_WIDTH) && ((CenterY + Radius) < GLCD_SCREEN_HEIGHT))
	{
		uint8_t x, y;
		int16_t xChange, radiusError;
		uint16_t yChange;
		x = Radius;
		y = 0;
		xChange = 1 - 2 * Radius;
		yChange = 1;
		radiusError = 0;

		while(x >= y)
		{
			GLCD_SetPixel(CenterX+x, CenterY+y, Color);
			GLCD_SetPixel(CenterX-x, CenterY+y, Color);
			GLCD_SetPixel(CenterX-x, CenterY-y, Color);
			GLCD_SetPixel(CenterX+x, CenterY-y, Color);
			GLCD_SetPixel(CenterX+y, CenterY+x, Color);
			GLCD_SetPixel(CenterX-y, CenterY+x, Color);
			GLCD_SetPixel(CenterX-y, CenterY-x, Color);
			GLCD_SetPixel(CenterX+y, CenterY-x, Color);
			y++;
			radiusError += yChange;
			yChange += 2;
			if((2 * radiusError + xChange) > 0)
			{
				x--;
				radiusError += xChange;
				xChange += 2;
			}
		}
	}
}

//**********************************************************************************************************

void GLCD_FillRectangle(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2, GLCD_Color_TypeDef Color)
{
	GLCD_SetPixels(X1, Y1, X2, Y2, Color);
}

//**********************************************************************************************************

void GLCD_FillRoundRectangle(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2, uint8_t Radius, GLCD_Color_TypeDef Color)
{
	if((X1 < GLCD_SCREEN_WIDTH) && (X2 < GLCD_SCREEN_WIDTH) &&
			(Y1 < GLCD_SCREEN_HEIGHT) && (Y2 < GLCD_SCREEN_HEIGHT))
	{
		int16_t tSwitch = 3 - 2 * Radius;
		uint8_t width, height, x, y;
		width = X2 - X1;
		height = Y2 - Y1;
		x = 0;
		y = Radius;

		//Fill center block
		GLCD_FillRectangle(X1+Radius, Y1, X2 - Radius, Y2, Color);

		while (x <= y)
		{
			//Left side
			GLCD_DrawLine(
			X1 + Radius - x, Y1 + Radius - y,				//Upper left corner upper half
			X1 + Radius - x, Y1 + height - Radius + y,		//Lower left corner lower half
			Color);
			GLCD_DrawLine(
			X1 + Radius - y, Y1 + Radius - x,					//Upper left corner lower half
			X1 + Radius - y, Y1 + height - Radius + x,			//Lower left corner upper half
			Color);

			//Right side
			GLCD_DrawLine(
			X1 + width - Radius	+ x, Y1 + Radius - y,			//Upper right corner upper half
			X1 + width - Radius + x, Y1 + height - Radius + y,	//Lower right corner lower half
			Color);
			GLCD_DrawLine(
			X1 + width - Radius + y, Y1 + Radius - x,			//Upper right corner lower half
			X1 + width - Radius + y, Y1 + height - Radius + x,	//Lower right corner upper half
			Color);

			if (tSwitch < 0)
			{
				tSwitch += 4 * x +6;
			}
			else
			{
				tSwitch += 4 * (x - y) + 10;
				y--;
			}
			x++;
		}
	}
}

//**********************************************************************************************************

void GLCD_FillTriangle(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t y2, uint8_t X3, uint8_t Y3, GLCD_Color_TypeDef Color)
{
	if(((X1 < GLCD_SCREEN_WIDTH) && (X2 < GLCD_SCREEN_WIDTH) && (X3 < GLCD_SCREEN_WIDTH) &&
			(Y1 < GLCD_SCREEN_HEIGHT) && (y2 < GLCD_SCREEN_HEIGHT) && (Y3 < GLCD_SCREEN_HEIGHT)))
	{
		uint8_t sl, sx1, sx2;
		double m1, m2, m3;
		sl = sx1 = sx2 = m1 = m2 = m3 = 0;

		if(y2 > Y3)
		{
			GLCD_SWAP(X2, X3);
			GLCD_SWAP(y2, Y3);
		}
		if(Y1 > y2)
		{
			GLCD_SWAP(X1, X2);
			GLCD_SWAP(Y1, y2);
		}
		m1 = (double)(X1 - X2) / (Y1 - y2);
		m2 = (double)(X2 - X3) / (y2 - Y3);
		m3 = (double)(X3 - X1) / (Y3 - Y1);
		for(sl = Y1 ; sl <= y2 ; sl++)
		{
			sx1= m1 * (sl - Y1) + X1;
			sx2= m3 * (sl - Y1) + X1;
			if (sx1> sx2)
			GLCD_SWAP(sx1, sx2);
			GLCD_DrawLine(sx1, sl, sx2, sl, Color);
		}
		for(sl = y2 ; sl <= Y3 ; sl++)
		{
			sx1= m2 * (sl - Y3) + X3;
			sx2= m3 * (sl - Y1) + X1;
			if (sx1 > sx2)
				GLCD_SWAP(sx1, sx2);
			GLCD_DrawLine(sx1, sl, sx2, sl, Color);
		}
	}
}

//**********************************************************************************************************

void GLCD_FillCircle(uint8_t CenterX, uint8_t CenterY, uint8_t Radius, GLCD_Color_TypeDef Color)
{
	if(((CenterX + Radius) < GLCD_SCREEN_WIDTH) &&
			((CenterY + Radius) < GLCD_SCREEN_HEIGHT))
	{
		int8_t f, ddF_x, ddF_y;
		uint8_t  x, y;
		f = 1 - Radius;
		ddF_x = 1;
		ddF_y = -2 * Radius;
		x = 0;
		y = Radius;


		//Fill in the center between the two halves
		GLCD_DrawLine(CenterX, CenterY - Radius, CenterX, CenterY + Radius, Color);

		while(x < y)
		{
			//ddF_x = 2 * x + 1;
			//ddF_y = -2 * y;
			//f = x*x + y*y - radius*radius + 2*x - y + 1;
			if (f >= 0)
			{
				y--;
				ddF_y += 2;
				f += ddF_y;
			}
			x++;
			ddF_x += 2;
			f += ddF_x;

			//Now draw vertical lines between the points on the circle rather than
			//draw the points of the circle. This draws lines between the
			//perimeter points on the upper and lower quadrants of the 2 halves of the circle.
			GLCD_Draw_V_Line(CenterY + y, CenterY - y, CenterX + x, Color);
			GLCD_Draw_V_Line(CenterY + y, CenterY - y, CenterX - x, Color);
			GLCD_Draw_V_Line(CenterY + x, CenterY - x, CenterX + y, Color);
			GLCD_Draw_V_Line(CenterY + x, CenterY - x, CenterX - y, Color);
		}
	}
}

//**********************************************************************************************************

void GLCD_InvertRect(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2)
{
	uint8_t width, height, offset, mask, h, i, data;

	width = X2 - X1 + 1;
	height = Y2 - Y1 + 1;
	offset = Y1 % GLCD_SCREEN_SIZE_PAGE;
	Y1 -= offset;
	mask = 0xFF;
	data = 0;

	//Calculate mask for top fractioned region
	if (height < (GLCD_SCREEN_SIZE_PAGE - offset))
	{
		mask >>= (GLCD_SCREEN_SIZE_PAGE - height);
		h = height;
	}
	else
		h = GLCD_SCREEN_SIZE_PAGE - offset;
	mask <<= offset;

	//Draw fractional rows at the top of the region
	GLCD_SetXY(X1, Y1);
	for (i = 0 ; i < width ; i++)
	{
		data = GLCD_ReadBuffer(GLCD_Parameters.Page, GLCD_Parameters.X);
		data = ((~data) & mask) | (data & (~mask));
		GLCD_WriteBuffer(GLCD_Parameters.Page, GLCD_Parameters.X++, data);
	}

	//Full rows
	while ((h + GLCD_SCREEN_SIZE_PAGE) <= height)
	{
		h += GLCD_SCREEN_SIZE_PAGE;
		Y1 += GLCD_SCREEN_SIZE_PAGE;
		GLCD_SetXY(X1, Y1);

		for (i=0 ; i < width ; i++)
		{
			data = ~GLCD_ReadBuffer(GLCD_Parameters.Page, GLCD_Parameters.X);
			GLCD_WriteBuffer(GLCD_Parameters.Page, GLCD_Parameters.X++, data);
		}
	}

	//Fractional rows at the bottom of the region
	if (h < height)
	{
		mask = ~(0xFF<<(height - h));
		GLCD_SetXY(X1, (Y1 + GLCD_SCREEN_SIZE_PAGE));

		for (i = 0 ; i < width ; i++)
		{
			data = GLCD_ReadBuffer(GLCD_Parameters.Page, GLCD_Parameters.X);
			data = ((~data) & mask) | (data & (~mask));
			GLCD_WriteBuffer(GLCD_Parameters.Page, GLCD_Parameters.X++, data);
		}
	}
}

//**********************************************************************************************************

