/*
 * motor.c
 *
 *  Created on: Oct 17, 2023
 *      Author: Mateusz Chwast
 */

#include <assert.h>
#include "motor.h"
#include "main.h"
#include "console.h"
#include "timers.h"


//set only one mode to true
#define DRIVE_WAVE false
#define DRIVE_FULL_STEP false
#define DRIVE_HALF_STEP true

#define START_RAMP_DEGREES 120
#define FINISH_RAMP_DEGREES 90
#define RAMP_INIT_SPEED_PERCENT 25

#define BASE_STEPS_PER_REVOLUTION 2048

#if !DRIVE_HALF_STEP
#define FINAL_DRIVE_STEP (4 - 1)
#define MOTOR_STEPS_PER_REVOLUTION (BASE_STEPS_PER_REVOLUTION)
#else
#define FINAL_DRIVE_STEP (8 - 1)
#define MOTOR_STEPS_PER_REVOLUTION (2 * BASE_STEPS_PER_REVOLUTION)
#endif

#define DEGREES_TO_STEPS(DEG) ((DEG) * MOTOR_STEPS_PER_REVOLUTION / 360)


static void Motor_Step(Motor_Dir dir);
static void MotorPinsWrite(uint32_t setMask);
static inline void MotorPowerOff(void);


volatile struct Motor {
	uint32_t stepsLeft;
	Motor_Dir dir;
	bool isRunning;
	bool isInfiniteMotion;
	uint32_t stepsPeriodUs;

	struct Ramp {
		uint32_t startPulses;
		uint32_t startPulsesLeft;
		uint32_t finishPulses;
		uint32_t finishPulsesLeft;
		uint8_t initSpeedPercent;
	} ramp;
} static motor;


void Motor_SetMovement(uint32_t degrees, Motor_Dir dir) {
	if(Motor_IsRunning()) {
		Console_LogErrorLn("Motor movement set while running");
		return;
	}

	motor.isInfiniteMotion = false;
	motor.stepsLeft = DEGREES_TO_STEPS(degrees);
	motor.dir = dir;
	Console_LogValLn("Motor steps set to ", motor.stepsLeft);

	uint32_t ramp = degrees < START_RAMP_DEGREES ? 0 : DEGREES_TO_STEPS(START_RAMP_DEGREES);
	motor.ramp.startPulses = ramp;
	motor.ramp.startPulsesLeft = ramp;
	ramp = degrees < START_RAMP_DEGREES + FINISH_RAMP_DEGREES ? 0 : DEGREES_TO_STEPS(FINISH_RAMP_DEGREES);
	motor.ramp.finishPulses = ramp;
	motor.ramp.finishPulsesLeft = ramp;
	motor.ramp.initSpeedPercent = RAMP_INIT_SPEED_PERCENT;

	motor.isRunning = true;
	Timers_Start(&TIMERS_MOTOR_TIMER);
}

void Motor_SetMovementInfinite(Motor_Dir dir) {
	//set some revolutions for correct ramps calculation
	Motor_SetMovement(MOTOR_REVOLUTIONS_TO_DEGREES(10), dir);
	motor.isInfiniteMotion = true;
}

void Motor_SetSpeed(uint32_t revPerHour) {
	uint16_t period = ((uint32_t)60 * 60 * 1000000) / (revPerHour * MOTOR_STEPS_PER_REVOLUTION);
	motor.stepsPeriodUs = period;
	Console_LogVal("Motor speed: ", revPerHour);
	Console_LogLn(" rev/hr");
}

bool Motor_IsRunning(void) {
	return motor.isRunning;
}

void Motor_IRQHandler(void) {
	if(motor.stepsLeft > 0) {

		uint8_t speedPercent;
		if(motor.ramp.startPulsesLeft > 0) {	//start ramp active
			//the following is y=ax+b, where b is init percent, a is (100% - init percent)/(1 - 0) and x is ramp progress
			speedPercent = ((100 - motor.ramp.initSpeedPercent) * (motor.ramp.startPulses - motor.ramp.startPulsesLeft)) / motor.ramp.startPulses;
			speedPercent += motor.ramp.initSpeedPercent;
			motor.ramp.startPulsesLeft--;
		}
		else if(motor.stepsLeft <= motor.ramp.finishPulses) {	//finish ramp active
			//similarly to start ramp, this is also y=ax+b, but: b=100%, a=(10%-100%)/(1-0) (a is NEGATIVE!
			// this is avoided by doing y=b-|a|x, which is the same mathematically in this case), x is ramp progress
			speedPercent = 100;
			speedPercent -= ((100 - motor.ramp.initSpeedPercent) *  (motor.ramp.finishPulses - motor.ramp.finishPulsesLeft)) / motor.ramp.finishPulses;
			motor.ramp.finishPulsesLeft--;
		}
		else {	//no ramp active
			speedPercent = 100;
		}

		uint16_t period = (uint32_t)motor.stepsPeriodUs * 100 / speedPercent;
		TIMERS_MOTOR_TIMER.Instance->CCR1 += period;
		Motor_Step(motor.dir);
		if(motor.isInfiniteMotion == false) {
			motor.stepsLeft--;
		}
	}
	else {
		Timers_Stop(&TIMERS_MOTOR_TIMER);
		MotorPowerOff();
		motor.isRunning = false;
	}
}

