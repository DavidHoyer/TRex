/*
 * Game.c
 *
 *  Created on: 30.12.2022
 *      Author: lukir
 */
#include <stdio.h>
#include <stdlib.h>
#include "include/game.h"
#include "include/resource.h"		//Only include ones! contains all pixel information of BMPs
#include "include/objects.h"

//--- they can defined in .c as they are only needed here
#define GROUND_HEIGHT 		50
#define GROUND_LINE_WIDTH	2
#define TREX_X_POS			50
#define OBSTACLES_NUMBER	3

bmp_t tRexBmp 			= {10, 10, 77, 90, tRex_pixelDataGoogle, 0, 0, 0};
bmp_t tRexBmpGoogle 	= {0, 0, 77, 90, tRex_pixelDataGoogle, 0, 1, 0};
bmp_t tRexBmpGreen		= {0, 0, 135, 100, tRex_pixelDataGreen, 0, 0, 0};


bmp_t startButtonBmp 	= {0, 0, 248, 101, startButton_pixelData, 0,0};
bmp_t jumpButtonBmp 	= {0, 0, 80, 80, jumpButton_pixelData, 0,0};
bmp_t pauseButtonBmp	= {0, 0, 75, 75, pauseButton_pixelData, 0, 0};
bmp_t obstacleBmp[3]	= { {200, 10, 59, 80, cactus_pixelData, 0,0},
							{0, 0, 59, 80, cactus_pixelData, 0,0},
							{0, 0, 59, 80, cactus_pixelData, 0,0} };

GameState_t gameState = STATE_NONE;

//*********************************************************************
//*** Enums      													***
//*********************************************************************
typedef enum {
	MOVE_NONE	= 0,
	MOVE_UP		= 1,
	MOVE_DOWN 	= -1,
}Move_t;

Move_t tRexDirection = MOVE_NONE;				//Müessemer no luege wie ds nid global mache

void GameInit(void){
	ConvertArray(tRex_pixelDataGoogle, tRexBmpGoogle.w, tRexBmpGoogle.h);
	int result = CheckLineCollision(0,480,126+2,350+27,50+76,300+88,50+75,300+65);
	result = result + 1;

	//--- Convert Array to BGRA_5658
	ConvertArray(tRex_pixelDataGreen, tRexBmpGreen.w, tRexBmpGreen.h);

	ConvertArray(startButton_pixelData, startButtonBmp.w, startButtonBmp.h);
	ConvertArray(jumpButton_pixelData, jumpButtonBmp.w, jumpButtonBmp.h);
	ConvertArray(pauseButton_pixelData, 75, 75);
	ConvertArray(cactus_pixelData, obstacleBmp[0].w, obstacleBmp[0].h);

	LCD_SetBackgroundColor(ColorWhite);
	ShowStartMenu();

	//--- Detect Border of BMP files
	tRexBmpGoogle.head = GetBoarder(tRexBmpGoogle);
	tRexBmpGreen.head = GetBoarder(tRexBmpGreen);

	obstacleBmp[0].head = GetBoarder(obstacleBmp[0]);

	PrintBorder(tRexBmp, ColorRed);
	tRexBmp.head = SortBoarder(tRexBmp.head);
	PrintBorder(tRexBmp, ColorGreen);
	tRexBmp.head = CreateEdges(tRexBmp.head);
	PrintBorder(tRexBmp, ColorBlue);

	PrintBorder(obstacleBmp[0], ColorRed);
	obstacleBmp[0].head = SortBoarder(obstacleBmp[0].head);
	PrintBorder(obstacleBmp[0], ColorGreen);
	obstacleBmp[0].head = CreateEdges(obstacleBmp[0].head);
	PrintBorder(obstacleBmp[0], ColorBlue);

	for(int i = 1; i < OBSTACLES_NUMBER; i++){
		obstacleBmp[i].head = obstacleBmp[0].head;
	}
}

GameState_t GetGameState(void) { return(gameState); }

void ShowStartMenu(void){
	LCD_Clear();
	gameState = STATE_MENU;
	tRexBmp.visible = FALSE;
	jumpButtonBmp.visible = FALSE;
	tRexDirection = MOVE_NONE;

	for(uint16_t i = 0; i < OBSTACLES_NUMBER; i++){
		obstacleBmp[i].visible = FALSE;
	}

	uint16_t x = (LCD_WIDTH - startButtonBmp.w)/2;
	uint16_t y = (LCD_HEIGHT - startButtonBmp.h)/2 -50;
	DrawBmp(&startButtonBmp, x, y);

	DrawBmp(&tRexBmpGoogle, 270,300);
	DrawBmp(&tRexBmpGreen, 400, 300);

	if (tRexBmpGoogle.selected)
		SelectTrexGoogle();
	if (tRexBmpGreen.selected)
		SelectTrexGreen();
}

