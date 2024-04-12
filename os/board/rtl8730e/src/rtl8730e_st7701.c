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


static int rtl8730_st7701_lcd_init(void);
static int rtl8730_st7701_gpio_reset(void);
static int rtl8730_st7701_cache_invalidate(u32 *buffer, int size);
static int rtl8730_st7701_lcd_reload(void);
static int rtl8730_st7701_lcd_enable(void);
static int rtl8730_st7701_lcd_layer_enable(int layer, bool enable);
static void lcd_put_area(u8 *LcdImgBuffer, u32 x1, u32 y1, u32 x2, u32 y2);
static void LcdcEnable(void);

struct st7701_config_s g_rtl8730_st7701_config_s = {
	.init = rtl8730_st7701_lcd_init,
	.reset = rtl8730_st7701_gpio_reset,
    .cache_invalidate = rtl8730_st7701_cache_invalidate,
    .lcd_reload = rtl8730_st7701_lcd_reload,
	.lcd_enable = rtl8730_st7701_lcd_enable,
	.lcd_layer_enable = rtl8730_st7701_lcd_layer_enable,
	.lcd_put_area = lcd_put_area,
};

typedef struct {
	u8 blue;
	u8 green;
	u8 red;
	u8 alpha;
} argb8888_t;

typedef struct {
	u8 blue;
	u8 green;
	u8 red;
} rgb888_t;

/*=========LCDC IRQ Info=========*/
typedef struct LCDC_Irq {
	u32 IrqNum;
	u32 IrqData;
	u32 IrqPriority;
} LCDC_IRQInfo ;

/*initialize the LCDC IRQ info*/
LCDC_IRQInfo LcdcIrqInfo = {
    .IrqNum = LCDC_IRQ,
    .IrqPriority = INT_PRI_MIDDLE,
    .IrqData = (u32)LCDC,
};

LCDC_TypeDef *pLCDC = LCDC;
LCDC_InitTypeDef LCDC_InitStruct;

u8 *LcdImgBuffer1 = (u8 *)(DDR_BASE + LCDC_IMG_BUF_OFFSET2 + LCDC_IMG_BUF_SIZE);
u32 UnderFlowCnt = 0;	//----> Debug

static void LcdcInitValues(void)
{
	LCDC_StructInit(&LCDC_InitStruct);
	LCDC_InitStruct.LCDC_ImageWidth = XRES;
	LCDC_InitStruct.LCDC_ImageHeight = YRES;
	for (u8 idx = 0; idx < LCDC_LAYER_MAX_NUM; idx++) {
		LCDC_InitStruct.layerx[idx].LCDC_LayerImgFormat = LCDC_LAYER_IMG_FORMAT_RGB565;
		LCDC_InitStruct.layerx[idx].LCDC_LayerImgBaseAddr = (u32)LcdImgBuffer1;
		LCDC_InitStruct.layerx[idx].LCDC_LayerHorizontalStart = 1;
		LCDC_InitStruct.layerx[idx].LCDC_LayerHorizontalStop = XRES;
		LCDC_InitStruct.layerx[idx].LCDC_LayerVerticalStart = 1;
		LCDC_InitStruct.layerx[idx].LCDC_LayerVerticalStop = YRES;
		LCDC_InitStruct.layerx[idx].LCDC_LayerColorKeyingEn = ENABLE;
	}
}

static u32 LCDCRgbHvIsr(void *Data)
{
	LCDC_TypeDef *LCDCx = (LCDC_TypeDef *)Data;
	volatile u32 IntId = LCDC_GetINTStatus(LCDCx);

	if (IntId & LCDC_BIT_LCD_LIN_INTS) {
		LCDC_ClearINT(LCDCx, LCDC_BIT_LCD_LIN_INTS);
	}
	if (IntId & LCDC_BIT_DMA_UN_INTS) {
		LCDC_ClearINT(LCDCx, LCDC_BIT_DMA_UN_INTS);
		UnderFlowCnt++;
		if (UnderFlowCnt == 1) {
			lldbg("LCDC DMA Under-----\n");
		}
	}
	return 0;
}

