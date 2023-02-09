#include "FreeRTOS.h"
#include "task.h"
#include "platform_stdlib.h"
#include "osdep_service.h"
#include <lwipconf.h>
#include "httpc.h"

int httpc_setsockopt_rcvtimeo(struct httpc_conn *conn, int recv_timeout)
{
	int ret = 0;
#if defined(LWIP_SO_SNDRCVTIMEO_NONSTANDARD) && (LWIP_SO_SNDRCVTIMEO_NONSTANDARD == 0)	//lwip 2.0.2
	struct timeval timeout;
	timeout.tv_sec  = recv_timeout / 1000;
	timeout.tv_usec = (recv_timeout % 1000) * 1000;
	ret = setsockopt(conn->sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
#else	//lwip 1.4.1
	ret = setsockopt(conn->sock, SOL_SOCKET, SO_RCVTIMEO, &recv_timeout, sizeof(recv_timeout));
#endif
	return ret;
}

#if (HTTPC_USE_TLS == HTTPC_TLS_POLARSSL)
#include "polarssl/ssl.h"
#include "polarssl/memory.h"
#include "polarssl/base64.h"

struct httpc_tls {
	ssl_context ctx;                 /*!< Context for PolarSSL */
	x509_crt ca;                     /*!< CA certificates */
	x509_crt cert;                   /*!< Certificate */
	pk_context key;                  /*!< Private key */
};

static int _verify_func(void *data, x509_crt *crt, int depth, int *flags)
{
	char buf[1024];
	x509_crt_info(buf, sizeof(buf) - 1, "", crt);

	if (*flags) {
		printf("\n[HTTPC] ERROR: certificate verify\n%s\n", buf);
	} else {
		printf("\n[HTTPC] Certificate verified\n%s\n", buf);
	}

	return 0;
}

#elif (HTTPC_USE_TLS == HTTPC_TLS_MBEDTLS)
#include "mbedtls/ssl.h"
#include "mbedtls/platform.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/base64.h"
#include "mbedtls/config.h"
#include "mbedtls/ssl.h"
#include "mbedtls/error.h"
#include "mbedtls/debug.h"
#include "mbedtls/version.h"

struct httpc_tls {
	mbedtls_ssl_context ctx;         /*!< Context for mbedTLS */
	mbedtls_ssl_config conf;         /*!< Configuration for mbedTLS */
	mbedtls_x509_crt ca;             /*!< CA certificates */
	mbedtls_x509_crt cert;           /*!< Certificate */

#if defined(configENABLE_TRUSTZONE) && (configENABLE_TRUSTZONE == 1) && defined(CONFIG_SSL_CLIENT_PRIVATE_IN_TZ) && (CONFIG_SSL_CLIENT_PRIVATE_IN_TZ == 1)
	mbedtls_pk_context *key;          /*!< Private key pointer */
#else
	mbedtls_pk_context key;          /*!< Private key */
#endif
};

static int _verify_func(void *data, mbedtls_x509_crt *crt, int depth, uint32_t *flags)
{
	/* To avoid gcc warnings */
	(void) data;
	(void) depth;

	char buf[1024];
	mbedtls_x509_crt_info(buf, sizeof(buf) - 1, "", crt);

	if (*flags) {
		printf("\n[HTTPC] ERROR: certificate verify\n%s\n", buf);
	} else {
		//printf("\n[HTTPC] Certificate verified\n%s\n", buf);
	}

	return 0;
}

static void *_calloc_func(size_t nmemb, size_t size)
{
	size_t mem_size;
	void *ptr = NULL;

	mem_size = nmemb * size;
	ptr = pvPortMalloc(mem_size);

	if (ptr) {
		memset(ptr, 0, mem_size);
	}

	return ptr;
}
#endif /* HTTPC_USE_POLARSSL */

static int _random_func(void *p_rng, unsigned char *output, size_t output_len)
{
	/* To avoid gcc warnings */
	(void) p_rng;

	rtw_get_random_bytes(output, output_len);
	return 0;
}

void *httpc_tls_new(int *sock, char *client_cert, char *client_key, char *ca_certs)
{
#if (HTTPC_USE_TLS == HTTPC_TLS_POLARSSL)
	int ret = 0;
	struct httpc_tls *tls = NULL;

	memory_set_own(pvPortMalloc, vPortFree);
	tls = (struct httpc_tls *) malloc(sizeof(struct httpc_tls));

	if (tls) {
		ssl_context *ssl = &tls->ctx;

		memset(tls, 0, sizeof(struct httpc_tls));
		x509_crt_init(&tls->ca);
		x509_crt_init(&tls->cert);
		pk_init(&tls->key);
		if ((ret = ssl_init(ssl)) != 0) {
			printf("\n[HTTPC] ERROR: ssl_init %d\n", ret);
			ret = -1;
			goto exit;
		}
		ssl_set_endpoint(ssl, SSL_IS_CLIENT);
		ssl_set_authmode(ssl, SSL_VERIFY_NONE);
		ssl_set_rng(ssl, _random_func, NULL);
		ssl_set_bio(ssl, net_recv, sock, net_send, sock);

		if (client_cert && client_key) {
			if ((ret = x509_crt_parse(&tls->cert, (const unsigned char *) client_cert, strlen(client_cert))) != 0) {
				printf("\n[HTTPC] ERROR: x509_crt_parse %d\n", ret);
				ret = -1;
				goto exit;
			}

			if ((ret = pk_parse_key(&tls->key, (const unsigned char *) client_key, strlen(client_key), NULL, 0)) != 0) {
				printf("\n[HTTPC] ERROR: pk_parse_key %d\n", ret);
				ret = -1;
				goto exit;
			}

			if ((ret = ssl_set_own_cert(ssl, &tls->cert, &tls->key)) != 0) {
				printf("\n[HTTPC] ERROR: ssl_set_own_cert %d\n", ret);
				ret = -1;
				goto exit;
			}
		}

		if (ca_certs) {
			// set trusted ca certificates next to client certificate
			if ((ret = x509_crt_parse(&tls->ca, (const unsigned char *) ca_certs, strlen(ca_certs))) != 0) {
				printf("\n[HTTPC] ERROR: x509_crt_parse %d\n", ret);
				ret = -1;
				goto exit;
			}

			ssl_set_ca_chain(ssl, &tls->ca, NULL, NULL);
			ssl_set_authmode(ssl, SSL_VERIFY_REQUIRED);
			ssl_set_verify(ssl, _verify_func, NULL);
		}
	} else {
		printf("\n[HTTPC] ERROR: malloc\n");
		ret = -1;
		goto exit;
	}

exit:
	if (ret && tls) {
		ssl_free(&tls->ctx);
		x509_crt_free(&tls->ca);
		x509_crt_free(&tls->cert);
		pk_free(&tls->key);
		free(tls);
		tls = NULL;
	}

	return (void *) tls;
#elif (HTTPC_USE_TLS == HTTPC_TLS_MBEDTLS)
	int ret = 0;
	struct httpc_tls *tls = NULL;

	mbedtls_platform_set_calloc_free(_calloc_func, vPortFree);
	tls = (struct httpc_tls *) malloc(sizeof(struct httpc_tls));

	if (tls) {
		mbedtls_ssl_context *ssl = &tls->ctx;
		mbedtls_ssl_config *conf = &tls->conf;

		memset(tls, 0, sizeof(struct httpc_tls));
		mbedtls_x509_crt_init(&tls->ca);
		mbedtls_x509_crt_init(&tls->cert);

#if !defined(configENABLE_TRUSTZONE) || (configENABLE_TRUSTZONE == 0) || !defined(CONFIG_SSL_CLIENT_PRIVATE_IN_TZ) || (CONFIG_SSL_CLIENT_PRIVATE_IN_TZ == 0)
		mbedtls_pk_init(&tls->key);
#endif
		mbedtls_ssl_init(ssl);
		mbedtls_ssl_config_init(conf);

		if ((ret = mbedtls_ssl_config_defaults(conf,
											   MBEDTLS_SSL_IS_CLIENT,
											   MBEDTLS_SSL_TRANSPORT_STREAM,
											   MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {

			printf("\n[HTTPC] ERROR: mbedtls_ssl_config_defaults %d\n", ret);
			ret = -1;
			goto exit;
		}

		mbedtls_ssl_conf_authmode(conf, MBEDTLS_SSL_VERIFY_NONE);
		mbedtls_ssl_conf_rng(conf, _random_func, NULL);

		if (client_cert && client_key) {
			if ((ret = mbedtls_x509_crt_parse(&tls->cert, (const unsigned char *) client_cert, strlen(client_cert) + 1)) != 0) {
				printf("\n[HTTPC] ERROR: mbedtls_x509_crt_parse %d\n", ret);
				ret = -1;
				goto exit;
			}

#if defined(configENABLE_TRUSTZONE) && (configENABLE_TRUSTZONE == 1) && defined(CONFIG_SSL_CLIENT_PRIVATE_IN_TZ) && (CONFIG_SSL_CLIENT_PRIVATE_IN_TZ == 1)
			extern mbedtls_pk_context *NS_ENTRY secure_mbedtls_pk_parse_key(void);
			tls->key = secure_mbedtls_pk_parse_key();

			if (tls->key == NULL) {
				ret = -1;
				goto exit;
			}

			if ((ret = mbedtls_ssl_conf_own_cert(conf, &tls->cert, tls->key)) != 0) {
				printf("\n[HTTPC] ERROR: mbedtls_ssl_conf_own_cert %d\n", ret);
				ret = -1;
				goto exit;
			}
#else
#if defined(MBEDTLS_VERSION_NUMBER) && (MBEDTLS_VERSION_NUMBER == 0x03000000)
			if ((ret = mbedtls_pk_parse_key(&tls->key, (const unsigned char *) client_key, strlen(client_key) + 1, NULL, 0, NULL, NULL)) != 0) {
#else
			if ((ret = mbedtls_pk_parse_key(&tls->key, (const unsigned char *) client_key, strlen(client_key) + 1, NULL, 0)) != 0) {
#endif
				printf("\n[HTTPC] ERROR: mbedtls_pk_parse_key %d\n", ret);
				ret = -1;
				goto exit;
			}

			if ((ret = mbedtls_ssl_conf_own_cert(conf, &tls->cert, &tls->key)) != 0) {
				printf("\n[HTTPC] ERROR: mbedtls_ssl_conf_own_cert %d\n", ret);
				ret = -1;
				goto exit;
			}
#endif
		}

		if (ca_certs) {
			// set trusted ca certificates next to client certificate
			if ((ret = mbedtls_x509_crt_parse(&tls->ca, (const unsigned char *) ca_certs, strlen(ca_certs) + 1)) != 0) {
				printf("\n[HTTPC] ERROR: mbedtls_x509_crt_parse %d\n", ret);
				ret = -1;
				goto exit;
			}

			mbedtls_ssl_conf_ca_chain(conf, &tls->ca, NULL);
			mbedtls_ssl_conf_authmode(conf, MBEDTLS_SSL_VERIFY_REQUIRED);
			mbedtls_ssl_conf_verify(conf, _verify_func, NULL);
		}

#if defined(MBEDTLS_SSL_MAX_CONTENT_LEN) && MBEDTLS_SSL_MAX_CONTENT_LEN == 4096
		if ((ret = mbedtls_ssl_conf_max_frag_len(conf, MBEDTLS_SSL_MAX_FRAG_LEN_4096)) < 0) {
			printf("\n[HTTPC] ERROR: mbedtls_ssl_conf_max_frag_len %d\n", ret);
			ret = -1;
			goto exit;
		}
#endif

		if ((ret = mbedtls_ssl_setup(ssl, conf)) != 0) {
			printf("\n[HTTPC] ERROR: mbedtls_ssl_setup %d\n", ret);
			ret = -1;
			goto exit;
		}

		mbedtls_ssl_set_bio(ssl, sock, mbedtls_net_send, mbedtls_net_recv, NULL);
	} else {
		printf("\n[HTTPC] ERROR: malloc\n");
		ret = -1;
		goto exit;
	}

exit:
	if (ret && tls) {
		mbedtls_ssl_free(&tls->ctx);
		mbedtls_ssl_config_free(&tls->conf);

		mbedtls_x509_crt_free(&tls->ca);
		mbedtls_x509_crt_free(&tls->cert);

#if defined(configENABLE_TRUSTZONE) && (configENABLE_TRUSTZONE == 1) && defined(CONFIG_SSL_CLIENT_PRIVATE_IN_TZ) && (CONFIG_SSL_CLIENT_PRIVATE_IN_TZ == 1)
		extern void NS_ENTRY secure_mbedtls_pk_free(mbedtls_pk_context * pk);
		secure_mbedtls_pk_free(tls->key);
#else
		mbedtls_pk_free(&tls->key);
#endif

		free(tls);
		tls = NULL;
	}

	return (void *) tls;
#endif
}

void httpc_tls_free(void *tls_in)
{
	struct httpc_tls *tls = (struct httpc_tls *) tls_in;

#if (HTTPC_USE_TLS == HTTPC_TLS_POLARSSL)
	ssl_free(&tls->ctx);
	x509_crt_free(&tls->ca);
	x509_crt_free(&tls->cert);
	pk_free(&tls->key);
	free(tls);
#elif (HTTPC_USE_TLS == HTTPC_TLS_MBEDTLS)
	mbedtls_ssl_free(&tls->ctx);
	mbedtls_ssl_config_free(&tls->conf);
	mbedtls_x509_crt_free(&tls->ca);
	mbedtls_x509_crt_free(&tls->cert);

#if defined(configENABLE_TRUSTZONE) && (configENABLE_TRUSTZONE == 1) && defined(CONFIG_SSL_CLIENT_PRIVATE_IN_TZ) && (CONFIG_SSL_CLIENT_PRIVATE_IN_TZ == 1)
	extern void NS_ENTRY secure_mbedtls_pk_free(mbedtls_pk_context * pk);
	secure_mbedtls_pk_free(tls->key);
#else
	mbedtls_pk_free(&tls->key);
#endif

	free(tls);
#endif
}

int httpc_tls_handshake(void *tls_in, char *host)
{
	struct httpc_tls *tls = (struct httpc_tls *) tls_in;

#if (HTTPC_USE_TLS == HTTPC_TLS_POLARSSL)
	int ret = 0;

	ssl_set_hostname(&tls->ctx, host);

	if ((ret = ssl_handshake(&tls->ctx)) != 0) {
		printf("\n[HTTPC] ERROR: ssl_handshake %d\n", ret);
		ret = -1;
	} else {
		printf("\n[HTTPC] Use ciphersuite %s\n", ssl_get_ciphersuite(&tls->ctx));
	}

	return ret;
#elif (HTTPC_USE_TLS == HTTPC_TLS_MBEDTLS)
	int ret = 0;

	mbedtls_ssl_set_hostname(&tls->ctx, host);

	if ((ret = mbedtls_ssl_handshake(&tls->ctx)) != 0) {
		printf("\n[HTTPC] ERROR: mbedtls_ssl_handshake %d\n", ret);
		ret = -1;
	} else {
		printf("\n[HTTPC] Use ciphersuite %s\n", mbedtls_ssl_get_ciphersuite(&tls->ctx));
	}

	return ret;
#endif
}

void httpc_tls_close(void *tls_in)
{
	struct httpc_tls *tls = (struct httpc_tls *) tls_in;

#if (HTTPC_USE_TLS == HTTPC_TLS_POLARSSL)
	ssl_close_notify(&tls->ctx);
#elif (HTTPC_USE_TLS == HTTPC_TLS_MBEDTLS)
	mbedtls_ssl_close_notify(&tls->ctx);
#endif
}

int httpc_tls_read(void *tls_in, uint8_t *buf, size_t buf_len)
{
	struct httpc_tls *tls = (struct httpc_tls *) tls_in;

#if (HTTPC_USE_TLS == HTTPC_TLS_POLARSSL)
	return ssl_read(&tls->ctx, buf, buf_len);
#elif (HTTPC_USE_TLS == HTTPC_TLS_MBEDTLS)
	return mbedtls_ssl_read(&tls->ctx, buf, buf_len);
#endif
}

int httpc_tls_write(void *tls_in, uint8_t *buf, size_t buf_len)
{
	struct httpc_tls *tls = (struct httpc_tls *) tls_in;

#if (HTTPC_USE_TLS == HTTPC_TLS_POLARSSL)
	return ssl_write(&tls->ctx, buf, buf_len);
#elif (HTTPC_USE_TLS == HTTPC_TLS_MBEDTLS)
	return mbedtls_ssl_write(&tls->ctx, buf, buf_len);
#endif
}

int httpc_base64_encode(uint8_t *data, size_t data_len, char *base64_buf, size_t buf_len)
{
#if (HTTPC_USE_TLS == HTTPC_TLS_POLARSSL)
	int ret = 0;

	if ((ret = base64_encode(base64_buf, &buf_len, data, data_len)) != 0) {
		printf("\n[HTTPC] ERROR: base64_encode %d\n", ret);
		ret = -1;
	}

	return ret;
#elif (HTTPC_USE_TLS == HTTPC_TLS_MBEDTLS)
	int ret = 0;
	size_t output_len = 0;

	if ((ret = mbedtls_base64_encode((unsigned char *)base64_buf, buf_len, &output_len, data, data_len)) != 0) {
		printf("\n[HTTPC] ERROR: mbedtls_base64_encode %d\n", ret);
		ret = -1;
	}

	return ret;
#endif
}

int httpc_tls_set_ciphersuites(struct httpc_conn *conn, int *ciphersuites)
{
#if (HTTPC_USE_TLS == HTTPC_TLS_POLARSSL)
	return -1;
#elif (HTTPC_USE_TLS == HTTPC_TLS_MBEDTLS)
	mbedtls_ssl_context *ssl_ctx = (mbedtls_ssl_context *) conn->tls;

	if (ssl_ctx) {
		mbedtls_ssl_config *ssl_conf = (mbedtls_ssl_config *) ssl_ctx->conf;

		if (ssl_conf && ciphersuites) {
			mbedtls_ssl_conf_ciphersuites(ssl_conf, ciphersuites);
			return 0;
		}
	}

	return -1;
#endif
}
