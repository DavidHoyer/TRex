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
  *  @file math.c
  *  @ingroup common
  *  @author Nikolaij Saegesser
  *  @brief Common math functions
  */

#include "common/math.h"

uint32_t MATH_ClampUl(uint32_t value, uint32_t min, uint32_t max) {
    if (value < min) return min;
    if (value > max) return max;

    return value;
}

int32_t MATH_ClampL(int32_t value, int32_t min, int32_t max) {
    if (value < min) return min;
    if (value > max) return max;

    return value;
}

float32_t MATH_ClampF(float32_t value, float32_t min, float32_t max) {
    if (value < min) return min;
    if (value > max) return max;

    return value;
}

float64_t MATH_ClampD(float64_t value, float64_t min, float64_t max) {
    if (value < min) return min;
    if (value > max) return max;

    return value;
}

int8_t MATH_Sign(int64_t a) {
    if (a > 0)
        return 1;
    else if (a < 0)
        return -1;
    else
        return 0;
}

uint8_t MATH_BinaryToBCD(uint8_t bin) {
    return (bin % 10) | ((bin / 10) << 4);
}
