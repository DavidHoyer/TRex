//include general librarys
#include <leguan.h>
#include <cube.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

//include own librarys
#include "Object.h"


//*********************************************************************
//*** Defines    													***
//*********************************************************************
#define TRUE		1
#define FALSE		0

#define GROUND_HEIGHT	LCD_HEIGHT - 100

//*********************************************************************
//*** Enums      													***
//*********************************************************************
typedef enum {
	STATE_NONE	= -1,
	STATE_MENU	= 0,
	STATE_GAME,
	STATE_GAME_OVER,
}ENUM_GAME_STATE;

//*********************************************************************
//*** Structs 														***
//*********************************************************************
typedef struct {
	char 		eventFlag;
	uint16_t 	x;
	uint16_t 	y;
}event_T;

//*********************************************************************
//*** Functions 													***
//*********************************************************************
void 	GlobalInit(void);

void 	ClearLCD(void);
void 	DrawBmp(bmp_t *Bmp, const uint16_t x, const uint16_t y);
void 	MoveBmp(bmp_t *Bmp, const uint16_t x, const uint16_t y);
void 	ShiftBmp(bmp_t *Bmp, const uint16_t ix, const uint16_t iy) { MoveBmp(Bmp, Bmp->x + ix, Bmp->y + iy); }

void 	ShowMenu(ENUM_GAME_STATE *state);
void 	ShowGame(ENUM_GAME_STATE *state);
void 	ShowGameOver(ENUM_GAME_STATE *state);

event_T CheckEvent(void);
char 	CheckEventBmp(bmp_t bmp, event_T event);

void 	MoveTree(void);

//*********************************************************************
//*** Main 															***
//*********************************************************************
int main()
{
	ENUM_GAME_STATE gameState = STATE_NONE;		//Current game event
	event_T event;								//Struct contains current event

	GlobalInit();								//initialisation of hardware, leguan and LCD
	ShowMenu(&gameState);						//Show Menu at beginning

	//--- Main loop
	while(TRUE)
	{
		event = CheckEvent();					//Check if a new event happend

		switch(gameState)
		{
			case STATE_NONE:
				break;

			case STATE_MENU:
				//--- Check events
				if(event.eventFlag){
					//--- Menu Button
					if(CheckEventBmp(button_START_BMP, event))
						ShowGame(&gameState);
				}
				break;

			case STATE_GAME:
				//--- Check events
				if(event.eventFlag){
					//--- Event on T_Rex
					if(CheckEventBmp(tRexBmp, event))
						ShiftBmp(&tRexBmp, 20, 0);
					if(CheckEventBmp(obstacle_tree_BMP, event)){
						ShowMenu(&gameState);
						break;
					}
				}

				MoveTree();
				break;

			case STATE_GAME_OVER:
				break;

		}
	}

	return 0;
}

//*********************************************************************
//*** Global Init 													***
//*********************************************************************
void GlobalInit(void){
	CUBEMX_Init();							// Initialize Hardware
	LEGUAN_Init();							// Initialize Leguan board
	LCD_Init();								// Initialize LCD
	LOG_SetDestination(LCD_Stream);			// Set logging output destination to be the LCD
}

//*********************************************************************
//*** Clear LCD Display												***
//*********************************************************************
void ClearLCD(void){
	LCD_Clear();

	// chöimer de no mitere liste ersetze!!
	tRexBmp.visible = FALSE;
	button_START_BMP.visible = FALSE;
}

//*********************************************************************
//*** Print Bmp to LCD												***
//*********************************************************************
void DrawBmp(bmp_t *Bmp, const uint16_t x, const uint16_t y){
	Bmp->visible = 1;
	Bmp->x = x;
	Bmp->y = y;
	uint32_t i;

	for(uint16_t yi = 0; yi < Bmp->h; yi++){
		for(uint16_t xi = 0; xi < Bmp->w; xi++){
			i = yi*Bmp->w + xi;

			color_t pixelClr = BGRA565_COLOR(	*(*(Bmp->pixels + i) +0),
												*(*(Bmp->pixels + i) +1),
												*(*(Bmp->pixels + i) +2),
												*(*(Bmp->pixels + i) +3));

			if(pixelClr.a == 0)	//Skip transparent pixels
				continue;

			LCD_SetForegroundColor(pixelClr);
			LCD_Pixel(Bmp->x + xi, Bmp->y + (Bmp->h - yi));
		}
	}
}

