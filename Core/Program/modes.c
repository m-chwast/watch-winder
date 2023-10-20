/*
 * modes.c
 *
 *  Created on: Oct 19, 2023
 *      Author: Mateusz Chwast
 */

#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "modes.h"
#include "console.h"
#include "watch_winder.h"


#define TURNS_PER_DAY_A 600
#define TURNS_PER_DAY_B 900
#define TURNS_PER_DAY_C 1200
#define TURNS_PER_DAY_D 1500

#define RPM_A 3
#define RPM_B 4
#define RPM_C 5
#define RPM_D 5
#define RPM_TO_RPH(RPM) ((RPM) * 60)

#define RPH_CONSTANT(TPD) (ceil((TPD) / 24))

#define FULL_DAY_SECONDS (24 * 60 * 60)
#define CYCLE_TIME_A (90 * 60)
#define CYCLE_TIME_B (75 * 60)
#define CYCLE_TIME_C (60 * 60)
#define CYCLE_TIME_D (45 * 60)

#define TURNS_PER_CYCLE_A (ceil(TURNS_PER_DAY_A * CYCLE_TIME_A / FULL_DAY_SECONDS))
#define TURNS_PER_CYCLE_B (ceil(TURNS_PER_DAY_B * CYCLE_TIME_B / FULL_DAY_SECONDS))
#define TURNS_PER_CYCLE_C (ceil(TURNS_PER_DAY_C * CYCLE_TIME_C / FULL_DAY_SECONDS))
#define TURNS_PER_CYCLE_D (ceil(TURNS_PER_DAY_D * CYCLE_TIME_D / FULL_DAY_SECONDS))


typedef struct {
	uint8_t value;
	uint8_t tmpValue;
	bool editActive;
} Mode;


static Mode mainMode;
static Mode timingMode;


static bool IsConstantMode(void) {
	if(Modes_Main_Get() == MAIN_MODE_CONSTANT_CLOCKWISE ||
			Modes_Main_Get() == MAIN_MODE_ANTICLOCKWISE) {
		return true;
	}
	return false;
}

uint32_t Modes_GetCyclePeriod(void) {
	if(IsConstantMode()) {
		return FULL_DAY_SECONDS;
	}

	switch(Modes_Timing_Get()) {
		case TIMING_MODE_A: {
			return CYCLE_TIME_A;
		}
		case TIMING_MODE_B: {
			return CYCLE_TIME_B;
		}
		case TIMING_MODE_C: {
			return CYCLE_TIME_C;
		}
		case TIMING_MODE_D: {
			return CYCLE_TIME_D;
		}
		default: {
			return 0;
		}
	}
}

uint32_t Modes_GetRevolutionsPerCycle(void) {
	switch(Modes_Timing_Get()) {
		case TIMING_MODE_A: {
			return TURNS_PER_CYCLE_A;
		}
		case TIMING_MODE_B: {
			return TURNS_PER_CYCLE_B;
		}
		case TIMING_MODE_C: {
			return TURNS_PER_CYCLE_C;
		}
		case TIMING_MODE_D: {
			return TURNS_PER_CYCLE_D;
		}
		default: {
			return 0;
		}
	}
}

uint32_t Modes_GetRevolutionsPerHour(void) {
	if(IsConstantMode()) {
		switch(Modes_Timing_Get()) {
			case TIMING_MODE_A: {
				return RPH_CONSTANT(TURNS_PER_DAY_A);
			}
			case TIMING_MODE_B: {
				return RPH_CONSTANT(TURNS_PER_DAY_B);
			}
			case TIMING_MODE_C: {
				return RPH_CONSTANT(TURNS_PER_DAY_C);
			}
			case TIMING_MODE_D: {
				return RPH_CONSTANT(TURNS_PER_DAY_D);
			}
			default: {
				return 0;
			}
		}
	}

	switch(Modes_Timing_Get()) {
		case TIMING_MODE_A: {
			return RPM_TO_RPH(RPM_A);
		}
		case TIMING_MODE_B: {
			return RPM_TO_RPH(RPM_B);
		}
		case TIMING_MODE_C: {
			return RPM_TO_RPH(RPM_C);
		}
		case TIMING_MODE_D: {
			return RPM_TO_RPH(RPM_D);
		}
		default: {
			return 0;
		}
	}
}

Modes_MainMode Modes_Main_Get(void) {
	return mainMode.value;
}

uint8_t Modes_Main_GetIncrementedTmp(void) {
	return mainMode.tmpValue + 1;
}

bool Modes_Main_GetEditActive(void) {
	return mainMode.editActive;
}

Modes_TimingMode Modes_Timing_Get(void) {
	return timingMode.value;
}

uint8_t Modes_Timing_GetIncrementedTmp(void) {
	return timingMode.tmpValue + 1;
}

bool Modes_Timing_GetEditActive(void) {
	return timingMode.editActive;
}


//button callbacks:

void Modes_Main_ToggleEditActive(void) {
	mainMode.editActive = !mainMode.editActive;
	Console_LogValLn("Main Mode edit: ", Modes_Main_GetEditActive());
	if(Modes_Main_GetEditActive()) {
		mainMode.tmpValue = mainMode.value;
	}
	else {
		if(mainMode.value != mainMode.tmpValue) {
			Console_LogLn("Main Mode changed");
			mainMode.value = mainMode.tmpValue;
			WatchWinder_Refresh();
		}
	}
}

void Modes_Main_SetNext(void) {
	if(mainMode.editActive == false) {
		return;
	}
	mainMode.tmpValue++;
	mainMode.tmpValue %= MAIN_MODE_COUNT;
	Console_LogValLn("Main Mode: ", mainMode.tmpValue);
}

void Modes_Timing_ToggleEditActive(void) {
	timingMode.editActive = !timingMode.editActive;
	Console_LogValLn("Timing Mode edit: ", Modes_Timing_GetEditActive());
	if(Modes_Timing_GetEditActive()) {
		timingMode.tmpValue = timingMode.value;
	}
	else {
		if(timingMode.value != timingMode.tmpValue) {
			Console_LogLn("Timing Mode changed");
			timingMode.value = timingMode.tmpValue;
			WatchWinder_Refresh();
		}
	}
}

void Modes_Timing_SetNext(void) {
	if(timingMode.editActive == false) {
		return;
	}
	timingMode.tmpValue++;
	timingMode.tmpValue %= TIMING_MODE_COUNT;
	Console_LogValLn("Timing Mode: ", timingMode.tmpValue);
}
