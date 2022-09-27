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
  *  @file icm42605.h
  *  @ingroup sensors
  *  @author Nikolaij Saegesser
  *  @brief ICM-42605 Sensor Abstraction
  */

#pragma once

#if defined(__cplusplus)
extern "C" {
#endif


/* Includes */
#include "common/types.h"
#include "common/attributes.h"
#include "common/mmio.h"

/* Types */

/**
 * @brief Accelerometer range
 */
typedef enum {
	ICM42605_AccelFullScaleRange_2G = 0x03,		///< Range from -2G to 2G with highest precision
	ICM42605_AccelFullScaleRange_4G = 0x02,		///< Range from -4G to 4G
	ICM42605_AccelFullScaleRange_8G = 0x01,		///< Range from -8G to 8G
	ICM42605_AccelFullScaleRange_16G = 0x00		///< Range from -16G to 16G with lowest precision
} ICM42605_AccelFullScaleRange;

/**
 * @brief Gyroscope range
 * @note DPS = degree per second
 */
typedef enum {
	ICM42605_GyroFullScaleRange_2000DPS   = 0x00,	///< Range from -2000DPS to 2000DPS with lowest precision
	ICM42605_GyroFullScaleRange_1000DPS   = 0x01,	///< Range from -1000DPS to 1000DPS
	ICM42605_GyroFullScaleRange_500DPS    = 0x02,	///< Range from -500DPS to 500DPS
	ICM42605_GyroFullScaleRange_250DPS    = 0x03,	///< Range from -250DPS to 250DPS
	ICM42605_GyroFullScaleRange_125DPS    = 0x04,	///< Range from -125DPS to 125DPS
	ICM42605_GyroFullScaleRange_62_5DPS   = 0x05,	///< Range from -62.5DPS to 65.5DPS
	ICM42605_GyroFullScaleRange_31_25DPS  = 0x06,	///< Range from -31.25DPS to 31.25DPS
	ICM42605_GyroFullScaleRange_15_125DPS = 0x07	///< Range from -15.125DPS to 15.125DPS with highest precision
} ICM42605_GyroFullScaleRange;

/**
 * @brief Accelerometer data rate
 */
typedef enum {
	ICM42605_AccelOutputDataRange_8000Hz   = 0x03,	///< 8000 measurements per second
	ICM42605_AccelOutputDataRange_4000Hz   = 0x04,	///< 4000 measurements per second
	ICM42605_AccelOutputDataRange_2000Hz   = 0x05,	///< 2000 measurements per second
	ICM42605_AccelOutputDataRange_1000Hz   = 0x06,	///< 1000 measurements per second
	ICM42605_AccelOutputDataRange_500Hz    = 0x0F,	///< 500 measurements per second
	ICM42605_AccelOutputDataRange_200Hz    = 0x07,	///< 200 measurements per second
	ICM42605_AccelOutputDataRange_100Hz    = 0x08,	///< 100 measurements per second
	ICM42605_AccelOutputDataRange_50Hz     = 0x09,	///< 50 measurements per second
	ICM42605_AccelOutputDataRange_25Hz     = 0x0A,	///< 25 measurements per second
	ICM42605_AccelOutputDataRange_12_5Hz   = 0x0B,	///< 12.5 measurements per second
	ICM42605_AccelOutputDataRange_6_25Hz   = 0x0C,	///< 6.25 measurements per second
	ICM42605_AccelOutputDataRange_3_125Hz  = 0x0D,	///< 3.125 measurements per second
	ICM42605_AccelOutputDataRange_1_5625Hz = 0x0E	///< 1.5625 measurements per second
} ICM42605_AccelOutputDataRange;

/**
 * @brief Gyroscope data rate
 */
typedef enum {
	ICM42605_GyroOutputDataRange_8000Hz   = 0x03,	///< 8000 measurements per second
	ICM42605_GyroOutputDataRange_4000Hz   = 0x04,	///< 4000 measurements per second
	ICM42605_GyroOutputDataRange_2000Hz   = 0x05,	///< 2000 measurements per second
	ICM42605_GyroOutputDataRange_1000Hz   = 0x06,	///< 1000 measurements per second
	ICM42605_GyroOutputDataRange_500Hz    = 0x0F,	///< 500 measurements per second
	ICM42605_GyroOutputDataRange_200Hz    = 0x07,	///< 200 measurements per second
	ICM42605_GyroOutputDataRange_100Hz    = 0x08,	///< 100 measurements per second
	ICM42605_GyroOutputDataRange_50Hz     = 0x09,	///< 50 measurements per second
	ICM42605_GyroOutputDataRange_25Hz     = 0x0A,	///< 25 measurements per second
	ICM42605_GyroOutputDataRange_12_5Hz   = 0x0B,	///< 12.5 measurements per second
} ICM42605_GyroOutputDataRange;

/* Functions */

/**
 * @brief Initializes the ICM42605 6-Axis Sensor
 * @return Status result
 */
result_t ICM42605_Init(ICM42605_AccelFullScaleRange accelScaleRange, ICM42605_GyroFullScaleRange gyroScaleRange, ICM42605_AccelOutputDataRange accelOutputDataRange, ICM42605_GyroOutputDataRange gyroOutputDataRange);

/**
 * @brief Gets the board rotation
 * @param Pointer to where rotation should be stored
 * @return Status result
 */
result_t ICM42605_GetRotation(vector3f_t *value);

/**
 * @brief Gets the board acceleration
 * @param Pointer to where acceleration should be stored
 * @return Status result
 */
result_t ICM42605_GetAcceleration(vector3f_t *value);


#if defined(__cplusplus)
}
#endif
