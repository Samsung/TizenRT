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
#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/kmalloc.h>
#include <tinyara/usb/usb.h>
#include <tinyara/usb/usbhost.h>
#include "usbhost_uvc.h"

static void usbhost_transfer_callback(FAR void *arg, ssize_t nbytes);
static FAR struct usbhost_uvc_state_s *g_usbuvc_handoff;
/****************************************************************************
* Name: uvc_circ_bbuf_init
*
* Description:
*   A helper function to initialize circular queue.
*
* Input Parameters:
*   c - pointer to circular queue object.
*
* Returned Values:
*   0 on success, -1 if queue is full.
*
****************************************************************************/
void uvc_circ_bbuf_init(struct uvc_circ_buf *c)
{
	int i = 0;
	c->head = -1;
	c->tail = -1;
	c->maxlen = UVC_MAX_TRF_BUF;
	while (i < c->maxlen) {
		c->bobj[i] = NULL;
		i++;
	}
}

/****************************************************************************
 * Name: uvc_circ_bbuf_push
 *
 * Description:
 *   A helper function to push the buffer object to circular queue.
 *
 * Input Parameters:
 *   c - pointer to circular queue object.
 *   buf - pointer to buffer object.
 *
 * Returned Values:
 *   0 on success, -1 if queue is full.
 *
 ****************************************************************************/
static inline int uvc_circ_bbuf_push(struct uvc_circ_buf *c, struct uvc_buf_object *buf)
{
	if ((c->head == 0 && c->tail == c->maxlen - 1) || (c->tail == (c->head - 1) % (c->maxlen - 1))) {
		return -1;
	} else if (c->head == -1) {	/* Insert First Element */
		c->head = c->tail = 0;
		c->bobj[c->tail] = buf;
	}

	else if (c->tail == c->maxlen - 1 && c->head != 0) {
		c->tail = 0;
		c->bobj[c->tail] = buf;
	}

	else {
		c->tail++;
		c->bobj[c->tail] = buf;
	}

	return 0;
}

/****************************************************************************
 * Name: uvc_circ_bbuf_pop
 *
 * Description:
 *   A helper function to pop the buffer object from circular queue.
 *
 * Input Parameters:
 *   c - pointer to circular queue object.
 *   buf - pointer to buffer object.
 *
 * Returned Values:
 *   0 on success, -1 if queue is empty.
 *
 ****************************************************************************/
static inline int uvc_circ_bbuf_pop(struct uvc_circ_buf *c, struct uvc_buf_object **buf)
{
	if (c->head == -1) {
		return -1;
	}

	*buf = c->bobj[c->head];
	if (c->head == c->tail) {
		c->head = -1;
		c->tail = -1;
	} else if (c->head == c->maxlen - 1) {
		c->head = 0;
	} else {
		c->head++;
	}

	return 0;
}

/****************************************************************************
 * Name: uvc_video_ctrl_size
 *
 * Description:
 *   A helper function to return the size of the video probe and commit controls,
 *   which depends on the protocol version.
 *
 * Input Parameters:
 *   stream - pointer to stream object.
 *
 * Returned Values:
 *   Size of commit control.
 *
 ****************************************************************************/
static inline size_t uvc_video_ctrl_size(struct uvc_streaming *stream)
{
	if (stream->dev->uvc_version < 0x0110) {
		return 26;
	} else if (stream->dev->uvc_version < 0x0150) {
		return 34;
	} else {
		return 48;
	}
}

/****************************************************************************
 * Name: uvc_format_by_index
 *
 * Description:
 *   A helper function to search the format queue with the index of format
 *
 * Input Parameters:
 *   stream - pointer to stream object.
 *   index - index of the format to be returned
 *
 * Returned Values:
 *   NULL on failure, respective format matching index on success.
 *
 ****************************************************************************/
struct uvc_format *uvc_format_by_index(FAR struct uvc_streaming *stream, uint8_t index)
{
	struct uvc_format *format = queue_entry(stream->formats.head, struct uvc_format, list);

	for (; &format->list != (dq_entry_t *)&stream->formats; format = queue_entry(format->list.flink, struct uvc_format, list)) {
		if (format->index == index) {
			return format;
		}
	}

	return NULL;
}

/****************************************************************************
 * Name: uvc_format_by_fcc
 *
 * Description:
 *   A helper function to search the format queue with the fcc of format
 *
 * Input Parameters:
 *   stream - pointer to stream object.
 *   fcc - fcc of the format to be returned
 *
 * Returned Values:
 *   NULL on failure, respective format matching fcc on success.
 *
 ****************************************************************************/
struct uvc_format *uvc_format_by_fcc(FAR struct uvc_streaming *stream, uint32_t fcc)
{
	struct uvc_format *format = queue_entry(stream->formats.head, struct uvc_format, list);

