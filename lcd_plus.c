/*
 * lcd_plus.c
 *
 *  Created on: 26.06.2022
 *      Author: fue1
 */

#include <leguan.h>
#include <cube.h>
#include <math.h>

#include "main.h"
#include "lcd_plus.h"

static color_t ColorBall;

void draw_ball (double x_m, double y_m, double r_m) {
	int x = x_m / PIXEL_SIZE;
	int y = y_m / PIXEL_SIZE;
	int r = r_m / PIXEL_SIZE;

	int x_start, x_end;
	int y_start, y_end;

	color_t colorWoodTexture;

	static int x_bup = 0;
	static int y_bup = 0;

	if (x_bup < x){
		x_start = -1.05*r - (x - x_bup);
		x_end   =  1.05*r;
	}
	else {
		x_start = -1.05*r;
		x_end   =  1.05*r + (x_bup - x);
	}

	if (y_bup < y){
		y_start = -1.05*r - (y - y_bup);
		y_end   =  1.05*r;
	}
	else {
		y_start = -1.05*r;
		y_end   =  1.05*r + (y_bup - y);
	}

	x_bup = x;
	y_bup = y;

	int r2 = r*r;
	for (int c = x_start; c < x_end; c++) {
		for (int l = y_start; l < y_end; l+=2) {
			int p = c*c + l*l;
			if (p <= r2) {
				float intensity = 1.0f - (float)p/(float)r2;
				ColorBall.r = (int)(102*intensity) >> 3;
				ColorBall.g = (int)(204*intensity) >> 2;
				ColorBall.b = 0xFF >> 3;
				LCD_SetForegroundColor(ColorBall);
			}
			else {
				if (labyrinth[(l+y)/20][(c+x)/20] == 0) {
					colorWoodTexture.b = imageWoodTexture[l+y][c+x][0] >> 3 & 0x1f;
					colorWoodTexture.g = imageWoodTexture[l+y][c+x][1] >> 2 & 0x3f;
					colorWoodTexture.r = imageWoodTexture[l+y][c+x][2] >> 3 & 0x1f;
					LCD_SetForegroundColor(colorWoodTexture);
				}
				else {
					LCD_SetForegroundColor(ColorBlack);
				}
			}
			LCD_Pixel(c+x, l+y);
		}
	}
	for (int c = x_start; c < x_end; c++) {
		for (int l = y_start+1; l < y_end; l+=2) {
			int p = c*c + l*l;
			if (p <= r2) {
				float intensity = 1.0f - (float)p/(float)r2;
				ColorBall.r = (int)(102*intensity) >> 3;
				ColorBall.g = (int)(204*intensity) >> 2;
				ColorBall.b = 0xFF >> 3;
				LCD_SetForegroundColor(ColorBall);
			}
			else {
				if (labyrinth[(l+y)/20][(c+x)/20] == 0) {
					colorWoodTexture.b = imageWoodTexture[l+y][c+x][0] >> 3 & 0x1f;
					colorWoodTexture.g = imageWoodTexture[l+y][c+x][1] >> 2 & 0x3f;
					colorWoodTexture.r = imageWoodTexture[l+y][c+x][2] >> 3 & 0x1f;
					LCD_SetForegroundColor(colorWoodTexture);
				}
				else {
					LCD_SetForegroundColor(ColorBlack);
				}
			}
			LCD_Pixel(c+x, l+y);
		}
	}
}

void LCD_Background () {
	color_t colorWoodTexture;
	colorWoodTexture.a = 0xff;
	for (int line = 0; line < LCD_HEIGHT; line++) {
		for (int column = 0; column < LCD_WIDTH; column++) {
			if (labyrinth[line/20][column/20] == 0) {
				colorWoodTexture.b = imageWoodTexture[line][column][0] >> 3 & 0x1f;
				colorWoodTexture.g = imageWoodTexture[line][column][1] >> 2 & 0x3f;
				colorWoodTexture.r = imageWoodTexture[line][column][2] >> 3 & 0x1f;
				LCD_SetForegroundColor(colorWoodTexture);
			}
			else {
				LCD_SetForegroundColor(ColorBlack);
			}
			// LCD_Pixel(column, LCD_HEIGHT - line);
			LCD_Pixel(column, line);
		}
	}
}

