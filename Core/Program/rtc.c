/*
 * rtc.c
 *
 *  Created on: Oct 19, 2023
 *      Author: Mateusz Chwast
 */

#include "main.h"
#include "rtc.h"
#include "modes.h"


extern RTC_HandleTypeDef hrtc;


void RTC_Init(void) {
	RTC_SetNextAlarm();
}

void RTC_SetNextAlarm(void) {
	uint32_t secondsToAlarm = Modes_GetCyclePeriod();

	uint8_t hours = secondsToAlarm / 3600;
	secondsToAlarm -= hours * 3600;
	uint8_t minutes = secondsToAlarm / 60;
	secondsToAlarm -= minutes * 60;

	RTC_AlarmTypeDef alarm = {
			.Alarm = RTC_ALARM_A,
			.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY,
	};

	RTC_TimeTypeDef* time = &alarm.AlarmTime;
	HAL_RTC_GetTime(&hrtc, time, RTC_FORMAT_BIN);

	time->Seconds += secondsToAlarm;
	time->Minutes += minutes;
	time->Hours += hours;

	//handle time overflows for seconds and minutes
	if(time->Seconds > 59) {
		time->Seconds -= 60;
		time->Minutes++;
	}
	if(time->Minutes > 59) {
		time->Minutes -= 60;
		time->Hours++;
	}

	HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);	//this is demanded by HAL
	HAL_RTC_SetAlarm_IT(&hrtc, &alarm, RTC_FORMAT_BIN);
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc) {

}
