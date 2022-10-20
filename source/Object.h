/*
 * Object.h
 *
 *  Created on: 11.10.2022
 *      Author: lukir
 */

#ifndef OBJECT_H_
#define OBJECT_H_

/****************************************/
/*  Declaration of the global variables */
/****************************************/
//unsigned char tRex_pixelData[2900][4];

typedef struct
{
	unsigned int 	x;		//X Position
	unsigned int 	y;		//Y Position

	unsigned char 	R;		// Red
	unsigned char 	G;		// Green
	unsigned char 	B;		// Blue
	unsigned char 	T;		// Transparency
}pixel_t;

typedef struct
{
	unsigned char	x;		// X Position
	unsigned char	y;		// Y Position
	unsigned char	w;		// Width
	unsigned char 	h;		// Height
	unsigned int 	size;	// number of pixels

    unsigned char	pixels[][4];
}bmp_t;

#endif /* OBJECT_H_ */
