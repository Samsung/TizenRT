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

/**
 * @brief Called when a message is received
 */
typedef void (*msg_callback_t)(int msg_type, pid_t sender_pid, void *callback_data, void *user_data);

/**
 * @brief The type of sending message
 * @details MSG_SEND_TYPE_SYNC  : A sender waits after sending message until receiving a reply message.\n
 *	    MSG_SEND_TYPE_ASYNC : A sender continues to run after sending message.
 */
enum msg_send_type_e {
	MSG_SEND_TYPE_SYNC = 0,
	MSG_SEND_TYPE_ASYNC = 1,
	MSG_SEND_TYPE_MAX
};
typedef enum msg_send_type_e msg_send_type_t;

/**
 * @brief The structure of parameters which used for sending
 */
struct msg_send_data_s {
	int priority;
	msg_send_type_t type; /* for sender : MSG_SEND_TYPE_SYNC or MSG_SEND_TYPE_ASYNC */
	const char *msg;
	int msglen;
};
typedef struct msg_send_data_s msg_send_data_t;

/**
 * @brief The type of receiving message
 * @details MSG_RECV_TYPE_BLOCK    : A receiver blocks until receiving the message.\n
 *	    MSG_RECV_TYPE_NONBLOCK : A receiver sets callback function for receiving message and continues to run.
 */
enum msg_recv_type_e {
	MSG_RECV_TYPE_BLOCK = 0,
	MSG_RECV_TYPE_NONBLOCK = 1,
	MSG_RECV_TYPE_MAX
};
typedef enum msg_recv_type_e msg_recv_type_t;

/**
 * @brief The structure of data buffer which used for receiving and replying
 */
struct msg_data_s {
	char *msg;
	int msglen;
	pid_t sender_pid;
	int msg_type;
};
typedef struct msg_data_s msg_data_t;

/**
 * @brief The structure of parameters which used for receiving
 */
struct msg_recv_param_s {
	msg_recv_type_t type; /* for receiver : MSG_RECV_BLOCK or MSG_RECV_NONBLOCK */
	msg_callback_t cb_func;
	char *cb_data;
};
typedef struct msg_recv_param_s msg_recv_param_t;
/**
 * @brief Send unicast message to specified message port
 * @details @b #include <messaging/messaging.h>\n
 * @param[in] port_name The message port name to send
 * @param[in] send_data
 *		  msg          : The message to be sent\n
 *		  msglen       : The length of message to be sent\n
 *		  priority     : A non-negative integer that specifies the priority of this message\n
 *		  type         : MSG_SEND_TYPE_SYNC or MSG_SEND_TYPE_ASYNC\n
 * @param reply_data
 *		  [out] reply_msg    : A message buffer to receive the sync reply message\n
 *		  [in] reply_msglen  : A message size for sync reply message
 *		  [out] sender_pid   : The pid who replies this message\n
 *		  [out] msg_type     : For replying, msg_type is set to default, MSG_SEND_TYPE_ASYNC.
 * @return On success, OK is returned. On failure, Error is returned.
 * @since TizenRT v3.0
 */
int messaging_send(const char *port_name, msg_send_data_t *send_data, msg_data_t *reply_data);
/**
 * @brief Wait to receive unicast message from specified message port
 * @details @b #include <messaging/messaging.h>\n
 * @param[in] port_name The message port name to receive
 * @param data\n
 *		[out] msg         : The message buffer to receive the message\n
 *		[in] msglen       : The length of message to receive\n
 *		[out] sender_pid  : The pid who sends this message\n
 *		[out] msg_type    : The received message type, MSG_SEND_TYPE_SYNC or MSG_SEND_TYPE_ASYNC.\n
 *				   If msg_type is MSG_SEND_TYPE_SYNC, receiver should reply with messaging_send_reply.
 * @param[in] param\n
 *		type    : MSG_RECV_BLOCK or MSG_RECV_NONBLOCK\n
 *		cb_func : user callback function for nonblocking receive\n
 *		cb_data : user callback data for nonblocking receive\n
 * @return On success, OK is returned. On failure, Error is returned.
 * @since TizenRT v3.0
 */
int messaging_recv(const char *port_name, msg_data_t *data, msg_recv_param_t *param);
/**
 * @brief Reply unicast message to specified message port when received sync message
 * @details @b #include <messaging/messaging.h>\n
 * Receiver should set sender_pid in reply_data who receives this reply.
 * @param[in] port_name The message port name to send\n
 *		This port name should be matched to original port name which received the message.
 * @param[in] reply_data
 *		  reply_msg    : The message to be sent\n
 *		  reply_msglen : The length of message to be sent\n
 *		  sender_pid   : The pid who will receive this reply\n
 * 		  msg_type     : For replying, msg_type is set to default, MSG_SEND_TYPE_ASYNC.
 * @return On success, OK is returned. On failure, Error is returned.
 * @since TizenRT v3.0
 */
int messaging_send_reply(const char *port_name, msg_data_t *reply_data);
/**
 * @brief Unregister the messaging port when set non-block receive
 * @details @b #include <messaging/messaging.h>\n
 * @param[in] port_name The message port name to unregister\n
 *		This API should be called when calling messaging_recv with non-block type.\n
 *		If this API is not called, memory leak can happen.
 * @return On success, OK is returned. On failure, Error is returned.
 * @since TizenRT v3.0
 */
int messaging_unregister(const char *port_name);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
/**
 * @}
 */
