/*
 * program.c
 *
 *  Created on: Oct 17, 2023
 *      Author: Mateusz Chwast
 */

#include "main.h"
#include "console.h"
#include "motor.h"
#include "buttons.h"
#include "modes.h"
#include "leds.h"
#include "rtc.h"
#include "watch_winder.h"


void Program_Init(void) {
	Console_LogLn("Init");

	RTC_SetupPeriodCallback(Modes_GetCyclePeriod);
	RTC_SetupAlarmCallback(WatchWinder_SetRotationRequest);

	Leds_BeginInit(&led0);
	Leds_BeginInit(&led1);

	Motor_SetSpeed(MOTOR_RPM_TO_RPH(4));

	Buttons_SetCallbacks(&button0, NULL, Modes_Main_ToggleEditActive, Modes_Main_SetNext, Buttons_EmptyCallback);
	Buttons_SetCallbacks(&button1, NULL, Modes_Timing_ToggleEditActive, Modes_Timing_SetNext, Buttons_EmptyCallback);

	Leds_LedCounterSetup(&led0, Modes_Main_GetEditActive, Modes_Main_GetIncrementedTmp);
	Leds_LedCounterSetup(&led1, Modes_Timing_GetEditActive, Modes_Timing_GetIncrementedTmp);

	WatchWinder_Refresh();
}

void Program_Loop(void) {
	WatchWinder_Manage();
	Buttons_Manage();
	Leds_Manage();
	RTC_Manage();
}
