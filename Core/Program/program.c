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


static struct Program {
	bool isRotationRequested;
} program;


static void Program_Manage(void);
static void SetRotationRequest(void);


void Program_Init(void) {
	Console_LogLn("Init");

	RTC_Init();
	RTC_SetupAlarmCallback(SetRotationRequest);

	Leds_BeginInit(&led0);
	Leds_BeginInit(&led1);

	Motor_SetSpeed(6);

	Buttons_SetCallbacks(&button0, NULL, Modes_Main_ToggleEditActive, Modes_Main_SetNext, Buttons_EmptyCallback);
	Buttons_SetCallbacks(&button1, NULL, Modes_Timing_ToggleEditActive, Modes_Timing_SetNext, Buttons_EmptyCallback);

	Leds_LedCounterSetup(&led0, Modes_Main_GetEditActive, Modes_Main_GetIncremented);
	Leds_LedCounterSetup(&led1, Modes_Timing_GetEditActive, Modes_Timing_GetIncremented);
}

void Program_Loop(void) {
	Program_Manage();
	Buttons_Manage();
	Leds_Manage();
	RTC_Manage();
}

static void Program_Manage(void) {
	if(program.isRotationRequested && Motor_IsRunning() == false) {
		program.isRotationRequested = false;
		Motor_SetMovement(MOTOR_REVOLUTIONS_TO_DEGREES(1), MOTOR_DIR_CLOCKWISE);
	}
}

static void SetRotationRequest(void) {
	program.isRotationRequested = true;
}
