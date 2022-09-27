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
  *  @file gpio.c
  *  @ingroup peripherals
  *  @author Nikolaij Saegesser
  *  @brief GPIO implementations
  */

#include "peripherals/internal/gpio.h"

result_t GPIO_Init() {

	/* Enable all GPIO Clocks */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOI_CLK_ENABLE();

	return RESULT_SUCCESS;
}

void GPIO_SetSpeed(GPIO_Handle_t handle, GPIO_Speed_t speed) {
    uint32_t register_value = handle.port->OSPEEDR;

	register_value &= ~(0b11 << (handle.pin * 2));
	register_value |= (speed << (handle.pin * 2));

	handle.port->OSPEEDR = register_value;
}

void GPIO_SetPullResistor(GPIO_Handle_t handle, GPIO_PullResistor_t pull_resistor) {
	uint32_t register_value = handle.port->PUPDR;

	register_value &= ~(0b11 << (handle.pin * 2));
	register_value |= (pull_resistor << (handle.pin * 2));

	handle.port->PUPDR = register_value;
}

void GPIO_SetMode(GPIO_Handle_t handle, GPIO_Mode_t mode) {
	uint32_t register_value = handle.port->MODER;

	register_value &= ~(0b11 << (handle.pin * 2));
	register_value |= (mode << (handle.pin * 2));

	handle.port->MODER = register_value;
}

bool GPIO_GetValue(GPIO_Handle_t handle) {
	bool result = (handle.port->IDR & (1U << handle.pin)) != 0x00;

	if (handle.low_active)
		result = !result;

	return result;
}

void GPIO_SetValue(GPIO_Handle_t handle, bool state) {
	if (handle.low_active)
		state = !state;

	if (state)
		handle.port->ODR |= (1U << handle.pin);
	else
		handle.port->ODR &= ~(1U << handle.pin);
}

void GPIO_Toggle(GPIO_Handle_t handle) {
	handle.port->ODR ^= (1U << handle.pin);
}
