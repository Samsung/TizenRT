/**
  ******************************************************************************
  * @file    usbd_inic.c
  * @author  Realsil WLAN5 Team
  * @brief   This file provides the functionalities of the USB INIC Class
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

#include "usbd_inic.h"
#include "usbd_inic_otp.h"
#include "usbd.h"

/* Private defines -----------------------------------------------------------*/

#define USBD_INIC_MFG_STRING	"Realtek"
#define USBD_INIC_PROD_STRING	"802.11n WLAN Adapter"
#define USBD_INIC_SN_STRING		"1234567890"
#define USBD_INIC_BT_STRING		"Bluetooth Radio"

/* Private types -------------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

static u8 usbd_inic_set_config(usb_dev_t *dev, u8 config);
static u8 usbd_inic_clear_config(usb_dev_t *dev, u8 config);
static u8 usbd_inic_setup(usb_dev_t *dev, usb_setup_req_t *req);
static u8 *usbd_inic_get_descriptor(usb_dev_t *dev, usb_setup_req_t *req, usb_speed_type_t speed, u16 *len);
static u8 usbd_inic_handle_ep0_data_out(usb_dev_t *dev);
static u8 usbd_inic_handle_ep_data_in(usb_dev_t *dev, u8 ep_num, u8 status);
static u8 usbd_inic_handle_ep_data_out(usb_dev_t *dev, u8 ep_num, u16 len);

/* Private variables ---------------------------------------------------------*/

/* USB Standard Device Descriptor */
static u8 usbd_inic_dev_desc[USB_LEN_DEV_DESC] USB_DMA_ALIGNED = {
	USB_LEN_DEV_DESC,             // bLength
	USB_DESC_TYPE_DEVICE,         // bDescriptorType
	0x00,                         // bcdUSB
	0x02,
	0xEF,                         // bDeviceClass
	0x02,                         // bDeviceSubClass
	0x01,                         // bDeviceProtocol
	USB_MAX_EP0_SIZE,             // bMaxPacketSize
	USB_LOW_BYTE(USBD_INIC_VID),  // idVendor
	USB_HIGH_BYTE(USBD_INIC_VID), // idVendor
	USB_LOW_BYTE(USBD_INIC_PID),  // idProduct
	USB_HIGH_BYTE(USBD_INIC_PID), // idProduct
	0x00,                         // bcdDevice
	0x00,
	USBD_IDX_MFC_STR,             // Index of manufacturer string
	USBD_IDX_PRODUCT_STR,         // Index of product string
	USBD_IDX_SERIAL_STR,          // Index of serial number string
	1                             // bNumConfigurations
}; // usbd_inic_dev_desc

static u8 usbd_inic_single_wifi_mode_dev_desc[USB_LEN_DEV_DESC] USB_DMA_ALIGNED = {
	USB_LEN_DEV_DESC,             // bLength
	USB_DESC_TYPE_DEVICE,         // bDescriptorType
	0x00,                         // bcdUSB
	0x02,
	0x00,                         // bDeviceClass
	0x00,                         // bDeviceSubClass
	0x00,                         // bDeviceProtocol
	USB_MAX_EP0_SIZE,             // bMaxPacketSize
	USB_LOW_BYTE(USBD_INIC_VID),  // idVendor
	USB_HIGH_BYTE(USBD_INIC_VID), // idVendor
	USB_LOW_BYTE(USBD_INIC_PID),  // idProduct
	USB_HIGH_BYTE(USBD_INIC_PID), // idProduct
	0x00,                         // bcdDevice
	0x00,
	USBD_IDX_MFC_STR,             // Index of manufacturer string
	USBD_IDX_PRODUCT_STR,         // Index of product string
	USBD_IDX_SERIAL_STR,          // Index of serial number string
	1                             // bNumConfigurations
}; // usbd_inic_single_wifi_mode_dev_desc

/* USB Standard Device Descriptor */
static u8 usbd_inic_lang_id_desc[USB_LEN_LANGID_STR_DESC] USB_DMA_ALIGNED = {
	USB_LEN_LANGID_STR_DESC,
	USB_DESC_TYPE_STRING,
	USB_LOW_BYTE(USBD_INIC_LANGID_STRING),
	USB_HIGH_BYTE(USBD_INIC_LANGID_STRING),
};

/* USB Standard Device Descriptor */
static u8 usbd_inic_dev_qualifier_desc[USB_LEN_DEV_QUALIFIER_DESC] USB_DMA_ALIGNED = {
	USB_LEN_DEV_QUALIFIER_DESC,
	USB_DESC_TYPE_DEVICE_QUALIFIER,
	0x00,
	0x02,
	0x00,
	0x00,
	0x00,
	USB_MAX_EP0_SIZE,
	0x01,
	0x00,
};

