/************************************************************************************
 * arch/arm/src/stm32/stm32_fmc.c
 *
 *   Copyright (C) 20019 Gregory Nutt. All rights reserved.
 *   Author: Jason T. Harris <sirmanlypowers@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ************************************************************************************/

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <tinyara/config.h>
#include <tinyara/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <semaphore.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/irq.h>
#include <tinyara/arch.h>
#include <tinyara/semaphore.h>
#include <tinyara/spi/ospi.h>
#include <tinyara/pm/pm.h>

#include "up_internal.h"
#include "up_arch.h"

#include "chip.h"
#include "stm32l4.h"

#include <arch/board/board.h>
#include "stm32l4xx_hal_interface.h"

#include <stdio.h>

#define CONFIG_STM32L4_FMC

#if defined(CONFIG_STM32L4_FMC)


/**
  * @brief  PSRAM status structure definition
  */
#define PSRAM_OK         ((uint8_t)0x00)
#define PSRAM_ERROR      ((uint8_t)0x01)

#define PSRAM_DEVICE_ADDR  ((uint32_t)0x60000000)
#define PSRAM_DEVICE_SIZE  ((uint32_t)0x400000)  /* PSRAM device size in Bytes */

/* #define SRAM_MEMORY_WIDTH    FMC_NORSRAM_MEM_BUS_WIDTH_8*/
#define PSRAM_MEMORY_WIDTH    FMC_NORSRAM_MEM_BUS_WIDTH_16

#define PSRAM_BURSTACCESS     FMC_BURST_ACCESS_MODE_DISABLE
/* #define PSRAM_BURSTACCESS     FMC_BURST_ACCESS_MODE_ENABLE*/

#define PSRAM_WRITEBURST      FMC_WRITE_BURST_DISABLE
/* #define PSRAM_WRITEBURST     FMC_WRITE_BURST_ENABLE */

#define CONTINUOUSCLOCK_FEATURE    FMC_CONTINUOUS_CLOCK_SYNC_ONLY
/* #define CONTINUOUSCLOCK_FEATURE     FMC_CONTINUOUS_CLOCK_SYNC_ASYNC */

/* DMA definitions for SRAM DMA transfer */
#define __PSRAM_DMAx_CLK_ENABLE            __HAL_RCC_DMA2_CLK_ENABLE
#define __PSRAM_DMAx_CLK_DISABLE           __HAL_RCC_DMA2_CLK_DISABLE
#define PSRAM_DMAx_INSTANCE                DMA2_Channel3
#define PSRAM_DMAx_IRQn                    DMA2_Channel3_IRQn
#define BSP_PSRAM_DMA_IRQHandler           DMA2_Channel3_IRQHandler

static SRAM_HandleTypeDef psramHandle;
extern uint32_t bsp_lcd_initialized;
extern uint32_t bsp_psram_initialized;

static void Fill_Buffer(uint16_t *pBuffer, uint32_t uwBufferLength, uint32_t uwOffset);
static uint8_t Buffercmp(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength);
uint8_t ubSramDmaCplt = 0;


/****************************************************************************
 * Name: stm32_fmc_enable
 *
 * Description:
 *   Enable clocking to the FMC.
 *
 ****************************************************************************/

void stm32l4_fmc_enable(void)
{
  __HAL_RCC_FMC_CLK_ENABLE();
  //modifyreg32(STM32_RCC_AHB3ENR, 0, RCC_AHB3ENR_FMCEN);
}

/****************************************************************************
 * Name: stm32_fmc_disable
 *
 * Description:
 *   Disable clocking to the FMC.
 *
 ****************************************************************************/

void stm32l4_fmc_disable(void)
{
  __HAL_RCC_FMC_CLK_DISABLE();
  //modifyreg32(STM32_RCC_AHB3ENR, RCC_AHB3ENR_FMCEN, 0);
}



/**
  * @brief  DeInitializes the PSRAM device.
  * @retval PSRAM status
  */
