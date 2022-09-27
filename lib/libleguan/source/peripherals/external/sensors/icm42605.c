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
  *  @file icm42605.c
  *  @ingroup sensors
  *  @author Nikolaij Saegesser
  *  @brief ICM-42605 Sensor Implementation
  */

#include "peripherals/external/sensors/icm42605.h"
#include "peripherals/internal/gpio.h"
#include "peripherals/internal/spi.h"
#include "common/core.h"

#define DEVICE_ID			0x42
#define REQUEST_RESPONSE	0x80

const static GPIO_Handle_t SensorChipSelect = { GPIOB, 2, true };

static ICM42605_AccelFullScaleRange s_accelScaleRange;
static ICM42605_GyroFullScaleRange s_gyroScaleRange;

/**
 * @brief Register bank 0
 */
typedef enum {
	DEVICE_CONFIG      = 0x11,
	DRIVE_CONFIG       = 0x13,
	INT_CONFIG         = 0x14,
	FIFO_CONFIG        = 0x16,
	TEMP_DATA1         = 0x1D,
	TEMP_DATA0         = 0x1E,
	ACCEL_DATA_X1      = 0x1F,
	ACCEL_DATA_X0      = 0x20,
	ACCEL_DATA_Y1      = 0x21,
	ACCEL_DATA_Y0      = 0x22,
	ACCEL_DATA_Z1      = 0x23,
	ACCEL_DATA_Z0      = 0x24,
	GYRO_DATA_X1       = 0x25,
	GYRO_DATA_X0       = 0x26,
	GYRO_DATA_Y1       = 0x27,
	GYRO_DATA_Y0       = 0x28,
	GYRO_DATA_Z1       = 0x29,
	GYRO_DATA_Z0       = 0x2A,
	TMST_FSYNCH        = 0x2B,
	TMST_FSYNCL        = 0x2C,
	INT_STATUS         = 0x2D,
	FIFO_COUNTH        = 0x2E,
	FIFO_COUNTL        = 0x2F,
	FIFO_DATA          = 0x30,
	APEX_DATA0         = 0x31,
	APEX_DATA1         = 0x32,
	APEX_DATA2         = 0x33,
	APEX_DATA3         = 0x34,
	APEX_DATA4         = 0x35,
	APEX_DATA5         = 0x36,
	INT_STATUS2        = 0x37,
	INT_STATUS3        = 0x38,
	SIGNAL_PATH_RESET  = 0x4B,
	INTF_CONFIG0       = 0x4C,
	INTF_CONFIG1       = 0x4D,
	PWR_MGMT0          = 0x4E,
	GYRO_CONFIG0       = 0x4F,
	ACCEL_CONFIG0      = 0x50,
	GYRO_CONFIG1       = 0x51,
	GYRO_ACCEL_CONFIG0 = 0x52,
	ACCEL_CONFIG1      = 0x53,
	TMST_CONFIG        = 0x54,
	APEX_CONFIG0       = 0x56,
	SMD_CONFIG         = 0x57,
	FIFO_CONFIG1       = 0x5F,
	FIFO_CONFIG2       = 0x60,
	FIFO_CONFIG3       = 0x61,
	FSYNC_CONFIG       = 0x62,
	INT_CONFIG0        = 0x63,
	INT_CONFIG1        = 0x64,
	INT_SOURCE0        = 0x65,
	INT_SOURCE1        = 0x66,
	INT_SOURCE3        = 0x68,
	INT_SOURCE4        = 0x69,
	FIFO_LOST_PKT0     = 0x6C,
	FIFO_LOST_PKT1     = 0x6D,
	SELF_TEST_CONFIG   = 0x70,
	WHO_AM_I           = 0x75,
	REG_BANK_SEL       = 0x76
} RegisterBank0;

/**
 * @brief Register bank 1
 */
typedef enum {
	SENSOR_CONFIG0       = 0x03,
	GYRO_CONFIG_STATIC2  = 0x0B,
	GYRO_CONFIG_STATIC3  = 0x0C,
	GYRO_CONFIG_STATIC4  = 0x0D,
	GYRO_CONFIG_STATIC5  = 0x0E,
	GYRO_CONFIG_STATIC6  = 0x0F,
	GYRO_CONFIG_STATIC7  = 0x10,
	GYRO_CONFIG_STATIC8  = 0x11,
	GYRO_CONFIG_STATIC9  = 0x12,
	GYRO_CONFIG_STATIC10 = 0x13,
	XG_ST_DATA           = 0x5F,
	YG_ST_DATA           = 0x60,
	ZG_ST_DATA           = 0x61,
	TMSTVAL0             = 0x62,
	TMSTVAL1             = 0x63,
	TMSTVAL2             = 0x64,
	INTF_CONFIG4         = 0x7A,
	INTF_CONFIG5         = 0x7B,
	INTF_CONFIG6         = 0x7C
} RegisterBank1;

/**
 * @brief Register bank 2
 */
