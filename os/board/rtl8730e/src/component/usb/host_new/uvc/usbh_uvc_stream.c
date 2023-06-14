/**
  ******************************************************************************
  * @file    usbh_uvc_stream.c
  * @author  Realsil WLAN5 Team
  * @brief   This file provides the functionalities of the USB UVC Class
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/

#include "usbh_uvc.h"


/* Private defines -----------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

static void usbh_uvc_decode_thread(void *param);
static void usbh_uvc_decode_thread_deinit(void);
static int usbh_uvc_decode_thread_init(void);
static uvc_frame *usbh_uvc_next_frame_buffer(uvc_frame *buf);
static void usbh_uvc_decode_urb(uvc_urb *urb);



/* Private function prototypes -----------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

extern usbh_uvc_host_t uvc_host;

static u32 wait_recv_timeout = 1;
static u32 wait_send_timeout = 0;
static u32 giveback_recv_timeout = 0;
static u32 giveback_send_timeout = 0;
static volatile u32 complete_flag = 0;
static volatile u32 complete_on = 0;

PSRAM_BSS_SECTION
static u8 __attribute__((aligned(64))) video_buffer[VIDEO_MAX_FRAME][VIDEO_FRAME_SIZE];


/* Private functions ---------------------------------------------------------*/


/**
  * @brief	UVC decode thread
  * @param	param: task parameter
  * @retval None
  */
static void usbh_uvc_decode_thread(void *param)
{
	usbh_uvc_host_t *uvc = &uvc_host;
	uvc_urb *urb_tmp;
	UNUSED(param);

	complete_on = 1;
	while (complete_on) {
		if (!usbh_get_status()) {
			//wait to delete task
			while (1);
		}

		if (rtw_pop_from_xqueue(&uvc->stream.urb_wait_queue, (void *)&urb_tmp, wait_recv_timeout) < 0) {
			//rtw_mdelay_os(1);
			continue;
		}

		usbh_uvc_decode_urb(urb_tmp);

		while (rtw_push_to_xqueue(&uvc->stream.urb_giveback_queue, (void *)&urb_tmp, giveback_send_timeout) < 0) {
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Fail to push urb\n");
		}
	}

	rtw_thread_exit();
}



/**
  * @brief	Deint UVC decode thread
  * @param	None
  * @retval None
  */
static void usbh_uvc_decode_thread_deinit(void)
{
	usbh_uvc_host_t *uvc = &uvc_host;

	if (uvc->decode_task.task != NULL) {
		rtw_delete_task(&uvc->decode_task);
	}

	complete_flag = 0;
	complete_on = 0;

	rtw_deinit_xqueue(&uvc->stream.urb_wait_queue);
	rtw_deinit_xqueue(&uvc->stream.urb_giveback_queue);
}

static int usbh_uvc_decode_thread_init(void)
{
	usbh_uvc_host_t *uvc = &uvc_host;
	struct task_struct *task = &uvc->decode_task;
	int i;

	if (rtw_init_xqueue(&uvc->stream.urb_wait_queue, NULL, sizeof(uvc_urb *), UVC_URB_NUMS) < 0) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Init urb_wait_queue fail\n");
		return -1;
	}

	if (rtw_init_xqueue(&uvc->stream.urb_giveback_queue, NULL, sizeof(uvc_urb *), UVC_URB_NUMS) < 0) {
		rtw_deinit_xqueue(&uvc->stream.urb_wait_queue);
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Init urb_giveback_queue fail\n");
		return -1;
	}

	for (i = 1; i < UVC_URB_NUMS; i ++) {
		if (rtw_push_to_xqueue(&uvc->stream.urb_giveback_queue, (void *)&uvc->stream.urb[i], wait_send_timeout) < 0) {
			rtw_deinit_xqueue(&uvc->stream.urb_wait_queue);
			rtw_deinit_xqueue(&uvc->stream.urb_giveback_queue);
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Push to urb_giveback_queue fail\n");
			return -1;
		}
	}

	if (rtw_create_task(task, NULL, UVC_DECODE_TASK_STACK, UVC_DECODE_TASK_PRIORITY, usbh_uvc_decode_thread, NULL) != pdPASS) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Fail to create uvc complete handle thread\n");
		return -1;
	}

	complete_flag = 1;

	return HAL_OK;
}


