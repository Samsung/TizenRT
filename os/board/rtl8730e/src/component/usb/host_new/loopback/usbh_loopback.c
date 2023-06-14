/**
  ******************************************************************************
  * @file    usbh_loopback.c
  * @author  Realsil WLAN5 Team
  * @brief   This file provides the functionalities of the USB Vendor Class
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

#include "usbh_loopback.h"
#include "usbh.h"

/* Private defines -----------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

static u8 usbh_loopback_cb_attach(usb_host_t *host);
static u8 usbh_loopback_cb_detach(usb_host_t *host);
static u8 usbh_loopback_cb_process(usb_host_t *host);
static u8 usbh_loopback_cb_setup(usb_host_t *host);
/* Private variables ---------------------------------------------------------*/

/* USB Standard Device Descriptor */
static usbh_class_driver_t usbh_loopback_driver = {
	.class_code = LOOPBACK_CLASS_CODE,
	.attach = usbh_loopback_cb_attach,
	.detach = usbh_loopback_cb_detach,
	.setup = usbh_loopback_cb_setup,
	.process = usbh_loopback_cb_process,
};

static usbh_loopback_host_t usbh_loopback_host;

/* Private functions ---------------------------------------------------------*/

static void usbh_loopback_get_endpoints(usbh_if_desc_t *intf)
{
	u32 tmp = 0;
	usbh_loopback_host_t *loopback = &usbh_loopback_host;
	usbh_ep_desc_t *ep_desc;

	for (tmp = 0; tmp < intf->bNumEndpoints; tmp++) {
		ep_desc = &intf->ep_desc[tmp];
		switch (ep_desc->bmAttributes & USB_EP_XFERTYPE_MASK) {
		case USB_CH_EP_TYPE_INTR:
			if ((ep_desc->bEndpointAddress & USB_REQ_DIR_MASK) == USB_D2H) {
				loopback->intr_in_ep = ep_desc->bEndpointAddress;
				loopback->intr_in_packet_size  = ep_desc->wMaxPacketSize;
				DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "INTR IN ep: 0x%02X\n", loopback->intr_in_ep);
				DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "INTR IN mps: %d\n", loopback->intr_in_packet_size);
			} else {
				loopback->intr_out_ep = ep_desc->bEndpointAddress;
				loopback->intr_out_packet_size  = ep_desc->wMaxPacketSize;
				DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "INTR OUT ep: 0x%02X\n", loopback->intr_out_ep);
				DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "INTR OUT mps: %d\n", loopback->intr_out_packet_size);
			}

			break;

		case USB_CH_EP_TYPE_ISOC:
			if ((ep_desc->bEndpointAddress & USB_REQ_DIR_MASK) == USB_D2H) {
				loopback->isoc_in_ep = ep_desc->bEndpointAddress;
				loopback->isoc_in_packet_size  = ep_desc->wMaxPacketSize;
				loopback->isoc_in_interval = 1 << (MIN((MAX(ep_desc->bInterval, 1)), 16) - 1);
				DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "ISOC IN ep: 0x%02X\n", loopback->isoc_in_ep);
				DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "ISOC IN mps: %d\n", loopback->isoc_in_packet_size);
				DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "ISOC IN interval: %d(%d)\n", loopback->isoc_in_interval, ep_desc->bInterval);
			} else {
				loopback->isoc_out_ep = ep_desc->bEndpointAddress;
				loopback->isoc_out_packet_size  = ep_desc->wMaxPacketSize;
				loopback->isoc_out_interval = 1 << (MIN((MAX(ep_desc->bInterval, 1)), 16) - 1);
				DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "ISOC OUT ep: 0x%02X\n", loopback->isoc_out_ep);
				DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "ISOC OUT mps: %d\n", loopback->isoc_out_packet_size);
				DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "ISOC OUT interval: %d(%d)\n", loopback->isoc_out_interval, ep_desc->bInterval);
			}

			break;

		default:
			break;
		}
	}
}

