/**
  ******************************************************************************
  * @file    usbd_vendor.c
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

#include "usbd_vendor.h"
#include "usbd.h"

/* Private defines -----------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

static u8 usbd_vendor_set_config(usb_dev_t *dev, u8 config);
static u8 usbd_vendor_clear_config(usb_dev_t *dev, u8 config);
static u8 usbd_vendor_setup(usb_dev_t *dev, usb_setup_req_t *req);
static u8 *usbd_vendor_get_descriptor(usb_setup_req_t *req, usb_speed_type_t speed, u16 *len);
static u8 usbd_vendor_handle_ep_data_in(usb_dev_t *dev, u8 ep_num, u8 status);
static u8 usbd_vendor_handle_ep_data_out(usb_dev_t *dev, u8 ep_num, u16 len);

/* Private variables ---------------------------------------------------------*/

/* USB Standard Device Descriptor */
static u8 usbd_vendor_dev_desc[USB_LEN_DEV_DESC] USB_DMA_ALIGNED = {
	USB_LEN_DEV_DESC,                               /* bLength */
	USB_DESC_TYPE_DEVICE,                           /* bDescriptorType */
	0x00,                                           /* bcdUSB */
	0x02,
	0x00,                                           /* bDeviceClass */
	0x00,                                           /* bDeviceSubClass */
	0x00,                                           /* bDeviceProtocol */
	USB_MAX_EP0_SIZE,                               /* bMaxPacketSize */
	USB_LOW_BYTE(USBD_VENDOR_VID),                  /* idVendor */
	USB_HIGH_BYTE(USBD_VENDOR_VID),
	USB_LOW_BYTE(USBD_VENDOR_PID),                  /* idProduct */
	USB_HIGH_BYTE(USBD_VENDOR_PID),
	0x00,                                           /* bcdDevice */
	0x02,
	USBD_IDX_MFC_STR,                               /* iManufacturer */
	USBD_IDX_PRODUCT_STR,                           /* iProduct */
	USBD_IDX_SERIAL_STR,                            /* iSerialNumber */
	0x01                                            /* bNumConfigurations */
};  /* usbd_vendor_dev_desc */

/* USB Standard String Descriptor 0 */
static u8 usbd_vendor_lang_id_desc[USB_LEN_LANGID_STR_DESC] USB_DMA_ALIGNED = {
	USB_LEN_LANGID_STR_DESC,                        /* bLength */
	USB_DESC_TYPE_STRING,                           /* bDescriptorType */
	USB_LOW_BYTE(USBD_VENDOR_LANGID_STRING),            /* wLANGID */
	USB_HIGH_BYTE(USBD_VENDOR_LANGID_STRING),
};  /* usbd_vendor_lang_id_desc */

/* USB Standard Manufacture String Descriptor */
static u8 usbd_vendor_mfg_string_desc[USBD_VENDOR_MFG_STRING_DESC_SIZE] USB_DMA_ALIGNED = {
	USBD_VENDOR_MFG_STRING_DESC_SIZE,                   /* bLength */
	USB_DESC_TYPE_STRING,                           /* bDescriptorType */
	'R',                                            /* bString */
	'e',
	'a',
	'l',
	't',
	'e',
	'k'
};  /* usbd_vendor_mfg_string_desc */

/* USB Standard Product String Descriptor for high-speed mode */
static u8 usbd_vendor_hs_product_string_desc[USBD_VENDOR_PRODUCT_STRING_DESC_SIZE] USB_DMA_ALIGNED = {
	USBD_VENDOR_PRODUCT_STRING_DESC_SIZE,               /* bLength */
	USB_DESC_TYPE_STRING,                           /* bDescriptorType */
	'R', 0,                                            /* bString */
	'e', 0,
	'a', 0,
	'l', 0,
	't', 0,
	'e', 0,
	'k', 0,
	' ', 0,
	'V', 0,
	'e', 0,
	'n', 0,
	'd', 0,
	'o', 0,
	'r', 0,
	' ', 0,
	'(', 0,
	'H', 0,
	'S', 0,
	')', 0
};  /* usbd_vendor_hs_product_string_desc */

