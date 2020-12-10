/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "usb_host_config.h"
#include "usb_host.h"
#include "usb_host_hci.h"
#include "usb_host_devices.h"
#include "usb_host_ehci.h"
#include <string.h>

#ifdef USB_TIZEN_RT

#include <errno.h>
#include <tinyara/usb/usb.h>
#include <tinyara/usb/usbdev.h>
#include <tinyara/usb/usbhost.h>
#include <tinyara/usb/usbhost.h>

#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief release host instance.
 *
 * @param hostInstance  host instance pointer.
 */
static void USB_HostReleaseInstance();

/*******************************************************************************
 * Variables
 ******************************************************************************/

USB_RAM_ADDRESS_ALIGNMENT(4096)
USB_CONTROLLER_DATA static uint8_t gIMXRT1020_USB_HOST_FRAME_LIST[USB_HOST_CONFIG_EHCI_FRAME_LIST_SIZE * 4];
USB_RAM_ADDRESS_ALIGNMENT(64)
USB_CONTROLLER_DATA static usb_host_ehci_data_t gIMXRT1020_USB_HOST_DATA;
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t s_Setupbuffer[USB_HOST_CONFIG_MAX_TRANSFERS][USB_DATA_ALIGN_SIZE_MULTIPLE(8)];

IMXRT1020_USB_HOST gIMXRT1020_USB_HOST = {
	.usb_analog_index = 0,
	.irqNumber = CONFIG_USB_HOST_IRQ_NUMBER,
	.irqPriority = CONFIG_USB_HOST_IRQ_PRIORITY,
	.usb = IMXRT1020_USB,
	.usb_nc = IMXRT1020_USB_NC,
	.usb_phy = IMXRT1020_USB_PHY,
	.usb_analog = IMXRT1020_USB_ANALOG,
	.occupied = 0,
	.ehciFrameList = gIMXRT1020_USB_HOST_FRAME_LIST,
};

struct usbhost_roothubport_s g_USBHOST_ROOT_HUB_PORT;
struct usbhost_driver_s g_USBHOST_DRIVER;
volatile struct usbhost_hubport_s *g_USBHOST_HUBPORT = NULL;

/*******************************************************************************
* Code
******************************************************************************/
#ifdef USB_TIZEN_RT

#define CONFIG_IMXRT_EHCI_BUFSIZE (128)
static int imxrt_ep0configure(FAR struct usbhost_driver_s *drvr, usbhost_ep_t ep0, uint8_t funcaddr, uint8_t speed, uint16_t maxpacketsize)
{
	if (gIMXRT1020_USB_HOST.deviceList) {
		if (USB_Host_EP0_Configure((usb_host_pipe_handle) ep0, funcaddr, speed, maxpacketsize) == kStatus_USB_Success) {
			return OK;
		}
	}
	return -ENOSYS;
}

