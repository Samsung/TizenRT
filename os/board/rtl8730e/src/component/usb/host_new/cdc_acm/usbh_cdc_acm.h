/**
  ******************************************************************************
  * The header file for usbh_cdc_acm.c
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef USBH_CDC_ACM_H
#define USBH_CDC_ACM_H

/* Includes ------------------------------------------------------------------*/

#include "usbh.h"

/* Exported defines ----------------------------------------------------------*/

/* CDC Class Codes */
#define CDC_CLASS_CODE                              0x02U
#define CDC_COMM_INTERFACE_CLASS_CODE               0x02U
#define CDC_DATA_INTERFACE_CLASS_CODE               0x0AU

/* CDC Communication sub class codes */
#define CDC_RESERVED                                0x00U
#define CDC_ABSTRACT_CONTROL_MODEL                  0x02U

/* CDC Class-Specific Request Codes */
#define CDC_SEND_ENCAPSULATED_COMMAND               0x00U
#define CDC_GET_ENCAPSULATED_RESPONSE               0x01U
#define CDC_SET_COMM_FEATURE                        0x02U
#define CDC_GET_COMM_FEATURE                        0x03U
#define CDC_CLEAR_COMM_FEATURE                      0x04U
#define CDC_SET_LINE_CODING                         0x20U
#define CDC_GET_LINE_CODING                         0x21U
#define CDC_SET_CONTROL_LINE_STATE                  0x22U
#define CDC_SEND_BREAK                              0x23U

/* Communication Interface Class Control Protocol Codes */
#define CDC_CTRL_PROTOCOL_NO_CLASS_SPECIFIC         0x00U
#define CDC_CTRL_PROTOCOL_COMMON_AT_COMMAND         0x01U
#define CDC_CTRL_PROTOCOL_VENDOR_SPECIFIC           0xFFU

#define CDC_LINE_CODING_DATA_LEN                    0x07U

/* Exported types ------------------------------------------------------------*/

/* States for CDC State Machine */
typedef enum {
	CDC_ACM_TRANSFER_STATE_IDLE = 0U,
	CDC_ACM_TRANSFER_STATE_TX,
	CDC_ACM_TRANSFER_STATE_TX_BUSY,
	CDC_ACM_TRANSFER_STATE_RX,
	CDC_ACM_TRANSFER_STATE_RX_BUSY,
} usbh_cdc_acm_transfer_state_t;

/* CDC ACM state */
typedef enum {
	CDC_ACM_STATE_IDLE = 0U,
	CDC_ACM_STATE_SET_LINE_CODING,
	CDC_ACM_STATE_GET_LINE_CODING,
	CDC_ACM_STATE_TRANSFER,
	CDC_ACM_STATE_ERROR,
} usbh_cdc_acm_state_t;

/* Line coding structure */
typedef union {
	u8 d8[CDC_LINE_CODING_DATA_LEN];
	struct {
		u32 dwDteRate;    /* Data terminal rate, in bits per second */
		u8  bCharFormat;  /* Stop bits */
		u8  bParityType;  /* Parity */
		u8  bDataBits;    /* Data bits (5, 6, 7, 8 or 16) */
	} b;
} usbh_cdc_acm_line_coding_t;

/* Line coding char format */
typedef enum {
	CDC_ACM_LINE_CODING_CHAR_FORMAT_1_STOP_BITS = 0U,
	CDC_ACM_LINE_CODING_CHAR_FORMAT_1_5_STOP_BITS,
	CDC_ACM_LINE_CODING_CHAR_FORMAT_2_STOP_BITS
} usbh_cdc_acm_line_coding_char_format_t;

/* Line coding parity type */
typedef enum {
	CDC_ACM_LINE_CODING_PARITY_NO = 0U,
	CDC_ACM_LINE_CODING_PARITY_ODD,
	CDC_ACM_LINE_CODING_PARITY_EVEN,
	CDC_ACM_LINE_CODING_PARITY_MARK,
	CDC_ACM_LINE_CODING_PARITY_SPACE
} usbh_cdc_acm_line_coding_parity_t;

/* CDC ACM communication interface */
typedef struct {
	u8  intr_in_pipe;
	u8  intr_in_ep;
	u16 intr_in_packet_size;
} usbh_cdc_acm_comm_if_t ;

/* CDC ACM data interface */
typedef struct {
	u8  bulk_in_pipe;
	u8  bulk_in_ep;
	u16 bulk_in_packet_size;
	u8  bulk_out_pipe;
	u8  bulk_out_ep;
	u16 bulk_out_packet_size;
} usbh_cdc_acm_data_if_t ;

/* CDC ACM user callback interface */
typedef struct {
	u8(* init)(void);
	u8(* deinit)(void);
	u8(* attach)(void);
	u8(* detach)(void);
	u8(* setup)(void);
	u8(* receive)(u8 *buf, u32 len);
	u8(* transmit)(usbh_urb_state_t state);
	u8(* line_coding_changed)(usbh_cdc_acm_line_coding_t *line_coding);
} usbh_cdc_acm_cb_t;

/* CDC ACM host */
typedef struct {
	usbh_cdc_acm_comm_if_t        comm_if;
	usbh_cdc_acm_data_if_t        data_if;

	u32                           tx_len;
	u8                            *tx_buf;
	usbh_cdc_acm_transfer_state_t data_tx_state;

	u32                           rx_len;
	u8                            *rx_buf;
	usbh_cdc_acm_transfer_state_t data_rx_state;

	usbh_cdc_acm_line_coding_t    line_coding;
	usbh_cdc_acm_line_coding_t    user_line_coding;

	usbh_cdc_acm_state_t          state;

	usbh_cdc_acm_cb_t             *cb;
	usb_host_t                    *host;
} usbh_cdc_acm_host_t;

/* Exported macros -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

u8 usbh_cdc_acm_init(usbh_cdc_acm_cb_t *cb);
u8 usbh_cdc_acm_deinit(void);

u8 usbh_cdc_acm_set_line_coding(usbh_cdc_acm_line_coding_t *lc);
u8 usbh_cdc_acm_get_line_coding(usbh_cdc_acm_line_coding_t *lc);

u8 usbh_cdc_acm_transmit(u8 *buf, u32 len);
u8 usbh_cdc_acm_receive(u8 *buf, u32 len);

#endif  /* USBD_CDC_ACM_H */

