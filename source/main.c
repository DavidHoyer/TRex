#include <leguan.h>
#include <cube.h>

int main() {
	int x =0;
	x++;
	x = 1;
	//Dave isch a spästu

	// Initialize Hardware
	CUBEMX_Init();
	// Initialize Leguan board
	LEGUAN_Init();

	// Set logging output destination to be the LCD
	LOG_SetDestination(LCD_Stream);

	// Main loop
	while (true) {

	}
}