/**
  * @brief	Get next frame buffer to fill video data
  * @param	buf: the frame which has been filled
  * @retval Pointer of next frame buffer to be filled
  */
static uvc_frame *usbh_uvc_next_frame_buffer(uvc_frame *buf)
{
	usbh_uvc_host_t *uvc = &uvc_host;
	uvc_frame *frame_buffer;

	if (buf->err == 1) {
		buf->err = 0;
		buf->byteused = 0;
		uvc->err_frame_cnt ++;
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "Drop error frame\n");
		return buf;
	}

	list_del_init(&buf->list);
	list_add(&buf->list, &uvc->frame_chain);

	rtw_up_sema(&uvc->frame_sema);

	if (!list_empty(&uvc->frame_empty)) {
		frame_buffer = list_first_entry(&uvc->frame_empty, uvc_frame, list);
	} else {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "No free uvc buffer 3\n");
		return NULL;
	}

	frame_buffer->byteused = 0;
	uvc->frame_cnt ++;

	return frame_buffer;
}



/**
  * @brief	Decode a given URB
  * @param  urb: URB to decode
  * @retval None
  */
static void usbh_uvc_decode_urb(uvc_urb *urb)
{
	int i;
	u32 length;
	usbh_uvc_host_t *uvc = &uvc_host;
	vs_payload_header *header;
	uvc_frame *frame_buffer;
	u8 *data;
	u8 *payload;
	u8 fid, eof;
	u32 bytes, maxlen;

	if (!list_empty(&uvc->frame_empty)) {
		frame_buffer = list_first_entry(&uvc->frame_empty, uvc_frame, list);
	} else {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "No free uvc buffer\n");
		return;
	}

	for (i = 0; i < (int)urb->packet_num; i ++) {

		data = urb->addr + urb->packet_info[i].offset;
		length = urb->packet_info[i].length;
		payload = data + data[0];

		header = (vs_payload_header *)data;

		if (length < header->bHeaderLength) {
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Error: payload length(%d) < header length(%d)\n", length, header->bHeaderLength);
			return;
		}

		if (header->bmHeaderInfo.b.err == 1) {
			//todo: send request to get reason
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "bmHeaderInfo.b.err = 1, drop this packet\n");
			frame_buffer->err = 1;
			continue;
		}

		fid = header->bmHeaderInfo.b.fid;
		eof = header->bmHeaderInfo.b.eof;

		if (fid != uvc->stream.last_fid) {
			if (frame_buffer->byteused != 0) {
				frame_buffer = usbh_uvc_next_frame_buffer(frame_buffer);
				if (!frame_buffer) {
					DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "No free uvc buffer 1\n");
					return ;
				}
			}
			uvc->stream.last_fid = fid;
		}

		maxlen = uvc->frame_buffer_size - frame_buffer->byteused;
		bytes = MIN(maxlen, (length - data[0]));

		if (bytes != 0) {
			rtw_memcpy(frame_buffer->buf + frame_buffer->byteused, payload, bytes);
			frame_buffer->byteused += bytes;
		}

		if (eof == 1) {
			frame_buffer = usbh_uvc_next_frame_buffer(frame_buffer);
			if (!frame_buffer) {
				DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "No free uvc buffer 2\n");
				return;
			}
			uvc->stream.last_fid = fid;
		}
	}

}


/* Exported functions --------------------------------------------------------*/



/**
  * @brief	Process received rx video data
  * @param	None
  * @retval Status
  */
