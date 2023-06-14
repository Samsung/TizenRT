/**
  ******************************************************************************
  * @file    usbh_uvc_parse.c
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

/* Private variables ---------------------------------------------------------*/


extern usbh_uvc_host_t uvc_host;

/* Private functions ---------------------------------------------------------*/


/**
  * @brief	Alloc entity memory
  * @param	extrabytes: extra memory except uvc_entity
  * @retval Pointer to alloc entity
  */
static uvc_entity *uvc_entity_alloc(u32 extrabytes)
{
	uvc_entity *entity;

	entity = (uvc_entity *) rtw_zmalloc(sizeof(uvc_entity) + extrabytes);

	return entity;
}



/**
  * @brief	Free entity memory
  * @param	entity: entity to free
  * @retval None
  */
static void uvc_entity_free(uvc_entity *entity)
{
	rtw_free(entity);
}



/**
  * @brief	Find next standard descriptor
  * @param	pbuf: given buffer to find
  			len: given buffer length
  * @retval Pointer of next standard descriptor
  */
static u8 *usbh_uvc_find_next_stdesc(u8 *pbuf, u32 *len)
{
	u8 *desc = pbuf;

	while (*len > 0) {
		if (((desc_header *)desc)->bDescriptorType == USB_DESC_TYPE_INTERFACE) {
			return (u8 *) desc;
		} else {
			*len -= ((desc_header *)desc)->bLength;
			desc += ((desc_header *)desc)->bLength;
		}
	}

	return NULL;
}



/**
  * @brief	Parse entity from given descriptor
  * @param	desc: given descriptor buffer
  * @retval Status
  */
static u8 usbh_uvc_parse_entity(u8 *desc)
{
	usbh_uvc_host_t *uvc = &uvc_host;
	uvc_vc *vc_intf = &uvc->uvc_desc.vc_intf;
	uvc_entity *entity = NULL;

	switch (desc[2]) {
	case UVC_VC_HEADER:
		vc_intf->vcheader = (uvc_vc_header_desc *)desc;
		break;

	case UVC_VC_INPUT_TERMINAL:
		vc_intf->entity_num ++;
		entity = uvc_entity_alloc(0);
		entity->p = desc;
		entity->id = desc[3];
		entity->source[0] = 0;
		entity->type = UVC_VC_INPUT_TERMINAL;
		break;

	case UVC_VC_OUTPUT_TERMINAL:
		vc_intf->entity_num ++;
		entity = uvc_entity_alloc(0);
		entity->p = desc;
		entity->id = desc[3];
		entity->source[0] = desc[7];
		entity->type = UVC_VC_OUTPUT_TERMINAL;
		break;

	case UVC_VC_SELECTOR_UNIT:
		vc_intf->entity_num ++;
		entity = uvc_entity_alloc(desc[4]);
		entity->p = desc;
		entity->id = desc[3];
		rtw_memcpy(entity->source, &desc[5], desc[4]);
		entity->type = UVC_VC_SELECTOR_UNIT;
		break;

	case UVC_VC_PROCESSING_UNIT:
		vc_intf->entity_num ++;
		entity = uvc_entity_alloc(0);
		entity->p = desc;
		entity->id = desc[3];
		entity->source[0] = desc[4];
		entity->type = UVC_VC_PROCESSING_UNIT;
		break;

	case UVC_VC_EXTENSION_UNIT:
		vc_intf->entity_num ++;
		entity = uvc_entity_alloc(desc[21]);
		entity->p = desc;
		entity->id = desc[3];
		rtw_memcpy(entity->source, &desc[22], desc[21]);
		entity->type = UVC_VC_EXTENSION_UNIT;
		break;

	case UVC_VC_ENCODING_UNIT:
		vc_intf->entity_num ++;
		entity = uvc_entity_alloc(0);
		entity->p = desc;
		entity->id = desc[3];
		entity->source[0] = desc[4];
		entity->type = UVC_VC_ENCODING_UNIT;
		break;

	default:
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Wrong entity type!");
		break;
	}

	if (entity) {
		list_add_tail((struct list_head *)&entity->list, &uvc->entity_list);
	}

	return 0;
}



/**
  * @brief	Parse video control interface
  * @param	desc: given descriptor buffer
  			length: lenghth of given buffer
  * @retval Status
  */
