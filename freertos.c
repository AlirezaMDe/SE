/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for MainTask */
osThreadId_t MainTaskHandle;
const osThreadAttr_t MainTask_attributes = {
  .name = "MainTask",
  .stack_size = 15000 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for MainScale01Task */
osThreadId_t MainScale01TaskHandle;
const osThreadAttr_t MainScale01Task_attributes = {
  .name = "MainScale01Task",
  .stack_size = 2000 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for MainScale02Task */
osThreadId_t MainScale02TaskHandle;
const osThreadAttr_t MainScale02Task_attributes = {
  .name = "MainScale02Task",
  .stack_size = 2000 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

extern void WeightDisplay_MainFunction(void);
extern void WeightDisplay_MainScale01Function(void);
extern void WeightDisplay_MainScale02Function(void);
extern void CreateQueuesWeightDisplay(void);

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void MainTaskFunction(void *argument);
void MainScale01Function(void *argument);
void MainScale02Function(void *argument);

extern void MX_USB_HOST_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */

	CreateQueuesWeightDisplay();

  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of MainTask */
  MainTaskHandle = osThreadNew(MainTaskFunction, NULL, &MainTask_attributes);

  /* creation of MainScale01Task */
  MainScale01TaskHandle = osThreadNew(MainScale01Function, NULL, &MainScale01Task_attributes);

  /* creation of MainScale02Task */
  MainScale02TaskHandle = osThreadNew(MainScale02Function, NULL, &MainScale02Task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* init code for USB_HOST */
  MX_USB_HOST_Init();
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_MainTaskFunction */
/**
* @brief Function implementing the MainTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_MainTaskFunction */
void MainTaskFunction(void *argument)
{
  /* USER CODE BEGIN MainTaskFunction */

	WeightDisplay_MainFunction();

  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END MainTaskFunction */
}

/* USER CODE BEGIN Header_MainScale01Function */
/**
* @brief Function implementing the MainScale01Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_MainScale01Function */
void MainScale01Function(void *argument)
{
  /* USER CODE BEGIN MainScale01Function */

	WeightDisplay_MainScale01Function();

  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END MainScale01Function */
}

/* USER CODE BEGIN Header_MainScale02Function */
/**
* @brief Function implementing the MainScale02Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_MainScale02Function */
void MainScale02Function(void *argument)
{
  /* USER CODE BEGIN MainScale02Function */

	WeightDisplay_MainScale02Function();

  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END MainScale02Function */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

