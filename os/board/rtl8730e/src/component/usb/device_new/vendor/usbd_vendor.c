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
static u8 *usbd_vendor_get_descriptor(usb_dev_t *dev, usb_setup_req_t *req, usb_speed_type_t speed, u16 *len);
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
	USB_LEN_CFG_DESC,								/* bLength */
	USB_DESC_TYPE_CONFIGURATION,					/* bDescriptorType */
	USBD_VENDOR_CONFIG_DESC_SIZE,					/* wTotalLength */
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
	USBD_VENDOR_CLASS_CODE,                         /* bInterfaceClass: Vendor */
	USBD_VENDOR_SUBCLASS_CODE,                      /* bInterfaceSubClass */
	USBD_VENDOR_PROTOCOL,                 			/* bInterfaceProtocol */
	0x00,                                           /* iInterface */

	/* INTR OUT Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_VENDOR_INTR_OUT_EP,						/* bEndpointAddress */
	USB_CH_EP_TYPE_INTR,							/* bmAttributes: INTR */
	USB_LOW_BYTE(USBD_VENDOR_HS_INTR_MPS),			/* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_HS_INTR_MPS),
	USBD_VENDOR_HS_INTR_OUT_INTERVAL,				/* bInterval */

	/* INTR IN Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_VENDOR_INTR_IN_EP,							/* bEndpointAddress */
	USB_CH_EP_TYPE_INTR,							/* bmAttributes: INTR */
	USB_LOW_BYTE(USBD_VENDOR_HS_INTR_MPS),			/* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_HS_INTR_MPS),
	USBD_VENDOR_HS_INTR_IN_INTERVAL,				/* bInterval */

	/* ISOC OUT Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_VENDOR_ISOC_OUT_EP,						/* bEndpointAddress */
	USB_CH_EP_TYPE_ISOC,							/* bmAttributes: ISOC */
	USB_LOW_BYTE(USBD_VENDOR_HS_ISOC_MPS),			/* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_HS_ISOC_MPS),
	USBD_VENDOR_HS_ISOC_OUT_INTERVAL,				/* bInterval */

	/* ISOC IN Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_VENDOR_ISOC_IN_EP,							/* bEndpointAddress */
	USB_CH_EP_TYPE_ISOC,							/* bmAttributes: ISOC */
	USB_LOW_BYTE(USBD_VENDOR_HS_ISOC_MPS),			/* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_HS_ISOC_MPS),
	USBD_VENDOR_HS_ISOC_IN_INTERVAL,				/* bInterval */

	/* Interface1 Descriptor */
	USB_LEN_IF_DESC,                                /* bLength */
	USB_DESC_TYPE_INTERFACE,                        /* bDescriptorType */
	0x00,                                           /* bInterfaceNumber */
	0x01,                                           /* bAlternateSetting */
	0x04,                                           /* bNumEndpoints */
	USBD_VENDOR_CLASS_CODE,                         /* bInterfaceClass: Vendor */
	USBD_VENDOR_SUBCLASS_CODE,                      /* bInterfaceSubClass */
	USBD_VENDOR_PROTOCOL,                 			/* bInterfaceProtocol */
	0x00,                                           /* iInterface */

	/* BULK OUT Endpoint Descriptor */
	USB_LEN_EP_DESC,								/* bLength */
	USB_DESC_TYPE_ENDPOINT,							/* bDescriptorType */
	USBD_VENDOR_BULK_OUT_EP,						/* bEndpointAddress */
	USB_CH_EP_TYPE_BULK,							/* bmAttributes: BULK */
	USB_LOW_BYTE(USBD_VENDOR_HS_BULK_MPS),			/* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_HS_BULK_MPS),
	0x00,											/* bInterval */

	/* BULK IN Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_VENDOR_BULK_IN_EP,							/* bEndpointAddress */
	USB_CH_EP_TYPE_BULK,							/* bmAttributes: BULK */
	USB_LOW_BYTE(USBD_VENDOR_HS_BULK_MPS),			/* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_HS_BULK_MPS),
	0x00,											/* bInterval */

	/* ISO OUT Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_VENDOR_ISOC_OUT_EP,						/* bEndpointAddress */
	USB_CH_EP_TYPE_ISOC,							/* bmAttributes: ISOC */
	USB_LOW_BYTE(USBD_VENDOR_HS_ISOC_MPS),			/* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_HS_ISOC_MPS),
	USBD_VENDOR_HS_ISOC_OUT_INTERVAL,				/* bInterval */

	/* ISO IN Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_VENDOR_ISOC_IN_EP,							/* bEndpointAddress */
	USB_CH_EP_TYPE_ISOC,							/* bmAttributes: ISOC */
	USB_LOW_BYTE(USBD_VENDOR_HS_ISOC_MPS),			/* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_HS_ISOC_MPS),
	USBD_VENDOR_HS_ISOC_IN_INTERVAL,				/* bInterval */
};  /* usbd_vendor_hs_config_desc */

