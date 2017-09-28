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

/* ============================================================
 * Control compile time options.
 * ============================================================
 *
 * Compile time options have moved to config.mk.
 */

/* ============================================================
 * Compatibility defines
 *
 * Generally for Windows native support.
 * ============================================================ */

#if defined(_MSC_VER) && _MSC_VER < 1900
#	define snprintf sprintf_s
#endif

#ifdef WIN32
#	ifndef strcasecmp
#		define strcasecmp strcmpi
#	endif
#define strtok_r strtok_s
#define strerror_r(e, b, l) strerror_s(b, l, e)
#endif

#define uthash_malloc(sz) _mosquitto_malloc(sz)
#define uthash_free(ptr,sz) _mosquitto_free(ptr)

#ifndef EPROTO
#	define EPROTO ECONNABORTED
#endif

#ifdef WITH_MBEDTLS
#include "mbedtls/ssl.h"
#include "mbedtls/net.h"
#include "mbedtls/x509_crt.h"
#include "mbedtls/pk.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/ssl_cache.h"
#include "mbedtls/entropy.h"
#endif
