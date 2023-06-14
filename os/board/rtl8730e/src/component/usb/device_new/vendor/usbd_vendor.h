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
#define CONFIG_USBD_VENDOR_ISO_IN_TEST          0
#define CONFIG_USBD_VENDOR_ISO_OUT_TEST         0
#define CONFIG_USBD_VENDOR_BULK_INOUT_TEST      1
#define USBD_VENDOR_OUT_BUF_SIZE                       512U   /* out buffer size */
#define USBD_VENDOR_IN_BUF_SIZE                       512U   /* in buffer size */
#define USBD_VENDOR_ISO_OUT_XFER_CNT                  100

/* Vendor Device parameters */
#define USBD_VENDOR_VID                                 0x0BDAU
#define USBD_VENDOR_PID                                 0x8730U
#define USBD_VENDOR_CONFIG_DESC_SIZE                    69U
#define USBD_VENDOR_LANGID_STRING                       0x0409U
#define USBD_VENDOR_MFG_STRING_DESC_SIZE                16U
#define USBD_VENDOR_PRODUCT_STRING_DESC_SIZE            40U
#define USBD_VENDOR_SN_STRING_DESC_SIZE                 26U
#define USBD_VENDOR_SELF_POWERED						1U

/* Vendor Endpoint parameters */
#define USBD_VENDOR_BULK_IN_EP                          0x81U  /* EP1 for BULK IN */
#define USBD_VENDOR_BULK_OUT_EP                         0x02U  /* EP2 for BULK OUT */
#define USBD_VENDOR_ISO_IN_EP                          0x83U  /* EP3 for ISO IN */
#define USBD_VENDOR_ISO_OUT_EP                          0x04U  /* EP4 for ISO OUT */

#define USBD_VENDOR_HS_MAX_PACKET_SIZE             512U   /* High speed IN & OUT packet size */
#define USBD_VENDOR_FS_MAX_PACKET_SIZE             64U    /* Full speed IN & OUT packet size */

#define USBD_VENDOR_HS_ISO_IN_INTERVAL                 1U    /* High speed ISO IN interval */
#define USBD_VENDOR_HS_ISO_OUT_INTERVAL                 1U    /* High speed ISO OUT interval */
#define USBD_VENDOR_FS_ISO_IN_INTERVAL                 2U    /* Full speed ISO IN interval */
#define USBD_VENDOR_FS_ISO_OUT_INTERVAL                 2U    /* Full speed ISO OUT interval */

#define USBD_VENDOR_CTRL_BUF_SIZE                       512U   /* Control buffer size */

/* Exported types ------------------------------------------------------------*/

typedef struct {
	u8 *out_buf;
	u8 *in_buf;
	usb_dev_t *dev;
} usbd_vendor_dev_t;

/* Exported macros -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

u8 usbd_vendor_init(void);
u8 usbd_vendor_deinit(void);
void usbd_vendor_send_data(u16 len);

#endif  /* USBD_VENDOR_H */