uint8_t stm32l4_fmc_psram_deinitialize(void)
{
  static uint8_t psram_status = PSRAM_OK;

  if (bsp_psram_initialized == 1)
  {
    /* PSRAM device de-initialization */
    psramHandle.Instance = FMC_NORSRAM_DEVICE;
    psramHandle.Extended = FMC_NORSRAM_EXTENDED_DEVICE;

    if(HAL_SRAM_DeInit(&psramHandle) != HAL_OK)
    {
      psram_status = PSRAM_ERROR;
    }
    else
    {
      psram_status = PSRAM_OK;
    }

    bsp_psram_initialized = 0;
  }

  return psram_status;
}

/**
  * @brief  Reads an amount of data from the PSRAM device in polling mode.
  * @param  uwStartAddress: Read start address
  * @param  pData: Pointer to data to be read
  * @param  uwDataSize: Size of read data from the memory
  * @retval PSRAM status
  */
uint8_t stm32l4_fmc_psram_read(uint32_t uwStartAddress, uint16_t *pData, uint32_t uwDataSize)
{
  if(HAL_SRAM_Read_16b(&psramHandle, (uint32_t *)uwStartAddress, pData, uwDataSize) != HAL_OK)
  {
    return PSRAM_ERROR;
  }
  else
  {
    return PSRAM_OK;
  }
}

/**
  * @brief  Reads an amount of data from the PSRAM device in DMA mode.
  * @param  uwStartAddress: Read start address
  * @param  pData: Pointer to data to be read
  * @param  uwDataSize: Size of read data from the memory
  * @retval PSRAM status
  */
uint8_t stm32l4_fmc_psram_read_dma(uint32_t uwStartAddress, uint16_t *pData, uint32_t uwDataSize)
{
  if(HAL_SRAM_Read_DMA(&psramHandle, (uint32_t *)uwStartAddress, (uint32_t *)pData, uwDataSize) != HAL_OK)
  {
    return PSRAM_ERROR;
  }
  else
  {
    return PSRAM_OK;
  }
}

/**
  * @brief  Writes an amount of data from the PSRAM device in polling mode.
  * @param  uwStartAddress: Write start address
  * @param  pData: Pointer to data to be written
  * @param  uwDataSize: Size of written data from the memory
  * @retval PSRAM status
  */
uint8_t stm32l4_fmc_psram_write(uint32_t uwStartAddress, uint16_t *pData, uint32_t uwDataSize)
{
  if(HAL_SRAM_Write_16b(&psramHandle, (uint32_t *)uwStartAddress, pData, uwDataSize) != HAL_OK)
  {
    return PSRAM_ERROR;
  }
  else
  {
    return PSRAM_OK;
  }
}

/**
  * @brief  Writes an amount of data from the PSRAM device in DMA mode.
  * @param  uwStartAddress: Write start address
  * @param  pData: Pointer to data to be written
  * @param  uwDataSize: Size of written data from the memory
  * @retval PSRAM status
  */
uint8_t stm32l4_fmc_psram_write_dma(uint32_t uwStartAddress, uint16_t *pData, uint32_t uwDataSize)
{
  if(HAL_SRAM_Write_DMA(&psramHandle, (uint32_t *)uwStartAddress, (uint32_t *)pData, uwDataSize) != HAL_OK)
  {
    return PSRAM_ERROR;
  }
  else
  {
    return PSRAM_OK;
  }
}


static int fmc_interrupt(int irq, void *context, FAR void *arg)
{
  HAL_DMA_IRQHandler(psramHandle.hdma);

  return OK;
}

void HAL_SRAM_DMA_XferCpltCallback(DMA_HandleTypeDef *hdma)
{
  ubSramDmaCplt++;
}

void HAL_SRAM_DMA_XferErrorCallback(DMA_HandleTypeDef *hdma)
{
}
/**
  * @brief  Initializes PSRAM MSP.
  * @param  hsram: PSRAM handle
  * @param  Params
  * @retval None
  */
