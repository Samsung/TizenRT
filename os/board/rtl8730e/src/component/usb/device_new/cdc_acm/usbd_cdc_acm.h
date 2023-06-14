/**
  ******************************************************************************
  * The header file for usbd_cdc.c
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef USBD_CDC_ACM_H
#define USBD_CDC_ACM_H

/* Includes ------------------------------------------------------------------*/

#include "usbd.h"

/* Exported defines ----------------------------------------------------------*/

#define CONFIG_CDC_ACM_NOTIFY                       1

/*  CDC definitions */
#define CDC_SEND_ENCAPSULATED_COMMAND               0x00U
#define CDC_GET_ENCAPSULATED_RESPONSE               0x01U
#define CDC_SET_COMM_FEATURE                        0x02U
#define CDC_GET_COMM_FEATURE                        0x03U
#define CDC_CLEAR_COMM_FEATURE                      0x04U
#define CDC_SET_LINE_CODING                         0x20U
#define CDC_GET_LINE_CODING                         0x21U
#define CDC_SET_CONTROL_LINE_STATE                  0x22U
#define CDC_SEND_BREAK                              0x23U

/* CDC ACM class-specific notification type */
#define CDC_NOTIFY_SERIAL_STATE                     0x20

#define CDC_ACM_CTRL_OVERRUN                        (1 << 6)
#define CDC_ACM_CTRL_PARITY                         (1 << 5)
#define CDC_ACM_CTRL_FRAMING                        (1 << 4)
#define CDC_ACM_CTRL_RI                             (1 << 3)
#define CDC_ACM_CTRL_BRK                            (1 << 2)
#define CDC_ACM_CTRL_DSR                            (1 << 1)
#define CDC_ACM_CTRL_DCD                            (1 << 0)

/* CDC ACM Device parameters */
#define CDC_ACM_VID                                 0x0BDAU
#define CDC_ACM_PID                                 0x4042U
#define CDC_ACM_CONFIG_DESC_SIZE                    67U
#define CDC_ACM_LANGID_STRING                       0x0409U
#define CDC_ACM_MFG_STRING_DESC_SIZE                16U
#define CDC_ACM_PRODUCT_STRING_DESC_SIZE            34U
#define CDC_ACM_SN_STRING_DESC_SIZE                 26U
#define CDC_ACM_SELF_POWERED                        1U

/* CDC ACM Endpoint parameters */
#define CDC_ACM_BULK_IN_EP                          0x81U  /* EP1 for BULK IN */
#define CDC_ACM_BULK_OUT_EP                         0x02U  /* EP2 for BULK OUT */
#define CDC_ACM_INTR_IN_EP                          0x83U  /* EP3 for INTR IN */

#define CDC_ACM_HS_BULK_MAX_PACKET_SIZE             512U   /* High speed BULK IN & OUT packet size */
#define CDC_ACM_FS_BULK_MAX_PACKET_SIZE             64U    /* Full speed BULK IN & OUT packet size */

#define CDC_ACM_HS_BULK_IN_PACKET_SIZE              CDC_ACM_HS_BULK_MAX_PACKET_SIZE
#define CDC_ACM_HS_BULK_OUT_PACKET_SIZE             CDC_ACM_HS_BULK_MAX_PACKET_SIZE

#define CDC_ACM_FS_BULK_IN_PACKET_SIZE              CDC_ACM_FS_BULK_MAX_PACKET_SIZE
#define CDC_ACM_FS_BULK_OUT_PACKET_SIZE             CDC_ACM_FS_BULK_MAX_PACKET_SIZE

#define CDC_ACM_INTR_IN_PACKET_SIZE                 10U    /* INTR IN packet size */
#define CDC_ACM_INTR_IN_REQUEST_SIZE                8U     /* INTR IN request size */
#define CDC_ACM_INTR_IN_DATA_SIZE                   2U     /* INTR IN data size */
#define CDC_ACM_HS_INTR_IN_INTERVAL                 8U    /* High speed INTR IN interval */
#define CDC_ACM_FS_INTR_IN_INTERVAL                 8U    /* Full speed INTR IN interval */

#define CDC_ACM_CTRL_BUF_SIZE                       512U   /* Control buffer size */

#define CDC_ACM_LINE_CODING_SIZE                    7U

/* Exported types ------------------------------------------------------------*/

typedef struct {
	u32 bitrate;
	u8  format;
	u8  parity_type;
	u8  data_type;
} usbd_cdc_acm_line_coding_t;

#if CONFIG_CDC_ACM_NOTIFY
typedef struct {
	u8 bmRequestType;
	u8 bNotificationType;
	u16 wValue;
	u16 wIndex;
	u16 wLength;
	u8 buf[CDC_ACM_INTR_IN_DATA_SIZE];
} __attribute__((packed)) usbd_cdc_acm_ntf_t;
#endif

typedef struct {
	u8(* init)(void);
	u8(* deinit)(void);
	u8(* setup)(u8 cmd, u8 *buf, u16 len, u16 value);
	u8(* receive)(u8 *buf, u32 len);
} usbd_cdc_acm_cb_t;

typedef struct {
	u8  ctrl_req;
	u8  ctrl_data_len;	/* TODO: Remove and get the data length via usbd_core_get_rx_data_size? */

	u8 *bulk_out_buf;
	u32 bulk_out_buf_size;
	u8 bulk_out_zlp;

	u8 *bulk_in_buf;
	u32 bulk_in_buf_size;
	__IO u32 bulk_in_state;

#if CONFIG_CDC_ACM_NOTIFY
	usbd_cdc_acm_ntf_t *intr_in_buf;
	__IO u32 intr_in_state;
#endif

	usbd_cdc_acm_cb_t *cb;
	usb_dev_t *dev;
} usbd_cdc_acm_dev_t;

/* Exported macros -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

u8 usbd_cdc_acm_init(usb_speed_type_t speed, usbd_cdc_acm_cb_t *cb);
u8 usbd_cdc_acm_deinit(void);
u8 usbd_cdc_acm_transmit(u8 *buf, u16 len);
u8 usbd_cdc_acm_receive(void);
#if CONFIG_CDC_ACM_NOTIFY
u8 usbd_cdc_acm_notify_serial_state(u16 serial_state);
#endif
#endif  /* USBD_CDC_ACM_H */