/* USB Configuration Descriptor */
static u8 usbd_inic_config_desc[USBD_INIC_CONFIG_DESC_SIZE] USB_DMA_ALIGNED = {
	/* Configuration Descriptor */
	USB_LEN_CFG_DESC,						// bLength: Configuration Descriptor size
	USB_DESC_TYPE_CONFIGURATION,			// bDescriptorType: Configuration
	USBD_INIC_CONFIG_DESC_SIZE,				// wTotalLength: number of returned bytes
	0x00,
	0x03,									// bNumInterfaces: 3 interface
	0x01,									// bConfigurationValue
	0x00,									// iConfiguration
	0x80,									// bmAttributes: decided by eFuse
	0xFA,									// MaxPower 500 mA

	/*---------------------------------------------------------------------------*/

	/* IAD Descriptor */
	USB_LEN_IAD_DESC,						// bLength: IAD Descriptor size
	USB_DESC_TYPE_IAD,						// bDescriptorType: IAD
	0x00,									// bFirstInterface
	0x02,									// bInterfaceCount
	0xE0,									// bFunctionClass: Wireless Controller
	0x01,									// bFunctionSubClass
	0x01,									// bFunctionProtocol
	USBD_INIC_IDX_INTERFACE_STR,			// iFunction: USBD_INIC_BT_STRING

	/*---------------------------------------------------------------------------*/

	/* Interface Descriptor */
	USB_LEN_IF_DESC,						// bLength: Interface Descriptor size
	USB_DESC_TYPE_INTERFACE,				// bDescriptorType: Interface
	0x00,									// bInterfaceNumber: Number of Interface
	0x00,									// bAlternateSetting: Alternate setting
	0x03,									// bNumEndpoints: 3 endpoints
	0xE0,									// bInterfaceClass: Wireless Controller
	0x01,									// bInterfaceSubClass
	0x01,									// bInterfaceProtocol: Bluetooth Programming Interface
	USBD_INIC_IDX_INTERFACE_STR,			// iInterface: USBD_INIC_BT_STRING

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT,					// bDescriptorType: Endpoint
	0x81,									// bEndpointAddress: EP1 IN
	USB_CH_EP_TYPE_INTR,						// bmAttributes: INTR
	0x10,									// wMaxPacketSize: 16 bytes
	0x00,
	0x04,									// bInterval

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT,					// bDescriptorType: Endpoint
	0x02,									// bEndpointAddress: EP2 OUT
	USB_CH_EP_TYPE_BULK,					// bmAttributes: BULK
	USB_LOW_BYTE(USBD_INIC_HS_BULK_MPS),	// wMaxPacketSize: 512 bytes
	USB_HIGH_BYTE(USBD_INIC_HS_BULK_MPS),
	0x00,									// bInterval

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT,					// bDescriptorType: Endpoint
	0x82,									// bEndpointAddress: EP2 IN
	USB_CH_EP_TYPE_BULK,					// bmAttributes: BULK
	USB_LOW_BYTE(USBD_INIC_HS_BULK_MPS),	// wMaxPacketSize: 512 bytes
	USB_HIGH_BYTE(USBD_INIC_HS_BULK_MPS),
	0x00,									// bInterval

	/*---------------------------------------------------------------------------*/

	/* Interface Descriptor */
	USB_LEN_IF_DESC,						// bLength: Interface Descriptor size
	USB_DESC_TYPE_INTERFACE,				// bDescriptorType: Interface
	0x01,									// bInterfaceNumber: Number of Interface
	0x00,									// bAlternateSetting: Alternate setting
	0x02,									// bNumEndpoints: 2 endpoints
	0xE0,									// bInterfaceClass: Wireless Controller
	0x01,									// bInterfaceSubClass
	0x01,									// bInterfaceProtocol: Bluetooth Programming Interface
	USBD_INIC_IDX_INTERFACE_STR,			// iInterface: USBD_INIC_BT_STRING

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x03,									// bEndpointAddress: EP3 OUT
	USB_CH_EP_TYPE_ISOC,					// bmAttributes: ISOC
	0x00,									// wMaxPacketSize: 0 bytes
	0x00,
	0x04,									// bInterval

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x83,									// bEndpointAddress: EP3 IN
	USB_CH_EP_TYPE_ISOC,					// bmAttributes: ISOC
	0x00,									// wMaxPacketSize: 0 bytes
	0x00,
	0x04,									// bInterval

	/* Interface Descriptor */
	USB_LEN_IF_DESC,						// bLength: Interface Descriptor size
	USB_DESC_TYPE_INTERFACE,				// bDescriptorType: Interface
	0x01,									// bInterfaceNumber: Number of Interface
	0x01,									// bAlternateSetting: Alternate setting
	0x02,									// bNumEndpoints: 2 endpoints
	0xE0,									// bInterfaceClass: Wireless Controller
	0x01,									// bInterfaceSubClass
	0x01,									// bInterfaceProtocol: Bluetooth Programming Interface
	USBD_INIC_IDX_INTERFACE_STR,			// iInterface: USBD_INIC_BT_STRING

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x03,									// bEndpointAddress: EP3 OUT
	USB_CH_EP_TYPE_ISOC,					// bmAttributes: ISOC
	0x09,									// wMaxPacketSize: 9 bytes
	0x00,
	0x04,									// bInterval

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x83,									// bEndpointAddress: EP3 IN
	USB_CH_EP_TYPE_ISOC,					// bmAttributes: ISOC
	0x09,									// wMaxPacketSize: 9 bytes
	0x00,
	0x04,									// bInterval

	/* Interface Descriptor */
	USB_LEN_IF_DESC,						// bLength: Interface Descriptor size
	USB_DESC_TYPE_INTERFACE,				// bDescriptorType: Interface
	0x01,									// bInterfaceNumber: Number of Interface
	0x02,									// bAlternateSetting: Alternate setting
	0x02,									// bNumEndpoints: 2 endpoints
	0xE0,									// bInterfaceClass: Wireless Controller
	0x01,									// bInterfaceSubClass
	0x01,									// bInterfaceProtocol: Bluetooth Programming Interface
	USBD_INIC_IDX_INTERFACE_STR,			// iInterface: USBD_INIC_BT_STRING

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x03,									// bEndpointAddress: EP3 OUT
	USB_CH_EP_TYPE_ISOC,					// bmAttributes: ISOC
	0x11,									// wMaxPacketSize: 17 bytes
	0x00,
	0x04,									// bInterval

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x83,									// bEndpointAddress: EP3 IN
	USB_CH_EP_TYPE_ISOC,					// bmAttributes: ISOC
	0x11,									// wMaxPacketSize: 17 bytes
	0x00,
	0x04,									// bInterval

	/* Interface Descriptor */
	USB_LEN_IF_DESC,						// bLength: Interface Descriptor size
	USB_DESC_TYPE_INTERFACE,				// bDescriptorType: Interface
	0x01,									// bInterfaceNumber: Number of Interface
	0x03,									// bAlternateSetting: Alternate setting
	0x02,									// bNumEndpoints: 2 endpoints
	0xE0,									// bInterfaceClass: Wireless Controller
	0x01,									// bInterfaceSubClass
	0x01,									// bInterfaceProtocol: Bluetooth Programming Interface
	USBD_INIC_IDX_INTERFACE_STR,			// iInterface: USBD_INIC_BT_STRING

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x03,									// bEndpointAddress: EP3 OUT
	USB_CH_EP_TYPE_ISOC,					// bmAttributes: ISOC
	0x19,									// wMaxPacketSize: 25 bytes
	0x00,
	0x04,									// bInterval

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x83,									// bEndpointAddress: EP3 IN
	USB_CH_EP_TYPE_ISOC,					// bmAttributes: ISOC
	0x19,									// wMaxPacketSize: 25 bytes
	0x00,
	0x04,									// bInterval

	/* Interface Descriptor */
	USB_LEN_IF_DESC,						// bLength: Interface Descriptor size
	USB_DESC_TYPE_INTERFACE,				// bDescriptorType: Interface
	0x01,									// bInterfaceNumber: Number of Interface
	0x04,									// bAlternateSetting: Alternate setting
	0x02,									// bNumEndpoints: 2 endpoints
	0xE0,									// bInterfaceClass: Wireless Controller
	0x01,									// bInterfaceSubClass
	0x01,									// bInterfaceProtocol: Bluetooth Programming Interface
	USBD_INIC_IDX_INTERFACE_STR,			// iInterface: USBD_INIC_BT_STRING

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x03,									// bEndpointAddress: EP3 OUT
	USB_CH_EP_TYPE_ISOC,					// bmAttributes: ISOC
	0x21,									// wMaxPacketSize: 33 bytes
	0x00,
	0x04,									// bInterval

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x83,									// bEndpointAddress: EP3 IN
	USB_CH_EP_TYPE_ISOC,					// bmAttributes: ISOC
	0x21,									// wMaxPacketSize: 33 bytes
	0x00,
	0x04,									// bInterval

	/* Interface Descriptor */
	USB_LEN_IF_DESC,						// bLength: Interface Descriptor size
	USB_DESC_TYPE_INTERFACE,				// bDescriptorType: Interface
	0x01,									// bInterfaceNumber: Number of Interface
	0x05,									// bAlternateSetting: Alternate setting
	0x02,									// bNumEndpoints: 2 endpoints
	0xE0,									// bInterfaceClass: Wireless Controller
	0x01,									// bInterfaceSubClass
	0x01,									// bInterfaceProtocol: Bluetooth Programming Interface
	USBD_INIC_IDX_INTERFACE_STR,			// iInterface: USBD_INIC_BT_STRING

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x03,									// bEndpointAddress: EP3 OUT
	USB_CH_EP_TYPE_ISOC,					// bmAttributes: ISOC
	0x31,									// wMaxPacketSize: 49 bytes
	0x00,
	0x04,									// bInterval

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x83,									// bEndpointAddress: EP3 IN
	USB_CH_EP_TYPE_ISOC,					// bmAttributes: ISOC
	0x31,									// wMaxPacketSize: 49 bytes
	0x00,
	0x04,									// bInterval

	/*---------------------------------------------------------------------------*/

	/* Interface Descriptor */
	USB_LEN_IF_DESC,						// bLength: Interface Descriptor size
	USB_DESC_TYPE_INTERFACE,				// bDescriptorType: Interface
	0x02,									// bInterfaceNumber: Number of Interface
	0x00,									// bAlternateSetting: Alternate setting
	0x04,									// bNumEndpoints: 4 endpoints
	0xFF,									// bInterfaceClass: Vendor Specific
	0xFF,									// bInterfaceSubClass
	0xFF,									// bInterfaceProtocol
	USBD_IDX_PRODUCT_STR,					// iInterface: USBD_INIC_PROD_STRING

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x84,									// bEndpointAddress: EP4 IN
	USB_CH_EP_TYPE_BULK,					// bmAttributes: BULK
	USB_LOW_BYTE(USBD_INIC_HS_BULK_MPS),	// wMaxPacketSize: 512 bytes
	USB_HIGH_BYTE(USBD_INIC_HS_BULK_MPS),
	0x00,									// bInterval

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x05,									// bEndpointAddress: EP5 OUT
	USB_CH_EP_TYPE_BULK,					// bmAttributes: BULK
	USB_LOW_BYTE(USBD_INIC_HS_BULK_MPS),	// wMaxPacketSize: 512 bytes
	USB_HIGH_BYTE(USBD_INIC_HS_BULK_MPS),
	0x00,									// bInterval

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x06,									// bEndpointAddress: EP6 OUT
	USB_CH_EP_TYPE_BULK,					// bmAttributes: BULK
	USB_LOW_BYTE(USBD_INIC_HS_BULK_MPS),	// wMaxPacketSize: 512 bytes
	USB_HIGH_BYTE(USBD_INIC_HS_BULK_MPS),
	0x00,									// bInterval

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x06,									// bEndpointAddress: EP7 OUT
	USB_CH_EP_TYPE_BULK,					// bmAttributes: BULK
	USB_LOW_BYTE(USBD_INIC_HS_BULK_MPS),	// wMaxPacketSize: 512 bytes
	USB_HIGH_BYTE(USBD_INIC_HS_BULK_MPS),
	0x00,									// bInterval
};

