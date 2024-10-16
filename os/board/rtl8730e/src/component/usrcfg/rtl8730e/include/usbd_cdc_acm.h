/**
  ******************************************************************************
  * The header file for usbd_cdc_acm.c
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

#define CONFIG_CDC_ACM_NOTIFY                       0

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
#define CDC_ACM_VID                                 USB_VID
#define CDC_ACM_PID                                 USB_PID
#define CDC_ACM_CONFIG_DESC_SIZE                    67U
#define CDC_ACM_SELF_POWERED                        1U
#define CDC_ACM_REMOTE_WAKEUP_EN					1U
#define CDC_ACM_LANGID_STRING                       0x0409U
#define CDC_ACM_MFG_STRING							"Realtek"
#define CDC_ACM_PROD_HS_STRING						"Realtek CDC ACM (HS)"
#define CDC_ACM_PROD_FS_STRING						"Realtek CDC ACM (FS)"
#define CDC_ACM_SN_STRING							"1234567890"

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
#define CDC_ACM_HS_INTR_IN_INTERVAL                 8U     /* High speed INTR IN interval */
#define CDC_ACM_FS_INTR_IN_INTERVAL                 8U     /* Full speed INTR IN interval */

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
	u8(* setup)(usb_setup_req_t *req, u8 *buf);
	u8(* received)(u8 *buf, u32 len);
	void(* transmitted)(u8 status);
	void (*status_changed)(u8 status);
} usbd_cdc_acm_cb_t;

typedef struct {
	usb_setup_req_t ctrl_req;
	usb_dev_t *dev;
	usbd_cdc_acm_cb_t *cb;
#if CONFIG_CDC_ACM_NOTIFY
	usbd_cdc_acm_ntf_t *intr_in_buf;
#endif
	u32 bulk_out_buf_size;
	u32 bulk_in_buf_size;
	u8 *bulk_out_buf;
	u8 *bulk_in_buf;
	u8 *ctrl_buf;
#if CONFIG_CDC_ACM_NOTIFY
	u16 intr_notify_idx;
#endif
	u8 bulk_out_zlp : 1;
	__IO u8 is_bulk_in_busy : 1;
	__IO u8 is_ready : 1;
	__IO u8 bulk_in_state : 1;
#if CONFIG_CDC_ACM_NOTIFY
	__IO u8 is_intr_in_busy : 1;
	__IO u8 intr_in_state : 1;
#endif
} usbd_cdc_acm_dev_t;

/* Exported macros -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

u8 usbd_cdc_acm_init(u16 bulk_out_xfer_size, u16 bulk_in_xfer_size, usbd_cdc_acm_cb_t *cb);
u8 usbd_cdc_acm_deinit(void);
u8 usbd_cdc_acm_transmit(u8 *buf, u16 len);
#if CONFIG_CDC_ACM_NOTIFY
u8 usbd_cdc_acm_notify_serial_state(u16 serial_state);
#endif
#endif  /* USBD_CDC_ACM_H */

