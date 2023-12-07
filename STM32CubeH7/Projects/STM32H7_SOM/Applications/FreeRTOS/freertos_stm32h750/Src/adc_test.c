#include "stm32h7xx_hal.h"
#ifdef HAL_ADC_MODULE_ENABLED
#include "stdio.h"
#include "string.h"
#include "FreeRTOS.h"
#include "console.h"
#include "adc_test.h"

static ADC_TypeDef * adc_test_base[3] = {
		ADC1, ADC2, ADC3
};
static uint32_t adc_test_channels[19] = {
		ADC_CHANNEL_1, ADC_CHANNEL_2, ADC_CHANNEL_3, ADC_CHANNEL_4, ADC_CHANNEL_5, ADC_CHANNEL_6, ADC_CHANNEL_7,
		ADC_CHANNEL_8, ADC_CHANNEL_9, ADC_CHANNEL_10, ADC_CHANNEL_11, ADC_CHANNEL_12, ADC_CHANNEL_13,
		ADC_CHANNEL_14, ADC_CHANNEL_15, ADC_CHANNEL_16, ADC_CHANNEL_17, ADC_CHANNEL_18, ADC_CHANNEL_19,
};

BaseType_t cmd_adc_se(char *wBuf, size_t wBufferLen, const char *pcCommandString)
{
	const char * args = pcCommandString + 6;
	ADC_HandleTypeDef hadc;
	ADC_ChannelConfTypeDef sConfig;
	uint32_t nadc;
	uint32_t chnl;
	uint32_t val;
	uint32_t input_voltage;

	sscanf(args, "%ld %ld", &nadc, &chnl);
	if (nadc == 0 || nadc > 3) {
		sprintf(wBuf, "Illegal ADC number");
		return pdFALSE;
	}
	if (chnl == 0 || chnl > 20) {
		sprintf(wBuf, "Illegal ADC channel number");
		return pdFALSE;
	}

	bzero(&hadc, sizeof(ADC_HandleTypeDef));
	bzero(&sConfig, sizeof(ADC_ChannelConfTypeDef));

	/* ADC Periph interface clock configuration */
	__HAL_RCC_ADC_CONFIG(RCC_ADCCLKSOURCE_CLKP);

	hadc.Instance = adc_test_base[nadc-1];
	hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV6;
	hadc.Init.Resolution = ADC_RESOLUTION_16B;
	hadc.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc.Init.LowPowerAutoWait = DISABLE;
	hadc.Init.ContinuousConvMode = DISABLE;
	hadc.Init.NbrOfConversion = 1;
	hadc.Init.DiscontinuousConvMode = DISABLE;
	hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
	hadc.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN; /* ADC_OVR_DATA_PRESERVED; */
	hadc.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
	hadc.Init.OversamplingMode = DISABLE;
	if (HAL_ADC_Init(&hadc) != HAL_OK) {
		sprintf(wBuf, "ADC Init error (0x%lx)", HAL_ADC_GetError(&hadc));
		return pdFALSE;
	}
	/* Configure Regular Channel
	 */
	sConfig.Channel = adc_test_channels[chnl-1];
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_8CYCLES_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
		sprintf(wBuf, "Configure Regular Channel error");
		goto Exit;
	}

	/* Run the ADC calibration in single-ended mode */
	if (HAL_ADCEx_Calibration_Start(&hadc, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK) {
		sprintf(wBuf, "Calibration error (0x%lx)", HAL_ADC_GetError(&hadc));
		goto Exit;
	}
	if (HAL_ADC_Start(&hadc) != HAL_OK) {
		sprintf(wBuf, "Start conversation error");
		goto Exit;
	}
	if (HAL_ADC_PollForConversion(&hadc, 10) != HAL_OK) {
		sprintf(wBuf, "End Of Conversion flag not set on time");
		goto Exit;
	}
	val = HAL_ADC_GetValue(&hadc);
	input_voltage = ((val * 3300) / 0xFFFF);
	sprintf(wBuf, "Input voltage = %ldmV", input_voltage);

Exit:
	HAL_ADC_DeInit(&hadc);
	return pdFALSE;
}
#endif
