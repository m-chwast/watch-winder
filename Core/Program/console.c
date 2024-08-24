/*
 * console.c
 *
 *  Created on: Oct 17, 2023
 *      Author: Mateusz Chwast
 */

#include <string.h>
#include <stdlib.h>
#include "console.h"
#include "main.h"


extern UART_HandleTypeDef huart1;

static inline void PrintLine(void) {
	Console_Log("\r\n");
}

void Console_Log(const char* msg) {
	if(msg == NULL) {
		return;
	}
	HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}

void Console_LogLn(const char* msg) {
	Console_Log(msg);
	PrintLine();
}

void Console_LogVal(const char* msg, int32_t val) {
	Console_Log(msg);

	char buff[12] = { 0 };
	itoa(val, buff, 10);
	Console_Log(buff);
}

void Console_LogValLn(const char* msg, int32_t val) {
	Console_LogVal(msg, val);
	PrintLine();
}

void Console_LogError(const char* msg) {
	Console_Log("[ERROR]: ");
	Console_Log(msg);
}

void Console_LogErrorLn(const char* msg) {
	Console_LogError(msg);
	PrintLine();
}
