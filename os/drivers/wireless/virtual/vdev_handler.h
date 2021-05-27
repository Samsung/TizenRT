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
#ifndef __VWIFI_HANDLER_H__
#define __VWIFI_HANDLER_H__

#define VWIFI_LOG vdvdbg
#define VWIFI_ENTRY vdvdbg("-->T%d %s:%d\n", getpid(), __FUNCTION__, __LINE__)
#define VWIFI_ERROR(res) vddbg("T%d error %d %d %s:%d\n", getpid(), res, errno, __FUNCTION__, __LINE__)

typedef enum {
	VWIFI_MSG_INIT,
	VWIFI_MSG_DEINIT,
	VWIFI_MSG_SCANAP,
	VWIFI_MSG_CONNECTAP,
	VWIFI_MSG_DISCONENCTAP,
	VWIFI_MSG_GETINFO,
	VWIFI_MSG_STARTSTA,
	VWIFI_MSG_STARTSOFTAP,
	VWIFI_MSG_STOPSOFTAP,
	VWIFI_MSG_SETAUTOCONNECT,
} vwifi_req_e;

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

struct vwifi_req {
	vwifi_req_e type;
	void *arg;
	trwifi_result_e res;
};

struct vwifi_msg {
	struct vwifi_req *req;
	void *signal;
};

struct vwifi_evt {
	main_t func;
};

typedef trwifi_result_e (*vwifi_init)(void *arg);
/*	block call */
typedef trwifi_result_e (*vwifi_deinit)(void *arg);
/*	non-block */
typedef trwifi_result_e (*vwifi_scan_ap)(void *arg);
/*	non-block */
typedef trwifi_result_e (*vwifi_connect_ap)(void *arg);
/*	non-block */
typedef trwifi_result_e (*vwifi_disconnect_ap)(void *arg);
/*	block */
typedef trwifi_result_e (*vwifi_get_info)(void *arg);
/*	block */
typedef trwifi_result_e (*vwifi_start_sta)(void *arg);
/*	block */
typedef trwifi_result_e (*vwifi_start_softap)(void *arg);
/*	block */
typedef trwifi_result_e (*vwifi_stop_softap)(void *arg);
/*	block */
typedef trwifi_result_e (*vwifi_set_autoconnect)(void *arg);
/*	block */
typedef trwifi_result_e (*vwifi_drv_ioctl)(void *arg);

struct vwifi_ops {
	vwifi_init init;
	vwifi_deinit deinit;
	vwifi_scan_ap scan_ap;
	vwifi_connect_ap connect_ap;
	vwifi_disconnect_ap disconnect_ap;
	vwifi_get_info get_info;
	vwifi_start_sta start_sta;
	vwifi_start_softap start_softap;
	vwifi_stop_softap stop_softap;
	vwifi_set_autoconnect set_autoconnect;
	vwifi_drv_ioctl drv_ioctl;
};

int vwifi_handle_message(struct vwifi_req *req);
int vwifi_create_event(struct vwifi_evt *vevent, int sleep, uint32_t event);
#endif // #define __VWIFI_HANDLER_H__
