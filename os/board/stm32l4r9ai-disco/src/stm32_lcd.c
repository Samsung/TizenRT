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
#include <stdio.h>
#include <stdint.h>

#include <tinyara/arch.h>
#include <tinyara/board.h>
#include <tinyara/spi/spi.h>

#include <arch/board/board.h>
#include <arch/board/stm32l4r9i_discovery.h>

#include <stm32l4.h>

#include "chip/stm32l4_dma2d.h"
#include "stm32l4r9ai-disco.h"
#include "stm32l4_dsi.h"

#define BRIGHTNESS_MIN      50
#define BRIGHTNESS_NORMAL   200

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static uint8_t stm32l4r9i_lcd_initialize(void);

/****************************************************************************
 * Public Functions
 ****************************************************************************/
void stm32l4r9i_lcd_poweron(void);
void stm32l4r9i_lcd_poweroff(void);
void lcd_refresh(void);

FAR struct dma2d_layer_s *up_dma2ddev(void);
extern struct stm32l4_dsi_s dsidev;
extern uint32_t bsp_lcd_initialized;

FAR struct dma2d_layer_s *up_dma2ddev(void)
{
    return stm32l4_dma2ddev();
}

void stm32l4r9i_lcd_poweron(void)
{
    FAR struct stm32l4_lcd_s *lcd = stm32l4_lcd_function();

    if (lcd == NULL){
        return;
    }else{
        lcd->poweron();
        printf("LCD Power On\n");
    }
}

void stm32l4r9i_lcd_poweroff(void)
{
    FAR struct stm32l4_lcd_s *lcd = stm32l4_lcd_function();

    if (lcd == NULL){
        return;
    }else{
        lcd->poweroff();
        printf("LCD Power Off\n");
    }
}

#if 0 // testing ... 
void dma2d_copybuffer(uint32_t *psrc, uint32_t *pdst, uint16_t x, uint16_t y,
                      uint16_t xsize, uint16_t ysize, ui_pixel_format_t pf)
#else
void dma2d_copybuffer(uint32_t *psrc, uint32_t *pdst, uint16_t x, uint16_t y,
                      uint16_t xsize, uint16_t ysize, bool alpha)
#endif
{
    FAR struct dma2d_layer_s *priv = up_dma2ddev();
    
    if(alpha == true)
    {
      priv->inputcolor(DMA2D_PF_ARGB8888);
    }
    else
    {
      priv->inputcolor(DMA2D_PF_RGB888);
    }
    priv->copybuffer(psrc, pdst, x, y, xsize, ysize);
    //STM32L4_LCD_Refresh();
}

void dma2d_fillcolor(FAR struct stm32_dma2d_overlay_s *oinfo,
                     FAR const struct fb_area_s *area, uint32_t argb)
{
    FAR struct dma2d_layer_s *dma2d = up_dma2ddev();
    dma2d->fillcolor(oinfo, area, argb);
}

void lcd_refresh(void)
{
    FAR struct stm32l4_lcd_s *lcd = stm32l4_lcd_function();

    lcd->refresh();
    
    while(STM32L4_LCD_IsFBAvailable() != LCD_OK){};
}

