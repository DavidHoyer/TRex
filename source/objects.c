/*
 * Objects.c
 *
 *  Created on: 30.12.2022
 *      Author: lukir
 */

#include <stdlib.h>
#include "include/objects.h"

const LCD_Color_t bgColor = {31, 63, 31};				//Sötme de no anders mache

void DrawBmp(bmp_t *bmp, const uint16_t x, const uint16_t y)
{
	bmp->visible = 1;
	bmp->x = x;
	bmp->y = y;

	LCD_SetDrawArea(bmp->x, bmp->y, bmp->x + bmp->w - 1, bmp->y + bmp->h - 1);
	LCD_EnableDrawMode();														// söt so ga

	uint32_t i;
	const uint32_t size = bmp->w * bmp->h;

	LCD_Color_t lcdColor = {0,0,0};

	for(uint16_t yi = 0; yi < bmp->h; yi++){
		for(uint16_t xi = bmp->w; xi >= 1; --xi){
			i = size - yi*bmp->w - xi;

			if(*(*(bmp->pixels + i) +3) == 0){
				LCD_Set(&bgColor);
				continue;
			}

			//lcdColor.b = ((uint8_t)(*(*(bmp->pixels + i) + 0))) >> 3;
			//lcdColor.g = ((uint8_t)(*(*(bmp->pixels + i) + 1))) >> 2;
			//lcdColor.r = ((uint8_t)(*(*(bmp->pixels + i) + 2))) >> 3;

			lcdColor.b = (uint8_t)(*(*(bmp->pixels + i) + 0));
			lcdColor.g = (uint8_t)(*(*(bmp->pixels + i) + 1));
			lcdColor.r = (uint8_t)(*(*(bmp->pixels + i) + 2));

			LCD_Set(&lcdColor);
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

node_t *GetBoarder (bmp_t bmp) {
	node_t *head = NULL;	// Head of the border pixel linked list
	int isBorder = 0;		// Indicates if the pixel is a border

	// Iterate through the entire image
	for (uint16_t y = 0; y < bmp.h; y++) {
		for (uint16_t x = 0; x < bmp.w; x++) {
			// Check if the pixel is on the border of the image
			if (x == 0 || x == bmp.w-1 || y == 0 || y == bmp.h-1) {
				if ((*(bmp.pixels + y*bmp.w + x))[3] != 0 )
				{
					isBorder = 1;
				}
			}
			// Check if the pixel is non-transparent and its neighbours are transparent
			else {
				if ((*(bmp.pixels + y*bmp.w + x))[3] != 0 &&
					((x > 0 && (*(bmp.pixels + y*bmp.w + x - 1))[3] == 0) ||
					(x < bmp.w - 1 && (*(bmp.pixels + y*bmp.w + x + 1))[3] == 0) ||
					(y > 0 && (*(bmp.pixels + (y-1)*bmp.w + x))[3] == 0) ||
					(y < bmp.h - 1 && (*(bmp.pixels + (y+1)*bmp.w + x))[3] == 0)))
				{
					isBorder = 1;
				}
			}

			if(isBorder == 1){
				isBorder = 0;
				node_t *pixel = malloc(sizeof(node_t));
				pixel->x = x;
				pixel->y = y;
				pixel->next = head;
				head = pixel;
			}
		}
	}
	return head;
}

void PrintBorder (bmp_t bmp) {

	node_t *ptr = bmp.head;

	LCD_SetForegroundColor(ColorBlue);

	while(ptr != NULL){
		LCD_Pixel(bmp.x + ptr->x, bmp.y + (bmp.h - ptr->y));
		ptr = ptr->next;
	}
}

void ConvertArray(unsigned char pixelData[][4], const uint16_t w, const uint16_t h){
	for (uint16_t i = 0; i < h * w; i++) {
		pixelData[i][0] = pixelData[i][0] >> 3;
		pixelData[i][1] = pixelData[i][1] >> 2;
		pixelData[i][2] = pixelData[i][2] >> 3;
	}
}
