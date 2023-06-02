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
#include <tinyara/fs/fs.h>
#include <tinyara/arch.h>
#include <tinyara/wqueue.h>

#include <tinyara/usb/usb.h>
#include <tinyara/usb/usbhost.h>
#include <video/video_halif.h>

#include "usbhost_uvc.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Configuration ************************************************************/

#ifndef CONFIG_SCHED_WORKQUEUE
#warning "Worker thread support is required (CONFIG_SCHED_WORKQUEUE)"
#endif

/* Driver support ***********************************************************/
/* This format is used to construct the /dev/video[n] device driver path.  It
 * defined here so that it will be used consistently in all places.
 */

#define DEV_FORMAT          "/dev/video%d"
#define DEV_NAMELEN         12

extern struct video_devops_s g_uvc_video_devops;

static struct uvc_format_desc uvc_fmts[] = {
	{"RGB565", UVC_GUID_FORMAT_RGBP, V4L2_PIX_FMT_RGB565},
	{"MJPEG", UVC_GUID_FORMAT_MJPEG, V4L2_PIX_FMT_MJPEG},
	{"YUV 4:2:2 (UYVY)", UVC_GUID_FORMAT_UYVY, V4L2_PIX_FMT_UYVY},
	{"YUY2", UVC_GUID_FORMAT_YUY2, V4L2_PIX_FMT_YUY2}
};

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static void uvc_stream_delete(struct uvc_streaming *stream);
static int uvc_register_camera(FAR uvc_state_t *dev);

/* Memory allocation services */

static inline FAR struct usbhost_uvc_state_s *usbhost_uvc_allocclass(void);
static inline void usbhost_uvc_freeclass(FAR struct usbhost_uvc_state_s *usbclass);

/* Device name management */

static int usbhost_uvc_allocdevno(FAR struct usbhost_uvc_state_s *priv);
static void usbhost_uvc_freedevno(FAR struct usbhost_uvc_state_s *priv);
static inline void usbhost_uvc_mkdevname(FAR struct usbhost_uvc_state_s *priv, FAR char *devname);

/* Worker thread actions */
static void usbhost_uvc_destroy(FAR void *arg);

/* Helpers for usbhost_uvc_connect() */

static int usbhost_uvc_cfgdesc(FAR struct usbhost_uvc_state_s *priv, FAR const uint8_t *configdesc, int desclen);
static inline int usbhost_uvc_devinit(FAR struct usbhost_uvc_state_s *priv);

/* Transfer descriptor memory management */
static int usbhost_alloc_buffers(FAR struct usbhost_uvc_state_s *priv);
static void usbhost_free_buffers(FAR struct usbhost_uvc_state_s *priv);

/* struct usbhost_uvc_registry_s methods */

static struct usbhost_class_s *usbhost_uvc_create(FAR struct usbhost_hubport_s *hport, FAR const struct usbhost_id_s *id);

/* struct usbhost_class_s methods */

static int usbhost_uvc_connect(FAR struct usbhost_class_s *usbclass, FAR const uint8_t *configdesc, int desclen);
static int usbhost_uvc_disconnected(FAR struct usbhost_class_s *usbclass);

/* Driver methods -- depend upon the type of TinyAra driver interface exported */

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* This structure provides the registry entry ID information that will  be
 * used to associate the USB class driver to a connected USB device.
 */

static const const struct usbhost_id_s g_id = {
	USB_CLASS_MISC,				/* base -- Must be one of the USB_CLASS_* definitions in usb.h */
	UVC_DEVICE_SUBCLASS,			/* subclass -- depends on the device */
	UVC_DEVICE_PROTOCOL,			/* proto -- depends on the device    */
	0,					/* vid      */
	0					/* pid      */
};

/* This is the USB host storage class's registry entry */

static struct usbhost_registry_s g_uvc = {
	NULL,						/* flink    */
	usbhost_uvc_create,			/* create   */
	1,							/* nids     */
	&g_id						/* id[]     */
};

/* This is the video lower half object */
static struct video_lowerhalf_s g_lower_half = {
	&g_uvc_video_devops,		/* ops */
	NULL						/* dev */
};

/* This is a bitmap that is used to allocate device names /dev/video<x>. */

static uint32_t g_devinuse = 1;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
* Name: uvc_get_colorspace
*
* Description:
*   This is a helper function to return the supported V4L2 color format from
*   UVC_VS_COLORFORMAT descriptor of the UVC.
*
* Input Parameters:
*   clrindex - Color Index.
*
* Returned Values:
*	 0 if no color format found, else return appropriate the V4L2 color format.
*
****************************************************************************/
static uint32_t uvc_get_colorspace(const uint8_t clrindex)
{
	static const uint8_t primarycolor[] = {
		0,
		V4L2_COLORSPACE_SRGB,
		V4L2_COLORSPACE_470_SYSTEM_M,
		V4L2_COLORSPACE_470_SYSTEM_BG,
		V4L2_COLORSPACE_SMPTE170M,
		V4L2_COLORSPACE_SMPTE240M,
	};

	if (clrindex < ARRAY_SIZE(primarycolor)) {
		return primarycolor[clrindex];
	}

	return 0;
}

/****************************************************************************
* Name: uvc_format_by_guid
*
* Description:
*   This is a helper function to search through format list and return the
*   format object which matches the guid of the format.
*
* Input Parameters:
*   guid - GUID object.
*
* Returned Values:
*	 NULL on failure, pointer to the format descriptor object on success.
*
****************************************************************************/
static struct uvc_format_desc *uvc_format_by_guid(const uint8_t guid[16])
{
	unsigned int len = ARRAY_SIZE(uvc_fmts);
	unsigned int i;

	for (i = 0; i < len; ++i) {
		if (memcmp(guid, uvc_fmts[i].guid, 16) == 0) {
			return &uvc_fmts[i];
		}
	}

	return NULL;
}

/****************************************************************************
 * Name: uvc_entity_by_id
 *
 * Description:
 *   This function parse through the entiry list and return the enity which
 *   matches the id passed to the function.
 *
 * Input Parameters:
 *   dev - Pointer to the device object.
 *   id - id of the entity object which to be retrieved.
 *
 * Returned Values:
 *	 NULL on failure, pointer to the entity object on success.
 *
 ****************************************************************************/
static struct uvc_entity *uvc_entity_by_id(FAR uvc_state_t *dev, int id)
{
	struct uvc_entity *entity = queue_entry(dev->entities.head, struct uvc_entity, list);

	for (; &entity->list != (dq_entry_t *)&dev->entities; entity = queue_entry(entity->list.flink, struct uvc_entity, list)) {
		if (entity->id == id) {
			return entity;
		}
	}

	return NULL;
}

/****************************************************************************
 * Name: uvc_entity_by_reference
 *
 * Description:
 *   This function parse through the entiry list and return the enity which
 *   matches the id passed to the function.
 *
 * Input Parameters:
 *   dev - Pointer to the device object.
 *   id - id of the entity object which to be retrieved.
 *   entity - entity from which parse starts.
 *
 * Returned Values:
 *	 NULL on failure, pointer to the entity object on success.
 *
 ****************************************************************************/
static struct uvc_entity *uvc_entity_by_reference(FAR uvc_state_t *dev, int id, struct uvc_entity *entity)
{
	unsigned int i;

	if (entity == NULL) {
		entity = queue_entry(&dev->entities, struct uvc_entity, list);
	}

	entity = queue_entry(entity->list.flink, struct uvc_entity, list);

	for (; &entity->list != (dq_entry_t *)&dev->entities; entity = queue_entry(entity->list.flink, struct uvc_entity, list)) {
		for (i = 0; i < entity->bNrInPins; ++i)
			if (entity->baSourceID[i] == id) {
				return entity;
			}
	}

	return NULL;
}

/****************************************************************************
 * Name: uvc_stream_by_id
 *
 * Description:
 *   This function parse through the stream list and return the stream which
 *   matches the id passed to the function.
 *
 * Input Parameters:
 *   dev - Pointer to the device object.
 *   id - id of the stream object which to be retrieved.
 *
 * Returned Values:
 *	 NULL on failure, pointer to the stream object on success.
 *
 ****************************************************************************/

static struct uvc_streaming *uvc_stream_by_id(FAR uvc_state_t *dev, int id)
{
	struct uvc_streaming *stream = queue_entry(dev->streams.head, struct uvc_streaming, list);

	for (; &stream->list != (dq_entry_t *)&dev->streams; stream = queue_entry(stream->list.flink, struct uvc_streaming, list)) {
		if (stream->header.bTerminalLink == id) {
			return stream;
		}
	}

	return NULL;
}

/****************************************************************************
 * Name: usbhost_uvc_allocclass
 *
 * Description:
 *   This is really part of the logic that implements the create() method
 *   of struct usbhost_registry_s.  This function allocates memory for one
 *   new class instance.
 *
 * Input Parameters:
 *   None
 *
 * Returned Values:
 *   On success, this function will return a non-NULL instance of struct
 *   usbhost_class_s.  NULL is returned on failure; this function will
 *   will fail only if there are insufficient resources to create another
 *   USB host class instance.
 *
 ****************************************************************************/

static inline FAR struct usbhost_uvc_state_s *usbhost_uvc_allocclass(void)
{
	FAR struct usbhost_uvc_state_s *priv;

	DEBUGASSERT(!up_interrupt_context());
	priv = (FAR struct usbhost_uvc_state_s *)kmm_zalloc(sizeof(struct usbhost_uvc_state_s));
	uvdbg("Allocated: %p\n", priv);
	return priv;
}

/****************************************************************************
 * Name: usbhost_uvc_freeclass
 *
 * Description:
 *   Free a class instance previously allocated by usbhost_uvc_allocclass().
 *
 * Input Parameters:
 *   usbclass - A reference to the class instance to be freed.
 *
 * Returned Values:
 *   None
 *
 ****************************************************************************/

