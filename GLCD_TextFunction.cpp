/*-----------------------------------------------------------------------------------------------------------
@file          	: GLCD_TextFunction.cpp
@author  		: Reza Farahani (nmmco2015@gmail.com)
C++ source 2024
-----------------------------------------------------------------------------------------------------------*/

#include "GLCD_TextFunction.h"
#include "Persian_Map.h"

//**********************************************************************************************************

const uint8_t 	*FontPointer;						// Font	Pointer
uint8_t			Rot 					= 0;		// Rot 0=0°, 1=90°
uint8_t 		Reverse					= 0;		// Reverse text direction on the display
uint8_t			Horizontal				= 1;		// 1: Portrait - 240x320 ,   2: Landscape - 320x240
uint8_t 		LastCharacter			= 0;  		// Last character buffer
GLCD_Color_TypeDef 		FontColor 				= BLACK;	// Text Font color, 5-6-5 RGB
GLCD_Color_TypeDef 		BackgroundColor 		= WHITE;	// Text background color, 5-6-5 RGB
uint8_t 		FontFixed				= 0;		// Text type 0=Proportional , 1=Fixed
uint8_t			NonTransparence 		= 1;		// Transparent 0=No, 1=Yes
uint16_t 		fontSize;							// size of current font
uint8_t 		firstchar;							// first character noumber of current font
uint8_t 		lastchar;							// last character noumber of current font
uint8_t 		charwidth;							// current character width register
uint8_t			FontWidth;							// max width of font
uint8_t			FontHeight;							// max height of font
uint8_t 		FontXScale 				= 1;		// X size of font
uint8_t 		FontYScale 				= 1;		// Y size of font

uint8_t 		FontSpaceX 				= 0;		// space between char
uint8_t 		FontSpaceY				= 0;		// space between lines

uint32_t 		cursorX 				= 0;		// x position
uint32_t 		cursorY 				= 0;		// y position


FontLetter_TypeDef peLETTER			= ENGLISH_LETTER;		// English or Persian letter

uint8_t			prevLet 				= 0xFF;		// previous persian character register
uint8_t			nextLet 				= 0xFF;		// next persian character register

//**********************************************************************************************************

uint8_t Bit_Is_Set(uint8_t Byte, uint8_t Mask)
{
	if(Byte & (0x01 << Mask))
		return 1;
	else
		return 0;
}

//**********************************************************************************************************

