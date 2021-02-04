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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>

#include "tls_common.h"

static unsigned char rootca[] = ROOT_CA;
static int rootca_len = sizeof(rootca);

static const unsigned char TLS_DATA[] = "This is from CLIENT";

#define TEST_CASE_COUNT 7
static unsigned char test_data[4100] = { 0, };
static int test_size[TEST_CASE_COUNT] = { 64, 128, 256, 512, 1024, 2048, 4096 };
static uint32_t test_log[2][TEST_CASE_COUNT] = { 0, };
static uint32_t log_diff = 0;
static int log_count[2] = { 0, };

#define ENCRYPT 0
#define DECRYPT 1

#if defined(CONFIG_TLS_SECLINK_USE_FRT_TIMER)
static struct timer_status_s before;
static struct timer_status_s after;
static int frt_fd;

#define TLS_START_TIMER(x)                                      \
	frt_fd = open(CONFIG_TLS_SECLINK_FRT_TIMER_PATH, O_RDONLY); \
	ioctl(frt_fd, TCIOC_SETMODE, MODE_FREERUN);                 \
	ioctl(frt_fd, TCIOC_START, 0);                              \
	ioctl(frt_fd, TCIOC_GETSTATUS, (unsigned long)(uintptr_t)&before);

#define TLS_STOP_TIMER(x)                                                          \
	ioctl(frt_fd, TCIOC_GETSTATUS, (unsigned long)(uintptr_t)&after);              \
	ioctl(frt_fd, TCIOC_STOP, 0);                                                  \
	close(frt_fd);                                                                 \
	do {                                                                           \
		log_diff = (unsigned long)after.timeleft - (unsigned long)before.timeleft; \
		test_log[x][log_count[x]++] = log_diff;                                    \
	} while (0);
#else
static struct timespec before;
static struct timespec after;

#define TLS_START_TIMER(x) clock_gettime(CLOCK_REALTIME, &before);

#define TLS_STOP_TIMER(x)                                                                             \
	clock_gettime(CLOCK_REALTIME, &after);                                                            \
	do {                                                                                              \
		if (after.tv_nsec < before.tv_nsec) {                                                         \
			log_diff = (1000000000UL + (unsigned long)after.tv_nsec) - (unsigned long)before.tv_nsec; \
		} else {                                                                                      \
			log_diff = (unsigned long)after.tv_nsec - (unsigned long)before.tv_nsec;                  \
		}                                                                                             \
		test_log[x][log_count[x]++] = log_diff;                                                       \
	} while (0);

#endif

static int ssl_preset_suiteb_ciphersuites[] = {
	MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256,
	0
};

static void my_tls_debug(void *ctx, int level, const char *file, int line, const char *str)
{
	((void)ctx);
	((void)level);

	TLS_SECLINK_CLIENT_LOG("%s:%04d: %s", file, line, str);
	fflush(stdout);
}

int tls_print_test_log()
{
	int i;
	int j;

	printf("-------------------------------------------------------------------------------------------\n");
	printf("        | ");
	for (i = 0; i < TEST_CASE_COUNT; i++) {
		printf("%9d | ", test_size[i]);
	}
	printf("\n");
	printf("-------------------------------------------------------------------------------------------\n");

	for (i = 0; i < 2; i++) {
		printf("%s | ", (i == 0 ? "Encrypt" : "Decrypt"));
		for (j = 0; j < TEST_CASE_COUNT; j++) {
			printf("%9lu | ", test_log[i][j]);
		}
		printf("\n");
	}
	printf("-------------------------------------------------------------------------------------------\n");
	return 0;
}

/****************************************************************************
 * tls_client_seclink
 ****************************************************************************/
