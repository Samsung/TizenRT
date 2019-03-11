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
#include <messaging/messaging.h>

#define MSG_ALLOC(a) malloc(a)
#define MSG_FREE(a) free(a)
#ifdef CONFIG_CPP_HAVE_VARARGS
#define MSG_ASPRINTF(p, f, ...) asprintf(p, f, ##__VA_ARGS__)
#else
#define MSG_ASPRINTF asprintf
#endif

#define MSG_HEADER_SIZE (sizeof(pid_t) + sizeof(int))

struct msg_recv_info_s {
	mqd_t mqdes;
	msg_callback_t user_cb;
	char *cb_data;
};
typedef struct msg_recv_info_s msg_recv_info_t;
