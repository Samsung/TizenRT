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
#include <tinyara/compiler.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <queue.h>
#include <messaging/messaging.h>


/* @cond
 * @internal
 */
#define MSG_ALLOC(a) malloc(a)
#define MSG_FREE(a) free(a)
#ifdef CONFIG_CPP_HAVE_VARARGS
#define MSG_ASPRINTF(p, f, ...) asprintf(p, f, ##__VA_ARGS__)
#else
#define MSG_ASPRINTF asprintf
#endif

#define MSG_VERSION 1
/* Messaging Version 1 */
struct messaging_packet_s {
	uint32_t version;
	uint32_t offset;
	pid_t sender_pid;
	uint32_t msg_type;
	char *message;
};
typedef struct messaging_packet_s messaging_packet_t;
#define MSG_HEADER_SIZE (sizeof(messaging_packet_t) - sizeof(char *)) /* Messaging Version 1 */

#define MAX_PORT_NAME_SIZE 64

/**
 * @brief The type of handling message internally
 * @details MSG_INFO_SAVE    : For saving receiver information\n
 * MSG_INFO_READ : For reading receiver information\n
 * MSG_INFO_REMOVE : For removing receiver information
 */
enum msg_info_type_e {
	MSG_INFO_SAVE = 0,
	MSG_INFO_READ = 1,
	MSG_INFO_REMOVE = 2,
	MSG_INFO_TYPE_MAX
};
typedef enum msg_info_type_e msg_info_type_t;

#define SAVE_MSG_RECEIVER(port_name)  messaging_handle_data(MSG_INFO_SAVE, port_name, NULL, NULL)
#define READ_MSG_RECEIVER(port_name, recv_arr, recv_cnt)  messaging_handle_data(MSG_INFO_READ, port_name, recv_arr, &recv_cnt)
#define FREE_MSG_RECEIVER(port_name)  messaging_handle_data(MSG_INFO_REMOVE, port_name, NULL, NULL)

/**
 * @brief The type of sending message
 * @details MSG_SEND_SYNC : Unicast send message type with sync mode\n
 * MSG_SEND_ASYNC : Unicast send message type with async mode
 * MSG_SEND_MULTI : Multicast send message type
 */
enum msg_send_type_e {
	MSG_SEND_NOREPLY = 0,
	MSG_SEND_SYNC = 1,
	MSG_SEND_ASYNC = 2,
	MSG_SEND_MULTI = 3,
	MSG_SEND_REPLY = 4,
	MSG_SEND_TYPE_MAX
};
typedef enum msg_send_type_e msg_send_type_t;

/**
 * @brief The internal structure for callback information and mq descriptor
 */
struct msg_recv_info_s {
	mqd_t mqdes;
	msg_recv_buf_t *msg;
	msg_callback_t user_cb;
	char *cb_data;
	char port_name[MAX_PORT_NAME_SIZE];
};
typedef struct msg_recv_info_s msg_recv_info_t;

/**
 * @brief The internal structure for saving message port information. It will be used when cleanup.
 */
struct msg_port_info_s {
	struct msg_port_info_s *flink;
	char name[MAX_PORT_NAME_SIZE];
	mqd_t mqdes;
	void *data;
	pid_t pid;
};
typedef struct msg_port_info_s msg_port_info_t;

/**
 * @brief Internal function for setting callback function to the messaging signal.
 */
int messaging_set_notify_signal(int signo, _sa_sigaction_t cb_func);
/**
 * @brief Internal function for setting mq notification.
 */
int messaging_set_notification(int signo, msg_recv_info_t *data);
/**
 * @brief Internal function for save/read the message information when sending and receiving the message.
 */
int messaging_handle_data(msg_info_type_t type, const char *port_name, int *data_arr, int *recv_cnt);
/**
 * @brief Internal function for unicast and multicast send APIs.
 */
int messaging_send_internal(const char *port_name, msg_send_type_t msg_type, msg_send_data_t *send_data, msg_recv_buf_t *recv_data, msg_callback_info_t *cb_info);
/**
 * @brief Internal function for sending message packet which has header and message.
 */
int messaging_send_packet(const char *port_name, msg_send_type_t msg_type, msg_send_data_t *send_data, msg_callback_info_t *cb_info);
/**
 * @brief Internal function for receiving APIs.
 */
int messaging_recv_internal(const char *port_name, msg_recv_buf_t *recv_buf, msg_callback_info_t *cb_info);
/**
 * @brief Internal function for running async/non-block callback
 */
void messaging_run_callback(int signo, siginfo_t *data);
/**
 * @brief Internal function for parsing received packet
 */
int messaging_parse_packet(char *packet, char *buf, int buflen, pid_t *sender_pid, int *msg_type);
/**
 * @brief Internal function for getting g_port_info_list
 */
sq_queue_t *messaging_get_port_info_list(void);
/*
 *@endcond
 */