static inline void usbhost_uvc_freeclass(FAR struct usbhost_uvc_state_s *usbclass)
{
	DEBUGASSERT(usbclass != NULL);

	/* Free the class instance (perhaps calling sched_kmm_free() in case we are
	 * executing from an interrupt handler.
	 */

	uvdbg("Freeing: %p\n", usbclass);
	kmm_free(usbclass);
}

/****************************************************************************
 * Name: uvc_alloc_entity
 *
 * Description:
 *   Allocate Video class entity.
 *
 * Input Parameters:
 *   type - Type of entity.
 *   id - ID of entity.
 *   num - No of input for the entity
 *   extra_size - Size for source ids
 *
 * Returned Value:
 *   On sucess, zero (OK) is returned.  On failure, an negated errno value
 *   is returned to indicate the nature of the failure.
 *
 ****************************************************************************/
static struct uvc_entity *uvc_alloc_entity(uint16_t type, uint8_t id, unsigned int num, unsigned int extra_size)
{
	struct uvc_entity *entity;
	unsigned int num_inputs;
	unsigned int size;

	num_inputs = (type & UVC_TERM_OUTPUT) ? num : num - 1;
	size = sizeof(*entity) + extra_size + num_inputs;
	entity = kmm_zalloc(size);
	if (entity == NULL) {
		return NULL;
	}

	entity->id = id;
	entity->type = type;
	entity->bNrInPins = num_inputs;
	entity->baSourceID = ((void *)(entity + 1)) + extra_size;

	return entity;
}

/****************************************************************************
 * Name: usbhost_alloc_buffers
 *
 * Description:
 *   Allocate transfer buffer memory.
 *
 * Input Parameters:
 *   priv - A reference to the class instance.
 *
 * Returned Value:
 *   On sucess, zero (OK) is returned.  On failure, an negated errno value
 *   is returned to indicate the nature of the failure.
 *
 ****************************************************************************/

static int usbhost_alloc_buffers(FAR struct usbhost_uvc_state_s *priv)
{
	FAR struct usbhost_hubport_s *hport;
	size_t maxlen;
	int ret;

	DEBUGASSERT(priv != NULL && priv->usbclass.hport != NULL);
	hport = priv->usbclass.hport;

	/* Allocate memory for control requests */

	ret = DRVR_ALLOC(hport->drvr, (FAR uint8_t **) & priv->ctrlreq, &maxlen);
	if (ret < 0) {
		uvdbg("ERROR: DRVR_ALLOC of ctrlreq failed: %d\n", ret);
		goto errout;
	}

	DEBUGASSERT(maxlen >= sizeof(struct usb_ctrlreq_s));

	return OK;

errout:
	usbhost_free_buffers(priv);
	return ret;
}

/****************************************************************************
 * Name: usbhost_free_buffers
 *
 * Description:
 *   Free transfer buffer memory.
 *
 * Input Parameters:
 *   priv - A reference to the class instance.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static void usbhost_free_buffers(FAR struct usbhost_uvc_state_s *priv)
{
	FAR struct usbhost_hubport_s *hport;

	DEBUGASSERT(priv != NULL && priv->usbclass.hport != NULL);
	hport = priv->usbclass.hport;

	if (priv->ctrlreq) {
		(void)DRVR_FREE(hport->drvr, priv->ctrlreq);
	}

	priv->pktsize = 0;
	priv->ctrlreq = NULL;
	priv->inbuf = NULL;
}

/****************************************************************************
 * Name: usbhost_uvc_allocdevno
 *
 * Description:
 *   This function allocate a new device number which can uniquely identify
 *   the current device. A tiny functions to coordinate management of device
 *   names.
 *
 * Input Parameters:
 *   priv - Pointer to driver object.
 *
 * Returned Values:
 *   None
 *
 ****************************************************************************/
static int usbhost_uvc_allocdevno(FAR struct usbhost_uvc_state_s *priv)
{
	irqstate_t flags;
	int devno;

	flags = enter_critical_section();
	for (devno = 0; devno < 26; devno++) {
		uint32_t bitno = 1 << devno;
		if ((g_devinuse & bitno) == 0) {
			g_devinuse |= bitno;
			priv->minor = devno;
			leave_critical_section(flags);
			return OK;
		}
	}

	leave_critical_section(flags);
	return -EMFILE;
}

/****************************************************************************
 * Name: usbhost_uvc_freedevno
 *
 * Description:
 *   This function free the earlier allocated device name on device disconnection
 *   or on failure of enumeration process
 *
 * Input Parameters:
 *   priv - Pointer to driver object.
 *
 * Returned Values:
 *   None
 *
 ****************************************************************************/
static void usbhost_uvc_freedevno(FAR struct usbhost_uvc_state_s *priv)
{
	int devno = priv->minor;

	if (devno >= 0 && devno < 26) {
		irqstate_t flags = enter_critical_section();
		g_devinuse &= ~(1 << devno);
		leave_critical_section(flags);
	}
}

/****************************************************************************
 * Name: usbhost_uvc_mkdevname
 *
 * Description:
 *   This is a helper function create a unique video devname.
 *
 * Input Parameters:
 *   priv - Pointer to driver object.
 *   devname - Pointer to device name.
 *
 * Returned Values:
 *   None
 *
 ****************************************************************************/
static inline void usbhost_uvc_mkdevname(FAR struct usbhost_uvc_state_s *priv, FAR char *devname)
{
	(void)snprintf(devname, DEV_NAMELEN, DEV_FORMAT, priv->minor);
}

/****************************************************************************
 * Name: usbhost_uvc_destroy
 *
 * Description:
 *   The USB device has been disconnected and the reference count on the USB
 *   host class instance has gone to 1.. Time to destroy the USB host class
 *   instance.
 *
 * Input Parameters:
 *   arg - A reference to the class instance to be destroyed.
 *
 * Returned Values:
 *   None
 *
 ****************************************************************************/
static void usbhost_uvc_destroy(FAR void *arg)
{
	FAR struct usbhost_uvc_state_s *dev = (FAR struct usbhost_uvc_state_s *)arg;
	FAR struct usbhost_hubport_s *hport;
	FAR struct usbhost_driver_s *drvr;
	struct uvc_video_chain *cprev, *cnext;
	struct uvc_entity *eprev, *enext;
	struct uvc_streaming *sprev, *snext;
	struct usb_inf_desc *iprev, *inext;
	struct usb_ep_desc *epprev, *epnext;

	DEBUGASSERT(dev != NULL && dev->usbclass.hport != NULL);
	hport = dev->usbclass.hport;

	DEBUGASSERT(hport->drvr);
	drvr = hport->drvr;

	uvdbg("crefs: %d\n", dev->crefs);

	/* Release control objects */
	uvc_ctrl_deinit_device(dev);

	/* Release video resources */
	uvc_video_deinit(dev->stream);

	/* Release chain objects */
	cprev = queue_entry(dev->chains.head, struct uvc_video_chain, list);
	cnext = queue_entry(cprev->list.flink, struct uvc_video_chain, list);

	for (; &cprev->list != (dq_entry_t *)&dev->chains; cprev = cnext, cnext = queue_entry(cprev->list.flink, struct uvc_video_chain, list)) {
		sem_destroy(&cprev->ctrl_mutex);
		kmm_free(cprev);
	}

	/* Release entitiy object */
	eprev = queue_entry(dev->entities.head, struct uvc_entity, list);
	enext = queue_entry(eprev->list.flink, struct uvc_entity, list);

	for (; &eprev->list != (dq_entry_t *)&dev->entities; eprev = enext, enext = queue_entry(eprev->list.flink, struct uvc_entity, list)) {
		kmm_free(eprev);
	}

	/* Release stream object */
	sprev = queue_entry(dev->streams.head, struct uvc_streaming, list);
	snext = queue_entry(sprev->list.flink, struct uvc_streaming, list);

	for (; &sprev->list != (dq_entry_t *)&dev->streams; sprev = snext, snext = queue_entry(sprev->list.flink, struct uvc_streaming, list)) {
		uvc_stream_delete(sprev);
	}

	/* Release interface and endpoint object */
	iprev = queue_entry(dev->config.interface.head, struct usb_inf_desc, list);
	inext = queue_entry(iprev->list.flink, struct usb_inf_desc, list);

	for (; &iprev->list != (dq_entry_t *)&dev->config.interface; iprev = inext, inext = queue_entry(iprev->list.flink, struct usb_inf_desc, list)) {

		epprev = queue_entry(iprev->ep.head, struct usb_ep_desc, list);
		epnext = queue_entry(epprev->list.flink, struct usb_ep_desc, list);
		for (; &epprev->list != (dq_entry_t *)&iprev->ep; epprev = epnext, epnext = queue_entry(epprev->list.flink, struct usb_ep_desc, list)) {
			kmm_free(epprev);
		}
		kmm_free(iprev);
	}

	/* Unregister the driver */
	video_unregister(dev->video_priv);

	/* Release the device name used by this connection */

	usbhost_uvc_freedevno(dev);

	/* Free any transfer buffers */
	usbhost_free_buffers(dev);

	/* Free the function address assigned to this device */

	usbhost_devaddr_destroy(hport, hport->funcaddr);
	hport->funcaddr = 0;

	/* Destroy the semaphores */
	sem_destroy(&dev->exclsem);
	sem_destroy(&dev->opsem);

	/* Disconnect the USB host device */

	DRVR_DISCONNECT(drvr, hport);

	/* And free the class instance.  Hmmm.. this may execute on the worker
	 * thread and the work structure is part of what is getting freed.  That
	 * should be okay because once the work contained is removed from the
	 * queue, it should not longer be accessed by the worker thread.
	 */

	usbhost_uvc_freeclass(dev);
}

