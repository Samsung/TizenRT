/**
  ******************************************************************************
  * The header file for usbh_msc.c
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef USBH_MSC_H
#define USBH_MSC_H

/* Includes ------------------------------------------------------------------*/

#include "usbh.h"

/* Exported defines ----------------------------------------------------------*/

/* MSC Class Request Codes */
#define USBH_MSC_BOT_RESET                              0xFFU
#define USBH_MSC_GET_MAX_LUN                            0xFEU

/* MSC Class Codes */
#define MSC_CLASS_CODE                                  0x08U

/* MSC Protocol */
#define USBH_MSC_BOT                                        0x50U
#define USBH_MSC_TRANSPARENT                                0x06U

/* Bulk Only Transport: CBW & CSW */
#define USBH_BOT_CBW_SIGNATURE            0x43425355U
#define USBH_BOT_CBW_TAG                  0x20304050U
#define USBH_BOT_CSW_SIGNATURE            0x53425355U
#define USBH_BOT_CBW_LENGTH               31U
#define USBH_BOT_CSW_LENGTH               13U

#define USBH_MSC_MAX_LUN       1U

/* Exported types ------------------------------------------------------------*/

typedef enum {
	BOT_CMD_IDLE  = 0,
	BOT_CMD_SEND,
	BOT_CMD_BUSY,
} usbh_bot_cmd_state_t;

/* CSW Status Definitions */
typedef enum {
	BOT_CSW_CMD_PASSED   =        0x00,
	BOT_CSW_CMD_FAILED   =        0x01,
	BOT_CSW_PHASE_ERROR  =        0x02,
} usbh_bot_csw_state_t;

typedef enum {
	BOT_SEND_CBW  = 1,
	BOT_SEND_CBW_WAIT,
	BOT_DATA_IN,
	BOT_DATA_IN_WAIT,
	BOT_DATA_OUT,
	BOT_DATA_OUT_WAIT,
	BOT_RECEIVE_CSW,
	BOT_RECEIVE_CSW_WAIT,
	BOT_ERROR_IN,
	BOT_ERROR_OUT,
	BOT_UNRECOVERED_ERROR
} usbh_bot_state_t;

typedef enum {
	MSC_INIT = 0,
	MSC_IDLE,
	MSC_TEST_UNIT_READY,
	MSC_READ_CAPACITY10,
	MSC_READ_INQUIRY,
	MSC_REQUEST_SENSE,
	MSC_READ,
	MSC_WRITE,
	MSC_UNRECOVERED_ERROR,
	MSC_PERIODIC_CHECK,
} usbh_msc_state_t;

typedef enum {
	MSC_OK,
	MSC_NOT_READY,
	MSC_ERROR,
} usbh_msc_error_t;

typedef enum {
	MSC_REQ_IDLE = 0,
	MSC_REQ_RESET,
	MSC_REQ_GET_MAX_LUN,
	MSC_REQ_ERROR,
} usbh_msc_req_state_t;

typedef union {
	struct {
		u32 Signature;
		u32 Tag;
		u32 DataTransferLength;
		u8  Flags;
		u8  LUN;
		u8  CBLength;
		u8  CB[16];
	} field;
	u8 data[31];
} usbh_bot_cbw_t;

typedef union {
	struct {
		u32 Signature;
		u32 Tag;
		u32 DataResidue;
		u8  Status;
	} field;
	u8 data[13];
} usbh_bot_csw_t;

typedef struct {
	u8 data[64];
	usbh_bot_state_t state;
	usbh_bot_cmd_state_t cmd_state;
	usbh_bot_cbw_t cbw;
	usbh_bot_csw_t csw;
	u8 *pbuf;
} usbh_bot_handle_t;

/* Capacity data */
typedef struct {
	u32 block_nbr;
	u16 block_size;
} usbh_scsi_capacity_t;

/* Sense data */
typedef struct {
	u8 key;
	u8 asc;
	u8 ascq;
} usbh_scsi_sense_t;

/* INQUIRY data */
typedef struct {
	u8 PeripheralQualifier;
	u8 DeviceType;
	u8 RemovableMedia;
	u8 vendor_id[8];
	u8 product_id[16];
	u8 revision_id[4];
} usbh_scsi_inquiry_t;

/* Structure for LUN */
typedef struct {
	usbh_msc_state_t state;
	usbh_msc_error_t error;
	HAL_Status prev_ready_state;
	usbh_scsi_capacity_t capacity;
	usbh_scsi_sense_t sense;
	usbh_scsi_inquiry_t inquiry;
	u8 state_changed;
} usbh_msc_lun_t;

/* MSC user callback interface */
typedef struct {
	u8(* attach)(void);
	u8(* detach)(void);
	u8(* setup)(void);
} usbh_msc_cb_t;

/* MSC host */
typedef struct {
	u8  bulk_in_pipe;
	u8  bulk_out_pipe;
	u8  bulk_out_ep;
	u8  bulk_in_ep;
	u16 bulk_out_packet_size;
	u16 bulk_in_packet_size;
	usbh_msc_error_t error;
	usbh_msc_req_state_t req_state;
	usbh_bot_handle_t hbot;
	usbh_msc_lun_t unit[USBH_MSC_MAX_LUN];
	u16 current_lun;
	u32 max_lun;
	usbh_msc_state_t state;
	usbh_msc_cb_t *cb;
	usb_host_t *host;
	u32 tick;
} usbh_msc_host_t;

/* Exported macros -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

u8 usbh_msc_init(usbh_msc_cb_t *cb);
u8 usbh_msc_deinit(void);
u8 usbh_msc_bot_process(usb_host_t *host, u8 lun);
u8  usbh_msc_is_rdy(void);
u8  usbh_msc_get_max_lun(void);
u8  usbh_msc_unit_is_rdy(u8 lun);
u8 usbh_msc_get_lun_info(u8 lun, usbh_msc_lun_t *info);
u8 usbh_msc_read(u8 lun, u32 address, u8 *pbuf, u32 length);
u8 usbh_msc_write(u8 lun, u32 address, u8 *pbuf, u32 length);

#endif  /* USBH_MSC_H */

