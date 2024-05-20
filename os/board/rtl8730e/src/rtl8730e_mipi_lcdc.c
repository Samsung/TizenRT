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
#if defined(CONFIG_LCD_ST7785)
#include <tinyara/lcd/st7785.h>
#elif defined(CONFIG_LCD_ST7701)
#include <tinyara/lcd/st7701.h>
#endif
#include <tinyara/mipidsi/mipi_dsi.h>
#include "objects.h"
#include "PinNames.h"
#include "gpio_api.h"
#include "pwmout_api.h"
#define PIN_LOW 0
#define PIN_HIGH 1
#define LCD_LAYER 0

#if defined(CONFIG_LCD_ST7785)
#define GPIO_PIN_BACKLIGHT      PB_11
#define MIPI_GPIO_RESET_PIN 	PA_15
#elif defined(CONFIG_LCD_ST7701)
#define GPIO_PIN_BACKLIGHT
#define MIPI_GPIO_RESET_PIN 	PA_14
#endif

struct rtl8730_lcdc_info_s {
	struct mipi_lcd_config_s lcd_config;
	pwmout_t pwm_led;
};
static void rtl8730_lcd_init(void);
static void rtl8730_gpio_reset(void);
static void rtl8730_lcd_mode_switch(bool flag);
static void rtl8730_lcd_layer_enable(int layer, bool enable);
static void rtl8730_lcd_put_area(u8 *lcd_img_buffer, u32 x_start, u32 y_start, u32 x_end, u32 y_end);
static void rtl8730_enable_lcdc(void);
static void rtl8730_register_lcdc_isr(void);
static void rtl8730_control_backlight(u8 level);
FAR void mipidsi_mode_switch(bool do_enable);
FAR void mipidsi_acpu_reg_clear(void);
FAR struct mipi_dsi_host *amebasmart_mipi_dsi_host_initialize(struct lcd_data *config);
FAR struct mipi_dsi_device *mipi_dsi_device_register(FAR struct mipi_dsi_host *host, FAR const char *name, int channel);
FAR struct lcd_dev_s *mipi_lcdinitialize(FAR struct mipi_dsi_device *dsi, struct mipi_lcd_config_s *config);

struct rtl8730_lcdc_info_s g_rtl8730_config_dev_s = {
	.lcd_config = {
		.init = rtl8730_lcd_init,
		.reset = rtl8730_gpio_reset,
		.lcd_mode_switch = rtl8730_lcd_mode_switch,
		.lcd_enable = rtl8730_enable_lcdc,
		.lcd_layer_enable = rtl8730_lcd_layer_enable,
		.lcd_put_area = rtl8730_lcd_put_area,
		.backlight = rtl8730_control_backlight,
	},
};


struct irq {
	u32 num;
	u32 data;
	u32 priority;
};

LCDC_TypeDef *pLCDC = LCDC;
LCDC_InitTypeDef lcdc_init_struct;
static u32 UnderFlowCnt = 0;

struct irq lcdc_irq_info = {
	.num = LCDC_IRQ,
	.data = (u32) LCDC,
	.priority = INT_PRI_MIDDLE,
};

extern struct irq mipi_irq_info;
u8 *lcd_img_buffer1 = (u8 *)(DDR_BASE + LCDC_IMG_BUF_OFFSET1 + LCDC_IMG_BUF_SIZE);
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
		//lcdc_init_struct.layerx[idx].LCDC_LayerEn = ENABLE;
	}
}

static void rtl8730_lcd_init(void)
{
	rtl8730_lcd_layer_enable(LCD_LAYER, true);
	rtl8730_lcd_layer_enable(1, false);
	rtl8730_lcd_layer_enable(2, false);

	LCDC_Init(pLCDC, &lcdc_init_struct);
	LCDC_DMAModeConfig(pLCDC, LCDC_LAYER_BURSTSIZE_2X64BYTES);
	LCDC_DMADebugConfig(pLCDC, LCDC_DMA_OUT_DISABLE, 0);
	rtl8730_register_lcdc_isr();
	LCDC_LineINTPosConfig(pLCDC, LCD_YRES * 4 / 5);
	LCDC_INTConfig(pLCDC, LCDC_BIT_LCD_LIN_INTEN | LCDC_BIT_DMA_UN_INTEN, ENABLE);
}