/* USB Other Speed Configuration Descriptor */
static u8 usbd_inic_other_speed_config_desc[USBD_INIC_CONFIG_DESC_SIZE] USB_DMA_ALIGNED = {
	/* Configuration Descriptor */
	USB_LEN_CFG_DESC,						// bLength: Configuration Descriptor size
	USB_DESC_TYPE_CONFIGURATION,			// bDescriptorType: Configuration
	USBD_INIC_CONFIG_DESC_SIZE,				// wTotalLength: number of returned bytes
	0x00,
	0x03,									// bNumInterfaces: 3 interface
	0x01,									// bConfigurationValue
	0x00,									// iConfiguration
	0x80,									// bmAttributes: decided by eFuse
	0xFA,									// MaxPower 500 mA

	/*---------------------------------------------------------------------------*/

	/* IAD Descriptor */
	USB_LEN_IAD_DESC,						// bLength: IAD Descriptor size
	USB_DESC_TYPE_IAD,						// bDescriptorType: IAD
	0x00,									// bFirstInterface
	0x02,									// bInterfaceCount
	0xE0,									// bFunctionClass: Wireless Controller
	0x01,									// bFunctionSubClass
	0x01,									// bFunctionProtocol
	USBD_INIC_IDX_INTERFACE_STR,			// iFunction: USBD_INIC_BT_STRING

	/*---------------------------------------------------------------------------*/

	/* Interface Descriptor */
	USB_LEN_IF_DESC,						// bLength: Interface Descriptor size
	USB_DESC_TYPE_INTERFACE,				// bDescriptorType: Interface
	0x00,									// bInterfaceNumber: Number of Interface
	0x00,									// bAlternateSetting: Alternate setting
	0x03,									// bNumEndpoints: 3 endpoints
	0xE0,									// bInterfaceClass: Wireless Controller
	0x01,									// bInterfaceSubClass
	0x01,									// bInterfaceProtocol: Bluetooth Programming Interface
	USBD_INIC_IDX_INTERFACE_STR,			// iInterface: USBD_INIC_BT_STRING

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT,					// bDescriptorType: Endpoint
	0x81,									// bEndpointAddress: EP1 IN
	USB_CH_EP_TYPE_INTR,					// bmAttributes: INTR
	0x10,									// wMaxPacketSize: 16 bytes
	0x00,
	0x04,									// bInterval

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT,					// bDescriptorType: Endpoint
	0x02,									// bEndpointAddress: EP2 OUT
	USB_CH_EP_TYPE_BULK,					// bmAttributes: BULK
	USB_LOW_BYTE(USBD_INIC_FS_BULK_MPS),	// wMaxPacketSize: 64 bytes
	USB_HIGH_BYTE(USBD_INIC_FS_BULK_MPS),
	0x00,									// bInterval

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT,					// bDescriptorType: Endpoint
	0x82,									// bEndpointAddress: EP2 IN
	USB_CH_EP_TYPE_BULK,					// bmAttributes: BULK
	USB_LOW_BYTE(USBD_INIC_FS_BULK_MPS),	// wMaxPacketSize: 64 bytes
	USB_HIGH_BYTE(USBD_INIC_FS_BULK_MPS),
	0x00,									// bInterval

	/*---------------------------------------------------------------------------*/

	/* Interface Descriptor */
	USB_LEN_IF_DESC,						// bLength: Interface Descriptor size
	USB_DESC_TYPE_INTERFACE,				// bDescriptorType: Interface
	0x01,									// bInterfaceNumber: Number of Interface
	0x00,									// bAlternateSetting: Alternate setting
	0x02,									// bNumEndpoints: 2 endpoints
	0xE0,									// bInterfaceClass: Wireless Controller
	0x01,									// bInterfaceSubClass
	0x01,									// bInterfaceProtocol: Bluetooth Programming Interface
	USBD_INIC_IDX_INTERFACE_STR,			// iInterface: USBD_INIC_BT_STRING

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x03,									// bEndpointAddress: EP3 OUT
	USB_CH_EP_TYPE_ISOC,					// bmAttributes: ISOC
	0x00,									// wMaxPacketSize: 0 bytes
	0x00,
	0x04,									// bInterval

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x83,									// bEndpointAddress: EP3 IN
	USB_CH_EP_TYPE_ISOC,					// bmAttributes: ISOC
	0x00,									// wMaxPacketSize: 0 bytes
	0x00,
	0x04,									// bInterval

	/* Interface Descriptor */
	USB_LEN_IF_DESC,						// bLength: Interface Descriptor size
	USB_DESC_TYPE_INTERFACE,				// bDescriptorType: Interface
	0x01,									// bInterfaceNumber: Number of Interface
	0x01,									// bAlternateSetting: Alternate setting
	0x02,									// bNumEndpoints: 2 endpoints
	0xE0,									// bInterfaceClass: Wireless Controller
	0x01,									// bInterfaceSubClass
	0x01,									// bInterfaceProtocol: Bluetooth Programming Interface
	USBD_INIC_IDX_INTERFACE_STR,			// iInterface: USBD_INIC_BT_STRING

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x03,									// bEndpointAddress: EP3 OUT
	USB_CH_EP_TYPE_ISOC,					// bmAttributes: ISOC
	0x09,									// wMaxPacketSize: 9 bytes
	0x00,
	0x04,									// bInterval

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x83,									// bEndpointAddress: EP3 IN
	USB_CH_EP_TYPE_ISOC,					// bmAttributes: ISOC
	0x09,									// wMaxPacketSize: 9 bytes
	0x00,
	0x04,									// bInterval

	/* Interface Descriptor */
	USB_LEN_IF_DESC,						// bLength: Interface Descriptor size
	USB_DESC_TYPE_INTERFACE,				// bDescriptorType: Interface
	0x01,									// bInterfaceNumber: Number of Interface
	0x02,									// bAlternateSetting: Alternate setting
	0x02,									// bNumEndpoints: 2 endpoints
	0xE0,									// bInterfaceClass: Wireless Controller
	0x01,									// bInterfaceSubClass
	0x01,									// bInterfaceProtocol: Bluetooth Programming Interface
	USBD_INIC_IDX_INTERFACE_STR,			// iInterface: USBD_INIC_BT_STRING

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x03,									// bEndpointAddress: EP3 OUT
	USB_CH_EP_TYPE_ISOC,					// bmAttributes: ISOC
	0x11,									// wMaxPacketSize: 17 bytes
	0x00,
	0x04,									// bInterval

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x83,									// bEndpointAddress: EP3 IN
	USB_CH_EP_TYPE_ISOC,					// bmAttributes: ISOC
	0x11,									// wMaxPacketSize: 17 bytes
	0x00,
	0x04,									// bInterval

	/* Interface Descriptor */
	USB_LEN_IF_DESC,						// bLength: Interface Descriptor size
	USB_DESC_TYPE_INTERFACE,				// bDescriptorType: Interface
	0x01,									// bInterfaceNumber: Number of Interface
	0x03,									// bAlternateSetting: Alternate setting
	0x02,									// bNumEndpoints: 2 endpoints
	0xE0,									// bInterfaceClass: Wireless Controller
	0x01,									// bInterfaceSubClass
	0x01,									// bInterfaceProtocol: Bluetooth Programming Interface
	USBD_INIC_IDX_INTERFACE_STR,			// iInterface: USBD_INIC_BT_STRING

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x03,									// bEndpointAddress: EP3 OUT
	USB_CH_EP_TYPE_ISOC,					// bmAttributes: ISOC
	0x19,									// wMaxPacketSize: 25 bytes
	0x00,
	0x04,									// bInterval

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x83,									// bEndpointAddress: EP3 IN
	USB_CH_EP_TYPE_ISOC,					// bmAttributes: ISOC
	0x19,									// wMaxPacketSize: 25 bytes
	0x00,
	0x04,									// bInterval

	/* Interface Descriptor */
	USB_LEN_IF_DESC,						// bLength: Interface Descriptor size
	USB_DESC_TYPE_INTERFACE,				// bDescriptorType: Interface
	0x01,									// bInterfaceNumber: Number of Interface
	0x04,									// bAlternateSetting: Alternate setting
	0x02,									// bNumEndpoints: 2 endpoints
	0xE0,									// bInterfaceClass: Wireless Controller
	0x01,									// bInterfaceSubClass
	0x01,									// bInterfaceProtocol: Bluetooth Programming Interface
	USBD_INIC_IDX_INTERFACE_STR,			// iInterface: USBD_INIC_BT_STRING

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x03,									// bEndpointAddress: EP3 OUT
	USB_CH_EP_TYPE_ISOC,					// bmAttributes: ISOC
	0x21,									// wMaxPacketSize: 33 bytes
	0x00,
	0x04,									// bInterval

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x83,									// bEndpointAddress: EP3 IN
	USB_CH_EP_TYPE_ISOC,					// bmAttributes: ISOC
	0x21,									// wMaxPacketSize: 33 bytes
	0x00,
	0x04,									// bInterval

	/* Interface Descriptor */
	USB_LEN_IF_DESC,						// bLength: Interface Descriptor size
	USB_DESC_TYPE_INTERFACE,				// bDescriptorType: Interface
	0x01,									// bInterfaceNumber: Number of Interface
	0x05,									// bAlternateSetting: Alternate setting
	0x02,									// bNumEndpoints: 2 endpoints
	0xE0,									// bInterfaceClass: Wireless Controller
	0x01,									// bInterfaceSubClass
	0x01,									// bInterfaceProtocol: Bluetooth Programming Interface
	USBD_INIC_IDX_INTERFACE_STR,			// iInterface: USBD_INIC_BT_STRING

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x03,									// bEndpointAddress: EP3 OUT
	USB_CH_EP_TYPE_ISOC,					// bmAttributes: ISOC
	0x31,									// wMaxPacketSize: 49 bytes
	0x00,
	0x04,									// bInterval

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x83,									// bEndpointAddress: EP3 IN
	USB_CH_EP_TYPE_ISOC,					// bmAttributes: ISOC
	0x31,									// wMaxPacketSize: 49 bytes
	0x00,
	0x04,									// bInterval

	/*---------------------------------------------------------------------------*/

	/* Interface Descriptor */
	USB_LEN_IF_DESC,						// bLength: Interface Descriptor size
	USB_DESC_TYPE_INTERFACE,				// bDescriptorType: Interface
	0x02,									// bInterfaceNumber: Number of Interface
	0x00,									// bAlternateSetting: Alternate setting
	0x04,									// bNumEndpoints: 4 endpoints
	0xFF,									// bInterfaceClass: Vendor Specific
	0xFF,									// bInterfaceSubClass
	0xFF,									// bInterfaceProtocol
	USBD_IDX_PRODUCT_STR,					// iInterface: USBD_INIC_PROD_STRING

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x84,									// bEndpointAddress: EP4 IN
	USB_CH_EP_TYPE_BULK,					// bmAttributes: BULK
	USB_LOW_BYTE(USBD_INIC_FS_BULK_MPS),	// wMaxPacketSize: 64 bytes
	USB_HIGH_BYTE(USBD_INIC_FS_BULK_MPS),
	0x00,									// bInterval

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x05,									// bEndpointAddress: EP5 OUT
	USB_CH_EP_TYPE_BULK,					// bmAttributes: BULK
	USB_LOW_BYTE(USBD_INIC_FS_BULK_MPS),	// wMaxPacketSize: 64 bytes
	USB_HIGH_BYTE(USBD_INIC_FS_BULK_MPS),
	0x00,									// bInterval

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x06,									// bEndpointAddress: EP6 OUT
	USB_CH_EP_TYPE_BULK,					// bmAttributes: BULK
	USB_LOW_BYTE(USBD_INIC_FS_BULK_MPS),	// wMaxPacketSize: 64 bytes
	USB_HIGH_BYTE(USBD_INIC_FS_BULK_MPS),
	0x00,									// bInterval

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x06,									// bEndpointAddress: EP7 OUT
	USB_CH_EP_TYPE_BULK,					// bmAttributes: BULK
	USB_LOW_BYTE(USBD_INIC_FS_BULK_MPS),	// wMaxPacketSize: 64 bytes
	USB_HIGH_BYTE(USBD_INIC_FS_BULK_MPS),
	0x00,									// bInterval
};

