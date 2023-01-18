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
			i = size - yi * bmp->w - xi;

			//--- skip transparent pixels
			if(*(*(bmp->pixels + i) +3) == 0){
				LCD_Set(&bgColor);
				continue;
			}

			//--- skip border points
			/*if( (*(bmp->pixels + i))[0] == 252 	&&
				(*(bmp->pixels + i))[1] == 2		&&
				(*(bmp->pixels + i))[2] == 1		&&
				(*(bmp->pixels + i))[3] == 255 )
			{
				LCD_Set(&bgColor);
				continue;
			}*/

			lcdColor.b = (uint8_t)(*(*(bmp->pixels + i) + 0));
			lcdColor.g = (uint8_t)(*(*(bmp->pixels + i) + 1));
			lcdColor.r = (uint8_t)(*(*(bmp->pixels + i) + 2));

			LCD_Set(&lcdColor);

		}
	}
}

void DrawBmpWithout_A(bmp_t *bmp, const uint16_t x, const uint16_t y)
{
	bmp->visible = 1;
	bmp->x = x;
	bmp->y = y;

	uint32_t i;
	const uint32_t size = bmp->w * bmp->h;

	LCD_Color_t lcdColor = {0,0,0};

	for(uint16_t yi = 0; yi < bmp->h; yi++){
		for(uint16_t xi = bmp->w; xi >= 1; --xi){
			i = size - yi * bmp->w - xi;

			if(*(*(bmp->pixels + i) +3) == 0){
				continue;
			}

			lcdColor.b = (uint8_t)(*(*(bmp->pixels + i) + 0));
			lcdColor.g = (uint8_t)(*(*(bmp->pixels + i) + 1));
			lcdColor.r = (uint8_t)(*(*(bmp->pixels + i) + 2));

			LCD_SetDrawArea(x + bmp->w - xi, y + yi, x + bmp->w - xi, y + yi);
			LCD_EnableDrawMode();
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
	node_t *start = NULL;
	node_t *current = NULL;
	node_t *last = NULL;

	uint16_t x_first =0;
	uint16_t y_first =0;
	unsigned char DONE =0;
	unsigned char found =0;

	//find first boarder pixel from top to bottom and left to right
	for (uint16_t y = bmp.h; y > 0; y--) {
		for (uint16_t x = 0; x < bmp.w; x++) {

			if ( (*(bmp.pixels + y*bmp.w + x))[3] != 0 )
			{
				node_t *pixel = malloc(sizeof(node_t));
				pixel->x = x;
				pixel->y = y + 1;
				current = pixel;
				current->next = NULL;
				last = current;
				last->x -= 1;
				last->next = NULL;
				y = 0;
				x = bmp.w;
			}
		}
	}

	do
	{
		//calculate direction from which pixel was enterd
		int dirx = current->x - last->x;
		int diry = current->y - last->y;

		//enterd from left side
		if (dirx = 1 && diry == 0) {

			//top left pixel transparent?
			if ( (*(bmp.pixels + (last->y+1)*bmp.w + last->x))[3] == 0 ){
				last->x +=1;
				last->y +=1;
			}
			else {
				//save new boarder pixel
				current->x = last->x -1;
				current->y = last->y +1;

				//ask if its edge pixel
				if ( 	(*(bmp.pixels + (last->y+1)*bmp.w + last->x -1))[0] == 252 &&
						(*(bmp.pixels + (last->y+1)*bmp.w + last->x -1))[1] == 2 &&
						(*(bmp.pixels + (last->y+1)*bmp.w + last->x -1))[2] == 1)
				{
					//create new node for edges
					node_t *pixel = malloc(sizeof(node_t));
					pixel->x = x;
					pixel->y = y + 1;
					pixel->next = head;
					head = pixel;
				}
				continue;
			}

			//top middle pixel transparent?
			if ( (*(bmp.pixels + last->y*bmp.w + last->x))[3] == 0 ) {
				last->x += 1;
			}
			else {
				current->x = last->x +1;
				current->y = last->y;
			}

			//top right pixel transparent
			if ( (*(bmp.pixels + last->y*bmp.w + last->x))[3] == 0 ) {
				last->y -= 1;
			}
			else {
				current->x = last->x +1;
				current->y = last->y;
			}

			if ( (*(bmp.pixels + last->y*bmp.w + last->x))[3] == 0 ) {
				last->y -= 1;
			}
			else {
				current->x = last->x +1;
				current->y = last->y;
			}

			if ( (*(bmp.pixels + last->y*bmp.w + last->x))[3] == 0 ) {
				last->x -= 1;
			}
			else {
				current->x = last->x +1;
				current->y = last->y;
			}

			if ( (*(bmp.pixels + last->y*bmp.w + last->x))[3] == 0 ) {
				last->x -= 1;
			}
			else {
				current->x = last->x +1;
				current->y = last->y;
			}
			if ( (*(bmp.pixels + last->y*bmp.w + last->x))[3] == 0 ) {
				return NULL;
			}
			else {
				current->x = last->x +1;
				current->y = last->y;
			}
		}



		if ( (*(bmp.pixels + last->y*bmp.w + last->x))[3] != 0 ){

		}
		else {

		}


	}while (current->x != start->x && current->y != start->y);









	// Iterate through the entire image
	for (uint16_t y = 0; y < bmp.h; y++) {
		for (uint16_t x = 0; x < bmp.w; x++) {
			// Check if the pixel is marked as a border edge {252, 2, 1, 255}
			if( (*(bmp.pixels + y*bmp.w + x))[0] == 252 &&
				(*(bmp.pixels + y*bmp.w + x))[1] == 2	&&
				(*(bmp.pixels + y*bmp.w + x))[2] == 1	&&
				(*(bmp.pixels + y*bmp.w + x))[3] == 255 )
			{
				node_t *pixel = malloc(sizeof(node_t));
				pixel->x = x;
				pixel->y = y + 1;
				pixel->next = head;
				head = pixel;
			}
		}
	}

	return head;
}


//*********************************************************************
//*** SortBoarder
//*** This function will sort the border pixels so that they are in a line and
//*** each pixel in the list is a neighbour of the next pixel in the list.
//*********************************************************************
node_t *SortBoarder(node_t *head)
{
    if (head == NULL)
        return NULL;

    node_t *current = head;
    node_t *min = current;

    // find the first pixel in the list that has the smallest distance to the next pixel
    while (current->next != NULL) {
        if (abs(current->x - current->next->x) + abs(current->y - current->next->y) < abs(min->x - min->next->x) + abs(min->y - min->next->y)) {
            min = current;
        }
        current = current->next;
    }

    // change the head of the list to the pixel with the smallest distance
    if (min != head) {
        current = head;
        while (current->next != min) {
            current = current->next;
        }
        current->next = min->next;
        min->next = head;
        head = min;
    }

    // sort the rest of the list
    current = head;
    while (current->next != NULL) {
        min = current->next;
        node_t *temp = current;
        while (temp->next != NULL) {
            if (abs(temp->next->x - current->x) + abs(temp->next->y - current->y) == 1) {
                if (min == current->next || abs(temp->next->x - current->x) + abs(temp->next->y - current->y) < abs(min->x - current->x) + abs(min->y - current->y)) {
                    min = temp->next;
                }
            }
            temp = temp->next;
        }
        if (min != current->next) {
            temp = current;
            while (temp->next != min) {
                temp = temp->next;
            }
            temp->next = min->next;
            min->next = current->next;
            current->next = min;
        }
        current = current->next;
    }

    return head;
}

// only leaves the edges of the border
node_t *CreateEdges(node_t *head) {
    node_t *prev = head, *current = head->next;
    while (current != NULL) {
        if (current->x == prev->x || current->y == prev->y) {
            prev->next = current->next;
            free(current);
            current = prev->next;
        } else {
            prev = current;
            current = current->next;
        }
    }
    return head;
}

void PrintBorder (bmp_t bmp, color_t clr) {

	node_t *ptr = bmp.head;

	LCD_SetForegroundColor(clr);

	while(ptr != NULL){
		LCD_Pixel(bmp.x + ptr->x, bmp.y + (bmp.h - ptr->y));
		ptr = ptr->next;
	}
}


void ConvertArray(unsigned char pixelData[][4], const uint16_t w, const uint16_t h){
	for (uint16_t i = 0; i < h * w; i++) {

		//--- border edges
		if( pixelData[i][0] == 252 	&&
			pixelData[i][1] == 2	&&
			pixelData[i][2] == 1	&&
			pixelData[i][3] == 255 )
		{
			//pixelData[i][0] = 0;
			//pixelData[i][1] = 0;
			//pixelData[i][2] = 0;
			pixelData[i][3] = 0;
			continue;
		}

		//--- convert BGRA_8888 to BGRA_5658
		pixelData[i][0] = pixelData[i][0] >> 3;
		pixelData[i][1] = pixelData[i][1] >> 2;
		pixelData[i][2] = pixelData[i][2] >> 3;
	}
}




