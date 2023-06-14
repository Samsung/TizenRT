/**
  ******************************************************************************
  * The header file for usbh_loopback.c
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef USBH_LOOPBACK_H
#define USBH_LOOPBACK_H

/* Includes ------------------------------------------------------------------*/

#include "usbh.h"

/* Exported defines ----------------------------------------------------------*/

/* Vendor Class Codes */
#define LOOPBACK_CLASS_CODE					0xFFU
/* Vendor SubClass Codes */
#define LOOPBACK_SUBCLASS_CODE				0x00U
/* Vendor Protocol */
#define LOOPBACK_PROTOCOL					0x00U

#define USB_EP_XFERTYPE_MASK				0x03U

#define USBH_LOOPBACK_INTR_IN_BUF_SIZE		64U
#define USBH_LOOPBACK_INTR_OUT_BUF_SIZE		64U
#define USBH_LOOPBACK_ISOC_IN_BUF_SIZE		128U
#define USBH_LOOPBACK_ISOC_OUT_BUF_SIZE		128U

#define USBH_LOOPBACK_START_TEST			1U

/* Exported types ------------------------------------------------------------*/

/* States for class */
typedef enum {
	LOOPBACK_STATE_IDLE = 0U,
	LOOPBACK_STATE_SETUP,
	LOOPBACK_STATE_TRANSFER,
	LOOPBACK_STATE_ERROR
} usbh_loopback_state_t;

/* States for transfer */
typedef enum {
	LOOPBACK_TRANSFER_STATE_IDLE = 0U,
	LOOPBACK_TRANSFER_STATE_TX,
	LOOPBACK_TRANSFER_STATE_TX_BUSY,
	LOOPBACK_TRANSFER_STATE_RX,
	LOOPBACK_TRANSFER_STATE_RX_BUSY
} usbh_loopback_transfer_state_t;

/* Vendor user callback interface */
typedef struct {
	u8(* init)(void);
	u8(* deinit)(void);
	u8(* attach)(void);
	u8(* detach)(void);
	u8(* setup)(void);
	u8(* intr_transmitted)(usbh_urb_state_t state);
	u8(* isoc_transmitted)(usbh_urb_state_t state);
	u8(* intr_received)(u8 *buf, u32 length);
	u8(* isoc_received)(u8 *buf, u32 length);
} usbh_loopback_cb_t;

/* Vendor host */
typedef struct {
	u8  ctrl_buf[18];

	u8 *intr_in_buf;
	u32 intr_in_len;
	u8  intr_in_pipe;
	u8  intr_in_ep;
	u16 intr_in_packet_size;
	usbh_loopback_transfer_state_t intr_in_state;

	u8 *intr_out_buf;
	u32 intr_out_len;
	u8  intr_out_pipe;
	u8  intr_out_ep;
	u16 intr_out_packet_size;
	usbh_loopback_transfer_state_t intr_out_state;

	u8 *isoc_in_buf;
	u32 isoc_in_len;
	u8  isoc_in_pipe;
	u8  isoc_in_ep;
	u16 isoc_in_packet_size;
	u32  isoc_in_interval;
	__IO u32 isoc_in_tick;
	usbh_loopback_transfer_state_t isoc_in_state;

	u8 *isoc_out_buf;
	u32 isoc_out_len;
	u8  isoc_out_pipe;
	u8  isoc_out_ep;
	u16 isoc_out_packet_size;
	u32  isoc_out_interval;
	__IO u32 isoc_out_tick;
	usbh_loopback_transfer_state_t isoc_out_state;

	usbh_loopback_state_t state;

	usbh_loopback_cb_t *cb;
	usb_host_t *host;
} usbh_loopback_host_t;

/* Exported macros -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

u8 usbh_loopback_init(usbh_loopback_cb_t *cb);
u8 usbh_loopback_deinit(void);
u8 usbh_loopback_setup(void);
u8 usbh_loopback_intr_transmit(u8 *buf, u32 len);
u8 usbh_loopback_isoc_transmit(u8 *buf, u32 len);
u8 usbh_loopback_intr_receive(u8 *buf, u32 len);
u8 usbh_loopback_isoc_receive(u8 *buf, u32 len);

#endif  /* USBH_LOOPBACK_H */

