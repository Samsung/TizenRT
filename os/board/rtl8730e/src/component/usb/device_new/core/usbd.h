/**
  ******************************************************************************
  * @file    usbd.h
  * @author  Realsil WLAN5 Team
  * @brief   This file provides the API for USB device library
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef USBD_H
#define USBD_H

/* Includes ------------------------------------------------------------------*/

#include "ameba_soc.h"
#include "usb_ch9.h"
#include "usb_os.h"

/* Exported defines ----------------------------------------------------------*/

/* USB device configurations */
#define USBD_MAX_NUM_INTERFACES			16U
#define USBD_MAX_NUM_CONFIGURATION		16U
#define USBD_MAX_ENDPOINTS				5U
#define USBD_MAX_RX_FIFO_SIZE			512U
#define USBD_MAX_NPTX_FIFO_SIZE			256U
#define USBD_MAX_PTX_FIFO_SIZE			256U

/* USB device string descriptor index */
#define USBD_IDX_LANGID_STR				0x00U
#define USBD_IDX_MFC_STR				0x01U
#define USBD_IDX_PRODUCT_STR			0x02U
#define USBD_IDX_SERIAL_STR				0x03U
#define USBD_IDX_CONFIG_STR				0x04U
#define USBD_IDX_INTERFACE_STR			0x05U

/* Exported types ------------------------------------------------------------*/

/* USB device state */
typedef enum {
	USBD_STATE_INIT       = 0U,
	USBD_STATE_DEFAULT    = 1U,
	USBD_STATE_ADDRESSED  = 2U,
	USBD_STATE_CONFIGURED = 3U,
	USBD_STATE_SUSPENDED  = 4U,
} usbd_state_t;

/* USB device attach status */
typedef enum {
	USBD_ATTACH_STATUS_INIT     = 0U,
	USBD_ATTACH_STATUS_ATTACHED = 1U,
	USBD_ATTACH_STATUS_DETACHED = 2U
} usbd_attach_status_t;

/* USB configuration structure */
typedef struct {
	u8 max_ep_num;			/* Max endpoint number used, 1~5, EP0-INOUT, EP1-IN, EP2-OUT, EP3-IN, EP4-OUT */
	u32 rx_fifo_size;		/* RX FIFO size */
	u32 nptx_fifo_size;		/* Non-Periodical TX FIFO size */
	u32 ptx_fifo_size;		/* Periodical TX FIFO size */
	u8 speed;				/* USB speed:
							   USB_SPEED_HIGH: USB 2.0 PHY, e.g. AmebaD/AmebaSmart
							   USB_SPEED_HIGH_IN_FULL: USB 2.0 PHY in full speed mode, e.g. AmebaD/AmebaSmart
							   USB_SPEED_FULL: USB 1.1 transceiver, e.g. AmebaDPlus */
	u8 dma_enable;			/* Enable USB internal DMA mode, 0-Disable, 1-Enable */
	u8 self_powered;		/* Self powered or not, 0-bus powered, 1-self powered */
	u8 isr_priority;		/* USB ISR thread priority */
	u32 nptx_max_err_cnt[USBD_MAX_ENDPOINTS]; /* Max Non-Periodical TX transfer error count allowed, if transfer
							   error count is higher than this value, the transfer status will be determined as failed */
} usbd_config_t;

struct _usbd_class_driver_t;

/* USB device */
typedef struct {
	u8 dev_config;							/* Device config index */
	u8 dev_speed;							/* Device speed, usb_speed_type_t */
	u8 dev_state;							/* Device state, usbd_state_t */
	u8 dev_old_state;						/* Device old state, usbd_state_t */
	u8 dev_attach_status;					/* Device attach status, usbd_attach_status_t */

	u8 ep0_state;							/* EP0 state */
	u16 ep0_data_len;						/* EP0 data length */
	u32 ep0_xfer_total_len;					/* The total data length to transfer */
	u32 ep0_xfer_rem_len;					/* The remain data length to transfer */
	u32 ep0_recv_rem_len;					/* The remain data length to receive */

	u8 test_mode;							/* Test mode */
	u8 remote_wakeup;						/* Remote wakeup */
	u8 alt_setting;							/* Interface alternate setting */

	u8 *ctrl_buf;							/* Buffer for control transfer */

	void *pcd;								/* PCD handle */
	struct _usbd_class_driver_t *driver;	/* Class driver */
} usb_dev_t;

/* USB class driver */
typedef struct _usbd_class_driver_t {
	u8 *(*get_descriptor)(usb_setup_req_t *req, usb_speed_type_t speed, u16 *len);

	u8(*set_config)(usb_dev_t *dev, u8 config);
	u8(*clear_config)(usb_dev_t *dev, u8 config);
	u8(*setup)(usb_dev_t *dev, usb_setup_req_t  *req);
	u8(*get_class_descriptor)(usb_dev_t *dev, usb_setup_req_t  *req);
	u8(*clear_feature)(usb_dev_t *dev, usb_setup_req_t  *req);

	u8(*sof)(usb_dev_t *dev);
	u8(*suspend)(usb_dev_t *dev);
	u8(*resume)(usb_dev_t *dev);

	u8(*ep0_data_in)(usb_dev_t *dev, u8 status);
	u8(*ep0_data_out)(usb_dev_t *dev);

	u8(*ep_data_in)(usb_dev_t *dev, u8 ep_num, u8 status);
	u8(*ep_data_out)(usb_dev_t *dev, u8 ep_num, u16 len);
} usbd_class_driver_t;

/* Exported macros -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/* API for application */
u8 usbd_init(usbd_config_t *cfg);
u8 usbd_deinit(void);
u8 usbd_get_status(void);

/* API for class */
u8 usbd_register_class(usbd_class_driver_t *driver);
u8 usbd_unregister_class(void);
u8 usbd_ep_init(usb_dev_t *dev, u8 ep_addr, u8 ep_type, u16 ep_mps);
u8 usbd_ep_deinit(usb_dev_t *dev, u8 ep_addr);
u8 usbd_ep_transmit(usb_dev_t *dev, u8 ep_addr, u8 *buf, u16  len);
u8 usbd_ep_receive(usb_dev_t *dev, u8 ep_addr, u8 *buf, u16  len);
u8 usbd_ep_set_stall(usb_dev_t *dev, u8 ep_addr);
u8 usbd_ep_clear_stall(usb_dev_t *dev, u8 ep_addr);
u8 usbd_ep_is_stall(usb_dev_t *dev, u8 ep_addr);
u8 usbd_ep0_transmit(usb_dev_t *dev, u8 *buf, u16 len);
u8 usbd_ep0_receive(usb_dev_t *dev, u8 *buf, u16 len);
u8 usbd_ep0_transmit_status(usb_dev_t *dev);
u8 usbd_ep0_receive_status(usb_dev_t *dev);
u8 usbd_ep0_set_stall(usb_dev_t *dev);

#endif /* USBD_H */

