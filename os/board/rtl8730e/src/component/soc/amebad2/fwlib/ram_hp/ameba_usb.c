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

/* Private defines -----------------------------------------------------------*/

#define USB_ID_WORKAROUND	0
#define USB_ID_GPIO			_PB_17

/* Private types -------------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Load PHY vendor control registers
  * @param  addr: PHY register address
  * @retval HAL status
  */
static u8 USB_LoadPhyVendorControlRegister(u8 addr)
{
	u8 ret = HAL_OK;
	u32 count = 0U;
	u32 reg = 0x0A300000U;

	reg |= (((u32)(USB_OTG_PHY_LOW_ADDR(addr))) << USB_OTG_GPVNDCTL_VCTRL_Pos);
	USB_GLOBAL->GPVNDCTL = reg;

	do {
		/* 1us timeout expected, 1ms for safe */
		DelayUs(1);
		if (++count > 1000U) {
			DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "USB_LoadPhyVendorControlRegister timeout\n");
			ret = HAL_TIMEOUT;
			break;
		}
	} while ((USB_GLOBAL->GPVNDCTL & USB_OTG_GPVNDCTL_VSTSDONE) == 0U);

	return ret;
}

/**
  * @brief  Write USB PHY register
  * @param  addr: USB PHY register address
  * @param  data: USB PHY register value
  * @retval HAL status
  */
static u8 USB_WritePhyRegister(u8 addr, u8 value)
{
	u8 ret;
	u32 reg;

	reg = HAL_READ32(USB_OTG_REG_BASE, USB_OTG_ADDON_REG_VND_STS_OUT);
	reg &= (~USB_OTG_PHY_DATA_MASK);
	reg |= value;
	HAL_WRITE32(USB_OTG_REG_BASE, USB_OTG_ADDON_REG_VND_STS_OUT, reg);

	/* Load low addr */
	ret = USB_LoadPhyVendorControlRegister(USB_OTG_PHY_LOW_ADDR(addr));
	if (ret == HAL_OK) {
		/* Load high addr */
		ret = USB_LoadPhyVendorControlRegister(USB_OTG_PHY_HIGH_ADDR(addr));
	}

	return ret;
}

/**
  * @brief  Read USB PHY register
  * @param  addr: USB PHY register address
  * @retval HAL status
  */
static u8 USB_ReadPhyRegister(u8 addr, u8 *value)
{
	u8 ret;
	u32 reg;
	u8 addr_read;

	if (addr >= 0xE0) {
		addr_read = addr - 0x20;
	} else {
		addr_read = addr;
	}

	/* Load low addr */
	ret = USB_LoadPhyVendorControlRegister(USB_OTG_PHY_LOW_ADDR(addr_read));
	if (ret == HAL_OK) {
		/* Load high addr */
		ret = USB_LoadPhyVendorControlRegister(USB_OTG_PHY_HIGH_ADDR(addr_read));
		if (ret == HAL_OK) {
			reg = USB_GLOBAL->GPVNDCTL;
			*value = ((reg & USB_OTG_GPVNDCTL_REGDATA_Msk) >> USB_OTG_GPVNDCTL_REGDATA_Pos) & 0xFF;
		}
	}

	return ret;
}

/**
  * @brief  Select USB PHY page
  * @param  None
  * @retval HAL status
  */
static u8 USB_PhySelectPage(u8 page)
{
	u8 ret;
	u8 reg;

	ret = USB_ReadPhyRegister(USB_OTG_PHY_REG_F4, &reg);
	if (ret != HAL_OK) {
		DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to read USB_OTG_PHY_REG_F4: %d\n", ret);
		return ret;
	}

	reg &= (~USB_OTG_PHY_REG_F4_BIT_PAGE_SEL_MASK);
	reg |= (page << USB_OTG_PHY_REG_F4_BIT_PAGE_SEL_POS) & USB_OTG_PHY_REG_F4_BIT_PAGE_SEL_MASK;
	ret = USB_WritePhyRegister(USB_OTG_PHY_REG_F4, reg);
	if (ret != HAL_OK) {
		DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_F4: %d\n", ret);
	}

	return ret;
}

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  USB PHY calibration
  *			Shall be called after soft disconnect
  * @param  None
  * @retval HAL status
  */
