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

#ifndef USBD_LOOPBACK_H
#define USBD_LOOPBACK_H

/* Includes ------------------------------------------------------------------*/

#include "usbd.h"

/* Exported defines ----------------------------------------------------------*/

/* Vendor Device parameters */
#define USBD_LOOPBACK_VID                                 0x0BDAU
#define USBD_LOOPBACK_PID                                 0x1234U
#define USBD_LOOPBACK_CONFIG_DESC_SIZE                    46U
#define USBD_LOOPBACK_LANGID_STRING                       0x0409U
#define USBD_LOOPBACK_MFG_STRING_DESC_SIZE                16U
#define USBD_LOOPBACK_PRODUCT_STRING_DESC_SIZE            44U
#define USBD_LOOPBACK_SN_STRING_DESC_SIZE                 26U
#define USBD_LOOPBACK_SELF_POWERED						  1U

/* Vendor Endpoint parameters */
#define USBD_LOOPBACK_INTR_IN_EP                          0x81U  /* EP1 for INTR IN */
#define USBD_LOOPBACK_INTR_OUT_EP                         0x02U  /* EP2 for INTR OUT */
#define USBD_LOOPBACK_ISOC_IN_EP                          0x83U  /* EP3 for ISOC IN */
#define USBD_LOOPBACK_ISOC_OUT_EP                         0x04U  /* EP4 for ISOC OUT */

#define USBD_LOOPBACK_INTR_HS_MAX_PACKET_SIZE             64U    /* High speed INTR packet size */
#define USBD_LOOPBACK_INTR_FS_MAX_PACKET_SIZE             64U    /* Full speed INTR packet size */

#define USBD_LOOPBACK_ISOC_HS_MAX_PACKET_SIZE             64U    /* High speed ISOC packet size */
#define USBD_LOOPBACK_ISOC_FS_MAX_PACKET_SIZE             64U    /* Full speed ISOC packet size */

#define USBD_LOOPBACK_INTR_HS_INTERVAL                    4U     /* High speed INTR interval */
#define USBD_LOOPBACK_INTR_FS_INTERVAL                    8U     /* Full speed INTR interval */

#define USBD_LOOPBACK_ISOC_HS_INTERVAL                    4U     /* High speed ISOC interval */
#define USBD_LOOPBACK_ISOC_FS_INTERVAL                    8U     /* Full speed ISOC interval */

#define USBD_LOOPBACK_START_TEST						  1U

/* Exported types ------------------------------------------------------------*/

typedef struct {
	u8(* init)(void);
	u8(* deinit)(void);
	u8(* setup)(u8 cmd, u8 *buf, u16 len, u16 value);
	u8(* intr_received)(u8 *buf, u32 len);
	u8(* isoc_received)(u8 *buf, u32 len);
	u8(* intr_transmitted)(u8 status);
	u8(* isoc_transmitted)(u8 status);
} usbd_loopback_cb_t;

typedef struct {
	usb_dev_t *dev;
	usbd_loopback_cb_t *cb;

	u32 intr_out_buf_len;
	u8 *intr_out_buf;
	u32 intr_in_buf_len;
	u8 *intr_in_buf;
	u8 intr_in_state;

	u32 isoc_out_buf_len;
	u8 *isoc_out_buf;
	u32 isoc_in_buf_len;
	u8 *isoc_in_buf;
	u8 isoc_in_state;
} usbd_loopback_dev_t;

/* Exported macros -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

u8 usbd_loopback_init(u32 nptx_buf_len, u32 nprx_buf_len, u32 ptx_buf_len, u32 prx_buf_len, usbd_loopback_cb_t *cb);
u8 usbd_loopback_deinit(void);
u8 usbd_loopback_intr_transmit(u8 *buf, u16 len);
u8 usbd_loopback_intr_receive(void);
u8 usbd_loopback_isoc_transmit(u8 *buf, u16 len);
u8 usbd_loopback_isoc_receive(void);

#endif  /* USBD_LOOPBACK_H */

