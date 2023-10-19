/*
 * buttons.h
 *
 *  Created on: Oct 18, 2023
 *      Author: Mateusz Chwast
 */

#ifndef PROGRAM_BUTTONS_H_
#define PROGRAM_BUTTONS_H_


typedef void(*Buttons_Callback)(void);

typedef void* Button;


extern Button button0;
extern Button button1;


void Buttons_Manage(void);

void Buttons_SetCallbacks(Button* button, Buttons_Callback onPressed, Buttons_Callback onLongPressed,
		Buttons_Callback onReleased, Buttons_Callback onReleasedLate);

/*
 * if onPressed is used, it may be sometimes useful to ignore onReleased callback.
 * this can be done by setting onReleasedLate to EmptyCallback instead of NULL - this
 * prevents onRelease callback call in case of longPressed event
 */
void Buttons_EmptyCallback(void);


#endif /* PROGRAM_BUTTONS_H_ */
