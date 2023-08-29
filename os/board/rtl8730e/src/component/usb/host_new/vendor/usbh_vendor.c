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

typedef u8(* usbh_xfer_func)(usb_host_t *host, u8 *buf, u16 len, u8 pipe_num);

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

static const char *TEXT_CTRL = "CTRL";
static const char *TEXT_BULK = "BULK";
static const char *TEXT_INTR = "INTR";
static const char *TEXT_ISOC = "ISOC";

/* Private functions ---------------------------------------------------------*/

static char *usbh_get_xfer_type_text(usbh_vendor_xfer_t *xfer)
{
	char *text = NULL;

	switch (xfer->ep_type) {
	case USB_CH_EP_TYPE_CTRL:
		text = (char *)TEXT_CTRL;
		break;
	case USB_CH_EP_TYPE_BULK:
		text = (char *)TEXT_BULK;
		break;
	case USB_CH_EP_TYPE_INTR:
		text = (char *)TEXT_INTR;
		break;
	case USB_CH_EP_TYPE_ISOC:
		text = (char *)TEXT_ISOC;
		break;
	default:
		break;
	}

	return text;
}

static usbh_xfer_func usbh_get_xfer_func(usbh_vendor_xfer_t *xfer)
{
	usbh_xfer_func func = NULL;
	u8 is_in_xfer = USB_EP_IS_IN(xfer->ep_num);

	if (is_in_xfer) {
		switch (xfer->ep_type) {
		case USB_CH_EP_TYPE_BULK:
			func = (usbh_xfer_func)usbh_bulk_receive_data;
			break;
		case USB_CH_EP_TYPE_INTR:
			func = (usbh_xfer_func)usbh_intr_receive_data;
			break;
		case USB_CH_EP_TYPE_ISOC:
			func = (usbh_xfer_func)usbh_isoc_receive_data;
			break;
		default:
			break;
		}
	} else {
		switch (xfer->ep_type) {
		case USB_CH_EP_TYPE_BULK:
			func = (usbh_xfer_func)usbh_bulk_send_data;
			break;
		case USB_CH_EP_TYPE_INTR:
			func = (usbh_xfer_func)usbh_intr_send_data;
			break;
		case USB_CH_EP_TYPE_ISOC:
			func = (usbh_xfer_func)usbh_isoc_send_data;
			break;
		default:
			break;
		}
	}

	return func;
}

static u8 usbh_vendor_init_ep(usbh_vendor_xfer_t *xfer, usbh_ep_desc_t *ep_desc)
{
	char *xfer_type = usbh_get_xfer_type_text(xfer);

	xfer->ep_num = ep_desc->bEndpointAddress;
	xfer->ep_mps = ep_desc->wMaxPacketSize;
	xfer->ep_type = ep_desc->bmAttributes & USB_EP_XFER_TYPE_MASK;
	if ((xfer->ep_type == USB_CH_EP_TYPE_INTR) || (xfer->ep_type == USB_CH_EP_TYPE_ISOC)) {
		xfer->ep_interval = 1 << (MIN((MAX(ep_desc->bInterval, 1)), 16) - 1);
	} else {
		xfer->ep_interval = 0;
	}

	DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "%s EP%02X MPS=%d bInterval=%d interval=%d\n",
			   xfer_type, xfer->ep_num, xfer->ep_mps, ep_desc->bInterval, xfer->ep_interval);

	return HAL_OK;
}

static void usbh_vendor_get_endpoints(usbh_if_desc_t *intf)
{
	u32 tmp = 0;
	usbh_vendor_host_t *vendor = &usbh_vendor_host;
	usbh_ep_desc_t *ep_desc;
	usbh_vendor_xfer_t *xfer;
	u8 ep_type;
	u8 ep_in;

	for (tmp = 0; tmp < intf->bNumEndpoints; tmp++) {
		ep_desc = &intf->ep_desc[tmp];
		ep_type = ep_desc->bmAttributes & USB_EP_XFER_TYPE_MASK;
		ep_in = USB_EP_IS_IN(ep_desc->bEndpointAddress);
		switch (ep_type) {
		case USB_CH_EP_TYPE_BULK:
			if (ep_in) {
				xfer = &vendor->bulk_in_xfer;
			} else {
				xfer = &vendor->bulk_out_xfer;
			}
			usbh_vendor_init_ep(xfer, ep_desc);
			break;

		case USB_CH_EP_TYPE_INTR:
			if (ep_in) {
				xfer = &vendor->intr_in_xfer;
			} else {
				xfer = &vendor->intr_out_xfer;
			}
			usbh_vendor_init_ep(xfer, ep_desc);
			break;

		case USB_CH_EP_TYPE_ISOC:
			if (ep_in) {
				xfer = &vendor->isoc_in_xfer;
			} else {
				xfer = &vendor->isoc_out_xfer;
			}
			usbh_vendor_init_ep(xfer, ep_desc);

			break;

		default:
			break;
		}
	}
}

