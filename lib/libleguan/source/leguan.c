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
  *  @file leguan.c
  *  @ingroup leguan
  *  @author Nikolaij Saegesser
  *  @brief Main leguan implementations
  */

#include "leguan.h"

result_t LEGUAN_Init(void) {
	R_TRY(FPGA_Init());
	R_TRY(LCD_Init());
	R_TRY(LCD_TouchInit());
	R_TRY(SENSOR_Init());

	R_TRY(GPIO_Init());
	R_TRY(SERIAL_Init());
	R_TRY(SPI_Init(SPI_Sensor));
	R_TRY(SPI_Init(SPI_Touch));
	R_TRY(I2C_Init());
	R_TRY(ADC_Init());
	R_TRY(DAC_Init(DAC1_CH1));
	R_TRY(DAC_Init(DAC1_CH2));

	return RESULT_SUCCESS;
}
