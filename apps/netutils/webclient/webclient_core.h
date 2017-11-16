/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
 * @file webclient_common.h
 * @brief APIs for HTTP Client.
 */

#ifndef __WEBCLIENT_CORE_H
#define __WEBCLIENT_CORE_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/compiler.h>
#include <debug.h>

#include <sys/socket.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#ifdef CONFIG_NET_LWIP_NETDB
#include <netdb.h>
#endif
#include <arpa/inet.h>
#include <netinet/in.h>

#include <tinyara/version.h>
#include <apps/netutils/netlib.h>

#include "../webserver/http_string_util.h"
#include "../webserver/http_client.h"
#include <apps/netutils/webclient.h>
#if defined(CONFIG_NETUTILS_CODECS)
#if defined(CONFIG_CODECS_URLCODE)
#	 define WGET_USE_URLENCODE 1
#	 include <apps/netutils/urldecode.h>
#endif
#if defined(CONFIG_CODECS_BASE64)
#	 include <apps/netutils/base64.h>
#endif
#else
#undef CONFIG_CODECS_URLCODE
#undef CONFIG_CODECS_BASE64
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

pthread_addr_t wget_base_11(void *arg);

#ifdef CONFIG_ENABLE_HTTP20
int wget_base_20(void *arg);
#endif

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* __WEBCLIENT_CORE_H */
/**@} */
