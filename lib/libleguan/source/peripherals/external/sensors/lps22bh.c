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
  *  @file lps22bh.c
  *  @ingroup sensors
  *  @author Nikolaij Saegesser
  *  @brief LPS22BH Sensor Implementation
  */

#include "peripherals/external/sensors/lps22bh.h"
#include "peripherals/internal/i2c.h"
#include "common/core.h"

#define DEVICE_ADDRESS		0x5C
#define DEVICE_ID			0xB1
#define REQUEST_RESPONSE	0x80
#define CONVERSION_DONE		0x01

enum Register {
	INTERRUPT_CFG 	= 0x0B,		///< Interrupt register
	THS_P_L			= 0x0C,		///< Pressure threshold registers low
	THS_P_H			= 0x0D,		///< Pressure threshold registers high
	WHO_AM_I		= 0x0F, 	///< Who am I
	CTRL_REG1		= 0x10,		///< Control registers 1
	CTRL_REG2		= 0x11,		///< Control registers 2
	CTRL_REG3		= 0x12,		///< Control registers 3
	FIFO_CTRL		= 0x14,		///< FIFO configuration register
	REF_P_XL		= 0x15,		///< Reference pressure registers bit [0 7]
	REF_P_L			= 0x16,		///< Reference pressure registers bit [8 15]
	REF_P_H			= 0x17,		///< Reference pressure registers bit [16 23]
	RPDS_L			= 0x18,		///< Pressure offset registers low
	RPDS_H			= 0x19,		///< Pressure offset registers high
	RES_CONF		= 0x1A,		///< Resolution register
	INT_SOURCE 		= 0x25, 	///< Interrupt register
	FIFO_STATUS 	= 0x26,		///< FIFO status register
	STATUS			= 0x27,		///< Status register
	PRESS_OUT_XL 	= 0x28,		///< Pressure output registers bit [0 7]
	PRESS_OUT_L 	= 0x29,		///< Pressure output registers bit [8 15]
	PRESS_OUT_H 	= 0x2A,		///< Pressure output registers bit [16 23]
	TEMP_OUT_L 		= 0x2B,		///< Temperature output registers low
	TEMP_OUT_H 		= 0x2C,		///< Temperature output registers high
	LPFP_RES 		= 0x33,		///< Filter reset register
};

typedef struct {
	uint8_t ONE_SHOT 	: 1; 	///< One-shot enable
	uint8_t Reserved0 	: 1; 	///< Write as 0
	uint8_t SWRESET		: 1; 	///< Software reset
	uint8_t I2C_DIS		: 1; 	///< Disable I2C interface
	uint8_t IF_ADD_INC   : 1; 	///< Register address automatically incremented during a multiple byte access
	uint8_t STOP_ON_FTH  : 1; 	///< Stop on FIFO watermark
	uint8_t FIFO_EN		: 1;	///< FIFO enable
	uint8_t BOOT			: 1;	///< Reboot memory content
} ControlRegister2_t;


result_t LPS22BH_Init(void) {
	uint8_t readDeviceId = 0x00;
	do {
		R_TRY(I2C_ReadRegister(DEVICE_ADDRESS, WHO_AM_I | REQUEST_RESPONSE, &readDeviceId));
		if (readDeviceId == DEVICE_ID)
			break;

		CORE_Delay(100);
	} while (true);

	return RESULT_SUCCESS;
}


static result_t StartConversion(void) {
    ControlRegister2_t reg = { .ONE_SHOT = 1 };

	R_TRY(I2C_WriteRegister(DEVICE_ADDRESS, CTRL_REG2, *(uint8_t*)&reg));

	uint8_t conversionDone = 0x00;
	do {
		R_TRY(I2C_ReadRegister(DEVICE_ADDRESS, CTRL_REG2 | REQUEST_RESPONSE, &conversionDone));
		if (conversionDone == 0x00)
			break;

		CORE_Delay(10);
	} while (true);

	return RESULT_SUCCESS;
}


result_t LPS22BH_GetTemperature(float32_t *value) {
    R_TRY(StartConversion());

    if (value == NULL)
    	return RESULT_INVALID_ARGUMENTS;

    uint8_t l, h;

	R_TRY(I2C_ReadRegister(DEVICE_ADDRESS, TEMP_OUT_L, &l));
	R_TRY(I2C_ReadRegister(DEVICE_ADDRESS, TEMP_OUT_H, &h));

	*value = (float32_t)(l | h << 8) / 100.0F;

	return RESULT_SUCCESS;
}

result_t LPS22BH_GetPressure(float32_t *value) {
    R_TRY(StartConversion());

    if (value == NULL)
    	return RESULT_INVALID_ARGUMENTS;

    uint8_t xl, l, h;
	R_TRY(I2C_ReadRegister(DEVICE_ADDRESS, PRESS_OUT_XL, &xl));
	R_TRY(I2C_ReadRegister(DEVICE_ADDRESS, PRESS_OUT_L, &l));
	R_TRY(I2C_ReadRegister(DEVICE_ADDRESS, PRESS_OUT_H, &h));

	*value =  (float32_t)(xl | (l << 8) | (h << 16)) / 4096.0F;

	return RESULT_SUCCESS;
}
