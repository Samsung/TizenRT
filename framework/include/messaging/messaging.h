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
 * @brief These configs are used internally for getting receivers information before send.
 * @details MSG_READ_YET : There are more than CONFIG_MESSAGING_RECV_LIST_SIZE receivers, messaging f/w tries to read information again.\n
 * MSG_READ_ALL : All of the receivers' information is read for sending.
 */
#define MSG_READ_YET 0
#define MSG_READ_ALL 1
/**
 * @brief The type of sending message
 * @details MSG_REPLY_REQUIRED : Sender waits the reply until receiving reply from the receiver.\n
 * MSG_REPLY_NO_REQUIRED : Sender cannot receive the reply, and continues to run.
 */
enum msg_reply_type_e {
	MSG_REPLY_NO_REQUIRED = 0,
	MSG_REPLY_REQUIRED = 1,
	MSG_TYPE_MAX
};
typedef enum msg_reply_type_e msg_reply_type_t;

/**
 * @brief The structure of parameters which used for sending
 */
struct msg_send_data_s {
	char *msg;
	int msglen;
	int priority;
};
typedef struct msg_send_data_s msg_send_data_t;

/**
 * @brief The structure of parameters which used for receiving
 */
struct msg_recv_buf_s {
	char *buf;
	int buflen;
	pid_t sender_pid;
};
typedef struct msg_recv_buf_s msg_recv_buf_t;

/**
 * @brief Called when a message is received
 */
typedef void (*msg_callback_t)(msg_reply_type_t msg_type, msg_recv_buf_t *recv_data, void *cb_data);

/**
 * @brief The structure of parameters which has callback information
 */
struct msg_callback_info_s {
	msg_callback_t cb_func;
	void *cb_data;
};
typedef struct msg_callback_info_s msg_callback_info_t;

/**
 * @brief Send(unicast) message with sync mode.
 * @details @b #include <messaging/messaging.h>\n
 * Sender waits after sending message until receiving the reply.
 * @param[in] port_name The message port name to send.
 * @param[in] send_data\n
 *		  msg          : The message to be sent.\n
 *		  msglen       : The length of message to be sent.\n
 *		  priority     : A non-negative integer that specifies the priority of this message.
 * @param reply_buf\n
 *		  [out] buf          : A message buffer to receive the reply message\n
 *		  [in] buflen        : A message size for reply message\n
 *		  [out] sender_pid   : The pid who replies this message
 * @return On success, OK is returned. On failure, ERROR is returned.
 * @since TizenRT v3.0
 */
int messaging_send_sync(const char *port_name, msg_send_data_t *send_data, msg_recv_buf_t *reply_buf);
/**
 * @brief Send(unicast) message with async mode.
 * @details @b #include <messaging/messaging.h>\n
 * Sender registers the reply callback and send the message.
 * @param[in] port_name The message port name to send.
 * @param[in] send_data\n
 *		  msg          : The message to be sent.\n
 *		  msglen       : The length of message to be sent.\n
 *		  priority     : A non-negative integer that specifies the priority of this message.
 * @param reply_buf\n
 *		  [out] buf          : A message buffer to receive the reply message\n
 *		  [in] buflen        : A message size for reply message\n
 *		  [out] sender_pid   : The pid who replies this message
 * @param [in]cb_info\n
 *		  cb_func : A user callback function for receiving reply message\n
 *		  cb_data : A user data which passed to callback function\n
 * @return On success, OK is returned. On failure, ERROR is returned.
 * @since TizenRT v3.0
 */
int messaging_send_async(const char *port_name, msg_send_data_t *send_data, msg_recv_buf_t *reply_buf, msg_callback_info_t *cb_info);
/**
 * @brief Send(unicast) message with noreply mode.
 * @details @b #include <messaging/messaging.h>\n
 * Sender sends the message, but cannot receive the reply. If receiver sends reply to this message port, messages are ignored.
 * @param[in] port_name The message port name to send.
 * @param[in] send_data\n
 *		  msg          : The message to be sent.\n
 *		  msglen       : The length of message to be sent.\n
 *		  priority     : A non-negative integer that specifies the priority of this message.
 * @param reply_buf\n
 *		  [out] buf          : A message buffer to receive the reply message\n
 *		  [in] buflen        : A message size for reply message\n
 *		  [out] sender_pid   : The pid who replies this message
 * @return On success, OK is returned. On failure, ERROR is returned.
 * @since TizenRT v3.0
 */
int messaging_send(const char *port_name, msg_send_data_t *send_data);
/**
 * @brief Reply(unicast) message to specified message port
 * @details @b #include <messaging/messaging.h>\n
 * Receiver should pass sender_pid for replying.
 * @param[in] port_name The message port name to send\n
 *		This port name should be matched to original port name which received the message.
 * @param[in] reply_data\n
 *		  msg      : The message to be sent\n
 *		  msglen   : The length of message to be sent\n
 *		  priority : For replying, priority is set to default.
 * @return On success, OK is returned. On failure, ERROR is returned.
 * @since TizenRT v3.0
 */
int messaging_reply(const char *port_name, pid_t sender_pid, msg_send_data_t *reply_data);

/**
 * @brief Send(multicast) message.
 * @details @b #include <messaging/messaging.h>\n
 * Sender sends the same message to multiple receiver who waits the same message port.
 * @param[in] port_name The message port name to send.
 * @param[in] send_data\n
 *		  msg          : The message to be sent.\n
 *		  msglen       : The length of message to be sent.\n
 *		  priority     : A non-negative integer that specifies the priority of this message.
 * @return On success, the number of receivers who received the message is returned. On failure, ERROR is returned.
 * @since TizenRT v3.0
 */
int messaging_multicast(const char *port_name, msg_send_data_t *send_data);

/**
 * @brief Wait to receive unicast message from specified message port
 * @details @b #include <messaging/messaging.h>\n
 * @param[in] port_name The message port name to receive
 * @param recv_buf
 *		[out] buf         : The message buffer to receive the message\n
 *		[in] buflen       : The length of message to receive\n
 *		[out] sender_pid  : The pid who sends this message\n
 * @return On success, Received message Type is returned. On failure, Error is returned.
 * @since TizenRT v3.0
 */
int messaging_recv_block(const char *port_name, msg_recv_buf_t *recv_buf);
/**
 * @brief Set the callback for receiving messages.
 * @details @b #include <messaging/messaging.h>\n
 * @param[in] port_name The message port name to receive
 * @param recv_buf
 *		[out] buf         : The message buffer to receive the message\n
 *		[in] buflen       : The length of message to receive\n
 *		[out] sender_pid  : The pid who sends this message\n
 * @param [in]cb_info\n
 *		  cb_func : A user callback function for receiving message\n
 *		  cb_data : A user data which passed to callback function\n
 * @return On success, OK is returned. On failure, Error is returned.
 * @since TizenRT v3.0
 */
int messaging_recv_nonblock(const char *port_name, msg_recv_buf_t *recv_buf, msg_callback_info_t *cb_info);
/**
 * @brief Remove the messaging port information if this message port is not used anymore.
 * @details @b #include <messaging/messaging.h>\n
 * @param[in] port_name The message port name.\n
 *		This API should be called from task/pthread who called\n
 *		messaging_recv_nonblock or messaging_unicast_send_async.\n
 *		If this API is not called, memory leak can happen.
 * @return On success, OK is returned. On failure, Error is returned.
 * @since TizenRT v3.0
 */
int messaging_cleanup(const char *port_name);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
/**
 * @}
 */
