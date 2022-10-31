//include general librarys
#include <leguan.h>
#include <cube.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

//include own librarys
#include "Object.h"

#define MENU		0
#define GAME		1
#define GAME_OVER	2

#define TRUE		1
#define FALSE		0


typedef struct {
	unsigned char EVENT_FLAG;
	unsigned char posX;
	unsigned char posY;
} EVENT_t;


void LukiInit(void);
void LukisFunction(void);

void printBMP(bmp_t);

void globalInit(void);

EVENT_t EVENT_CATCH(EVENT_t);

int main()
{
	unsigned char STATE = 0;
	EVENT_t input;
	input.EVENT_FLAG = 0;


	//initialisation of hardware, leguan and LCD
	globalInit();

	//--- Declarations
	uint32_t tick = 0;

	// Set logging output destination to be the LCD
	LOG_SetDestination(LCD_Stream);

	//printBMP(obstacle_tree_BMP);

	//--- Main loop
	while (true)
	{
		if(HAL_GetTick() >= tick + 15)
		{
			//--- roundabout 60 fps
			tick = HAL_GetTick();

			//--- FPS anzeige
			//char *spast;
			//itoa(100, spast);
		}

		input = EVENT_CATCH(input);

		switch(STATE) {
			case MENU:
				if (input.EVENT_FLAG == 1)
				{
					input.EVENT_FLAG = 0;
					input.posX =0;
					input.posY =0;

					STATE = GAME;
					LCD_Clear();
					printBMP(tRexBmp);
				}

				break;

			case GAME:

					if (input.EVENT_FLAG == 1)
					{
						input.EVENT_FLAG = 0;
						input.posX =0;
						input.posY =0;

						STATE = MENU;
						LCD_Clear();
						printBMP(button_START_BMP);
					}
				break;

			case GAME_OVER:

				break;
		}


		//LukisFunction();
	}
}

void globalInit(void)
{
	// Initialize Hardware
	CUBEMX_Init();
	// Initialize Leguan board
	LEGUAN_Init();
	// Initialize LCD
	LCD_Init();

	//--- Print Bmps on the LCD screen

	printBMP(button_START_BMP);
}

EVENT_t EVENT_CATCH(EVENT_t input)
{

	static char touch_enable = 1;

	LCD_TouchPosition_t touchPosition = {0, 0};

	if (R_SUCCESS(LCD_TouchGetPosition(&touchPosition)))
	{

		if ((touchPosition.x != 0 && touchPosition.y != 0) && touch_enable == 1)
		{
			touch_enable =0;
			input.EVENT_FLAG = 1;
			input.posX = touchPosition.x;
			input.posY = touchPosition.y;

		}
		else if(touchPosition.x == 0){
			input.EVENT_FLAG = 0;
			input.posX = touchPosition.x;
			input.posY = touchPosition.y;
		}
	}

	return input;
}


void printBMP (bmp_t Bmp)
{
	unsigned int i;

	//{B, G, R, T}
	//RGBA16_COLOR(r, g, b, a)

	for(unsigned int y = 0; y < Bmp.h; y++)
	{
		for(unsigned int x = 0; x < Bmp.w; x++)
		{
			//if(tRex_pixelData[i][3] == 0)
				//continue;
			//if(pixelClr.a == 0)
				//continue;

			i = y*Bmp.w + x;

			/*color_t pixelClr = COLOR(	*(*(Bmp.pixels + i) +2),
										*(*(Bmp.pixels + i) +1),
										*(*(Bmp.pixels + i) +0),
										*(*(Bmp.pixels + i) +3));*/
			color_t pixelClr = BGRA565_COLOR(	*(*(Bmp.pixels + i) +0),
												*(*(Bmp.pixels + i) +1),
												*(*(Bmp.pixels + i) +2),
												*(*(Bmp.pixels + i) +3));

			LCD_SetForegroundColor(pixelClr);
			LCD_Pixel(Bmp.PosX + x, Bmp.PosY + (Bmp.h - y));
		}
	}
}


void LukisFunction(void)
{
	static int x_old = 0;
	static int y_old = 0;

	LCD_TouchPosition_t touchPosition = {0, 0};
	if (R_SUCCESS(LCD_TouchGetPosition(&touchPosition)))
	{
		int x_new = touchPosition.x;
		int y_new = touchPosition.y;

		if (x_new != 0 || y_new != 480)
		{
			LCD_SetForegroundColor(ColorWhite);
			LCD_Rect(x_old - 5, y_old - 5, 30, 30);

			LCD_SetForegroundColor(ColorRed);
			LCD_Rect (x_new, y_new, 20, 20);

			x_old = x_new;
			y_old = y_new;
		}
	}
}
