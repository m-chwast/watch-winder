/*
 * timers.h
 *
 *  Created on: Oct 18, 2023
 *      Author: Mateusz Chwast
 */

#ifndef PROGRAM_TIMERS_H_
#define PROGRAM_TIMERS_H_

#include "main.h"


#define TIMERS_MOTOR_TIMER htim14

extern TIM_HandleTypeDef htim14;

void Timers_Start(TIM_HandleTypeDef* htim);
void Timers_Stop(TIM_HandleTypeDef* htim);

#endif /* PROGRAM_TIMERS_H_ */