/****************************************************************************
 * Name: uvc_scan_chain_entity
 *
 * Description:
 *   This function list of entity from it output to input connection
 *
 * Input Parameters:
 *   chain - Pointer to chain object.
 *   entity - Pointer to entity list.
 *
 * Returned Values:
 *   On success, zero (OK) is returned. On a failure, a negated errno value is
 *   returned indicating the nature of the failure
 *
 ****************************************************************************/
int uvc_scan_chain_entity(struct uvc_video_chain *chain, struct uvc_entity *entity)
{
	switch (UVC_ENTITY_TYPE(entity)) {
	case UVC_VC_EXTENSION_UNIT:
		if (entity->bNrInPins != 1) {
			uvdbg("EU %d has more than 1 input pin.\n", entity->id);
			return -1;
		}
		break;
	case UVC_VC_PROCESSING_UNIT:
		if (chain->processing != NULL) {
			uvdbg("Multiple PU in chain!!.\n");
			return -1;
		}
		chain->processing = entity;
		break;
	case UVC_VC_SELECTOR_UNIT:
		/* Single-input selector units are ignored. */
		if (entity->bNrInPins == 1) {
			break;
		}
		if (chain->selector != NULL) {
			uvdbg("Multiple SU in chain.\n");
			return -1;
		}
		chain->selector = entity;
		break;
	case UVC_ITT_VENDOR_SPECIFIC:
	case UVC_ITT_CAMERA:
	case UVC_ITT_MEDIA_TRANSPORT_INPUT:
	case UVC_TT_STREAMING:
		break;
	default:
		uvdbg("Unsupported entity type 0x%04x\n", UVC_ENTITY_TYPE(entity));
		return -EINVAL;
	}

	/* Add this entity to the chains entity list */
	dq_addfirst((dq_entry_t *)&entity->chain, &chain->entities);
	return 0;
}

/****************************************************************************
 * Name: uvc_stream_delete
 *
 * Description:
 *   The function release all resources related to format/frame allocation while
 *   creating stream instance.
 *
 * Input Parameters:
 *   stream - A reference to stream object to be release.
 *
 * Returned Values:
 *   None
 *
 ****************************************************************************/
static void uvc_stream_delete(struct uvc_streaming *stream)
{
	struct uvc_format *formats = NULL;
	struct uvc_format *format = NULL;
	struct uvc_frame *frames = NULL;
	struct uvc_frame *frame = NULL;
	sem_destroy(&stream->mutex);

	formats = queue_entry(stream->formats.head, struct uvc_format, list);
	format = queue_entry(formats->list.flink, struct uvc_format, list);

	/* Scan through all formats in the current stream and delete one by one */
	for (; &formats->list != (dq_entry_t *)&stream->formats; formats = format, format = queue_entry(format->list.flink, struct uvc_format, list)) {

		frames = queue_entry(formats->frames.head, struct uvc_frame, list);
		frame = queue_entry(frames->list.flink, struct uvc_frame, list);
		/* Scan through all frames in the current format and delete one by one */
		for (; &frames->list != (dq_entry_t *)&formats->frames; frames = frame, frame = queue_entry(frame->list.flink, struct uvc_frame, list)) {
			kmm_free(frames);
		}
		kmm_free(formats);
	}
	/* Release the bmcontrol and the stream object */
	kmm_free(stream->header.bmaControls);
	kmm_free(stream);
}

/****************************************************************************
 * Name: uvc_scan_chain_forward
 *
 * Description:
 *   This function list of entity from it output to input connection
 *
 * Input Parameters:
 *   chain - Pointer to chain object.
 *   entity - Pointer to entity list.
 *   prev - Pointer to previous entity object.
 *
 * Returned Values:
 *   On success, zero (OK) is returned. On a failure, a negated errno value is
 *   returned indicating the nature of the failure
 *
 ****************************************************************************/
int uvc_scan_chain_forward(struct uvc_video_chain *chain, struct uvc_entity *entity, struct uvc_entity *prev)
{
	struct uvc_entity *forward;
	/* Forward scan */
	forward = NULL;

	while (1) {
		forward = uvc_entity_by_reference(chain->dev, entity->id, forward);
		if (forward == NULL) {
			break;
		}
		if (forward == prev) {
			continue;
		}

		switch (UVC_ENTITY_TYPE(forward)) {
		case UVC_VC_EXTENSION_UNIT:
			if (forward->bNrInPins != 1) {
				uvdbg("EU %d has more than 1 input pin.\n", entity->id);
				return -EINVAL;
			}

			dq_addfirst((dq_entry_t *)&forward->chain, &chain->entities);
			break;

		case UVC_OTT_VENDOR_SPECIFIC:
		case UVC_OTT_DISPLAY:
		case UVC_OTT_MEDIA_TRANSPORT_OUTPUT:
		case UVC_TT_STREAMING:
			if (UVC_ENTITY_IS_ITERM(forward)) {
				uvdbg("Unsupported IT %u \n", forward->id);
				return -EINVAL;
			}

			dq_addfirst((dq_entry_t *)&forward->chain, &chain->entities);
			break;
		}
	}

	return 0;
}

/****************************************************************************
 * Name: uvc_scan_chain_backward
 *
 * Description:
 *   This function list of entity from it output to input connection
 *
 * Input Parameters:
 *   chain - Pointer to chain object.
 *   next - Address to a pointer to entity list.
 *
 * Returned Values:
 *   On success, zero (OK) is returned. On a failure, a negated errno value is
 *   returned indicating the nature of the failure
 *
 ****************************************************************************/
int uvc_scan_chain_backward(struct uvc_video_chain *chain, struct uvc_entity **next)
{
	struct uvc_entity *entity = *next;
	struct uvc_entity *term;
	int id = -EINVAL, i;

	switch (UVC_ENTITY_TYPE(entity)) {
	case UVC_VC_EXTENSION_UNIT:
	case UVC_VC_PROCESSING_UNIT:
	case UVC_TT_STREAMING:
		id = entity->baSourceID[0];
		break;
	case UVC_ITT_VENDOR_SPECIFIC:
	case UVC_ITT_CAMERA:
	case UVC_ITT_MEDIA_TRANSPORT_INPUT:
		id = 0;
		break;
	case UVC_VC_SELECTOR_UNIT:
		/* Single-input selector units are ignored. */
		if (entity->bNrInPins == 1) {
			id = entity->baSourceID[0];
			break;
		}

		chain->selector = entity;
		for (i = 0; i < entity->bNrInPins; ++i) {
			id = entity->baSourceID[i];
			term = uvc_entity_by_id(chain->dev, id);
			if (term == NULL || !UVC_ENTITY_IS_ITERM(term)) {
				uvdbg("SU %d input %d not connected to any IT\n", entity->id, i);
				return -EINVAL;
			}

			dq_addfirst((dq_entry_t *)&term->chain, &chain->entities);
			uvc_scan_chain_forward(chain, term, entity);
		}
		id = 0;
		break;
	}

	if (id <= 0) {
		*next = NULL;
		return id;
	}

	entity = uvc_entity_by_id(chain->dev, id);
	if (entity == NULL) {
		uvdbg("Unknown entity %d.\n", id);
		return -EINVAL;
	}

	*next = entity;
	return 0;
}

/****************************************************************************
 * Name: uvc_scan_chain
 *
 * Description:
 *   This function list of entity from it output to input connection
 *
 * Input Parameters:
 *   chain - Pointer to chain object.
 *   term - Pointer to entity list.
 *
 * Returned Values:
 *   On success, zero (OK) is returned. On a failure, a negated errno value is
 *   returned indicating the nature of the failure
 *
 ****************************************************************************/
int uvc_scan_chain(struct uvc_video_chain *chain, struct uvc_entity *term)
{
	struct uvc_entity *entity, *prev;

	entity = term;
	prev = NULL;

	while (entity != NULL) {
		/* Entity must not be part of an existing chain */
		if (entity->chain.head || entity->chain.tail) {
			uvdbg("Entity %d already in chain.\n", entity->id);
			return -EINVAL;
		}

		/* Process entity */
		if (uvc_scan_chain_entity(chain, entity) < 0) {
			return -EINVAL;
		}

		/* Forward scan */
		if (uvc_scan_chain_forward(chain, entity, prev) < 0) {
			return -EINVAL;
		}

		/* Backward scan */
		prev = entity;
		if (uvc_scan_chain_backward(chain, &entity) < 0) {
			return -EINVAL;
		}
	}

	return OK;
}

/****************************************************************************
 * Name: uvc_scan_stream
 *
 * Description:
 *   This function stream descriptor units and update the number of alternate
 *   interfaces available, update the max packets size supported.
 *
 * Input Parameters:
 *   dev - The USB host class instance.
 *
 * Returned Values:
 *   On success, zero (OK) is returned. On a failure, a negated errno value is
 *   returned indicating the nature of the failure
 *
 ****************************************************************************/
int uvc_scan_stream(FAR uvc_state_t *dev)
{
	uint16_t psize;
	struct usb_inf_desc *intf;
	struct usb_ep_desc *ep;
	struct uvc_streaming *stream = queue_entry(dev->streams.head, struct uvc_streaming, list);

	for (; &stream->list != (dq_entry_t *)&dev->streams; stream = queue_entry(stream->list.flink, struct uvc_streaming, list)) {
		/* Parse the alternate settings to find the maximum bandwidth. */
		DEBUGASSERT(stream->intfnum <= dev->config.cfg.ninterfaces);
		intf = queue_entry(dev->config.interface.head, struct usb_inf_desc, list);

		for (; &intf->list != (dq_entry_t *)&dev->config.interface; intf = queue_entry(intf->list.flink, struct usb_inf_desc, list)) {
			if (intf->intf.ifno != stream->intfnum) {
				continue;
			}
			ep = queue_entry(intf->ep.head, struct usb_ep_desc, list);
			for (; &ep->list != (dq_entry_t *)&intf->ep; ep = queue_entry(ep->list.flink, struct usb_ep_desc, list)) {
				if (stream->header.bEndpointAddress != ep->ep.addr) {
					continue;
				}
				/* Update the available alternative interfaces count */
				stream->altinfcount++;
				psize = usbhost_uvc_getle16((const uint8_t *)&ep->ep.mxpacketsize);
				psize = (psize & 0x07ff) * (1 + ((psize >> 11) & 3));
				if (psize > stream->maxpsize) {
					stream->maxpsize = psize;
				}
			}
		}
	}

	return OK;
}