/* USB Standard Product String Descriptor for high-speed mode */
static u8 usbd_vendor_fs_product_string_desc[USBD_VENDOR_PRODUCT_STRING_DESC_SIZE] USB_DMA_ALIGNED = {
	USBD_VENDOR_PRODUCT_STRING_DESC_SIZE,               /* bLength */
	USB_DESC_TYPE_STRING,                           /* bDescriptorType */
	'R', 0,                                           /* bString */
	'e', 0,
	'a', 0,
	'l', 0,
	't', 0,
	'e', 0,
	'k', 0,
	' ', 0,
	'V', 0,
	'e', 0,
	'n', 0,
	'd', 0,
	'o', 0,
	'r', 0,
	' ', 0,
	'(', 0,
	'F', 0,
	'S', 0,
	')', 0
};  /* usbd_vendor_fs_product_string_desc */

/* USB Standard SN String Descriptor, ASCII only */
static u8 usbd_vendor_sn_string_desc[USBD_VENDOR_SN_STRING_DESC_SIZE] USB_DMA_ALIGNED = {
	USBD_VENDOR_SN_STRING_DESC_SIZE,                    /* bLength */
	USB_DESC_TYPE_STRING,                           /* bDescriptorType */
	'1', 0,                                           /* bString */
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
};  /* usbd_vendor_sn_string_desc */

/* USB Standard Device Qualifier Descriptor */
static u8 usbd_vendor_device_qualifier_desc[USB_LEN_DEV_QUALIFIER_DESC] USB_DMA_ALIGNED = {
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
};  /* usbd_vendor_device_qualifier_desc */

/* USB Vendor Device High Speed Configuration Descriptor */
static u8 usbd_vendor_hs_config_desc[USBD_VENDOR_CONFIG_DESC_SIZE] USB_DMA_ALIGNED = {
	/* USB Standard Configuration Descriptor */
	USB_LEN_CFG_DESC,                               /* bLength */
	USB_DESC_TYPE_CONFIGURATION,                    /* bDescriptorType */
	USBD_VENDOR_CONFIG_DESC_SIZE,                       /* wTotalLength */
	0x00,
	0x01,                                           /* bNumInterfaces */
	0x01,                                           /* bConfigurationValue */
	0x00,                                           /* iConfiguration */
#if USBD_VENDOR_SELF_POWERED
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
	0xff,                                           /* bInterfaceClass: Vendor */
	0x00,                                           /* bInterfaceSubClass: No */
	0x00,                                           /* bInterfaceProtocol: None */
	0x00,                                           /* iInterface */

	/* BULK OUT Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_VENDOR_BULK_OUT_EP,                            /* bEndpointAddress */
	0x02,                                           /* bmAttributes: BULK */
	USB_LOW_BYTE(USBD_VENDOR_HS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_HS_MAX_PACKET_SIZE),
	0x00,                                           /* bInterval */

	/* BULK IN Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_VENDOR_BULK_IN_EP,                             /* bEndpointAddress */
	0x02,                                           /* bmAttributes: BULK */
	USB_LOW_BYTE(USBD_VENDOR_HS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_HS_MAX_PACKET_SIZE),
	0x00,                                            /* bInterval */

	/* ISO OUT Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_VENDOR_ISO_OUT_EP,                            /* bEndpointAddress */
	0x01,                                           /* bmAttributes: ISO */
	USB_LOW_BYTE(USBD_VENDOR_HS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_HS_MAX_PACKET_SIZE),
	USBD_VENDOR_HS_ISO_OUT_INTERVAL,                                           /* bInterval */

	/* ISO IN Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_VENDOR_ISO_IN_EP,                             /* bEndpointAddress */
	0x01,                                           /* bmAttributes: ISO */
	USB_LOW_BYTE(USBD_VENDOR_HS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_HS_MAX_PACKET_SIZE),
	USBD_VENDOR_HS_ISO_IN_INTERVAL,                                            /* bInterval */

	/* Interface1 Descriptor */
	USB_LEN_IF_DESC,                                /* bLength */
	USB_DESC_TYPE_INTERFACE,                        /* bDescriptorType */
	0x00,                                           /* bInterfaceNumber */
	0x01,                                           /* bAlternateSetting */
	0x02,                                           /* bNumEndpoints */
	0xff,                                           /* bInterfaceClass: Vendor */
	0x00,                                           /* bInterfaceSubClass: No */
	0x00,                                           /* bInterfaceProtocol: None */
	0x00,                                           /* iInterface */

	/* BULK OUT Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_VENDOR_BULK_OUT_EP,                            /* bEndpointAddress */
	0x02,                                           /* bmAttributes: BULK */
	USB_LOW_BYTE(USBD_VENDOR_HS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_HS_MAX_PACKET_SIZE),
	0x00,                                           /* bInterval */

	/* BULK IN Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_VENDOR_BULK_IN_EP,                             /* bEndpointAddress */
	0x02,                                           /* bmAttributes: BULK */
	USB_LOW_BYTE(USBD_VENDOR_HS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_HS_MAX_PACKET_SIZE),
	0x00                                            /* bInterval */
};  /* usbd_vendor_hs_config_desc */

