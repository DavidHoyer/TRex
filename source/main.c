//*********************************************************************
//*** Includes     													***
//*********************************************************************
#include "include/game.h"
#include <cube.h>
#include <stdio.h>
#include <stdlib.h>

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

		ScoreCount();						//Counting GameScore when running
		event = CheckEvent();				//Check for incomming Touch event

		//--- Handle Touch Events on the LCD Screen
		if(event.eventFlag){

			switch(GetGameState())
			{
				case STATE_PAUSE:
					//--- Pause Button pressed
					if(OnClickPauseButton(event))
						event = ContinueGame(event);
					break;
				case STATE_MENU:
					//--- Start Button pressed
					if(OnClickStartButton(event))
						StartGame();
					//--- Touch on T-Rex
					else if(OnClickTRexGoogle(event))
						SelectTrexGoogle();
					//--- Touch on T-Rex
					else if(OnClickTRexGreen(event))
						SelectTrexGreen();
					break;

				case STATE_GAME:
					//--- Pause Button pressed
					if(OnClickPauseButton(event))
						PauseGame(event);
					//--- Jump Button pressed
					else if(OnClickJumpButton(event))
						InitTRexJump();
					break;

				default:
					break;

			}
		}

		if(GetGameState() == STATE_GAME){
			MoveObstacles();
			MoveTRex();

			//check if the T-Rex collided with one of the obstacles
			if(CheckCollision()){
				DisplayGameOver();
				ShowStartMenu();
			}
		}

	}//-- End while
};
