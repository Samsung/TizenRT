/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
 * drivers/usbhost/usbhost_cdcacm.c
 *
 *   Copyright (C) 2015 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/kmalloc.h>
#include <tinyara/arch.h>
#include <tinyara/wqueue.h>
#include <tinyara/clock.h>

#include <tinyara/usb/usb.h>
#include <tinyara/usb/usbhost.h>
#include <tinyara/usb/cdc.h>
#include <tinyara/usb/cdcacm.h>
#include <tinyara/usb/usbhost_devaddr.h>

#ifdef CONFIG_USBHOST_RTL8723D

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Configuration ************************************************************/

#ifndef CONFIG_USBHOST
#warning USB host support not enabled (CONFIG_USBHOST)
#endif

#ifdef CONFIG_USBHOST_BULK_DISABLE
#warning USB bulk endpoint support is disabled (CONFIG_USBHOST_BULK_DISABLE)
#endif

#define MAX_OUT_ENDPOINT_NUM 	4
#define MAX_IN_ENDPOINT_NUM	  	 1

struct usbhost_rtk_wifi_s {
	/* This is the externally visible portion of the state.  The usbclass must
	 * the first element of the structure.
	 */
	struct usbhost_class_s usbclass;
	usbhost_ep_t bulkin[MAX_IN_ENDPOINT_NUM];
	unsigned char nr_inep;
	usbhost_ep_t bulkout[MAX_OUT_ENDPOINT_NUM];
	unsigned char nr_outep;
	unsigned char disconnected;
};

static const const struct usbhost_id_s g_id = {
	0xff,				/* base     */
	0xff,								/* subclass */
	0xff,						/* proto    */
	0x0bda,									/* vid      */
	0xd723									/* pid      */
};

void *g_rtk_wifi_usb = NULL;
unsigned char g_rtk_wifi_connect = 0;

/****************************************************************************
local functions
****************************************************************************/
/****************************************************************************
 * Name: usbhost_getle16
 *
 * Description:
 *	 Get a (possibly unaligned) 16-bit little endian value.
 *
 * Input Parameters:
 *	 val - A pointer to the first byte of the little endian value.
 *
 * Returned Value:
 *	 A uint16_t representing the whole 16-bit integer value
 *
 ****************************************************************************/
static inline uint16_t usbhost_getle16(const uint8_t *val)
{
	return (uint16_t)val[1] << 8 | (uint16_t)val[0];
}

/****************************************************************************
 * Name: usbhost_putle16
 *
 * Description:
 *   Put a (possibly unaligned) 16-bit little endian value.
 *
 ****************************************************************************/
static void usbhost_putle16(uint8_t *dest, uint16_t val)
{
	dest[0] = val & 0xff;		/* Little endian means LS byte first in byte stream */
	dest[1] = val >> 8;
}

static FAR struct usbhost_rtk_wifi_s *usbhost_allocclass(void)
{
	FAR struct usbhost_rtk_wifi_s *priv;

	/* We are not executing from an interrupt handler so we can just call
	 * kmm_malloc() to get memory for the class instance.
	 */

	DEBUGASSERT(!up_interrupt_context());
	priv = (FAR struct usbhost_rtk_wifi_s *)kmm_malloc(sizeof(struct usbhost_rtk_wifi_s));

	memset(priv,0,sizeof(FAR struct usbhost_rtk_wifi_s ));
	
	return priv;
}

static void usbhost_freeclass(FAR struct usbhost_rtk_wifi_s *usbclass)
{
	DEBUGASSERT(usbclass != NULL);

	/* Free the class instance (calling sched_kfree() in case we are executing
	 * from an interrupt handler.
	 */

	printf("Freeing: %p\n", usbclass);
	sched_kfree(usbclass);
}


