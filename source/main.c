#include <leguan.h>
#include <cube.h>
#include <stdint.h>
#include <limits.h>

void LukiInit(void);
void LukisFunction(void);

int main()
{
	// Initialize Hardware
	CUBEMX_Init();
	// Initialize Leguan board
	LEGUAN_Init();
	// Initialize LCD
	LCD_Init();

	//--- Declarations


	// Set logging output destination to be the LCD
	LOG_SetDestination(LCD_Stream);

	//--- Init functions
	LukiInit();

	//--- Main loop
	while (true)
	{
		LukisFunction();
	}
}

void LukiInit(void)
{
	//LCD_SetForegroundColor(ColorWhite);
	//LCD_Rect(0, 0, 480, 480);

	//Draw Down Button
	LCD_SetForegroundColor(ColorRed);
	LCD_String(0, 0, "Hello");
	//Draw Up Button
}

void LukisFunction(void)
{
	static int x_old = 0;
	static int y_old = 0;

	LCD_TouchPosition_t touchPosition = {0, 0};
	if (R_SUCCESS(LCD_TouchGetPosition(&touchPosition)))
	{
		int x_new = touchPosition.x;
		int y_new = touchPosition.y;

		if (x_new != 0 || y_new != 480)
		{
			LCD_SetForegroundColor(ColorWhite);
			LCD_Rect(x_old - 5, y_old - 5, 30, 30);

			LCD_SetForegroundColor(ColorRed);
			LCD_Rect (x_new, y_new, 20, 20);

			x_old = x_new;
			y_old = y_new;
		}
	}
	//CORE_Delay(200);
}
