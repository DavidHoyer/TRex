//*********************************************************************
//*** Includes     													***
//*********************************************************************
#include "include/game.h"
#include <cube.h>
#include <stdio.h>
#include <stdlib.h>

//*********************************************************************
//*** Functions    													***
//*********************************************************************
char OnTimer(uint16_t miliseconds);

//*********************************************************************
//*** Main      													***
//*********************************************************************
int main()
{
	event_T event;							//Event struct for Touch input

	CUBEMX_Init();							// Initialize Hardware
	LEGUAN_Init();							// Initialize Leguan board
	LOG_SetDestination(LCD_Stream);			// Set logging output destination to be the LCD
	GameInit();								// Initialize Game

	// Main loop
	while(TRUE){

		event = CheckEvent();				//Check for incomming Touch event
		ScoreCount();						//Counting GameScore when running

		if(event.eventFlag){

			//--- Pause Button pressed
			if(GetGameState() == STATE_PAUSE && OnClickPauseButton(event) == TRUE){
				event = ContinueGame(event);
			}

			//--- Start Button pressed
			if(GetGameState() == STATE_MENU && OnClickStartButton(event) == TRUE){
				StartGame();
			}
			//--- Start Button pressed
			if(GetGameState() == STATE_MENU && OnClickTRexGoogle(event) == TRUE){
				SelectTrexGoogle();
			}
			//--- Start Button pressed
			if(GetGameState() == STATE_MENU && OnClickTRexGreen(event) == TRUE){
				SelectTrexGreen();
			}

			//--- Pause Button pressed
			if(GetGameState() == STATE_GAME && OnClickPauseButton(event) == TRUE){
				event = PauseGame(event);
			}
			//--- Jump Button pressed
			if(GetGameState() == STATE_GAME && OnClickJumpButton(event) == TRUE){
				InitTRexJump();
			}
		}

		if(GetGameState() == STATE_GAME){
			MoveObstacles();
			MoveTRex();

			if(CheckCollision() == TRUE){
				DisplayGameOver();
				ShowStartMenu();
			}
		}
	}
};

char OnTimer(uint16_t miliseconds){
	static uint32_t tickOld = 0;
	const uint32_t tickNew = HAL_GetTick();

	if(tickNew < tickOld + miliseconds)
		return FALSE;

	tickOld = tickNew;
	return TRUE;
}