static int usbhost_cfgdesc(FAR struct usbhost_rtk_wifi_s *priv, FAR const uint8_t *configdesc, int desclen)
{
	FAR struct usbhost_hubport_s *hport;
	FAR struct usb_cfgdesc_s *cfgdesc = NULL;
	FAR struct usb_desc_s *desc;
	FAR struct usbhost_epdesc_s bindesc[MAX_IN_ENDPOINT_NUM];
	FAR struct usbhost_epdesc_s boutdesc[MAX_OUT_ENDPOINT_NUM];
	int outdesc_index = 0;
	int indesc_index = 0;
	int remaining = 0;
	int i,ret;

	
	DEBUGASSERT(priv != NULL && priv->usbclass.hport && configdesc != NULL && desclen >= sizeof(struct usb_cfgdesc_s));

	hport = priv->usbclass.hport;
	cfgdesc = (FAR struct usb_cfgdesc_s *)configdesc;

	/* Verify that we were passed a configuration descriptor */
	if (cfgdesc->type != USB_DESC_TYPE_CONFIG) {
		return -EINVAL;
	}

	
	/* Get the total length of the configuration descriptor (little endian).
	 * It might be a good check to get the number of interfaces here too.
	 */

	remaining = (int)usbhost_getle16(cfgdesc->totallen);

	/* Skip to the next entry descriptor */
	configdesc += cfgdesc->len;
	remaining -= cfgdesc->len;

	
	/* Loop where there are more descriptors to examine */

	while (remaining >= sizeof(struct usb_desc_s)) {
		/* What is the next descriptor? */

		desc = (FAR struct usb_desc_s *)configdesc;
		switch (desc->type) {
		/* Interface descriptor. The CDC/ACM device may include two
		 * interfaces:
		 *
		 * 1) A data interface which consists of two endpoints (bulk in +
		 *    bulk out) and
		 * 2) A control interface which consists of one interrupt in
		 *    endpoint.
		 */

		case USB_DESC_TYPE_INTERFACE: {
			FAR struct usb_ifdesc_s *ifdesc = (FAR struct usb_ifdesc_s *)configdesc;
			printf("Interface descriptor: class: %d subclass: %d proto: %d\n", ifdesc->classid, ifdesc->subclass, ifdesc->protocol);
			DEBUGASSERT(remaining >= USB_SIZEOF_IFDESC);
			
		}
		break;
		

		/* Endpoint descriptor.  We expect five bulk endpoints, an IN and four
		 * OUT.
		 */

		case USB_DESC_TYPE_ENDPOINT: {
			FAR struct usb_epdesc_s *epdesc = (FAR struct usb_epdesc_s *)configdesc;
		 	if((epdesc->attr & USB_EP_ATTR_XFERTYPE_MASK) == USB_EP_ATTR_XFER_BULK){
				if (USB_ISEPOUT(epdesc->addr)) {
					printf("out endpoint:  epdesc->addr = %d \n", epdesc->addr);
					boutdesc[outdesc_index].hport = hport;
					boutdesc[outdesc_index].addr = epdesc->addr & USB_EP_ADDR_NUMBER_MASK;
					boutdesc[outdesc_index].in = false;
					boutdesc[outdesc_index].xfrtype = USB_EP_ATTR_XFER_BULK;
					boutdesc[outdesc_index].interval = epdesc->interval;
					boutdesc[outdesc_index].mxpacketsize = usbhost_getle16(epdesc->mxpacketsize);
					outdesc_index ++;
				}else if(USB_ISEPIN(epdesc->addr)) {
					printf("in endpoint epdesc->addr = %x \n",epdesc->addr);
					bindesc[indesc_index].hport = hport;
					bindesc[indesc_index].addr = epdesc->addr & USB_EP_ADDR_NUMBER_MASK;
					bindesc[indesc_index].in = 1;
					bindesc[indesc_index].xfrtype = USB_EP_ATTR_XFER_BULK;
					bindesc[indesc_index].interval = epdesc->interval;
					bindesc[indesc_index].mxpacketsize = usbhost_getle16(epdesc->mxpacketsize);
					indesc_index ++;
				}
		 	}
			
			DEBUGASSERT(remaining >= USB_SIZEOF_EPDESC);

		}
		break;

		/* Other descriptors are just ignored for now */

		default:
			break;
		}

		/* Increment the address of the next descriptor */

		configdesc += desc->len;
		remaining -= desc->len;
	}

	printf("outdesc_index = %d \n",outdesc_index);
	for(i=0;i<outdesc_index;i++){
		ret = DRVR_EPALLOC(hport->drvr, &boutdesc[i], &priv->bulkout[i]);
		if (ret < 0) {
			printf("ERROR: Failed to allocate Bulk OUT endpoint\n");
			for(i=0;i<outdesc_index;i++){
				(void)DRVR_EPFREE(hport->drvr, priv->bulkout[i]);
			}
			return ret;
		}
	}
	priv->nr_outep = outdesc_index;
	

	for(i=0;i<indesc_index;i++){
		ret = DRVR_EPALLOC(hport->drvr, &bindesc[i], &priv->bulkin[i]);
		if (ret < 0) {
			printf("ERROR: Failed to allocate Bulk IN endpoint\n");
			for(i=0;i<indesc_index;i++){
				(void)DRVR_EPFREE(hport->drvr, priv->bulkin[i]);
			}
			return ret;
		}
	}
	priv->nr_inep = indesc_index;

	return OK;
}


