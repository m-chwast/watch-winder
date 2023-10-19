/*
 * leds.c
 *
 *  Created on: Oct 19, 2023
 *      Author: Mateusz Chwast
 */


#include <stdbool.h>
#include "main.h"
#include "console.h"


#define INIT_BLINK_TIME_MS 1000
#define INIT_BLINKS_COUNT 2

#define LED_COUNTER_BLINK_PERIOD_MS 200
#define LED_COUNTER_FULL_PERIOD_MS (10 * LED_COUNTER_BLINK_PERIOD_MS)


typedef enum {
	LED_STATE_IDLE,
	LED_STATE_INIT_START,
	LED_STATE_INIT,
} Led_State;

typedef enum {
	LED_COUNTER_STATE_IDLE,
	LED_COUNTER_STATE_BLINK,
	LED_COUNTER_STATE_AWAIT_PERIOD,
} LedCounter_State;

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
	const uint8_t id;
} Led;

typedef bool(*Leds_Predicate)(void);
typedef uint8_t(*Leds_ValueGetter)(void);

typedef struct {
	Led led;

	LedCounter_State counterState;
	Leds_Predicate counterEnable;
	Leds_ValueGetter counterValue;
} LedCounter;


LedCounter led0 = { .led = { .id = 0, .gpio = LED0_GPIO_Port, .pin = LED0_Pin, .type = LED_TYPE_COUNTER } };
LedCounter led1 = { .led = { .id = 1, .gpio = LED1_GPIO_Port, .pin = LED1_Pin, .type = LED_TYPE_COUNTER } };


static void LedManage(Led* led);
static void LedCounterManage(LedCounter* led);
static void PrintLedStatus(Led* led, const char* status);
static inline void LedWrite(Led* led, bool ledOn);
static inline void LedToggle(Led* led);


void Leds_Manage(void) {
	LedManage((Led*)&led0);
	LedManage((Led*)&led1);
}

static void LedManage(Led* led) {
	switch(led->state) {
		case LED_STATE_IDLE: {
			//idle state calls more specific managers
			switch(led->type) {
				case LED_TYPE_STD: {
					break;
				}
				case LED_TYPE_COUNTER: {
					LedCounterManage((LedCounter*)led);
					break;
				}
			}
			break;
		}
		case LED_STATE_INIT_START: {
			PrintLedStatus(led, "init");
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
				//end of initialization
				led->cnt = 0;
				led->time = 0;
				led->state = LED_STATE_IDLE;
				PrintLedStatus(led, "init done");
				break;
			}

			LedToggle(led);
			led->time = HAL_GetTick();
			led->cnt++;
			break;
		}
	}
}

static void LedCounterManage(LedCounter* led) {
	if(led->counterEnable == NULL || led->counterValue == NULL) {
		//no callbacks
		return;
	}

	uint32_t* time = &led->led.time;
	uint8_t* cnt = &led->led.cnt;

	switch(led->counterState) {
		case LED_COUNTER_STATE_IDLE: {
			if(led->counterEnable()) {
				LedWrite((Led*)led, true);
				*time = HAL_GetTick();
				*cnt = 0;
				led->counterState = LED_COUNTER_STATE_BLINK;
			}
			break;
		}
		case LED_COUNTER_STATE_BLINK: {
			break;
		}
		case LED_COUNTER_STATE_AWAIT_PERIOD: {
			break;
		}
	}
}

void Leds_BeginInit(Led* led) {
	led->state = LED_STATE_INIT_START;
}

void Leds_LedCounterSetup(LedCounter* led, Leds_Predicate counterEnable, Leds_ValueGetter counterValue) {
	led->counterEnable = counterEnable;
	led->counterValue = counterValue;
}

static void PrintLedStatus(Led* led, const char* status) {
	Console_LogVal("Led ", led->id);
	Console_Log(" ");
	Console_LogLn(status);
}

static inline void LedWrite(Led* led, bool ledOn) {
	//leds are assumed to be active-low
	HAL_GPIO_WritePin(led->gpio, led->pin, !ledOn);
}

static inline void LedToggle(Led* led) {
	HAL_GPIO_TogglePin(led->gpio, led->pin);
}
