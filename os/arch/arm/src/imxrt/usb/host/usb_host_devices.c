/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016,2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "usb_host_config.h"
#include "usb_host.h"
#include "usb_host_hci.h"
#include "usb_host_devices.h"
#include "usb_host_ehci.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief notify the application event, the callback is registered when initializing host.
 *
 * @param deviceInstance   device instance pointer.
 * @param eventCode        event code.
 *
 * @return kStatus_USB_Success or error codes
 */
static usb_status_t USB_HostNotifyDevice(usb_host_device_instance_t *deviceInstance, uint32_t eventCode, uint32_t eventParameter);

/*!
 * @brief allocate one address.
 *
 * @param hostInstance    host instance pointer.
 *
 * @return address, 0 is invalid.
 */
static uint8_t USB_HostAllocateDeviceAddress();

/*!
 * @brief release device resource.
 *
 * @param hostInstance   host instance pointer.
 * @param deviceInstance    device instance pointer.
 */
static void USB_HostReleaseDeviceResource(usb_host_device_instance_t *deviceInstance);

/*!
 * @brief remove device instance from host device list.
 *
 * @param deviceHandle    device handle.
 *
 * @return kStatus_USB_Success or error codes.
 */
static usb_status_t USB_HostRemoveDeviceInstance(usb_device_handle deviceHandle);

/*!
 * @brief control the bus.
 *
 * This function control the host bus.
 *
 * @param[in] hostHandle     the host handle.
 * @param[in] controlType    the control code, please reference to bus_event_t.
 *
 * @retval kStatus_USB_Success              control successfully.
 * @retval kStatus_USB_InvalidHandle        The hostHandle is a NULL pointer.
 */
static usb_status_t USB_HostControlBus(uint8_t controlType);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
//static int imxrt_ctrlin(FAR struct usbhost_driver_s *drvr, usbhost_ep_t ep0, FAR const struct usb_ctrlreq_s *req, FAR uint8_t *buffer)
usb_status_t USB_Host_ControlTransfer(usb_host_pipe_handle pipeHandle, uint8_t *usb_ctrlreq, uint8_t *buffer)
{
	usb_status_t status = kStatus_USB_Success;
	usb_host_transfer_t *transfer;
	usb_host_ehci_sync_result_t *result = NULL;

	result = (usb_host_ehci_sync_result_t *) usb_aligned_malloc(sizeof(usb_host_ehci_sync_result_t), USB_CACHE_LINESIZE);
	if (!result) {
		return kStatus_USB_AllocFail;
	}
	memset(result, 0x00, sizeof(usb_host_ehci_sync_result_t));

#ifndef USB_TIZEN_RT
	result->finished = &result->finishedEvent;
#endif

	if (USB_EHCI_EVENT_TRIGGER_CREATE(&(result->finished)) != USB_OK) {
		result->finished = NULL;
		usb_aligned_free(result);
		return kStatus_USB_AllocFail;
	}

	/* malloc transfer */
	if (USB_HostMallocTransfer(&transfer) != kStatus_USB_Success) {
		usb_aligned_free(result);
		return kStatus_USB_Error;
	}

	memcpy(transfer->setupPacket, usb_ctrlreq, sizeof(usb_setup_struct_t));
	transfer->callbackFn = USB_HostSynchronousCallback;
	transfer->callbackParam = result;

	transfer->transferBuffer = buffer;
	transfer->transferLength = transfer->setupPacket->wLength;

	status = USB_HostSendSetup(pipeHandle, transfer);
	if (status == kStatus_USB_Success) {
		USB_EHCI_EVENT_TRIGGER_RECV(result->finished);
		status = result->result;
	}
	USB_HostFreeTransfer(transfer);	// free transfer

	USB_EHCI_EVENT_TRIGGER_DESTROY(&(result->finished));
	usb_aligned_free(result);
	return status;
}