void HAL_SRAM_MspInit(SRAM_HandleTypeDef  *hsram)
{
  static DMA_HandleTypeDef dma_handle;
  GPIO_InitTypeDef GPIO_Init_Structure;

  /* Enable DMAx clock */
  __PSRAM_DMAx_CLK_ENABLE();
  __HAL_RCC_DMAMUX1_CLK_ENABLE();

  /* Enable FMC clock */
  __HAL_RCC_FMC_CLK_ENABLE();

  /* Enable GPIOs clock */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  /* IOSV bit MUST be set to access GPIO port G[2:15] */
  __HAL_RCC_PWR_CLK_ENABLE();
  SET_BIT(PWR->CR2, PWR_CR2_IOSV);

  /* Common GPIO configuration */
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF12_FMC;

  /*## Data Bus #######*/
  /* GPIOD configuration */
  GPIO_Init_Structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8 | GPIO_PIN_9 |
                              GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOD, &GPIO_Init_Structure);

  /* GPIOE configuration */
  GPIO_Init_Structure.Pin   = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |
                              GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 |
                              GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOE, &GPIO_Init_Structure);


  /*## Address Bus #######*/
  /* GPIOF configuration */
  GPIO_Init_Structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                              GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_12 | GPIO_PIN_13 |
                              GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOF, &GPIO_Init_Structure);


  /* GPIOG configuration */
  GPIO_Init_Structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 |
                              GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
  HAL_GPIO_Init(GPIOG, &GPIO_Init_Structure);

  /* GPIOD configuration */
  GPIO_Init_Structure.Pin   = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13;
  HAL_GPIO_Init(GPIOD, &GPIO_Init_Structure);

  /* GPIOE configuration */
  GPIO_Init_Structure.Pin   = GPIO_PIN_3 | GPIO_PIN_4;
  HAL_GPIO_Init(GPIOE, &GPIO_Init_Structure);
  GPIO_Init_Structure.Pull = GPIO_PULLUP;


  /*## NOE and NWE configuration #######*/
  GPIO_Init_Structure.Pin = GPIO_PIN_4 |GPIO_PIN_5;
  HAL_GPIO_Init(GPIOD, &GPIO_Init_Structure);

  /* Chip select configuration */
  /*## NE1 configuration #######*/
  GPIO_Init_Structure.Pin = GPIO_PIN_7;
  HAL_GPIO_Init(GPIOD, &GPIO_Init_Structure);

  /*## NE2, NE3, NE4 configuration #######*/
  GPIO_Init_Structure.Pin = GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_12;
  HAL_GPIO_Init(GPIOG, &GPIO_Init_Structure);

  /*## NBL0, NBL1 configuration #######*/
  GPIO_Init_Structure.Pin = GPIO_PIN_0 | GPIO_PIN_1;
  GPIO_Init_Structure.Pull      = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_Init_Structure);


  GPIO_Init_Structure.Pull      = GPIO_PULLDOWN;
  /*## CLK and NWAIT configuration #######*/
  GPIO_Init_Structure.Pin = GPIO_PIN_3 | GPIO_PIN_6;
  HAL_GPIO_Init(GPIOD, &GPIO_Init_Structure);

  /*## ADVn configuration #######*/
  GPIO_Init_Structure.Pin = GPIO_PIN_7;
  HAL_GPIO_Init(GPIOB, &GPIO_Init_Structure);

  /* Configure common DMA parameters */
  dma_handle.Init.Direction           = DMA_MEMORY_TO_MEMORY;
  dma_handle.Init.PeriphInc           = DMA_PINC_ENABLE;
  dma_handle.Init.MemInc              = DMA_MINC_ENABLE;
  dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  dma_handle.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
  dma_handle.Init.Mode                = DMA_NORMAL;
  dma_handle.Init.Priority            = DMA_PRIORITY_HIGH;

  dma_handle.Instance = PSRAM_DMAx_INSTANCE;

  /* Deinitialize the Channel for new transfer */
  HAL_DMA_DeInit(&dma_handle);

  /* Configure the DMA Channel */
  HAL_DMA_Init(&dma_handle);

  /* Associate the DMA handle to the FMC SRAM one */
  hsram->hdma = &dma_handle;

  //HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);

  /* NVIC configuration for DMA transfer complete interrupt */
  irq_attach(STM32L4_IRQ_DMA2CH3, fmc_interrupt, NULL);

  HAL_NVIC_SetPriority(PSRAM_DMAx_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(PSRAM_DMAx_IRQn);
}


