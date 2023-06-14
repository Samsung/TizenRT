/**
  ******************************************************************************
  * @file    usbh_cdc_ecm.c
  * @author  Realsil WLAN5 Team
  * @brief   This file provides the functionalities of the USB CDC ECM Class
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2023, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "usbh_cdc_ecm.h"
#include "usb_os.h"
#include "usbh.h"

/* Private defines -----------------------------------------------------------*/
#define USBH_CDC_BUFFER_SIZE             1024
#define ECM_ENABLE_DUMP_DESCRIPYOT_PARSE (0)

/* conventional codes for class-specific descriptors */
#define USB_DT_CS_INTERFACE              0x24


/* Private types -------------------------------------------------------------*/
typedef struct {
	u8  bLength;
	u8  bDescriptorType;
} usb_ecm_descriptor_header;

/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static u8 usbh_cdc_ecm_attach(usb_host_t *host);
static u8 usbh_cdc_ecm_detach(usb_host_t *host);
static u8 usbh_cdc_ecm_process(usb_host_t *host);
static u8 usbh_cdc_ecm_setup(usb_host_t *host);
static u8 usbh_cdc_ecm_sof(usb_host_t *host);
static u8 usbh_cdc_ecm_nak(usb_host_t *host, u8 pipe_num);
static void usbh_cdc_ecm_process_bulk_out(usb_host_t *host);
static void usbh_cdc_ecm_process_bulk_in(usb_host_t *host);
static void usbh_cdc_ecm_process_intr_in(usb_host_t *host);
static u8 usbh_cdc_acm_process_get_string(usb_host_t *host, u8 *pbuf, u16 buflen, u8 str_id);
static u8 usbh_cdc_ecm_process_get_statistic(usb_host_t *host);
static u8 usbh_cdc_ecm_process_set_packet_filter(usb_host_t *host);
static u8 usbh_cdc_ecm_process_set_alt(usb_host_t *host);
static u8 usbh_cdc_ecm_process_set_muiticast_filter(usb_host_t *host);


/* Private variables ---------------------------------------------------------*/

/* USB Standard Device Descriptor */
static usbh_class_driver_t usbh_cdc_ecm_driver = {
	.class_code = CDC_CLASS_CODE,
	.attach = usbh_cdc_ecm_attach,
	.detach = usbh_cdc_ecm_detach,
	.setup = usbh_cdc_ecm_setup,
	.process = usbh_cdc_ecm_process,
	.sof = usbh_cdc_ecm_sof,
	.nak = usbh_cdc_ecm_nak,
};

static usbh_cdc_ecm_host_t usbh_cdc_ecm_host;

/* Private functions ---------------------------------------------------------*/
static u8 hex_to_char(u8 hex_num)
{
	if ((hex_num >= 0x30) && (hex_num <= 0x39)) {
		/* code */
		hex_num = 0 + hex_num - 0x30;
	} else if ((hex_num >= 0x41) && (hex_num <= 0x5A)) {
		/* code */
		hex_num = 0xA + (hex_num - 0x41);
	} else if ((hex_num >= 0x61) && (hex_num <= 0x7A)) {
		/* code */
		hex_num = 0xA + (hex_num - 0x61);
	} else {
		hex_num = 0x0;
	}

	return hex_num;
}

static void dump_ecm_cfgdesc(void)
{
#if ECM_ENABLE_DUMP_DESCRIPYOT_PARSE
	usbh_cdc_ecm_host_t *ecm = &usbh_cdc_ecm_host;
	usbh_cdc_ecm_comm_if_t        *pcomm_if = &(ecm->comm_if);
	usbh_cdc_ecm_data_if_t        *pdata_if = &(ecm->data_if);
	usbh_cdc_ecm_network_func_t   *pfunc_if = &(ecm->func_if);

	DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "--------------------Dump Descriptor Start------------------\n");

	//dump ecm control
	DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "[ECM]epaddr=0x%x,intr_in_packet_size=%d\n",
			   pcomm_if->intr_in_ep, pcomm_if->intr_in_packet_size);
	DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "[ECM]mac=0x%x,maxsize=%d\n",
			   pfunc_if->iMACAddress, pfunc_if->wMaxSegmentSize);
	//dump ecm data
	DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "[BULK][in:epaddr=0x%x,packsize=%d][out:epaddr=0x%x,packsize=%d]\n",
			   pdata_if->bulk_in_ep, pdata_if->bulk_in_packet_size,
			   pdata_if->bulk_out_ep, pdata_if->bulk_out_packet_size);

	DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "---------------------Dump End-----------------------------\n");
#endif
}
static u8 usbh_cdc_ecm_usb_status_check(void)
{
	usbh_cdc_ecm_host_t *cdc = &usbh_cdc_ecm_host;

	if (cdc->usbh_state < USBH_CDC_ECM_SETUP) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "[ECM]Usb Not Setup success,wait...\n");
		rtw_msleep_os(1000);
		return HAL_BUSY;
	}

	return HAL_OK;
}

/**
  * @brief	Find next standard descriptor interface
  * @param	pbuf: given buffer to find
  			len: given buffer length
  * @retval Pointer of next standard descriptor
  */
static u8 *usbh_cdc_ecm_find_next_stdesc(u8 *pbuf, u32 *len)
{
	u8 *desc = pbuf;

	while (*len > 0) {
		if (((usb_ecm_descriptor_header *)desc)->bDescriptorType == USB_DESC_TYPE_INTERFACE) {
			return (u8 *) desc;
		} else {
			*len -= ((usb_ecm_descriptor_header *)desc)->bLength;
			desc += ((usb_ecm_descriptor_header *)desc)->bLength;
		}
	}

	return NULL;
}


/**
  * @brief	Parse ecm control interface
  * @param	desc: given descriptor buffer
  			length: lenghth of given buffer
  * @retval Status
  */