/**
  * @brief  Bulk test
  * @param  host: Host handle
  * @retval None
*/
static void usbh_loopback_intr_in_process(usb_host_t *host)
{
	usbh_loopback_host_t *loopback = &usbh_loopback_host;
	usbh_urb_state_t urb_state = USBH_URB_IDLE;
	u32 len;

	switch (loopback->intr_in_state) {
	case LOOPBACK_TRANSFER_STATE_RX:
		usbh_intr_receive_data(host,
							   loopback->intr_in_buf,
							   loopback->intr_in_packet_size,
							   loopback->intr_in_pipe);

		loopback->intr_in_state = LOOPBACK_TRANSFER_STATE_RX_BUSY;
		usbh_notify_class_state_change(host);
		break;

	case LOOPBACK_TRANSFER_STATE_RX_BUSY:
		urb_state = usbh_get_urb_state(host, loopback->intr_in_pipe);
		if (urb_state == USBH_URB_DONE) {
			len = usbh_get_last_transfer_size(host, loopback->intr_in_pipe);
			if ((loopback->cb != NULL) && (loopback->cb->intr_received != NULL)) {
				loopback->cb->intr_received(loopback->intr_in_buf, len);
			}

			if ((loopback->intr_in_len > len) && (len >= loopback->intr_in_packet_size)) {
				loopback->intr_in_len -= len ;
				loopback->intr_in_buf += len;
				loopback->intr_in_state = LOOPBACK_TRANSFER_STATE_RX;
			} else {
				DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "INTR IN ok\n");
				loopback->intr_in_state = LOOPBACK_TRANSFER_STATE_IDLE;
			}
		} else if (urb_state == USBH_URB_BUSY) {
			// Keep on waiting
		} else if ((urb_state == USBH_URB_ERROR) || (urb_state == USBH_URB_STALL)) {
			loopback->intr_in_state = LOOPBACK_TRANSFER_STATE_IDLE;
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "INTR in failed %d\n", urb_state);
		} else {
			loopback->intr_in_state = LOOPBACK_TRANSFER_STATE_RX;
		}
		usbh_notify_class_state_change(host);
		break;

	default:
		break;
	}
}

/**
  * @brief  Bulk test
  * @param  host: Host handle
  * @retval None
*/
static void usbh_loopback_intr_out_process(usb_host_t *host)
{
	usbh_loopback_host_t *loopback = &usbh_loopback_host;
	usbh_urb_state_t urb_state = USBH_URB_IDLE;

	switch (loopback->intr_out_state) {
	case LOOPBACK_TRANSFER_STATE_TX:
		if (loopback->intr_out_len > loopback->intr_out_packet_size) {
			usbh_intr_send_data(host,
								loopback->intr_out_buf,
								loopback->intr_out_packet_size,
								loopback->intr_out_pipe);
		} else {
			usbh_intr_send_data(host,
								loopback->intr_out_buf,
								(u16)loopback->intr_out_len,
								loopback->intr_out_pipe);
		}

		loopback->intr_out_state = LOOPBACK_TRANSFER_STATE_TX_BUSY;
		usbh_notify_class_state_change(host);
		break;

	case LOOPBACK_TRANSFER_STATE_TX_BUSY:
		urb_state = usbh_get_urb_state(host, loopback->intr_out_pipe);
		if (urb_state == USBH_URB_DONE) {
			if (loopback->intr_out_len > loopback->intr_out_packet_size) {
				loopback->intr_out_len -= loopback->intr_out_packet_size;
				loopback->intr_out_buf += loopback->intr_out_packet_size;
			} else {
				loopback->intr_out_len = 0U;
			}

			if (loopback->intr_out_len > 0U) {
				loopback->intr_out_state = LOOPBACK_TRANSFER_STATE_TX;
			} else {
				DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "INTR out ok\n");
				loopback->intr_out_state = LOOPBACK_TRANSFER_STATE_IDLE;
				if ((loopback->cb != NULL) && (loopback->cb->intr_transmitted != NULL)) {
					loopback->cb->intr_transmitted(urb_state);
				}
			}
		} else if (urb_state == USBH_URB_ERROR) {
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "INTR out failed\n");
			loopback->intr_out_state = LOOPBACK_TRANSFER_STATE_IDLE;
			if ((loopback->cb != NULL) && (loopback->cb->intr_transmitted != NULL)) {
				loopback->cb->intr_transmitted(urb_state);
			}
		}
		usbh_notify_class_state_change(host);
		break;
	default:
		break;
	}
}

