/**
  ******************************************************************************
  * @file    usbh_uvc_class.c
  * @author  Realsil WLAN5 Team
  * @brief   This file provides the functionalities of the USB UVC Class
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

#include "usbh_uvc.h"


/* Private defines -----------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

extern usbh_uvc_host_t uvc_host;

static u8 usbh_uvc_attach(usb_host_t *host);
static u8 usbh_uvc_detach(usb_host_t *host);
static u8 usbh_uvc_process(usb_host_t *host);
static u8 usbh_uvc_sof(usb_host_t *host);
static u8 usbh_uvc_setup(usb_host_t *host);


/* Private variables ---------------------------------------------------------*/

/* USB Standard Device Descriptor */
static usbh_class_driver_t usbh_uvc_driver = {
	.class_code = UVC_CLASS_CODE,
	.attach = usbh_uvc_attach,
	.detach = usbh_uvc_detach,
	.setup = usbh_uvc_setup,
	.process = usbh_uvc_process,
	.sof = usbh_uvc_sof
};


/* Private functions ---------------------------------------------------------*/


static void usbh_uvc_set_alt(void)
{
	usbh_uvc_host_t *uvc = &uvc_host;
	usb_host_t *host = uvc->host;
	int i;
	u32 ep_size;
	u32 XferSize = uvc->stream.stream_ctrl.dwMaxPayloadTransferSize;
	u32 rx_fifo_size = host->config.rx_fifo_size * 4;
	u32 max_ep_size;
	u32 cur_vs_num = uvc->uvc_desc.cur_vs_num;

	if (host->config.speed == USB_SPEED_HIGH) {
		max_ep_size = USB_HS_MAX_PACKET_SIZE;
	} else {
		max_ep_size = USB_FS_MAX_PACKET_SIZE;
	}

	max_ep_size = max_ep_size > rx_fifo_size ? rx_fifo_size : max_ep_size;

	XferSize = XferSize > 1024 ? XferSize : 1024;

	// todo_eric: this setting is used for dma mode.
	for (i = 0; i < uvc->uvc_desc.vs_intf[cur_vs_num].alt_num; i++) {
		ep_size = uvc->uvc_desc.vs_intf[cur_vs_num].altsetting[i].endpoint->wMaxPacketSize;

		if (host->config.speed == USB_SPEED_HIGH) {
			ep_size = (ep_size & 0x07ff) * (1 + ((ep_size >> 11) & 3));
		} else {
			ep_size = ep_size & 0x07ff;
		}

		if ((ep_size >= XferSize) && (ep_size <= max_ep_size)) {
			uvc->cur_setting.altsetting = &uvc->uvc_desc.vs_intf[cur_vs_num].altsetting[i];
			uvc->cur_setting.bAlternateSetting = ((usbh_if_desc_t *)uvc->cur_setting.altsetting->p)->bAlternateSetting;
			uvc->cur_setting.ep_addr = uvc->cur_setting.altsetting->endpoint->bEndpointAddress;
			uvc->cur_setting.ep_size = uvc->cur_setting.altsetting->endpoint->wMaxPacketSize;
			uvc->cur_setting.interval = uvc->cur_setting.altsetting->endpoint->bInterval;
			uvc->cur_setting.ep_type = uvc->cur_setting.altsetting->endpoint->bmAttributes & USB_EP_XFERTYPE_MASK;
			uvc->cur_setting.bInterfaceNumber = uvc->cur_setting.cur_vs_intf->bInterfaceNumber;
			uvc->cur_setting.valid = 1;
		}
	}
}


/**
  * @brief  Attach callback.
  * @param  host: Host handle
  * @retval Status
  */
