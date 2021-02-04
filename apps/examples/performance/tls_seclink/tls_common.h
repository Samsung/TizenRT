/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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
#include <tinyara/config.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <tinyara/timer.h>

#include "mbedtls/config.h"
#include "tls_cert.h"

#if defined(CONFIG_TLS_SECLINK_DEBUG)
#define TLS_SECLINK_LOG printf
#else
#define TLS_SECLINK_LOG
#endif

#define TLS_SECLINK_SERVER_LOG(...)              \
	do {                                         \
		TLS_SECLINK_LOG("[SERVER] "__VA_ARGS__); \
	} while (0);

#define TLS_SECLINK_CLIENT_LOG(...)              \
	do {                                         \
		TLS_SECLINK_LOG("[CLIENT] "__VA_ARGS__); \
	} while (0);

#if defined(CONFIG_TLS_SECLINK_DEBUG_LEVEL)
#define DEBUG_LEVEL CONFIG_TLS_SECLINK_DEBUG_LEVEL
#else
#define DEBUG_LEVEL 0
#endif

#define TLS_SERVER_PRI 105
#define TLS_STACKSIZE 10240

#include "mbedtls/net.h"
#include "mbedtls/debug.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "mbedtls/certs.h"
#include "mbedtls/timing.h"

#define SERVER_PORT "4433"
#define SERVER_ADDR "127.0.0.1"

int tls_print_test_log(void);
void *tls_client_seclink_main(void *arg);
void *tls_server_seclink_main(void *arg);
