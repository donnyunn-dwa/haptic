/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

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
void I2C1_set_byte(uint8_t addr, uint8_t data);
void I2C1_read_RAM(uint8_t* pData, uint16_t addr, uint16_t len);
void I2C1_write_RAM(uint8_t* pData, uint16_t addr, uint16_t len);
void I2C2_set_byte(uint8_t addr, uint8_t data);
void I2C2_read_RAM(uint8_t* pData, uint16_t addr, uint16_t len);
void I2C2_write_RAM(uint8_t* pData, uint16_t addr, uint16_t len);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED1_Pin GPIO_PIN_11
#define LED1_GPIO_Port GPIOA
#define MCU_ACT_Pin GPIO_PIN_12
#define MCU_ACT_GPIO_Port GPIOA
#define EN2_Pin GPIO_PIN_6
#define EN2_GPIO_Port GPIOB
#define EN1_Pin GPIO_PIN_7
#define EN1_GPIO_Port GPIOB
#define ADC_CH2_Pin GPIO_PIN_0
#define ADC_CH2_GPIO_Port GPIOA
#define ADC_CH1_Pin GPIO_PIN_1
#define ADC_CH1_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

#define NFFT 512				// FFT Data N
#define NFFT2 (NFFT/2)
#define NFFT3_4 ((NFFT*3)/4)	// Twiddle factor Data N	
#define NOVLP 32

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
