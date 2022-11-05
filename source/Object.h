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

    unsigned char	(*pixels)[4];	//pointer to pixelData Array

	unsigned char	visible;	// visibility flag
}bmp_t;


/*Bitmap File Header
File type:        BMDino.txt
Bitmap file size: 54054
Reserved 1:       0
Reserved 2:       0
Data start:       54

Bitmap Info Header
Size of Header:       40
Width in pixel:       135
Height in pixel:      100
Nr. of clr planes:    1
Bits per pixel:       32
Compression method:   0
Image size:           0
X pixel per meter:    2834
Y pixel per meter:    2834
Number of colors:     0
Important colors:     0*/

bmp_t tRexBmp = {50, 250, 135, 100, tRex_pixelData, 0};

/*Bitmap File Header
File type:        BMbutton_menu_start.txt
Bitmap file size: 273722
Reserved 1:       0
Reserved 2:       0
Data start:       122

Bitmap Info Header
Size of Header:       108
Width in pixel:       400
Height in pixel:      171
Nr. of clr planes:    1
Bits per pixel:       32
Compression method:   3
Image size:           273722
X pixel per meter:    2834
Y pixel per meter:    2834
Number of colors:     0
Important colors:     0*/

bmp_t button_START_BMP = {200, 150, 400, 171, button_START_pixelData, 0};

/*Bitmap File Header
File type:        BMtree.txt
Bitmap file size: 90122
Reserved 1:       0
Reserved 2:       0
Data start:       122

Bitmap Info Header
Size of Header:       108
Width in pixel:       150
Height in pixel:      150
Nr. of clr planes:    1
Bits per pixel:       32
Compression method:   3
Image size:           90122
X pixel per meter:    3780
Y pixel per meter:    3780
Number of colors:     0
Important colors:     0*/

bmp_t obstacle_tree_BMP = { 600, 250, 150, 150, TREE_pixelData, 0};



#endif /* OBJECT_H_ */
