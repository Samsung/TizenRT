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

#include <tinyara/config.h>
#include <stdio.h>
#include <string.h>
#include <mbedtls/easy_tls.h>
#include <mbedtls/debug.h>

#include <sys/socket.h>
#include <sys/types.h>

#if defined(CONFIG_TLS_WITH_SSS)
#include <mbedtls/see_cert.h>
#include <mbedtls/see_api.h>
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define TLS_MALLOC(type, buf, size)			\
	if ((buf = (type *)malloc(size)) == NULL) {	\
		return TLS_ALLOC_FAIL;			\
	}

#define TLS_FREE(buf)				\
	if (buf) {						\
		free(buf);					\
		buf = NULL;					\
	}

#define PEM_END_CERTIFICATE	"-----END CERTIFICATE-----\r\n"

/****************************************************************************
 * Static Functions
 ****************************************************************************/

static void easy_tls_debug(void *ctx, int level, const char *file, int line, const char *str)
{
	EASY_TLS_DEBUG("%04d: |%d| %s", line, level, str);
}

static int tls_context_init(tls_ctx *ctx)
{
	if (ctx == NULL) {
		return TLS_CONTEXT_INIT_FAIL;
	}

	mbedtls_ssl_config_init(ctx->conf);
	mbedtls_x509_crt_init(ctx->crt);
	mbedtls_pk_init(ctx->pkey);
	mbedtls_entropy_init(ctx->entropy);
	mbedtls_ctr_drbg_init(ctx->ctr_drbg);
#ifdef MBEDTLS_SSL_CACHE_C
	mbedtls_ssl_cache_init(ctx->cache);
#endif
	return 0;
}

static int tls_context_alloc(tls_ctx *ctx)
{
	memset(ctx, 0, sizeof(tls_ctx));

	TLS_MALLOC(mbedtls_ssl_config, ctx->conf, sizeof(mbedtls_ssl_config));
	TLS_MALLOC(mbedtls_x509_crt, ctx->crt, sizeof(mbedtls_x509_crt));
	TLS_MALLOC(mbedtls_pk_context, ctx->pkey, sizeof(mbedtls_pk_context));
	TLS_MALLOC(mbedtls_entropy_context, ctx->entropy, sizeof(mbedtls_entropy_context));
	TLS_MALLOC(mbedtls_ctr_drbg_context, ctx->ctr_drbg, sizeof(mbedtls_ctr_drbg_context));
	TLS_MALLOC(mbedtls_timing_delay_context, ctx->timer, sizeof(mbedtls_timing_delay_context));
#ifdef MBEDTLS_SSL_CACHE_C
	TLS_MALLOC(mbedtls_ssl_cache_context, ctx->cache, sizeof(mbedtls_ssl_cache_context));
#endif
	return 0;
}

static void tls_context_free(tls_ctx *ctx)
{
	if (ctx) {
		TLS_FREE(ctx->conf);
		TLS_FREE(ctx->crt);
		TLS_FREE(ctx->pkey);
		TLS_FREE(ctx->entropy);
		TLS_FREE(ctx->ctr_drbg);
		TLS_FREE(ctx->timer);
#ifdef MBEDTLS_SSL_CACHE_C
		TLS_FREE(ctx->cache);
#endif
		if (ctx->cookie) {
			TLS_FREE(ctx->cookie);
		}
	}
}

static void tls_context_release(tls_ctx *ctx)
{
	if (ctx) {
		mbedtls_ssl_config_free(ctx->conf);
		mbedtls_x509_crt_free(ctx->crt);
		mbedtls_pk_free(ctx->pkey);
		mbedtls_entropy_free(ctx->entropy);
		mbedtls_ctr_drbg_free(ctx->ctr_drbg);
#ifdef MBEDTLS_SSL_CACHE_C
		mbedtls_ssl_cache_free(ctx->cache);
#endif
		if (ctx->cookie) {
			mbedtls_ssl_cookie_free(ctx->cookie);
		}
	}
}

static int tls_entropy_init(tls_ctx *ctx)
{
	return mbedtls_ctr_drbg_seed(ctx->ctr_drbg, mbedtls_entropy_func, ctx->entropy, NULL, 0);
}

