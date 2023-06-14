#include "platform_opts.h"
#include <websocket/libwsclient.h>
#include "FreeRTOS.h"

#if (WSCLIENT_USE_TLS == WSCLIENT_TLS_POLARSSL)
#include "polarssl/net.h"
#include "polarssl/ssl.h"
#include <polarssl/memory.h>

struct wss_tls {
	ssl_context ctx;
};

#elif (WSCLIENT_USE_TLS == WSCLIENT_TLS_MBEDTLS)
#include "mbedtls/ssl.h"
#include "mbedtls/net_sockets.h"

struct wss_tls {
	mbedtls_ssl_context ctx;
	mbedtls_ssl_config conf;
	mbedtls_net_context socket;
};
static void *my_calloc(size_t nelements, size_t elementSize)
{
	size_t size;
	void *ptr = NULL;

	size = nelements * elementSize;
	ptr = pvPortMalloc(size);

	if (ptr) {
		memset(ptr, 0, size);
	}

	return ptr;
}

static char *ws_itoa(int value)
{
	char *val_str;
	int tmp = value, len = 1;

	while ((tmp /= 10) > 0) {
		len ++;
	}

	val_str = (char *) pvPortMalloc(len + 1);
	sprintf(val_str, "%d", value);

	return val_str;
}
#endif /* WSCLIENT_USE_TLS */