static uint8_t stm32l4r9i_lcd_initialize(void)
{
    FAR struct stm32l4_lcd_s *lcd = stm32l4_lcd_function();
    if (lcd == NULL){
        return LCD_ERROR;
    }

    /*************************/
    /* LCD POWER ON SEQUENCE */
    /*************************/
    /* Step 1 */
    /* Go to command 2 */
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0xFE, 0x01);
    /* IC Frame rate control, set power, sw mapping, mux swithc timing command */
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x06, 0x62);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x0E, 0x80);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x0F, 0x80);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x10, 0x71);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x13, 0x81);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x14, 0x81);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x15, 0x82);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x16, 0x82);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x18, 0x88);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x19, 0x55);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x1A, 0x10);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x1C, 0x99);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x1D, 0x03);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x1E, 0x03);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x1F, 0x03);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x20, 0x03);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x25, 0x03);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x26, 0x8D);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x2A, 0x03);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x2B, 0x8D);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x36, 0x00);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x37, 0x10);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x3A, 0x00);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x3B, 0x00);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x3D, 0x20);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x3F, 0x3A);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x40, 0x30);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x41, 0x1A);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x42, 0x33);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x43, 0x22);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x44, 0x11);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x45, 0x66);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x46, 0x55);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x47, 0x44);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x4C, 0x33);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x4D, 0x22);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x4E, 0x11);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x4F, 0x66);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x50, 0x55);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x51, 0x44);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x57, 0x33);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x6B, 0x1B);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x70, 0x55);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x74, 0x0C);

    /* Go to command 3 */
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0xFE, 0x02);
    /* Set the VGMP/VGSP coltage control */
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x9B, 0x40);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x9C, 0x00);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x9D, 0x20);

    /* Go to command 4 */
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0xFE, 0x03);
    /* Set the VGMP/VGSP coltage control */
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x9B, 0x40);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x9C, 0x00);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x9D, 0x20);

    /* Go to command 5 */
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0xFE, 0x04);
    /* VSR command */
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x5D, 0x10);
    /* VSR1 timing set */
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x00, 0x8D);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x01, 0x00);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x02, 0x01);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x03, 0x01);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x04, 0x10);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x05, 0x01);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x06, 0xA7);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x07, 0x20);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x08, 0x00);
    /* VSR2 timing set */
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x09, 0xC2);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x0A, 0x00);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x0B, 0x02);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x0C, 0x01);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x0D, 0x40);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x0E, 0x06);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x0F, 0x01);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x10, 0xA7);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x11, 0x00);
    /* VSR3 timing set */
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x12, 0xC2);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x13, 0x00);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x14, 0x02);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x15, 0x01);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x16, 0x40);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x17, 0x07);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x18, 0x01);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x19, 0xA7);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x1A, 0x00);
    /* VSR4 timing set */
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x1B, 0x82);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x1C, 0x00);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x1D, 0xFF);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x1E, 0x05);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x1F, 0x60);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x20, 0x02);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x21, 0x01);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x22, 0x7C);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x23, 0x00);
    /* VSR5 timing set */
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x24, 0xC2);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x25, 0x00);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x26, 0x04);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x27, 0x02);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x28, 0x70);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x29, 0x05);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x2A, 0x74);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x2B, 0x8D);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x2D, 0x00);
    /* VSR6 timing set */
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x2F, 0xC2);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x30, 0x00);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x31, 0x04);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x32, 0x02);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x33, 0x70);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x34, 0x07);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x35, 0x74);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x36, 0x8D);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x37, 0x00);
    /* VSR marping command */
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x5E, 0x20);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x5F, 0x31);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x60, 0x54);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x61, 0x76);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x62, 0x98);

    /* Go to command 6 */
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0xFE, 0x05);
    /* Set the ELVSS voltage */
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x05, 0x17);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x2A, 0x04);
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x91, 0x00);

    /* Go back in standard commands */
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0xFE, 0x00);

    /* Set the Pixel format */
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x3A, 0x07);

    /* Set tear off */
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, DSI_SET_TEAR_OFF, 0x0);
    
    /* Set DSI mode to internal timing added vs ORIGINAL for Command mode */
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0xC2, 0x0);

    /* Set memory address MODIFIED vs ORIGINAL */
    {
        uint8_t InitParam1[4]= {0x00, 0x04, 0x01, 0x89};
        uint8_t InitParam2[4]= {0x00, 0x00, 0x01, 0x85};

        lcd->sendlparam(DSI_DCS_LONG_PKT_WRITE, 4, DSI_SET_COLUMN_ADDRESS, InitParam1);
        lcd->sendlparam(DSI_DCS_LONG_PKT_WRITE, 4, DSI_SET_PAGE_ADDRESS, InitParam2);
    }

    /* Sleep out */
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P0, DSI_EXIT_SLEEP_MODE, 0x0);

    HAL_Delay(120);

    /* Set default Brightness */
    lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P1, 0x51, BRIGHTNESS_NORMAL);

    /* Set display on */
    if(lcd->sendsparam(DSI_DCS_SHORT_PKT_WRITE_P0, DSI_SET_DISPLAY_ON, 0x0) != HAL_OK) {
        return(LCD_ERROR);
    }
    /* Enable DSI Wrapper */
    lcd->enablewap();

    return LCD_OK;    
}

uint8_t stm32l4_lcdinitialize(void)
{
    //static bool ltdc_initialized = false;
    //static bool dsi_initialized = false;
    uint8_t res = LCD_ERROR;
    FAR struct stm32l4_dsi_s *priv = &dsidev;
    /* Power On LCD */
    LCD_PowerOn();

    printf("LCD Power On\n");

    if(stm32l4_ltdc_initialize() != LCD_OK){
        printf("LTDC init error 0x%x\n", res);
        return res;
    }
    
    if(stm32l4_dsi_initialize() != LCD_OK){
        printf("DSI Init error 0x%x\n", res);
        return res;
    }

    if(stm32l4r9i_lcd_initialize() != LCD_OK){
        printf("BSP LCD Init error 0x%x\n", res);
        return res;
    }

    printf("LCD Initialization done\n");
    bsp_lcd_initialized = 1;
    priv->isframebuffer = 1;

    return res;
}