typedef enum {
	ACCEL_CONFIG_STATIC2 = 0x03,
	ACCEL_CONFIG_STATIC3 = 0x04,
	ACCEL_CONFIG_STATIC4 = 0x05,
	XA_ST_DATA           = 0x3B,
	YA_ST_DATA           = 0x3C,
	ZA_ST_DATA           = 0x3D
} RegisterBank2;

/**
 * @brief Register bank 4
 */
typedef enum {
	GYRO_ON_OFF_CONFIG = 0x0E,
	APEX_CONFIG1       = 0x40,
	APEX_CONFIG2       = 0x41,
	APEX_CONFIG3       = 0x42,
	APEX_CONFIG4       = 0x43,
	APEX_CONFIG5       = 0x44,
	APEX_CONFIG6       = 0x45,
	APEX_CONFIG7       = 0x46,
	APEX_CONFIG8       = 0x47,
	APEX_CONFIG9       = 0x48,
	ACCEL_WOM_X_THR    = 0x4A,
	ACCEL_WOM_Y_THR    = 0x4B,
	ACCEL_WOM_Z_THR    = 0x4C,
	INT_SOURCE6        = 0x4D,
	INT_SOURCE7        = 0x4E,
	INT_SOURCE8        = 0x4F,
	INT_SOURCE9        = 0x50,
	INT_SOURCE10       = 0x51,
	OFFSET_USER0       = 0x77,
	OFFSET_USER1       = 0x78,
	OFFSET_USER2       = 0x79,
	OFFSET_USER3       = 0x7A,
	OFFSET_USER4       = 0x7B,
	OFFSET_USER5       = 0x7C,
	OFFSET_USER6       = 0x7D,
	OFFSET_USER7       = 0x7E,
	OFFSET_USER8       = 0x7F
} RegisterBank4;


static result_t write(const uint8_t *buffer, size_t size) {
	GPIO_SetValue(SensorChipSelect, true);
	CORE_Delay(10);
	result_t result = SPI_Write(SPI_Sensor, buffer, size);
	CORE_Delay(10);
	GPIO_SetValue(SensorChipSelect, false);

	return result;
}

static result_t writeRead(const uint8_t *sendBuffer, uint8_t *readBuffer, size_t size) {
	GPIO_SetValue(SensorChipSelect, true);
	CORE_Delay(10);
	result_t result = SPI_WriteRead(SPI_Sensor, readBuffer, sendBuffer, size);
	CORE_Delay(10);
	GPIO_SetValue(SensorChipSelect, false);

	return result;
}

