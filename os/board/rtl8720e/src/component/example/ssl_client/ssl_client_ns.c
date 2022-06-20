#include "FreeRTOS.h"
#include "task.h"
#include <string.h>
#include <stdio.h>

#include "platform_opts.h"
#include "osdep_service.h"
#include "device_lock.h"
#include "mbedtls/config.h"
#include "mbedtls/platform.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/ssl.h"
#include "mbedtls/error.h"
#include "mbedtls/debug.h"

#define SERVER_PORT   "443"
#define SERVER_HOST   "192.168.13.15"
#define GET_REQUEST   "GET / HTTP/1.0\r\n\r\n"
#define DEBUG_LEVEL   0

#define SSL_CLIENT_EXT
#define STACKSIZE     2048

static int is_task = 0;
static char server_host[32];
static size_t min_heap_size = 0;

#ifdef SSL_CLIENT_EXT
extern int ssl_client_ext_init(void);
extern int ssl_client_ext_setup(mbedtls_ssl_config *conf);
extern void ssl_client_ext_free(void);
#endif

extern int NS_ENTRY secure_mbedtls_platform_set_calloc_free(void);
extern int NS_ENTRY secure_mbedtls_platform_set_ns_calloc_free(void *(*calloc_func)(size_t, size_t), void (*free_func)(void *));
extern void NS_ENTRY secure_mbedtls_ssl_init(mbedtls_ssl_context *ssl);
extern void NS_ENTRY secure_mbedtls_ssl_free(mbedtls_ssl_context *ssl);
extern void NS_ENTRY secure_mbedtls_ssl_conf_rng(mbedtls_ssl_config *conf, void *p_rng);
extern void NS_ENTRY secure_mbedtls_ssl_conf_dbg(mbedtls_ssl_config *conf, void  *p_dbg);
extern void NS_ENTRY secure_mbedtls_ssl_config_free(mbedtls_ssl_config *conf);
extern int NS_ENTRY secure_mbedtls_ssl_setup(mbedtls_ssl_context *ssl, const mbedtls_ssl_config *conf);
extern int NS_ENTRY secure_mbedtls_ssl_handshake(mbedtls_ssl_context *ssl);
extern char *NS_ENTRY secure_mbedtls_ssl_get_ciphersuite(const mbedtls_ssl_context *ssl, char *buf);
extern int NS_ENTRY secure_mbedtls_ssl_read(mbedtls_ssl_context *ssl, unsigned char *buf, size_t len);
extern int NS_ENTRY secure_mbedtls_ssl_write(mbedtls_ssl_context *ssl, const unsigned char *buf, size_t len);
extern int NS_ENTRY secure_mbedtls_ssl_close_notify(mbedtls_ssl_context *ssl);
extern void NS_ENTRY secure_set_ns_device_lock(void (*device_mutex_lock_func)(uint32_t), void (*device_mutex_unlock_func)(uint32_t));

static void *my_calloc(size_t nelements, size_t elementSize)
{
	size_t current_heap_size, size;
	void *ptr = NULL;

	size = nelements * elementSize;
	ptr = pvPortMalloc(size);

	if (ptr) {
		memset(ptr, 0, size);
	}

	current_heap_size = xPortGetFreeHeapSize();

	if ((current_heap_size < min_heap_size) || (min_heap_size == 0)) {
		min_heap_size = current_heap_size;
	}

	return ptr;
}

#define my_free		vPortFree

