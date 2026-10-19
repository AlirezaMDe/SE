/*
    created with FontEditor written by H. Reddmann
    HaReddmann at t-online dot de

    File Name           : Symbol_16.h
    Date                : 3/15/2024
    Font size in bytes  : 0x0057, 87
    Font width          : 15
    Font height         : 16
    Font first char     : 0x00
    Font last char      : 0x03
    Font bits per pixel : 1
    Font is compressed  : false

    The font data are defined as

    struct _FONT_ {
     // common shared fields
       uint16_t   font_Size_in_Bytes_over_all_included_Size_it_self;
       uint8_t    font_Width_in_Pixel_for_fixed_drawing;
       uint8_t    font_Height_in_Pixel_for_all_Characters;
       uint8_t    font_Bits_per_Pixels;
                    // if MSB are set then font is a compressed font
       uint8_t    font_First_Char;
       uint8_t    font_Last_Char;
       uint8_t    font_Char_Widths[font_Last_Char - font_First_Char +1];
                    // for each character the separate width in pixels,
                    // characters < 128 have an implicit virtual right empty row
                    // characters with font_Char_Widths[] == 0 are undefined

     // if compressed font then additional fields
       uint8_t    font_Byte_Padding;
                    // each Char in the table are aligned in size to this value
       uint8_t    font_RLE_Table[3];
                    // Run Length Encoding Table for compression
       uint8_t    font_Char_Size_in_Bytes[font_Last_Char - font_First_Char +1];
                    // for each char the size in (bytes / font_Byte_Padding) are stored,
                    // this get us the table to seek to the right beginning of each char
                    // in the font_data[].

     // for compressed and uncompressed fonts
       uint8_t    font_data[];
                    // bit field of all characters
    }
*/

#ifndef Symbol_16_H
#define Symbol_16_H

#define Symbol_16_WIDTH 15
#define Symbol_16_HEIGHT 16

const uint8_t Symbol_16[] = {
    0x00, 0x57, 0x0F, 0x10, 0x01, 0x00, 0x03,
    0x05, 0x05, 0x0E, 0x0E,
    0x08, 0x00, 0x0C, 0x00, 0xFE, 0x7F, 0x0C, 0x00, 0x08, 0x00, 0x00, 0x10, 0x00, 0x30, 0xFE, 0x7F,
    0x00, 0x30, 0x00, 0x10, 0x00, 0x00, 0x7E, 0x00, 0x7C, 0x00, 0x78, 0x00, 0x70, 0x00, 0x60, 0x00,
    0x40, 0x00, 0x40, 0x00, 0x60, 0x00, 0x70, 0x00, 0x78, 0x00, 0x7C, 0x00, 0x7E, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x44, 0x00, 0x28, 0x00, 0x10, 0x00, 0x02, 0x00, 0x02, 0x00, 0x7E, 0x00, 0x7E,
    0x00, 0x02, 0x00, 0x02, 0x00, 0x10, 0x00, 0x28, 0x00, 0x44, 0x00, 0x00
};

#endif

