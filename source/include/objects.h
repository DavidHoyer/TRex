/*
 * Objects.h
 *
 *  Created on: 30.12.2022
 *      Author: lukir
 */

#ifndef INCLUDE_OBJECTS_H_
#define INCLUDE_OBJECTS_H_

//#include <peripherals/external/lcd.c>
#include <leguan.h>

#define TRUE		1
#define FALSE		0
#define BGRA565_COLOR(r, g, b, a) 	{ ((uint8_t)(b)) >> 3, ((uint8_t)(g)) >> 2, ((uint8_t)(r)) >> 3, ((uint8_t)(a)) }

//*******************************************************************
//*** BMP Struct
//*******************************************************************

typedef struct node
{
	uint16_t		x;
	uint16_t		y;

	struct node	*next;
}node_t;

typedef struct
{
	uint16_t		x;				// X Position
	uint16_t		y;				// Y Position
	uint16_t		w;				// Width
	uint16_t 		h;				// Height

	unsigned char	(*pixels)[4];	//pointer to pixelData Array

	unsigned char	visible;		// visibility flag
	node_t			*head;
}bmp_t;

void DrawBmp(bmp_t *bmp, const uint16_t x, const uint16_t y);
void MoveBmp(bmp_t *bmp, const uint16_t x, const uint16_t y);
void ShiftBmp(bmp_t *bmp, const uint16_t ix, const uint16_t iy);
void DeleteBmp(bmp_t *bmp);
void LcdClearArea(const uint16_t x1, const uint16_t y1, const uint16_t x2, const uint16_t y2);
node_t *GetBoarder (bmp_t bmp);
void PrintBorder (bmp_t bmp);
void ConvertArray(unsigned char pixelData[][4], const uint16_t w, const uint16_t h);

#endif /* INCLUDE_OBJECTS_H_ */
