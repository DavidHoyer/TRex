/*
 * Object.h
 *
 *  Created on: 11.10.2022
 *      Author: lukir
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include "pixelData.h"

/****************************************/
/*  Declaration of the global variables */
/****************************************/

typedef struct
{
	unsigned int	x;		// X Position
	unsigned int	y;		// Y Position
	unsigned int	w;		// Width
	unsigned int 	h;		// Height
	unsigned int 	size;	// number of pixels

    unsigned int	(*pixels)[4];	//pointer to pixelData Array
}bmp_t;

/*BM grafic TRex
		DataStart: 54
		With: 135
		Length: 144

		Bits/Pixel: 32*/

bmp_t tRexBmp = {50, 250, 135, 100, (135*100), tRex_pixelData};

/*BM
DataStart: 122
With: 75
Length: 75

Bits/Pixel: 32*/

bmp_t JumpButtonBMP = {0, 0,  100, 60, (100*60), JumpButton_pixelData};

#endif /* OBJECT_H_ */
