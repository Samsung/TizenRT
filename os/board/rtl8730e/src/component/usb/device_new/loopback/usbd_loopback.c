/**
  ******************************************************************************
  * @file    usbd_loopback.c
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

#include "usbd_loopback.h"
#include "usbd.h"

/* Private defines -----------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

static u8 usbd_loopback_set_config(usb_dev_t *dev, u8 config);
static u8 usbd_loopback_clear_config(usb_dev_t *dev, u8 config);
static u8 usbd_loopback_setup(usb_dev_t *dev, usb_setup_req_t *req);
static u8 *usbd_loopback_get_descriptor(usb_setup_req_t *req, usb_speed_type_t speed, u16 *len);
static u8 usbd_loopback_handle_ep_data_in(usb_dev_t *dev, u8 ep_num, u8 status);
static u8 usbd_loopback_handle_ep_data_out(usb_dev_t *dev, u8 ep_num, u16 len);
static u8 usbd_loopback_handle_ep0_data_out(usb_dev_t *dev);

/* Private variables ---------------------------------------------------------*/

/* USB Standard Device Descriptor */
static u8 usbd_loopback_dev_desc[USB_LEN_DEV_DESC] USB_DMA_ALIGNED = {
	USB_LEN_DEV_DESC,                               /* bLength */
	USB_DESC_TYPE_DEVICE,                           /* bDescriptorType */
	0x00,                                           /* bcdUSB */
	0x02,
	0x00,                                           /* bDeviceClass */
	0x00,                                           /* bDeviceSubClass */
	0x00,                                           /* bDeviceProtocol */
	USB_MAX_EP0_SIZE,                               /* bMaxPacketSize */
	USB_LOW_BYTE(USBD_LOOPBACK_VID),                /* idVendor */
	USB_HIGH_BYTE(USBD_LOOPBACK_VID),
	USB_LOW_BYTE(USBD_LOOPBACK_PID),                /* idProduct */
	USB_HIGH_BYTE(USBD_LOOPBACK_PID),
	0x00,                                           /* bcdDevice */
	0x02,
	USBD_IDX_MFC_STR,                               /* iManufacturer */
	USBD_IDX_PRODUCT_STR,                           /* iProduct */
	USBD_IDX_SERIAL_STR,                            /* iSerialNumber */
	0x01                                            /* bNumConfigurations */
};  /* usbd_loopback_dev_desc */

/* USB Standard String Descriptor 0 */
static u8 usbd_loopback_lang_id_desc[USB_LEN_LANGID_STR_DESC] USB_DMA_ALIGNED = {
	USB_LEN_LANGID_STR_DESC,                        /* bLength */
	USB_DESC_TYPE_STRING,                           /* bDescriptorType */
	USB_LOW_BYTE(USBD_LOOPBACK_LANGID_STRING),      /* wLANGID */
	USB_HIGH_BYTE(USBD_LOOPBACK_LANGID_STRING),
};  /* usbd_loopback_lang_id_desc */

/* USB Standard Manufacture String Descriptor */
static u8 usbd_loopback_mfg_string_desc[USBD_LOOPBACK_MFG_STRING_DESC_SIZE] USB_DMA_ALIGNED = {
	USBD_LOOPBACK_MFG_STRING_DESC_SIZE,             /* bLength */
	USB_DESC_TYPE_STRING,                           /* bDescriptorType */
	'R', 0,                                         /* bString */
	'e', 0,
	'a', 0,
	'l', 0,
	't', 0,
	'e', 0,
	'k', 0
};  /* usbd_loopback_mfg_string_desc */

/* USB Standard Product String Descriptor for high-speed mode */
static u8 usbd_loopback_hs_product_string_desc[USBD_LOOPBACK_PRODUCT_STRING_DESC_SIZE] USB_DMA_ALIGNED = {
	USBD_LOOPBACK_PRODUCT_STRING_DESC_SIZE,         /* bLength */
	USB_DESC_TYPE_STRING,                           /* bDescriptorType */
	'R', 0,                                         /* bString */
	'e', 0,
	'a', 0,
	'l', 0,
	't', 0,
	'e', 0,
	'k', 0,
	' ', 0,
	'L', 0,
	'o', 0,
	'o', 0,
	'p', 0,
	'b', 0,
	'a', 0,
	'c', 0,
	'k', 0,
	' ', 0,
	'(', 0,
	'H', 0,
	'S', 0,
	')', 0
};  /* usbd_loopback_hs_product_string_desc */