static u8 usbh_uvc_parse_vc(u8 *pbuf, u32 *length)
{
	u8 *desc = pbuf;
	int ret;
	usbh_uvc_host_t *uvc = &uvc_host;
	uvc_vc *vc_intf = &uvc->uvc_desc.vc_intf;

	vc_intf->p = desc;
	vc_intf->bInterfaceNumber = desc[2];
	*length = 0;

	while (1) {
		/*find next descripter*/
		*length += ((desc_header *) desc)->bLength;
		desc = pbuf + *length;

		switch (((desc_header *) desc)->bDescriptorType) {
		case USB_DESC_TYPE_CS_INTERFACE:
			ret = usbh_uvc_parse_entity((u8 *)desc);
			if (ret) {
				DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Fail to parse entity\n");
				return ret;
			}
			break;

		case USB_DESC_TYPE_ENDPOINT:
			if (((usbh_ep_desc_t *)desc)->bmAttributes == USB_CH_EP_TYPE_INTR) {
				vc_intf->intr_ep = desc;
			} else {
				DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Wrong endpoint type\n");
			}
			break;

		case USB_DESC_TYPE_CS_ENDPOINT:		//class-specific VC interrupt endpoint descriptor
			if (((uvc_vc_intr_ep_desc *)desc)->bDescriptorType == 0x25 && \
				((uvc_vc_intr_ep_desc *)desc)->bDescriptorSubType == USB_CH_EP_TYPE_INTR) {
				vc_intf->cs_intr_desc = desc;
			} else {
				DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Wrong class-specific vc intr descriptor\n");
			}
			break;

		case USB_DESC_TYPE_INTERFACE:
			return 0;

		default:
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Wrong descriptor type: %d\n", ((desc_header *) desc)->bDescriptorType);
			return 0;
		}
	}

}



/**
  * @brief	Parse video format
  * @param	vs_intf: pointer of video streaming interface
  			desc: given descriptor buffer
  			length: lenghth of given buffer
  * @retval Status
  */
static u8 usbh_uvc_parse_format(uvc_vs *vs_intf, u8 *pbuf, u16 *length)
{
	u8 *desc = pbuf;
	u16 len = 0;
	u16 totallen;
	u32 nformat = 0, nframe_mjepg = 0, nframe_uncomp = 0;
	u32 index = -1;
	u32 parsed_frame_num = 0;
	vs_frame *frame;
	vs_frame *tmp_frame;

	if (desc[2] != UVC_VS_INPUT_HEADER) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Header is no vs input header\n");
		return HAL_ERR_PARA;
	}

	vs_intf->InputHeader = (uvc_vs_input_header_desc *) desc;
	totallen = ((uvc_vs_input_header_desc *) desc)->wTotalLength;
	*length = totallen;

	/*first scan to get total number of format and frame*/
	while (len < totallen) {
		switch (desc[2]) {
		case UVC_VS_INPUT_HEADER:
		case UVC_VS_STILL_IMAGE_FRAME:
		case UVC_VS_COLORFORMAT:
			break;

		case UVC_VS_FORMAT_UNCOMPRESSED:
		case UVC_VS_FORMAT_MJPEG:
			nformat++;
			break;

		case UVC_VS_FRAME_MJPEG:
			nframe_mjepg ++;
			break;
		case UVC_VS_FRAME_UNCOMPRESSED:
			nframe_uncomp ++;
			break;

		default:
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Unsupported vs Format Type!");
			break;
		}
		/*find next descripter*/
		len += ((desc_header *) desc)->bLength;
		desc = pbuf + len;
	}

	desc = pbuf;
	len = 0;

	vs_intf->nformat = nformat;
	vs_intf->format = (vs_format *) rtw_zmalloc(nformat * sizeof(vs_format) + \
					  (nframe_mjepg + nframe_uncomp) * sizeof(vs_frame));

	tmp_frame = (vs_frame *)((u8 *)vs_intf->format + nformat * sizeof(vs_format));

	while (len < totallen) {
		switch (desc[2]) {
		case UVC_VS_INPUT_HEADER:
			break;

		case UVC_VS_FORMAT_MJPEG:
		case UVC_VS_FORMAT_UNCOMPRESSED:
			index ++;
			vs_intf->format[index].frame = (vs_frame *)((u8 *)vs_intf->format + \
										   nformat * sizeof(vs_format) + \
										   parsed_frame_num * sizeof(vs_frame));
			vs_intf->format[index].type = desc[2];
			vs_intf->format[index].index = desc[3];
			//vs_intf->format[index].bpp = desc[21];
			break;

		case UVC_VS_FRAME_UNCOMPRESSED:
		case UVC_VS_FRAME_MJPEG:
			vs_intf->format[index].nframes ++;
			frame = (vs_frame *)((u8 *)tmp_frame + parsed_frame_num * sizeof(vs_frame));
			frame->bFrameIndex = desc[3];
			frame->bmCapabilities = desc[4];
			frame->wWidth = *(u16 *)(desc + 5); //desc[5]|desc[6]<<8;
			frame->wHeight = *(u16 *)(desc + 7); //desc[7]|desc[8]<<8;
			frame->dwMinBitRate = *(u32 *)(desc + 9);
			frame->dwMaxBitRate = *(u32 *)(desc + 13);
			frame->dwMaxVideoFrameBufferSize = *(u32 *)(desc + 17);
			frame->dwDefaultFrameInterval = *(u32 *)(desc + 21);
			frame->bFrameIntervalType = desc[25];
			frame->dwFrameInterval = (u32 *)&desc[26];
			parsed_frame_num ++;
			break;

		case UVC_VS_STILL_IMAGE_FRAME:
			break;

		case UVC_VS_COLORFORMAT:
			break;

		default:
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Unsupported Format Type!");
			break;

		}

		/*find next descripter*/
		len += ((desc_header *) desc)->bLength;
		desc = pbuf + len;
	}

	return HAL_OK;
}



