/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/arch.h>

#include <video/video.h>
#include <video/video_halif.h>
#include "usbhost_uvc.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/* video driver HAL infterface */

static int uvc_open(FAR void *video_private);
static int uvc_close(FAR void *video_private);
static int uvc_do_halfpush(FAR void *video_private, bool enable);
static int uvc_set_buf(FAR void *video_private, enum v4l2_buf_type type, uint32_t bufaddr, uint32_t bufsize);
static int uvc_cancel_dma(FAR void *video_private);
static int uvc_get_range_of_fmt(FAR void *video_private, FAR struct v4l2_fmtdesc *format);
static int uvc_get_range_of_framesize(FAR void *video_private, FAR struct v4l2_frmsizeenum
									  *frmsize);
static int uvc_try_format(FAR void *video_private, FAR struct v4l2_format *format);
static int uvc_set_format(FAR void *video_private, FAR struct v4l2_format *format);
static int uvc_get_range_of_frameinterval(FAR void *video_private, FAR struct v4l2_frmivalenum
		*frmival);
static int uvc_set_frameinterval(FAR void *video_private, FAR struct v4l2_streamparm *parm);
static int uvc_get_range_of_ctrlvalue(FAR void *video_private, FAR struct v4l2_query_ext_ctrl
									  *range);
static int uvc_get_menu_of_ctrlvalue(FAR void *video_private, FAR struct v4l2_querymenu *menu);
static int uvc_get_ctrlvalue(FAR void *video_private, uint16_t ctrl_class, FAR struct v4l2_ext_control *control);
static int uvc_set_ctrlvalue(FAR void *video_private, uint16_t ctrl_class, FAR struct v4l2_ext_control *control);
static int uvc_refresh(FAR void *video_private);

static uint32_t uvc_try_frame_interval(struct uvc_frame *frame, uint32_t interval);

/****************************************************************************
 * Private Data
 ****************************************************************************/
struct video_devops_s g_uvc_video_devops = {
	.open = uvc_open,
	.close = uvc_close,
	.do_halfpush = uvc_do_halfpush,
	.set_buf = uvc_set_buf,
	.cancel_dma = uvc_cancel_dma,
	.get_range_of_fmt = uvc_get_range_of_fmt,
	.get_range_of_framesize = uvc_get_range_of_framesize,
	.try_format = uvc_try_format,
	.set_format = uvc_set_format,
	.get_range_of_frameinterval = uvc_get_range_of_frameinterval,
	.set_frameinterval = uvc_set_frameinterval,
	.get_range_of_ctrlvalue = uvc_get_range_of_ctrlvalue,
	.get_menu_of_ctrlvalue = uvc_get_menu_of_ctrlvalue,
	.get_ctrlvalue = uvc_get_ctrlvalue,
	.set_ctrlvalue = uvc_set_ctrlvalue,
	.refresh = uvc_refresh,
};

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: uvc_reduce_fraction
 *
 * Description:
 *   This is a helper function to reduce the fraction say 10/1000 as 1/100 etc.
 *
 * Input Parameters:
 *   *ntor - Pointer to numerator.
 *   *dtor - Pointer to denominator
 *
 * Returned Values:
 *   None
 *
 ****************************************************************************/
static void uvc_reduce_fraction(uint32_t *ntor, uint32_t *dtor)
{
	uint32_t arbt[8];
	uint32_t x = 0, y = 0, r = 0;
	unsigned int i = 0, n = 0;

	for (x = *ntor, y = *dtor, n = 0; n < 8 && y != 0; ++n) {
		arbt[n] = x / y;
		if (arbt[n] >= 333) {
			if (n < 2) {
				n++;
			}
			break;
		}
		r = x - arbt[n] * y;
		x = y;
		y = r;
	}

	for (x = 0, y = 1, i = n; i > 0; --i) {
		r = y;
		y = arbt[i - 1] * y + x;
		x = r;
	}

	*ntor = y;
	*dtor = x;
}

/****************************************************************************
 * Name: uvc_convert_interval
 *
 * Description:
 *   This is a helper function to calulate frame interval from it numerator &
 *   denominator.
 *
 * Input Parameters:
 *   ntor - frame numerator.
 *   dtor - frame denominator
 *
 * Returned Values:
 *   Return interval value.
 *
 ****************************************************************************/
static uint32_t uvc_convert_interval(uint32_t ntor, uint32_t dtor)
{
	uint32_t multi = 10000000;

	if (dtor == 0 || ntor / dtor >= ((uint32_t) - 1) / multi) {
		return (uint32_t) - 1;
	}

	while (ntor > ((uint32_t) - 1) / multi) {
		multi /= 2;
		dtor /= 2;
	}

	return dtor ? ntor * multi / dtor : 0;
}

