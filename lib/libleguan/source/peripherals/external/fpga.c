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
  *  @file fpga.c
  *  @ingroup peripherals
  *  @author Nikolaij Saegesser
  *  @brief FPGA implementation
  */

#include "peripherals/external/fpga.h"
#include "common/math.h"


result_t FPGA_Init(void) {
	FPGA_Reset();

	return RESULT_SUCCESS;
}


result_t FPGA_Reset(void) {
    FPGA->reset = true;

    return RESULT_SUCCESS;
}


static result_t Set7SegDigit(uint8_t digit, char value) {
	/* There's only four 7 segment displays, bail if out of range */
    if (digit > 3)
    	return RESULT_INVALID_ARGUMENTS;

    switch(value){
        case 0:     FPGA->seven_segment.digits[digit] = 0x3F; break;
        case 1:     FPGA->seven_segment.digits[digit] = 0x06; break;
        case 2:     FPGA->seven_segment.digits[digit] = 0x5B; break;
        case 3:     FPGA->seven_segment.digits[digit] = 0x4F; break;
        case 4:     FPGA->seven_segment.digits[digit] = 0x66; break;
        case 5:     FPGA->seven_segment.digits[digit] = 0x6D; break;
        case 6:     FPGA->seven_segment.digits[digit] = 0x7D; break;
        case 7:     FPGA->seven_segment.digits[digit] = 0x07; break;
        case 8:     FPGA->seven_segment.digits[digit] = 0x7F; break;
        case 9:     FPGA->seven_segment.digits[digit] = 0x6F; break;
        case 0x0A:  FPGA->seven_segment.digits[digit] = 0x77; break;
        case 0x0B:  FPGA->seven_segment.digits[digit] = 0x7C; break;
        case 0x0C:  FPGA->seven_segment.digits[digit] = 0x39; break;
        case 0x0D:  FPGA->seven_segment.digits[digit] = 0x5E; break;
        case 0x0E:  FPGA->seven_segment.digits[digit] = 0x79; break;
        case 0x0F:  FPGA->seven_segment.digits[digit] = 0x71; break;
        case '-':   FPGA->seven_segment.digits[digit] = 0x40; break;
        case ' ':	FPGA->seven_segment.digits[digit] = 0x00; break;
        default:    FPGA->seven_segment.digits[digit] = 0x00; break;
    }

    return RESULT_SUCCESS;
}

result_t FPGA_7SegDisplayDecimal(uint16_t value) {
	/* Maximum 4 digit decimal value is 9999 */
    if (value > 9999) value = 9999;

    /* Turn every digit into bcd */
    uint16_t bcd = 0;
    bcd |= MATH_BinaryToBCD((value / 1000) % 10) << 12;
    bcd |= MATH_BinaryToBCD((value / 100)  % 10) << 8;
    bcd |= MATH_BinaryToBCD((value / 10)   % 10) << 4;
    bcd |= MATH_BinaryToBCD((value / 1)    % 10) << 0;

    FPGA->seven_segment.dp.all = 0b0000;
    FPGA->seven_segment.bcd.all = bcd;

    return RESULT_SUCCESS;
}

result_t FPGA_7SegDisplayFloat(float32_t value, uint8_t precision) {
	/* There's only 4 displays. Decimal precision higher than 3 can't be displayed */
    if (precision > 3)
    	return RESULT_INVALID_ARGUMENTS;

    bool negative = value < 0;

    if (negative) {
    	if (precision == 0)
    		return RESULT_INVALID_ARGUMENTS;

    	precision--;
    	value *= -1;
    }

    FPGA->seven_segment.dp.all = 0b0001 << precision;

    /* Move all relevant digits above decimal point */
    for (uint8_t i = 0; i < precision; i++)
        value *= 10;

    /* Turn every digit into bcd */
    uint16_t bcd = 0;
    bcd |= MATH_BinaryToBCD((int32_t)(value / 1000) % 10) << 12;
    bcd |= MATH_BinaryToBCD((int32_t)(value / 100)  % 10) << 8;
    bcd |= MATH_BinaryToBCD((int32_t)(value / 10)   % 10) << 4;
    bcd |= MATH_BinaryToBCD((int32_t)(value / 1)    % 10) << 0;

    FPGA->seven_segment.bcd.all = bcd;

    /* Display negative sign if number was negative */
    if (negative)
    	Set7SegDigit(3, '-');

    return RESULT_SUCCESS;
}

