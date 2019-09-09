/**
******************************************************************************
* @file    stm32_lcd.c
* @author  MCD Application Team
* @brief   This file provides a set of firmware functions to manage Leds,
*          push-button and joystick of STM32L4R9I_DISCOVERY board (MB1311)
******************************************************************************
* @attention
*
* <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
* All rights reserved.</center></h2>
*
* This software component is licensed by ST under BSD 3-Clause license,
* the "License"; You may not use this file except in compliance with the
* License. You may obtain a copy of the License at:
*                        opensource.org/licenses/BSD-3-Clause
*
******************************************************************************
*/
#include <tinyara/config.h>

#include <debug.h>
#include <stdint.h>

#include <tinyara/arch.h>
#include <tinyara/board.h>
#include <tinyara/spi/spi.h>

#include <arch/board/board.h>
#include <arch/board/stm32l4r9ai-disco.h>
#include <arch/board/stm32l4r9i_discovery.h>
#include <arch/board/image_390x390_rgb888.h>

#define BRIGHTNESS_MIN      50
#define BRIGHTNESS_NORMAL   200

uint32_t pframebuffer[114075] __attribute__ ((aligned (16)));

DSI_HandleTypeDef hdsi;
LTDC_HandleTypeDef hltdc;
DMA2D_HandleTypeDef   hdma2d;

static uint8_t LCD_Config(void);
static void LCD_PowerOn(void);
static void CopyInVirtualBuffer(uint32_t *pSrc, uint32_t *pDst, uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize);

static void CopyInVirtualBuffer(uint32_t *pSrc, uint32_t *pDst, uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize)
{
  uint32_t destination = (uint32_t)pDst + (y * 390 + x) * 4;
  uint32_t source      = (uint32_t)pSrc;

  hdma2d.Instance          = DMA2D;

  /*##-1- Configure the DMA2D Mode, Color Mode and output offset #############*/
  hdma2d.Init.Mode           = DMA2D_M2M_PFC;
  hdma2d.Init.ColorMode      = DMA2D_OUTPUT_RGB888;
  hdma2d.Init.OutputOffset   = 0;//1024 - 390;
  hdma2d.Init.AlphaInverted  = DMA2D_REGULAR_ALPHA;  /* No Output Alpha Inversion */
  hdma2d.Init.RedBlueSwap    = DMA2D_RB_REGULAR;     /* No Output Red & Blue swap */
  hdma2d.Init.BytesSwap      = DMA2D_BYTES_REGULAR;  /* Regular output byte order */
  hdma2d.Init.LineOffsetMode = DMA2D_LOM_PIXELS;     /* Pixel mode                */

  /*##-2- Foreground Configuration ###########################################*/
  hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB888;
  hdma2d.LayerCfg[1].InputOffset    = 0;
  hdma2d.LayerCfg[1].AlphaMode      = DMA2D_NO_MODIF_ALPHA;
  hdma2d.LayerCfg[1].InputAlpha     = 0xFF;                /* Not used */
  hdma2d.LayerCfg[1].RedBlueSwap    = DMA2D_RB_SWAP; //DMA2D_RB_REGULAR;    /* No ForeGround Red/Blue swap */
  hdma2d.LayerCfg[1].AlphaInverted  = DMA2D_REGULAR_ALPHA; /* No ForeGround Alpha inversion */

  /* DMA2D Initialization */
  if(HAL_DMA2D_Init(&hdma2d) == HAL_OK)
  {
    if(HAL_DMA2D_ConfigLayer(&hdma2d, 1) == HAL_OK)
    {
      if (HAL_DMA2D_Start(&hdma2d, source, destination, xsize, ysize) == HAL_OK)
      {
        /* Polling For DMA transfer */
        HAL_DMA2D_PollForTransfer(&hdma2d, 100);
      }
    }
  }
}


void stm32l4_lcdinitialize(void)
{
  if(LCD_Config() != LCD_OK)
  {
      while(1);
  }
  /*Draw image in GFXMMU (Chome-GRC) Virtual buffer*/
  CopyInVirtualBuffer((uint32_t *)image_390x390_rgb888, (uint32_t *)pframebuffer/*LAYER_ADDRESS*/, 0, 0, 390, 390);
  
  /*Refresh the LCD display*/
  HAL_DSI_Refresh(&hdsi);
}

