/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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
#define LED_R_Pin GPIO_PIN_5
#define LED_R_GPIO_Port GPIOA
#define LED_G_Pin GPIO_PIN_6
#define LED_G_GPIO_Port GPIOA
#define LED_B_Pin GPIO_PIN_7
#define LED_B_GPIO_Port GPIOA
#define RIGHT_IN1_Pin GPIO_PIN_14
#define RIGHT_IN1_GPIO_Port GPIOB
#define RIGHT_IN2_Pin GPIO_PIN_15
#define RIGHT_IN2_GPIO_Port GPIOB
#define LEFT_IN1_Pin GPIO_PIN_6
#define LEFT_IN1_GPIO_Port GPIOC
#define LEFT_IN2_Pin GPIO_PIN_8
#define LEFT_IN2_GPIO_Port GPIOC
#define Sonic_R_ECHO_Pin GPIO_PIN_9
#define Sonic_R_ECHO_GPIO_Port GPIOC
#define BUZZ_Pin GPIO_PIN_10
#define BUZZ_GPIO_Port GPIOA
#define Sonic_L_TRIG_Pin GPIO_PIN_10
#define Sonic_L_TRIG_GPIO_Port GPIOC
#define Sonic_C_TRIG_Pin GPIO_PIN_11
#define Sonic_C_TRIG_GPIO_Port GPIOC
#define Sonic_R_TRIG_Pin GPIO_PIN_2
#define Sonic_R_TRIG_GPIO_Port GPIOD
#define Sonic_L_ECHO_Pin GPIO_PIN_4
#define Sonic_L_ECHO_GPIO_Port GPIOB
#define Sonic_C_ECHO_Pin GPIO_PIN_5
#define Sonic_C_ECHO_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
