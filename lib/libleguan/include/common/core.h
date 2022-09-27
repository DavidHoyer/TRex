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
  *  @file core.h
  *  @ingroup common
  *  @author Nikolaij Saegesser
  *  @brief Core control features
  */

#pragma once

#if defined(__cplusplus)
extern "C" {
#endif


/* Includes */

#include "common/types.h"


/* Types */

typedef struct {
	uint8_t serial[12];
} CORE_SerialNumber_t;


/* Functions */

/**
 * @brief Blocks execution of a specific number of milliseconds
 * @param ms Milliseconds to wait
 */
void CORE_Delay(uint32_t ms);

/**
 * @brief Gets the unique Serial number of the Controller
 * @return 96 bit serial number
 */
CORE_SerialNumber_t CORE_GetSerialNumber(void);

/**
 * @brief Resets the current CPU core
 */
void CORE_Reset(void);

/**
 * @brief Enables interrupt controller
 */
void CORE_EnableInterrupts(void);

/**
 * @brief Disables interrupt controller
 */
void CORE_DisableInterrupts(void);

/**
 * @brief Clears all currently pending interrupts
 */
void CORE_ClearPendingInterrupts(void);


#if defined(__cplusplus)
}
#endif
