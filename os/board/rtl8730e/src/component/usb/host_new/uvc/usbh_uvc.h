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

#include "usbh.h"
#include "usbh_core.h"
#include "usbh_uvc_desc.h"
#include "usbh_uvc_intf.h"


/* Exported defines ----------------------------------------------------------*/


#define UVC_MAX_NUM_IN_TERMINAL			5
#define UVC_MAX_NUM_OUT_TERMINAL		2
#define UVC_MAX_NUM_CAMERA_TERMINAL		2
#define UVC_MAX_NUM_PROCESSING_UNIT		2
#define UVC_MAX_NUM_EXTENSION_UNIT		2
#define UVC_MAX_NUM_ENCODING_UNIT		2
#define UVC_MAX_NUM_SELECTOR_UNIT		2


#define USHB_MAX_NUM_INPUT_HEADER 		3
#define USHB_MAX_NUM_MJPEG_FORMAT		10
#define USHB_MAX_NUM_MJPEG_FRAME		10
#define USHB_MAX_NUM_UNCOMP_FORMAT		10
#define USHB_MAX_NUM_UNCOMP_FRAME		10

#define USHB_MAX_NUM_VS_DESC			3

#define USB_EP_XFERTYPE_MASK			0x3U



#define UVC_URB_NUMS		4
#define UVC_URB_SIZE		512*8

#define VIDEO_MAX_FRAME		2
#define VIDEO_FRAME_SIZE	200*1024


/*For Freertos real size is parameter*4 bytes*/
#define UVC_DECODE_TASK_STACK		512
#define UVC_DECODE_TASK_PRIORITY	tskIDLE_PRIORITY + 5




/* Exported types ------------------------------------------------------------*/

typedef struct {
	u8  bFrameIndex;
	u8  bmCapabilities;
	u16 wWidth;
	u16 wHeight;
	u32 dwMinBitRate;
	u32 dwMaxBitRate;
	u32 dwMaxVideoFrameBufferSize;
	u32 dwDefaultFrameInterval;
	u8  bFrameIntervalType;
	u32 *dwFrameInterval;
} vs_frame;


typedef struct {
	u8 type;
	u8 index;
	u8 bpp;
	u8 colorspace;
	u32 fcc;
	u32 flags;
	char name[32];
	u32 nframes;
	vs_frame *frame;
} vs_format;


typedef struct {
	struct list_head stream_list;
	struct list_head format_list;
	u32 intf_num;
	u16 max_packege_size;
	u32 format_num;
	u32 type;
} vs_stream;


typedef struct {
	void *p;
	u8 bInterfaceNumber;
	uvc_vc_header_desc *vcheader;
	struct list_head entity_chain;
	u8 entity_num;
	u8 *intr_ep;
	u8 *cs_intr_desc;
} uvc_vc;


typedef struct {
	uvc_vs_input_header_desc *InputHeader;
	u8 *MJPEGFormat;
	u8 *MJPEGFrame[USHB_MAX_NUM_UNCOMP_FORMAT];
	u8 *UncompFormat;
	u8 *UncompFrame[USHB_MAX_NUM_UNCOMP_FRAME];

	u8 mjpeg_frame_num;
	u8 uncomp_frame_num;

	u32 nformat;
} uvc_vs_format;


typedef struct {
	void *p;
	usbh_ep_desc_t *endpoint;
} uvc_alt;


typedef struct {
	void *p;
	u8 bInterfaceNumber;
	u8 alt_num;
	uvc_alt altsetting[10];
	uvc_vs_input_header_desc *InputHeader;

	u32 nformat;
	vs_format *format;
} uvc_vs;


typedef struct {
	uvc_vc vc_intf;
	uvc_vs vs_intf[3];
	u8 vs_num;
	u8 cur_vs_num;
} uvc_cfg;


typedef struct {
	uvc_alt *altsetting;
	u8 bAlternateSetting;
	uvc_vs *cur_vs_intf;
	u8 bInterfaceNumber;
	u8 pipe;
	u8 ep_addr;
	u8 interval;
	u16 ep_size;
	u8 ep_type;
	u8 valid;
	u8 timer;
} uvc_setting;


typedef struct {
	u32 offset;
	u32 length;
} packet_desc;


typedef struct {
	u8 *addr;
	u8 index;
	u32 packet_num;
	u32 packet_length;
	u32 cur_packet;
	packet_desc packet_info[0];
} uvc_urb;


typedef struct {
	uvc_urb *urb[UVC_URB_NUMS];
	u8 *uvc_buffer;
	u32 urb_buffer_size;
	u32 cur_urb;

	_xqueue urb_wait_queue;
	_xqueue urb_giveback_queue;
	u8 last_fid;

	uvc_stream_control stream_ctrl;

	vs_format *def_format;
	vs_format *cur_format;
	vs_frame *cur_frame;
} uvc_stream;


typedef struct {
	usb_host_t *host;

	uvc_cfg uvc_desc;
	struct list_head entity_list;
	struct list_head video_chain;

	uvc_setting cur_setting;

	uvc_stream stream;
	u8 stream_state;
	u8 stream_data_state;

	uvc_frame frame_buffer[VIDEO_MAX_FRAME];
	u8 *frame_buf;
	u32 frame_buffer_size;
	struct list_head frame_empty;
	struct list_head frame_chain;
	_sema frame_sema;
	u32 frame_cnt;
	u32 err_frame_cnt;

	struct task_struct decode_task;

	usbh_uvc_cb_t *cb;
} usbh_uvc_host_t;


/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/*class module*/
u8 usbh_uvc_class_init(void);

void usbh_uvc_class_deinit(void);


/*stream module*/
int usbh_uvc_process_rx(usb_host_t *host);

uvc_urb *usbh_uvc_urb_complete(uvc_urb *urb);

int usbh_uvc_set_video(uvc_stream_control *ctrl, int probe);

int usbh_uvc_get_video(uvc_stream_control *ctrl, int probe, u16 request);

int usbh_uvc_probe_video(uvc_stream_control *ctrl);

int usbh_uvc_commit_video(uvc_stream_control *ctrl);

int usbh_uvc_video_init(void);

int usbh_uvc_stream_init(void);

void usbh_uvc_stream_deinit(void);


/*parse module*/
u8 usbh_uvc_parse_cfgdesc(usb_host_t *host);

void usbh_uvc_desc_init(void);

void usbh_uvc_desc_free(void);



