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
  *  @file core.c
  *  @ingroup common
  *  @author Nikolaij Saegesser
  *  @brief Core control features
  */

#include "common/core.h"
#include "common/mmio.h"

#include "common/processor.h"

#include <string.h>

void CORE_Delay(uint32_t ms) {
    HAL_Delay(ms);
}

CORE_SerialNumber_t CORE_GetSerialNumber(void) {
	volatile CORE_SerialNumber_t *U_ID = MMIO_MAP(0x1FF1E800, CORE_SerialNumber_t);

	return *U_ID;
}

void CORE_Reset(void) {
    NVIC_SystemReset();
}

void CORE_EnableInterrupts(void) {
    asm volatile ("cpsie i" : : : "memory");
}

void CORE_DisableInterrupts(void) {
    asm volatile ("cpsid i" : : : "memory");
}

void CORE_ClearPendingInterrupts(void) {
    for (uint8_t i = 0; i < 8; i++)
        NVIC->ICPR[i] = 0xFF;
}
