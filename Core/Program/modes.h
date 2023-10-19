/*
 * modes.h
 *
 *  Created on: Oct 19, 2023
 *      Author: Mateusz Chwast
 */

#ifndef PROGRAM_MODES_H_
#define PROGRAM_MODES_H_

typedef enum {
	MAIN_MODE_COUNT = 4
} Modes_MainMode;

typedef enum {
	TIMING_MODE_COUNT = 3
} Modes_TimingMode;


void Modes_Main_SetEditActive(void);
void Modes_Main_SetEditInactive(void);
void Modes_Main_SetNext(void);

void Modes_Timing_SetEditActive(void);
void Modes_Timing_SetEditInactive(void);
void Modes_Timing_SetNext(void);


#endif /* PROGRAM_MODES_H_ */
