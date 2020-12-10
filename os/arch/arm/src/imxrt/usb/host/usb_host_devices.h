/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _USB_HOST_DEV_MNG_H_
#define _USB_HOST_DEV_MNG_H_

#include "usb_host.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @addtogroup usb_host_drv
 * @{
 */
/*! @brief States of device instances enumeration */
typedef enum _usb_host_device_enumeration_status {
	kStatus_DEV_Notinit = 0, /*!< Device is invalid */
	kStatus_DEV_Initial, /*!< Device has been processed by host driver */
	kStatus_DEV_GetDes8, /*!< Enumeration process: get 8 bytes' device descriptor */
	kStatus_DEV_SetAddress, /*!< Enumeration process: set device address */
	kStatus_DEV_GetDes, /*!< Enumeration process: get device descriptor */
	kStatus_DEV_GetCfg9, /*!< Enumeration process: get 9 bytes' configuration descriptor */
	kStatus_DEV_GetCfg, /*!< Enumeration process: get configuration descriptor */
	kStatus_DEV_SetCfg, /*!< Enumeration process: set configuration */
	kStatus_DEV_EnumDone, /*!< Enumeration is done */
	kStatus_DEV_AppUsed, /*!< This device has been used by application */
} usb_host_device_enumeration_status_t;

/*! @brief States of device's interface */
typedef enum _usb_host_interface_state {
	kStatus_interface_Attached = 1, /*!< Interface's default status */
	kStatus_interface_Opened, /*!< Interface is used by application */
	kStatus_interface_Detached, /*!< Interface is not used by application */
} usb_host_interface_state_t;

/*! @brief States of device */
typedef enum _usb_host_device_state {
	kStatus_device_Detached = 0, /*!< Device is used by application */
	kStatus_device_Attached, /*!< Device's default status */
} usb_host_device_state_t;

/*! @brief Device instance */
typedef struct _usb_host_device_instance {

	struct _usb_host_device_instance *next;	/*!< Next device, or NULL */

//JS
#if 0
	usb_host_handle hostHandle;	/*!< Host handle */

#else	/* 
 */
	usb_host_handle m_host_device_hostHandle;	/*!< Host handle */

#endif	/* 
 */
	usb_host_configuration_t configuration;	/*!< Parsed configuration information for the device */

	usb_descriptor_device_t *deviceDescriptor;	/*!< Standard device descriptor */

	uint8_t *configurationDesc;	/*!< Configuration descriptor pointer */

	uint16_t configurationLen;	/*!< Configuration descriptor length */

	uint16_t configurationValue;	/*!< Configuration index */

	uint8_t interfaceStatus[USB_HOST_CONFIG_CONFIGURATION_MAX_INTERFACE];	/*!< Interfaces' status, please reference to #usb_host_interface_state_t */

	uint8_t *enumBuffer;		/*!< Buffer for enumeration */

	uint8_t state;				/*!< Device state for enumeration */

	uint8_t enumRetries;		/*!< Re-enumeration when error in control transfer */

	uint8_t stallRetries;		/*!< Re-transfer when stall */

	uint8_t speed;				/*!< Device speed */
} usb_host_device_instance_t;

typedef struct _usb_host_enum_process_entry {

	uint8_t successState;		/*!< When the last step is successful, the next state value */

	uint8_t retryState;		/*!< When the last step need retry, the next state value */

	/*! When the last step transfer is done, the function is used to process the transfer data */
	usb_status_t(*process)(usb_host_device_instance_t *deviceInstance, uint32_t dataLength);
} usb_host_enum_process_entry_t;

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief Calls this function when device attach.
 *
 * @param hostHandle    Host instance handle.
 * @param speed         Device speed.
 * @param hubNumber     Device hub no. root device's hub no. is 0.
 * @param portNumber    Device port no. root device's port no. is 0.
 * @param level         Device level. root device's level is 1.
 * @param deviceHandle  Return device handle.
 *
 * @return kStatus_USB_Success or error codes.
 */
extern usb_status_t USB_HostAttachDevice(uint8_t speed,
		uint8_t hubNumber,
		uint8_t portNumber,
		uint8_t level,
		usb_device_handle *deviceHandle);

/*!
 * @brief Call this function when device detaches.
 *
 * @param hostHandle   Host instance handle.
 * @param hubNumber    Device hub no. root device's hub no. is 0.
 * @param portNumber   Device port no. root device's port no. is 0.
 *
 * @return kStatus_USB_Success or error codes.
 */
extern usb_status_t USB_HostDetachDevice(uint8_t hubNumber, uint8_t portNumber);

/*!
 * @brief Call this function when device detaches.
 *
 * @param hostHandle    Host instance handle.
 * @param deviceHandle  Device handle.
 *
 * @return kStatus_USB_Success or error codes.
 */
extern usb_status_t USB_HostDetachDeviceInternal(usb_device_handle deviceHandle);
extern usb_status_t USB_Host_EP0_Configure(usb_host_pipe_handle pipeHandle, uint8_t funcaddr, uint8_t speed, uint16_t maxpacketsize);
extern usb_status_t USB_Host_ControlTransfer(usb_host_pipe_handle pipeHandle, uint8_t *usb_ctrlreq, uint8_t *buffer);
extern usb_status_t USB_Host_Transfer(usb_host_pipe_handle pipeHandle, uint8_t *buffer, uint32_t buffer_len, ssize_t *transferred);

/*! @}*/
#endif	/* _USB_HOST_DEV_MNG_H_ */
