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
  *  @file i2c.c
  *  @ingroup peripherals
  *  @author Nikolaij Saegesser
  *  @brief I2C implementations
  */

#include "peripherals/internal/i2c.h"
#include "common/processor.h"


extern I2C_HandleTypeDef hi2c2;

result_t I2C_Init(void) {
	/* Noting to do */

	return RESULT_SUCCESS;
}

result_t I2C_Write(uint8_t address, uint8_t data) {
	if (HAL_I2C_Master_Transmit(&hi2c2, address << 1, &data, 1, HAL_MAX_DELAY) != HAL_OK)
		return RESULT_I2C_WRITE_ERROR;

	return RESULT_SUCCESS;
}

result_t I2C_WriteRegister(uint8_t address, uint8_t reg, uint8_t data) {
	uint8_t transmission[] = { reg, data };
	if (HAL_I2C_Master_Transmit(&hi2c2, address << 1, transmission, 2, HAL_MAX_DELAY) != HAL_OK)
		return RESULT_I2C_WRITE_ERROR;

	return RESULT_SUCCESS;
}

result_t I2C_Read(uint8_t address, uint8_t *value) {
	if (value == NULL)
		return RESULT_INVALID_ARGUMENTS;

	if (HAL_I2C_Master_Receive(&hi2c2, address << 1, value, 1, HAL_MAX_DELAY) != HAL_OK)
		return RESULT_I2C_READ_ERROR;

	return RESULT_SUCCESS;
}

result_t I2C_ReadBuffer(uint8_t address, uint8_t *buffer, size_t size) {
	if (HAL_I2C_Master_Receive(&hi2c2, address << 1, buffer, size, HAL_MAX_DELAY) != HAL_OK)
		return RESULT_I2C_READ_ERROR;

	return RESULT_SUCCESS;
}

result_t I2C_ReadRegister(uint8_t address, uint8_t reg, uint8_t *value) {
	if (value == NULL)
		return RESULT_INVALID_ARGUMENTS;

	if (HAL_I2C_Master_Transmit(&hi2c2, address << 1, &reg, 1, HAL_MAX_DELAY) != HAL_OK)
		return RESULT_I2C_READ_ERROR;

	if (I2C_Read(address, value) != RESULT_SUCCESS)
		return RESULT_I2C_READ_ERROR;

	return RESULT_SUCCESS;
}

result_t I2C_ReadRegisterBuffer(uint8_t address, uint8_t reg, uint8_t *buffer, size_t size) {
	if (buffer == NULL)
		return RESULT_INVALID_ARGUMENTS;

	if (HAL_I2C_Master_Transmit(&hi2c2, address << 1, &reg, 1, HAL_MAX_DELAY) != HAL_OK)
		return RESULT_I2C_READ_ERROR;

	if (I2C_ReadBuffer(address, buffer, size) != RESULT_SUCCESS)
		return RESULT_I2C_READ_ERROR;

	return RESULT_SUCCESS;
}
