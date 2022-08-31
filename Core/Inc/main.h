/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

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
#define UART_DEBUG_TX_Pin GPIO_PIN_2
#define UART_DEBUG_TX_GPIO_Port GPIOA
#define UART_DEBUG_RX_Pin GPIO_PIN_3
#define UART_DEBUG_RX_GPIO_Port GPIOA
#define SIM7000G_BAT_ENA_Pin GPIO_PIN_4
#define SIM7000G_BAT_ENA_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_2
#define LED_GPIO_Port GPIOB
#define SIM7000G_PWRKEY_Pin GPIO_PIN_13
#define SIM7000G_PWRKEY_GPIO_Port GPIOB
#define SIM7000G_RX_Pin GPIO_PIN_9
#define SIM7000G_RX_GPIO_Port GPIOA
#define SIM7000G_TX_Pin GPIO_PIN_10
#define SIM7000G_TX_GPIO_Port GPIOA
#define SIM7000G_RI_Pin GPIO_PIN_15
#define SIM7000G_RI_GPIO_Port GPIOA
#define SIM7000G_RI_EXTI_IRQn EXTI15_10_IRQn
/* USER CODE BEGIN Private defines */


// Agregado manualmente
#define SPI1_CS_Pin   GPIO_PIN_8
#define SPI1_CS_Port  GPIOB
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
