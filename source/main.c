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
void 	globalInit(void);

void 	ClearLCD(void);
void 	printBMP(bmp_t *Bmp);

void 	ShowMenu(ENUM_GAME_STATE *state);
void 	ShowGame(ENUM_GAME_STATE *state);
void 	ShowGameOver(ENUM_GAME_STATE *state);

event_T CheckEvent(void);
char 	CheckEventBmp(bmp_t bmp, event_T event);

//*********************************************************************
//*** Main 															***
//*********************************************************************
int main()
{
	ENUM_GAME_STATE gameState = STATE_NONE;		//Current game event
	event_T event;								//Struct contains current event

	globalInit();								//initialisation of hardware, leguan and LCD
	ShowMenu(&gameState);						//Show Menu at beginning

	//--- Main loop
	while(TRUE)
	{
		event = CheckEvent();					//Check if a new event happend

		//switch(state)
		//case game:
		//GameOnTick();
		//GameEventHAndle(event)

		if(event.eventFlag){
			//--- Menu Button
			if(CheckEventBmp(button_START_BMP, event)){
				ShowGame(&gameState);
			}
			//--- Event on T_Rex
			else if(CheckEventBmp(tRexBmp, event)){
				ShowMenu(&gameState);
			}
		}
	}
}

//*********************************************************************
//*** Global Init 													***
//*********************************************************************
void globalInit(void){
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
//*** Print Bmp to LCD												***
//*********************************************************************
void printBMP(bmp_t *Bmp)
{
	Bmp->visible = TRUE;
	unsigned int i;

	for(unsigned int y = 0; y < Bmp->h; y++){
		for(unsigned int x = 0; x < Bmp->w; x++){
			i = y*Bmp->w + x;

			color_t pixelClr = BGRA565_COLOR(	*(*(Bmp->pixels + i) +0),
												*(*(Bmp->pixels + i) +1),
												*(*(Bmp->pixels + i) +2),
												*(*(Bmp->pixels + i) +3));
			LCD_SetForegroundColor(pixelClr);
			LCD_Pixel(Bmp->x + x, Bmp->y + (Bmp->h - y));
		}
	}
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
		return FALSE;
	}

	return TRUE;
}

//*********************************************************************
//*** Show all Menu Objects											***
//*********************************************************************
void ShowMenu(ENUM_GAME_STATE *state){
	if(*state == STATE_MENU)					//Already handled
		return;

	*state = STATE_MENU;						//Set state to menu
	ClearLCD();									//Clear LCD Display

	printBMP(&button_START_BMP);				//Draw start button to display
}

//*********************************************************************
//*** Show all Game Objects											***
//*********************************************************************
void ShowGame(ENUM_GAME_STATE *state){
	if(*state == STATE_GAME)					//Already handled
		return;

	*state = STATE_GAME;						//Set state to game
	ClearLCD();									//Clear LCD Display

	printBMP(&tRexBmp);							//Draw T-Rex to Display
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
