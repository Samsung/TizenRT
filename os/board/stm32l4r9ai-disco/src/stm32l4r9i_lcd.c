/****************************************************************************
 * configs/stm32l4r9ai-disco/src/stm32_buttons.c
 *
 *   Copyright (C) 2018 Gregory Nutt. All rights reserved.
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

#include <stdint.h>
#include <stdio.h>
#include <errno.h>

#include <tinyara/arch.h>
#include <tinyara/board.h>
#include <arch/board/board.h>

#include <arch/board/stm32l4r9i_discovery_io.h>

#include "stm32l4_dsi.h"
#include "stm32l4r9ai-disco.h"

/****************************************************************************
 * Private Types
 ****************************************************************************/
struct stm32l4r9i_lcd_s
{
    struct stm32l4_lcd_s dev;
};

extern DSI_HandleTypeDef  hdsi;

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static uint8_t STM32L4_LCD_LongParamWrite(uint32_t Mode, uint32_t NbParams, uint32_t Param1, uint8_t *ParametersTable);
static uint8_t STM32L4_LCD_ShortParamWrite(uint32_t Mode, uint32_t Param1, uint32_t Param2);
static void STM32L4_LCD_Wrapper_Enable(void);
static void STM32L4_LCD_Refresh(void);
static void STM32L4_LCD_DisplayOn(void);
static void STM32L4_LCD_DisplayOff(void);

void LCD_PowerOn(void);
void LCD_PowerOff(void);

void BSP_LCD_MspInit(void);
void BSP_LCD_MspDeInit(void);
/****************************************************************************
 * Private Data
 ****************************************************************************/
struct stm32l4r9i_lcd_s g_lcddev;
extern struct stm32l4_dsi_s dsidev;

FAR struct stm32l4_lcd_s *stm32l4_lcd_function(void)
{
    FAR struct stm32l4r9i_lcd_s *priv = &g_lcddev;

    priv->dev.sendsparam    = STM32L4_LCD_ShortParamWrite;
    priv->dev.sendlparam    = STM32L4_LCD_LongParamWrite;
    priv->dev.enablewap     = STM32L4_LCD_Wrapper_Enable;
    priv->dev.poweron       = STM32L4_LCD_DisplayOn;//LCD_PowerOn;
    priv->dev.poweroff      = STM32L4_LCD_DisplayOff;//LCD_PowerOff;
    priv->dev.refresh       = STM32L4_LCD_Refresh;
    
    return &priv->dev;
}

uint8_t STM32L4_LCD_IsFBAvailable(void)
{
    FAR struct stm32l4_dsi_s *priv = &dsidev;
    uint8_t status;

    status = (priv->isframebuffer == 1) ? LCD_OK : LCD_ERROR;

    return (status);
}

static uint8_t STM32L4_LCD_LongParamWrite(uint32_t Mode, uint32_t NbParams, uint32_t Param1, uint8_t *ParametersTable)
{
    if(HAL_DSI_LongWrite(&hdsi, 0, Mode, NbParams, Param1, ParametersTable) != HAL_OK){
        /* Add the exception handling function */
        return(LCD_ERROR);
    }

    return LCD_OK;
}

static uint8_t STM32L4_LCD_ShortParamWrite(uint32_t Mode, uint32_t Param1, uint32_t Param2)
{
    if(HAL_DSI_ShortWrite(&hdsi, 0, Mode, Param1, Param2) != HAL_OK){
        /* Add the exception handling function */
        return(LCD_ERROR);
    }

    return LCD_OK;
}

static void STM32L4_LCD_Wrapper_Enable(void)
{
    /* Enable DSI Wrapper */
    __HAL_DSI_WRAPPER_ENABLE(&hdsi);
}

static void STM32L4_LCD_Refresh(void)
{
    FAR struct stm32l4_dsi_s *priv = &dsidev;
    
    /* Set frame buffer busy */
    priv->isframebuffer = 0;
    
    /* Set tear on */
    HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, DSI_SET_TEAR_ON, 0x0);
}

static void STM32L4_LCD_DisplayOn(void)
{
  /* Send Display on DCS command to display */
  HAL_DSI_ShortWrite(&hdsi,
                     0,
                     DSI_DCS_SHORT_PKT_WRITE_P0,
                     DSI_SET_DISPLAY_ON,
                     0x0);
}

/**
  * @brief  Switch Off the display.
  */
static void STM32L4_LCD_DisplayOff(void)
{
  /* Send Display off DCS Command to display */
  HAL_DSI_ShortWrite(&hdsi,
                     0,
                     DSI_DCS_SHORT_PKT_WRITE_P0,
                     DSI_SET_DISPLAY_OFF,
                     0x0);
}

void LCD_PowerOn(void)
{
    uint8_t res = 0;
    
    res = BSP_IO_Init();
    if(res != IO_OK){
        printf("IO_ERROR\n");
    }

    /* Configure the GPIO connected to DSI_3V3_POWERON signal as output low */
    /* to activate 3V3_LCD. VDD_LCD is also activated if VDD = 3.3V */
    BSP_IO_WritePin(IO_PIN_8, GPIO_PIN_RESET);
    BSP_IO_ConfigPin(IO_PIN_8, IO_MODE_OUTPUT);

    /* Wait at least 1ms before enabling 1V8_LCD */
    HAL_Delay(1);

    /* Configure the GPIO connected to DSI_1V8_PWERON signal as output low */
    /* To activate 1V8_LCD. VDD_LCD is also activated if VDD = 1.8 */
    BSP_IO_WritePin(AGPIO_PIN_2, GPIO_PIN_RESET);
    BSP_IO_ConfigPin(AGPIO_PIN_2, IO_MODE_OUTPUT);

    /* Wait at least 15ms (minimum reset low width is 10ms and add margin for 1V8_LCD ramp-up */
    HAL_Delay(15);

    /* Configure the GPIO connected to DSI_RESET signal */
    BSP_IO_ConfigPin(IO_PIN_10, IO_MODE_OUTPUT);

    /* Descativate DSI_RESET */
    BSP_IO_WritePin(IO_PIN_10, GPIO_PIN_SET);

    /* Wait reset complete time (maximum time is 5ms when LCD in sleep mode) */
    HAL_Delay(120);
}

void LCD_PowerOff(void)
{
    /* Activate DSI_RESET */
    BSP_IO_WritePin(IO_PIN_10, GPIO_PIN_RESET);

    /* Wait at least 5 ms */
    HAL_Delay(5);

    /* JSet DSI_POWER_ON to analog mode only if psram is not currently used */

    /* Disable first DSI_1V8_PWRON then DSI_3V3_PWRON */
    BSP_IO_ConfigPin(AGPIO_PIN_2, IO_MODE_ANALOG);
    BSP_IO_ConfigPin(IO_PIN_8, IO_MODE_ANALOG);
}