/**
  * @brief  Iso in test
  * @param  host: Host handle
  * @retval None
*/
static void usbh_loopback_isoc_in_process(usb_host_t *host)
{
	usbh_loopback_host_t *loopback = &usbh_loopback_host;
	usbh_urb_state_t urb_state = USBH_URB_IDLE;
	u32 len;
	u32 ticks;

	switch (loopback->isoc_in_state) {
	case LOOPBACK_TRANSFER_STATE_RX:
		ticks = host->tick;
		if ((ticks - loopback->isoc_in_tick) >= loopback->isoc_in_interval) {
			loopback->isoc_in_tick = ticks;
			usbh_isoc_receive_data(host,
								   loopback->isoc_in_buf,
								   loopback->isoc_in_packet_size,
								   loopback->isoc_in_pipe);

			loopback->isoc_in_state = LOOPBACK_TRANSFER_STATE_RX_BUSY;
		}
		usbh_notify_class_state_change(host);
		break;

	case LOOPBACK_TRANSFER_STATE_RX_BUSY:
		urb_state = usbh_get_urb_state(host, loopback->isoc_in_pipe);
		if (urb_state == USBH_URB_DONE) {
			len = usbh_get_last_transfer_size(host, loopback->isoc_in_pipe);
			if ((loopback->cb != NULL) && (loopback->cb->isoc_received != NULL)) {
				loopback->cb->isoc_received(loopback->isoc_in_buf, len);
			}
			loopback->isoc_in_state = LOOPBACK_TRANSFER_STATE_IDLE;
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "ISOC IN ok\n");
		} else if (urb_state == USBH_URB_ERROR) {
			loopback->isoc_in_state = LOOPBACK_TRANSFER_STATE_RX;
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "ISOC IN failed\n");
		}
		usbh_notify_class_state_change(host);
		break;

	default:
		break;
	}
}

/**
  * @brief  Iso out test
  * @param  host: Host handle
  * @retval None
*/
static void usbh_loopback_isoc_out_process(usb_host_t *host)
{
	usbh_loopback_host_t *loopback = &usbh_loopback_host;
	usbh_urb_state_t urb_state = USBH_URB_IDLE;
	u32 ticks;

	switch (loopback->isoc_out_state) {
	case LOOPBACK_TRANSFER_STATE_TX:
		ticks = host->tick;
		if ((ticks - loopback->isoc_out_tick) >= loopback->isoc_out_interval) {
			loopback->isoc_out_tick = ticks;
			if (loopback->isoc_out_len > loopback->isoc_out_packet_size) {
				usbh_isoc_send_data(host,
									loopback->isoc_out_buf,
									loopback->isoc_out_packet_size,
									loopback->isoc_out_pipe);
			} else {
				usbh_isoc_send_data(host,
									loopback->isoc_out_buf,
									(u16)loopback->isoc_out_len,
									loopback->isoc_out_pipe);
			}

			loopback->isoc_out_state = LOOPBACK_TRANSFER_STATE_TX_BUSY;
		}
		usbh_notify_class_state_change(host);
		break;

	case LOOPBACK_TRANSFER_STATE_TX_BUSY:
		urb_state = usbh_get_urb_state(host, loopback->isoc_out_pipe);
		if (urb_state == USBH_URB_DONE) {
			if (loopback->isoc_out_len > loopback->isoc_out_packet_size) {
				loopback->isoc_out_len -= loopback->isoc_out_packet_size;
				loopback->isoc_out_buf += loopback->isoc_out_packet_size;
			} else {
				loopback->isoc_out_len = 0U;
			}

			if (loopback->isoc_out_len > 0U) {
				loopback->isoc_out_state = LOOPBACK_TRANSFER_STATE_TX;
			} else {
				DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "ISOC OUT ok\n");
				loopback->isoc_out_state = LOOPBACK_TRANSFER_STATE_IDLE;
				if ((loopback->cb != NULL) && (loopback->cb->isoc_transmitted != NULL)) {
					loopback->cb->isoc_transmitted(urb_state);
				}
			}
		} else if (urb_state == USBH_URB_ERROR) {
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "ISOC OUT failed\n");
			loopback->isoc_out_state = LOOPBACK_TRANSFER_STATE_IDLE;
			if ((loopback->cb != NULL) && (loopback->cb->isoc_transmitted != NULL)) {
				loopback->cb->isoc_transmitted(urb_state);
			}
		}
		usbh_notify_class_state_change(host);
		break;

	default:
		break;
	}
}