/* USB Vendor Device Full Speed Configuration Descriptor */
static u8 usbd_vendor_fs_config_desc[USBD_VENDOR_CONFIG_DESC_SIZE] USB_DMA_ALIGNED = {
	/* USB Standard Configuration Descriptor */
	USB_LEN_CFG_DESC,                               /* bLength */
	USB_DESC_TYPE_CONFIGURATION,                    /* bDescriptorType */
	USBD_VENDOR_CONFIG_DESC_SIZE,					/* wTotalLength */
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
	USBD_VENDOR_CLASS_CODE,                         /* bInterfaceClass: Vendor */
	USBD_VENDOR_SUBCLASS_CODE,                      /* bInterfaceSubClass*/
	USBD_VENDOR_PROTOCOL,                 			/* bInterfaceProtocol */
	0x00,                                           /* iInterface */

	/* INTR OUT Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_VENDOR_INTR_OUT_EP,                        /* bEndpointAddress */
	USB_CH_EP_TYPE_INTR,							/* bmAttributes: INTR */
	USB_LOW_BYTE(USBD_VENDOR_FS_INTR_MPS),          /* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_FS_INTR_MPS),
	USBD_VENDOR_FS_INTR_OUT_INTERVAL,				/* bInterval */

	/* INTR IN Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_VENDOR_INTR_IN_EP,                         /* bEndpointAddress */
	USB_CH_EP_TYPE_INTR,							/* bmAttributes: INTR */
	USB_LOW_BYTE(USBD_VENDOR_FS_INTR_MPS),          /* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_FS_INTR_MPS),
	USBD_VENDOR_FS_INTR_IN_INTERVAL,				/* bInterval */

	/* ISOC OUT Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_VENDOR_ISOC_OUT_EP,                        /* bEndpointAddress */
	USB_CH_EP_TYPE_ISOC,                            /* bmAttributes: ISOC */
	USB_LOW_BYTE(USBD_VENDOR_FS_ISOC_MPS),          /* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_FS_ISOC_MPS),
	USBD_VENDOR_FS_ISOC_OUT_INTERVAL,               /* bInterval */

	/* ISOC IN Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_VENDOR_ISOC_IN_EP,                         /* bEndpointAddress */
	USB_CH_EP_TYPE_ISOC,                            /* bmAttributes: ISOC */
	USB_LOW_BYTE(USBD_VENDOR_FS_ISOC_MPS),          /* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_FS_ISOC_MPS),
	USBD_VENDOR_FS_ISOC_IN_INTERVAL,                /* bInterval */

	/* Interface1 Descriptor */
	USB_LEN_IF_DESC,                                /* bLength */
	USB_DESC_TYPE_INTERFACE,                        /* bDescriptorType */
	0x00,                                           /* bInterfaceNumber */
	0x01,                                           /* bAlternateSetting */
	0x04,                                           /* bNumEndpoints */
	USBD_VENDOR_CLASS_CODE,                         /* bInterfaceClass: Vendor */
	USBD_VENDOR_SUBCLASS_CODE,                      /* bInterfaceSubClass */
	USBD_VENDOR_PROTOCOL,                 			/* bInterfaceProtocol */
	0x00,                                           /* iInterface */

	/* BULK OUT Endpoint Descriptor */
	USB_LEN_EP_DESC,								/* bLength */
	USB_DESC_TYPE_ENDPOINT,							/* bDescriptorType */
	USBD_VENDOR_BULK_OUT_EP,						/* bEndpointAddress */
	USB_CH_EP_TYPE_BULK,							/* bmAttributes: BULK */
	USB_LOW_BYTE(USBD_VENDOR_FS_BULK_MPS),			/* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_FS_BULK_MPS),
	0x00,											/* bInterval */

	/* BULK IN Endpoint Descriptor */
	USB_LEN_EP_DESC,								/* bLength */
	USB_DESC_TYPE_ENDPOINT,							/* bDescriptorType */
	USBD_VENDOR_BULK_IN_EP,							/* bEndpointAddress */
	USB_CH_EP_TYPE_BULK,							/* bmAttributes: BULK */
	USB_LOW_BYTE(USBD_VENDOR_FS_BULK_MPS),			/* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_FS_BULK_MPS),
	0x00,											/* bInterval */

	/* ISOC OUT Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_VENDOR_ISOC_OUT_EP,                        /* bEndpointAddress */
	USB_CH_EP_TYPE_ISOC,                            /* bmAttributes: ISOC */
	USB_LOW_BYTE(USBD_VENDOR_FS_ISOC_MPS),          /* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_FS_ISOC_MPS),
	USBD_VENDOR_FS_ISOC_OUT_INTERVAL,               /* bInterval */

	/* ISOC IN Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_VENDOR_ISOC_IN_EP,                         /* bEndpointAddress */
	USB_CH_EP_TYPE_ISOC,                            /* bmAttributes: ISOC */
	USB_LOW_BYTE(USBD_VENDOR_FS_ISOC_MPS),          /* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_FS_ISOC_MPS),
	USBD_VENDOR_FS_ISOC_IN_INTERVAL,                /* bInterval */
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
	USBD_VENDOR_CLASS_CODE,                         /* bInterfaceClass: Vendor */
	USBD_VENDOR_SUBCLASS_CODE,                      /* bInterfaceSubClass*/
	USBD_VENDOR_PROTOCOL,                 			/* bInterfaceProtocol */
	0x00,                                           /* iInterface */

	/* INTR OUT Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_VENDOR_INTR_OUT_EP,                        /* bEndpointAddress */
	USB_CH_EP_TYPE_INTR,                            /* bmAttributes: INTR */
	USB_LOW_BYTE(USBD_VENDOR_FS_INTR_MPS),          /* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_FS_INTR_MPS),
	USBD_VENDOR_FS_INTR_OUT_INTERVAL,				/* bInterval */

	/* INTR IN Endpoint Descriptor */
	USB_LEN_EP_DESC,								/* bLength */
	USB_DESC_TYPE_ENDPOINT,							/* bDescriptorType */
	USBD_VENDOR_INTR_IN_EP,							/* bEndpointAddress */
	USB_CH_EP_TYPE_INTR,							/* bmAttributes: INTR */
	USB_LOW_BYTE(USBD_VENDOR_FS_INTR_MPS),			/* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_FS_INTR_MPS),
	USBD_VENDOR_FS_INTR_IN_INTERVAL,				/* bInterval */

	/* ISOC OUT Endpoint Descriptor */
	USB_LEN_EP_DESC,								/* bLength */
	USB_DESC_TYPE_ENDPOINT,							/* bDescriptorType */
	USBD_VENDOR_ISOC_OUT_EP,						/* bEndpointAddress */
	USB_CH_EP_TYPE_ISOC,                            /* bmAttributes: ISOC */
	USB_LOW_BYTE(USBD_VENDOR_FS_ISOC_MPS),			/* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_FS_ISOC_MPS),
	USBD_VENDOR_FS_ISOC_OUT_INTERVAL,				/* bInterval */

	/* ISOC IN Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_VENDOR_ISOC_IN_EP,                         /* bEndpointAddress */
	USB_CH_EP_TYPE_ISOC,                            /* bmAttributes: ISOC */
	USB_LOW_BYTE(USBD_VENDOR_FS_ISOC_MPS),			/* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_FS_ISOC_MPS),
	USBD_VENDOR_FS_ISOC_IN_INTERVAL,                /* bInterval */

	/* Interface1 Descriptor */
	USB_LEN_IF_DESC,                                /* bLength */
	USB_DESC_TYPE_INTERFACE,                        /* bDescriptorType */
	0x00,                                           /* bInterfaceNumber */
	0x01,                                           /* bAlternateSetting */
	0x04,                                           /* bNumEndpoints */
	USBD_VENDOR_CLASS_CODE,                         /* bInterfaceClass: Vendor */
	USBD_VENDOR_SUBCLASS_CODE,                      /* bInterfaceSubClass */
	USBD_VENDOR_PROTOCOL,                 			/* bInterfaceProtocol */
	0x00,                                           /* iInterface */

	/* BULK OUT Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_VENDOR_BULK_OUT_EP,                        /* bEndpointAddress */
	USB_CH_EP_TYPE_BULK,							/* bmAttributes: BULK */
	USB_LOW_BYTE(USBD_VENDOR_FS_BULK_MPS),			/* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_FS_BULK_MPS),
	0x00,                                           /* bInterval */

	/* BULK IN Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_VENDOR_BULK_IN_EP,                         /* bEndpointAddress */
	USB_CH_EP_TYPE_BULK,							/* bmAttributes: BULK */
	USB_LOW_BYTE(USBD_VENDOR_FS_BULK_MPS),			/* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_FS_BULK_MPS),
	0x00,                                           /* bInterval */

	/* ISOC OUT Endpoint Descriptor */
	USB_LEN_EP_DESC,								/* bLength */
	USB_DESC_TYPE_ENDPOINT,							/* bDescriptorType */
	USBD_VENDOR_ISOC_OUT_EP,						/* bEndpointAddress */
	USB_CH_EP_TYPE_ISOC,                            /* bmAttributes: ISOC */
	USB_LOW_BYTE(USBD_VENDOR_FS_ISOC_MPS),			/* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_FS_ISOC_MPS),
	USBD_VENDOR_FS_ISOC_OUT_INTERVAL,				/* bInterval */

	/* ISOC IN Endpoint Descriptor */
	USB_LEN_EP_DESC,                                /* bLength */
	USB_DESC_TYPE_ENDPOINT,                         /* bDescriptorType */
	USBD_VENDOR_ISOC_IN_EP,                         /* bEndpointAddress */
	USB_CH_EP_TYPE_ISOC,                            /* bmAttributes: ISOC */
	USB_LOW_BYTE(USBD_VENDOR_FS_ISOC_MPS),			/* wMaxPacketSize: */
	USB_HIGH_BYTE(USBD_VENDOR_FS_ISOC_MPS),
	USBD_VENDOR_FS_ISOC_IN_INTERVAL,                /* bInterval */
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

/* Private functions ---------------------------------------------------------*/

#if CONFIG_USBD_VENDOR_BULK_TEST

/**
  * @brief  Transmit BULK IN ZLP packet
  * @retval Status
  */
static u8 usbd_vendor_transmit_bulk_zlp(void)
{
	usbd_vendor_dev_t *cdev = &usbd_vendor_dev;

	DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "BULK TX ZLP\n");
	usbd_ep_transmit(cdev->dev, USBD_VENDOR_BULK_IN_EP, NULL, 0);

	return HAL_OK;
}