/****************************************************************************
 * Name: uvc_parse_vs_descriptor
 *
 * Description:
 *   This function implements the Video Stream descriptor parsing and create
 *   the list of available supported formats and its frames.
 *
 * Input Parameters:
 *   dev - The USB host class instance.
 *   buffer - A pointer to a buffer container the VC descriptor.
 *   buflen - The length in bytes of the VC descriptor.
 *   infno - Current interface number.
 *
 * Returned Values:
 *   On success, zero (OK) is returned. On a failure, a negated errno value is
 *   returned indicating the nature of the failure
 *
 * Assumptions:
 *   This function will *not* be called from an interrupt handler.
 *
 ****************************************************************************/
int uvc_parse_vs_descriptor(FAR uvc_state_t *dev, const unsigned char *buffer, int buflen, uint8_t infno)
{
	struct uvc_streaming *streaming = NULL;
	struct uvc_format_desc *fmtdesc;
	struct uvc_format *format;
	struct uvc_frame *frame;
	unsigned int size, i, n, p;
	int ret = -EINVAL;
	uint32_t interval = 0;

	if (buflen <= 2) {
		uvdbg("Invalid streaming interface descriptor.\n");
		return -EINVAL;
	}

	/* Parse the header descriptor. */
	switch (buffer[2]) {
	case UVC_VS_OUTPUT_HEADER:
		/* Not supporting */
		break;

	case UVC_VS_INPUT_HEADER:
		size = 13;
		p = buflen >= 4 ? buffer[3] : 0;
		n = buflen >= size ? buffer[size - 1] : 0;

		if (buflen < size + p * n) {
			uvdbg("Input descriptor is invalid.\n");
			goto error;
		}

		streaming = kmm_zalloc(sizeof(*streaming));
		if (streaming == NULL) {
			uvdbg("Memory allocation failure!!\n");
			return -ENOMEM;
		}

		INIT_QUEUE_HEAD(&streaming->formats);
		sem_init(&streaming->mutex, 0, 1);

		streaming->dev = dev;
		streaming->intfnum = infno;
		streaming->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		streaming->header.bNumFormats = p;
		streaming->header.bEndpointAddress = buffer[6];
		streaming->header.bmInfo = buffer[7];
		streaming->header.bTerminalLink = buffer[8];
		streaming->header.bStillCaptureMethod = buffer[9];
		streaming->header.bTriggerSupport = buffer[10];
		streaming->header.bTriggerUsage = buffer[11];
		streaming->header.bControlSize = n;

		streaming->header.bmaControls = kmm_zalloc(n);
		if (streaming->header.bmaControls == NULL) {
			ret = -ENOMEM;
			uvdbg("Memory allocation failure!!\n");
			goto error;
		}

		memcpy(streaming->header.bmaControls, &buffer[size], p * n);
		dq_addfirst(&streaming->list, &dev->streams);
		break;
	case UVC_VS_FORMAT_UNCOMPRESSED:
	case UVC_VS_FORMAT_FRAME_BASED:
		n = buffer[2] == UVC_VS_FORMAT_UNCOMPRESSED ? 27 : 28;
		if (buflen < n) {
			return -EINVAL;
		}
		/* Get the current stream from head and add the format to it */
		streaming = queue_entry(dev->streams.head, struct uvc_streaming, list);
		if (streaming == NULL) {
			uvdbg("No valid stream found!!\n");
			return -EINVAL;
		}
		format = kmm_zalloc(sizeof(*format));
		if (format == NULL) {
			uvdbg("Memory allocation failure!!\n");
			return -ENOMEM;
		}
		INIT_QUEUE_HEAD(&format->frames);
		format->type = buffer[2];
		format->index = buffer[3];
		/* Find the format descriptor from its GUID. */
		fmtdesc = (struct uvc_format_desc *)uvc_format_by_guid(&buffer[5]);

		if (fmtdesc != NULL) {
			format->fcc = fmtdesc->fcc;
			snprintf(format->name, sizeof(format->name), "%s", fmtdesc->name);
		} else {
			uvdbg("Unknown video format %pUl\n", &buffer[5]);
			snprintf(format->name, sizeof(format->name), "%pUl", &buffer[5]);
			format->fcc = 0;
		}

		format->bpp = buffer[21];

		if (buffer[2] == UVC_VS_FORMAT_UNCOMPRESSED) {
			format->ftype = UVC_VS_FRAME_UNCOMPRESSED;
		} else {
			format->ftype = UVC_VS_FRAME_FRAME_BASED;
			if (buffer[27]) {
				format->flags = UVC_FMT_FLAG_COMPRESSED;
			}
		}

		dq_addfirst(&format->list, &streaming->formats);
		streaming->nformats++;
		break;

	case UVC_VS_FORMAT_MJPEG:
		if (buflen < 11) {
			uvdbg("Not a valid MPEG header\n");
			return -EINVAL;
		}
		/* Get the current stream and add the format to it */
		streaming = queue_entry(dev->streams.head, struct uvc_streaming, list);
		if (streaming == NULL) {
			uvdbg("No valid stream found!!\n");
			return -EINVAL;
		}

		format = kmm_zalloc(sizeof(*format));
		if (format == NULL) {
			udbg("Memory allocation failure!!\n");
			return -ENOMEM;
		}

		INIT_QUEUE_HEAD(&format->frames);
		format->type = buffer[2];
		format->index = buffer[3];
		format->fcc = V4L2_PIX_FMT_MJPEG;
		format->flags = UVC_FMT_FLAG_COMPRESSED;
		format->bpp = 0;
		format->ftype = UVC_VS_FRAME_MJPEG;
		snprintf(format->name, sizeof(format->name), "MJPEG");
		dq_addfirst(&format->list, &streaming->formats);
		streaming->nformats++;
		break;
	case UVC_VS_FORMAT_DV:
	case UVC_VS_FORMAT_MPEG2TS:
	case UVC_VS_FORMAT_STREAM_BASED:
		/* Not supported yet. */
		break;
	case UVC_VS_FRAME_UNCOMPRESSED:
	case UVC_VS_FRAME_MJPEG:
	case UVC_VS_FRAME_FRAME_BASED:
		if (buflen < 21) {
			return -EINVAL;
		}

		/* Get the current stream and add the format to it */
		streaming = queue_entry(dev->streams.head, struct uvc_streaming, list);
		if (streaming == NULL) {
			uvdbg("No valid stream found!!\n");
			return -EINVAL;
		}

		format = queue_entry(streaming->formats.head, struct uvc_format, list);
		if (format == NULL) {
			uvdbg("No valid format found!!\n");
			return -EINVAL;
		}
		/* Check frame type matches with the one in format */
		if (buffer[2] == format->ftype) {
			if (format->ftype != UVC_VS_FRAME_FRAME_BASED) {
				n = buflen > 25 ? buffer[25] : 0;
			} else {
				n = buflen > 21 ? buffer[21] : 0;
			}

			n = n ? n : 3;

			if (buflen < 26 + 4 * n) {
				udbg("Invalid Frame Descriptor\n");
				return -EINVAL;
			}

			frame = kmm_zalloc(sizeof(*frame) + sizeof(uint32_t) * n);
			if (frame == NULL) {
				uvdbg("Memory allocation failure!!\n");
				return -ENOMEM;
			}
			frame->bFrameIndex = buffer[3];
			frame->bmCapabilities = buffer[4];
			frame->wWidth = usbhost_uvc_getle16((const uint8_t *)&buffer[5]);
			frame->wHeight = usbhost_uvc_getle16((const uint8_t *)&buffer[7]);
			frame->dwMinBitRate = usbhost_uvc_getle32((const uint8_t *)&buffer[9]);
			frame->dwMaxBitRate = usbhost_uvc_getle32((const uint8_t *)&buffer[13]);
			if (format->ftype != UVC_VS_FRAME_FRAME_BASED) {
				frame->dwMaxVideoFrameBufferSize = usbhost_uvc_getle32((const uint8_t *)&buffer[17]);
				frame->dwDefaultFrameInterval = usbhost_uvc_getle32((const uint8_t *)&buffer[21]);
				frame->bFrameIntervalType = buffer[25];
			} else {
				frame->dwMaxVideoFrameBufferSize = 0;
				frame->dwDefaultFrameInterval = usbhost_uvc_getle32((const uint8_t *)&buffer[17]);
				frame->bFrameIntervalType = buffer[21];
			}

			for (i = 0; i < n; ++i) {
				interval = usbhost_uvc_getle32((const uint8_t *)&buffer[26 + 4 * i]);
				frame->dwFrameInterval[i] = interval ? interval : 1;
			}

			if (!(format->flags & UVC_FMT_FLAG_COMPRESSED)) {
				frame->dwMaxVideoFrameBufferSize = format->bpp * frame->wWidth * frame->wHeight / 8;
			}

			/* Make sure that the default frame interval stays between
			 * the boundaries.
			 */
			n -= frame->bFrameIntervalType ? 1 : 2;
			frame->dwDefaultFrameInterval = min_t(frame->dwFrameInterval[n], max_t(frame->dwFrameInterval[0], frame->dwDefaultFrameInterval));

			dq_addfirst(&frame->list, &format->frames);
			format->nframes++;
		}
		break;
	case UVC_VS_FRAME_H264:
	case UVC_VS_FRAME_VP8:
		/* Not supported yet */
		break;
	case UVC_VS_COLORFORMAT:
		if (buflen < 6) {
			uvdbg("Invalid color format descriptor\n");
			return -EINVAL;
		}
		/* Get the current stream and format to it the color format */
		streaming = queue_entry(dev->streams.head, struct uvc_streaming, list);
		if (streaming == NULL) {
			uvdbg("No valid stream found!!");
			return -EINVAL;
		}

		format = queue_entry(streaming->formats.head, struct uvc_format, list);
		if (format == NULL) {
			uvdbg("No valid format found!!");
			return -EINVAL;
		}
		format->colorspace = uvc_get_colorspace(buffer[3]);
		break;
	default:
		uvdbg("Invalid Descriptor!!\n");
		break;
	}

	return 0;

error:
	uvc_stream_delete(streaming);
	return ret;
}