/* USB Configuration Descriptor for WiFi-only mode */
static u8 usbd_inic_single_wifi_mode_config_desc[USBD_INIC_WIFI_ONLY_MODE_CONFIG_DESC_SIZE] USB_DMA_ALIGNED = {
	/* Configuration Descriptor */
	USB_LEN_CFG_DESC,						// bLength: Configuration Descriptor size
	USB_DESC_TYPE_CONFIGURATION,			// bDescriptorType: Configuration
	USBD_INIC_WIFI_ONLY_MODE_CONFIG_DESC_SIZE,	// wTotalLength: number of returned bytes
	0x00,
	0x01,									// bNumInterfaces: 1 interface
	0x01,									// bConfigurationValue
	0x00,									// iConfiguration
	0x80,									// bmAttributes: decided by eFuse
	0xFA,									// MaxPower 500 mA

	/*---------------------------------------------------------------------------*/

	/* Interface Descriptor */
	USB_LEN_IF_DESC,						// bLength: Interface Descriptor size
	USB_DESC_TYPE_INTERFACE,				// bDescriptorType: Interface
	0x00,									// bInterfaceNumber: Number of Interface
	0x00,									// bAlternateSetting: Alternate setting
	0x04,									// bNumEndpoints: 4 endpoints
	0xFF,									// bInterfaceClass: Vendor Specific
	0xFF,									// bInterfaceSubClass
	0xFF,									// bInterfaceProtocol
	USBD_IDX_PRODUCT_STR,					// iInterface: USBD_INIC_PROD_STRING

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x84,									// bEndpointAddress: EP4 IN
	USB_CH_EP_TYPE_BULK,					// bmAttributes: BULK
	USB_LOW_BYTE(USBD_INIC_HS_BULK_MPS),	// wMaxPacketSize: 512 bytes
	USB_HIGH_BYTE(USBD_INIC_HS_BULK_MPS),
	0x00,									// bInterval

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x05,									// bEndpointAddress: EP5 OUT
	USB_CH_EP_TYPE_BULK,					// bmAttributes: BULK
	USB_LOW_BYTE(USBD_INIC_HS_BULK_MPS),	// wMaxPacketSize: 512 bytes
	USB_HIGH_BYTE(USBD_INIC_HS_BULK_MPS),
	0x00,									// bInterval

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x06,									// bEndpointAddress: EP6 OUT
	USB_CH_EP_TYPE_BULK,					// bmAttributes: BULK
	USB_LOW_BYTE(USBD_INIC_HS_BULK_MPS),	// wMaxPacketSize: 512 bytes
	USB_HIGH_BYTE(USBD_INIC_HS_BULK_MPS),
	0x00,									// bInterval

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x06,									// bEndpointAddress: EP7 OUT
	USB_CH_EP_TYPE_BULK,					// bmAttributes: BULK
	USB_LOW_BYTE(USBD_INIC_HS_BULK_MPS),	// wMaxPacketSize: 512 bytes
	USB_HIGH_BYTE(USBD_INIC_HS_BULK_MPS),
	0x00,									// bInterval
};

/* USB Other Speed Configuration Descriptor for WiFi-only mode */
static u8 usbd_inic_wifi_only_mode_other_speed_config_desc[USBD_INIC_WIFI_ONLY_MODE_CONFIG_DESC_SIZE] USB_DMA_ALIGNED = {
	/* Configuration Descriptor */
	USB_LEN_CFG_DESC,						// bLength: Configuration Descriptor size
	USB_DESC_TYPE_CONFIGURATION,			// bDescriptorType: Configuration
	USBD_INIC_WIFI_ONLY_MODE_CONFIG_DESC_SIZE,	// wTotalLength: number of returned bytes
	0x00,
	0x01,									// bNumInterfaces: 1 interface
	0x01,									// bConfigurationValue
	0x00,									// iConfiguration
	0x80,									// bmAttributes: decided by eFuse
	0xFA,									// MaxPower 500 mA

	/*---------------------------------------------------------------------------*/

	/* Interface Descriptor */
	USB_LEN_IF_DESC,						// bLength: Interface Descriptor size
	USB_DESC_TYPE_INTERFACE,				// bDescriptorType: Interface
	0x00,									// bInterfaceNumber: Number of Interface
	0x00,									// bAlternateSetting: Alternate setting
	0x04,									// bNumEndpoints: 4 endpoints
	0xFF,									// bInterfaceClass: Vendor Specific
	0xFF,									// bInterfaceSubClass
	0xFF,									// bInterfaceProtocol
	USBD_IDX_PRODUCT_STR,					// iInterface: USBD_INIC_PROD_STRING

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x84,									// bEndpointAddress: EP4 IN
	USB_CH_EP_TYPE_BULK,					// bmAttributes: BULK
	USB_LOW_BYTE(USBD_INIC_FS_BULK_MPS),	// wMaxPacketSize: 64 bytes
	USB_HIGH_BYTE(USBD_INIC_FS_BULK_MPS),
	0x00,									// bInterval

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x05,									// bEndpointAddress: EP5 OUT
	USB_CH_EP_TYPE_BULK,					// bmAttributes: BULK
	USB_LOW_BYTE(USBD_INIC_FS_BULK_MPS),	// wMaxPacketSize: 64 bytes
	USB_HIGH_BYTE(USBD_INIC_FS_BULK_MPS),
	0x00,									// bInterval

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x06,									// bEndpointAddress: EP6 OUT
	USB_CH_EP_TYPE_BULK,					// bmAttributes: BULK
	USB_LOW_BYTE(USBD_INIC_FS_BULK_MPS),	// wMaxPacketSize: 64 bytes
	USB_HIGH_BYTE(USBD_INIC_FS_BULK_MPS),
	0x00,									// bInterval

	/* Endpoint Descriptor */
	USB_LEN_EP_DESC,						// bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT, 				// bDescriptorType: Endpoint
	0x06,									// bEndpointAddress: EP7 OUT
	USB_CH_EP_TYPE_BULK,					// bmAttributes: BULK
	USB_LOW_BYTE(USBD_INIC_FS_BULK_MPS),	// wMaxPacketSize: 64 bytes
	USB_HIGH_BYTE(USBD_INIC_FS_BULK_MPS),
	0x00,									// bInterval
};