/****************************************************************************
 * Name: usbhost_connect
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

static int usbhost_connect(FAR struct usbhost_class_s *usbclass, FAR const uint8_t *configdesc, int desclen)
{
	FAR struct usbhost_rtk_wifi_s *priv =  (FAR struct  usbhost_rtk_wifi_s*)usbclass;
	int ret;
	
	DEBUGASSERT(priv != NULL && configdesc != NULL && desclen >= sizeof(struct usb_cfgdesc_s));

	printf("-------------> usbhost_connect \n");

	/* Parse the configuration descriptor to get the endpoints */
	ret = usbhost_cfgdesc(priv, configdesc, desclen);
	if (ret < 0) {
		printf("usbhost_cfgdesc() failed: %d\n", ret);
	} 

	g_rtk_wifi_connect =  1;
	printf("<------------- usbhost_connect \n");
	return ret;	
}

/****************************************************************************
 * Name: usbhost_destroy
 *
 * Description:
 *   The USB CDC/ACM device has been disconnected and the reference count
 *   on the USB host class instance has gone to 1.. Time to destroy the USB
 *   host class instance.
 *
 * Input Parameters:
 *   arg - A reference to the class instance to be destroyed.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static void usbhost_destroy(FAR void *arg)
{
	FAR struct usbhost_rtk_wifi_s *priv = (FAR struct usbhost_rtk_wifi_s *)arg;
	FAR struct usbhost_hubport_s *hport;
	int i;

	DEBUGASSERT(priv != NULL && priv->usbclass.hport != NULL);
	hport = priv->usbclass.hport;

	/* Free the allocated endpoints */
	for(i=0;i<priv->nr_inep;i++) {
		DRVR_EPFREE(hport->drvr, priv->bulkin[i]);
	}

	for(i=0;i<priv->nr_outep;i++) {
		DRVR_EPFREE(hport->drvr, priv->bulkout[i]);
	}

	/* Disconnect the USB host device */

	DRVR_DISCONNECT(hport->drvr, hport);

	/* Free the function address assigned to this device */

	usbhost_devaddr_destroy(hport, hport->funcaddr);
	hport->funcaddr = 0;

	/* And free the class instance.  */

	usbhost_freeclass(priv);
}

/****************************************************************************
 * Name: usbhost_disconnected
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

static int usbhost_disconnected(struct usbhost_class_s *usbclass)
{
	FAR struct usbhost_rtk_wifi_s *priv = (FAR struct usbhost_rtk_wifi_s *)usbclass;
	irqstate_t flags;
	int i;
	int ret;

	DEBUGASSERT(priv != NULL);

	/* Set an indication to any users of the device that the device is no
	 * longer available.
	 */

	flags = irqsave();
	priv->disconnected = true;
	g_rtk_wifi_connect = 0;

	/* Canncel bulk transfer before destroy resource
	*  include bulk in and bulk out
	*/

	for(i=0;i<priv->nr_inep;i++){
		ret = DRVR_CANCEL(priv->usbclass.hport->drvr,priv->bulkin[i]);
		if(ret < 0)
			printf("usbhost_disconnect: cancel bulk in fail \n");
	}
		
	for(i=0;i<priv->nr_outep;i++){
		ret = DRVR_CANCEL(priv->usbclass.hport->drvr,priv->bulkout[i]);
		if(ret < 0)
			printf("usbhost_disconnect: cancel bulk out fail \n");
	}
	
      /* call usbhost_destroy to free resource  */
       usbhost_destroy(priv);
	
	irqrestore(flags);
	return OK;
}


static FAR struct usbhost_class_s *usbhost_create(FAR struct usbhost_hubport_s *hport, FAR const struct usbhost_id_s *id)
{	
	struct usbhost_rtk_wifi_s *priv = NULL;


	priv = usbhost_allocclass();

	if(priv){
		priv->usbclass.hport = hport;
		priv->usbclass.connect = usbhost_connect;
		priv->usbclass.disconnected = usbhost_disconnected;

		g_rtk_wifi_usb = priv;
		return &priv->usbclass;
	}

	if(priv){
		usbhost_freeclass(priv);
	}

	return NULL;
}

