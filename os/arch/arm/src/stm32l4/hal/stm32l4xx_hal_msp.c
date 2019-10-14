/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : stm32l4xx_hal_msp.c
  * Description        : This file provides code for the MSP Initialization 
  *                      and de-Initialization codes.
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

/* Includes ------------------------------------------------------------------*/
#include "chip/stm32l4xx.h"
#include "stm32l4xx_hal.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */
 
/* USER CODE END Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External functions --------------------------------------------------------*/
/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{
  /* USER CODE BEGIN MspInit 0 */

  /* USER CODE END MspInit 0 */

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  /* System interrupt init*/

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}


/**
* @brief UART MSP Initialization
* This function configures the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(huart->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration    
    PA3     ------> USART2_RX
    PA2     ------> USART2_TX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
}

/**
* @brief UART MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();
  
    /**USART2 GPIO Configuration    
    PA3     ------> USART2_RX
    PA2     ------> USART2_TX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_3|GPIO_PIN_2);
}
/**
  * @brief LTDC MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  * @param hltdc: LTDC handle pointer
  * @retval None
  */
void HAL_LTDC_MspInit(LTDC_HandleTypeDef *hltdc)
{
    RCC_PeriphCLKInitTypeDef  PeriphClkInit = {0};

  /* Enable the LTDC clock */
  __HAL_RCC_LTDC_CLK_ENABLE();

  /* Reset of LTDC IP */
  __HAL_RCC_LTDC_FORCE_RESET();
  __HAL_RCC_LTDC_RELEASE_RESET();

  /* Configure the clock for the LTDC */
  /* We want DSI PHI at 500MHz */
  /* We have only one line => 500Mbps */
  /* With 24bits per pixel, equivalent PCLK is 500/24 = 20.8MHz */
  /* We will set PCLK at 15MHz */
  /* Following values are OK with MSI = 4MHz */
  /* (4*60)/(1*4*4) = 15MHz */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInit.PLLSAI2.PLLSAI2Source = RCC_PLLSOURCE_MSI;
  PeriphClkInit.PLLSAI2.PLLSAI2M = 1;
  PeriphClkInit.PLLSAI2.PLLSAI2N = 20;//60; /* Change to 20 from 60 for asynchronoce PSRAM embedded on STM32L4R9IDISCO
  PeriphClkInit.PLLSAI2.PLLSAI2R = RCC_PLLR_DIV4;
  PeriphClkInit.LtdcClockSelection = RCC_LTDCCLKSOURCE_PLLSAI2_DIV4;
  PeriphClkInit.PLLSAI2.PLLSAI2ClockOut = RCC_PLLSAI2_LTDCCLK;
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    while(1);
  }

  /* NVIC configuration for LTDC interrupts that are now enabled */
  HAL_NVIC_SetPriority(LTDC_IRQn, 0x05, 0);
  HAL_NVIC_EnableIRQ(LTDC_IRQn);
  HAL_NVIC_SetPriority(LTDC_ER_IRQn, 0x05, 0);
  HAL_NVIC_EnableIRQ(LTDC_ER_IRQn);
}

/**
  * @brief DSI MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  * @param hdsi: DSI handle pointer
  * @retval None
  */
void HAL_DSI_MspInit(DSI_HandleTypeDef *hdsi)
{
    RCC_OscInitTypeDef        OscInitStruct;
    GPIO_InitTypeDef  GPIO_InitStruct;

  /* Enable DSI Host and wrapper clocks */
  __HAL_RCC_DSI_CLK_ENABLE();

  /* Reset the DSI Host and wrapper */
  __HAL_RCC_DSI_FORCE_RESET();
  __HAL_RCC_DSI_RELEASE_RESET();

  /* Enable HSE used for DSI PLL */
  HAL_RCC_GetOscConfig(&OscInitStruct);
  if(OscInitStruct.HSEState == RCC_HSE_OFF) {
      /* Workaround for long HSE startup time (set PH0 to ouput PP low) */
      __HAL_RCC_GPIOH_CLK_ENABLE();
      GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
      GPIO_InitStruct.Pull      = GPIO_NOPULL;
      GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
      GPIO_InitStruct.Pin       = GPIO_PIN_0;
      HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
      HAL_GPIO_WritePin(GPIOH, GPIO_PIN_0, GPIO_PIN_RESET);

      OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
      OscInitStruct.HSEState       = RCC_HSE_ON;
      OscInitStruct.PLL.PLLState   = RCC_PLL_NONE;
      if (HAL_RCC_OscConfig(&OscInitStruct) != HAL_OK) {
          while(1);
      }
  }

  /* NVIC configuration for DSI interrupt that is now enabled */
  HAL_NVIC_SetPriority(DSI_IRQn, 0x05, 0);
  HAL_NVIC_EnableIRQ(DSI_IRQn);
}

/**
  * @brief DMA2D MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  * @param Dma2dHandle: DMA2D handle pointer
  * @retval None
  */
void HAL_DMA2D_MspInit(DMA2D_HandleTypeDef *Dma2dHandle)
{
  /* Enable the DMA2D clock */
  __HAL_RCC_DMA2D_CLK_ENABLE();

  /* Reset of DMA2D IP */
  __HAL_RCC_DMA2D_FORCE_RESET();
  __HAL_RCC_DMA2D_RELEASE_RESET();
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
