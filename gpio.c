/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, LPT_D2_Pin|LPT_D3_Pin|LPT_D4_Pin|LPT_D5_Pin
                          |LPT_D6_Pin|LPT_D7_Pin|LPT_STROB_Pin|LCD_BLK_Pin
                          |LPT_D0_Pin|LPT_D1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, FLASH_CS_Pin|KEY_COL08_Pin|KEY_COL01_Pin|KEY_COL02_Pin
                          |KEY_COL03_Pin|KEY_COL04_Pin|KEY_COL05_Pin|KEY_COL06_Pin
                          |KEY_COL07_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ADC1_RST_GPIO_Port, ADC1_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, BUZZER_Pin|ADC2_RST_Pin|EEPROM_WP_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, RS485_DIR_Pin|LCD_RD_Pin|LCD_CE_Pin|LCD_CD_Pin
                          |LCD_RST_Pin|LCD_WR_Pin|LCD_DB0_Pin|LCD_DB1_Pin
                          |LCD_DB2_Pin|LCD_DB3_Pin|LCD_DB4_Pin|LCD_DB5_Pin
                          |LCD_DB6_Pin|LCD_DB7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LPT_D2_Pin LPT_D3_Pin LPT_D4_Pin LPT_D5_Pin
                           LPT_D6_Pin LPT_D7_Pin LPT_STROB_Pin LCD_BLK_Pin
                           LPT_D0_Pin LPT_D1_Pin */
  GPIO_InitStruct.Pin = LPT_D2_Pin|LPT_D3_Pin|LPT_D4_Pin|LPT_D5_Pin
                          |LPT_D6_Pin|LPT_D7_Pin|LPT_STROB_Pin|LCD_BLK_Pin
                          |LPT_D0_Pin|LPT_D1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : FLASH_CS_Pin KEY_COL08_Pin KEY_COL01_Pin KEY_COL02_Pin
                           KEY_COL03_Pin KEY_COL04_Pin KEY_COL05_Pin KEY_COL06_Pin
                           KEY_COL07_Pin */
  GPIO_InitStruct.Pin = FLASH_CS_Pin|KEY_COL08_Pin|KEY_COL01_Pin|KEY_COL02_Pin
                          |KEY_COL03_Pin|KEY_COL04_Pin|KEY_COL05_Pin|KEY_COL06_Pin
                          |KEY_COL07_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : ADC1_RST_Pin */
  GPIO_InitStruct.Pin = ADC1_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ADC1_RST_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : BUZZER_Pin ADC2_RST_Pin EEPROM_WP_Pin */
  GPIO_InitStruct.Pin = BUZZER_Pin|ADC2_RST_Pin|EEPROM_WP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : KEY_ROW04_Pin KEY_ROW03_Pin KEY_ROW02_Pin KEY_ROW01_Pin */
  GPIO_InitStruct.Pin = KEY_ROW04_Pin|KEY_ROW03_Pin|KEY_ROW02_Pin|KEY_ROW01_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : LPT_BUSY_Pin LPT_PAPER_Pin CAL_Pin */
  GPIO_InitStruct.Pin = LPT_BUSY_Pin|LPT_PAPER_Pin|CAL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : RS485_DIR_Pin LCD_RD_Pin LCD_CE_Pin LCD_CD_Pin
                           LCD_RST_Pin LCD_WR_Pin LCD_DB0_Pin LCD_DB1_Pin
                           LCD_DB2_Pin LCD_DB3_Pin LCD_DB4_Pin LCD_DB5_Pin
                           LCD_DB6_Pin LCD_DB7_Pin */
  GPIO_InitStruct.Pin = RS485_DIR_Pin|LCD_RD_Pin|LCD_CE_Pin|LCD_CD_Pin
                          |LCD_RST_Pin|LCD_WR_Pin|LCD_DB0_Pin|LCD_DB1_Pin
                          |LCD_DB2_Pin|LCD_DB3_Pin|LCD_DB4_Pin|LCD_DB5_Pin
                          |LCD_DB6_Pin|LCD_DB7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
