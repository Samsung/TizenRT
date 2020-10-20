/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _USB_HOST_HCI_H_
#define _USB_HOST_HCI_H_

#include "usb_host_config.h"
#include "usb_host.h"

#ifdef USB_TIZEN_RT

#include <errno.h>
#include <tinyara/usb/usb.h>
#include <tinyara/usb/usbdev.h>
#include <tinyara/usb/usbhost.h>
#include <tinyara/usb/usbhost.h>

extern struct usbhost_roothubport_s g_USBHOST_ROOT_HUB_PORT;
extern struct usbhost_driver_s g_USBHOST_DRIVER;
extern volatile struct usbhost_hubport_s *g_USBHOST_HUBPORT;

#endif	/* 
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief USB host lock */
#define USB_HostLock()		USB_ENTER_CRITICAL()
/*! @brief USB host unlock */
#define USB_HostUnlock()	USB_EXIT_CRITICAL()

/*!
 * @addtogroup usb_host_controller_driver
 * @{
 */

/*! @brief USB host controller control code */
typedef enum _usb_host_controller_control {
	kUSB_HostCancelTransfer = 1U, /*!< Cancel transfer code */
	kUSB_HostBusControl, /*!< Bus control code */
	kUSB_HostGetFrameNumber, /*!< Get frame number code */
	kUSB_HostUpdateControlEndpointAddress, /*!< Update control endpoint address */
	kUSB_HostUpdateControlPacketSize, /*!< Update control endpoint maximum  packet size */
	kUSB_HostPortAttachDisable, /*!< Disable the port attach event */
	kUSB_HostPortAttachEnable, /*!< Enable the port attach event */
	kUSB_HostL1Config, /*!< L1 suspend Bus control code */
	kUSB_HostSetChargerType, /*!< set charger type */
} usb_host_controller_control_t;

/*! @brief USB host controller bus control code */
typedef enum _usb_host_bus_control {
	kUSB_HostBusReset = 1U, /*!< Reset bus */
	kUSB_HostBusRestart, /*!< Restart bus */
	kUSB_HostBusEnableAttach, /*!< Enable attach */
	kUSB_HostBusDisableAttach, /*!< Disable attach */
	kUSB_HostBusSuspend, /*!< Suspend BUS */
	kUSB_HostBusResume, /*!< Resume BUS */
	kUSB_HostBusL1SuspendInit, /*!< L1 Suspend BUS */
	kUSB_HostBusL1Sleep, /*!< L1 Suspend BUS */
	kUSB_HostBusL1Resume, /*!< L1 Resume BUS */
} usb_host_bus_control_t;


void USB_HostIsrEnable(void);
void USB_HostSynchronousCallback(void *param, usb_host_transfer_t *transfer, usb_status_t status);

/*! @}*/

#endif	/* _USB_HOST_HCI_H_ */