static u8 usbh_cdc_ecm_parse_control(u8 *pbuf, u32 *length)
{
	usbh_cdc_ecm_host_t *ecm = &usbh_cdc_ecm_host;
	usbh_cdc_ecm_comm_if_t *pcomm_if = &(ecm->comm_if);
	usbh_cdc_ecm_network_func_t *pfunc_if = &(ecm->func_if);

	u8 *desc = pbuf ;
	while (1) {
		if (USB_DESC_TYPE_ENDPOINT == ((usb_ecm_descriptor_header *) desc)->bDescriptorType) {
			pcomm_if->intr_in_packet_size = ((usbh_ep_desc_t *)desc)->wMaxPacketSize;
			pcomm_if->intr_in_ep = ((usbh_ep_desc_t *)desc)->bEndpointAddress;
			*length += ((usb_ecm_descriptor_header *) desc)->bLength;
			return HAL_OK;
		} else if (USB_DT_CS_INTERFACE == ((usbh_cdc_ecm_network_func_t *)desc)->bDescriptorType
				   && CDC_ECM_NETWORK_FUNC_DESCRIPTOR == ((usbh_cdc_ecm_network_func_t *)desc)->bDescriptorSubtype) {
			memcpy(pfunc_if, desc, sizeof(usbh_cdc_ecm_network_func_t)); //Ethernet Networking Functional Descriptor
			*length += ((usb_ecm_descriptor_header *) desc)->bLength;
			desc = desc + ((usb_ecm_descriptor_header *) desc)->bLength;
		} else {
			*length += ((usb_ecm_descriptor_header *) desc)->bLength;
			desc = desc + ((usb_ecm_descriptor_header *) desc)->bLength;
		}
	}

	return HAL_ERR_PARA;
}


/**
  * @brief	Parse ecm data interface
  * @param	pbuf: given descriptor buffer
  			length: lenghth of given buffer
  			leftlen: left buffer length of the given buffer
  * @retval Status
  */
static u8 usbh_cdc_ecm_parse_data(u8 *pbuf, u32 *length, u32 leftlen)
{
	usbh_cdc_ecm_host_t *ecm = &usbh_cdc_ecm_host;
	usbh_cdc_ecm_data_if_t *pdata_if = &(ecm->data_if);
	u8 *desc = pbuf ;
	while (1) {
		if (USB_DESC_TYPE_INTERFACE == ((usb_ecm_descriptor_header *) desc)->bDescriptorType) {
			*length += ((usb_ecm_descriptor_header *) desc)->bLength;
			desc = desc + ((usb_ecm_descriptor_header *) desc)->bLength;
		} else if (USB_DESC_TYPE_ENDPOINT == ((usb_ecm_descriptor_header *) desc)->bDescriptorType) {
			if ((((usbh_ep_desc_t *) desc)->bEndpointAddress & USB_REQ_DIR_MASK) == USB_D2H) { //in
				pdata_if->bulk_in_ep = ((usbh_ep_desc_t *) desc)->bEndpointAddress;
				pdata_if->bulk_in_packet_size  = ((usbh_ep_desc_t *) desc)->wMaxPacketSize;
			} else { //out
				pdata_if->bulk_out_ep = ((usbh_ep_desc_t *) desc)->bEndpointAddress;
				pdata_if->bulk_out_packet_size  = ((usbh_ep_desc_t *) desc)->wMaxPacketSize;
			}

			*length += ((usb_ecm_descriptor_header *) desc)->bLength;
			desc = desc + ((usb_ecm_descriptor_header *) desc)->bLength;

			if (*length >= leftlen) {
				return HAL_OK;
			}
		} else {
			return HAL_OK;
		}
	}
	return HAL_OK;
}

/**
  * @brief	Parse configuration descriptor
  * @param	host: usb host structure
  * @retval Status
  */
u8 usbh_cdc_ecm_parse_cfgdesc(usb_host_t *host)
{
	u8 ret = HAL_OK;
	usbh_cfg_desc_t *desc = (usbh_cfg_desc_t *)usbh_get_raw_configuration_descriptor(host);
	u32 cfglen = (u32)desc->wTotalLength;
	usbh_if_desc_t *pbuf = (usbh_if_desc_t *) desc;
	u32 len = 0;

	while (1) {
		pbuf = (usbh_if_desc_t *)usbh_cdc_ecm_find_next_stdesc((u8 *)pbuf, &cfglen);
		if (pbuf == NULL) {
			break;
		}

		switch (pbuf->bInterfaceClass) {
		case CDC_COMM_INTERFACE_CLASS_CODE: //cdc control
			len = 0;
			ret = usbh_cdc_ecm_parse_control((u8 *)pbuf, &len);
			if (HAL_OK != ret) {
				DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Ecm parse control fail\n");
				return ret;
			}
			pbuf = (usbh_if_desc_t *)((u8 *) pbuf + len);
			cfglen -= len;
			break;

		case CDC_DATA_INTERFACE_CLASS_CODE: //cdc data
			if (pbuf->bAlternateSetting == 0) { //alt setting
				len = ((usb_ecm_descriptor_header *) pbuf)->bLength;
				pbuf = (usbh_if_desc_t *)((u8 *) pbuf + len);
				cfglen -= len;
			} else {
				len = 0;
				ret = usbh_cdc_ecm_parse_data((u8 *)pbuf, &len, cfglen);
				if (HAL_OK != ret) {
					DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Ecm parse data fail\n");
					return ret;
				}

				pbuf = (usbh_if_desc_t *)((u8 *) pbuf + len);
				cfglen -= len;
			}
			break;

		default:
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Ecm  subclass(%d) is not control or data\n", pbuf->bInterfaceClass);
			return HAL_ERR_PARA;
		}
	}

	return ret;
}


/**
  * @brief  Usb Attach callback function
  * @param  host: Host handle
  * @retval Status
  */