static void usbh_vendor_dump_buf(u8 *buf, u32 len)
{
	u32 i;

	for (i = 0; i < len;) {
		if (i + 10 <= len) {
			printf("%3d %3d %3d %3d %3d %3d %3d %3d %3d %3d\n", buf[i], buf[i + 1], buf[i + 2], buf[i + 3], buf[i + 4], buf[i + 5], buf[i + 6], buf[i + 7], buf[i + 8],
				   buf[i + 9]);
			i += 10;
		} else {
			for (; i < len; i++) {
				printf("%3d ", buf[i]);
			}
			printf("\n");
			break;
		}
	}
}

static void usbh_vendor_xfer_process(usb_host_t *host, usbh_vendor_xfer_t *in_xfer, usbh_vendor_xfer_t *out_xfer)
{
	u8 status;
	u32 len;
	u32 ticks;
	u8 is_timeout = 0;
	u8 is_ptx = ((out_xfer->ep_type == USB_CH_EP_TYPE_INTR) || (out_xfer->ep_type == USB_CH_EP_TYPE_ISOC));
	usbh_urb_state_t urb_state = USBH_URB_IDLE;
	usbh_vendor_host_t *vendor = &usbh_vendor_host;
	char *xfer_type = usbh_get_xfer_type_text(in_xfer);
	usbh_xfer_func xfer_func;

	xfer_func = usbh_get_xfer_func(in_xfer);

	switch (in_xfer->state) {
	case VENDOR_STATE_XFER:
		xfer_func(host, in_xfer->xfer_buf, in_xfer->ep_mps, in_xfer->pipe_num);
		in_xfer->state = VENDOR_STATE_BUSY;
		printf("%s IN %d#...\n", xfer_type, in_xfer->xfer_cnt);
		break;

	case VENDOR_STATE_BUSY:
		urb_state = usbh_get_urb_state(host, in_xfer->pipe_num);
		if (urb_state == USBH_URB_DONE) {
			len = usbh_get_last_transfer_size(host, in_xfer->pipe_num);
			if (len == 0) {
				in_xfer->state = VENDOR_STATE_XFER;
				return;
			}

			printf("%s IN %d# (%d) done\n", xfer_type, in_xfer->xfer_cnt, in_xfer->xfer_buf[0]);

			if (((in_xfer->xfer_len - len) > 0U) && (len >= in_xfer->ep_mps)) {
				in_xfer->xfer_len -= len ;
				in_xfer->xfer_buf += len;
				in_xfer->state = VENDOR_STATE_XFER;
			} else {
				in_xfer->test_buf[in_xfer->xfer_cnt++] = in_xfer->xfer_buf[0];
				if (out_xfer->xfer_cnt == out_xfer->xfer_max_cnt) {
					in_xfer->state = VENDOR_STATE_IDLE;
					printf("%s test result: IN/OUT = %d/%d:\n", xfer_type, in_xfer->xfer_cnt, out_xfer->xfer_cnt);
					usbh_vendor_dump_buf(in_xfer->test_buf, in_xfer->xfer_cnt);
					in_xfer->xfer_cnt = 0;
					out_xfer->xfer_cnt = 0;
				} else {
					in_xfer->xfer_len = in_xfer->xfer_max_len;
					in_xfer->xfer_buf = in_xfer->xfer_bk_buf;
					in_xfer->state = VENDOR_STATE_XFER;
				}
			}
		} else if (urb_state == USBH_URB_ERROR) {
			printf("%s IN error\n", xfer_type);
			in_xfer->state = VENDOR_STATE_IDLE;
		} else {
			if (out_xfer->xfer_cnt == out_xfer->xfer_max_cnt) {
				if ((host->tick - in_xfer->tick) >= 10 * in_xfer->ep_interval) {
					in_xfer->state = VENDOR_STATE_IDLE;
					printf("%s test result: IN/OUT = %d/%d:\n", xfer_type, in_xfer->xfer_cnt, out_xfer->xfer_cnt);
					usbh_vendor_dump_buf(in_xfer->test_buf, in_xfer->xfer_cnt);
					in_xfer->xfer_cnt = 0;
					out_xfer->xfer_cnt = 0;
					break;
				}
			}
			in_xfer->state = VENDOR_STATE_XFER;
		}
		break;

	default:
		break;
	}

	xfer_func = usbh_get_xfer_func(out_xfer);

	switch (out_xfer->state) {
	case VENDOR_STATE_XFER:
		if (is_ptx) {
			ticks = host->tick;
			if ((ticks - out_xfer->tick) >= out_xfer->ep_interval) {
				out_xfer->tick = ticks;
				is_timeout = 1;
			}
		}

		if ((!is_ptx) || is_timeout) {
			if (is_ptx && vendor->ptx_busy) {
				printf("PTX FIFO busy, %s OUT ignored\n", xfer_type);
				return;
			}
			if (out_xfer->xfer_len > out_xfer->ep_mps) {
				status = xfer_func(host, out_xfer->xfer_buf, out_xfer->ep_mps, out_xfer->pipe_num);
			} else {
				status = xfer_func(host, out_xfer->xfer_buf, out_xfer->xfer_len, out_xfer->pipe_num);
			}

			if (status == HAL_OK) {
				out_xfer->state = VENDOR_STATE_BUSY;
				if (is_ptx) {
					vendor->ptx_busy = 1;
				}
				printf("%s OUT %d#...\n", xfer_type, out_xfer->xfer_cnt);
			} else {
				printf("%s OUT %d# busy, retry...\n", xfer_type, out_xfer->xfer_cnt);
			}
		}

		break;

	case VENDOR_STATE_BUSY:
		urb_state = usbh_get_urb_state(host, out_xfer->pipe_num);
		if (urb_state == USBH_URB_DONE) {
			if (is_ptx) {
				vendor->ptx_busy = 0;
			}
			if (out_xfer->xfer_len > out_xfer->ep_mps) {
				out_xfer->xfer_len -= out_xfer->ep_mps;
				out_xfer->xfer_buf += out_xfer->ep_mps;
			} else {
				out_xfer->xfer_len = 0U;
			}

			printf("%s OUT %d# done\n", xfer_type, out_xfer->xfer_cnt);

			if (out_xfer->xfer_len > 0U) {
				out_xfer->state = VENDOR_STATE_XFER;
			} else {
				out_xfer->xfer_cnt++;
				if (out_xfer->xfer_cnt == out_xfer->xfer_max_cnt) {
					printf("%s OUT done\n", xfer_type);
					out_xfer->state = VENDOR_STATE_IDLE;
				} else {
					out_xfer->xfer_len = out_xfer->xfer_max_len;
					out_xfer->xfer_buf = out_xfer->xfer_bk_buf;
					rtw_memset((void *)out_xfer->xfer_buf, out_xfer->xfer_cnt, out_xfer->xfer_len);
					out_xfer->state = VENDOR_STATE_XFER;
				}
			}
		} else if (urb_state == USBH_URB_ERROR) {
			printf("%s OUT error\n", xfer_type);
			if (is_ptx) {
				vendor->ptx_busy = 0;
			}
			out_xfer->state = VENDOR_STATE_IDLE;
		} else {
#if 0
			// Check timeout
			if (is_ptx) {
				if ((host->tick - out_xfer->tick) >= out_xfer->ep_interval) {
					vendor->ptx_busy = 0;
					out_xfer->xfer_len = out_xfer->xfer_max_len;
					out_xfer->xfer_buf = out_xfer->xfer_bk_buf;
					out_xfer->state = VENDOR_STATE_XFER;
				}
			} else {
				// TODO
			}
#endif
		}
		break;

	default:
		break;
	}
}

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
	case VENDOR_STATE_XFER:
		setup.b.bmRequestType = USB_D2H | USB_REQ_TYPE_STANDARD | USB_REQ_RECIPIENT_DEVICE;
		setup.b.bRequest = USB_REQ_GET_DESCRIPTOR;
		setup.b.wValue = 0x0100;
		setup.b.wIndex = 0U;
		setup.b.wLength = 0x12;

		ret = usbh_ctrl_request(host, &setup, vendor->ctrl_buf);
		if (ret == HAL_OK) {
			vendor->state = VENDOR_STATE_IDLE;
			usbh_vendor_dump_buf(vendor->ctrl_buf, 18);
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

static u8 usbh_vendor_init_xfer(usb_host_t *host, usbh_vendor_xfer_t *xfer, u16 xfer_len, u16 test_cnt)
{
	u8 status = HAL_OK;
	u8 pipe_num;
	char *xfer_type = usbh_get_xfer_type_text(xfer);

	pipe_num = usbh_alloc_pipe(host, xfer->ep_num);
	if (pipe_num != 0xFFU) {
		xfer->pipe_num = pipe_num;
	} else {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Fail to allocate pipe for %s EP%02X\n", xfer_type, xfer->ep_num);
		status = HAL_ERR_MEM;
		goto exit;
	}

	DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "Allocate pipe %d for %s EP%02X\n", pipe_num, xfer_type, xfer->ep_num);

	xfer->xfer_buf = rtw_zmalloc(xfer_len);
	if (xfer->xfer_buf == NULL) {
		status = HAL_ERR_MEM;
		goto alloc_xfer_buf_fail_exit;
	}

	xfer->xfer_bk_buf = xfer->xfer_buf;
	xfer->xfer_max_len = xfer_len;
	xfer->xfer_len = xfer_len;
	xfer->xfer_max_cnt = test_cnt;
	xfer->xfer_cnt = 0;
	xfer->tick = 0;
	xfer->state = VENDOR_STATE_XFER;

	if (USB_EP_IS_OUT(xfer->ep_num)) {
		xfer->test_buf = NULL;
	} else {
		xfer->test_buf = rtw_zmalloc(test_cnt);
		if (xfer->test_buf == NULL) {
			status = HAL_ERR_MEM;
			goto alloc_test_buf_fail_exit;
		}
	}

	status = usbh_open_pipe(host, xfer->pipe_num, xfer->ep_num, xfer->ep_type, xfer->ep_mps);
	if (status != HAL_OK) {
		status = HAL_ERR_HW;
		goto open_pipe_fail_exit;
	}

	return status;

open_pipe_fail_exit:
	if (xfer->test_buf != NULL) {
		rtw_free(xfer->test_buf);
		xfer->test_buf = NULL;
	}
alloc_test_buf_fail_exit:
	rtw_free(xfer->xfer_buf);
	xfer->xfer_buf = NULL;
alloc_xfer_buf_fail_exit:
	usbh_free_pipe(host, xfer->pipe_num);
	xfer->pipe_num = 0;
exit:
	return status;
}

