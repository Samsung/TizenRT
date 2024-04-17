/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/lcd/st7701.h>
#include <tinyara/mipidsi/mipi_dsi.h>
#include "objects.h"
#include "PinNames.h"
#include "gpio_api.h"

static void rtl8730_st7701_lcd_init(void);
static void rtl8730_st7701_gpio_reset(void);
static void rtl8730_st7701_cache_invalidate(u32 *buffer, int size);
static void rtl8730_st7701_lcd_reload(void);
static void rtl8730_st7701_lcd_enable(void);
static void rtl8730_st7701_lcd_layer_enable(int layer, bool enable);
static void rtl8730_st7701_lcd_put_area(u8 *lcd_img_buffer, u32 x1, u32 y1, u32 x2, u32 y2);
static void rtl8730_st7701_enable_lcdc(void);
FAR void mipidsi_mode_switch(bool do_enable);
FAR struct mipi_dsi_host *mipi_dsi_host_initialize(struct lcd_data *config);
FAR struct mipi_dsi_device *mipi_dsi_device_register(FAR struct mipi_dsi_host *host, FAR const char *name, int channel);
FAR struct lcd_dev_s *st7701_lcdinitialize(FAR struct mipi_dsi_device *dsi, struct st7701_config_s *config);

struct st7701_config_s g_rtl8730_st7701_config_s = {
	.init = rtl8730_st7701_lcd_init,
	.reset = rtl8730_st7701_gpio_reset,
	.cache_invalidate = rtl8730_st7701_cache_invalidate,
	.lcd_reload = rtl8730_st7701_lcd_reload,
	.lcd_enable = rtl8730_st7701_lcd_enable,
	.lcd_layer_enable = rtl8730_st7701_lcd_layer_enable,
	.lcd_put_area = rtl8730_st7701_lcd_put_area,
};

LCDC_TypeDef *pLCDC = LCDC;
LCDC_InitTypeDef lcdc_init_struct;

u8 *lcd_img_buffer1 = (u8 *)(DDR_BASE + LCDC_IMG_BUF_OFFSET2 + LCDC_IMG_BUF_SIZE);

static void LcdcInitValues(struct lcd_data config)
{
	LCDC_StructInit(&lcdc_init_struct);
	lcdc_init_struct.LCDC_ImageWidth = config.XPixels;
	lcdc_init_struct.LCDC_ImageHeight = config.YPixels;
	for (u8 idx = 0; idx < LCDC_LAYER_MAX_NUM; idx++) {
		lcdc_init_struct.layerx[idx].LCDC_LayerImgFormat = LCDC_LAYER_IMG_FORMAT_RGB565;
		lcdc_init_struct.layerx[idx].LCDC_LayerImgBaseAddr = (u32) lcd_img_buffer1;
		lcdc_init_struct.layerx[idx].LCDC_LayerHorizontalStart = 1;
		lcdc_init_struct.layerx[idx].LCDC_LayerHorizontalStop = config.XPixels;
		lcdc_init_struct.layerx[idx].LCDC_LayerVerticalStart = 1;
		lcdc_init_struct.layerx[idx].LCDC_LayerVerticalStop = config.YPixels;
		lcdc_init_struct.layerx[idx].LCDC_LayerColorKeyingEn = ENABLE;
	}
}

static u32 LCDCRgbHvIsr(void *Data)
{
	LCDC_TypeDef *LCDCx = (LCDC_TypeDef *) Data;
	volatile u32 IntId = LCDC_GetINTStatus(LCDCx);

	if (IntId & LCDC_BIT_LCD_LIN_INTS) {
		LCDC_ClearINT(LCDCx, LCDC_BIT_LCD_LIN_INTS);
	}
	if (IntId & LCDC_BIT_DMA_UN_INTS) {
		LCDC_ClearINT(LCDCx, LCDC_BIT_DMA_UN_INTS);
	}
	return 0;
}

