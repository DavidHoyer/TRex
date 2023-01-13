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
  *  @file color.h
  *  @ingroup common
  *  @author Nikolaij Saegesser
  *  @brief Color type definitions
  */

#pragma once

#if defined(__cplusplus)
extern "C" {
#endif


/* Macros */

#define RGB8_COLOR(r, g, b)   		{ (uint8_t)(r), 			(uint8_t)(g), 		    (uint8_t)(b)        }
#define RGBA16_COLOR(r, g, b, a)  	{ (uint16_t)(r), 		    (uint16_t)(g), 		    (uint16_t)(b)       }
#define BGR8_COLOR(r, g, b)   		{ (uint8_t)(b), 			(uint8_t)(g), 		    (uint8_t)(r)        }
#define RGB565_COLOR(r, g, b) 		{ ((uint8_t)(r)) >> 3, 	    ((uint8_t)(g)) >> 2,    ((uint8_t)(b)) >> 3 }
#define BGR565_COLOR(r, g, b) 		{ ((uint8_t)(b)) >> 3, 	    ((uint8_t)(g)) >> 2,    ((uint8_t)(r)) >> 3 }
#define RGB1_COLOR(r, g, b)   		{ (uint8_t)(r > 0), 		(uint8_t)(g > 0), 	    (uint8_t)(b > 0)    }
#define BGR1_COLOR(r, g, b)   		{ (uint8_t)(b > 0), 		(uint8_t)(g > 0), 	    (uint8_t)(r > 0)    }

/* Types */

typedef struct {
	uint8_t r, g, b;
	uint8_t padding;
} RGB8_t;

typedef struct {
	uint8_t b, g, r;
	uint8_t padding;
} BGR8_t;

typedef struct {
	uint16_t r, g, b, a;
} RGBA16_t;

typedef struct {
	uint16_t r : 5;
	uint16_t g : 6;
	uint16_t b : 5;
} RGB565_t;

typedef struct {
	uint16_t b : 5;
	uint16_t g : 6;
	uint16_t r : 5;
} BGR565_t;

typedef struct {
	uint8_t r : 1;
	uint8_t g : 1;
	uint8_t b : 1;
} RGB1_t;

typedef struct {
	uint8_t b : 1;
	uint8_t g : 1;
	uint8_t r : 1;
} BGR1_t;

typedef RGBA16_t color_t;
#define COLOR(r, g, b, a) RGBA16_COLOR(r, g, b, a)

/* Constants */
const static color_t ColorRed      = COLOR(0xFF, 0x00, 0x00, 0xFF);
const static color_t ColorGreen    = COLOR(0x00, 0xFF, 0x00, 0xFF);
const static color_t ColorBlue     = COLOR(0x00, 0x00, 0xFF, 0xFF);
const static color_t ColorYellow   = COLOR(0xFF, 0xFF, 0x00, 0xFF);
const static color_t ColorCyan     = COLOR(0x00, 0xFF, 0xFF, 0xFF);
const static color_t ColorPurple   = COLOR(0xFF, 0x00, 0xFF, 0xFF);
const static color_t ColorBlack    = COLOR(0x00, 0x00, 0x00, 0xFF);
const static color_t ColorWhite    = COLOR(0xFF, 0xFF, 0xFF, 0xFF);
const static color_t ColorGray     = COLOR(0x80, 0x80, 0x80, 0xFF);


#if defined(__cplusplus)
}
#endif
