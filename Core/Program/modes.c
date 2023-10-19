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


Modes_MainMode Modes_Main_Get(void) {
	return mainMode.value;
}

bool Modes_Main_GetEditActive(void) {
	return mainMode.editActive;
}

Modes_TimingMode Modes_Timing_Get(void) {
	return timingMode.value;
}

bool Modes_Timing_GetEditActive(void) {
	return timingMode.editActive;
}


//button callbacks:

void Modes_Main_ToggleEditActive(void) {
	mainMode.editActive = !mainMode.editActive;
	Console_LogValLn("Main Mode edit: ", mainMode.editActive);
}

void Modes_Main_SetNext(void) {
	if(mainMode.editActive == false) {
		return;
	}
	mainMode.value++;
	mainMode.value %= MAIN_MODE_COUNT;
	Console_LogValLn("Main Mode: ", mainMode.value);
}

void Modes_Timing_ToggleEditActive(void) {
	timingMode.editActive = !timingMode.editActive;
	Console_LogValLn("Timing Mode edit: ", timingMode.editActive);
}

void Modes_Timing_SetNext(void) {
	if(timingMode.editActive == false) {
		return;
	}
	timingMode.value++;
	timingMode.value %= TIMING_MODE_COUNT;
	Console_LogValLn("Timing Mode: ", timingMode.value);
}
