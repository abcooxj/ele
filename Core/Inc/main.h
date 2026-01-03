/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "key.h"
#include "oled.h"
#include "stdio.h"
#include "motor.h"
#include "stdbool.h"
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
#define BIN1_Pin GPIO_PIN_2
#define BIN1_GPIO_Port GPIOE
#define BIN2_Pin GPIO_PIN_3
#define BIN2_GPIO_Port GPIOE
#define BUZZER_Pin GPIO_PIN_0
#define BUZZER_GPIO_Port GPIOB
#define R2_Pin GPIO_PIN_1
#define R2_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_8
#define LED_GPIO_Port GPIOD
#define L2_Pin GPIO_PIN_6
#define L2_GPIO_Port GPIOG
#define L1_Pin GPIO_PIN_7
#define L1_GPIO_Port GPIOG
#define M0_Pin GPIO_PIN_8
#define M0_GPIO_Port GPIOG
#define R1_Pin GPIO_PIN_9
#define R1_GPIO_Port GPIOG
#define STBY_Pin GPIO_PIN_15
#define STBY_GPIO_Port GPIOG
#define AIN1_Pin GPIO_PIN_0
#define AIN1_GPIO_Port GPIOE
#define AIN2_Pin GPIO_PIN_1
#define AIN2_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */
#define LED_ON      HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET)           // 点亮LED
#define LED_OFF     HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET)             // 熄灭LED

#define Buzzer_ON   HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET)       // 开启蜂鸣器
#define Buzzer_OFF  HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET)     // 关闭蜂鸣器

#define L2          HAL_GPIO_ReadPin(L2_GPIO_Port, L2_Pin)                              // 循到黑线返回高电平
#define L1          HAL_GPIO_ReadPin(L1_GPIO_Port, L1_Pin)                              // 循到黑线返回高电平
#define M0          HAL_GPIO_ReadPin(M0_GPIO_Port, M0_Pin)                              // 循到黑线返回高电平
#define R1          HAL_GPIO_ReadPin(R1_GPIO_Port, R1_Pin)                              // 循到黑线返回高电平
#define R2          HAL_GPIO_ReadPin(R2_GPIO_Port, R2_Pin)                              // 循到黑线返回高电平
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
