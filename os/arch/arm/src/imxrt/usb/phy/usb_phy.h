/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __USB_PHY_H__
#define __USB_PHY_H__

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef struct _usb_phy_config_struct {


	uint8_t D_CAL;			/* Decode to trim the nominal 17.78mA current source */


	uint8_t TXCAL45DP;		/* Decode to trim the nominal 45-Ohm series termination resistance to the USB_DP output pin */


	uint8_t TXCAL45DM;		/* Decode to trim the nominal 45-Ohm series termination resistance to the USB_DM output pin */
} usb_phy_config_struct_t;

#if defined(__cplusplus)
extern "C" {


#endif	/* 
 */

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @brief EHCI PHY initialization.
 *
 * This function initializes the EHCI PHY IP.
 *
 * @param[in] freq            The external input clock.
 *
 * @retval kStatus_USB_Success      Cancel successfully.
 * @retval kStatus_USB_Error        The freq value is incorrect.
 */
extern uint32_t USB_EhciPhyInit(uint32_t freq, usb_phy_config_struct_t *phyConfig);



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
extern uint32_t USB_EhciLowPowerPhyInit(uint32_t freq, usb_phy_config_struct_t *phyConfig);



/*!
 * @brief EHCI PHY deinitialization.
 *
 * This function deinitializes the EHCI PHY IP.
 *
 */
extern void USB_EhciPhyDeinit();



/*!
 * @brief EHCI PHY disconnect detection enable or disable.
 *
 * This function enable/disable the host EHCI disconnect detection.
 *
 * @param[in] enable
 *            1U - enable;
 *            0U - disable;
 */
extern void USB_EhcihostPhyDisconnectDetectCmd(uint8_t enable);



#if defined(__cplusplus)
}
#endif	/* 
 */

#endif	/* __USB_PHY_H__ */
