/*
 * Objects.c
 *
 *  Created on: 30.12.2022
 *      Author: lukir
 */

#include "include/objects.h"
const LCD_Color_t bgColor = {31, 63, 31};				//Sötme de no anders mache

void DrawBmp(bmp_t *bmp, const uint16_t x, const uint16_t y)
{
	bmp->visible = 1;
	bmp->x = x;
	bmp->y = y;

	//Set working region
	//R_TRY(LCD_SetDrawArea(bmp->x, bmp->y, bmp->x + bmp->w, bmp->y + bmp->h));	// söt so ga
	LCD_SetDrawArea(bmp->x, bmp->y, bmp->x + bmp->w - 1, bmp->y + bmp->h - 1);
	LCD_EnableDrawMode();														// söt so ga

	uint32_t i;
	const uint32_t size = bmp->w * bmp->h;

	for(uint16_t yi = 0; yi < bmp->h; yi++){
		for(uint16_t xi = bmp->w; xi >= 1; --xi){
			i = size - yi*bmp->w - xi; //FUCK HET MI DE SCHEISS VERARSCHT!!!!!

			color_t pixelClr = BGRA565_COLOR(	*(*(bmp->pixels + i) +0),
												*(*(bmp->pixels + i) +1),
												*(*(bmp->pixels + i) +2),
												*(*(bmp->pixels + i) +3));

			if(pixelClr.a == 0){
				LCD_Set(&bgColor);
			}
			else{
				const LCD_Color_t lcdColor = {pixelClr.b, pixelClr.g, pixelClr.r};
				LCD_Set(&lcdColor);
			}
		}
	}
}

void MoveBmp(bmp_t *bmp, const uint16_t x, const uint16_t y){
	if(bmp->visible == 0)
		return;

	if(x < 0 || x + bmp->w > LCD_WIDTH || y < 0 || y + bmp->h > LCD_HEIGHT)
		return;

	if(x == bmp->x && y == bmp->y)
		return;

	const uint16_t xOld = bmp->x;
	const uint16_t yOld = bmp->y;

	DrawBmp(bmp, x, y);	//--- Draw new

	//--- Clear shifted rest
	if(xOld < x && xOld + bmp->w > x){
		LcdClearArea(xOld, yOld, x, yOld + bmp->h);						//-- Clear x left side
	}
	else if(xOld > x && xOld < x + bmp->w){
		LcdClearArea(x + bmp->w, yOld, xOld + bmp->w, yOld + bmp->h);	//-- Clear x right side
	}
	if(yOld > y && yOld < y + bmp->h){
		LcdClearArea(xOld, y + bmp->h, xOld + bmp->w, yOld + bmp->h);	//-- Clear y bottom side
	}
	else if(yOld < y && y < yOld + bmp->h){
		LcdClearArea(xOld, yOld, xOld + bmp->w, y);						//-- Clear y top side
	}

	/*
	LCD_SetDrawArea(bmp->x, bmp->y, bmp->x + bmp->w - 1, bmp->y + bmp->h - 1);
	LCD_EnableDrawMode();

	for (uint32_t pixel = 0; pixel < LCD_WIDTH * LCD_HEIGHT; pixel++) {
		LCD_Set(&bgColor);
	}*/
}

void ShiftBmp(bmp_t *bmp, const uint16_t ix, const uint16_t iy){
	MoveBmp(bmp, bmp->x + ix, bmp->y + iy);
}

void DeleteBmp(bmp_t *bmp){
	bmp->visible = FALSE;
	LcdClearArea(bmp->x, bmp->y, bmp->x + bmp->w, bmp->y + bmp->h);
}

void LcdClearArea(const uint16_t x1, const uint16_t y1, const uint16_t x2, const uint16_t y2)
{
	if(x1 < 0 || x2 > LCD_WIDTH || y1 < 0 || y2 > LCD_HEIGHT)
		return;

	LCD_SetDrawArea(x1, y1, x2 - 1, y2 - 1);
	LCD_EnableDrawMode();

	for (uint32_t pixel = 0; pixel < (x2 - x1) * (y2 - y1); pixel++) {
		LCD_Set(&bgColor);
	}
}