static int imxrt_epalloc(FAR struct usbhost_driver_s *drvr, const FAR struct usbhost_epdesc_s *epdesc, usbhost_ep_t *ep)
{
	struct usbhost_hubport_s *hport;
	usb_host_pipe_init_t pipe_init;
	usb_host_pipe_handle pipe = NULL;

	/* Sanity check.  NOTE that this method should only be called if a device is
	 * connected (because we need a valid low speed indication).
	 */
	DEBUGASSERT(drvr != 0 && epdesc != NULL && epdesc->hport != NULL && ep != NULL);
	hport = epdesc->hport;

	udbg("EP%d DIR=%s FA=%08x TYPE=%d Interval=%d MaxPacket=%d hub_port=%d\n", epdesc->addr, epdesc->in ? "IN" : "OUT", hport->funcaddr, epdesc->xfrtype, epdesc->interval, epdesc->mxpacketsize, hport->port);

	/* open pipe */
	// need to check same enum value or not !!!!

	switch (epdesc->xfrtype) {
	case USB_EP_ATTR_XFER_CONTROL:
		pipe_init.pipeType = USB_ENDPOINT_CONTROL;
		uvdbg("imxrt_epalloc() : USB_ENDPOINT_CONTROL\n");
		break;
	case USB_EP_ATTR_XFER_ISOC:
		pipe_init.pipeType = USB_ENDPOINT_ISOCHRONOUS;
		uvdbg("imxrt_epalloc() : USB_ENDPOINT_ISOCHRONOUS\n");
		break;
	case USB_EP_ATTR_XFER_BULK:
		pipe_init.pipeType = USB_ENDPOINT_BULK;
		uvdbg("imxrt_epalloc() : USB_ENDPOINT_BULK\n");
		break;
	case USB_EP_ATTR_XFER_INT:
		pipe_init.pipeType = USB_ENDPOINT_INTERRUPT;
		uvdbg("imxrt_epalloc() : USB_ENDPOINT_INTERRUPT\n");
		break;
	}

	pipe_init.direction = epdesc->in ? USB_IN : USB_OUT;	// confirmed

	uvdbg("imxrt_epalloc() : dirrection : %s\n", pipe_init.direction ? "USB_IN" : "USB_OUT");

	// SDK
	// IMXRT_USB_SPEED_FULL         (0x00U) // 12Mbps
	// IMXRT_USB_SPEED_LOW          (0x01U) // 1.5Mbps
	// IMXRT_USB_SPEED_HIGH         (0x02U) // 480Mbps
	// IMXRT_USB_SPEED_SUPER        (0x04U)

	// Tizen RT
	// USB_SPEED_LOW                1   /* USB 1.0 */   // 1.5Mbps
	// USB_SPEED_FULL               2   /* USB 1.1 */   // 12Mbps
	// USB_SPEED_HIGH               3   /* USB 2.0 */   // 480Mbps

	switch (hport->speed) {
	case USB_SPEED_LOW:		// 1.5Mbps
		uvdbg("imxrt_epalloc() : USB_SPEED_LOW\n");
		pipe_init.speed = IMXRT_USB_SPEED_LOW;
		break;
	case USB_SPEED_HIGH:		// 480 Mbps
		uvdbg("imxrt_epalloc() : USB_SPEED_HIGH\n");
		pipe_init.speed = IMXRT_USB_SPEED_HIGH;
		break;
	case USB_SPEED_FULL:		// 12Mbps
	default:
		uvdbg("imxrt_epalloc() : USB_SPEED_FULL\n");
		pipe_init.speed = IMXRT_USB_SPEED_FULL;
		break;
	}

	pipe_init.deviceAddress = hport->funcaddr;	// default address => 0
	pipe_init.endpointAddress = (epdesc->addr & USB_DESCRIPTOR_ENDPOINT_ADDRESS_NUMBER_MASK);
	pipe_init.interval = epdesc->interval;
	pipe_init.maxPacketSize = (uint16_t)(epdesc->mxpacketsize & USB_DESCRIPTOR_ENDPOINT_MAXPACKETSIZE_SIZE_MASK);
	pipe_init.numberPerUframe = (epdesc->mxpacketsize & USB_DESCRIPTOR_ENDPOINT_MAXPACKETSIZE_MULT_TRANSACTIONS_MASK) >> USB_DESCRIPTOR_ENDPOINT_MAXPACKETSIZE_MULT_TRANSACTIONS_SHFIT;

	uvdbg("imxrt_epalloc() : device address   => %d\n", pipe_init.deviceAddress);
	uvdbg("imxrt_epalloc() : endpoint address => %d\n", pipe_init.endpointAddress);
	uvdbg("imxrt_epalloc() : interval         => %d\n", pipe_init.interval);
	uvdbg("imxrt_epalloc() : maxPacketSize    => %d\n", pipe_init.maxPacketSize);
	uvdbg("imxrt_epalloc() : numberPerUframe  => %d\n", pipe_init.numberPerUframe);

	// Warning hard coded !!
	pipe_init.hubNumber = 0;	/*!< Device's first connected hub address (root hub = 0) */
	pipe_init.portNumber = hport->port;	/*!< Device's first connected hub's port no (1 - 8) */
	pipe_init.level = 1;

#ifdef CONFIG_USBHOST_HUB
	uvdbg("imxrt_epalloc() : in case of using hub => currently hubnumber,level is hard coded!!\n", hport->parent);
	if (hport->parent) {
		pipe_init.hubNumber = 1;	/*!< Device's first connected hub address (root hub = 0) */
		pipe_init.portNumber = hport->port;	/*!< Device's first connected hub's port no (1 - 8) */
		pipe_init.level = 2;	/*!< Device's level (root device = 0) */
	} 
#endif
	// Warning : asumption => all usb hub has same speed
	pipe_init.hsHubNumber = pipe_init.hubNumber;	/*!< Device's first connected high-speed hub's address (1 - 8) => temporary : same as hubNumber */
	pipe_init.hsHubPort = pipe_init.portNumber;	/*!< Device's first connected high-speed hub's port no (1 - 8) => temporary : same as portNumber */
	pipe_init.nakCount = USB_HOST_CONFIG_MAX_NAK;

	uvdbg("imxrt_epalloc() : hubNumber        => %d\n", pipe_init.hubNumber);
	uvdbg("imxrt_epalloc() : portNumber       => %d\n", pipe_init.portNumber);
	uvdbg("imxrt_epalloc() : hsHubNumber      => %d\n", pipe_init.hsHubNumber);
	uvdbg("imxrt_epalloc() : hsHubPort        => %d\n", pipe_init.hsHubPort);
	uvdbg("imxrt_epalloc() : level            => %d\n", pipe_init.level);

	if (USB_HostOpenPipe(&pipe, &pipe_init) != kStatus_USB_Success) {
		return -ENOMEM;
	}

	*ep = (usbhost_ep_t) pipe;
	return OK;
}

static int imxrt_epfree(FAR struct usbhost_driver_s *drvr, usbhost_ep_t ep)
{
	usb_host_pipe_handle pipe = (usb_host_pipe_handle) ep;

	DEBUGASSERT(drvr && pipe);

	/* There should not be any pending, transfers */
	if (pipe != NULL) {
		USB_HostCancelTransfer(pipe, NULL);
		if (USB_HostClosePipe(pipe) != kStatus_USB_Success) {
			udbg("error when close pipe 0x%x\r\n", pipe);
		}
		pipe = NULL;
	}
	return OK;
}

