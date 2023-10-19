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


void Program_Init(void) {
	Console_LogLn("Init");

	Leds_BeginInit(&led0);
	Leds_BeginInit(&led1);

	Motor_SetSpeed(6);

	Buttons_SetCallbacks(&button0, NULL, Modes_Main_SetNext, Modes_Main_ToggleEditActive);
	Buttons_SetCallbacks(&button1, NULL, Modes_Timing_SetNext, Modes_Timing_ToggleEditActive);
}

void Program_Loop(void) {
	/*
	if(Motor_IsRunning()) {
		return;
	}

	static Motor_Dir dir;
	dir = !dir;

	Motor_SetMovement(2 * 360, dir);
	HAL_Delay(2000);
	*/

	Buttons_Manage();
	Leds_Manage();
}
