/*
 * timers.c
 *
 *  Created on: Oct 18, 2023
 *      Author: Mateusz Chwast
 */

#include "timers.h"
#include "main.h"
#include "motor.h"


void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef* htim) {
	if(htim == &TIMERS_MOTOR_TIMER) {
		Motor_IRQHandler();
	}
}

void Timers_Start(TIM_HandleTypeDef* htim) {
	if(htim == &TIMERS_MOTOR_TIMER) {
		HAL_TIM_OC_Start_IT(htim, TIM_CHANNEL_1);
	}
	else {
		HAL_TIM_Base_Start(htim);
	}
}

void Timers_Stop(TIM_HandleTypeDef* htim) {
	if(htim == &TIMERS_MOTOR_TIMER) {
		HAL_TIM_OC_Stop_IT(htim, TIM_CHANNEL_1);
	}
	else {
		HAL_TIM_Base_Stop(htim);
	}
}