int up_dsi_isr(int irq, uint32_t *regs)
{
    HAL_DSI_IRQHandler(&hdsi);
    return 0;
}

int up_ltdc_isr(int irq, uint32_t *regs)
{
    return 0;
}

int up_ltdc_er_isr(int irq, uint32_t *regs)
{
    return 0;
}

static uint8_t LCD_Config(void)
{
    LTDC_LayerCfgTypeDef    LayerCfg = {0};
    DSI_PLLInitTypeDef      dsiPllInit = {0};
    DSI_PHY_TimerTypeDef    PhyTimings = {0};
    DSI_HOST_TimeoutTypeDef HostTimeouts = {0};
    DSI_LPCmdTypeDef        LPCmd = {0};
    DSI_CmdCfgTypeDef       CmdCfg = {0};

  /* Power on LCD */
  LCD_PowerOn();

  /**********************/
  /* LTDC CONFIGURATION */
  /**********************/

  /* LTDC initialization */
  (void)irq_attach(STM32L4_IRQ_LCD_TFT, (xcpt_t)up_ltdc_isr, NULL);
  (void)irq_attach(STM32L4_IRQ_LCD_TFT_ER, (xcpt_t)up_ltdc_er_isr, NULL);

  __HAL_LTDC_RESET_HANDLE_STATE(&hltdc);
  hltdc.Instance = LTDC;
  hltdc.Init.HSPolarity         = LTDC_HSPOLARITY_AL;
  hltdc.Init.VSPolarity         = LTDC_VSPOLARITY_AL;
  hltdc.Init.DEPolarity         = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity         = LTDC_PCPOLARITY_IPC;
  hltdc.Init.HorizontalSync     = 0;   /* HSYNC width - 1 */
  hltdc.Init.VerticalSync       = 0;   /* VSYNC width - 1 */
  hltdc.Init.AccumulatedHBP     = 1;   /* HSYNC width + HBP - 1 */
  hltdc.Init.AccumulatedVBP     = 1;   /* VSYNC width + VBP - 1 */
  hltdc.Init.AccumulatedActiveW = 391; /* HSYNC width + HBP + Active width - 1 */
  hltdc.Init.AccumulatedActiveH = 391; /* VSYNC width + VBP + Active height - 1 */
  hltdc.Init.TotalWidth         = 392; /* HSYNC width + HBP + Active width + HFP - 1 */
  hltdc.Init.TotalHeigh         = 392; /* VSYNC width + VBP + Active height + VFP - 1 */
  hltdc.Init.Backcolor.Red      = 0;   /* Not used default value */
  hltdc.Init.Backcolor.Green    = 0;   /* Not used default value */
  hltdc.Init.Backcolor.Blue     = 0;   /* Not used default value */
  hltdc.Init.Backcolor.Reserved = 0xFF;
  if(HAL_LTDC_Init(&hltdc) != HAL_OK)
  {
    return(LCD_ERROR);
  }

  /* LTDC layer 1 configuration */
  LayerCfg.WindowX0        = 0;
  LayerCfg.WindowX1        = 390;
  LayerCfg.WindowY0        = 0;
  LayerCfg.WindowY1        = 390;
  LayerCfg.PixelFormat     = LTDC_PIXEL_FORMAT_RGB888;
  LayerCfg.Alpha           = 0xFF; /* NU default value */
  LayerCfg.Alpha0          = 0; /* NU default value */
  LayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA; /* Not Used: default value */
  LayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA; /* Not Used: default value */
  LayerCfg.FBStartAdress   = (uint32_t)pframebuffer;//LAYER_ADDRESS;
  LayerCfg.ImageWidth      = 390;//1024; /* virtual frame buffer contains 768 pixels per line for 24bpp */
                                   /* (192 blocs * 16) / (24bpp/3) = 1024 pixels per ligne        */
  LayerCfg.ImageHeight     = 390;
  LayerCfg.Backcolor.Red   = 0; /* Not Used: default value */
  LayerCfg.Backcolor.Green = 0; /* Not Used: default value */
  LayerCfg.Backcolor.Blue  = 0; /* Not Used: default value */
  LayerCfg.Backcolor.Reserved = 0xFF;
  if(HAL_LTDC_ConfigLayer(&hltdc, &LayerCfg, LTDC_LAYER_1) != HAL_OK)
  {
    return(LCD_ERROR);
  }

  /*********************/
  /* DSI CONFIGURATION */
  /*********************/
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
  if(HAL_DSI_Init(&hdsi, &dsiPllInit) != HAL_OK)
  {
    return(LCD_ERROR);
  }

  PhyTimings.ClockLaneHS2LPTime  = 33; /* Tclk-post + Tclk-trail + Ths-exit = [(60ns + 52xUI) + (60ns) + (300ns)]/16ns */
  PhyTimings.ClockLaneLP2HSTime  = 30; /* Tlpx + (Tclk-prepare + Tclk-zero) + Tclk-pre = [150ns + 300ns + 8xUI]/16ns */
  PhyTimings.DataLaneHS2LPTime   = 11; /* Ths-trail + Ths-exit = [(60ns + 4xUI) + 100ns]/16ns */
  PhyTimings.DataLaneLP2HSTime   = 21; /* Tlpx + (Ths-prepare + Ths-zero) + Ths-sync = [150ns + (145ns + 10xUI) + 8xUI]/16ns */
  PhyTimings.DataLaneMaxReadTime = 0;
  PhyTimings.StopWaitTime        = 7;
  if(HAL_DSI_ConfigPhyTimer(&hdsi, &PhyTimings) != HAL_OK)
  {
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
  if(HAL_DSI_ConfigHostTimeouts(&hdsi, &HostTimeouts) != HAL_OK)
  {
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
  if(HAL_DSI_ConfigCommand(&hdsi, &LPCmd) != HAL_OK)
  {
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
  if(HAL_DSI_ConfigAdaptedCommandMode(&hdsi, &CmdCfg) != HAL_OK)
  {
    return(LCD_ERROR);
  }

  /* Disable the Tearing Effect interrupt activated by default on previous function */
  __HAL_DSI_DISABLE_IT(&hdsi, DSI_IT_TE);

  if(HAL_DSI_ConfigFlowControl(&hdsi, DSI_FLOW_CONTROL_BTA) != HAL_OK)
  {
    return(LCD_ERROR);
  }

  /* Enable DSI */
  __HAL_DSI_ENABLE(&hdsi);

  /*************************/
  /* LCD POWER ON SEQUENCE */
  /*************************/
  /* Step 1 */
  /* Go to command 2 */
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0xFE, 0x01);
  /* IC Frame rate control, set power, sw mapping, mux swithc timing command */
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x06, 0x62);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x0E, 0x80);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x0F, 0x80);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x10, 0x71);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x13, 0x81);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x14, 0x81);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x15, 0x82);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x16, 0x82);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x18, 0x88);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x19, 0x55);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x1A, 0x10);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x1C, 0x99);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x1D, 0x03);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x1E, 0x03);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x1F, 0x03);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x20, 0x03);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x25, 0x03);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x26, 0x8D);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x2A, 0x03);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x2B, 0x8D);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x36, 0x00);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x37, 0x10);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x3A, 0x00);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x3B, 0x00);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x3D, 0x20);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x3F, 0x3A);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x40, 0x30);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x41, 0x1A);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x42, 0x33);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x43, 0x22);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x44, 0x11);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x45, 0x66);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x46, 0x55);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x47, 0x44);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x4C, 0x33);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x4D, 0x22);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x4E, 0x11);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x4F, 0x66);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x50, 0x55);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x51, 0x44);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x57, 0x33);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x6B, 0x1B);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x70, 0x55);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x74, 0x0C);

  /* Go to command 3 */
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0xFE, 0x02);
  /* Set the VGMP/VGSP coltage control */
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x9B, 0x40);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x9C, 0x00);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x9D, 0x20);

  /* Go to command 4 */
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0xFE, 0x03);
  /* Set the VGMP/VGSP coltage control */
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x9B, 0x40);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x9C, 0x00);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x9D, 0x20);

  /* Go to command 5 */
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0xFE, 0x04);
  /* VSR command */
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x5D, 0x10);
  /* VSR1 timing set */
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x00, 0x8D);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x01, 0x00);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x02, 0x01);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x03, 0x01);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x04, 0x10);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x05, 0x01);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x06, 0xA7);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x07, 0x20);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x08, 0x00);
  /* VSR2 timing set */
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x09, 0xC2);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x0A, 0x00);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x0B, 0x02);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x0C, 0x01);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x0D, 0x40);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x0E, 0x06);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x0F, 0x01);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x10, 0xA7);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x11, 0x00);
  /* VSR3 timing set */
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x12, 0xC2);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x13, 0x00);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x14, 0x02);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x15, 0x01);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x16, 0x40);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x17, 0x07);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x18, 0x01);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x19, 0xA7);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x1A, 0x00);
  /* VSR4 timing set */
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x1B, 0x82);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x1C, 0x00);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x1D, 0xFF);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x1E, 0x05);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x1F, 0x60);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x20, 0x02);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x21, 0x01);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x22, 0x7C);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x23, 0x00);
  /* VSR5 timing set */
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x24, 0xC2);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x25, 0x00);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x26, 0x04);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x27, 0x02);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x28, 0x70);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x29, 0x05);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x2A, 0x74);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x2B, 0x8D);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x2D, 0x00);
  /* VSR6 timing set */
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x2F, 0xC2);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x30, 0x00);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x31, 0x04);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x32, 0x02);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x33, 0x70);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x34, 0x07);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x35, 0x74);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x36, 0x8D);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x37, 0x00);
  /* VSR marping command */
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x5E, 0x20);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x5F, 0x31);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x60, 0x54);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x61, 0x76);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x62, 0x98);

  /* Go to command 6 */
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0xFE, 0x05);
  /* Set the ELVSS voltage */
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x05, 0x17);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x2A, 0x04);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x91, 0x00);

  /* Go back in standard commands */
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0xFE, 0x00);

  /* Set the Pixel format */
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x3A, 0x07);

  /* Set tear off */
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, DSI_SET_TEAR_OFF, 0x0);

  /* Set DSI mode to internal timing added vs ORIGINAL for Command mode */
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0xC2, 0x0);

  /* Set memory address MODIFIED vs ORIGINAL */
  {
    uint8_t InitParam1[4]= {0x00, 0x04, 0x01, 0x89};
    uint8_t InitParam2[4]= {0x00, 0x00, 0x01, 0x85};

    HAL_DSI_LongWrite(&hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, DSI_SET_COLUMN_ADDRESS, InitParam1);
    HAL_DSI_LongWrite(&hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, DSI_SET_PAGE_ADDRESS, InitParam2);
  }

  /* Sleep out */
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P0, DSI_EXIT_SLEEP_MODE, 0x0);

  HAL_Delay(120);

  /* Set default Brightness */
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x51, BRIGHTNESS_NORMAL);

  /* Set display on */
  if(HAL_DSI_ShortWrite(&hdsi,
                        0,
                        DSI_DCS_SHORT_PKT_WRITE_P0,
                        DSI_SET_DISPLAY_ON,
                        0x0) != HAL_OK)
  {
    return(LCD_ERROR);
  }

  /* Enable DSI Wrapper */
  __HAL_DSI_WRAPPER_ENABLE(&hdsi);

  return LCD_OK;
}

