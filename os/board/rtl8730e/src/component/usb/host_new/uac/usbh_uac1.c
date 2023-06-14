/**
  ******************************************************************************
  * @file    usbh_uac.c
  * @author  Realsil WLAN5 Team
  * @brief   This file provides the functionalities of the USB Vendor Class
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbh.h"
#include "usbh_uac1.h"


/* Private defines -----------------------------------------------------------*/
#define CLASS_TAG "UAC1.0 HOST"
/* Private types -------------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/
#define USB_DT_CS_INTERFACE            		0x24
#define USB_DT_CS_ENDPOINT             		0x25

#define ENABLE_DUMP_DESCRIPYOT_PARSE (1)
//#undef ENABLE_DUMP_DESCRIPYOT_PARSE

typedef struct {
	u8  bLength;
	u8  bDescriptorType;
} usb_descriptor_header;

/* Private function prototypes -----------------------------------------------*/

static u8 usbh_uac_cb_attach(usb_host_t *host);
static u8 usbh_uac_cb_detach(usb_host_t *host);
static u8 usbh_uac_cb_process(usb_host_t *host);
static u8 usbh_uac_cb_setup(usb_host_t *host);
static u8 usbh_uac_cb_sof(usb_host_t *host);
/* Private variables ---------------------------------------------------------*/

/* USB Standard Device Descriptor */
static usbh_class_driver_t usbh_uac_driver = {
	.class_code = USB_CLASS_AUDIO,
	.attach = usbh_uac_cb_attach,
	.detach = usbh_uac_cb_detach,
	.setup = usbh_uac_cb_setup,
	.process = usbh_uac_cb_process,
	.sof = usbh_uac_cb_sof,
};

static usbh_uac_host_t usbh_uac_host;

static void dump_cfgdesc(void)
{
#ifdef ENABLE_DUMP_DESCRIPYOT_PARSE
	int i, j, k;
	usbh_uac_host_t *uac = &usbh_uac_host;
	uac_cfg *puacdesc = &uac->uac_desc;
	USB_AUDIO_INFO("--------------------Dump Start------------------------------\n");
	USB_AUDIO_INFO("[%s]As Count=%d[%d-%d] \n", CLASS_TAG, puacdesc->as_count, puacdesc->cur_as_index[USB_IN], puacdesc->cur_as_index[USB_OUT]);

	//dump uac ac
	//todo
	//dump uac as
	for (i = 0; i < puacdesc->as_count ; i ++) {
		uac_as *pasintf = &puacdesc->as_intf[i];
		USB_AUDIO_INFO("[%s]interNum=%d[AltCount=%d,type=%d] \n", CLASS_TAG, pasintf->bInterfaceNumber, pasintf->alt_count, pasintf->msg_type);
		for (j = 0; j < pasintf->alt_count ; j ++) {
			USB_AUDIO_INFO("[%s][ep=0x%x,maxsize=%d] \n", CLASS_TAG,
						   pasintf->altsetting[j].audio_ep->bEndpointAddress, pasintf->altsetting[j].audio_ep->wMaxPacketSize);
			USB_AUDIO_INFO("[%s][ch=%d,bitwidth=%d,freqCount=%d] \n", CLASS_TAG, pasintf->altsetting[j].format->bNrChannels,
						   pasintf->altsetting[j].format->bBitResolution, pasintf->altsetting[j].format->bSamFreqType);
			for (k = 0; k < pasintf->altsetting[j].format->bSamFreqType; k++) {
				USB_AUDIO_INFO("fre:id=%d ,freq=%d-%d-%d ", k, (u32)pasintf->altsetting[j].format->tSamFreq[k][0]
							   , ((u32)pasintf->altsetting[j].format->tSamFreq[k][1]) << 8, ((u32)pasintf->altsetting[j].format->tSamFreq[k][2]) << 16);
				USB_AUDIO_INFO("fre:id=%d ,freq=%d \n", k, (u32)pasintf->altsetting[j].format->tSamFreq[k][0]
							   | ((u32)pasintf->altsetting[j].format->tSamFreq[k][1]) << 8 | ((u32)pasintf->altsetting[j].format->tSamFreq[k][2]) << 16);
			}
		}
	}
	USB_AUDIO_INFO("---------------------Dump End-----------------------------\n");
#endif
}
static void dump_cur_setting_cfgdesc(void)
{
#ifdef ENABLE_DUMP_DESCRIPYOT_PARSE
	int i;
	usbh_uac_host_t *uac = &usbh_uac_host;

	USB_AUDIO_INFO("--------------------Dump Start------------------------------\n");
	for (i = 0; i < USB_MAX ; i ++) {
		uac_setting *psetting = &uac->cur_setting[i];
		USB_AUDIO_INFO("[%s]interNum=%d[bAlternateSetting=%d,freId=%d] \n", CLASS_TAG,
					   psetting->bInterfaceNumber, psetting->bAlternateSetting, psetting->bFreqId);
	}
	USB_AUDIO_INFO("---------------------Dump End-----------------------------\n");
#endif
}

