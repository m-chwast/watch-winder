/*
 * watch_winder.c
 *
 *  Created on: Oct 20, 2023
 *      Author: Mateusz Chwast
 */

#include <stdbool.h>
#include "motor.h"
#include "modes.h"
#include "console.h"
#include "rtc.h"


typedef enum {
	WW_STATE_IDLE,
	WW_STATE_ROTATION_INIT,
	WW_STATE_ROTATION,
	WW_STATE_CONSTANT_ROTATION,
	WW_STATE_STOP,
	WW_STATE_STOPPING,
} WatchWinderState;

static struct WatchWinder {
	WatchWinderState state;
	bool isRotationRequested;
	bool previousCycleWasClockwise;
	uint16_t turnsRemainingClockwise;
	uint16_t turnsRemainingCounterclockwise;
} watchWinder;


void WatchWinder_Manage(void) {
	switch(watchWinder.state) {
		case WW_STATE_IDLE: {
			if(watchWinder.isRotationRequested && !Motor_IsRunning()) {
				watchWinder.isRotationRequested = false;
				watchWinder.state = WW_STATE_ROTATION_INIT;
			}
			break;
		}
		case WW_STATE_ROTATION_INIT: {
			Motor_SetSpeed(Modes_GetRevolutionsPerHour());
			uint16_t turnsClockwise = 0, turnsCounterclockwise = 0;
			uint16_t turnsRequested = Modes_GetRevolutionsPerCycle();
			bool setConstantRotation = false;
			switch(Modes_Main_Get()) {
				case MAIN_MODE_CLOCKWISE: {
					turnsClockwise = turnsRequested;
					watchWinder.previousCycleWasClockwise = false;
					break;
				}
				case MAIN_MODE_ANTICLOCKWISE: {
					turnsCounterclockwise = turnsRequested;
					watchWinder.previousCycleWasClockwise = true;
					break;
				}
				case MAIN_MODE_MIX_A: {
					if(watchWinder.previousCycleWasClockwise) {
						turnsCounterclockwise = turnsRequested;
					}
					else {
						turnsClockwise = turnsRequested;
					}
					watchWinder.previousCycleWasClockwise = !watchWinder.previousCycleWasClockwise;
					break;
				}
				case MAIN_MODE_MIX_B: {
					turnsClockwise = turnsRequested / 2;
					turnsCounterclockwise = turnsRequested / 2;
					if(turnsRequested % 2) {
						if(watchWinder.previousCycleWasClockwise) {
							turnsClockwise++;
						}
						else {
							turnsCounterclockwise++;
						}
					}
					watchWinder.previousCycleWasClockwise = !watchWinder.previousCycleWasClockwise;
					break;
				}
				case MAIN_MODE_CONSTANT_CLOCKWISE: {
					watchWinder.previousCycleWasClockwise = true;
					setConstantRotation = true;
					break;
				}
				case MAIN_MODE_CONSTANT_ANTICLOCKWISE: {
					watchWinder.previousCycleWasClockwise = false;
					setConstantRotation = true;
					break;
				}
				default: {
					break;
				}
			}
			Console_LogValLn("Starting rotation. Turns: ", turnsRequested);
			watchWinder.turnsRemainingClockwise = turnsClockwise;
			watchWinder.turnsRemainingCounterclockwise = turnsCounterclockwise;
			if(setConstantRotation) {
				watchWinder.state = WW_STATE_CONSTANT_ROTATION;
			}
			else {
				watchWinder.state = WW_STATE_ROTATION;
			}
			break;
		}
		case WW_STATE_ROTATION: {
			if(Motor_IsRunning()) {
				break;
			}

			uint16_t turns = 0;
			Motor_Dir dir;

			//this is the current cycle info, since it was updated in init state
			if(watchWinder.previousCycleWasClockwise) {
				if(watchWinder.turnsRemainingClockwise) {
					turns = watchWinder.turnsRemainingClockwise;
					dir = MOTOR_DIR_CLOCKWISE;
				}
				else if(watchWinder.turnsRemainingCounterclockwise) {
					turns = watchWinder.turnsRemainingCounterclockwise;
					dir = MOTOR_DIR_ANTICLOCKWISE;
				}
			}
			else {
				if(watchWinder.turnsRemainingCounterclockwise) {
					turns = watchWinder.turnsRemainingCounterclockwise;
					dir = MOTOR_DIR_ANTICLOCKWISE;
				}
				else if(watchWinder.turnsRemainingClockwise) {
					turns = watchWinder.turnsRemainingClockwise;
					dir = MOTOR_DIR_CLOCKWISE;
				}
			}

			if(turns == 0) {
				Console_LogLn("Rotation finished");
				watchWinder.state = WW_STATE_IDLE;
			}
			else {
				Motor_SetMovement(MOTOR_REVOLUTIONS_TO_DEGREES(turns), dir);
			}
			break;
		}
		case WW_STATE_CONSTANT_ROTATION: {
			if(Motor_IsRunning()) {
				break;
			}

			Motor_Dir dir = watchWinder.previousCycleWasClockwise ? MOTOR_DIR_CLOCKWISE : MOTOR_DIR_ANTICLOCKWISE;
			Motor_SetMovementInfinite(dir);
			break;
		}
		case WW_STATE_STOP: {
			if(Motor_IsRunning()) {
				Motor_RequestStop();
			}
			watchWinder.turnsRemainingClockwise = 0;
			watchWinder.turnsRemainingCounterclockwise = 0;
			watchWinder.state = WW_STATE_STOPPING;
			break;
		}
		case WW_STATE_STOPPING: {
			if(Motor_IsRunning() == false) {
				watchWinder.state = WW_STATE_IDLE;
			}
			break;
		}
	}
}

void WatchWinder_SetRotationRequest(void) {
	watchWinder.isRotationRequested = true;
}

void WatchWinder_Refresh(void) {
	WatchWinder_SetRotationRequest();
	RTC_Init();
	watchWinder.state = WW_STATE_STOP;
}
