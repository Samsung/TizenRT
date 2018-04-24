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

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define TLS_MALLOC(buf, size)			\
	if ((buf = malloc(size)) == NULL) {	\
		return TLS_ALLOC_FAIL;			\
	}

#define TLS_FREE(buf)				\
	if (buf) {						\
		free(buf);					\
		buf = NULL;					\
	}

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

	TLS_MALLOC(ctx->conf, sizeof(mbedtls_ssl_config));
	TLS_MALLOC(ctx->crt, sizeof(mbedtls_x509_crt));
	TLS_MALLOC(ctx->pkey, sizeof(mbedtls_pk_context));
	TLS_MALLOC(ctx->entropy, sizeof(mbedtls_entropy_context));
	TLS_MALLOC(ctx->ctr_drbg, sizeof(mbedtls_ctr_drbg_context));
	TLS_MALLOC(ctx->timer, sizeof(mbedtls_timing_delay_context));
#ifdef MBEDTLS_SSL_CACHE_C
	TLS_MALLOC(ctx->cache, sizeof(mbedtls_ssl_cache_context));
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

	if (cred == NULL) {
		return TLS_INVALID_CRED;
	}

	if (cred->psk && cred->psk_len) {
		ret = mbedtls_ssl_conf_psk(ctx->conf, cred->psk, cred->psk_len, (const unsigned char *)cred->psk_identity, strlen(cred->psk_identity));

		if (ret) {
			return TLS_INVALID_PSK;
		}
	}

	/* Mandatory */
	if (cred->ca_cert) {
		ret = mbedtls_x509_crt_parse(ctx->crt, cred->ca_cert, cred->ca_certlen);
		if (ret) {
			return TLS_INVALID_CACERT;
		}
		mbedtls_ssl_conf_ca_chain(ctx->conf, ctx->crt, NULL);
	}

	/* Optional */
	if (cred->dev_cert && cred->dev_key) {
		ret = mbedtls_x509_crt_parse(ctx->crt, cred->dev_cert, cred->dev_certlen);
		if (ret) {
			return TLS_INVALID_DEVCERT;
		}
		ret = mbedtls_pk_parse_key(ctx->pkey, cred->dev_key, cred->dev_keylen, NULL, 0);
		if (ret) {
			return TLS_INVALID_DEVKEY;
		}
		ret = mbedtls_ssl_conf_own_cert(ctx->conf, ctx->crt->next, ctx->pkey);
		if (ret) {
			return TLS_INVALID_DEVCERT;
		}
	}

	return ret;
}

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
	mbedtls_ssl_conf_rng(ctx->conf, mbedtls_ctr_drbg_random, ctx->ctr_drbg);
	mbedtls_ssl_conf_dbg(ctx->conf, easy_tls_debug, stdout);

#if defined(MBEDTLS_SSL_CACHE_C)
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

	if (opt->transport == MBEDTLS_SSL_TRANSPORT_DATAGRAM) {
		mbedtls_ssl_set_timer_cb(session->ssl, ctx->timer, mbedtls_timing_set_delay, mbedtls_timing_get_delay);
		if (opt->server == MBEDTLS_SSL_IS_SERVER) {
			ctx->cookie = malloc(sizeof(mbedtls_ssl_cookie_ctx));
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

	if ((ret = mbedtls_ssl_setup(session->ssl, ctx->conf) != 0)) {
		ret = TLS_SET_DEFAULT_FAIL;
		goto errout;
	}

	return TLS_SUCCESS;

errout:
	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

tls_ctx *TLSCtx(tls_cred *cred)
{
	int ret = 0;
	tls_ctx *ctx = NULL;

	ctx = malloc(sizeof(tls_ctx));
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
	mbedtls_net_context listen_ctx;
	tls_session *session = NULL;

	if (fd < 0 || ctx == NULL || opt == NULL) {
		EASY_TLS_DEBUG("TLSSession input error\n");
		return NULL;
	}

	session = (tls_session *)malloc(sizeof(tls_session));
	if (session == NULL) {
		EASY_TLS_DEBUG("tls session alloc fail\n");
		return NULL;
	}

	session->ssl = malloc(sizeof(mbedtls_ssl_context));
	if (session->ssl == NULL) {
		EASY_TLS_DEBUG("tls ssl alloc fail\n");
		TLS_FREE(session);
		return NULL;
	}

	if ((ret = tls_set_default(session, ctx, opt)) != TLS_SUCCESS) {
		EASY_TLS_DEBUG("tls_set_default fail %d\n", ret);
		goto errout;
	}

	if (opt->transport == MBEDTLS_SSL_TRANSPORT_DATAGRAM) {
		mbedtls_ssl_set_bio(session->ssl, &session->net, mbedtls_net_send, mbedtls_net_recv, mbedtls_net_recv_timeout);
	} else {
		mbedtls_ssl_set_bio(session->ssl, &session->net, mbedtls_net_send, mbedtls_net_recv, NULL);
	}

	listen_ctx.fd = fd;
	session->net.fd = fd;
reset:
	if (opt->server == MBEDTLS_SSL_IS_SERVER) {
		mbedtls_ssl_session_reset(session->ssl);
		if ((ret = mbedtls_net_accept(&listen_ctx, &session->net, ip, sizeof(ip), &ip_len)) != 0) {
			EASY_TLS_DEBUG("mbedtls_net_accept failed %d\n", ret);
			goto errout;
		}

		if ((ret = mbedtls_net_set_block(&session->net)) != 0) {
			EASY_TLS_DEBUG("mbedtls_net_set_block failed %d\n", ret);
			goto errout;
		}

		if (opt->transport == MBEDTLS_SSL_TRANSPORT_DATAGRAM) {
			if ((ret = mbedtls_ssl_set_client_transport_id(session->ssl, ip, ip_len)) != 0) {
				EASY_TLS_DEBUG("mbedtls_ssl_set_client_transport_id failed -0x%x\n", -ret);
				goto errout;
			}
		}
	}

	EASY_TLS_DEBUG("Handshake start .... ");

	while ((ret = mbedtls_ssl_handshake(session->ssl)) != 0) {
		if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
			if (ret == MBEDTLS_ERR_SSL_HELLO_VERIFY_REQUIRED) {
				EASY_TLS_DEBUG("Hello verification requested\n");
				mbedtls_net_free(&session->net);
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

	TLS_FREE(session->ssl);
	TLS_FREE(session);

	EASY_TLS_DEBUG("TLSSession free\n");
	return 0;
}

int TLSSend(tls_session *session, unsigned char *buf, size_t size)
{
	return mbedtls_ssl_write(session->ssl, buf, size);
}

int TLSRecv(tls_session *session, unsigned char *buf, size_t size)
{
	return mbedtls_ssl_read(session->ssl, buf, size);
}
