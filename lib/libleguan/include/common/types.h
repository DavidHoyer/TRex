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
  *  @file types.h
  *  @ingroup common
  *  @author Nikolaij Saegesser
  *  @brief Common types
  */

#pragma once

#if defined(__cplusplus)
extern "C" {
#endif


/* Includes */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>

#include "common/errors.h"

/* Types */

/** Floating point types **/
typedef float               float32_t;
typedef double              float64_t;

/** MMIO types **/
typedef uint8_t             addr_t;
typedef int32_t             off_t;

typedef volatile uint16_t   reg16_t;
typedef volatile uint32_t   reg32_t;

/** Commonly used structures **/
typedef struct {
	float x, y, z;
} vector3f_t;


#if defined(__cplusplus)
}
#endif
