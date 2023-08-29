/**
  ******************************************************************************
  * The header file for usbd_inic.c
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef USBD_INIC_H
#define USBD_INIC_H

/* Includes ------------------------------------------------------------------*/

#include "usbd.h"
#include "usbd_inic_otp.h"

/* Exported defines ----------------------------------------------------------*/

#define USBD_INIC_VID									0x0BDA
#define USBD_INIC_PID									0x8720

#define USBD_INIC_LANGID_STRING							0x409
#define USBD_INIC_IDX_INTERFACE_STR						0x04U

#define USBD_INIC_CONFIG_DESC_SIZE						0xE5U
#define USBD_INIC_WIFI_ONLY_MODE_CONFIG_DESC_SIZE		0x35U

#define USBD_INIC_DESC_BUF_SIZE							(USBD_INIC_CONFIG_DESC_SIZE)

#define USBD_INIC_ITF_NUM								3U
#define USBD_INIC_ITF_BT								0U
#define USBD_INIC_ITF_BT_SCO							1U
#define USBD_INIC_ITF_WIFI								2U

#define USBD_INIC_HS_BULK_MPS							512U   /* High speed BULK IN & OUT max packet size */
#define USBD_INIC_FS_BULK_MPS							64U   /* Full speed BULK IN & OUT max packet size */
#define USBD_INIC_HS_INTR_MPS							16U   /* High speed INTR IN & OUT max packet size */
#define USBD_INIC_FS_INTR_MPS							16U   /* Full speed INTR IN & OUT max packet size */

#define USBD_INIT_BT_EP1_INTR_IN						0x81U
#define USBD_INIC_BT_EP2_BULK_IN						0x82U
#define USBD_INIC_BT_EP2_BULK_OUT						0x02U
#define USBD_INIC_WIFI_EP4_BULK_IN 						0x84U
#define USBD_INIC_WIFI_EP5_BULK_OUT						0x05U
#define USBD_INIC_WIFI_EP6_BULK_OUT						0x06U
#define USBD_INIC_WIFI_EP7_BULK_OUT						0x07U

#define USBD_INIC_CTRL_BUF_SIZE							512U  /* CTRL buffer size */
#define USBD_INIC_BT_EP1_INTR_IN_BUF_SIZE				16U   /* BT EP1 INTR IN buffer size */
#define USBD_INIC_BT_EP2_BULK_IN_BUF_SIZE				512U  /* BT EP2 BULK IN buffer size */
#define USBD_INIC_BT_EP2_BULK_OUT_BUF_SIZE				512U  /* BT EP2 BULK OUT buffer size */
#define USBD_INIC_WIFI_EP4_BULK_IN_BUF_SIZE 			512U  /* WiFi EP4 BULK IN buffer size */
#define USBD_INIC_WIFI_EP5_BULK_OUT_BUF_SIZE			512U  /* WiFi EP5 BULK OUT buffer size */
#define USBD_INIC_WIFI_EP6_BULK_OUT_BUF_SIZE			512U  /* WiFi EP5 BULK OUT buffer size */
#define USBD_INIC_WIFI_EP7_BULK_OUT_BUF_SIZE			512U  /* WiFi EP5 BULK OUT buffer size */

#define USBD_INIC_EP_STATE_IDLE							0U
#define USBD_INIC_EP_STATE_BUSY							1U

/* Exported types ------------------------------------------------------------*/

typedef struct {
	u8 num;
	u16 mps;
	u8 valid;
	u16 buf_len;
	u8 *buf;
	__IO u8 state;
	__IO u8 zlp;
} usbd_inic_ep_t;

typedef struct {
	// Common callbacks
	u8(* init)(void);
	u8(* deinit)(void);
	u8(* setup)(u8 cmd, u8 *buf, u16 len, u16 value);
	u8(* set_config)(void);
	u8(* clear_config)(void);
	void(* transmitted)(usbd_inic_ep_t *ep, u8 status);
	u8(* received)(usbd_inic_ep_t *ep, u16 len);
} usbd_inic_cb_t;

typedef struct {
	u16 ctrl_buf_len;
	u8 *ctrl_buf;
	u8  ctrl_req;
	u16 ctrl_data_len;
	__IO u8 ctrl_in_state;
	u8  bt_alt;
	u8  bt_sco_alt;
	u8  wifi_alt;
	usbd_inic_ep_t in_ep[USB_OTG_MAX_ENDPOINTS];
	usbd_inic_ep_t out_ep[USB_OTG_MAX_ENDPOINTS];
	usbd_otp_t otp;
	usb_dev_t *dev;
	usbd_inic_cb_t *cb;
} usbd_inic_dev_t;

/* Exported macros -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

u8 usbd_inic_init(usbd_inic_cb_t *cb);
u8 usbd_inic_deinit(void);
u8 usbd_inic_transmit_ctrl_data(u8 *buf, u16 len);
u8 usbd_inic_transmit_data(u8 ep_num, u8 *buf, u16 len);
u8 usbd_inic_receive_data(u8 ep_num);

#endif  /* USBD_INIC_H */