/* INIC Class Driver */
usbd_class_driver_t usbd_inic_driver = {
	.get_descriptor = usbd_inic_get_descriptor,
	.set_config = usbd_inic_set_config,
	.clear_config = usbd_inic_clear_config,
	.setup = usbd_inic_setup,
	.ep0_data_out = usbd_inic_handle_ep0_data_out,
	.ep_data_in = usbd_inic_handle_ep_data_in,
	.ep_data_out = usbd_inic_handle_ep_data_out,
};

/* INIC Device */
static usbd_inic_dev_t usbd_inic_dev;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Transmit BULK IN ZLP packet
  * @retval Status
  */
static u8 usbd_inic_transmit_zlp(u8 ep_num)
{
	usbd_inic_dev_t *idev = &usbd_inic_dev;

	DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "IN EP%d TX ZLP\n", ep_num);
	usbd_ep_transmit(idev->dev, ep_num | USB_D2H, NULL, 0);

	return HAL_OK;
}

/**
  * @brief  Set class configuration for WiFi interface
  * @param  dev: USB device instance
  * @param  config: USB configuration index
  * @retval Status
  */
static u8 usbd_inic_set_wifi_config(usb_dev_t *dev, u8 config)
{
	usbd_inic_dev_t *idev = &usbd_inic_dev;
	usbd_inic_ep_t *ep;

	UNUSED(config);

	/* Init BULK IN EP4 */
	ep = &idev->in_ep[USB_EP_ADDR(USBD_INIC_WIFI_EP4_BULK_IN)];
	ep->mps = (dev->dev_speed == USB_SPEED_HIGH) ? USBD_INIC_HS_BULK_MPS : USBD_INIC_FS_BULK_MPS;
	usbd_ep_init(dev, USBD_INIC_WIFI_EP4_BULK_IN, USB_CH_EP_TYPE_BULK, ep->mps);
	ep->state = USBD_INIC_EP_STATE_IDLE;

	/* Init BULK OUT EP5 */
	ep = &idev->out_ep[USB_EP_ADDR(USBD_INIC_WIFI_EP5_BULK_OUT)];
	ep->mps = (dev->dev_speed == USB_SPEED_HIGH) ? USBD_INIC_HS_BULK_MPS : USBD_INIC_FS_BULK_MPS;
	usbd_ep_init(dev, USBD_INIC_WIFI_EP5_BULK_OUT, USB_CH_EP_TYPE_BULK, ep->mps);
	ep->state = USBD_INIC_EP_STATE_IDLE;

	/* Init BULK OUT EP6 */
	ep = &idev->out_ep[USB_EP_ADDR(USBD_INIC_WIFI_EP6_BULK_OUT)];
	ep->mps = (dev->dev_speed == USB_SPEED_HIGH) ? USBD_INIC_HS_BULK_MPS : USBD_INIC_FS_BULK_MPS;
	usbd_ep_init(dev, USBD_INIC_WIFI_EP6_BULK_OUT, USB_CH_EP_TYPE_BULK, ep->mps);
	ep->state = USBD_INIC_EP_STATE_IDLE;

	/* Init BULK OUT EP7 */
	ep = &idev->out_ep[USB_EP_ADDR(USBD_INIC_WIFI_EP7_BULK_OUT)];
	ep->mps = (dev->dev_speed == USB_SPEED_HIGH) ? USBD_INIC_HS_BULK_MPS : USBD_INIC_FS_BULK_MPS;
	usbd_ep_init(dev, USBD_INIC_WIFI_EP7_BULK_OUT, USB_CH_EP_TYPE_BULK, ep->mps);
	ep->state = USBD_INIC_EP_STATE_IDLE;

	return HAL_OK;
}

/**
  * @brief  Set class configuration for BT interface
  * @param  dev: USB device instance
  * @param  config: USB configuration index
  * @retval Status
  */
static u8 usbd_inic_set_bt_config(usb_dev_t *dev, u8 config)
{
	usbd_inic_dev_t *idev = &usbd_inic_dev;
	usbd_inic_ep_t *ep;

	UNUSED(config);

	/* Init INTR IN EP1 */
	ep = &idev->in_ep[USB_EP_ADDR(USBD_INIT_BT_EP1_INTR_IN)];
	ep->mps = (dev->dev_speed == USB_SPEED_HIGH) ? USBD_INIC_HS_INTR_MPS : USBD_INIC_FS_INTR_MPS;
	usbd_ep_init(dev, USBD_INIT_BT_EP1_INTR_IN, USB_CH_EP_TYPE_INTR, ep->mps);
	ep->state = USBD_INIC_EP_STATE_IDLE;

	/* Init BULK IN EP2 */
	ep = &idev->in_ep[USB_EP_ADDR(USBD_INIC_BT_EP2_BULK_IN)];
	ep->mps = (dev->dev_speed == USB_SPEED_HIGH) ? USBD_INIC_HS_BULK_MPS : USBD_INIC_FS_BULK_MPS;
	usbd_ep_init(dev, USBD_INIC_BT_EP2_BULK_IN, USB_CH_EP_TYPE_BULK, ep->mps);
	ep->state = USBD_INIC_EP_STATE_IDLE;

	/* Init BULK OUT EP2 */
	ep = &idev->out_ep[USB_EP_ADDR(USBD_INIC_BT_EP2_BULK_OUT)];
	ep->mps = (dev->dev_speed == USB_SPEED_HIGH) ? USBD_INIC_HS_BULK_MPS : USBD_INIC_FS_BULK_MPS;
	usbd_ep_init(dev, USBD_INIC_BT_EP2_BULK_OUT, USB_CH_EP_TYPE_BULK, ep->mps);
	ep->state = USBD_INIC_EP_STATE_IDLE;

	return HAL_OK;
}

/**
  * @brief  Set class configuration
  * @param  dev: USB device instance
  * @param  config: USB configuration index
  * @retval Status
  */
static u8 usbd_inic_set_config(usb_dev_t *dev, u8 config)
{
	usbd_inic_dev_t *idev = &usbd_inic_dev;
	usbd_otp_t *otp = &idev->otp;

	idev->dev = dev;

	usbd_inic_set_wifi_config(dev, config);

	if (otp->bt_en) {
		usbd_inic_set_bt_config(dev, config);
	}

	if (idev->cb->set_config != NULL) {
		idev->cb->set_config();
	}

	return HAL_OK;
}

/**
  * @brief  Clear class configuration for WiFi interface
  * @param  dev: USB device instance
  * @param  config: USB configuration index
  * @retval Status
  */
static u8 usbd_inic_clear_wifi_config(usb_dev_t *dev, u8 config)
{
	usbd_inic_dev_t *idev = &usbd_inic_dev;
	usbd_inic_ep_t *ep;

	UNUSED(config);

	/* DeInit BULK IN EP4 */
	ep = &idev->in_ep[USB_EP_ADDR(USBD_INIC_WIFI_EP4_BULK_IN)];
	ep->state = USBD_INIC_EP_STATE_IDLE;
	usbd_ep_deinit(dev, USBD_INIC_WIFI_EP4_BULK_IN);

	/* DeInit BULK OUT EP5 */
	ep = &idev->out_ep[USB_EP_ADDR(USBD_INIC_WIFI_EP5_BULK_OUT)];
	ep->state = USBD_INIC_EP_STATE_IDLE;
	usbd_ep_deinit(dev, USBD_INIC_WIFI_EP5_BULK_OUT);

	/* DeInit BULK OUT EP6 */
	ep = &idev->out_ep[USB_EP_ADDR(USBD_INIC_WIFI_EP6_BULK_OUT)];
	ep->state = USBD_INIC_EP_STATE_IDLE;
	usbd_ep_deinit(dev, USBD_INIC_WIFI_EP6_BULK_OUT);

	/* DeInit BULK OUT EP7 */
	ep = &idev->out_ep[USB_EP_ADDR(USBD_INIC_WIFI_EP7_BULK_OUT)];
	ep->state = USBD_INIC_EP_STATE_IDLE;
	usbd_ep_deinit(dev, USBD_INIC_WIFI_EP7_BULK_OUT);

	return HAL_OK;
}

/**
  * @brief  Clear class configuration for BT interface
  * @param  dev: USB device instance
  * @param  config: USB configuration index
  * @retval Status
  */