/* USB Standard Product String Descriptor for high-speed mode */
static u8 usbd_loopback_fs_product_string_desc[USBD_LOOPBACK_PRODUCT_STRING_DESC_SIZE] USB_DMA_ALIGNED = {
	USBD_LOOPBACK_PRODUCT_STRING_DESC_SIZE,         /* bLength */
	USB_DESC_TYPE_STRING,                           /* bDescriptorType */
	'R', 0,                                         /* bString */
	'e', 0,
	'a', 0,
	'l', 0,
	't', 0,
	'e', 0,
	'k', 0,
	' ', 0,
	'L', 0,
	'o', 0,
	'o', 0,
	'p', 0,
	'b', 0,
	'a', 0,
	'c', 0,
	'k', 0,
	' ', 0,
	'(', 0,
	'F', 0,
	'S', 0,
	')', 0
};  /* usbd_loopback_fs_product_string_desc */

/* USB Standard SN String Descriptor, ASCII only */
static u8 usbd_loopback_sn_string_desc[USBD_LOOPBACK_SN_STRING_DESC_SIZE] USB_DMA_ALIGNED = {
	USBD_LOOPBACK_SN_STRING_DESC_SIZE,              /* bLength */
	USB_DESC_TYPE_STRING,                           /* bDescriptorType */
	'1', 0,                                         /* bString */
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
};  /* usbd_loopback_sn_string_desc */

/* USB Standard Device Qualifier Descriptor */
static u8 usbd_loopback_device_qualifier_desc[USB_LEN_DEV_QUALIFIER_DESC] USB_DMA_ALIGNED = {
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
};  /* usbd_loopback_device_qualifier_desc */

/* USB Vendor Device High Speed Configuration Descriptor */
static u8 usbd_loopback_hs_config_desc[USBD_LOOPBACK_CONFIG_DESC_SIZE] USB_DMA_ALIGNED = {
	/* USB Standard Configuration Descriptor */
	USB_LEN_CFG_DESC,                               /* bLength */
	USB_DESC_TYPE_CONFIGURATION,                    /* bDescriptorType */
	USB_LOW_BYTE(USBD_LOOPBACK_CONFIG_DESC_SIZE),   /* wTotalLength */
	USB_HIGH_BYTE(USBD_LOOPBACK_CONFIG_DESC_SIZE),
	0x01,                                           /* bNumInterfaces */
	0x01,                                           /* bConfigurationValue */
	0x00,                                           /* iConfiguration */
#if USBD_LOOPBACK_SELF_POWERED
	0xC0,                                           /* bmAttributes: self powered */
#else
	0x80,                                           /* bmAttributes: bus powered */
#endif
	0x32,                                           /* bMaxPower */

	/* Interface Descriptor */
	USB_LEN_IF_DESC,                                /* bLength */
	USB_DESC_TYPE_INTERFACE,                        /* bDescriptorType */
	0x00,                                           /* bInterfaceNumber */
	0x00,                                           /* bAlternateSetting */
	0x04,                                           /* bNumEndpoints */
	0xFF,                                           /* bInterfaceClass: Vendor */
	0x00,                                           /* bInterfaceSubClass: No */
	0x00,                                           /* bInterfaceProtocol: None */
	0x00,                                           /* iInterface */

	/* INTR OUT Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_LOOPBACK_INTR_OUT_EP,                      /* bEndpointAddress */
	USB_CH_EP_TYPE_INTR,                            /* bmAttributes: INTR */
	USB_LOW_BYTE(USBD_LOOPBACK_INTR_HS_MAX_PACKET_SIZE), /* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_LOOPBACK_INTR_HS_MAX_PACKET_SIZE),
	USBD_LOOPBACK_INTR_HS_INTERVAL,                 /* bInterval */

	/* INTR IN Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_LOOPBACK_INTR_IN_EP,                       /* bEndpointAddress */
	USB_CH_EP_TYPE_INTR,                            /* bmAttributes: INTR */
	USB_LOW_BYTE(USBD_LOOPBACK_INTR_HS_MAX_PACKET_SIZE), /* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_LOOPBACK_INTR_HS_MAX_PACKET_SIZE),
	USBD_LOOPBACK_INTR_HS_INTERVAL,                 /* bInterval */

	/* ISO OUT Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_LOOPBACK_ISOC_OUT_EP,                      /* bEndpointAddress */
	USB_CH_EP_TYPE_ISOC,                            /* bmAttributes: ISOC */
	USB_LOW_BYTE(USBD_LOOPBACK_ISOC_HS_MAX_PACKET_SIZE), /* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_LOOPBACK_ISOC_HS_MAX_PACKET_SIZE),
	USBD_LOOPBACK_ISOC_HS_INTERVAL,                 /* bInterval */

	/* ISO IN Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_LOOPBACK_ISOC_IN_EP,                       /* bEndpointAddress */
	USB_CH_EP_TYPE_ISOC,                            /* bmAttributes: ISOC */
	USB_LOW_BYTE(USBD_LOOPBACK_ISOC_HS_MAX_PACKET_SIZE), /* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_LOOPBACK_ISOC_HS_MAX_PACKET_SIZE),
	USBD_LOOPBACK_ISOC_HS_INTERVAL,                 /* bInterval */
};  /* usbd_loopback_hs_config_desc */