void PutCharEnglish(uint8_t CharEnglish)
{
	uint8_t	byte = 0;
	uint8_t	bitoffset = 0;
	uint8_t	maske = 0;
	uint16_t bcounter	= 0;
	uint16_t bitsbischar = 0;
	uint16_t bytesbischar = 0;
	uint16_t xPos,yPos;
	uint8_t Ccounter = 0;
	uint8_t xc = 0;
	uint8_t yc = 0;
	uint8_t sx = 0;
	uint8_t sy = 0;

   // get current character width
	charwidth = (FontPointer[(((uint32_t)CharEnglish) + FONT_HEADER_SIZE - firstchar)]);

	if(CharEnglish == '\n')
	{
		cursorX = 0;
		cursorY = cursorY + (uint32_t)FontHeight * FontYScale;
		return;
	}

	// character out of range.
	if((CharEnglish < firstchar) || (CharEnglish > lastchar) || (fontSize == 0))
	{
		return;
	}
	// character is not in list.
	if (charwidth == 0)
	{
		return;
	}
	// sara thai font. line remain at last position
	if(FontFixed)
	{
		charwidth = FontWidth;
	}
	else
	{
		//english special fonts!
		if (((CharEnglish >= 0xd4) && (CharEnglish <= 0xda)) ||
	  	  ((CharEnglish >= 0xe7) && (CharEnglish <= 0xec)) ||
				 (CharEnglish == 0xd1))
		{
			cursorX = cursorX - (uint32_t)charwidth * FontXScale;
		}
	}

	// fixed width for digit
 	if((CharEnglish >= '0') && (CharEnglish <= '9'))		// english : 0 to 9
		charwidth = (FontPointer[(uint32_t)(FONT_HEADER_SIZE)+'0'-firstchar]);		//width reference = ZERO

	// line adjust
	if(((int)cursorX + charwidth * FontXScale) > GET_MAX_X())
	{
		cursorY = cursorY + (uint32_t)FontHeight * FontYScale;
		cursorX = 0;
	}

	// calculate current character position on the table
	for(Ccounter = 0; Ccounter < CharEnglish-firstchar; Ccounter++)
		bitsbischar += ((FontPointer[(uint32_t)(Ccounter)+FONT_HEADER_SIZE]));	//c0_width +...+ cn_width


	bitsbischar *= FontHeight;																										//c_widths * FH
	bitsbischar += (((uint16_t)(lastchar)-firstchar)+FONT_HEADER_SIZE)*8;		//plus font headers
	bytesbischar = bitsbischar/8;																//
	bitoffset = bitsbischar % 8;															//
	maske = bitoffset % 8;																//

	// draw character
	for(xc = 0; xc < charwidth; xc++)
	{
		for(yc = 0; yc < FontHeight; yc++)
		{
			if(maske > 7)
			{
				maske = 0;
				bcounter += 1;
			}
			byte = (FontPointer[bytesbischar + bcounter + 1]);

			xPos = (uint16_t)xc*FontXScale + cursorX;

			if(Horizontal)	//128x64
			{
				if(Reverse)
					yPos = (((uint16_t)FontHeight-yc)*FontYScale + cursorY);
				else
					yPos = (((uint16_t)yc)*FontYScale + cursorY);
			}
			else						//64x128
			{
				if(Reverse)
					yPos = (((uint16_t)FontHeight-yc)*FontYScale + cursorY);
				else
					yPos = (((uint16_t)yc)*FontYScale + cursorY);

			}	//Horizontal

			for(sx = 0; sx < FontXScale; sx++)
			{
				for(sy = 0; sy < FontYScale; sy++)
				{
					if(Bit_Is_Set(byte, maske))
					{
						if(Rot)
							PutPixel(yPos+sy, xPos+sx, FontColor);
						else
							PutPixel(xPos+sx, yPos+sy, FontColor);
					}
					else
					{
						if(NonTransparence)
						{
							if(Rot)
								PutPixel(yPos+sy, xPos+sx, BackgroundColor);
							else
								PutPixel(xPos+sx, yPos+sy, BackgroundColor);
						}
					}
				};//for sy
			};//for sx

			maske++;
		};//for yc
	};//for xc

	// adjust cursor to next position - english
	//cursorX	+= (uint32_t)charwidth * FontXScale + FontSpace;
	cursorX	+= (uint32_t)charwidth * FontXScale + FontSpaceX;
}	//*PutCharEN

//**********************************************************************************************************

void PutCharPersian(uint8_t CharPersian)
{
	uint8_t	byte = 0;
	uint8_t	bitoffset = 0;
	uint8_t	maske = 0;
	uint16_t bcounter = 0;
	uint16_t bitsbischar = 0;
	uint16_t bytesbischar = 0;
	uint16_t xPos,yPos;
	uint8_t Ccounter = 0;
	uint8_t xc = 0;
	uint8_t yc = 0;
	uint8_t sx = 0;
	uint8_t sy = 0;

	// get current character width
	charwidth = FontPointer[(((uint32_t)CharPersian) + FONT_HEADER_SIZE - firstchar)];

	// line feed, goto next line
	if(CharPersian == '\n')
	{
		cursorX = GET_MAX_X();
		cursorY += (uint32_t)(FontHeight +FontSpaceY)* FontYScale;
		return;
	}

	// character out of range.
	if((CharPersian < firstchar) || (CharPersian > lastchar) || (fontSize == 0))
	{
		return;
	};
	// character is not in list.
	if(charwidth == 0)
	{
		return;
	};
	// sara thai font. line remain at last position
	if(FontFixed)
	{
		charwidth = FontWidth;
	};
	// line adjust
	if(((int)cursorX - charwidth * FontXScale) < 0)
	{
		cursorY = cursorY + (uint32_t)FontHeight * FontYScale;
		cursorX = GET_MAX_X() - (uint32_t)charwidth * FontXScale;
	}

	// adjust cursor to current position - persian
	cursorX	-= (uint32_t)charwidth * FontXScale + FontSpaceX;

	// calculate current character position on the table
	for(Ccounter = 0; Ccounter < CharPersian-firstchar; Ccounter++)
		bitsbischar += ((FontPointer[(uint32_t)(Ccounter)+FONT_HEADER_SIZE]));	//c0_width +...+ cn_width

	bitsbischar *= FontHeight;																										//c_widths * FH
	bitsbischar += (((uint16_t)(lastchar)-firstchar)+FONT_HEADER_SIZE)*8;		//plus font headers
	bytesbischar = bitsbischar/8;									//
	bitoffset = bitsbischar % 8;								//
	maske = bitoffset % 8;									//

	// draw character
	for(xc = 0; xc < charwidth; xc++)
	{
		for(yc = 0; yc < FontHeight; yc++)
		{
			if(maske > 7)
			{
				maske = 0;
				bcounter+=1;
			}
			byte = (FontPointer[bytesbischar + bcounter + 1]);

			xPos = (uint16_t)xc*FontXScale + cursorX;

			if(Horizontal)	//128x64
			{
				if(Reverse)
					yPos = (((uint16_t)FontHeight-yc)*FontYScale + cursorY);
				else
					yPos = (((uint16_t)yc)*FontYScale + cursorY);
			}
			else						//64x128
			{
				if(Reverse)
					yPos = (((uint16_t)FontHeight-yc)*FontYScale + cursorY);
				else
					yPos = (((uint16_t)yc)*FontYScale + cursorY);

			}	//Horizontal

			for(sx = 0; sx < FontXScale; sx++)
			{
				for(sy = 0; sy < FontYScale; sy++)
				{
					if(Bit_Is_Set(byte, maske))
					{
						if(Rot)
							PutPixel(yPos+sy, xPos+sx, FontColor);
						else
							PutPixel(xPos+sx, yPos+sy, FontColor);
					}
					else
					{
						if(NonTransparence)
						{
							if(Rot)
								PutPixel(yPos+sy, xPos+sx, BackgroundColor);
							else
								PutPixel(xPos+sx, yPos+sy, BackgroundColor);
						}
					}
				};//for sy
			};//for sx

			maske++;
		}//for yc
	}//for xc
}	//*PutCharPE