static int imxrt_alloc(FAR struct usbhost_driver_s *drvr, FAR uint8_t **buffer, FAR size_t *maxlen)
{
	int ret = -ENOMEM;
	DEBUGASSERT(drvr && buffer && maxlen);

	/* The only special requirements for transfer/descriptor buffers are that (1)
	 * they be aligned to a cache line boundary and (2) they are a multiple of the
	 * cache line size in length.
	 */

	*buffer = (FAR uint8_t *)kmm_memalign(USB_DATA_ALIGN_SIZE, roundup((CONFIG_IMXRT_EHCI_BUFSIZE), (USB_DATA_ALIGN_SIZE)));
	if (*buffer) {
		*maxlen = roundup((CONFIG_IMXRT_EHCI_BUFSIZE), (USB_DATA_ALIGN_SIZE));
		ret = OK;
	}
	return ret;
}

static int imxrt_free(FAR struct usbhost_driver_s *drvr, FAR uint8_t *buffer)
{
	DEBUGASSERT(drvr && buffer);

	/* No special action is require to free the transfer/descriptor buffer memory */
	kmm_free(buffer);
	return OK;
}

static int imxrt_ioalloc(FAR struct usbhost_driver_s *drvr, FAR uint8_t **buffer, size_t buflen)
{
	DEBUGASSERT(drvr && buffer && buflen > 0);

	/* The only special requirements for I/O buffers are that (1) they be aligned to a
	 * cache line boundary, (2) they are a multiple of the cache line size in length,
	 * and (3) they might need to be user accessible (depending on how the class driver
	 * implements its buffering).
	 */
	*buffer = (FAR uint8_t *) kumm_memalign(USB_DATA_ALIGN_SIZE, roundup((buflen), (USB_DATA_ALIGN_SIZE)));
	udbg("\nimxrt_ioalloc() requested %d bytes and returned %d bytes\n", buflen, roundup((buflen), (USB_DATA_ALIGN_SIZE)));
	udbg("\nimxrt_ioalloc() returned [0x%x]\n", *buffer);
	return *buffer ? OK : -ENOMEM;
}

static int imxrt_iofree(FAR struct usbhost_driver_s *drvr, FAR uint8_t *buffer)
{
	DEBUGASSERT(drvr && buffer);

	/* No special action is require to free the I/O buffer memory */

	kumm_free(buffer);
	return OK;
}

static int imxrt_ctrlin(FAR struct usbhost_driver_s *drvr, usbhost_ep_t ep0, FAR const struct usb_ctrlreq_s *req, FAR uint8_t *buffer)
{
	DEBUGASSERT(drvr != NULL && ep0 != NULL && req != NULL);

	if (USB_Host_ControlTransfer(ep0, (uint8_t *) req, buffer) == kStatus_USB_Success) {
		return OK;
	}

	return -EIO;
}

static int imxrt_ctrlout(FAR struct usbhost_driver_s *drvr, usbhost_ep_t ep0, FAR const struct usb_ctrlreq_s *req, FAR const uint8_t *buffer)
{
	/* imxrt_ctrlin can handle both directions.  We just need to work around the
	 * differences in the function signatures.
	 */

	return imxrt_ctrlin(drvr, ep0, req, (uint8_t *) buffer);
}

static ssize_t imxrt_transfer(FAR struct usbhost_driver_s *drvr, usbhost_ep_t ep, FAR uint8_t *buffer, size_t buflen)
{
	ssize_t transferred = 0;
	DEBUGASSERT(drvr && ep && buffer && buflen > 0);

	if (USB_Host_Transfer(ep, buffer, buflen, &transferred) == kStatus_USB_Success) {
		return transferred;
	}
	return -EIO;
}

#ifdef CONFIG_USBHOST_ASYNCH
typedef struct _usb_host_async_op_cb_ {
	usbhost_asynch_t callback;
	void *arg;
} usb_host_async_op_cb;

void imxrt_usb_async_callback(void *param, usb_host_transfer_t *transfer, usb_status_t status)
{
	usb_host_async_op_cb *async_op_cb = (usb_host_async_op_cb *) param;
	if (async_op_cb) {
		if (async_op_cb->callback) {
			async_op_cb->callback(async_op_cb->arg, transfer->transferSofar);
		}
		usb_aligned_free(async_op_cb);
	}
	USB_HostFreeTransfer(transfer);
}

