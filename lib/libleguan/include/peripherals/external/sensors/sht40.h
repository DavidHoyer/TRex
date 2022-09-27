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
  *  @file sht40.h
  *  @ingroup sensors
  *  @author Nikolaij Saegesser
  *  @brief SHT40 Sensor Abstraction
  */

#pragma once

#if defined(__cplusplus)
extern "C" {
#endif


/* Includes */
#include "common/types.h"
#include "common/attributes.h"
#include "common/mmio.h"

/* Functions */

/**
 * @brief Initializes the SHT40 temperature and humidity sensor
 * @return Status result
 */
result_t SHT40_Init(void);

/**
 * @brief Gets the current temparature
 * @param Pointer to where temperature in °C is stored
 * @return Status result
 */
result_t SHT40_GetTemperature(float32_t *value);

/**
 * @brief Gets the current relative humidity
 * @param Pointer to where humidity in %R is stored
 * @return Status result
 */
result_t SHT40_GetHumidity(float32_t *value);


#if defined(__cplusplus)
}
#endif