/**
  * @brief	Parse video streaming interface
  * @param	pbuf: given descriptor buffer
  			length: lenghth of given buffer
  * @retval Status
  */
static u8 usbh_uvc_parse_vs(u8 *pbuf, u32 *length)
{
	u8 *desc = pbuf;
	u16 len = 0;
	u8 bAlternateSetting;
	usbh_uvc_host_t *uvc = &uvc_host;
	uvc_vs *vs_intf = &uvc->uvc_desc.vs_intf[uvc->uvc_desc.vs_num];
	uvc->uvc_desc.vs_num++;

	vs_intf->p = desc;
	vs_intf->bInterfaceNumber = desc[2];
	*length = 0;
	/*find next descripter*/
	*length += ((desc_header *) desc)->bLength;
	desc = pbuf + *length;

	while (1) {
		switch (((desc_header *) desc)->bDescriptorType) {
		case USB_DESC_TYPE_CS_INTERFACE:
			usbh_uvc_parse_format(vs_intf, desc, &len);
			desc = desc + len;
			*length += len;
			break;

		case USB_DESC_TYPE_INTERFACE:
			bAlternateSetting = ((usbh_if_desc_t *)desc)->bAlternateSetting;
			if (bAlternateSetting != 0) {
				vs_intf->altsetting[bAlternateSetting - 1].p = desc;
				vs_intf->alt_num++;

				len = ((desc_header *) desc)->bLength;
				*length += len;
				desc = desc + len;
				vs_intf->altsetting[bAlternateSetting - 1].endpoint = (usbh_ep_desc_t *)desc;

				len = ((desc_header *) desc)->bLength;
				desc = desc + len;
				*length += len;
			} else {
				return 0;
			}
			break;

		default:
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "bDescriptorType: %d\n", ((desc_header *) desc)->bDescriptorType);
			return 0;
		}
	}
}



/**
  * @brief	Parse configuration descriptor
  * @param	host: usb host structure
  * @retval Status
  */
u8 usbh_uvc_parse_cfgdesc(usb_host_t *host)
{
	u8 ret = HAL_OK;
	usbh_uvc_host_t *uvc = &uvc_host;
	desc_header *desc = (desc_header *)usbh_get_raw_configuration_descriptor(host);
	u32 cfglen = (u32)((usbh_cfg_desc_t *) desc)->wTotalLength;
	usbh_if_desc_t *pbuf = (usbh_if_desc_t *) desc;
	u32 len;

	while (1) {
		pbuf = (usbh_if_desc_t *)usbh_uvc_find_next_stdesc((u8 *)pbuf, &cfglen);
		if (pbuf == NULL) {
			break;
		}

		if (pbuf->bInterfaceClass == UVC_CLASS_CODE) {
			switch (pbuf->bInterfaceSubClass) {
			case USB_SUBCLASS_VIDEOCONTROL:
				ret = usbh_uvc_parse_vc((u8 *)pbuf, &len);
				if (ret) {
					DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "UVC parse video control fail\n");
					return ret;
				}
				pbuf = (usbh_if_desc_t *)((u8 *) pbuf + len);
				cfglen -= len;
				break;

			case USB_SUBCLASS_VIDEOSTREAMING:
				if (pbuf->bAlternateSetting == 0) {
					ret = usbh_uvc_parse_vs((u8 *)pbuf, &len);
					if (ret) {
						DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "UVC parse video streaming fail\n");
						return ret;
					}

					pbuf = (usbh_if_desc_t *)((u8 *) pbuf + len);
					cfglen -= len;
				}
				break;

			default:
				DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "subclass(%d) is not VC or VS\n", pbuf->bInterfaceClass);
				return HAL_ERR_PARA;

			}
		} else {

			//skip non-uvc descriptor
			cfglen -= ((desc_header *)desc)->bLength;
			pbuf = (usbh_if_desc_t *)((u8 *) pbuf + ((desc_header *)desc)->bLength);
		}
	}

	//select video streaming interface 0 as current interface
	uvc->uvc_desc.cur_vs_num = 0;

	return ret;

}



/**
  * @brief	Init uvc descriptor related
  * @param	None
  * @retval None
  */
void usbh_uvc_desc_init(void)
{
	usbh_uvc_host_t *uvc = &uvc_host;

	INIT_LIST_HEAD(&uvc->entity_list);
}


/**
  * @brief	Deinit uvc descriptor related
  * @param	None
  * @retval None
  */
void usbh_uvc_desc_free(void)
{
	usbh_uvc_host_t *uvc = &uvc_host;
	struct list_head *p, *n;
	uvc_entity *ent;
	uvc_vs *vs_intf = &uvc->uvc_desc.vs_intf[uvc->uvc_desc.cur_vs_num];

	list_for_each_safe(p, n, &uvc->entity_list) {
		ent = list_entry(p, uvc_entity, list);
		uvc_entity_free(ent);
	}

	rtw_free(vs_intf->format);
}

