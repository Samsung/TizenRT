/**
  ******************************************************************************
  * @file    usbh_vendor.c
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

#include "usbh_vendor.h"
#include "usbh.h"

/* Private defines -----------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

static u8 usbh_vendor_attach(usb_host_t *host);
static u8 usbh_vendor_detach(usb_host_t *host);
static u8 usbh_vendor_process(usb_host_t *host);
static u8 usbh_vendor_setup(usb_host_t *host);
/* Private variables ---------------------------------------------------------*/

/* USB Standard Device Descriptor */
static usbh_class_driver_t usbh_vendor_driver = {
	.class_code = VENDOR_CLASS_CODE,
	.attach = usbh_vendor_attach,
	.detach = usbh_vendor_detach,
	.setup = usbh_vendor_setup,
	.process = usbh_vendor_process,
};

static usbh_vendor_host_t usbh_vendor_host;
#if CONFIG_USBH_VENDOR_ISO_IN_TEST
static u32 vendor_sum_iso_in[USBH_VENDOR_ISO_IN_CNT];
#endif
volatile int vendor_iso_in_cnt = 0;
volatile int vendor_iso_out_cnt = 0;


/* Private functions ---------------------------------------------------------*/

static void usbh_vendor_get_endpoints(usbh_if_desc_t *intf)
{
	u32 tmp = 0;
	usbh_vendor_host_t *vendor = &usbh_vendor_host;
	usbh_ep_desc_t *ep_desc;

	for (tmp = 0; tmp < intf->bNumEndpoints; tmp++) {
		ep_desc = &intf->ep_desc[tmp];
		switch (ep_desc->bmAttributes & USB_EP_XFERTYPE_MASK) {
		case USB_CH_EP_TYPE_BULK:
			if ((ep_desc->bEndpointAddress & USB_REQ_DIR_MASK) == USB_D2H) {
				vendor->bulk_in_ep = ep_desc->bEndpointAddress;
				vendor->bulk_in_packet_size  = ep_desc->wMaxPacketSize;
			} else {
				vendor->bulk_out_ep = ep_desc->bEndpointAddress;
				vendor->bulk_out_packet_size  = ep_desc->wMaxPacketSize;
			}

			break;

		case USB_CH_EP_TYPE_ISOC:
			if ((ep_desc->bEndpointAddress & USB_REQ_DIR_MASK) == USB_D2H) {
				vendor->iso_in_ep = ep_desc->bEndpointAddress;
				vendor->iso_in_packet_size  = ep_desc->wMaxPacketSize;
			} else {
				vendor->iso_out_ep = ep_desc->bEndpointAddress;
				vendor->iso_out_packet_size  = ep_desc->wMaxPacketSize;
			}

			break;

		default:
			break;
		}
	}
}

#if !CONFIG_USBH_VENDOR_ISO_OUT_TEST
static void usbh_vendor_dump_buf(u8 *pbuf, u32 len, char *name)
{
	u32 i;

	DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "%s\n", name);

	for (i = 0; i < len; i++) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "buf[%d] = %02x\n", i, pbuf[i]);
	}
}
#endif

#if CONFIG_USBH_VENDOR_CTRL_TEST
/**
  * @brief  Contrl test
  * @param  host: Host handle
  * @retval None
  */
static void usbh_vendor_ctrl_process(usb_host_t *host)
{
	usbh_setup_req_t setup;
	u8 ret = HAL_BUSY;
	usbh_vendor_host_t *vendor = &usbh_vendor_host;

	switch (vendor->state) {
	case VENDOR_STATE_SEND_REQ:
		setup.b.bmRequestType = USB_D2H | USB_REQ_TYPE_STANDARD | USB_REQ_RECIPIENT_DEVICE;
		setup.b.bRequest = USB_REQ_GET_DESCRIPTOR;
		setup.b.wValue = 0x0100;
		setup.b.wIndex = 0U;
		setup.b.wLength = 0x12;

		ret = usbh_ctrl_request(host, &setup, vendor->ctrl_buf);
		if (ret == HAL_OK) {
			vendor->state = VENDOR_STATE_IDLE;
			usbh_vendor_dump_buf(vendor->ctrl_buf, 18, "Device descriptor:");
		} else if (ret != HAL_BUSY) {
			vendor->state = VENDOR_STATE_ERROR;
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Fail to send control request, ret: %d\n", ret);
		}
		break;

	case VENDOR_STATE_ERROR:
		ret = usbh_ctrl_clear_feature(host, 0x00U);
		if (ret == HAL_OK) {
			vendor->state = VENDOR_STATE_IDLE;
		}
		break;

	default:
		break;
	}
}
#endif

