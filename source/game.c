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

bmp_t tRexBmp;			//
bmp_t tRexBmpGoogle 	= {270, 300, 77, 90, tRex_pixelDataGoogle, 0, 1, 0};
bmp_t tRexBmpGreen		= {400, 300, 122, 90, tRex_pixelDataGreen, 0, 0, 0};


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
	//BG Color to White
	LCD_SetBackgroundColor(ColorWhite);

	//--- Detect Border of BMP files (Must be done before Array conversion)
	tRexBmpGoogle.head  = GetBoarder(tRexBmpGoogle);
	tRexBmpGreen.head   = GetBoarder(tRexBmpGreen);
	obstacleBmp[0].head = GetBoarder(obstacleBmp[0]);
	for(int i = 1; i < OBSTACLES_NUMBER; i++)
		obstacleBmp[i].head = obstacleBmp[0].head;

	//--- Convert Array to BGRA_5658
	ConvertArray(tRex_pixelDataGreen, tRexBmpGreen.w, tRexBmpGreen.h);
	ConvertArray(tRex_pixelDataGoogle, tRexBmpGoogle.w, tRexBmpGoogle.h);
	ConvertArray(startButton_pixelData, startButtonBmp.w, startButtonBmp.h);
	ConvertArray(jumpButton_pixelData, jumpButtonBmp.w, jumpButtonBmp.h);
	ConvertArray(pauseButton_pixelData, 75, 75);
	ConvertArray(cactus_pixelData, obstacleBmp[0].w, obstacleBmp[0].h);

	//--- show the start menu to begin
	ShowStartMenu();
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
		PrintBorder(obstacleBmp[0], ColorBlue);
		PrintBorder(tRexBmp, ColorRed);
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

	//--- delete border of T-Rex Green
	LCD_SetForegroundColor(ColorWhite);
	LCD_Rect(tRexBmpGreen.x-10, tRexBmpGreen.y-10, tRexBmpGreen.w+20, tRexBmpGreen.h+20);
	DrawBmp(&tRexBmpGreen, tRexBmpGreen.x, tRexBmpGreen.y);

	//--- Draw Border (gradient) for Google T-Rex
	LCD_Color_t clr1 = {0,0,31};
	LCD_Color_t clr2 = {0,50,31};
	LCD_Rect_Gradient(tRexBmp.x - 10, tRexBmp.y - 10, tRexBmp.w + 20, tRexBmp.h + 20, clr1, clr2);

	//--- Draw Google T-Rex
	LCD_SetForegroundColor(ColorWhite);
	LCD_Rect(tRexBmp.x-5, tRexBmp.y-5, tRexBmp.w+10, tRexBmp.h+10);
	DrawBmp(&tRexBmp, tRexBmp.x, tRexBmp.y);
}

