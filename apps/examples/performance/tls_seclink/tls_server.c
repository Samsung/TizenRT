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

static unsigned char srv_cert[] = SERVER_CERT;
static int srv_cert_len = sizeof(srv_cert);
static unsigned char srv_key[] = SERVER_KEY;
static int srv_key_len = sizeof(srv_key);
static unsigned char rootca[] = ROOT_CA;
static int rootca_len = sizeof(rootca);

static const unsigned char TLS_DATA[] = "This is from SERVER";

static unsigned char test_data[4100] = { 0, };
static int test_size[7] = {64, 128, 256, 512, 1024, 2048, 4096};

static void my_tls_debug(void *ctx, int level, const char *file, int line, const char *str)
{
	((void)ctx);
	((void)level);

	TLS_SECLINK_SERVER_LOG("%s:%04d: %s", file, line, str);
	fflush(stdout);
}

void *tls_server_seclink_main(void *arg)
{
	printf("Hello, TLS Server Test\n");

	int ret, len;
	mbedtls_net_context listen_fd, client_fd;
	unsigned char buf[4100];
	const char *pers = "ssl_server";

	mbedtls_entropy_context entropy;
	mbedtls_ctr_drbg_context ctr_drbg;
	mbedtls_ssl_context ssl;
	mbedtls_ssl_config conf;
	mbedtls_x509_crt srvcert;
	mbedtls_pk_context pkey;

	mbedtls_net_init(&listen_fd);
	mbedtls_net_init(&client_fd);
	mbedtls_ssl_init(&ssl);
	mbedtls_ssl_config_init(&conf);
	mbedtls_x509_crt_init(&srvcert);
	mbedtls_pk_init(&pkey);
	mbedtls_entropy_init(&entropy);
	mbedtls_ctr_drbg_init(&ctr_drbg);

#if defined(MBEDTLS_DEBUG_C)
	mbedtls_debug_set_threshold(DEBUG_LEVEL);
#endif

	/*
     * 1. Load the certificates and private RSA key
     */
	TLS_SECLINK_SERVER_LOG("  . Loading the server cert. and key...");

	/*
     * This demonstration program uses embedded test certificates.
     * Instead, you may want to use mbedtls_x509_crt_parse_file() to read the
     * server and CA certificates, as well as mbedtls_pk_parse_keyfile().
     */

	ret = mbedtls_x509_crt_parse(&srvcert, (const unsigned char *)srv_cert,
								 srv_cert_len);
	if (ret != 0) {
		TLS_SECLINK_SERVER_LOG(" failed\n  !  mbedtls_x509_crt_parse returned %d\n\n", ret);
		goto exit;
	}

	ret = mbedtls_x509_crt_parse(&srvcert, (const unsigned char *)rootca,
								 rootca_len);
	if (ret != 0) {
		TLS_SECLINK_SERVER_LOG(" failed\n  !  mbedtls_x509_crt_parse returned %d\n\n", ret);
		goto exit;
	}

	ret = mbedtls_pk_parse_key(&pkey, (const unsigned char *)srv_key,
							   srv_key_len, NULL, 0);
	if (ret != 0) {
		TLS_SECLINK_SERVER_LOG(" failed\n  !  mbedtls_pk_parse_key returned %d\n\n", ret);
		goto exit;
	}
	TLS_SECLINK_SERVER_LOG(" ok\n");

	/*
     * 2. Setup the listening TCP socket
     */
	TLS_SECLINK_SERVER_LOG("  . Bind on https://localhost:4433/ ...");

	if ((ret = mbedtls_net_bind(&listen_fd, NULL, "4433", MBEDTLS_NET_PROTO_TCP)) != 0) {
		TLS_SECLINK_SERVER_LOG(" failed\n  ! mbedtls_net_bind returned %d\n\n", ret);
		goto exit;
	}

	TLS_SECLINK_SERVER_LOG(" ok\n");

	/*
     * 3. Seed the RNG
     */
	TLS_SECLINK_SERVER_LOG("  . Seeding the random number generator...");

	if ((ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
									 (const unsigned char *)pers,
									 strlen(pers))) != 0) {
		TLS_SECLINK_SERVER_LOG(" failed\n  ! mbedtls_ctr_drbg_seed returned %d\n", ret);
		goto exit;
	}

	TLS_SECLINK_SERVER_LOG(" ok\n");

	/*
     * 4. Setup stuff
     */
	TLS_SECLINK_SERVER_LOG("  . Setting up the SSL data....");

	if ((ret = mbedtls_ssl_config_defaults(&conf,
										   MBEDTLS_SSL_IS_SERVER,
										   MBEDTLS_SSL_TRANSPORT_STREAM,
										   MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
		TLS_SECLINK_SERVER_LOG(" failed\n  ! mbedtls_ssl_config_defaults returned %d\n\n", ret);
		goto exit;
	}

	mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);
	mbedtls_ssl_conf_dbg(&conf, my_tls_debug, stdout);

	mbedtls_ssl_conf_ca_chain(&conf, srvcert.next, NULL);
	if ((ret = mbedtls_ssl_conf_own_cert(&conf, &srvcert, &pkey)) != 0) {
		TLS_SECLINK_SERVER_LOG(" failed\n  ! mbedtls_ssl_conf_own_cert returned %d\n\n", ret);
		goto exit;
	}

	if ((ret = mbedtls_ssl_setup(&ssl, &conf)) != 0) {
		TLS_SECLINK_SERVER_LOG(" failed\n  ! mbedtls_ssl_setup returned %d\n\n", ret);
		goto exit;
	}

	TLS_SECLINK_SERVER_LOG(" ok\n");

reset:
#ifdef MBEDTLS_ERROR_C
	if (ret != 0) {
		char error_buf[100];
		mbedtls_strerror(ret, error_buf, 100);
		TLS_SECLINK_SERVER_LOG("[SERVER] Last error was: %d - %s\n", ret, error_buf);
		TLS_SECLINK_SERVER_LOG("[SERVER] Sever Reset\n\n");
	}
#endif

	mbedtls_net_free(&client_fd);

	mbedtls_ssl_session_reset(&ssl);

	/*
     * 3. Wait until a client connects
     */
	
	TLS_SECLINK_SERVER_LOG("  . Waiting for a remote connection ...");

	if ((ret = mbedtls_net_accept(&listen_fd, &client_fd,
								  NULL, 0, NULL)) != 0) {
		TLS_SECLINK_SERVER_LOG(" failed\n  ! mbedtls_net_accept returned %d\n\n", ret);
		goto exit;
	}

	mbedtls_ssl_set_bio(&ssl, &client_fd, mbedtls_net_send, mbedtls_net_recv, NULL);

	TLS_SECLINK_SERVER_LOG(" ok\n");

	/*
     * 5. Handshake
     */
	TLS_SECLINK_SERVER_LOG("  . Performing the SSL/TLS handshake...");

	while ((ret = mbedtls_ssl_handshake(&ssl)) != 0) {
		if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
			TLS_SECLINK_SERVER_LOG(" failed\n  ! mbedtls_ssl_handshake returned %d\n\n", ret);
			goto exit;
		}
	}

	TLS_SECLINK_SERVER_LOG(" ok\n");

	/*
     * 6. Read the HTTP Request
     */
	TLS_SECLINK_SERVER_LOG("  < Read from client:\n");

	do {
		len = sizeof(buf) - 1;
		memset(buf, 0, sizeof(buf));
		ret = mbedtls_ssl_read(&ssl, buf, len);

		if (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE)
			continue;

		if (ret <= 0) {
			switch (ret) {
			case MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY:
				TLS_SECLINK_SERVER_LOG(" connection was closed gracefully\n");
				break;

			case MBEDTLS_ERR_NET_CONN_RESET:
				TLS_SECLINK_SERVER_LOG(" connection was reset by peer\n");
				break;

			default:
				TLS_SECLINK_SERVER_LOG(" mbedtls_ssl_read returned -0x%x\n", (unsigned int)-ret);
				break;
			}

			break;
		}

		len = ret;
		// TLS_SECLINK_SERVER_LOG(" %d bytes read\n", len);

		if (ret >= 4096)
			break;
	} while (1);

	/*
     * 7. Write the 200 Response
     */
	int i;
	int j;

	while ((ret = mbedtls_ssl_write(&ssl, TLS_DATA, sizeof(TLS_DATA))) <= 0) {
		if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
			printf(" failed\n  ! mbedtls_ssl_write returned %d\n\n", ret);
			goto exit;
		}
	}

	TLS_SECLINK_SERVER_LOG("  > Start Packet Perfomance Test [%ld]\n", sizeof(test_size));

	for (i = 0; i < sizeof(test_size) / sizeof(int); i++) {
		// TLS_SECLINK_SERVER_LOG("Try #%d [ %d bytes ] -->\n", i + 1, test_size[i]);

		for (j = 0; j < test_size[i]; j++) {
			test_data[j] = 'A' + (j % 26);
		}

		len = test_size[i];

		while ((ret = mbedtls_ssl_write(&ssl, test_data, len)) <= 0) {
			if (ret == MBEDTLS_ERR_NET_CONN_RESET) {
				TLS_SECLINK_SERVER_LOG(" failed\n  ! peer closed the connection\n\n");
				goto reset;
			}

			if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
				TLS_SECLINK_SERVER_LOG(" failed\n  ! mbedtls_ssl_write returned %d\n\n", ret);
				goto exit;
			}
		}

		// TLS_SECLINK_SERVER_LOG("Try #%d Done <--\n", i + 1);
	}

	TLS_SECLINK_SERVER_LOG("  . Closing the connection...");

	while ((ret = mbedtls_ssl_close_notify(&ssl)) < 0) {
		if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
			TLS_SECLINK_SERVER_LOG(" failed\n  ! mbedtls_ssl_close_notify returned %d\n\n", ret);
			goto exit;
		}
	}

	TLS_SECLINK_SERVER_LOG(" ok\n");

	ret = 0;

exit:
#ifdef MBEDTLS_ERROR_C
	if (ret != 0) {
		char error_buf[100];
		mbedtls_strerror(ret, error_buf, 100);
		TLS_SECLINK_SERVER_LOG("[SERVER] Last error was: %d - %s\n\n", ret, error_buf);
	}
#endif

	mbedtls_net_free(&client_fd);
	mbedtls_net_free(&listen_fd);

	mbedtls_x509_crt_free(&srvcert);
	mbedtls_pk_free(&pkey);
	mbedtls_ssl_free(&ssl);
	mbedtls_ssl_config_free(&conf);
	mbedtls_ctr_drbg_free(&ctr_drbg);
	mbedtls_entropy_free(&entropy);

	return (void *)ret;
}