/****************************************************************************
 * Name: uvc_open
 *
 * Description:
 *   This function opens and allocates all the resources requried
 *   for the video upper layer.
 *
 *   This function is called from the video driver using dev ops.
 *
 * Input Parameters:
 *   video_private - Reference to private data.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static int uvc_open(FAR void *video_private)
{
	int ret = OK;
	struct video_lowerhalf_s *phalf = (struct video_lowerhalf_s *)video_private;
	FAR uvc_state_t *priv = NULL;
	struct uvc_streaming *stream;

	if (phalf == NULL) {
		return -EINVAL;
	}

	priv = (FAR uvc_state_t *) phalf->dev;

	if (priv == NULL) {
		return -EINVAL;
	}
	stream = priv->stream;
	DEBUGASSERT(stream);
	usbhost_uvc_takesem(&priv->opsem);
	/* Increment the driver reference count */
	usbhost_uvc_takesem(&priv->exclsem);
	/* Connection is not complete or got disconnected, dont increment the count */
	if (!priv->disconnected) {
		/* Check if already open by any other process */
		if (priv->crefs > 1) {
			ret = -EBUSY;
			goto errout;
		}
		priv->crefs++;
	} else {
		ret = -ENODEV;
		goto errout;
	}
	/* Fresh open request, reset the circular buffers */
	if (ret == OK) {
		usbhost_uvc_takesem(&stream->mutex);
		uvc_circ_bbuf_init(&stream->cbuf);
		uvc_circ_bbuf_init(&stream->fbuf);
		usbhost_uvc_givesem(&stream->mutex);
	}

errout:
	usbhost_uvc_givesem(&priv->exclsem);
	return ret;
}

/****************************************************************************
 * Name: uvc_close
 *
 * Description:
 *   This function closes and releases all the resources acquired by
 *    the video upper layer.
 *
 *   This function is called from the video driver using dev ops.
 *
 * Input Parameters:
 *   video_private - Reference to private data.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static int uvc_close(FAR void *video_private)
{
	int ret = OK;
	struct uvc_streaming *stream;
	struct video_lowerhalf_s *phalf = (struct video_lowerhalf_s *)video_private;
	FAR uvc_state_t *priv = NULL;

	if (phalf == NULL) {
		return -EINVAL;
	}

	priv = (FAR uvc_state_t *) phalf->dev;

	if (priv == NULL) {
		return -EINVAL;
	}

	stream = priv->stream;
	DEBUGASSERT(stream);
	/* This will stop and pending stream transfer and cancel any work queue */
	usbhost_uvc_takesem(&priv->exclsem);
	if (!priv->disconnected) {
		uvc_video_stop_streaming(stream);
	}

	/* Decrement the driver reference count */
	if (priv->crefs > 0) {
		priv->crefs--;
	}
	usbhost_uvc_givesem(&priv->exclsem);
	usbhost_uvc_givesem(&priv->opsem);
	return ret;
}

/****************************************************************************
 * Name: uvc_do_halfpush
 *
 * Description:
 *   This function is video dev ops to send the partial data.
 *
 *   This function is called from the video driver using dev ops.
 *
 * Input Parameters:
 *   video_private - Reference to private data.
 *   enable - flag represet half push enable or not.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static int uvc_do_halfpush(FAR void *video_private, bool enable)
{
	int ret = -EPERM;

	return ret;
}

/****************************************************************************
 * Name: uvc_set_buf
 *
 * Description:
 *   This function is video dev ops to set the tranfer buffer, as well initiate
 *   a stream transfer .
 *
 *   This function is called from the video driver using dev ops.
 *
 * Input Parameters:
 *   video_private - Reference to private data.
 *   bufaddr - address of the buffer to which stream to be copied.
 *   bufsize - size of the buffer.
 *   args - argument to be pass in call back function of transfer completion.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static int uvc_set_buf(FAR void *video_private, enum v4l2_buf_type type, uint32_t bufaddr, uint32_t bufsize)
{
	int ret = OK;
	uint32_t size;
	struct uvc_streaming *stream;
	FAR uvc_state_t *priv = NULL;
	struct video_lowerhalf_s *phalf = (struct video_lowerhalf_s *)video_private;

	DEBUGASSERT(phalf || bufaddr || bufsize);

	priv = (FAR uvc_state_t *) phalf->dev;
	DEBUGASSERT(priv);

	/* Device got disconnected, no further processing */
	usbhost_uvc_takesem(&priv->exclsem);

	if (priv->disconnected) {
		usbhost_uvc_givesem(&priv->exclsem);
		return -ENODEV;
	}
	usbhost_uvc_givesem(&priv->exclsem);

	stream = priv->stream;
	DEBUGASSERT(stream);
	/* Video driver will queue one frame buffer at a time, on completion of
	 * one frame buffer, UVC driver will notify the upper video layer and it will
	 * queue the next frame buffer.
	 */
	usbhost_uvc_takesem(&stream->mutex);
	DEBUGASSERT(stream->cur_frame);
	/* Sanity check: Buffer size should be atleast 2 * width * height the current frame */
	size = stream->cur_frame->wWidth * stream->cur_frame->wHeight * 2;
	if (bufsize < size) {
		uvdbg("Buffer size should be atleast twice width * height\n");
		usbhost_uvc_givesem(&stream->mutex);
		return -EINVAL;
	}
	usbhost_uvc_givesem(&stream->mutex);

	usbhost_uvc_takesem(&stream->trfmutex);
	/* TODO: match the supported type with requested type else fail this request */
	stream->reqtype = type;
	stream->reqbuff = (uint8_t *) bufaddr;
	stream->reqsize = bufsize;
	usbhost_uvc_givesem(&stream->trfmutex);

	ret = uvc_video_start_streaming(stream);

	return ret;
}

