/**
  ******************************************************************************
  * @file    usbd_cdc_acm.c
  * @author  Realsil WLAN5 Team
  * @brief   This file provides the functionalities of the USB CDC Class
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

#include "usbd_hid.h"
#include "osdep_service.h"

/* Private defines -----------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

static u8 hid_setup(usb_dev_t *dev, usb_setup_req_t *req);
static u8 hid_get_class_descriptor(usb_dev_t *dev, usb_setup_req_t *req);
static u8 hid_set_config(usb_dev_t *dev, u8 config);
static u8 hid_clear_config(usb_dev_t *dev, u8 config);
static u8 hid_handle_ep_data_in(usb_dev_t *dev, u8 ep_num, u8 status);
static u8 *hid_get_descriptor(usb_setup_req_t *req, usb_speed_type_t speed, u16 *len);

/* Private variables ---------------------------------------------------------*/

/* USB Standard Device Descriptor */
static u8 usbd_hid_dev_desc[USB_LEN_DEV_DESC] USB_DMA_ALIGNED = {
	USB_LEN_DEV_DESC,								/* bLength */
	USB_DESC_TYPE_DEVICE,							/* bDescriptorType */
	0x00,											/* bcdUSB */
	0x02,
	0x00,											/* bDeviceClass */
	0x00,											/* bDeviceSubClass */
	0x00,											/* bDeviceProtocol */
	USB_MAX_EP0_SIZE,								/* bMaxPacketSize */
	USB_LOW_BYTE(HID_VID),							/* idVendor */
	USB_HIGH_BYTE(HID_VID),
	USB_LOW_BYTE(HID_PID),							/* idProduct */
	USB_HIGH_BYTE(HID_PID),
	0x00,											/* bcdDevice */
	0x02,
	USBD_HID_STRING_MANUFACTURER,					/* iManufacturer */
	USBD_HID_STRING_PRODUCT,						/* iProduct */
	USBD_HID_STRING_SERIALNUMBER,					/* iSerialNumber */
	0x01											/* bNumConfigurations */
};  /* usbd_hid_dev_desc */


/* USB Standard String Descriptor 0 */
static u8 usbd_hid_lang_id_desc[USB_LEN_LANGID_STR_DESC] USB_DMA_ALIGNED = {
	USB_LEN_LANGID_STR_DESC,                        /* bLength */
	USB_DESC_TYPE_STRING,                           /* bDescriptorType */
	USB_LOW_BYTE(HID_LANGID_STRING),            	/* wLANGID */
	USB_HIGH_BYTE(HID_LANGID_STRING),
};  /* usbd_hid_lang_id_desc */

/* USB Standard Manufacture String Descriptor */
static u8 usbd_hid_mfg_string_desc[HID_MFG_STRING_DESC_SIZE] USB_DMA_ALIGNED = {
	HID_MFG_STRING_DESC_SIZE,                   	/* bLength */
	USB_DESC_TYPE_STRING,                           /* bDescriptorType */
	'R', 0,                                         /* bString, UNICODE */
	'e', 0,
	'a', 0,
	'l', 0,
	't', 0,
	'e', 0,
	'k', 0
};  /* usbd_hid_mfg_string_desc */

/* USB Standard Product String Descriptor for high-speed mode */
static u8 usbd_hid_hs_product_string_desc[HID_PRODUCT_STRING_DESC_SIZE] USB_DMA_ALIGNED = {
	HID_PRODUCT_STRING_DESC_SIZE,               	/* bLength */
	USB_DESC_TYPE_STRING,                           /* bDescriptorType */
	'R', 0,                                         /* bString, UNICODE */
	'e', 0,
	'a', 0,
	'l', 0,
	't', 0,
	'e', 0,
	'k', 0,
	' ', 0,
	'H', 0,
	'I', 0,
	'D', 0,
	' ', 0,
	'(', 0,
	'H', 0,
	'S', 0,
	')', 0
};  /* usbd_hid_hs_product_string_desc */