	for (; &format->list != (dq_entry_t *)&stream->formats; format = queue_entry(format->list.flink, struct uvc_format, list)) {
		if (format->fcc == fcc) {
			return format;
		}
	}

	return NULL;
}

/****************************************************************************
 * Name: uvc_frame_by_index
 *
 * Description:
 *   A helper function to search the frame queue with the index of frame
 *
 * Input Parameters:
 *   stream - pointer to stream object.
 *   index - index of the frame to be returned
 *
 * Returned Values:
 *   NULL on failure, respective frame matching index on success.
 *
 ****************************************************************************/
struct uvc_frame *uvc_frame_by_index(FAR struct uvc_format *format, uint8_t index)
{
	struct uvc_frame *frame = queue_entry(format->frames.head, struct uvc_frame, list);

	for (; &frame->list != (dq_entry_t *)&format->frames; frame = queue_entry(frame->list.flink, struct uvc_frame, list)) {
		if (frame->bFrameIndex == index) {
			return frame;
		}
	}

	return NULL;
}

/****************************************************************************
 * Name: uvc_video_deinit_transfer
 *
 * Description:
 *   This function will releases the endpoints and work queue allocated for
 *   UVC camera streaming.
 *
 * Input Parameters:
 *   stream - pointer to stream object.
 *
 * Returned Values:
 *   None
 *
 ****************************************************************************/
static inline void uvc_video_deinit_transfer(struct uvc_streaming *stream)
{
	int bufindex = 0;
	FAR struct usbhost_uvc_state_s *dev = stream->dev;
	FAR struct usbhost_hubport_s *hport;

	DEBUGASSERT(dev != NULL && dev->usbclass.hport != NULL);
	hport = dev->usbclass.hport;

	if (stream->isocin) {
		DRVR_EPFREE(hport->drvr, stream->isocin);
		stream->isocin = NULL;
	}

	if (stream->bulkin) {
		DRVR_EPFREE(hport->drvr, stream->bulkin);
		stream->bulkin = NULL;
	}

	while (bufindex < UVC_MAX_TRF_BUF) {
		if (stream->bufobj[bufindex].buffer) {
			DRVR_IOFREE(hport->drvr, (FAR uint8_t *) stream->bufobj[bufindex].buffer);
			stream->bufobj[bufindex].buffer = NULL;
		}
		bufindex++;
	}
}

/****************************************************************************
 * Name: uvc_init_video_endpoint
 *
 * Description:
 *   This function will initializes endpoints and work queue UVC camera streaming.
 *
 * Input Parameters:
 *   stream - pointer to stream object.
 *   ep - ep descriptor
 *   type - type of endpoint to be allocated, bulk or isochronous
 *
 * Returned Values:
 *   0 on success, appropriate error code in case of failure.
 *
 ****************************************************************************/
static inline int uvc_init_video_endpoint(struct uvc_streaming *stream, struct usb_ep_desc *ep, int type)
{
	int ret;
	int maxp;
	int nframe;
	int bufindex = 0;
	FAR struct usbhost_uvc_state_s *dev = stream->dev;
	FAR struct usbhost_hubport_s *hport;
	FAR struct usbhost_epdesc_s indesc;
	FAR struct usb_epdesc_s *epdesc = &ep->ep;

	/* Release the earlier transfer allocations before setting new */
	uvc_video_deinit_transfer(stream);

	DEBUGASSERT(dev != NULL && dev->usbclass.hport != NULL);
	hport = dev->usbclass.hport;

	/* Initialize the endpoint */
	indesc.hport = hport;
	indesc.addr = epdesc->addr & USB_EP_ADDR_NUMBER_MASK;
	indesc.in = 1;
	indesc.interval = epdesc->interval;
	indesc.mxpacketsize = usbhost_uvc_getle16(epdesc->mxpacketsize);

	maxp = (indesc.mxpacketsize & 0x7FF);
	nframe = (indesc.mxpacketsize & 0x1800) >> 11;
	stream->bufsize = maxp * (nframe + 1);

	/* Allocate endpoint */
	if (type == 0) {
		indesc.xfrtype = USB_EP_ATTR_XFER_ISOC;
		ret = DRVR_EPALLOC(hport->drvr, (struct usbhost_epdesc_s *)&indesc, &stream->isocin);
	} else {
		indesc.xfrtype = USB_EP_ATTR_XFER_BULK;
		ret = DRVR_EPALLOC(hport->drvr, (struct usbhost_epdesc_s *)&indesc, &stream->bulkin);
	}
	if (ret < 0) {
		uvdbg("ERROR: DRVR_ALLOC of ctrlreq failed: %d\n", ret);
		goto errout;
	}
	while (bufindex < UVC_MAX_TRF_BUF) {
		stream->bufobj[bufindex].bufsize = stream->bufsize;
		stream->bufobj[bufindex].recsize = 0;
		stream->bufobj[bufindex].context = (uint8_t *) dev;
		/* Allocate memory for isochronous transfer */
		ret = DRVR_IOALLOC(hport->drvr, (FAR uint8_t **) & stream->bufobj[bufindex].buffer, stream->bufsize);
		if (ret < 0) {
			uvdbg("ERROR: DRVR_IOALLOC of isoc failed: %d\n", ret);
			goto errout;
		}
		uvc_circ_bbuf_push(&stream->fbuf, (struct uvc_buf_object *)&stream->bufobj[bufindex]);
		bufindex++;
	}

	return OK;
errout:
	if (stream->isocin) {
		DRVR_EPFREE(hport->drvr, stream->isocin);
		stream->isocin = NULL;
	}
	if (stream->bulkin) {
		DRVR_EPFREE(hport->drvr, stream->bulkin);
		stream->bulkin = NULL;
	}
	bufindex = 0;
	while (bufindex < UVC_MAX_TRF_BUF) {
		if (stream->bufobj[bufindex].buffer) {
			DRVR_IOFREE(hport->drvr, (FAR uint8_t *) stream->bufobj[bufindex].buffer);
			stream->bufobj[bufindex].buffer = NULL;
		}
		bufindex++;
	}

	return ret;
}