//**********************************************************************************************************

uint32_t CalcTextWidthEnglish(char *str)
{
	uint32_t strSize = 0;
	uint8_t c;
	uint32_t i = 0;

	while(str[i])
	{
		c = str[i++];

		if(c == '\n')
			continue;

		if(FontFixed)
			strSize += (uint32_t)(FontWidth) * FontXScale;
		else
		{
		  if((c < firstchar) || (c > lastchar))
				charwidth = FontWidth;
			else
			{
				charwidth = (FontPointer[(uint32_t)(c)+FONT_HEADER_SIZE-firstchar]);

				//english spesial fonts!
				if (((c >= 0xD4) && (c <= 0xDA)) ||
		    		((c >= 0xE7) && (c <= 0xEC)) ||
				  	 (c == 0xD1))

					charwidth = 0;
			}

			strSize += (uint32_t)(charwidth) * FontXScale;
		}

		strSize += FontSpaceX;
	}//while

	return strSize;
}

//**********************************************************************************************************

uint32_t CalcTextWidthPersian(char *str)
{
	uint8_t curnLet, dumyLet;
	uint8_t Pstat, stat, Nstat;
	uint8_t backFontSpace =FontSpaceX;
	uint32_t strSize = 0;
	uint32_t i = 0;

 	while(str[i])
	{
		curnLet = str[i++];
		if(curnLet == '\n')
			continue;

		if(FontFixed)
			strSize += (uint32_t)(FontWidth) * FontXScale;
		else
		{
		 	if((curnLet < firstchar) || (curnLet > lastchar))
				charwidth = FontWidth;
			else
			{
		 		switch(curnLet)
		 		{
		 			case 0x81:				//peh
		    		curnLet = 0xEA;
		 				break;
		 			case 0x8D:				//cheh
		    		curnLet = 0xEB;
		 				break;
		 			case 0x8E:				//zheh
		    		curnLet = 0xEE;
		 				break;
		 			case 0x90:				//geh
		    		curnLet = 0xEF;
		 				break;
					case 0x98:				//Ke
		    		curnLet = 0xDF;
		 				break;
		 			case 0xE1:				//laa
		    		nextLet = str[i++];

		    		if(nextLet == 0xC7)
		    			curnLet = 0xFB;
		    		else
		    		{
		    			i--;
		    			curnLet = 0xE1;
		    		}
		 				break;
		 			case 0x30:				//persian digits 0...9
					case 0x31:
					case 0x32:
					case 0x33:
					case 0x34:
					case 0x35:
					case 0x36:
					case 0x37:
					case 0x38:
					case 0x39:
						curnLet -= 0x20;
		 				break;
		 			case 0x3F:				//persian question mark
		    		curnLet = 0xBF;
		 				break;
		 			case 0x3B:				//persian semicolon
		    		curnLet = 0x1F;
		 				break;
		 			case 0x2C:				//persian comma
		    		curnLet = 0x1D;
		 				break;
		 			case 0x2E:				//persian point
		    		curnLet = 0x1C;
		 				break;
				}//switch curnLet

				//--------------------------------
		  	nextLet = str[i++];
		  	i--;

		 		switch(nextLet)
		 		{
		 			case 0x81:				//peh
		    		nextLet = 0xEA;
		 				break;
		 			case 0x8D:				//cheh
		    		nextLet = 0xEB;
		 				break;
		 			case 0x8E:				//zheh
		    		nextLet = 0xEE;
		 				break;
		 			case 0x90:				//geh
		    		nextLet = 0xEF;
		 				break;
					case 0x98:				//Ke
		    		nextLet = 0xDF;
		 				break;
		 			case 0x30:				//0...9
					case 0x31:
					case 0x32:
					case 0x33:
					case 0x34:
					case 0x35:
					case 0x36:
					case 0x37:
					case 0x38:
					case 0x39:
						nextLet -= 0x20;
		 				break;
		 			case 0x3F:				//persian question mark
		    		nextLet = 0xBF;
		 				break;
		 			case 0x3B:				//persian semicolon
		    		nextLet = 0x1F;
		 				break;
		 			case 0x2C:				//persian comma
		    		nextLet = 0x1D;
		 				break;
		 			case 0x2E:				//persian point
		    		nextLet = 0x1C;
		 				break;
				}//switch nextLet

				if(curnLet > 0xC0)
				{
					if(prevLet > 0xC0)
						Pstat = (((PersianMap[prevLet-0xC1][5])));
					else
						Pstat = 0;

					if(nextLet > 0xC0)
						Nstat = (((PersianMap[nextLet-0xC1][4])));
					else
						Nstat = 0;

					stat = (Pstat<<1) | Nstat;

					if(stat>1)	FontSpaceX = 0;
					else 				FontSpaceX = backFontSpace;
					////////////////
					dumyLet = ((PersianMap[curnLet-0xC1][stat]));

					charwidth = (FontPointer[(uint32_t)(dumyLet)+FONT_HEADER_SIZE-firstchar]);
					strSize += (uint32_t)(charwidth) * FontXScale;
					strSize += FontSpaceX;
					////////////////
					FontSpaceX = backFontSpace;
				}
				else
				{
					charwidth = (FontPointer[(uint32_t)(curnLet)+FONT_HEADER_SIZE-firstchar]);
					strSize += (uint32_t)(charwidth) * FontXScale;
					strSize += FontSpaceX;
				}

				prevLet = curnLet;
			}
		}
	}//while

 	prevLet = 0xFF;

	return(strSize);
}