int usbh_uvc_process_rx(usb_host_t *host)
{
	usbh_uvc_host_t *uvc = &uvc_host;
	u8 urb_state;
	u32 *urb_index;
	u32 *packet_index;
	u8 *addr;
	u32 rx_len;
	uvc_urb *urb;

	urb_state = usbh_get_urb_state(host, uvc->cur_setting.pipe);

	//todo: if interval need to be checked? If yes, sof interrupt should enable when open isoc pipe
	//if((urb_state == USBH_URB_DONE) && ((host->tick - uvc->cur_setting.timer) >= uvc->cur_setting.interval)) {
	if (urb_state == USBH_URB_DONE) {

		//uvc->cur_setting.timer = host->tick;
		rx_len = usbh_get_last_transfer_size(host, uvc->cur_setting.pipe);

		urb_index = &uvc->stream.cur_urb;
		packet_index = &uvc->stream.urb[*urb_index]->cur_packet;

		/* some camera may send zero length packet, just drop it */
		if (rx_len == 0) {
			goto isoc_recv;
		}

		uvc->stream.urb[*urb_index]->packet_info[*packet_index].length = rx_len;
		(*packet_index) ++;

		if (*packet_index == uvc->stream.urb[*urb_index]->packet_num) {
			*packet_index = 0;
			urb = usbh_uvc_urb_complete(uvc->stream.urb[*urb_index]);
			if (!urb) {
				DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "URB complete fail\n");
				return HAL_TIMEOUT;
			}

			*urb_index = urb->index;
			packet_index = &uvc->stream.urb[*urb_index]->cur_packet;
			*packet_index = 0;
		}

isoc_recv:
		addr = uvc->stream.urb[*urb_index]->addr + uvc->stream.urb[*urb_index]->packet_info[*packet_index].offset;
		usbh_isoc_receive_data(host,
							   addr,                //todo_eric: static or dynamic buffer
							   uvc->cur_setting.ep_size,
							   uvc->cur_setting.pipe);

		uvc->stream_data_state = STREAM_DATA_IN;

	} else {

		if (uvc->stream_state == STREAMING_ON) {
			usbh_notify_class_state_change(uvc->host);
		}
	}

	return HAL_OK;

}



/**
  * @brief	Push an completed URB to decode thread
  * @param	urb: the completed URB
  * @retval URB has been decoded and returned from decode thread
  */
uvc_urb *usbh_uvc_urb_complete(uvc_urb *urb)
{
	uvc_urb *urb_tmp = urb;
	usbh_uvc_host_t *uvc = &uvc_host;

	if (!usbh_get_status()) {
		return NULL;
	}

	if (!complete_flag) {
		return NULL;
	}
	if (rtw_push_to_xqueue(&uvc->stream.urb_wait_queue, (void *)&urb_tmp, wait_send_timeout) < 0) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Fail to push urb\n");
		return NULL;
	}

	urb_tmp = NULL;

	if (rtw_pop_from_xqueue(&uvc->stream.urb_giveback_queue, (void *)&urb_tmp, giveback_recv_timeout) == 0) {
		if (!urb_tmp) {
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Fail to pop urb\n");
		}
	}

	rtw_mdelay_os(1);

	return urb_tmp;
}




/**
  * @brief	Send SET_CUR request
  * @param	ctrl: uvc streaming contorl data
  			probe: indicate probe or commit
  * @retval Status
  */
int usbh_uvc_set_video(uvc_stream_control *ctrl, int probe)
{
	int ret;
	u16 size;
	u8 *data;
	usbh_uvc_host_t *uvc = &uvc_host;
	usb_host_t *host = uvc->host;
	u8 *ptr = (u8 *)uvc->uvc_desc.vc_intf.vcheader;
	usbh_setup_req_t setup;

	/*note that for uvc1.5 wLength=48, not support now*/
	size = ((ptr[3] | ptr[4] << 8) >= 0x0110) ? 34 : 26;

	data = rtw_malloc(size);
	rtw_memcpy(data, (void *) ctrl, size);

	setup.b.bmRequestType = USB_H2D | USB_REQ_RECIPIENT_INTERFACE | \
							USB_REQ_TYPE_CLASS;
	setup.b.bRequest = UVC_SET_CUR;

	if (probe) {
		setup.b.wValue = VS_PROBE_CONTROL;
	} else {
		setup.b.wValue = VS_COMMIT_CONTROL;
	}

	setup.b.wLength = size;
	setup.b.wIndex = uvc->cur_setting.bInterfaceNumber;

	do {
		ret = usbh_ctrl_request(host, &setup, data);
	} while (ret == HAL_BUSY);

	rtw_free(data);

	return HAL_OK;
}