static int imxrt_asynch(FAR struct usbhost_driver_s *drvr, usbhost_ep_t ep, FAR uint8_t *buffer, size_t buflen, usbhost_asynch_t callback, FAR void *arg)
{
	usb_status_t status = kStatus_USB_Success;
	usb_host_transfer_t *transfer;
	usb_host_async_op_cb *result = NULL;
	usb_host_ehci_pipe_t *ehciPipePointer = (usb_host_ehci_pipe_t *) ep;

	if (buffer && buflen) {
		result = (usb_host_async_op_cb *) usb_aligned_malloc(sizeof(usb_host_async_op_cb), USB_CACHE_LINESIZE);
		if (!result) {
			return -ENOMEM;
		}

		result->callback = callback;
		result->arg = arg;

		/* malloc transfer */
		if (USB_HostMallocTransfer(&transfer) != kStatus_USB_Success) {
			usb_aligned_free(result);
			return -EIO;
		}

		transfer->callbackFn = imxrt_usb_async_callback;
		transfer->callbackParam = result;

		transfer->transferBuffer = buffer;
		transfer->transferLength = buflen;

		if (ehciPipePointer->pipeCommon.direction == USB_OUT) {
			if (USB_HostSend((usb_host_pipe_handle) ehciPipePointer, transfer) != kStatus_USB_Success) {
				usb_aligned_free(result);
				udbg("imxrt_asynch() USB_HostSend failed\n");
				return -EIO;
			}
		} else {
			if (USB_HostRecv((usb_host_pipe_handle) ehciPipePointer, transfer) != kStatus_USB_Success) {
				usb_aligned_free(result);
				udbg("imxrt_asynch() USB_HostRecv failed\n");
				return -EIO;
			}
		}
	}
	return OK;
}

#endif							/* CONFIG_USBHOST_ASYNCH */

static int imxrt_cancel(FAR struct usbhost_driver_s *drvr, usbhost_ep_t ep)
{
	if (ep) {
		if (USB_HostCancelTransfer((usb_host_pipe_handle) ep, NULL) == kStatus_USB_Success) {
			return OK;
		}
	}
	return -EIO;
}

#ifdef CONFIG_USBHOST_HUB
static int imxrt_connect(FAR struct usbhost_driver_s *drvr, FAR struct usbhost_hubport_s *hport, bool connected)
{
	USB_SR_ALLOC();

	/* Set the connected/disconnected flag */
	hport->connected = connected;
	udbg("Hub port %d connected: %s\n", hport->port, connected ? "YES" : "NO");

	/* Report the connection event */

	USB_ENTER_CRITICAL();

	DEBUGASSERT(g_USBHOST_HUBPORT == NULL);	/* REVISIT */
	g_USBHOST_HUBPORT = hport;	// imxrt_connect set g_ehci.g_USBHOST_HUBPORT

	USB_EXIT_CRITICAL();

	USB_EVENT_FIFO_PUSH(&(gIMXRT1020_USB_HOST.ehciStatusChangeFifo), USB_HUB_EVENT_ATTACHED);
	USB_EHCI_EVENT_TRIGGER_SEND(gIMXRT1020_USB_HOST.ehciStatusChangeTrigger);
	sleep(1);					//1sec sleep to avoid crash for multipel devices on hubport
	return OK;
}
#endif

static void imxrt_disconnect(FAR struct usbhost_driver_s *drvr, FAR struct usbhost_hubport_s *hport)
{
	DEBUGASSERT(hport != NULL);
	hport->devclass = NULL;
}

#endif

static void USB_HostReleaseInstance()
{
	USB_SR_ALLOC();
	USB_ENTER_CRITICAL();
	gIMXRT1020_USB_HOST.occupied = 0;

	USB_EXIT_CRITICAL();
}

void USB_HostIsrEnable(void)
{
#ifdef USB_TIZEN_RT
	//  up_prioritize_irq(IMXRT_IRQ_USBOTG1, CONFIG_IMXRT_ENET_PRIO);
	up_enable_irq(IMXRT_IRQ_USBOTG1);	/* enable USB interrupt */
#else
	/* Install isr, set priority, and enable IRQ. */
	NVIC_SetPriority((IRQn_Type) gIMXRT1020_USB_HOST.irqNumber, gIMXRT1020_USB_HOST.irqPriority);
	EnableIRQ((IRQn_Type) gIMXRT1020_USB_HOST.irqNumber);
#endif
}

void USB_HostSynchronousCallback(void *param, usb_host_transfer_t *transfer, usb_status_t status)
{
	usb_host_ehci_sync_result_t *result = (usb_host_ehci_sync_result_t *) param;
	result->result = status;
	result->dataLength = transfer->transferSofar;
	USB_EHCI_EVENT_TRIGGER_SEND(result->finished);
}

usb_status_t USB_HostOpenPipe(usb_host_pipe_handle *pipeHandle, usb_host_pipe_init_t *pipeInit)
{
	usb_status_t status = kStatus_USB_Success;
	if (pipeInit == NULL) {
		return kStatus_USB_InvalidHandle;
	}

	status = USB_HostEhciOpenPipe(pipeHandle, pipeInit);
	return status;
}

usb_status_t USB_HostClosePipe(usb_host_pipe_handle pipeHandle)
{
	usb_status_t status = kStatus_USB_Success;
	if (pipeHandle == NULL) {
		return kStatus_USB_InvalidHandle;
	}

	status = USB_HostEhciClosePipe(pipeHandle);

	return status;
}