/**
  * @brief  Attach callback.
  * @param  host: Host handle
  * @retval Status
  */
static u8 usbh_loopback_cb_attach(usb_host_t *host)
{
	u8 status = HAL_ERR_UNKNOWN;
	u8 pipe_num;
	u8 interface;
	usbh_loopback_host_t *loopback = &usbh_loopback_host;
	usbh_if_desc_t *loopback_if_desc;

	loopback->state = LOOPBACK_STATE_IDLE;
	loopback->intr_in_state = LOOPBACK_TRANSFER_STATE_IDLE;
	loopback->intr_out_state = LOOPBACK_TRANSFER_STATE_IDLE;
	loopback->isoc_in_state = LOOPBACK_TRANSFER_STATE_IDLE;
	loopback->isoc_out_state = LOOPBACK_TRANSFER_STATE_IDLE;

	/* Get interface index */
	interface = usbh_get_interface(host, LOOPBACK_CLASS_CODE, LOOPBACK_SUBCLASS_CODE, LOOPBACK_PROTOCOL);
	if (interface == 0xFFU) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Fail to find the interface for Vendor Class\n");
		goto attach_exit;
	}

	loopback->host = host;
	usbh_set_interface(host, interface);

	loopback_if_desc = usbh_get_interface_descriptor(host, interface);
	if (loopback_if_desc == NULL) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Fail to get the interface descriptor for Vendor Class\n");
		goto attach_exit;
	}

	/* Get data in/out endpoints */
	usbh_loopback_get_endpoints(loopback_if_desc);

	pipe_num = usbh_alloc_pipe(host, loopback->intr_out_ep);
	if (pipe_num != 0xFFU) {
		loopback->intr_out_pipe = pipe_num;
	} else {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Fail to allocate INTR OUT pipe for LOOPBACK class\n");
		status = HAL_ERR_MEM;
		goto attach_exit;
	}

	pipe_num = usbh_alloc_pipe(host, loopback->intr_in_ep);
	if (pipe_num != 0xFFU) {
		loopback->intr_in_pipe = pipe_num;
	} else {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Fail to allocate INTR IN pipe for LOOPBACK class\n");
		status = HAL_ERR_MEM;
		goto attach_free_intr_out_ep_exit;
	}

	usbh_open_pipe(host, loopback->intr_out_pipe, loopback->intr_out_ep, USB_CH_EP_TYPE_INTR, loopback->intr_out_packet_size);
	usbh_open_pipe(host, loopback->intr_in_pipe, loopback->intr_in_ep, USB_CH_EP_TYPE_INTR, loopback->intr_in_packet_size);

	pipe_num = usbh_alloc_pipe(host, loopback->isoc_out_ep);
	if (pipe_num != 0xFFU) {
		loopback->isoc_out_pipe = pipe_num;
	} else {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Fail to allocate ISO OUT pipe for LOOPBACK class\n");
		status = HAL_ERR_MEM;
		goto attach_free_intr_in_ep_exit;
	}

	pipe_num = usbh_alloc_pipe(host, loopback->isoc_in_ep);
	if (pipe_num != 0xFFU) {
		loopback->isoc_in_pipe = pipe_num;
	} else {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Fail to allocate ISO IN pipe for LOOPBACK class\n");
		status = HAL_ERR_MEM;
		goto attach_free_isoc_out_ep_exit;
	}

	usbh_open_pipe(host, loopback->isoc_in_pipe, loopback->isoc_in_ep, USB_CH_EP_TYPE_ISOC, loopback->isoc_in_packet_size);
	usbh_open_pipe(host, loopback->isoc_out_pipe, loopback->isoc_out_ep, USB_CH_EP_TYPE_ISOC, loopback->isoc_out_packet_size);

	DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "INTR IN CH: %d\n", loopback->intr_in_pipe);
	DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "INTR OUT CH: %d\n", loopback->intr_out_pipe);
	DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "ISOC IN CH: %d\n", loopback->isoc_in_pipe);
	DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "ISOC OUT CH: %d\n", loopback->isoc_out_pipe);

	if ((loopback->cb != NULL) && (loopback->cb->attach != NULL)) {
		loopback->cb->attach();
	}

	return HAL_OK;

