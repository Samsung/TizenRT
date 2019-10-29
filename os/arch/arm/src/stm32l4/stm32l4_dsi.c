/****************************************************************************
 *
 * Copyright 2019 STMicroelectronics All Rights Reserved.
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
 * arch/arm/src/stm32l4/stm32l4_ltdc.c
 *
 *   Copyright (C) 2013-2014 . All rights reserved.
 *   Authors: 
 *
 * References:
 *   STM32L4R9xx Technical Reference Manual and Data Sheet
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
#include <string.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/lcd/lcd.h>
#include <tinyara/kmalloc.h>

#include <arch/chip/ltdc.h>
#include <arch/board/board.h>
#include <hal/stm32l4xx_hal_dsi.h>

#include "up_arch.h"
#include "up_internal.h"
#include "stm32l4.h"
#include "stm32l4_dsi.h"

/****************************************************************************
 * Pre-Processor Definitions
 ****************************************************************************/

/* Register definition ******************************************************/

/* Configuration ************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/
/* Interrupt handling */

struct stm32_interrupt_s
{
  int    irq;       /* irq number */
  int  error;       /* Interrupt error */
  sem_t *sem;       /* Semaphore for waiting for irq */
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
uint8_t stm32l4_dsi_config(void);
void stm32l4_dsi_enable(void);
void stm32l4_dsi_disable(void);
void stm32_dsi_refresh(void);

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/
DSI_HandleTypeDef    hdsi;
struct stm32l4_dsi_s dsidev;
/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Configure global register
 ****************************************************************************/

static const struct dsi_ops_s g_dsiops = {
  .config  = stm32l4_dsi_config,
  .enable  = stm32l4_dsi_enable,
  .disable = stm32l4_dsi_disable,
  .refresh = stm32_dsi_refresh,
};

uint8_t stm32l4_dsi_config(void)
{
    DSI_PLLInitTypeDef      dsiPllInit;
    DSI_PHY_TimerTypeDef    PhyTimings;
    DSI_HOST_TimeoutTypeDef HostTimeouts;
    DSI_LPCmdTypeDef        LPCmd;
    DSI_CmdCfgTypeDef       CmdCfg;

    /* DSI initialization */
    __HAL_DSI_RESET_HANDLE_STATE(&hdsi);
    hdsi.Instance = DSI;
    hdsi.Init.AutomaticClockLaneControl = DSI_AUTO_CLK_LANE_CTRL_DISABLE;
    /* We have 1 data lane at 500Mbps => lane byte clock at 500/8 = 62,5 MHZ */
    /* We want TX escape clock at arround 20MHz and under 20MHz so clock division is set to 4 */
    hdsi.Init.TXEscapeCkdiv             = 4;
    hdsi.Init.NumberOfLanes             = DSI_ONE_DATA_LANE;
    /* We have HSE value at 16 Mhz and we want data lane at 500Mbps */
    dsiPllInit.PLLNDIV = 125;
    dsiPllInit.PLLIDF  = DSI_PLL_IN_DIV4;
    dsiPllInit.PLLODF  = DSI_PLL_OUT_DIV1;
    if(HAL_DSI_Init(&hdsi, &dsiPllInit) != HAL_OK) {
        return(LCD_ERROR);
    }

    PhyTimings.ClockLaneHS2LPTime  = 33; /* Tclk-post + Tclk-trail + Ths-exit = [(60ns + 52xUI) + (60ns) + (300ns)]/16ns */
    PhyTimings.ClockLaneLP2HSTime  = 30; /* Tlpx + (Tclk-prepare + Tclk-zero) + Tclk-pre = [150ns + 300ns + 8xUI]/16ns */
    PhyTimings.DataLaneHS2LPTime   = 11; /* Ths-trail + Ths-exit = [(60ns + 4xUI) + 100ns]/16ns */
    PhyTimings.DataLaneLP2HSTime   = 21; /* Tlpx + (Ths-prepare + Ths-zero) + Ths-sync = [150ns + (145ns + 10xUI) + 8xUI]/16ns */
    PhyTimings.DataLaneMaxReadTime = 0;
    PhyTimings.StopWaitTime        = 7;
    if(HAL_DSI_ConfigPhyTimer(&hdsi, &PhyTimings) != HAL_OK) {
        return(LCD_ERROR);
    }

    HostTimeouts.TimeoutCkdiv                 = 1;
    HostTimeouts.HighSpeedTransmissionTimeout = 0;
    HostTimeouts.LowPowerReceptionTimeout     = 0;
    HostTimeouts.HighSpeedReadTimeout         = 0;
    HostTimeouts.LowPowerReadTimeout          = 0;
    HostTimeouts.HighSpeedWriteTimeout        = 0;
    HostTimeouts.HighSpeedWritePrespMode      = 0;
    HostTimeouts.LowPowerWriteTimeout         = 0;
    HostTimeouts.BTATimeout                   = 0;
    if(HAL_DSI_ConfigHostTimeouts(&hdsi, &HostTimeouts) != HAL_OK) {
        return(LCD_ERROR);
    }

    LPCmd.LPGenShortWriteNoP  = DSI_LP_GSW0P_ENABLE;
    LPCmd.LPGenShortWriteOneP = DSI_LP_GSW1P_ENABLE;
    LPCmd.LPGenShortWriteTwoP = DSI_LP_GSW2P_ENABLE;
    LPCmd.LPGenShortReadNoP   = DSI_LP_GSR0P_ENABLE;
    LPCmd.LPGenShortReadOneP  = DSI_LP_GSR1P_ENABLE;
    LPCmd.LPGenShortReadTwoP  = DSI_LP_GSR2P_ENABLE;
    LPCmd.LPGenLongWrite      = DSI_LP_GLW_DISABLE;
    LPCmd.LPDcsShortWriteNoP  = DSI_LP_DSW0P_ENABLE;
    LPCmd.LPDcsShortWriteOneP = DSI_LP_DSW1P_ENABLE;
    LPCmd.LPDcsShortReadNoP   = DSI_LP_DSR0P_ENABLE;
    LPCmd.LPDcsLongWrite      = DSI_LP_DLW_DISABLE;
    LPCmd.LPMaxReadPacket     = DSI_LP_MRDP_DISABLE;
    LPCmd.AcknowledgeRequest  = DSI_ACKNOWLEDGE_DISABLE;
    if(HAL_DSI_ConfigCommand(&hdsi, &LPCmd) != HAL_OK) {
        return(LCD_ERROR);
    }

    CmdCfg.VirtualChannelID      = 0;
    CmdCfg.ColorCoding           = DSI_RGB888;
    CmdCfg.CommandSize           = 390;
    CmdCfg.TearingEffectSource   = DSI_TE_DSILINK;
    CmdCfg.TearingEffectPolarity = DSI_TE_FALLING_EDGE;
    CmdCfg.HSPolarity            = DSI_HSYNC_ACTIVE_LOW;
    CmdCfg.VSPolarity            = DSI_VSYNC_ACTIVE_LOW;
    CmdCfg.DEPolarity            = DSI_DATA_ENABLE_ACTIVE_HIGH;
    CmdCfg.VSyncPol              = DSI_VSYNC_FALLING;
    CmdCfg.AutomaticRefresh      = DSI_AR_ENABLE;
    CmdCfg.TEAcknowledgeRequest  = DSI_TE_ACKNOWLEDGE_ENABLE;
    if(HAL_DSI_ConfigAdaptedCommandMode(&hdsi, &CmdCfg) != HAL_OK) {
        return(LCD_ERROR);
    }

    /* Disable the Tearing Effect interrupt activated by default on previous function */
    __HAL_DSI_DISABLE_IT(&hdsi, DSI_IT_TE);

    if(HAL_DSI_ConfigFlowControl(&hdsi, DSI_FLOW_CONTROL_BTA) != HAL_OK) {
        return(LCD_ERROR);
    }
}

void stm32l4_dsi_enable(void)
{
    __HAL_DSI_ENABLE(&hdsi);
}

void stm32l4_dsi_disable(void)
{
    __HAL_DSI_DISABLE(&hdsi);
}

int up_dsi_isr(int irq, uint32_t *regs)
{
    HAL_DSI_IRQHandler(&hdsi);
    return 0;
}

uint8_t stm32l4_dsi_initialize(void)
{
    DSI_PLLInitTypeDef      dsiPllInit;
    DSI_PHY_TimerTypeDef    PhyTimings;
    DSI_HOST_TimeoutTypeDef HostTimeouts;
    DSI_LPCmdTypeDef        LPCmd;
    DSI_CmdCfgTypeDef       CmdCfg;

    (void)irq_attach(STM32L4_IRQ_DSIHSOT, (xcpt_t)up_dsi_isr, NULL);
    /* DSI initialization */
    __HAL_DSI_RESET_HANDLE_STATE(&hdsi);
    hdsi.Instance = DSI;
    hdsi.Init.AutomaticClockLaneControl = DSI_AUTO_CLK_LANE_CTRL_DISABLE;
    /* We have 1 data lane at 500Mbps => lane byte clock at 500/8 = 62,5 MHZ */
    /* We want TX escape clock at arround 20MHz and under 20MHz so clock division is set to 4 */
    hdsi.Init.TXEscapeCkdiv             = 4;
    hdsi.Init.NumberOfLanes             = DSI_ONE_DATA_LANE;
    /* We have HSE value at 16 Mhz and we want data lane at 500Mbps */
    dsiPllInit.PLLNDIV = 125;
    dsiPllInit.PLLIDF  = DSI_PLL_IN_DIV4;
    dsiPllInit.PLLODF  = DSI_PLL_OUT_DIV1;
    if(HAL_DSI_Init(&hdsi, &dsiPllInit) != HAL_OK) {
        return(LCD_ERROR);
    }

    PhyTimings.ClockLaneHS2LPTime  = 33; /* Tclk-post + Tclk-trail + Ths-exit = [(60ns + 52xUI) + (60ns) + (300ns)]/16ns */
    PhyTimings.ClockLaneLP2HSTime  = 30; /* Tlpx + (Tclk-prepare + Tclk-zero) + Tclk-pre = [150ns + 300ns + 8xUI]/16ns */
    PhyTimings.DataLaneHS2LPTime   = 11; /* Ths-trail + Ths-exit = [(60ns + 4xUI) + 100ns]/16ns */
    PhyTimings.DataLaneLP2HSTime   = 21; /* Tlpx + (Ths-prepare + Ths-zero) + Ths-sync = [150ns + (145ns + 10xUI) + 8xUI]/16ns */
    PhyTimings.DataLaneMaxReadTime = 0;
    PhyTimings.StopWaitTime        = 7;
    if(HAL_DSI_ConfigPhyTimer(&hdsi, &PhyTimings) != HAL_OK) {
        return(LCD_ERROR);
    }

    HostTimeouts.TimeoutCkdiv                 = 1;
    HostTimeouts.HighSpeedTransmissionTimeout = 0;
    HostTimeouts.LowPowerReceptionTimeout     = 0;
    HostTimeouts.HighSpeedReadTimeout         = 0;
    HostTimeouts.LowPowerReadTimeout          = 0;
    HostTimeouts.HighSpeedWriteTimeout        = 0;
    HostTimeouts.HighSpeedWritePrespMode      = 0;
    HostTimeouts.LowPowerWriteTimeout         = 0;
    HostTimeouts.BTATimeout                   = 0;
    if(HAL_DSI_ConfigHostTimeouts(&hdsi, &HostTimeouts) != HAL_OK) {
        return(LCD_ERROR);
    }

    LPCmd.LPGenShortWriteNoP  = DSI_LP_GSW0P_ENABLE;
    LPCmd.LPGenShortWriteOneP = DSI_LP_GSW1P_ENABLE;
    LPCmd.LPGenShortWriteTwoP = DSI_LP_GSW2P_ENABLE;
    LPCmd.LPGenShortReadNoP   = DSI_LP_GSR0P_ENABLE;
    LPCmd.LPGenShortReadOneP  = DSI_LP_GSR1P_ENABLE;
    LPCmd.LPGenShortReadTwoP  = DSI_LP_GSR2P_ENABLE;
    LPCmd.LPGenLongWrite      = DSI_LP_GLW_DISABLE;
    LPCmd.LPDcsShortWriteNoP  = DSI_LP_DSW0P_ENABLE;
    LPCmd.LPDcsShortWriteOneP = DSI_LP_DSW1P_ENABLE;
    LPCmd.LPDcsShortReadNoP   = DSI_LP_DSR0P_ENABLE;
    LPCmd.LPDcsLongWrite      = DSI_LP_DLW_DISABLE;
    LPCmd.LPMaxReadPacket     = DSI_LP_MRDP_DISABLE;
    LPCmd.AcknowledgeRequest  = DSI_ACKNOWLEDGE_DISABLE;
    if(HAL_DSI_ConfigCommand(&hdsi, &LPCmd) != HAL_OK) {
        return(LCD_ERROR);
    }

    CmdCfg.VirtualChannelID      = 0;
    CmdCfg.ColorCoding           = DSI_RGB888;
    CmdCfg.CommandSize           = 390;
    CmdCfg.TearingEffectSource   = DSI_TE_DSILINK;
    CmdCfg.TearingEffectPolarity = DSI_TE_FALLING_EDGE;
    CmdCfg.HSPolarity            = DSI_HSYNC_ACTIVE_LOW;
    CmdCfg.VSPolarity            = DSI_VSYNC_ACTIVE_LOW;
    CmdCfg.DEPolarity            = DSI_DATA_ENABLE_ACTIVE_HIGH;
    CmdCfg.VSyncPol              = DSI_VSYNC_FALLING;
    CmdCfg.AutomaticRefresh      = DSI_AR_ENABLE;
    CmdCfg.TEAcknowledgeRequest  = DSI_TE_ACKNOWLEDGE_ENABLE;
    if(HAL_DSI_ConfigAdaptedCommandMode(&hdsi, &CmdCfg) != HAL_OK) {
        return(LCD_ERROR);
    }

    /* Disable the Tearing Effect interrupt activated by default on previous function */
    __HAL_DSI_DISABLE_IT(&hdsi, DSI_IT_TE);

    if(HAL_DSI_ConfigFlowControl(&hdsi, DSI_FLOW_CONTROL_BTA) != HAL_OK) {
        return(LCD_ERROR);
    }
  
    stm32l4_dsi_enable();

    return OK;
}

void stm32_dsi_refresh(void)
{
    HAL_DSI_Refresh(&hdsi);
}

/**
  * @brief  End of Refresh DSI callback.
  * @param  hdsi: pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @retval None
  */
void HAL_DSI_EndOfRefreshCallback(DSI_HandleTypeDef *hdsi)
{
    /* Clear pending tearing effect flag */
    __HAL_DSI_CLEAR_FLAG(hdsi, DSI_FLAG_TE);
    /* Set frame buffer available */
    dsidev.isframebuffer = 1;
}
