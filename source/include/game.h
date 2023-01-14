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
	STATE_PAUSE,
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
event_T PauseGame (event_T);
event_T ContinueGame (event_T);


void SelectTrexGoogle (void);
void SelectTrexGreen (void);


GameState_t GetGameState(void);

event_T CheckEvent(void);
//char 	CheckEventBmp(bmp_t bmp, event_T event);
char 	OnClickStartButton(event_T event);
char 	OnClickJumpButton(event_T event);
char 	OnClickPauseButton(event_T event);
char 	OnClickTRexGoogle(event_T event);
char 	OnClickTRexGreen(event_T event);

void	MoveObstacles(void);
void	MoveTRex(void);
void	InitTRexJump(void);

node_t *GetBoarderTRex (void);
node_t *GetBoarderCactus (void);

void PrintBorderTRex (void);
void PrintBorderCactus (void);

char 	CheckCollision(void);

void ScoreCount (void);

#endif /* INCLUDE_GAME_H_ */