/****************************************************************************
 * Name: uvc_cancel_dma
 *
 * Description:
 *   This function is video dev ops to cancel the current transfer request.
 *
 *   This function is called from the video driver using dev ops.
 *
 * Input Parameters:
 *   video_private - Reference to private data.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static int uvc_cancel_dma(FAR void *video_private)
{
	int ret = OK;
	struct uvc_streaming *stream;
	FAR uvc_state_t *priv = NULL;
	struct video_lowerhalf_s *phalf = (struct video_lowerhalf_s *)video_private;

	DEBUGASSERT(phalf);

	priv = (FAR uvc_state_t *) phalf->dev;
	DEBUGASSERT(priv);

	stream = priv->stream;
	DEBUGASSERT(stream);

	uvc_video_stop_streaming(stream);

	return ret;
}

/****************************************************************************
 * Name: uvc_get_range_of_framesize
 *
 * Description:
 *   This function is video dev ops to check the range of format supported.
 *
 *   This function is called from the video driver using dev ops.
 *
 * Input Parameters:
 *   video_private - Reference to private data.
 *   fsize - Pointer to frame object.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static int uvc_get_range_of_framesize(FAR void *video_private, FAR struct v4l2_frmsizeenum *fsize)
{
	int flag = 0;
	unsigned int index;
	struct uvc_streaming *stream;
	struct uvc_format *format = NULL;
	struct uvc_frame *frame = NULL;
	struct uvc_frame *tframe = NULL;
	struct video_lowerhalf_s *phalf = (struct video_lowerhalf_s *)video_private;
	FAR uvc_state_t *priv = NULL;

	if (phalf == NULL) {
		return -EINVAL;
	}

	priv = (FAR uvc_state_t *) phalf->dev;

	if ((priv == NULL) || (fsize == NULL)) {
		return -EINVAL;
	}

	/* Device got disconnected, no further processing */
	usbhost_uvc_takesem(&priv->exclsem);

	if (priv->disconnected) {
		usbhost_uvc_givesem(&priv->exclsem);
		return -ENODEV;
	}
	usbhost_uvc_givesem(&priv->exclsem);

	stream = priv->stream;

	if (stream == NULL) {
		return -EINVAL;
	}

	/* Look for the given pixel format */
	format = (struct uvc_format *)uvc_format_by_fcc(stream, fsize->pixel_format);

	if (format == NULL) {
		return -EINVAL;
	}

	/* Skip duplicate frame sizes */
	tframe = queue_entry(format->frames.head, struct uvc_frame, list);

	for (index = 0; &tframe->list != (dq_entry_t *)&format->frames; tframe = queue_entry(tframe->list.flink, struct uvc_frame, list)) {
		if (frame && frame->wWidth == tframe->wWidth && frame->wHeight == tframe->wHeight) {
			continue;
		}
		frame = tframe;
		if (index == fsize->index) {
			/* Found the frame user is looking for  */
			flag = 1;
			break;
		}
		index++;
	}

	/* No frame found and index user looking for  */
	if (flag == 0) {
		return -EINVAL;
	}

	if (frame == NULL) {
		return -EINVAL;
	}

	fsize->type = V4L2_FRMSIZE_TYPE_DISCRETE;
	fsize->discrete.width = frame->wWidth;
	fsize->discrete.height = frame->wHeight;

	return OK;
}

/****************************************************************************
 * Name: uvc_get_range_of_fmt
 *
 * Description:
 *   This function is video dev ops to check the range of format supported.
 *
 *   This function is called from the video driver using dev ops.
 *
 * Input Parameters:
 *   video_private - Reference to private data.
 *   fmt - Pointer to format object.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static int uvc_get_range_of_fmt(FAR void *video_private, FAR struct v4l2_fmtdesc *fmt)
{
	struct uvc_format *format;
	enum v4l2_buf_type type = fmt->type;
	uint32_t index = fmt->index;
	struct uvc_streaming *stream = NULL;
	struct video_lowerhalf_s *phalf = (struct video_lowerhalf_s *)video_private;
	FAR uvc_state_t *priv = NULL;

	if (phalf == NULL) {
		return -EINVAL;
	}

	priv = (FAR uvc_state_t *) phalf->dev;

	if ((priv == NULL) || (fmt == NULL)) {
		return -EINVAL;
	}

	/* Device got disconnected, no further processing */
	usbhost_uvc_takesem(&priv->exclsem);

	if (priv->disconnected) {
		usbhost_uvc_givesem(&priv->exclsem);
		return -ENODEV;
	}
	usbhost_uvc_givesem(&priv->exclsem);

	stream = priv->stream;

	if (stream == NULL) {
		return -EINVAL;
	}
	/* Format can be of still or capture type, but steam will be always type capture
	 * still capture is not always supported by camere device, but any frame can be used
	 * for still capture
	 */
	if ((fmt->type & 0x0F) != stream->type || fmt->index > stream->nformats) {
		return -EINVAL;
	}

	memset(fmt, 0, sizeof(*fmt));
	fmt->index = index;
	fmt->type = type;

	format = (struct uvc_format *)uvc_format_by_index(stream, fmt->index);
	if (format == NULL) {
		uvdbg("No valid format found in given index!!\n");
		return -EINVAL;
	}

	fmt->flags = 0;
	strncpy(fmt->description, format->name, (sizeof(fmt->description) - 1));
	fmt->description[sizeof(fmt->description) - 1] = 0;
	fmt->pixelformat = format->fcc;

	return OK;
}

