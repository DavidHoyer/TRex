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
  *  @file spi.c
  *  @ingroup peripherals
  *  @author Nikolaij Saegesser
  *  @brief SPI implementations
  */

#include "peripherals/internal/spi.h"
#include "common/intrinsics.h"

#include <stm32h7xx.h>
#include <stm32h7xx_hal.h>


extern SPI_HandleTypeDef hspi4;
extern SPI_HandleTypeDef hspi6;

SPI_Handle_t SPI_Sensor = { &hspi6 };
SPI_Handle_t SPI_Touch  = { &hspi4 };

result_t SPI_Init(SPI_Handle_t handle) {
    LEGUAN_UNUSED(handle);

    return RESULT_SUCCESS;
}


result_t SPI_Read(SPI_Handle_t handle, uint8_t *buffer, size_t size) {
	if (buffer == NULL || size == 0)
		return RESULT_INVALID_ARGUMENTS;

    if (HAL_SPI_Receive(handle.handle, buffer, size, HAL_MAX_DELAY) != HAL_OK)
    	return RESULT_SPI_READ_ERROR;

    return RESULT_SUCCESS;
}

result_t SPI_Write(SPI_Handle_t handle, const uint8_t *buffer, size_t size) {
	if (buffer == NULL || size == 0)
		return RESULT_INVALID_ARGUMENTS;

    if (HAL_SPI_Transmit(handle.handle, (uint8_t*)buffer, size, HAL_MAX_DELAY) != HAL_OK)
    	return RESULT_SPI_WRITE_ERROR;

    return RESULT_SUCCESS;
}

result_t SPI_WriteRead(SPI_Handle_t handle, uint8_t *read_buffer, const uint8_t *write_buffer, size_t size) {
	if (read_buffer == NULL || write_buffer == NULL || size == 0)
		return RESULT_INVALID_ARGUMENTS;

	if (HAL_SPI_TransmitReceive(handle.handle, (uint8_t*)write_buffer, read_buffer, size, HAL_MAX_DELAY) != HAL_OK)
		return RESULT_SPI_READ_ERROR;

	return RESULT_SUCCESS;
}
