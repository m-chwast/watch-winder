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
	MAIN_MODE_COUNT = 4
} Modes_MainMode;

typedef enum {
	TIMING_MODE_COUNT = 3
} Modes_TimingMode;


//gets the period of the whole cycle in current combination of modes in seconds
uint32_t Modes_GetCyclePeriod(void);

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