static u8 usbd_inic_clear_bt_config(usb_dev_t *dev, u8 config)
{
	usbd_inic_dev_t *idev = &usbd_inic_dev;
	usbd_inic_ep_t *ep;

	UNUSED(config);

	/* DeInit INTR IN EP1 */
	ep = &idev->in_ep[USB_EP_ADDR(USBD_INIT_BT_EP1_INTR_IN)];
	ep->state = USBD_INIC_EP_STATE_IDLE;
	usbd_ep_deinit(dev, USBD_INIT_BT_EP1_INTR_IN);

	/* DeInit BULK IN EP2 */
	ep = &idev->in_ep[USB_EP_ADDR(USBD_INIC_BT_EP2_BULK_IN)];
	ep->state = USBD_INIC_EP_STATE_IDLE;
	usbd_ep_deinit(dev, USBD_INIC_BT_EP2_BULK_IN);

	/* DeInit BULK OUT EP2 */
	ep = &idev->out_ep[USB_EP_ADDR(USBD_INIC_BT_EP2_BULK_OUT)];
	ep->state = USBD_INIC_EP_STATE_IDLE;
	usbd_ep_deinit(dev, USBD_INIC_BT_EP2_BULK_OUT);

	return HAL_OK;
}

/**
  * @brief  Clear class configuration
  * @param  dev: USB device instance
  * @param  config: USB configuration index
  * @retval Status
  */
static u8 usbd_inic_clear_config(usb_dev_t *dev, u8 config)
{
	u8 ret = 0U;
	usbd_inic_dev_t *idev = &usbd_inic_dev;
	usbd_otp_t *otp = &idev->otp;

	UNUSED(config);

	usbd_inic_clear_wifi_config(dev, config);

	if (otp->bt_en) {
		usbd_inic_clear_bt_config(dev, config);
	}

	if (idev->cb->clear_config != NULL) {
		idev->cb->clear_config();
	}

	return ret;
}

/**
  * @brief  Handle INIC specific CTRL requests
  * @param  dev: USB device instance
  * @param  req: USB CTRL requests
  * @retval Status
  */
static u8 usbd_inic_setup(usb_dev_t *dev, usb_setup_req_t *req)
{
	u8 alt;
	u8 ret = HAL_OK;
	usbd_inic_dev_t *idev = &usbd_inic_dev;

	DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE,
			   "usbd_inic_setup bmRequestType=0x%02X bRequest=0x%02X wLength=0x%04X wValue=%x\n",
			   req->bmRequestType,
			   req->bRequest,
			   req->wLength,
			   req->wValue);

	switch (req->bmRequestType & USB_REQ_TYPE_MASK) {
	case USB_REQ_TYPE_STANDARD:
		switch (req->bRequest) {
		case USB_REQ_SET_INTERFACE:
			if (dev->dev_state == USBD_STATE_CONFIGURED) {
				alt = (u8)(req->wValue & 0xFF);
				switch (req->wIndex) {
				case USBD_INIC_ITF_BT:
					idev->bt_alt = alt;
					break;
				case USBD_INIC_ITF_BT_SCO:
					idev->bt_sco_alt = alt;
					break;
				case USBD_INIC_ITF_WIFI:
					idev->wifi_alt = alt;
					break;
				default:
					break;
				}
			} else {
				ret = HAL_ERR_HW;
			}
			break;
		case USB_REQ_GET_INTERFACE:
			if (dev->dev_state == USBD_STATE_CONFIGURED) {
				switch (req->wIndex) {
				case USBD_INIC_ITF_BT:
					alt = idev->bt_alt;
					break;
				case USBD_INIC_ITF_BT_SCO:
					alt = idev->bt_sco_alt;
					break;
				case USBD_INIC_ITF_WIFI:
					alt = idev->wifi_alt;
					break;
				default:
					ret = HAL_ERR_HW;
					break;
				}
				if (ret == HAL_OK) {
					idev->ctrl_buf[0] = alt;
					usbd_ep0_transmit(dev, idev->ctrl_buf, 1U);
				}
			} else {
				ret = HAL_ERR_HW;
			}
			break;
		case USB_REQ_GET_STATUS:
			if (dev->dev_state == USBD_STATE_CONFIGURED) {
				idev->ctrl_buf[0] = 0U;
				idev->ctrl_buf[1] = 0U;
				usbd_ep0_transmit(dev, idev->ctrl_buf, 2U);
			} else {
				ret = HAL_ERR_HW;
			}
			break;
		default:
			ret = HAL_ERR_HW;
			break;
		}
		break;
	case USB_REQ_TYPE_CLASS:
	case USB_REQ_TYPE_VENDOR:
		if ((req->bmRequestType & USB_REQ_DIR_MASK) == USB_D2H) {
			if (req->wLength) {
				// SETUP + DATA IN + STATUS
				idev->cb->setup(req->bRequest, idev->ctrl_buf, req->wLength, req->wValue);
				usbd_ep0_transmit(dev, idev->ctrl_buf, req->wLength);
			} else {
				// SETUP + ZLP IN + STATUS, invalid
			}
		} else {
			if (req->wLength) {
				// SETUP + DATA OUT + STATUS, the DATA OUT phase is processed in ep0_data_out callback
				idev->ctrl_req = req->bRequest;
				idev->ctrl_data_len = req->wLength;
				usbd_ep0_receive(dev, idev->ctrl_buf, req->wLength);
			} else {
				// SETUP + STATUS
				idev->cb->setup(req->bRequest, NULL, 0, req->wValue);
			}
		}
		break;
	default:
		ret = HAL_ERR_HW;
		break;
	}

	return ret;
}

/**
  * @brief  Handle EP0 Rx Ready event
  * @param  dev: USB device instance
  * @retval Status
  */
static u8 usbd_inic_handle_ep0_data_out(usb_dev_t *dev)
{
	u8 ret = HAL_OK;
	usbd_inic_dev_t *idev = &usbd_inic_dev;
	usbd_inic_cb_t *cb = idev->cb;

	UNUSED(dev);

	if (cb != NULL) {
		ret = cb->setup(idev->ctrl_req, idev->ctrl_buf, idev->ctrl_data_len, 0);
	}

	return ret;
}

/**
  * @brief  Data sent on non-control IN endpoint
  * @param  dev: USB device instance
  * @param  ep_num: endpoint number
  * @retval Status
  */
static u8 usbd_inic_handle_ep_data_in(usb_dev_t *dev, u8 ep_num, u8 status)
{
	usbd_inic_dev_t *idev = &usbd_inic_dev;
	usbd_inic_cb_t *cb = idev->cb;
	usbd_inic_ep_t *ep = &idev->in_ep[ep_num];

	UNUSED(dev);

	if (status == HAL_OK) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "IN EP%d TX done\n", ep_num);
		if (ep->zlp) {
			ep->zlp = 0;
			usbd_inic_transmit_zlp(ep_num);
		} else {
			ep->state = USBD_INIC_EP_STATE_IDLE;
		}
	} else {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "IN EP%d TX error: %d\n", ep_num, status);
		ep->state = USBD_INIC_EP_STATE_IDLE;
	}

	if (cb->transmitted != NULL) {
		cb->transmitted(ep, status);
	}

	return HAL_OK;
}

/**
  * @brief  Data received on non-control Out endpoint
  * @param  dev: USB device instance
  * @param  ep_num: endpoint number
  * @retval Status
  */
static u8 usbd_inic_handle_ep_data_out(usb_dev_t *dev, u8 ep_num, u16 len)
{
	usbd_inic_dev_t *idev = &usbd_inic_dev;
	usbd_inic_cb_t *cb = idev->cb;
	usbd_inic_ep_t *ep = &idev->out_ep[ep_num];

	UNUSED(dev);

	if (len == 0) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "EP%d RX ZLP\n", ep_num);
		return HAL_OK;
	}

	if (cb->received != NULL) {
		cb->received(ep, len);
	}

	return HAL_OK;
}

/**
  * @brief  Get descriptor callback
  * @param  req: setup request
  * @param  speed: device speed
  * @param  len: descriptor length
  * @retval descriptor buffer
  */