/* USB Vendor Device Full Speed Configuration Descriptor */
static u8 usbd_vendor_fs_config_desc[USBD_VENDOR_CONFIG_DESC_SIZE] USB_DMA_ALIGNED = {
	/* USB Standard Configuration Descriptor */
	USB_LEN_CFG_DESC,                               /* bLength */
	USB_DESC_TYPE_CONFIGURATION,                    /* bDescriptorType */
	USBD_VENDOR_CONFIG_DESC_SIZE,                       /* wTotalLength */
	0x00,
	0x01,                                           /* bNumInterfaces */
	0x01,                                           /* bConfigurationValue */
	0x00,                                           /* iConfiguration */
#if USBD_VENDOR_SELF_POWERED
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
	0xff,                                           /* bInterfaceClass: Vendor */
	0x00,                                           /* bInterfaceSubClass: No */
	0x00,                                           /* bInterfaceProtocol: None */
	0x00,                                           /* iInterface */

	/* BULK OUT Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_VENDOR_BULK_OUT_EP,                            /* bEndpointAddress */
	0x02,                                           /* bmAttributes: BULK */
	USB_LOW_BYTE(USBD_VENDOR_FS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_FS_MAX_PACKET_SIZE),
	0x00,                                           /* bInterval */

	/* BULK IN Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_VENDOR_BULK_IN_EP,                             /* bEndpointAddress */
	0x02,                                           /* bmAttributes: BULK */
	USB_LOW_BYTE(USBD_VENDOR_FS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_FS_MAX_PACKET_SIZE),
	0x00,                                            /* bInterval */

	/* ISO OUT Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_VENDOR_ISO_OUT_EP,                            /* bEndpointAddress */
	0x01,                                           /* bmAttributes: ISO */
	USB_LOW_BYTE(USBD_VENDOR_FS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_FS_MAX_PACKET_SIZE),
	USBD_VENDOR_FS_ISO_OUT_INTERVAL,                                           /* bInterval */

	/* ISO IN Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_VENDOR_ISO_IN_EP,                             /* bEndpointAddress */
	0x01,                                           /* bmAttributes: ISO */
	USB_LOW_BYTE(USBD_VENDOR_FS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_FS_MAX_PACKET_SIZE),
	USBD_VENDOR_FS_ISO_IN_INTERVAL,                                            /* bInterval */

	/* Interface1 Descriptor */
	USB_LEN_IF_DESC,                                /* bLength */
	USB_DESC_TYPE_INTERFACE,                        /* bDescriptorType */
	0x00,                                           /* bInterfaceNumber */
	0x01,                                           /* bAlternateSetting */
	0x02,                                           /* bNumEndpoints */
	0xff,                                           /* bInterfaceClass: Vendor */
	0x00,                                           /* bInterfaceSubClass: No */
	0x00,                                           /* bInterfaceProtocol: None */
	0x00,                                           /* iInterface */

	/* BULK OUT Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_VENDOR_BULK_OUT_EP,                            /* bEndpointAddress */
	0x02,                                           /* bmAttributes: BULK */
	USB_LOW_BYTE(USBD_VENDOR_FS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_FS_MAX_PACKET_SIZE),
	0x00,                                           /* bInterval */

	/* BULK IN Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_VENDOR_BULK_IN_EP,                             /* bEndpointAddress */
	0x02,                                           /* bmAttributes: BULK */
	USB_LOW_BYTE(USBD_VENDOR_FS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_FS_MAX_PACKET_SIZE),
	0x00                                            /* bInterval */
};  /* usbd_vendor_fs_config_desc */