/****************************************************************************
 * Name: uvc_set_video_ctrl
 *
 * Description:
 *   Sets the video control options via the uvc_query_ctrl function.
 *
 * Input Parameters:
 *   stream - reference to stream object
 *   ctrl - reference to control object
 *   probe - reference to probe object
 *
 * Returned Value:
 *   OK on success, appropriate error on failures
 ****************************************************************************/
static inline int uvc_set_video_ctrl(struct uvc_streaming *stream, struct uvc_streaming_control *ctrl, int probe)
{
	uint16_t size = uvc_video_ctrl_size(stream);
	uint8_t *data;
	int ret;

	data = kmm_zalloc(size);
	if (data == NULL) {
		uvdbg("Failed to allocate memory!!\n");
		return -ENOMEM;
	}

	usbhost_uvc_putle16(&data[0], ctrl->bmHint);
	data[2] = ctrl->bFormatIndex;
	data[3] = ctrl->bFrameIndex;
	usbhost_uvc_putle32(&data[4], ctrl->dwFrameInterval);
	usbhost_uvc_putle16(&data[8], ctrl->wKeyFrameRate);
	usbhost_uvc_putle16(&data[10], ctrl->wPFrameRate);
	usbhost_uvc_putle16(&data[12], ctrl->wCompQuality);
	usbhost_uvc_putle16(&data[14], ctrl->wCompWindowSize);
	usbhost_uvc_putle16(&data[16], ctrl->wDelay);
	usbhost_uvc_putle32(&data[18], ctrl->dwMaxVideoFrameSize);
	usbhost_uvc_putle32(&data[22], ctrl->dwMaxPayloadTransferSize);

	if (size >= 34) {
		usbhost_uvc_putle32(&data[26], ctrl->dwClockFrequency);
		data[30] = ctrl->bmFramingInfo;
		data[31] = ctrl->bPreferedVersion;
		data[32] = ctrl->bMinVersion;
		data[33] = ctrl->bMaxVersion;
	}

	ret = uvc_query_ctrl(stream->dev, UVC_SET_CUR, 0, stream->intfnum, probe ? UVC_VS_PROBE_CONTROL : UVC_VS_COMMIT_CONTROL, data, size);
	if (ret != size) {
		uvdbg("Fail to query probe(%)\n", probe);
		ret = -EIO;
	} else {
		ret = OK;
	}

	kmm_free(data);
	return ret;
}

/****************************************************************************
 * Name: uvc_get_video_ctrl
 *
 * Description:
 *   This function retrieves the control settings for the specific control
 *   option of the stream.
 *
 *   This function is called from the UVC_video methods.
 *
 * Input Parameters:
 *   stream - uvc_streaming object
 *   ctrl - UVC stream object control
 *   probe - pointer to probe object
 *   query - query type on stream object control
 *
 * Returned Values:
 *   OK on success, appropriate error code on failure.
 *
 ****************************************************************************/
