/*
 * buttons.c
 *
 *  Created on: Oct 18, 2023
 *      Author: Mateusz Chwast
 */

#include <stdbool.h>
#include "main.h"
#include "console.h"


#define DEBOUNCING_TIME_MS 25
#define LONG_PRESSING_TIME_MS 2500


typedef enum {
	BUTTON_STATE_RELEASED,
	BUTTON_STATE_DEBOUNCING_PRESSED,
	BUTTON_STATE_PRESSED,
	BUTTON_STATE_DEBOUNCING_RELEASED,
} ButtonState;

typedef void(*Buttons_Callback)(void);

typedef struct {
	ButtonState state;
	const uint8_t id;
	const uint16_t pin;
	GPIO_TypeDef* const gpio;
	uint32_t startTime;	//used for debouncing and for long pressing check

	Buttons_Callback onPressed;
	Buttons_Callback onReleased;
	Buttons_Callback onReleasedLate;	//called when released after long pressing
} Button;


Button button0 = { .pin = BUTTON0_Pin, .gpio = BUTTON0_GPIO_Port, .id = 0 };
Button button1 = { .pin = BUTTON1_Pin, .gpio = BUTTON1_GPIO_Port, .id = 1 };


static void ButtonManage(Button* button);
static inline void PrintButtonStatus(const Button* button, const char* status);


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

}

void Buttons_SetCallbacks(Button* button, Buttons_Callback onPressed, Buttons_Callback onReleased, Buttons_Callback onReleasedLate) {
	button->onPressed = onPressed;
	button->onReleased = onReleased;
	button->onReleasedLate = onReleasedLate;
}

void Buttons_Manage(void) {
	ButtonManage(&button0);
	ButtonManage(&button1);
}

static void ButtonManage(Button* button) {
	switch(button->state) {
		case BUTTON_STATE_RELEASED: {
			if(HAL_GPIO_ReadPin(button->gpio, button->pin) == GPIO_PIN_SET) {
				//buttons are active low, so this is released condition -> do nothing
				break;
			}

			button->startTime = HAL_GetTick();
			button->state = BUTTON_STATE_DEBOUNCING_PRESSED;
			PrintButtonStatus(button, "pressed");

			if(button->onPressed) {
				button->onPressed();
			}
			break;
		}
		case BUTTON_STATE_DEBOUNCING_PRESSED: {
			if(HAL_GetTick() - button->startTime > DEBOUNCING_TIME_MS) {
				button->startTime = HAL_GetTick();
				button->state = BUTTON_STATE_PRESSED;
			}
			break;
		}
		case BUTTON_STATE_PRESSED: {
			if(HAL_GPIO_ReadPin(button->gpio, button->pin) == GPIO_PIN_RESET) {
				//buttons are active low, so this is pressed condition -> do nothing
				break;
			}

			bool longPressing = HAL_GetTick() - button->startTime > LONG_PRESSING_TIME_MS;

			button->startTime = HAL_GetTick();
			button->state = BUTTON_STATE_DEBOUNCING_RELEASED;

			if(longPressing) {
				PrintButtonStatus(button, "released late");
			}
			else {
				PrintButtonStatus(button, "released");
			}

			//call long released or released
			if(longPressing && button->onReleasedLate) {
				button->onReleasedLate();
			}
			else if(button->onReleased) {
				button->onReleased();
			}
			break;
		}
		case BUTTON_STATE_DEBOUNCING_RELEASED: {
			if(HAL_GetTick() - button->startTime > DEBOUNCING_TIME_MS) {
				button->state = BUTTON_STATE_RELEASED;
			}
			break;
		}
	}
}

static inline void PrintButtonStatus(const Button* button, const char* status) {
	Console_LogVal("Button ", button->id);
	Console_Log(" ");
	Console_LogLn(status);
}
