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

#ifndef _WIFI_MANAGER_CB_H__
#define _WIFI_MANAGER_CB_H__

enum _wifimgr_usr_cb_type {
	CB_STA_CONNECTED,
	CB_STA_CONNECT_FAILED,
	CB_STA_DISCONNECTED,
	CB_STA_RECONNECTED,
	CB_STA_JOINED,
	CB_STA_LEFT,
	CB_SCAN_DONE,
	CB_SOFTAP_DONE, /* This callback does not exist, but is used for stats management. */
	CB_MAX,
	CB_EVT_NONE = -1
};
typedef enum _wifimgr_usr_cb_type _wifimgr_usr_cb_type_e;

int wifimgr_register_cb(wifi_manager_cb_s *cb);
int wifimgr_unregister_cb(wifi_manager_cb_s *cb);
void wifimgr_unregister_all(void);
void wifimgr_call_cb(_wifimgr_usr_cb_type_e evt, void *arg);

#endif // _WIFI_MANAGER_CB_H__