static u8 usbh_cdc_ecm_attach(usb_host_t *host)
{
	u8 status = HAL_ERR_UNKNOWN;
	u8 pipe_num;
	u32 max_ep_size;
//	u32 rx_fifo_size = host->config.rx_fifo_size;

	usbh_cdc_ecm_host_t 		  *ecm = &usbh_cdc_ecm_host;
	usbh_cdc_ecm_comm_if_t        *pcomm_if = &(ecm->comm_if);
	usbh_cdc_ecm_data_if_t        *pdata_if = &(ecm->data_if);
	ecm->usbh_state = USBH_CDC_ECM_ATTACH;
	ecm->host = host;

	if (host->config.speed == USB_SPEED_HIGH) {
		max_ep_size = USB_HS_MAX_PACKET_SIZE;
	} else {
		max_ep_size = USB_FS_MAX_PACKET_SIZE;
	}
	//max_ep_size = max_ep_size > rx_fifo_size ? rx_fifo_size : max_ep_size;

	status = usbh_cdc_ecm_parse_cfgdesc(host);
	if (status) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Fail to parse ecm class specific descriptor\n");
		return status;
	}
	dump_ecm_cfgdesc();

	//limited the max ep size
	if (pdata_if->bulk_in_packet_size >= max_ep_size) {
		pdata_if->bulk_in_packet_size = max_ep_size;
	}
	if (pdata_if->bulk_out_packet_size >= max_ep_size) {
		pdata_if->bulk_out_packet_size = max_ep_size;
	}
	if (pcomm_if->intr_in_packet_size >= max_ep_size) {
		pcomm_if->intr_in_packet_size = max_ep_size;
	}

	//control
	pipe_num = usbh_alloc_pipe(host, pcomm_if->intr_in_ep);
	if (pipe_num != 0xFFU) {
		pcomm_if->intr_in_pipe = pipe_num;
	} else {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Fail to allocate INTR IN pipe for CDC ECM class.");
		return HAL_ERR_MEM;
	}
	usbh_open_pipe(host,
				   pcomm_if->intr_in_pipe,
				   pcomm_if->intr_in_ep,
				   USB_CH_EP_TYPE_INTR,
				   pcomm_if->intr_in_packet_size);

	//data
	pipe_num = usbh_alloc_pipe(host, pdata_if->bulk_out_ep);
	if (pipe_num != 0xFFU) {
		pdata_if->bulk_out_pipe = pipe_num;
	} else {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Fail to allocate BULK OUT pipe for CDC ECM class.");
		usbh_free_pipe(host, pcomm_if->intr_in_pipe);
		return HAL_ERR_MEM;
	}

	pipe_num = usbh_alloc_pipe(host, pdata_if->bulk_in_ep);
	if (pipe_num != 0xFFU) {
		pdata_if->bulk_in_pipe = pipe_num;
	} else {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Fail to allocate BULK IN pipe for CDC ECM class.");
		usbh_free_pipe(host, pcomm_if->intr_in_pipe);
		usbh_free_pipe(host, pdata_if->bulk_out_pipe);
		return HAL_ERR_MEM;
	}

	usbh_open_pipe(host,
				   pdata_if->bulk_out_pipe,
				   pdata_if->bulk_out_ep,
				   USB_CH_EP_TYPE_BULK,
				   pdata_if->bulk_out_packet_size);

	usbh_open_pipe(host,
				   pdata_if->bulk_in_pipe,
				   pdata_if->bulk_in_ep,
				   USB_CH_EP_TYPE_BULK,
				   pdata_if->bulk_in_packet_size);

	//dump ecm data
	DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "[in:epaddr=0x%x,chan=%d packsize=%d][out:epaddr=0x%x,chan=%d,packsize=%d]\n",
			   pdata_if->bulk_in_ep, pdata_if->bulk_in_pipe, pdata_if->bulk_in_packet_size,
			   pdata_if->bulk_out_ep, pdata_if->bulk_out_pipe, pdata_if->bulk_in_packet_size);

	ecm->state = CDC_ECM_STATE_IDLE;

	if ((ecm->cb != NULL) && (ecm->cb->attach != NULL)) {
		ecm->cb->attach();
	}
	status = HAL_OK;
	return status;
}


/**
  * @brief  Usb Detach callback function.
  * @param  host: Host handle
  * @retval HAL_OK
  */
static u8 usbh_cdc_ecm_detach(usb_host_t *host)
{
	usbh_cdc_ecm_host_t *cdc = &usbh_cdc_ecm_host;
	cdc->usbh_state = USBH_CDC_ECM_DETACHED;

	if ((cdc->cb != NULL) && (cdc->cb->detach != NULL)) {
		cdc->cb->detach();
	}

	if (cdc->comm_if.intr_in_pipe) {
		usbh_close_pipe(host, cdc->comm_if.intr_in_pipe);
		usbh_free_pipe(host, cdc->comm_if.intr_in_pipe);
		cdc->comm_if.intr_in_pipe = 0U;
	}

	if (cdc->data_if.bulk_in_pipe) {
		usbh_close_pipe(host, cdc->data_if.bulk_in_pipe);
		usbh_free_pipe(host, cdc->data_if.bulk_in_pipe);
		cdc->data_if.bulk_in_pipe = 0U;
	}

	if (cdc->data_if.bulk_out_pipe) {
		usbh_close_pipe(host, cdc->data_if.bulk_out_pipe);
		usbh_free_pipe(host, cdc->data_if.bulk_out_pipe);
		cdc->data_if.bulk_out_pipe = 0U;
	}

	return HAL_OK;
}

/**
  * @brief  Usb Setup callback function.
  * @param  host: Host handle
  * @retval Status
  */
