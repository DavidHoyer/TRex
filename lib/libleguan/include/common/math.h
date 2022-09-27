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
  *  @file math.h
  *  @ingroup common
  *  @author Nikolaij Saegesser
  *  @brief Common math functions
  */

#pragma once

#if defined(__cplusplus)
extern "C" {
#endif


/* Includes */
#include "common/types.h"

#include <math.h>
#include <stdlib.h>

uint32_t  MATH_ClampUl(uint32_t value, uint32_t min, uint32_t max);
int32_t   MATH_ClampL(int32_t value, int32_t min, int32_t max);
float32_t MATH_ClampF(float32_t value, float32_t min, float32_t max);
float64_t MATH_ClampD(float64_t value, float64_t min, float64_t max);

/**
 * @brief Clamps a value between a min and max value
 * @param value Value to clamp
 * @param min Minimal value
 * @param max Maximal value
 * @return Clamped value
 */
#define MATH_Clamp(value, min, max) _Generic((value),                   \
                                    uint32_t:           clamp_ul,       \
                                    default:            clamp_l,        \
                                    float32_t:          clamp_f,        \
                                    float64_t:          clamp_d         \
                                )(value, min, max)

/**
 * @brief Returns the sign of the passed in number
 * @param x Number
 * @param Sign of x. 1 if > 0, -1 if < 0, 0 if 0
 */
int8_t MATH_Sign(int64_t x);

/**
 * @brief Turns a binary value into bcd notation
 * @param bin Binary value
 * @return BCD Value
 */
uint8_t MATH_BinaryToBCD(uint8_t bin);


#if defined(__cplusplus)
}
#endif
