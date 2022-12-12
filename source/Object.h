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
	uint16_t		x;				// X Position
	uint16_t		y;				// Y Position
	uint16_t		w;				// Width
	uint16_t 		h;				// Height

    unsigned char	(*pixels)[4];	//pointer to pixelData Array

	unsigned char	visible;		// visibility flag

	struct border *head;
}bmp_t;

struct border
{
	uint16_t		x;
	uint16_t		y;

	struct	border	*next;
};


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

bmp_t tRexBmp = {50, 250, 135, 100, tRex_pixelData, 0,0};

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

bmp_t button_START_BMP = {200, 150, 400, 171, button_START_pixelData, 0,0};

/*Bitmap File Header
File type:        BMC:\Users\lukir\OneDrive\Documents\Studium\Semester_3\BTE5057_Software-Projekte\bmp_Converter\bmp_Converter\tree.txt
Bitmap file size: 67254
Reserved 1:       0
Reserved 2:       0
Data start:       54

Bitmap Info Header
Size of Header:       40
Width in pixel:       112
Height in pixel:      150
Nr. of clr planes:    1
Bits per pixel:       32
Compression method:   0
Image size:           0
X pixel per meter:    2834
Y pixel per meter:    2834
Number of colors:     0
Important colors:     0*/

bmp_t obstacle_tree_BMP = { 600, 250, 112, 150, TREE_pixelData, 0,0};

#endif /* OBJECT_H_ */