static u8 usbh_cdc_ecm_setup(usb_host_t *host)
{
	u8 i = 0;
	u8 status = HAL_ERR_UNKNOWN ;
	usbh_cdc_ecm_host_t *cdc = &usbh_cdc_ecm_host;

	/*Issue the get line coding request*/
	status = usbh_cdc_acm_process_get_string(host, cdc->mac_string, CDC_ECM_MAC_STRING_LEN, cdc->func_if.iMACAddress);
	if (status == HAL_OK) {
		for (i = 0; i < 6; i++) {
			cdc->mac[i] = hex_to_char(cdc->mac_string[2 + 4 * i]) * 16 + hex_to_char(cdc->mac_string[2 + 4 * i + 2]) ;
		}

		cdc->usbh_state = USBH_CDC_ECM_SETUP;
		if ((cdc->cb != NULL) && (cdc->cb->setup != NULL)) {
			cdc->cb->setup();
		}
	}

	return status;
}
/**
  * @brief  Usb Sof callback function.
  * @param  host: Host handle
  * @retval HAL_OK
  */
static u8 usbh_cdc_ecm_sof(usb_host_t *host)
{
	u8 i = 0;
	usbh_cdc_ecm_host_t *cdc = &usbh_cdc_ecm_host;

	for (i = 0U; i < host->config.pipes; i++) {
		if (cdc->host_pipe[i]) {
			cdc->host_pipe[i] = 0;
			usbh_reactivate_pipe(host, i);
		}
	}

	return HAL_OK;
}
/**
  * @brief  Usb nak callback function.
  * @param  host: Host handle
  * @param  pipe_num: Pipe number
  * @retval return OK while success, else return HAL_ERR_UNKNOWN
  */
static u8 usbh_cdc_ecm_nak(usb_host_t *host, u8 pipe_num)
{
	u8 ep_type ;
	usbh_cdc_ecm_host_t *cdc = &usbh_cdc_ecm_host;
	if (pipe_num > CDC_ECM_HOST_PIPE_COUNT) {
		return HAL_ERR_PARA ;
	}

	ep_type = usbh_get_ep_type(host, pipe_num);
	if (ep_type == USB_CH_EP_TYPE_BULK || ep_type == USB_CH_EP_TYPE_INTR) {
		//do not retransmit in this loop, it will do in next SOF
		cdc->host_pipe[pipe_num] = 1;
		return HAL_OK;
	}

	return HAL_ERR_UNKNOWN ;
}


/**
  * @brief  Usb State Machine handling callback
  * @param  host: Host handle
  * @retval Status
  */
static u8 usbh_cdc_ecm_process(usb_host_t *host)
{
	u8 req_status = HAL_OK;
	usbh_cdc_ecm_host_t *cdc = &usbh_cdc_ecm_host;

	switch (cdc->state) {

	case CDC_ECM_STATE_IDLE:
		break;

	case CDC_ECM_STATE_ALT_SETTING:
		req_status = usbh_cdc_ecm_process_set_alt(host);
		if (req_status == HAL_OK) {
			cdc->state = CDC_ECM_STATE_IDLE;
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "Ecm alt setting success\n");
		} else if (req_status != HAL_BUSY) {
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "Ecm alt setting error\n");
			cdc->state = CDC_ECM_STATE_ERROR;
		}
		break;

	case CDC_ECM_STATE_SET_ETHERNET_MULTICAST_FILTER:
		req_status = usbh_cdc_ecm_process_set_muiticast_filter(host);
		if (req_status == HAL_OK) {
			cdc->state = CDC_ECM_STATE_IDLE;
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "Ecm set eth multicast filter success\n");
		} else if (req_status != HAL_BUSY) {
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "Ecm set eth multicast filter error\n");
			cdc->state = CDC_ECM_STATE_ERROR;
		}
		break;

	case CDC_ECM_STATE_SET_ETHERNET_PACKET_FILTER:
		req_status = usbh_cdc_ecm_process_set_packet_filter(host);
		if (req_status == HAL_OK) {
			cdc->state = CDC_ECM_STATE_IDLE;
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "Ecm set eth packet filter success\n");
		} else if (req_status != HAL_BUSY) {
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "Ecm set eth packet filter error\n");
			cdc->state = CDC_ECM_STATE_ERROR;
		}
		break;

	case CDC_ECM_STATE_GET_ETHERNET_STATISTIC:
		req_status = usbh_cdc_ecm_process_get_statistic(host);
		if (req_status == HAL_OK) {
			cdc->state = CDC_ECM_STATE_IDLE;
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "Ecm get eth statistic success\n");
		} else if (req_status != HAL_BUSY) {
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "Ecm get eth statistic error\n");
			cdc->state = CDC_ECM_STATE_ERROR;
		}
		break;

	case CDC_ECM_STATE_SWITCH_TO_TRANSFER:
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "Ecm switch to transfer \n");
		req_status = HAL_OK;
		cdc->state = CDC_ECM_STATE_TRANSFER;
		break;

	case CDC_ECM_STATE_TRANSFER:
		usbh_cdc_ecm_process_bulk_in(host);
		usbh_cdc_ecm_process_bulk_out(host);
		usbh_cdc_ecm_process_intr_in(host);
		if (usbh_cdc_ecm_host.next_transfor) {
			usbh_cdc_ecm_host.next_transfor = 0;
			usbh_notify_class_state_change(host);
		}
		break;

	case CDC_ECM_STATE_ERROR:
		req_status = usbh_ctrl_clear_feature(host, 0x00U);
		if (req_status == HAL_OK) {
			cdc->state = CDC_ECM_STATE_IDLE;
		}
		break;

	default:
		break;

	}

	return req_status;
}