/****************************************************************************
 * Name: uvc_parse_vc_descriptor
 *
 * Description:
 *   This function implements the Video Control descriptor parsing and create
 *   necessary control units on which camera control works.
 *
 * Input Parameters:
 *   dev - The USB host class instance.
 *   buffer - A pointer to a buffer container the VC descriptor.
 *   buflen - The length in bytes of the VC descriptor.
 *
 * Returned Values:
 *   On success, zero (OK) is returned. On a failure, a negated errno value is
 *   returned indicating the nature of the failure
 *
 * Assumptions:
 *   This function will *not* be called from an interrupt handler.
 *
 ****************************************************************************/
int uvc_parse_vc_descriptor(FAR uvc_state_t *dev, const unsigned char *buffer, int buflen)
{
	struct uvc_entity *unit, *term;
	unsigned int n, p, len;
	uint16_t type;

	switch (buffer[2]) {
	case UVC_VC_HEADER: {
		struct uvc_hdesc_s *vchdr = (struct uvc_hdesc_s *)buffer;

		len = vchdr->bLength;

		if (buflen < len) {
			uvdbg("UVC_VC_HEADER Invalid Header, length does not match\n");
			return -EINVAL;
		}

		dev->uvc_version = usbhost_uvc_getle16((const uint8_t *)&vchdr->bcdUVC);
		dev->clock_frequency = usbhost_uvc_getle32((const uint8_t *)&vchdr->dwClockFrequency);
		break;
	}
	case UVC_VC_INPUT_TERMINAL: {
		struct uvc_itdesc_s *itdes = (struct uvc_itdesc_s *)buffer;
		if (buflen < itdes->bLength) {
			uvdbg("UVC_VC_INPUT_TERMINAL Invalid Header, length does not match\n");
			return -EINVAL;
		}

		type = usbhost_uvc_getle16((const uint8_t *)&itdes->wTerminalType);
		if (type != UVC_ITT_CAMERA) {
			uvdbg("UVC_VC_INPUT_TERMINAL Support Only Camera Type\n");
			return -EINVAL;
		}

		/* Reject invalid terminal types that would cause issues */
		if ((type & 0x7f00) == 0 || (type & 0x8000) != 0) {
			uvdbg("Invalid input terminal type(%d)\n", type);
			return 0;
		}

		/* Allocate and initialize the input terminal entity */
		term = uvc_alloc_entity(type | UVC_TERM_INPUT, itdes->bTerminalID, 1, itdes->bControlSize);
		if (term == NULL) {
			udbg("Memory allocation failure!!\n");
			return -ENOMEM;
		}

		term->camera.bControlSize = itdes->bControlSize;
		term->camera.bmControls = (uint8_t *) term + sizeof(*term);
		term->camera.wObjectiveFocalLengthMin = usbhost_uvc_getle16((const uint8_t *)&itdes->wObjectiveFocalLengthMin);
		term->camera.wObjectiveFocalLengthMax = usbhost_uvc_getle16((const uint8_t *)&itdes->wObjectiveFocalLengthMax);
		term->camera.wOcularFocalLength = usbhost_uvc_getle16((const uint8_t *)&itdes->wOcularFocalLength);
		memcpy(term->camera.bmControls, &itdes->bmControls, itdes->bControlSize);

		sprintf(term->name, "Camera %u", itdes->bTerminalID);

		dq_addfirst(&term->list, &dev->entities);
		break;
	}
	case UVC_VC_OUTPUT_TERMINAL: {
		struct uvc_otdesc_s *otdes = (struct uvc_otdesc_s *)buffer;
		if (buflen < otdes->bLength) {
			uvdbg("UVC_VC_OUTPUT_TERMINAL Invalid Header, length does not match\n");
			return -EINVAL;
		}

		type = usbhost_uvc_getle16((const uint8_t *)&otdes->wTerminalType);
		/* Make sure the terminal type MSB is not null, otherwise it
		 * could be confused with a unit.
		 */
		if ((type & 0xff00) == 0) {
			uvdbg("Invalid input terminal type(%d)\n", type);
			return 0;
		}

		term = uvc_alloc_entity(type | UVC_TERM_OUTPUT, otdes->bTerminalID, 1, 0);
		if (term == NULL) {
			udbg("Memory allocation failure!!\n");
			return -ENOMEM;
		}

		memcpy(term->baSourceID, &otdes->bSourceID, 1);

		sprintf(term->name, "Output %u", otdes->bTerminalID);

		dq_addfirst(&term->list, &dev->entities);
		break;
	}
	case UVC_VC_SELECTOR_UNIT: {
		struct uvc_seldesc_s *seldesc = (struct uvc_seldesc_s *)buffer;
		if (buflen < seldesc->bLength) {
			uvdbg("UVC_VC_SELECTOR_UNIT Invalid Header, length does not match\n");
			return -EINVAL;
		}
		p = seldesc->bNrInPins;
		unit = uvc_alloc_entity(seldesc->bDescriptorType, seldesc->bUnitID, p + 1, 0);
		if (unit == NULL) {
			udbg("Memory allocation failure!!\n");
			return -ENOMEM;
		}

		memcpy(unit->baSourceID, &seldesc->bSourceID, p);

		sprintf(unit->name, "Selector %u", seldesc->bUnitID);

		dq_addfirst(&unit->list, &dev->entities);
		break;
	}
	case UVC_VC_PROCESSING_UNIT: {
		struct uvc_procdesc_s *procdesc = (struct uvc_procdesc_s *)buffer;

		if (buflen < procdesc->bLength) {
			uvdbg("UVC_VC_PROCESSING_UNIT Invalid Header, length does not match\n");
			return -EINVAL;
		}

		n = buflen >= 8 ? procdesc->bControlSize : 0;

		unit = uvc_alloc_entity(procdesc->bDescriptorSubtype, procdesc->bUnitID, 2, n);
		if (unit == NULL) {
			udbg("Memory allocation failure!!\n");
			return -ENOMEM;
		}

		memcpy(unit->baSourceID, &procdesc->bSourceID, 1);
		unit->processing.wMaxMultiplier = usbhost_uvc_getle16((const uint8_t *)&procdesc->wMaxMultiplier);
		unit->processing.bControlSize = procdesc->bControlSize;
		unit->processing.bmControls = (uint8_t *) unit + sizeof(*unit);
		memcpy(unit->processing.bmControls, &procdesc->bmControls, n);

		sprintf(unit->name, "Processing %u", procdesc->bUnitID);

		dq_addfirst(&unit->list, &dev->entities);
		break;
	}
	case UVC_VC_EXTENSION_UNIT: {
		struct uvc_extdesc_s *extdesc = (struct uvc_extdesc_s *)buffer;
		if (buflen < extdesc->bLength) {
			uvdbg("UVC_VC_EXTENSION_UNIT Invalid Header, length does not match\n");
			return -EINVAL;
		}

		p = buflen >= 22 ? extdesc->bNrInPins : 0;
		n = buflen >= 24 + p ? buffer[22 + p] : 0;

		unit = uvc_alloc_entity(extdesc->bDescriptorSubtype, extdesc->bUnitID, p + 1, n);
		if (unit == NULL) {
			udbg("Memory allocation failure!!\n");
			return -ENOMEM;
		}

		memcpy(unit->extension.guidExtensionCode, &extdesc->guidExtensionCode, 16);
		unit->extension.bNumControls = buffer[20];
		memcpy(unit->baSourceID, &buffer[22], p);
		unit->extension.bControlSize = buffer[22 + p];
		unit->extension.bmControls = (uint8_t *) unit + sizeof(*unit);
		memcpy(unit->extension.bmControls, &buffer[23 + p], n);

		sprintf(unit->name, "Extension %u", extdesc->bUnitID);

		dq_addfirst(&unit->list, &dev->entities);
		break;
	}
	default:
		udbg("Unknown Descriptor :%x\n", buffer[2]);
		break;
	}

	return OK;
}

/****************************************************************************
 * Name: usbhost_uvc_cfgdesc
 *
 * Description:
 *   This function implements the connect() method of struct
 *   usbhost_class_s.  This method is a callback into the class
 *   implementation.  It is used to provide the device's configuration
 *   descriptor to the class so that the class may initialize properly
 *
 * Input Parameters:
 *   priv - The USB host class instance.
 *   configdesc - A pointer to a uint8_t buffer container the configuration
 *     descriptor.
 *   desclen - The length in bytes of the configuration descriptor.
 *
 * Returned Values:
 *   On success, zero (OK) is returned. On a failure, a negated errno value is
 *   returned indicating the nature of the failure
 *
 * Assumptions:
 *   This function will *not* be called from an interrupt handler.
 *
 ****************************************************************************/