attach_free_isoc_out_ep_exit:
	usbh_free_pipe(host, loopback->isoc_out_pipe);
attach_free_intr_in_ep_exit:
	usbh_free_pipe(host, loopback->intr_in_pipe);
attach_free_intr_out_ep_exit:
	usbh_free_pipe(host, loopback->intr_out_pipe);

attach_exit:

	return status;
}

/**
  * @brief  Detach callback.
  * @param  host: Host handle
  * @retval Status
  */
static u8 usbh_loopback_cb_detach(usb_host_t *host)
{
	usbh_loopback_host_t *loopback = &usbh_loopback_host;

	if ((loopback->cb != NULL) && (loopback->cb->detach != NULL)) {
		loopback->cb->detach();
	}

	if (loopback->intr_in_pipe) {
		usbh_close_pipe(host, loopback->intr_in_pipe);
		usbh_free_pipe(host, loopback->intr_in_pipe);
		loopback->intr_in_pipe = 0U;
	}

	if (loopback->intr_out_pipe) {
		usbh_close_pipe(host, loopback->intr_out_pipe);
		usbh_free_pipe(host, loopback->intr_out_pipe);
		loopback->intr_out_pipe = 0U;
	}

	if (loopback->isoc_in_pipe) {
		usbh_close_pipe(host, loopback->isoc_in_pipe);
		usbh_free_pipe(host, loopback->isoc_in_pipe);
		loopback->isoc_out_pipe = 0U;
	}

	if (loopback->isoc_out_pipe) {
		usbh_close_pipe(host, loopback->isoc_out_pipe);
		usbh_free_pipe(host, loopback->isoc_out_pipe);
		loopback->isoc_out_pipe = 0U;
	}

	loopback->state = LOOPBACK_STATE_IDLE;
	loopback->intr_in_state = LOOPBACK_TRANSFER_STATE_IDLE;
	loopback->intr_out_state = LOOPBACK_TRANSFER_STATE_IDLE;
	loopback->isoc_in_state = LOOPBACK_TRANSFER_STATE_IDLE;
	loopback->isoc_out_state = LOOPBACK_TRANSFER_STATE_IDLE;

	return HAL_OK;
}

/**
  * @brief  Standard control requests handling callback
  * @param  host: Host handle
  * @retval Status
  */
static u8 usbh_loopback_cb_setup(usb_host_t *host)
{
	usbh_loopback_host_t *loopback = &usbh_loopback_host;

	UNUSED(host);

	if ((loopback->cb != NULL) && (loopback->cb->setup != NULL)) {
		loopback->cb->setup();
	}

	return HAL_OK;
}

