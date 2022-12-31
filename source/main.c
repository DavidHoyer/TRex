#include "include/game.h"

//#include <leguan.h>
#include <cube.h>
#include <stdio.h>

char OnTimer(uint16_t miliseconds);

int main()
{
	event_T event;

	CUBEMX_Init();							// Initialize Hardware
	LEGUAN_Init();							// Initialize Leguan board
	LOG_SetDestination(LCD_Stream);			// Set logging output destination to be the LCD
	GameInit();								// Initialize Game

	// Main loop
	while(TRUE){
		event = CheckEvent();

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
}

char OnTimer(uint16_t miliseconds){
	static uint32_t tickOld = 0;
	const uint32_t tickNew = HAL_GetTick();

	if(tickNew < tickOld + miliseconds)
		return FALSE;

	tickOld = tickNew;
	return TRUE;
}
