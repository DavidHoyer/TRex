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
  *  @file tcs34725.c
  *  @ingroup sensors
  *  @author Nikolaij Saegesser
  *  @brief TCS34725FN Sensor Implementation
  */

#include "peripherals/external/sensors/tcs34725.h"
#include "peripherals/internal/i2c.h"
#include "common/core.h"
#include "common/intrinsics.h"
#include "common/color.h"

#define DEVICE_ADDRESS		0x29
#define COMMAND				0x80
#define DEVICE_ID			0x44

typedef struct {
	uint8_t PON 			: 1;	///< Power on
	uint8_t AEN 			: 1;	///< RGBC enable, activates the two channel ADC
	uint8_t reserved1 	    : 1;	///< Write as 0
	uint8_t WEN 			: 1;	///< Wait enable
	uint8_t AIEN 		: 1;	///< RGBC interrupt enable
	uint8_t reserved2 	: 3;	///< Write as 0
} EnableRegister_t;


typedef enum {
	EN			= 0x00,		///< Enable Register
	ATIME  		= 0x01,		///< RGBC Timing Register
	WTIME		= 0x02,		///< Wait Time Register
	AILTL		= 0x04,		///< RGBC clear channel low threshold lower byte
	AILTH		= 0x05,		///< RGBC clear channel low threshold upper byte
	AIHTL		= 0x06,		///< RGBC clear channel high threshold lower byte
	AIHTH		= 0x07,		///< RGBC clear channel high threshold upper byte
	PERS		= 0x0C, 	///< Persistence Register
	CNFG		= 0x0D, 	///< Configuration Register
	CTRL		= 0x0F, 	///< Control Register
	ID			= 0x12,		///< ID Register
	STATUS		= 0x13,		///< Status Register
	CDATA 		= 0x14,		///< Clear data low byte
	CDATAH 		= 0x15,		///< Clear data high byte
	RDATA 		= 0x16, 	///< Red data low byte
	RDATAH 		= 0x17,		///< Red data high byte
	GDATA 		= 0x18,		///< Green data low byte
	GDATAH		= 0x19,		///< Green data high byte
	BDATA 		= 0x1A,		///< Blue data low byte
	BDATAH 		= 0x1B, 	///< Blue data high byte
} Register_t;

typedef enum {
	_2_4ms 	= 0xFF, ///<  2.4ms - 1 cycle
	_10ms	= 0xFB, ///<  10ms	- 4 cycles
	_24ms 	= 0xF6, ///<  24ms  - 10 cycles
	_50ms 	= 0xEB, ///<  50ms  - 20 cycles
	_101ms	= 0xD5,	///<  101ms - 42 cycles
	_154ms 	= 0xC0, ///<  154ms - 64 cycles
	_700ms 	= 0x00 	///<  700ms - 256 cycles
} IntegrationTime_t;

typedef enum {
  _1x 	= 0x00, ///<  No gain
  _4x 	= 0x01, ///<  4x gain
  _16x 	= 0x02, ///<  16x gain
  _60x 	= 0x03  ///<  60x gain
} Gain_t;


static result_t SetIntegrationTime(IntegrationTime_t integrationTime) {
	R_TRY(I2C_WriteRegister(DEVICE_ADDRESS, ATIME | COMMAND, integrationTime));

	return RESULT_SUCCESS;
}

static result_t SetGain(Gain_t gain) {
    R_TRY(I2C_WriteRegister(DEVICE_ADDRESS, CTRL | COMMAND, gain));

    return RESULT_SUCCESS;
}

static result_t Enable(void) {
	EnableRegister_t reg = { .PON = true };
	R_TRY(I2C_WriteRegister(DEVICE_ADDRESS, EN | COMMAND, *(uint8_t*)&reg));

	CORE_Delay(3);

	return RESULT_SUCCESS;
}

LEGUAN_MAYBE_UNUSED
static result_t Disable(void) {
    EnableRegister_t reg = { .PON = false };
	R_TRY(I2C_WriteRegister(DEVICE_ADDRESS, EN | COMMAND, *(uint8_t*)&reg));

	CORE_Delay(3);

	return RESULT_SUCCESS;
}

static result_t StartConversion(void) {
    EnableRegister_t reg = { .PON = true, .AEN = true };
	R_TRY(I2C_WriteRegister(DEVICE_ADDRESS, EN | COMMAND, *(uint8_t*)&reg));

	CORE_Delay(3);

	return RESULT_SUCCESS;
}

static bool IsDone(void) {
	uint8_t status = 0x00;

	if (I2C_ReadRegister(DEVICE_ADDRESS, STATUS | COMMAND, &status) != RESULT_SUCCESS)
		return false;

	return status & 0x01;
}

result_t TC3A227E_Init(void) {
	R_TRY(SetIntegrationTime(_24ms));
	R_TRY(SetGain(_60x));
	R_TRY(Enable());
	R_TRY(StartConversion());

	return RESULT_SUCCESS;
}


result_t TC3A227E_GetColor(color_t *value) {
	while (!IsDone())
		CORE_Delay(1);

	if (value == NULL)
		return RESULT_INVALID_ARGUMENTS;

	uint8_t l, h;

	R_TRY(I2C_ReadRegister(DEVICE_ADDRESS, RDATA  | COMMAND, &l));
	R_TRY(I2C_ReadRegister(DEVICE_ADDRESS, RDATAH | COMMAND, &h));

	value->r = (h << 8) | l;

	R_TRY(I2C_ReadRegister(DEVICE_ADDRESS, GDATA  | COMMAND, &l));
	R_TRY(I2C_ReadRegister(DEVICE_ADDRESS, GDATAH | COMMAND, &h));

	value->g = (h << 8) | l;

	R_TRY(I2C_ReadRegister(DEVICE_ADDRESS, BDATA  | COMMAND, &l));
	R_TRY(I2C_ReadRegister(DEVICE_ADDRESS, BDATAH | COMMAND, &h));

	value->b = (h << 8) | l;

	R_TRY(I2C_ReadRegister(DEVICE_ADDRESS, CDATA  | COMMAND, &l));
	R_TRY(I2C_ReadRegister(DEVICE_ADDRESS, CDATAH | COMMAND, &h));

	value->a = (h << 8) | l;

	R_TRY(StartConversion());

	return RESULT_SUCCESS;
}