/**
  * @brief  Usb alt setting function
  * @param  host: Host handle
  * @retval Status
*/
static u8 usbh_cdc_ecm_process_set_alt(usb_host_t *host)
{
	usbh_setup_req_t setup;

	setup.b.bmRequestType = USB_H2D | USB_REQ_TYPE_STANDARD | USB_REQ_RECIPIENT_INTERFACE;
	setup.b.bRequest = USB_REQ_SET_INTERFACE;
	setup.b.wValue = 1;
	setup.b.wIndex = 1;
	setup.b.wLength = 0U;

	return usbh_ctrl_request(host, &setup, NULL);
}

/**
  * @brief  Bulk Out handling function
  * @param  host: Host handle
  * @retval Status
*/
static void usbh_cdc_ecm_process_bulk_out(usb_host_t *host)
{
	usbh_cdc_ecm_host_t *cdc = &usbh_cdc_ecm_host;
	usbh_urb_state_t urb_state = USBH_URB_IDLE;

	switch (cdc->bulk_data_out_state) {
	case CDC_ECM_TRANSFER_STATE_TX:
		if (cdc->bulk_data_out_len > cdc->data_if.bulk_out_packet_size) {
			usbh_bulk_send_data(host,
								cdc->bulk_data_out_buf,
								cdc->data_if.bulk_out_packet_size,
								cdc->data_if.bulk_out_pipe);
		} else {
			usbh_bulk_send_data(host,
								cdc->bulk_data_out_buf,
								(u16)cdc->bulk_data_out_len,
								cdc->data_if.bulk_out_pipe);
		}
		cdc->bulk_data_out_state = CDC_ECM_TRANSFER_STATE_TX_BUSY;
		cdc->next_transfor = 1;
		break;

	case CDC_ECM_TRANSFER_STATE_TX_BUSY:
		urb_state = usbh_get_urb_state(host, cdc->data_if.bulk_out_pipe);
		if (urb_state == USBH_URB_DONE) {
			if (cdc->bulk_data_out_len >= cdc->data_if.bulk_out_packet_size) {
				cdc->bulk_data_out_len -= cdc->data_if.bulk_out_packet_size;
				cdc->bulk_data_out_buf += cdc->data_if.bulk_out_packet_size;
				cdc->bulk_data_out_state = CDC_ECM_TRANSFER_STATE_TX;
			} else {
				cdc->bulk_data_out_len = 0U;
				cdc->bulk_data_out_state = CDC_ECM_TRANSFER_STATE_IDLE;
				if ((cdc->cb != NULL) && (cdc->cb->bulk_send != NULL)) {
					cdc->cb->bulk_send(urb_state);
				}
			}
			cdc->next_transfor = 1;
		} else if (urb_state == USBH_URB_BUSY) { //retransmit
			cdc->bulk_data_out_state = CDC_ECM_TRANSFER_STATE_TX;
			cdc->next_transfor = 1;
		} else if ((urb_state == USBH_URB_ERROR) || (urb_state == USBH_URB_STALL)) {
			cdc->bulk_data_out_state = CDC_ECM_TRANSFER_STATE_IDLE;
			if ((cdc->cb != NULL) && (cdc->cb->bulk_send != NULL)) {
				cdc->cb->bulk_send(urb_state);
			}
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "BULK out failed %d\n", urb_state);
		}
		break;

	default:
		break;
	}
}


/**
  * @brief  Bulk In handling  function
  * @param  host: Host handle
  * @retval Status
*/
static void usbh_cdc_ecm_process_bulk_in(usb_host_t *host)
{
	usbh_cdc_ecm_host_t *cdc = &usbh_cdc_ecm_host;
	usbh_urb_state_t urb_state = USBH_URB_IDLE;
	u32 len;

	switch (cdc->bulk_data_in_state) {
	case CDC_ECM_TRANSFER_STATE_RX:
		usbh_bulk_receive_data(host,
							   cdc->bulk_data_in_buf,
							   cdc->data_if.bulk_in_packet_size,
							   cdc->data_if.bulk_in_pipe);
		cdc->bulk_data_in_state = CDC_ECM_TRANSFER_STATE_RX_BUSY;
		cdc->next_transfor = 1;
		break;

	case CDC_ECM_TRANSFER_STATE_RX_BUSY:
		urb_state = usbh_get_urb_state(host, cdc->data_if.bulk_in_pipe);
		if (urb_state == USBH_URB_DONE) {
			len = usbh_get_last_transfer_size(host, cdc->data_if.bulk_in_pipe);
			if ((cdc->cb != NULL) && (cdc->cb->bulk_receive != NULL)) {
				cdc->cb->bulk_receive(cdc->bulk_data_in_buf, len);
			}
			if (len >= cdc->data_if.bulk_in_packet_size) {	//it is not the last packet
				cdc->bulk_data_in_state = CDC_ECM_TRANSFER_STATE_RX;
			} else {
				cdc->bulk_data_in_state = CDC_ECM_TRANSFER_STATE_IDLE;  //set rx to accelerate speed
			}
			cdc->next_transfor = 1;
		} else if (urb_state == USBH_URB_BUSY) {
		} else if ((urb_state == USBH_URB_ERROR) || (urb_state == USBH_URB_STALL)) {
			cdc->bulk_data_in_state = CDC_ECM_TRANSFER_STATE_IDLE;
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "BULK in failed %d\n", urb_state);
		}
		break;

	default:
		break;
	}
}
/**
  * @brief  Intr in function
  * @param  host: Host handle
  * @retval Status
*/
static void usbh_cdc_ecm_process_intr_in(usb_host_t *host)
{
	usbh_cdc_ecm_host_t *cdc = &usbh_cdc_ecm_host;
	usbh_urb_state_t urb_state = USBH_URB_IDLE;
	u32 len;

	switch (cdc->intr_in_state) {
	case CDC_ECM_INT_TRANSFER_STATE_RX:
		usbh_intr_receive_data(host,
							   cdc->intr_in_buf,
							   cdc->comm_if.intr_in_packet_size,
							   cdc->comm_if.intr_in_pipe);

		cdc->intr_in_state = CDC_ECM_INT_TRANSFER_STATE_RX_BUSY;
		cdc->next_transfor = 1;
		break;

	case CDC_ECM_INT_TRANSFER_STATE_RX_BUSY:
		urb_state = usbh_get_urb_state(host, cdc->comm_if.intr_in_pipe);
		if (urb_state == USBH_URB_DONE) {
			len = usbh_get_last_transfer_size(host, cdc->comm_if.intr_in_pipe);
			if (len && (cdc->cb != NULL) && (cdc->cb->intr_received != NULL)) {
				cdc->cb->intr_received(cdc->intr_in_buf, len);
			}
			if (len >= cdc->comm_if.intr_in_packet_size) {
				cdc->intr_in_state = CDC_ECM_INT_TRANSFER_STATE_RX;
			} else {
				cdc->intr_in_state = CDC_ECM_TRANSFER_STATE_IDLE;
			}
			cdc->next_transfor = 1;
		} else if (urb_state == USBH_URB_BUSY) {
		} else if ((urb_state == USBH_URB_ERROR) || (urb_state == USBH_URB_STALL)) {
			cdc->intr_in_state = CDC_ECM_TRANSFER_STATE_IDLE;
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "INTR in failed %d\n", urb_state);
		} else if (urb_state == USBH_URB_IDLE) {
			//todo fix this issue:
			//while run usbh_intr_receive_data , but the interrupt not happen
			cdc->intr_idle_state_cnt ++ ;
			if (cdc->intr_idle_state_cnt > 10) {
				cdc->intr_idle_state_cnt = 0  ;
				cdc->intr_in_state = CDC_ECM_TRANSFER_STATE_IDLE;
				cdc->next_transfor = 1;
			}
		}
		break;

	default:
		break;
	}
}
/**
  * @brief  Get string descriptor by str id
  * @param  host: Host handle
  * @param  pbuf: buffer handle which will save the string value
  * @param  buflen: buffer length
  * @param  str_id: string index
  * @retval Status
  */