void DisplayGameOver(void){
	for(int i = 0; i < 5; i++){
		//DeleteBmp(&tRexBmp);
		//HAL_Delay(200);
		DrawBmpWithout_A(&tRexBmp, tRexBmp.x, tRexBmp.y);
		HAL_Delay(1000);
	}
}

void DisplayGameOver(void){
	for(int i = 0; i < 5; i++){
		//DeleteBmp(&tRexBmp);
		//HAL_Delay(200);
		DrawBmpWithout_A(&tRexBmp, tRexBmp.x, tRexBmp.y);
		HAL_Delay(1000);
	}
}

void StartGame(void){
	LCD_Clear();
	gameState = STATE_GAME;
	startButtonBmp.visible = FALSE;

	DrawBmp(&tRexBmp, TREX_X_POS, LCD_HEIGHT - GROUND_HEIGHT - tRexBmp.h); //Weis ni werum +1
	DrawBmp(&jumpButtonBmp, LCD_WIDTH - jumpButtonBmp.w - 50, 100);
	DrawBmp(&obstacleBmp[0], LCD_WIDTH - obstacleBmp[0].w, LCD_HEIGHT - GROUND_HEIGHT - obstacleBmp[0].h);
	DrawBmp(&pauseButtonBmp, 20,20);

	//--- Draw Ground
	const LCD_Color_t clrGround = {0, 0, 0};
	uint16_t y1 = LCD_HEIGHT - GROUND_HEIGHT;
	uint16_t y2 = y1 + GROUND_LINE_WIDTH;

	LCD_SetDrawArea(0, y1, LCD_WIDTH - 1, y2 - 1);
	LCD_EnableDrawMode();

	for (uint32_t pixel = 0; pixel < LCD_WIDTH * GROUND_LINE_WIDTH; pixel++) {
		LCD_Set(&clrGround);
	}
}

event_T PauseGame (event_T event)
{
	gameState = STATE_PAUSE;
	event.eventFlag = FALSE;
	return event;
}

event_T ContinueGame (event_T event)
{
	gameState = STATE_GAME;
	event.eventFlag = FALSE;
	return event;
}

void SelectTrexGoogle (void)
{
	tRexBmp = tRexBmpGoogle;
	tRexBmpGoogle.selected = TRUE;
	tRexBmpGreen.selected = FALSE;
	LCD_SetForegroundColor(ColorWhite);
	LCD_Rect(tRexBmpGreen.x-10, tRexBmpGreen.y-10, tRexBmpGreen.w+20, tRexBmpGreen.h+20);
	DrawBmp(&tRexBmpGreen, tRexBmpGreen.x, tRexBmpGreen.y);

	LCD_SetForegroundColor(ColorRed);
	LCD_Rect(tRexBmp.x-10, tRexBmp.y-10, tRexBmp.w+20, tRexBmp.h+20);
	DrawBmp(&tRexBmp, tRexBmp.x, tRexBmp.y);
}