static u8 usbh_uvc_attach(usb_host_t *host)
{
	usbh_uvc_host_t *uvc = &uvc_host;
	u8 status = HAL_ERR_UNKNOWN;
	u8 cur_vs_num;
	int i;
	u32 ep_size, cur_ep_size = 0;
	u32 max_ep_size;
	u32 rx_fifo_size = host->config.rx_fifo_size * 4;

	uvc->host = host;

	if (host->config.speed == USB_SPEED_HIGH) {
		max_ep_size = USB_HS_MAX_PACKET_SIZE;
	} else {
		max_ep_size = USB_FS_MAX_PACKET_SIZE;
	}

	max_ep_size = max_ep_size > rx_fifo_size ? rx_fifo_size : max_ep_size;

	status = usbh_uvc_parse_cfgdesc(host);
	if (status) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Fail to parse uvc class specific descriptor\n");
		return status;
	}

	/*find best alt setting and enpoint*/
	cur_vs_num = uvc->uvc_desc.cur_vs_num;
	uvc->cur_setting.cur_vs_intf = &uvc->uvc_desc.vs_intf[cur_vs_num];

	for (i = 0; i < uvc->uvc_desc.vs_intf[cur_vs_num].alt_num; i++) {
		ep_size = uvc->uvc_desc.vs_intf[cur_vs_num].altsetting[i].endpoint->wMaxPacketSize;

		if (host->config.speed == USB_SPEED_HIGH) {
			ep_size = (ep_size & 0x07ff) * (1 + ((ep_size >> 11) & 3));
		} else {
			ep_size = ep_size & 0x07ff;
		}

		if ((ep_size >= cur_ep_size) && (ep_size <= max_ep_size)) {
			uvc->cur_setting.altsetting = &uvc->uvc_desc.vs_intf[cur_vs_num].altsetting[i];
			uvc->cur_setting.bAlternateSetting = ((usbh_if_desc_t *)uvc->cur_setting.altsetting->p)->bAlternateSetting;
			uvc->cur_setting.ep_addr = uvc->cur_setting.altsetting->endpoint->bEndpointAddress;
			uvc->cur_setting.ep_size = uvc->cur_setting.altsetting->endpoint->wMaxPacketSize;
			uvc->cur_setting.interval = uvc->cur_setting.altsetting->endpoint->bInterval;
			uvc->cur_setting.ep_type = uvc->cur_setting.altsetting->endpoint->bmAttributes & USB_EP_XFERTYPE_MASK;
			uvc->cur_setting.bInterfaceNumber = uvc->cur_setting.cur_vs_intf->bInterfaceNumber;
			uvc->cur_setting.valid = 1;
			cur_ep_size = ep_size;
		}
	}

	uvc->cur_setting.pipe = usbh_alloc_pipe(host, uvc->cur_setting.ep_addr);

	//todo: vc may has a interrupt endpoint, vs may has a bulk endpoint for still image data

	if ((uvc->cb != NULL) && (uvc->cb->attach != NULL)) {
		uvc->cb->attach();
	}

	uvc->stream_data_state = STREAM_DEFAULT_INTF1;

	status = HAL_OK;
	return status;
}


/**
  * @brief  Detach callback.
  * @param  host: Host handle
  * @retval Status
  */
static u8 usbh_uvc_detach(usb_host_t *host)
{
	usbh_uvc_host_t *uvc = &uvc_host;

	if ((uvc->cb != NULL) && (uvc->cb->detach != NULL)) {
		uvc->cb->detach();
	}

	if (uvc->cur_setting.pipe) {
		usbh_close_pipe(host, uvc->cur_setting.pipe);
		usbh_free_pipe(host, uvc->cur_setting.pipe);
		uvc->cur_setting.pipe = 0U;
	}

	return HAL_OK;
}


/**
  * @brief  Standard control requests handling callback
  * @param  host: Host handle
  * @retval Status
  */
