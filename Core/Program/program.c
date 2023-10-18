/*
 * program.c
 *
 *  Created on: Oct 17, 2023
 *      Author: Mateusz Chwast
 */

#include "console.h"
#include "motor.h"
#include "main.h"


void Program_Init(void) {
	Console_LogLn("Init");
	Motor_SetSpeed(6);
}

void Program_Loop(void) {
	if(Motor_IsRunning()) {
		return;
	}

	static Motor_Dir dir;
	dir = !dir;

	Motor_SetMovement(3 * 360, dir);
	HAL_Delay(2000);
}