/**
  * @brief	Find next standard descriptor
  * @param	pbuf: given buffer to find
  			len: given buffer length
  * @retval Pointer of next standard descriptor
  */
static u8 *usbh_uac_find_next_stdesc(u8 *pbuf, u32 *len)
{
	u8 *desc = pbuf;

	while (*len > 0) {
		if (((usb_descriptor_header *)desc)->bDescriptorType == USB_DESC_TYPE_INTERFACE) {
			return (u8 *) desc;
		} else {
			*len -= ((usb_descriptor_header *)desc)->bLength;
			desc += ((usb_descriptor_header *)desc)->bLength;
		}
	}

	return NULL;
}


/**
  * @brief	Parse audio control interface
  * @param	desc: given descriptor buffer
  			length: lenghth of given buffer
  * @retval Status
  */
static u8 usbh_uac_parse_ac(u8 *pbuf, u32 *length)
{
	*length = ((usb_descriptor_header *) pbuf)->bLength;
	USB_AUDIO_INFO("enter usbh_uac_parse_ac *length=%d\n", *length);
	//todo
	return  0 ;
}


/**
  * @brief	Parse audio streaming interface
  * @param	pbuf: given descriptor buffer
  			length: lenghth of given buffer
  * @retval Status
  */
static u8 usbh_uac_parse_as(u8 *pbuf, u32 *length)
{
	u8 *desc = pbuf;
	u16 len = 0;
	u8 bAlternateSetting;
	usbh_uac_host_t *uac = &usbh_uac_host;

	//uac alt setting 0
	uac_as *as_intf = &uac->uac_desc.as_intf[uac->uac_desc.as_count];
	as_intf->p = desc;
	as_intf->bInterfaceNumber = desc[2];
	*length = ((usb_descriptor_header *) desc)->bLength;
	desc = pbuf + *length;

	//for next alt setting type: in out , it means two
	uac->uac_desc.as_count++;

	//finish setting change
	while (1) {
		USB_AUDIO_INFO("enter loop type:0x%x\n", ((usb_descriptor_header *) desc)->bDescriptorType);
		switch (((usb_descriptor_header *) desc)->bDescriptorType) {
		case USB_DESC_TYPE_INTERFACE:
			if (((usbh_if_desc_t *)desc)->bInterfaceNumber != as_intf->bInterfaceNumber) {
				USB_AUDIO_INFO("newnum=%d:old=%d, return\n", ((usbh_if_desc_t *)desc)->bInterfaceNumber, as_intf->bInterfaceNumber);
				return 0;
			}
			bAlternateSetting = ((usbh_if_desc_t *)desc)->bAlternateSetting;
			if (bAlternateSetting != 0) {//not setting 0
				as_intf->altsetting[bAlternateSetting - 1].p = desc;
				as_intf->alt_count++;
				as_intf->altsetting[bAlternateSetting - 1].desc = (usbh_if_desc_t *)desc;

				len = ((usb_descriptor_header *) desc)->bLength;
				desc = desc + len;
				*length += len;
			} else {
				return 0;
			}
			break;

		case USB_DT_CS_INTERFACE: {
			struct uac_format_type_i_discrete_descriptor *psubtype = (struct uac_format_type_i_discrete_descriptor *)desc;
			if (UAC_FORMAT_TYPE_II == psubtype->bDescriptorSubtype) {
				as_intf->altsetting[bAlternateSetting - 1].format = (struct uac_format_type_i_discrete_descriptor *)desc;
			} else {
				as_intf->altsetting[bAlternateSetting - 1].header = (struct uac1_as_header_descriptor *)desc;
			}
			len = ((usb_descriptor_header *) desc)->bLength;
			desc = desc + len;
			*length += len;
		}
		break;

		case USB_DESC_TYPE_ENDPOINT:
			if (bAlternateSetting != 0) {
				as_intf->altsetting[bAlternateSetting - 1].audio_ep = (struct usb_audio_endpoint_descriptor *)desc;
				if (as_intf->altsetting[bAlternateSetting - 1].audio_ep->bEndpointAddress & 0x80) {
					as_intf->msg_type = USB_IN;
				} else {
					as_intf->msg_type = USB_OUT;
				}
				len = ((usb_descriptor_header *) desc)->bLength;
				desc = desc + len;
				*length += len;
			} else {
				return 0;
			}
			break;

		case USB_DT_CS_ENDPOINT:
			if (bAlternateSetting != 0) {
				as_intf->altsetting[bAlternateSetting - 1].iso_ep = (struct uac_iso_endpoint_descriptor *)desc;
				len = ((usb_descriptor_header *) desc)->bLength;
				desc = desc + len;
				*length += len;
				USB_AUDIO_INFO("parse finish !\n");
			} else {
				return 0;
			}
			break;

		default:
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "bDescriptorType: %d\n", ((usb_descriptor_header *) desc)->bDescriptorType);
			return 0;
		}
	}
}

