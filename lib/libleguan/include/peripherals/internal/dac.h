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
  *  @file dac.h
  *  @ingroup peripherals
  *  @author Nikolaij Saegesser
  *  @brief DAC Abstraction
  */

#pragma once

#if defined(__cplusplus)
extern "C" {
#endif


/* Includes */
#include "common/types.h"
#include "common/attributes.h"
#include "common/mmio.h"
#include "common/processor.h"

/* Macros */
#define DAC_REFERENCE_VOLTAGE 3.3/*V*/

/* Types */
typedef struct {
	DAC_TypeDef *handle;
	uint32_t channel;
} DAC_Handle_t;

/* Constants */

extern DAC_Handle_t DAC1_CH1;
extern DAC_Handle_t DAC1_CH2;

/* Functions */

/**
 * @brief Initializes the DAC Hardware
 * @return Status result
 */
result_t DAC_Init(DAC_Handle_t handle);

/**
 * @brief Sets the voltage on a DAC pin
 * @param handle DAC Handle
 * @param value Value to set in Volts
 * @return Status result
 */
result_t DAC_SetValue(DAC_Handle_t handle, float32_t value);


#if defined(__cplusplus)
}
#endif
