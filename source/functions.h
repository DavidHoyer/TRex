/*
 * functions.h
 *
 *  Created on: 13.12.2022
 *      Author: david
 */



#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
/*
//include general librarys
#include <leguan.h>
#include <cube.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>


#include "Object.h"
*/
//*********************************************************************
//*** Defines    													***
//*********************************************************************

#define TRUE		1
#define FALSE		0

#define GROUND_HEIGHT	LCD_HEIGHT - 100

//*********************************************************************
//*** Print Bmp to LCD												***
//*********************************************************************
/*
void DrawBmp(bmp_t *Bmp, const uint16_t x, const uint16_t y){
	Bmp->visible = 1;
	Bmp->x = x;
	Bmp->y = y;
	uint32_t i;

	 //R_TRY(LCD_SetDrawArea(x, y, x + Bmp->w, y + Bmp->h));
	 LCD_EnableDrawMode();

	for(uint16_t yi = 1; yi < Bmp->h-1; yi++){
		for(uint16_t xi = 1; xi < Bmp->w-1; xi++){


			i = yi*Bmp->w + xi;

			color_t pixelClr = BGRA565_COLOR(	*(*(Bmp->pixels + i) +0),
												*(*(Bmp->pixels + i) +1),
												*(*(Bmp->pixels + i) +2),
												*(*(Bmp->pixels + i) +3));

			if(pixelClr.a == 0){ 	//Skip transparent pixels

				continue;
			}

			//LCD_Set(&pixelClr);
			//LCD_Pixel(Bmp->x + xi, Bmp->y + (Bmp->h - yi));
		}
	}
}
*/

void DrawBmp(bmp_t *Bmp, const uint16_t x, const uint16_t y){
	Bmp->visible = 1;
	Bmp->x = x;
	Bmp->y = y;
	uint32_t i;

	for(uint16_t yi = 1; yi < Bmp->h-1; yi++){
		for(uint16_t xi = 1; xi < Bmp->w-1; xi++){


			i = yi*Bmp->w + xi;

			if ( *(*(Bmp->pixels + i) +3) != *(*(Bmp->pixels + i + 1) +3)
				|| *(*(Bmp->pixels + i) +3) != *(*(Bmp->pixels + i - 1) +3)){

				LCD_SetForegroundColor(ColorWhite);
				LCD_Pixel(Bmp->x + xi, Bmp->y + (Bmp->h - yi));
				continue;
			}


			if (*(*(Bmp->pixels + i) +3) != *(*(Bmp->pixels + i +Bmp->w) +3)
				|| *(*(Bmp->pixels + i) +3) != *(*(Bmp->pixels + i - Bmp->w) +3)) {

				LCD_SetForegroundColor(ColorWhite);
				LCD_Pixel(Bmp->x + xi, Bmp->y + (Bmp->h - yi));
				continue;
			}

			color_t pixelClr = BGRA565_COLOR(	*(*(Bmp->pixels + i) +0),
												*(*(Bmp->pixels + i) +1),
												*(*(Bmp->pixels + i) +2),
												*(*(Bmp->pixels + i) +3));

			if(pixelClr.a == 0){ 	//Skip transparent pixels

				continue;
			}

			LCD_SetForegroundColor(pixelClr);
			LCD_Pixel(Bmp->x + xi, Bmp->y + (Bmp->h - yi));
		}
	}
}


void MoveBmp(bmp_t *Bmp, const uint16_t x, const uint16_t y){

	//uint8_t boarder[Bmp->w][Bmp->h];

	//uint32_t i;

	if(Bmp->visible == FALSE)
		return;

	if (x > LCD_WIDTH || y > LCD_HEIGHT)
		return;
/*
	if (Bmp->x - x != 1 && Bmp->x - x != -1 && Bmp->y - y != 1 && Bmp->y - y != -1) {
		LCD_SetForegroundColor(ColorWhite);
		LCD_Rect(Bmp->x, Bmp->y, Bmp->w, Bmp->h);
	}
	else {
		//PrintBorder(Bmp);
	}



	LCD_SetForegroundColor(ColorWhite);

	for (uint16_t xi = 0; xi < Bmp->w; xi++){
		for (uint16_t yi = 0; yi < Bmp->h; yi++) {

			i = yi*Bmp->w + xi;

			if ( *(*(Bmp->pixels + i) +3) != *(*(Bmp->pixels + i + 1) +3)
				|| *(*(Bmp->pixels + i) +3) != *(*(Bmp->pixels + i - 1) +3)){
				LCD_Pixel(Bmp->x + xi, Bmp->y + (Bmp->h -yi));
			}

		}

	}
*/
	DrawBmp(Bmp, x, y);
}

void ShiftBmp(bmp_t *Bmp, const uint16_t ix, const uint16_t iy){ MoveBmp(Bmp, Bmp->x + ix, Bmp->y + iy); }


//*********************************************************************
//*** BMP movement functions										***
//*********************************************************************
/*
//-- This function draws the Bitmap picture to the LCD
void 	DrawBmp(bmp_t *Bmp, const uint16_t x, const uint16_t y);

//Parameter 1:	adress to strukt of BMP
//Parameter 2: 	absolute X position
//Parameter 3:	absolute Y position

//-- This function moves an existing Bitmap picture on the LCD to a given position
void 	MoveBmp(bmp_t *Bmp, const uint16_t x, const uint16_t y);
//Parameter 1:	adress to strukt of BMP
//Parameter 2: 	absolute X position
//Parameter 3:	absolute Y position


//-- This function shifts an existing Bitmap for a given amount of pixels in a given direction
void 	ShiftBmp(bmp_t *Bmp, const uint16_t ix, const uint16_t iy)
{ MoveBmp(Bmp, Bmp->x + ix, Bmp->y + iy); }
//Parameter 1:	adress to strukt of BMP
//Parameter 2: 	shift in X direction
//Parameter 3:	shift in Y direction
*/


#endif /* FUNCTIONS_H_ */
