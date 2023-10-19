/*
 * leds.h
 *
 *  Created on: Oct 19, 2023
 *      Author: Mateusz Chwast
 */

#ifndef PROGRAM_LEDS_H_
#define PROGRAM_LEDS_H_


typedef struct {} Led;
typedef Led LedCounter;

extern LedCounter led0;
extern LedCounter led1;


void Leds_Manage(void);

void Leds_BeginInit(Led* led);


#endif /* PROGRAM_LEDS_H_ */