/**
  * @brief	Parse configuration descriptor
  * @param	host: usb host structure
  * @retval Status
  */
u8 usbh_uac_parse_cfgdesc(usb_host_t *host)
{
	u8 ret = HAL_OK;
	usbh_cfg_desc_t *desc = (usbh_cfg_desc_t *)usbh_get_raw_configuration_descriptor(host);
	u32 cfglen = (u32)desc->wTotalLength;
	usbh_if_desc_t *pbuf = (usbh_if_desc_t *) desc;
	u32 len = 0;

	while (1) {
		pbuf = (usbh_if_desc_t *)usbh_uac_find_next_stdesc((u8 *)pbuf, &cfglen);
		if (pbuf == NULL) {
			break;
		}

		if (pbuf->bInterfaceClass == USB_CLASS_AUDIO) {
			switch (pbuf->bInterfaceSubClass) {
			case USB_SUBCLASS_AUDIOCONTROL: //audio control
				ret = usbh_uac_parse_ac((u8 *)pbuf, &len);
				if (ret) {
					DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "UAC parse audio control fail\n");
					return ret;
				}
				pbuf = (usbh_if_desc_t *)((u8 *) pbuf + len);
				cfglen -= len;
				len = 0;
				break;

			case USB_SUBCLASS_AUDIOSTREAMING: //audio streaming
				USB_AUDIO_INFO("setting=%d,len=%d,type=%d\n",
							   pbuf->bAlternateSetting, pbuf->bLength, pbuf->bDescriptorType);
				if (pbuf->bAlternateSetting == 0) { //setting 0
					ret = usbh_uac_parse_as((u8 *)pbuf, &len);
					if (ret) {
						DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "UAC parse audio streaming fail\n");
						return ret;
					}

					pbuf = (usbh_if_desc_t *)((u8 *) pbuf + len);
					cfglen -= len;
					len = 0;
				}
				break;

			default:
				DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "subclass(%d) is not AC or AS\n", pbuf->bInterfaceClass);
				return HAL_ERR_PARA;

			}
		} else {
			//skip no audio descriptor
			USB_AUDIO_INFO("skip ((usb_descriptor_header *)desc)->bDescriptorType=0x%x,len=%d\n", ((usb_descriptor_header *)desc)->bDescriptorType,
						   ((usb_descriptor_header *)desc)->bLength);
			cfglen -= ((usb_descriptor_header *)desc)->bLength;
			pbuf = (usbh_if_desc_t *)((u8 *) pbuf + ((usb_descriptor_header *)desc)->bLength);
		}
	}

	return ret;
}


