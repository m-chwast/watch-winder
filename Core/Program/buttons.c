/*
 * buttons.c
 *
 *  Created on: Oct 18, 2023
 *      Author: Mateusz Chwast
 */

#include "main.h"
#include "buttons.h"
#include "console.h"


#define DEBOUNCING_TIME_MS 25


typedef enum {
	BUTTON_STATE_RELEASED,
	BUTTON_STATE_DEBOUNCING_PRESSED,
	BUTTON_STATE_PRESSED,
	BUTTON_STATE_DEBOUNCING_RELEASED,
} ButtonState;

typedef struct {
	ButtonState state;
	const uint8_t id;
	const uint16_t pin;
	GPIO_TypeDef* const gpio;
	uint32_t debouncingStartTime;
} Button;


static Button button0 = { .pin = BUTTON0_Pin, .gpio = BUTTON0_GPIO_Port, .id = 0 };
static Button button1 = { .pin = BUTTON1_Pin, .gpio = BUTTON1_GPIO_Port, .id = 1 };


static void ButtonManage(Button* button);
static inline void PrintButtonStatus(const Button* button, const char* status);


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

}

void Buttons_Manage(void) {
	ButtonManage(&button0);
	ButtonManage(&button1);
}

static void ButtonManage(Button* button) {
	switch(button->state) {
		case BUTTON_STATE_RELEASED: {
			if(HAL_GPIO_ReadPin(button->gpio, button->pin) == GPIO_PIN_SET) {
				//buttons are active low, so this is idle condition
				break;
			}
			button->debouncingStartTime = HAL_GetTick();
			button->state = BUTTON_STATE_DEBOUNCING_PRESSED;
			PrintButtonStatus(button, "pressed");
			break;
		}
		case BUTTON_STATE_DEBOUNCING_PRESSED: {
			if(HAL_GetTick() - button->debouncingStartTime > DEBOUNCING_TIME_MS) {
				button->state = BUTTON_STATE_PRESSED;
			}
			break;
		}
		case BUTTON_STATE_PRESSED: {
			break;
		}
		case BUTTON_STATE_DEBOUNCING_RELEASED: {
			if(HAL_GetTick() - button->debouncingStartTime > DEBOUNCING_TIME_MS) {
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