#if CONFIG_USBH_VENDOR_BULK_TEST
/**
  * @brief  Bulk test
  * @param  host: Host handle
  * @retval None
*/
static void usbh_vendor_bulk_process(usb_host_t *host)
{
	usbh_vendor_host_t *vendor = &usbh_vendor_host;
	usbh_urb_state_t urb_state = USBH_URB_IDLE;
	u32 len;

	switch (vendor->state) {
	case VENDOR_STATE_TX:
		if (vendor->bulk_out_len > vendor->bulk_out_packet_size) {
			usbh_bulk_send_data(host,
								vendor->bulk_out_buf,
								vendor->bulk_out_packet_size,
								vendor->bulk_out_pipe);
		} else {
			usbh_bulk_send_data(host,
								vendor->bulk_out_buf,
								(u16)vendor->bulk_out_len,
								vendor->bulk_out_pipe);
		}

		vendor->state = VENDOR_STATE_TX_BUSY;
		break;

	case VENDOR_STATE_TX_BUSY:
		urb_state = usbh_get_urb_state(host, vendor->bulk_out_pipe);
		if (urb_state == USBH_URB_DONE) {
			if (vendor->bulk_out_len > vendor->bulk_out_packet_size) {
				vendor->bulk_out_len -= vendor->bulk_out_packet_size;
				vendor->bulk_out_buf += vendor->bulk_out_packet_size;
			} else {
				vendor->bulk_out_len = 0U;
			}

			if (vendor->bulk_out_len > 0U) {
				vendor->state = VENDOR_STATE_TX;
			} else {
				vendor->state = VENDOR_STATE_RX;
			}
			usbh_notify_class_state_change(host);
		} else if (urb_state == USBH_URB_BUSY) {
			vendor->state = VENDOR_STATE_TX;
			usbh_notify_class_state_change(host);
		} else if (urb_state == USBH_URB_ERROR) {
			vendor->state = VENDOR_STATE_IDLE;
		}
		break;

	case VENDOR_STATE_RX:
		usbh_bulk_receive_data(host,
							   vendor->bulk_in_buf,
							   vendor->bulk_in_packet_size,
							   vendor->bulk_in_pipe);

		vendor->state = VENDOR_STATE_RX_BUSY;
		break;

	case VENDOR_STATE_RX_BUSY:
		urb_state = usbh_get_urb_state(host, vendor->bulk_in_pipe);
		if (urb_state == USBH_URB_DONE) {
			len = usbh_get_last_transfer_size(host, vendor->bulk_in_pipe);
			if (((vendor->bulk_in_len - len) > 0U) && (len >= vendor->bulk_in_packet_size)) {
				vendor->bulk_in_len -= len ;
				vendor->bulk_in_buf += len;
				vendor->state = VENDOR_STATE_RX;
			} else {
				vendor->state = VENDOR_STATE_IDLE;
				usbh_vendor_dump_buf(vendor->bulk_in_buf, USBH_VENDOR_IN_BUF_SIZE, "Bulk in data:");
			}
			usbh_notify_class_state_change(host);
		}
		break;

	default:
		break;
	}
}
#endif

#if CONFIG_USBH_VENDOR_ISO_IN_TEST
/**
  * @brief  Iso in test
  * @param  host: Host handle
  * @retval None
*/
static void usbh_vendor_iso_in_process(usb_host_t *host)
{
	usbh_vendor_host_t *vendor = &usbh_vendor_host;
	usbh_urb_state_t urb_state = USBH_URB_IDLE;
	u32 len;
	u32 i;

	switch (vendor->state) {
	case VENDOR_STATE_RX:
		usbh_isoc_receive_data(host,
							   vendor->iso_in_buf,
							   vendor->iso_in_packet_size,
							   vendor->iso_in_pipe);

		vendor->state = VENDOR_STATE_RX_BUSY;
		break;

	case VENDOR_STATE_RX_BUSY:
		urb_state = usbh_get_urb_state(host, vendor->iso_in_pipe);
		if (urb_state == USBH_URB_DONE) {
			len = usbh_get_last_transfer_size(host, vendor->iso_in_pipe);
			if (((vendor->iso_in_len - len) > 0U) && (len >= vendor->iso_in_packet_size)) {
				vendor->iso_in_len -= len ;
				vendor->iso_in_buf += len;
				vendor->state = VENDOR_STATE_RX;
			} else {
				for (i = 0; i < USBH_VENDOR_IN_BUF_SIZE; i++) {
					vendor_sum_iso_in[vendor_iso_in_cnt] += vendor->iso_in_buf[i];
				}
				vendor_sum_iso_in[vendor_iso_in_cnt] /= USBH_VENDOR_IN_BUF_SIZE;
				vendor_iso_in_cnt ++;
				if (vendor_iso_in_cnt == USBH_VENDOR_ISO_IN_CNT) {
					vendor->state = VENDOR_STATE_IDLE;
					usbh_vendor_dump_buf((u8 *)vendor_sum_iso_in, USBH_VENDOR_ISO_IN_CNT, "Iso in data:");
					vendor_iso_in_cnt = 0;
				} else {
					vendor->iso_in_len = USBH_VENDOR_IN_BUF_SIZE;
					vendor->iso_in_buf = vendor->iso_in_buf_save;
					vendor->state = VENDOR_STATE_RX;
				}
			}
			usbh_notify_class_state_change(host);
		}
		break;

	default:
		break;
	}
}
#endif

