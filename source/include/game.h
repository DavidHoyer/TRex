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

//--- Enum for all possible game states
typedef enum {
	STATE_NONE,
	STATE_MENU,
	STATE_GAME,
	STATE_PAUSE,
	STATE_GAME_OVER,
}GameState_t;

//*********************************************************************
//*** Structs 														***
//*********************************************************************

//--- Event Struct
typedef struct {
	uint8_t		eventFlag;		// 1 = event happened, 0 = nothing happened
	uint16_t 	x;				// Event position x
	uint16_t 	y;				// Event position y
}event_T;

//*********************************************************************
//*** Functions														***
//*********************************************************************

/**
 * @brief Initializes the game
 */
void GameInit(void);

/**
 * @brief displays the start menu
 */
void ShowStartMenu(void);

/**
 * @brief Starts the game, draw T-Rex etc.
 */
void StartGame(void);

/**
 * @brief Stops the movement of the T-Rex aswell as the obstacles
 */
void PauseGame(void);

/**
 * @brief Resumes the movement
 */
void ContinueGame(void);

/**
 * @brief Resumes the movement
 */
void DisplayGameOver(void);

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

char 	CheckCollision(void);
int CheckLineCollision(uint16_t A1_x, uint16_t A1_y, uint16_t A2_x, uint16_t A2_y,
					   uint16_t B1_x, uint16_t B1_y, uint16_t B2_x, uint16_t B2_y);
int doIntersect(uint16_t A1_x, uint16_t A1_y, uint16_t B1_x, uint16_t B1_y,
        		uint16_t A2_x, uint16_t A2_y, uint16_t B2_x, uint16_t B2_y);

void ScoreCount (void);

#endif /* INCLUDE_GAME_H_ */
