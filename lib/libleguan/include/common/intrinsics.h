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
  *  @file intrinsics.h
  *  @ingroup common
  *  @author Nikolaij Saegesser
  *  @brief Compiler intrinsics
  */

#pragma once

#if defined(__cplusplus)
extern "C" {
#endif


#include "common/attributes.h"

/* Macros */

/**
 * @brief Confuses the compiler to prevent it from optimizing away a variable
 * @param variable to prevent optimization of
 */
#define LEGUAN_DO_NOT_OPTIMIZE(x) asm volatile("" : : "r,m"(x) : "memory");

/**
 * @brief Marks a variable as used, even if it wasn't
 */
#define LEGUAN_UNUSED(x) (void)(x)

/**
 * @brief Changes endianess of a integer or floating point
 */
#define LEGUAN_BYTE_SWAP(x) _Generic((x), 				\
								u16: __builtin_bswap16,	\
								u32: __builtin_bswap32,	\
								u64: __builtin_bswap64,	\
								i16: __builtin_bswap16,	\
								i32: __builtin_bswap32,	\
								i64: __builtin_bswap64,	\
								f32: __builtin_bswap32,	\
								f64: __builtin_bswap64	\
							)(x)
/* Functions */

/**
 * @brief Function to tell the compiler that a specific code path is unreachable
 */

LEGUAN_MAYBE_UNUSED
static void LEGUAN_Unreachable() {
	__builtin_unreachable();
}


#if defined(__cplusplus)
}
#endif
