/*
 * motor.h
 *
 *  Created on: Oct 17, 2023
 *      Author: Mateusz Chwast
 */

#ifndef PROGRAM_MOTOR_H_
#define PROGRAM_MOTOR_H_

#include <stdbool.h>
#include <stdint.h>


#define MOTOR_REVOLUTIONS_TO_DEGREES(REV) (REV * 360)
#define MOTOR_RPM_TO_RPH(RPM) ((RPM) * 60)


typedef enum {
	MOTOR_DIR_CLOCKWISE,
	MOTOR_DIR_ANTICLOCKWISE
} Motor_Dir;


void Motor_SetMovement(uint32_t degrees, Motor_Dir dir);
void Motor_SetMovementInfinite(Motor_Dir dir);
void Motor_SetSpeed(uint32_t revPerHour);

bool Motor_IsRunning(void);

void Motor_RequestStop(void);

void Motor_IRQHandler(void);

#endif /* PROGRAM_MOTOR_H_ */
