/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

#ifndef __http_h__
#define __http_h__

#include <mqueue.h>

#ifdef CONFIG_ENDIAN_BIG
#define HTTP_HTONS(ns) (ns)
#define HTTP_HTONL(nl) (nl)
#else
#define HTTP_HTONS(ns)  (unsigned short)(((((unsigned short)(ns)) & 0x00ff) << 8) | \
					((((unsigned short)(ns)) >> 8) & 0x00ff))
#define HTTP_HTONL(nl)  (unsigned long)(((((unsigned long)(nl)) & 0x000000ffUL) << 24) | \
					((((unsigned long)(nl)) & 0x0000ff00UL) <<  8) | \
					((((unsigned long)(nl)) & 0x00ff0000UL) >>  8) | \
					((((unsigned long)(nl)) & 0xff000000UL) >> 24))
#endif

typedef enum {
	HTTP_ERROR_EVENT,
	HTTP_CONNECT_EVENT,
	HTTP_STOP_EVENT,
} http_server_event_t;

struct http_msg_t {
	http_server_event_t event;
	int data;
};

int http_server_mq_flush(mqd_t msg_q);
mqd_t http_server_mq_open(int port);
int http_server_mq_close(int port);
#endif
