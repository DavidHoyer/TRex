//include general librarys
#include <leguan.h>
#include <cube.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

//include own librarys
#include "Object.h"
#include "functions.h"

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

void 	ShowMenu(ENUM_GAME_STATE *state);
void 	ShowGame(ENUM_GAME_STATE *state);
void 	ShowGameOver(ENUM_GAME_STATE *state);

event_T CheckEvent(void);
char 	CheckEventBmp(bmp_t bmp, event_T event);

void MoveObject(bmp_t *Bmp, short int x, short int y);
void JumpTRex(void);

node_t *GetBoarder (bmp_t *Bmp);
void PrintBorder (bmp_t *Bmp);

//*********************************************************************
//*** Main 															***
//*********************************************************************
int main()
{
	ENUM_GAME_STATE gameState = STATE_NONE;		//Current game event
	event_T event;								//Struct contains current event

	GlobalInit();								//initialisation of hardware, leguan and LCD
	ShowMenu(&gameState);						//Show Menu at beginning
	//tRexBmp.head = GetBoarder(&tRexBmp);
	//PrintBorder(&tRexBmp);

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
					if(CheckEventBmp(tRexBmp, event)){
						//ShiftBmp(&tRexBmp, 20, 0);

					}
					if(CheckEventBmp(obstacle_tree_BMP, event)){
						ShowMenu(&gameState);
						break;
					}

				}
				//MoveObject(&obstacle_tree_BMP, -1, 0);
				JumpTRex();
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

	tRexBmp.head = GetBoarder(&tRexBmp);						//generating pixel list for picture border
	obstacle_tree_BMP.head = GetBoarder(&obstacle_tree_BMP);	//generating pixel list for picture border
}

//*********************************************************************
//*** Clear LCD Display												***
//*********************************************************************
void ClearLCD(void){
	LCD_Clear();

	// ch??imer de no mitere liste ersetze!!
	tRexBmp.visible = FALSE;
	button_START_BMP.visible = FALSE;
}

//*********************************************************************
//*** Get boarder of bmp											***
//*********************************************************************

node_t *GetBoarder (bmp_t *Bmp) {

	node_t *head = NULL;


	uint32_t i;


	for(uint16_t yi = 1; yi < Bmp->h-1; yi++){
		for(uint16_t xi = 1; xi < Bmp->w-1; xi++){


			i = yi*Bmp->w + xi;

			if ( *(*(Bmp->pixels + i) +3) != *(*(Bmp->pixels + i + 1) +3)
				|| *(*(Bmp->pixels + i) +3) != *(*(Bmp->pixels + i - 1) +3)){

				node_t *pixel = malloc(sizeof(node_t));
				pixel->x = xi;
				pixel->y = yi;
				pixel->next = head;
				head = pixel;

				continue;
			}


			if (*(*(Bmp->pixels + i) +3) != *(*(Bmp->pixels + i +Bmp->w) +3)
				|| *(*(Bmp->pixels + i) +3) != *(*(Bmp->pixels + i - Bmp->w) +3)) {

				node_t *pixel = malloc(sizeof(node_t));
				pixel->x = xi;
				pixel->y = yi;
				pixel->next = head;
				head = pixel;

				continue;
			}
		}
	}
	return head;
}

//*********************************************************************
//*** Print border to LCD												***
//*********************************************************************

void PrintBorder (bmp_t *Bmp) {

	node_t *ptr = Bmp->head;

	LCD_SetForegroundColor(ColorWhite);

	while(ptr != NULL){
		LCD_Pixel(Bmp->x + ptr->x, Bmp->y + (Bmp->h - ptr->y));
		ptr = ptr->next;
	}
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

//*********************************************************************
//*** Move gameobject										***
//*********************************************************************

void MoveObject(bmp_t *Bmp, short int x, short int y){

	static uint32_t tickOld = 0;
	const uint32_t tickNew = HAL_GetTick();

	if(tickNew < tickOld + 5)
		return;

	ShiftBmp(Bmp, x, y);
	tickOld = tickNew;
}

void JumpTRex(void) {

	static uint16_t direction = -1;
	static uint32_t tickOld_trex = 0;

	const uint32_t tickNew_trex = HAL_GetTick();

	if(tickNew_trex < tickOld_trex + 2)
		return;

	ShiftBmp(&tRexBmp, 0, direction);
	tickOld_trex = tickNew_trex;
	//tRexBmp.velocity -=1;

}