#if CONFIG_USBH_VENDOR_ISO_OUT_TEST
/**
  * @brief  Iso out test
  * @param  host: Host handle
  * @retval None
*/
static void usbh_vendor_iso_out_process(usb_host_t *host)
{
	usbh_vendor_host_t *vendor = &usbh_vendor_host;
	usbh_urb_state_t urb_state = USBH_URB_IDLE;

	switch (vendor->state) {
	case VENDOR_STATE_TX:
		if (vendor->iso_out_len > vendor->iso_out_packet_size) {
			usbh_isoc_send_data(host,
								vendor->iso_out_buf,
								vendor->iso_out_packet_size,
								vendor->iso_out_pipe);
		} else {
			usbh_isoc_send_data(host,
								vendor->iso_out_buf,
								(u16)vendor->iso_out_len,
								vendor->iso_out_pipe);
		}

		vendor->state = VENDOR_STATE_TX_BUSY;
		break;

	case VENDOR_STATE_TX_BUSY:
		urb_state = usbh_get_urb_state(host, vendor->iso_out_pipe);
		if (urb_state == USBH_URB_DONE) {
			if (vendor->iso_out_len > vendor->iso_out_packet_size) {
				vendor->iso_out_len -= vendor->iso_out_packet_size;
				vendor->iso_out_buf += vendor->iso_out_packet_size;
			} else {
				vendor->iso_out_len = 0U;
			}

			if (vendor->iso_out_len > 0U) {
				vendor->state = VENDOR_STATE_TX;
			} else {
				vendor_iso_out_cnt ++;
				if (vendor_iso_out_cnt == USBH_VENDOR_ISO_OUT_CNT) {
					vendor->state = VENDOR_STATE_IDLE;
				} else {
					vendor->iso_out_len = USBH_VENDOR_OUT_BUF_SIZE;
					vendor->iso_out_buf = vendor->iso_out_buf_save;
					rtw_memset((void *) vendor->iso_out_buf, (vendor_iso_out_cnt & 0xFF), vendor->iso_out_len);
					vendor->state = VENDOR_STATE_TX;
				}
			}
			usbh_notify_class_state_change(host);
		} else if (urb_state == USBH_URB_BUSY) {
			vendor->state = VENDOR_STATE_TX;
			usbh_notify_class_state_change(host);
		} else if (urb_state == USBH_URB_ERROR) {
			vendor->state = VENDOR_STATE_IDLE;
		}
		break;

	default:
		break;
	}
}
#endif

/**
  * @brief  Attach callback.
  * @param  host: Host handle
  * @retval Status
  */
static u8 usbh_vendor_attach(usb_host_t *host)
{
	u8 status = HAL_ERR_UNKNOWN;
	u8 pipe_num;
	u8 interface;
	usbh_vendor_host_t *vendor = &usbh_vendor_host;
	usbh_if_desc_t *vendor_if_desc;

	/* Get interface index */
	interface = usbh_get_interface(host, VENDOR_CLASS_CODE, VENDOR_SUBCLASS_CODE, VENDOR_PROTOCOL);
	if (interface == 0xFFU) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "Fail to find the interface for Vendor Class.");
		return status;
	}

	vendor->host = host;
	usbh_set_interface(host, interface);

	vendor_if_desc = usbh_get_interface_descriptor(host, interface);
	if (vendor_if_desc == NULL) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Fail to get the interface descriptor for Vendor Class.");
		return status;
	}

	/* Get data in/out endpoints */
	usbh_vendor_get_endpoints(vendor_if_desc);

