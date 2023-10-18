/*
 * motor.c
 *
 *  Created on: Oct 17, 2023
 *      Author: Mateusz Chwast
 */

#include <assert.h>
#include "motor.h"
#include "main.h"


#define DEGREES_TO_STEPS(DEG) ((DEG) * MOTOR_STEPS_PER_REVOLUTION / 360)


struct Motor {
	volatile uint32_t stepsLeft;
	volatile Motor_Dir dir;
	bool isRunning;
} static motor;


void Motor_SetMovement(uint32_t degrees, Motor_Dir dir) {
	motor.stepsLeft = DEGREES_TO_STEPS(degrees);
	motor.dir = dir;
}

bool Motor_IsRunning(void) {
	return motor.isRunning;
}

static void MotorPinsWrite(uint32_t setMask) {
	//if any assert fails, function needs modifications to work correctly
	static_assert(MOTOR_OUT1_GPIO_Port == MOTOR_OUT2_GPIO_Port);
	static_assert(MOTOR_OUT1_GPIO_Port == MOTOR_OUT3_GPIO_Port);
	static_assert(MOTOR_OUT1_GPIO_Port == MOTOR_OUT4_GPIO_Port);

	uint32_t resetMask = MOTOR_OUT1_Pin | MOTOR_OUT2_Pin | MOTOR_OUT3_Pin | MOTOR_OUT4_Pin;
	resetMask &= ~(setMask);
	HAL_GPIO_WritePin(MOTOR_OUT1_GPIO_Port, resetMask, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR_OUT1_GPIO_Port, setMask, GPIO_PIN_SET);
}

void Motor_Step(Motor_Dir dir) {
	//assert correct mode selection
	static_assert((DRIVE_WAVE && DRIVE_FULL_STEP) == false);
	static_assert((DRIVE_WAVE && DRIVE_HALF_STEP) == false);
	static_assert((DRIVE_FULL_STEP && DRIVE_HALF_STEP) == false);
	static_assert(DRIVE_WAVE || DRIVE_FULL_STEP || DRIVE_HALF_STEP);

	static uint8_t state;

	if(dir == MOTOR_DIR_CLOCKWISE) {
		state >= FINAL_DRIVE_STEP ? state = 0 : state++;
	}
	else {
		state == 0 ? state = FINAL_DRIVE_STEP : state--;
	}

#if DRIVE_WAVE
	switch(state) {
		case 0: {
			MotorPinsWrite(MOTOR_OUT1_Pin);
			break;
		}
		case 1: {
			MotorPinsWrite(MOTOR_OUT2_Pin);
			break;
		}
		case 2: {
			MotorPinsWrite(MOTOR_OUT3_Pin);
			break;
		}
		case 3: {
			MotorPinsWrite(MOTOR_OUT4_Pin);
			break;
		}
	}
#endif
#if DRIVE_FULL_STEP
	switch(state) {
		case 0: {
			MotorPinsWrite(MOTOR_OUT4_Pin | MOTOR_OUT1_Pin);
			break;
		}
		case 1: {
			MotorPinsWrite(MOTOR_OUT1_Pin | MOTOR_OUT2_Pin);
			break;
		}
		case 2: {
			MotorPinsWrite(MOTOR_OUT2_Pin | MOTOR_OUT3_Pin);
			break;
		}
		case 3: {
			MotorPinsWrite(MOTOR_OUT3_Pin | MOTOR_OUT4_Pin);
			break;
		}
	}
#endif
#if DRIVE_HALF_STEP
	switch(state) {
		case 0: {
			MotorPinsWrite(MOTOR_OUT4_Pin | MOTOR_OUT1_Pin);
			break;
		}
		case 1: {
			MotorPinsWrite(MOTOR_OUT1_Pin);
			break;
		}
		case 2: {
			MotorPinsWrite(MOTOR_OUT1_Pin | MOTOR_OUT2_Pin);
			break;
		}
		case 3: {
			MotorPinsWrite(MOTOR_OUT2_Pin);
			break;
		}
		case 4: {
			MotorPinsWrite(MOTOR_OUT2_Pin | MOTOR_OUT3_Pin);
			break;
		}
		case 5: {
			MotorPinsWrite(MOTOR_OUT3_Pin);
			break;
		}
		case 6: {
			MotorPinsWrite(MOTOR_OUT3_Pin | MOTOR_OUT4_Pin);
			break;
		}
		case 7: {
			MotorPinsWrite(MOTOR_OUT4_Pin);
			break;
		}
	}
#endif
}
