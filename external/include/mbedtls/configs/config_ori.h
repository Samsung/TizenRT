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
/*
 *  Minimal configuration for TLS 1.2 with RSA Key exchange and authentication, AES 128/256, and SHA256
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */
#ifndef MBEDTLS_CONFIG_H
#define MBEDTLS_CONFIG_H

/* #define MBEDTLS_DEBUG_C */

/* For Session */
#define MBEDTLS_SSL_CACHE_C

/* mbed TLS feature support */
#define MBEDTLS_SSL_PROTO_TLS1_2

/* Error module - Enable to run test programs */
#define MBEDTLS_ERROR_C

/* mbed TLS modules */
#define MBEDTLS_SSL_CLI_C
#define MBEDTLS_SSL_SRV_C
#define MBEDTLS_SSL_TLS_C
#define MBEDTLS_CIPHER_C
#define MBEDTLS_CTR_DRBG_C
#define MBEDTLS_ENTROPY_C
#define MBEDTLS_NET_C

/* Key exchange and authentication: RSA and RSA */
#define MBEDTLS_KEY_EXCHANGE_RSA_ENABLED
#define MBEDTLS_RSA_C
#define MBEDTLS_DHM_C
#define MBEDTLS_HARDWARE_RSA
#define MBEDTLS_KEY_EXCHANGE_DHE_RSA_ENABLED

/* Encryption */
#define MBEDTLS_AES_C
#define MBEDTLS_CIPHER_MODE_CBC
//#define MBEDTLS_GCM_C
/* for Padding */
#define MBEDTLS_CIPHER_PADDING_PKCS7
#define MBEDTLS_CIPHER_PADDING_ONE_AND_ZEROS

/* MAC */
#define MBEDTLS_SHA1_C
#define MBEDTLS_SHA256_C
#define MBEDTLS_SHA1_PROCESS_ALT
/* Additions */
#define MBEDTLS_BIGNUM_C
//#define MBEDTLS_DHM_C
#define MBEDTLS_RSA_C
#define MBEDTLS_OID_C
#define MBEDTLS_X509_CRT_PARSE_C
#define MBEDTLS_X509_USE_C
#define MBEDTLS_PK_PARSE_C
#define MBEDTLS_PKCS1_V15
#define MBEDTLS_MD_C
#define MBEDTLS_PK_C
#define MBEDTLS_PKCS12_C
#define MBEDTLS_RSA_C
#define MBEDTLS_PEM_PARSE_C
#define MBEDTLS_BASE64_C
#define MBEDTLS_ASN1_WRITE_C
#define MBEDTLS_ASN1_PARSE_C
#define MBEDTLS_FS_IO

/* Certificates related */
#define MBEDTLS_X509_CHECK_KEY_USAGE
#define MBEDTLS_X509_CHECK_EXTENDED_KEY_USAGE
//#define MBEDTLS_X509_CSR_PARSE_C
#define MBEDTLS_X509_CREATE_C
#define MBEDTLS_X509_CRT_WRITE_C
#define MBEDTLS_X509_CSR_WRITE_C
#define MBEDTLS_PK_WRITE_C
#define MBEDTLS_PEM_PARSE_C
#define MBEDTLS_BASE64_C
#define MBEDTLS_PEM_WRITE_C
#define MBEDTLS_GENPRIME

// Test certificates. Enable to run test programs
#define MBEDTLS_CERTS_C

/* Save RAM at the expense of ROM */
#define MBEDTLS_AES_ROM_TABLES

/*
 * You should adjust this to the exact number of sources you're using: default
 * is the "platform_entropy_poll" source, but you may want to add other ones
 * Minimum is 2 for the entropy test suite.
 */
#define MBEDTLS_ENTROPY_MAX_SOURCES 2

/*
 * Save RAM at the expense of interoperability: do this only if you control
 * both ends of the connection!  (See comments in "mbedtls/ssl.h".)
 * The optimal size here depends on the typical size of records.
 */
#define MBEDTLS_SSL_MAX_CONTENT_LEN             16384

#define MBEDTLS_USE_HW_ENGINE

/* HW CRYPTO and RSA Support*/
#ifdef MBEDTLS_USE_HW_ENGINE
#define MBEDTLS_HW_SHA256
#define MBEDTLS_HW_SHA1
#define MBEDTLS_HW_RNG			/* This required SE_Init(). Please make sure invoking SE_Init() function before enabling this macro */
#define MBEDTLS_HW_AES

#endif

#include "check_config.h"
//#define MBEDTLS_SELF_TEST
#endif							/* MBEDTLS_CONFIG_H */