#if CONFIG_USBH_VENDOR_CTRL_TEST
	UNUSED(pipe_num);
	vendor->state = VENDOR_STATE_SEND_REQ;
#elif CONFIG_USBH_VENDOR_BULK_TEST
	pipe_num = usbh_alloc_pipe(host, vendor->bulk_out_ep);
	if (pipe_num != 0xFFU) {
		vendor->bulk_out_pipe = pipe_num;
	} else {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Fail to allocate BULK OUT pipe for VENDOR class.");
		return HAL_ERR_MEM;
	}

	pipe_num = usbh_alloc_pipe(host, vendor->bulk_in_ep);
	if (pipe_num != 0xFFU) {
		vendor->bulk_in_pipe = pipe_num;
	} else {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Fail to allocate BULK IN pipe for VENDOR class.");
		usbh_free_pipe(host, vendor->bulk_out_pipe);
		return HAL_ERR_MEM;
	}

	usbh_open_pipe(host, vendor->bulk_out_pipe, vendor->bulk_out_ep, USB_CH_EP_TYPE_BULK, vendor->bulk_out_packet_size);
	usbh_open_pipe(host, vendor->bulk_in_pipe, vendor->bulk_in_ep, USB_CH_EP_TYPE_BULK, vendor->bulk_in_packet_size);

	vendor->bulk_out_buf = rtw_zmalloc(USBH_VENDOR_OUT_BUF_SIZE);
	if (vendor->bulk_out_buf == NULL) {
		usbh_free_pipe(host, vendor->bulk_out_pipe);
		usbh_free_pipe(host, vendor->bulk_in_ep);
		return HAL_ERR_MEM;
	}
	vendor->bulk_out_len = USBH_VENDOR_OUT_BUF_SIZE;
	rtw_memset((void *) vendor->bulk_out_buf, 0xA5, vendor->bulk_out_len);

	vendor->bulk_in_buf = rtw_zmalloc(USBH_VENDOR_IN_BUF_SIZE);
	if (vendor->bulk_in_buf == NULL) {
		usbh_free_pipe(host, vendor->bulk_out_pipe);
		usbh_free_pipe(host, vendor->bulk_in_ep);
		rtw_free(vendor->bulk_out_buf);
		vendor->bulk_out_buf = NULL;
		return HAL_ERR_MEM;
	}
	vendor->bulk_in_len = USBH_VENDOR_IN_BUF_SIZE;
	vendor->state = VENDOR_STATE_TX;
#elif CONFIG_USBH_VENDOR_ISO_IN_TEST
	pipe_num = usbh_alloc_pipe(host, vendor->iso_in_ep);
	if (pipe_num != 0xFFU) {
		vendor->iso_in_pipe = pipe_num;
	} else {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Fail to allocate ISO IN pipe for VENDOR class.");
		return HAL_ERR_MEM;
	}

	usbh_open_pipe(host, vendor->iso_in_pipe, vendor->iso_in_ep, USB_CH_EP_TYPE_ISOC, vendor->iso_in_packet_size);

	vendor->iso_in_buf = rtw_zmalloc(USBH_VENDOR_IN_BUF_SIZE);
	if (vendor->iso_in_buf == NULL) {
		usbh_free_pipe(host, vendor->iso_in_ep);
		return HAL_ERR_MEM;
	}
	vendor->iso_in_buf_save = vendor->iso_in_buf;
	vendor->iso_in_len = USBH_VENDOR_IN_BUF_SIZE;
	vendor->state = VENDOR_STATE_RX;
	vendor_iso_in_cnt = 0;
	rtw_memset((void *)vendor_sum_iso_in, 0, USBH_VENDOR_ISO_IN_CNT * 4);
#elif CONFIG_USBH_VENDOR_ISO_OUT_TEST
	pipe_num = usbh_alloc_pipe(host, vendor->iso_out_ep);
	if (pipe_num != 0xFFU) {
		vendor->iso_out_pipe = pipe_num;
	} else {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Fail to allocate ISO OUT pipe for VENDOR class.");
		return HAL_ERR_MEM;
	}

	usbh_open_pipe(host, vendor->iso_out_pipe, vendor->iso_out_ep, USB_CH_EP_TYPE_ISOC, vendor->iso_out_packet_size);
	// add delay to fix first packet loss when test with amebad usbd_vendor_new
	rtw_mdelay_os(10);

	vendor->iso_out_buf = rtw_zmalloc(USBH_VENDOR_OUT_BUF_SIZE);
	if (vendor->iso_out_buf == NULL) {
		usbh_free_pipe(host, vendor->iso_out_ep);
		return HAL_ERR_MEM;
	}
	vendor->iso_out_buf_save = vendor->iso_out_buf;
	vendor->iso_out_len = USBH_VENDOR_OUT_BUF_SIZE;
	vendor_iso_out_cnt = 0;
	rtw_memset((void *) vendor->iso_out_buf, (vendor_iso_out_cnt & 0xFF), vendor->iso_out_len);
	vendor->state = VENDOR_STATE_TX;