static u8 usbh_cdc_acm_process_get_string(usb_host_t *host, u8 *pbuf, u16 buflen, u8 str_id)
{
	usbh_setup_req_t setup;

	setup.b.bmRequestType = USB_D2H | USB_REQ_RECIPIENT_DEVICE | USB_REQ_TYPE_STANDARD;
	setup.b.bRequest = USB_REQ_GET_DESCRIPTOR;
	setup.b.wValue = USB_DESC_STRING | str_id;
	setup.b.wIndex = 1U;
	setup.b.wLength = buflen;

	return usbh_ctrl_request(host, &setup, pbuf);
}
/**
  * @brief  Set Ethernet Packet Filter
  * @param  host: Host handle
  * @retval Status
  */
static u8 usbh_cdc_ecm_process_set_packet_filter(usb_host_t *host)
{
	usbh_setup_req_t setup;
	usbh_cdc_ecm_host_t *cdc = &usbh_cdc_ecm_host;

	setup.b.bmRequestType = USB_H2D | USB_REQ_TYPE_CLASS | USB_REQ_RECIPIENT_INTERFACE;
	setup.b.bRequest = CDC_ECM_SET_ETHERNET_PACKET_FILTER;
	setup.b.wValue = cdc->packet_filter;
	setup.b.wIndex = 1U;
	setup.b.wLength = 0;

	return usbh_ctrl_request(host, &setup, NULL);
}
/**
  * @brief  Set Ethernet Muticast Filter
  * @param  host: Host handle
  * @retval Status
  */
static u8 usbh_cdc_ecm_process_set_muiticast_filter(usb_host_t *host)
{
	usbh_setup_req_t setup;
	usbh_cdc_ecm_host_t *cdc = &usbh_cdc_ecm_host;

	setup.b.bmRequestType = USB_H2D | USB_REQ_TYPE_CLASS | USB_REQ_RECIPIENT_INTERFACE;
	setup.b.bRequest = CDC_ECM_SET_ETHERNET_MULTICAST_FILTERS;
	setup.b.wValue = 1;
	setup.b.wIndex = 1U;
	setup.b.wLength = cdc->muticast_filter_len;

	return usbh_ctrl_request(host, &setup, cdc->muticast_filter);
}

/**
  * @brief  Get Ethernet Statistic
  * @param  host: Host handle
  * @retval Status
  */
static u8 usbh_cdc_ecm_process_get_statistic(usb_host_t *host)
{
	usbh_setup_req_t setup;
	usbh_cdc_ecm_host_t *cdc = &usbh_cdc_ecm_host;

	setup.b.bmRequestType = USB_D2H | USB_REQ_TYPE_CLASS | USB_REQ_RECIPIENT_INTERFACE;
	setup.b.bRequest = CDC_ECM_GET_ETHERNET_STATISTIC;
	setup.b.wValue = cdc->feature_selector;
	setup.b.wIndex = 1U;
	setup.b.wLength = 4;

	return usbh_ctrl_request(host, &setup, (u8 *)&cdc->eth_statistic_count);
}

/* Exported functions --------------------------------------------------------*/
/*
	below is the private apis that called by ecm_hal
*/

/**
  * @brief  Init CDC ECM class
  * @param  cb: User callback
  * @retval Status
  */
u8 usbh_cdc_ecm_init(usbh_cdc_ecm_state_cb_t *cb)
{
	u8 ret = HAL_OK;
	usbh_cdc_ecm_host_t *cdc = &usbh_cdc_ecm_host;
	memset(cdc, 0x00, sizeof(usbh_cdc_ecm_host_t));

	if (cb != NULL) {
		cdc->cb = cb;
		if (cb->init != NULL) {
			ret = cb->init();
			if (ret != HAL_OK) {
				DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "CDC ECM user init error: %d", ret);
				return ret;
			}
		}
	}
	cdc->usbh_state = USBH_CDC_ECM_IDLE;

	usbh_register_class(&usbh_cdc_ecm_driver);

	return ret;
}

