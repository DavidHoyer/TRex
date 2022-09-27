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
  *  @file serial.c
  *  @ingroup peripherals
  *  @author Nikolaij Saegesser
  *  @brief Serial implementations
  */

#include "peripherals/internal/serial.h"
#include "common/processor.h"

#include <string.h>


extern UART_HandleTypeDef huart1;

result_t SERIAL_Init(void){
	/* Noting to do */

	return RESULT_SUCCESS;
}


void SERIAL_Stream(const char *string, bool new_line) {
	SERIAL_Write(string, HAL_MAX_DELAY);

	if (new_line)
	    SERIAL_Write("\r\n", HAL_MAX_DELAY);
}

result_t SERIAL_Read(char *buffer, size_t size, uint32_t timeout) {
	if (buffer == NULL || size == 0)
		return RESULT_SERIAL_READ_ERROR;

	if (HAL_UART_Receive(&huart1, (uint8_t*)buffer, size - 1, timeout) != HAL_OK)
		return RESULT_SERIAL_READ_ERROR;

	/* Null terminate string in buffer */
	buffer[size - 1] = '\x00';

	return RESULT_SUCCESS;
}

result_t SERIAL_Write(const char *string, uint32_t timeout) {
	if (string == NULL)
		return RESULT_SERIAL_WRITE_ERROR;

	if (HAL_UART_Transmit(&huart1, (uint8_t*)string, strlen(string), timeout) != HAL_OK)
		return RESULT_SERIAL_WRITE_ERROR;

	return RESULT_SUCCESS;
}
