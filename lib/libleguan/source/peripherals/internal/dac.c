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
  *  @file dac.c
  *  @ingroup peripherals
  *  @author Nikolaij Saegesser
  *  @brief DAC implementations
  */

#include "peripherals/internal/dac.h"
#include "common/processor.h"

DAC_Handle_t DAC1_CH1 = { DAC1, 1 };
DAC_Handle_t DAC1_CH2 = { DAC1, 2 };

result_t DAC_Init(DAC_Handle_t handle) {
	if (handle.channel == 1)
		handle.handle->CR |= DAC_CR_EN1;
	else if (handle.channel == 2)
		handle.handle->CR |= DAC_CR_EN2;
	else
		return RESULT_INVALID_ARGUMENTS;

	return RESULT_SUCCESS;
}


result_t DAC_SetValue(DAC_Handle_t handle, float32_t value) {
	uint32_t dac_value = (value / DAC_REFERENCE_VOLTAGE) * ((1U << 12) - 1);
	if (handle.channel == 1)
		handle.handle->DHR12R1 = dac_value;
	else if (handle.channel == 2)
		handle.handle->DHR12R2 = dac_value;
	else
		return RESULT_INVALID_ARGUMENTS;

	return RESULT_SUCCESS;
}