void SelectTrexGreen (void)
{
	tRexBmp = tRexBmpGreen;
	tRexBmpGoogle.selected = FALSE;
	tRexBmpGreen.selected = TRUE;

	//--- Delete border of Google T-Rex
	LCD_SetForegroundColor(ColorWhite);
	LCD_Rect(tRexBmpGoogle.x-10, tRexBmpGoogle.y-10, tRexBmpGoogle.w+20, tRexBmpGoogle.h+20);
	DrawBmp(&tRexBmpGoogle, tRexBmpGoogle.x, tRexBmpGoogle.y);

	//--- Draw Border (gradient) for T-Rex Green
	LCD_Color_t clr2 = {0,0,31};
	LCD_Color_t clr1 = {0,50,31};
	LCD_Rect_Gradient(tRexBmp.x - 10, tRexBmp.y - 10, tRexBmp.w + 20, tRexBmp.h + 20, clr1, clr2);

	//--- Draw Google T-Rex
	LCD_SetForegroundColor(ColorWhite);
	LCD_Rect(tRexBmp.x-5, tRexBmp.y-5, tRexBmp.w+10, tRexBmp.h+10);
	DrawBmp(&tRexBmp, tRexBmp.x, tRexBmp.y);

	//--- Draw Green T-Rex
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
				ShiftBmp(&obstacleBmp[i], -4, 0);
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

//*********************************************************************
//*** Initialized the jump for the T-Rex							***
//*********************************************************************
void InitTRexJump(void){
	if(tRexDirection == MOVE_NONE)
		tRexDirection = MOVE_UP;
}

//*********************************************************************
//*** Checks if the T-Rex is colliding with one of the obstacles	***
//*********************************************************************
char CheckCollision(void){
	if(tRexBmp.visible == FALSE)
		return FALSE;

	for(uint16_t i = 0; i < OBSTACLES_NUMBER; i++){
		if(obstacleBmp[i].visible == FALSE)
			continue;

		//Check if the image borders of the T-Rex and the obstacles are colliding
		if (tRexBmp.x + tRexBmp.w < obstacleBmp[i].x 		||
			tRexBmp.x > obstacleBmp[i].x + obstacleBmp[i].w ||
			tRexBmp.y + tRexBmp.h < obstacleBmp[i].y 		||
			tRexBmp.y > obstacleBmp[i].y + obstacleBmp[i].h)
		{
			continue;
		}

		node_t *TRexPixelA = tRexBmp.head;			//Point 1 of T-Rex Border
		node_t *TRexPixelB = TRexPixelA->next;		//Point 2 of T-Rex Border
		node_t *ObstPixelA = obstacleBmp[0].head;	//Point 1 of Obstacle Border
		node_t *ObstPixelB = ObstPixelA->next;		//Point 2 of Obstacle Border

		while(ObstPixelA != NULL && ObstPixelB != NULL){
			while(TRexPixelA != NULL && TRexPixelB != NULL){
				//check for a collision between Lines
				if(doIntersect(	obstacleBmp[i].x + ObstPixelA->x, obstacleBmp[i].y + ObstPixelA->y,		//Line 1 Pos A
								obstacleBmp[i].x + ObstPixelB->x, obstacleBmp[i].y + ObstPixelB->y,		//Line 1 Pos B
								tRexBmp.x 		 + TRexPixelA->x, tRexBmp.y 	   + TRexPixelA->y,		//Line 2 Pos A
								tRexBmp.x 		 + TRexPixelB->x, tRexBmp.y 	   + TRexPixelB->y		//Line 2 Pos B
							   ) == true)
				{
					return true;
				}

				//--- Next line of the T-Rex border
				TRexPixelA = TRexPixelB;
				TRexPixelB = TRexPixelA->next;
			}

			//--- Next line of the Obstacle border
			ObstPixelA = ObstPixelB;
			ObstPixelB = ObstPixelA->next;

			//Start again on T-Rex border
			TRexPixelA   = tRexBmp.head;
			TRexPixelB   = TRexPixelA->next;
		}
	}
	return false;
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

typedef struct Point {
    double x;
    double y;
} Point;

// Given three collinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
bool onSegment(Point p, Point q, Point r)
{
    if (q.x <= fmax(p.x, r.x) && q.x >= fmin(p.x, r.x) &&
        q.y <= fmax(p.y, r.y) && q.y >= fmin(p.y, r.y))
       return true;

    return false;
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are collinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(Point p, Point q, Point r)
{
    // See https://www.geeksforgeeks.org/orientation-3-ordered-points/
    // for details of below formula.
    int val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);

    if (val == 0) return 0;  // collinear

    return (val > 0)? 1: 2; // clock or counterclock wise
}

// The main function that returns true if line segment 'p1q1' and 'p2q2' intersect.
//bool doIntersect(Point p1, Point q1, Point p2, Point q2)
int doIntersect(uint16_t A1_x, uint16_t A1_y, 	//Line 1 Pos A
        		uint16_t A2_x, uint16_t A2_y, 	//Line 1 Pos B
				uint16_t B1_x, uint16_t B1_y,	//Line 2 Pos A
				uint16_t B2_x, uint16_t B2_y)	//Line 2 Pos B
{
	Point p1 = {A1_x, A1_y};
	Point q1 = {A2_x, A2_y};
	Point p2 = {B1_x, B1_y};
	Point q2 = {B2_x, B2_y};

    // Find the four orientations needed for general and special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special Cases
    // p1, q1 and p2 are collinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1))
    	return true;

    // p1, q1 and q2 are collinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1))
    	return true;

    // p2, q2 and p1 are collinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2))
    	return true;

     // p2, q2 and q1 are collinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2))
    	return true;

    return false; // Doesn't fall in any of the above cases
}
