//*********************************************************************
//*** Includes     													***
//*********************************************************************
#include "include/game.h"
//#include <leguan.h>
#include <cube.h>
#include <stdio.h>
#include <stdlib.h>

//*********************************************************************
//*** Functions    													***
//*********************************************************************
char OnTimer(uint16_t miliseconds);
void ScoreCount (void);


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
			//--- Start Button pressed
			if(GetGameState() == STATE_MENU && OnClickStartButton(event) == TRUE){
				StartGame();
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
				HAL_Delay(3000);
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
