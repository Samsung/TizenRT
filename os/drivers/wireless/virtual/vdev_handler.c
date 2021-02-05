/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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
#include <unistd.h>
#include <errno.h>
#include <debug.h>
#include <tinyara/lwnl/lwnl.h>
#include "vdev_handler.h"

typedef enum {
	VWIFI_EVT_STA_CONNECTED,
	VWIFI_EVT_STA_CONNECT_FAILED,
	VWIFI_EVT_STA_DISCONNECTED,
	VWIFI_EVT_SOFTAP_STA_JOINED,
	VWIFI_EVT_SOFTAP_STA_LEFT,
	VWIFI_EVT_SCAN_DONE,
	VWIFI_EVT_SCAN_FAILED,
	VWIFI_EVT_EXIT,
	VWIFI_EVT_UNKNOWN,
} vwifi_evt_e;

struct vwifi_evt {
	main_t func;
};

static int vwifi_handle_init(struct vwifi_req *req);
static int vwifi_handle_deinit(struct vwifi_req *req);
static int vwifi_handle_scanap(struct vwifi_req *req);
static int vwifi_handle_connectap(struct vwifi_req *req);
static int vwifi_handle_disconnectap(struct vwifi_req *req);
static int vwifi_handle_getinfo(struct vwifi_req *req);
static int vwifi_handle_startsta(struct vwifi_req *req);
static int vwifi_handle_startsoftap(struct vwifi_req *req);
static int vwifi_handle_stopsoftap(struct vwifi_req *req);
static int vwifi_handle_setautoconnect(struct vwifi_req *req);

static int vwifi_create_event(struct vwifi_evt *event);


static void vwifi_callback_handler(vwifi_evt_e evt)
{
	switch (evt) {
	case VWIFI_EVT_STA_CONNECTED:
		lwnl_postmsg(LWNL_STA_CONNECTED, NULL);
		break;
	case VWIFI_EVT_STA_CONNECT_FAILED:
		lwnl_postmsg(LWNL_STA_CONNECT_FAILED, NULL);
		break;
	case VWIFI_EVT_SOFTAP_STA_JOINED:
		lwnl_postmsg(LWNL_SOFTAP_STA_JOINED, NULL);
		break;
	case VWIFI_EVT_STA_DISCONNECTED:
		lwnl_postmsg(LWNL_STA_DISCONNECTED, NULL);
		break;
	case VWIFI_EVT_SOFTAP_STA_LEFT:
		lwnl_postmsg(LWNL_SOFTAP_STA_LEFT, NULL);
		break;
	default:
		lwnl_postmsg(LWNL_UNKNOWN, NULL);
		break;
	}
}

void _generate_disconnect(int argc, char *argv[])
{
	sleep(3);
	vwifi_callback_handler(VWIFI_EVT_STA_DISCONNECTED);
}

int vwifi_create_event(struct vwifi_evt *event)
{
	int res = kernel_thread("vwifi_evt", 100, 1024, event->func, NULL);
	if (res < 0) {
		VWIFI_ERROR(0);
		return -1;
	}
	return 0;
}

int vwifi_handle_init(struct vwifi_req *req)
{
	VWIFI_ENTRY;
	return 0;
}

int vwifi_handle_deinit(struct vwifi_req *req)
{
	VWIFI_ENTRY;
	return 0;
}

int vwifi_handle_scanap(struct vwifi_req *req)
{
	VWIFI_ENTRY;
	return 0;
}

int vwifi_handle_connectap(struct vwifi_req *req)
{
	VWIFI_ENTRY;
	vwifi_callback_handler(VWIFI_EVT_STA_CONNECTED);

	struct vwifi_evt event;
	event.func = (main_t)_generate_disconnect;
	vwifi_create_event(&event);

	return 0;
}

int vwifi_handle_disconnectap(struct vwifi_req *req)
{
	VWIFI_ENTRY;

	struct vwifi_evt event;
	event.func = (main_t)_generate_disconnect;
	vwifi_create_event(&event);

	return 0;
}

int vwifi_handle_getinfo(struct vwifi_req *req)
{
	VWIFI_ENTRY;
	return 0;
}

int vwifi_handle_startsta(struct vwifi_req *req)
{
	VWIFI_ENTRY;
	return 0;
}

int vwifi_handle_startsoftap(struct vwifi_req *req)
{
	VWIFI_ENTRY;
	return 0;
}

int vwifi_handle_stopsoftap(struct vwifi_req *req)
{
	VWIFI_ENTRY;
	return 0;
}

int vwifi_handle_setautoconnect(struct vwifi_req *req)
{
	VWIFI_ENTRY;
	return 0;
}

int vwifi_handle_message(struct vwifi_req *req)
{
	int res = 0;
	switch(req->type) {
	case VWIFI_MSG_INIT:
		req->res = vwifi_handle_init(req);
		break;
	case VWIFI_MSG_DEINIT:
		req->res = vwifi_handle_deinit(req);
		break;
	case VWIFI_MSG_SCANAP:
		req->res = vwifi_handle_scanap(req);
		break;
	case VWIFI_MSG_CONNECTAP:
		req->res = vwifi_handle_connectap(req);
		break;
	case VWIFI_MSG_DISCONENCTAP:
		req->res = vwifi_handle_disconnectap(req);
		break;
	case VWIFI_MSG_GETINFO:
		req->res = vwifi_handle_getinfo(req);
		break;
	case VWIFI_MSG_STARTSTA:
		req->res = vwifi_handle_startsta(req);
		break;
	case VWIFI_MSG_STARTSOFTAP:
		req->res = vwifi_handle_stopsoftap(req);
		break;
	case VWIFI_MSG_STOPSOFTAP:
		req->res = vwifi_handle_stopsoftap(req);
		break;
	case VWIFI_MSG_SETAUTOCONNECT:
		req->res = vwifi_handle_setautoconnect(req);
		break;
	default:
		VWIFI_ERROR(0);
		break;
	}
	return res;
}
