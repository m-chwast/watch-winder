/*
 * rtc.c
 *
 *  Created on: Oct 19, 2023
 *      Author: Mateusz Chwast
 */

#include <stdbool.h>
#include "main.h"
#include "rtc.h"
#include "console.h"


extern RTC_HandleTypeDef hrtc;


static struct Rtc {
	volatile bool alarmFlag;
	struct Callbacks {
		void(*alarm)(void);
		uint32_t(*periodSeconds)(void);
	} callbacks;
} rtc;


static void PrintTime(void);
static void GetTime(RTC_TimeTypeDef* time);


void RTC_Init(void) {
	RTC_SetNextAlarm();
}

void RTC_Manage(void) {
	if(rtc.alarmFlag) {
		rtc.alarmFlag = false;
		Console_Log("Alarm! ");
		PrintTime();
		RTC_SetNextAlarm();
		if(rtc.callbacks.alarm != NULL) {
			rtc.callbacks.alarm();
		}
	}
}

void RTC_SetupAlarmCallback(void(*cb)(void)) {
	rtc.callbacks.alarm = cb;
}

void RTC_SetupPeriodCallback(uint32_t(*periodSecondsCb)(void)) {
	rtc.callbacks.periodSeconds = periodSecondsCb;
}

void RTC_SetNextAlarm(void) {
	if(rtc.callbacks.periodSeconds == NULL) {
		return;
	}

	uint32_t secondsToAlarm = rtc.callbacks.periodSeconds();

	uint8_t hours = secondsToAlarm / 3600;
	secondsToAlarm -= hours * 3600;
	uint8_t minutes = secondsToAlarm / 60;
	secondsToAlarm -= minutes * 60;

	RTC_AlarmTypeDef alarm = {
			.Alarm = RTC_ALARM_A,
			.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY,
	};

	RTC_TimeTypeDef* time = &alarm.AlarmTime;
	GetTime(time);

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
	rtc.alarmFlag = true;
}

static void PrintTime(void) {
	RTC_TimeTypeDef time;
	GetTime(&time);

	Console_LogVal("Time: ", time.Hours);
	Console_LogVal(":", time.Minutes);
	Console_LogValLn(":", time.Seconds);
}

static void GetTime(RTC_TimeTypeDef* time) {
	HAL_RTC_GetTime(&hrtc, time, RTC_FORMAT_BIN);

	//date read to unlock shadow register
	RTC_DateTypeDef date;
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BCD);
}
