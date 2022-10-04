#include <leguan.h>
#include <cube.h>
#include <stdint.h>
#include <limits.h>

void DrawInterface(void);

int main()
{
	// Initialize Hardware
	CUBEMX_Init();
	// Initialize Leguan board
	LEGUAN_Init();
	// Initialize LCD
	LCD_Init();

	LCD_Circle(50, 50, radius)

	// Set logging output destination to be the LCD
	LOG_SetDestination(LCD_Stream);

	//--- Main loop
	while (true)
	{
		DrawInterface();
	}
}

void DrawInterface(void)
{

}
