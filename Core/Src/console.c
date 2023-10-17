/*
 * console.c
 *
 *  Created on: Oct 17, 2023
 *      Author: Mateusz Chwast
 */

#include <string.h>
#include "console.h"
#include "main.h"


extern UART_HandleTypeDef huart1;


void Console_Log(const char* msg) {
	HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}