static void usbh_vendor_deinit_xfer(usb_host_t *host, usbh_vendor_xfer_t *xfer)
{
	if (xfer->pipe_num) {
		usbh_close_pipe(host, xfer->pipe_num);
		usbh_free_pipe(host, xfer->pipe_num);
		xfer->pipe_num = 0U;
	}

	if (xfer->xfer_buf != NULL) {
		rtw_free(xfer->xfer_buf);
		xfer->xfer_buf = NULL;
	}

	xfer->xfer_len = 0;
	xfer->xfer_cnt = 0;
	xfer->xfer_bk_buf = NULL;
}

/**
  * @brief  Attach callback.
  * @param  host: Host handle
  * @retval Status
  */
static u8 usbh_vendor_attach(usb_host_t *host)
{
	u8 status = HAL_OK;
	u8 interface;
	usbh_vendor_host_t *vendor = &usbh_vendor_host;
	usbh_if_desc_t *vendor_if_desc;

	vendor->ptx_busy = 0;

	/* Get interface index */
	interface = usbh_get_interface(host, VENDOR_CLASS_CODE, VENDOR_SUBCLASS_CODE, VENDOR_PROTOCOL);
	if (interface == 0xFFU) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Fail to find the interface for Vendor Class.");
		return HAL_ERR_PARA;
	}

	vendor->host = host;
	vendor->state = VENDOR_STATE_XFER;

	usbh_set_interface(host, interface);

