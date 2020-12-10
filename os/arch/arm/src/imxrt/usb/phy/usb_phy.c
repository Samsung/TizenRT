/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "usb.h"
#include "usb_host_ehci.h"

#include "usb_phy.h"

/*!
 * @brief ehci phy initialization.
 *
 * This function initialize ehci phy IP.
 *
 * @param[in] freq            the external input clock.
 *                            for example: if the external input clock is 16M, the parameter freq should be 16000000.
 *
 * @retval kStatus_USB_Success      cancel successfully.
 * @retval kStatus_USB_Error        the freq value is incorrect.
 */
uint32_t USB_EhciPhyInit(uint32_t freq, usb_phy_config_struct_t *phyConfig)
{

	gIMXRT1020_USB_HOST.usb_analog->INSTANCE[gIMXRT1020_USB_HOST.usb_analog_index].CHRG_DETECT_SET = USB_ANALOG_CHRG_DETECT_CHK_CHRG_B(1) | USB_ANALOG_CHRG_DETECT_EN_B(1);

	gIMXRT1020_USB_HOST.usb_phy->CTRL |= USBPHY_CTRL_SET_ENUTMILEVEL2_MASK;	/* support LS device. */

	gIMXRT1020_USB_HOST.usb_phy->CTRL |= USBPHY_CTRL_SET_ENUTMILEVEL3_MASK;	/* support external FS Hub with LS device connected. */

	/* PWD register provides overall control of the PHY power state */
	gIMXRT1020_USB_HOST.usb_phy->PWD = 0U;


	if (NULL != phyConfig)

	{

		/* Decode to trim the nominal 17.78mA current source for the High Speed TX drivers on USB_DP and USB_DM. */
		gIMXRT1020_USB_HOST.usb_phy->TX = ((gIMXRT1020_USB_HOST.usb_phy->TX & (~(USBPHY_TX_D_CAL_MASK | USBPHY_TX_TXCAL45DM_MASK | USBPHY_TX_TXCAL45DP_MASK))) |
										   (USBPHY_TX_D_CAL(phyConfig->D_CAL) | USBPHY_TX_TXCAL45DP(phyConfig->TXCAL45DP) |
											USBPHY_TX_TXCAL45DM(phyConfig->TXCAL45DM))
										  );

	}

	return kStatus_USB_Success;
}


/*!
 * @brief ehci phy initialization for suspend and resume.
 *
 * This function initialize ehci phy IP for suspend and resume.
 *
 * @param[in] freq            the external input clock.
 *                            for example: if the external input clock is 16M, the parameter freq should be 16000000.
 *
 * @retval kStatus_USB_Success      cancel successfully.
 * @retval kStatus_USB_Error        the freq value is incorrect.
 */
uint32_t USB_EhciLowPowerPhyInit(uint32_t freq, usb_phy_config_struct_t *phyConfig)
{

	gIMXRT1020_USB_HOST.usb_phy->CTRL |= USBPHY_CTRL_ENAUTO_PWRON_PLL_MASK;

	gIMXRT1020_USB_HOST.usb_phy->CTRL |= USBPHY_CTRL_ENAUTOCLR_CLKGATE_MASK | USBPHY_CTRL_ENAUTOCLR_PHY_PWD_MASK;

	gIMXRT1020_USB_HOST.usb_phy->CTRL |= USBPHY_CTRL_SET_ENUTMILEVEL2_MASK;	/* support LS device. */

	gIMXRT1020_USB_HOST.usb_phy->CTRL |= USBPHY_CTRL_SET_ENUTMILEVEL3_MASK;	/* support external FS Hub with LS device connected. */

	/* PWD register provides overall control of the PHY power state */
	gIMXRT1020_USB_HOST.usb_phy->PWD = 0U;


	if (NULL != phyConfig)

	{

		/* Decode to trim the nominal 17.78mA current source for the High Speed TX drivers on USB_DP and USB_DM. */
		gIMXRT1020_USB_HOST.usb_phy->TX =
			((gIMXRT1020_USB_HOST.usb_phy->TX & (~(USBPHY_TX_D_CAL_MASK | USBPHY_TX_TXCAL45DM_MASK | USBPHY_TX_TXCAL45DP_MASK))) |
			 (USBPHY_TX_D_CAL(phyConfig->D_CAL) | USBPHY_TX_TXCAL45DP(phyConfig->TXCAL45DP) |
			  USBPHY_TX_TXCAL45DM(phyConfig->TXCAL45DM)));

	}

	return kStatus_USB_Success;
}


/*!
 * @brief ehci phy de-initialization.
 *
 * This function de-initialize ehci phy IP.
 *
 */
void USB_EhciPhyDeinit()
{

	gIMXRT1020_USB_HOST.usb_phy->CTRL |= USBPHY_CTRL_CLKGATE_MASK;	/* set to 1U to gate clocks */
}


/*!
 * @brief ehci phy disconnect detection enable or disable.
 *
 * This function enable/disable host ehci disconnect detection.
 *
 * @param[in] enable
 *            1U - enable;
 *            0U - disable;
 */
void USB_EhcihostPhyDisconnectDetectCmd(uint8_t enable)
{

	if (enable)

	{

		gIMXRT1020_USB_HOST.usb_phy->CTRL |= USBPHY_CTRL_ENHOSTDISCONDETECT_MASK;

	}

	else

	{

		gIMXRT1020_USB_HOST.usb_phy->CTRL &= (~USBPHY_CTRL_ENHOSTDISCONDETECT_MASK);

	}
}


