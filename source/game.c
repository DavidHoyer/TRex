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

bmp_t tRexBmp 			= {0, 0, 77, 90, tRex_pixelData, 0,0};
bmp_t startButtonBmp 	= {0, 0, 248, 101, startButton_pixelData, 0,0};
bmp_t jumpButtonBmp 	= {0, 0, 80, 80, jumpButton_pixelData, 0,0};
bmp_t obstacleBmp[3]	= { {0, 0, 59, 80, cactus_pixelData, 0,0},
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
	LCD_SetBackgroundColor(ColorWhite);
	ShowStartMenu();

	tRexBmp.head = GetBoarderTRex();
	obstacleBmp[0].head = GetBoarderCactus();
	obstacleBmp[1].head = GetBoarderCactus();
	obstacleBmp[2].head = GetBoarderCactus();
	//PrintBorderTRex();
	//PrintBorderCactus();
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
	uint16_t y = (LCD_HEIGHT - startButtonBmp.h)/2;
	DrawBmp(&startButtonBmp, x, y);
}

void StartGame(void){
	LCD_Clear();
	gameState = STATE_GAME;
	startButtonBmp.visible = FALSE;

	DrawBmp(&tRexBmp, TREX_X_POS, LCD_HEIGHT - GROUND_HEIGHT - tRexBmp.h); //Weis ni werum +1
	DrawBmp(&jumpButtonBmp, LCD_WIDTH - jumpButtonBmp.w - 50, 100);
	DrawBmp(&obstacleBmp[0], LCD_WIDTH - obstacleBmp[0].w, LCD_HEIGHT - GROUND_HEIGHT - obstacleBmp[0].h);

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

	node_t *ptrTRex = tRexBmp.head;
	node_t *ptrCactus = NULL;


	/*
	bmp 0 punmkt oben links
	array von unten links
	lcd 0 punkt oben links
	 */

	for(uint16_t i = 0; i < OBSTACLES_NUMBER; i++){

		ptrCactus = obstacleBmp[i].head;

		while(ptrCactus != NULL){
			while(ptrTRex != NULL){

				if (obstacleBmp[i].visible == TRUE &&
					obstacleBmp[i].x + ptrCactus->x == tRexBmp.x + ptrTRex->x &&
					obstacleBmp[i].y + obstacleBmp[i].h - ptrCactus->y == tRexBmp.y + tRexBmp.h - ptrTRex->y)
				{
					return TRUE;
				}
				ptrTRex = ptrTRex->next;
			}
			ptrCactus = ptrCactus->next;
		}


		/*if(	obstacleBmp[i].visible == TRUE &&
			obstacleBmp[i].x < tRexBmp.x + tRexBmp.w &&
			obstacleBmp[i].x + obstacleBmp[i].w > tRexBmp.x &&
			tRexBmp.y + tRexBmp.h >= obstacleBmp[i].y)
		{
			return TRUE;
		}*/
	}

	return FALSE;
}

void PrintBorderTRex (void) {

	node_t *ptr = tRexBmp.head;

	LCD_SetForegroundColor(ColorBlue);

	while(ptr != NULL){
		LCD_Pixel(tRexBmp.x + ptr->x, tRexBmp.y + (tRexBmp.h - ptr->y));
		ptr = ptr->next;
	}
}

void PrintBorderCactus (void) {

	node_t *ptr = obstacleBmp[0].head;

	LCD_SetForegroundColor(ColorBlue);

	while(ptr != NULL){
		LCD_Pixel(obstacleBmp[0].x + ptr->x, obstacleBmp[0].y + (obstacleBmp[0].h - ptr->y));
		ptr = ptr->next;
	}
}

node_t *GetBoarderTRex (void) {

	node_t *head = NULL;
	uint32_t test = 0;

	uint32_t i;


	for(uint16_t yi = 1; yi < tRexBmp.h-1; yi++){
		for(uint16_t xi = 1; xi < tRexBmp.w-1; xi++){


			i = yi*tRexBmp.w + xi;

			if ( *(*(tRexBmp.pixels + i) +3) != *(*(tRexBmp.pixels + i + 1) +3)
				|| *(*(tRexBmp.pixels + i) +3) != *(*(tRexBmp.pixels + i - 1) +3)){
				test++;
				node_t *pixel = malloc(sizeof(node_t));
				pixel->x = xi+1;
				pixel->y = yi+1;
				pixel->next = head;
				head = pixel;

				continue;
			}


			if (*(*(tRexBmp.pixels + i) +3) != *(*(tRexBmp.pixels + i +tRexBmp.w) +3)
				|| *(*(tRexBmp.pixels + i) +3) != *(*(tRexBmp.pixels + i - tRexBmp.w) +3)) {
				test++;
				node_t *pixel = malloc(sizeof(node_t));
				pixel->x = xi+1;
				pixel->y = yi+1;
				pixel->next = head;
				head = pixel;

				continue;
			}
		}
	}
	test = 0;
	return head;
}

node_t *GetBoarderCactus (void) {

	node_t *head = NULL;
	uint32_t test = 0;

	uint32_t i;


	for(uint16_t yi = 1; yi < obstacleBmp[0].h-1; yi++){
		for(uint16_t xi = 1; xi < obstacleBmp[0].w-1; xi++){


			i = yi*obstacleBmp[0].w + xi;

			if ( *(*(obstacleBmp[0].pixels + i) +3) != *(*(obstacleBmp[0].pixels + i + 1) +3)
				|| *(*(obstacleBmp[0].pixels + i) +3) != *(*(obstacleBmp[0].pixels + i - 1) +3)){
				test++;
				node_t *pixel = malloc(sizeof(node_t));
				pixel->x = xi+1;
				pixel->y = yi+1;
				pixel->next = head;
				head = pixel;

				continue;
			}


			if (*(*(obstacleBmp[0].pixels + i) +3) != *(*(obstacleBmp[0].pixels + i +obstacleBmp[0].w) +3)
				|| *(*(obstacleBmp[0].pixels + i) +3) != *(*(obstacleBmp[0].pixels + i - obstacleBmp[0].w) +3)) {
				test++;
				node_t *pixel = malloc(sizeof(node_t));
				pixel->x = xi+1;
				pixel->y = yi+1;
				pixel->next = head;
				head = pixel;

				continue;
			}
		}
	}
	test = 0;
	return head;
}


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