static int tls_set_cred(tls_ctx *ctx, tls_cred *cred)
{
	int ret = TLS_PARSE_CRED_FAIL;
#if defined(CONFIG_TLS_WITH_SSS)
	const mbedtls_pk_info_t *pk_info = NULL;
#endif
	char *start = NULL, *end = NULL, *copy = NULL;
	int remain = 0;

	if (cred == NULL) {
		return TLS_INVALID_CRED;
	}

#if defined(MBEDTLS_KEY_EXCHANGE__SOME__PSK_ENABLED)
	if (cred->psk && cred->psk_len) {
		ret = mbedtls_ssl_conf_psk(ctx->conf, cred->psk, cred->psk_len, (const unsigned char *)cred->psk_identity, strlen(cred->psk_identity));

		if (ret) {
			return TLS_INVALID_PSK;
		}
	}
#endif

	/* Mandatory */
	if (cred->ca_cert) {
		/* CA certs may come as a bundle, parse them all */
		start = (char *)cred->ca_cert;
		end = start;
		remain = cred->ca_certlen - 1;
		do {
			end = strstr(start, PEM_END_CERTIFICATE);
			if (!end)
				break;

			end += strlen(PEM_END_CERTIFICATE);
			copy = strndup(start, end - start);
			if (!copy)
				return TLS_ALLOC_FAIL;

			if (mbedtls_x509_crt_parse(ctx->crt, (const unsigned char *)copy,
						end - start + 1) != 0) {
				free(copy);
				return TLS_INVALID_CACERT;
			}
			remain -= end - start;
			start = end;
			free(copy);
		} while (remain);

		mbedtls_ssl_conf_ca_chain(ctx->conf, ctx->crt, NULL);
	}

	/* Optional */
	if (cred->dev_cert) {
		ret = mbedtls_x509_crt_parse(ctx->crt, cred->dev_cert, cred->dev_certlen);
		if (ret) {
			return TLS_INVALID_DEVCERT;
		}

		if (cred->dev_key) {
			ret = mbedtls_pk_parse_key(ctx->pkey, cred->dev_key, cred->dev_keylen, NULL, 0);
			if (ret) {
				return TLS_INVALID_DEVKEY;
			}
		}
#if defined(CONFIG_TLS_WITH_SSS)
		else if (cred->use_se) {
			pk_info = mbedtls_pk_info_from_type(MBEDTLS_PK_ECKEY);
			if (!pk_info) {
				return TLS_INVALID_DEVKEY;
			}

			ret = mbedtls_pk_setup(ctx->pkey, pk_info);
			if (ret) {
				return TLS_INVALID_DEVKEY;
			}

			((mbedtls_ecdsa_context *)(ctx->pkey->pk_ctx))->grp.id =
						MBEDTLS_ECP_DP_SECP256R1;
			((mbedtls_ecdsa_context *)(ctx->pkey->pk_ctx))->key_index =
						FACTORYKEY_ARTIK_DEVICE;
			ctx->use_se = true;
		}
#endif
		else {
			return TLS_INVALID_DEVKEY;
		}

		ret = mbedtls_ssl_conf_own_cert(ctx->conf, ctx->crt->next ? ctx->crt->next : ctx->crt, ctx->pkey);
		if (ret) {
			return TLS_INVALID_DEVCERT;
		}

	}

	return TLS_SUCCESS;
}

#if defined(CONFIG_TLS_WITH_SSS)
static int see_generate_random_client(void *ctx, unsigned char *data, size_t len)
{
	size_t offset = 0;
	int ret = 0;
	unsigned int rand[SEE_MAX_RANDOM_SIZE];

	if (!data || !len)
		return -1;

	while (len != 0) {
		size_t rand_size = len >= SEE_MAX_RANDOM_SIZE ? SEE_MAX_RANDOM_SIZE : len;

		ret = see_generate_random(rand, rand_size);
		if (ret != SEE_OK) {
			return -1;
		}

		memcpy(data + offset, rand, rand_size);

		len -= rand_size;
		offset += rand_size;
	}

	return 0;
}
#endif