/* USB Vendor Device Full Speed Configuration Descriptor */
static u8 usbd_loopback_fs_config_desc[USBD_LOOPBACK_CONFIG_DESC_SIZE] USB_DMA_ALIGNED = {
	/* USB Standard Configuration Descriptor */
	USB_LEN_CFG_DESC,                               /* bLength */
	USB_DESC_TYPE_CONFIGURATION,                    /* bDescriptorType */
	USB_LOW_BYTE(USBD_LOOPBACK_CONFIG_DESC_SIZE),   /* wTotalLength */
	USB_HIGH_BYTE(USBD_LOOPBACK_CONFIG_DESC_SIZE),
	0x01,                                           /* bNumInterfaces */
	0x01,                                           /* bConfigurationValue */
	0x00,                                           /* iConfiguration */
#if USBD_LOOPBACK_SELF_POWERED
	0xC0,                                           /* bmAttributes: self powered */
#else
	0x80,                                           /* bmAttributes: bus powered */
#endif
	0x32,                                           /* bMaxPower */

	/* Interface Descriptor */
	USB_LEN_IF_DESC,                                /* bLength */
	USB_DESC_TYPE_INTERFACE,                        /* bDescriptorType */
	0x00,                                           /* bInterfaceNumber */
	0x00,                                           /* bAlternateSetting */
	0x04,                                           /* bNumEndpoints */
	0xFF,                                           /* bInterfaceClass: Vendor */
	0x00,                                           /* bInterfaceSubClass: No */
	0x00,                                           /* bInterfaceProtocol: None */
	0x00,                                           /* iInterface */

	/* INTR OUT Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_LOOPBACK_INTR_OUT_EP,                      /* bEndpointAddress */
	USB_CH_EP_TYPE_INTR,                            /* bmAttributes: INTR */
	USB_LOW_BYTE(USBD_LOOPBACK_INTR_FS_MAX_PACKET_SIZE), /* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_LOOPBACK_INTR_FS_MAX_PACKET_SIZE),
	USBD_LOOPBACK_INTR_FS_INTERVAL,                 /* bInterval */

	/* INTR IN Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_LOOPBACK_INTR_IN_EP,                       /* bEndpointAddress */
	USB_CH_EP_TYPE_INTR,                            /* bmAttributes: INTR */
	USB_LOW_BYTE(USBD_LOOPBACK_INTR_FS_MAX_PACKET_SIZE), /* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_LOOPBACK_INTR_FS_MAX_PACKET_SIZE),
	USBD_LOOPBACK_INTR_FS_INTERVAL,                 /* bInterval */

	/* ISO OUT Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_LOOPBACK_ISOC_OUT_EP,                      /* bEndpointAddress */
	USB_CH_EP_TYPE_ISOC,                            /* bmAttributes: ISOC */
	USB_LOW_BYTE(USBD_LOOPBACK_ISOC_FS_MAX_PACKET_SIZE), /* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_LOOPBACK_ISOC_FS_MAX_PACKET_SIZE),
	USBD_LOOPBACK_ISOC_FS_INTERVAL,                 /* bInterval */

	/* ISO IN Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_LOOPBACK_ISOC_IN_EP,                       /* bEndpointAddress */
	USB_CH_EP_TYPE_ISOC,                            /* bmAttributes: ISOC */
	USB_LOW_BYTE(USBD_LOOPBACK_ISOC_FS_MAX_PACKET_SIZE), /* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_LOOPBACK_ISOC_FS_MAX_PACKET_SIZE),
	USBD_LOOPBACK_ISOC_FS_INTERVAL,                 /* bInterval */

};  /* usbd_loopback_fs_config_desc */