/* USB Vendor Device Other Speed Configuration Descriptor */
static u8 usbd_vendor_other_speed_config_desc[USBD_VENDOR_CONFIG_DESC_SIZE] USB_DMA_ALIGNED = {
	/* USB Standard Configuration Descriptor */
	USB_LEN_CFG_DESC,                               /* bLength */
	USB_DESC_TYPE_CONFIGURATION,                    /* bDescriptorType */
	USBD_VENDOR_CONFIG_DESC_SIZE,                       /* wTotalLength */
	0x00,
	0x01,                                           /* bNumInterfaces */
	0x01,                                           /* bConfigurationValue */
	0x00,                                           /* iConfiguration */
#if USBD_VENDOR_SELF_POWERED
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
	0xff,                                           /* bInterfaceClass: Vendor */
	0x00,                                           /* bInterfaceSubClass: No */
	0x00,                                           /* bInterfaceProtocol: None */
	0x00,                                           /* iInterface */

	/* BULK OUT Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_VENDOR_BULK_OUT_EP,                            /* bEndpointAddress */
	0x02,                                           /* bmAttributes: BULK */
	USB_LOW_BYTE(USBD_VENDOR_FS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_FS_MAX_PACKET_SIZE),
	0x00,                                           /* bInterval */

	/* BULK IN Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_VENDOR_BULK_IN_EP,                             /* bEndpointAddress */
	0x02,                                           /* bmAttributes: BULK */
	USB_LOW_BYTE(USBD_VENDOR_FS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_FS_MAX_PACKET_SIZE),
	0x00,                                            /* bInterval */

	/* ISO OUT Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_VENDOR_ISO_OUT_EP,                            /* bEndpointAddress */
	0x01,                                           /* bmAttributes: ISO */
	USB_LOW_BYTE(USBD_VENDOR_FS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_FS_MAX_PACKET_SIZE),
	USBD_VENDOR_FS_ISO_OUT_INTERVAL,                                           /* bInterval */

	/* ISO IN Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_VENDOR_ISO_IN_EP,                             /* bEndpointAddress */
	0x01,                                           /* bmAttributes: ISO */
	USB_LOW_BYTE(USBD_VENDOR_FS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_FS_MAX_PACKET_SIZE),
	USBD_VENDOR_FS_ISO_IN_INTERVAL,                                            /* bInterval */

	/* Interface1 Descriptor */
	USB_LEN_IF_DESC,                                /* bLength */
	USB_DESC_TYPE_INTERFACE,                        /* bDescriptorType */
	0x00,                                           /* bInterfaceNumber */
	0x01,                                           /* bAlternateSetting */
	0x02,                                           /* bNumEndpoints */
	0xff,                                           /* bInterfaceClass: Vendor */
	0x00,                                           /* bInterfaceSubClass: No */
	0x00,                                           /* bInterfaceProtocol: None */
	0x00,                                           /* iInterface */

	/* BULK OUT Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_VENDOR_BULK_OUT_EP,                            /* bEndpointAddress */
	0x02,                                           /* bmAttributes: BULK */
	USB_LOW_BYTE(USBD_VENDOR_FS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_FS_MAX_PACKET_SIZE),
	0x00,                                           /* bInterval */

	/* BULK IN Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_VENDOR_BULK_IN_EP,                             /* bEndpointAddress */
	0x02,                                           /* bmAttributes: BULK */
	USB_LOW_BYTE(USBD_VENDOR_FS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_FS_MAX_PACKET_SIZE),
	0x00                                            /* bInterval */
};  /* usbd_vendor_other_speed_config_desc */

/* Vendor Class Driver */
usbd_class_driver_t usbd_vendor_driver = {
	.get_descriptor = usbd_vendor_get_descriptor,
	.set_config = usbd_vendor_set_config,
	.clear_config = usbd_vendor_clear_config,
	.setup = usbd_vendor_setup,
	.ep_data_in = usbd_vendor_handle_ep_data_in,
	.ep_data_out = usbd_vendor_handle_ep_data_out,
};

/* Vendor Device */
static usbd_vendor_dev_t usbd_vendor_dev;
static u32 vendor_iso_in_value = 0;
#if CONFIG_USBD_VENDOR_ISO_OUT_TEST
static u32 vendor_iso_out_cnt = 0;
#endif
u32 iso_out_sum[USBD_VENDOR_ISO_OUT_XFER_CNT];
/* Private functions ---------------------------------------------------------*/