/****************************************************************************
 * Name: uvc_v4l2_try_format
 *
 * Description:
 *   This function is video dev ops for try and check the format can be applied.
 *
 *   This function is called from uvc_try_format and uvc_set_format functions.
 *
 * Input Parameters:
 *   stream - Reference to stream data.
 *   fmt - Pointer to format to be tried.
 *   flag - flag which determine weather to update current format details.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static int uvc_v4l2_try_format(struct uvc_streaming *stream, struct v4l2_format *fmt, bool flag)
{
	int ret = 0;
	uint16_t rw, rh;
	unsigned int d, maxd;
	uint32_t interval;
	struct uvc_format *format = NULL;
	struct uvc_frame *frame = NULL;
	struct uvc_frame *tframe = NULL;
	struct uvc_streaming_control probe;

	/* Format can be of still or capture type, but steam will be always type capture
	 * still capture is not always supported by camere device, but any frame can be used
	 * for still capture
	 */
	if ((fmt->type & 0x0F) != stream->type) {
		return -EINVAL;
	}

	/* Check if the hardware supports the requested format, use the default
	 * format otherwise.
	 */
	format = (struct uvc_format *)uvc_format_by_fcc(stream, fmt->fmt.pix.pixelformat);

	if (format == NULL) {
		format = stream->def_format;
		fmt->fmt.pix.pixelformat = format->fcc;
	}

	/* Find the closest image size. The distance between image sizes is
	 * the size in pixels of the non-overlapping regions between the
	 * requested size and the frame-specified size.
	 */
	rw = fmt->fmt.pix.width;
	rh = fmt->fmt.pix.height;
	maxd = (unsigned int) - 1;

	tframe = queue_entry(format->frames.head, struct uvc_frame, list);

	for (; &tframe->list != (dq_entry_t *)&format->frames; tframe = queue_entry(tframe->list.flink, struct uvc_frame, list)) {
		uint16_t w = tframe->wWidth;
		uint16_t h = tframe->wHeight;
		d = min_t(w, rw) * min_t(h, rh);
		d = w * h + rw * rh - 2 * d;
		if (d < maxd) {
			maxd = d;
			frame = tframe;
		}
		if (maxd == 0) {
			break;
		}
	}

	if (frame == NULL) {
		uvdbg("Unsupported size %ux%u.\n", fmt->fmt.pix.width, fmt->fmt.pix.height);
		return -EINVAL;
	}

	/* Use the default frame interval. */
	interval = frame->dwDefaultFrameInterval;

	/* Set the format index, frame index and frame interval. */
	memset(&probe, 0, sizeof(probe));
	probe.bmHint = 1;			/* dwFrameInterval */
	probe.bFormatIndex = format->index;
	probe.bFrameIndex = frame->bFrameIndex;
	probe.dwFrameInterval = uvc_try_frame_interval(frame, interval);

	usbhost_uvc_takesem(&stream->mutex);

	/* Probe the device. */
	ret = uvc_probe_video(stream, &probe);
	if ((ret >= 0) && flag) {
		stream->ctrl = probe;
		stream->cur_format = format;
		stream->cur_frame = frame;
	}

	if (ret < 0) {
		goto done;
	}
	/* When flag is true, more than a try the format/frame is changed
	 * requries adjusting the transfer parameters accordingly
	 */
	if (flag) {
		/* Initialize the transfer endpoints */
		ret = uvc_video_init_transfer(stream);
		if (ret < 0) {
			goto done;
		}
	}

	fmt->fmt.pix.width = frame->wWidth;
	fmt->fmt.pix.height = frame->wHeight;
	fmt->fmt.pix.field = V4L2_FIELD_NONE;
	fmt->fmt.pix.bytesperline = format->bpp * frame->wWidth / 8;
	fmt->fmt.pix.sizeimage = probe.dwMaxVideoFrameSize;
	fmt->fmt.pix.colorspace = format->colorspace;

done:
	usbhost_uvc_givesem(&stream->mutex);
	return ret;
}

