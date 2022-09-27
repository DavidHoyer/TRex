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
  *  @file fpu.h
  *  @ingroup common
  *  @author Nikolaij Saegesser
  *  @brief Floating Point Unit access
  */

#pragma once

#if defined(__cplusplus)
extern "C" {
#endif


/* Includes */

#include "common/types.h"
#include "common/attributes.h"


/* Functions */

LEGUAN_ALWAYS_INLINE float32_t FPU_abs32(float32_t x);
LEGUAN_ALWAYS_INLINE float64_t FPU_abs64(float64_t x);

LEGUAN_ALWAYS_INLINE float32_t FPU_sqrt32(float32_t x);
LEGUAN_ALWAYS_INLINE float64_t FPU_sqrt64(float64_t x);


#if defined(__cplusplus)
}
#endif