static void rtl8730_gpio_reset(void)
{
	GPIO_WriteBit(MIPI_GPIO_RESET_PIN, PIN_HIGH);
        DelayMs(10);
	GPIO_WriteBit(MIPI_GPIO_RESET_PIN, PIN_LOW);
        DelayMs(10);
	GPIO_WriteBit(MIPI_GPIO_RESET_PIN, PIN_HIGH);
        DelayMs(120);
	return;
}

static void rtl8730_lcd_mode_switch(bool flag)
{
	if (flag == false) {
		mipidsi_mode_switch(false);
		MIPI_DSI_INT_Config(MIPI, DISABLE, ENABLE, FALSE);
		DelayMs(20);
	} else {
		MIPI_DSI_INT_Config(MIPI, DISABLE, DISABLE, FALSE);
		mipidsi_mode_switch(true);
	}
}

static void rtl8730_lcd_layer_enable(int layer, bool enable)
{
	if (enable) {
		lcdc_init_struct.layerx[layer].LCDC_LayerEn = ENABLE;
	} else {
		lcdc_init_struct.layerx[layer].LCDC_LayerEn = DISABLE;
	}
}

static void rtl8730_lcd_put_area(u8 *lcd_img_buffer, u32 x_start, u32 y_start, u32 x_end, u32 y_end)
{
	lcdc_init_struct.layerx[LCD_LAYER].LCDC_LayerImgBaseAddr = (u32) lcd_img_buffer;
#if defined(CONFIG_ENABLE_LCD_CHANGE_WINDOW_SIZE)
	lcdc_init_struct.layerx[LCD_LAYER].LCDC_LayerHorizontalStart = y_start;
	lcdc_init_struct.layerx[LCD_LAYER].LCDC_LayerHorizontalStop = y_end;
	lcdc_init_struct.layerx[LCD_LAYER].LCDC_LayerVerticalStart = x_start;
	lcdc_init_struct.layerx[LCD_LAYER].LCDC_LayerVerticalStop = x_end;
	lcdc_init_struct.layerx[LCD_LAYER].LCDC_LayerEn = ENABLE;
#endif
	DCache_CleanInvalidate((u32)lcd_img_buffer, LCDC_IMG_BUF_SIZE);
	LCDC_LayerConfig(pLCDC, LCD_LAYER, &lcdc_init_struct.layerx[LCD_LAYER]);
	LCDC_TrigerSHWReload(pLCDC);
}

static void rtl8730_gpio_init(void)
{
	Pinmux_Swdoff();
	Pinmux_Config(MIPI_GPIO_RESET_PIN, PINMUX_FUNCTION_GPIO);

	GPIO_InitTypeDef ResetPin;
	ResetPin.GPIO_Pin = MIPI_GPIO_RESET_PIN;
	ResetPin.GPIO_PuPd = GPIO_PuPd_NOPULL;
	ResetPin.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(&ResetPin);
#if defined(CONFIG_LCD_ST7785)
	pwmout_init(&g_rtl8730_config_dev_s.pwm_led, GPIO_PIN_BACKLIGHT);
	lcdvdbg("initial pwm read %f\n", pwmout_read(&g_rtl8730_config_dev_s.pwm_led));
#endif
}

static void rtl8730_control_backlight(uint8_t level)
{
	float pwm_level = level/100.0;
	lcdvdbg("level :%d , pwm level:%f\n", level, pwm_level);
#if defined(CONFIG_LCD_ST7785)
	pwmout_write(&g_rtl8730_config_dev_s.pwm_led, 1.0-pwm_level);
#endif
}

static void rtl8730_enable_lcdc(void)
{
	LCDC_Cmd(pLCDC, ENABLE);
	while (!LCDC_CheckLCDCReady(pLCDC)) ;
	mipidsi_mode_switch(true);
}

