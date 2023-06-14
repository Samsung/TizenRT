/**
  ******************************************************************************
  * @file    usbh_core.h
  * @author  Realsil WLAN5 Team
  * @brief   This file is the header file for usbh_core.c
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef __USBH_CORE_H
#define __USBH_CORE_H

/* Includes ------------------------------------------------------------------*/

#include "usbh_hal.h"
#include "usbh.h"

/* Exported defines ----------------------------------------------------------*/

#define USBH_CONFIG_DESCRIPTOR_MAX_COUNT		2

#define USBH_MAX_DATA_BUFFER					0x200U
#define USBH_MAX_CFG_DATA_BUFFER				0x500U
#define USBH_MAX_ERROR_COUNT					0x02U
#define USBH_MSG_QUEUE_DEPTH					0x10U

#define USBH_ADDRESS_DEFAULT                    0x00U
#define USBH_ADDRESS_ASSIGNED                   0x01U
#define USBH_MPS_DEFAULT                        0x40U

#define USBH_TOKEN_SETUP                        0U
#define USBH_TOKEN_DATA                         1U

#define USBH_SETUP_PKT_SIZE                     8U

/*Standard Feature Selector for clear feature command*/
#define USBH_FEATURE_SELECTOR_ENDPOINT          0x00U
#define USBH_FEATURE_SELECTOR_DEVICE            0x01U
#define USBH_FEATURE_SELECTOR_REMOTEWAKEUP      0X01U

/* Exported types ------------------------------------------------------------*/

/* USB descriptor header */
typedef struct {
	u8  bLength;										/* Descriptor length */
	u8  bDescriptorType;								/* Descriptor type */
} usbh_desc_header_t;

/* USB enumeration state */
typedef enum {
	USBH_ENUM_IDLE = 0U,
	USBH_ENUM_GET_FULL_DEV_DESC,
	USBH_ENUM_SET_ADDR,
	USBH_ENUM_GET_CFG_DESC,
	USBH_ENUM_GET_FULL_CFG_DESC,
	USBH_ENUM_GET_MFC_STRING_DESC,
	USBH_ENUM_GET_PRODUCT_STRING_DESC,
	USBH_ENUM_GET_SERIALNUM_STRING_DESC,
} usbh_enum_state_t;

/* USB control transfer state */
typedef enum {
	USBH_CTRL_IDLE = 0U,
	USBH_CTRL_SETUP,
	USBH_CTRL_SETUP_BUSY,
	USBH_CTRL_DATA_IN,
	USBH_CTRL_DATA_IN_BUSY,
	USBH_CTRL_DATA_OUT,
	USBH_CTRL_DATA_OUT_BUSY,
	USBH_CTRL_STATUS_IN,
	USBH_CTRL_STATUS_IN_BUSY,
	USBH_CTRL_STATUS_OUT,
	USBH_CTRL_STATUS_OUT_BUSY,
	USBH_CTRL_ERROR,
	USBH_CTRL_STALLED,
	USBH_CTRL_COMPLETE
} usbh_ctrl_transfer_state_t;

/* USB control request state */
typedef enum {
	USBH_CTRL_REQ_IDLE = 0U,
	USBH_CTRL_REQ_SEND,
	USBH_CTRL_REQ_BUSY
} usbh_ctrl_req_state_t;

/* USB event */
typedef enum {
	USBH_PORT_EVENT = 1U,
	USBH_URB_EVENT,
	USBH_CONTROL_EVENT,
	USBH_CLASS_EVENT,
	USBH_STATE_CHANGED_EVENT,
} usbh_event_t;

/* USB control request */
typedef struct {
	u8                pipe_in;							/* In pipe */
	u8                pipe_out;							/* Out pipe */
	u8                pipe_size;						/* Pipe size */
	u8                *buf;								/* Request buffer */
	u16               length;							/* Request length */
	u16               timestamp;						/* Request timestamp */
	usbh_setup_req_t  setup;							/* Setup request */
	usbh_ctrl_transfer_state_t state;					/* Control transfer state */
	u8                error_cnt;						/* Error count, USBH_MSG_ERROR will be issued if USBH_MAX_ERROR_COUNT achieved */
} usbh_ctrl_req_t;

/* USB config descriptor struct*/
typedef struct {
	usbh_cfg_desc_t     cfg_desc;							/* Parsed configuration descriptor */
	u8                  cfg_buf[USBH_MAX_CFG_DATA_BUFFER];	/* Raw data for configuration descriptor */
} usbh_cfg_desc_array;

