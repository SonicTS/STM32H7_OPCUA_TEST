#include "stm32h7xx_hal.h"
#ifdef HAL_TIM_MODULE_ENABLED
#include "stdio.h"
#include "string.h"
#include "FreeRTOS.h"
#include "console.h"
#include "pwm_test.h"

extern void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

static TIM_TypeDef * pwm_test_timers[17] = {
		TIM1, TIM2, TIM3, TIM4, TIM5, TIM6, TIM7, TIM8,
		NULL, NULL, NULL, TIM12, TIM13, TIM14, TIM15,
		TIM16, TIM17
};

static uint32_t pwm_test_timer_channels[6] = {
		TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3,
		TIM_CHANNEL_4, TIM_CHANNEL_5, TIM_CHANNEL_6
};

BaseType_t cmd_pwm_start(char *wBuf, size_t wBufferLen, const char *pcCommandString)
{
	const char * args = pcCommandString + 9;
	TIM_HandleTypeDef pwm_tim_handler;
	TIM_OC_InitTypeDef sConfig;
	uint32_t prescale;
	uint32_t period;
	uint32_t tim;
	uint32_t chnl;
	uint32_t duty;
	uint32_t pwm_output;
	uint32_t tim_chnl;

	bzero(&pwm_tim_handler, sizeof(pwm_tim_handler));
	sscanf(args, "%ld %ld %ld %ld", &tim, &chnl, &pwm_output, &duty);
	if (tim == 0 || tim > 17) {
		sprintf(wBuf, "Illegal TIM number.");
		return pdFALSE;
	}
	if (chnl == 0 || chnl > 6) {
		sprintf(wBuf, "Illegal TIM channel number.");
		return pdFALSE;
	}
	if (duty == 0 || duty > 100) {
		sprintf(wBuf, "Illegal duty cycle.");
		return pdFALSE;
	}

	prescale = (uint32_t)(((SystemCoreClock / 2) / 20000000) - 1);
	period = (uint32_t)(((20000000000) / pwm_output) - 1);
	duty = period * duty / 100;

	pwm_tim_handler.Instance               = pwm_test_timers[tim-1];
	pwm_tim_handler.Init.Prescaler         = prescale;
	pwm_tim_handler.Init.Period            = period;
	pwm_tim_handler.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
	pwm_tim_handler.Init.CounterMode       = TIM_COUNTERMODE_UP;
	pwm_tim_handler.Init.RepetitionCounter = 0;
	if (tim == 16) {
		if (HAL_TIM_Base_Init(&pwm_tim_handler) != HAL_OK) {
			sprintf(wBuf, "HAL_TIM_Base_Init error");
			return pdFALSE;
		}
	}
	if (HAL_TIM_PWM_Init(&pwm_tim_handler) != HAL_OK) {
	    /* Initialization Error */
	    sprintf(wBuf, "PWM Init error");
	    return pdFALSE;
	}

	sConfig.OCMode       = TIM_OCMODE_PWM1;
	sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
	sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
	sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
	sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;

	tim_chnl = pwm_test_timer_channels[chnl-1];
	/* Set the pulse value for channel N */
	sConfig.Pulse = duty;
	if (HAL_TIM_PWM_ConfigChannel(&pwm_tim_handler, &sConfig, tim_chnl) != HAL_OK) {
	    /* Configuration Error */
		sprintf(wBuf, "HAL_TIM_PWM_ConfigChannel error");
		return pdFALSE;
	}
	HAL_TIM_MspPostInit(&pwm_tim_handler);

	if (HAL_TIM_PWM_Start(&pwm_tim_handler, tim_chnl) != HAL_OK) {
		sprintf(wBuf, "HAL_TIM_PWM_Start error");
		return pdFALSE;
	}

	*wBuf = 0;
	return pdFALSE;
}

BaseType_t cmd_pwm_stop(char *wBuf, size_t wBufferLen, const char *pcCommandString)
{
	const char * args = pcCommandString + 8;
	TIM_HandleTypeDef pwm_tim_handler;
	uint32_t tim;
	uint32_t chnl;

	sscanf(args, "%ld %ld", &tim, &chnl);
	if (tim == 0 || tim > 17) {
		sprintf(wBuf, "Illegal TIM number");
		return pdFALSE;
	}
	if (chnl == 0 || chnl > 6) {
		sprintf(wBuf, "Illegal TIM channel number");
		return pdFALSE;
	}

	pwm_tim_handler.Instance = pwm_test_timers[tim-1];
	if (HAL_TIM_PWM_Stop(&pwm_tim_handler, pwm_test_timer_channels[chnl-1]) != HAL_OK) {
		sprintf(wBuf, "HAL_TIM_PWM_Stop error");
		return pdFALSE;
	}

	*wBuf = 0;
	return pdFALSE;
}
#endif
