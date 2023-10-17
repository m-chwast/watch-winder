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
	static uint16_t cnt;
	static Motor_Dir dir = MOTOR_DIR_CLOCKWISE;
	cnt++;
	if(cnt == 2048) {
		cnt = 0;
		dir = !dir;
	}

	HAL_Delay(2);
	Motor_Step(dir);
}
