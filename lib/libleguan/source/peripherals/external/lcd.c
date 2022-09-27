 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  *          _                                                      *
  *         | |                                                     *
  *         | |     ___  __ _ _   _  __ _ _ __                      *
  *         | |    / _ \/ _` | | | |/ _` | '_ \                     *
  *         | |___|  __/ (_| | |_| | (_| | | | |                    *
  *         \_____/\___|\__, |\__,_|\__,_|_| |_|                    *
  *        ============= __/ | ==================                   *
  *                     |___/           BFH 2021                    *
  *                                                                 *
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  * This software can be used by students and other personal of the *
  * Bern University of Applied Sciences under the terms of the MIT  *
  * license.                                                        *
  * For other persons this software is under the terms of the GNU   *
  * General Public License version 2.                               *
  *                                                                 *
  * Copyright &copy; 2021, Bern University of Applied Sciences.     *
  * All rights reserved.                                            *
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
  *  @file lcd.c
  *  @ingroup peripherals
  *  @author Nikolaij Saegesser
  *  @brief Display implementation
  */

#include "peripherals/external/lcd.h"

#include "peripherals/internal/spi.h"

#include "common/core.h"
#include "common/attributes.h"
#include "common/intrinsics.h"
#include "common/processor.h"
#include "common/math.h"

#include "resources/font.inc"

#include <string.h>

static LCD_Color_t m_foreground_color, m_background_color;

static uint16_t m_console_cursor_x, m_console_cursor_y;

/* LCD */

static LEGUAN_ALWAYS_INLINE void LCD_Command(uint8_t cmd) {
    LCD->cmd = cmd;
}

static LEGUAN_ALWAYS_INLINE void LCD_Data8(uint8_t data) {
    LCD->data8 = data;
}

static LEGUAN_ALWAYS_INLINE void LCD_Data16(uint16_t data) {
    LCD->data16 = data;
}

static LEGUAN_ALWAYS_INLINE void LCD_CommandData8(uint8_t cmd, uint8_t data) {
    LCD_Command(cmd);
    LCD_Data8(data);
}

static LEGUAN_ALWAYS_INLINE void LCD_CommandData16(uint8_t cmd, uint16_t data) {
    LCD_Command(cmd);
    LCD_Data16(data);
}

result_t LCD_SetDrawArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	if (x1 > LCD_WIDTH || y1 > LCD_HEIGHT)
		return RESULT_INVALID_ARGUMENTS;
	if (x2 > LCD_WIDTH || y2 > LCD_HEIGHT)
		return RESULT_INVALID_ARGUMENTS;

    /* Set horizontal region */
    LCD_Command(LCD_CMD_SET_COLUMN_ADDRESS);
    LCD_Data8(x1 >> 8);
    LCD_Data8(x1 & 0xFF);
    LCD_Data8(x2 >> 8);
    LCD_Data8(x2 & 0xFF);

    /* Set vertical region */
    LCD_Command(LCD_CMD_SET_PAGE_ADDRESS);
    LCD_Data8(y1 >> 8);
    LCD_Data8(y1 & 0xFF);
    LCD_Data8(y2 >> 8);
    LCD_Data8(y2 & 0xFF);

    return RESULT_SUCCESS;
}

void LCD_EnableDrawMode(void) {
    LCD_Command(LCD_CMD_WRITE_MEMORY_START);
}

result_t LCD_Init(void) {

    /* Turn display on */
    LCD_Command(LCD_CMD_SET_DISPLAY_ON);

    /* Flip LCD horizontally */
    LCD_CommandData8(LCD_CMD_SET_ADDRESS_MODE, 0b00000010);

    CORE_Delay(10);

    /* Set LCD color format */
    LCD->cmd  = LCD_CMD_SET_PIXEL_DATA_INTERFACE;
    LCD_CommandData8(LCD_CMD_SET_PIXEL_DATA_INTERFACE, 0b011); // RGB565

    LCD_SetDrawArea(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);

    CORE_Delay(100);

    /* Start pixel access mode */
    LCD_EnableDrawMode();

    LCD_SetForegroundColor(ColorWhite);
    LCD_SetBackgroundColor(ColorBlack);
    LCD_Clear();

    return RESULT_SUCCESS;
}


void LCD_Stream(const char *string, bool new_line) {
	if (m_console_cursor_y >= LCD_HEIGHT - CONSOLE_FONT_CHAR_HEIGHT) {
		m_console_cursor_y = 0;
	}

	if (new_line) {
		LCD_StringContinue(string);
		LCD_StringContinue("\n");
	}
	else
	    LCD_StringContinue(string);
}


void LCD_SetForegroundColor(color_t color) {
	m_foreground_color.r = color.r;
	m_foreground_color.g = color.g;
	m_foreground_color.b = color.b;
}

void LCD_SetBackgroundColor(color_t color) {
	m_background_color.r = color.r;
	m_background_color.g = color.g;
	m_background_color.b = color.b;
}


static LEGUAN_ALWAYS_INLINE void LCD_Set(const LCD_Color_t *color) {
    LCD_Data16(*(const uint16_t*)color);
}

result_t LCD_Pixel(uint16_t x, uint16_t y) {
	if (x > LCD_WIDTH || y > LCD_HEIGHT)
		return RESULT_INVALID_ARGUMENTS;

    LCD_SetDrawArea(x, y, x, y);
    LCD_EnableDrawMode();

    LCD_Set(&m_foreground_color);

    return RESULT_SUCCESS;
}

result_t LCD_Rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
    /* Set working region */
    R_TRY(LCD_SetDrawArea(x, y, x + width, y + height));
    LCD_EnableDrawMode();

    /* Fill working region with color */
    for (uint32_t i = 0; i < width * height; i++)
        LCD_Set(&m_foreground_color);

    return RESULT_SUCCESS;
}

