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
  *  @file attributes.h
  *  @ingroup common
  *  @author Nikolaij Saegesser
  *  @brief Compiler attributes
  */

#pragma once

#if defined(__cplusplus)
extern "C" {
#endif


#define LEGUAN_LIKELY                  __attribute__((likely))
#define LEGUAN_UNLIKELY                __attribute__((unlikely))
#define LEGUAN_DEPRECTATED(reason)     __attribute__((deprecated))
#define LEGUAN_NO_RETURN               __attribute__((noreturn))
#define LEGUAN_MAYBE_UNUSED			   __attribute__((unused))

#define LEGUAN_WEAK                    __attribute__((weak))
#define LEGUAN_ALWAYS_INLINE           __attribute__((always_inline)) inline
#define LEGUAN_PACKED                  __attribute__((packed))
#define LEGUAN_NAKED                   __attribute__((naked))
#define LEGUAN_SECTION(name)           __attribute__((section(name)))
#define LEGUAN_ALIGNED(alignment)      __attribute__((aligned(alignment)))

#if defined(__cplusplus)
    #define LEGUAN_C_LINKAGE extern "C"
#else
    #define LEGUAN_C_LINKAGE
#endif


#if defined(__cplusplus)
}
#endif