usb_status_t USB_HostSend(usb_host_pipe_handle pipeHandle, usb_host_transfer_t *transfer)
{
	USB_SR_ALLOC();
	usb_status_t status = kStatus_USB_Success;

	if ((pipeHandle == NULL) || (transfer == NULL)) {
		return kStatus_USB_InvalidHandle;
	}

	/* initialize transfer */
	transfer->transferSofar = 0;
	transfer->direction = USB_OUT;

	USB_HostLock();				/* This api can be called by host task and app task */

	/* call controller write pipe interface */
	if (transfer->transferLength > 0) {
		arch_clean_dcache_by_range_to_poc((uintptr_t)(transfer->transferBuffer), (uint32_t)(transfer->transferBuffer) + transfer->transferLength - 1);
	}

	status = USB_HostEhciWritePipe(pipeHandle, transfer);

	USB_HostUnlock();
	return status;
}

usb_status_t USB_HostSendSetup(usb_host_pipe_handle pipeHandle, usb_host_transfer_t *transfer)
{
	USB_SR_ALLOC();
	usb_status_t status = kStatus_USB_Success;

	if ((pipeHandle == NULL) || (transfer == NULL)) {
		return kStatus_USB_InvalidHandle;
	}

	/* initialize transfer */
	transfer->transferSofar = 0;
	transfer->next = NULL;
	transfer->setupStatus = 0;
	if ((transfer->setupPacket->bmRequestType & USB_REQUEST_TYPE_DIR_MASK) == USB_REQUEST_TYPE_DIR_IN) {
		transfer->direction = USB_IN;
	} else {
		transfer->direction = USB_OUT;
	}

	USB_HostLock();				/* This API can be called by host task and application task */

	/* call controller write pipe interface */
	arch_clean_dcache_by_range_to_poc((uintptr_t)(&(transfer->setupPacket->bmRequestType)), (uint32_t)(&(transfer->setupPacket->bmRequestType)) + sizeof(usb_setup_struct_t) - 1);
	if (transfer->transferLength > 0) {
		arch_clean_invalidate_dcache_by_range_to_poc((uintptr_t)(transfer->transferBuffer), (((uint32_t)(transfer->transferBuffer)) + transfer->transferLength - 1));
	}

	status = USB_HostEhciWritePipe(pipeHandle, transfer);

	USB_HostUnlock();
	return status;
}

usb_status_t USB_HostRecv(usb_host_pipe_handle pipeHandle, usb_host_transfer_t *transfer)
{
	USB_SR_ALLOC();
	usb_status_t status = kStatus_USB_Success;

	if ((pipeHandle == NULL) || (transfer == NULL)) {
		return kStatus_USB_InvalidHandle;
	}

	/* initialize transfer */
	transfer->transferSofar = 0;
	transfer->direction = USB_IN;

	USB_HostLock();				/* This API can be called by host task and application task */

	if (transfer->transferLength > 0) {
		arch_clean_invalidate_dcache_by_range_to_poc((uintptr_t)(transfer->transferBuffer), (((uint32_t)(transfer->transferBuffer)) + transfer->transferLength - 1));
	}

	status = USB_HostEhciReadpipe(pipeHandle, transfer);

	USB_HostUnlock();
	return status;
}

usb_status_t USB_HostCancelTransfer(usb_host_pipe_handle pipeHandle, usb_host_transfer_t *transfer)
{
	usb_status_t status = kStatus_USB_Success;
	usb_host_cancel_param_t cancelParam;

	if (pipeHandle == NULL) {
		return kStatus_USB_InvalidHandle;
	}

	/* initialize cancel parameter */
	cancelParam.pipeHandle = pipeHandle;
	cancelParam.transfer = transfer;

	/* USB_HostLock(); This api can be called by host task and app task */
	status = USB_HostEhciIoctl(kUSB_HostCancelTransfer, &cancelParam);
	/* USB_HostUnlock(); */

	return status;
}

usb_status_t USB_HostMallocTransfer(usb_host_transfer_t **transfer)
{
	USB_SR_ALLOC();
	if (transfer == NULL) {
		return kStatus_USB_InvalidHandle;
	}

	/* get one from the transfer_head */
	USB_HostLock();

	if (gIMXRT1020_USB_HOST.transferHead != NULL) {
		*transfer = gIMXRT1020_USB_HOST.transferHead;
		gIMXRT1020_USB_HOST.transferHead = gIMXRT1020_USB_HOST.transferHead->next;
		USB_HostUnlock();
		return kStatus_USB_Success;
	} else {
		*transfer = NULL;
		USB_HostUnlock();
		return kStatus_USB_Error;
	}
}

usb_status_t USB_HostFreeTransfer(usb_host_transfer_t *transfer)
{
	USB_SR_ALLOC();
	if (transfer == NULL) {
		return kStatus_USB_Success;
	}

	/* release one to the transfer_head */
	USB_HostLock();
	transfer->next = gIMXRT1020_USB_HOST.transferHead;
	gIMXRT1020_USB_HOST.transferHead = transfer;
	USB_HostUnlock();
	return kStatus_USB_Success;
}

