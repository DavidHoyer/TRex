#include <leguan.h>
#include <cube.h>

int main() {
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
