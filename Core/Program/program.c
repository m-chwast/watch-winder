/*
 * program.c
 *
 *  Created on: Oct 17, 2023
 *      Author: Mateusz Chwast
 */

#include "console.h"
#include "motor.h"
#include "main.h"

extern TIM_HandleTypeDef htim14;

void Program_Init(void) {
	Console_LogLn("Init");
	HAL_TIM_Base_Start(&htim14);
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