/* USB Vendor Device Other Speed Configuration Descriptor */
static u8 usbd_loopback_other_speed_config_desc[USBD_LOOPBACK_CONFIG_DESC_SIZE] USB_DMA_ALIGNED = {
	/* USB Standard Configuration Descriptor */
	USB_LEN_CFG_DESC,                               /* bLength */
	USB_DESC_TYPE_CONFIGURATION,                    /* bDescriptorType */
	USB_LOW_BYTE(USBD_LOOPBACK_CONFIG_DESC_SIZE),   /* wTotalLength */
	USB_HIGH_BYTE(USBD_LOOPBACK_CONFIG_DESC_SIZE),
	0x01,                                           /* bNumInterfaces */
	0x01,                                           /* bConfigurationValue */
	0x00,                                           /* iConfiguration */
#if USBD_LOOPBACK_SELF_POWERED
	0xC0,                                           /* bmAttributes: self powered */
#else
	0x80,                                           /* bmAttributes: bus powered */
#endif
	0x32,                                           /* bMaxPower */

	/* Interface Descriptor */
	USB_LEN_IF_DESC,                                /* bLength */
	USB_DESC_TYPE_INTERFACE,                        /* bDescriptorType */
	0x00,                                           /* bInterfaceNumber */
	0x00,                                           /* bAlternateSetting */
	0x04,                                           /* bNumEndpoints */
	0xFF,                                           /* bInterfaceClass: Vendor */
	0x00,                                           /* bInterfaceSubClass: No */
	0x00,                                           /* bInterfaceProtocol: None */
	0x00,                                           /* iInterface */

	/* INTR OUT Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_LOOPBACK_INTR_OUT_EP,                      /* bEndpointAddress */
	USB_CH_EP_TYPE_INTR,                            /* bmAttributes: INTR */
	USB_LOW_BYTE(USBD_LOOPBACK_INTR_FS_MAX_PACKET_SIZE), /* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_LOOPBACK_INTR_FS_MAX_PACKET_SIZE),
	USBD_LOOPBACK_INTR_FS_INTERVAL,                 /* bInterval */

	/* INTR IN Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_LOOPBACK_INTR_IN_EP,                       /* bEndpointAddress */
	USB_CH_EP_TYPE_INTR,                            /* bmAttributes: INTR */
	USB_LOW_BYTE(USBD_LOOPBACK_INTR_FS_MAX_PACKET_SIZE), /* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_LOOPBACK_INTR_FS_MAX_PACKET_SIZE),
	USBD_LOOPBACK_INTR_FS_INTERVAL,                 /* bInterval */

	/* ISO OUT Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_LOOPBACK_ISOC_OUT_EP,                      /* bEndpointAddress */
	USB_CH_EP_TYPE_ISOC,                            /* bmAttributes: ISOC */
	USB_LOW_BYTE(USBD_LOOPBACK_ISOC_FS_MAX_PACKET_SIZE), /* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_LOOPBACK_ISOC_FS_MAX_PACKET_SIZE),
	USBD_LOOPBACK_ISOC_FS_INTERVAL,                 /* bInterval */

	/* ISO IN Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_LOOPBACK_ISOC_IN_EP,                       /* bEndpointAddress */
	USB_CH_EP_TYPE_ISOC,                            /* bmAttributes: ISOC */
	USB_LOW_BYTE(USBD_LOOPBACK_ISOC_FS_MAX_PACKET_SIZE), /* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_LOOPBACK_ISOC_FS_MAX_PACKET_SIZE),
	USBD_LOOPBACK_ISOC_FS_INTERVAL,                 /* bInterval */
};  /* usbd_loopback_other_speed_config_desc */

/* Vendor Class Driver */
usbd_class_driver_t usbd_loopback_driver = {
	.get_descriptor = usbd_loopback_get_descriptor,
	.set_config = usbd_loopback_set_config,
	.clear_config = usbd_loopback_clear_config,
	.setup = usbd_loopback_setup,

	.ep0_data_out = usbd_loopback_handle_ep0_data_out,
	.ep_data_in = usbd_loopback_handle_ep_data_in,
	.ep_data_out = usbd_loopback_handle_ep_data_out,
};

/* Vendor Device */
static usbd_loopback_dev_t usbd_loopback_dev;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Set Vendor class configuration
  * @param  dev: USB device instance
  * @param  config: USB configuration index
  * @retval Status
  */