/**
  * @brief	Send GET request, such as GET_CUR, GET_DEF
  * @param	ctrl: uvc streaming contorl data
  			probe: indicate probe or commit
  			request: indicate which request
  * @retval Status
  */
int usbh_uvc_get_video(uvc_stream_control *ctrl, int probe, u16 request)
{
	int ret;
	u16 size;
	u8 *data;
	usbh_uvc_host_t *uvc = &uvc_host;
	usb_host_t *host = uvc->host;
	usbh_setup_req_t setup;

	/*note that for uvc1.5 wLength=48, not support now*/
	size = (uvc->uvc_desc.vc_intf.vcheader->bcdUVC >= 0x110) ? 34 : 26;

	data = rtw_malloc(size);
	rtw_memcpy(data, (void *) ctrl, size);

	setup.b.bmRequestType = USB_D2H | USB_REQ_RECIPIENT_INTERFACE | \
							USB_REQ_TYPE_CLASS;
	setup.b.bRequest = request;

	if (probe) {
		setup.b.wValue = VS_PROBE_CONTROL;
	} else {
		setup.b.wValue = VS_COMMIT_CONTROL;
	}

	setup.b.wLength = size;
	setup.b.wIndex = uvc->cur_setting.bInterfaceNumber;

	do {
		ret = usbh_ctrl_request(host, &setup, data);
	} while (ret == HAL_BUSY);

	/*todo: Do we need check data here because some broken devices may return wrong
	 dwMaxVideoFrameSize and dwMaxPayloadTransferSize ?*/

	rtw_memcpy((void *) ctrl, (void *)data, size);
	rtw_free(data);

	return HAL_OK;

}


/**
  * @brief	Probe video process
  * @param	ctrl: uvc streaming contorl data
  * @retval Status
  */
int usbh_uvc_probe_video(uvc_stream_control *ctrl)
{
	/*probe video setting*/
	usbh_uvc_set_video(ctrl, 1);
	usbh_uvc_get_video(ctrl, 1, UVC_GET_CUR);
	usbh_uvc_set_video(ctrl, 1);

	return HAL_OK;
}



/**
  * @brief	Commit video process
  * @param	ctrl: uvc streaming contorl data
  * @retval Status
  */
int usbh_uvc_commit_video(uvc_stream_control *ctrl)
{
	/*commit video setting*/
	usbh_uvc_set_video(ctrl, 0);

	return HAL_OK;
}



/**
  * @brief	Init video data
  * @param	None
  * @retval Status
  */
int usbh_uvc_video_init(void)
{
	int i;
	usbh_uvc_host_t *uvc = &uvc_host;
	uvc_stream_control *ctrl = &uvc->stream.stream_ctrl;
	u8 cur_vs = uvc->uvc_desc.cur_vs_num;
	uvc_vs *vs =  &uvc->uvc_desc.vs_intf[cur_vs];
	u32 num_format = vs->nformat;
	u32 num_frame;
	vs_format *format = NULL;
	vs_frame *frame = NULL;

	/*get default video setting*/
	usbh_uvc_get_video(ctrl, 1, UVC_GET_DEF);

	/*todo: should check video setting here?*/

	/*set defalt video setting*/
	usbh_uvc_set_video(ctrl, 1);

	/*get current video setting*/
	usbh_uvc_get_video(ctrl, 1, UVC_GET_CUR);

	/*select matched format */
	for (i = 0; i < (int)num_format; i ++) {
		format = &vs->format[i];

		if (format->index == ctrl->bFormatIndex) {
			break;
		}
	}

	if (format != NULL) {
		/*select matched frame */
		num_frame = format->nframes;
		for (i = 0; i < (int)num_frame; i ++) {

			frame = &format->frame[i];
			if (frame->bFrameIndex == ctrl->bFrameIndex) {
				break;
			}
		}

		uvc->stream.def_format = format;
		uvc->stream.cur_format = format;

		if (frame != NULL) {
			uvc->stream.cur_frame = frame;
		}
	}

	return HAL_OK;
}