usb_status_t USB_Host_Transfer(usb_host_pipe_handle pipeHandle, uint8_t *buffer, uint32_t buffer_len, ssize_t *transferred)
{
	usb_host_ehci_pipe_t *ehciPipePointer = (usb_host_ehci_pipe_t *) pipeHandle;
	usb_status_t status = kStatus_USB_Success;
	usb_host_transfer_t *transfer;
	usb_host_ehci_sync_result_t *result = NULL;

	if (buffer && transferred) {
		result = (usb_host_ehci_sync_result_t *) usb_aligned_malloc(sizeof(usb_host_ehci_sync_result_t), USB_CACHE_LINESIZE);
		if (!result) {
			return kStatus_USB_AllocFail;
		}

		memset(result, 0x00, sizeof(usb_host_ehci_sync_result_t));

#ifndef USB_TIZEN_RT
		result->finished = &result->finishedEvent;
#endif

		if (USB_EHCI_EVENT_TRIGGER_CREATE(&(result->finished)) != USB_OK) {
			result->finished = NULL;
			usb_aligned_free(result);
			return kStatus_USB_AllocFail;
		}

		/* malloc transfer */
		if (USB_HostMallocTransfer(&transfer) != kStatus_USB_Success) {
			usb_aligned_free(result);
			return kStatus_USB_Error;
		}

		transfer->callbackFn = USB_HostSynchronousCallback;
		transfer->callbackParam = result;

		transfer->transferBuffer = buffer;
		transfer->transferLength = buffer_len;

		if (ehciPipePointer->pipeCommon.direction == USB_OUT) {
			status = USB_HostSend(pipeHandle, transfer);
		} else {
			status = USB_HostRecv(pipeHandle, transfer);
		}

		if (status == kStatus_USB_Success) {
			USB_EHCI_EVENT_TRIGGER_RECV(result->finished);
			status = result->result;
			*transferred = result->dataLength;
		}

		USB_HostFreeTransfer(transfer);	// free transfer

		USB_EHCI_EVENT_TRIGGER_DESTROY(&(result->finished));
		usb_aligned_free(result);
		if (status == kStatus_USB_Success) {
			return kStatus_USB_Success;
		}
	}
	return kStatus_USB_Error;
}

usb_status_t USB_Host_EP0_Configure(usb_host_pipe_handle pipeHandle, uint8_t funcaddr, uint8_t speed, uint16_t maxpacketsize)
{
	usb_host_ehci_pipe_t *ehciPipePointer = (usb_host_ehci_pipe_t *) pipeHandle;

	if (ehciPipePointer) {
		if (ehciPipePointer->pipeCommon.deviceAddress != funcaddr) {
			ehciPipePointer->pipeCommon.deviceAddress = funcaddr;
			USB_HostEhciIoctl(kUSB_HostUpdateControlEndpointAddress, ehciPipePointer);
		}

		if (ehciPipePointer->pipeCommon.maxPacketSize != maxpacketsize) {
			ehciPipePointer->pipeCommon.maxPacketSize = maxpacketsize;
			USB_HostEhciIoctl(kUSB_HostUpdateControlPacketSize, ehciPipePointer);
		}

		return kStatus_USB_Success;
	}
	return kStatus_USB_Error;
}

static usb_status_t USB_HostNotifyDevice(usb_host_device_instance_t *deviceInstance, uint32_t eventCode, uint32_t eventParameter)
{
	usb_status_t status1 = kStatus_USB_Error;
	eventCode = (((uint32_t) eventParameter << 16u) | (uint32_t) eventCode);

	if (gIMXRT1020_USB_HOST.deviceCallback != NULL) {
		if (deviceInstance == NULL) {
			gIMXRT1020_USB_HOST.deviceCallback(NULL, NULL, eventCode);
			return kStatus_USB_InvalidHandle;
		}
		/* call host callback function, function is initialized in USB_HostInit */
		status1 = gIMXRT1020_USB_HOST.deviceCallback(deviceInstance, &deviceInstance->configuration, eventCode);
	}
	return status1;
}

static uint8_t USB_HostAllocateDeviceAddress()
{
	uint8_t address = 0;
	uint8_t addressIndex;
	uint8_t addressBitIndex;
	for (addressIndex = 0; addressIndex < 8; ++addressIndex) {	/* find the idle address position byte */
		if (gIMXRT1020_USB_HOST.addressBitMap[addressIndex] != 0xFF) {
			break;
		}
	}
	if (addressIndex < 8) {
		for (addressBitIndex = 0; addressBitIndex < 8; ++addressBitIndex) {	/* find the idle address position bit */
			if (!(gIMXRT1020_USB_HOST.addressBitMap[addressIndex] & (0x01u << addressBitIndex))) {
				gIMXRT1020_USB_HOST.addressBitMap[addressIndex] |= (0x01u << addressBitIndex);	/* set the allocated bit */
				address = addressIndex * 8 + addressBitIndex + 1;	/* the address minimum is 1 */
				break;
			}
		}
	}
	return address;
}

static usb_status_t USB_HostRemoveDeviceInstance(usb_device_handle deviceHandle)
{
	usb_host_device_instance_t *currentInstance;
	usb_host_device_instance_t *prevInstance;
	if (deviceHandle == NULL) {
		return kStatus_USB_InvalidHandle;
	}

	/* search and remove device instance */
	prevInstance = (usb_host_device_instance_t *) gIMXRT1020_USB_HOST.deviceList;
	if (prevInstance == deviceHandle) {
		gIMXRT1020_USB_HOST.deviceList = prevInstance->next;
		return kStatus_USB_Success;
	} else {
		currentInstance = prevInstance->next;
	}

	while (currentInstance != NULL) {
		if (currentInstance == deviceHandle) {
			prevInstance->next = currentInstance->next;
			return kStatus_USB_Success;
		}
		prevInstance = currentInstance;
		currentInstance = currentInstance->next;
	}

	return kStatus_USB_Success;
}

