/*
 * leds.h
 *
 *  Created on: Oct 19, 2023
 *      Author: Mateusz Chwast
 */

#ifndef PROGRAM_LEDS_H_
#define PROGRAM_LEDS_H_

#include <stdbool.h>
#include <stdint.h>


typedef struct {} Led;
typedef Led LedCounter;

typedef bool(*Leds_Predicate)(void);
typedef uint32_t(*Leds_ValueGetter)(void);


extern LedCounter led0;
extern LedCounter led1;


void Leds_Manage(void);

void Leds_BeginInit(Led* led);

void Leds_LedCounterSetup(LedCounter* led, Leds_Predicate counterEnable, Leds_ValueGetter counterValue);


#endif /* PROGRAM_LEDS_H_ */