int uvc_get_video_ctrl(struct uvc_streaming *stream, struct uvc_streaming_control *ctrl, int probe, uint8_t query)
{
	int ret = OK;
	uint8_t *data;
	struct uvc_format *format = NULL;
	struct uvc_frame *frame = NULL;
	uint16_t size = uvc_video_ctrl_size(stream);

	data = kmm_zalloc(size);
	if (data == NULL) {
		uvdbg("Fail to allocate memory\n");
		return -ENOMEM;
	}

	ret = uvc_query_ctrl(stream->dev, query, 0, stream->intfnum, probe ? UVC_VS_PROBE_CONTROL : UVC_VS_COMMIT_CONTROL, data, size);

	if (ret != size) {
		uvdbg("Fail to query probe(%)\n", probe);
		ret = -EIO;
		goto out;
	}

	ctrl->bmHint = usbhost_uvc_getle16((const uint8_t *)&data[0]);
	ctrl->bFormatIndex = data[2];
	ctrl->bFrameIndex = data[3];
	ctrl->dwFrameInterval = usbhost_uvc_getle32((const uint8_t *)&data[4]);
	ctrl->wKeyFrameRate = usbhost_uvc_getle16((const uint8_t *)&data[8]);
	ctrl->wPFrameRate = usbhost_uvc_getle16((const uint8_t *)&data[10]);
	ctrl->wCompQuality = usbhost_uvc_getle16((const uint8_t *)&data[12]);
	ctrl->wCompWindowSize = usbhost_uvc_getle16((const uint8_t *)&data[14]);
	ctrl->wDelay = usbhost_uvc_getle16((const uint8_t *)&data[16]);
	ctrl->dwMaxVideoFrameSize = usbhost_uvc_getle32((const uint8_t *)&data[18]);
	ctrl->dwMaxPayloadTransferSize = usbhost_uvc_getle32((const uint8_t *)&data[22]);

	if (size >= 34) {
		ctrl->dwClockFrequency = usbhost_uvc_getle32((const uint8_t *)&data[26]);
		ctrl->bmFramingInfo = data[30];
		ctrl->bPreferedVersion = data[31];
		ctrl->bMinVersion = data[32];
		ctrl->bMaxVersion = data[33];
	} else {
		ctrl->dwClockFrequency = stream->dev->clock_frequency;
		ctrl->bmFramingInfo = 0;
		ctrl->bPreferedVersion = 0;
		ctrl->bMinVersion = 0;
		ctrl->bMaxVersion = 0;
	}

	/* Get the format object matching the index */
	format = uvc_format_by_index(stream, ctrl->bFormatIndex);

	if (format == NULL) {
		uvdbg("No format matching found!!\n");
		ret = -EINVAL;
		goto out;
	}

	/* Get the frame object matching the index */
	frame = uvc_frame_by_index(format, ctrl->bFrameIndex);

	if (frame == NULL) {
		uvdbg("No frame matching found!!\n");
		ret = -EINVAL;
		goto out;
	}

	if (!(format->flags & UVC_FMT_FLAG_COMPRESSED) || (ctrl->dwMaxVideoFrameSize == 0 && stream->dev->uvc_version < 0x0110)) {
		ctrl->dwMaxVideoFrameSize = frame->dwMaxVideoFrameBufferSize;
	}

	ret = OK;
out:
	kmm_free(data);
	return ret;
}

/****************************************************************************
 * Name: uvc_video_init_transfer
 *
 * Description:
 *   This function will parses though the endpoint descriptors and select the
 *   best one based on max packet and bandwidth requirement for video streaming.
 *
 * Input Parameters:
 *   stream - pointer to stream object.
 *
 * Returned Values:
 *   0 on success, appropriate error code in case of failure.
 *
 ****************************************************************************/
int uvc_video_init_transfer(struct uvc_streaming *stream)
{
	int ret = OK;
	int type;
	int altintf = 0;
	unsigned int i;
	struct usb_inf_desc *intf;
	struct usb_ep_desc *ep;
	struct usbhost_uvc_state_s *dev = stream->dev;

	stream->sequence = -1;
	stream->last_fid = -1;

	if (stream->altinfcount > 1) {
		struct usb_ep_desc *bestep = NULL;
		unsigned int bestpsize = UINT_MAX;
		unsigned int bandwidth;

		/* Isochronous endpoint, select the alternate setting. */
		bandwidth = stream->ctrl.dwMaxPayloadTransferSize;

		if (bandwidth == 0) {
			uvdbg("Requested zero bandwidth, which is wrong!! set to 1\n");
			bandwidth = 1;
		}

		for (i = 0; i < stream->altinfcount; ++i) {
			unsigned int psize;

			intf = uvc_interface_by_number(dev, stream->intfnum, i);
			if (intf == NULL) {
				continue;
			}

			ep = (struct usb_ep_desc *)uvc_endpoint_by_address(intf, stream->header.bEndpointAddress);
			if (ep == NULL) {
				continue;
			}

			/* Check if the bandwidth is high enough. */
			psize = usbhost_uvc_getle16((const uint8_t *)&ep->ep.mxpacketsize);
			if (psize >= bandwidth && psize <= bestpsize) {
				bestpsize = psize;
				bestep = ep;
				altintf = intf->intf.alt;
			}
		}

		if (bestep == NULL) {
			uvdbg("No endpoint with bandwidth support!!\n");
			return -EIO;
		}
		type = 0;

		/* Commit the stream control */
		ret = uvc_set_video_ctrl(stream, &stream->ctrl, 0);
		if (ret < 0) {
			return ret;
		}

		/*Set interface */
		ret = usbhost_uvc_setinterface(dev, stream->intfnum, altintf);
		if (ret < 0) {
			return ret;
		}
		ep = bestep;
	} else {
		/* Bulk endpoint, proceed to initialization. */
		intf = (struct usb_inf_desc *)uvc_interface_by_number(dev, stream->intfnum, 0);
		if (intf == NULL) {
			return -EIO;
		}

		ep = (struct usb_ep_desc *)uvc_endpoint_by_address(intf, stream->header.bEndpointAddress);

		if (ep == NULL) {
			return -EIO;
		}

		/* Commit the stream control */
		ret = uvc_set_video_ctrl(stream, &stream->ctrl, 0);
		if (ret < 0) {
			return ret;
		}

		type = 1;
	}

	ret = uvc_init_video_endpoint(stream, ep, type);

	if (ret < 0) {
		return ret;
	}

	return 0;
}

