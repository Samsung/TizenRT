/**
  ******************************************************************************
  * @file    usbh_uvc_intf.c
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

/* Private function prototypes -----------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

usbh_uvc_host_t uvc_host;

/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Find suitable format and frame index
  * @param  context: user configuration
  			format_index: pointer to format index
  			frame_index: pointer to frame index
  * @retval Status
  */
static int usbh_uvc_find_format_frame(struct uvc_config *context,
									  u32 *format_index, u32 *frame_index)
{
	u32 type, h, w, nformat, nframe, tmp1, tmp2;
	int i;
	int found = 0;
	usbh_uvc_host_t *uvc = &uvc_host;
	uvc_vs *vs = &uvc->uvc_desc.vs_intf[uvc->uvc_desc.cur_vs_num];
	vs_format *format = vs->format;
	vs_frame *frame;
	nformat = vs->nformat;

	type = context->fmt_type;
	h = context->height;
	w = context->width;

	for (i = 0; i < (int) nformat; i ++) {
		if ((format + i)->type == type) {
			format += i;
			*format_index = format->index;
			found = 1;
			break;
		}
	}

	if (found == 0) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Fail to find format\n");
		return HAL_ERR_PARA;
	}

	found = 0;
	nframe = format->nframes;
	frame = format->frame;


	for (i = 0; i < (int) nframe; i ++) {
		if (((frame + i)->wWidth == w) && ((frame + i)->wHeight == h)) {
			frame += i;
			*frame_index = frame->bFrameIndex;
			found = 1;
			break;
		}
	}

	if (found == 0) {
		*frame_index = 1;
		tmp1 = 0;
		for (i = 0; i < (int) nframe; i ++) {
			tmp2 = (frame + i)->wWidth * (frame + i)->wHeight;
			if ((tmp2 < h * w) && (tmp2 > tmp1)) {
				tmp1 = (frame + i)->wWidth * (frame + i)->wHeight;
				frame += i;
				*frame_index = frame->bFrameIndex;
			}
		}
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "Fail to find the passed_in frame @%d*%d\n", w, h);

		context->height = frame->wHeight;
		context->width = frame->wWidth;

		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "Use closest frame @%d*%d\n", frame->wWidth, frame->wHeight);
	}

	return HAL_OK;
}


/**
  * @brief	Find suitable frame rate
  * @param	context: user configuration
  			intv: pointer to bInterval value
			format_index: pointer to format index
			frame_index: pointer to frame index
  * @retval None
  */
static void usbh_uvc_find_frame_rate(struct uvc_config *context, u32 *intv,
									 u32 *format_index, u32 *frame_index)

{
	u32 fps = 10000000 / context->frame_rate;   //unit: us
	int i;

	u32 best = UINT_MAX;
	u32 min, max, step, dist;
	usbh_uvc_host_t *uvc = &uvc_host;
	uvc_vs *vs = &uvc->uvc_desc.vs_intf[uvc->uvc_desc.cur_vs_num];
	vs_format *format = vs->format + *format_index - 1;
	vs_frame *frame = format->frame + *frame_index - 1;

	if (frame->bFrameIntervalType) {
		/*discrete frame interval*/
		for (i = 0; i < frame->bFrameIntervalType; ++i) {
			dist = fps > frame->dwFrameInterval[i]
				   ? fps - frame->dwFrameInterval[i]
				   : frame->dwFrameInterval[i] - fps;

			if (dist > best) {
				break;
			}

			best = dist;
		}

		fps = frame->dwFrameInterval[i - 1];
	} else {
		/*continuous frame interval*/
		min = frame->dwFrameInterval[0];
		max = frame->dwFrameInterval[1];
		step = frame->dwFrameInterval[2];
		fps = min + (fps - min + step / 2) / step * step;

		if (fps > max) {
			fps = max;
		}
	}

	context->frame_rate = 10000000 / fps;
	*intv = fps;

}


/* Exported functions --------------------------------------------------------*/



/**
  * @brief	Init host video class
  * @param	cb: user callback function
  * @retval Status
  */
int usbh_uvc_init(usbh_uvc_cb_t *cb)
{
	int ret;
	usbh_uvc_host_t *uvc = &uvc_host;

	rtw_memset(uvc, 0, sizeof(usbh_uvc_host_t));

	usbh_uvc_desc_init();

	if (cb != NULL) {
		uvc->cb = cb;
		if (cb->init != NULL) {
			ret = cb->init();
			if (ret != HAL_OK) {
				DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "uvc user cb init error: %d", ret);
				goto init_fail;
			}
		}
	}

	ret = usbh_uvc_class_init();
	if (ret) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "uvc class init error: %d", ret);
		goto init_fail;
	}

	return HAL_OK;

init_fail:
	usbh_uvc_deinit();
	return ret;

}


/**
  * @brief	Deinit host video class
  * @param	None
  * @retval None
  */
