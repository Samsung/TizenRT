/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef __BT_ADAPTATION_GATT_H__
#define __BT_ADAPTATION_GATT_H__

#include <bluetooth/bluetooth_type.h>
#include <bluetooth/bluetooth_type_internal.h>

#include "bt-adaptation-common.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#define BT_GATT_SERVICES_MAX 40 /**< This specifies maximum number of services
							a device can support */

#define BT_ATT_DATA_MAX 610 /**< GATT ATT value max len */

#define BT_UUID_HEX_MAX 16 /**< UUID complete len in hex */

typedef enum {
	BT_GATT_SERVICE_CHANGE_TYPE_ADD = 0x01,
	BT_GATT_SERVICE_CHANGE_TYPE_REMOVE = 0x02,
} bt_gatt_svc_change_type_t;

/**
 * GATT Server Response/Indication data
 */
typedef struct {
	uint8_t data[BT_ATT_DATA_MAX];
	int length;
	int offset;
} bt_gatt_att_data_t;

/**
 * Structure to GATT attribute handle data
 */
typedef struct {
	int count;
	char **handle;
	char uuids[BT_GATT_SERVICES_MAX][BT_UUID_STR_MAX];
	int inst_id[BT_GATT_SERVICES_MAX];
} bt_gatt_handle_info_t;

/**
 * Structure to a most basic GATT attribute handle data
 */
typedef struct {
	int instance_id;
	unsigned char uuid[BT_UUID_HEX_MAX];
	char address[BT_ADDR_STR_MAX];
} bt_gatt_handle_prop_t;

/**
 * Structure to GATT Remote service data
 */

typedef struct {
	char *uuid;
	char *handle;
	bool primary;
	bt_gatt_handle_info_t include_handles;
	bt_gatt_handle_info_t char_handle;
	bt_gatt_handle_prop_t prop; /* Added Service UUID, instance_id & associated remote device  */
} bt_gatt_svc_prop_t;

/**
 * Structure to GATT Remote characteristic data
 */

typedef struct {
	char *service_handle;
	bt_gatt_handle_info_t handle_info;
} bt_gatt_discovered_char_t;

/**
 * Structure to format of GATT Characteristic Value
 */

typedef struct {
	unsigned char format;
	unsigned char exponent;
	unsigned short unit;
	unsigned char name_space;
	unsigned short description;
} bt_gatt_char_format_t;

/**
 * Structure to GATT Characteristic property
 */

typedef struct {
	char *handle;
	char *uuid;
	char *name;
	char *description;
	bt_gatt_char_format_t format;
	unsigned char *val;
	unsigned int val_len;
	unsigned int permission;
	char *representation;
	bt_gatt_handle_info_t char_desc_handle;
	bt_gatt_handle_prop_t prop;      /* Added Char UUID, instance_id */
	bt_gatt_handle_prop_t svc_prop;  /* Added Service UUID, instance_id */
	char value[BT_ATT_DATA_MAX]; /* Added */
	char address[BT_ADDR_STR_MAX];         /* Added */
} bt_gatt_char_prop_t;

/**
 * Structure to GATT Characteristic descriptor property
 */

typedef struct {
	char *handle;
	char *uuid;
	unsigned char *val;
	unsigned int val_len;
	bt_gatt_handle_prop_t prop;      /* Added Descriptor UUID, instance_id */
	bt_gatt_handle_prop_t char_prop; /* Added Char UUID, instance_id */
	bt_gatt_handle_prop_t svc_prop;  /* Added Service UUID, instance_id */
	char value[BT_ATT_DATA_MAX];/* Added */
	char address[BT_ADDR_STR_MAX];          /* Added */
} bt_gatt_char_desc_prop_t;

/**
 * Structure to GATT Characteristic value
 */

typedef struct {
	char *char_handle;
	uint8_t *char_value;
	uint32_t val_len;
} bt_gatt_char_value_t;

typedef struct {
	unsigned char UUID[BT_UUID_HEX_MAX];
	char address[BT_ADDR_STR_MAX];
	char *val;
	int len;
} bt_gatt_notify_req_t ;

/**
 * Structure to GATT Read Request
 */
typedef struct {
	char *att_handle;
	char *service_handle;
	char *address;
	uint16_t offset;
	unsigned int req_id;
} bt_gatt_read_req_t;