/**
  * @brief  Deinit CDC ECM class
  * @retval Status
  */
u8 usbh_cdc_ecm_deinit(void)
{
	u8 ret = HAL_OK;
	usbh_cdc_ecm_host_t *cdc = &usbh_cdc_ecm_host;
	usb_host_t *host = cdc->host;

	if ((cdc->cb != NULL) && (cdc->cb->deinit != NULL)) {
		cdc->cb->deinit();
	}

	if ((host != NULL) && (host->state == USBH_CLASS_READY)) {
		cdc->state = CDC_ECM_STATE_IDLE;

		usbh_close_pipe(host, cdc->comm_if.intr_in_pipe);
		usbh_close_pipe(host, cdc->data_if.bulk_in_pipe);
		usbh_close_pipe(host, cdc->data_if.bulk_out_pipe);
	}

	usbh_unregister_class(&usbh_cdc_ecm_driver);

	return ret;
}

/**
  * @brief  Start to send bulk out data
  * @param  buf: Data buffer
  * @param  len: Data length
  * @retval Status
  */
u8 usbh_cdc_ecm_bulk_send(u8 *buf, u32 len)
{
	u8 ret = HAL_BUSY;
	usbh_cdc_ecm_host_t *cdc = &usbh_cdc_ecm_host;
	usb_host_t *host = cdc->host;
	if (cdc->usbh_state < USBH_CDC_ECM_SETUP) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Usb Not Setup success,just return[%s]\n", __func__);
		return ret;
	}

	if ((cdc->state == CDC_ECM_STATE_IDLE) || (cdc->state == CDC_ECM_STATE_TRANSFER)) {
		if (cdc->bulk_data_out_state == CDC_ECM_TRANSFER_STATE_IDLE) {
			cdc->bulk_data_out_buf = buf;
			cdc->bulk_data_out_len = len;
			cdc->bulk_data_out_state = CDC_ECM_TRANSFER_STATE_TX;
			cdc->state = CDC_ECM_STATE_TRANSFER;
			usbh_notify_class_state_change(host);
			ret = HAL_OK;
		} else {
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "BULK OUT %d: busy\n", len);
		}
	} else {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "BULK OUT not ready\n");
	}

	return ret;
}

/**
  * @brief  Start to receive bulk in data
  * @param  none
  * @retval Status
  */
u8 usbh_cdc_ecm_bulk_receive(void)
{
	u8 ret = HAL_BUSY;
	usbh_cdc_ecm_host_t *cdc = &usbh_cdc_ecm_host;
	usb_host_t *host = cdc->host;

	if (usbh_cdc_ecm_usb_status_check() != HAL_OK) {
		return ret;
	}
	if ((cdc->state == CDC_ECM_STATE_IDLE) || (cdc->state == CDC_ECM_STATE_TRANSFER)) {
		if (cdc->bulk_data_in_state == CDC_ECM_TRANSFER_STATE_IDLE) {
			cdc->bulk_data_in_state = CDC_ECM_TRANSFER_STATE_RX;
			cdc->state = CDC_ECM_STATE_TRANSFER;
			usbh_notify_class_state_change(host);
			ret = HAL_OK;
		} else {
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "BULK OUT busy %d\n", cdc->bulk_data_in_state);
		}
	} else {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "BULK OUT not ready\n");
	}

	return ret;
}

/**
  * @brief  Start to receive INTR data
  * @param  buf: Data buffer
  * @param  len: Data length
  * @retval Status
  */