#endif

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
	cdev->alt_setting = 0U;

#if CONFIG_USBD_VENDOR_INTR_TEST
	/* Init INTR IN EP */
	ep_mps = (dev->dev_speed == USB_SPEED_HIGH) ? USBD_VENDOR_HS_INTR_MPS : USBD_VENDOR_FS_INTR_MPS;
	usbd_ep_init(dev, USBD_VENDOR_INTR_IN_EP, USB_CH_EP_TYPE_INTR, ep_mps);
	cdev->intr_in_state = 0;

	/* Init INTR OUT EP */
	ep_mps = (dev->dev_speed == USB_SPEED_HIGH) ? USBD_VENDOR_HS_INTR_MPS : USBD_VENDOR_FS_INTR_MPS;
	usbd_ep_init(dev, USBD_VENDOR_INTR_OUT_EP, USB_CH_EP_TYPE_INTR, ep_mps);
	usbd_ep_receive(dev, USBD_VENDOR_INTR_OUT_EP, cdev->intr_out_buf, USBD_VENDOR_INTR_OUT_BUF_SIZE);
#endif

#if CONFIG_USBD_VENDOR_ISOC_TEST
	/* Init ISO IN EP */
	ep_mps = (dev->dev_speed == USB_SPEED_HIGH) ? USBD_VENDOR_HS_ISOC_MPS : USBD_VENDOR_FS_ISOC_MPS;
	usbd_ep_init(dev, USBD_VENDOR_ISOC_IN_EP, USB_CH_EP_TYPE_ISOC, ep_mps);

	/* Init ISO OUT EP */
	ep_mps = (dev->dev_speed == USB_SPEED_HIGH) ? USBD_VENDOR_HS_ISOC_MPS : USBD_VENDOR_FS_ISOC_MPS;
	usbd_ep_init(dev, USBD_VENDOR_ISOC_OUT_EP, USB_CH_EP_TYPE_ISOC, ep_mps);
	usbd_ep_receive(dev, USBD_VENDOR_ISOC_OUT_EP, cdev->isoc_out_buf, USBD_VENDOR_ISOC_OUT_BUF_SIZE);
