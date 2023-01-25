/**
  *  @file main.c
  *  @ingroup source
  *  @author Lukas Roth
  *  @brief main file of this game
  */

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
	while(true){

		ScoreCount();						//Counting GameScore when running
		event = CheckEvent();				//Check for incoming Touch event

		//--- Handle Touch Events on the LCD Screen
		if(event.eventFlag){
			switch(GetGameState())
			{
				case STATE_PAUSE:
					//--- Pause Button pressed
					if(OnClickPauseButton(event))
						ContinueGame();
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
						PauseGame();
					//--- Jump Button pressed
					else if(OnClickJumpButton(event))
						InitTRexJump();
					break;

				default:
					break;

			}
		}

		if(GetGameState() == STATE_GAME){
			MoveObstacles();				//--- Move obstacles to the left
			MoveTRex();						//--- If T-Rex jump is initialized, move T-Rex up or down

			//check if the T-Rex collided with one of the obstacles
			if(CheckCollision()){
				DisplayGameOver();			//--- Game Over with a delay
				ShowStartMenu();			//--- Restart Game
			}
		}

	}//-- End while
};//-- End main