static u8 usbh_uvc_setup(usb_host_t *host)
{
	u8 status;
	u8 ret = HAL_BUSY;
	usbh_uvc_host_t *uvc = &uvc_host;

	switch (uvc->stream_data_state) {
	case STREAM_DEFAULT_INTF1:
		status = usbh_ctrl_set_interface(host, uvc->cur_setting.bInterfaceNumber, 0);
		if (status) {
			usbh_notify_class_state_change(host);
			return ret;
		} else {
			uvc->stream_data_state = STREAM_DEFALT_PROBE;
		}
		break;

	case STREAM_DEFALT_PROBE:
		/*probe default setting, set format and frame*/
		usbh_uvc_video_init();
		ret = HAL_OK;
		break;

	default:
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Should never reach here\n");
		break;
	}

	return ret;
}


/**
  * @brief  State machine handling callback
  * @param  host: Host handle
  * @retval Status
  */
static u8 usbh_uvc_process(usb_host_t *host)
{
	u8 status = HAL_BUSY;
	usbh_uvc_host_t *uvc = &uvc_host;

	if (uvc->cur_setting.valid == 1) {
		switch (uvc->stream_data_state) {

		case STREAM_OPEN_PIPE:
			usbh_open_pipe(host,
						   uvc->cur_setting.pipe,
						   uvc->cur_setting.ep_addr,
						   uvc->cur_setting.ep_type,
						   uvc->cur_setting.ep_size);

			usbh_set_toggle(host, uvc->cur_setting.pipe, 0);

			uvc->stream_data_state = STREAM_PROBE_COMMIT;
			usbh_notify_class_state_change(host);
			break;

		case STREAM_PROBE_COMMIT:
			usbh_uvc_commit_video(&uvc->stream.stream_ctrl);

			uvc->stream_data_state = STREAM_SEL_SETTING;
			break;

		case STREAM_SEL_SETTING:
			usbh_uvc_set_alt();

			uvc->stream_data_state = STREAM_SET_INTF;
			usbh_notify_class_state_change(host);
			break;

		case STREAM_SET_INTF:
			status = usbh_ctrl_set_interface(host, uvc->cur_setting.bInterfaceNumber, \
											 uvc->cur_setting.bAlternateSetting);
			if (status) {
				usbh_notify_class_state_change(host);
				return status;
			} else {
				uvc->stream_data_state = STREAM_START_IN;
			}
			break;


		case STREAM_START_IN:
			status = usbh_isoc_receive_data(host,
											uvc->stream.urb[0]->addr,
											uvc->cur_setting.ep_size,
											uvc->cur_setting.pipe);

			uvc->stream_data_state = STREAM_DATA_IN;
			break;

		case STREAM_DATA_IN:
			status = usbh_uvc_process_rx(host);
			if (status) {
				DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Process rx error\n");
			}

			if (uvc->stream_state == STREAMING_OFF) {
				uvc->stream_data_state = STREAM_CLOSE_PIPE;
			}
			break;

		case STREAM_CLOSE_PIPE:
			usbh_hal_stop();
			usbh_close_pipe(host, uvc->cur_setting.pipe);

			uvc->stream_data_state = STREAM_DEFAULT_INTF2;
			usbh_notify_class_state_change(host);
			break;


		case STREAM_DEFAULT_INTF2:
			status = usbh_ctrl_set_interface(host, uvc->cur_setting.bInterfaceNumber, 0);
			if (status) {
				usbh_notify_class_state_change(host);
				return status;
			} else {
				uvc->stream_data_state = STREAM_STATE_IDLE;
			}
			break;

		default:
			break;
		}
	} else {
		vTaskDelay(5);
		status = HAL_OK;
	}

	return status;
}


/**
  * @brief  SOF callback
  * @param  host: Host handle
  * @retval Status
  */
static u8 usbh_uvc_sof(usb_host_t *host)
{
	UNUSED(host);

	return HAL_OK;
}



/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Init UVC class
  * @param  cb: User callback
  * @retval Status
  */
u8 usbh_uvc_class_init(void)
{
	u8 ret = HAL_OK;
	ret = usbh_register_class(&usbh_uvc_driver);

	return ret;
}


/**
  * @brief  Deinit UVC class
  * @retval void
  */
void usbh_uvc_class_deinit(void)
{
	usbh_unregister_class(&usbh_uvc_driver);
}