/****************************************************************************
 * Name: uvc_probe_video
 *
 * Description:
 *   Probe the possible setting of camera device
 *
 * Input Parameters:
 *   stream - reference to stream object
 *   probe - reference to probe object
 *
 * Returned Value:
 *   OK on success, appropriate error on failures
 ****************************************************************************/
int uvc_probe_video(struct uvc_streaming *stream, struct uvc_streaming_control *probe)
{
	int ret;
	unsigned int i;
	uint16_t bandwidth;
	struct uvc_streaming_control probe_min, probe_max;
	struct usbhost_uvc_state_s *dev = stream->dev;

	/* DA Camera need to set interface to zero before changing any settings
	 * Set interface, by default interface 0 would be set on most of the camera
	 * also some device stall the request if you try to set the interface again
	 * lets not return failure in case setinterface fails
	 */
	ret = usbhost_uvc_setinterface(dev, stream->intfnum, 0);
	if (ret < 0) {
		udbg("Failed to configure interface\n");
	}

	ret = uvc_set_video_ctrl(stream, probe, 1);
	if (ret < 0) {
		uvdbg("Fail to set probe parameters!!\n");
		goto done;
	}

	/* Get the minimum and maximum values for compression settings. */
	ret = uvc_get_video_ctrl(stream, &probe_min, 1, UVC_GET_MIN);
	if (ret < 0) {
		uvdbg("Fail to get video min parameters!!\n");
		goto done;
	}

	ret = uvc_get_video_ctrl(stream, &probe_max, 1, UVC_GET_MAX);
	if (ret < 0) {
		uvdbg("Fail to get video max parameters!!\n");
		goto done;
	}

	/* Try and negotiate the image quality */
	probe->wCompQuality = probe_max.wCompQuality;

	for (i = 0; i < 2; ++i) {
		ret = uvc_set_video_ctrl(stream, probe, 1);
		if (ret < 0) {
			uvdbg("Fail to set video parameters!!\n");
			goto done;
		}
		ret = uvc_get_video_ctrl(stream, probe, 1, UVC_GET_CUR);
		if (ret < 0) {
			uvdbg("Fail to get video current parameters!!\n");
			goto done;
		}

		if (stream->altinfcount == 1) {
			break;
		}

		bandwidth = probe->dwMaxPayloadTransferSize;
		if (bandwidth <= stream->maxpsize) {
			break;
		}

		probe->wKeyFrameRate = probe_min.wKeyFrameRate;
		probe->wPFrameRate = probe_min.wPFrameRate;
		probe->wCompQuality = probe_max.wCompQuality;
		probe->wCompWindowSize = probe_min.wCompWindowSize;
	}

done:
	return ret;
}

/****************************************************************************
 * Name: uvc_video_decode
 *
 * Description:
 *   Decode the data recevied on isoc/bulk endpoint based streaming device.
 *
 * Input Parameters:
 *   a\buf - pointer to data buffer
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *   Probably called from an interrupt handler.
 *
 ****************************************************************************/