u8 USB_Calibrate(void)
{
	u8 ret = HAL_OK;
#if USB_ID_WORKAROUND
	GPIO_InitTypeDef GPIO_InitStruct;
#endif
#ifdef CONFIG_USB_OTG
	u8 reg;

	/* Enable OTG */
	ret = USB_PhySelectPage(USB_OTG_PHY_REG_F4_BIT_PAGE1);
	if (ret != HAL_OK) {
		DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to select page 1: %d\n", ret);
		return ret;
	}

	ret = USB_ReadPhyRegister(USB_OTG_PHY_REG_E1, &reg);
	if (ret != HAL_OK) {
		DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to read USB_OTG_PHY_REG_P1_E1: %d\n", ret);
		return ret;
	}

	reg |= USB_OTG_PHY_REG_E1_PAGE1_BIT_EN_OTG;

	ret = USB_WritePhyRegister(USB_OTG_PHY_REG_E1, reg);
	if (ret != HAL_OK) {
		DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_P1_E1: %d\n", ret);
		return ret;
	}
#endif

#if USB_ID_WORKAROUND
	// CutA: Use PB20 instead of ID_USB to workaround VBUS abnormal output issue
	if ((SYSCFG_RLVersion() == SYSCFG_CUT_VERSION_A) && ((USB_GLOBAL->GINTSTS & USB_OTG_GINTSTS_CMOD) == 1U)) {
		if ((USB_GLOBAL->GOTGCTL & USB_OTG_GOTGCTL_CIDSTS) == 0U) {
			GPIO_InitStruct.GPIO_Pin = USB_ID_GPIO;
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_Init(&GPIO_InitStruct);
			GPIO_WriteBit(USB_ID_GPIO, GPIO_PIN_LOW);
		}
	}
#endif

	/* 3ms + 2.5us from DD, 3ms already delayed after soft disconnect */
	DelayUs(3);

	if (SYSCFG_RLVersion() != SYSCFG_CUT_VERSION_A) {
		/* Switch to page 1 */
		ret = USB_PhySelectPage(USB_OTG_PHY_REG_F4_BIT_PAGE1);
		if (ret != HAL_OK) {
			DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to select page 1: %d\n", ret);
			return ret;
		}

		/* E5 / Page 1 */
		ret = USB_WritePhyRegister(USB_OTG_PHY_REG_E5, 0x0A);
		if (ret != HAL_OK) {
			DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_P1_E5: %d\n", ret);
			return ret;
		}

		/* Switch to page 0 */
		ret = USB_PhySelectPage(USB_OTG_PHY_REG_F4_BIT_PAGE0);
		if (ret != HAL_OK) {
			DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to select page 0: %d\n", ret);
			return ret;
		}

		/* E1 / Page 0 */
		ret = USB_WritePhyRegister(USB_OTG_PHY_REG_E1, 0x19);
		if (ret != HAL_OK) {
			DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_P0_E1: %d\n", ret);
			return ret;
		}

		/* F7 / Page 0 */
		ret = USB_WritePhyRegister(USB_OTG_PHY_REG_F7, 0x3A);
		if (ret != HAL_OK) {
			DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_P0_F7: %d\n", ret);
			return ret;
		}

		/* Switch to page 1 */
		ret = USB_PhySelectPage(USB_OTG_PHY_REG_F4_BIT_PAGE1);
		if (ret != HAL_OK) {
			DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to select page 1: %d\n", ret);
			return ret;
		}

		/* E6 / Page 1 */
		ret = USB_WritePhyRegister(USB_OTG_PHY_REG_E6, 0xD8);
		if (ret != HAL_OK) {
			DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_P1_E6: %d\n", ret);
			return ret;
		}

		/* Switch to page 0 */
		ret = USB_PhySelectPage(USB_OTG_PHY_REG_F4_BIT_PAGE0);
		if (ret != HAL_OK) {
			DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to select page 0: %d\n", ret);
			return ret;
		}

		/* E0 / Page 0 */
		ret = USB_WritePhyRegister(USB_OTG_PHY_REG_E0, 0x9D);
		if (ret != HAL_OK) {
			DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_P0_E0: %d\n", ret);
			return ret;
		}

		/* E2 / Page 0 */
		ret = USB_WritePhyRegister(USB_OTG_PHY_REG_E2, 0xDB);
		if (ret != HAL_OK) {
			DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_P0_E2: %d\n", ret);
			return ret;
		}

		/* E4 / Page 0 */
		ret = USB_WritePhyRegister(USB_OTG_PHY_REG_E4, 0x6B);
		if (ret != HAL_OK) {
			DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_P0_E4: %d\n", ret);
			return ret;
		}

		/* Switch to page 2 */
		ret = USB_PhySelectPage(USB_OTG_PHY_REG_F4_BIT_PAGE2);
		if (ret != HAL_OK) {
			DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to select page 2: %d\n", ret);
			return ret;
		}

		/* E7 / Page 2 */
		ret = USB_WritePhyRegister(USB_OTG_PHY_REG_E7, 0x42);
		if (ret != HAL_OK) {
			DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_P2_E7: %d\n", ret);
			return ret;
		}

		/* Switch to page 1 */
		ret = USB_PhySelectPage(USB_OTG_PHY_REG_F4_BIT_PAGE1);
		if (ret != HAL_OK) {
			DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to select page 1: %d\n", ret);
			return ret;
		}

		/* E0 / Page 1 */
		ret = USB_WritePhyRegister(USB_OTG_PHY_REG_E0, 0x04);
		if (ret != HAL_OK) {
			DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_P1_E0: %d\n", ret);
			return ret;
		}

		/* E0 / Page 1 */
		ret = USB_WritePhyRegister(USB_OTG_PHY_REG_E0, 0x00);
		if (ret != HAL_OK) {
			DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_P1_E0: %d\n", ret);
			return ret;
		}

		/* E0 / Page 1 */
		ret = USB_WritePhyRegister(USB_OTG_PHY_REG_E0, 0x04);
		if (ret != HAL_OK) {
			DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_P1_E0: %d\n", ret);
			return ret;
		}
	} else {
		/* Switch to page 1 */
		ret = USB_PhySelectPage(USB_OTG_PHY_REG_F4_BIT_PAGE1);
		if (ret != HAL_OK) {
			DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to select page 1: %d\n", ret);
			return ret;
		}

		/* E5 / Page 1 */
		ret = USB_WritePhyRegister(USB_OTG_PHY_REG_E5, 0x0A);
		if (ret != HAL_OK) {
			DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_P1_E5: %d\n", ret);
			return ret;
		}

		/* Switch to page 0 */
		ret = USB_PhySelectPage(USB_OTG_PHY_REG_F4_BIT_PAGE0);
		if (ret != HAL_OK) {
			DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to select page 0: %d\n", ret);
			return ret;
		}

		/* E1 / Page 0 */
		ret = USB_WritePhyRegister(USB_OTG_PHY_REG_E1, 0x19);
		if (ret != HAL_OK) {
			DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_P0_E1: %d\n", ret);
			return ret;
		}

		/* Switch to page 1 */
		ret = USB_PhySelectPage(USB_OTG_PHY_REG_F4_BIT_PAGE1);
		if (ret != HAL_OK) {
			DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to select page 1: %d\n", ret);
			return ret;
		}

		/* E6 / Page 1 */
		ret = USB_WritePhyRegister(USB_OTG_PHY_REG_E6, 0xD8);
		if (ret != HAL_OK) {
			DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_P1_E6: %d\n", ret);
			return ret;
		}

		/* Switch to page 0 */
		ret = USB_PhySelectPage(USB_OTG_PHY_REG_F4_BIT_PAGE0);
		if (ret != HAL_OK) {
			DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to select page 0: %d\n", ret);
			return ret;
		}

		/* E0 / Page 0 */
		ret = USB_WritePhyRegister(USB_OTG_PHY_REG_E0, 0x9D);
		if (ret != HAL_OK) {
			DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_P0_E0: %d\n", ret);
			return ret;
		}

		/* E2 / Page 0 */
		ret = USB_WritePhyRegister(USB_OTG_PHY_REG_E2, 0xDB);
		if (ret != HAL_OK) {
			DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_P0_E2: %d\n", ret);
			return ret;
		}

		/* E4 / Page 0 */
		ret = USB_WritePhyRegister(USB_OTG_PHY_REG_E4, 0x6D);
		if (ret != HAL_OK) {
			DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_P0_E4: %d\n", ret);
			return ret;
		}

		/* Switch to page 2 */
		ret = USB_PhySelectPage(USB_OTG_PHY_REG_F4_BIT_PAGE2);
		if (ret != HAL_OK) {
			DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to select page 2: %d\n", ret);
			return ret;
		}

		/* E7 / Page 2 */
		ret = USB_WritePhyRegister(USB_OTG_PHY_REG_E7, 0x62);
		if (ret != HAL_OK) {
			DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_P2_E7: %d\n", ret);
			return ret;
		}

		/* Switch to page 1 */
		ret = USB_PhySelectPage(USB_OTG_PHY_REG_F4_BIT_PAGE1);
		if (ret != HAL_OK) {
			DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to select page 1: %d\n", ret);
			return ret;
		}

		/* E0 / Page 1 */
		ret = USB_WritePhyRegister(USB_OTG_PHY_REG_E0, 0x04);
		if (ret != HAL_OK) {
			DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_P1_E0: %d\n", ret);
			return ret;
		}

		/* E0 / Page 1 */
		ret = USB_WritePhyRegister(USB_OTG_PHY_REG_E0, 0x00);
		if (ret != HAL_OK) {
			DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_P1_E0: %d\n", ret);
			return ret;
		}

		/* E0 / Page 1 */
		ret = USB_WritePhyRegister(USB_OTG_PHY_REG_E0, 0x04);
		if (ret != HAL_OK) {
			DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_P1_E0: %d\n", ret);
			return ret;
		}
	}

	return ret;
}

