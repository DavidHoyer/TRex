/**
  *  @file objects.c
  *  @ingroup source
  *  @date 30.12.2022
  *  @author Lukas Roth
  *  @brief Working with objects on the LCD
  */

//*********************************************************************
//*** Includes     													***
//*********************************************************************
#include <stdlib.h>
#include "include/objects.h"

//*********************************************************************
//*** Variables    													***
//*********************************************************************
const LCD_Color_t bgColor = {31, 63, 31};	//--- set background color



//*********************************************************************
//*** Draw funktion for BMP on LCD									***
//*** uses LCD_SetDrawArea and LCD_Set								***
//***	-so it can't draw transparent pixels						***
//*********************************************************************
void DrawBmp(bmp_t *bmp, const uint16_t x, const uint16_t y)
{
	//--- enable object
	bmp->visible = 1;		//--- set to visible
	bmp->x = x;				//--- set new x coordinate
	bmp->y = y;				//--- set new y coordinate

	//--- set Draw area for BMP on LCD
	LCD_SetDrawArea(bmp->x, bmp->y, bmp->x + bmp->w - 1, bmp->y + bmp->h - 1);
	//--- enable Draw mode on LCD
	LCD_EnableDrawMode();

	//--- define variables
	uint32_t i;
	const uint32_t size = bmp->w * bmp->h;
	LCD_Color_t lcdColor = {0,0,0};

	//--- loop trough every pixel
	for(uint16_t yi = 0; yi < bmp->h; yi++){
		for(uint16_t xi = 0; xi < bmp->w; xi++){

			//--- calculate pixel position in pixel array
			i = size - yi * bmp->w - bmp->w + xi;

			//--- set transparent pixel to background color
			if(*(*(bmp->pixels + i) +3) == 0){
				LCD_Set(&bgColor);
				continue;
			}

			//--- read color value of current pixel out of pixel array
			lcdColor.b = (uint8_t)(*(*(bmp->pixels + i) + 0));
			lcdColor.g = (uint8_t)(*(*(bmp->pixels + i) + 1));
			lcdColor.r = (uint8_t)(*(*(bmp->pixels + i) + 2));

			//--- set color for current pixel
			LCD_Set(&lcdColor);
		}
	}
}

//*********************************************************************
//*** Draw funktion for BMP on LCD									***
//*** uses LCD_SetDrawArea and LCD_Set								***
//***	-so it can't draw transparent pixels						***
//*********************************************************************

void DrawBmpWithout_A(bmp_t *bmp, const uint16_t x, const uint16_t y)
{
	bmp->visible = 1;		//--- set to visible
	bmp->x = x;				//--- set new x coordinate
	bmp->y = y;				//--- set new y coordinate

	//--- define variables
	uint32_t i;
	const uint32_t size = bmp->w * bmp->h;
	LCD_Color_t lcdColor = {0,0,0};

	//--- loop through every pixel
	for(uint16_t yi = 0; yi < bmp->h; yi++){
		for(uint16_t xi = bmp->w; xi >= 1; --xi){

			//--- calculate pixel position in pixel array
			i = size - yi * bmp->w - xi;

			//--- if pixel is transparen, don't draw this pixel
			if(*(*(bmp->pixels + i) +3) == 0){
				continue;
			}

			//--- if pixel is not transparent, read color values out of pixel array
			lcdColor.b = (uint8_t)(*(*(bmp->pixels + i) + 0));
			lcdColor.g = (uint8_t)(*(*(bmp->pixels + i) + 1));
			lcdColor.r = (uint8_t)(*(*(bmp->pixels + i) + 2));

			//--- draw the pixel
			LCD_SetDrawArea(x + bmp->w - xi, y + yi, x + bmp->w - xi, y + yi);
			LCD_EnableDrawMode();
			LCD_Set(&lcdColor);
		}
	}
}