static inline void uvc_video_decode(struct uvc_buf_object *bobj)
{
	uint8_t fid = 0;
	uint8_t *pbuff;
	uint8_t *trfbuff;
	uint8_t hdrsize;
	uint32_t pktsize;
	FAR struct usbhost_uvc_state_s *dev = (FAR struct usbhost_uvc_state_s *)bobj->context;
	struct uvc_streaming *stream = dev->stream;

	trfbuff = (uint8_t *) bobj->buffer;
	hdrsize = trfbuff[0];
	/* A payload must be at least 2 bytes long, Header Length value must
	 * be at least 2 bytes and can't be larger than the packet size.
	 */
	if (bobj->recsize < 2 || hdrsize < 2 || hdrsize > bobj->recsize) {
		uvdbg("Recevied an invalid payload!!\n");
		goto error;
	}

	fid = trfbuff[1] & UVC_STREAM_FID;

	if (stream->last_fid != fid) {
		stream->sequence++;
		/* Discard the current frame and start fresh */
		stream->trfsize = 0;
		uvdbg("New Frame Detected:%d\n", stream->trfsize);
	}

	/* Mark the buffer as bad if the error bit is set. */
	if (trfbuff[1] & UVC_STREAM_ERR) {
		uvdbg(" Error in received payload.\n");
		/* Discard the current frame and start fresh */
		stream->trfsize = 0;
		goto error;
	}

	pktsize = bobj->recsize - hdrsize;
	if (pktsize > 0) {
		/* Copy the data to user buffer */
		pbuff = (uint8_t *) stream->reqbuff;
		pbuff += stream->trfsize;
		memcpy(pbuff, (trfbuff + hdrsize), pktsize);
		/* Update received size */
		stream->trfsize += pktsize;
		if (stream->trfsize > stream->reqsize) {
			DEBUGASSERT(0);
		}
	}

	if (trfbuff[1] & UVC_STREAM_EOF) {
		uvdbg("EOF Detected:%d\n", stream->trfsize);
		stream->reqsize = 0;
	}

	stream->last_fid = fid;
error:
	return;
}

/****************************************************************************
 * Name: uvc_transfer_thread
 *
 * Description:
 *   Write worker thread which will process the recevied USB data, write the
 *   data from USB transfer buffer to video frame.
 *
 * Input Parameters:
 *   argc - argument countr
 *   argv - array of arguments
 *
 * Returned Value:
 *   Return always OK
 *
 ****************************************************************************/
int uvc_transfer_thread(int argc, char *argv[])
{
	int ret;
	uint32_t datasize;
	usbhost_ep_t inep;
	FAR struct usbhost_uvc_state_s *dev = g_usbuvc_handoff;
	struct uvc_streaming *stream = dev->stream;
	FAR struct usbhost_hubport_s *hport;

	g_usbuvc_handoff = NULL;
	hport = dev->usbclass.hport;
	dev->thstate = USBUVC_STATE_IDLE;
	/* ISOC or BULK? */
	if (stream->bulkin) {
		inep = stream->bulkin;
	} else {
		inep = stream->isocin;
	}

	/* No way we can reach here without an endpoint initialized, lets assert */
	DEBUGASSERT(inep);

	while (sem_wait(&stream->waitsem) == 0) {
		struct uvc_buf_object *bobj = NULL;
		if (dev->disconnected) {
			dev->thstate = USBUVC_STATE_TERMINATED;
			goto errout;
		}

		usbhost_uvc_takesem(&stream->mutex);
		/* De-Queue the transfer object for processing */
		if (uvc_circ_bbuf_pop(&stream->cbuf, &bobj) == -1) {
			uvdbg("No object in circular queue!!\n");
			usbhost_uvc_givesem(&stream->mutex);
			continue;
		}

		/* Check if the transfer is already cancelled by applications */
		if ((stream->active == FALSE) || stream->reqcancel) {
			usbhost_uvc_givesem(&stream->mutex);
			uvdbg("Transfer already cancelled!!\n");
			continue;
		}
		usbhost_uvc_givesem(&stream->mutex);

		usbhost_uvc_takesem(&stream->trfmutex);
		/* Decode the video data */
		uvc_video_decode(bobj);

		/* Setup to receive the next data transfer data to be received */
		ret = DRVR_ASYNCH(hport->drvr, inep, (FAR uint8_t *) bobj->buffer, bobj->bufsize, usbhost_transfer_callback, bobj);
		if (ret < 0) {
			udbg("ERROR: DRVR_ASYNCH failed: %d\n", ret);
			DEBUGASSERT(!ret);
			uvc_circ_bbuf_push(&stream->fbuf, bobj);
		}

		/* Check the user request data size is satisfied? */
		if (stream->reqsize == 0) {
			datasize = stream->trfsize;
			usbhost_uvc_givesem(&stream->trfmutex);
			video_common_notify_dma_done(0, stream->reqtype, datasize, dev->video_priv);
		} else {
			usbhost_uvc_givesem(&stream->trfmutex);
		}
	}

errout:
	return OK;
}

/****************************************************************************
 * Name: usbhost_transfer_callback
 *
 * Description:
 *   This function a callback register with the USB Host driver to receive
 *   transfer completion notifications.
 *
 *   This function is called from the V4L2 method.
 *
 * Input Parameters:
 *   arg - pointer to device object.
 *   nbytes - number of by transfered
 *
 * Returned Values:
 *   None
 *
 ****************************************************************************/
