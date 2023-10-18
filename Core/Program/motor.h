/*
 * motor.h
 *
 *  Created on: Oct 17, 2023
 *      Author: Mateusz Chwast
 */

#ifndef PROGRAM_MOTOR_H_
#define PROGRAM_MOTOR_H_

#include <stdbool.h>

//set only one mode to true
#define DRIVE_WAVE false
#define DRIVE_FULL_STEP false
#define DRIVE_HALF_STEP true

#define BASE_STEPS_PER_REVOLUTION 2048

#if !DRIVE_HALF_STEP
#define FINAL_DRIVE_STEP (4 - 1)
#define MOTOR_STEPS_PER_REVOLUTION (BASE_STEPS_PER_REVOLUTION)
#else
#define FINAL_DRIVE_STEP (8 - 1)
#define MOTOR_STEPS_PER_REVOLUTION (2 * BASE_STEPS_PER_REVOLUTION)
#endif


typedef enum {
	MOTOR_DIR_CLOCKWISE,
	MOTOR_DIR_ANTICLOCKWISE
} Motor_Dir;


void Motor_Step(Motor_Dir dir);

#endif /* PROGRAM_MOTOR_H_ */
