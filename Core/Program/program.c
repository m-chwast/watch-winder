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


void Program_Init(void) {
	Console_LogLn("Init");
	Motor_SetSpeed(6);
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
}