static void USB_HostReleaseDeviceResource(usb_host_device_instance_t *deviceInstance)
{
#if ((defined(USB_HOST_CONFIG_LOW_POWER_MODE)) && (USB_HOST_CONFIG_LOW_POWER_MODE > 0U))
	if (deviceInstance == gIMXRT1020_USB_HOST.suspendedDevice) {
		gIMXRT1020_USB_HOST.suspendedDevice = NULL;
	}
#endif

	/* close control pipe */
	if (gIMXRT1020_USB_HOST.controlPipe != NULL) {
		USB_HostCancelTransfer(gIMXRT1020_USB_HOST.controlPipe, NULL);
		if (USB_HostClosePipe(gIMXRT1020_USB_HOST.controlPipe) != kStatus_USB_Success) {
			udbg("error when close pipe\r\n");
		}
		gIMXRT1020_USB_HOST.controlPipe = NULL;
	}

	/* free configuration buffer */
	if (deviceInstance->configurationDesc != NULL) {
		usb_aligned_free(deviceInstance->configurationDesc);
	}

	usb_aligned_free(deviceInstance->deviceDescriptor);

	/* free device instance buffer */
	usb_aligned_free(deviceInstance);

	/* enable controller attach */
	USB_HostControlBus(kUSB_HostBusEnableAttach);
}

// create "control EP" and  return usb_host_device_instance_t as deviceHandle.
usb_status_t USB_HostAttachDevice(uint8_t speed, uint8_t hubNumber, uint8_t portNumber, uint8_t level, usb_device_handle *deviceHandle)
{
	USB_SR_ALLOC();
	usb_host_device_instance_t *newInstance;
	uint8_t address;
	usb_host_pipe_init_t pipeInit;
	usb_status_t status;

	/* check whether is the device attached? */
	if (gIMXRT1020_USB_HOST.deviceList != NULL) {
		*deviceHandle = NULL;
		uvdbg("device has attached\r\n");
		return kStatus_USB_Busy;
	}

	/* Allocate new device instance */
	newInstance = (usb_host_device_instance_t *) usb_aligned_malloc(sizeof(usb_host_device_instance_t), USB_CACHE_LINESIZE);
	if (newInstance == NULL) {
		udbg("allocate dev instance fail\r\n");
		(void)USB_HostNotifyDevice(NULL, USB_HOST_EVENT_ENUM_FAIL, kStatus_USB_AllocFail);
		return kStatus_USB_AllocFail;
	}
	memset(newInstance, 0x00, sizeof(usb_host_device_instance_t));

	/* new instance fields init */
	newInstance->speed = speed;
	newInstance->stallRetries = USB_HOST_CONFIG_ENUMERATION_MAX_STALL_RETRIES;
	newInstance->enumRetries = USB_HOST_CONFIG_ENUMERATION_MAX_RETRIES;
	newInstance->deviceDescriptor = (usb_descriptor_device_t *) usb_aligned_malloc(sizeof(usb_descriptor_device_t) + 9, USB_CACHE_LINESIZE);

	if (newInstance->deviceDescriptor == NULL) {
		udbg("allocate newInstance->deviceDescriptor fail\r\n");
		usb_aligned_free(newInstance->deviceDescriptor);
		usb_aligned_free(newInstance);
		(void)USB_HostNotifyDevice(NULL, USB_HOST_EVENT_ENUM_FAIL, kStatus_USB_AllocFail);
		return kStatus_USB_AllocFail;
	}
	memset(newInstance->deviceDescriptor, 0x00, sizeof(usb_descriptor_device_t) + 9);

	newInstance->enumBuffer = (uint8_t *)((uint8_t *) newInstance->deviceDescriptor + sizeof(usb_descriptor_device_t));

	//JS now it will be triggered by USB host stack
	USB_HostLock();

	/* allocate address && insert to the dev list */
	address = USB_HostAllocateDeviceAddress();
	if (address == 0) {
		udbg("allocate address fail\r\n");
		USB_HostUnlock();

		usb_aligned_free(newInstance->deviceDescriptor);
		usb_aligned_free(newInstance);

		(void)USB_HostNotifyDevice(NULL, USB_HOST_EVENT_ENUM_FAIL, kStatus_USB_Error);
		return kStatus_USB_Error;
	}

	newInstance->next = (usb_host_device_instance_t *) gIMXRT1020_USB_HOST.deviceList;
	gIMXRT1020_USB_HOST.deviceList = newInstance;
	newInstance->state = kStatus_DEV_Initial;
	USB_HostUnlock();

	/* open control pipe */
	pipeInit.pipeType = USB_ENDPOINT_CONTROL;
	pipeInit.deviceAddress = 0;	// default address => 0
	pipeInit.direction = 0;
	pipeInit.endpointAddress = portNumber;
	pipeInit.interval = 0;
	pipeInit.maxPacketSize = 8;
	pipeInit.numberPerUframe = 0;
	pipeInit.speed = speed;

	pipeInit.hubNumber = hubNumber;	/*!< Device's first connected hub address (root hub = 0) */
	pipeInit.portNumber = portNumber;	/*!< Device's first connected hub's port no (1 - 8) */
	pipeInit.hsHubNumber = pipeInit.hubNumber;	/*!< Device's first connected high-speed hub's address (1 - 8) => temporary : same as hubNumber */
	pipeInit.hsHubPort = pipeInit.portNumber;	/*!< Device's first connected high-speed hub's port no (1 - 8) => temporary : same as portNumber */
	pipeInit.level = level;		/*!< Device's level (root device = 0) */

	pipeInit.nakCount = USB_HOST_CONFIG_MAX_NAK;

	if (USB_HostOpenPipe(&(gIMXRT1020_USB_HOST.controlPipe), &pipeInit) != kStatus_USB_Success) {
		/* don't need release resource, resource is released when detach */
		*deviceHandle = newInstance;
		usb_aligned_free(newInstance->deviceDescriptor);
		usb_aligned_free(newInstance);

		(void)USB_HostNotifyDevice(NULL, USB_HOST_EVENT_ENUM_FAIL, kStatus_USB_Error);
		return kStatus_USB_Error;
	}

	*deviceHandle = newInstance;

	gIMXRT1020_USB_HOST.deviceAttached = kEHCIDeviceAttached;
	USB_HostNotifyDevice(newInstance, USB_HOST_EVENT_ATTACHED, kStatus_USB_Success);
	return kStatus_USB_Success;
}