result_t ICM42605_Init(ICM42605_AccelFullScaleRange accelScaleRange, ICM42605_GyroFullScaleRange gyroScaleRange, ICM42605_AccelOutputDataRange accelOutputDataRange, ICM42605_GyroOutputDataRange gyroOutputDataRange) {

	s_accelScaleRange = accelScaleRange;
	s_gyroScaleRange = gyroScaleRange;

	{
		uint8_t writeBuffer[2] = { WHO_AM_I | REQUEST_RESPONSE, 0x00 };
		uint8_t readBuffer[2] = { 0x00 };
		R_TRY(writeRead(writeBuffer, readBuffer, 2));

		if (readBuffer[1] != DEVICE_ID)
			return RESULT_INITIALIZATION_ERROR;
	}

	// Enable gyroscope and accelerometer in low noise mode
	{
		uint8_t pwrMgmt0 = 0;
		{
			uint8_t writeBuffer[2] = { PWR_MGMT0 | REQUEST_RESPONSE, 0x00 };
			uint8_t readBuffer[2] = { 0x00 };
			R_TRY(writeRead(writeBuffer, readBuffer, 2));
			pwrMgmt0 = readBuffer[1];
		}

		{
			uint8_t writeBuffer[2] = { PWR_MGMT0, pwrMgmt0 | 0x0F };
			R_TRY(write(writeBuffer, 2));
		}
	}

	CORE_Delay(100);

	// Set gyro scale and data rate
	{
		uint8_t gyroConfig0 = 0;
		{
			uint8_t writeBuffer[2] = { GYRO_CONFIG0 | REQUEST_RESPONSE, 0x00 };
			uint8_t readBuffer[2] = { 0x00 };
			R_TRY(writeRead(writeBuffer, readBuffer, 2));
			gyroConfig0 = readBuffer[1];
		}

		{
			uint8_t writeBuffer[2] = { GYRO_CONFIG0, gyroConfig0 | gyroOutputDataRange | (gyroScaleRange << 5) };
			R_TRY(write(writeBuffer, 2));
		}
	}

	// Set accel scale and data rate
	{
		uint8_t accelConfig0 = 0;
		{
			uint8_t writeBuffer[2] = { ACCEL_CONFIG0 | REQUEST_RESPONSE, 0x00 };
			uint8_t readBuffer[2] = { 0x00 };
			R_TRY(writeRead(writeBuffer, readBuffer, 2));
			accelConfig0 = readBuffer[1];
		}

		{
			uint8_t writeBuffer[2] = { ACCEL_CONFIG0, accelConfig0 | accelOutputDataRange | (accelScaleRange << 5) };
			R_TRY(write(writeBuffer, 2));
		}
	}

	// Set temperature low pass filter to a minimum and use first order filter for gyro
	{
		uint8_t gyroConfig1  = 0;
		{
			uint8_t writeBuffer[2] = { GYRO_CONFIG1 | REQUEST_RESPONSE, 0x00 };
			uint8_t readBuffer[2] = { 0x00 };
			R_TRY(writeRead(writeBuffer, readBuffer, 2));
			gyroConfig1  = readBuffer[1];
		}

		{
			uint8_t writeBuffer[2] = { GYRO_CONFIG1, gyroConfig1 | 0x0D };
			R_TRY(write(writeBuffer, 2));
		}
	}

	// Setup interrupt pins
	{
		uint8_t intConfig0  = 0;
		{
			uint8_t writeBuffer[2] = { INT_CONFIG | REQUEST_RESPONSE, 0x00 };
			uint8_t readBuffer[2] = { 0x00 };
			R_TRY(writeRead(writeBuffer, readBuffer, 2));
			intConfig0 = readBuffer[1];
		}

		{
			uint8_t writeBuffer[2] = { INT_CONFIG, intConfig0 | 0x18 | 0x03 };
			R_TRY(write(writeBuffer, 2));
		}
	}

	// Enable async interrupt reset
	{
		uint8_t intConfig1 = 0;
		{
			uint8_t writeBuffer[2] = { INT_CONFIG1 | REQUEST_RESPONSE, 0x00 };
			uint8_t readBuffer[2] = { 0x00 };
			R_TRY(writeRead(writeBuffer, readBuffer, 2));
			intConfig1 = readBuffer[1];
		}

		{
			uint8_t writeBuffer[2] = { INT_CONFIG1, intConfig1 & ~0x10 };
			R_TRY(write(writeBuffer, 2));
		}
	}

	// Route data ready interrupt to INT1
	{
		uint8_t intSource0 = 0;
		{
			uint8_t writeBuffer[2] = { INT_SOURCE0 | REQUEST_RESPONSE, 0x00 };
			uint8_t readBuffer[2] = { 0x00 };
			R_TRY(writeRead(writeBuffer, readBuffer, 2));
			intSource0 = readBuffer[1];
		}

		{
			uint8_t writeBuffer[2] = { INT_SOURCE0, intSource0 | 0x08 };
			R_TRY(write(writeBuffer, 2));
		}
	}

	// Route AGC interrupt to INT2
	{
		uint8_t intSource3 = 0;
		{
			uint8_t writeBuffer[2] = { INT_SOURCE3 | REQUEST_RESPONSE, 0x00 };
			uint8_t readBuffer[2] = { 0x00 };
			R_TRY(writeRead(writeBuffer, readBuffer, 2));
			intSource3 = readBuffer[1];
		}

		{
			uint8_t writeBuffer[2] = { INT_SOURCE3, intSource3 | 0x01 };
			R_TRY(write(writeBuffer, 2));
		}
	}

	return RESULT_SUCCESS;
}

result_t ICM42605_GetRotation(vector3f_t *value) {
	if (value == NULL)
		return RESULT_INVALID_ARGUMENTS;

	uint8_t writeBuffer[7] = { GYRO_DATA_X1 | REQUEST_RESPONSE, 0x00 };
	uint8_t readBuffer[7] = { 0x00 };

	R_TRY(writeRead(writeBuffer, readBuffer, 7));

	value->x = ((float32_t)((int16_t)(readBuffer[2] | readBuffer[1] << 8)) / 0xFFFF) * (2000 / (s_gyroScaleRange + 1));
	value->y = ((float32_t)((int16_t)(readBuffer[4] | readBuffer[3] << 8)) / 0xFFFF) * (2000 / (s_gyroScaleRange + 1));
	value->z = ((float32_t)((int16_t)(readBuffer[6] | readBuffer[5] << 8)) / 0xFFFF) * (2000 / (s_gyroScaleRange + 1));

	return RESULT_SUCCESS;
}

result_t ICM42605_GetAcceleration(vector3f_t *value) {
	if (value == NULL)
		return RESULT_INVALID_ARGUMENTS;

	uint8_t writeBuffer[7] = { ACCEL_DATA_X1 | REQUEST_RESPONSE, 0x00 };
	uint8_t readBuffer[7] = { 0x00 };

	R_TRY(writeRead(writeBuffer, readBuffer, 7));

	value->x = ((float32_t)((int16_t)(readBuffer[2] | readBuffer[1] << 8)) / 0xFFFF) * (16 / (s_accelScaleRange + 1));
	value->y = ((float32_t)((int16_t)(readBuffer[4] | readBuffer[3] << 8)) / 0xFFFF) * (16 / (s_accelScaleRange + 1));
	value->z = ((float32_t)((int16_t)(readBuffer[6] | readBuffer[5] << 8)) / 0xFFFF) * (16 / (s_accelScaleRange + 1));

	return RESULT_SUCCESS;
}