#endif

	if (cdev->cb->set_config != NULL) {
		cdev->cb->set_config();
	}

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
	usbd_vendor_dev_t *cdev = &usbd_vendor_dev;

	UNUSED(config);

	if (cdev->alt_setting == 0U) {
#if CONFIG_USBD_VENDOR_INTR_TEST
		/* DeInit INTR IN EP */
		usbd_ep_deinit(dev, USBD_VENDOR_INTR_IN_EP);

		/* DeInit INTR OUT EP */
		usbd_ep_deinit(dev, USBD_VENDOR_INTR_OUT_EP);
#endif
	} else {
#if CONFIG_USBD_VENDOR_BULK_TEST
		/* DeInit BULK IN EP */
		usbd_ep_deinit(dev, USBD_VENDOR_BULK_IN_EP);

		/* DeInit BULK OUT EP */
		usbd_ep_deinit(dev, USBD_VENDOR_BULK_OUT_EP);
#endif
	}

#if CONFIG_USBD_VENDOR_ISOC_TEST
	/* DeInit ISOC IN EP */
	usbd_ep_deinit(dev, USBD_VENDOR_ISOC_IN_EP);

	/* DeInit ISOC OUT EP */
	usbd_ep_deinit(dev, USBD_VENDOR_ISOC_OUT_EP);
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
	u16 ep_mps;
	u8 ret = HAL_OK;
	usbd_vendor_dev_t *cdev = &usbd_vendor_dev;

	DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE,
			   "usbd_vendor_setup bmRequestType=0x%02X bRequest=0x%02X wLength=0x%04X wValue=%x\n",
			   req->bmRequestType,
			   req->bRequest,
			   req->wLength,
			   req->wValue);

	switch (req->bmRequestType & USB_REQ_TYPE_MASK) {
	case USB_REQ_TYPE_STANDARD:
		switch (req->bRequest) {
		case USB_REQ_SET_INTERFACE:
			if (dev->dev_state == USBD_STATE_CONFIGURED) {
				if (req->wIndex == 0U) {
					cdev->alt_setting = (u8)(req->wValue & 0xFF);
#if CONFIG_USBD_VENDOR_ISOC_TEST
					/* DeInit ISOC IN EP */
					usbd_ep_deinit(dev, USBD_VENDOR_ISOC_IN_EP);

					/* DeInit ISOC OUT EP */
					usbd_ep_deinit(dev, USBD_VENDOR_ISOC_OUT_EP);
#endif
					if (cdev->alt_setting == 0U) {
#if CONFIG_USBD_VENDOR_BULK_TEST
						/* DeInit BULK IN EP */
						usbd_ep_deinit(dev, USBD_VENDOR_BULK_IN_EP);

						/* DeInit BULK OUT EP */
						usbd_ep_deinit(dev, USBD_VENDOR_BULK_OUT_EP);
#endif
#if CONFIG_USBD_VENDOR_INTR_TEST
						/* Init INTR IN EP */
						ep_mps = (dev->dev_speed == USB_SPEED_HIGH) ? USBD_VENDOR_HS_INTR_MPS : USBD_VENDOR_FS_INTR_MPS;
						usbd_ep_init(dev, USBD_VENDOR_INTR_IN_EP, USB_CH_EP_TYPE_INTR, ep_mps);
						cdev->intr_in_state = 0;

						/* Init INTR OUT EP */
						ep_mps = (dev->dev_speed == USB_SPEED_HIGH) ? USBD_VENDOR_HS_INTR_MPS : USBD_VENDOR_FS_INTR_MPS;
						usbd_ep_init(dev, USBD_VENDOR_INTR_OUT_EP, USB_CH_EP_TYPE_INTR, ep_mps);
						usbd_ep_receive(dev, USBD_VENDOR_INTR_OUT_EP, cdev->intr_out_buf, USBD_VENDOR_INTR_OUT_BUF_SIZE);
#endif
					} else {
#if CONFIG_USBD_VENDOR_INTR_TEST
						/* DeInit INTR IN EP */
						usbd_ep_deinit(dev, USBD_VENDOR_INTR_IN_EP);

						/* DeInit INTR OUT EP */
						usbd_ep_deinit(dev, USBD_VENDOR_INTR_OUT_EP);
#endif
#if CONFIG_USBD_VENDOR_BULK_TEST
						/* Init BULK IN EP */
						ep_mps = (dev->dev_speed == USB_SPEED_HIGH) ? USBD_VENDOR_HS_BULK_MPS : USBD_VENDOR_FS_BULK_MPS;
						usbd_ep_init(dev, USBD_VENDOR_BULK_IN_EP, USB_CH_EP_TYPE_BULK, ep_mps);
						cdev->bulk_in_state = 0;

						/* Init BULK OUT EP */
						ep_mps = (dev->dev_speed == USB_SPEED_HIGH) ? USBD_VENDOR_HS_BULK_MPS : USBD_VENDOR_FS_BULK_MPS;
						usbd_ep_init(dev, USBD_VENDOR_BULK_OUT_EP, USB_CH_EP_TYPE_BULK, ep_mps);
						usbd_ep_receive(dev, USBD_VENDOR_BULK_OUT_EP, cdev->bulk_out_buf, USBD_VENDOR_BULK_OUT_BUF_SIZE);
#endif
					}
#if CONFIG_USBD_VENDOR_ISOC_TEST
					/* Init ISO IN EP */
					ep_mps = (dev->dev_speed == USB_SPEED_HIGH) ? USBD_VENDOR_HS_ISOC_MPS : USBD_VENDOR_FS_ISOC_MPS;
					usbd_ep_init(dev, USBD_VENDOR_ISOC_IN_EP, USB_CH_EP_TYPE_ISOC, ep_mps);

					/* Init ISO OUT EP */
					ep_mps = (dev->dev_speed == USB_SPEED_HIGH) ? USBD_VENDOR_HS_ISOC_MPS : USBD_VENDOR_FS_ISOC_MPS;
					usbd_ep_init(dev, USBD_VENDOR_ISOC_OUT_EP, USB_CH_EP_TYPE_ISOC, ep_mps);
					usbd_ep_receive(dev, USBD_VENDOR_ISOC_OUT_EP, cdev->isoc_out_buf, USBD_VENDOR_ISOC_OUT_BUF_SIZE);
#endif
				} else {
					ret = HAL_ERR_HW;
				}
			} else {
				ret = HAL_ERR_HW;
			}
			break;
		case USB_REQ_GET_INTERFACE:
			if (dev->dev_state == USBD_STATE_CONFIGURED) {
				cdev->ctrl_buf[0] = cdev->alt_setting;
				usbd_ep0_transmit(dev, cdev->ctrl_buf, 1U);
			} else {
				ret = HAL_ERR_HW;
			}
			break;
		case USB_REQ_GET_STATUS:
			if (dev->dev_state == USBD_STATE_CONFIGURED) {
				cdev->ctrl_buf[0] = 0U;
				cdev->ctrl_buf[1] = 0U;
				usbd_ep0_transmit(dev, cdev->ctrl_buf, 2U);
			} else {
				ret = HAL_ERR_HW;
			}
			break;
		default:
			ret = HAL_ERR_HW;
			break;
		}
		break;
	case USB_REQ_TYPE_CLASS :
	case USB_REQ_TYPE_VENDOR:
		if (req->wLength) {
			if (req->bmRequestType & 0x80U) {
				cdev->cb->setup(req->bRequest, cdev->ctrl_buf, req->wLength, req->wValue);
				usbd_ep0_transmit(dev, cdev->ctrl_buf, req->wLength);
			} else {
				cdev->ctrl_req = req->bRequest;
				cdev->ctrl_data_len = req->wLength;
				usbd_ep0_receive(dev, cdev->ctrl_buf, req->wLength);
			}
		} else {
			cdev->cb->setup(req->bRequest, cdev->ctrl_buf, 0, req->wValue);
		}
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
	usbd_vendor_dev_t *cdev = &usbd_vendor_dev;
	usbd_vendor_cb_t *cb = cdev->cb;

	UNUSED(dev);

	if (cdev->alt_setting == 0U) {
#if CONFIG_USBD_VENDOR_INTR_TEST
		if (ep_num == (USBD_VENDOR_INTR_IN_EP & (~USB_REQ_DIR_MASK))) {
			cdev->intr_in_state = 0U;
			if (status == HAL_OK) {
				DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "INTR TX done\n");
			} else {
				DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "INTR TX error: %d\n", status);
			}
			if (cb->intr_transmitted != NULL) {
				cb->intr_transmitted(status);
			}
		}