static int tls_set_default(tls_session *session, tls_ctx *ctx, tls_opt *opt)
{
	int ret;

	mbedtls_ssl_init(session->ssl);

	if (opt->server != MBEDTLS_SSL_IS_SERVER && opt->server != MBEDTLS_SSL_IS_CLIENT) {
		ret = TLS_INVALID_INPUT_PARAM;
		goto errout;
	}

	if (opt->transport != MBEDTLS_SSL_TRANSPORT_STREAM && opt->transport != MBEDTLS_SSL_TRANSPORT_DATAGRAM) {
		ret = TLS_INVALID_INPUT_PARAM;
		goto errout;
	}

	ret = mbedtls_ssl_config_defaults(ctx->conf, opt->server, opt->transport, MBEDTLS_SSL_PRESET_DEFAULT);
	if (ret) {
		ret = TLS_SET_DEFAULT_FAIL;
		goto errout;
	}
#if defined(MBEDTLS_DEBUG_C)
	if (opt->debug_mode) {
		mbedtls_debug_set_threshold(opt->debug_mode);
	}
#endif

#if defined(CONFIG_TLS_WITH_SSS)
	if (ctx->use_se) {
		mbedtls_ssl_conf_rng(ctx->conf, see_generate_random_client, ctx->ctr_drbg);
	} else
#endif
	{
		mbedtls_ssl_conf_rng(ctx->conf, mbedtls_ctr_drbg_random, ctx->ctr_drbg);
	}

	mbedtls_ssl_conf_dbg(ctx->conf, easy_tls_debug, stdout);

#if defined(MBEDTLS_SSL_CACHE_C)
	if (opt->server == MBEDTLS_SSL_IS_SERVER)
		mbedtls_ssl_conf_session_cache(ctx->conf, ctx->cache, mbedtls_ssl_cache_get, mbedtls_ssl_cache_set);
#endif

	if (opt->auth_mode <= MBEDTLS_SSL_VERIFY_UNSET) {
		mbedtls_ssl_conf_authmode(ctx->conf, opt->auth_mode);
	}

	if (opt->host_name) {
		ret = mbedtls_ssl_set_hostname(session->ssl, opt->host_name);
		if (ret) {
			ret = TLS_SET_HOSTNAME_FAIL;
			goto errout;
		}
	}

	if (opt->recv_timeout) {
		mbedtls_ssl_conf_read_timeout(ctx->conf, opt->recv_timeout);
	}

#if defined(MBEDTLS_SSL_PROTO_DTLS)
	if (opt->hs_timeout_min >= 0 && opt->hs_timeout_max >= 0)
		mbedtls_ssl_conf_handshake_timeout(ctx->conf, opt->hs_timeout_min, opt->hs_timeout_max);
#endif

	if (opt->transport == MBEDTLS_SSL_TRANSPORT_DATAGRAM) {
		mbedtls_ssl_set_timer_cb(session->ssl, ctx->timer, mbedtls_timing_set_delay, mbedtls_timing_get_delay);
		if (opt->server == MBEDTLS_SSL_IS_SERVER) {

			if (ctx->cookie) {
				mbedtls_ssl_cookie_free(ctx->cookie);
			}

			TLS_FREE(ctx->cookie);

			ctx->cookie = (mbedtls_ssl_cookie_ctx *)malloc(sizeof(mbedtls_ssl_cookie_ctx));
			if (ctx->cookie == NULL) {
				ret = TLS_ALLOC_FAIL;
				goto errout;
			}
			mbedtls_ssl_cookie_init(ctx->cookie);
			if ((mbedtls_ssl_cookie_setup(ctx->cookie, mbedtls_ctr_drbg_random, ctx->ctr_drbg)) != 0) {
				ret = TLS_SET_COOKIE_FAIL;
				goto errout;
			}
			mbedtls_ssl_conf_dtls_cookies(ctx->conf, mbedtls_ssl_cookie_write, mbedtls_ssl_cookie_check, ctx->cookie);
		}
	}

	if (opt->force_ciphersuites[0] > 0) {
		mbedtls_ssl_conf_ciphersuites(ctx->conf, opt->force_ciphersuites);
	}

	if (opt->force_curves[0] > 0) {
		mbedtls_ssl_conf_curves(ctx->conf, (mbedtls_ecp_group_id *)opt->force_curves);
	}

#if defined(MBEDTLS_SSL_CLIENT_RPK)
	if (opt->client_rpk) {
		mbedtls_ssl_conf_client_rpk_support(ctx->conf, 1);
	}
#endif

#if defined(MBEDTLS_SSL_SERVER_RPK)
	if (opt->server_rpk) {
		mbedtls_ssl_conf_server_rpk_support(ctx->conf, 1);
	}
#endif

#if defined(MBEDTLS_KEY_EXCHANGE__SOME__PSK_ENABLED)
	if (opt->psk_callback) {
		mbedtls_ssl_conf_psk_cb(ctx->conf, opt->psk_callback, opt->user_data);
	}
#endif

	ret = mbedtls_ssl_setup(session->ssl, ctx->conf);

	if (ret != 0) {
		ret = TLS_SET_DEFAULT_FAIL;
		goto errout;
	}

	return TLS_SUCCESS;

errout:
	return ret;
}

