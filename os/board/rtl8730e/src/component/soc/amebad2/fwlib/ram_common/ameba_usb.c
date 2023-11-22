/**
  ******************************************************************************
  * @file    ameba_usb.c
  * @date    2021-03-23
  * @brief   This file provides the USB HAL functionalities
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "ameba_soc.h"

static const char *TAG = "USB";
/* Private defines -----------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

USB_DATA_SECTION
static const usb_cal_data_t usb_cut_a_cal_data[] = {
	{0x00, 0xE0, 0x9D},
	{0x00, 0xE1, 0x19},
	{0x00, 0xE2, 0xDB},
	{0x00, 0xE4, 0x6D},
	{0x01, 0xE5, 0x0A},
	{0x01, 0xE6, 0xD8},
	{0x02, 0xE7, 0x32},
	{0x01, 0xE0, 0x04},
	{0x01, 0xE0, 0x00},
	{0x01, 0xE0, 0x04},

	{0xFF, 0x00, 0x00}
};

USB_DATA_SECTION
static const usb_cal_data_t usb_cut_b_cal_data[] = {
	{0x00, 0xE0, 0x9D},
	{0x00, 0xE1, 0x19},
	{0x00, 0xE2, 0xDB},
	{0x00, 0xE4, 0x6B},
	{0x01, 0xE5, 0x0A},
	{0x01, 0xE6, 0xD8},
	{0x02, 0xE7, 0x32},
	{0x01, 0xE0, 0x04},
	{0x01, 0xE0, 0x00},
	{0x01, 0xE0, 0x04},

	{0xFF, 0x00, 0x00}
};

/* Private functions ---------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Get USB chip specific calibration data
  * @param  mode: 0 - device; 1 - host
  * @retval Pointer to calibration data buffer
  */
USB_TEXT_SECTION
usb_cal_data_t *usb_chip_get_cal_data(u8 mode)
{
	usb_cal_data_t *data;

	UNUSED(mode);

	if (SYSCFG_RLVersion() != SYSCFG_CUT_VERSION_A) {
		data = (usb_cal_data_t *)&usb_cut_b_cal_data[0];
	} else {
		data = (usb_cal_data_t *)&usb_cut_a_cal_data[0];
	}

	return data;
}

/**
  * @brief  USB chip specific initialization
  * @param  void
  * @retval Status
  */
USB_TEXT_SECTION
u8 usb_chip_init(void)
{
	u32 reg = 0;
	u32 count = 0;

	/* Enable BG */
	reg = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_AIP_CTRL1);
	reg |= (LSYS_BIT_BG_PWR | LSYS_BIT_BG_ON_USB2);
	reg &= ~LSYS_MASK_BG_ALL;
	if (SYSCFG_RLVersion() != SYSCFG_CUT_VERSION_A) {
		reg |= LSYS_BG_ALL(0x2);
	}
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_AIP_CTRL1, reg);

	RCC_PeriphClockCmd(APBPeriph_USB, APBPeriph_USB_CLOCK, ENABLE);

	/* USB Power Sequence */
	/* USB digital pad en,dp/dm sharing GPIO PAD */
	reg = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_USB_CTRL);
	reg &= ~(HSYS_BIT_USB2_DIGOTGPADEN | HSYS_BIT_USB_OTGMODE | HSYS_BIT_USB2_DIGPADEN);
#if CONFIG_USB_OTG
	reg |= (HSYS_BIT_USB_OTGMODE | HSYS_BIT_OTG_ANA_EN);