static void LCD_PowerOn(void)
{
  BSP_IO_Init();

  /* Configure the GPIO connected to DSI_3V3_POWERON signal as output low */
  /* to activate 3V3_LCD. VDD_LCD is also activated if VDD = 3,3V */
  BSP_IO_WritePin(IO_PIN_8, GPIO_PIN_RESET);
  BSP_IO_ConfigPin(IO_PIN_8, IO_MODE_OUTPUT);

  /* Wait at least 1ms before enabling 1V8_LCD */
  HAL_Delay(1);

  /* Configure the GPIO connected to DSI_1V8_POWERON signal as output low */
  /* to activate 1V8_LCD. VDD_LCD is also activated if VDD = 1,8V */
  BSP_IO_WritePin(AGPIO_PIN_2, GPIO_PIN_RESET);
  BSP_IO_ConfigPin(AGPIO_PIN_2, IO_MODE_OUTPUT);

  /* Wait at least 15 ms (minimum reset low width is 10ms and add margin for 1V8_LCD ramp-up) */
  HAL_Delay(15);

  /* Configure the GPIO connected to DSI_RESET signal */
  BSP_IO_ConfigPin(IO_PIN_10, IO_MODE_OUTPUT);

  /* Desactivate DSI_RESET */
  BSP_IO_WritePin(IO_PIN_10, GPIO_PIN_SET);

  /* Wait reset complete time (maximum time is 5ms when LCD in sleep mode and 120ms when LCD is not in sleep mode) */
  HAL_Delay(120);
}
