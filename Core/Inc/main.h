/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MOTOR_OUT1_Pin GPIO_PIN_0
#define MOTOR_OUT1_GPIO_Port GPIOA
#define MOTOR_OUT2_Pin GPIO_PIN_1
#define MOTOR_OUT2_GPIO_Port GPIOA
#define MOTOR_OUT3_Pin GPIO_PIN_2
#define MOTOR_OUT3_GPIO_Port GPIOA
#define MOTOR_OUT4_Pin GPIO_PIN_3
#define MOTOR_OUT4_GPIO_Port GPIOA
#define BOARD_LED_Pin GPIO_PIN_4
#define BOARD_LED_GPIO_Port GPIOA
#define BUTTON0_Pin GPIO_PIN_6
#define BUTTON0_GPIO_Port GPIOA
#define BUTTON0_EXTI_IRQn EXTI4_15_IRQn
#define BUTTON1_Pin GPIO_PIN_7
#define BUTTON1_GPIO_Port GPIOA
#define BUTTON1_EXTI_IRQn EXTI4_15_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