#endif
	} else {
#if CONFIG_USBD_VENDOR_BULK_TEST
		if (ep_num == (USBD_VENDOR_BULK_IN_EP & (~USB_REQ_DIR_MASK))) {
			if (status == HAL_OK) {
				DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "BULK TX done\n");
				if (cdev->bulk_in_zlp) {
					cdev->bulk_in_zlp = 0;
					usbd_vendor_transmit_bulk_zlp();
				} else {
					cdev->bulk_in_state = 0U;
				}
			} else {
				DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "BULK TX error: %d\n", status);
				cdev->bulk_in_state = 0U;
			}
			if (cb->bulk_transmitted != NULL) {
				cb->bulk_transmitted(status);
			}
		}
#endif
	}

#if CONFIG_USBD_VENDOR_ISOC_TEST
	if (ep_num == (USBD_VENDOR_ISOC_IN_EP & (~USB_REQ_DIR_MASK))) {
		if (status == HAL_OK) {
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "ISOC TX done\n");
		} else {
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "ISOC TX error: %d\n", status);
		}
		if (cb->isoc_transmitted != NULL) {
			cb->isoc_transmitted(status);
		}
	}
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

	UNUSED(dev);

	if (len == 0) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "EP%d RX ZLP\n", ep_num);
		return HAL_OK;
	}

	if (cdev->alt_setting == 0U) {
#if CONFIG_USBD_VENDOR_INTR_TEST
		if (ep_num == (USBD_VENDOR_INTR_OUT_EP & (~USB_REQ_DIR_MASK))) {
			cdev->cb->intr_received(cdev->intr_out_buf, len);
		}
#endif
	} else {
#if CONFIG_USBD_VENDOR_BULK_TEST
		if (ep_num == (USBD_VENDOR_BULK_OUT_EP & (~USB_REQ_DIR_MASK))) {
			cdev->cb->bulk_received(cdev->bulk_out_buf, len);
		}
#endif
	}

