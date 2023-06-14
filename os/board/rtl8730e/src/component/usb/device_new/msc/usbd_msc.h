/**
  ******************************************************************************
  * The header file for usbd_msc.c
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef USBD_MSC_H
#define USBD_MSC_H

/* Includes ------------------------------------------------------------------*/

#include "usbd.h"
#if defined(CONFIG_RTL8721D)
#include "rtl8721dhp_sd.h"
#elif defined(CONFIG_AMEBAD2)
#include "ameba_sd.h"
#endif

/* Exported defines ----------------------------------------------------------*/

/* CTRL buffer size */
#define USBD_MSC_CTRL_BUF_SIZE             512U

/* MSC Device parameters */
#define USBD_MSC_VID                                 0x0BDAU
#define USBD_MSC_PID                                 0x8730U
#define USBD_MSC_CONFIG_DESC_SIZE                    32U
#define USBD_MSC_LANGID_STRING                       0x0409U
#define USBD_MSC_MFG_STRING_DESC_SIZE                16U
#define USBD_MSC_PRODUCT_STRING_DESC_SIZE            34U
#define USBD_MSC_SN_STRING_DESC_SIZE                 26U
#define USBD_MSC_SELF_POWERED						1U

/* MSC Endpoint parameters */
#define USBD_MSC_BULK_IN_EP                          0x81U  /* EP1 for BULK IN */
#define USBD_MSC_BULK_OUT_EP                       0x02U  /* EP2 for BULK OUT */
#define USBD_MSC_HS_MAX_PACKET_SIZE             512U   /* High speed BULK IN & OUT packet size */
#define USBD_MSC_FS_MAX_PACKET_SIZE             64U    /* Full speed BULK IN & OUT packet size */

/* MSC configurations */
#define USBD_MSC_RAM_DISK                    0       /* Use RAM as storage media, for test purpose only */

#define USBD_MSC_BLK_BITS                    9
#define USBD_MSC_BLK_SIZE                   (1 << USBD_MSC_BLK_BITS)
#define USBD_MSC_BUFLEN                     (32*512) /* Default size of buffer length */
/* RAM disk configurations */
#if USBD_MSC_RAM_DISK
#define USBD_MSC_RAM_DISK_SIZE              (USBD_MSC_BUFLEN * 2) // Should be >= 32KB
#define USBD_MSC_RAM_DISK_SECTORS           (USBD_MSC_RAM_DISK_SIZE >> USBD_MSC_BLK_BITS)
#endif

/* MSC Request Codes */
#define USBD_MSC_REQUEST_RESET               0xFF
#define USBD_MSC_REQUEST_GET_MAX_LUN         0xFE

/* CBW/CSW configurations */
#define USBD_MSC_CB_WRAP_LEN                         31U
#define USBD_MSC_CB_SIGN                             0x43425355U    /*spells out USBC */
#define USBD_MSC_MAX_DATA                            256U
#define USBD_MSC_CS_WRAP_LEN                         13U
#define USBD_MSC_CS_SIGN                             0x53425355U      /* spells out 'USBS' */

/* CSW Status Definitions */
#define USBD_MSC_CSW_CMD_PASSED                0x00U
#define USBD_MSC_CSW_CMD_FAILED                0x01U
#define USBD_MSC_CSW_PHASE_ERROR               0x02U

/* BOT State */
#define USBD_MSC_IDLE                      0U       /* Idle state */
#define USBD_MSC_DATA_OUT                  1U       /* Data Out state */
#define USBD_MSC_DATA_IN                   2U       /* Data In state */
#define USBD_MSC_LAST_DATA_IN              3U       /* Last Data In Last */
#define USBD_MSC_SEND_DATA                 4U       /* Send Immediate data */
#define USBD_MSC_NO_DATA                   5U       /* No data Stage */

/* BOT Status */
#define USBD_MSC_STATUS_NORMAL             0U
#define USBD_MSC_STATUS_RECOVERY           1U
#define USBD_MSC_STATUS_ERROR              2U

/* Sense */
#define SENSE_LIST_DEEPTH                           4U

typedef struct {
	SD_RESULT(*disk_init)(void);
	SD_RESULT(*disk_deinit)(void);
	SD_RESULT(*disk_getcapacity)(u32 *sectors);
	SD_RESULT(*disk_read)(u32 sector, u8 *buffer, u32 count);
	SD_RESULT(*disk_write)(u32 sector, const u8 *buffer, u32 count);
} usb_msc_opts_t;

/* command block wrapper */
typedef struct {
	u32 Signature;                        /* contains 'USBC', denote bulk_cb_wrap */
	u32 Tag;                              /* unique per command id */
	u32 DataTransferLength;               /* size of data for transfer */
	u8 Flags;                            /* data transfer direction */
	u8 Lun;                              /* LUN normally 0, (which command block is sent) */
	u8 Length;                           /* length of the CDB */
	u8 CDB[16];  /* max command */
} bulk_cb_wrap;

/* command status wrapper */
typedef struct {
	u32 Signature;                      /* should = 'USBS' */
	u32 Tag;                            /* same as original command, echoed by the device as received */
	u32 Residue;                        /* amount not transferred */
	u8 Status;                         /* execute command status */
} bulk_cs_wrap;

typedef struct {
	char Skey;
	union {
		struct _ASCs {
			char ASC;
			char ASCQ;
		} b;
		u8 ASC;
		char *pData;
	} w;
} us_sense_typedef;

typedef struct {
	u8 phase_error;
	u8 is_open;
	u8 ro;
	u8 intf;
	u8 bot_state;
	u8 bot_status;
	u32 data_length;
	u8 *data;
	us_sense_typedef scsi_sense [SENSE_LIST_DEEPTH];
	u8 scsi_sense_head;
	u8 scsi_sense_tail;
	u32 num_sectors;
	u32 lba; // logic block address
	u32 blkbits; /* bits of logical block size of bound block device */
	u32 blksize;
	u32 blklen;
	bulk_cb_wrap cbw;
	bulk_cs_wrap csw;
	usb_msc_opts_t disk_operation;
	usb_dev_t *dev;
} usbd_msc_dev_t;

int usbd_msc_init(void);
void usbd_msc_deinit(void);

#endif // USBD_MSC_H