u8 usbh_uac_setup_out(u8 id)
{
	usbh_uac_host_t *uac = &usbh_uac_host;
	usb_host_t *host = uac->host;
	int ret = HAL_ERR_UNKNOWN;

	uac->cur_setting[USB_OUT].bFreqId = id ;

	if ((host->state == USBH_CLASS_REQUEST) || (host->state == USBH_CLASS_READY)) {
		uac->state = UAC_STATE_SETUP_OUT_ALT;
		usbh_notify_class_state_change(host);
		ret = HAL_OK;
	}

	return ret;
}
u8 usbh_uac_setup_in(u8 id)
{
	usbh_uac_host_t *uac = &usbh_uac_host;
	usb_host_t *host = uac->host;
	int ret = HAL_ERR_UNKNOWN;
	uac->cur_setting[USB_IN].bFreqId = id ;

	if ((host->state == USBH_CLASS_REQUEST) || (host->state == USBH_CLASS_READY)) {
		uac->state = UAC_STATE_SETUP_IN_ALT;
		usbh_notify_class_state_change(host);
		ret = HAL_OK;
	}

	return ret;
}
static void dump_req_struxt(usb_setup_req_t *ctrl)
{
	UNUSED(ctrl);
	USB_AUDIO_INFO("[UAC] bmRequestType=0x%02X bRequest=0x%02X wValue=0x%04X wIndex=0x%04X wLength=0x%02X\n",
				   ctrl->bmRequestType,
				   ctrl->bRequest,
				   ctrl->wValue,
				   ctrl->wIndex,
				   ctrl->wLength);
}

static u8 usbh_uac_process_set_out_alt(usb_host_t *host)
{
	usbh_setup_req_t setup;
	usbh_uac_host_t *uac = &usbh_uac_host;

	setup.b.bmRequestType = USB_H2D | USB_REQ_TYPE_STANDARD | USB_REQ_RECIPIENT_INTERFACE;
	setup.b.bRequest = USB_REQ_SET_INTERFACE;
	setup.b.wValue = uac->cur_setting[USB_OUT].bAlternateSetting;
	setup.b.wIndex = uac->cur_setting[USB_OUT].bInterfaceNumber;
	setup.b.wLength = 0U;
	dump_req_struxt(&setup.b);

	return usbh_ctrl_request(host, &setup, NULL);
}
static u8 usbh_uac_process_set_in_alt(usb_host_t *host)
{
	usbh_setup_req_t setup;
	usbh_uac_host_t *uac = &usbh_uac_host;

	setup.b.bmRequestType = USB_H2D | USB_REQ_TYPE_STANDARD | USB_REQ_RECIPIENT_INTERFACE;
	setup.b.bRequest = USB_REQ_SET_INTERFACE;
	setup.b.wValue = uac->cur_setting[USB_IN].bAlternateSetting;
	setup.b.wIndex = uac->cur_setting[USB_IN].bInterfaceNumber;
	setup.b.wLength = 0U;
	dump_req_struxt(&setup.b);

	return usbh_ctrl_request(host, &setup, NULL);
}


static u8 usbh_uac_process_set_out_freq(usb_host_t *host)
{
	usbh_setup_req_t setup;
	usbh_uac_host_t *uac = &usbh_uac_host;
	u8 id = uac->cur_setting[USB_OUT].bFreqId;

	setup.b.bmRequestType = USB_H2D | USB_REQ_TYPE_CLASS | USB_REQ_RECIPIENT_ENDPOINT;
	setup.b.bRequest = UAC_SET_CUR;
	setup.b.wValue = ((u16)uac->cur_setting[USB_OUT].bAlternateSetting) << 16;
	setup.b.wIndex = uac->cur_setting[USB_OUT].ep_info.isoc_ep_addr;
	setup.b.wLength = 3;
	dump_req_struxt(&setup.b);

	return usbh_ctrl_request(host, &setup, uac->cur_setting[USB_OUT].format_info.format_freq[id]);
}
static u8 usbh_uac_process_set_in_freq(usb_host_t *host)
{
	usbh_setup_req_t setup;
	usbh_uac_host_t *uac = &usbh_uac_host;
	u8 id = uac->cur_setting[USB_OUT].bFreqId;

	setup.b.bmRequestType = USB_H2D | USB_REQ_TYPE_CLASS | USB_REQ_RECIPIENT_ENDPOINT;
	setup.b.bRequest = UAC_SET_CUR;
	setup.b.wValue = ((u16)uac->cur_setting[USB_IN].bAlternateSetting) << 16;
	setup.b.wIndex = uac->cur_setting[USB_IN].ep_info.isoc_ep_addr;
	setup.b.wLength = 0U;
	dump_req_struxt(&setup.b);

	return usbh_ctrl_request(host, &setup, uac->cur_setting[USB_IN].format_info.format_freq[id]);
}