/* USB Standard Product String Descriptor for high-speed mode */
static u8 usbd_hid_fs_product_string_desc[HID_PRODUCT_STRING_DESC_SIZE] USB_DMA_ALIGNED = {
	HID_PRODUCT_STRING_DESC_SIZE,               	/* bLength */
	USB_DESC_TYPE_STRING,                           /* bDescriptorType */
	'R', 0,                                         /* bString, UNICODE */
	'e', 0,
	'a', 0,
	'l', 0,
	't', 0,
	'e', 0,
	'k', 0,
	' ', 0,
	'H', 0,
	'I', 0,
	'D', 0,
	' ', 0,
	'(', 0,
	'F', 0,
	'S', 0,
	')', 0
};  /* usbd_hid_fs_product_string_desc */

/* USB Standard Device Qualifier Descriptor */
static u8 usbd_hid_device_qualifier_desc[USB_LEN_DEV_QUALIFIER_DESC] USB_DMA_ALIGNED = {
	USB_LEN_DEV_QUALIFIER_DESC,                     /* bLength */
	USB_DESC_TYPE_DEVICE_QUALIFIER,                 /* bDescriptorType */
	0x00,                                           /* bcdUSB */
	0x02,
	0x00,                                           /* bDeviceClass */
	0x00,                                           /* bDeviceSubClass */
	0x00,                                           /* bDeviceProtocol */
	0x40,                                           /* bMaxPacketSize */
	0x01,                                           /* bNumConfigurations */
	0x00,                                           /* Reserved */
};  /* usbd_hid_device_qualifier_desc */


/* USB Standard SN String Descriptor, ASCII only */
static u8 usbd_hid_sn_string_desc[HID_SN_STRING_DESC_SIZE] USB_DMA_ALIGNED = {
	HID_SN_STRING_DESC_SIZE,                    	/* bLength */
	USB_DESC_TYPE_STRING,                           /* bDescriptorType */
	'1', 0,                                         /* bString, UNICODE */
	'2', 0,
	'3', 0,
	'4', 0,
	'5', 0,
	'6', 0,
	'7', 0,
	'8', 0,
	'9', 0,
	'0', 0,
	'A', 0,
	'B', 0
};  /* usbd_hid_sn_string_desc */


/* USB HID device FS Configuration Descriptor */
static u8 usbd_hid_fs_config_desc[HID_CONFIG_DESC_SIZ] USB_DMA_ALIGNED = {
	/* USB Standard Configuration Descriptor */
	USB_LEN_CFG_DESC,					/*bLength*/
	USB_DESC_TYPE_CONFIGURATION, 		/*bDescriptorType: Configuration*/
	0x22,								/*wTotalLength*/
	0x00,
	0x01,         						/*bNumInterfaces*/
	0x01,         						/*bConfigurationValue*/
	0x00,        						/*iConfiguration*/
	0x80,         						/*bmAttributes*/
	0x32,         						/*MaxPower 100 mA*/

	/* HID Interface Descriptor*/
	0x09,       						/*bLength*/
	USB_DESC_TYPE_INTERFACE,			/*bDescriptorType*/
	0x00,								/*bInterfaceNumber*/
	0x00,								/*bAlternateSetting*/
	0x01,								/*bNumEndpoints*/
	0x03,								/*bInterfaceClass: HID*/
	0x01,								/*bInterfaceSubClass: 1=BOOT, 0=no boot*/
	0x02,								/*nInterfaceProtocol: 0=none, 1=keyboard, 2=mouse*/
	0,									/*iInterface*/

	/*HID Descriptor*/
	0x09,								/*bLength*/
	HID_DESC, 							/*bDescriptorType*/
	0x11,								/*bcdHID*/
	0x01,
	0x00,								/*bCountryCode*/
	0x01,								/*bNumDescriptors*/
	HID_REPORT_DESC,					/*bDescriptorType*/
	HID_MOUSE_REPORT_DESC_SIZE,			/*wItemLength*/
	0x00,

	/* Endpoint Descriptor*/
	0x07,								/*bLength*/
	USB_DESC_TYPE_ENDPOINT,				/*bDescriptorType:*/
	HID_INTERRUPT_IN_EP_ADDRESS,	/*bEndpointAddress*/
	0x03,								/*bmAttributes*/
	0x8,								/*wMaxPacketSize*/
	0x00,
	0xA,								/*bInterval*/
};

