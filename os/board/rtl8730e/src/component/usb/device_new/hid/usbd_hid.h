/**
  ******************************************************************************
  * The header file for usbd_hid.c
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef USBD_HID_H
#define USBD_HID_H

/* Includes ------------------------------------------------------------------*/

#include "usbd.h"


/* Exported defines ----------------------------------------------------------*/


#define HID_VID                                 0x0BDAU
#define HID_PID                                 0x4042U


/*hid request*/
#define USBD_HID_GET_REPORT			0x01
#define USBD_HID_GET_IDLE			0x02
#define USBD_HID_GET_PROTOCOL		0x03
#define USBD_HID_SET_REPORT			0x09
#define USBD_HID_SET_IDLE			0x0a
#define USBD_HID_SET_PROTOCOL		0x0b



#define HID_CTRL_BUF_SIZE   512
#define HID_INTR_IN_BUF_SIZE   512


#define HID_VID                                 0x0BDAU
#define HID_PID                                 0x4042U
#define HID_CONFIG_DESC_SIZ						34U
#define HID_LANGID_STRING                       0x0409U
#define HID_MFG_STRING_DESC_SIZE                16U
#define HID_PRODUCT_STRING_DESC_SIZE            34U
#define HID_SN_STRING_DESC_SIZE                 26U
#define HID_SELF_POWERED						1U


/*wValue of HID report request*/
#define HID_DESC 			0x21
#define HID_REPORT_DESC 	0x22

// String index
#define USBD_HID_STRING_MANUFACTURER         1
#define USBD_HID_STRING_PRODUCT              2
#define USBD_HID_STRING_SERIALNUMBER         3
#define USBD_HID_STRING_STRING_INTERFACE     4

// Endpoint address
#define HID_INTERRUPT_IN_EP_ADDRESS		0x81
#define HID_DEFAULT_INT_IN_XFER_SIZE		8
#define HID_MOUSE_REPORT_DESC_SIZE				52
#define USBD_HID_DESC_SIZE						0x9

//mouse button define
#define MOUSE_BUTTON_LEFT			0x01	//left button. 0: release, 1: press
#define MOUSE_BUTTON_RIGHT			0x02	//right button. 0: release, 1: press
#define MOUSE_BUTTON_MIDDLE			0x04	//wheel button. 0: release, 1: press
#define MOUSE_BUTTON_RESERVED		0xf8	//reserved. all bit should set to 1.


/* Exported types ------------------------------------------------------------*/


typedef struct {
	void(* init)(void);
	void(* deinit)(void);
	void(* setup)(void);
	void(* transmit_complete)(u8 status);
} usbd_hid_usr_cb_t;


struct usb_hid_device_t {
	u32 intr_in_state;
	u8 *intr_in_buf;

	u32 Protocol;
	u32 IdleState;
	u32 AltSetting;

	u8 *ctrl_buf; 		/* TODO: Move this buffer to usb_dev_t */
	u8  ctrl_req;
	u8  ctrl_data_len;	/* TODO: Remove and get the data length via usbd_core_get_rx_data_size? */
	usbd_hid_usr_cb_t *cb;
	usb_dev_t *dev;
};

u8 usbd_hid_init(u16 tx_buf_len, usbd_hid_usr_cb_t *cb);
u8 usbd_hid_deinit(void);
u8 usbd_hid_send_data(u8 *data, u16 len);

/* Exported functions --------------------------------------------------------*/

#endif // USBD_HID_H
