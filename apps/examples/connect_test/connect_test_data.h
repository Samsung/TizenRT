/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License\n");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License
 ****************************************************************************/
#pragma once
#define CT_TLS_DTLS_PORT "8888"
#define PORT 8888
#define BUF_SIZE 1024
#define ACK_SIZE 128
#define MAC_BUF_SIZE 13
#define IP_BUF_SIZE 15
#define BUF_SIZE_UDP 256
#define ACK_SIZE_UDP 128

#define DECLARE_TIMEOUT             \
	struct timeval tv;          \
	tv.tv_sec = 30;             \
	tv.tv_usec = 0;             

#define DATA_TRANSFER_ERROR                                             \
	if (nbytes == 0) {                                              \
		CT_LOGE(TAG, "connection closed");                      \
		return -1;                                              \
	} else if (nbytes < 0) {                                        \
		if (errno == EWOULDBLOCK) {			        \
			CT_LOGE(TAG, "timeout error %d", errno);        \
			return -1;                                      \
		} else {                                                \
			CT_LOGE(TAG, "connection error %d", errno);     \
			return -1;                                      \
		}                                                       \
	}

static int g_listenfd = -1;
#define TERMINATE                                      \
	if (ret < 0) {                                 \
		close(sockfd);                         \
		if (g_listenfd > -1) {		       \
			close(g_listenfd);               \
		}                                      \
		return -1;                             \
	}