static int usbhost_uvc_cfgdesc(FAR struct usbhost_uvc_state_s *priv, FAR const uint8_t *configdesc, int desclen)
{
	int ret = OK;
	int remaining;
	uint8_t bIfSubClass = 0;
	uint8_t bIfNumber = 0;
	uint8_t bIfClass = 0;
	FAR struct usb_cfgdesc_s *cfgdesc;
	FAR struct usb_desc_s *desc;
	FAR struct usb_inf_desc *intf;
	FAR struct usb_ep_desc *ep;

	DEBUGASSERT(priv != NULL && priv->usbclass.hport && configdesc != NULL && desclen >= sizeof(struct usb_cfgdesc_s));

	/* Verify that we were passed a configuration descriptor */
	cfgdesc = (FAR struct usb_cfgdesc_s *)configdesc;
	if (cfgdesc->type != USB_DESC_TYPE_CONFIG) {
		udbg("Not a valid configuration descriptor!!\n");
		return -EINVAL;
	}

	/* Get the total length of the configuration descriptor (little endian).
	 * It might be a good check to get the number of interfaces here too.
	 */
	remaining = (int)usbhost_uvc_getle16((const uint8_t *)cfgdesc->totallen);

	/* Loop where there are more dscriptors to examine */
	while (remaining >= sizeof(struct usb_desc_s)) {
		/* What is the next descriptor? */
		desc = (FAR struct usb_desc_s *)configdesc;
		switch (desc->type) {
		case USB_DESC_TYPE_CONFIG: {
			FAR struct usb_cfgdesc_s *cfdesc = (FAR struct usb_cfgdesc_s *)configdesc;

			uvdbg("Configuration descriptor\n");

			/* Copy the configuration descriptor and allocate interfaces descriptors */
			memcpy(&priv->config, cfdesc, sizeof(struct usb_cfgdesc_s));
			INIT_QUEUE_HEAD(&priv->config.interface);
		}
		break;
		case USB_DESC_TYPE_INTERFACEASSOCIATION: {
			uvdbg("Interface Association descriptor\n");
		}
		break;
		/* Interface descriptor. We really should get the number of endpoints
		 * from this descriptor too.
		 */

		case USB_DESC_TYPE_INTERFACE: {
			FAR struct usb_ifdesc_s *ifdesc = (FAR struct usb_ifdesc_s *)configdesc;

			uvdbg("Interface descriptor\n");
			DEBUGASSERT(remaining >= USB_SIZEOF_IFDESC);

			/* Save the interface number */
			bIfClass = ifdesc->classid;
			bIfSubClass = ifdesc->subclass;
			bIfNumber = ifdesc->ifno;

			intf = kmm_zalloc(sizeof(*intf));
			if (intf == NULL) {
				udbg("Memory allocation failure!!\n");
				return -ENOMEM;
			}
			memcpy(intf, ifdesc, sizeof(struct usb_ifdesc_s));
			INIT_QUEUE_HEAD(&intf->ep);

			dq_addfirst(&intf->list, &priv->config.interface);

			priv->config.ifcount++;
		}
		break;

		case USB_DESC_TYPE_CS_INTERFACE: {
			uvdbg("CS Interface descriptor\n");
			if ((bIfClass == CC_VIDEO) && (bIfSubClass == USB_SUBCLASS_VIDEOCONTROL)) {
				priv->intfnum = bIfNumber;
				ret = uvc_parse_vc_descriptor(priv, (const unsigned char *)desc, desc->len);
			} else if ((bIfClass == CC_VIDEO) && (bIfSubClass == USB_SUBCLASS_VIDEOSTREAMING)) {
				ret = uvc_parse_vs_descriptor(priv, (const unsigned char *)desc, remaining, bIfNumber);
			}
			if (ret < 0) {
				return -EINVAL;
			}
			/* TODO AUDIO CONTROL/STREAMING */
		}
		break;

		case USB_DESC_TYPE_CSENDPOINT: {
			uvdbg("CS Endpoint Interface descriptor\n");
		}
		break;

		/* Endpoint descriptor.  Here, we expect one IN ISO endpoint and one IN INT endpoint */

		case USB_DESC_TYPE_ENDPOINT: {
			FAR struct usb_epdesc_s *epdesc = (FAR struct usb_epdesc_s *)configdesc;

			uvdbg("Endpoint descriptor %d\n", epdesc->attr);
			DEBUGASSERT(remaining >= USB_SIZEOF_EPDESC);

			ep = kmm_zalloc(sizeof(*ep));
			if (ep == NULL) {
				udbg("Memory allocation failure!!\n");
				return -ENOMEM;
			}

			memcpy(ep, epdesc, sizeof(struct usb_epdesc_s));

			DEBUGASSERT(intf);

			dq_addfirst(&ep->list, &intf->ep);
		}
		break;

		/* Other descriptors are just ignored for now */

		default:
			uvdbg("Unknown descriptor %d\n", desc->type);
			break;
		}

		/* Increment the address of the next descriptor */

		configdesc += desc->len;
		remaining -= desc->len;
	}

	return OK;
}

/****************************************************************************
 * Name: uvc_scan_device
 *
 * Description:
 *   The function is used to scan all the entities created on UVC descriptor
 *   parsing and assign them to proper terminal(in/out).
 *
 *   This function is called from the usbhost_uvc_devinit() method.
 *
 * Input Parameters:
 *   dev - A reference to the class instance.
 *
 * Returned Values:
 *   On success, zero (OK) is returned. On a failure, a negated errno value is
 *   returned indicating the nature of the failure
 *
 ****************************************************************************/
int uvc_scan_device(FAR uvc_state_t *dev)
{
	struct uvc_video_chain *chain;
	struct uvc_entity *term = queue_entry(dev->entities.head, struct uvc_entity, list);

	/* Parse through the entity and add them to approprate terminal(in/out) */
	for (; &term->list != (dq_entry_t *)&dev->entities; term = queue_entry(term->list.flink, struct uvc_entity, list)) {
		if (!UVC_ENTITY_IS_OTERM(term)) {
			continue;
		}

		if (term->chain.head || term->chain.tail) {
			continue;
		}

		chain = kmm_zalloc(sizeof(*chain));
		if (chain == NULL) {
			udbg("Memory allocation failed!!\n");
			return -ENOMEM;
		}

		INIT_QUEUE_HEAD(&chain->entities);
		sem_init(&chain->ctrl_mutex, 0, 1);
		chain->dev = dev;

		term->flags |= UVC_ENTITY_FLAG_DEFAULT;

		if (uvc_scan_chain(chain, term) < 0) {
			kmm_free(chain);
			continue;
		}

		uvdbg("Found a valid video chain!!.\n");
		dq_addfirst(&chain->list, &dev->chains);
	}

	if (queue_empty(&dev->chains)) {
		uvdbg("No valid video chain found.\n");
		return -1;
	}

	return OK;
}

/****************************************************************************
 * Name: usbhost_uvc_devinit
 *
 * Description:
 *   The USB device has been successfully connected.  This completes the
 *   initialization operations.  It is first called after the
 *   configuration descriptor has been received.
 *
 *   This function is called from the connect() method.  This function always
 *   executes on the thread of the caller of connect().
 *
 * Input Parameters:
 *   priv - A reference to the class instance.
 *
 * Returned Values:
 *   None
 *
 ****************************************************************************/

static inline int usbhost_uvc_devinit(FAR struct usbhost_uvc_state_s *priv)
{
	int ret = OK;
	FAR struct usbhost_hubport_s *hport;
	unsigned int max_packet_size = 0;

	hport = priv->usbclass.hport;

	/* Set aside a transfer buffer for exclusive use by the class driver */

	/* Increment the reference count.  This will prevent usbhost_uvc_destroy() from
	 * being called asynchronously if the device is removed.
	 */

	priv->crefs++;
	DEBUGASSERT(priv->crefs == 2);

	/* Set aside a transfer buffer for exclusive use by the driver */
	ret = usbhost_alloc_buffers(priv);
	if (ret < 0) {
		udbg("Failed to allocate transfer buffer\n");
		goto errout;
	}

	/*configuration endpoint 0 */
	max_packet_size = (hport->speed == USB_SPEED_HIGH) ? 512 : 64;

	DRVR_EP0CONFIGURE(hport->drvr, hport->ep0, hport->funcaddr, hport->speed, max_packet_size);

	/* Initialize controls. */
	ret = uvc_ctrl_init_device(priv);
	if (ret < 0) {
		udbg("Failed to initialize controls !\n");
		goto errout;
	}

	/* Scan the device for video chains. */
	ret = uvc_scan_device(priv);
	if (ret < 0) {
		udbg("Failed to scan and unit list !\n");
		goto errout;
	}

	ret = uvc_scan_stream(priv);
	if (ret < 0) {
		udbg("Failed to scan and stream list !\n");
	}

	/* Register the Camera terminal to this device */
	ret = uvc_register_camera(priv);
	if (ret < 0) {
		udbg("Failed to get a camera terminal in this device !\n");
		goto errout;
	}

	/* Configure the device */

	/* Register the driver */
	if (ret >= 0) {
		char devname[DEV_NAMELEN];

		uvdbg("Register UVC driver\n");
		usbhost_uvc_mkdevname(priv, devname);
		g_lower_half.dev = priv;
		priv->video_priv = video_register(devname, &g_lower_half);
		if (priv->video_priv == NULL) {
			udbg("Fail to register video driver!!\n");
			goto errout;
		}
	}

	/* Check if we successfully initialized. We now have to be concerned
	 * about asynchronous modification of crefs because the block
	 * driver has been registerd.
	 */

	if (ret >= 0) {
		usbhost_uvc_takesem(&priv->exclsem);
		DEBUGASSERT(priv->crefs >= 2);

		/* Handle a corner case where (1) open() has been called so the
		 * reference count is > 2, but the device has been disconnected.
		 * In this case, the class instance needs to persist until close()
		 * is called.
		 */

		if (priv->crefs <= 2 && priv->disconnected) {
			/* We don't have to give the semaphore because it will be
			 * destroyed when usb_destroy is called.
			 */

			ret = -ENODEV;
		} else {
			/* Ready for normal operation as a block device driver */

			uvdbg("Successfully initialized\n");
			priv->crefs--;
			usbhost_uvc_givesem(&priv->exclsem);
		}
	}

errout:
	return ret;
}