void usbd_vendor_send_data(u16 len)
{
	usbd_vendor_dev_t *cdev = &usbd_vendor_dev;

	/* Prepare to send ISO IN packet */
	rtw_memset(cdev->in_buf, (vendor_iso_in_value & 0xFF), len);
	vendor_iso_in_value++;
	usbd_ep_transmit(cdev->dev, USBD_VENDOR_ISO_IN_EP, cdev->in_buf, len);
}

/**
  * @brief  Set Vendor class configuration
  * @param  dev: USB device instance
  * @param  config: USB configuration index
  * @retval Status
  */
static u8 usbd_vendor_set_config(usb_dev_t *dev, u8 config)
{
	u16 ep_mps;
	u8 ret = HAL_OK;
	usbd_vendor_dev_t *cdev = &usbd_vendor_dev;

	UNUSED(config);

	cdev->dev = dev;

#if CONFIG_USBD_VENDOR_BULK_INOUT_TEST
	/* Init BULK IN EP */
	ep_mps = (dev->dev_speed == USB_SPEED_HIGH) ? USBD_VENDOR_HS_MAX_PACKET_SIZE : USBD_VENDOR_FS_MAX_PACKET_SIZE;
	usbd_ep_init(dev, USBD_VENDOR_BULK_IN_EP, USB_CH_EP_TYPE_BULK, ep_mps);

	/* Init BULK OUT EP */
	ep_mps = (dev->dev_speed == USB_SPEED_HIGH) ? USBD_VENDOR_HS_MAX_PACKET_SIZE : USBD_VENDOR_FS_MAX_PACKET_SIZE;
	usbd_ep_init(dev, USBD_VENDOR_BULK_OUT_EP, USB_CH_EP_TYPE_BULK, ep_mps);

	/* Prepare to receive next BULK OUT packet */
	usbd_ep_receive(dev, USBD_VENDOR_BULK_OUT_EP, cdev->out_buf, USBD_VENDOR_OUT_BUF_SIZE);
#endif

#if CONFIG_USBD_VENDOR_ISO_IN_TEST
	/* Init ISO IN EP */
	ep_mps = (dev->dev_speed == USB_SPEED_HIGH) ? USBD_VENDOR_HS_MAX_PACKET_SIZE : USBD_VENDOR_FS_MAX_PACKET_SIZE;
	usbd_ep_init(dev, USBD_VENDOR_ISO_IN_EP, USB_CH_EP_TYPE_ISOC, ep_mps);
	usbd_vendor_send_data(USBD_VENDOR_IN_BUF_SIZE);
#endif

#if CONFIG_USBD_VENDOR_ISO_OUT_TEST
	/* Init ISO OUT EP */
	ep_mps = (dev->dev_speed == USB_SPEED_HIGH) ? USBD_VENDOR_HS_MAX_PACKET_SIZE : USBD_VENDOR_FS_MAX_PACKET_SIZE;
	usbd_ep_init(dev, USBD_VENDOR_ISO_OUT_EP, USB_CH_EP_TYPE_ISOC, ep_mps);

	/* Prepare to receive next ISO OUT packet */
	usbd_ep_receive(dev, USBD_VENDOR_ISO_OUT_EP, cdev->out_buf, USBD_VENDOR_OUT_BUF_SIZE);
#endif

	return ret;
}

/**
  * @brief  Clear Vendor configuration
  * @param  dev: USB device instance
  * @param  config: USB configuration index
  * @retval Status
  */
static u8 usbd_vendor_clear_config(usb_dev_t *dev, u8 config)
{
	u8 ret = 0U;

	UNUSED(config);

#if CONFIG_USBD_VENDOR_BULK_INOUT_TEST
	/* DeInit BULK IN EP */
	usbd_ep_deinit(dev, USBD_VENDOR_BULK_IN_EP);

	/* DeInit BULK OUT EP */
	usbd_ep_deinit(dev, USBD_VENDOR_BULK_OUT_EP);
#endif

#if CONFIG_USBD_VENDOR_ISO_IN_TEST
	/* DeInit ISO IN EP */
	usbd_ep_deinit(dev, USBD_VENDOR_ISO_IN_EP);
#endif

#if CONFIG_USBD_VENDOR_ISO_OUT_TEST
	/* DeInit ISO OUT EP */
	usbd_ep_deinit(dev, USBD_VENDOR_ISO_OUT_EP);
#endif

	return ret;
}

/**
  * @brief  Handle Vendor specific CTRL requests
  * @param  dev: USB device instance
  * @param  req: USB CTRL requests
  * @retval Status
  */