static u8 usbd_loopback_set_config(usb_dev_t *dev, u8 config)
{
	u16 ep_mps;
	u8 ret = HAL_OK;
	usbd_loopback_dev_t *cdev = &usbd_loopback_dev;

	UNUSED(config);

	cdev->dev = dev;

	/* Init INTR IN EP */
	ep_mps = (dev->dev_speed == USB_SPEED_HIGH) ? USBD_LOOPBACK_INTR_HS_MAX_PACKET_SIZE : USBD_LOOPBACK_INTR_FS_MAX_PACKET_SIZE;
	usbd_ep_init(dev, USBD_LOOPBACK_INTR_IN_EP, USB_CH_EP_TYPE_INTR, ep_mps);

	/* Init INTR OUT EP */
	ep_mps = (dev->dev_speed == USB_SPEED_HIGH) ? USBD_LOOPBACK_INTR_HS_MAX_PACKET_SIZE : USBD_LOOPBACK_INTR_FS_MAX_PACKET_SIZE;
	usbd_ep_init(dev, USBD_LOOPBACK_INTR_OUT_EP, USB_CH_EP_TYPE_INTR, ep_mps);

	/* Prepare to receive next INTR OUT packet */
	usbd_ep_receive(dev, USBD_LOOPBACK_INTR_OUT_EP, cdev->intr_out_buf, cdev->intr_out_buf_len);

	/* Init ISOC IN EP */
	ep_mps = (dev->dev_speed == USB_SPEED_HIGH) ? USBD_LOOPBACK_ISOC_HS_MAX_PACKET_SIZE : USBD_LOOPBACK_ISOC_FS_MAX_PACKET_SIZE;
	usbd_ep_init(dev, USBD_LOOPBACK_ISOC_IN_EP, USB_CH_EP_TYPE_ISOC, ep_mps);

	/* Init ISOC OUT EP */
	ep_mps = (dev->dev_speed == USB_SPEED_HIGH) ? USBD_LOOPBACK_ISOC_HS_MAX_PACKET_SIZE : USBD_LOOPBACK_ISOC_FS_MAX_PACKET_SIZE;
	usbd_ep_init(dev, USBD_LOOPBACK_ISOC_OUT_EP, USB_CH_EP_TYPE_ISOC, ep_mps);

	/* Prepare to receive next ISO OUT packet */
	usbd_ep_receive(dev, USBD_LOOPBACK_ISOC_OUT_EP, cdev->isoc_out_buf, cdev->isoc_out_buf_len);

	return ret;
}

/**
  * @brief  Clear Vendor configuration
  * @param  dev: USB device instance
  * @param  config: USB configuration index
  * @retval Status
  */
static u8 usbd_loopback_clear_config(usb_dev_t *dev, u8 config)
{
	u8 ret = 0U;

	UNUSED(config);

	/* DeInit BULK IN EP */
	usbd_ep_deinit(dev, USBD_LOOPBACK_INTR_IN_EP);

	/* DeInit BULK OUT EP */
	usbd_ep_deinit(dev, USBD_LOOPBACK_INTR_OUT_EP);

	/* DeInit ISO IN EP */
	usbd_ep_deinit(dev, USBD_LOOPBACK_ISOC_IN_EP);

	/* DeInit ISO OUT EP */
	usbd_ep_deinit(dev, USBD_LOOPBACK_ISOC_OUT_EP);

	return ret;
}

/**
  * @brief  Handle Vendor specific CTRL requests
  * @param  dev: USB device instance
  * @param  req: USB CTRL requests
  * @retval Status
  */
static u8 usbd_loopback_setup(usb_dev_t *dev, usb_setup_req_t *req)
{
	u8 ret = HAL_OK;
	usbd_loopback_dev_t *cdev = &usbd_loopback_dev;

	DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE,
			   "usbd_loopback_setup bmRequestType=0x%02X bRequest=0x%02X wLength=0x%04X wValue=%x\n",
			   req->bmRequestType,
			   req->bRequest,
			   req->wLength,
			   req->wValue);

	switch (req->bmRequestType & USB_REQ_TYPE_MASK) {
	case USB_REQ_TYPE_CLASS:
	case USB_REQ_TYPE_VENDOR:
		if (req->wLength) {
			if (req->bmRequestType & 0x80U) {
				cdev->cb->setup(req->bRequest, dev->ctrl_buf, req->wLength, req->wValue);
				// Data in phase
				usbd_ep0_transmit(dev, dev->ctrl_buf, req->wLength);
			} else {
				// Data out phase
				usbd_ep0_receive(dev, dev->ctrl_buf, req->wLength);
			}
		} else {
			// No data phase
			cdev->cb->setup(req->bRequest, dev->ctrl_buf, 0, req->wValue);
		}
		break;
	default:
		ret = HAL_ERR_HW;
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Unexpected bmRequestType 0x%02X => 0x%02X\n", req->bmRequestType & USB_REQ_TYPE_MASK);
		break;
	}

	return ret;
}