/* USB device */
typedef struct {
	u8              speed;								/* Device speed */
	u8              address;							/* Device address */
	__IO u8         is_connected;						/* Flag indicates whether device is connected */
	u8              port_enabled;						/* Flag indicates whether port is enabled */
	u8              active_if;							/* Active device interface */
	usbh_dev_desc_t dev_desc;							/* Parsed device descriptor */
	u8              desc_buf[USBH_MAX_DATA_BUFFER];		/* Raw buffer for device descriptor */

	u8              cfg_index;							/* index for config_desc, which config is used */
	u8              cfg_count_max;						/* max count for config_desc */
	usbh_cfg_desc_array config_desc[USBH_CONFIG_DESCRIPTOR_MAX_COUNT];
} usbh_dev_t;

/* USB host */
typedef struct {
	/* State parameters */
	usbh_enum_state_t     enum_state;    				/* Enumeration state */
	usbh_ctrl_req_state_t ctrl_req_state;				/* Control request state */
	usbh_ctrl_req_t       ctrl_req;						/* Current control request */

	/* Host parameters */
	u32                   pipes[USBH_MAX_PIPES_NUM];	/* Pipes */
	usbh_dev_t            device;						/* Attached device */
	usb_host_t            *host;						/* Host handler */
	void                  *hcd;							/* HCD handler */

	/* Host process routine parameters */
	_xqueue               msg_queue;					/* Host message queue */
	struct task_struct    main_task;					/* Host main task dealing with host message queue */
	struct task_struct    isr_task;						/* Host ISR task dealing with host interrupts */
} usbh_core_t;

/* Exported macros -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

u8 usbh_core_init(usb_host_t *host);
u8 usbh_core_deinit(usb_host_t *host);
u8 usbh_core_reenumerate(usb_host_t *host);

u8 usbh_core_alloc_pipe(usbh_core_t *core, u8 ep_addr);
u8 usbh_core_free_pipe(usbh_core_t *core, u8 pipe_num);
u8 usbh_core_open_pipe(usbh_core_t *core, u8 pipe_num, u8 ep_num, u8 ep_type, u16 mps);
u8 usbh_core_close_pipe(usbh_core_t *core, u8 pipe_num);
u8 usbh_core_get_eptype(usbh_core_t *core, u8 pipe_num);
u8 usbh_core_reactivate_pipe(usbh_core_t *core, u8 pipe_num);

u8 usbh_core_set_toggle(usbh_core_t *core, u8 pipe_num, u8 toggle);
u8 usbh_core_get_toggle(usbh_core_t *core, u8 pipe_num);

u8 usbh_core_ctrl_set_interface(usbh_core_t *core, u8 ep_num, u8 alt_interface);
u8 usbh_core_ctrl_set_feature(usbh_core_t *core, u8 value);
u8 usbh_core_ctrl_clear_feature(usbh_core_t *core, u8 ep_num);
u8 usbh_core_ctrl_request(usbh_core_t *core, usbh_setup_req_t *req, u8 *buf);

u8 usbh_core_bulk_send_data(usbh_core_t *core, u8 *buf, u16 len, u8 pipe_num);
u8 usbh_core_bulk_receive_data(usbh_core_t *core, u8 *buf, u16 len, u8 pipe_num);
u8 usbh_core_intr_receive_data(usbh_core_t *core, u8 *buf, u8 len, u8 pipe_num);
u8 usbh_core_intr_send_data(usbh_core_t *core, u8 *buf, u8 len, u8 pipe_num);
u8 usbh_core_isoc_receive_data(usbh_core_t *core, u8 *buf, u32 len, u8 pipe_num);
u8 usbh_core_isoc_send_data(usbh_core_t *core, u8 *buf, u32 len, u8 pipe_num);

void usbh_core_notify_host_state_change(usbh_core_t *core);
void usbh_core_notify_port_state_change(usbh_core_t *core);
void usbh_core_notify_class_state_change(usbh_core_t *core);
void usbh_core_notify_urb_state_change(usbh_core_t *core);

usbh_urb_state_t usbh_core_get_urb_state(usbh_core_t *core, u8 pipe_num);
u8 usbh_core_get_active_class(usbh_core_t *core);
u8 usbh_core_set_configuration(usbh_core_t *core, u8 cfg_num);
u8 usbh_core_get_interface(usbh_core_t *core, u8 class_code, u8 sub_class_code, u8 protocol);
u8 usbh_core_set_interface(usbh_core_t *core, u8 if_num);
usbh_if_desc_t *usbh_core_get_interface_descriptor(usbh_core_t *core, u8 if_num);

void usbh_core_disable_port(usbh_core_t *core);
void usbh_core_enable_port(usbh_core_t *core);
u8 usbh_core_is_port_enabled(usbh_core_t *core);

u8 usbh_core_connect(usbh_core_t *core);
u8 usbh_core_disconnect(usbh_core_t *core);

#endif /* __USBD_CORE_H */

