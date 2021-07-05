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
#pragma once

/*  State Definition */
typedef enum {
	BLEMGR_UNINITIALIZED = 0,
	BLEMGR_INITIALIZED,
	BLEMGR_STATE_MAX,
} blemgr_state_e;

typedef enum {
	BLEMGR_SERVER_NONE = 0,
	BLEMGR_SERVER_CONNECTED,
	BLEMGR_SERVER_DISCONNECTED,
	BLEMGR_SERVER_MAX,
} blemgr_server_state_e;

typedef enum {
	BLEMGR_SERVER_ADV_NONE = 0,
	BLEMGR_SERVER_ADV_STOPPED,
	BLEMGR_SERVER_ADV_STARTED,
	BLEMGR_SERVER_ADV_MAX,
} blemgr_server_adv_state_e;

typedef enum {
	BLEMGR_CLIENT_NONE = 0,
	BLEMGR_CLIENT_CONNECTED,
	BLEMGR_CLIENT_DISCONNECTED,
	BLEMGR_CLIENT_MAX,
} blemgr_client_state_e;

typedef enum {
	BLEMGR_CLIENT_SCAN_NONE = 0,
	BLEMGR_CLIENT_SCAN_STARTED,
	BLEMGR_CLIENT_SCAN_STOPPED,
	BLEMGR_CLIENT_SCAN_MAX,
} blemgr_client_scan_state_e;
