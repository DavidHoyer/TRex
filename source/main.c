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
	char 		EVENT_FLAG;
	uint16_t 	posX;
	uint16_t 	posY;
}event_T;

void ClearLCD(void){
	LCD_Clear();

	// chöimer de no mitere liste ersetze!!
	tRexBmp.visible = 0;
	button_START_BMP.visible = 0;
}

void LukiInit(void);
void LukisFunction(void);

void printBMP(bmp_t *Bmp);

void globalInit(void);

void ShowMenu(int *state){
	if(*state == MENU)	//Already handled
		return;

	*state = MENU;
	ClearLCD();

	printBMP(&button_START_BMP);
}

void ShowGame(int *state){
	if(*state == GAME)	//Already handled
		return;

	*state = GAME;
	ClearLCD();

	printBMP(&tRexBmp);
}

void ShowGameOver(int *state){
	if(*state == GAME_OVER)	//Already handled
			return;

	*state = GAME_OVER;
	ClearLCD();
}

event_T CheckEvent(void);
char 	CheckEventBmp(bmp_t bmp, event_T event);

int main()
{
	int gameState = -1;
	event_T event;

	//initialisation of hardware, leguan and LCD
	globalInit();

	// Set logging output destination to be the LCD
	LOG_SetDestination(LCD_Stream);

	ShowMenu(&gameState);

	//--- Main loop
	while (true)
	{
		event = CheckEvent();

		//switch(state)
		//case game:
		//GameOnTick();
		//GAmeEventHAndle(event)

		if(event.EVENT_FLAG){
			//--- Menu Button
			if(CheckEventBmp(button_START_BMP, event)){
				ShowGame(&gameState);
			}
			else if(CheckEventBmp(tRexBmp, event)){
				ShowMenu(&gameState);
			}
		}
	}
}

//*********************************************************************
//*** Global Init 													***
//*********************************************************************
void globalInit(void)
{
	// Initialize Hardware
	CUBEMX_Init();
	// Initialize Leguan board
	LEGUAN_Init();
	// Initialize LCD
	LCD_Init();
}

//*********************************************************************
//*** Check Event 													***
//*********************************************************************
event_T CheckEvent(void)
{
	event_T event;
	LCD_TouchPosition_t touchPosition = {0, 0};
	static char flag = 0;

	if (R_SUCCESS(LCD_TouchGetPosition(&touchPosition)))
	{
		if(touchPosition.x == 0 && touchPosition.y == 480){
			event.EVENT_FLAG = 0;
			flag = 0;
			return event;
		}

		if(flag == 1){
			event.EVENT_FLAG = 0;
			return event;
		}

		event.EVENT_FLAG = 1;
		event.posX = touchPosition.x;
		event.posY = touchPosition.y;
		flag = 1;
	}

	return event;
}

//*********************************************************************
//*** Print Bmp to LCD												***
//*********************************************************************
void printBMP(bmp_t *Bmp)
{
	Bmp->visible = 1;
	unsigned int i;

	//{B, G, R, T}
	//RGBA16_COLOR(r, g, b, a)

	for(unsigned int y = 0; y < Bmp->h; y++)
	{
		for(unsigned int x = 0; x < Bmp->w; x++)
		{
			i = y*Bmp->w + x;

			color_t pixelClr = BGRA565_COLOR(	*(*(Bmp->pixels + i) +0),
												*(*(Bmp->pixels + i) +1),
												*(*(Bmp->pixels + i) +2),
												*(*(Bmp->pixels + i) +3));

			LCD_SetForegroundColor(pixelClr);
			LCD_Pixel(Bmp->PosX + x, Bmp->PosY + (Bmp->h - y));
		}
	}
}

//*********************************************************************
//*** Check if event happend to be on teh bmp						***
//*********************************************************************
char 	CheckEventBmp(bmp_t bmp, event_T event){
	if(bmp.visible == 0)
		return 0;

	if(	event.posX >= bmp.PosX && event.posX <= bmp.PosX + bmp.w &&
		event.posY >= bmp.PosY && event.posY <= bmp.PosY + bmp.h)
	{
		return 1;
	}

	return 0;
}
