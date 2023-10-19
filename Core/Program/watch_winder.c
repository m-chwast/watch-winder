/*
 * watch_winder.c
 *
 *  Created on: Oct 20, 2023
 *      Author: Mateusz Chwast
 */

#include <stdbool.h>
#include "motor.h"
#include "modes.h"


typedef enum {
	WW_STATE_IDLE,
	WW_STATE_ROTATION_INIT,
} WatchWinderState;

static struct WatchWinder {
	WatchWinderState state;
	bool isRotationRequested;
} watchWinder;


void WatchWinder_Manage(void) {
	/*
	if(watchWinder.isRotationRequested && Motor_IsRunning() == false) {
		watchWinder.isRotationRequested = false;
		Motor_SetSpeed(Modes_GetRevolutionsPerHour());
		uint32_t revolutions = MOTOR_REVOLUTIONS_TO_DEGREES(Modes_GetRevolutionsPerCycle());
		Motor_SetMovement(revolutions, MOTOR_DIR_CLOCKWISE);
	}
	*/

	switch(watchWinder.state) {
		case WW_STATE_IDLE: {
			if(watchWinder.isRotationRequested && !Motor_IsRunning()) {
				watchWinder.isRotationRequested = false;
				watchWinder.state = WW_STATE_ROTATION_INIT;
			}
			break;
		}
		case WW_STATE_ROTATION_INIT: {
			break;
		}
	}
}

void WatchWinder_SetRotationRequest(void) {
	watchWinder.isRotationRequested = true;
}
