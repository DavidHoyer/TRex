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
  *  @file fpga.h
  *  @ingroup peripherals
  *  @author Nikolaij Saegesser
  *  @brief FPGA MMIO Registers
  */

#pragma once

#if defined(__cplusplus)
extern "C" {
#endif


/* Includes */
#include "common/types.h"
#include "common/attributes.h"
#include "common/mmio.h"
#include "common/color.h"


/* Macros */
#define FPGA_BASE_ADDRESS               0xC8000000

#define FPGA                            MMIO_MAP(FPGA_BASE_ADDRESS, FPGA_MemoryMap_t)


#define FPGA_RGB_LED_MATRIX_WIDTH       11
#define FPGA_RGB_LED_MATRIX_HEIGHT      10

/* Types */

typedef BGR1_t FPGA_MatrixColor_t;

typedef struct PACKED {
    uint16_t all;
} FPGA_Packed7SegmentRegister_t;
static_assert(sizeof(FPGA_Packed7SegmentRegister_t) == sizeof(reg16_t), "FPGA_Packed7SegmentRegister_t type definition invalid!");

typedef struct LEGUAN_PACKED {
    uint8_t all : 4;
    uint8_t padding;
} FPGA_DecimalPointRegister_t;
static_assert(sizeof(FPGA_DecimalPointRegister_t) == sizeof(reg16_t), "FPGA_DecimalPointRegister_t type definition invalid!");

typedef struct LEGUAN_PACKED {
    union {
        struct {
            uint8_t _1 : 1;
            uint8_t _2 : 1;
            uint8_t _3 : 1;
            uint8_t _4 : 1;
            uint8_t _5 : 1;
            uint8_t _6 : 1;
            uint8_t _7 : 1;
            uint8_t _8 : 1;
        };
        uint8_t all;
    };
} FPGA_DIPSwitchRegister_t;
static_assert(sizeof(FPGA_DIPSwitchRegister_t) == sizeof(uint8_t), "FPGA_DIPSwitchRegister_t type definition invalid!");

typedef struct LEGUAN_PACKED {
    union {
        struct {
            uint8_t button_0         : 1;
            uint8_t button_1         : 1;
            uint8_t button_2         : 1;
            uint8_t button_3         : 1;
            uint8_t joystick_north   : 1;
            uint8_t joystick_east    : 1;
            uint8_t joystick_south   : 1;
            uint8_t joystick_west    : 1;
            uint8_t joystick_center  : 1;
        };
        uint16_t all;
    };

} FPGA_Buttons_t;
static_assert(sizeof(FPGA_Buttons_t) == sizeof(reg16_t), "FPGA_Buttons_t type definition invalid!");

typedef struct LEGUAN_PACKED {
    uint16_t color_mode;
    uint32_t line_index;
    uint16_t access_mode;
} FPGA_RGBLineRegister_t;
static_assert(sizeof(FPGA_RGBLineRegister_t) == 8, "FPGA_RGBLineRegister_t type definition invalid!");

typedef struct LEGUAN_PACKED {
    uint8_t b        : 1;
    uint8_t g        : 1;
    uint8_t r        : 1;
    uint16_t padding : 13;
} FPGA_RGPPixelRegister_t;
static_assert(sizeof(FPGA_RGPPixelRegister_t) == sizeof(reg16_t), "FPGA_RGPPixelRegister_t type definition invalid!");

typedef struct LEGUAN_PACKED {
    reg16_t reset;                                  /* 0x00 */

    uint8_t padding_x02[0x0E];                      /* 0x02 */

    struct {
        reg16_t digits[4];                          /* 0x10 */
        FPGA_Packed7SegmentRegister_t hexadecimal;  /* 0x18 */
        FPGA_Packed7SegmentRegister_t bcd;          /* 0x1A */
        FPGA_DecimalPointRegister_t dp;             /* 0x1C */
        uint8_t padding_x1E[2];                     /* 0x1E */
    } seven_segment;

    struct {
        FPGA_DIPSwitchRegister_t state_right;       /* 0x20 */
        FPGA_DIPSwitchRegister_t state_left;        /* 0x21 */
        reg16_t pressed_irq;                        /* 0x22 */
        reg16_t released_irq;                       /* 0x24 */
    } dip_switches;

    struct {
        FPGA_Buttons_t state;                       /* 0x26 */
        reg16_t pressed_irq;                        /* 0x28 */
        reg16_t released_irq;                       /* 0x2A */
    } buttons;

    reg16_t scanning_divider;                       /* 0x2C */
    reg16_t irq_generators;                         /* 0x2E */

    uint8_t padding_x30[0x400 - 0x30];              /* 0x30 */

    struct {
        FPGA_RGBLineRegister_t line;                /* 0x400 */
        uint8_t padding[0x600 - 0x408];             /* 0x408 */
        FPGA_RGPPixelRegister_t pixel[10][11];      /* 0x600 */
    } rgb_matrix;
                                                    /* 0x6DC */
} FPGA_MemoryMap_t;
static_assert(sizeof(FPGA_MemoryMap_t) == 0x6DC, "FPGA_MemoryMap_t type definition invalid!");

typedef enum {
    Button0 = 0,
    Button1 = 1,
    Button2 = 2,
    Button3 = 3,
    JoystickNorth = 4,
    JoystickEast = 5,
    JoystickSouth = 6,
    JoystickWest = 7,
    JoystickCenter = 8
} FPGA_Button_t;


/* Functions */

/**
 * @brief Initializes the FPGA peripheral
 */
result_t FPGA_Init(void);

/**
 * @brief Resets all FPGA registers
 */
result_t FPGA_Reset(void);

/**
 * @brief Displays a decimal value between 0 and 9999 on the 7 Segments
 * @param value Integer to display
 */
result_t FPGA_7SegDisplayDecimal(uint16_t value);

/**
 * @brief Displays a floating point value on the 7 Segments
 * @param value Floating point to display
 * @param precision Number of decimal places to be displayed (maximum 3)
 */
result_t FPGA_7SegDisplayFloat(float32_t value, uint8_t precision);

/**
 * @brief Displays a hexadecimal value between 0x00 and 0xFFFF on the 7 Segments
 * @param value Integer to display
 */
result_t FPGA_7SegDisplayHexadecimal(uint16_t value);


/**
 * @brief Gets the value of the left DIP switches
 * @return Binary representation of enabled switches
 */
uint8_t FPGA_DIPGetLeft(void);

/**
 * @brief Gets the value of the right DIP switches
 * @return Binary representation of enabled switches
 */
uint8_t FPGA_DIPGetRight(void);

/**
 * @brief Unmasks (enables) the "switch moved to ON position" interrupt for one switch
 * @param switch_id Number of the switch between 1 and 8
 */
result_t FPGA_DIPUnmaskOnIRQ(uint8_t switch_id);

/**
 * @brief Unmasks (enables) the "switch moved to OFF position" interrupt for one switch
 * @param switch_id Number of the switch between 1 and 8
 */
result_t FPGA_DIPUnmaskOffIRQ(uint8_t switch_id);

/**
 * @brief Masks (disables) the "switch moved to ON position" interrupt for one switch
 * @param switch_id Number of the switch between 1 and 8
 */
result_t FPGA_DIPMaskOnIRQ(uint8_t switch_id);

/**
 * @brief Masks (disables) the "switch moved to OFF position" interrupt for one switch
 * @param switch_id Number of the switch between 1 and 8
 */
result_t FPGA_DIPMaskOffIRQ(uint8_t switch_id);


/**
 * @brief Queries the state of a button or the joystick
 * @param button Button to query
 * @return State of button
 */
bool FPGA_GetButton(FPGA_Button_t button);

/**
 * @brief Unmasks (enables) the "button pressed" interrupt for one button
 * @param button Button to enable interrupt for
 */
result_t FPGA_ButtonUnmaskPressIRQ(FPGA_Button_t button);

/**
 * @brief Unmasks (enables) the "button released" interrupt for one button
 * @param button Button to enable interrupt for
 */
result_t FPGA_ButtonUnmaskReleaseIRQ(FPGA_Button_t button);

/**
 * @brief Masks (disables) the "button pressed" interrupt for one switch
 * @param button Button to disable interrupt for
 */
result_t FPGA_ButtonMaskPressIRQ(FPGA_Button_t button);

/**
 * @brief Masks (disables) the "button released" interrupt for one switch
 * @param button Button to disable interrupt for
 */
result_t FPGA_ButtonMaskReleaseIRQ(FPGA_Button_t button);


/**
 * @brief Get the current button scanning divider
 * @return Scanning divider
 */
uint8_t FPGA_GetScanningDivider(void);

/**
 * @brief Sets teh button scanning divider
 * @param divider Divider
 */
void FPGA_SetScanningDivider(uint16_t divider);


/**
 * @brief Sets the color of one pixel on the RGB LED Matrix
 * @param x X coordinate of the pixel
 * @param y Y coordinate of the pixel
 * @param red Red LED State
 * @param green Green LED State
 * @param blue Blue LED State
 */
result_t FPGA_MatrixSetPixel(uint8_t x, uint8_t y, color_t color);


#if defined(__cplusplus)
}
#endif
