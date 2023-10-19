/*
 * modes.h
 *
 *  Created on: Oct 19, 2023
 *      Author: Mateusz Chwast
 */

#ifndef PROGRAM_MODES_H_
#define PROGRAM_MODES_H_

#include <stdbool.h>
#include <stdint.h>


typedef enum {
	MAIN_MODE_CLOCKWISE,
	MAIN_MODE_ANTICLOCKWISE,
	MAIN_MODE_MIX_A,
	MAIN_MODE_MIX_B,
	MAIN_MODE_CONSTANT_CLOCKWISE,
	MAIN_MODE_CONSTANT_ANTICLOCKWISE,
	MAIN_MODE_COUNT
} Modes_MainMode;

typedef enum {
	TIMING_MODE_A,
	TIMING_MODE_B,
	TIMING_MODE_C,
	TIMING_MODE_D,
	TIMING_MODE_COUNT
} Modes_TimingMode;


//gets the period of the whole cycle in current combination of modes in seconds
uint32_t Modes_GetCyclePeriod(void);
uint32_t Modes_GetRevolutionsPerCycle(void);
uint32_t Modes_GetRevolutionsPerHour(void);


Modes_MainMode Modes_Main_Get(void);
uint8_t Modes_Main_GetIncremented(void);
bool Modes_Main_GetEditActive(void);

Modes_TimingMode Modes_Timing_Get(void);
uint8_t Modes_Timing_GetIncremented(void);
bool Modes_Timing_GetEditActive(void);


void Modes_Main_ToggleEditActive(void);
void Modes_Main_SetNext(void);

void Modes_Timing_ToggleEditActive(void);
void Modes_Timing_SetNext(void);


#endif /* PROGRAM_MODES_H_ */