result_t LCD_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	if (x1 > LCD_WIDTH || y1 > LCD_HEIGHT)
		return RESULT_INVALID_ARGUMENTS;
	if (x2 > LCD_WIDTH || y2 > LCD_HEIGHT)
		return RESULT_INVALID_ARGUMENTS;

    int16_t delta_x = +abs((int32_t)x2 - (int32_t)x1);
    int16_t delta_y = -abs((int32_t)y2 - (int32_t)y1);

    int16_t speed_x = x1 < x2 ? 1 : -1;
    int16_t speed_y = y1 < y2 ? 1 : -1;

    int16_t error = delta_x + delta_y;

    /* Bresenham's line drawing algorithm */
    while (x1 != x2 && y1 != y2) {
        LCD_Pixel(x1, y1);

        if ((error * 2) > delta_y) {
            error += delta_y;
            x1 += speed_x;
        }

        if ((error * 2) < delta_x) {
            error += delta_x;
            y1 += speed_y;
        }
    }

    return RESULT_SUCCESS;
}

static void LCD_CircleImpl(int16_t center_x, int16_t center_y, int16_t x, int16_t y) {
    LCD_Pixel(center_x + x, center_y + y);
    LCD_Pixel(center_x - x, center_y + y);
    LCD_Pixel(center_x + x, center_y - y);
    LCD_Pixel(center_x - x, center_y - y);

    LCD_Pixel(center_x + y, center_y + x);
    LCD_Pixel(center_x - y, center_y + x);
    LCD_Pixel(center_x + y, center_y - x);
    LCD_Pixel(center_x - y, center_y - x);
}

result_t LCD_Circle(uint16_t center_x, uint16_t center_y, uint16_t radius) {
	/* Explicitly not check for invalid positions here because
	 * placing a circle outside of the viewing window might
	 * be useful to draw e.g semi-circles
	 */

    int16_t x = 0;
    int16_t y = radius;
    int16_t d = 3 - 2 * radius;

    LCD_CircleImpl(center_x, center_y, x, y);

    /* Bresenham's circle drawing algorithm */
    while (y >= x) {
        x += 1;

        if (d > 0) {
            y -= 1;
            d += 4 * (x - y) + 10;
        } else {
            d += 4 * x + 6;
        }

        LCD_CircleImpl(center_x, center_y, x, y);
    }

    return RESULT_SUCCESS;
}

result_t LCD_Character(uint16_t x, uint16_t y, char c) {
	/* Set working region same size as character bitmap */
    R_TRY(LCD_SetDrawArea(x, y, x + CONSOLE_FONT_CHAR_WIDTH, y + CONSOLE_FONT_CHAR_HEIGHT));
    LCD_EnableDrawMode();

    /* Draw all 1s in bitmap with foreground color, all 0s with background color */
    for (uint32_t i = 0; i < CONSOLE_FONT_CHAR_WIDTH * CONSOLE_FONT_CHAR_HEIGHT; i++) {
        if (ConsoleFont[(uint8_t)c][i / 8] & (1 << (i % 8)))
            LCD_Set(&m_foreground_color);
        else {
            LCD_Set(&m_background_color);
        }
    }

    return RESULT_SUCCESS;
}

result_t LCD_String(uint16_t start_x, uint16_t start_y, const char *string) {
    uint16_t x = start_x, y = start_y;
    while (true) {
    	char c = *string++;

    	if (c == '\n') {
    		x = 0;
    		y += CONSOLE_FONT_CHAR_HEIGHT;
    		continue;
    	} else if (c == '\x00') {
    		break;
    	}

        if (LCD_Character(x, y, c) != RESULT_SUCCESS)
        	return RESULT_INVALID_ARGUMENTS;

        /* Advance cursor horizontally */
        x += CONSOLE_FONT_CHAR_WIDTH;
    }

    m_console_cursor_x = x;
    m_console_cursor_y = y;

    return RESULT_SUCCESS;
}

result_t LCD_StringContinue(const char *string) {
	return LCD_String(m_console_cursor_x, m_console_cursor_y, string);
}