//*********************************************************************
//*** Move Bmp to (x,y)												***
//*********************************************************************
void MoveBmp(bmp_t *Bmp, const uint16_t x, const uint16_t y){
	if(Bmp->visible == FALSE)
		return;

	if (x > LCD_WIDTH || y > LCD_HEIGHT)
		return;

	//--- Delete the old object.
	//uint32_t i;
	LCD_SetForegroundColor(ColorWhite);	//BG Color

	LCD_Rect((Bmp->x + Bmp->w) , Bmp->y, y, Bmp->h);

	/*
	for(uint16_t yi = 0; yi < Bmp->h; yi++){
		for(uint16_t xi = 0; xi < Bmp->w; xi++){
			i = yi*Bmp->w + xi;

			if(*(*(Bmp->pixels + i) +3) == 0)					//if it's transparent we can skipp it
				continue;

			LCD_Pixel(Bmp->x + xi, Bmp->y + (Bmp->h - yi));		//reset the pixel to bg
		}
	}
*/
	DrawBmp(Bmp, x, y);
}

//*********************************************************************
//*** Check Event 													***
//*********************************************************************
event_T CheckEvent(void)
{
	event_T event;
	LCD_TouchPosition_t touchPosition = {0, 0};
	static char flag = FALSE;

	if (R_SUCCESS(LCD_TouchGetPosition(&touchPosition)))
	{
		if(touchPosition.x == 0 && touchPosition.y == 480){
			event.eventFlag = FALSE;
			flag = FALSE;
			return event;
		}

		if(flag == TRUE){
			event.eventFlag = FALSE;
			return event;
		}

		event.eventFlag = TRUE;
		event.x = touchPosition.x;
		event.y = touchPosition.y;
		flag = TRUE;
	}

	return event;
}

//*********************************************************************
//*** Check if event happend to be on teh bmp						***
//*********************************************************************
char 	CheckEventBmp(bmp_t bmp, event_T event){
	if(bmp.visible == FALSE)
		return FALSE;

	//--- Check if the event was in the domain of the object
	if(	event.x >= bmp.x && event.x <= bmp.x + bmp.w &&
		event.y >= bmp.y && event.y <= bmp.y + bmp.h)
	{
		return TRUE;
	}

	return FALSE;
}

//*********************************************************************
//*** Show all Menu Objects											***
//*********************************************************************
void ShowMenu(ENUM_GAME_STATE *state){
	if(*state == STATE_MENU)					//Already handled
		return;

	*state = STATE_MENU;						//Set state to menu
	ClearLCD();									//Clear LCD Display

	DrawBmp(&button_START_BMP, 200, 150);		//Draw start button to display
}

//*********************************************************************
//*** Show all Game Objects											***
//*********************************************************************
void ShowGame(ENUM_GAME_STATE *state){
	if(*state == STATE_GAME)					//Already handled
		return;

	*state = STATE_GAME;						//Set state to game
	ClearLCD();									//Clear LCD Display

	DrawBmp(&tRexBmp, 			 50, GROUND_HEIGHT - tRexBmp.h);			//Draw T-Rex to Display
	DrawBmp(&obstacle_tree_BMP, 600, GROUND_HEIGHT - obstacle_tree_BMP.h);	//Draw Tree to Display
}

//*********************************************************************
//*** Show all Gameover Objects										***
//*********************************************************************
void ShowGameOver(ENUM_GAME_STATE *state){
	if(*state == STATE_GAME_OVER)				//Already handled
			return;

	*state = STATE_GAME_OVER;					//Set state to gameover
	ClearLCD();									//Clear LCD Display
}

void MoveTree(void){
	static uint32_t tickOld = 0;
	uint32_t tickNew = HAL_GetTick();

	if(tickNew < tickOld + 5)
		return;

	ShiftBmp(&obstacle_tree_BMP, -1, 0);
	tickOld = tickNew;
}
