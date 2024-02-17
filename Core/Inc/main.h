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
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "my_global.h"
#include "my_astronode.h"
#include "my_rtc.h"
#include "my_gnss.h"
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

// my_astro

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ACC_INT1_IT0_Pin GPIO_PIN_0
#define ACC_INT1_IT0_GPIO_Port GPIOA
#define ACC_INT2_Pin GPIO_PIN_1
#define ACC_INT2_GPIO_Port GPIOA
#define DBG_TX_Pin GPIO_PIN_2
#define DBG_TX_GPIO_Port GPIOA
#define DBG_RX_Pin GPIO_PIN_3
#define DBG_RX_GPIO_Port GPIOA
#define ACC_SPI1_CS_Pin GPIO_PIN_4
#define ACC_SPI1_CS_GPIO_Port GPIOA
#define ACC_SPI1_SCK_Pin GPIO_PIN_5
#define ACC_SPI1_SCK_GPIO_Port GPIOA
#define ACC_SPI1_MISO_Pin GPIO_PIN_6
#define ACC_SPI1_MISO_GPIO_Port GPIOA
#define ACC_SPI1_MOSI_Pin GPIO_PIN_7
#define ACC_SPI1_MOSI_GPIO_Port GPIOA
#define ASTRO_WKUP_Pin GPIO_PIN_0
#define ASTRO_WKUP_GPIO_Port GPIOB
#define ASTRO_RST_Pin GPIO_PIN_1
#define ASTRO_RST_GPIO_Port GPIOB
#define ASTRO_EVT_Pin GPIO_PIN_2
#define ASTRO_EVT_GPIO_Port GPIOB
#define RF_SW_CTL2_Pin GPIO_PIN_10
#define RF_SW_CTL2_GPIO_Port GPIOB
#define RF_SW_CTL1_Pin GPIO_PIN_11
#define RF_SW_CTL1_GPIO_Port GPIOB
#define GNSS_RST_Pin GPIO_PIN_13
#define GNSS_RST_GPIO_Port GPIOB
#define ASTRO_ANT_USE_Pin GPIO_PIN_14
#define ASTRO_ANT_USE_GPIO_Port GPIOB
#define GNSS_PWR_SW_Pin GPIO_PIN_15
#define GNSS_PWR_SW_GPIO_Port GPIOB
#define DBG2_TX_Pin GPIO_PIN_9
#define DBG2_TX_GPIO_Port GPIOA
#define DBG2_RX_Pin GPIO_PIN_10
#define DBG2_RX_GPIO_Port GPIOA
#define LDG_Pin GPIO_PIN_0
#define LDG_GPIO_Port GPIOD
#define LDB_Pin GPIO_PIN_1
#define LDB_GPIO_Port GPIOD
#define SW1_Pin GPIO_PIN_2
#define SW1_GPIO_Port GPIOD
#define SW2_Pin GPIO_PIN_3
#define SW2_GPIO_Port GPIOD
#define GNSS_TX_Pin GPIO_PIN_3
#define GNSS_TX_GPIO_Port GPIOB
#define GNSS_RX_Pin GPIO_PIN_4
#define GNSS_RX_GPIO_Port GPIOB
#define GNSS_3DFIX_IT5_Pin GPIO_PIN_5
#define GNSS_3DFIX_IT5_GPIO_Port GPIOB
#define GNSS_JAM_Pin GPIO_PIN_6
#define GNSS_JAM_GPIO_Port GPIOB
#define ASTRO_TX_Pin GPIO_PIN_8
#define ASTRO_TX_GPIO_Port GPIOB
#define ASTRO_RX_Pin GPIO_PIN_9
#define ASTRO_RX_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define FIRMWARE_RELEASE_YEAR			2024
#define HUART_DBG2						huart1 // BR = 115200 8N1
#define HUART_DBG						huart2 // BR = 115200 8N1 -> COM na st-link/v3
#define HUART_ASTRO						huart3 // BR = 9600 8N1
#define HUART_GNSS						huart5 // BR = 115200 8N1
#define SPI_ACC							hspi1
#define TIM								htim6
#define TIM_SECONDS_THS_SYSTEM_RESET	600

#define UART_TIMEOUT 					1000

#define ASTRONODE_PAYLOAD_MAX_LEN		160

#define MY_RTC_ALARM_5MIN				300
#define MY_RTC_ALARM_10MIN				600
#define MY_RTC_ALARM_1H					3600

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
