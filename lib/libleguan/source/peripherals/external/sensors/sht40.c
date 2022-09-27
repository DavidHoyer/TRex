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
  *  @file sht40.c
  *  @ingroup sensors
  *  @author Nikolaij Saegesser
  *  @brief SHT40 Sensor Implementation
  */

#include "peripherals/external/sensors/sht40.h"
#include "peripherals/internal/i2c.h"
#include "common/core.h"
#include "common/intrinsics.h"

#define DEVICE_ADDRESS		0x44

typedef enum {
	Precision_High		= 0xFD,
	Precision_Medium	= 0xF6,
	Precision_Low		= 0xE0
} Precision_t;

typedef enum {
	ReadSerial = 0x89,
	SoftReset  = 0x94
} Command_t;

typedef struct {
    float32_t humidity;
    float32_t temperature;
} SensorData_t;

result_t SHT40_Init(void) {
	R_TRY(I2C_Write(DEVICE_ADDRESS, SoftReset));
	CORE_Delay(2);

	return RESULT_SUCCESS;
}


static result_t GetSensorData(SensorData_t *sensorData) {
	if (sensorData == NULL)
		return RESULT_INVALID_ARGUMENTS;

    R_TRY(I2C_Write(DEVICE_ADDRESS, Precision_High));
	CORE_Delay(10);

	struct {
		uint8_t th;
		uint8_t tl;
		uint8_t tcrc8;
		uint8_t rhh;
		uint8_t rhl;
		uint8_t rhcrc8;
	} raw_data = { 0 };

	R_TRY(I2C_ReadBuffer(DEVICE_ADDRESS, (uint8_t*)&raw_data, sizeof(raw_data)));

	sensorData->humidity    = -6  + 125 * (float32_t)((((uint16_t)raw_data.rhh) << 8) | raw_data.rhl) / 0xFFFF;
	sensorData->temperature = -45 + 175 * (float32_t)((((uint16_t)raw_data.th)  << 8) | raw_data.tl)  / 0xFFFF;

	return RESULT_SUCCESS;
}

result_t SHT40_GetTemperature(float32_t *value) {
	SensorData_t sensorData;

	if (value == NULL)
		return RESULT_INVALID_ARGUMENTS;

	R_TRY(GetSensorData(&sensorData));

	*value = sensorData.temperature;

	return RESULT_SUCCESS;
}


result_t SHT40_GetHumidity(float32_t *value) {
	SensorData_t sensorData;

	if (value == NULL)
		return RESULT_INVALID_ARGUMENTS;

	R_TRY(GetSensorData(&sensorData));

	*value = sensorData.humidity;

	return RESULT_SUCCESS;
}
