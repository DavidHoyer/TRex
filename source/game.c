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
bmp_t tRexBmpGoogle 	= {10, 10, 77, 90, tRex_pixelDataGoogle, 0, 1, 0};
bmp_t tRexBmpGreen		= {0, 0, 122, 90, tRex_pixelDataGreen, 0, 0, 0};


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
	LCD_SetBackgroundColor(ColorWhite);			//BG Color to White

	//--- tests wege line collision
	//int result = doIntersect(10,10,0,0,1,0,2,1);
	//result = result + 1;

	//--- Detect Border of BMP files (Must be done before Array conversion)
	//tRexBmpGoogle.head  = GetBoarder(tRexBmpGoogle);
	AssignBorderDinoGoogle();
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

	//-- Tests fürd border detection
	PrintBorder(obstacleBmp[0], ColorRed);
	PrintBorder(tRexBmpGoogle, ColorRed);
	PrintBorder(tRexBmpGreen, ColorRed);
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
			while(TRexPixel1 != NULL && TRexPixel2 != NULL){
				//check for a collision between Lines

				if( doIntersect( pixel.x, pixel.y, 													//Line 1 Pos A
								ObstPixel->x + obstacleBmp[i].x, ObstPixel->y + obstacleBmp[i].h - obstacleBmp[i].y, 	//Line 1 Pos B
								TRexPixel1->x + tRexBmp.x, TRexPixel1->y + tRexBmp.h - tRexBmp.y,				//Line 2 Pos A
								TRexPixel2->x + tRexBmp.x, TRexPixel2->y + tRexBmp.h - tRexBmp.y) == true)				//Line 2 Pos B
				{
					//LCD_Pixel(bmp.x + ptr->x, bmp.y + (bmp.h - ptr->y));
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
				PrintBorder(tRexBmp, ColorRed);
				PrintBorder(obstacleBmp[i], ColorBlue);
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

// The main function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
//bool doIntersect(Point p1, Point q1, Point p2, Point q2)
int doIntersect(uint16_t A1_x, uint16_t A1_y, uint16_t B1_x, uint16_t B1_y,
        		uint16_t A2_x, uint16_t A2_y, uint16_t B2_x, uint16_t B2_y)
{
	Point p1 = {A1_x, A1_y};
	Point q1 = {B1_x, B1_y};
	Point p2 = {A2_x, A2_y};
	Point q2 = {B2_x, B2_y};

    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special Cases
    // p1, q1 and p2 are collinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;

    // p1, q1 and q2 are collinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;

    // p2, q2 and p1 are collinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;

     // p2, q2 and q1 are collinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false; // Doesn't fall in any of the above cases
}

//*********************************************************************
//*** Funktions for Collision Check									***
//*********************************************************************
int CheckLineCollision(uint16_t A1_x, uint16_t A1_y, uint16_t B1_x, uint16_t B1_y,
                       uint16_t A2_x, uint16_t A2_y, uint16_t B2_x, uint16_t B2_y)
{
	node_t A1,A2,B1,B2;

	if (A1_x < B1_x) {
		A1.x = A1_x;	A1.y = A1_y;
		B1.x = B1_x;	B1.y = B1_y;
	}
	else{
		A1.x = B1_x;	A1.y = B1_y;
		B1.x = A1_x;	B1.y = A1_y;
	}
	if (A2_x < B2_x) {
		A2.x = A2_x;	A2.y = A2_y;
		B2.x = B2_x;	B2.y = B2_y;
	}
	else{
		A2.x = B2_x;	A2.y = B2_y;
		B2.x = A2_x;	B2.y = A2_y;
	}

	double slope1 = (B1.y - A1.y) / (B1.x - A1.x);
    double slope2 = (B2.y - A2.y) / (B2.x - A2.x);
    double y_intercept1 = A1.y - (slope1 * A1.x);
    double y_intercept2 = A2.y - (slope2 * A2.x);
    double x, y;

    if (slope1 == slope2) {
        if (y_intercept1 == y_intercept2) {
            return 1; // Lines are identical
        } else {
            return 0; // Lines are parallel
        }
    }

    x = (y_intercept2 - y_intercept1) / (slope1 - slope2);
    y = slope1 * x + y_intercept1;

    if (((A1.x <= x && x <= B1.x) || (B1.x <= x && x <= A1.x)) &&
        ((A1.y <= y && y <= B1.y) || (B1.y <= y && y <= A1.y)) &&
        ((A2.x <= x && x <= B2.x) || (B2.x <= x && x <= A2.x)) &&
        ((A2.y <= y && y <= B2.y) || (B2.y <= y && y <= A2.y))) {
        return 1; // Lines intersect
    } else {
        return 0; // Lines do not intersect
    }
}

//Temporary function will be replaced in the future
void AssignBorderDinoGoogle(void){
	uint16_t pixels[19][2] = { 	{34,90}, {76,90}, {76,66}, {66,66}, {66,56},
								{56,56}, {56,38}, {37,19}, {37,4 }, {42,4 },
								{42,1 }, {33,1 }, {33,8 }, {27,8 }, {18,8 },
								{18,15}, {0,33},  {0,54},  {34,54} };

	node_t *head = NULL;	// Head of the border pixel linked list

	for(int i = 0; i < 19; i++){
		node_t *pixel = malloc(sizeof(node_t));
		pixel->x = pixels[i][0];
		pixel->y = pixels[i][1];
		pixel->next = head;
		head = pixel;
	}

	tRexBmpGoogle.head = head;
}
