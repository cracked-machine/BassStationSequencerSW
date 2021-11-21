/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <SEGGER_RTT.h>
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
#define TLC5955_SPI2_LAT_Pin GPIO_PIN_9
#define TLC5955_SPI2_LAT_GPIO_Port GPIOB
#define SPI1_DC_Pin GPIO_PIN_0
#define SPI1_DC_GPIO_Port GPIOA
#define SPI1_RESET_Pin GPIO_PIN_3
#define SPI1_RESET_GPIO_Port GPIOA
#define TLC5955_SPI2_GSCLK_Pin GPIO_PIN_6
#define TLC5955_SPI2_GSCLK_GPIO_Port GPIOB
#define TLC5955_SPI2_MOSI_Pin GPIO_PIN_7
#define TLC5955_SPI2_MOSI_GPIO_Port GPIOB
#define TLC5955_SPI2_SCK_Pin GPIO_PIN_8
#define TLC5955_SPI2_SCK_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/