//**********************************************************************************************************

uint32_t CalcTextWidth(char *Str)
{
	if(peLETTER == ENGLISH_LETTER)
		return CalcTextWidthEnglish(Str);
	else
		return CalcTextWidthPersian(Str);
}

//**********************************************************************************************************

uint32_t CalcTextHeight(void)
{
	return((uint32_t)(FontHeight) * FontYScale);
}

//**********************************************************************************************************

char UTF8_To_ASCII(char Ascii)
{
    if(Ascii < 128)   // Standard ASCII-set 0..0x7F handling
    {
    	LastCharacter = 0;
        return Ascii;
    }

    // get previous input
    char last = LastCharacter;   // get last char
    LastCharacter = Ascii;         // remember actual character

    switch(last)     // conversion depending on first UTF8-character
    {
    	case 0xD8:
    	{
    		if((Ascii >= 0xA0) && (Ascii <= 0xB6))
    		{
    			return (Ascii + 0x20);
    			break;
    		}

    		if((Ascii >= 0xB7) && (Ascii <= 0xBA))
    		{
    			return (Ascii + 0x21);
    			break;
    		}
    	}
    	case 0xD9:
    	{
    		if((Ascii >= 0x80) && (Ascii <= 0x83))
    		{
    			return (Ascii + 0x5C);
    			break;
    		}

    		if(Ascii == 0x84)
    		{
    			return (Ascii + 0x5D);
    			break;
    		}

    		if((Ascii >= 0x85) && (Ascii <= 0x88))
    		{
    			return (Ascii + 0x5E);
    			break;
    		}

    		if((Ascii == 0x89) || (Ascii == 0x8A))
    		{
    			return (Ascii + 0x63);
    			break;
    		}

    		if(Ascii == 0xBE)
    		{
    			return 0x81;
    			break;
    		}
    	}
    	case 0xDA:
    	{
    		if(Ascii == 0x86)
    		{
    			return 0x8D;
    			break;
    		}

    		if(Ascii == 0x98)//zhe
    		{
    			return 0x8E;
    			break;
    		}

    		if(Ascii == 0xAF)
    		{
    			return 0x90;
    			break;
    		}

    		if(Ascii == 0xA9)//ke
    		{
    			return 0xDF;
    			break;
    		}
    	}
    	case 0xDB:
    	{
    		if(Ascii == 0x8C)
    		{
    			return 0xED;
    			break;
    		}
    	}
	}

    return 0;                                     // otherwise: return zero, if character has to be ignored
}