result_t FPGA_7SegDisplayHexadecimal(uint16_t value) {
    FPGA->seven_segment.dp.all = 0b0000;
    FPGA->seven_segment.hexadecimal.all = value;

    return RESULT_SUCCESS;
}


uint8_t FPGA_DIPGetLeft(void) {
    return FPGA->dip_switches.state_left.all;
}

uint8_t FPGA_DIPGetRight(void) {
    return FPGA->dip_switches.state_right.all;
}

result_t FPGA_DIPUnmaskOnIRQ(uint8_t switch_id) {
    switch_id--;

    if (switch_id > 8)
    	return RESULT_INVALID_ARGUMENTS;

    FPGA->dip_switches.pressed_irq |= (1UL << switch_id);

    return RESULT_SUCCESS;
}

result_t FPGA_DIPUnmaskOffIRQ(uint8_t switch_id) {
    switch_id--;

    if (switch_id > 8)
    	return RESULT_INVALID_ARGUMENTS;

    FPGA->dip_switches.released_irq |= (1UL << switch_id);

    return RESULT_SUCCESS;
}

result_t FPGA_DIPMaskOnIRQ(uint8_t switch_id) {
    switch_id--;

    if (switch_id > 8)
    	return RESULT_INVALID_ARGUMENTS;

    FPGA->dip_switches.pressed_irq &= ~(1UL << switch_id);

    return RESULT_SUCCESS;
}

result_t FPGA_DIPMaskOffIRQ(uint8_t switch_id) {
    switch_id--;

    if (switch_id > 8)
    	return RESULT_INVALID_ARGUMENTS;

    FPGA->dip_switches.released_irq &= ~(1UL << switch_id);

    return RESULT_SUCCESS;
}


bool FPGA_GetButton(FPGA_Button_t button) {
    return (FPGA->buttons.state.all & (1UL << button)) != 0;
}

result_t FPGA_ButtonUnmaskPressIRQ(FPGA_Button_t button) {
    FPGA->buttons.pressed_irq |= (1UL << button);

    return RESULT_SUCCESS;
}

result_t FPGA_ButtonUnmaskReleaseIRQ(FPGA_Button_t button) {
    FPGA->buttons.released_irq |= (1UL << button);

    return RESULT_SUCCESS;
}

result_t FPGA_ButtonMaskPressIRQ(FPGA_Button_t button) {
    FPGA->buttons.pressed_irq &= ~(1UL << button);

    return RESULT_SUCCESS;
}

result_t FPGA_ButtonMaskReleaseIRQ(FPGA_Button_t button) {
    FPGA->buttons.released_irq &= ~(1UL << button);

    return RESULT_SUCCESS;
}

uint8_t FPGA_GetScanningDivider(void) {
    return FPGA->scanning_divider;
}

void FPGA_SetScanningDivider(uint16_t divider) {
    FPGA->scanning_divider = divider;
}


result_t FPGA_MatrixSetPixel(uint8_t x, uint8_t y, color_t color) {
    if (x >= FPGA_RGB_LED_MATRIX_WIDTH || y >= FPGA_RGB_LED_MATRIX_HEIGHT)
    	return RESULT_INVALID_ARGUMENTS;

    volatile FPGA_RGPPixelRegister_t *pixel = &FPGA->rgb_matrix.pixel[y][x];

    pixel->r = color.r;
    pixel->g = color.g;
    pixel->b = color.b;

    return RESULT_SUCCESS;
}
