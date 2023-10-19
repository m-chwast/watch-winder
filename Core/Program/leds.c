/*
 * leds.c
 *
 *  Created on: Oct 19, 2023
 *      Author: Mateusz Chwast
 */


#include <stdbool.h>
#include "main.h"


#define INIT_BLINK_TIME_MS 1000
#define INIT_BLINKS_COUNT 2


typedef enum {
	LED_STATE_IDLE,
	LED_STATE_INIT_START,
	LED_STATE_INIT,
	LED_STATE_INIT_FINISH,
} Led_State;

typedef enum {
	LED_TYPE_STD,
	LED_TYPE_COUNTER,
} Led_Type;

typedef struct {
	GPIO_TypeDef* const gpio;
	const uint16_t pin;
	Led_State state;
	Led_Type type;
	uint32_t time;	//general-purpose led time var
	uint8_t cnt;	//general-purpose led counter
} Led;

typedef struct {
	Led led;
} LedCounter;


LedCounter led0 = { .led = { .gpio = LED0_GPIO_Port, .pin = LED0_Pin, .type = LED_TYPE_COUNTER } };
LedCounter led1 = { .led = { .gpio = LED1_GPIO_Port, .pin = LED1_Pin, .type = LED_TYPE_COUNTER } };


static void LedManage(Led* led);
static inline void LedWrite(Led* led, bool ledOn);
static inline void LedToggle(Led* led);


void Leds_Manage(void) {
	LedManage((Led*)&led0);
	LedManage((Led*)&led1);
}

static void LedManage(Led* led) {
	switch(led->state) {
		case LED_STATE_IDLE: {
			break;
		}
		case LED_STATE_INIT_START: {
			led->cnt = 0;
			led->time = HAL_GetTick();
			led->state = LED_STATE_INIT;
			LedWrite(led, false);
			break;
		}
		case LED_STATE_INIT: {
			if(HAL_GetTick() - led->time <= INIT_BLINK_TIME_MS) {
				break;
			}

			if(led->cnt / 2 >= INIT_BLINKS_COUNT) {
				led->state = LED_STATE_INIT_FINISH;
				break;
			}

			LedToggle(led);
			led->time = HAL_GetTick();
			led->cnt++;
			break;
		}
		case LED_STATE_INIT_FINISH: {
			led->cnt = 0;
			led->time = 0;
			led->state = LED_STATE_IDLE;
			break;
		}
	}
}

void Leds_BeginInit(Led* led) {
	led->state = LED_STATE_INIT_START;
}

static inline void LedWrite(Led* led, bool ledOn) {
	//leds are assumed to be active-low
	HAL_GPIO_WritePin(led->gpio, led->pin, !ledOn);
}

static inline void LedToggle(Led* led) {
	HAL_GPIO_TogglePin(led->gpio, led->pin);
}
