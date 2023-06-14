/**
  ******************************************************************************
  * The header file for usbh_uac.c
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef USBH_UAC_H
#define USBH_UAC_H

/* Includes ------------------------------------------------------------------*/
#include "usbh.h"
#include "usbh_core.h"
#include "usb_ch9.h"
#include "usb_audio.h"

/* Exported defines ----------------------------------------------------------*/
#define USB_OUT (0)
#define USB_IN  (1)
#define USB_MAX (2)

#define ALT_SETTING_MAX (5)
#define FORMAT_FREQ_MAX (6)

/* Exported types ------------------------------------------------------------*/


/* States for class */
typedef enum {
	UAC_STATE_IDLE = 0U,
	UAC_STATE_SETUP_OUT_ALT,
	UAC_STATE_SETUP_OUT_FREQ,
	UAC_STATE_SETUP_IN_ALT,
	UAC_STATE_SETUP_IN_FREQ,
	UAC_STATE_SETUP_VOLUME,
	UAC_STATE_SETUP_MUTE,
	UAC_STATE_TRANSFER,
	UAC_STATE_ERROR
} usbh_uac_state_t;

/* States for transfer */
typedef enum {
	UAC_TRANSFER_STATE_IDLE = 0U,
	UAC_TRANSFER_STATE_PROCESS,
	UAC_TRANSFER_STATE_PROCESS_BUSY,
} usbh_uac_transfer_state_t;

/* Vendor user callback interface */
typedef struct {
	u8(* init)(void);
	u8(* deinit)(void);
	u8(* attach)(void);
	u8(* detach)(void);
	u8(* setup)(void);
	u8(* isoc_transmitted)(usbh_urb_state_t state);
	u8(* isoc_received)(u8 *buf, u32 length);
} usbh_uac_cb_t;


/*uac descriptor struct*/

/*AC:audio control*/
typedef struct {
	void *p;
	u8 bInterfaceNumber;
} uac_ac;

/*AS:audio streaming*/
typedef struct {
	void *p;
	usbh_if_desc_t *desc;
	struct uac1_as_header_descriptor *header;
	struct uac_format_type_i_discrete_descriptor *format;
	struct usb_audio_endpoint_descriptor *audio_ep;
	struct uac_iso_endpoint_descriptor *iso_ep;
} uac_alt;
/*descriptor alt tree*/
typedef struct {
	void *p;
	u8 bInterfaceNumber;
	u8 alt_count;
	u8 msg_type;
	uac_alt altsetting[ALT_SETTING_MAX];//max
} uac_as;

/*descriptor tree*/
typedef struct {
	uac_ac ac_intf;
	uac_as as_intf[USB_MAX];
	u8 as_count;
	u8 cur_as_index[USB_MAX];
} uac_cfg;

typedef struct {
	u8 *isoc_buf;
	u32 isoc_len;
	u8  isoc_pipe;
	u8  isoc_ep_addr;
	u16 isoc_packet_size;
	u32  isoc_interval;
	__IO u32 isoc_tick;
	usbh_uac_transfer_state_t isoc_state;
} uac_ep_cfg;

typedef struct {
	u8 bNrChannels;
	u8 bSubframeSize;
	u8 bSamFreqType;
	u8 format_freq[FORMAT_FREQ_MAX][3];
} uac_format_cfg;

typedef struct {
	uac_alt *cur_altsetting; //alt1/alt2

	u8 bInterfaceNumber;
	u8 bAlternateSetting;
	u8 bFreqId;

	uac_format_cfg format_info;
	uac_ep_cfg ep_info;
} uac_setting;


/* Vendor host */
typedef struct {
	uac_cfg uac_desc;
	uac_setting cur_setting[USB_MAX];

	usbh_uac_state_t state;

	usbh_uac_cb_t *cb;
	usb_host_t *host;
} usbh_uac_host_t;

/* Exported macros -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

u8 usbh_uac_init(usbh_uac_cb_t *cb);
u8 usbh_uac_deinit(void);
u8 usbh_uac_isoc_transmit(u8 *buf, u32 len);
u8 usbh_uac_isoc_receive(u8 *buf, u32 len);

u8 usbh_uac_setup_out(u8 id);
u8 usbh_uac_setup_in(u8 id);


#endif  /* USBH_UAC_H */

