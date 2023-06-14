/**
  ******************************************************************************
  * The header file for usbh_vendor.c
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef USBH_VENDOR_H
#define USBH_VENDOR_H

/* Includes ------------------------------------------------------------------*/

#include "usbh.h"

/* Exported defines ----------------------------------------------------------*/

/* Vendor Class Codes */
#define VENDOR_CLASS_CODE						0xFFU
/* Vendor SubClass Codes */
#define VENDOR_SUBCLASS_CODE					0x00U
/* Vendor Protocol */
#define VENDOR_PROTOCOL						0x00U
#define USB_EP_XFERTYPE_MASK					0x03U

#define CONFIG_USBH_VENDOR_CTRL_TEST		0
#define CONFIG_USBH_VENDOR_BULK_TEST		1
#define CONFIG_USBH_VENDOR_ISO_IN_TEST		0
#define CONFIG_USBH_VENDOR_ISO_OUT_TEST	0

#define USBH_VENDOR_IN_BUF_SIZE				512
#define USBH_VENDOR_OUT_BUF_SIZE			512
#define USBH_VENDOR_ISO_IN_CNT				100
#define USBH_VENDOR_ISO_OUT_CNT				100

/* Exported types ------------------------------------------------------------*/

/* States for Vendor State Machine */
typedef enum {
	VENDOR_STATE_IDLE = 0U,
	VENDOR_STATE_TX,
	VENDOR_STATE_TX_BUSY,
	VENDOR_STATE_RX,
	VENDOR_STATE_RX_BUSY,
	VENDOR_STATE_SEND_REQ,
	VENDOR_STATE_ERROR,
} usbh_vendor_state_t;

/* Vendor user callback interface */
typedef struct {
	u8(* detach)(void);
	u8(* setup)(void);
} usbh_vendor_cb_t;

/* Vendor host */
typedef struct {
	u8 ctrl_buf[18];
	u8  bulk_in_pipe;
	u8  bulk_in_ep;
	u16 bulk_in_packet_size;
	u8  bulk_out_pipe;
	u8  bulk_out_ep;
	u16 bulk_out_packet_size;
	u8  iso_in_pipe;
	u8  iso_in_ep;
	u16 iso_in_packet_size;
	u8  iso_out_pipe;
	u8  iso_out_ep;
	u16 iso_out_packet_size;
	u8 *bulk_in_buf;
	u32 bulk_in_len;
	u8 *bulk_out_buf;
	u32 bulk_out_len;
	u8 *iso_in_buf;
	u32 iso_in_len;
	u8 *iso_out_buf;
	u32 iso_out_len;
	u8 *iso_in_buf_save;
	u8 *iso_out_buf_save;
	usbh_vendor_state_t          state;
	usbh_vendor_cb_t             *cb;
	usb_host_t                    *host;
} usbh_vendor_host_t;

/* Exported macros -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

u8 usbh_vendor_init(usbh_vendor_cb_t *cb);
u8 usbh_vendor_deinit(void);

#endif  /* USBH_VENDOR_H */

