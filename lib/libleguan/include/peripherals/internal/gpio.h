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
  *  @file gpio.h
  *  @ingroup peripherals
  *  @author Nikolaij Saegesser
  *  @brief GPIO Abstraction
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

/* Types */
typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
	bool low_active;
} GPIO_Handle_t;

#define GPIO(port, pin) 			{ GPIO##port, pin, false }
#define GPIO_LOW_ACTIVE(port, pin) 	{ GPIO##port, pin, true }

typedef uint32_t GPIO_Speed_t;
typedef uint32_t GPIO_PullResistor_t;
typedef uint32_t GPIO_Mode_t;

/* Functions */

/**
 * @brief Initializes the GPIO Hardware
 * @return Status result
 */
result_t GPIO_Init();

/**
 * @brief Sets the output speed of a GPIO pin
 * @warning Higher values mean higher current flowing. This can lead to reflections or hardware damage
 * @param handle GPIO Handle
 * @param speed GPIO Speed
 */
void GPIO_SetSpeed(GPIO_Handle_t handle, GPIO_Speed_t speed);

/**
 * @brief Enables or disables a pull-resistor for a GPIO pin
 * @param handle GPIO Handle
 * @param pull_resistor Pull Resistor to use
 */
void GPIO_SetPullResistor(GPIO_Handle_t handle, GPIO_PullResistor_t pull_resistor);

/**
 * @brief Sets the mode of the GPIO pin
 * @param handle GPIO Handle
 * @param mode Mode of the GPIO pin (open drain, push-pull, etc)
 */
void GPIO_SetMode(GPIO_Handle_t handle, GPIO_Mode_t mode);


/**
 * @brief Gets the value of a GPIO pin
 * @param handle GPIO Handle
 * @return State of GPIO pin
 */
bool GPIO_GetValue(GPIO_Handle_t handle);

/**
 * @brief Sets the value of a GPIO pin
 * @param handle GPIO Handle
 * @param New state of GPIO pin
 */
void GPIO_SetValue(GPIO_Handle_t handle, bool state);

/**
 * @brief Toggles the state of a GPIO pin
 * @param handle GPIO Handle
 */
void GPIO_ToggleValue(GPIO_Handle_t handle);


#if defined(__cplusplus)
}
#endif
