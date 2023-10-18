/*
 * buttons.c
 *
 *  Created on: Oct 18, 2023
 *      Author: Mateusz Chwast
 */

#include "buttons.h"
#include "main.h"


typedef enum {
	BUTTON_STATE_IDLE,
	BUTTON_STATE_PRESSED,
	BUTTON_STATE_DEBOUNCING_PRESSED,
	BUTTON_STATE_RELEASED,
	BUTTON_STATE_DEBOUNCING_RELEASED,
} ButtonState;

typedef struct {
	ButtonState state;
	const uint16_t pin;
	GPIO_TypeDef* const gpio;
} Button;


static Button button0 = { .pin = BUTTON0_Pin, .gpio = BUTTON0_GPIO_Port };
static Button button1 = { .pin = BUTTON1_Pin, .gpio = BUTTON1_GPIO_Port };


static void ButtonManage(Button* button);


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

}

void Buttons_Manage(void) {
	ButtonManage(&button0);
	ButtonManage(&button1);
}

static void ButtonManage(Button* button) {
	switch(button->state) {
		case BUTTON_STATE_IDLE: {
			break;
		}
		case BUTTON_STATE_PRESSED: {
			break;
		}
		case BUTTON_STATE_DEBOUNCING_PRESSED: {
			break;
		}
		case BUTTON_STATE_RELEASED: {
			break;
		}
		case BUTTON_STATE_DEBOUNCING_RELEASED: {
			break;
		}
	}
}
