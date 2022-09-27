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
  *  @file sensors.c
  *  @ingroup peripherals
  *  @author Nikolaij Saegesser
  *  @brief Common sensor interface implementation
  */

#include "peripherals/external/sensors/lps22bh.h"
#include "peripherals/external/sensors/sht40.h"
#include "peripherals/external/sensors/tcs34725.h"
#include "peripherals/external/sensors/icm42605.h"
#include "peripherals/external/sensors.h"

result_t SENSOR_Init(void) {
	R_TRY(LPS22BH_Init());
	R_TRY(SHT40_Init());
	R_TRY(TC3A227E_Init());
  R_TRY(ICM42605_Init(ICM42605_AccelFullScaleRange_2G, ICM42605_GyroFullScaleRange_250DPS, ICM42605_AccelOutputDataRange_1000Hz, ICM42605_GyroOutputDataRange_1000Hz));

	return RESULT_SUCCESS;
}


result_t SENSOR_GetTemperature(float32_t *value) {
	return LPS22BH_GetTemperature(value);
}

result_t SENSOR_GetPressure(float32_t *value) {
	return LPS22BH_GetPressure(value);
}

result_t SENSOR_GetHumidity(float32_t *value) {
	return SHT40_GetHumidity(value);
}

result_t SENSOR_GetColor(color_t *value) {
	return TC3A227E_GetColor(value);
}

result_t SENSOR_GetAcceleration(vector3f_t *value) {
	return ICM42605_GetAcceleration(value);
}

result_t SENSOR_GetRotation(vector3f_t *value) {
	return ICM42605_GetRotation(value);
}