/**
 * Structure to GATT Value change
 */
typedef struct {
	char *att_handle;
	char *service_handle;
	char *address;
	unsigned int req_id;
	bool response_needed;
	uint16_t offset;
	uint8_t *att_value;
	uint32_t val_len;
} bt_gatt_value_change_t;

typedef struct {
	char address[BT_ADDR_STR_MAX];
	char *svc_path;
	bt_gatt_svc_change_type_t change_type;
} bt_gatt_svc_change_t;

/**
 * Structure to GATT characteristc Notification change
 */
typedef struct {
	char *att_handle;
	char *service_handle;
	bool att_notify;
} bt_gatt_char_notify_change_t;

/**
 * Structure to Indication confirmation
 */
typedef struct {
	char *att_handle;
	char *service_handle;
	char *address;
	bool complete;
} bt_gatt_indicate_confirm_t;

/**
 * Structure for GATT response data
 */
typedef struct {
	char *handle;
	uint8_t *value;
	uint32_t len;
	void *user_data;
} bt_gatt_resp_data_t;

typedef struct {
	char address[BT_ADDR_STR_MAX];
	int count;
	char uuids[BT_GATT_SERVICES_MAX][BT_UUID_STR_MAX];
	int inst_id[BT_GATT_SERVICES_MAX];
	int primary[BT_GATT_SERVICES_MAX];
} bt_gatt_svc_browse_info_t;

/* Structure used for browsing a particular service's properties(chars and Incldued svcs) */
typedef struct {
	char address[BT_ADDR_STR_MAX];
	bt_gatt_handle_prop_t svc;
} bt_gatt_svc_prop_info_t;

typedef struct {
	char address[BT_ADDR_STR_MAX];
	int count;
	unsigned char svc_uuid[BT_UUID_HEX_MAX];
	int svc_inst_id;
	char uuids[BT_GATT_SERVICES_MAX][BT_UUID_STR_MAX];
	int inst_id[BT_GATT_SERVICES_MAX];
	int props[BT_GATT_SERVICES_MAX];
} bt_gatt_char_info_t;

typedef struct {
	char address[BT_ADDR_STR_MAX];
	int count;
	unsigned char svc_uuid[BT_UUID_HEX_MAX];
	int svc_inst_id;
	char uuids[BT_GATT_SERVICES_MAX][BT_UUID_STR_MAX];
	int inst_id[BT_GATT_SERVICES_MAX];
} bt_gatt_included_svc_info_t;

typedef struct {
	char address[BT_ADDR_STR_MAX];
	int count;
	unsigned char svc_uuid[BT_UUID_HEX_MAX];
	int svc_inst_id;
	unsigned char char_uuid[BT_UUID_HEX_MAX];
	int char_inst_id;
	int char_props_map;
	char uuids[BT_GATT_SERVICES_MAX][BT_UUID_STR_MAX];
	int inst_id[BT_GATT_SERVICES_MAX];
} bt_gatt_desc_info_t;

/* Structure used for browsing a particular charactertic's properties(descriptors)  or reading char's value */
typedef struct {
	char address[BT_ADDR_STR_MAX];
	bt_gatt_handle_prop_t svc;
	bt_gatt_handle_prop_t characteristic;
} bt_gatt_char_prop_info_t;

/* Structure used for reading Descriptor's value */
typedef struct {
	char address[BT_ADDR_STR_MAX];
	bt_gatt_handle_prop_t svc;
	bt_gatt_handle_prop_t characteristic;
	bt_gatt_handle_prop_t descriptor;
} bt_gatt_desc_prop_info_t;


/*******************************
 * GATT Common Function Header *
********************************/

int bt_adapt_gatt_free_svc_property(bt_gatt_svc_prop_t *svc_pty);

int bt_adapt_gatt_free_char_property(bt_gatt_char_prop_t *char_pty);

int bt_adapt_gatt_free_desc_property(bt_gatt_char_desc_prop_t *desc_pty);

int bt_adapt_gatt_connect(const char *address, bool auto_connect, int client_id);

int bt_adapt_gatt_disconnect(const char *address, int client_id);

/*******************************
 * GATT Client Function Header *
********************************/

int bt_adapt_gatt_client_init(const char *address, int *client_id);

int bt_adapt_gatt_client_deinit(int client_id);

