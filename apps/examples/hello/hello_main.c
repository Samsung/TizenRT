/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * examples/hello/hello_main.c
 *
 *   Copyright (C) 2008, 2011-2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
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
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <565_390x390.h>

/****************************************************************************
 * hello_main
 ****************************************************************************/
extern bool TouchItOccurred;

static void psram_check(void);
static void exflash_check(void);

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int hello_main(int argc, char *argv[])
#endif
{
    dma2d_copybuffer((uint32_t *)image_390x390_rgb565, NULL, 0, 0, 390, 390);
    printf("Hello, World!!\n");

    exflash_check();
    psram_check();

    while(1) {
        if(TouchItOccurred == true) {
            stm32_touch_printf_coord();
            HAL_Delay(50);
        }
        HAL_Delay(100);
    }
    return 0;
}




#define SRAM_BANK_ADDR      ((uint32_t)0x60000000)
#define BUFFER_SIZE         ((uint32_t)0x0100)
#define WRITE_READ_ADDR     ((uint32_t)0x0800)

static uint16_t aTxBuffer[BUFFER_SIZE];
static uint16_t aRxBuffer[BUFFER_SIZE];

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
    if (*pBuffer1 != *pBuffer2)
    {
      return 1;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}


static void psram_check(void)
{
  Fill_Buffer(aTxBuffer, BUFFER_SIZE, 0xC20F);

  /* Write data to the SRAM memory */
  for(int uwIndex = 0; uwIndex < BUFFER_SIZE; uwIndex++)
  {
    *(volatile uint16_t *)(SRAM_BANK_ADDR + WRITE_READ_ADDR + 2 * uwIndex) = aTxBuffer[uwIndex];
  }

  /* Read back data from the SRAM memory */
  for(int uwIndex = 0; uwIndex < BUFFER_SIZE; uwIndex++)
  {
    aRxBuffer[uwIndex] = *(volatile uint16_t *)(SRAM_BANK_ADDR + WRITE_READ_ADDR + 2 * uwIndex);
  }

  if(Buffercmp(aTxBuffer, aRxBuffer, BUFFER_SIZE) != 0)
  {
    printf("PSRAM DEMO Compare failed!!\n");
  }
  else
  {
    printf("PSRAM DEMO Compare succeeded!!\n");
  }
}

#define EXFLASH_BASE          ((uint32_t)0x90000000)
#define EXFLASH_BUFF_LEN      (512)
static void exflash_check(void)
{
  volatile uint8_t *mem_addr;

  stm32_exflash_enter_memorymapped();

  mem_addr = (volatile uint8_t *)(EXFLASH_BASE);

  for(int i=0; i<EXFLASH_BUFF_LEN; i++)
  {
    printf("mem_addr : 0x%x is with aTxBuffer[%d], 0x%x \n", mem_addr, i, *mem_addr);
    mem_addr++;
  }
}