/****************************************************************************
 * Name: uvc_try_format
 *
 * Description:
 *   This function is video dev ops for try and check the format can be applied.
 *
 *   This function is called from the video driver using dev ops.
 *
 * Input Parameters:
 *   video_private - Reference to private data.
 *   format - Pointer to format to be tried.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static int uvc_try_format(FAR void *video_private, FAR struct v4l2_format *format)
{
	struct uvc_streaming *stream = NULL;
	struct video_lowerhalf_s *phalf = (struct video_lowerhalf_s *)video_private;
	FAR uvc_state_t *priv = NULL;

	if (phalf == NULL) {
		return -EINVAL;
	}

	priv = (FAR uvc_state_t *) phalf->dev;

	if ((format == NULL) || (priv == NULL)) {
		return -EINVAL;
	}

	/* Device got disconnected, no further processing */
	usbhost_uvc_takesem(&priv->exclsem);

	if (priv->disconnected) {
		usbhost_uvc_givesem(&priv->exclsem);
		return -ENODEV;
	}
	usbhost_uvc_givesem(&priv->exclsem);

	stream = priv->stream;

	if (stream == NULL) {
		return -EINVAL;
	}

	return uvc_v4l2_try_format(stream, format, 0);
}

/****************************************************************************
 * Name: uvc_set_format
 *
 * Description:
 *   This function is video dev ops for setting format.
 *
 *   This function is called from the video driver using dev ops.
 *
 * Input Parameters:
 *   video_private - Reference to private data.
 *   fmt - Pointer to format to be set.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static int uvc_set_format(FAR void *video_private, FAR struct v4l2_format *fmt)
{
	int ret;
	struct uvc_streaming *stream = NULL;
	struct video_lowerhalf_s *phalf = (struct video_lowerhalf_s *)video_private;
	FAR uvc_state_t *priv = NULL;

	if (phalf == NULL) {
		return -EINVAL;
	}

	priv = (FAR uvc_state_t *) phalf->dev;

	if ((fmt == NULL) || (priv == NULL)) {
		return -EINVAL;
	}

	/* Device got disconnected, no further processing */
	usbhost_uvc_takesem(&priv->exclsem);

	if (priv->disconnected) {
		usbhost_uvc_givesem(&priv->exclsem);
		return -ENODEV;
	}
	usbhost_uvc_givesem(&priv->exclsem);

	stream = priv->stream;

	if (stream == NULL) {
		return -EINVAL;
	}

	/* Format can be of still or capture type, but steam will be always type capture
	 * still capture is not always supported by camere device, but any frame can be used
	 * for still capture
	 */
	if ((fmt->type & 0x0F) != stream->type) {
		return -EINVAL;
	}

	ret = uvc_v4l2_try_format(stream, fmt, 1);
	if (ret < 0) {
		return ret;
	}

	return OK;
}

/****************************************************************************
 * Name: uvc_try_frame_interval
 *
 * Description:
 *   This function is call to check the frame interval can be applied to device.
 *
 * Input Parameters:
 *   frame - Pointer to frame interval.
 *   interval - Interval to to match with frame.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static uint32_t uvc_try_frame_interval(struct uvc_frame *frame, uint32_t interval)
{
	unsigned int i;
	uint32_t min;
	uint32_t max;
	uint32_t step;

	if (frame->bFrameIntervalType) {
		uint32_t best = -1, dist;

		for (i = 0; i < frame->bFrameIntervalType; ++i) {
			dist = interval > frame->dwFrameInterval[i]
				   ? interval - frame->dwFrameInterval[i]
				   : frame->dwFrameInterval[i] - interval;

			if (dist > best) {
				break;
			}

			best = dist;
		}

		interval = frame->dwFrameInterval[i - 1];
	} else {
		min = frame->dwFrameInterval[0];
		max = frame->dwFrameInterval[1];
		step = frame->dwFrameInterval[2];

		interval = min + (interval - min + step / 2) / step * step;
		if (interval > max) {
			interval = max;
		}
	}

	return interval;
}

/****************************************************************************
 * Name: uvc_get_range_of_frameinterval
 *
 * Description:
 *   This function is video dev ops for get the range of frame interval.
 *
 *   This function is called from the video driver using dev ops.
 *
 * Input Parameters:
 *   video_private - Reference to private data.
 *   fival - Pointer to frame interval.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static int uvc_get_range_of_frameinterval(FAR void *video_private, FAR struct v4l2_frmivalenum *fival)
{
	unsigned int index;
	unsigned int nintervals;
	struct uvc_streaming *stream = NULL;
	struct uvc_format *format = NULL;
	struct uvc_frame *frame = NULL;
	struct uvc_frame *tframe = NULL;
	struct video_lowerhalf_s *phalf = (struct video_lowerhalf_s *)video_private;
	FAR uvc_state_t *priv = NULL;

	if (phalf == NULL) {
		return -EINVAL;
	}

	priv = (FAR uvc_state_t *) phalf->dev;

	if ((priv == NULL) || (fival == NULL)) {
		return -EINVAL;
	}

	/* Device got disconnected, no further processing */
	usbhost_uvc_takesem(&priv->exclsem);

	if (priv->disconnected) {
		usbhost_uvc_givesem(&priv->exclsem);
		return -ENODEV;
	}
	usbhost_uvc_givesem(&priv->exclsem);

	stream = priv->stream;

	if (stream == NULL) {
		return -EINVAL;
	}

	/* Look for the given pixel format and frame size */
	format = (struct uvc_format *)uvc_format_by_fcc(stream, fival->pixel_format);

	if (format == NULL) {
		return -EINVAL;
	}

	index = fival->index;

	tframe = queue_entry(format->frames.head, struct uvc_frame, list);

	for (; &tframe->list != (dq_entry_t *)&format->frames; tframe = queue_entry(tframe->list.flink, struct uvc_frame, list)) {
		if (tframe->wWidth == fival->width && tframe->wHeight == fival->height) {
			nintervals = tframe->bFrameIntervalType ? : 1;
			frame = tframe;
			if (index < nintervals) {
				break;
			} else {
				/* User requesting frame interval which is beyond the supported index */
				return -EINVAL;
			}
		}
	}

	if (frame == NULL) {
		uvdbg("No valid frame found !!\n");
		return -EINVAL;
	}

	if (frame->bFrameIntervalType) {
		fival->type = V4L2_FRMIVAL_TYPE_DISCRETE;
		fival->discrete.numerator = frame->dwFrameInterval[index];
		fival->discrete.denominator = 10000000;
		uvc_reduce_fraction(&fival->discrete.numerator, &fival->discrete.denominator);
	} else {
		fival->type = V4L2_FRMIVAL_TYPE_STEPWISE;
		fival->stepwise.min.numerator = frame->dwFrameInterval[0];
		fival->stepwise.min.denominator = 10000000;
		fival->stepwise.max.numerator = frame->dwFrameInterval[1];
		fival->stepwise.max.denominator = 10000000;
		fival->stepwise.step.numerator = frame->dwFrameInterval[2];
		fival->stepwise.step.denominator = 10000000;
		uvc_reduce_fraction(&fival->stepwise.min.numerator, &fival->stepwise.min.denominator);
		uvc_reduce_fraction(&fival->stepwise.max.numerator, &fival->stepwise.max.denominator);
		uvc_reduce_fraction(&fival->stepwise.step.numerator, &fival->stepwise.step.denominator);
	}

	return OK;
}