/* USB HID device HS Configuration Descriptor */
static u8 usbd_hid_hs_config_desc[HID_CONFIG_DESC_SIZ] USB_DMA_ALIGNED = {
	/* USB Standard Configuration Descriptor */
	0x09,								/* bLength*/
	USB_DESC_TYPE_CONFIGURATION,		/* bDescriptorType*/
	0x22,								/* wTotalLength*/
	0x00,
	0x01,								/*bNumInterfaces*/
	0x01,								/*bConfigurationValue*/
	0x00,								/*iConfiguration*/
	0x80,								/*bmAttributes*/
	0x32,								/*MaxPower*/

	/* HID Interface Descriptor*/
	0x09,								/*bLength*/
	USB_DESC_TYPE_INTERFACE,			/*bDescriptorType*/
	0x00,								/*bInterfaceNumber*/
	0x00,								/*bAlternateSetting*/
	0x01,								/*bNumEndpoints*/
	0x03,								/*bInterfaceClass*/
	0x01,								/*bInterfaceSubClass: 1=BOOT, 0=no boot*/
	0x02,								/*nInterfaceProtocol: 0=none, 1=keyboard, 2=mouse*/
	0,									/*iInterface*/

	/*HID Descriptor*/
	0x09,								/*bLength*/
	HID_DESC,								/*bDescriptorType*/
	0x11,								/*bcdHID*/
	0x01,
	0x00,								/*bCountryCode*/
	0x01,								/*bNumDescriptors*/
	HID_REPORT_DESC,					/*bDescriptorType*/
	HID_MOUSE_REPORT_DESC_SIZE,			/*wItemLength*/
	0x00,

	/* Endpoint Descriptor*/
	0x07,								/*bLength*/
	USB_DESC_TYPE_ENDPOINT,				/*bDescriptorType*/
	HID_INTERRUPT_IN_EP_ADDRESS,	/*bEndpointAddress*/
	0x03,								/*bmAttributest*/
	0x8,								/*wMaxPacketSize*/
	0x00,
	0xA,								/*bInterval*/
};


/* USB HID device HS Configuration Descriptor */
static u8 usbd_hid_other_speed_config_desc[HID_CONFIG_DESC_SIZ] USB_DMA_ALIGNED = {
	/* USB Standard Configuration Descriptor */
	0x09,								/* bLength*/
	USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION,		/* bDescriptorType*/
	0x22,								/* wTotalLength*/
	0x00,
	0x01,								/*bNumInterfaces*/
	0x01,								/*bConfigurationValue*/
	0x00,								/*iConfiguration*/
	0x80,								/*bmAttributes*/
	0x32,								/*MaxPower*/

	/* HID Interface Descriptor*/
	0x09,								/*bLength*/
	USB_DESC_TYPE_INTERFACE,			/*bDescriptorType*/
	0x00,								/*bInterfaceNumber*/
	0x00,								/*bAlternateSetting*/
	0x01,								/*bNumEndpoints*/
	0x03,								/*bInterfaceClass*/
	0x01,								/*bInterfaceSubClass: 1=BOOT, 0=no boot*/
	0x02,								/*nInterfaceProtocol: 0=none, 1=keyboard, 2=mouse*/
	0,									/*iInterface*/

	/*HID Descriptor*/
	0x09,								/*bLength*/
	HID_DESC,							/*bDescriptorType*/
	0x11,								/*bcdHID*/
	0x01,
	0x00,								/*bCountryCode*/
	0x01,								/*bNumDescriptors*/
	HID_REPORT_DESC,					/*bDescriptorType*/
	HID_MOUSE_REPORT_DESC_SIZE,			/*wItemLength*/
	0x00,

	/* Endpoint Descriptor*/
	0x07,								/*bLength*/
	USB_DESC_TYPE_ENDPOINT,				/*bDescriptorType*/
	HID_INTERRUPT_IN_EP_ADDRESS,	/*bEndpointAddress*/
	0x03,								/*bmAttributest*/
	0x8,								/*wMaxPacketSize*/
	0x00,
	0xA,								/*bInterval*/
};