static void usbhost_transfer_callback(FAR void *arg, ssize_t nbytes)
{
	FAR struct uvc_buf_object *bobj = (FAR struct uvc_buf_object *)arg;
	FAR uvc_state_t *dev = (FAR uvc_state_t *) bobj->context;
	struct uvc_streaming *stream = NULL;

	DEBUGASSERT(dev);

	/* Are we still connected? */
	if (!dev->disconnected) {
		stream = dev->stream;
		if (stream != NULL) {
			/* Request is already canceled, we dont have to further process,
			 * lower layer may call the callback and may end up in dead lock
			 */
			if (stream->reqcancel == 1) {
				return;
			}
			DEBUGASSERT(nbytes >= INT16_MIN && nbytes <= INT16_MAX);
			/* Yes.. Was an transfer IN received correctly? */
			usbhost_uvc_takesem(&stream->mutex);
			bobj->recsize = (int16_t) nbytes;

			uvc_circ_bbuf_push(&stream->cbuf, bobj);
			sem_post(&stream->waitsem);

			usbhost_uvc_givesem(&stream->mutex);
		} else {
			video_common_notify_dma_done(-EIO, stream->reqtype, 0, dev->video_priv);
			return;
		}
	}
}

/****************************************************************************
 * Name: uvc_video_start_streaming
 *
 * Description:
 *   This function start a new stream transfer.
 *
 *   This function is called from the V4L2 method.
 *
 * Input Parameters:
 *   stream - Reference to stream object.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
int uvc_video_start_streaming(struct uvc_streaming *stream)
{
	int ret = OK;
	usbhost_ep_t inep;
	struct usbhost_uvc_state_s *dev = stream->dev;
	FAR struct usbhost_hubport_s *hport;
	FAR struct uvc_buf_object *bobj;

	DEBUGASSERT(dev != NULL && dev->usbclass.hport != NULL);
	hport = dev->usbclass.hport;

	usbhost_uvc_takesem(&stream->trfmutex);
	/* Clear off the previous transfer request */
	stream->trfsize = 0;
	usbhost_uvc_givesem(&stream->trfmutex);

	usbhost_uvc_takesem(&stream->mutex);
	/* Clear off the previous transfer request */
	stream->reqcancel = 0;
	/* ISOC or BULK? */
	if (stream->bulkin) {
		inep = stream->bulkin;
	} else {
		inep = stream->isocin;
	}

	if (inep == NULL) {
		uvdbg("ERROR: Endpoint not allocated!!");
		ret = -EIO;
		goto error;
	}
	stream->active = 1;

	do {
		if (uvc_circ_bbuf_pop(&stream->fbuf, &bobj) == -1) {
			break;
		}
		/* Setup transfer request for data to be received */
		ret = DRVR_ASYNCH(hport->drvr, inep, (FAR uint8_t *) bobj->buffer, stream->bufsize, usbhost_transfer_callback, bobj);
		if (ret < 0) {
			udbg("ERROR: DRVR_ASYNCH failed: %d\n", ret);
			uvc_circ_bbuf_push(&stream->fbuf, bobj);
			goto error;
		}
	} while (1);

error:
	usbhost_uvc_givesem(&stream->mutex);

	return ret;
}

/****************************************************************************
 * Name: uvc_video_stop_streaming
 *
 * Description:
 *   This function stop the current stream transfer.
 *
 *   This function is called from the V4L2 method.
 *
 * Input Parameters:
 *   stream - Reference to stream object.
 *
 * Returned Values:
 *   None
 *
 ****************************************************************************/
void uvc_video_stop_streaming(struct uvc_streaming *stream)
{
	int trfactive;
	usbhost_ep_t inep;
	struct usbhost_uvc_state_s *dev = stream->dev;
	FAR struct usbhost_hubport_s *hport;

	DEBUGASSERT(dev != NULL && dev->usbclass.hport != NULL);
	hport = dev->usbclass.hport;

	usbhost_uvc_takesem(&stream->mutex);

	/* ISOC or BULK? */
	if (stream->bulkin) {
		inep = stream->bulkin;
	} else {
		inep = stream->isocin;
	}

	trfactive = stream->active;
	stream->reqcancel = 1;
	stream->active = 0;

	uvc_video_deinit_transfer(stream);

	usbhost_uvc_givesem(&stream->mutex);

	if (trfactive && dev->disconnected) {
		/* TODO: Calling nofify will issue another request */
		video_common_notify_dma_done(-EIO, stream->reqtype, 0, dev->video_priv);
	}
}