#if CONFIG_USBD_VENDOR_ISOC_TEST
	if (ep_num == (USBD_VENDOR_ISOC_OUT_EP & (~USB_REQ_DIR_MASK))) {
		cdev->cb->isoc_received(cdev->isoc_out_buf, len);
	}
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
static u8 *usbd_vendor_get_descriptor(usb_dev_t *dev, usb_setup_req_t *req, usb_speed_type_t speed, u16 *len)
{
	u8 *buf = NULL;

	dev->self_powered = USBD_VENDOR_SELF_POWERED;

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
u8 usbd_vendor_init(usbd_vendor_cb_t *cb)
{
	u8 ret = HAL_OK;
	usbd_vendor_dev_t *cdev = &usbd_vendor_dev;

	cdev->ctrl_buf = rtw_zmalloc(USBD_VENDOR_CTRL_BUF_SIZE);
	if (cdev->ctrl_buf == NULL) {
		ret = HAL_ERR_MEM;
		goto init_exit;
	}

	cdev->bulk_out_buf = rtw_zmalloc(USBD_VENDOR_BULK_OUT_BUF_SIZE);
	if (cdev->bulk_out_buf == NULL) {
		ret = HAL_ERR_MEM;
		goto init_clean_ctrl_buf_exit;
	}

	cdev->bulk_in_buf = rtw_zmalloc(USBD_VENDOR_BULK_IN_BUF_SIZE);
	if (cdev->bulk_in_buf == NULL) {
		ret = HAL_ERR_MEM;
		goto init_clean_bulk_out_buf_exit;
	}

	cdev->intr_out_buf = rtw_zmalloc(USBD_VENDOR_INTR_OUT_BUF_SIZE);
	if (cdev->intr_out_buf == NULL) {
		ret = HAL_ERR_MEM;
		goto init_clean_bulk_in_buf_exit;
	}

	cdev->intr_in_buf = rtw_zmalloc(USBD_VENDOR_INTR_IN_BUF_SIZE);
	if (cdev->intr_in_buf == NULL) {
		ret = HAL_ERR_MEM;
		goto init_clean_intr_out_buf_exit;
	}

	cdev->isoc_out_buf = rtw_zmalloc(USBD_VENDOR_ISOC_OUT_BUF_SIZE);
	if (cdev->isoc_out_buf == NULL) {
		ret = HAL_ERR_MEM;
		goto init_clean_intr_in_buf_exit;
	}

	cdev->isoc_in_buf = rtw_zmalloc(USBD_VENDOR_ISOC_IN_BUF_SIZE);
	if (cdev->isoc_in_buf == NULL) {
		ret = HAL_ERR_MEM;
		goto init_clean_isoc_out_buf_exit;
	}

	if (cb != NULL) {
		cdev->cb = cb;
		if (cb->init != NULL) {
			ret = cb->init();
			if (ret != HAL_OK) {
				goto init_clean_isoc_in_buf_exit;
			}
		}
	}

	usbd_register_class(&usbd_vendor_driver);

	return ret;

init_clean_isoc_in_buf_exit:
	rtw_free(cdev->isoc_in_buf);
	cdev->isoc_in_buf = NULL;

init_clean_isoc_out_buf_exit:
	rtw_free(cdev->isoc_out_buf);
	cdev->isoc_out_buf = NULL;

init_clean_intr_in_buf_exit:
	rtw_free(cdev->intr_in_buf);
	cdev->intr_in_buf = NULL;

init_clean_intr_out_buf_exit:
	rtw_free(cdev->intr_out_buf);
	cdev->intr_out_buf = NULL;

init_clean_bulk_in_buf_exit:
	rtw_free(cdev->bulk_in_buf);
	cdev->bulk_in_buf = NULL;

init_clean_bulk_out_buf_exit:
	rtw_free(cdev->bulk_out_buf);
	cdev->bulk_out_buf = NULL;

init_clean_ctrl_buf_exit:
	rtw_free(cdev->ctrl_buf);
	cdev->ctrl_buf = NULL;

init_exit:
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

	if (cdev->cb != NULL) {
		if (cdev->cb->deinit != NULL) {
			cdev->cb->deinit();
		}
		cdev->cb = NULL;
	}

	usbd_unregister_class();

	if (cdev->ctrl_buf != NULL) {
		rtw_free(cdev->ctrl_buf);
		cdev->ctrl_buf = NULL;
	}

	if (cdev->bulk_in_buf != NULL) {
		rtw_free(cdev->bulk_in_buf);
		cdev->bulk_in_buf = NULL;
	}

	if (cdev->bulk_out_buf != NULL) {
		rtw_free(cdev->bulk_out_buf);
		cdev->bulk_out_buf = NULL;
	}

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

u8 usbd_vendor_transmit_ctrl_data(u8 *buf, u16 len)
{
	usbd_vendor_dev_t *cdev = &usbd_vendor_dev;
	usb_dev_t *dev = cdev->dev;
	u16 ep_mps = (dev->dev_speed == USB_SPEED_HIGH) ? USB_HS_MAX_PACKET_SIZE : USB_FS_MAX_PACKET_SIZE;

	if (len > ep_mps) {
		len = ep_mps;
	}

	DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "CTRL TX len=%d\n", len);

	rtw_memcpy(cdev->ctrl_buf, buf, len);

	usbd_ep0_transmit(dev, cdev->ctrl_buf, len);

	return HAL_OK;
}

u8 usbd_vendor_transmit_bulk_data(u8 *buf, u16 len)
{
	u8 ret = HAL_OK;
	usbd_vendor_dev_t *cdev = &usbd_vendor_dev;
	usb_dev_t *dev = cdev->dev;
	u16 ep_mps = (dev->dev_speed == USB_SPEED_HIGH) ? USBD_VENDOR_HS_BULK_MPS : USBD_VENDOR_FS_BULK_MPS;

	if (len > USBD_VENDOR_BULK_IN_BUF_SIZE) {
		len = USBD_VENDOR_BULK_IN_BUF_SIZE;
	}

	if (len == ep_mps) {
		cdev->bulk_in_zlp = 1;
	} else {
		cdev->bulk_in_zlp = 0;
	}

	if (cdev->bulk_in_state == 0U) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "BULK TX len=%d data=%d\n", len, buf[0]);
		cdev->bulk_in_state = 1U;
		rtw_memcpy((void *)cdev->bulk_in_buf, (void *)buf, len);
		usbd_ep_transmit(cdev->dev, USBD_VENDOR_BULK_IN_EP, cdev->bulk_in_buf, len);
	} else {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "BULK TX len=%d data=%d: BUSY\n", len, buf[0]);
		ret = HAL_BUSY;
	}

	return ret;
}