usb_status_t USB_HostDetachDevice(uint8_t hubNumber, uint8_t portNumber)
{
	usb_status_t ret;
	USB_SR_ALLOC();
	usb_host_device_instance_t *deviceInstance;

	uvdbg("\nUSB_HostDetachDevice() called - 1\n");
	USB_HostLock();
	/* search for device instance handle */
	deviceInstance = (usb_host_device_instance_t *) gIMXRT1020_USB_HOST.deviceList;	//JS => current device instance

	USB_HostUnlock();
	uvdbg("\nUSB_HostDetachDevice() called - 2\n");
	if (deviceInstance != NULL) {
		uvdbg("\nUSB_HostDetachDevice() called - 3\n");
		ret = USB_HostDetachDeviceInternal(deviceInstance);	/* device instance detach */
		uvdbg("\nUSB_HostDetachDevice() called - 4\n");
		return ret;
	}
	uvdbg("\nUSB_HostDetachDevice() called - 5\n");
	return kStatus_USB_Success;
}

usb_status_t USB_HostDetachDeviceInternal(usb_device_handle deviceHandle)
{
	usb_host_device_instance_t *deviceInstance = (usb_host_device_instance_t *) deviceHandle;
	if (deviceHandle == NULL) {
		return kStatus_USB_InvalidHandle;
	}

	if (deviceInstance->state >= kStatus_DEV_Initial) {	/* device instance is valid */
		uvdbg("\nUSB_HostDetachDeviceInternal() called\n");
		/* detach internally */
		if (deviceInstance->state < kStatus_DEV_AppUsed) {	/* enumeration is not done */
			if (gIMXRT1020_USB_HOST.controlPipe != NULL) {
				USB_HostCancelTransfer(gIMXRT1020_USB_HOST.controlPipe, NULL);
			}

			USB_HostNotifyDevice(deviceInstance, USB_HOST_EVENT_DETACHED, kStatus_USB_Success);
			sleep(1);

			/* remove device instance from host */
			USB_HostRemoveDeviceInstance(deviceInstance);
			USB_HostReleaseDeviceResource(deviceInstance);
		} else {				/* enumeration has be done and notified application */

			/* notify application device detach */
			USB_HostNotifyDevice(deviceInstance, USB_HOST_EVENT_DETACHED, kStatus_USB_Success);
		}
	}

	return kStatus_USB_Success;
}

static usb_status_t USB_HostControlBus(uint8_t controlType)
{
	usb_status_t status = kStatus_USB_Success;

	status = USB_HostEhciIoctl(kUSB_HostBusControl, &controlType);

	return status;
}