#if CONFIG_USBH_VENDOR_BULK_TEST
	usbh_ctrl_set_interface(host, interface, 1U);
#else
	usbh_ctrl_set_interface(host, interface, 0U);
#endif

	vendor_if_desc = usbh_get_interface_descriptor(host, interface);
	if (vendor_if_desc == NULL) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Fail to get the interface descriptor for Vendor Class.");
		return HAL_ERR_PARA;
	}

	/* Get data in/out endpoints */
	usbh_vendor_get_endpoints(vendor_if_desc);

#if CONFIG_USBH_VENDOR_BULK_TEST
	status = usbh_vendor_init_xfer(host, &vendor->bulk_in_xfer, USBH_VENDOR_BULK_IN_BUF_SIZE, USBH_VENDOR_BULK_XFER_CNT);
	if (status != HAL_OK) {
		goto bulk_in_fail_exit;
	}

	status = usbh_vendor_init_xfer(host, &vendor->bulk_out_xfer, USBH_VENDOR_BULK_OUT_BUF_SIZE, USBH_VENDOR_BULK_XFER_CNT);
	if (status != HAL_OK) {
		goto bulk_out_fail_exit;
	}
#endif

#if CONFIG_USBH_VENDOR_INTR_TEST
	status = usbh_vendor_init_xfer(host, &vendor->intr_in_xfer, USBH_VENDOR_INTR_IN_BUF_SIZE, USBH_VENDOR_INTR_XFER_CNT);
	if (status != HAL_OK) {
		goto intr_in_fail_exit;
	}

	status = usbh_vendor_init_xfer(host, &vendor->intr_out_xfer, USBH_VENDOR_INTR_OUT_BUF_SIZE, USBH_VENDOR_INTR_XFER_CNT);
	if (status != HAL_OK) {
		goto intr_out_fail_exit;
	}