/****************************************************************************
 * Name: uvc_set_frameinterval
 *
 * Description:
 *   This function is video dev ops for setting frame interval.
 *
 *   This function is called from the video driver using dev ops.
 *
 * Input Parameters:
 *   video_private - Reference to private data.
 *   parm - Pointer to stream paramaters to be set.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static int uvc_set_frameinterval(FAR void *video_private, FAR struct v4l2_streamparm *parm)
{
	int ret;
	uint32_t interval, maxd;
	struct uvc_frame *frame = NULL;
	struct uvc_frame *tframe = NULL;
	struct uvc_format *format = NULL;
	struct v4l2_fract timeperframe;
	struct uvc_streaming_control probe;
	struct uvc_streaming *stream = NULL;
	struct video_lowerhalf_s *phalf = (struct video_lowerhalf_s *)video_private;
	FAR uvc_state_t *priv = NULL;

	if (phalf == NULL) {
		return -EINVAL;
	}

	priv = (FAR uvc_state_t *) phalf->dev;

	if ((priv == NULL) || (parm == NULL)) {
		return -EINVAL;
	}

	/* Device got disconnected, no further processing */
	usbhost_uvc_takesem(&priv->exclsem);

	if (priv->disconnected) {
		usbhost_uvc_givesem(&priv->exclsem);
		return -ENODEV;
	}
	usbhost_uvc_givesem(&priv->exclsem);

	stream = priv->stream;

	if (stream == NULL) {
		return -EINVAL;
	}
	/* Frame interval is required only in case of capture, so still case can be ignore here */
	if (parm->type != stream->type) {
		return -EINVAL;
	}

	if (parm->type == V4L2_BUF_TYPE_VIDEO_CAPTURE) {
		timeperframe = parm->parm.capture.timeperframe;
	} else {
		return -EINVAL;
	}

	interval = uvc_convert_interval(timeperframe.numerator, timeperframe.denominator);

	uvdbg("Setting frame interval to %u/%u (%u).\n", timeperframe.numerator, timeperframe.denominator, interval);

	usbhost_uvc_takesem(&stream->mutex);

	format = stream->cur_format;
	frame = stream->cur_frame;
	probe = stream->ctrl;
	probe.dwFrameInterval = uvc_try_frame_interval(frame, interval);
	maxd = abs((s32) probe.dwFrameInterval - interval);

	/* Try frames with matching size to find the best frame interval. */
	tframe = queue_entry(format->frames.head, struct uvc_frame, list);

	for (; (&tframe->list != (dq_entry_t *)&format->frames) && (maxd != 0); tframe = queue_entry(tframe->list.flink, struct uvc_frame, list)) {
		uint32_t d, ival;

		if (tframe == stream->cur_frame) {
			continue;
		}

		if (tframe->wWidth != stream->cur_frame->wWidth || tframe->wHeight != stream->cur_frame->wHeight) {
			continue;
		}

		ival = uvc_try_frame_interval(tframe, interval);
		d = abs((int32_t) ival - interval);
		if (d >= maxd) {
			continue;
		}
		frame = tframe;
		probe.bFrameIndex = frame->bFrameIndex;
		probe.dwFrameInterval = ival;
		maxd = d;
	}

	/* Probe the device with the new settings. */
	ret = uvc_probe_video(stream, &probe);
	if (ret < 0) {
		usbhost_uvc_givesem(&stream->mutex);
		return ret;
	}

	stream->ctrl = probe;
	stream->cur_frame = frame;
	usbhost_uvc_givesem(&stream->mutex);

	/* Return the actual frame period. */
	timeperframe.numerator = probe.dwFrameInterval;
	timeperframe.denominator = 10000000;
	uvc_reduce_fraction(&timeperframe.numerator, &timeperframe.denominator);

	parm->parm.capture.timeperframe = timeperframe;

	/* Initialize the transfer endpoints */
	ret = uvc_video_init_transfer(stream);
	if (ret < 0) {
		return ret;
	}

	return 0;
}

