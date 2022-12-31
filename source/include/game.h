/*
 * Game.h
 *
 *  Created on: 30.12.2022
 *      Author: lukir
 */

#ifndef INCLUDE_GAME_H_
#define INCLUDE_GAME_H_

#include "objects.h"

//*********************************************************************
//*** Enums      													***
//*********************************************************************
typedef enum {
	STATE_NONE	= -1,
	STATE_MENU	= 0,
	STATE_GAME,
	STATE_GAME_OVER,
}GameState_t;

//*********************************************************************
//*** Structs 														***
//*********************************************************************
typedef struct {
	char 		eventFlag;
	uint16_t 	x;
	uint16_t 	y;
}event_T;

//*********************************************************************
//*** Functions														***
//*********************************************************************
void GameInit(void);
void ShowStartMenu(void);
void StartGame(void);

GameState_t GetGameState(void);

event_T CheckEvent(void);
//char 	CheckEventBmp(bmp_t bmp, event_T event);
char 	OnClickStartButton(event_T event);
char 	OnClickJumpButton(event_T event);

void	MoveObstacles(void);
void	MoveTRex(void);
void	InitTRexJump(void);
char 	CheckCollision(void);

#endif /* INCLUDE_GAME_H_ */