static u8 usbd_vendor_setup(usb_dev_t *dev, usb_setup_req_t *req)
{
	u8 ret = HAL_OK;

	DBG_PRINTF(MODULE_USB_OTG, LEVEL_TRACE,
			   "usbd_vendor_setup bmRequestType=0x%02X bRequest=0x%02X wLength=0x%04X wValue=%x\n",
			   req->bmRequestType,
			   req->bRequest,
			   req->wLength,
			   req->wValue);

	switch (req->bmRequestType & USB_REQ_TYPE_MASK) {
	case USB_REQ_TYPE_CLASS :
	case USB_REQ_TYPE_VENDOR:
		rtw_memcpy(dev->ctrl_buf, (void *)req, sizeof(usb_setup_req_t));
		usbd_ep0_transmit(dev, dev->ctrl_buf, sizeof(usb_setup_req_t));
		break;
	default:
		ret = HAL_ERR_HW;
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
static u8 usbd_vendor_handle_ep_data_in(usb_dev_t *dev, u8 ep_num, u8 status)
{
	UNUSED(dev);

	if (status == HAL_OK) {
		DBG_PRINTF(MODULE_USB_OTG, LEVEL_TRACE, "EP%d TX done\n", ep_num);
	} else {
		DBG_PRINTF(MODULE_USB_OTG, LEVEL_WARN, "EP%d TX error: %d\n", ep_num, status);
	}

#if CONFIG_USBD_VENDOR_ISO_IN_TEST
	usbd_vendor_send_data(USBD_VENDOR_IN_BUF_SIZE);
#endif

	return HAL_OK;
}

/**
  * @brief  Data received on non-control Out endpoint
  * @param  dev: USB device instance
  * @param  ep_num: endpoint number
  * @retval Status
  */
static u8 usbd_vendor_handle_ep_data_out(usb_dev_t *dev, u8 ep_num, u16 len)
{
	usbd_vendor_dev_t *cdev = &usbd_vendor_dev;

	UNUSED(ep_num);
	UNUSED(len);

#if CONFIG_USBD_VENDOR_BULK_INOUT_TEST
	rtw_memset((void *) cdev->in_buf, 0x55, USBD_VENDOR_IN_BUF_SIZE);
	usbd_ep_transmit(dev, USBD_VENDOR_BULK_IN_EP, cdev->in_buf, USBD_VENDOR_IN_BUF_SIZE);

	rtw_memset((void *) cdev->out_buf, 0, USBD_VENDOR_OUT_BUF_SIZE);
	usbd_ep_receive(dev, USBD_VENDOR_BULK_OUT_EP, cdev->out_buf, USBD_VENDOR_OUT_BUF_SIZE);
#endif

#if CONFIG_USBD_VENDOR_ISO_OUT_TEST
	u32 i = 0;

	for (i = 0; i < USBD_VENDOR_OUT_BUF_SIZE; i++) {
		iso_out_sum[vendor_iso_out_cnt] += cdev->out_buf[i];
	}
	iso_out_sum[vendor_iso_out_cnt] /= USBD_VENDOR_OUT_BUF_SIZE;

	vendor_iso_out_cnt++;
	if (USBD_VENDOR_ISO_OUT_XFER_CNT == vendor_iso_out_cnt) {
		for (i = 0; i < vendor_iso_out_cnt; i++) {
			if (iso_out_sum[i] != i) {
				DBG_8195A("usbd iso out data fail\n");
				return HAL_ERR_HW;
			}
		}
		DBG_8195A("usbd iso out data success\n");
		vendor_iso_out_cnt = 0;
	}

	rtw_memset((void *) cdev->out_buf, 0, USBD_VENDOR_OUT_BUF_SIZE);
	usbd_ep_receive(dev, USBD_VENDOR_ISO_OUT_EP, cdev->out_buf, USBD_VENDOR_OUT_BUF_SIZE);
#endif

	return HAL_OK;
}

/**
  * @brief  Get descriptor callback
  * @param  req: setup request
  * @param  speed: device speed
  * @param  len: descriptor length
  * @retval descriptor buffer
  */
static u8 *usbd_vendor_get_descriptor(usb_setup_req_t *req, usb_speed_type_t speed, u16 *len)
{
	u8 *buf = NULL;

	switch ((req->wValue >> 8) & 0xFF) {

	case USB_DESC_TYPE_DEVICE:
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "Get descriptor USB_DESC_TYPE_DEVICE\n");
		buf = usbd_vendor_dev_desc;
		*len = sizeof(usbd_vendor_dev_desc);
		break;

	case USB_DESC_TYPE_CONFIGURATION:
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "Get descriptor USB_DESC_TYPE_CONFIGURATION\n");
		if (speed == USB_SPEED_HIGH) {
			buf = usbd_vendor_hs_config_desc;
			*len = sizeof(usbd_vendor_hs_config_desc);
		} else {
			buf = usbd_vendor_fs_config_desc;
			*len = sizeof(usbd_vendor_fs_config_desc);
		}
		break;

	case USB_DESC_TYPE_DEVICE_QUALIFIER:
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "Get descriptor USB_DESC_TYPE_DEVICE_QUALIFIER\n");
		if (speed == USB_SPEED_HIGH) {
			buf = usbd_vendor_device_qualifier_desc;
			*len = sizeof(usbd_vendor_device_qualifier_desc);
		} else {
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "Trying to get descriptor USB_DESC_TYPE_DEVICE_QUALIFIER at full speed\n");
		}
		break;

	case USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION:
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "Get descriptor USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION\n");
		if (speed == USB_SPEED_HIGH) {
			buf = usbd_vendor_other_speed_config_desc;
			*len = sizeof(usbd_vendor_other_speed_config_desc);
		} else {
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "Trying to get descriptor USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION at full speed\n");
		}
		break;

	case USB_DESC_TYPE_STRING:
		switch (req->wValue & 0xFF) {
		case USBD_IDX_LANGID_STR:
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "Get descriptor USBD_IDX_LANGID_STR\n");
			buf = usbd_vendor_lang_id_desc;
			*len = sizeof(usbd_vendor_lang_id_desc);
			break;
		case USBD_IDX_MFC_STR:
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "Get descriptor USBD_IDX_MFC_STR\n");
			buf = usbd_vendor_mfg_string_desc;
			*len = USBD_VENDOR_MFG_STRING_DESC_SIZE;
			break;
		case USBD_IDX_PRODUCT_STR:
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "Get descriptor USBD_IDX_PRODUCT_STR\n");
			*len = USBD_VENDOR_PRODUCT_STRING_DESC_SIZE;
			if (speed == USB_SPEED_HIGH) {
				buf = usbd_vendor_hs_product_string_desc;
			} else {
				buf = usbd_vendor_fs_product_string_desc;
			}
			break;
		case USBD_IDX_SERIAL_STR:
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "Get descriptor USBD_IDX_SERIAL_STR\n");
			buf = usbd_vendor_sn_string_desc;
			*len = USBD_VENDOR_SN_STRING_DESC_SIZE;
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
  * @brief  Initialize vendor device
  * @retval Status
  */
