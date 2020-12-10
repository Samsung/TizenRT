/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _USB_HOST_CONFIG_H_
#define _USB_HOST_CONFIG_H_

#include "usb_porting.h"

#define USB_HOST_INTERRUPT_THRESHOLD	(0)

#ifdef USB_TIZEN_RT
#define CONFIG_USB_HOST_IRQ_NUMBER	(IMXRT_IRQ_USBOTG1)
#define CONFIG_USB_HOST_IRQ_PRIORITY	(6U)
#else	/* 
 */
#define CONFIG_USB_HOST_IRQ_NUMBER	(USB_OTG1_IRQn)
#define CONFIG_USB_HOST_IRQ_PRIORITY	(6U)
#endif	/* 
 */

/*!
 * @brief host ehci instance count, meantime it indicates ehci enable or disable.
 *        - if 0, host ehci driver is disable.
 *        - if greater than 0, host ehci driver is enable.
 */
#define USB_HOST_CONFIG_EHCI (1U)

/* Common configuration macros for all controllers */

/*!
 * @brief host driver instance max count.
 * for example: 2 - one for khci, one for ehci.
 */
#define USB_HOST_CONFIG_MAX_HOST (USB_HOST_CONFIG_EHCI)

/*!
 * @brief host pipe max count.
 * pipe is the host driver resource for device endpoint, one endpoint need one pipe.
 */
#define USB_HOST_CONFIG_MAX_PIPES (16U)

/*!
 * @brief host transfer max count.
 * transfer is the host driver resource for data transmission mission, one transmission mission need one transfer.
 */
#define USB_HOST_CONFIG_MAX_TRANSFERS (16U)

/*!
 * @brief the max endpoint for one interface.
 * the max endpoint descriptor number that one interface descriptor contain.
 */
#define USB_HOST_CONFIG_INTERFACE_MAX_EP (4U)

/*!
 * @brief the max interface for one configuration.
 * the max interface descriptor number that one configuration descriptor can contain.
 */
#define USB_HOST_CONFIG_CONFIGURATION_MAX_INTERFACE (5U)

/*!
 * @brief the max power for one device.
 * the max power the host can provide for one device.
 */
#define USB_HOST_CONFIG_MAX_POWER (250U)

/*!
 * @brief the max retries for enumeration.
 * retry time when enumeration fail.
 */
#define USB_HOST_CONFIG_ENUMERATION_MAX_RETRIES (3U)

/*!
 * @brief the max retries for enumeration setup stall.
 * the max times for one transfer can stall.
 */
#define USB_HOST_CONFIG_ENUMERATION_MAX_STALL_RETRIES (1U)

/*!
 * @brief the max NAK count for one transaction.
 * when nak count reach to the value, the transaction fail.
 */
#define USB_HOST_CONFIG_MAX_NAK (3000U)

/*! @brief if 1, class driver clear stall automatically; if 0, class driver don't clear stall. */
#define USB_HOST_CONFIG_CLASS_AUTO_CLEAR_STALL (0U)

/*! @brief if 1, enable the low power; if 0, disable the low power. */
#define USB_HOST_CONFIG_LOW_POWER_MODE (1U)

/* EHCI configuration */

/*!
 * @brief ehci periodic frame list size.
 * the value can be 1024, 512, 256, 128, 64, 32, 16 or 8.
 */
#define USB_HOST_CONFIG_EHCI_FRAME_LIST_SIZE (1024U)

/*!
 * @brief ehci QH max count.
 */
#define USB_HOST_CONFIG_EHCI_MAX_QH (16U)

/*!
 * @brief ehci QTD max count.
 */
#define USB_HOST_CONFIG_EHCI_MAX_QTD (16U)

/*!
 * @brief ehci ITD max count.
 */
#define USB_HOST_CONFIG_EHCI_MAX_ITD (16U)

/*!
 * @brief ehci SITD max count.
 */
#define USB_HOST_CONFIG_EHCI_MAX_SITD (16U)


/*!
 * @brief host HID class instance count, meantime it indicates HID class enable or disable.
 *        - if 0, host HID class driver is disable.
 *        - if greater than 0, host HID class driver is enable.
 */
#define USB_HOST_CONFIG_HID (1U)

/*!
 * @brief host MSD class instance count, meantime it indicates MSD class enable or disable.
 *        - if 0, host MSD class driver is disable.
 *        - if greater than 0, host MSD class driver is enable.
 */
#define USB_HOST_CONFIG_MSD (1U)

/*!
 * @brief host CDC class instance count, meantime it indicates CDC class enable or disable.
 *        - if 0, host CDC class driver is disable.
 *        - if greater than 0, host CDC class driver is enable.
 */
#define USB_HOST_CONFIG_CDC (1U)

/*!
 * @brief host AUDIO class instance count, meantime it indicates AUDIO class enable or disable.
 *        - if 0, host AUDIO class driver is disable.
 *        - if greater than 0, host AUDIO class driver is enable.
 */
#define USB_HOST_CONFIG_AUDIO (1U)

/*!
 * @brief host PHDC class instance count, meantime it indicates PHDC class enable or disable.
 *        - if 0, host PHDC class driver is disable.
 *        - if greater than 0, host PHDC class driver is enable.
 */
#define USB_HOST_CONFIG_PHDC (1U)

/*!
 * @brief host printer class instance count, meantime it indicates printer class enable or disable.
 *        - if 0, host printer class driver is disable.
 *        - if greater than 0, host printer class driver is enable.
 */
#define USB_HOST_CONFIG_PRINTER (1U)

#endif	/* _USB_HOST_CONFIG_H_ */
