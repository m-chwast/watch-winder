/*
 * leds.c
 *
 *  Created on: Oct 19, 2023
 *      Author: Mateusz Chwast
 */


#include "main.h"

typedef enum {
	LED_STATE_IDLE,
	LED_STATE_INIT,
} Led_State;

typedef enum {
	LED_TYPE_STD,
} Led_Type;

typedef struct {
	GPIO_TypeDef* const gpio;
	const uint16_t pin;
	Led_State state;
	Led_Type type;
	uint32_t time;
} Led;

static void LedManage(Led* led);

void Leds_Manage(void) {

}

static void LedManage(Led* led) {

}
