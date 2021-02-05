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

#define VWIFI_LOG nlvdbg
#define VWIFI_ENTRY nlvdbg("-->T%d %s:%d\n", getpid(), __FUNCTION__, __LINE__)
#define VWIFI_ERROR(res) nldbg("T%d error %d %d %s:%d\n", getpid(), res, errno, __FUNCTION__, __LINE__)

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

struct vwifi_req {
	vwifi_req_e type;
	int res;
};

struct vwifi_msg {
	struct vwifi_req *req;
	void *signal;
};

int vwifi_handle_message(struct vwifi_req *req);
#endif // #define __VWIFI_HANDLER_H__
