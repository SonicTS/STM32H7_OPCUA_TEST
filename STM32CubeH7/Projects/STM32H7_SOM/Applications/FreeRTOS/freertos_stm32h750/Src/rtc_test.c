#include "stm32h7xx_hal.h"
#ifdef HAL_RTC_MODULE_ENABLED
#include <stdio.h>
#include "string.h"
#include "console.h"
#include "rtc_test.h"

static RTC_HandleTypeDef * rtc_handle = NULL;

static int rtc_init(void)
{
	if (rtc_handle == NULL) {
		rtc_handle = pvPortMalloc(sizeof(RTC_HandleTypeDef));
		if (!rtc_handle) return -1;
		bzero(rtc_handle, sizeof(RTC_HandleTypeDef));
	}
	/* RTC configured as follow:
	 * Hour Format    = Format 24
	 * Asynch Prediv  = Value according to source clock
	 * Synch Prediv   = Value according to source clock
	 * OutPut         = Output Disable
	 * OutPutPolarity = High Polarity
	 * OutPutType     = Open Drain
	 */
	rtc_handle->Instance            = RTC;
	rtc_handle->Init.HourFormat     = RTC_HOURFORMAT_24;
	rtc_handle->Init.AsynchPrediv   = 0x7F;
	rtc_handle->Init.SynchPrediv    = 0xFF;
	rtc_handle->Init.OutPut         = RTC_OUTPUT_DISABLE;
	rtc_handle->Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	rtc_handle->Init.OutPutType     = RTC_OUTPUT_TYPE_OPENDRAIN;
	return HAL_RTC_Init(rtc_handle);
}

BaseType_t cmd_rtc_get_date(char * wBuf, size_t wBufferLen, const char * pcCommandString)
{
	RTC_DateTypeDef   sdate;
	RTC_TimeTypeDef   stime;

	if (rtc_handle == NULL) {
		int error = rtc_init();
		if (error != HAL_OK) {
			sprintf(wBuf, "RTC Init error (%d)", error);
			return pdFALSE;
		}
	}

	/* Get the RTC current Time */
	HAL_RTC_GetTime(rtc_handle, &stime, RTC_FORMAT_BIN);
	/* Get the RTC current Date */
	HAL_RTC_GetDate(rtc_handle, &sdate, RTC_FORMAT_BIN);

	/* Display Date and time */
	sprintf(wBuf,"%.2d-%.2d-%.4d %.2d:%.2d:%.2d", sdate.Month, sdate.Date, 2000 + sdate.Year,
			stime.Hours, stime.Minutes, stime.Seconds);
	return pdFALSE;
}

BaseType_t cmd_rtc_set_date(char * wBuf, size_t wBufferLen, const char * pcCommandString)
{
	RTC_DateTypeDef   sdate;
	RTC_TimeTypeDef   stime;
	char * args = (char *)pcCommandString + 13;
	char * str;
	int d;

	if (strlen(args) > 16) {
		strcpy(wBuf, "Invalid format of date.");
		return pdFALSE;
	}
	bzero(&sdate, sizeof(sdate));
	bzero(&stime, sizeof(stime));
	str = strchr(args, ':');
	if (str == NULL) {
		strcpy(wBuf, "Invalid format of date.");
		return pdFALSE;
	}
	sscanf(args, "%4d", &d);
	d -= 2000;
	if (d < 0 ) {
		strcpy(wBuf, "Specified invalid year.");
		return pdFALSE;
	}
	sdate.Year = (uint8_t)d;

	args = str + 1;
	str = strchr(args, ':');
	if (str == NULL) {
		strcpy(wBuf, "Invalid format of date (month).");
		return pdFALSE;
	}
	sscanf(args, "%2d", &d);
	if (d <= 0 || d > 12) {
		strcpy(wBuf, "Specified invalid month.");
		return pdFALSE;
	}
	sdate.Month =  (uint8_t)d;

	args = str + 1;
	str = strchr(args, ':');
	if (str == NULL) {
		strcpy(wBuf, "Invalid format of date (day).");
		return pdFALSE;
	}
	sscanf(args, "%2d", &d);
	if (d <= 0 || d > 31) {
		strcpy(wBuf, "Specified invalid day.");
		return pdFALSE;
	}
	sdate.Date =  (uint8_t)d;
	sdate.WeekDay = RTC_WEEKDAY_MONDAY;

	args = str + 1;
	str = strchr(args, ':');
	if (str == NULL) {
		strcpy(wBuf, "Invalid format of date (hour).");
		return pdFALSE;
	}
	sscanf(args, "%2d", &d);
	if (d < 0 || d > 24) {
		strcpy(wBuf, "Invalid hour specified.");
		return pdFALSE;
	}
	stime.Hours =  (uint8_t)d;

	args = str + 1;
	sscanf(args, "%2d", &d);
	if (d < 0 || d > 60) {
		strcpy(wBuf, "Invalid minute specified.");
		return pdFALSE;
	}
	stime.Minutes =  (uint8_t)d;

	if (rtc_handle == NULL) {
		int error = rtc_init();
		if (error != HAL_OK) {
			sprintf(wBuf, "RTC Init error (%d)", error);
			return pdFALSE;
		}
	}
	if ((d = HAL_RTC_SetDate(rtc_handle, &sdate, RTC_FORMAT_BIN)) != HAL_OK) {
		sprintf(wBuf, "HAL_RTC_SetDate error=0x%x", d);
		return pdFALSE;
	}
	if ((d = HAL_RTC_SetTime(rtc_handle, &stime, RTC_FORMAT_BIN)) != HAL_OK) {
		sprintf(wBuf, "HAL_RTC_SetTime error=0x%x", d);
		return pdFALSE;
	}
	HAL_RTC_WaitForSynchro(rtc_handle);
	*wBuf = 0;

	return pdFALSE;
}

/*
 * RTC MSP Initialization
 */
void HAL_RTC_MspInit(RTC_HandleTypeDef * hrtc)
{
	RCC_OscInitTypeDef Osc = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

	/* Configure LSE as RTC clock source */
	Osc.OscillatorType = RCC_OSCILLATORTYPE_LSE;
	Osc.LSEState = RCC_LSE_ON;
	Osc.HSIState = RCC_HSI_OFF;
	Osc.HSICalibrationValue = 0;
	Osc.LSIState = RCC_LSI_OFF;
	Osc.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&Osc) != HAL_OK) {
		console_puts("HAL_RCC_OscConfig error\n\r");
		for(;;);
	}

	/* Select LSI as RTC clock source */
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
		console_puts("HAL_RCCEx_PeriphCLKConfig error\n\r");
		for(;;);
	}

	__HAL_RCC_RTC_CLKPRESCALER(RCC_RTCCLKSOURCE_LSE);
	__HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSE);

	/* Enable RTC Clock */
	__HAL_RCC_RTC_ENABLE();
}

/*
 * RTC MSP De-Initialization
 */
void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
	__HAL_RCC_RTC_DISABLE();
}
#endif