void *tls_client_seclink_main(void *arg)
{
	printf("Hello, TLS Clinet Test\n");

	int ret = 1;
	int len = 0;
	int exit_code = -1;
	mbedtls_net_context server_fd;
	uint32_t flags;
	const char *pers = "ssl_client";

	int i;
	int j;

	mbedtls_entropy_context entropy;
	mbedtls_ctr_drbg_context ctr_drbg;
	mbedtls_ssl_context ssl;
	mbedtls_ssl_config conf;
	mbedtls_x509_crt cacert;

#if defined(MBEDTLS_DEBUG_C)
	mbedtls_debug_set_threshold(DEBUG_LEVEL);
#endif

	/*
     * 0. Initialize the RNG and the session data
     */
	mbedtls_net_init(&server_fd);
	mbedtls_ssl_init(&ssl);
	mbedtls_ssl_config_init(&conf);
	mbedtls_x509_crt_init(&cacert);
	mbedtls_ctr_drbg_init(&ctr_drbg);

	TLS_SECLINK_CLIENT_LOG("  . Seeding the random number generator...");

	mbedtls_entropy_init(&entropy);
	if ((ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
									 (const unsigned char *)pers,
									 strlen(pers))) != 0) {
		TLS_SECLINK_CLIENT_LOG(" failed\n  ! mbedtls_ctr_drbg_seed returned %d\n", ret);
		goto exit;
	}

	TLS_SECLINK_CLIENT_LOG(" ok\n");

	/*
     * 0. Initialize certificates
     */
	TLS_SECLINK_CLIENT_LOG("  . Loading the CA root certificate ...");

	ret = mbedtls_x509_crt_parse(&cacert, (const unsigned char *)rootca,
								 rootca_len);
	if (ret < 0) {
		TLS_SECLINK_CLIENT_LOG(" failed\n  !  mbedtls_x509_crt_parse returned -0x%x\n\n", (unsigned int)-ret);
		goto exit;
	}

	TLS_SECLINK_CLIENT_LOG(" ok (%d skipped)\n", ret);

	/*
     * 1. Start the connection
     */
	TLS_SECLINK_CLIENT_LOG("  . Connecting to tcp/%s/%s...", SERVER_ADDR, SERVER_PORT);

	if ((ret = mbedtls_net_connect(&server_fd, SERVER_ADDR,
								   SERVER_PORT, MBEDTLS_NET_PROTO_TCP)) != 0) {
		TLS_SECLINK_CLIENT_LOG(" failed\n  ! mbedtls_net_connect returned %d\n\n", ret);
		goto exit;
	}

	TLS_SECLINK_CLIENT_LOG(" ok\n");

	/*
     * 2. Setup stuff
     */
	TLS_SECLINK_CLIENT_LOG("  . Setting up the SSL/TLS structure...");

	if ((ret = mbedtls_ssl_config_defaults(&conf,
										   MBEDTLS_SSL_IS_CLIENT,
										   MBEDTLS_SSL_TRANSPORT_STREAM,
										   MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
		TLS_SECLINK_CLIENT_LOG(" failed\n  ! mbedtls_ssl_config_defaults returned %d\n\n", ret);
		goto exit;
	}

	TLS_SECLINK_CLIENT_LOG(" ok\n");

	/* OPTIONAL is not optimal for security,
     * but makes interop easier in this simplified example */
	mbedtls_ssl_conf_authmode(&conf, MBEDTLS_SSL_VERIFY_REQUIRED);
	mbedtls_ssl_conf_ca_chain(&conf, &cacert, NULL);
	mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);
	mbedtls_ssl_conf_dbg(&conf, my_tls_debug, stdout);
	mbedtls_ssl_conf_ciphersuites(&conf, ssl_preset_suiteb_ciphersuites);

	if ((ret = mbedtls_ssl_setup(&ssl, &conf)) != 0) {
		TLS_SECLINK_CLIENT_LOG(" failed\n  ! mbedtls_ssl_setup returned %d\n\n", ret);
		goto exit;
	}

	if ((ret = mbedtls_ssl_set_hostname(&ssl, SERVER_NAME)) != 0) {
		TLS_SECLINK_CLIENT_LOG(" failed\n  ! mbedtls_ssl_set_hostname returned %d\n\n", ret);
		goto exit;
	}

	mbedtls_ssl_set_bio(&ssl, &server_fd, mbedtls_net_send, mbedtls_net_recv, NULL);

	/*
     * 4. Handshake
     */
	TLS_SECLINK_CLIENT_LOG("  . Performing the SSL/TLS handshake...");

	while ((ret = mbedtls_ssl_handshake(&ssl)) != 0) {
		if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
			TLS_SECLINK_CLIENT_LOG(" failed\n  ! mbedtls_ssl_handshake returned -0x%x\n\n", (unsigned int)-ret);
			goto exit;
		}
	}

	TLS_SECLINK_CLIENT_LOG(" ok\n");

	/*
     * 5. Verify the server certificate
     */
	TLS_SECLINK_CLIENT_LOG("  . Verifying peer X.509 certificate...");

	/* In real life, we probably want to bail out when ret != 0 */
	if ((flags = mbedtls_ssl_get_verify_result(&ssl)) != 0) {
		char vrfy_buf[512];

		TLS_SECLINK_CLIENT_LOG(" failed\n");

		mbedtls_x509_crt_verify_info(vrfy_buf, sizeof(vrfy_buf), "  ! ", flags);

		TLS_SECLINK_CLIENT_LOG("%s\n", vrfy_buf);
	} else
		TLS_SECLINK_CLIENT_LOG(" ok\n");

	/*
     * 3. Write the GET request
     */

	TLS_SECLINK_CLIENT_LOG("  > Start Packet Perfomance Test [%d]\n", sizeof(test_size));

	while ((ret = mbedtls_ssl_write(&ssl, TLS_DATA, sizeof(TLS_DATA))) <= 0) {
		if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
			TLS_SECLINK_CLIENT_LOG(" failed\n  ! mbedtls_ssl_write returned %d\n\n", ret);
			goto exit;
		}
	}

	for (i = 0; i < sizeof(test_size) / sizeof(int); i++) {
		TLS_SECLINK_CLIENT_LOG("Try Write #%d [ %d bytes ] -->\n", i + 1, test_size[i]);

		for (j = 0; j < test_size[i]; j++) {
			test_data[j] = 'A' + (j % 26);
		}

		len = test_size[i];

		TLS_START_TIMER(ENCRYPT)
		while ((ret = mbedtls_ssl_write(&ssl, test_data, len)) <= 0) {
			if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
				TLS_SECLINK_CLIENT_LOG(" failed\n  ! mbedtls_ssl_write returned %d\n\n", ret);
				TLS_STOP_TIMER(ENCRYPT)
				goto exit;
			}
		}
		TLS_STOP_TIMER(ENCRYPT)

		TLS_SECLINK_CLIENT_LOG("Try Write #%d Done <--\n", i + 1);
	}

	/*
     * 7. Read the HTTP response
     */
	TLS_SECLINK_CLIENT_LOG("  < Read from server:\n");

	memset(test_data, 0, sizeof(test_data));
	ret = mbedtls_ssl_read(&ssl, test_data, len);

	do {
		len = sizeof(test_data) - 1;
		memset(test_data, 0, sizeof(test_data));
		TLS_START_TIMER(DECRYPT)
		ret = mbedtls_ssl_read(&ssl, test_data, len);
		TLS_STOP_TIMER(DECRYPT)

		if (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE) {
			continue;
		}

		if (ret == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY) {
			break;
		}

		if (ret < 0) {
			TLS_SECLINK_CLIENT_LOG("failed\n  ! mbedtls_ssl_read returned %d\n\n", ret);
			break;
		}

		if (ret == 0) {
			TLS_SECLINK_CLIENT_LOG("\n\nEOF\n\n");
			break;
		}
		TLS_SECLINK_CLIENT_LOG(" %d bytes read\n", ret);

	} while (1);

	mbedtls_ssl_close_notify(&ssl);
	exit_code = 0;

exit:

#ifdef MBEDTLS_ERROR_C
	if (exit_code != 0) {
		char error_buf[100];
		mbedtls_strerror(ret, error_buf, 100);
		TLS_SECLINK_CLIENT_LOG("[CLIENT] Last error was: %d - %s\n\n", ret, error_buf);
	}
#endif

	mbedtls_net_free(&server_fd);

	mbedtls_x509_crt_free(&cacert);
	mbedtls_ssl_free(&ssl);
	mbedtls_ssl_config_free(&conf);
	mbedtls_ctr_drbg_free(&ctr_drbg);
	mbedtls_entropy_free(&entropy);

	return (void *)exit_code;
}