u8 usbd_vendor_receive_bulk_data(void)
{
	usbd_vendor_dev_t *cdev = &usbd_vendor_dev;

	usbd_ep_receive(cdev->dev, USBD_VENDOR_BULK_OUT_EP, cdev->bulk_out_buf, USBD_VENDOR_BULK_OUT_BUF_SIZE);

	return HAL_OK;
}

u8 usbd_vendor_transmit_intr_data(u8 *buf, u16 len)
{
	u8 ret = HAL_OK;
	usbd_vendor_dev_t *cdev = &usbd_vendor_dev;

	if (len > USBD_VENDOR_INTR_IN_BUF_SIZE) {
		len = USBD_VENDOR_INTR_IN_BUF_SIZE;
	}

	if (cdev->intr_in_state == 0U) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "INTR TX len=%d data=%d\n", len, buf[0]);
		cdev->intr_in_state = 1U;
		rtw_memcpy((void *)cdev->intr_in_buf, (void *)buf, len);
		usbd_ep_transmit(cdev->dev, USBD_VENDOR_INTR_IN_EP, cdev->intr_in_buf, len);
	} else {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "INTR TX len=%d data=%d: BUSY\n", len, buf[0]);
		ret = HAL_BUSY;
	}

	return ret;
}

u8 usbd_vendor_receive_intr_data(void)
{
	usbd_vendor_dev_t *cdev = &usbd_vendor_dev;

	usbd_ep_receive(cdev->dev, USBD_VENDOR_INTR_OUT_EP, cdev->intr_out_buf, USBD_VENDOR_INTR_OUT_BUF_SIZE);

	return HAL_OK;
}

u8 usbd_vendor_transmit_isoc_data(u8 *buf, u16 len)
{
	usbd_vendor_dev_t *cdev = &usbd_vendor_dev;

	if (len > USBD_VENDOR_ISOC_IN_BUF_SIZE) {
		len = USBD_VENDOR_ISOC_IN_BUF_SIZE;
	}

	DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "ISOC TX len=%d data=%d\n", len, buf[0]);
	rtw_memcpy(cdev->isoc_in_buf, buf, len);
	usbd_ep_transmit(cdev->dev, USBD_VENDOR_ISOC_IN_EP, cdev->isoc_in_buf, len);

	return HAL_OK;
}

u8 usbd_vendor_receive_isoc_data(void)
{
	usbd_vendor_dev_t *cdev = &usbd_vendor_dev;

	usbd_ep_receive(cdev->dev, USBD_VENDOR_ISOC_OUT_EP, cdev->isoc_out_buf, USBD_VENDOR_ISOC_OUT_BUF_SIZE);

	return HAL_OK;
}

