/**
  ******************************************************************************
  * The header file for uvc class
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/

/* Exported defines ----------------------------------------------------------*/

#define UVC_GET_FRAME_TIMEOUT		2000   //unit:ms

#define VS_STILL_IMAGE_FRAME		0x3
#define VS_FORMAT_UNCOMPRESSED		0x4
#define VS_FRAME_UNCOMPRESSED		0X5
#define VS_FORMAT_MJPEG				0x6
#define VS_FRAME_MJPEG				0x7
#define VS_COLORFORMAT				0xd
#define VS_FORMAT_FRAME_BASED		0x10
#define VS_FRAME_FRAME_BASED		0x11


/*Supported type*/
#define UVC_FORMAT_MJPEG			VS_FORMAT_MJPEG
#define UVC_FORMAT_UNCOMPRESSED		VS_FORMAT_UNCOMPRESSED

/* Exported types ------------------------------------------------------------*/


enum streaming_state {
	STREAMING_OFF = 0,
	STREAMING_ON = 1,
};

struct uvc_config {
	int fmt_type;    		 //video format type
	int width;               //video frame width
	int height;              //video frame height
	int frame_rate;          //video frame rate
};


typedef struct {
	int(* init)(void);
	int(* deinit)(void);
	int(* attach)(void);
	int(* detach)(void);
} usbh_uvc_cb_t;


typedef struct {
	struct list_head list;
	u8 *buf;
	u32 byteused;
	u32 err;
} uvc_frame;




/* Exported macros -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
int usbh_uvc_init(usbh_uvc_cb_t *cb);

void usbh_uvc_deinit(void);

int usbh_uvc_stream_on(void);

int usbh_uvc_stream_off(void);

int usbh_uvc_stream_state(void);

int usbh_uvc_set_param(struct uvc_config *para);

uvc_frame *usbh_uvc_get_frame(void);

void usbh_uvc_put_frame(uvc_frame *frame);