/**
  * @brief  Init streaming data
  * @param	None
  * @retval Status
  */
int usbh_uvc_stream_init(void)
{
	int i, j;
	u32 maxpktsize, npkt;
	usbh_uvc_host_t *uvc = &uvc_host;

	rtw_init_sema(&uvc->frame_sema, 0);

	/*init frame buffer*/
	INIT_LIST_HEAD(&uvc->frame_chain);
	INIT_LIST_HEAD(&uvc->frame_empty);
	uvc->frame_buf = (u8 *)video_buffer;
	uvc->frame_buffer_size = VIDEO_FRAME_SIZE;

	for (i = 0; i < VIDEO_MAX_FRAME; i++) {
		uvc->frame_buffer[i].buf = uvc->frame_buf + i * VIDEO_FRAME_SIZE ;
		uvc->frame_buffer[i].byteused = 0;
		INIT_LIST_HEAD(&uvc->frame_buffer[i].list);
		list_add(&uvc->frame_buffer[i].list, &uvc->frame_empty);
	}

	/*init urb*/
	uvc->stream.uvc_buffer = (u8 *)rtw_malloc(UVC_URB_NUMS * UVC_URB_SIZE);
	uvc->stream.urb_buffer_size =  UVC_URB_SIZE;

	maxpktsize = uvc->cur_setting.ep_size;
	if (maxpktsize & 0x3) {
		maxpktsize &= ~0x3;   // for aligned
		maxpktsize += 4;
	}

	npkt = uvc->stream.urb_buffer_size / maxpktsize;

	for (i = 0; i < UVC_URB_NUMS; i ++) {
		uvc->stream.urb[i] = (uvc_urb *) rtw_malloc(sizeof(uvc_urb) + npkt * sizeof(packet_desc));
		uvc->stream.urb[i]->addr = uvc->stream.uvc_buffer + i * UVC_URB_SIZE;
		uvc->stream.urb[i]->packet_num = npkt;
		uvc->stream.urb[i]->index = i;
		uvc->stream.urb[i]->packet_length = maxpktsize;
		uvc->stream.urb[i]->cur_packet = 0;

		for (j = 0; j < (int)npkt; j ++) {
			uvc->stream.urb[i]->packet_info[j].length = maxpktsize;
			uvc->stream.urb[i]->packet_info[j].offset = j * maxpktsize;
		}
	}

	uvc->stream.cur_urb = 0;

	usbh_uvc_decode_thread_init();

	return HAL_OK;
}


/**
  * @brief  Deinit streaming data
  * @param	None
  * @retval Status
  */
void usbh_uvc_stream_deinit(void)
{
	usbh_uvc_host_t *uvc = &uvc_host;
	int i;

	usbh_uvc_decode_thread_deinit();

	INIT_LIST_HEAD(&uvc->frame_chain);
	INIT_LIST_HEAD(&uvc->frame_empty);

	for (i = 0; i < VIDEO_MAX_FRAME; i++) {
		INIT_LIST_HEAD(&uvc->frame_buffer[i].list);
		uvc->frame_buffer[i].byteused = 0;
		uvc->frame_buffer[i].buf = NULL;
	}

	uvc->stream.cur_urb = 0;
	uvc->stream.last_fid = 0;

	rtw_free(uvc->frame_buf);

	for (i = 0; i < UVC_URB_NUMS; i++) {
		uvc->stream.urb[i]->addr = NULL;
		uvc->stream.urb[i]->packet_num = 0;
		uvc->stream.urb[i]->index = 0;
		uvc->stream.urb[i]->packet_length = 0;
		rtw_free(uvc->stream.urb[i]);
	}

	rtw_free(uvc->stream.uvc_buffer);
	uvc->stream.urb_buffer_size = 0;

	rtw_free_sema(&uvc->frame_sema);

}