void rtl8730_mipidsi_underflowreset(void)
{
	u32 reg_val2 = MIPI_DSI_INTS_ACPU_Get((MIPI_TypeDef *) mipi_irq_info.data);

	if (reg_val2) {
		UnderFlowCnt = 0;
		MIPI_DSI_INT_Config((MIPI_TypeDef *) mipi_irq_info.data, DISABLE, DISABLE, DISABLE);
		mipidsi_acpu_reg_clear();

		/*Disable the LCDC*/
		LCDC_Cmd(pLCDC, DISABLE);
		lcddbg("ERROR: LCDC_CTRL 0x%x\n", pLCDC->LCDC_CTRL);

		rtl8730_enable_lcdc();
	}
}

u32 rtl8730_hv_isr(void *Data)
{
	/*get LCDC interrupt status*/
	volatile u32 IntId = LCDC_GetINTStatus(pLCDC);
	/*select operations according to interrupt ID*/
	if (IntId & LCDC_BIT_LCD_LIN_INTS) {
		LCDC_ClearINT(pLCDC, LCDC_BIT_LCD_LIN_INTS);
	}

	if (IntId & LCDC_BIT_LCD_FRD_INTS) {
                LCDC_ClearINT(pLCDC, LCDC_BIT_LCD_FRD_INTS);
        }

	if (IntId & LCDC_BIT_DMA_UN_INTS) {
		LCDC_ClearINT(pLCDC, LCDC_BIT_DMA_UN_INTS);
		UnderFlowCnt++;
		if (UnderFlowCnt == 1) {
			lcddbg("ERROR: LCDC DMA Underflow-----\n");
			InterruptRegister((IRQ_FUN)rtl8730_mipidsi_underflowreset, mipi_irq_info.num, (u32)mipi_irq_info.data, mipi_irq_info.priority);
			InterruptEn(mipi_irq_info.num, mipi_irq_info.priority);
			mipidsi_acpu_reg_clear();
			mipidsi_mode_switch(false);
			MIPI_DSI_INT_Config(MIPI, ENABLE, ENABLE, ENABLE);
		}
	}
	return 0;
}

static void rtl8730_register_lcdc_isr(void){
	InterruptDis(lcdc_irq_info.num);
	InterruptUnRegister(lcdc_irq_info.num);
	InterruptRegister((IRQ_FUN)rtl8730_hv_isr, lcdc_irq_info.num, (u32)lcdc_irq_info.data, lcdc_irq_info.priority);
	InterruptEn(lcdc_irq_info.num, lcdc_irq_info.priority);
}

void rtl8730_lcdc_initialize(void)
{
	rtl8730_gpio_init();
	struct lcd_data config;
	config.XPixels = LCD_XRES;
	config.YPixels = LCD_YRES;
	config.mipi_frame_rate = MIPI_FRAME_RATE;
	config.mipi_dsi_HBP = MIPI_DSI_HBP;
	config.mipi_dsi_HFP = MIPI_DSI_HFP;
	config.mipi_dsi_HSA = MIPI_DSI_HSA;
	config.mipi_dsi_RTNI = MIPI_DSI_RTNI;
	config.mipi_dsi_VBP = MIPI_DSI_VBP;
	config.mipi_dsi_VFP = MIPI_DSI_VFP;
	config.mipi_dsi_VSA = MIPI_DSI_VSA;
	config.mipi_lcd_limit = MIPI_LCD_LIMIT;
	config.lcd_lane_num = MIPI_LANE_NUMBER;
	rtl8730_gpio_reset();
	struct mipi_dsi_host *dsi_host = (struct mipi_dsi_host *)amebasmart_mipi_dsi_host_initialize(&config);
	struct mipi_dsi_device *dsi_device = (struct mipi_dsi_device *)mipi_dsi_device_register(dsi_host, "dsi", 0);
	struct lcd_dev_s *dev = (struct lcd_dev_s *)mipi_lcdinitialize(dsi_device, &g_rtl8730_config_dev_s.lcd_config);
	LcdcInitValues(config);
	rtl8730_lcd_init();
	if (lcddev_register(dev) < 0) {
		lcddbg("ERROR: LCD driver register fail\n");
		return;
	}
	lcdvdbg("LCD driver register success\n");
}