/**
  * @brief  Iso in test
  * @param  host: Host handle
  * @retval None
*/
static void usbh_uac_isoc_in_process(usb_host_t *host)
{
	usbh_uac_host_t *uac = &usbh_uac_host;
	uac_ep_cfg *ep = &(uac->cur_setting[USB_IN].ep_info);
	usbh_urb_state_t urb_state = USBH_URB_IDLE;
	u32 len;

	switch (ep->isoc_state) {
	case UAC_TRANSFER_STATE_PROCESS:
		if ((host->tick - ep->isoc_tick) >= ep->isoc_interval) {
			ep->isoc_tick = host->tick;
			usbh_isoc_receive_data(host,
								   ep->isoc_buf,
								   ep->isoc_packet_size,
								   ep->isoc_pipe);

			ep->isoc_state = UAC_TRANSFER_STATE_PROCESS_BUSY;
		}
		usbh_notify_class_state_change(host);
		break;

	case UAC_TRANSFER_STATE_PROCESS_BUSY:
		urb_state = usbh_get_urb_state(host, ep->isoc_pipe);
		if (urb_state == USBH_URB_DONE) {
			len = usbh_get_last_transfer_size(host, ep->isoc_pipe);
			if ((uac->cb != NULL) && (uac->cb->isoc_received != NULL)) {
				uac->cb->isoc_received(ep->isoc_buf, len);
			}
			ep->isoc_state = UAC_TRANSFER_STATE_IDLE;
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "ISOC IN ok\n");
		} else if (urb_state == USBH_URB_ERROR) {
			ep->isoc_state = UAC_TRANSFER_STATE_PROCESS;
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
static void usbh_uac_isoc_out_process(usb_host_t *host)
{
	usbh_uac_host_t *uac = &usbh_uac_host;
	uac_ep_cfg *ep = &(uac->cur_setting[USB_OUT].ep_info);
	usbh_urb_state_t urb_state = USBH_URB_IDLE;

	switch (ep->isoc_state) {
	case UAC_TRANSFER_STATE_PROCESS:
		if ((host->tick - ep->isoc_tick) >= ep->isoc_interval) {
			ep->isoc_tick = host->tick;
			if (ep->isoc_len > ep->isoc_packet_size) {
				usbh_isoc_send_data(host,
									ep->isoc_buf,
									ep->isoc_packet_size,
									ep->isoc_pipe);
			} else {
				usbh_isoc_send_data(host,
									ep->isoc_buf,
									(u16)ep->isoc_len,
									ep->isoc_pipe);
			}

			ep->isoc_state = UAC_TRANSFER_STATE_PROCESS_BUSY;
		}
		usbh_notify_class_state_change(host);
		break;

	case UAC_TRANSFER_STATE_PROCESS_BUSY:
		urb_state = usbh_get_urb_state(host, ep->isoc_pipe);
		if (urb_state == USBH_URB_DONE) {
			if (ep->isoc_len > ep->isoc_packet_size) {
				ep->isoc_len -= ep->isoc_packet_size;
				ep->isoc_buf += ep->isoc_packet_size;
			} else {
				ep->isoc_len = 0U;
			}

			if (ep->isoc_len > 0U) {
				ep->isoc_state = UAC_TRANSFER_STATE_PROCESS;
			} else {
				DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "ISOC OUT ok\n");
				ep->isoc_state = UAC_TRANSFER_STATE_IDLE;
				if ((uac->cb != NULL) && (uac->cb->isoc_transmitted != NULL)) {
					uac->cb->isoc_transmitted(urb_state);
				}
			}
		} else if (urb_state == USBH_URB_ERROR) {
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "ISOC OUT failed\n");
			ep->isoc_state = UAC_TRANSFER_STATE_IDLE;
			if ((uac->cb != NULL) && (uac->cb->isoc_transmitted != NULL)) {
				uac->cb->isoc_transmitted(urb_state);
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
static u8 usbh_uac_cb_attach(usb_host_t *host)
{
	usbh_uac_host_t *uac = &usbh_uac_host;
	u8 status = HAL_ERR_UNKNOWN;
	int j, k;
	u32 ep_size, cur_ep_size = 0;
	u32 max_ep_size;
	//u32 rx_fifo_size = host->config.rx_fifo_size;

	uac->host = host;

	if (host->config.speed == USB_SPEED_HIGH) {
		max_ep_size = 1024;
	} else {
		max_ep_size = 1023;
	}
	//max_ep_size = max_ep_size > rx_fifo_size ? rx_fifo_size : max_ep_size;

	status = usbh_uac_parse_cfgdesc(host);
	if (status) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Fail to parse uac class specific descriptor\n");
		return status;
	}
	dump_cfgdesc();

	//select audio streaming interface 0 as current interface both of usb in/out
	uac->uac_desc.cur_as_index[USB_IN] = 0;
	uac->uac_desc.cur_as_index[USB_OUT] = 0;
	//uac->cur_setting[USB_IN].bFreqId =

	/*find best alt setting and enpoint*/
	for (j = 0; j < USB_MAX; j++) {
		uac->cur_setting[j].cur_altsetting = &uac->uac_desc.as_intf[j].altsetting[uac->uac_desc.cur_as_index[j]];
		uac_ep_cfg *ep = &uac->cur_setting[j].ep_info;
		uac_format_cfg *format = &uac->cur_setting[j].format_info;
		uac_alt *cur_alt = uac->cur_setting[j].cur_altsetting;

		USB_AUDIO_INFO("[%s]Index=%d,num=%d,alt_count%d,type=%d \n", CLASS_TAG,
					   uac->uac_desc.cur_as_index[j],
					   uac->uac_desc.as_intf[j].bInterfaceNumber,
					   uac->uac_desc.as_intf[j].alt_count,
					   uac->uac_desc.as_intf[j].msg_type);

		//format
		format->bNrChannels = cur_alt->format->bNrChannels;
		format->bSubframeSize = cur_alt->format->bSubframeSize;
		format->bSamFreqType = cur_alt->format->bSamFreqType;
		for (k = 0; k < format->bSamFreqType; k++) {
			format->format_freq[k][0] = cur_alt->format->tSamFreq[k][0];
			format->format_freq[k][1] = cur_alt->format->tSamFreq[k][1];
			format->format_freq[k][2] = cur_alt->format->tSamFreq[k][2];
		}
		//choose the first freqId
		uac->cur_setting[j].bFreqId = 0 ;
		uac->cur_setting[j].bInterfaceNumber = cur_alt->desc->bInterfaceNumber;
		uac->cur_setting[j].bAlternateSetting = cur_alt->desc->bAlternateSetting;

		//ep
		ep_size = cur_alt->audio_ep->wMaxPacketSize;
		USB_AUDIO_INFO("[%s]EP size info %d,%d,%d \n", CLASS_TAG, ep_size, cur_ep_size, max_ep_size);
		if ((ep_size >= cur_ep_size) && (ep_size <= max_ep_size)) {

			ep->isoc_ep_addr = cur_alt->audio_ep->bEndpointAddress;
			ep->isoc_packet_size = cur_alt->audio_ep->wMaxPacketSize;
			ep->isoc_interval = cur_alt->audio_ep->bInterval;

			ep->isoc_state = UAC_TRANSFER_STATE_IDLE;

			USB_AUDIO_INFO("address =0x%x,size=%d\n", ep->isoc_ep_addr, ep->isoc_packet_size);
		}
	}

	uac->cur_setting[USB_IN].ep_info.isoc_pipe = usbh_alloc_pipe(host, uac->cur_setting[USB_IN].ep_info.isoc_ep_addr);
	uac->cur_setting[USB_OUT].ep_info.isoc_pipe = usbh_alloc_pipe(host, uac->cur_setting[USB_OUT].ep_info.isoc_ep_addr);

	dump_cur_setting_cfgdesc();

	usbh_open_pipe(host,
				   uac->cur_setting[USB_OUT].ep_info.isoc_pipe,
				   uac->cur_setting[USB_OUT].ep_info.isoc_ep_addr,
				   USB_CH_EP_TYPE_ISOC,
				   uac->cur_setting[USB_OUT].ep_info.isoc_packet_size);
	usbh_open_pipe(host,
				   uac->cur_setting[USB_IN].ep_info.isoc_pipe,
				   uac->cur_setting[USB_IN].ep_info.isoc_ep_addr,
				   USB_CH_EP_TYPE_ISOC,
				   uac->cur_setting[USB_IN].ep_info.isoc_packet_size);

	if ((uac->cb != NULL) && (uac->cb->attach != NULL)) {
		uac->cb->attach();
	}

	status = HAL_OK;
	return status;
}


/**
  * @brief  Detach callback.
  * @param  host: Host handle
  * @retval Status
  */
static u8 usbh_uac_cb_detach(usb_host_t *host)
{
	usbh_uac_host_t *uac = &usbh_uac_host;
	UNUSED(host);
	if ((uac->cb != NULL) && (uac->cb->detach != NULL)) {
		uac->cb->detach();
	}
#if  0 // todo
	if (uac->isoc_in_pipe) {
		usbh_close_pipe(host, uac->isoc_in_pipe);
		usbh_free_pipe(host, uac->isoc_in_pipe);
		uac->isoc_out_pipe = 0U;
	}

	if (uac->isoc_out_pipe) {
		usbh_close_pipe(host, uac->isoc_out_pipe);
		usbh_free_pipe(host, uac->isoc_out_pipe);
		uac->isoc_out_pipe = 0U;
	}
#endif

	uac->state = UAC_STATE_IDLE;

	return HAL_OK;
}

/**
  * @brief  Standard control requests handling callback
  * @param  host: Host handle
  * @retval Status
  */
static u8 usbh_uac_cb_setup(usb_host_t *host)
{
	usbh_uac_host_t *uac = &usbh_uac_host;

	UNUSED(host);

	if ((uac->cb != NULL) && (uac->cb->setup != NULL)) {
		uac->cb->setup();
	}

	return HAL_OK;
}

/**
  * @brief  Sof callback
  * @param  host: Host handle
  * @retval Status
  */
static u8 usbh_uac_cb_sof(usb_host_t *host)
{
	UNUSED(host);
	return 0;
}


/**
  * @brief  State machine handling callback
  * @param  host: Host handle
  * @retval Status
  */
static u8 usbh_uac_cb_process(usb_host_t *host)
{
	u8 ret = HAL_OK;
	usbh_uac_host_t *uac = &usbh_uac_host;
	UNUSED(host);

	switch (uac->state) {
	case UAC_STATE_SETUP_OUT_ALT:
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "Uac SETUP OUT Alt\n");
		ret = usbh_uac_process_set_out_alt(host);
		if (ret == HAL_OK) {
			uac->state = UAC_STATE_SETUP_OUT_FREQ;
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "Uac SETUP OUT Alt Success\n");
		} else if (ret != HAL_BUSY) {
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "Uac process OUT Alt error\n");
			uac->state = UAC_STATE_ERROR;
		}
		break;
	case UAC_STATE_SETUP_OUT_FREQ:
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "Uac SETUP OUT freq\n");
		ret = usbh_uac_process_set_out_freq(host);
		if (ret == HAL_OK) {
			uac->state = UAC_STATE_SETUP_IN_ALT;
		} else if (ret != HAL_BUSY) {
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "Uac process OUT freq error\n");
			uac->state = UAC_STATE_ERROR;
		}
		break;

	case UAC_STATE_SETUP_IN_ALT:
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "Uac SETUP IN alt\n");
		ret = usbh_uac_process_set_in_alt(host);
		if (ret == HAL_OK) {
			uac->state = UAC_STATE_SETUP_IN_FREQ;
		} else if (ret != HAL_BUSY) {
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "Uac process IN Alt error\n");
			uac->state = UAC_STATE_ERROR;
		}
		break;
	case UAC_STATE_SETUP_IN_FREQ:
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "Uac SETUP IN freq\n");
		ret = usbh_uac_process_set_in_freq(host);
		if (ret == HAL_OK) {
			uac->state = UAC_STATE_TRANSFER;
		} else if (ret != HAL_BUSY) {
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "Uac process IN freq error\n");
			uac->state = UAC_STATE_ERROR;
		}
		break;


	case UAC_STATE_TRANSFER:
		usbh_uac_isoc_out_process(host);
		usbh_uac_isoc_in_process(host);
		break;
	case UAC_STATE_ERROR:
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "Uac ERROR\n");
		ret = usbh_ctrl_clear_feature(host, 0x00U);
		if (ret == HAL_OK) {
			uac->state = UAC_STATE_IDLE;
		}
		break;
	default:
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "Uac IDLE\n");
		break;
	}
	return ret;
}

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Init uac class
  * @param  cb: User callback
  * @retval Status
  */
