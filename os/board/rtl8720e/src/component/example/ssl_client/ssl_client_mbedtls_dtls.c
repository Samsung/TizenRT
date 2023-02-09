#include "FreeRTOS.h"
#include "task.h"
#include <string.h>
#include <stdio.h>

#include "mbedtls/config.h"
#include "mbedtls/platform.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/ssl.h"
#include "mbedtls/error.h"
#include "mbedtls/debug.h"
#include "mbedtls/timing.h"

#define SERVER_PORT   "4433"
#define SERVER_HOST   "192.168.13.15"
#define GET_REQUEST   "GET / HTTP/1.0\r\n\r\n"
#define DEBUG_LEVEL   0

//#define SSL_CLIENT_EXT
#define STACKSIZE     2048

static int is_task = 0;
static char server_host[32];
static size_t min_heap_size = 0;

static void my_debug(void *ctx, int level, const char *file, int line, const char *str)
{
	printf("\n\r%s:%d: %s\n\r", file, line, str);
}

static int my_random(void *p_rng, unsigned char *output, size_t output_len)
{
	rtw_get_random_bytes(output, output_len);
	return 0;
}

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
	mbedtls_timing_delay_context timer;

	mbedtls_platform_set_calloc_free(my_calloc, my_free);
#if defined(MBEDTLS_DEBUG_C)
	mbedtls_debug_set_threshold(DEBUG_LEVEL);
#endif

	/*
	 * 1. Start the connection
	 */
	printf("\n\r  . Connecting to udp/%s/%s...", server_host, SERVER_PORT);

	mbedtls_net_init(&server_fd);

	if ((ret = mbedtls_net_connect(&server_fd, server_host, SERVER_PORT, MBEDTLS_NET_PROTO_UDP)) != 0) {
		printf(" failed\n\r  ! mbedtls_net_connect returned %d\n", ret);
		goto exit1;
	}

	printf(" ok\n");

	/*
	 * 2. Setup stuff
	 */
	printf("  . Setting up the SSL/TLS structure...");

	mbedtls_ssl_init(&ssl);
	mbedtls_ssl_config_init(&conf);

#ifdef SSL_CLIENT_EXT
	if ((ret = ssl_client_ext_init()) != 0) {
		printf(" failed\n\r  ! ssl_client_ext_init returned %d\n", ret);
		goto exit;
	}
#endif

	mbedtls_ssl_set_bio(&ssl, &server_fd, mbedtls_net_send, mbedtls_net_recv, mbedtls_net_recv_timeout);
	mbedtls_ssl_set_timer_cb(&ssl, &timer, mbedtls_timing_set_delay, mbedtls_timing_get_delay);

	if ((ret = mbedtls_ssl_config_defaults(&conf,
										   MBEDTLS_SSL_IS_CLIENT,
										   MBEDTLS_SSL_TRANSPORT_DATAGRAM,
										   MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {

		printf(" failed\n  ! mbedtls_ssl_config_defaults returned %d\n\n", ret);
		goto exit;
	}

	mbedtls_ssl_conf_authmode(&conf, MBEDTLS_SSL_VERIFY_NONE);
	mbedtls_ssl_conf_rng(&conf, my_random, NULL);
	mbedtls_ssl_conf_dbg(&conf, my_debug, NULL);

#ifdef SSL_CLIENT_EXT
	if ((ret = ssl_client_ext_setup(&conf)) != 0) {
		printf(" failed\n\r  ! ssl_client_ext_setup returned %d\n", ret);
		goto exit;
	}
#endif

	if ((ret = mbedtls_ssl_setup(&ssl, &conf)) != 0) {
		printf(" failed\n  ! mbedtls_ssl_setup returned %d\n\n", ret);
		goto exit;
	}

	printf(" ok\n");

	/*
	 * 3. Handshake
	 */
	printf("\n\r  . Performing the SSL/TLS handshake...");

	while ((ret = mbedtls_ssl_handshake(&ssl)) != 0) {
		if ((ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE
			 && ret != MBEDTLS_ERR_NET_RECV_FAILED) || retry_count >= 20) {

			printf(" failed\n\r  ! mbedtls_ssl_handshake returned -0x%x\n", -ret);
			goto exit;
		}

		retry_count++;
	}

	printf(" ok\n");
	printf("\n\r  . Use ciphersuite %s\n", mbedtls_ssl_get_ciphersuite(&ssl));

	/*
	 * 4. Write the GET request
	 */
	printf("\n\r  > Write to server:");

	len = sprintf((char *) buf, GET_REQUEST);

	while ((ret = mbedtls_ssl_write(&ssl, buf, len)) <= 0) {
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
		ret = mbedtls_ssl_read(&ssl, buf, len);

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

	do {
		ret = mbedtls_ssl_close_notify(&ssl);
	} while (ret == MBEDTLS_ERR_SSL_WANT_WRITE);

	ret = 0;

exit:

#ifdef MBEDTLS_ERROR_C
	if (ret != 0) {
		char error_buf[100];
		mbedtls_strerror(ret, error_buf, 100);
		printf("Last error was: %d - %s\n\n", ret, error_buf);
	}
#endif

	mbedtls_net_free(&server_fd);
	mbedtls_ssl_free(&ssl);
	mbedtls_ssl_config_free(&conf);

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
