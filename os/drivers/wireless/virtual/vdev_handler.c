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
#include <sys/types.h>
#include <tinyara/config.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <debug.h>
#include <time.h>
#include <tinyara/lwnl/lwnl.h>
#include <tinyara/net/if/wifi.h>
#include "vdev_handler.h"
#include "vdev_command.h"

extern trwifi_scan_list_s *vwifi_get_scan_list(void);
extern struct vwifi_ops *get_vdev_ops(void);
extern struct netdev *g_vwifi_dev;
extern void vwifi_generate_dhcp_discover(void);

static void vwifi_callback_handler(lwnl_cb_status evt, int result)
{
	if (evt.evt == LWNL_EVT_SCAN_DONE) {
		trwifi_scan_list_s *scanlist = vwifi_get_scan_list();
		TRWIFI_POST_SCANEVENT(g_vwifi_dev, LWNL_EVT_SCAN_DONE, scanlist);
	} else if (evt.evt == LWNL_EVT_SCAN_FAILED) {
		trwifi_post_event(g_vwifi_dev, evt.evt, NULL, 0);
	} else if (evt.evt == LWNL_EVT_STA_DISCONNECTED) {
		trwifi_cbk_msg_s msg = {result, {0,}, NULL};
		trwifi_post_event(g_vwifi_dev, evt.evt, &msg, sizeof(trwifi_cbk_msg_s));
	} else {
		trwifi_cbk_msg_s msg = {TRWIFI_REASON_UNKNOWN, {0,}, NULL};
		trwifi_post_event(g_vwifi_dev, evt.evt, &msg, sizeof(trwifi_cbk_msg_s));
	}
}

static void _generate_evt(int argc, char *argv[])
{
	lwnl_cb_status event_type = {LWNL_DEV_WIFI, 0};
	event_type.evt = atoi(argv[1]);
	int result = atoi(argv[2]);
	int sleep_time = atoi(argv[3]);

	VWIFI_LOG("[VDEV] sleep (%d) result (%d)event type(%d)\n",
				 sleep_time, result, event_type.evt);

	sleep(sleep_time);
	if (event_type.evt == VWIFI_PKT_DHCPS_EVT) {
		vwifi_generate_dhcp_discover();
	} else {
		vwifi_callback_handler(event_type, result);
	}
}

int vwifi_create_event(uint32_t event, int32_t result, int32_t sleep)
{
	char event_buf[16] = {0,};
	char res_buf[16] = {0,};
	char sleep_buf[16] = {0,};

	memset(event_buf, 0, 16);
	memset(res_buf, 0, 16);
	memset(sleep_buf, 0, 16);

	snprintf(event_buf, 16, "%d", event);
	snprintf(res_buf, 16, "%d", result);
	snprintf(sleep_buf, 16, "%d", sleep);

	char *argv[4] = {0,};
	argv[0] = event_buf;
	argv[1] = res_buf;
	argv[2] = sleep_buf;
	argv[2] = NULL;

	int res = kernel_thread("vwifi_evt", 100, 2048, _generate_evt, argv);
	if (res == -1) {
		VWIFI_ERROR(0);
		return -1;
	}
	return 0;
}

int vwifi_handle_message(struct vwifi_req *req)
{
	struct vwifi_ops *ops = get_vdev_ops();
	int res = 0;
	switch (req->type) {
	case VWIFI_MSG_INIT:
		*req->res = ops->init(req);
		break;
	case VWIFI_MSG_DEINIT:
		*req->res = ops->deinit(req);
		break;
	case VWIFI_MSG_SCANAP:
		*req->res = ops->scan_ap(req);
		break;
	case VWIFI_MSG_CONNECTAP:
		*req->res = ops->connect_ap(req);
		break;
	case VWIFI_MSG_DISCONENCTAP:
		*req->res = ops->disconnect_ap(req);
		break;
	case VWIFI_MSG_GETINFO:
		*req->res = ops->get_info(req);
		break;
	case VWIFI_MSG_STARTSTA:
		*req->res = ops->start_sta(req);
		break;
	case VWIFI_MSG_STARTSOFTAP:
		*req->res = ops->start_softap(req);
		break;
	case VWIFI_MSG_STOPSOFTAP:
		*req->res = ops->stop_softap(req);
		break;
	case VWIFI_MSG_SETAUTOCONNECT:
		*req->res = ops->set_autoconnect(req);
		break;
	case VWIFI_MSG_IOCTL:
		*req->res = ops->drv_ioctl(req);
		break;
	default:
		VWIFI_ERROR(0);
		break;
	}
	return res;
}