int easy_tls_net_sendto(void *ctx, const unsigned char *buf, size_t len)
{
	int ret;
	bio_context *b_ctx = (bio_context *)ctx;
#if defined(MBEDTLS_OCF_PATCH)
	mbedtls_socket fd = b_ctx->fd;
#else
	int fd = b_ctx->fd;
#endif
	struct sockaddr *dest_addr = b_ctx->addr;

	socklen_t n = b_ctx->n;

	if ( fd < 0 )
		return ( MBEDTLS_ERR_NET_INVALID_CONTEXT) ;
	ret = (int) sendto( fd, buf, len, 0, (struct sockaddr *)dest_addr, n );

	if ( ret < 0 )
	{

		if ( errno == EPIPE || errno == ECONNRESET )
			return ( MBEDTLS_ERR_NET_CONN_RESET );

		if ( errno == EINTR )
			return ( MBEDTLS_ERR_SSL_WANT_WRITE );

		return ( MBEDTLS_ERR_NET_SEND_FAILED );

	}


	return( ret );
}

int easy_tls_net_recv(void *ctx, unsigned char *buf, size_t len)
{
	int ret;
	bio_context *b_ctx = (bio_context *)ctx;
#if defined(MBEDTLS_OCF_PATCH)
	mbedtls_socket fd = b_ctx->fd;
#else
	int fd = b_ctx->fd;
#endif

	if( fd < 0 )
		return( MBEDTLS_ERR_NET_INVALID_CONTEXT );

	ret = (int) read( fd, (void *)buf, len );

	if( ret < 0 )
	{
		if( errno == EPIPE || errno == ECONNRESET )
			return( MBEDTLS_ERR_NET_CONN_RESET );

		if( errno == EINTR )
			return( MBEDTLS_ERR_SSL_WANT_READ );

		return( MBEDTLS_ERR_NET_RECV_FAILED );
	}

	return( ret );
}