usb_status_t USB_HostHelperGetPeripheralInformation(usb_device_handle deviceHandle, uint32_t infoCode, uint32_t *infoValue)
{
	usb_host_device_instance_t *deviceInstance = (usb_host_device_instance_t *) deviceHandle;
	if ((deviceHandle == NULL) || (infoValue == NULL)) {
		return kStatus_USB_InvalidParameter;
	}

	switch (infoCode) {
	case kUSB_HostGetDevicePID:	/* device pid */
		*infoValue = (uint32_t) USB_SHORT_FROM_LITTLE_ENDIAN_ADDRESS(deviceInstance->deviceDescriptor->idProduct);
		break;

	case kUSB_HostGetDeviceVID:	/* device vid */
		*infoValue = (uint32_t) USB_SHORT_FROM_LITTLE_ENDIAN_ADDRESS(deviceInstance->deviceDescriptor->idVendor);
		break;

	case kUSB_HostGetDeviceConfigIndex:	/* device config index */
		*infoValue = (uint32_t) deviceInstance->configurationValue - 1U;
		break;

	case kUSB_HostGetConfigurationDes:	/* configuration descriptor pointer */
		*infoValue = (uint32_t) deviceInstance->configurationDesc;
		break;

	case kUSB_HostGetConfigurationLength:	/* configuration descriptor length */
		*infoValue = (uint32_t) deviceInstance->configurationLen;
		break;

	default:
		return kStatus_USB_Error;
	}

	return kStatus_USB_Success;
}

void USB_HostGetVersion(uint32_t *version)
{
	if (version) {
		*version = (uint32_t) USB_MAKE_VERSION(USB_STACK_VERSION_MAJOR, USB_STACK_VERSION_MINOR, USB_STACK_VERSION_BUGFIX);
	}
}

#if ((defined(USB_HOST_CONFIG_LOW_POWER_MODE)) && (USB_HOST_CONFIG_LOW_POWER_MODE > 0U))
/* Send BUS or specific device suspend request */
usb_status_t USB_HostSuspendDeviceResquest(usb_device_handle deviceHandle)
{
	usb_status_t status = kStatus_USB_Error;
	usb_host_bus_control_t type = kUSB_HostBusSuspend;

	gIMXRT1020_USB_HOST.suspendedDevice = (void *)deviceHandle;

	if (NULL == deviceHandle) {
		status = USB_HostEhciIoctl(kUSB_HostBusControl, &type);
	} else {
		if (gIMXRT1020_USB_HOST.deviceList == deviceHandle) {
			status = USB_HostEhciIoctl(kUSB_HostBusControl, &type);
		}
	}
	if (kStatus_USB_Error == status) {
		gIMXRT1020_USB_HOST.suspendedDevice = NULL;
	}
	return status;
}

/* Send BUS or specific device resume request */
usb_status_t USB_HostResumeDeviceResquest(usb_device_handle deviceHandle)
{
	usb_status_t status = kStatus_USB_Error;
	usb_host_bus_control_t type = kUSB_HostBusResume;

	if (gIMXRT1020_USB_HOST.suspendedDevice != deviceHandle) {
		return kStatus_USB_InvalidParameter;
	}
	gIMXRT1020_USB_HOST.suspendedDevice = (void *)deviceHandle;

	if (NULL == deviceHandle) {
		status = USB_HostEhciIoctl(kUSB_HostBusControl, &type);
	} else {
		if (gIMXRT1020_USB_HOST.deviceList == deviceHandle) {
			status = USB_HostEhciIoctl(kUSB_HostBusControl, &type);
		}
	}

	return status;
}

#if ((defined(USB_HOST_CONFIG_LPM_L1)) && (USB_HOST_CONFIG_LPM_L1 > 0U))
/* Send BUS or specific device suspend request */
usb_status_t USB_HostL1SleepDeviceResquest(usb_device_handle deviceHandle, uint8_t sleepType)
{
	usb_status_t status = kStatus_USB_Error;
	usb_host_bus_control_t type = kUSB_HostBusL1Sleep;

	gIMXRT1020_USB_HOST.suspendedDevice = (void *)deviceHandle;

	if (1U == sleepType) {
		status = USB_HostEhciIoctl(kUSB_HostBusControl, &type);
	} else {
		if (gIMXRT1020_USB_HOST.deviceList == deviceHandle) {
			status = USB_HostEhciIoctl(kUSB_HostBusControl, &type);
		}
	}
	if (kStatus_USB_Error == status) {
		gIMXRT1020_USB_HOST.suspendedDevice = NULL;
	}
	return status;
}

/* Send BUS or specific device suspend request */
usb_status_t USB_HostL1SleepDeviceResquestConfig(uint8_t *lpmParam)
{
	usb_status_t status = kStatus_USB_Error;
	status = USB_HostEhciIoctl(kUSB_HostL1Config, lpmParam);

	return status;
}

/* Send BUS or specific device resume request */
usb_status_t USB_HostL1ResumeDeviceResquest(usb_device_handle deviceHandle, uint8_t sleepType)
{
	usb_status_t status = kStatus_USB_Error;
	usb_host_bus_control_t type = kUSB_HostBusL1Resume;

	if (1U == sleepType) {
		status = USB_HostEhciIoctl(kUSB_HostBusControl, &type);
	} else {
		if (gIMXRT1020_USB_HOST.deviceList == deviceHandle) {
			status = USB_HostEhciIoctl(kUSB_HostBusControl, &type);
		}
	}

	return status;
}
#endif
/* Update HW tick(unit is ms) */
usb_status_t USB_HostUpdateHwTick(uint64_t tick)
{
	usb_status_t status = kStatus_USB_Success;
	gIMXRT1020_USB_HOST.hwTick = tick;
	return status;
}
#endif