static u8 *usbd_inic_get_descriptor(usb_dev_t *dev, usb_setup_req_t *req, usb_speed_type_t speed, u16 *len)
{
	u8 *buf = NULL;
	u16 length;
	usbd_inic_dev_t *idev = &usbd_inic_dev;
	usbd_otp_t *otp = &idev->otp;
	u8 *desc = idev->ctrl_buf;

	UNUSED(speed);

	dev->self_powered = otp->self_powered;
	dev->remote_wakeup_en = otp->remote_wakeup_en;

	switch ((req->wValue >> 8) & 0xFF) {

	case USB_DESC_TYPE_DEVICE:
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "Get descriptor USB_DESC_TYPE_DEVICE\n");
		if (otp->bt_en) {
			buf = usbd_inic_dev_desc;
		} else {
			buf = usbd_inic_single_wifi_mode_dev_desc;
		}

		length = USB_LEN_DEV_DESC;

		rtw_memcpy((void *)desc, (void *)buf, length);

		if (otp->otp_param) {
			desc[USB_DEV_DESC_OFFSET_VID] = USB_LOW_BYTE(otp->vid);
			desc[USB_DEV_DESC_OFFSET_VID + 1] = USB_HIGH_BYTE(otp->vid);
			desc[USB_DEV_DESC_OFFSET_PID] = USB_LOW_BYTE(otp->pid);
			desc[USB_DEV_DESC_OFFSET_PID + 1] = USB_HIGH_BYTE(otp->pid);
		}

		buf = desc;
		*len = length;
		break;

	case USB_DESC_TYPE_CONFIGURATION:
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "Get descriptor USB_DESC_TYPE_CONFIGURATION\n");
		if (otp->bt_en) {
			buf = usbd_inic_config_desc;
			length = USBD_INIC_CONFIG_DESC_SIZE;
		} else {
			buf = usbd_inic_single_wifi_mode_config_desc;
			length = USBD_INIC_WIFI_ONLY_MODE_CONFIG_DESC_SIZE;
		}
		rtw_memcpy((void *)desc, (void *)buf, length);
		desc[USB_CFG_DESC_OFFSET_ATTR] &= ~(USB_CFG_DESC_OFFSET_ATTR_BIT_SELF_POWERED | USB_CFG_DESC_OFFSET_ATTR_BIT_REMOTE_WAKEUP);
		if (otp->self_powered) {
			desc[USB_CFG_DESC_OFFSET_ATTR] |= USB_CFG_DESC_OFFSET_ATTR_BIT_SELF_POWERED;
		}
		if (otp->remote_wakeup_en) {
			desc[USB_CFG_DESC_OFFSET_ATTR] |= USB_CFG_DESC_OFFSET_ATTR_BIT_REMOTE_WAKEUP;
		}
		buf = desc;
		*len = length;
		break;

	case USB_DESC_TYPE_DEVICE_QUALIFIER:
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "Get descriptor USB_DESC_TYPE_DEVICE_QUALIFIER\n");
		buf = usbd_inic_dev_qualifier_desc;
		*len = USB_LEN_DEV_QUALIFIER_DESC;
		break;

	case USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION:
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "Get descriptor USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION\n");
		if (otp->bt_en) {
			buf = usbd_inic_other_speed_config_desc;
			length = USBD_INIC_CONFIG_DESC_SIZE;
		} else {
			buf = usbd_inic_wifi_only_mode_other_speed_config_desc;
			length = USBD_INIC_WIFI_ONLY_MODE_CONFIG_DESC_SIZE;
		}
		rtw_memcpy((void *)desc, (void *)buf, length);
		desc[USB_CFG_DESC_OFFSET_ATTR] &= ~(USB_CFG_DESC_OFFSET_ATTR_BIT_SELF_POWERED | USB_CFG_DESC_OFFSET_ATTR_BIT_REMOTE_WAKEUP);
		if (otp->self_powered) {
			desc[USB_CFG_DESC_OFFSET_ATTR] |= USB_CFG_DESC_OFFSET_ATTR_BIT_SELF_POWERED;
		}
		if (otp->remote_wakeup_en) {
			desc[USB_CFG_DESC_OFFSET_ATTR] |= USB_CFG_DESC_OFFSET_ATTR_BIT_REMOTE_WAKEUP;
		}
		buf = desc;
		*len = length;
		break;

	case USB_DESC_TYPE_STRING:
		switch (req->wValue & 0xFF) {
		case USBD_IDX_LANGID_STR:
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "Get descriptor USBD_IDX_LANGID_STR\n");
			buf = usbd_inic_lang_id_desc;
			*len = USB_LEN_LANGID_STR_DESC;
			break;
		case USBD_IDX_MFC_STR:
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "Get descriptor USBD_IDX_MFC_STR\n");
			if (otp->otp_param) {
				buf = otp->mfg_str;
				*len = otp->mfg_str_len;
			} else {
				usbd_get_str_desc(USBD_INIC_MFG_STRING, desc, len);
				buf = desc;
			}
			break;
		case USBD_IDX_PRODUCT_STR:
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "Get descriptor USBD_IDX_PRODUCT_STR\n");
			if (otp->otp_param) {
				buf = otp->prod_str;
				*len = otp->prod_str_len;
			} else {
				usbd_get_str_desc(USBD_INIC_PROD_STRING, desc, len);
				buf = desc;
			}
			break;
		case USBD_IDX_SERIAL_STR:
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "Get descriptor USBD_IDX_SERIAL_STR\n");
			if (otp->otp_sn) {
				buf = otp->sn_str;
				*len = otp->sn_str_len;
			} else {
				usbd_get_str_desc(USBD_INIC_SN_STRING, desc, len);
				buf = desc;
			}
			break;
		case USBD_INIC_IDX_INTERFACE_STR:
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "Get descriptor USBD_INIC_IDX_INTERFACE_STR\n");
			usbd_get_str_desc(USBD_INIC_BT_STRING, desc, len);
			buf = desc;
			break;
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

static u8 usbd_inic_wifi_init(void)
{
	u8 ret;
	usbd_inic_dev_t *idev = &usbd_inic_dev;
	usbd_inic_ep_t *ep;
	u8 ep_num;

	ep_num = USB_EP_ADDR(USBD_INIC_WIFI_EP4_BULK_IN);
	ep = &idev->in_ep[ep_num];
	ep->num = ep_num;
	ep->buf_len = USBD_INIC_WIFI_EP4_BULK_IN_BUF_SIZE;
	ep->buf = (u8 *)rtw_zmalloc(ep->buf_len);
	if (ep->buf == NULL) {
		ret = HAL_ERR_MEM;
		goto wifi_init_exit;
	}
	ep->valid = 1U;

	ep_num = USB_EP_ADDR(USBD_INIC_WIFI_EP5_BULK_OUT);
	ep = &idev->out_ep[ep_num];
	ep->num = ep_num;
	ep->buf_len = USBD_INIC_WIFI_EP5_BULK_OUT_BUF_SIZE;
	ep->buf = (u8 *)rtw_zmalloc(ep->buf_len);
	if (ep->buf == NULL) {
		ret = HAL_ERR_MEM;
		goto wifi_init_clean_ep4_bulk_in_buf_exit;
	}
	ep->valid = 1U;

	ep_num = USB_EP_ADDR(USBD_INIC_WIFI_EP6_BULK_OUT);
	ep = &idev->out_ep[ep_num];
	ep->num = ep_num;
	ep->buf_len = USBD_INIC_WIFI_EP6_BULK_OUT_BUF_SIZE;
	ep->buf = (u8 *)rtw_zmalloc(ep->buf_len);
	if (ep->buf == NULL) {
		ret = HAL_ERR_MEM;
		goto wifi_init_clean_ep5_bulk_out_buf_exit;
	}
	ep->valid = 1U;

	ep_num = USB_EP_ADDR(USBD_INIC_WIFI_EP7_BULK_OUT);
	ep = &idev->out_ep[ep_num];
	ep->num = ep_num;
	ep->buf_len = USBD_INIC_WIFI_EP7_BULK_OUT_BUF_SIZE;
	ep->buf = (u8 *)rtw_zmalloc(ep->buf_len);
	if (ep->buf == NULL) {
		ret = HAL_ERR_MEM;
		goto wifi_init_clean_ep6_bulk_out_buf_exit;
	}
	ep->valid = 1U;

	return HAL_OK;

wifi_init_clean_ep6_bulk_out_buf_exit:
	ep = &idev->out_ep[USB_EP_ADDR(USBD_INIC_WIFI_EP6_BULK_OUT)];
	rtw_free(ep->buf);
	ep->buf = NULL;
	ep->valid = 0U;

wifi_init_clean_ep5_bulk_out_buf_exit:
	ep = &idev->out_ep[USB_EP_ADDR(USBD_INIC_WIFI_EP5_BULK_OUT)];
	rtw_free(ep->buf);
	ep->buf = NULL;
	ep->valid = 0U;

wifi_init_clean_ep4_bulk_in_buf_exit:
	ep = &idev->in_ep[USB_EP_ADDR(USBD_INIC_WIFI_EP4_BULK_IN)];
	rtw_free(ep->buf);
	ep->buf = NULL;
	ep->valid = 0U;

wifi_init_exit:
	return ret;
}