/**
  * @brief  Data sent on non-control IN endpoint
  * @param  dev: USB device instance
  * @param  ep_num: endpoint number
  * @retval Status
  */
static u8 usbd_loopback_handle_ep_data_in(usb_dev_t *dev, u8 ep_num, u8 status)
{
	u8 ret = HAL_OK;
	usbd_loopback_dev_t *cdev = &usbd_loopback_dev;

	UNUSED(dev);

	if (status == HAL_OK) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "EP%d TX done\n", ep_num);
	} else {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "EP%d TX error: %d\n", ep_num, status);
	}

	if (ep_num == (USBD_LOOPBACK_INTR_IN_EP & (~USB_REQ_DIR_MASK))) {
		if (cdev->cb->intr_transmitted != NULL) {
			ret = cdev->cb->intr_transmitted(status);
		}
		cdev->intr_in_state = 0U;
	}

	if (ep_num == (USBD_LOOPBACK_ISOC_IN_EP & (~USB_REQ_DIR_MASK))) {
		if (cdev->cb->isoc_transmitted != NULL) {
			ret = cdev->cb->isoc_transmitted(status);
		}
		cdev->isoc_in_state = 0U;
	}

	return ret;
}

/**
  * @brief  Data received on non-control Out endpoint
  * @param  dev: USB device instance
  * @param  ep_num: endpoint number
  * @retval Status
  */
static u8 usbd_loopback_handle_ep_data_out(usb_dev_t *dev, u8 ep_num, u16 len)
{
	u8 ret = HAL_OK;
	usbd_loopback_dev_t *cdev = &usbd_loopback_dev;

	UNUSED(dev);

	DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "EP%d RX %d\n", ep_num, len);

	if (ep_num == (USBD_LOOPBACK_INTR_OUT_EP & (~USB_REQ_DIR_MASK))) {
		if (cdev->cb->intr_received != NULL) {
			ret = cdev->cb->intr_received(cdev->intr_out_buf, len);
		}
	}

	if (ep_num == (USBD_LOOPBACK_ISOC_OUT_EP & (~USB_REQ_DIR_MASK))) {
		if (cdev->cb->isoc_received != NULL) {
			ret = cdev->cb->isoc_received(cdev->isoc_out_buf, len);
		}
	}

	return ret;
}

/**
  * @brief  usbd_loopback_handle_ep0_data_out
  *         Handle EP0 Rx Ready event
  * @param  dev: USB device instance
  * @retval Status
  */
static u8 usbd_loopback_handle_ep0_data_out(usb_dev_t *dev)
{
	printf("EP0 RX [state=%d]len=%d,totallen=%d!\n", dev->ep0_state, dev->ep0_data_len, dev->ep0_xfer_total_len);
	return HAL_OK;
}


/**
  * @brief  Get descriptor callback
  * @param  req: setup request
  * @param  speed: device speed
  * @param  len: descriptor length
  * @retval descriptor buffer
  */
