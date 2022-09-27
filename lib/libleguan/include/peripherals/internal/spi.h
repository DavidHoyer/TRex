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
  *  @file spi.h
  *  @ingroup peripherals
  *  @author Nikolaij Saegesser
  *  @brief SPI Abstraction
  */

#pragma once

#if defined(__cplusplus)
extern "C" {
#endif


/* Includes */
#include "common/types.h"
#include "common/attributes.h"
#include "common/mmio.h"

#include <stm32h7xx_hal.h>

/* Types */

typedef struct {
    SPI_HandleTypeDef *handle;
} SPI_Handle_t;


/* Constants */

extern SPI_Handle_t SPI_Sensor;
extern SPI_Handle_t SPI_Touch;

/* Functions */

/**
 * @brief Initializes the SPI interface
 * @return Status result
 */
result_t SPI_Init(SPI_Handle_t handle);


/**
 * @brief Reads data from the SPI interface
 * @param buffer Buffer to store received data in
 * @param size Buffer size
 * @return Status result
 */
result_t SPI_Read(SPI_Handle_t handle, uint8_t *buffer, size_t size);

/**
 * @brief Writes data to the SPI interface
 * @param buffer Buffer to send data from
 * @param size Buffer size
 * @return Status result
 */
result_t SPI_Write(SPI_Handle_t handle, const uint8_t *buffer, size_t size);

/**
 * @brief Writes and reads data simultaneously to/from the SPI interface
 * @param read_buffer Buffer to read data into
 * @param write_buffer Buffer to send data from
 * @param size Buffer size
 * @return Status result
 */
result_t SPI_WriteRead(SPI_Handle_t handle, uint8_t *read_buffer, const uint8_t *write_buffer, size_t size);


#if defined(__cplusplus)
}
#endif
