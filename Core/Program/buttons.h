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

void Buttons_SetCallbacks(Button* button, Buttons_Callback onPressed, Buttons_Callback onReleased, Buttons_Callback onReleasedLate);


#endif /* PROGRAM_BUTTONS_H_ */