static void ssl_client(void *param)
{
	int ret, len;
	int retry_count = 0;
	unsigned char buf[512];
	mbedtls_net_context server_fd;
	mbedtls_ssl_context ssl;
	mbedtls_ssl_config conf;

	rtw_create_secure_context(STACKSIZE * 4);
	secure_mbedtls_platform_set_calloc_free();
	secure_mbedtls_platform_set_ns_calloc_free(my_calloc, my_free);
	secure_set_ns_device_lock(device_mutex_lock, device_mutex_unlock);

	/*
	 * 1. Start the connection
	 */
	printf("\n\r  . Connecting to tcp/%s/%s...", server_host, SERVER_PORT);

	mbedtls_net_init(&server_fd);

	if ((ret = mbedtls_net_connect(&server_fd, server_host, SERVER_PORT, MBEDTLS_NET_PROTO_TCP)) != 0) {
		printf(" failed\n\r  ! mbedtls_net_connect returned %d\n", ret);
		goto exit1;
	}

	printf(" ok\n");

	/*
	 * 2. Setup stuff
	 */
	printf("  . Setting up the SSL/TLS structure...");

	secure_mbedtls_ssl_init(&ssl);
	mbedtls_ssl_config_init(&conf);

	mbedtls_ssl_set_bio(&ssl, &server_fd, mbedtls_net_send, mbedtls_net_recv, NULL);

	if ((ret = mbedtls_ssl_config_defaults(&conf,
										   MBEDTLS_SSL_IS_CLIENT,
										   MBEDTLS_SSL_TRANSPORT_STREAM,
										   MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {

		printf(" failed\n  ! mbedtls_ssl_config_defaults returned %d\n\n", ret);
		goto exit;
	}

	mbedtls_ssl_conf_authmode(&conf, MBEDTLS_SSL_VERIFY_NONE);
	secure_mbedtls_ssl_conf_rng(&conf, NULL);
	secure_mbedtls_ssl_conf_dbg(&conf, NULL);

#ifdef SSL_CLIENT_EXT
	if ((ret = ssl_client_ext_setup(&conf)) != 0) {
		printf(" failed\n\r  ! ssl_client_ext_setup returned %d\n", ret);
		goto exit;
	}
#endif

	if ((ret = secure_mbedtls_ssl_setup(&ssl, &conf)) != 0) {
		printf(" failed\n  ! mbedtls_ssl_setup returned %d\n\n", ret);
		goto exit;
	}

	printf(" ok\n");

	/*
	 * 3. Handshake
	 */
	printf("\n\r  . Performing the SSL/TLS handshake...");

	while ((ret = secure_mbedtls_ssl_handshake(&ssl)) != 0) {
		if ((ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE
			 && ret != MBEDTLS_ERR_NET_RECV_FAILED) || retry_count >= 5) {

			printf(" failed\n\r  ! mbedtls_ssl_handshake returned -0x%x\n", -ret);
			goto exit;
		}

		retry_count++;
	}

	printf(" ok\n");
	char ciphersuite_name_buf[100];
	printf("\n\r  . Use ciphersuite %s\n", secure_mbedtls_ssl_get_ciphersuite(&ssl, ciphersuite_name_buf));

	/*
	 * 4. Write the GET request
	 */
	printf("\n\r  > Write to server:");

	len = sprintf((char *) buf, GET_REQUEST);

	while ((ret = secure_mbedtls_ssl_write(&ssl, buf, len)) <= 0) {
		if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
			printf(" failed\n\r  ! mbedtls_ssl_write returned %d\n", ret);
			goto exit;
		}
	}

	len = ret;
	printf(" %d bytes written\n\n%s", len, (char *) buf);

	/*
	 * 5. Read the HTTP response
	 */
	printf("  < Read from server:");

	do {
		len = sizeof(buf) - 1;
		memset(buf, 0, sizeof(buf));
		ret = secure_mbedtls_ssl_read(&ssl, buf, len);

		if (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE) {
			continue;
		}

		if (ret == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY) {
			break;
		}

		if (ret < 0) {
			printf(" failed\n  ! mbedtls_ssl_read returned %d\n", ret);
			break;
		}

		if (ret == 0) {
			printf("\n\nEOF\n\n");
			break;
		}

		len = ret;
		printf(" %d bytes read\n\n%s", len, (char *) buf);
	} while (1);

	secure_mbedtls_ssl_close_notify(&ssl);

exit:

#ifdef MBEDTLS_ERROR_C
	if (ret != 0) {
		char error_buf[100];
		mbedtls_strerror(ret, error_buf, 100);
		printf("Last error was: %d - %s\n\n", ret, error_buf);
	}
#endif

	mbedtls_net_free(&server_fd);
	secure_mbedtls_ssl_free(&ssl);
	secure_mbedtls_ssl_config_free(&conf);

#ifdef SSL_CLIENT_EXT
	ssl_client_ext_free();
#endif

exit1:
	if (is_task) {
#if defined(INCLUDE_uxTaskGetStackHighWaterMark) && (INCLUDE_uxTaskGetStackHighWaterMark == 1)
		printf("\n\rMin available stack size of %s = %d * %d bytes\n\r", __FUNCTION__, uxTaskGetStackHighWaterMark(NULL), sizeof(portBASE_TYPE));
#endif

		if (min_heap_size > 0) {
			printf("\n\rMin available heap size = %d bytes during %s\n\r", min_heap_size, __FUNCTION__);
		}

		vTaskDelete(NULL);
	}

	if (param != NULL) {
		*((int *) param) = ret;
	}
}

void start_ssl_client(void)
{
	is_task = 1;
	//strcpy(server_host, SERVER_HOST);

	if (xTaskCreate(ssl_client, "ssl_client", STACKSIZE, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
		printf("\n\r%s xTaskCreate failed", __FUNCTION__);
	}
}

void do_ssl_connect(void)
{
	int ret;
	static int success = 0;
	static int fail = 0;

	is_task = 0;
	strcpy(server_host, SERVER_HOST);
	ssl_client(&ret);

	if (ret != 0) {
		printf("\n\r%s fail (success %d times, fail %d times)\n\r", __FUNCTION__, success, ++ fail);
	} else {
		printf("\n\r%s success (success %d times, fail %d times)\n\r", __FUNCTION__, ++ success, fail);
	}
}

void cmd_ssl_client(int argc, char **argv)
{
	if (argc == 2) {
		strcpy(server_host, argv[1]);
	} else {
		printf("\n\rUsage: %s SSL_SERVER_HOST", argv[0]);
		return;
	}

	start_ssl_client();
}