void Motor_RequestStop(void) {
	motor.isInfiniteMotion = false;
	if(motor.stepsLeft <= motor.ramp.finishPulses) {
		return;	//already stopping
	}
	motor.stepsLeft = motor.ramp.startPulsesLeft + motor.ramp.finishPulses;
}

static void Motor_Step(Motor_Dir dir) {
	//assert correct mode selection
	static_assert((DRIVE_WAVE && DRIVE_FULL_STEP) == false);
	static_assert((DRIVE_WAVE && DRIVE_HALF_STEP) == false);
	static_assert((DRIVE_FULL_STEP && DRIVE_HALF_STEP) == false);
	static_assert(DRIVE_WAVE || DRIVE_FULL_STEP || DRIVE_HALF_STEP);

	static uint8_t state;

	if(dir == MOTOR_DIR_CLOCKWISE) {
		state >= FINAL_DRIVE_STEP ? state = 0 : state++;
	}
	else {
		state == 0 ? state = FINAL_DRIVE_STEP : state--;
	}

#if DRIVE_WAVE
	switch(state) {
		case 0: {
			MotorPinsWrite(MOTOR_OUT1_Pin);
			break;
		}
		case 1: {
			MotorPinsWrite(MOTOR_OUT2_Pin);
			break;
		}
		case 2: {
			MotorPinsWrite(MOTOR_OUT3_Pin);
			break;
		}
		case 3: {
			MotorPinsWrite(MOTOR_OUT4_Pin);
			break;
		}
	}
#endif
#if DRIVE_FULL_STEP
	switch(state) {
		case 0: {
			MotorPinsWrite(MOTOR_OUT4_Pin | MOTOR_OUT1_Pin);
			break;
		}
		case 1: {
			MotorPinsWrite(MOTOR_OUT1_Pin | MOTOR_OUT2_Pin);
			break;
		}
		case 2: {
			MotorPinsWrite(MOTOR_OUT2_Pin | MOTOR_OUT3_Pin);
			break;
		}
		case 3: {
			MotorPinsWrite(MOTOR_OUT3_Pin | MOTOR_OUT4_Pin);
			break;
		}
	}
#endif
#if DRIVE_HALF_STEP
	switch(state) {
		case 0: {
			MotorPinsWrite(MOTOR_OUT4_Pin | MOTOR_OUT1_Pin);
			break;
		}
		case 1: {
			MotorPinsWrite(MOTOR_OUT1_Pin);
			break;
		}
		case 2: {
			MotorPinsWrite(MOTOR_OUT1_Pin | MOTOR_OUT2_Pin);
			break;
		}
		case 3: {
			MotorPinsWrite(MOTOR_OUT2_Pin);
			break;
		}
		case 4: {
			MotorPinsWrite(MOTOR_OUT2_Pin | MOTOR_OUT3_Pin);
			break;
		}
		case 5: {
			MotorPinsWrite(MOTOR_OUT3_Pin);
			break;
		}
		case 6: {
			MotorPinsWrite(MOTOR_OUT3_Pin | MOTOR_OUT4_Pin);
			break;
		}
		case 7: {
			MotorPinsWrite(MOTOR_OUT4_Pin);
			break;
		}
	}
#endif
}

static void MotorPinsWrite(uint32_t setMask) {
	//if any assert fails, function needs modifications to work correctly
	static_assert(MOTOR_OUT1_GPIO_Port == MOTOR_OUT2_GPIO_Port);
	static_assert(MOTOR_OUT1_GPIO_Port == MOTOR_OUT3_GPIO_Port);
	static_assert(MOTOR_OUT1_GPIO_Port == MOTOR_OUT4_GPIO_Port);

	uint32_t resetMask = MOTOR_OUT1_Pin | MOTOR_OUT2_Pin | MOTOR_OUT3_Pin | MOTOR_OUT4_Pin;
	resetMask &= ~(setMask);
	HAL_GPIO_WritePin(MOTOR_OUT1_GPIO_Port, resetMask, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR_OUT1_GPIO_Port, setMask, GPIO_PIN_SET);
}

static inline void MotorPowerOff(void) {
	MotorPinsWrite(0);
}