static u8 usbd_inic_bt_init(void)
{
	u8 ret = HAL_OK;
	usbd_inic_dev_t *idev = &usbd_inic_dev;
	usbd_inic_ep_t *ep;
	u8 ep_num;

	ep_num = USB_EP_ADDR(USBD_INIT_BT_EP1_INTR_IN);
	ep = &idev->in_ep[ep_num];
	ep->num = ep_num;
	ep->buf_len = USBD_INIC_BT_EP1_INTR_IN_BUF_SIZE;
	ep->buf = (u8 *)rtw_zmalloc(ep->buf_len);
	if (ep->buf == NULL) {
		ret = HAL_ERR_MEM;
		goto bt_init_exit;
	}
	ep->valid = 1U;

	ep_num = USB_EP_ADDR(USBD_INIC_BT_EP2_BULK_IN);
	ep = &idev->in_ep[ep_num];
	ep->num = ep_num;
	ep->buf_len = USBD_INIC_BT_EP2_BULK_IN_BUF_SIZE;
	ep->buf = (u8 *)rtw_zmalloc(ep->buf_len);
	if (ep->buf == NULL) {
		ret = HAL_ERR_MEM;
		goto bt_init_clean_ep1_intr_in_buf_exit;
	}
	ep->valid = 1U;

	ep_num = USB_EP_ADDR(USBD_INIC_BT_EP2_BULK_OUT);
	ep = &idev->out_ep[ep_num];
	ep->num = ep_num;
	ep->buf_len = USBD_INIC_BT_EP2_BULK_OUT_BUF_SIZE;
	ep->buf = (u8 *)rtw_zmalloc(ep->buf_len);
	if (ep->buf == NULL) {
		ret = HAL_ERR_MEM;
		goto bt_init_clean_ep2_bulk_in_buf_exit;
	}
	ep->valid = 1U;

	return HAL_OK;

bt_init_clean_ep2_bulk_in_buf_exit:
	ep = &idev->in_ep[USB_EP_ADDR(USBD_INIC_BT_EP2_BULK_IN)];
	rtw_free(ep->buf);
	ep->buf = NULL;

bt_init_clean_ep1_intr_in_buf_exit:
	ep = &idev->in_ep[USB_EP_ADDR(USBD_INIT_BT_EP1_INTR_IN)];
	rtw_free(ep->buf);
	ep->buf = NULL;

bt_init_exit:
	return ret;
}

static void usbd_inic_wifi_deinit(void)
{
	usbd_inic_dev_t *idev = &usbd_inic_dev;
	usbd_inic_ep_t *ep;

	ep = &idev->in_ep[USB_EP_ADDR(USBD_INIC_WIFI_EP4_BULK_IN)];
	if (ep->buf != NULL) {
		rtw_free(ep->buf);
		ep->buf = NULL;
	}
	ep->valid = 0U;

	ep = &idev->out_ep[USB_EP_ADDR(USBD_INIC_WIFI_EP5_BULK_OUT)];
	if (ep->buf != NULL) {
		rtw_free(ep->buf);
		ep->buf = NULL;
	}
	ep->valid = 0U;

	ep = &idev->out_ep[USB_EP_ADDR(USBD_INIC_WIFI_EP6_BULK_OUT)];
	if (ep->buf != NULL) {
		rtw_free(ep->buf);
		ep->buf = NULL;
	}
	ep->valid = 0U;

	ep = &idev->out_ep[USB_EP_ADDR(USBD_INIC_WIFI_EP7_BULK_OUT)];
	if (ep->buf != NULL) {
		rtw_free(ep->buf);
		ep->buf = NULL;
	}
	ep->valid = 0U;
}

static void usbd_inic_bt_deinit(void)
{
	usbd_inic_dev_t *idev = &usbd_inic_dev;
	usbd_inic_ep_t *ep;

	ep = &idev->in_ep[USB_EP_ADDR(USBD_INIT_BT_EP1_INTR_IN)];
	if (ep->buf != NULL) {
		rtw_free(ep->buf);
		ep->buf = NULL;
	}
	ep->valid = 0U;

	ep = &idev->in_ep[USB_EP_ADDR(USBD_INIC_BT_EP2_BULK_IN)];
	if (ep->buf != NULL) {
		rtw_free(ep->buf);
		ep->buf = NULL;
	}
	ep->valid = 0U;

	ep = &idev->out_ep[USB_EP_ADDR(USBD_INIC_BT_EP2_BULK_OUT)];
	if (ep->buf != NULL) {
		rtw_free(ep->buf);
		ep->buf = NULL;
	}
	ep->valid = 0U;
}

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Initialize inic device
  * @retval Status
  */
u8 usbd_inic_init(usbd_inic_cb_t *cb)
{
	u8 ret = HAL_OK;
	usbd_inic_dev_t *idev = &usbd_inic_dev;
	usbd_otp_t *otp = &idev->otp;

	if (usbd_otp_init(otp) != HAL_OK) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Fail to load USB OTP parameters\n");
	}

	idev->ctrl_buf_len = USBD_INIC_CTRL_BUF_SIZE;
	idev->ctrl_buf = (u8 *)rtw_zmalloc(idev->ctrl_buf_len);
	if (idev->ctrl_buf == NULL) {
		ret = HAL_ERR_MEM;
		goto init_exit;
	}

	ret = usbd_inic_wifi_init();
	if (ret != HAL_OK) {
		goto init_clean_ctrl_buf_exit;
	}

	if (otp->bt_en) {
		ret = usbd_inic_bt_init();
		if (ret != HAL_OK) {
			goto init_deinit_wifi_exit;
		}
	}

	if (cb != NULL) {
		idev->cb = cb;
		if (cb->init != NULL) {
			ret = cb->init();
			if (ret != HAL_OK) {
				goto init_deinit_bt_exit;
			}
		}
	}

	usbd_register_class(&usbd_inic_driver);

	return ret;

init_deinit_bt_exit:
	usbd_inic_bt_deinit();

init_deinit_wifi_exit:
	usbd_inic_wifi_deinit();

init_clean_ctrl_buf_exit:
	rtw_free(idev->ctrl_buf);
	idev->ctrl_buf = NULL;

init_exit:
	return ret;
}

/**
  * @brief  DeInitialize inic device
  * @param  void
  * @retval Status
  */
u8 usbd_inic_deinit(void)
{
	usbd_inic_dev_t *idev = &usbd_inic_dev;
	usbd_otp_t *otp = &idev->otp;

	if (idev->cb != NULL) {
		if (idev->cb->deinit != NULL) {
			idev->cb->deinit();
		}
		idev->cb = NULL;
	}

	usbd_unregister_class();

	if (idev->ctrl_buf != NULL) {
		rtw_free(idev->ctrl_buf);
		idev->ctrl_buf = NULL;
	}

	usbd_inic_wifi_deinit();

	if (otp->bt_en) {
		usbd_inic_bt_deinit();
	}

	usbd_otp_deinit(otp);

	return HAL_OK;
}

u8 usbd_inic_transmit_ctrl_data(u8 *buf, u16 len)
{
	usbd_inic_dev_t *idev = &usbd_inic_dev;
	usb_dev_t *dev = idev->dev;

	if (len > idev->ctrl_buf_len) {
		len = idev->ctrl_buf_len;
	}

	DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "CTRL TX len=%d\n", len);

	rtw_memcpy(idev->ctrl_buf, buf, len);

	usbd_ep0_transmit(dev, idev->ctrl_buf, len);

	return HAL_OK;
}

u8 usbd_inic_transmit_data(u8 ep_num, u8 *buf, u16 len)
{
	u8 ret = HAL_OK;
	u8 num = USB_EP_ADDR(ep_num);
	usbd_inic_dev_t *idev = &usbd_inic_dev;
	usbd_inic_ep_t *ep;

	if (USB_EP_IS_OUT(ep_num) || (num >= USB_OTG_MAX_ENDPOINTS)) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Invalid IN EP number: 0x%02X\n", ep_num);
		return HAL_ERR_PARA;
	}

	ep = &idev->in_ep[num];

	if (len > ep->buf_len) {
		len = ep->buf_len;
	}

	if (len == ep->mps) {
		ep->zlp = 1;
	} else {
		ep->zlp = 0;
	}

	if (ep->state == USBD_INIC_EP_STATE_IDLE) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "EP%d TX len=%d data=%d\n", num, len, buf[0]);
		ep->state = USBD_INIC_EP_STATE_BUSY;
		rtw_memcpy((void *)ep->buf, (void *)buf, len);
		usbd_ep_transmit(idev->dev, ep_num, ep->buf, len);
	} else {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "EP%d TX len=%d data=%d: BUSY\n", num, len, buf[0]);
		ret = HAL_BUSY;
	}

	return ret;
}

u8 usbd_inic_receive_data(u8 ep_num)
{
	u8 num = USB_EP_ADDR(ep_num);
	usbd_inic_dev_t *idev = &usbd_inic_dev;
	usbd_inic_ep_t *ep;

	if (USB_EP_IS_IN(ep_num) || (num >= USB_OTG_MAX_ENDPOINTS)) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Invalid OUT EP number: 0x%02X\n", ep_num);
		return HAL_ERR_PARA;
	}

	ep = &idev->out_ep[num];

	usbd_ep_receive(idev->dev, ep_num, ep->buf, ep->buf_len);

	return HAL_OK;
}