static int rtl8730_st7701_lcd_init(void) {

	//MIPI_DSI_INT_Config(MIPI, DISABLE, DISABLE, FALSE);
	LcdcInitValues();

	rtl8730_st7701_lcd_layer_enable(0, true);
	rtl8730_st7701_lcd_layer_enable(1, false);
	rtl8730_st7701_lcd_layer_enable(2, false);

	LCDC_Init(LCDC, &LCDC_InitStruct);
	LCDC_DMAModeConfig(LCDC, LCDC_LAYER_BURSTSIZE_4X64BYTES);
	LCDC_DMADebugConfig(LCDC, LCDC_DMA_OUT_DISABLE, 0);

	LCDC_LineINTPosConfig(LCDC, YRES * 4 / 5);
	LCDC_INTConfig(LCDC, LCDC_BIT_LCD_LIN_INTEN | LCDC_BIT_DMA_UN_INTEN, ENABLE);	
}


static int rtl8730_st7701_gpio_reset(void) {
}

static int rtl8730_st7701_cache_invalidate(u32 *buffer, int size) {
	DCache_CleanInvalidate((u32)buffer, size); 
}

static int rtl8730_st7701_lcd_reload(void) {
	LCDC_TrigerSHWReload(pLCDC);
}


static int rtl8730_st7701_lcd_enable(void) {
	LcdcEnable();
}

static int rtl8730_st7701_lcd_layer_enable(int layer, bool enable) {
	if(enable){
		LCDC_InitStruct.layerx[layer].LCDC_LayerEn = ENABLE;
	}
	else{
		LCDC_InitStruct.layerx[layer].LCDC_LayerEn = DISABLE;
	}
}

static void lcd_put_area(u8 *LcdImgBuffer, u32 x1, u32 y1, u32 x2, u32 y2){

	//lldbg("data :%x %x\n", LcdImgBuffer[0], LcdImgBuffer[1]);
	LCDC_InitStruct.layerx[0].LCDC_LayerImgBaseAddr = (u32)LcdImgBuffer;
	/*LCDC_InitStruct.layerx[0].LCDC_LayerHorizontalStart = x1;
	LCDC_InitStruct.layerx[0].LCDC_LayerHorizontalStop = x2;
	LCDC_InitStruct.layerx[0].LCDC_LayerVerticalStart = y1;
	LCDC_InitStruct.layerx[0].LCDC_LayerVerticalStop = y2;*/

	LCDC_LayerConfig(LCDC, 0, &LCDC_InitStruct.layerx[0]);
	LCDC_TrigerSHWReload(LCDC);
}

static void LcdcEnable(void)
{
	LCDC_TypeDef *pLCDC = LCDC;

	/*enable the LCDC*/
	LCDC_Cmd(pLCDC, ENABLE);
	while (!LCDC_CheckLCDCReady(pLCDC));

	MIPI_DSI_Mode_Switch(MIPI, ENABLE);
}

void rtl8730_st7701_initialize(void)
{
	struct lcd_data config;
	config.XPixels = XRES;
	config.YPixels = YRES;
	
	FAR struct mipi_dsi_host *dsi_host = mipi_dsi_host_initialize(&config);
	FAR struct mipi_dsi_device *dsi_device = st7710_dsi_device_initialize(dsi_host);
	//FAR struct mipi_dsi_device *dsi_device = mipi_dsi_device_register(dsi_host, "dsi", 0);
	struct lcd_dev_s *dev = st7701_lcdinitialize(dsi_device, &g_rtl8730_st7701_config_s);
	rtl8730_st7701_lcd_init();

	if (lcddev_register(dev) < 0) {
		lldbg("ERROR: LCD driver register fail\n");
	} else {
		lldbg("LCD driver register success\n");
	}
	LcdcEnable();
	return;
}