static struct usbhost_registry_s g_rtl8723d = {
	NULL,						/* flink    */
	usbhost_create,				/* create   */
	1,							/* nids     */
	&g_id						/* id[]     */
};


/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
usb control message read/write API
priv: struct usbhost_rtk_wifi_s *
bdir_in: true if ctrl in while false if ctrl out
wvalue: register address
buf: input buffer if ctrl in while output buffer if ctrl out
len: length of buffer
*****************************************************************************/
int usbhost_ctrl_req(void *priv,unsigned char bdir_in,unsigned int wvalue,unsigned char *buf,unsigned int len)
{
	FAR struct usbhost_rtk_wifi_s *p_rtk_wifi_usb = (struct usbhost_rtk_wifi_s *)priv;
	FAR struct usbhost_hubport_s *hport;
	FAR struct usb_ctrlreq_s *ctrlreq = NULL;
	int ret = OK;
	unsigned int maxlen = 0;
	unsigned int max_packet_size = 0;
	
	DEBUGASSERT(p_rtk_wifi_usb != NULL && p_rtk_wifi_usb->usbclass.hport);
	hport = p_rtk_wifi_usb->usbclass.hport;

	/* alloc crtl req*/
	ret = DRVR_ALLOC(hport->drvr, (FAR uint8_t **)&ctrlreq, &maxlen);
	if (ret < 0) {
		printf("DRVR_ALLOC failed: %d\n", ret);
		return ret;
	}

	/*configuration enpoint 0*/

	if(len > 8)
		max_packet_size = len;
	else
		max_packet_size = USB_SIZEOF_CTRLREQ;
	
	DRVR_EP0CONFIGURE(hport->drvr, hport->ep0, hport->funcaddr, hport->speed, max_packet_size);

	/*construct ctrl req*/
	/*
		USB2.0 spec Table 9-2
	*/
	ctrlreq->type = 0x40;						
	if(bdir_in)
		ctrlreq->type |= USB_REQ_DIR_IN;
	else
		ctrlreq->type |= USB_REQ_DIR_OUT;

	ctrlreq->req = USB_REQ_SETADDRESS;
	usbhost_putle16(ctrlreq->value, wvalue);
	usbhost_putle16(ctrlreq->index, 0);
	usbhost_putle16(ctrlreq->len, len);

	/*submit ctrl request: imxrt_ctrlin()*/
	ret = DRVR_CTRLIN(hport->drvr, hport->ep0, ctrlreq, buf);
	if (ret < 0) {
		printf("ERROR: Failed to get device descriptor, %d\n", ret);
		return ret;
	}

	if(ctrlreq)
		DRVR_FREE(hport->drvr, (FAR uint8_t *)ctrlreq); 

	return ret;
	
}


/****************************************************************************
get usb speed info
*****************************************************************************/
int usbhost_get_speed(void *priv)
{	
	FAR struct usbhost_rtk_wifi_s *p_rtk_wifi_usb = (struct usbhost_rtk_wifi_s *)priv;

	return p_rtk_wifi_usb->usbclass.hport->speed;
}

/****************************************************************************
get out endpoint info and number
*****************************************************************************/
int usbhost_get_out_ep_info(void *priv, unsigned char *ep_addr_array)
{	
	FAR struct usbhost_rtk_wifi_s *p_rtk_wifi_usb = (struct usbhost_rtk_wifi_s *)priv;
	int i;

	for(i=0;i<p_rtk_wifi_usb->nr_outep;i++){
		ep_addr_array[i] = i;
	}
	
	return p_rtk_wifi_usb->nr_outep;
}


/****************************************************************************
get in endpoint info and number
*****************************************************************************/
int usbhost_get_in_ep_info(void *priv,unsigned char *ep_addr_array)
{	
	FAR struct usbhost_rtk_wifi_s *p_rtk_wifi_usb = (struct usbhost_rtk_wifi_s *)priv;
	int i;

	for(i=0;i<p_rtk_wifi_usb->nr_inep;i++){
		ep_addr_array[i] = i;
	}
	
	return p_rtk_wifi_usb->nr_inep;
}

/*****************************************************************************
usb get in pipe
*****************************************************************************/
unsigned char usbhost_get_bulk_in_pipe(void *priv,unsigned char en_addr)
{
	return en_addr;
}


