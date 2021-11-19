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

#ifndef __EASY_TLS_H
#define __EASY_TLS_H

#include <debug.h>

#include "mbedtls/config.h"
#include "mbedtls/ssl.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/x509_crt.h"
#include "mbedtls/timing.h"
#include "mbedtls/ssl_cookie.h"

#ifdef MBEDTLS_SSL_CACHE_C
#include "mbedtls/ssl_cache.h"
#endif

#include <sys/socket.h>
#include <sys/types.h>

#define EASY_TLS_DEBUG	printf

enum easy_tls_error {
	TLS_SUCCESS,
	TLS_ALLOC_FAIL,
	TLS_INVALID_INPUT_PARAM,
	TLS_CONTEXT_INIT_FAIL,
	TLS_PARSE_CRED_FAIL,
	TLS_INVALID_CRED,
	TLS_SET_DEFAULT_FAIL,
	TLS_SET_HOSTNAME_FAIL,
	TLS_SET_COOKIE_FAIL,
	TLS_INVALID_CACERT,
	TLS_INVALID_DEVCERT,
	TLS_INVALID_DEVKEY,
	TLS_INVALID_PSK,
};

typedef struct tls_cert_and_key {
	const unsigned char *ca_cert;
	const unsigned char *dev_cert;
	const unsigned char *dev_key;
	unsigned int ca_certlen;
	unsigned int dev_certlen;
	unsigned int dev_keylen;
	bool use_se;
#ifdef MBEDTLS_KEY_EXCHANGE__SOME__PSK_ENABLED
	unsigned char *psk;
	char *psk_identity;
	size_t psk_len;
#endif
} tls_cred;

typedef struct tls_context {
	mbedtls_ssl_config *conf;
	mbedtls_x509_crt *crt;
	mbedtls_pk_context *pkey;
	mbedtls_entropy_context *entropy;
	mbedtls_ctr_drbg_context *ctr_drbg;
	mbedtls_timing_delay_context *timer;
	mbedtls_ssl_cookie_ctx *cookie;
#ifdef MBEDTLS_SSL_CACHE_C
	mbedtls_ssl_cache_context *cache;
#endif
	bool use_se;
} tls_ctx;

typedef struct tls_options {
	int server;					///< MBEDTLS_SSL_IS_CLIENT(0) or MBEDTLS_SSL_IS_SERVER(1)
	int transport;				///< MBEDTLS_SSL_TRANSPORT_STREAM(0) or MBEDTLS_SSL_TRANSPORT_DATAGRAM(1)
	int auth_mode;				///< select authentication level (0 ~ 2)
	int debug_mode;				///< select debug level (0 ~ 5)
	char *host_name;			///< set host_name (NULL or char *)
	int force_ciphersuites[3];	///< set force ciphersuites
	int force_curves[3];		///< set force curves
	unsigned int recv_timeout;	///< parameter for setting tls recv timeout milisecond
	unsigned int client_rpk:1;
	unsigned int server_rpk:1;
	int hs_timeout_min;
	int hs_timeout_max;
	int (*psk_callback)(void *, mbedtls_ssl_context *, const unsigned char *, size_t);
	void *user_data;
} tls_opt;

typedef struct bio_context {
	int fd;
	struct sockaddr *addr;
	socklen_t n;
} bio_context;

typedef struct tls_session_context {
	mbedtls_net_context net;
	bio_context b_ctx;
	mbedtls_ssl_context *ssl;
} tls_session;

/**
 * @brief TLSCtx() allocates the basic tls context pointer (tls_ctx *) and
 *                 parsing a key and certificates. Please get this context before
 *		   creating or connecting(accepting) socket.
 *
 * @param[in] cred     a structure pointer of key and certificates
 * @return On success, a structure pointer of tls context will be returned.
 *         On failure, NULL will be returned.
 *
 */
tls_ctx *TLSCtx(tls_cred *cred);

/**
 * @brief TLSCtx_free() is released all allocated memory in tls_ctx*. This function
 *			should be paired up with TLSCtx().
 *
 * @param[in] ctx	a structure pointer of tls context.
 * @return On success,	TLS_SUCCESS(0) will be returned.
 *         On failure,	positive value will be returned.
 *
 */
int TLSCtx_free(tls_ctx *ctx);

/**
 * @brief TLSSession()	allocates the secure session context and makes it. This function
 *			should be called after connect(accept) socket because it will do
 *			tls handshake with input file descriptor.
 *
 * @param[in] fd	a file descriptor(socket) to make secure session.
 * @param[in] ctx	initialized structure pointer for TLSCtx();
 * @param[in] opt	a structure pointer including several tls options.
 * @return On success,	a structure pointer of tls session context will be returned.
 *         On failure,	positive value will be returned.
 *
 */
tls_session *TLSSession(int fd, tls_ctx *ctx, tls_opt *opt);

/**
 * @brief TLSSession_free()	is released all allocated memory in tls_session*. This function
 *				should be paired up with TLSSession().
 *
 * @param[in] session	a structure pointer of tls session context.
 * @return On success,	TLS_SUCCESS(0) will be returned.
 *         On failure,	positive value will be returned.
 *
 */
int TLSSession_free(tls_session *session);

/**
 * @brief TLSSend()	send security data.
 *			This function should be called after TLSCtx and TLSSession.
 *
 * @param[in] session	a structure pointer of tls session context.
 * @param[in] buf	send buffer address.
 * @param[in] size	send data size.
 * @return On success,	sent size will be returned.
 *         On failure,	0 or negative value will be returned.
 *
 */
int TLSSend(tls_session *session, const unsigned char *buf, size_t size);

/**
 * @brief TLSRecv()	recv security data.
 *			This function should be called after TLSCtx and TLSSession.
 *
 * @param[in] session	a structure pointer of tls session context.
 * @param[in] buf	buffer address.
 * @param[in] size	received data size.
 * @return On success,	received size will be returned.
 *         On failure,	0 or negative value will be returned.
 *
 */
int TLSRecv(tls_session *session, unsigned char *buf, size_t size);

#endif							/* __EASY_TLS_H */
