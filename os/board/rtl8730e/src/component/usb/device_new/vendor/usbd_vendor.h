/**
  ******************************************************************************
  * The header file for usbd_vendor.c
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef USBD_VENDOR_H
#define USBD_VENDOR_H

/* Includes ------------------------------------------------------------------*/

#include "usbd.h"

/* Exported defines ----------------------------------------------------------*/

/*  Vendor Test parameters */
#define CONFIG_USBD_VENDOR_BULK_TEST					0
#define CONFIG_USBD_VENDOR_INTR_TEST					1
#define CONFIG_USBD_VENDOR_ISOC_TEST					1
#define USBD_VENDOR_BULK_OUT_XFER_CNT					100
#define USBD_VENDOR_INTR_OUT_XFER_CNT					100
#define USBD_VENDOR_ISOC_OUT_XFER_CNT					100
#define USBD_VENDOR_BULK_OUT_BUF_SIZE					64U   /* BULK OUT buffer size */
#define USBD_VENDOR_BULK_IN_BUF_SIZE					64U   /* BULK IN buffer size */
#define USBD_VENDOR_INTR_OUT_BUF_SIZE					64U   /* INTR OUT buffer size */
#define USBD_VENDOR_INTR_IN_BUF_SIZE					64U   /* INTR IN buffer size */
#define USBD_VENDOR_ISOC_OUT_BUF_SIZE					64U   /* ISOC OUT buffer size */
#define USBD_VENDOR_ISOC_IN_BUF_SIZE					64U   /* ISOC IN buffer size */
#define USBD_VENDOR_CTRL_BUF_SIZE						512U  /* CTRL buffer size */

/* Vendor Device parameters */
#define USBD_VENDOR_VID									0x0BDAU
#define USBD_VENDOR_PID									0x8730U
#define USBD_VENDOR_CLASS_CODE							0xFFU
#define USBD_VENDOR_SUBCLASS_CODE						0x00U
#define USBD_VENDOR_PROTOCOL							0x00U
#define USBD_VENDOR_CONFIG_DESC_SIZE					83U
#define USBD_VENDOR_LANGID_STRING						0x0409U
#define USBD_VENDOR_MFG_STRING_DESC_SIZE				16U
#define USBD_VENDOR_PRODUCT_STRING_DESC_SIZE			40U
#define USBD_VENDOR_SN_STRING_DESC_SIZE					26U
#define USBD_VENDOR_SELF_POWERED						1U
#define USBD_VENDOR_REMOTE_WAKEUP_EN					1U

/* Vendor Endpoint parameters */
#define USBD_VENDOR_BULK_IN_EP							0x81U  /* EP1 for BULK IN */
#define USBD_VENDOR_BULK_OUT_EP							0x02U  /* EP2 for BULK OUT */
#define USBD_VENDOR_INTR_IN_EP							0x81U  /* EP1 for INTR IN */
#define USBD_VENDOR_INTR_OUT_EP							0x02U  /* EP2 for INTR OUT */
#define USBD_VENDOR_ISOC_IN_EP							0x83U  /* EP3 for ISOC IN */
#define USBD_VENDOR_ISOC_OUT_EP							0x04U  /* EP4 for ISOC OUT */

#define USBD_VENDOR_HS_BULK_MPS							64U   /* High speed BULK IN & OUT max packet size */
#define USBD_VENDOR_FS_BULK_MPS							64U   /* Full speed BULK IN & OUT max packet size */
#define USBD_VENDOR_HS_INTR_MPS							64U   /* High speed INTR IN & OUT max packet size */
#define USBD_VENDOR_FS_INTR_MPS							64U   /* Full speed INTR IN & OUT max packet size */
#define USBD_VENDOR_HS_ISOC_MPS							64U   /* High speed ISOC IN & OUT max packet size */
#define USBD_VENDOR_FS_ISOC_MPS							64U   /* Full speed ISOC IN & OUT max packet size */

#define USBD_VENDOR_HS_INTR_IN_INTERVAL					1U    /* High speed INTR IN interval */
#define USBD_VENDOR_HS_INTR_OUT_INTERVAL				1U    /* High speed INTR OUT interval */
#define USBD_VENDOR_FS_INTR_IN_INTERVAL					2U    /* Full speed INTR IN interval */
#define USBD_VENDOR_FS_INTR_OUT_INTERVAL				2U    /* Full speed INTR OUT interval */

#define USBD_VENDOR_HS_ISOC_IN_INTERVAL					1U    /* High speed ISOC IN interval */
#define USBD_VENDOR_HS_ISOC_OUT_INTERVAL				1U    /* High speed ISOC OUT interval */
#define USBD_VENDOR_FS_ISOC_IN_INTERVAL					2U    /* Full speed ISOC IN interval */
#define USBD_VENDOR_FS_ISOC_OUT_INTERVAL				2U    /* Full speed ISOC OUT interval */

/* Exported types ------------------------------------------------------------*/

typedef struct {
	u8(* init)(void);
	u8(* deinit)(void);
	u8(* setup)(u8 cmd, u8 *buf, u16 len, u16 value);
	u8(* set_config)(void);
	u8(* bulk_received)(u8 *buf, u32 len);
	u8(* intr_received)(u8 *buf, u32 len);
	u8(* isoc_received)(u8 *buf, u32 len);
	void(* bulk_transmitted)(u8 status);
	void(* intr_transmitted)(u8 status);
	void(* isoc_transmitted)(u8 status);
} usbd_vendor_cb_t;

typedef struct {
	u8 *ctrl_buf;
	u8  ctrl_req;
	u16 ctrl_data_len;
	u8  alt_setting;
	__IO u32 ctrl_in_state;
	u8 *bulk_out_buf;
	u8 *bulk_in_buf;
	__IO u32 bulk_in_state;
	__IO u32 bulk_in_zlp;
	u8 *intr_out_buf;
	u8 *intr_in_buf;
	__IO u32 intr_in_state;
	u8 *isoc_out_buf;
	u8 *isoc_in_buf;
	usb_dev_t *dev;
	usbd_vendor_cb_t *cb;
} usbd_vendor_dev_t;

/* Exported macros -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

u8 usbd_vendor_init(usbd_vendor_cb_t *cb);
u8 usbd_vendor_deinit(void);
u8 usbd_vendor_transmit_ctrl_data(u8 *buf, u16 len);
u8 usbd_vendor_transmit_bulk_data(u8 *buf, u16 len);
u8 usbd_vendor_receive_bulk_data(void);
u8 usbd_vendor_transmit_intr_data(u8 *buf, u16 len);
u8 usbd_vendor_receive_intr_data(void);
u8 usbd_vendor_transmit_isoc_data(u8 *buf, u16 len);
u8 usbd_vendor_receive_isoc_data(void);

#endif  /* USBD_VENDOR_H */