static u8 *usbd_loopback_get_descriptor(usb_setup_req_t *req, usb_speed_type_t speed, u16 *len)
{
	u8 *buf = NULL;

	switch ((req->wValue >> 8) & 0xFF) {

	case USB_DESC_TYPE_DEVICE:
		buf = usbd_loopback_dev_desc;
		*len = sizeof(usbd_loopback_dev_desc);
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "Get descriptor USB_DESC_TYPE_DEVICE %d\n", *len);
		break;

	case USB_DESC_TYPE_CONFIGURATION:
		if (speed == USB_SPEED_HIGH) {
			buf = usbd_loopback_hs_config_desc;
			*len = sizeof(usbd_loopback_hs_config_desc);
		} else {
			buf = usbd_loopback_fs_config_desc;
			*len = sizeof(usbd_loopback_fs_config_desc);
		}
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "Get descriptor USB_DESC_TYPE_CONFIGURATION %d\n", *len);
		break;

	case USB_DESC_TYPE_DEVICE_QUALIFIER:
		if (speed == USB_SPEED_HIGH) {
			buf = usbd_loopback_device_qualifier_desc;
			*len = sizeof(usbd_loopback_device_qualifier_desc);
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "Get descriptor USB_DESC_TYPE_DEVICE_QUALIFIER %d\n", *len);
		} else {
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "Trying to get descriptor USB_DESC_TYPE_DEVICE_QUALIFIER at full speed\n");
		}
		break;

	case USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION:
		if (speed == USB_SPEED_HIGH) {
			buf = usbd_loopback_other_speed_config_desc;
			*len = sizeof(usbd_loopback_other_speed_config_desc);
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "Get descriptor USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION %d\n", *len);
		} else {
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "Trying to get descriptor USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION at full speed\n");
		}
		break;

	case USB_DESC_TYPE_STRING:
		switch (req->wValue & 0xFF) {
		case USBD_IDX_LANGID_STR:
			buf = usbd_loopback_lang_id_desc;
			*len = sizeof(usbd_loopback_lang_id_desc);
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "Get descriptor USBD_IDX_LANGID_STR %d\n", *len);
			break;
		case USBD_IDX_MFC_STR:
			buf = usbd_loopback_mfg_string_desc;
			*len = USBD_LOOPBACK_MFG_STRING_DESC_SIZE;
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "Get descriptor USBD_IDX_MFC_STR %d\n", *len);
			break;
		case USBD_IDX_PRODUCT_STR:
			*len = USBD_LOOPBACK_PRODUCT_STRING_DESC_SIZE;
			if (speed == USB_SPEED_HIGH) {
				buf = usbd_loopback_hs_product_string_desc;
			} else {
				buf = usbd_loopback_fs_product_string_desc;
			}
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "Get descriptor USBD_IDX_PRODUCT_STR %d\n", *len);
			break;
		case USBD_IDX_SERIAL_STR:
			buf = usbd_loopback_sn_string_desc;
			*len = USBD_LOOPBACK_SN_STRING_DESC_SIZE;
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "Get descriptor USBD_IDX_SERIAL_STR %d\n", *len);
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

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Initialize loopback device
  * @retval Status
  */
u8 usbd_loopback_init(u32 nptx_buf_len, u32 nprx_buf_len, u32 ptx_buf_len, u32 prx_buf_len, usbd_loopback_cb_t *cb)
{
	u8 ret = HAL_OK;
	usbd_loopback_dev_t *cdev = &usbd_loopback_dev;

	if (cb != NULL) {
		cdev->cb = cb;
	}

	cdev->intr_out_buf_len = nprx_buf_len;
	cdev->intr_out_buf = rtw_zmalloc(nprx_buf_len);
	if (cdev->intr_out_buf == NULL) {
		ret = HAL_ERR_MEM;
		goto Init_exit;
	}

	cdev->intr_in_buf_len = nptx_buf_len;
	cdev->intr_in_buf = rtw_zmalloc(nptx_buf_len);
	if (cdev->intr_in_buf == NULL) {
		ret = HAL_ERR_MEM;
		goto Init_free_intr_out_buf_exit;
	}

	cdev->isoc_out_buf_len = prx_buf_len;
	cdev->isoc_out_buf = rtw_zmalloc(prx_buf_len);
	if (cdev->isoc_out_buf == NULL) {
		ret = HAL_ERR_MEM;
		goto Init_free_intr_in_buf_exit;
	}

	cdev->isoc_in_buf_len = ptx_buf_len;
	cdev->isoc_in_buf = rtw_zmalloc(ptx_buf_len);
	if (cdev->isoc_in_buf == NULL) {
		ret = HAL_ERR_MEM;
		goto Init_free_isoc_out_buf_exit;
	}

	if (cb != NULL) {
		cdev->cb = cb;
		if (cb->init != NULL) {
			ret = cb->init();
			if (ret != HAL_OK) {
				goto Init_free_isoc_in_buf_exit;
			}
		}
	}

	usbd_register_class(&usbd_loopback_driver);

	return ret;

Init_free_isoc_in_buf_exit:
	rtw_free(cdev->isoc_in_buf);
	cdev->isoc_in_buf = NULL;

Init_free_isoc_out_buf_exit:
	rtw_free(cdev->isoc_out_buf);
	cdev->isoc_out_buf = NULL;

Init_free_intr_in_buf_exit:
	rtw_free(cdev->intr_in_buf);
	cdev->intr_in_buf = NULL;

Init_free_intr_out_buf_exit:
	rtw_free(cdev->intr_out_buf);
	cdev->intr_out_buf = NULL;

Init_exit:
	return ret;
}

