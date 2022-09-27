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
  *  @file leguan.h
  *  @ingroup leguan
  *  @author Nikolaij Saegesser
  *  @brief Main header
  */

#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

/* Includes */
#include "common/core.h"
#include "common/types.h"
#include "common/errors.h"
#include "common/attributes.h"
#include "common/intrinsics.h"
#include "common/mmio.h"
#include "common/logger.h"

#include "peripherals/internal/adc.h"
#include "peripherals/internal/dac.h"
#include "peripherals/internal/gpio.h"
#include "peripherals/internal/serial.h"
#include "peripherals/internal/i2c.h"
#include "peripherals/internal/spi.h"
#include "peripherals/internal/ram.h"

#include "peripherals/external/fpga.h"
#include "peripherals/external/lcd.h"
#include "peripherals/external/sensors.h"

/* Macros */

/**
 * @brief Define to signalize that libleguan is included
 */
#define LIBLEGUAN

/**
 * @brief libleguan version number
 */

#define LIBLEGUAN_VERSION_MAJOR  1
#define LIBLEGUAN_VERSION_MINOR  0
#define LIBLEGUAN_VERSION_PATCH  0
//#define LIBLEGUAN_VERSION_FLAVOR RC1

#if defined(LIBLEGUAN_VERSION_FLAVOR)
	#define LIBLEGUAN_VERSION_STRING #LIBLEGUAN_VERSION_MAJOR "." #LIBLEGUAN_VERSION_MINOR "." #LIBLEGUAN_VERSION_PATCH "-" #LIBLEGUAN_VERSION_FLAVOR
#else
	#define LIBLEGUAN_VERSION_STRING #LIBLEGUAN_VERSION_MAJOR "." #LIBLEGUAN_VERSION_MINOR "." #LIBLEGUAN_VERSION_PATCH
#endif


/* Functions */

/**
 * @brief Leguan platform initialization function
 * @note This needs to be called before making any further libleguan calls
 */
result_t LEGUAN_Init(void);


#if defined(__cplusplus)
}
#endif