//*********************************************************************
//*** Move funktion for BMP											***
//*** input parameters:												***
//*** 	-bmp struct													***
//*** 	-x and y position											***
//*********************************************************************
void MoveBmp(bmp_t *bmp, const uint16_t x, const uint16_t y){

	//--- if bmp is not visible, don't move
	if(bmp->visible == 0)
		return;

	//--- check if move is outside of the display
	if(x < 0 || x + bmp->w > LCD_WIDTH || y < 0 || y + bmp->h > LCD_HEIGHT)
		return;

	//--- check if move goes to same position
	if(x == bmp->x && y == bmp->y)
		return;

	//--- save old position of bmp
	const uint16_t xOld = bmp->x;
	const uint16_t yOld = bmp->y;

	//--- Draw bmp new
	DrawBmp(bmp, x, y);

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


//*********************************************************************
//*** Sifts bmp for given amount of pixel							***
//*** input parameters:												***
//*** 	-bmp struct													***
//*** 	-x and y distance											***
//*********************************************************************

void ShiftBmp(bmp_t *bmp, const uint16_t ix, const uint16_t iy){
	//--- move bmp to given position
	MoveBmp(bmp, bmp->x + ix, bmp->y + iy);
}


//*********************************************************************
//*** deletes a bmp													***
//*** input parameters:												***
//*** 	-bmp struct													***
//*********************************************************************

void DeleteBmp(bmp_t *bmp){
	//--- sets visibility to FALSE and clear draw area
	bmp->visible = FALSE;
	LcdClearArea(bmp->x, bmp->y, bmp->x + bmp->w, bmp->y + bmp->h);
}


//*********************************************************************
//*** Draws given area in background color							***
//*********************************************************************

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


//*********************************************************************
//*** creates a list of boarder pixels								***
//*** input parameters:												***
//*** 	-bmp struct													***
//*** Info:															***
//*** 	in order for this border function to work, it is important	***
//*** 	that the border are marks and they are all on the border of ***
//***	the object.													***
//*********************************************************************

pixel_t *GetBoarder (bmp_t bmp) {

	//--- create start of list and current pixel
	pixel_t *head = NULL;
	pixel_t *current = NULL;

	//--- create variables
	uint32_t i;
	const uint32_t size = bmp.w * bmp.h;

	//--- search for first not transparent pixel (left to right)
	for(uint16_t yi = 0; yi < bmp.h; yi++){
		for(uint16_t xi = 0; xi < bmp.w; xi++){

			//--- calculate position of pixel in pixel array
			i = size - yi * bmp.w - bmp.w + xi;

			//--- if pixel is not transparent, create current pixel
			if((*(bmp.pixels + i))[3] != 0)
			{
				pixel_t *pixel = malloc(sizeof(pixel_t));
				pixel->x = xi;
				pixel->y = yi;
				pixel->next = NULL;
				current = pixel;
				break;
			}
		}
		if(current != NULL)
			break;
	}

	//--- safe start position.
	uint16_t x_start = current->x;
	uint16_t y_start = current->y;

	//--- more neighbor algorithm
	uint8_t mooreLast = 8;		// 1 - 8 Moore neighborhood pixel (more info in report)
	uint8_t mooreNew  = mooreLast;

	int x_next;
	int y_next;

	while(current != NULL){

		for(int notUsed = 0; notUsed < 8; notUsed++){

			//---Get next moore neighbor pixel
			mooreNew++;
			if(mooreNew >= 9)
				mooreNew = 1;

			//--- Get next position based on moore pixel
			x_next = current->x;
			y_next = current->y;

			switch(mooreNew){
				case 1:	x_next --;	y_next --;	break;
				case 2:				y_next --;	break;
				case 3:	x_next ++;	y_next --;	break;
				case 4:	x_next ++;				break;
				case 5:	x_next ++;	y_next ++;	break;
				case 6:				y_next ++;	break;
				case 7:	x_next --;	y_next ++;	break;
				case 8:	x_next --;				break;
			}

			//--- check border
			if(x_next < 0 || y_next < 0 || x_next >= bmp.w || y_next >= bmp.h)
				continue;

			//--- check transparency
			i = size - y_next * bmp.w - bmp.w + x_next;			// Get array index
			if((*(bmp.pixels + i))[3] == 0)						// If its transparent conitnue
				continue;

			//--- Check if the pixel is marked as a border edge {252, 2, 1, 255}
			if( (*(bmp.pixels + i))[0] == 252 &&
				(*(bmp.pixels + i))[1] == 2	&&
				(*(bmp.pixels + i))[2] == 1	&&
				(*(bmp.pixels + i))[3] == 255 )
			{
				pixel_t *pixel = malloc(sizeof(pixel_t));
				pixel->x = x_next;
				pixel->y = y_next;
				pixel->next = head;
				head = pixel;
			}

			current->x = x_next;
			current->y = y_next;

			//--- get next moore neighborhood starting pixel
			if(mooreNew <= 4)
				mooreLast = mooreNew + 4;
			else
				mooreLast = mooreNew - 4;

			mooreNew = mooreLast;
			break;
		}

		//--- check if we are again at the beginning
		if( current->x == x_start && current->y == y_start)
			break;
	}
	return head;
}
//*********************************************************************************
//*** SortBoarder																***
//*** This function will sort the border pixels so that they are in a line and	***
//*** each pixel in the list is a neighbour of the next pixel in the list.		***
//*** input parameters:															***
//*** 	-head of pixel list														***
//*********************************************************************************
pixel_t *SortBoarder(pixel_t *head)
{
    if (head == NULL)
        return NULL;

    pixel_t *current = head;
    pixel_t *min = current;

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
        pixel_t *temp = current;
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

//*********************************************************************************
//*** only leaves the edges of the border										***
//*** input parameters:															***
//*** 	-head of pixel list														***
//*********************************************************************************

pixel_t *CreateEdges(pixel_t *head) {

	//--- creat previouse and current pixel
	pixel_t *prev = head;
	pixel_t *current = head->next;

	//--- while end of list is not reached
    while (current != NULL) {
    	//--- sort list
        if (current->x == prev->x || current->y == prev->y) {
            prev->next = current->next;
            free(current);
            current = prev->next;
        } else {
            prev = current;
            current = current->next;
        }
    }
    //--- return sorted list
    return head;
}


//*********************************************************************************
//*** convert BGRA_8888 to BGRA_5658 color code									***
//*** input parameters:															***
//*** 	-pointer to pixel array													***
//*** 	-with and hight of bmp													***
//*********************************************************************************

void ConvertArray(unsigned char pixelData[][4], const uint16_t w, const uint16_t h)
{
	for (uint16_t i = 0; i < h * w; i++) {
		//--- border edges
		if( pixelData[i][0] == 252 	&&
			pixelData[i][1] == 2	&&
			pixelData[i][2] == 1	&&
			pixelData[i][3] == 255 )
		{
			pixelData[i][3] = 0;
			continue;
		}

		//--- convert BGRA_8888 to BGRA_5658
		pixelData[i][0] = pixelData[i][0] >> 3;
		pixelData[i][1] = pixelData[i][1] >> 2;
		pixelData[i][2] = pixelData[i][2] >> 3;
	}
}


//*********************************************************************************
//*** creates gradient boarder color for T-Rex selection						***
//*********************************************************************************

void LCD_Rect_Gradient(uint16_t x, uint16_t y, uint16_t width, uint16_t height, LCD_Color_t clr1, LCD_Color_t clr2) {
    //--- Set working region
    LCD_SetDrawArea(x, y, x + width -1, y + height - 1);
    LCD_EnableDrawMode();

    //--- define variables
    LCD_Color_t clr = clr1;

    const double diff_r = abs(clr1.r - clr2.r);
    const double diff_g = abs(clr1.g - clr2.g);
    const double diff_b = abs(clr1.b - clr2.b);

    const double factor_r = diff_r != 0 ? diff_r / width : 0.00;
    const double factor_g = diff_g != 0 ? diff_g / width : 0.00;
    const double factor_b = diff_b != 0 ? diff_b / width : 0.00;

    //--- Fill working region with color
    for (uint32_t y = 0; y < height; y++){
    	for (uint32_t x = 0; x < width ; x++){

    		//--- Red
    		if(diff_r != 0){
				if(clr1.r < clr2.r)
					clr.r = clr1.r + (uint16_t)round(factor_r * x);
				else
					clr.r = clr1.r - (uint16_t)round(factor_r * x);
    		}

    		//--- Green
			if(diff_g != 0){
				if(clr1.g < clr2.g)
					clr.g = clr1.g + (uint16_t)round(factor_g * x);
				else
					clr.g = clr1.g - (uint16_t)round(factor_g * x);
			}

			//--- Blue
			if(diff_b != 0){
				if(clr1.b < clr2.b)
					clr.b = clr1.b + (uint16_t)round(factor_b * x);
				else
					clr.b = clr1.b - (uint16_t)round(factor_b * x);
			}

			//--- set color for the current pixel
    		LCD_Set(&clr);
    	}
    }
}




