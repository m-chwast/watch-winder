/*
 * console.h
 *
 *  Created on: Oct 17, 2023
 *      Author: Mateusz Chwast
 */

#ifndef INC_CONSOLE_H_
#define INC_CONSOLE_H_

#include <stdint.h>

void Console_Log(const char* msg);
void Console_LogLn(const char* msg);

void Console_LogVal(const char* msg, int32_t val);
void Console_LogValLn(const char* msg, int32_t val);


#endif /* INC_CONSOLE_H_ */