//**********************************************************************************************************

void PutsStringPersian(char *strPersian)
{
	uint8_t curnLet;
	uint8_t Pstat, stat, Nstat;
	uint8_t backFontSpace = FontSpaceX;
	char strTemp[50];
	char str[50];
	uint8_t i = 0, j = 0, c = 0;
	uint32_t k = 0;

	// ****** UTF8-Decoder: convert UTF8-string to extended ASCII *******
	for (i = 0; i < strlen(strPersian); i++)
	{
		c = UTF8_To_ASCII(strPersian[i]);
		if (c!=0)
			str[k++]=c;
	}

	str[k] = 0;
	i = 0;
	k=0;

 	while(str[k])
	{
 		curnLet = str[k++];

 		switch(curnLet)
 		{
 			case 0x81:				//peh
    		curnLet = 0xEA;
 				break;
 			case 0x8D:				//cheh
    		curnLet = 0xEB;
 				break;
 			case 0x8E:				//zheh
    		curnLet = 0xEE;
 				break;
 			case 0x90:				//geh
    		curnLet = 0xEF;
 				break;
			case 0x98:				//Ke
		    curnLet = 0xDF;
		 		break;
 			case 0xE1:				//laa
    		nextLet = str[k++];

    		if(nextLet == 0xC7)
    			curnLet = 0xFB;
    		else
    		{
    			k--;
    			curnLet = 0xE1;
    		}
 				break;
 			case 0x3F:				//persian question mark
    		curnLet = 0xBF;
 				break;
 			case 0x3B:				//persian semicolon
    		curnLet = 0x1F;
 				break;
 			case 0x2C:				//persian comma
    		curnLet = 0x1D;
 				break;
 			case 0x2E:				//persian point
    		curnLet = 0x1C;
 				break;
		}//switch curnLet

		//--------------------------------
  	nextLet = str[k++];
  	k--;

 		switch(nextLet)
 		{
 			case 0x81:				//peh
    		nextLet = 0xEA;
 				break;
 			case 0x8D:				//cheh
    		nextLet = 0xEB;
 				break;
 			case 0x8E:				//zheh
    		nextLet = 0xEE;
 				break;
 			case 0x90:				//geh
    		nextLet = 0xEF;
 				break;
			case 0x98:				//Ke
		    nextLet = 0xDF;
		 		break;
 			case 0x3F:				//persian question mark
    		nextLet = 0xBF;
 				break;
 			case 0x3B:				//persian semicolon
    		nextLet = 0x1F;
 				break;
 			case 0x2C:				//persian comma
    		nextLet = 0x1D;
 				break;
 			case 0x2E:				//persian point
    		nextLet = 0x1C;
 				break;
		}//switch nextLet

		if(curnLet > 0xC0)
		{
			if(prevLet > 0xC0)
				Pstat = (((PersianMap[prevLet-0xC1][5])));		// 1: prevLet attach to curnLet -- 0: prevLet don't attach to curnLet
			else
				Pstat = 0;

			if(nextLet > 0xC0)
				Nstat = (((PersianMap[nextLet-0xC1][4])));		// 1: nextLet attach to curnLet -- 0: nextLet don't attach to curnLet
			else
				Nstat = 0;

			//			Pstat	|	Nstat	|	stat
			//		 -------+-------+------
			//				0		|		0		|		0				curnLet, don't attach to prevLet and nextLet
			//				0		|		1		|		1				curnLet, don't attach to prevLet and attach to nextLet
			//				1		|		0		|		2				curnLet, attach to prevLet and don't attach to nextLet
			//				1		|		1		|		3				curnLet, attach to prevLet and nextLet

			stat = (Pstat<<1) | Nstat;

			if(stat>1)
				FontSpaceX = 0;
			else
				FontSpaceX = backFontSpace;

			PutCharPersian(((PersianMap[curnLet-0xC1][stat])));
			FontSpaceX = backFontSpace;
		}
		else	//Original 'curnLet' below 193 without ( � � � � )  -- for digits and symbols!
		{
			if((curnLet >= '0') && (curnLet <= '9'))		// 0 to 9
			{
				i++;
				strTemp[i] 	 = curnLet;

				if(!((nextLet >= '0') && (nextLet <= '9')))
				{
					for(j=i; j>0; j--)
						PutCharPersian(strTemp[j]-0x20);

					i = 0;
				}
			}
			else
				PutCharPersian(curnLet);
		}

		prevLet = curnLet;
	}//while

 	prevLet = 0xFF;
}