/****************************************************************************
 * Name: uvc_register_camera
 *
 * Description:
 *   This function parse through the available streams and initializes it
 *   with the chain of controls and dev. Usually in camera devices there would
 *   be only one stream interface.
 *
 * Input Parameters:
 *   dev - Pointer to USB host class object.
 *
 * Returned Value:
 *   On success, zero (OK) is returned. On a failure, a negated errno value is
 *   returned indicating the nature of the failure
 *
 * Assumptions:
 *   Probably called from an interrupt handler.
 *
 ****************************************************************************/
static int uvc_register_camera(FAR uvc_state_t *dev)
{
	struct uvc_video_chain *chain = queue_entry(dev->chains.head, struct uvc_video_chain, list);
	struct uvc_streaming *stream;
	struct uvc_entity *term;

	for (; &chain->list != (dq_entry_t *)&dev->chains; chain = queue_entry(chain->list.flink, struct uvc_video_chain, list)) {

		term = queue_entry(chain->entities.head, struct uvc_entity, chain);

		for (; &term->list != (dq_entry_t *)&chain->entities; term = queue_entry(term->list.flink, struct uvc_entity, list)) {

			if (UVC_ENTITY_TYPE(term) != UVC_TT_STREAMING) {
				continue;
			}

			stream = (struct uvc_streaming *)uvc_stream_by_id(dev, term->id);
			if (stream == NULL) {
				uvdbg("No streaming interface found for terminal %u.\n", term->id);
				continue;
			}

			stream->chain = chain;

			dev->stream = stream;
			return uvc_video_init(stream);
		}
	}

	return -EINVAL;
}

/****************************************************************************
 * struct usbhost_uvc_registry_s methods
 ****************************************************************************/

/****************************************************************************
 * Name: usbhost_uvc_create
 *
 * Description:
 *   This function implements the create() method of struct usbhost_uvc_registry_s.
 *   The create() method is a callback into the class implementation.  It is
 *   used to (1) create a new instance of the USB host class state and to (2)
 *   bind a USB host driver "session" to the class instance.  Use of this
 *   create() method will support environments where there may be multiple
 *   USB ports and multiple USB devices simultaneously connected.
 *
 * Input Parameters:
 *   hport - The hub hat manages the new class instance.
 *   id - In the case where the device supports multiple base classes,
 *     subclasses, or protocols, this specifies which to configure for.
 *
 * Returned Values:
 *   On success, this function will return a non-NULL instance of struct
 *   usbhost_class_s that can be used by the USB host driver to communicate
 *   with the USB host class.  NULL is returned on failure; this function
 *   will fail only if the hport input parameter is NULL or if there are
 *   insufficient resources to create another USB host class instance.
 *
 ****************************************************************************/

static FAR struct usbhost_class_s *usbhost_uvc_create(FAR struct usbhost_hubport_s *hport, FAR const struct usbhost_id_s *id)
{
	FAR struct usbhost_uvc_state_s *priv;

	/* Allocate a USB host class instance */

	priv = usbhost_uvc_allocclass();
	if (priv) {
		/* Initialize the allocated storage class instance */

		memset(priv, 0, sizeof(struct usbhost_uvc_state_s));

		/* Assign a device number to this class instance */

		if (usbhost_uvc_allocdevno(priv) == OK) {
			/* Initialize class method function pointers */

			priv->usbclass.hport = hport;
			priv->usbclass.connect = usbhost_uvc_connect;
			priv->usbclass.disconnected = usbhost_uvc_disconnected;

			/* The initial reference count is 1... One reference is held by the driver */

			priv->crefs = 1;

			/* Initialize semaphores (this works okay in the interrupt context) */

			sem_init(&priv->exclsem, 0, 1);
			sem_init(&priv->opsem, 0, 1);

			/* Initialize the list heads */
			INIT_QUEUE_HEAD(&priv->entities);
			INIT_QUEUE_HEAD(&priv->chains);
			INIT_QUEUE_HEAD(&priv->streams);

			/* Return the instance of the USB class driver */

			return &priv->usbclass;
		}
	}

	/* An error occurred. Free the allocation and return NULL on all failures */

	if (priv) {
		usbhost_uvc_freeclass(priv);
	}
	return NULL;
}

/****************************************************************************
 * struct usbhost_uvc_class_s methods
 ****************************************************************************/
/****************************************************************************
 * Name: usbhost_uvc_connect
 *
 * Description:
 *   This function implements the connect() method of struct
 *   usbhost_class_s.  This method is a callback into the class
 *   implementation.  It is used to provide the device's configuration
 *   descriptor to the class so that the class may initialize properly
 *
 * Input Parameters:
 *   usbclass - The USB host class entry previously obtained from a call to
 *     create().
 *   configdesc - A pointer to a uint8_t buffer container the configuration
 *     descriptor.
 *   desclen - The length in bytes of the configuration descriptor.
 *
 * Returned Values:
 *   On success, zero (OK) is returned. On a failure, a negated errno value is
 *   returned indicating the nature of the failure
 *
 *   NOTE that the class instance remains valid upon return with a failure.  It is
 *   the responsibility of the higher level enumeration logic to call
 *   CLASS_DISCONNECTED to free up the class driver resources.
 *
 * Assumptions:
 *   - This function will *not* be called from an interrupt handler.
 *   - If this function returns an error, the USB host controller driver
 *     must call to DISCONNECTED method to recover from the error
 *
 ****************************************************************************/

static int usbhost_uvc_connect(FAR struct usbhost_class_s *usbclass, FAR const uint8_t *configdesc, int desclen)
{
	FAR struct usbhost_uvc_state_s *priv = (FAR struct usbhost_uvc_state_s *)usbclass;
	int ret;

	DEBUGASSERT(priv != NULL && configdesc != NULL && desclen >= sizeof(struct usb_cfgdesc_s));

	/* Parse the configuration descriptor to get the endpoints */

	ret = usbhost_uvc_cfgdesc(priv, configdesc, desclen);
	if (ret < 0) {
		uvdbg("usbhost_uvc_cfgdesc() failed: %d\n", ret);
		goto errout;
	} else {
		/* Now configure the device and register the TinyAra driver */

		ret = usbhost_uvc_devinit(priv);
		if (ret < 0) {
			uvdbg("usbhost_uvc_devinit() failed: %d\n", ret);
			goto errout;
		}
	}
errout:
	return ret;
}

/****************************************************************************
 * Name: usbhost_uvc_disconnected
 *
 * Description:
 *   This function implements the disconnected() method of struct
 *   usbhost_class_s.  This method is a callback into the class
 *   implementation.  It is used to inform the class that the USB device has
 *   been disconnected.
 *
 * Input Parameters:
 *   usbclass - The USB host class entry previously obtained from a call to
 *     create().
 *
 * Returned Values:
 *   On success, zero (OK) is returned. On a failure, a negated errno value
 *   is returned indicating the nature of the failure
 *
 * Assumptions:
 *   This function may be called from an interrupt handler.
 *
 ****************************************************************************/

static int usbhost_uvc_disconnected(struct usbhost_class_s *usbclass)
{
	FAR struct usbhost_uvc_state_s *priv = (FAR struct usbhost_uvc_state_s *)usbclass;
	irqstate_t flags;

	DEBUGASSERT(priv != NULL);

	/* Set an indication to any users of the device that the device is no
	 * longer available.
	 */

	flags = enter_critical_section();
	priv->disconnected = true;

	/* Now check the number of references on the class instance.  If it is one,
	 * then we can free the class instance now.  Otherwise, we will have to
	 * wait until the holders of the references free them by closing the
	 * block driver.
	 */

	uvdbg("crefs: %d\n", priv->crefs);
	if (priv->crefs > 0) {
		/* Destroy the class instance.  If we are executing from an interrupt
		 * handler, then defer the destruction to the worker thread.
		 * Otherwise, destroy the instance now.
		 */

		if (up_interrupt_context()) {
			/* Destroy the instance on the worker thread. */

			uvdbg("Queuing destruction: worker %p->%p\n", priv->work.worker, usbhost_uvc_destroy);
			DEBUGASSERT(priv->work.worker == NULL);
			(void)work_queue(HPWORK, &priv->work, usbhost_uvc_destroy, priv, 0);
		} else {
			/* Do the work now */

			usbhost_uvc_destroy(priv);
		}
	}

	leave_critical_section(flags);
	return OK;
}

/****************************************************************************
 * Name: usbhost_uvc_ctrl_req
 *
 * Description:
 *   This function send commands to the USB device over EP0 endpoint.
 *
 * Input Parameters:
 *   arg - The argument provided with the asynchronous I/O was setup
 *   type - Type of request.
 *   req - Request command.
 *   wvalue - value to be send along with command.
 *   index - index of interface to which the command to be send.
 *   buf - pointer to a data buffer to be send.
 *   len - length of data buffer.
 *
 * Returned Value:
 *   On success, zero (OK) is returned. On a failure, a negated errno value is
 *   returned indicating the nature of the failure
 *
 ****************************************************************************/