#endif
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_USB_CTRL, reg);

	/* USB PWC_UALV_EN,  PWC_UAHV_EN */
	reg = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_USB_CTRL);
	reg |= (HSYS_BIT_PWC_UALV_EN | HSYS_BIT_PWC_UAHV_EN);
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_USB_CTRL, reg);
	DelayUs(2);

	/* USB PWC_UABG_EN */
	reg = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_USB_CTRL);
	reg |= HSYS_BIT_PWC_UABG_EN;
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_USB_CTRL, reg);
	DelayUs(10);

	/* USB ISO_USBD_EN = 0 => disable isolation output signal from PD_USBD*/
	reg = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_USB_CTRL);
	reg &= ~HSYS_BIT_ISO_USBA_EN;
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_USB_CTRL, reg);
	DelayUs(10);

	/* USB enable PHY */
	reg = HAL_READ32(USB_OTG_REG_BASE, USB_ADDON_REG_CTRL);
	reg &= ~USB_ADDON_REG_CTRL_BIT_HS_IP_GAP_OPT_MASK;
	reg |= (0x3U << USB_ADDON_REG_CTRL_BIT_HS_IP_GAP_OPT_POS); // Inter-packet gap 343ns, spec 399ns
	reg |= (USB_ADDON_REG_CTRL_BIT_USB_DPHY_FEN | USB_ADDON_REG_CTRL_BIT_USB_APHY_EN | USB_ADDON_REG_CTRL_BIT_LS_HST_UTMI_EN);
	HAL_WRITE32(USB_OTG_REG_BASE, USB_ADDON_REG_CTRL, reg);
	DelayUs(34);

	/* Wait UPLL_CKRDY */
	do {
		/* 1ms timeout expected, 10ms for safe */
		DelayUs(10);
		if (++count > 1000U) {
			RTK_LOGE(TAG, "USB_Init timeout\n");
			return HAL_TIMEOUT;
		}
	} while (!(HAL_READ32(USB_OTG_REG_BASE, USB_ADDON_REG_CTRL) & USB_ADDON_REG_CTRL_BIT_UPLL_CKRDY));

	/* USBOTG_EN = 1 => enable USBOTG */
	reg = HAL_READ32(USB_OTG_REG_BASE, USB_ADDON_REG_CTRL);
	reg |= USB_ADDON_REG_CTRL_BIT_USB_OTG_RST;
	HAL_WRITE32(USB_OTG_REG_BASE, USB_ADDON_REG_CTRL, reg);

	return HAL_OK;
}

/**
  * @brief  USB chip specific deinitialization
  * @param  void
  * @retval Status
  */
USB_TEXT_SECTION
u8 usb_chip_deinit(void)
{
	u32 reg = 0;

	/* USBOTG_EN = 0 => disable USBOTG */
	reg = HAL_READ32(USB_OTG_REG_BASE, USB_ADDON_REG_CTRL);
	reg &= (~USB_ADDON_REG_CTRL_BIT_USB_OTG_RST);
	HAL_WRITE32(USB_OTG_REG_BASE, USB_ADDON_REG_CTRL, reg);

	/* USBPHY_EN = 0 */
	reg = HAL_READ32(USB_OTG_REG_BASE, USB_ADDON_REG_CTRL);
	reg &= (~(USB_ADDON_REG_CTRL_BIT_USB_DPHY_FEN | USB_ADDON_REG_CTRL_BIT_USB_APHY_EN));
	HAL_WRITE32(USB_OTG_REG_BASE, USB_ADDON_REG_CTRL, reg);

	/* USB ISO_USBD_EN = 1 => enable isolation output signal from PD_USBD*/
	reg = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_USB_CTRL);
	reg |= HSYS_BIT_ISO_USBA_EN;
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_USB_CTRL, reg);

	/* USB PWC_UABG_EN = 0 */
	reg = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_USB_CTRL);
	reg &= ~HSYS_BIT_PWC_UABG_EN;
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_USB_CTRL, reg);

	/* PWC_UPHV_EN  = 0 => disable USBPHY analog 3.3V power */
	/* PWC_UPLV_EN = 0 => disable USBPHY analog 1.2V power */
	reg = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_USB_CTRL);
	reg &= ~(HSYS_BIT_PWC_UALV_EN | HSYS_BIT_PWC_UAHV_EN);
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_USB_CTRL, reg);

	/* USB digital pad disable*/
	reg = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_USB_CTRL);
	reg |= (HSYS_BIT_USB2_DIGOTGPADEN | HSYS_BIT_USB2_DIGPADEN);
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_USB_CTRL, reg);

	RCC_PeriphClockCmd(APBPeriph_USB, APBPeriph_USB_CLOCK, DISABLE);

	/* Disable BG */
	reg = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_AIP_CTRL1);
	reg &= ~LSYS_BIT_BG_ON_USB2;
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_AIP_CTRL1, reg);

	return HAL_OK;
}