/****************************************************************************
 * Name: uvc_get_range_of_ctrlvalue
 *
 * Description:
 *   This function is video dev ops for retrive the range of control commands.
 *
 *   This function is called from the video driver using dev ops.
 *
 * Input Parameters:
 *   video_private - Reference to private data.
 *   range - Pointer to range data to be retrived
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static int uvc_get_range_of_ctrlvalue(FAR void *video_private, FAR struct v4l2_query_ext_ctrl *range)
{
	int ret = OK;
	struct v4l2_queryctrl qc = { range->ctrl_class, range->id };
	struct uvc_video_chain *chain;
	struct video_lowerhalf_s *phalf = (struct video_lowerhalf_s *)video_private;
	FAR uvc_state_t *priv = NULL;

	if (phalf == NULL) {
		return -EINVAL;
	}

	priv = (FAR uvc_state_t *) phalf->dev;

	if ((range == NULL) || (priv == NULL)) {
		return -EINVAL;
	}

	if ((priv->stream == NULL) || (priv->stream->chain == NULL)) {
		return -ENOSYS;
	}

	chain = priv->stream->chain;

	/* Device got disconnected, no further processing */
	usbhost_uvc_takesem(&priv->exclsem);

	if (priv->disconnected) {
		usbhost_uvc_givesem(&priv->exclsem);
		return -ENODEV;
	}
	usbhost_uvc_givesem(&priv->exclsem);

	ret = uvc_query_v4l2_ctrl(chain, &qc);
	if (ret) {
		return ret;
	}

	range->id = qc.id;
	range->type = qc.type;
	strncpy(range->name, qc.name, (sizeof(range->name) - 1));
	range->name[sizeof(range->name) - 1] = 0;
	range->minimum = qc.minimum;
	range->maximum = qc.maximum;
	range->step = qc.step;
	range->default_value = qc.default_value;
	range->flags = qc.flags;
	range->elem_size = 4;
	range->elems = 1;
	range->nr_of_dims = 0;
	memset(range->dims, 0, sizeof(range->dims));

	return OK;
}

/****************************************************************************
 * Name: uvc_get_menu_of_ctrlvalue
 *
 * Description:
 *   This function is video dev ops for retrive the menu data.
 *
 *   This function is called from the video driver using dev ops.
 *
 * Input Parameters:
 *   video_private - Reference to private data.
 *   menu - Pointer to menu data to be retrived
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static int uvc_get_menu_of_ctrlvalue(FAR void *video_private, FAR struct v4l2_querymenu *menu)
{
	int ret;
	struct uvc_menu_info *menu_info;
	struct uvc_control_mapping *mapping;
	struct uvc_control *ctrl;
	uint32_t index = menu->index;
	uint32_t id = menu->id;
	uint16_t ctrl_class = menu->ctrl_class;
	struct uvc_video_chain *chain;
	struct video_lowerhalf_s *phalf = (struct video_lowerhalf_s *)video_private;
	FAR uvc_state_t *priv = NULL;

	if (phalf == NULL) {
		return -EINVAL;
	}

	priv = (FAR uvc_state_t *) phalf->dev;

	if ((priv == NULL) || (menu == NULL)) {
		return -EINVAL;
	}

	if ((priv->stream == NULL) || (priv->stream->chain == NULL)) {
		return -ENOSYS;
	}

	chain = priv->stream->chain;

	/* Device got disconnected, no further processing */
	usbhost_uvc_takesem(&priv->exclsem);

	if (priv->disconnected) {
		usbhost_uvc_givesem(&priv->exclsem);
		return -ENODEV;
	}
	usbhost_uvc_givesem(&priv->exclsem);

	memset(menu, 0, sizeof(*menu));
	menu->id = id;
	menu->index = index;
	menu->ctrl_class = ctrl_class;

	usbhost_uvc_takesem(&chain->ctrl_mutex);

	ctrl = (struct uvc_control *)uvc_search_control(chain, menu->id, menu->ctrl_class, &mapping);
	if (ctrl == NULL || mapping->v4l2_type != V4L2_CTRL_TYPE_MENU) {
		ret = -EINVAL;
		goto done;
	}

	if (menu->index >= mapping->menu_count) {
		ret = -EINVAL;
		goto done;
	}

	menu_info = &mapping->menu_info[menu->index];

	if (mapping->data_type == UVC_CTRL_DATA_TYPE_BITMASK && (ctrl->info.flags & F_GET_RES)) {
		int32_t bitmap;

		ret = uvc_ctrl_populate_values(chain, ctrl);
		if (ret < 0) {
			goto done;
		}

		bitmap = uvc_get_value(mapping, UVC_GET_RES, ctrl->step);
		if (!(bitmap & menu_info->value)) {
			ret = -EINVAL;
			goto done;
		}
	}

	strncpy(menu->name, menu_info->name, (sizeof(menu->name) - 1));
	menu->name[sizeof(menu->name) - 1] = 0;