int easy_tls_net_recv_timeout(void *ctx, unsigned char *buf, size_t len,
		uint32_t timeout)
{
	int ret;
	struct timeval tv;
	fd_set read_fds;
	bio_context *b_ctx = (bio_context *)ctx;
#if defined(MBEDTLS_OCF_PATCH)
	mbedtls_socket fd = b_ctx->fd;
#else
	int fd = b_ctx->fd;
#endif

	if( fd < 0 )
		return( MBEDTLS_ERR_NET_INVALID_CONTEXT );

	FD_ZERO( &read_fds );
	FD_SET( fd, &read_fds );

	tv.tv_sec  = timeout / 1000;
	tv.tv_usec = ( timeout % 1000 ) * 1000;

#if defined(MBEDTLS_OCF_PATCH)
	ret = select( (int)fd + 1, &read_fds, NULL, NULL, timeout == 0 ? NULL : &tv );
#else
	ret = select( fd + 1, &read_fds, NULL, NULL, timeout == 0 ? NULL : &tv );
#endif

	/* Zero fds ready means we timed out */
	if( ret == 0 )
		return( MBEDTLS_ERR_SSL_TIMEOUT );

	if( ret < 0 )
	{
		if( errno == EINTR )
			return( MBEDTLS_ERR_SSL_WANT_READ );
		else
			return( MBEDTLS_ERR_NET_RECV_FAILED );
	}

	/* This call will not block */
	return( easy_tls_net_recv( ctx, buf, len ) );
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

tls_ctx *TLSCtx(tls_cred *cred)
{
	int ret = 0;
	tls_ctx *ctx = NULL;

	ctx = (tls_ctx *)malloc(sizeof(tls_ctx));
	if (ctx == NULL) {
		EASY_TLS_DEBUG("Input Context is Null\n");
		return NULL;
	}

	if ((ret = tls_context_alloc(ctx)) != TLS_SUCCESS) {
		EASY_TLS_DEBUG("tls_context_alloc fail %d\n", ret);
		tls_context_free(ctx);
		TLS_FREE(ctx);
		return NULL;
	}

	if ((ret = tls_context_init(ctx)) != TLS_SUCCESS) {
		EASY_TLS_DEBUG("tls_context_init fail %d\n", ret);
		goto errout;
	}

	if ((ret = tls_entropy_init(ctx)) != TLS_SUCCESS) {
		EASY_TLS_DEBUG("tls_entropy_init fail %d\n", ret);
		goto errout;
	}

	if ((ret = tls_set_cred(ctx, cred)) != TLS_SUCCESS) {
		EASY_TLS_DEBUG("tls_set_cred fail %d\n", ret);
		goto errout;
	}

	return ctx;
errout:
	TLSCtx_free(ctx);
	TLS_FREE(ctx);

	return NULL;
}

int TLSCtx_free(tls_ctx *ctx)
{
	if (ctx == NULL) {
		return TLS_INVALID_INPUT_PARAM;
	}

	tls_context_release(ctx);
	tls_context_free(ctx);

	TLS_FREE(ctx);

	EASY_TLS_DEBUG("TLSCtx free\n");
	return TLS_SUCCESS;
}

tls_session *TLSSession(int fd, tls_ctx *ctx, tls_opt *opt)
{
	int ret;
	unsigned char ip[16];
	unsigned int ip_len;
	struct sockaddr_storage client_addr;
	socklen_t n = (socklen_t) sizeof( client_addr );
	char buf[1] = { 0 };
	tls_session *session = NULL;
	int type;
	socklen_t type_len = (int)sizeof(type);

	if (fd < 0 || ctx == NULL || opt == NULL) {
		EASY_TLS_DEBUG("TLSSession input error\n");
		return NULL;
	}

	session = (tls_session *)malloc(sizeof(tls_session));
	if (session == NULL) {
		EASY_TLS_DEBUG("tls session alloc fail\n");
		return NULL;
	}

	session->ssl = (mbedtls_ssl_context *)malloc(sizeof(mbedtls_ssl_context));
	if (session->ssl == NULL) {
		EASY_TLS_DEBUG("tls ssl alloc fail\n");
		TLS_FREE(session);
		return NULL;
	}

	if ((ret = tls_set_default(session, ctx, opt)) != TLS_SUCCESS) {
		EASY_TLS_DEBUG("tls_set_default fail 0x%x\n", ret);
		goto errout;
	}

	memset(&session->b_ctx, 0, sizeof(bio_context));

	ret = mbedtls_ssl_handle_message_type(session->ssl);

	if (opt->server == MBEDTLS_SSL_IS_CLIENT)
		session->net.fd = fd;
	else
		session->net.fd = -1;

	struct timeval timeout;

	timeout.tv_sec = 0;
	timeout.tv_usec = opt->recv_timeout > 0 ? opt->recv_timeout*1000 : 0;

reset:

	if (getsockopt(fd, SOL_SOCKET, SO_TYPE, (void *)&type, &type_len) != 0 ||
					(type != SOCK_STREAM && type != SOCK_DGRAM)) {
		EASY_TLS_DEBUG("error getsockopt : %s\n", strerror(errno));
		goto errout;
	}

	if (opt->server == MBEDTLS_SSL_IS_SERVER) {
		mbedtls_ssl_session_reset(session->ssl);

		if (type == SOCK_STREAM) {
			ret = fd = (int)accept(fd, (struct sockaddr *)&client_addr, &n);
			if (ret < 0) {
				EASY_TLS_DEBUG("accept failed: %s\n", strerror(errno));
				goto errout;
			}
		} else {

			if ((ret = recvfrom(fd, buf, sizeof(buf),
				MSG_PEEK, (struct sockaddr *)&client_addr, &n)) < 0) {
				EASY_TLS_DEBUG("recvfrom failed: %s\n", strerror(errno));
				goto errout;
			}
		}

		if( client_addr.ss_family == AF_INET )
		{
			struct sockaddr_in *addr4 = (struct sockaddr_in *) &client_addr;
			ip_len = sizeof( addr4->sin_addr.s_addr );

			memcpy(ip, &addr4->sin_addr.s_addr, ip_len);
		}
		else
		{
			struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *) &client_addr;
			ip_len = sizeof( addr6->sin6_addr.s6_addr );

			memcpy(ip, &addr6->sin6_addr.s6_addr, ip_len);
		}

		if (opt->transport == MBEDTLS_SSL_TRANSPORT_DATAGRAM) {
			if ((ret = mbedtls_ssl_set_client_transport_id(session->ssl, ip, ip_len)) != 0) {
				EASY_TLS_DEBUG("mbedtls_ssl_set_client_transport_id failed -0x%x\n", -ret);
				goto errout;
			}
		}

		if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
			EASY_TLS_DEBUG("error setsockopt : %s\n", strerror(errno));
		}

		if (type == SOCK_STREAM) {
			session->net.fd = fd;
		} else {
			session->b_ctx.fd = fd;
		}

		TLS_FREE(session->b_ctx.addr);

		session->b_ctx.addr = (struct sockaddr *)malloc(n);

		if (session->b_ctx.addr) {
			memcpy(session->b_ctx.addr, (struct sockaddr *)&client_addr, n);
		} else {
			EASY_TLS_DEBUG("Error memory\n");
			goto errout;
		}
		session->b_ctx.n = n;
	} else {
		if (session->net.fd < 0) {
			EASY_TLS_DEBUG("wrong value of fd");
			goto errout;
		}

		if (setsockopt(session->net.fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
			EASY_TLS_DEBUG("error setsockopt : %s\n", strerror(errno));
		}
	}

	if (opt->transport == MBEDTLS_SSL_TRANSPORT_DATAGRAM && opt->server == MBEDTLS_SSL_IS_SERVER) {
		mbedtls_ssl_set_bio(session->ssl, &session->b_ctx, easy_tls_net_sendto, easy_tls_net_recv, easy_tls_net_recv_timeout);
	} else if (opt->transport == MBEDTLS_SSL_TRANSPORT_DATAGRAM && opt->server == MBEDTLS_SSL_IS_CLIENT) {
		mbedtls_ssl_set_bio(session->ssl, &session->net, mbedtls_net_send, mbedtls_net_recv, mbedtls_net_recv_timeout);
	}
	else {
		mbedtls_ssl_set_bio(session->ssl, &session->net, mbedtls_net_send, mbedtls_net_recv, NULL);
	}

	EASY_TLS_DEBUG("Handshake start ....\n");

	while ((ret = mbedtls_ssl_handshake(session->ssl)) != 0) {
		if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
			if (ret == MBEDTLS_ERR_SSL_HELLO_VERIFY_REQUIRED) {
				EASY_TLS_DEBUG("Hello verification requested\n");
				goto reset;
			}
			if (ret == MBEDTLS_ERR_X509_CERT_VERIFY_FAILED) {
				EASY_TLS_DEBUG("Failed !! certificate verify fail %d\n", ret);
			}
			EASY_TLS_DEBUG("Failed !! %d\n", ret);
			goto errout;
		}

	}

	EASY_TLS_DEBUG("Success !!\n");
	return session;

errout:
	TLSSession_free(session);
	return NULL;
}

int TLSSession_free(tls_session *session)
{
	if (session == NULL) {
		return TLS_INVALID_INPUT_PARAM;
	}

	mbedtls_ssl_free(session->ssl);
	mbedtls_net_free(&session->net);

	TLS_FREE(session->b_ctx.addr);
	TLS_FREE(session->ssl);
	TLS_FREE(session);

	EASY_TLS_DEBUG("TLSSession free\n");
	return 0;
}

int TLSSend(tls_session *session, const unsigned char *buf, size_t size)
{
	return mbedtls_ssl_write(session->ssl, buf, size);
}

int TLSRecv(tls_session *session, unsigned char *buf, size_t size)
{
	return mbedtls_ssl_read(session->ssl, buf, size);
}
