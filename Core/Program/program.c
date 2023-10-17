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
}

void Program_Loop(void) {
	HAL_Delay(4);
	Motor_Step(MOTOR_DIR_CLOCKWISE);
}
