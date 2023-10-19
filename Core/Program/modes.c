/*
 * modes.c
 *
 *  Created on: Oct 19, 2023
 *      Author: Mateusz Chwast
 */

#include <stdbool.h>
#include <stdint.h>
#include "modes.h"
#include "console.h"


typedef struct {
	uint8_t value;
	bool editActive;
} Mode;


static Mode mainMode;
static Mode timingMode;


//button callbacks:

void Modes_Main_SetEditActive(void) {
	mainMode.editActive = true;
}

void Modes_Main_SetEditInactive(void) {
	mainMode.editActive = false;
}

void Modes_Main_SetNext(void) {
	if(mainMode.editActive == false) {
		return;
	}
	mainMode.value++;
	mainMode.value %= MAIN_MODE_COUNT;
	Console_LogValLn("Main Mode: ", mainMode.value);
}

void Modes_Timing_SetEditActive(void) {
	timingMode.editActive = true;
}

void Modes_Timing_SetEditInactive(void) {
	timingMode.editActive = false;
}

void Modes_Timing_SetNext(void) {
	if(timingMode.editActive == false) {
		return;
	}
	timingMode.value++;
	timingMode.value %= TIMING_MODE_COUNT;
	Console_LogValLn("Timing Mode: ", timingMode.value);
}