//**********************************************************************************************************

void SetFont(const uint8_t *pointer)
{
	FontPointer = pointer;
	FontWidth = (FontPointer[2]);
	FontHeight = (FontPointer[3]);
	fontSize = (uint16_t)((FontPointer[1])) << 8;
	fontSize += (FontPointer[0]);
	firstchar = (FontPointer[5]);
	lastchar = (FontPointer[6]);
}

//**********************************************************************************************************

void SetFontLetter(FontLetter_TypeDef FontLetter)
{
	peLETTER = FontLetter;

	if(FontLetter == PERSIAN_LETTER)
		cursorX = GET_MAX_X();
	else
		cursorX = 0;
}

//**********************************************************************************************************

void SetTextPixelPosition(uint8_t X, uint8_t Y)
{
	if(peLETTER == PERSIAN_LETTER)
		cursorX = GET_MAX_X() - X;
	else
		cursorX = X;

	cursorY = Y;
}

//**********************************************************************************************************

void GetTextPixelPosition(uint8_t *X, uint8_t *Y)
{
	if(peLETTER == PERSIAN_LETTER)
		*X = GET_MAX_X() - cursorX;
	else
		*X = cursorX;

	*Y = cursorY;
}

//**********************************************************************************************************

void SetTextPosition(uint8_t Line, uint8_t Column)
{
	if(peLETTER == PERSIAN_LETTER)
		cursorX = GET_MAX_X() - (uint32_t)FontWidth  * FontXScale * Column;
	else
		cursorX = (uint32_t)FontWidth  * FontXScale * Column;

	cursorY = (uint32_t)(FontHeight +FontSpaceY)* FontYScale * Line;
}

//**********************************************************************************************************

void SetLinePixelPosition_X(uint8_t Line, uint8_t X)
{
	if(peLETTER == PERSIAN_LETTER)
		cursorX =GET_MAX_X() - X;
	else
		cursorX = X;

	cursorY =(uint32_t)(FontHeight +FontSpaceY)* FontYScale * Line;
}

//**********************************************************************************************************

void PutChar(uint8_t Char)
{
	if(peLETTER == PERSIAN_LETTER)
		PutCharPersian(Char);
	else
		PutCharEnglish(Char);
}

//**********************************************************************************************************

void PutsString(char *Str)
{
	if(peLETTER == PERSIAN_LETTER)
		PutsStringPersian(Str);
	else
		PutsStringEnglish(Str);
}

//**********************************************************************************************************

void PutsStringEnglish(char *strEnglish)
{
	uint32_t i = 0;

	while(strEnglish[i])
		PutCharEnglish(strEnglish[i++]);
}

//**********************************************************************************************************

void DrawStringAt(uint8_t Line, uint8_t Column, char *Str, GLCD_Color_TypeDef FontColor, GLCD_Color_TypeDef backgroundColor)
{
	SetFontColor(FontColor);
	SetBackgroundColor(backgroundColor);
	SetTextPosition(Line, Column);
	PutsString(Str);
}

//**********************************************************************************************************






