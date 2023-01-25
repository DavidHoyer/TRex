/**
  *  @file game.h
  *  @ingroup include
  *  @date 30.12.2022
  *  @author Lukas Roth
  *  @brief Game control functions
  */

#ifndef INCLUDE_GAME_H_
#define INCLUDE_GAME_H_

//*********************************************************************
//*** Includes     													***
//*********************************************************************
#include "objects.h"

//*********************************************************************
//*** Enums      													***
//*********************************************************************

/** @enum GameState_t
 * 	@brief This enum contains all the States the game can get into
 */
typedef enum {
	/**No state*/
	STATE_NONE,
	/**State: MENU*/
	STATE_MENU,
	/**State: Game is running*/
	STATE_GAME,
	/**State: Game is paused*/
	STATE_PAUSE,
	/**State: Game is over*/
	STATE_GAME_OVER,
}GameState_t;

//*********************************************************************
//*** Structs 														***
//*********************************************************************

/** @struct event_T
 * 	@brief This struct is used to save an incoming touch event
 */
typedef struct {
	/**1 = event happened, 0 = nothing happened*/
	uint8_t		eventFlag;
	/**Event position x*/
	uint16_t 	x;
	/**Event position y*/
	uint16_t 	y;
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
 * @brief Checks if an event on the LCD display occurred
 * @return event
 */
event_T CheckEvent(void);

/**
 * @brief Checks if the event happend on the start button
 * @param event : incoming touch event
 * @return flag : Start button pressed or not
 */
char OnClickStartButton(event_T event);

/**
 * @brief Checks if the event happend on the jump button
 * @param event : incoming touch event
 * @return flag : Jump button pressed or not
 */
char OnClickJumpButton(event_T event);

/**
 * @brief Checks if the event happend on the pause button
 * @param event : incoming touch event
 * @return flag : Pause button pressed or not
 */
char OnClickPauseButton(event_T event);

/**
 * @brief Checks if the event happend on the Google TRex
 * @param event : incoming touch event
 * @return flag : T-Rex Google pressed or not
 */
char OnClickTRexGoogle(event_T event);

/**
 * @brief Checks if the event happend on the Green TRex
 * @param event : incoming touch event
 * @return flag : T-Rex Green pressed or not
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
 * @return flag : collision happend or not
 */
char CheckCollision(void);

/**
 * @brief Counts the current score for the player
 */
void ScoreCount (void);

#endif /* INCLUDE_GAME_H_ */