/**
  * @brief  DeInitialize loopback device
  * @param  void
  * @retval Status
  */
u8 usbd_loopback_deinit(void)
{
	usbd_loopback_dev_t *cdev = &usbd_loopback_dev;

	usbd_unregister_class();

	if (cdev->intr_in_buf != NULL) {
		rtw_free(cdev->intr_in_buf);
		cdev->intr_in_buf = NULL;
	}

	if (cdev->intr_out_buf != NULL) {
		rtw_free(cdev->intr_out_buf);
		cdev->intr_out_buf = NULL;
	}

	if (cdev->isoc_in_buf != NULL) {
		rtw_free(cdev->isoc_in_buf);
		cdev->isoc_in_buf = NULL;
	}

	if (cdev->isoc_out_buf != NULL) {
		rtw_free(cdev->isoc_out_buf);
		cdev->isoc_out_buf = NULL;
	}

	return HAL_OK;
}

/**
  * @brief  Transmit INTR IN packet
  * @param  buf: data buffer
  * @param  len: data length
  * @retval Status
  */
u8 usbd_loopback_intr_transmit(u8 *buf, u16 len)
{
	u8 ret = HAL_ERR_HW;
	usbd_loopback_dev_t *cdev = &usbd_loopback_dev;
	usb_dev_t *dev = cdev->dev;

	if (len > cdev->intr_in_buf_len) {
		len = cdev->intr_in_buf_len;
	}

	if (cdev->intr_in_state == 0U) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "EP%d TX %d\n", USBD_LOOPBACK_INTR_IN_EP & 0xFU, len);

		/* Tx Transfer in progress */
		cdev->intr_in_state = 1U;

		rtw_memcpy((void *)cdev->intr_in_buf, (void *)buf, len);

		/* Transmit next packet */
		usbd_ep_transmit(dev, USBD_LOOPBACK_INTR_IN_EP, cdev->intr_in_buf, len);

		ret = HAL_OK;
	} else {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "EP%d TX %d: BUSY\n", USBD_LOOPBACK_INTR_IN_EP & 0xFU, len);
		ret = HAL_BUSY;
	}

	return ret;
}

/**
  * @brief  Prepare to receive INTR OUT packet
  * @retval Status
  */
u8 usbd_loopback_intr_receive(void)
{
	usbd_loopback_dev_t *cdev = &usbd_loopback_dev;

	DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "INTR receive...\n");

	return usbd_ep_receive(cdev->dev, USBD_LOOPBACK_INTR_OUT_EP, cdev->intr_out_buf, cdev->intr_out_buf_len);
}

/**
  * @brief  Transmit ISOC IN packet
  * @param  buf: data buffer
  * @param  len: data length
  * @retval Status
  */
u8 usbd_loopback_isoc_transmit(u8 *buf, u16 len)
{
	u8 ret = HAL_ERR_HW;
	usbd_loopback_dev_t *cdev = &usbd_loopback_dev;
	usb_dev_t *dev = cdev->dev;

	if (len > cdev->isoc_in_buf_len) {
		len = cdev->isoc_in_buf_len;
	}

	DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "EP%d TX %d\n", USBD_LOOPBACK_ISOC_IN_EP & 0xFU, len);

	if (cdev->isoc_in_state == 0U) {
		/* Tx Transfer in progress */
		cdev->isoc_in_state = 1U;
	} else {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "EP%d TX %d: BUSY\n", USBD_LOOPBACK_ISOC_IN_EP & 0xFU, len);
	}

	rtw_memcpy((void *)cdev->isoc_in_buf, (void *)buf, len);

	/* Transmit next packet */
	usbd_ep_transmit(dev, USBD_LOOPBACK_ISOC_IN_EP, cdev->isoc_in_buf, len);

	ret = HAL_OK;

	return ret;
}

/**
  * @brief  Prepare to receive ISOC OUT packet
  * @retval Status
  */
u8 usbd_loopback_isoc_receive(void)
{
	usbd_loopback_dev_t *cdev = &usbd_loopback_dev;

	DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "ISOC receive...\n");

	return usbd_ep_receive(cdev->dev, USBD_LOOPBACK_ISOC_OUT_EP, cdev->isoc_out_buf, cdev->isoc_out_buf_len);
}


