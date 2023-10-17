/*
 * motor.h
 *
 *  Created on: Oct 17, 2023
 *      Author: Mateusz Chwast
 */

#ifndef PROGRAM_MOTOR_H_
#define PROGRAM_MOTOR_H_

typedef enum {
	MOTOR_DIR_CLOCKWISE,
	MOTOR_DIR_ANTICLOCKWISE
} Motor_Dir;


void Motor_Step(Motor_Dir dir);

#endif /* PROGRAM_MOTOR_H_ */