/**
  * @brief  DeInitializes SRAM MSP.
  * @param  hsram: SRAM handle
  * @param  Params
  * @retval None
  */
void HAL_SRAM_MspDeInit(SRAM_HandleTypeDef  *hsram)
{
  static DMA_HandleTypeDef dma_handle;

  /* Disable NVIC configuration for DMA interrupt */
  HAL_NVIC_DisableIRQ(PSRAM_DMAx_IRQn);

  /* Deinitialize the stream for new transfer */
  dma_handle.Instance = PSRAM_DMAx_INSTANCE;
  HAL_DMA_DeInit(&dma_handle);

  /* Deinitialize GPIOs */
  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7);
  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_3  | GPIO_PIN_4  |
                         GPIO_PIN_5  | GPIO_PIN_6  | GPIO_PIN_7  | GPIO_PIN_8  |
                         GPIO_PIN_9  | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 |
                         GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
  HAL_GPIO_DeInit(GPIOE, GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_3  | GPIO_PIN_4  |
                         GPIO_PIN_7  | GPIO_PIN_8  | GPIO_PIN_9  | GPIO_PIN_10 |
                         GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 |
                         GPIO_PIN_15);
  HAL_GPIO_DeInit(GPIOF, GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_2  | GPIO_PIN_3  |
                         GPIO_PIN_4  | GPIO_PIN_5  | GPIO_PIN_12 | GPIO_PIN_13 |
                         GPIO_PIN_14 | GPIO_PIN_15);
  HAL_GPIO_DeInit(GPIOG, GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_2  | GPIO_PIN_3  |
                         GPIO_PIN_4  | GPIO_PIN_5  | GPIO_PIN_9  | GPIO_PIN_10 |
                         GPIO_PIN_12);

  /* GPIO pins clock, FMC clock and DMA clock can be shut down in the applications
     by surcharging this __weak function */
}

#define BUFFER_SIZE         ((uint32_t)0x0100)
#define WRITE_READ_ADDR     ((uint32_t)0x0800)

static uint16_t psram_aTxBuffer[BUFFER_SIZE];
static uint16_t psram_aRxBuffer[BUFFER_SIZE];


/**
  * @brief  Fills buffer with user predefined data.
  * @param  pBuffer: pointer on the buffer to fill
  * @param  uwBufferLength: size of the buffer to fill
  * @param  uwOffset: first value to fill on the buffer
  * @retval None
  */