#if ((defined(USB_HOST_CONFIG_BATTERY_CHARGER)) && (USB_HOST_CONFIG_BATTERY_CHARGER > 0U))
usb_status_t USB_HostSetChargerType(uint8_t type)
{
	return USB_HostEhciIoctl(kUSB_HostSetChargerType, &type);
}
#endif
#ifdef CONFIG_USBHOST_UVC
extern int usbhost_uvc_init(void);
#endif
#ifdef CONFIG_USBHOST_HUB
extern int usbhost_hub_initialize();
#endif
#ifdef CONFIG_USBHOST_RTL8723D
extern int usbhost_rtl8723d_initialize(void);
#endif
usb_status_t USB_HostInit(host_callback_t callbackFn)
{
	usb_status_t status = kStatus_USB_Success;
	usb_host_transfer_t *transferPrev = NULL;
	uint32_t i = 0;

	USB_SR_ALLOC();
#ifdef CONFIG_USBHOST_HUB
	if (usbhost_hub_initialize() != OK) {
		udbg("ERROR: usbhost_hub_initialize failed:\n");
	}
#endif
#ifdef CONFIG_USBHOST_MSC
	/* Register the USB mass storage class class */
	if (usbhost_msc_initialize() != OK) {
		udbg("Failed to register the mass storage class\n");
	}
#endif
#ifdef CONFIG_USBHOST_UVC
	if (usbhost_uvc_init() != OK) {
		udbg("ERROR: Failed to start USB UVC services\n");
	}
#endif
#ifdef CONFIG_USBHOST_RTL8723D
	extern unsigned char g_rtk_wifi_connect;

	if (!g_rtk_wifi_connect) {
		if (usbhost_rtl8723d_initialize() != OK) {
			udbg("ERROR: Failed to start rtl8723d Wifi services\n");
		}
	}
#endif

	USB_ENTER_CRITICAL();

	if (gIMXRT1020_USB_HOST.occupied != 1) {
		memset(&gIMXRT1020_USB_HOST, 0x00, sizeof(gIMXRT1020_USB_HOST));

		gIMXRT1020_USB_HOST.usb_analog_index = 0;

		gIMXRT1020_USB_HOST.irqNumber = CONFIG_USB_HOST_IRQ_NUMBER;
		gIMXRT1020_USB_HOST.irqPriority = CONFIG_USB_HOST_IRQ_PRIORITY;

		gIMXRT1020_USB_HOST.usb = IMXRT1020_USB;
		gIMXRT1020_USB_HOST.usb_nc = IMXRT1020_USB_NC;
		gIMXRT1020_USB_HOST.usb_phy = IMXRT1020_USB_PHY;
		gIMXRT1020_USB_HOST.usb_analog = IMXRT1020_USB_ANALOG;
		gIMXRT1020_USB_HOST.ehciFrameList = gIMXRT1020_USB_HOST_FRAME_LIST;
		gIMXRT1020_USB_HOST.occupied = 1;

		/* initialize qh/qtd/itd/sitd/iso list */
		gIMXRT1020_USB_HOST.ehciQhList = gIMXRT1020_USB_HOST_DATA.ehciQh;
		gIMXRT1020_USB_HOST.ehciQtdHead = gIMXRT1020_USB_HOST_DATA.ehciQtd;
		gIMXRT1020_USB_HOST.ehciItdList = gIMXRT1020_USB_HOST_DATA.ehciItd;
		gIMXRT1020_USB_HOST.ehciSitdIndexBase = gIMXRT1020_USB_HOST_DATA.ehciSitd;
		gIMXRT1020_USB_HOST.ehciSitdList = gIMXRT1020_USB_HOST_DATA.ehciSitd;
		gIMXRT1020_USB_HOST.ehciIsoList = gIMXRT1020_USB_HOST_DATA.ehciIso;
		gIMXRT1020_USB_HOST.ehciPipeIndexBase = gIMXRT1020_USB_HOST_DATA.ehciPipe;

		for (i = 0; i < USB_HOST_CONFIG_MAX_TRANSFERS; ++i) {
			gIMXRT1020_USB_HOST.transferList[i].setupPacket = (usb_setup_struct_t *)&(s_Setupbuffer[i][0]);
		}

#ifdef USB_TIZEN_RT
		gIMXRT1020_USB_HOST.ehciStatusMonitorTask = -1;
#else
		gIMXRT1020_USB_HOST.ehciEventTask = NULL;
		gIMXRT1020_USB_HOST.ehciStatusMonitorTask = NULL;
#endif
	}
	USB_EXIT_CRITICAL();

	/* HOST instance init */
	gIMXRT1020_USB_HOST.deviceCallback = callbackFn;
	gIMXRT1020_USB_HOST.deviceList = NULL;

	/* initialize transfer list */

	gIMXRT1020_USB_HOST.transferHead = &(gIMXRT1020_USB_HOST.transferList[0]);
	transferPrev = gIMXRT1020_USB_HOST.transferHead;
	for (i = 1; i < USB_HOST_CONFIG_MAX_TRANSFERS; ++i) {
		transferPrev->next = &(gIMXRT1020_USB_HOST.transferList[i]);
		transferPrev = transferPrev->next;
	}

	status = USB_HostEhciCreate();
	if (status != kStatus_USB_Success) {
		USB_HostReleaseInstance();
		udbg("host init: controller init fail\r\n");
		return kStatus_USB_Error;
	}

	uvdbg("\nUSB_HostInit() will register into tizenRT USB stack\n");

#ifdef USB_TIZEN_RT
	/* Initialize the device operations */

	g_USBHOST_DRIVER.ep0configure = imxrt_ep0configure;
	g_USBHOST_DRIVER.epalloc = imxrt_epalloc;
	g_USBHOST_DRIVER.epfree = imxrt_epfree;
	g_USBHOST_DRIVER.alloc = imxrt_alloc;
	g_USBHOST_DRIVER.free = imxrt_free;
	g_USBHOST_DRIVER.ioalloc = imxrt_ioalloc;
	g_USBHOST_DRIVER.iofree = imxrt_iofree;
	g_USBHOST_DRIVER.ctrlin = imxrt_ctrlin;
	g_USBHOST_DRIVER.ctrlout = imxrt_ctrlout;
	g_USBHOST_DRIVER.transfer = imxrt_transfer;

#ifdef CONFIG_USBHOST_ASYNCH
	g_USBHOST_DRIVER.asynch = imxrt_asynch;
#endif
	g_USBHOST_DRIVER.cancel = imxrt_cancel;

#ifdef CONFIG_USBHOST_HUB
	g_USBHOST_DRIVER.connect = imxrt_connect;
#endif
	g_USBHOST_DRIVER.disconnect = imxrt_disconnect;

	/* Initialize the public port representation */
	g_USBHOST_ROOT_HUB_PORT.hport.drvr = &g_USBHOST_DRIVER;
#ifdef CONFIG_USBHOST_HUB
	g_USBHOST_ROOT_HUB_PORT.hport.parent = NULL;
	udbg("g_USBHOST_ROOT_HUB_PORT.hport.parent = [0x%x]\n", (uint32_t) g_USBHOST_ROOT_HUB_PORT.hport.parent);
#endif
	g_USBHOST_ROOT_HUB_PORT.hport.ep0 = NULL;	// struct imxrt_epinfo_s ep0;   /* EP0 endpoint info */, hport.ep0 => usbhost_ep_t ep0; => void* => pointer to control point
	g_USBHOST_ROOT_HUB_PORT.hport.port = 0;
	g_USBHOST_ROOT_HUB_PORT.hport.speed = USB_SPEED_FULL;

	/* Initialize function address generation logic */
	usbhost_devaddr_initialize(&g_USBHOST_ROOT_HUB_PORT);
#endif

	USB_HostIsrEnable();
	return kStatus_USB_Success;
}