u8 usbh_cdc_ecm_intr_receive(u8 *buf, u32 len)
{
	u8 ret = HAL_BUSY;
	usbh_cdc_ecm_host_t *cdc = &usbh_cdc_ecm_host;
	usbh_cdc_ecm_comm_if_t *pcomm_if = &(cdc->comm_if);
	usb_host_t *host = cdc->host;

	if (usbh_cdc_ecm_usb_status_check() != HAL_OK) {
		return ret;
	}
	if ((cdc->state == CDC_ECM_STATE_IDLE) || (cdc->state == CDC_ECM_STATE_TRANSFER)) {
		if (cdc->intr_in_state == CDC_ECM_TRANSFER_STATE_IDLE) {
			cdc->intr_in_buf = buf;
			cdc->intr_in_state = CDC_ECM_INT_TRANSFER_STATE_RX;
			cdc->state = CDC_ECM_STATE_TRANSFER;
			usbh_notify_class_state_change(host);
			if (len < pcomm_if->intr_in_packet_size) {
				pcomm_if->intr_in_packet_size = len;
				DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "[USB]Notice,Please enlarge the inbuffer length\n");
			}
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
  * @brief  set config descriptor index while there has more then one config descriptor
  * @param  host: Host handle
  * @param  cfg_num: config index
  * @retval Status
  */
u8 usbh_cdc_ecm_setconfig(usb_host_t *host, u8 cfg_num)
{
	return usbh_set_configuration(host, cfg_num);
}

/**
  * @brief  get thernet statistic info
  * @param  selector: selectot index
  * @retval Status
  */
u8 usbh_cdc_ecm_get_ethernet_statistic(u16 selector)
{
	u8 ret = HAL_BUSY;
	usbh_cdc_ecm_host_t *cdc = &usbh_cdc_ecm_host;
	usb_host_t *host = cdc->host;

	if (usbh_cdc_ecm_usb_status_check() != HAL_OK) {
		return ret;
	}
	if ((cdc->state == CDC_ECM_STATE_IDLE) || (cdc->state == CDC_ECM_STATE_GET_ETHERNET_STATISTIC)) {
		cdc->state = CDC_ECM_STATE_GET_ETHERNET_STATISTIC;
		cdc->feature_selector = selector;
		usbh_notify_class_state_change(host);
		ret = HAL_OK;
	}

	return ret;
}

/**
  * @brief  set thernet packet filter
  * @param  filter: filer value,
  * @retval Status
  */
u8 usbh_cdc_ecm_set_ethernet_packetfilter(u16 filter)
{
	u8 ret = HAL_BUSY;
	usbh_cdc_ecm_host_t *cdc = &usbh_cdc_ecm_host;
	usb_host_t *host = cdc->host;

	if (usbh_cdc_ecm_usb_status_check() != HAL_OK) {
		return ret;
	}

	if ((cdc->state == CDC_ECM_STATE_IDLE) || (cdc->state == CDC_ECM_STATE_SET_ETHERNET_PACKET_FILTER)) {
		cdc->state = CDC_ECM_STATE_SET_ETHERNET_PACKET_FILTER;
		cdc->packet_filter = filter;
		usbh_notify_class_state_change(host);
		ret = HAL_OK;
	}

	return ret;
}

/**
  * @brief  set thernet multicast filter array
  * @param  filter: multicast buffer handle
  			len: buffer length
  * @retval Status
  */
u8 usbh_cdc_ecm_set_ethernet_multicast_filter(u8 *filter, u8 len)
{
	u8 ret = HAL_BUSY;
	usbh_cdc_ecm_host_t *cdc = &usbh_cdc_ecm_host;
	usb_host_t *host = cdc->host;

	if (usbh_cdc_ecm_usb_status_check() != HAL_OK) {
		return ret;
	}

	if ((cdc->state == CDC_ECM_STATE_IDLE) || (cdc->state == CDC_ECM_STATE_SET_ETHERNET_MULTICAST_FILTER)) {
		cdc->state = CDC_ECM_STATE_SET_ETHERNET_MULTICAST_FILTER;
		memcpy(cdc->muticast_filter, filter, len);
		cdc->muticast_filter_len = len;
		usbh_notify_class_state_change(host);
		ret = HAL_OK;
	}

	return ret;
}

/**
  * @brief  set thernet packet filter
  * @param  NONE
  * @retval Status
  */
u8 usbh_cdc_ecm_set_ethernet_start_transfer(void)
{
	u8 ret = HAL_BUSY;
	usbh_cdc_ecm_host_t *cdc = &usbh_cdc_ecm_host;
	usb_host_t *host = cdc->host;

	if (usbh_cdc_ecm_usb_status_check() != HAL_OK) {
		return ret;
	}

	if ((cdc->state == CDC_ECM_STATE_IDLE) || (cdc->state == CDC_ECM_STATE_SWITCH_TO_TRANSFER)) {
		cdc->state = CDC_ECM_STATE_SWITCH_TO_TRANSFER;
		usbh_notify_class_state_change(host);
		ret = HAL_OK;
	}

	return ret;
}

/**
  * @brief  set the alternate setting
  * @param  NONE
  * @retval Status
  */
u8 usbh_cdc_ecm_alt_setting(void)
{
	usbh_cdc_ecm_host_t *cdc = &usbh_cdc_ecm_host;
	usb_host_t *host = cdc->host;
	int ret = HAL_ERR_UNKNOWN;

	if (usbh_cdc_ecm_usb_status_check() != HAL_OK) {
		return ret;
	}

	if ((host->state == USBH_CLASS_REQUEST) || (host->state == USBH_CLASS_READY)) {
		cdc->state = CDC_ECM_STATE_ALT_SETTING;
		usbh_notify_class_state_change(host);
		ret = HAL_OK;
	}

	return ret;
}
/**
  * @brief  get the bulk in process status
  * @param  NONE
  * @retval Status
  */
u8 usbh_cdc_ecm_bulk_in_processing(void)
{
	usbh_cdc_ecm_host_t *cdc = &usbh_cdc_ecm_host;

	return !((cdc->state == CDC_ECM_STATE_IDLE) || (cdc->bulk_data_in_state == CDC_ECM_TRANSFER_STATE_IDLE)) ;
}
/**
  * @brief  get the intr in process status
  * @param  NONE
  * @retval Status
  */
u8 usbh_cdc_ecm_intr_in_processing(void)
{
	usbh_cdc_ecm_host_t *cdc = &usbh_cdc_ecm_host;

	return !((cdc->state == CDC_ECM_STATE_IDLE) || (cdc->intr_in_state == CDC_ECM_TRANSFER_STATE_IDLE)) ;
}

/**
  * @brief  return bulkin MPS
  * @retval Status
  */
u16 usbh_cdc_ecm_get_usbin_mps(void)
{
	u32 i = 0;
	usbh_cdc_ecm_host_t *cdc = &usbh_cdc_ecm_host;
	while (cdc->usbh_state < USBH_CDC_ECM_SETUP && i < 10) {
		usb_os_delay_ms(1000);
		i++;
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "Ecm hostState=%d,want=%d,keep wait ...\n", cdc->usbh_state, USBH_CDC_ECM_SETUP);
	}
	return cdc->data_if.bulk_in_packet_size;
}

/* Exported functions --------------------------------------------------------*/
/*
	below is the public apis that called by lwip
*/
/**
  * @brief  get the mac str
  * @retval mac string,the length is 6 Bytes
  */
const u8 *usbh_cdc_ecm_process_mac_str(void)
{
	u32 i = 0;
	usbh_cdc_ecm_host_t *cdc = &usbh_cdc_ecm_host;
	while (cdc->usbh_state < USBH_CDC_ECM_SETUP && i < 10) {
		usb_os_delay_ms(1000);
		i++;
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "Ecm hostState=%d,want=%d,keep wait get mac string!\n", cdc->usbh_state, USBH_CDC_ECM_SETUP);
	}
	return cdc->mac;
}