static void Fill_Buffer(uint16_t *pBuffer, uint32_t uwBufferLength, uint32_t uwOffset)
{
  uint32_t tmpindex = 0;

  /* Put in global buffer different values */
  for (tmpindex = 0; tmpindex < uwBufferLength; tmpindex++ )
  {
    pBuffer[tmpindex] = tmpindex + uwOffset;
  }
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 1: pBuffer identical to pBuffer1
  *         0: pBuffer differs from pBuffer1
  */
static uint8_t Buffercmp(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    printf("Buffercmp : Rx 0x%x, Tx 0x%x\n", *pBuffer1, *pBuffer2);
    if (*pBuffer1 != *pBuffer2)
    {
      return 1;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}


void PSRAM_demo (void)
{
  ubSramDmaCplt = 0;
  Fill_Buffer(psram_aTxBuffer, BUFFER_SIZE, 0xa5a5);
  Fill_Buffer(psram_aRxBuffer, BUFFER_SIZE, 0xdead);

  if(stm32l4_fmc_psram_write_dma(PSRAM_DEVICE_ADDR + WRITE_READ_ADDR, psram_aTxBuffer, BUFFER_SIZE) != PSRAM_OK)
  {
    printf("PSRAM DEMO Write Failed!!\n");
  }
  else
  {
    printf("PSRAM DEMO Write succeeded!!\n");
  }

  while(ubSramDmaCplt == 0);
  ubSramDmaCplt = 0;
  for(int i=0; i<BUFFER_SIZE; i++)
  {
    printf("Tx Buffer : Index %d, Data 0x%x\n", i, psram_aTxBuffer[i]);
  }

  if(stm32l4_fmc_psram_read_dma(PSRAM_DEVICE_ADDR + WRITE_READ_ADDR, psram_aRxBuffer, BUFFER_SIZE) != PSRAM_OK)
  {
    printf("PSRAM DEMO Read Failed!!\n");
  }
  else
  {
    printf("PSRAM DEMO Read succeeded!!\n");
  }

  while(ubSramDmaCplt == 0);
  ubSramDmaCplt = 0;

  int ret = Buffercmp(psram_aRxBuffer, psram_aTxBuffer, BUFFER_SIZE);
  if(ret > 0)
  {
    printf("%d - PSRAM DEMO Compare Failed!!\n", ret);
    while(1);
  }
  else
  {
    printf("PSRAM DEMO Compare succeeded!!\n");
  }
}



uint8_t stm32l4_fmc_psram_initialize(void)
{
  static uint8_t psram_status = PSRAM_OK;

  if (bsp_psram_initialized == 0)
  {
    static FMC_NORSRAM_TimingTypeDef Timing;

    /* PSRAM device configuration */
    /* Timing configuration derived from system clock (up to 120Mhz)
    for 60Mhz as PSRAM clock frequency */
    Timing.AddressSetupTime      = 4;
    Timing.AddressHoldTime       = 2;
    Timing.DataSetupTime         = 6;
    Timing.BusTurnAroundDuration = 1;
    Timing.CLKDivision           = 2;
    Timing.DataLatency           = 2;
    Timing.AccessMode        = FMC_ACCESS_MODE_A;

    psramHandle.Init.NSBank             = FMC_NORSRAM_BANK1;
    psramHandle.Init.DataAddressMux     = FMC_DATA_ADDRESS_MUX_DISABLE;
    psramHandle.Init.MemoryType         = FMC_MEMORY_TYPE_PSRAM;
    psramHandle.Init.MemoryDataWidth    = FMC_NORSRAM_MEM_BUS_WIDTH_16;
    psramHandle.Init.BurstAccessMode    = FMC_BURST_ACCESS_MODE_DISABLE;
    psramHandle.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_HIGH;
    psramHandle.Init.WaitSignalActive   = FMC_WAIT_TIMING_BEFORE_WS;
    psramHandle.Init.WriteOperation     = FMC_WRITE_OPERATION_ENABLE;
    psramHandle.Init.WaitSignal         = FMC_WAIT_SIGNAL_DISABLE;
    psramHandle.Init.ExtendedMode       = FMC_EXTENDED_MODE_DISABLE;
    psramHandle.Init.AsynchronousWait   = FMC_ASYNCHRONOUS_WAIT_DISABLE;
    psramHandle.Init.WriteBurst         = FMC_WRITE_BURST_DISABLE;
    psramHandle.Init.ContinuousClock    = FMC_CONTINUOUS_CLOCK_SYNC_ASYNC;
    psramHandle.Init.WriteFifo          = FMC_WRITE_FIFO_DISABLE;
    psramHandle.Init.NBLSetupTime       = 0;
    psramHandle.Init.PageSize           = FMC_PAGE_SIZE_NONE;

    psramHandle.Instance  = FMC_NORSRAM_DEVICE;
    psramHandle.Extended  = FMC_NORSRAM_EXTENDED_DEVICE;

    /* PSRAM controller initialization */
    if(HAL_SRAM_Init(&psramHandle, &Timing, &Timing) != HAL_OK)
    {
      psram_status = PSRAM_ERROR;
    }
    else
    {
      psram_status = PSRAM_OK;
    }

    bsp_psram_initialized = 1;
  }

  return psram_status;
}

#endif /* CONFIG_STM32_FMC */