/****************************************************************************
 * Name: uvc_video_init
 *
 * Description:
 *   This function initializes the stream, which include the setting the default
 *   format, frame and interval. Bulk/Isoc endpoints and buffers for transfer is
 *   initialized here before hand, user application can request for streaming with
 *   default settings
 *
 *   This function is called from the uvc_register_camera method.
 *
 * Input Parameters:
 *   stream - Reference to stream object.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
int uvc_video_init(struct uvc_streaming *stream)
{
	int ret = OK;
	struct uvc_frame *frame = NULL;
	struct uvc_format *format = NULL;
	struct uvc_streaming_control *probe = &stream->ctrl;
	struct usbhost_uvc_state_s *dev = stream->dev;

	DEBUGASSERT(dev);
	usbhost_uvc_takesem(&stream->mutex);
	sem_init(&stream->trfmutex, 0, 1);
	sem_init(&stream->waitsem, 0, 0);
	/*Initializes the transfer circular buffers */
	uvc_circ_bbuf_init(&stream->cbuf);
	uvc_circ_bbuf_init(&stream->fbuf);

	if (stream->nformats == 0) {
		udbg("No supported video formats found.\n");
		ret = -EINVAL;
		goto errout;
	}

	/* Set interface, by default interface 0 would be set on most of the camera
	 * also some device stall the request if you try to set the interface again
	 * lets not return failure in case setinterface fails
	 */
	ret = usbhost_uvc_setinterface(dev, stream->intfnum, 0);
	if (ret < 0) {
		udbg("Failed to configure interface\n");
	}

	if (uvc_get_video_ctrl(stream, probe, 1, UVC_GET_DEF) == 0) {
		uvc_set_video_ctrl(stream, probe, 1);
	}

	ret = uvc_get_video_ctrl(stream, probe, 1, UVC_GET_CUR);
	if (ret < 0) {
		uvdbg("Fail to get probe parameters!!\n");
		goto errout;
	}

	/* Check if the default format descriptor exists. */
	format = uvc_format_by_index(stream, probe->bFormatIndex);

	if (format == NULL) {
		udbg("No format found !!\n");
		ret = -EINVAL;
		goto errout;
	}

	/* If the default frame descriptor is not found, use the first
	 * available frame.
	 */
	frame = uvc_frame_by_index(format, probe->bFrameIndex);
	if (frame == NULL) {
		udbg("No frame descriptor found !!\n");
		ret = -EINVAL;
		goto errout;
	}

	probe->bFormatIndex = format->index;
	probe->bFrameIndex = frame->bFrameIndex;

	stream->def_format = format;
	stream->cur_format = format;
	stream->cur_frame = frame;

	/* Select the video decoding function */
	if (stream->type != V4L2_BUF_TYPE_VIDEO_CAPTURE) {
		/* Video output devices are not supported */
		udbg("This device does not support any camera feature!!\n");
		ret = -EINVAL;
		goto errout;
	}

	/* Initialize the transfer endpoints, application can call start
	 * transfer without doing the probe, in that case default setting
	 * should be used and transfer endpoints be ready before it
	 */
	ret = uvc_video_init_transfer(stream);
	if (ret < 0) {
		udbg("Fail to initialize stream transfer settings!!\n");
		goto errout;
	}
	/* Create a write worker thread, all transfer object will be processed from
	 * worker thread, including the notifying the user. This thread should run
	 * at priority lesser than the application thread priority
	 */
	g_usbuvc_handoff = dev;
	uvdbg("Starting transfer worker thread\n");
	dev->thpid = kernel_thread("uvctrf", CONFIG_USBUVC_TRF_PRIO, CONFIG_USBUVC_TRF_STACKSIZE, uvc_transfer_thread, NULL);
	if (dev->thpid <= 0) {
		udbg("Fail to create transfer thread!!\n");
		goto errout;
	}

	stream->reqcancel = 0;
	stream->active = 0;

errout:
	usbhost_uvc_givesem(&stream->mutex);
	return ret;
}

void uvc_video_deinit(struct uvc_streaming *stream)
{
	struct usbhost_uvc_state_s *dev = stream->dev;
	/* Stop the current active stream, this will notify the video layer
	 * an error occured in tranfer and will result in application to handle
	 * the error case and close the video driver handle
	 */
	uvc_video_stop_streaming(stream);
	/* Wait for application to close the driver handle */
	usbhost_uvc_takesem(&dev->opsem);
	/* Release all resources taken for video transfer */
	usbhost_uvc_takesem(&stream->mutex);
	uvc_video_deinit_transfer(stream);
	usbhost_uvc_givesem(&stream->mutex);
	/* Post waitsem so that the write thread can exit */
	sem_post(&stream->waitsem);
	sem_destroy(&stream->trfmutex);
	sem_destroy(&stream->waitsem);
}
