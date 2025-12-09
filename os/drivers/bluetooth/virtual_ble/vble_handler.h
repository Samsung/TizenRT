/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
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
#ifndef __VBLE_HANDLER_H__
#define __VBLE_HANDLER_H__

#define VBLE_LOG vdvdbg
#define VBLE_ERROR(res) vddbg("T%d error %d %d %s:%d\n", getpid(), res, errno, __FUNCTION__, __LINE__)

#include <ble_manager/ble_manager.h>

#define VBLE_NO_CONNECTION 3

// Set return result else.
#define VBLE_CMD_SET 0x0
// Generate an event
#define VBLE_CMD_GEN_EVT 0x1

extern trble_client_init_config client_init_parm;
extern trble_server_init_config server_init_parm;
extern bool is_server_init;
extern bool is_client_init;
extern bool is_scan_started;
extern uint16_t server_profile_count;
extern trble_device_connected connected_device[TRBLE_MAX_CONNECTION_COUNT];
extern trble_connected_list vble_connected_list;

typedef struct {
	int key;
	int value;
	int wait;
} vble_ioctl_msg_s;

typedef enum {
	// Common
    VBLE_MSG_INIT,
    VBLE_MSG_DEINIT,
    VBLE_MSG_GET_MAC_ADDR,
    VBLE_MSG_GET_BONDED_DEVICE,
    VBLE_MSG_DELETE_BONDED,
    VBLE_MSG_DELETE_BONDED_ALL,
    VBLE_MSG_CONN_IS_ACTIVE,
    VBLE_MSG_CONN_IS_ANY_ACTIVE,
    VBLE_MSG_IOCTL,

    // Observer
    VBLE_MSG_START_SCAN,
    VBLE_MSG_STOP_SCAN,
    VBLE_MSG_SCAN_WHITELIST_ADD,
    VBLE_MSG_SCAN_WHITELIST_DELETE,
    VBLE_MSG_SCAN_WHITELIST_CLEAR_ALL,

    // Client
    VBLE_MSG_CLIENT_CONNECT,
    VBLE_MSG_CLIENT_DISCONNECT,
    VBLE_MSG_CLIENT_DISCONNECT_ALL,
    VBLE_MSG_CONNECTED_DEVICE_LIST,
    VBLE_MSG_CONNECTED_INFO,
    VBLE_MSG_OPERATION_ENABLE_NOTIFICATION,
    VBLE_MSG_OPERATION_READ,
    VBLE_MSG_OPERATION_WRITE,
    VBLE_MSG_OPERATION_WRITE_NO_RESPONSE,

    // Server
    VBLE_MSG_GET_PROFILE_COUNT,
    VBLE_MSG_CHARACTER_NOTIFY,
    VBLE_MSG_ATTR_SET_DATA,
    VBLE_MSG_ATTR_GET_DATA,
    VBLE_MSG_ATTR_REJECT,
    VBLE_MSG_SERVER_DISCONNECT,
    VBLE_MSG_GET_MAC_ADDR_BY_CONN_HANDLE,
    VBLE_MSG_GET_CONN_HANDLE_BY_ADDR,

    // Broadcaster
    VBLE_MSG_SET_ADV_DATA,
    VBLE_MSG_SET_ADV_RESP,
    VBLE_MSG_SET_ADV_TYPE,
    VBLE_MSG_SET_ADV_INTERVAL,
    VBLE_MSG_START_ADV,
    VBLE_MSG_STOP_ADV,
} vble_req_e;

struct vconfigs {
	trble_client_init_config *client;
	trble_server_init_config *server;
};

typedef struct {
	trble_attr_handle attr_handle;
	trble_data *data;
} vattr;

typedef struct {
	vattr attr_data[7];
	uint8_t count;
} vattr_table;

extern vattr_table attr_table;

struct vble_req {
	vble_req_e type;
	void *arg;
	trble_result_e *res;
};

struct vble_msg {
	struct vble_req *req;
	void *signal;
};

int vble_handle_message(struct vble_req *req);
int vble_create_event(uint32_t event, int32_t res, int32_t sleep);
static void init_handler(struct vconfigs *config);
static void connect_handler(trble_conn_info *conn_info);
static void disconnect_all_handler(void);
static void connected_device_list_handler(trble_connected_list *out_connected_list);
static void scan_handler(trble_scan_filter *filter);
static void ioctl_handler(trble_msg_s *msg);

#endif // #define __vble_HANDLER_H__