int bt_adapt_gatt_client_get_primary_services(
		const char *address, /* Remote GATT Server */
		bt_gatt_handle_info_t *prim_svc); /* UUID & instance_id */

int bt_adapt_gatt_client_get_svc_property(
		const char *address,
		bt_gatt_handle_prop_t *service_handle,
		bt_gatt_svc_prop_t *service);

int bt_adapt_gatt_client_get_char_property(
		const char *address,
		bt_gatt_handle_prop_t *service_handle,
		bt_gatt_handle_prop_t *char_handle,
		bt_gatt_char_prop_t *char_property);

int bt_adapt_gatt_client_get_char_desc_property(
		const char *address,
		bt_gatt_handle_prop_t *service_handle,
		bt_gatt_handle_prop_t *char_handle,
		bt_gatt_handle_prop_t *descriptor_handle,
		bt_gatt_char_desc_prop_t *desc_prop);

int bt_adapt_gatt_client_watch_char(
			const char *address,
			bt_gatt_handle_prop_t *service_handle,
			bt_gatt_handle_prop_t *char_handle,
			int client_id,
			bool is_notify,
			bool is_indicate);

int bt_adapt_gatt_client_unwatch_char(
			const char *address,
			bt_gatt_handle_prop_t *service_handle,
			bt_gatt_handle_prop_t *char_handle,
			int client_id);

int bt_adapt_gatt_client_read_char_value(
		const char *address,
		bt_gatt_handle_prop_t *service_handle,
		bt_gatt_handle_prop_t *char_handle);

int bt_adapt_gatt_client_read_desc_value(
		const char *address,
		bt_gatt_handle_prop_t *service_handle,
		bt_gatt_handle_prop_t *char_handle,
		bt_gatt_handle_prop_t *descriptor_handle);

int bt_adapt_gatt_client_write_char_value_by_type(
		const char *address,
		bt_gatt_handle_prop_t *service_handle,
		bt_gatt_handle_prop_t *char_handle,
		bt_gatt_att_data_t *data,
		bt_gatt_write_type_e  write_type);

int bt_adapt_gatt_client_write_desc_value(
		const char *address,
		bt_gatt_handle_prop_t *service_handle,
		bt_gatt_handle_prop_t *char_handle,
		bt_gatt_handle_prop_t *descriptor_handle,
		bt_gatt_att_data_t *data,
		bt_gatt_write_type_e  write_type);

int bt_adapt_gatt_client_set_service_change_watcher(
		const char *address, bool enable);

/*******************************
 * GATT Server Function Header *
********************************/

int bt_adapt_gatt_server_init(int *instance_id);

int bt_adapt_gatt_server_deinit(void);

int bt_adapt_gatt_server_register(int instance_id);

int bt_adapt_gatt_server_unregister(int instance_id);

int bt_adapt_gatt_server_add_service(const char *svc_uuid, int type, int numhandles, int instance_id, int *service_handle);

int bt_adapt_gatt_server_add_included_service(int instance_id, int service_handle, int included_handle);

int bt_adapt_gatt_server_add_new_char(const char *char_uuid,
		bt_gatt_permission_e permissions, int properties,
		int service_handle, int instance_id, int *char_handle);

int bt_adapt_gatt_server_add_desc(const char *desc_uuid,
		bt_gatt_permission_e permissions, int service_handle,
		int instance_id, int *descriptor_handle);

int bt_adapt_gatt_server_start_service(int service_handle, int instance_id);

int bt_adapt_gatt_server_stop_service(int service_handle, int instance_id);

int bt_adapt_gatt_server_delete_service(int service_handle, int instance_id);

int bt_adapt_gatt_server_send_indication(const char *address,
		int atrribute_handle,
		bool need_confirmation,
		int instance_id,
		const bt_gatt_att_data_t *att_value);

int bt_adapt_gatt_server_send_response(bt_gatt_att_request_type_e req_type,
		int request_id,
		int response_status,
		bool auth_req,
		const bt_gatt_att_data_t *value);

int bt_adapt_gatt_server_update_multi_adv_instance(const char *unique_name, int instance_id);

int bt_adapt_gatt_server_update_char(int instance_id,
		int attribute_handle,
		const char *value,
		int length);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __BT_ADAPTATION_GATT_H__ */