void SelectTrexGreen (void)
{
	tRexBmp = tRexBmpGreen;
	tRexBmpGoogle.selected = FALSE;
	tRexBmpGreen.selected = TRUE;
	LCD_SetForegroundColor(ColorWhite);
	LCD_Rect(tRexBmpGoogle.x-10, tRexBmpGoogle.y-10, tRexBmpGoogle.w+20, tRexBmpGoogle.h+20);
	DrawBmp(&tRexBmpGoogle, tRexBmpGoogle.x, tRexBmpGoogle.y);

	LCD_SetForegroundColor(ColorRed);
	LCD_Rect(tRexBmp.x-10, tRexBmp.y-10, tRexBmp.w+20, tRexBmp.h+20);
	DrawBmp(&tRexBmp, tRexBmp.x, tRexBmp.y);
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
static char CheckEventBmp(bmp_t bmp, event_T event){
	if(bmp.visible == FALSE)
		return FALSE;

	if (!event.eventFlag)
		return FALSE;

	//--- Check if the event was in the domain of the object
	if(	event.x >= bmp.x && event.x <= bmp.x + bmp.w &&
		event.y >= bmp.y && event.y <= bmp.y + bmp.h)
	{
		return TRUE;
	}

	return FALSE;
}

char OnClickStartButton(event_T event){
	return(CheckEventBmp(startButtonBmp, event));
}

char OnClickJumpButton(event_T event){
	return(CheckEventBmp(jumpButtonBmp, event));
}

char OnClickPauseButton(event_T event){
	return(CheckEventBmp(pauseButtonBmp, event));
}

char OnClickTRexGoogle(event_T event){
	return(CheckEventBmp(tRexBmpGoogle, event));
}

char OnClickTRexGreen(event_T event){
	return(CheckEventBmp(tRexBmpGreen, event));
}

void MoveObstacles(void){
	//--- Timer for Obtscales
	static uint32_t tickOld = 0;
	const uint32_t tickNew = HAL_GetTick();

	if(tickNew < tickOld + 15)
		return;

	tickOld = tickNew;

	//--- Move obstacles
	//No nid random, momänt immer nach 350 pixel
	for(uint16_t i = 0; i < OBSTACLES_NUMBER; i++){
		if(obstacleBmp[i].visible == TRUE){
			//--- Obstacle is visible
			if(obstacleBmp[i].x <= 3){
				//--- if it reached left side, delete
				DeleteBmp(&obstacleBmp[i]);
			}
			else{
				//--- shift obstacle
				ShiftBmp(&obstacleBmp[i], -5, 0);
			}
		}
		else{
			uint16_t j = i == 0 ? OBSTACLES_NUMBER - 1 : i - 1;
			if(obstacleBmp[j].visible == TRUE){
				//--- previous obstacle is visible
				if(obstacleBmp[j].x < LCD_WIDTH - (400 + ((rand() %20) *10))){
					//--- previous obstacle moved more than 350 pixel
					DrawBmp(&obstacleBmp[i], LCD_WIDTH - obstacleBmp[i].w, LCD_HEIGHT - GROUND_HEIGHT - obstacleBmp[i].h);
				}
			}
		}
	}
}
void MoveTRex(void){
	static uint32_t tickOld = 0;

	if(tRexDirection == MOVE_NONE)
		return;

	//--- Timer for Obtscales
	const uint32_t tickNew = HAL_GetTick();

	if(tickNew < tickOld + 10)
		return;

	tickOld = tickNew;

	//--- Move TRex
	if(tRexDirection == MOVE_UP){
		if(tRexBmp.y <= 150){
			tRexDirection = MOVE_DOWN;
		}
		else{
			if(tRexBmp.y <160)
				ShiftBmp(&tRexBmp, 0, -2);
			else if(tRexBmp.y <180)
				ShiftBmp(&tRexBmp, 0, -3);
			else if(tRexBmp.y <210)
				ShiftBmp(&tRexBmp, 0, -4);
			else {
					ShiftBmp(&tRexBmp, 0, -5);
				 }
		}
	}
	else if(tRexDirection == MOVE_DOWN){
		if(tRexBmp.y >= (LCD_HEIGHT - GROUND_HEIGHT - startButtonBmp.h + 1)){
			tRexDirection = MOVE_NONE;
			MoveBmp(&tRexBmp, TREX_X_POS, LCD_HEIGHT - GROUND_HEIGHT - tRexBmp.h);
		}
		else{
			if(tRexBmp.y <160)
				ShiftBmp(&tRexBmp, 0, 2);
			else if(tRexBmp.y <180)
				ShiftBmp(&tRexBmp, 0, 3);
			else if(tRexBmp.y <210)
				ShiftBmp(&tRexBmp, 0, 4);
			else
				{
					ShiftBmp(&tRexBmp, 0, 5);
				}
		}
	}
}
void InitTRexJump(void){
	if(tRexDirection == MOVE_NONE){
		tRexDirection = MOVE_UP;
	}
}

//*********************************************************************
//*** Funktions for Collision Check									***
//*********************************************************************
char CheckCollision(void){
	if(tRexBmp.visible == FALSE)
		return FALSE;

	for(uint16_t i = 0; i < OBSTACLES_NUMBER; i++){
		if(obstacleBmp[i].visible == FALSE)
			continue;

		if (tRexBmp.x + tRexBmp.w < obstacleBmp[i].x 		||
			tRexBmp.x > obstacleBmp[i].x + obstacleBmp[i].w ||
			tRexBmp.y + tRexBmp.h < obstacleBmp[i].y 		||
			tRexBmp.y > obstacleBmp[i].y + obstacleBmp[i].h)
		{
			//The image border (rectangle) of the TRex and the obstacle are not colliding
			continue;
		}

		node_t *TRexPixel1   = tRexBmp.head;			//Pixel 1 and 2 create a line
		node_t *TRexPixel2   = TRexPixel1->next;
		node_t *ObstPixel 	 = obstacleBmp[0].head;		//pixel of obstacle
		node_t pixel = {0, LCD_HEIGHT, NULL};					//a pixel which is definitely not in the TRex shape

		int collisions = 0;								//Number of collisions

		while(ObstPixel != NULL){
			while(TRexPixel1 != NULL && TRexPixel2){
				//check for a collision between Lines
				if( CheckLineCollision(	pixel.x, pixel.y, 													//Line 1 Pos A
										ObstPixel->x + obstacleBmp[i].x, ObstPixel->y + obstacleBmp[i].y, 	//Line 1 Pos B
										TRexPixel1->x + tRexBmp.x, TRexPixel1->y + tRexBmp.y,				//Line 2 Pos A
										TRexPixel2->x + tRexBmp.x, TRexPixel2->y + tRexBmp.y) == 1)				//Line 2 Pos B
				{
					collisions++;
				}

				//--- next line of the Obstacle border
				node_t *tmp = TRexPixel2->next;
				TRexPixel1 = TRexPixel2;
				TRexPixel2 = tmp;
			}

			//--- if there were an odd number of collisions, the point was in the TRex Shape
			if(collisions != 0 && collisions % 2 != 0){
				DrawBmp(&obstacleBmp[i], obstacleBmp[i].x, obstacleBmp[i].y);	//redraw osbtacle (as maybe the TRex Border deleted some parts of the obstacle)
				PrintBorder(tRexBmp, ColorBlue);
				PrintBorder(obstacleBmp[i], ColorBlue);
				LCD_SetForegroundColor(ColorRed);
				LCD_Rect(ObstPixel->x + obstacleBmp[i].x - 1, ObstPixel->y + obstacleBmp[i].y - 1, 3, 3);
				return TRUE;
			}

			TRexPixel1 = tRexBmp.head;
			TRexPixel2 = TRexPixel1->next;
			ObstPixel  = ObstPixel->next;
			collisions = 0;
		}
	}
	return FALSE;
}
/*char CheckCollision(void){

	if(tRexBmp.visible == FALSE)
		return FALSE;

	node_t *ptrTRex   = NULL;
	node_t *ptrCactus = NULL;

	//bmp 0 punmkt oben links
	//array von unten links
	//lcd 0 punkt oben links


	for(uint16_t i = 0; i < OBSTACLES_NUMBER; i++){

		if(obstacleBmp[i].visible == FALSE)
			continue;

		if (tRexBmp.x + tRexBmp.w < obstacleBmp[i].x ||
			tRexBmp.x > obstacleBmp[i].x + obstacleBmp[i].w ||
			tRexBmp.y + tRexBmp.h < obstacleBmp[i].y ||
			tRexBmp.y > obstacleBmp[i].y + obstacleBmp[i].h)
		{
			//The image border (rectangle) of the TRex and the obstacle are not colliding
			continue;
		}

		ptrTRex   = tRexBmp.head;
		ptrCactus = obstacleBmp[0].head;

		while(ptrCactus != NULL){
			while(ptrTRex != NULL){
				if (obstacleBmp[i].x + ptrCactus->x == tRexBmp.x + ptrTRex->x  &&
					obstacleBmp[i].y + obstacleBmp[i].h - ptrCactus->y == tRexBmp.y + tRexBmp.h - ptrTRex->y)
				{
					return TRUE;
				}
				ptrTRex = ptrTRex->next;
			}
			ptrTRex   = tRexBmp.head;
			ptrCactus = ptrCactus->next;
		}
	}
	return FALSE;
}*/

//*********************************************************************
//*** Game score counting and display on LCD						***
//*********************************************************************

void ScoreCount (void) {

	static uint32_t GameScore =0;			//Variable for GameScore counting
	char score_string [4];					//String for Score to Print on LCD

	//Timing variables
	static uint32_t tickOld = 0;
	const uint32_t tickNew = HAL_GetTick();

	if (GetGameState() == STATE_GAME) {

		if(tickNew > tickOld + 100) {					//if 100ms passed

			tickOld = tickNew;
			GameScore+=1;								//increment score

			//convert score to string and print on LCD
			itoa(GameScore, score_string, 10);
			LCD_SetForegroundColor(ColorRed);
			LCD_String(640, 25, "Your Score is: ");
			LCD_String(750, 25, score_string);
		}
	}
	else {
		GameScore =0;
	}
}

int CheckLineCollision(uint16_t A1_x, uint16_t A1_y, uint16_t B1_x, uint16_t B1_y,
                       uint16_t A2_x, uint16_t A2_y, uint16_t B2_x, uint16_t B2_y) {

	//Please do it for me i ma nüm
}

