/*
 * rtc.h
 *
 *  Created on: Oct 19, 2023
 *      Author: Mateusz Chwast
 */

#ifndef PROGRAM_RTC_H_
#define PROGRAM_RTC_H_

#include <stdint.h>

void RTC_SetupAlarmCallback(void(*cb)(void));
void RTC_SetupPeriodCallback(uint32_t(*periodSecondsCb)(void));

void RTC_Init(void);
void RTC_SetNextAlarm(void);

void RTC_Manage(void);


#endif /* PROGRAM_RTC_H_ */