/*****************************************************************************
usb get out pipe
*****************************************************************************/
unsigned char usbhost_get_bulk_out_pipe(void *priv,unsigned char en_addr)
{
	return en_addr;
}

/*****************************************************************************
usb bulk in
*****************************************************************************/
ssize_t usbhost_bulk_in(void *priv,unsigned char pipe,unsigned char *buf,unsigned int len)
{
	FAR struct usbhost_rtk_wifi_s *p_rtk_wifi_usb = (struct usbhost_rtk_wifi_s *)priv;
	FAR struct usbhost_hubport_s *hport;

	DEBUGASSERT(p_rtk_wifi_usb != NULL && p_rtk_wifi_usb->usbclass.hport);
	hport = p_rtk_wifi_usb->usbclass.hport;

	if(p_rtk_wifi_usb->disconnected){
		return -ENODEV;
	}
	
	return DRVR_TRANSFER(hport->drvr,p_rtk_wifi_usb->bulkin[pipe],buf,len);

}

/*****************************************************************************
usb bulk out
*****************************************************************************/
ssize_t usbhost_bulk_out(void *priv,unsigned char pipe,unsigned char *buf,unsigned int len)
{
	FAR struct usbhost_rtk_wifi_s *p_rtk_wifi_usb = (struct usbhost_rtk_wifi_s *)priv;
	FAR struct usbhost_hubport_s *hport;
	ssize_t  nwritten_bytes = -1;

	DEBUGASSERT(p_rtk_wifi_usb != NULL && p_rtk_wifi_usb->usbclass.hport);
	hport = p_rtk_wifi_usb->usbclass.hport;
	
	if(p_rtk_wifi_usb->disconnected){
		nwritten_bytes = -ENODEV;
		goto exit;
	}

	nwritten_bytes = DRVR_TRANSFER(hport->drvr,p_rtk_wifi_usb->bulkout[pipe],buf,len);
		
exit:
	return nwritten_bytes;
}

/*****************************************************************************
usb cancel bulk in
*****************************************************************************/
int usbhost_cancel_bulk_in(void *priv)
{
	FAR struct usbhost_rtk_wifi_s *p_rtk_wifi_usb = (struct usbhost_rtk_wifi_s *)priv;
	FAR struct usbhost_hubport_s *hport;
	int i = 0,ret = 0;
	
	DEBUGASSERT(p_rtk_wifi_usb != NULL && p_rtk_wifi_usb->usbclass.hport);
	hport = p_rtk_wifi_usb->usbclass.hport;

	if(p_rtk_wifi_usb->disconnected){
		ret = -ENODEV;
		goto exit;
	}

	for(i=0;i<p_rtk_wifi_usb->nr_inep;i++){
		ret = DRVR_CANCEL(hport->drvr,p_rtk_wifi_usb->bulkin[i]);
		if(ret < 0)
			printf("ERROR: Failed to cancel bulk in, %d\n", ret);
	}

exit:
	return ret;
}

/*****************************************************************************
usb cancel bulk out
*****************************************************************************/
int usbhost_cancel_bulk_out(void *priv)
{
	FAR struct usbhost_rtk_wifi_s *p_rtk_wifi_usb = (struct usbhost_rtk_wifi_s *)priv;
	FAR struct usbhost_hubport_s *hport;
	int i = 0,ret = 0;
	
	DEBUGASSERT(p_rtk_wifi_usb != NULL && p_rtk_wifi_usb->usbclass.hport);
	hport = p_rtk_wifi_usb->usbclass.hport;

	if(p_rtk_wifi_usb->disconnected){
		ret = -ENODEV;
		goto exit;
	}

	for(i=0;i<p_rtk_wifi_usb->nr_outep;i++){
		ret = DRVR_CANCEL(hport->drvr,p_rtk_wifi_usb->bulkout[i]);
		if(ret < 0)
			printf("ERROR: Failed to cancel bulk out, %d\n", ret);
	}

exit:
	return ret;
}


void usbhost_rtl8723d_initialize(void)
{
	/* If we have been configured to use pre-allocated RTK WIFI class instances,
	 * then place all of the pre-allocated USB host RTK WIFI class instances
	 * into a free list.
	 */
	printf("------->usbhost_rtl8723d_initialize \n ");
	usbhost_registerclass(&g_rtl8723d);
}

#endif							