int ws_random(void *p_rng, unsigned char *output, size_t output_len);
extern int mbedtls_platform_set_calloc_free(void *(*calloc_func)(size_t, size_t), void (*free_func)(void *));
void *wss_tls_connect(int *sock, char *host, int port)
{
#if (WSCLIENT_USE_TLS == WSCLIENT_TLS_POLARSSL)
	int ret;
	struct wss_tls *tls = NULL;

	memory_set_own(pvPortMalloc, vPortFree);
	tls = (struct wss_tls *) malloc(sizeof(struct wss_tls));

	if (tls) {
		ssl_context *ssl = &tls->ctx;
		memset(tls, 0, sizeof(struct wss_tls));

		if ((ret = net_connect(sock, host, port)) != 0) {
			printf("\n[WSCLIENT] ERROR: net_connect %d\n", ret);
			goto exit;
		}

		if ((ret = ssl_init(ssl)) != 0) {
			printf("\n[WSCLIENT] ERROR: ssl_init %d\n", ret);
			goto exit;
		}

		ssl_set_endpoint(ssl, 0);
		ssl_set_authmode(ssl, 0);
		ssl_set_rng(ssl, ws_random, NULL);
		ssl_set_bio(ssl, net_recv, sock, net_send, sock);
	} else {
		printf("\n[WSCLIENT] ERROR: malloc\n");
		ret = -1;
		goto exit;
	}
exit:
	if (ret && tls) {
		net_close(*sock);
		ssl_free(&tls->ctx);
		free(tls);
		tls = NULL;
	}
	return (void *) tls;

#elif (WSCLIENT_USE_TLS == WSCLIENT_TLS_MBEDTLS)
	int ret;
	struct wss_tls *tls = NULL;

	mbedtls_platform_set_calloc_free(my_calloc, vPortFree);
	tls = (struct wss_tls *) malloc(sizeof(struct wss_tls));

	if (tls) {
		mbedtls_ssl_context *ssl = &tls->ctx;
		mbedtls_ssl_config *conf = &tls->conf;
		mbedtls_net_context *server_fd = &tls->socket;
		memset(tls, 0, sizeof(struct wss_tls));

		server_fd->fd = *sock;
		char *port_str = ws_itoa(port);

		if ((ret = mbedtls_net_connect(server_fd, host, port_str, MBEDTLS_NET_PROTO_TCP)) != 0) {
			printf("\n[WSCLIENT] ERROR: net_connect %d\n", ret);
			free(port_str);
			goto exit;
		}

		free(port_str);
		*sock = server_fd->fd;
		mbedtls_ssl_init(ssl);
		mbedtls_ssl_config_init(conf);
		mbedtls_ssl_set_bio(ssl, server_fd, mbedtls_net_send, mbedtls_net_recv, NULL);

		if ((ret = mbedtls_ssl_config_defaults(conf,
											   MBEDTLS_SSL_IS_CLIENT,
											   MBEDTLS_SSL_TRANSPORT_STREAM,
											   MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
			printf("\n[WSCLIENT] ERROR: ssl_config %d\n", ret);
			goto exit;
		}

		mbedtls_ssl_conf_authmode(conf, MBEDTLS_SSL_VERIFY_NONE);
		mbedtls_ssl_conf_rng(conf, ws_random, NULL);

#if MBEDTLS_SSL_MAX_CONTENT_LEN == 4096
		if ((ret = mbedtls_ssl_conf_max_frag_len(conf, MBEDTLS_SSL_MAX_FRAG_LEN_4096)) < 0) {
			printf("\n[WSCLIENT] ERROR: mbedtls_ssl_conf_max_frag_len %d\n", ret);
			goto exit;
		}
#endif

		if ((ret = mbedtls_ssl_setup(ssl, conf)) != 0) {
			printf("\n[WSCLIENT] ERROR: ssl_setup %d\n", ret);
			goto exit;
		}
	} else {
		printf("\n[WSCLIENT] ERROR: malloc\n");
		ret = -1;
		goto exit;
	}
exit:
	if (ret && tls) {
		mbedtls_net_free(&tls->socket);
		mbedtls_ssl_free(&tls->ctx);
		mbedtls_ssl_config_free(&tls->conf);
		free(tls);
		tls = NULL;
	}
	return (void *) tls;
#endif /* WSCLIENT_USE_TLS */
}

int wss_tls_handshake(void *tls_in)
{
	struct wss_tls *tls = (struct wss_tls *) tls_in;

#if (WSCLIENT_USE_TLS == WSCLIENT_TLS_POLARSSL)
	int ret;

	if ((ret = ssl_handshake(&tls->ctx)) != 0) {
		printf("\n[WSCLIENT] ERROR: ssl_handshake %d\n", ret);
		ret = -1;
	} else {
		printf("\n[WSCLIENT] Use ciphersuite %s\n", ssl_get_ciphersuite(&tls->ctx));
	}

	return ret;
#elif (WSCLIENT_USE_TLS == WSCLIENT_TLS_MBEDTLS)
	int ret;

	if ((ret = mbedtls_ssl_handshake(&tls->ctx)) != 0) {
		printf("\n[WSCLIENT] ERROR: ssl_handshake -0x%x\n", -ret);
		ret = -1;
	} else {
		printf("\n[WSCLIENT] Use ciphersuite %s\n", mbedtls_ssl_get_ciphersuite(&tls->ctx));
	}

	return ret;
#endif /* WSCLIENT_USE_TLS */
}


void wss_tls_close(void *tls_in, int *sock)
{
	struct wss_tls *tls = (struct wss_tls *) tls_in;

#if (WSCLIENT_USE_TLS == WSCLIENT_TLS_POLARSSL)
	if (tls) {
		ssl_close_notify(&tls->ctx);
	}

	if (*sock != -1) {
		net_close(*sock);
		*sock = -1;
	}
	ssl_free(&tls->ctx);
	free(tls);
	tls = NULL;
#elif (WSCLIENT_USE_TLS == WSCLIENT_TLS_MBEDTLS)
	if (tls) {
		mbedtls_ssl_close_notify(&tls->ctx);
	}

	if (*sock != -1) {
		mbedtls_net_free(&tls->socket);
		*sock = -1;
	}
	mbedtls_ssl_free(&tls->ctx);
	if (tls) {
		mbedtls_ssl_config_free(&tls->conf);
	}
	free(tls);
	tls = NULL;
#endif /* WSCLIENT_USE_TLS */
}

int wss_tls_write(void *tls_in, char *request, int request_len)
{
	int ret;
	struct wss_tls *tls = (struct wss_tls *) tls_in;

#if (WSCLIENT_USE_TLS == WSCLIENT_TLS_POLARSSL)
	ret = ssl_write(&tls->ctx, request, request_len);
	if (ret == POLARSSL_ERR_NET_WANT_READ || ret == POLARSSL_ERR_NET_WANT_WRITE) {
		ret = 0;
	}
#elif (WSCLIENT_USE_TLS == WSCLIENT_TLS_MBEDTLS)
	ret = mbedtls_ssl_write(&tls->ctx, (unsigned char const *)request, request_len);
	if (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE) {
		ret = 0;
	}
#endif /* WSCLIENT_USE_TLS */
	return ret;
}

int wss_tls_read(void *tls_in, char *buffer, int buf_len)
{
	int ret;
	struct wss_tls *tls = (struct wss_tls *) tls_in;

#if (WSCLIENT_USE_TLS == WSCLIENT_TLS_POLARSSL)
	ret = ssl_read(&tls->ctx, buffer, buf_len);
	if (ret == POLARSSL_ERR_NET_WANT_READ || ret == POLARSSL_ERR_NET_WANT_WRITE
		|| ret == POLARSSL_ERR_NET_RECV_FAILED) {
		ret = 0;
	}
#elif (WSCLIENT_USE_TLS == WSCLIENT_TLS_MBEDTLS)
	ret = mbedtls_ssl_read(&tls->ctx, (unsigned char *)buffer, buf_len);
	if (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE
		|| ret == MBEDTLS_ERR_NET_RECV_FAILED) {
		ret = 0;
	}
#endif /* WSCLIENT_USE_TLS */
	return ret;
}