/**
  * @brief  USB HAL initialization
  * @param  void
  * @retval HAL status
  */
u8 USB_Init(void)
{
	u32 reg = 0;
	u32 count = 0;

	/* Enable BG */
	reg = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_AIP_CTRL1);
	reg |= (LSYS_BIT_BG_PWR | LSYS_BIT_BG_ON_USB2);
	if (SYSCFG_RLVersion() != SYSCFG_CUT_VERSION_A) {
		reg &= ~LSYS_MASK_BG_ALL;
		reg |= LSYS_BG_ALL(0x2);
	}
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_AIP_CTRL1, reg);

	RCC_PeriphClockCmd(APBPeriph_USB, APBPeriph_USB_CLOCK, ENABLE);

	/* USB Power Sequence */
	/* USB digital pad en,dp/dm sharing GPIO PAD */
	reg = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_USB_CTRL);
	reg &= ~(HSYS_BIT_USB2_DIGOTGPADEN | HSYS_BIT_USB_OTGMODE | HSYS_BIT_USB2_DIGPADEN);
#ifdef CONFIG_USB_OTG
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
	reg = HAL_READ32(USB_OTG_REG_BASE, USB_OTG_ADDON_REG_CTRL);
	reg &= ~USB_OTG_ADDON_REG_CTRL_BIT_HS_IP_GAP_OPT_MASK;
	reg |= (0x3U << USB_OTG_ADDON_REG_CTRL_BIT_HS_IP_GAP_OPT_POS); // Inter-packet gap 343ns, spec 399ns
	reg |= (USB_OTG_ADDON_REG_CTRL_BIT_USB_DPHY_FEN | USB_OTG_ADDON_REG_CTRL_BIT_USB_APHY_EN | USB_OTG_ADDON_REG_CTRL_BIT_LS_HST_UTMI_EN);
	HAL_WRITE32(USB_OTG_REG_BASE, USB_OTG_ADDON_REG_CTRL, reg);
	DelayUs(34);

	/* Wait UPLL_CKRDY */
	do {
		/* 1ms timeout expected, 10ms for safe */
		DelayUs(10);
		if (++count > 1000U) {
			DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "USB_Init timeout\n");
			return HAL_TIMEOUT;
		}
	} while (!(HAL_READ32(USB_OTG_REG_BASE, USB_OTG_ADDON_REG_CTRL) & USB_OTG_ADDON_REG_CTRL_BIT_UPLL_CKRDY));

	/* USBOTG_EN = 1 => enable USBOTG */
	reg = HAL_READ32(USB_OTG_REG_BASE, USB_OTG_ADDON_REG_CTRL);
	reg |= USB_OTG_ADDON_REG_CTRL_BIT_USB_OTG_RST;
	HAL_WRITE32(USB_OTG_REG_BASE, USB_OTG_ADDON_REG_CTRL, reg);

	return HAL_OK;
}

