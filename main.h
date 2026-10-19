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
#define LPT_D2_Pin GPIO_PIN_2
#define LPT_D2_GPIO_Port GPIOE
#define LPT_D3_Pin GPIO_PIN_3
#define LPT_D3_GPIO_Port GPIOE
#define LPT_D4_Pin GPIO_PIN_4
#define LPT_D4_GPIO_Port GPIOE
#define LPT_D5_Pin GPIO_PIN_5
#define LPT_D5_GPIO_Port GPIOE
#define LPT_D6_Pin GPIO_PIN_6
#define LPT_D6_GPIO_Port GPIOE
#define FLASH_CS_Pin GPIO_PIN_13
#define FLASH_CS_GPIO_Port GPIOC
#define KEY_COL08_Pin GPIO_PIN_0
#define KEY_COL08_GPIO_Port GPIOC
#define ADC2_DOUT_Pin GPIO_PIN_2
#define ADC2_DOUT_GPIO_Port GPIOC
#define KEY_COL01_Pin GPIO_PIN_3
#define KEY_COL01_GPIO_Port GPIOC
#define RS232_02_TXD_Pin GPIO_PIN_2
#define RS232_02_TXD_GPIO_Port GPIOA
#define RS232_02_RXD_Pin GPIO_PIN_3
#define RS232_02_RXD_GPIO_Port GPIOA
#define ADC1_RST_Pin GPIO_PIN_4
#define ADC1_RST_GPIO_Port GPIOA
#define ADC1_SCLK_Pin GPIO_PIN_5
#define ADC1_SCLK_GPIO_Port GPIOA
#define ADC1_DOUT_Pin GPIO_PIN_6
#define ADC1_DOUT_GPIO_Port GPIOA
#define KEY_COL02_Pin GPIO_PIN_4
#define KEY_COL02_GPIO_Port GPIOC
#define KEY_COL03_Pin GPIO_PIN_5
#define KEY_COL03_GPIO_Port GPIOC
#define BUZZER_Pin GPIO_PIN_0
#define BUZZER_GPIO_Port GPIOB
#define KEY_ROW04_Pin GPIO_PIN_2
#define KEY_ROW04_GPIO_Port GPIOB
#define LPT_D7_Pin GPIO_PIN_7
#define LPT_D7_GPIO_Port GPIOE
#define LPT_STROB_Pin GPIO_PIN_8
#define LPT_STROB_GPIO_Port GPIOE
#define LCD_BLK_Pin GPIO_PIN_9
#define LCD_BLK_GPIO_Port GPIOE
#define LPT_BUSY_Pin GPIO_PIN_10
#define LPT_BUSY_GPIO_Port GPIOE
#define LPT_PAPER_Pin GPIO_PIN_11
#define LPT_PAPER_GPIO_Port GPIOE
#define CAL_Pin GPIO_PIN_15
#define CAL_GPIO_Port GPIOE
#define ADC2_RST_Pin GPIO_PIN_12
#define ADC2_RST_GPIO_Port GPIOB
#define ADC2_SCLK_Pin GPIO_PIN_13
#define ADC2_SCLK_GPIO_Port GPIOB
#define RS485_TXD_Pin GPIO_PIN_8
#define RS485_TXD_GPIO_Port GPIOD
#define RS485_RXD_Pin GPIO_PIN_9
#define RS485_RXD_GPIO_Port GPIOD
#define RS485_DIR_Pin GPIO_PIN_10
#define RS485_DIR_GPIO_Port GPIOD
#define LCD_RD_Pin GPIO_PIN_11
#define LCD_RD_GPIO_Port GPIOD
#define LCD_CE_Pin GPIO_PIN_12
#define LCD_CE_GPIO_Port GPIOD
#define LCD_CD_Pin GPIO_PIN_13
#define LCD_CD_GPIO_Port GPIOD
#define LCD_RST_Pin GPIO_PIN_14
#define LCD_RST_GPIO_Port GPIOD
#define LCD_WR_Pin GPIO_PIN_15
#define LCD_WR_GPIO_Port GPIOD
#define KEY_COL04_Pin GPIO_PIN_6
#define KEY_COL04_GPIO_Port GPIOC
#define KEY_COL05_Pin GPIO_PIN_7
#define KEY_COL05_GPIO_Port GPIOC
#define KEY_COL06_Pin GPIO_PIN_8
#define KEY_COL06_GPIO_Port GPIOC
#define KEY_COL07_Pin GPIO_PIN_9
#define KEY_COL07_GPIO_Port GPIOC
#define RS232_01_TXD_Pin GPIO_PIN_9
#define RS232_01_TXD_GPIO_Port GPIOA
#define RS232_01_RXD_Pin GPIO_PIN_10
#define RS232_01_RXD_GPIO_Port GPIOA
#define USB_B_DM_Pin GPIO_PIN_11
#define USB_B_DM_GPIO_Port GPIOA
#define USB_B_DP_Pin GPIO_PIN_12
#define USB_B_DP_GPIO_Port GPIOA
#define FLASH_SCK_Pin GPIO_PIN_10
#define FLASH_SCK_GPIO_Port GPIOC
#define FLASH_D0_Pin GPIO_PIN_11
#define FLASH_D0_GPIO_Port GPIOC
#define FLASH_DI_Pin GPIO_PIN_12
#define FLASH_DI_GPIO_Port GPIOC
#define LCD_DB0_Pin GPIO_PIN_0
#define LCD_DB0_GPIO_Port GPIOD
#define LCD_DB1_Pin GPIO_PIN_1
#define LCD_DB1_GPIO_Port GPIOD
#define LCD_DB2_Pin GPIO_PIN_2
#define LCD_DB2_GPIO_Port GPIOD
#define LCD_DB3_Pin GPIO_PIN_3
#define LCD_DB3_GPIO_Port GPIOD
#define LCD_DB4_Pin GPIO_PIN_4
#define LCD_DB4_GPIO_Port GPIOD
#define LCD_DB5_Pin GPIO_PIN_5
#define LCD_DB5_GPIO_Port GPIOD
#define LCD_DB6_Pin GPIO_PIN_6
#define LCD_DB6_GPIO_Port GPIOD
#define LCD_DB7_Pin GPIO_PIN_7
#define LCD_DB7_GPIO_Port GPIOD
#define KEY_ROW03_Pin GPIO_PIN_3
#define KEY_ROW03_GPIO_Port GPIOB
#define KEY_ROW02_Pin GPIO_PIN_4
#define KEY_ROW02_GPIO_Port GPIOB
#define KEY_ROW01_Pin GPIO_PIN_5
#define KEY_ROW01_GPIO_Port GPIOB
#define EEPROM_SCL_Pin GPIO_PIN_6
#define EEPROM_SCL_GPIO_Port GPIOB
#define EEPROM_SDA_Pin GPIO_PIN_7
#define EEPROM_SDA_GPIO_Port GPIOB
#define EEPROM_WP_Pin GPIO_PIN_8
#define EEPROM_WP_GPIO_Port GPIOB
#define LPT_D0_Pin GPIO_PIN_0
#define LPT_D0_GPIO_Port GPIOE
#define LPT_D1_Pin GPIO_PIN_1
#define LPT_D1_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