void usbh_uvc_deinit(void)
{
	int ret;
	usbh_uvc_host_t *uvc = &uvc_host;
	usbh_uvc_cb_t *cb = uvc->cb;

	if (usbh_uvc_stream_state() == STREAMING_ON) {
		usbh_uvc_stream_off();
	}

	usbh_uvc_stream_deinit();

	usbh_uvc_desc_free();

	usbh_uvc_class_deinit();

	if (cb != NULL) {
		uvc->cb = cb;
		if (cb->deinit != NULL) {
			ret = cb->deinit();
			if (ret != HAL_OK) {
				DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "uvc user cb deinit error: %d", ret);
			}
		}
	}

	uvc->cb = NULL;

}


/**
  * @brief	Enable video streaming
  * @param	None
  * @retval Status
  */
int usbh_uvc_stream_on(void)
{
	usbh_uvc_host_t *uvc = &uvc_host;

	if (usbh_uvc_stream_state() == STREAMING_ON) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "UVC streaming is already on\n");
		return HAL_OK;
	}

	usbh_uvc_stream_init();

	uvc->stream_state = STREAMING_ON;
	uvc->stream_data_state = STREAM_OPEN_PIPE;

	usbh_notify_class_state_change(uvc->host);

	return HAL_OK;
}



/**
  * @brief	Disable video streaming
  * @param	None
  * @retval Status
  */
int usbh_uvc_stream_off(void)
{
	usbh_uvc_host_t *uvc = &uvc_host;

	if (usbh_uvc_stream_state() == STREAMING_OFF) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "UVC streaming is already off\n");
		return HAL_OK;
	}

	uvc->stream_state = STREAMING_OFF;
	uvc->stream_data_state = STREAM_CLOSE_PIPE;

	usbh_notify_class_state_change(uvc->host);

	usbh_uvc_stream_deinit();

	return HAL_OK;
}



/**
  * @brief	Get video streaming state
  * @param	None
  * @retval Status
  */
int usbh_uvc_stream_state(void)
{
	usbh_uvc_host_t *uvc = &uvc_host;

	return uvc->stream_state;
}


/**
  * @brief	Set video parameter
  * @param	para: user parameter, such as FPS, resolution
  * @retval Status
  */
int usbh_uvc_set_param(struct uvc_config *para)
{
	int ret;
	usbh_uvc_host_t *uvc = &uvc_host;
	uvc_stream_control ctrl;
	u32 format_index, frame_index;
	u32 frame_intv = 0;

	if ((para->fmt_type != UVC_FORMAT_MJPEG) && (para->fmt_type != UVC_FORMAT_UNCOMPRESSED)) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Unsupported format, only support MJEPG and UNCOMPRESSED\n");
		return -HAL_ERR_PARA;
	}

	/*Find format and closest resolution*/
	ret = usbh_uvc_find_format_frame(para, &format_index, &frame_index);
	if (ret) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Fail to find format or frame \n");
		return -HAL_ERR_PARA;
	}

	/*Find closest frame rate*/
	usbh_uvc_find_frame_rate(para, &frame_intv, &format_index, &frame_index);

	rtw_memset(&ctrl, 0, sizeof(uvc_stream_control));
	ctrl.bmHint = 1;  /* dwFrameInterval */
	ctrl.bFormatIndex = format_index;
	ctrl.bFrameIndex = frame_index;
	ctrl.dwFrameInterval = frame_intv;
	ctrl.dwMaxVideoFrameSize = uvc->stream.stream_ctrl.dwMaxVideoFrameSize;

	ret = usbh_uvc_probe_video(&ctrl);

	rtw_memcpy(&uvc->stream.stream_ctrl, &ctrl, sizeof(uvc_stream_control));

	return HAL_OK;

}



/**
  * @brief	Get a frame from video streaming
  * @param	None
  * @retval Status
  */
uvc_frame *usbh_uvc_get_frame(void)
{
	usbh_uvc_host_t *uvc = &uvc_host;
	uvc_frame *frame;

	if (rtw_down_timeout_sema(&uvc->frame_sema, UVC_GET_FRAME_TIMEOUT)) {
		if (list_empty(&uvc->frame_chain)) {
			/*should not reach here*/
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "No frame in frame_chain when sema down successful\n");
			return NULL;
		} else {
			frame = list_first_entry(&uvc->frame_chain, uvc_frame, list);
			list_del_init(&frame->list);
			return frame;
		}
	} else {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "Fail to down frame sema\n");
		return NULL;
	}

}



/**
  * @brief	Put frame buffer to video streaming
  * @param	frame: uvc frame buffer to put
  * @retval None
  */
void usbh_uvc_put_frame(uvc_frame *frame)
{
	usbh_uvc_host_t *uvc = &uvc_host;

	if (frame != NULL) {
		frame->byteused = 0;
		list_add_tail(&frame->list, &uvc->frame_empty);
	}

}

