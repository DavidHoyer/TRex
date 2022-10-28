//include general librarys
#include <leguan.h>
#include <cube.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

//include own librarys
#include "Object.h"

void LukiInit(void);
void LukisFunction(void);

void printBMP(bmp_t);

void globalInit(void);

int main()
{
	//initialisation of hardware, leguan and LCD
	globalInit();

	//--- Declarations
	uint32_t tick = 0;

	// Set logging output destination to be the LCD
	LOG_SetDestination(LCD_Stream);


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




		LukisFunction();
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
	printBMP(tRexBmp);
	printBMP(JumpButtonBMP);
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
			color_t pixelClr = BGRA565_COLOR(	*(*(Bmp.pixels + i) +2),
										*(*(Bmp.pixels + i) +1),
										*(*(Bmp.pixels + i) +0),
										*(*(Bmp.pixels + i) +3));

			LCD_SetForegroundColor(pixelClr);
			LCD_Pixel(Bmp.x + x, Bmp.y + (Bmp.h - y));
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
