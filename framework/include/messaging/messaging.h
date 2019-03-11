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
/**
 * @defgroup Messaging Messaging
 * @ingroup Messaging
 * @brief Provides APIs for Messaging
 * @{
 */

/**
 * @file messaging/messaging.h
 */
#ifndef __MESSAGING_H__
#define __MESSAGING_H__

#include <tinyara/config.h>

/**
 * @brief The type of sending message
 * @details MSG_SEND_SYNC  : A sender waits after sending message until receiving a reply message.\n
 * 	    MSG_SEND_ASYNC : A sender continues to run after sending message.
 */
#define MSG_SEND_SYNC     0
#define MSG_SEND_ASYNC    1

/**
 * @brief The type of receiving message
 * @details MSG_RECV_BLOCK    : A receiver blocks until receiving the message.\n
 * 	    MSG_RECV_NONBLOCK : A receiver sets callback function for receiving message and continues to run.
 */
#define MSG_RECV_BLOCK    2
#define MSG_RECV_NONBLOCK 3

/**
 * @brief Called when a message is received
 */
typedef void (*msg_callback_t)(void *data, void *callback_data);

/**
 * @brief The structure of parameters which used for sending
 */
struct msg_send_param_s {
	int priority;
	int type; /* for sender : MSG_SEND_SYNC or MSG_SEND_ASYNC */
	char *sync_msg;
	int sync_msglen;
};
typedef struct msg_send_param_s msg_send_param_t;

/**
 * @brief The structure of parameters which used for receiving
 */
struct msg_recv_param_s {
	int type; /* for receiver : MSG_RECV_BLOCK or MSG_RECV_NONBLOCK */
	msg_callback_t cb_func;
};
typedef struct msg_recv_param_s msg_recv_param_t;
/**
 * @brief Send unicast message to specified message port
 * @details @b #include <messaging/messaging.h>\n
 * @param[in] port_name The message port name to send
 * @param[in] msg The message to be sent
 * @param[in] msglen The length of message to be sent
 * @param[in/out] param\n
 * 		priority    : A non-negative integer that specifies the priority of this message\n
 * 		type        : MSG_SEND_SYNC or MSG_SEND_ASYNC\n
 * 		sync_msg    : A message buffer to receive the sync reply message\n
 * 		sync_msglen : A message size for sync reply message
 * @return On success, OK is returned. On failure, Error is returned.
 * @since TizenRT v2.0
 */
int messaging_send(const char *port_name, const char *msg, size_t msglen, msg_send_param_t *param);
/**
 * @brief Wait to receive unicast message from specified message port
 * @details @b #include <messaging/messaging.h>\n
 * @param[in] port_name The message port name to receive
 * @param[in] msg The message buffer to receive the message
 * @param[in] msglen The length of message to receive
 * @param[in] param\n
 * 		type    : MSG_RECV_BLOCK or MSG_RECV_NONBLOCK\n
 * 		cb_func : user callback function for nonblocking receive
 * @return On success, OK is returned. On failure, Error is returned.
 * @since TizenRT v2.0
 */
int messaging_recv(const char *port_name, char *msg, size_t msglen, msg_recv_param_t *param);
/**
 * @brief Reply unicast message to specified message port when received sync message
 * @details @b #include <messaging/messaging.h>\n
 * @param[in] port_name The message port name to send\n
 * 			This port name should be matched to original port name which received the message.
 * @param[in] msg The message to be sent
 * @param[in] msglen The length of message to be sent
 * @return On success, OK is returned. On failure, Error is returned.
 * @since TizenRT v2.0
 */
int messaging_send_reply(const char *port_name, const char *msg, size_t msglen);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
/**
 * @}
 */
