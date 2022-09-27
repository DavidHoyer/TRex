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
  *  @file adc.c
  *  @ingroup peripherals
  *  @author Nikolaij Saegesser
  *  @brief ADC implementations
  */

#include "peripherals/internal/adc.h"
#include "common/processor.h"


extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc3;


static ADC_HandleTypeDef* GetHandle(ADC_Input_t input) {
	switch (input) {
		case ADC1_14:
		case ADC1_15:
		case ADC1_16:
			return &hadc1;
		case ADC2_3:
			return &hadc2;
		case ADC3_8:
		case Potentiometer:
			return &hadc3;
		default: return NULL;
	}
}

static uint8_t GetResolution(ADC_Input_t input) {
	ADC_HandleTypeDef *handle = GetHandle(input);

	switch (handle->Init.Resolution) {
		case ADC_RESOLUTION_8B: return 8;
		case ADC_RESOLUTION_10B: return 10;
		case ADC_RESOLUTION_12B: return 12;
		case ADC_RESOLUTION_14B: return 14;
		case ADC_RESOLUTION_16B: return 16;
		default: return 16;
	}
}


result_t ADC_Init(void) {
	/* Noting to do */
	return RESULT_SUCCESS;
}


result_t ADC_StartMeasurement(ADC_Input_t input) {
	/* Switch channel */
	ADC_ChannelConfTypeDef channelConfig = { 0 };

	switch (input) {
		case ADC1_14: channelConfig.Channel = ADC_CHANNEL_14; break;
		case ADC1_15: channelConfig.Channel = ADC_CHANNEL_15; break;
		case ADC1_16: channelConfig.Channel = ADC_CHANNEL_16; break;
		case ADC2_3:  channelConfig.Channel = ADC_CHANNEL_3;  break;

		case Potentiometer:
		case ADC3_8:  
			channelConfig.Channel = ADC_CHANNEL_8;  
			break;
		default: return RESULT_INVALID_ARGUMENTS;
	}

	channelConfig.Rank = ADC_REGULAR_RANK_1;
	channelConfig.SamplingTime = ADC_SAMPLETIME_8CYCLES_5;

	ADC_HandleTypeDef *handle = GetHandle(input);
	if (HAL_ADC_ConfigChannel(handle, &channelConfig) != HAL_OK)
		return RESULT_ADC_MEASUREMENT_ERROR;

	/* Start conversion */
	if (HAL_ADC_Start(handle) != HAL_OK)
		return RESULT_ADC_MEASUREMENT_ERROR;

	return RESULT_SUCCESS;
}

bool ADC_IsReady(ADC_Input_t input) {
	return HAL_ADC_PollForConversion(GetHandle(input), 0) == HAL_OK;
}

result_t ADC_GetValue(ADC_Input_t input, float32_t *result) {
	ADC_HandleTypeDef *handle = GetHandle(input);

	if (HAL_ADC_PollForConversion(handle, INT32_MAX) != HAL_OK)
		return RESULT_ADC_INVALID_VALUE;

	if (result == NULL)
		return RESULT_INVALID_ARGUMENTS;

	*result = (float32_t)(HAL_ADC_GetValue(handle)) / (1U << GetResolution(input)) * ADC_REFERENCE_VOLTAGE;

	return RESULT_SUCCESS;
}
