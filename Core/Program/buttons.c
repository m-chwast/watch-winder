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
	BUTTON_STATE_DEBOUNCING,
	BUTTON_STATE_RELEASED,
} ButtonState;

typedef struct {
	ButtonState state;
} Button;


static Button button0;
static Button button1;


static void ButtonManage(Button* button);


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

}

void Buttons_Manage(void) {
	ButtonManage(&button0);
	ButtonManage(&button1);
}

static void ButtonManage(Button* button) {

}
