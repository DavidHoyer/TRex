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
  *  @file lps22bh.h
  *  @ingroup sensors
  *  @author Nikolaij Saegesser
  *  @brief LPS22BH Sensor Abstraction
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
 * @brief Initializes the LPS22BH temperature and pressure sensor
 * @return Status result
 */
result_t LPS22BH_Init(void);

/**
 * @brief Gets the current temparature
 * @param Pointer to where temperature in °C is stored
 * @return Status result
 */
result_t LPS22BH_GetTemperature(float32_t *value);

/**
 * @brief Gets the current atmospheric pressure
 * @return Pointer to where pressure in hPa is stored
 * @return Status result
 */
result_t LPS22BH_GetPressure(float32_t *value);


#if defined(__cplusplus)
}
#endif