static u8 usbd_hid_desc[USBD_HID_DESC_SIZE] USB_DMA_ALIGNED = {
	0x09,								/*bLength: HID Descriptor size*/
	HID_DESC,							/*bDescriptorType: HID*/
	0x11,								/*bcdHID: HID Class Spec release number*/
	0x01,
	0x00,								/*bCountryCode: Hardware target country*/
	0x01,								/*bNumDescriptors: Number of HID class descriptors to follow*/
	HID_REPORT_DESC,					/*bDescriptorType*/
	HID_MOUSE_REPORT_DESC_SIZE,		/*wItemLength: Total length of Report descriptor*/
	0x00,
};

static u8 HID_MOUSE_ReportDesc[HID_MOUSE_REPORT_DESC_SIZE] USB_DMA_ALIGNED = {
	0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
	0x09, 0x02,                    // USAGE (Mouse)
	0xa1, 0x01,                    // COLLECTION (Application)
	0x09, 0x01,                    //   USAGE (Pointer)
	0xa1, 0x00,                    //   COLLECTION (Physical)
	0x05, 0x09,                    //     USAGE_PAGE (Button)
	0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
	0x29, 0x03,                    //     USAGE_MAXIMUM (Button 3)
	0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
	0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
	0x95, 0x03,                    //     REPORT_COUNT (3)
	0x75, 0x01,                    //     REPORT_SIZE (1)
	0x81, 0x02,                    //     INPUT (Data,Var,Abs)
	0x95, 0x01,                    //     REPORT_COUNT (1)
	0x75, 0x05,                    //     REPORT_SIZE (5)
	0x81, 0x03,                    //     INPUT (Cnst,Var,Abs)
	0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
	0x09, 0x30,                    //     USAGE (X)
	0x09, 0x31,                    //     USAGE (Y)
	0x09, 0x38,                    //     USAGE (Wheel)
	0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
	0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
	0x75, 0x08,                    //     REPORT_SIZE (8)
	0x95, 0x03,                    //     REPORT_COUNT (3)
	0x81, 0x06,                    //     INPUT (Data,Var,Rel)
	0xc0,                          //   END_COLLECTION
	0xc0                           // END_COLLECTION
};


usbd_class_driver_t usbd_hid_driver = {
	.get_descriptor = hid_get_descriptor,
	.set_config = hid_set_config,
	.clear_config = hid_clear_config,
	.setup = hid_setup,
	.get_class_descriptor = hid_get_class_descriptor,
	.ep_data_in = hid_handle_ep_data_in,
};


static struct usb_hid_device_t hid_device;


/* Private functions ---------------------------------------------------------*/


static u8 hid_setup(usb_dev_t *dev, usb_setup_req_t *req)
{
	struct usb_hid_device_t *hid = &hid_device;
	u8 ret = HAL_OK;

	DBG_PRINTF(MODULE_USB_CLASS,
			   LEVEL_TRACE,
			   "hid_setup bmRequestType=0x%02X bRequest=0x%02X wLength=0x%04X wValue=%x\n",
			   req->bmRequestType,
			   req->bRequest,
			   req->wLength,
			   req->wValue);

	switch (req->bmRequestType & USB_REQ_TYPE_MASK) {
	case USB_REQ_TYPE_CLASS :
		switch (req->bRequest) {
		case USBD_HID_SET_PROTOCOL:
			hid->Protocol = req->wValue;
			break;
		case USBD_HID_GET_PROTOCOL:
			usbd_ep0_transmit(dev, (u8 *) &hid->Protocol, 1);
			break;
		case USBD_HID_SET_IDLE:
			hid->IdleState = req->wValue >> 8;
			break;
		case USBD_HID_GET_IDLE:
			usbd_ep0_transmit(dev, (u8 *) &hid->IdleState, 1);
			break;
		default:
			ret = HAL_ERR_HW;
			break;
		}
		if (hid->cb->setup) {
			hid->cb->setup();
		}
		break;
	default:
		ret = HAL_ERR_HW;
		break;
	}

	return ret;
}