static int usbhost_uvc_ctrl_req(void *arg, uint8_t type, uint8_t req, uint16_t wvalue, uint16_t index, unsigned char *buf, uint16_t len)
{
	FAR struct usbhost_uvc_state_s *priv = (struct usbhost_uvc_state_s *)arg;
	FAR struct usbhost_hubport_s *hport;
	FAR struct usb_ctrlreq_s *ctrlreq = (FAR struct usb_ctrlreq_s *)priv->ctrlreq;
	int ret = OK;

	DEBUGASSERT(priv != NULL && priv->usbclass.hport);
	hport = priv->usbclass.hport;

	ctrlreq->type = type;
	ctrlreq->req = req;
	usbhost_uvc_putle16((uint8_t *)&ctrlreq->value, wvalue);
	usbhost_uvc_putle16((uint8_t *)&ctrlreq->index, index);
	usbhost_uvc_putle16((uint8_t *)&ctrlreq->len, len);

	if ((type & USB_REQ_DIR_MASK) == USB_REQ_DIR_IN) {
		ret = DRVR_CTRLIN(hport->drvr, hport->ep0, ctrlreq, buf);
	} else {
		ret = DRVR_CTRLOUT(hport->drvr, hport->ep0, ctrlreq, buf);
	}
	if (ret < 0) {
		udbg("Failed to send EP0 request, %d\n", ret);
		return ret;
	}
	/* On success return the length requested */
	return ret >= 0 ? len : ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: usbhost_uvc_init
 *
 * Description:
 *   Initialize the USB class driver.  This function should be called
 *   be platform-specific code in order to initialize and register support
 *   for the USB host class device.
 *
 * Input Parameters:
 *   None
 *
 * Returned Values:
 *   On success this function will return zero (OK);  A negated errno value
 *   will be returned on failure.
 *
 ****************************************************************************/

int usbhost_uvc_init(void)
{
	/* Perform any one-time initialization of the class implementation */

	/* Advertise our availability to support (certain) devices */

	return usbhost_registerclass(&g_uvc);
}

/****************************************************************************
 * Name: usbhost_uvc_setinterface
 *
 * Description:
 *   This function is used to send set interface command to the device.
 *
 * Input Parameters:
 *   dev - Pointer to device object
 *   intfnum - interface number to be set
 *   atlintf - alternate interface number
 *
 * Returned Values:
 *   On success this function will return zero (OK);  A negated errno value
 *   will be returned on failure.
 *
 ****************************************************************************/
int usbhost_uvc_setinterface(FAR uvc_state_t *dev, uint8_t intfnum, uint8_t atlintf)
{
	int ret;
	uint8_t type = USB_REQ_TYPE_STANDARD | USB_REQ_RECIPIENT_INTERFACE | USB_REQ_DIR_OUT;

	ret = usbhost_uvc_ctrl_req(dev, type, USB_REQ_SETINTERFACE, atlintf, intfnum, NULL, 0);
	if (ret < 0) {
		uvdbg("Failed to set interface!!\n");
	}

	return ret;
}

/****************************************************************************
 * Name: uvc_query_ctrl
 *
 * Description:
 *   Handle receipt of line status from the UVC device
 *
 * Input Parameters:
 *   dev - Pointer to USB host class object
 *   query - type of query on control
 *   unit - unit id/number
 *   intfnum - interface number
 *   cs - UVC selector Id
 *   data - pointer to data object
 *   size - size of control item
 *
 * Returned Value:
 *   On success, zero (OK) is returned. On a failure, a negated errno value is
 *   returned indicating the nature of the failure
 *
 ****************************************************************************/
int uvc_query_ctrl(FAR uvc_state_t *dev, uint8_t query, uint8_t unit, uint8_t intfnum, uint8_t cs, void *data, uint16_t size)
{
	int ret;
	uint8_t error;
	uint8_t tmp;
	uint8_t type = USB_REQ_TYPE_CLASS | USB_REQ_RECIPIENT_INTERFACE;

	type |= (query & 0x80) ? USB_REQ_DIR_IN : USB_REQ_DIR_OUT;

	ret = usbhost_uvc_ctrl_req(dev, type, query, cs << 8, (unit << 8 | intfnum), data, size);
	if (ret == size) {
		return ret;
	}

	udbg("ailed to query (%d) UVC control %u on unit %u: %d (exp. %u).\n", query, cs, unit, ret, size);

	if (ret != -EPIPE) {
		return ret;
	}

	tmp = *(uint8_t *) data;

	type |= USB_REQ_DIR_IN;

	ret = usbhost_uvc_ctrl_req(dev, type, UVC_GET_CUR, (UVC_VC_REQUEST_ERROR_CODE_CONTROL << 8), (intfnum << 8), data, 1);

	error = *(uint8_t *) data;
	*(uint8_t *) data = tmp;

	if (ret != 1) {
		return ret < 0 ? ret : -EPIPE;
	}

	udbg("Control error %u\n", error);

	switch (error) {
	case 0:
		/* Cannot happen - we received a STALL */
		return -EPIPE;
	case 1:					/* Not ready */
		return -EBUSY;
	case 2:					/* Wrong state */
		return -EILSEQ;
	case 3:					/* Power */
		return -EREMOTE;
	case 4:					/* Out of range */
		return -ERANGE;
	case 5:					/* Invalid unit */
	case 6:					/* Invalid control */
	case 7:					/* Invalid Request */
	case 8:					/* Invalid value within range */
		return -EINVAL;
	default:					/* reserved or unknown */
		break;
	}

	return -EPIPE;
}

/****************************************************************************
 * Name: usbhost_uvc_getle16
 *
 * Description:
 *   Get a (possibly unaligned) 16-bit little endian value.
 *
 * Input Parameters:
 *   val - A pointer to the first byte of the little endian value.
 *
 * Returned Values:
 *   A uint16_t representing the whole 16-bit integer value
 *
 ****************************************************************************/

uint16_t usbhost_uvc_getle16(const uint8_t *val)
{
	return (uint16_t) val[1] << 8 | (uint16_t) val[0];
}

/****************************************************************************
 * Name: usbhost_uvc_putle16
 *
 * Description:
 *   Put a (possibly unaligned) 16-bit little endian value.
 *
 * Input Parameters:
 *   dest - A pointer to the first byte to save the little endian value.
 *   val - The 16-bit value to be saved.
 *
 * Returned Values:
 *   None
 *
 ****************************************************************************/

void usbhost_uvc_putle16(uint8_t *dest, uint16_t val)
{
	dest[0] = val & 0xff;		/* Little endian means LS byte first in byte stream */
	dest[1] = val >> 8;
}

/****************************************************************************
 * Name: usbhost_uvc_getle32
 *
 * Description:
 *   Get a (possibly unaligned) 32-bit little endian value.
 *
 * Input Parameters:
 *   dest - A pointer to the first byte to save the big endian value.
 *   val - The 32-bit value to be saved.
 *
 * Returned Values:
 *   None
 *
 ****************************************************************************/

uint32_t usbhost_uvc_getle32(const uint8_t *val)
{
	/* Little endian means LS halfword first in byte stream */

	return (uint32_t) usbhost_uvc_getle16((const uint8_t *)&val[2]) << 16 | (uint32_t) usbhost_uvc_getle16((const uint8_t *)val);
}

/****************************************************************************
 * Name: usbhost_uvc_putle32
 *
 * Description:
 *   Put a (possibly unaligned) 32-bit little endian value.
 *
 * Input Parameters:
 *   dest - A pointer to the first byte to save the little endian value.
 *   val - The 32-bit value to be saved.
 *
 * Returned Values:
 *   None
 *
 ****************************************************************************/

void usbhost_uvc_putle32(uint8_t *dest, uint32_t val)
{
	/* Little endian means LS halfword first in byte stream */

	usbhost_uvc_putle16(dest, (uint16_t)(val & 0xffff));
	usbhost_uvc_putle16(dest + 2, (uint16_t)(val >> 16));
}

/****************************************************************************
 * Name: usbhost_uvc_takesem
 *
 * Description:
 *   This is just a wrapper to handle the annoying behavior of semaphore
 *   waits that return due to the receipt of a signal.
 *
 ****************************************************************************/

void usbhost_uvc_takesem(sem_t *sem)
{
	/* Take the semaphore (perhaps waiting) */

	while (sem_wait(sem) != 0) {
		/* The only case that an error should occur here is if the wait was
		 * awakened by a signal.
		 */

		ASSERT(errno == EINTR);
	}
}

/****************************************************************************
 * Name: uvc_endpoint_by_address
 *
 * Description:
 *   This is a helper function parse through the list of endpoints and return
 *   the one matches endpoint address requested by the caller.
 *
 ****************************************************************************/
struct usb_ep_desc *uvc_endpoint_by_address(struct usb_inf_desc *intf, int address)
{
	struct usb_ep_desc *ep = queue_entry(intf->ep.head, struct usb_ep_desc, list);
	for (; &ep->list != (dq_entry_t *)&intf->ep; ep = queue_entry(ep->list.flink, struct usb_ep_desc, list)) {
		if (ep->ep.addr == address) {
			return ep;
		}
	}
	udbg("No valid endpoint with address (%d)\n", address);
	return NULL;
}

/****************************************************************************
 * Name: uvc_interface_by_number
 *
 * Description:
 *   This is helper function parse through the list of interfaces and return
 *   the one matches interface and alternate number.
 *
 ****************************************************************************/
struct usb_inf_desc *uvc_interface_by_number(FAR uvc_state_t *dev, int ifno, int altno)
{
	struct usb_inf_desc *intf = queue_entry(dev->config.interface.head, struct usb_inf_desc, list);
	for (; &intf->list != (dq_entry_t *)&dev->config.interface; intf = queue_entry(intf->list.flink, struct usb_inf_desc, list)) {
		if ((intf->intf.ifno == ifno) && (intf->intf.alt == altno)) {
			return intf;
		}
	}
	udbg("No valid interface with interface no(%d) and alternate no(%d)\n", ifno, altno);
	return NULL;
}
