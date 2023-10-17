/*
 * motor.c
 *
 *  Created on: Oct 17, 2023
 *      Author: Mateusz Chwast
 */

#include <assert.h>
#include "motor.h"
#include "main.h"


static void ResetMotorPins(uint32_t noResetMask) {
	//if any assert fails, function needs modifications to work correctly
	static_assert(MOTOR_OUT1_GPIO_Port == MOTOR_OUT2_GPIO_Port);
	static_assert(MOTOR_OUT1_GPIO_Port == MOTOR_OUT3_GPIO_Port);
	static_assert(MOTOR_OUT1_GPIO_Port == MOTOR_OUT4_GPIO_Port);

	uint32_t pinMask = MOTOR_OUT1_Pin | MOTOR_OUT2_Pin | MOTOR_OUT3_Pin | MOTOR_OUT4_Pin;
	pinMask &= ~(noResetMask);
	HAL_GPIO_WritePin(MOTOR_OUT1_GPIO_Port, pinMask, GPIO_PIN_RESET);
}

void Motor_Step(Motor_Dir dir) {
	static uint8_t state;

	switch(state) {
		case 0: {
			ResetMotorPins(MOTOR_OUT1_Pin);
			HAL_GPIO_WritePin(MOTOR_OUT1_GPIO_Port, MOTOR_OUT1_Pin, 1);
			break;
		}
		case 1: {
			ResetMotorPins(MOTOR_OUT2_Pin);
			HAL_GPIO_WritePin(MOTOR_OUT2_GPIO_Port, MOTOR_OUT2_Pin, 1);
			break;
		}
		case 2: {
			ResetMotorPins(MOTOR_OUT3_Pin);
			HAL_GPIO_WritePin(MOTOR_OUT3_GPIO_Port, MOTOR_OUT3_Pin, 1);
			break;
		}
		case 3: {
			ResetMotorPins(MOTOR_OUT4_Pin);
			HAL_GPIO_WritePin(MOTOR_OUT4_GPIO_Port, MOTOR_OUT4_Pin, 1);
			break;
		}
	}

	state++;
	state %= 4;
}