#endif

	status = HAL_OK;

	return status;
}

/**
  * @brief  Detach callback.
  * @param  host: Host handle
  * @retval Status
  */
static u8 usbh_vendor_detach(usb_host_t *host)
{
	usbh_vendor_host_t *vendor = &usbh_vendor_host;

	if ((vendor->cb != NULL) && (vendor->cb->detach != NULL)) {
		vendor->cb->detach();
	}

	if (vendor->bulk_in_pipe) {
		usbh_close_pipe(host, vendor->bulk_in_pipe);
		usbh_free_pipe(host, vendor->bulk_in_pipe);
		vendor->bulk_in_pipe = 0U;
	}

	if (vendor->bulk_out_pipe) {
		usbh_close_pipe(host, vendor->bulk_out_pipe);
		usbh_free_pipe(host, vendor->bulk_out_pipe);
		vendor->bulk_out_pipe = 0U;
	}

	if (vendor->iso_in_pipe) {
		usbh_close_pipe(host, vendor->iso_in_pipe);
		usbh_free_pipe(host, vendor->iso_in_pipe);
		vendor->iso_out_pipe = 0U;
	}

	if (vendor->iso_out_pipe) {
		usbh_close_pipe(host, vendor->iso_out_pipe);
		usbh_free_pipe(host, vendor->iso_out_pipe);
		vendor->iso_out_pipe = 0U;
	}

	if (vendor->bulk_in_buf != NULL) {
		rtw_free(vendor->bulk_in_buf);
		vendor->bulk_in_buf = NULL;
	}

	if (vendor->bulk_out_buf != NULL) {
		rtw_free(vendor->bulk_out_buf);
		vendor->bulk_out_buf = NULL;
	}

	if (vendor->iso_in_buf != NULL) {
		rtw_free(vendor->iso_in_buf);
		vendor->iso_in_buf = NULL;
	}

	if (vendor->iso_out_buf != NULL) {
		rtw_free(vendor->iso_out_buf);
		vendor->iso_out_buf = NULL;
	}

	vendor_iso_in_cnt = 0;
	vendor_iso_out_cnt = 0;

	return HAL_OK;
}

/**
  * @brief  Standard control requests handling callback
  * @param  host: Host handle
  * @retval Status
  */
static u8 usbh_vendor_setup(usb_host_t *host)
{
	usbh_vendor_host_t *vendor = &usbh_vendor_host;

	UNUSED(host);

	if ((vendor->cb != NULL) && (vendor->cb->setup != NULL)) {
		vendor->cb->setup();
	}

	return HAL_OK;
}

/**
  * @brief  State machine handling callback
  * @param  host: Host handle
  * @retval Status
  */
static u8 usbh_vendor_process(usb_host_t *host)
{
#if CONFIG_USBH_VENDOR_CTRL_TEST
	usbh_vendor_ctrl_process(host);
#elif CONFIG_USBH_VENDOR_BULK_TEST
	usbh_vendor_bulk_process(host);
#elif CONFIG_USBH_VENDOR_ISO_IN_TEST
	usbh_vendor_iso_in_process(host);
#elif CONFIG_USBH_VENDOR_ISO_OUT_TEST
	usbh_vendor_iso_out_process(host);
#endif
	return HAL_OK;
}

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Init vendor class
  * @param  cb: User callback
  * @retval Status
  */
u8 usbh_vendor_init(usbh_vendor_cb_t *cb)
{
	usbh_vendor_host_t *vendor = &usbh_vendor_host;

	if (cb != NULL) {
		vendor->cb = cb;
	}

	usbh_register_class(&usbh_vendor_driver);

	return HAL_OK;
}

/**
  * @brief  Deinit vendor class
  * @retval Status
  */
u8 usbh_vendor_deinit(void)
{
	usbh_unregister_class(&usbh_vendor_driver);

	return HAL_OK;
}