static u8 hid_get_class_descriptor(usb_dev_t *dev, usb_setup_req_t *req)
{
	u16 len = 0;
	u8 *pbuf = NULL;

	if (req->wValue >> 8 == HID_REPORT_DESC) {
		len = MIN(HID_MOUSE_REPORT_DESC_SIZE, req->wLength);
		pbuf = HID_MOUSE_ReportDesc;
	} else if (req->wValue >> 8 == HID_DESC) {
		/* HID Descriptor */
		len = MIN(USBD_HID_DESC_SIZE, req->wLength);
		pbuf = usbd_hid_desc;
	} else {
		return HAL_ERR_HW;
	}
	usbd_ep0_transmit(dev, pbuf, len);

	return HAL_OK;

}
static u8 hid_set_config(usb_dev_t *dev, u8 config)
{
	u8 ret = HAL_OK;
	struct usb_hid_device_t *hid = &hid_device;

	UNUSED(config);

	hid->dev = dev;
	hid->intr_in_state = 0;

	/* Init INTR IN EP */
	usbd_ep_init(dev, HID_INTERRUPT_IN_EP_ADDRESS, USB_CH_EP_TYPE_INTR, HID_DEFAULT_INT_IN_XFER_SIZE);

	return ret;
}

static u8 hid_clear_config(usb_dev_t *dev, u8 config)
{
	u8 ret = 0;

	UNUSED(config);

	/* DeInit INTR IN EP */
	usbd_ep_deinit(dev, HID_INTERRUPT_IN_EP_ADDRESS);

	return ret;
}

static u8 hid_handle_ep_data_in(usb_dev_t *dev, u8 ep_num, u8 status)
{
	struct usb_hid_device_t *hid = &hid_device;

	UNUSED(dev);

	if (status == HAL_OK) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "EP%d TX done\n", ep_num);
	} else {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "EP%d TX error: %d\n", ep_num, status);
	}

	hid->cb->transmit_complete(status);
	hid->intr_in_state = 0U;

	return HAL_OK;
}