usb_status_t USB_HostDeinit()
{
	usb_status_t status = kStatus_USB_Success;
	usb_host_device_instance_t *deviceInstance = NULL;

	/* device list detach */
	deviceInstance = (usb_host_device_instance_t *) gIMXRT1020_USB_HOST.deviceList;
	while (deviceInstance != NULL) {
		deviceInstance = (usb_host_device_instance_t *) gIMXRT1020_USB_HOST.deviceList;
		USB_HostDetachDeviceInternal(deviceInstance);
	}

	status = USB_HostEhciDestroy();
	if (status != kStatus_USB_Success) {
		udbg("host controller destroy fail\r\n");
	}

	/* resource release */
	USB_HostReleaseInstance();
	return status;
}

static usb_status_t imxrt_usbhost_eventhandler(usb_device_handle deviceHandle, usb_host_configuration_handle configurationHandle, uint32_t eventCode)
{
	int event_received = 0;
	switch (eventCode & 0x0000FFFFU) {
	case USB_HOST_EVENT_ATTACHED:
		USB_EVENT_FIFO_PUSH(&(gIMXRT1020_USB_HOST.ehciStatusChangeFifo), USB_HOST_EVENT_ATTACHED);
		event_received = 1;
		break;
	case USB_HOST_EVENT_DETACHED:
		USB_EVENT_FIFO_PUSH(&(gIMXRT1020_USB_HOST.ehciStatusChangeFifo), USB_HOST_EVENT_DETACHED);
		event_received = 1;
		break;
	case USB_HOST_EVENT_ENUM_FAIL:
		USB_EVENT_FIFO_PUSH(&(gIMXRT1020_USB_HOST.ehciStatusChangeFifo), USB_HOST_EVENT_ENUM_FAIL);
		event_received = 1;
		break;
	default:
		break;
	}
	if (event_received) {
		USB_EHCI_EVENT_TRIGGER_SEND(gIMXRT1020_USB_HOST.ehciStatusChangeTrigger);
	}
	return kStatus_USB_Success;
}

void imxrt_usbhost_service_initialize(void)
{
	usb_status_t status = kStatus_USB_Success;
	status = USB_HostInit(imxrt_usbhost_eventhandler);
	if (status != kStatus_USB_Success) {
		udbg("host init error\r\n");
		return;
	}
}