void LCD_Clear(void) {
	/* Set working region fill entire screen */
    LCD_SetDrawArea(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
    LCD_EnableDrawMode();

    for (uint32_t pixel = 0; pixel < LCD_WIDTH * LCD_HEIGHT; pixel++) {
        LCD_Set(&m_background_color);
    }

    m_console_cursor_x = 0;
    m_console_cursor_y = 0;

}

result_t LCD_DrawBuffer(LCD_Color_t *buffer, size_t size) {
	if (buffer == NULL || size == 0)
		return RESULT_INVALID_ARGUMENTS;

    LCD_EnableDrawMode();
    for (uint32_t i = 0; i < size; i++)
        LCD_Data16(*(uint16_t*)&buffer[i]);

    return RESULT_SUCCESS;
}


/* Touch */

static void LCD_TouchEnable(void) {
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
}

static void LCD_TouchDisable(void) {
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
}

LEGUAN_MAYBE_UNUSED
static result_t LCD_TouchReadRegister(uint8_t address, uint8_t *value) {
	if (value == NULL)
		return RESULT_INVALID_ARGUMENTS;

    uint8_t send_buffer[]    = { address << 1 | 1, 0 };
    uint8_t receive_buffer[] = { 0, 0 };

    LCD_TouchEnable();
    result_t result = SPI_WriteRead(SPI_Touch, receive_buffer, send_buffer, 2);
    LCD_TouchDisable();

    *value = receive_buffer[1];

    return result;
}

LEGUAN_MAYBE_UNUSED
static result_t LCD_TouchReadBuffer(uint8_t address, uint8_t *buffer, size_t size) {
	if (buffer == NULL || size == 0)
		return RESULT_INVALID_ARGUMENTS;

    address = (address << 1) | 1;

    LCD_TouchEnable();
    result_t result = SPI_WriteRead(SPI_Touch, buffer, &address, size);
    LCD_TouchDisable();

    return result;
}

static result_t LCD_TouchWriteRegister(uint8_t address, uint8_t data) {
    uint8_t send_buffer[] = { address << 1 | 0, data };

    LCD_TouchEnable();
    result_t result = SPI_Write(SPI_Touch, send_buffer, 2);
    LCD_TouchDisable();

    return result;
}

static result_t LCD_TouchSendCommand(uint8_t address) {
    address <<= 1;

    LCD_TouchEnable();
    result_t result = SPI_Write(SPI_Touch, &address, 1);
    LCD_TouchDisable();

    return result;
}

result_t LCD_TouchInit(void) {
    /* Set interrupt line to push-pull */
    //lcd_touch_write_register(LCD_TOUCH_REG_GENERAL_CONFIG, 0xC0);

    /* Enable position and pressure averaging */
    R_TRY(LCD_TouchWriteRegister(LCD_TOUCH_REG_OP_MODE_CONFIG, 0x06));

    /* Average touch position over 4 samples */
    //lcd_touch_write_register(LCD_TOUCH_REG_MEASURE_AVERAGE_CONFIG, 0x55);

    /* Set panel setup time to 800us */
    //lcd_touch_write_register(LCD_TOUCH_REG_PANEL_SETUP_TIME_CONFIG, 0x77);

    /* Set interrupt line rise time to 4us and fall time to 50us */
    //lcd_touch_write_register(LCD_TOUCH_REG_TOUCH_DETECT_PULLUP_CONFIG, 0x22);

    /* Sample for 32us */
    //lcd_touch_write_register(LCD_TOUCH_REG_ADC_SAMPLE_TIME_CONFIG, 0xA0);

    /* Start first measurement */
    LCD_TouchStartMeasurement();

    return RESULT_SUCCESS;
}

void LCD_TouchStartMeasurement(void) {
    LCD_TouchSendCommand(LCD_TOUCH_CMD_MEASURE_XY);
}

void LCD_TouchSetTouchCallback(LCD_TouchCallback_t callback) {

}

result_t LCD_TouchGetPosition(LCD_TouchPosition_t *position) {
	if (position == NULL)
		return RESULT_INVALID_ARGUMENTS;

    uint8_t send_buffer[5] = { (LCD_TOUCH_CMD_MEASURE_X << 1) | 1, 0, 0, 0, 0 };
    uint8_t recv_buffer[5] = { 0 };

   do {
        LCD_TouchStartMeasurement();
        LCD_TouchEnable();
        CORE_Delay(3);

        memset(recv_buffer, 0x00, 5);

        R_TRY(SPI_WriteRead(SPI_Touch, recv_buffer, send_buffer, 5));

        LCD_TouchDisable();
    } while ((recv_buffer[2] & 0b11) == 0b11 || (recv_buffer[4] & 0b11) == 0b11);

   LCD_TouchStartMeasurement();

    if ((recv_buffer[2] & 0b11) != 0b00 || (recv_buffer[4] & 0b11) != 0b00) {
    	position->x = 0;
    	position->y = 0;
    } else {
    	position->x = recv_buffer[1] << 4 | recv_buffer[2] >> 4;
    	position->y = recv_buffer[3] << 4 | recv_buffer[4] >> 4;
    }

    position->x = ((float32_t)position->x / 0x1000) * LCD_WIDTH;
    position->y = ((float32_t)(0x1000 - position->y) / 0x1000) * LCD_HEIGHT;

    return RESULT_SUCCESS;
}
