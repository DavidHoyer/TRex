/*
 * functions.c
 *
 *  Created on: 13.12.2022
 *      Author: david
 */

//include general librarys
/*
#include <leguan.h>
#include <cube.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

//include own librarys
//#include "functions.h"
#include "Object.h"
*/
//*********************************************************************
//*** Print Bmp to LCD												***
//*********************************************************************
/*
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
*/
//*********************************************************************
//*** Move Bmp to (x,y)												***
//*********************************************************************
/*
void MoveBmp(bmp_t *Bmp, const uint16_t x, const uint16_t y){

	//uint8_t boarder[Bmp->w][Bmp->h];

	//uint32_t i;

	if(Bmp->visible == FALSE)
		return;

	if (x > LCD_WIDTH || y > LCD_HEIGHT)
		return;

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

	DrawBmp(Bmp, x, y);

*/
/*
	//--- Delete the old object.
	uint32_t u;

	LCD_SetForegroundColor(ColorWhite);	//BG Color

	//LCD_Rect((Bmp->x + Bmp->w) , Bmp->y, x, Bmp->h);


	for(uint16_t yi = 0; yi < Bmp->h; yi++){
		for(uint16_t xi = 0; xi < Bmp->w; xi++){

			u = yi*Bmp->w + xi;

			if(*(*(Bmp->pixels + u) +3) == 0)					//if it's transparent we can skipp it
				continue;

			LCD_Pixel(Bmp->x + xi, Bmp->y + (Bmp->h - yi));		//reset the pixel to bg
		}
	}

}
*/