static void rtl8730_st7701_lcd_init(void)
{
	rtl8730_st7701_lcd_layer_enable(0, true);
	rtl8730_st7701_lcd_layer_enable(1, false);
	rtl8730_st7701_lcd_layer_enable(2, false);

	LCDC_Init(pLCDC, &lcdc_init_struct);
	LCDC_DMAModeConfig(pLCDC, LCDC_LAYER_BURSTSIZE_4X64BYTES);
	LCDC_DMADebugConfig(pLCDC, LCDC_DMA_OUT_DISABLE, 0);

	LCDC_LineINTPosConfig(pLCDC, YRES * 4 / 5);
	LCDC_INTConfig(pLCDC, LCDC_BIT_LCD_LIN_INTEN | LCDC_BIT_DMA_UN_INTEN, ENABLE);
	return;
}

static void rtl8730_st7701_gpio_reset(void)
{
	return;
}

static void rtl8730_st7701_cache_invalidate(u32 *buffer, int size)
{
	DCache_CleanInvalidate((u32) buffer, size);
	return;
}

static void rtl8730_st7701_lcd_reload(void)
{
	LCDC_TrigerSHWReload(pLCDC);
	return;
}

static void rtl8730_st7701_lcd_enable(void)
{
	rtl8730_st7701_enable_lcdc();
	return;
}

static void rtl8730_st7701_lcd_layer_enable(int layer, bool enable)
{
	if (enable) {
		lcdc_init_struct.layerx[layer].LCDC_LayerEn = ENABLE;
	} else {
		lcdc_init_struct.layerx[layer].LCDC_LayerEn = DISABLE;
	}
	return;
}

static void rtl8730_st7701_lcd_put_area(u8 *lcd_img_buffer, u32 x1, u32 y1, u32 x2, u32 y2)
{
	lcdc_init_struct.layerx[0].LCDC_LayerImgBaseAddr = (u32) lcd_img_buffer;

	//Uncomment below lines in case LCD window size needs to be changed...
	/*
	lcdc_init_struct.layerx[0].LCDC_LayerHorizontalStart = x1;
	lcdc_init_struct.layerx[0].LCDC_LayerHorizontalStop = x2;
	lcdc_init_struct.layerx[0].LCDC_LayerVerticalStart = y1;
	lcdc_init_struct.layerx[0].LCDC_LayerVerticalStop = y2;
	*/
	LCDC_LayerConfig(pLCDC, 0, &lcdc_init_struct.layerx[0]);
	LCDC_TrigerSHWReload(pLCDC);
	return;
}

static void rtl8730e_st7701_reset_pin(u8 Newstatus)
{
	u32 pin_name = _PA_14;

	Pinmux_Swdoff();
	Pinmux_Config(pin_name, PINMUX_FUNCTION_GPIO);

	GPIO_InitTypeDef ResetPin;
	ResetPin.GPIO_Pin = pin_name;
	ResetPin.GPIO_PuPd = GPIO_PuPd_NOPULL;
	ResetPin.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(&ResetPin);

	if (Newstatus) {
		GPIO_WriteBit(pin_name, 1);
	} else {
		GPIO_WriteBit(pin_name, 0);
	}
}

static void rtl8730_st7701_enable_lcdc(void)
{
	/*enable the LCDC */
	LCDC_Cmd(pLCDC, ENABLE);
	while (!LCDC_CheckLCDCReady(pLCDC)) ;

	mipidsi_mode_switch(true);
	return;
}

void rtl8730_st7701_initialize(void)
{
	struct lcd_data config;
	config.XPixels = XRES;
	config.YPixels = YRES;

	struct mipi_dsi_host *dsi_host = (struct mipi_dsi_host *)mipi_dsi_host_initialize(&config);
	struct mipi_dsi_device *dsi_device = (struct mipi_dsi_device *)mipi_dsi_device_register(dsi_host, "dsi", 0);
	struct lcd_dev_s *dev = (struct lcd_dev_s *)st7701_lcdinitialize(dsi_device, &g_rtl8730_st7701_config_s);
	LcdcInitValues(config);
	rtl8730_st7701_lcd_init();

	if (lcddev_register(dev) < 0) {
		lldbg("ERROR: LCD driver register fail\n");
	} else {
		lldbg("LCD driver register success\n");
	}
	rtl8730_st7701_enable_lcdc();
	
	rtl8730e_st7701_reset_pin(1);
	DelayMs(10);
	rtl8730e_st7701_reset_pin(0);
	DelayMs(10);
	rtl8730e_st7701_reset_pin(1);
	DelayMs(120);

	return;
}
