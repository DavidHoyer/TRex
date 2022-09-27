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
  *  @file serial.h
  *  @ingroup peripherals
  *  @author Nikolaij Saegesser
  *  @brief Serial UART Abstraction
  */

#pragma once

#if defined(__cplusplus)
extern "C" {
#endif


/* Includes */
#include "common/types.h"
#include "common/attributes.h"
#include "common/mmio.h"

/* Functions */

/**
 * @brief Initializes the UART interface
 * @return Status result
 */
result_t SERIAL_Init(void);

/**
 * @brief Serial Stream function for use with the logger
 * @param string String to print
 * @param new_line Advance a line after printing
 */
void SERIAL_Stream(const char *string, bool new_line);

/**
 * @brief Receives a string from the UART interface
 * @param buffer Buffer to write data to
 * @param size Size of buffer
 * @return Status result
 */
result_t SERIAL_Read(char *buffer, size_t size, uint32_t timeout);

/**
 * @brief Writes a string to the UART interface
 * @param string String to write
 * @return Status result
 */
result_t SERIAL_Write(const char *string, uint32_t timeout);


#if defined(__cplusplus)
}
#endif
