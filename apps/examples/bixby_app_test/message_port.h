/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
#ifndef __MESSAGE_PORT_H__
#define __MESSAGE_PORT_H__
#include "tizen_error.h"
#include "bundle.h"

#define TIZEN_ERROR_MESSAGE_PORT 123
typedef void (*message_port_message_cb)(int local_port_id, const char *remote_app_id, const char *remote_port, bool trusted_remote_port, bundle *message, void *user_data);
typedef void (*message_port_trusted_message_cb)(int trusted_local_port_id, const char *remote_app_id, const char *remote_port, bool trusted_remote_port, bundle *message, void *user_data);

struct msg_port_data_s {
	message_port_message_cb cb;
	char *app_id;
	char port[32];
	int port_id;
	void *user_data;
};
typedef struct msg_port_data_s msg_port_data_t;

struct msg_port_info_s {
	char *msg_port;
	int msg_id; //for TizenRT Broadcast
	int port_id;
};
typedef struct msg_port_info_s msg_port_info_t;

/**
 * @brief Enumeration for error codes of a message port.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef enum {
	MESSAGE_PORT_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
	MESSAGE_PORT_ERROR_IO_ERROR = TIZEN_ERROR_IO_ERROR, /**< Internal I/O error */
	MESSAGE_PORT_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
	MESSAGE_PORT_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
	MESSAGE_PORT_ERROR_PORT_NOT_FOUND = TIZEN_ERROR_MESSAGE_PORT | 0x01, /**< The message port of the remote application is not found */
	MESSAGE_PORT_ERROR_CERTIFICATE_NOT_MATCH = TIZEN_ERROR_MESSAGE_PORT | 0x02, /**< The remote application is not signed with the same certificate */
	MESSAGE_PORT_ERROR_MAX_EXCEEDED = TIZEN_ERROR_MESSAGE_PORT | 0x03, /**< The size of the message has exceeded the maximum limit */
	MESSAGE_PORT_ERROR_RESOURCE_UNAVAILABLE = TIZEN_ERROR_MESSAGE_PORT | 0x04 /**< Resource is temporarily unavailable */
} message_port_error_e;

int message_port_register_local_port(const char *local_port, message_port_message_cb callback, void *user_data);
int message_port_unregister_local_port(int local_port_id);
int message_port_check_remote_port(const char *remote_app_id, const char *remote_port, bool *exist);
int message_port_send_message(const char *remote_app_id, const char *remote_port, bundle *message);

int message_port_register_trusted_local_port(const char *trusted_local_port, message_port_trusted_message_cb callback, void *user_data);
int message_port_unregister_trusted_local_port(int trusted_local_port_id);
int message_port_send_trusted_message(const char *remote_app_id, const char *remote_port, bundle *message);
#endif //__MESSAGE_PORT_H__