static u8 *hid_get_descriptor(usb_setup_req_t *req, usb_speed_type_t speed, u16 *len)
{
	u8 *buf = NULL;

	switch ((req->wValue >> 8) & 0xFF) {

	case USB_DESC_TYPE_DEVICE:
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "Get descriptor USB_DESC_TYPE_DEVICE\n");
		buf = usbd_hid_dev_desc;
		*len = sizeof(usbd_hid_dev_desc);
		break;

	case USB_DESC_TYPE_CONFIGURATION:
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "Get descriptor USB_DESC_TYPE_CONFIGURATION\n");
		if (speed == USB_SPEED_HIGH) {
			buf = usbd_hid_hs_config_desc;
			*len = sizeof(usbd_hid_hs_config_desc);
		} else {
			buf = usbd_hid_fs_config_desc;
			*len = sizeof(usbd_hid_fs_config_desc);
		}
		break;

	case USB_DESC_TYPE_DEVICE_QUALIFIER:
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "Get descriptor USB_DESC_TYPE_DEVICE_QUALIFIER\n");
		if (speed == USB_SPEED_HIGH) {
			buf = usbd_hid_device_qualifier_desc;
			*len = sizeof(usbd_hid_device_qualifier_desc);
		} else {
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "Trying to get descriptor USB_DESC_TYPE_DEVICE_QUALIFIER at full speed\n");
		}
		break;

	case USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION:
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "Get descriptor USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION\n");
		if (speed == USB_SPEED_HIGH) {
			buf = usbd_hid_other_speed_config_desc;
			*len = sizeof(usbd_hid_other_speed_config_desc);
		} else {
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "Trying to get descriptor USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION at full speed\n");
		}
		break;

	case USB_DESC_TYPE_STRING:
		switch (req->wValue & 0xFF) {
		case USBD_IDX_LANGID_STR:
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "Get descriptor USBD_IDX_LANGID_STR\n");
			buf = usbd_hid_lang_id_desc;
			*len = sizeof(usbd_hid_lang_id_desc);
			break;
		case USBD_IDX_MFC_STR:
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "Get descriptor USBD_IDX_MFC_STR\n");
			buf = usbd_hid_mfg_string_desc;
			*len = HID_MFG_STRING_DESC_SIZE;
			break;
		case USBD_IDX_PRODUCT_STR:
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "Get descriptor USBD_IDX_PRODUCT_STR\n");
			*len = HID_PRODUCT_STRING_DESC_SIZE;
			if (speed == USB_SPEED_HIGH) {
				buf = usbd_hid_hs_product_string_desc;
			} else {
				buf = usbd_hid_fs_product_string_desc;
			}
			break;
		case USBD_IDX_SERIAL_STR:
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "Get descriptor USBD_IDX_SERIAL_STR\n");
			buf = usbd_hid_sn_string_desc;
			*len = HID_SN_STRING_DESC_SIZE;
			break;
		/* Add customer string here */
		default:
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "Get descriptor failed, invalid string index %d\n", req->wValue & 0xFF);
			break;
		}
		break;

	default:
		break;
	}

	return buf;
}

u8 usbd_hid_init(u16 tx_buf_len, usbd_hid_usr_cb_t *cb)
{
	u8 ret = HAL_OK;
	struct usb_hid_device_t *hid = &hid_device;

	if (tx_buf_len > 0) {
		hid->intr_in_buf = rtw_zmalloc(tx_buf_len);
	} else {
		hid->intr_in_buf = rtw_zmalloc(HID_INTR_IN_BUF_SIZE);
	}

	if (hid->intr_in_buf == NULL) {
		ret = HAL_ERR_MEM;
		goto usbd_hid_init_exit;
	}

	if (cb != NULL) {
		hid->cb = cb;
		if (cb->init != NULL) {
			cb->init();
		}
	}

	usbd_register_class(&usbd_hid_driver);

	return ret;

usbd_hid_init_exit:
	return ret;
}

u8 usbd_hid_deinit(void)
{
	struct usb_hid_device_t *hid = &hid_device;

	usbd_unregister_class();

	if ((hid->cb != NULL) && (hid->cb->deinit != NULL)) {
		hid->cb->deinit();
	}

	if (hid->intr_in_buf != NULL) {
		rtw_free(hid->intr_in_buf);
		hid->intr_in_buf = NULL;
	}

	return HAL_OK;
}

u8 usbd_hid_send_data(u8 *data, u16 len)
{
	u8 ret = HAL_ERR_HW;
	struct usb_hid_device_t *hid = &hid_device;

	if (!hid->intr_in_state) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "EP%d TX: %d\n", HID_INTERRUPT_IN_EP_ADDRESS & 0xFU, len);

		hid->intr_in_state = 1U;
		rtw_memcpy((void *)hid->intr_in_buf, (void *)data, len);

		usbd_ep_transmit(hid->dev, HID_INTERRUPT_IN_EP_ADDRESS, hid->intr_in_buf, len);

		ret = HAL_OK;
	} else {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "EP%d TX: %d BUSY\n", HID_INTERRUPT_IN_EP_ADDRESS & 0xFU, len);
		ret = HAL_BUSY;
	}

	return ret;
}