static u8 usbh_loopback_process_setup(usb_host_t *host)
{
	usbh_setup_req_t setup;

	setup.b.bmRequestType = USB_H2D | USB_REQ_TYPE_CLASS | USB_REQ_RECIPIENT_INTERFACE;
	setup.b.bRequest = USBH_LOOPBACK_START_TEST;
	setup.b.wValue = 1U;
	setup.b.wIndex = 0U;
	setup.b.wLength = 0U;

	return usbh_ctrl_request(host, &setup, NULL);
}

/**
  * @brief  State machine handling callback
  * @param  host: Host handle
  * @retval Status
  */
static u8 usbh_loopback_cb_process(usb_host_t *host)
{
	u8 ret = HAL_OK;
	usbh_loopback_host_t *loopback = &usbh_loopback_host;
	switch (loopback->state) {
	case LOOPBACK_STATE_SETUP:
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "Loopback SETUP\n");
		ret = usbh_loopback_process_setup(host);
		if (ret == HAL_OK) {
			loopback->state = LOOPBACK_STATE_TRANSFER;
		} else if (ret != HAL_BUSY) {
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "Loopback process error\n");
			loopback->state = LOOPBACK_STATE_ERROR;
		}
		break;
	case LOOPBACK_STATE_TRANSFER:
		usbh_loopback_intr_in_process(host);
		usbh_loopback_intr_out_process(host);
		usbh_loopback_isoc_out_process(host);
		usbh_loopback_isoc_in_process(host);
		break;
	case LOOPBACK_STATE_ERROR:
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "Loopback ERROR\n");
		ret = usbh_ctrl_clear_feature(host, 0x00U);
		if (ret == HAL_OK) {
			loopback->state = LOOPBACK_STATE_IDLE;
		}
		break;
	default:
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "Loopback IDLE\n");
		break;
	}
	return ret;
}

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Init loopback class
  * @param  cb: User callback
  * @retval Status
  */
u8 usbh_loopback_init(usbh_loopback_cb_t *cb)
{
	u8 ret;
	usbh_loopback_host_t *loopback = &usbh_loopback_host;

	if (cb != NULL) {
		loopback->cb = cb;
		if (cb->init != NULL) {
			ret = cb->init();
			if (ret != HAL_OK) {
				DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "USBH loopbach user init fail\n");
				return ret;
			}
		}
	}

	usbh_register_class(&usbh_loopback_driver);

	return HAL_OK;
}

/**
  * @brief  Deinit loopback class
  * @retval Status
  */
u8 usbh_loopback_deinit(void)
{
	usbh_loopback_host_t *loopback = &usbh_loopback_host;

	if ((loopback->cb != NULL) && (loopback->cb->deinit != NULL)) {
		loopback->cb->deinit();
	}

	usbh_unregister_class(&usbh_loopback_driver);

	return HAL_OK;
}

u8 usbh_loopback_setup(void)
{
	usbh_loopback_host_t *loopback = &usbh_loopback_host;
	usb_host_t *host = loopback->host;
	int ret = HAL_ERR_UNKNOWN;

	if ((host->state == USBH_CLASS_REQUEST) || (host->state == USBH_CLASS_READY)) {
		loopback->state = LOOPBACK_STATE_SETUP;
		usbh_notify_class_state_change(host);
		ret = HAL_OK;
	}

	return ret;
}

/**
  * @brief  Start to transfer INTR data
  * @param  buf: Data buffer
  * @param  len: Data length
  * @retval Status
  */
