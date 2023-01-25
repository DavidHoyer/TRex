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
 * @brief displays the GameOver state and inits a delay
 */
void DisplayGameOver(void);

/**
 * @brief Selects the Google T-Rex as player character
 */
void SelectTrexGoogle (void);

/**
 * @brief Selects the green T-Rex as player character
 */
void SelectTrexGreen (void);

/**
 * @brief Returns the current game state.
 * @return Game state
 */
GameState_t GetGameState(void);

/**
 * @brief Checks if an event on teh LCD display occured
 * @return event
 */
event_T CheckEvent(void);

/**
 * @brief Checks if the event happend on the start button
 * @param event
 * @return flag
 */
char OnClickStartButton(event_T event);

/**
 * @brief Checks if the event happend on the jump button
 * @param event
 * @return flag
 */
char OnClickJumpButton(event_T event);

/**
 * @brief Checks if the event happend on the pause button
 * @param event
 * @return flag
 */
char OnClickPauseButton(event_T event);

/**
 * @brief Checks if the event happend on the Google TRex
 * @param event
 * @return flag
 */
char OnClickTRexGoogle(event_T event);

/**
 * @brief Checks if the event happend on the Green TRex
 * @param event
 * @return flag
 */
char OnClickTRexGreen(event_T event);

/**
 * @brief Moves the obstacles on the LCD
 */
void MoveObstacles(void);

/**
 * @brief Moves the T-Rex on the LCD
 */
void MoveTRex(void);

/**
 * @brief Initializes the jump of the T-Rex
 */
void InitTRexJump(void);

/**
 * @brief Checks for a collision between the T-Rex and the obstacles
 * @return flag
 */
char CheckCollision(void);

/**
 * @brief Counts the current score for the player
 */
void ScoreCount (void);

#endif /* INCLUDE_GAME_H_ */