/**
  * @brief  USB HAL deinitialization
  * @param  void
  * @retval HAL status
  */
u8 USB_DeInit(void)
{
	u32 reg = 0;

	/* USBOTG_EN = 0 => disable USBOTG */
	reg = HAL_READ32(USB_OTG_REG_BASE, USB_OTG_ADDON_REG_CTRL);
	reg &= (~USB_OTG_ADDON_REG_CTRL_BIT_USB_OTG_RST);
	HAL_WRITE32(USB_OTG_REG_BASE, USB_OTG_ADDON_REG_CTRL, reg);

	/* USBPHY_EN = 0 */
	reg = HAL_READ32(USB_OTG_REG_BASE, USB_OTG_ADDON_REG_CTRL);
	reg &= (~(USB_OTG_ADDON_REG_CTRL_BIT_USB_DPHY_FEN | USB_OTG_ADDON_REG_CTRL_BIT_USB_APHY_EN));
	HAL_WRITE32(USB_OTG_REG_BASE, USB_OTG_ADDON_REG_CTRL, reg);

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

	return HAL_OK;
}

/**
  * @brief  USB HAL enable interrupt
  * @param  void
  * @retval void
  */
void USB_EnableInterrupt(void)
{
	InterruptEn(USB_OTG_IRQ, 5);
}

/**
  * @brief  USB HAL disable interrupt
  * @param  void
  * @retval void
  */
void USB_DisableInterrupt(void)
{
	InterruptDis(USB_OTG_IRQ);
}

/**
  * @brief  USB HAL register IRQ handler
  * @param  handler: IRQ handler
  * @retval void
  */
void USB_RegisterIrqHandler(IRQ_FUN handler)
{
	if (handler != NULL) {
		InterruptRegister(handler, USB_OTG_IRQ, NULL, 5);
	}
}

/**
  * @brief  USB HAL unregister IRQ handler
  * @param  void
  * @retval void
  */
void USB_UnregisterIrqHandler(void)
{
	InterruptUnRegister(USB_OTG_IRQ);
}