u8 usbh_uac_init(usbh_uac_cb_t *cb)
{
	u8 ret;
	usbh_uac_host_t *uac = &usbh_uac_host;

	rtw_memset(uac, 0x00, sizeof(usbh_uac_host_t));

	if (cb != NULL) {
		uac->cb = cb;
		if (cb->init != NULL) {
			ret = cb->init();
			if (ret != HAL_OK) {
				DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "USBH uac user init fail\n");
				return ret;
			}
		}
	}

	usbh_register_class(&usbh_uac_driver);

	return HAL_OK;
}

/**
  * @brief  Deinit uac class
  * @retval Status
  */
u8 usbh_uac_deinit(void)
{
	usbh_uac_host_t *uac = &usbh_uac_host;

	if ((uac->cb != NULL) && (uac->cb->deinit != NULL)) {
		uac->cb->deinit();
	}

	usbh_unregister_class(&usbh_uac_driver);

	return HAL_OK;
}



/**
  * @brief  Start to transfer ISOC data
  * @param  buf: Data buffer
  * @param  len: Data length
  * @retval Status
  */
u8 usbh_uac_isoc_transmit(u8 *buf, u32 len)
{
	u8 ret = HAL_BUSY;
	usbh_uac_host_t *uac = &usbh_uac_host;
	usb_host_t *host = uac->host;
	uac_ep_cfg *ep = &(uac->cur_setting[USB_OUT].ep_info);

	if ((uac->state == UAC_STATE_IDLE) || (uac->state == UAC_STATE_TRANSFER)) {
		if (ep->isoc_state == UAC_TRANSFER_STATE_IDLE) {
			ep->isoc_buf = buf;
			ep->isoc_len = len;
			ep->isoc_state = UAC_TRANSFER_STATE_PROCESS;
			uac->state = UAC_STATE_TRANSFER;
			usbh_notify_class_state_change(host);
			ret = HAL_OK;
		} else {
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "ISOC OUT busy uac->state =%d\n", ep->isoc_state);
		}
	} else {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "ISOC OUT not ready\n");
	}

	return ret;
}


/**
  * @brief  Start to receive ISOC data
  * @param  buf: Data buffer
  * @param  len: Data length
  * @retval Status
  */
u8 usbh_uac_isoc_receive(u8 *buf, u32 len)
{
	u8 ret = HAL_BUSY;
	usbh_uac_host_t *uac = &usbh_uac_host;
	usb_host_t *host = uac->host;
	uac_ep_cfg *ep = &(uac->cur_setting[USB_IN].ep_info);
	UNUSED(len);
	if ((uac->state == UAC_STATE_IDLE) || (uac->state == UAC_STATE_TRANSFER)) {
		if (ep->isoc_state == UAC_TRANSFER_STATE_IDLE) {
			ep->isoc_buf = buf;
			ep->isoc_len = ep->isoc_packet_size;
			ep->isoc_state = UAC_TRANSFER_STATE_PROCESS;
			uac->state = UAC_STATE_TRANSFER;
			usbh_notify_class_state_change(host);
			ret = HAL_OK;
		} else {
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "ISOC IN busy uac->state =%d\n", ep->isoc_state);
			ret = HAL_BUSY;
		}
	} else {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "ISOC IN not ready\n");
		ret = HAL_BUSY;
	}

	return ret;
}