#endif

#if CONFIG_USBH_VENDOR_ISOC_TEST
	status = usbh_vendor_init_xfer(host, &vendor->isoc_in_xfer, USBH_VENDOR_ISOC_IN_BUF_SIZE, USBH_VENDOR_ISOC_XFER_CNT);
	if (status != HAL_OK) {
		goto isoc_in_fail_exit;
	}

	status = usbh_vendor_init_xfer(host, &vendor->isoc_out_xfer, USBH_VENDOR_ISOC_OUT_BUF_SIZE, USBH_VENDOR_ISOC_XFER_CNT);
	if (status != HAL_OK) {
		goto isoc_out_fail_exit;
	}
#endif

	return status;

#if CONFIG_USBH_VENDOR_ISOC_TEST
isoc_out_fail_exit:
	usbh_vendor_deinit_xfer(host, &vendor->isoc_in_xfer);
isoc_in_fail_exit:
#endif

#if CONFIG_USBH_VENDOR_INTR_TEST
	usbh_vendor_deinit_xfer(host, &vendor->intr_out_xfer);
intr_out_fail_exit:
	usbh_vendor_deinit_xfer(host, &vendor->intr_in_xfer);
intr_in_fail_exit:
#endif

#if CONFIG_USBH_VENDOR_BULK_TEST
	usbh_vendor_deinit_xfer(host, &vendor->bulk_out_xfer);
bulk_out_fail_exit:
	usbh_vendor_deinit_xfer(host, &vendor->bulk_in_xfer);
bulk_in_fail_exit:
#endif

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

#if CONFIG_USBH_VENDOR_BULK_TEST
	usbh_vendor_deinit_xfer(host, &vendor->bulk_in_xfer);
	usbh_vendor_deinit_xfer(host, &vendor->bulk_out_xfer);
#endif

#if CONFIG_USBH_VENDOR_INTR_TEST
	usbh_vendor_deinit_xfer(host, &vendor->intr_in_xfer);
	usbh_vendor_deinit_xfer(host, &vendor->intr_out_xfer);
#endif

#if CONFIG_USBH_VENDOR_ISOC_TEST
	usbh_vendor_deinit_xfer(host, &vendor->isoc_in_xfer);
	usbh_vendor_deinit_xfer(host, &vendor->isoc_out_xfer);
#endif

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
	usbh_vendor_host_t *vendor = &usbh_vendor_host;

#if CONFIG_USBH_VENDOR_CTRL_TEST
	usbh_vendor_ctrl_process(host);
#endif
#if CONFIG_USBH_VENDOR_BULK_TEST
	usbh_vendor_xfer_process(host, &vendor->bulk_in_xfer, &vendor->bulk_out_xfer);
#endif
#if CONFIG_USBH_VENDOR_INTR_TEST
	usbh_vendor_xfer_process(host, &vendor->intr_in_xfer, &vendor->intr_out_xfer);
#endif
#if CONFIG_USBH_VENDOR_ISOC_TEST
	usbh_vendor_xfer_process(host, &vendor->isoc_in_xfer, &vendor->isoc_out_xfer);
#endif

	usbh_notify_class_state_change(host);

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