u8 usbd_vendor_init(void)
{
	u8 ret = HAL_OK;
	usbd_vendor_dev_t *cdev = &usbd_vendor_dev;

	cdev->out_buf = rtw_zmalloc(USBD_VENDOR_OUT_BUF_SIZE);
	if (cdev->out_buf == NULL) {
		ret = HAL_ERR_MEM;
		goto Init_exit;
	}

	cdev->in_buf = rtw_zmalloc(USBD_VENDOR_IN_BUF_SIZE);
	if (cdev->in_buf == NULL) {
		ret = HAL_ERR_MEM;
		goto Init_clean_bulk_out_buf_exit;
	}

	rtw_memset((void *) iso_out_sum, 0, (USBD_VENDOR_ISO_OUT_XFER_CNT * 4));

	usbd_register_class(&usbd_vendor_driver);

	return ret;

Init_clean_bulk_out_buf_exit:
	rtw_free(cdev->out_buf);
	cdev->out_buf = NULL;

Init_exit:
	return ret;
}

/**
  * @brief  DeInitialize vendor device
  * @param  void
  * @retval Status
  */
u8 usbd_vendor_deinit(void)
{
	usbd_vendor_dev_t *cdev = &usbd_vendor_dev;

	usbd_unregister_class();

	if (cdev->in_buf != NULL) {
		rtw_free(cdev->in_buf);
		cdev->in_buf = NULL;
	}

	if (cdev->out_buf != NULL) {
		rtw_free(cdev->out_buf);
		cdev->out_buf = NULL;
	}

	return HAL_OK;
}