u8 usbh_loopback_intr_transmit(u8 *buf, u32 len)
{
	u8 ret = HAL_BUSY;
	usbh_loopback_host_t *loopback = &usbh_loopback_host;
	usb_host_t *host = loopback->host;

	if ((loopback->state == LOOPBACK_STATE_IDLE) || (loopback->state == LOOPBACK_STATE_TRANSFER)) {
		if (loopback->intr_out_state == LOOPBACK_TRANSFER_STATE_IDLE) {
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "INTR OUT %d\n", len);
			loopback->intr_out_buf = buf;
			loopback->intr_out_len = len;
			loopback->intr_out_state = LOOPBACK_TRANSFER_STATE_TX;
			loopback->state = LOOPBACK_STATE_TRANSFER;
			usbh_notify_class_state_change(host);
			ret = HAL_OK;
		} else {
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "INTR OUT %d: busy\n", len);
		}
	} else {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "INTR OUT not ready\n");
	}

	return ret;
}

/**
  * @brief  Start to transfer ISOC data
  * @param  buf: Data buffer
  * @param  len: Data length
  * @retval Status
  */
u8 usbh_loopback_isoc_transmit(u8 *buf, u32 len)
{
	u8 ret = HAL_BUSY;
	usbh_loopback_host_t *loopback = &usbh_loopback_host;
	usb_host_t *host = loopback->host;

	if ((loopback->state == LOOPBACK_STATE_IDLE) || (loopback->state == LOOPBACK_STATE_TRANSFER)) {
		if (loopback->isoc_out_state == LOOPBACK_TRANSFER_STATE_IDLE) {
			loopback->isoc_out_buf = buf;
			loopback->isoc_out_len = len;
			loopback->isoc_out_state = LOOPBACK_TRANSFER_STATE_TX;
			loopback->state = LOOPBACK_STATE_TRANSFER;
			usbh_notify_class_state_change(host);
			ret = HAL_OK;
		} else {
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "ISOC OUT busy\n");
		}
	} else {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "ISOC OUT not ready\n");
	}

	return ret;
}

/**
  * @brief  Start to receive INTR data
  * @param  buf: Data buffer
  * @param  len: Data length
  * @retval Status
  */
u8 usbh_loopback_intr_receive(u8 *buf, u32 len)
{
	u8 ret = HAL_BUSY;
	usbh_loopback_host_t *loopback = &usbh_loopback_host;
	usb_host_t *host = loopback->host;

	if ((loopback->state == LOOPBACK_STATE_IDLE) || (loopback->state == LOOPBACK_STATE_TRANSFER)) {
		if (loopback->intr_in_state == LOOPBACK_TRANSFER_STATE_IDLE) {
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "INTR IN %d\n", len);
			loopback->intr_in_buf = buf;
			loopback->intr_in_len = len;
			loopback->intr_in_state = LOOPBACK_TRANSFER_STATE_RX;
			loopback->state = LOOPBACK_STATE_TRANSFER;
			usbh_notify_class_state_change(host);
			ret = HAL_OK;
		} else {
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "INTR IN %d: busy\n", len);
		}
	} else {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "INTR IN not ready\n");
	}

	return ret;
}

/**
  * @brief  Start to receive ISOC data
  * @param  buf: Data buffer
  * @param  len: Data length
  * @retval Status
  */
u8 usbh_loopback_isoc_receive(u8 *buf, u32 len)
{
	u8 ret = HAL_BUSY;
	usbh_loopback_host_t *loopback = &usbh_loopback_host;
	usb_host_t *host = loopback->host;

	if ((loopback->state == LOOPBACK_STATE_IDLE) || (loopback->state == LOOPBACK_STATE_TRANSFER)) {
		if (loopback->isoc_in_state == LOOPBACK_TRANSFER_STATE_IDLE) {
			loopback->isoc_in_buf = buf;
			loopback->isoc_in_len = loopback->isoc_in_packet_size;
			loopback->isoc_in_state = LOOPBACK_TRANSFER_STATE_RX;
			loopback->state = LOOPBACK_STATE_TRANSFER;
			usbh_notify_class_state_change(host);
			ret = HAL_OK;
		} else {
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "ISOC IN busy\n");
			ret = HAL_BUSY;
		}
	} else {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "ISOC IN not ready\n");
		ret = HAL_BUSY;
	}

	return ret;
}

