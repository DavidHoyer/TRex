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
  *  @file sensors.h
  *  @ingroup peripherals
  *  @author Nikolaij Saegesser
  *  @brief Common sensor interface
  */

#pragma once

#if defined(__cplusplus)
extern "C" {
#endif


/* Includes */
#include "common/types.h"
#include "common/attributes.h"
#include "common/mmio.h"
#include "common/color.h"

#include "peripherals/external/sensors/lps22bh.h"
#include "peripherals/external/sensors/sht40.h"
#include "peripherals/external/sensors/tcs34725.h"
#include "peripherals/external/sensors/icm42605.h"

/* Functions */

/**
 * @brief Initializes all sensors
 * @return Status result
 */
result_t SENSOR_Init(void);


/**
 * @brief Gets the current ambient temperature
 * @param Pointer where to store ambient temperature in °C at
 * @return Status result
 */
result_t SENSOR_GetTemperature(float32_t *value);

/**
 * @brief Gets the current atmospheric pressure
 * @return Pointer where to store atmospheric pressure in hPa at
 * @return Status result
 */
result_t SENSOR_GetPressure(float32_t *value);

/**
 * @brief Gets the current relative humidity
 * @return Pointer where to store relative humidity in %R at
 * @return Status result
 */
result_t SENSOR_GetHumidity(float32_t *value);

/**
 * @brief Gets the current color as seen by the color sensor
 * @return Color sensed
 * @return Status result
 */
result_t SENSOR_GetColor(color_t *color);

/**
 * @brief Gets the board acceleration
 * @return Acceleration vector
 * @return Status result
 */
result_t SENSOR_GetAcceleration(vector3f_t *value);

/**
 * @brief Gets the current board rotation
 * @return Rotation vector
 * @return Status result
 */
result_t SENSOR_GetRotation(vector3f_t *value);


#if defined(__cplusplus)
}
#endif