done:
	usbhost_uvc_givesem(&chain->ctrl_mutex);
	return ret;
}

/****************************************************************************
 * Name: uvc_get_ctrlvalue
 *
 * Description:
 *   This function is video dev ops for retrive the control data.
 *
 *   This function is called from the video driver using dev ops.
 *
 * Input Parameters:
 *   video_private - Reference to private data.
 *   ctrl_class - V4L2 control class id
 *   control - Pointer to control data to be set.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static int uvc_get_ctrlvalue(FAR void *video_private, uint16_t ctrl_class, FAR struct v4l2_ext_control *control)
{
	int ret = -EINVAL;
	struct v4l2_ext_control xctrl;
	struct uvc_video_chain *chain;
	struct video_lowerhalf_s *phalf = (struct video_lowerhalf_s *)video_private;
	FAR uvc_state_t *priv = NULL;

	if (phalf == NULL) {
		return -EINVAL;
	}

	priv = (FAR uvc_state_t *) phalf->dev;

	if ((control == NULL) || (priv == NULL)) {
		return -EINVAL;
	}

	if ((priv->stream == NULL) || (priv->stream->chain == NULL)) {
		return -ENOSYS;
	}

	chain = priv->stream->chain;

	/* Device got disconnected, no further processing */
	usbhost_uvc_takesem(&priv->exclsem);

	if (priv->disconnected) {
		usbhost_uvc_givesem(&priv->exclsem);
		return -ENODEV;
	}
	usbhost_uvc_givesem(&priv->exclsem);

	memset(&xctrl, 0, sizeof(xctrl));
	xctrl.id = control->id;

	usbhost_uvc_takesem(&chain->ctrl_mutex);

	ret = uvc_ctrl_get(chain, NULL, ctrl_class, &xctrl);
	uvc_ctrl_commit(chain, 1, NULL, 0);
	if (ret < 0) {
		return ret;
	}

	control->value = xctrl.value;

	return ret;
}

/****************************************************************************
 * Name: uvc_set_ctrlvalue
 *
 * Description:
 *   This function is video dev ops for setting control commands.
 *
 *   This function is called from the video driver using dev ops.
 *
 * Input Parameters:
 *   video_private - Reference to private data.
 *   ctrl_class - V4L2 control class id
 *   control - Pointer to control data to be set.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static int uvc_set_ctrlvalue(FAR void *video_private, uint16_t ctrl_class, FAR struct v4l2_ext_control *control)
{
	int ret = -EINVAL;
	struct v4l2_ext_control xctrl;
	struct uvc_video_chain *chain;
	struct video_lowerhalf_s *phalf = (struct video_lowerhalf_s *)video_private;
	FAR uvc_state_t *priv = NULL;

	if (phalf == NULL) {
		return -EINVAL;
	}

	priv = (FAR uvc_state_t *) phalf->dev;

	if ((control == NULL) || (priv == NULL)) {
		return -EINVAL;
	}

	if ((priv->stream == NULL) || (priv->stream->chain == NULL)) {
		return -ENOSYS;
	}

	chain = priv->stream->chain;

	/* Device got disconnected, no further processing */
	usbhost_uvc_takesem(&priv->exclsem);

	if (priv->disconnected) {
		usbhost_uvc_givesem(&priv->exclsem);
		return -ENODEV;
	}
	usbhost_uvc_givesem(&priv->exclsem);

	memset(&xctrl, 0, sizeof(xctrl));
	xctrl.id = control->id;
	xctrl.value = control->value;

	usbhost_uvc_takesem(&chain->ctrl_mutex);

	ret = uvc_ctrl_set(chain, ctrl_class, &xctrl);
	if (ret < 0) {
		uvc_ctrl_commit(chain, 1, NULL, 0);
		return ret;
	}
	uvc_ctrl_commit(chain, 0, &xctrl, 1);
	if (ret < 0) {
		return ret;
	}

	control->value = xctrl.value;

	return ret;
}

/****************************************************************************
 * Name: uvc_refresh
 *
 * Description:
 *   This function is video dev ops for refreshing the video stream.
 *
 *   This function is called from the video driver using dev ops.
 *
 * Input Parameters:
 *   video_private - Reference to private data.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static int uvc_refresh(FAR void *video_private)
{
	int ret = OK;

	return ret;
}
