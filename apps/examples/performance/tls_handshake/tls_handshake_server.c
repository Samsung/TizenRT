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
/*
 *  SSL server demonstration program
 *
 *  Copyright The Mbed TLS Contributors
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
 */
#include <tinyara/config.h>
#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#include <stdlib.h>
#define mbedtls_time time
#define mbedtls_time_t time_t
#define mbedtls_fprintf fprintf
#define mbedtls_printf printf
#define mbedtls_exit exit
#define MBEDTLS_EXIT_SUCCESS EXIT_SUCCESS
#define MBEDTLS_EXIT_FAILURE EXIT_FAILURE
#endif

#if !defined(MBEDTLS_BIGNUM_C) || !defined(MBEDTLS_CERTS_C) ||       \
	!defined(MBEDTLS_ENTROPY_C) || !defined(MBEDTLS_SSL_TLS_C) ||    \
	!defined(MBEDTLS_SSL_SRV_C) || !defined(MBEDTLS_NET_C) ||        \
	!defined(MBEDTLS_RSA_C) || !defined(MBEDTLS_CTR_DRBG_C) ||       \
	!defined(MBEDTLS_X509_CRT_PARSE_C) || !defined(MBEDTLS_FS_IO) || \
	!defined(MBEDTLS_PEM_PARSE_C)
int main(void)
{
	mbedtls_printf("MBEDTLS_BIGNUM_C and/or MBEDTLS_CERTS_C and/or MBEDTLS_ENTROPY_C "
				   "and/or MBEDTLS_SSL_TLS_C and/or MBEDTLS_SSL_SRV_C and/or "
				   "MBEDTLS_NET_C and/or MBEDTLS_RSA_C and/or "
				   "MBEDTLS_CTR_DRBG_C and/or MBEDTLS_X509_CRT_PARSE_C "
				   "and/or MBEDTLS_PEM_PARSE_C not defined.\n");
	mbedtls_exit(0);
}
#else

#include <stdlib.h>
#include <string.h>

#if defined(_WIN32)
#include <windows.h>
#endif

#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/certs.h"
#include "mbedtls/x509.h"
#include "mbedtls/ssl.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/error.h"
#include "mbedtls/debug.h"

#if defined(MBEDTLS_SSL_CACHE_C)
#include "mbedtls/ssl_cache.h"
#endif

#define HTTP_RESPONSE                                    \
	"HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n" \
	"<h2>mbed TLS Test Server</h2>\r\n"                  \
	"<p>Successful connection using: %s</p>\r\n"
#define TLS_DATA "This is from SERVER"

#define DEBUG_LEVEL 3

static void my_debug(void *ctx, int level,
					 const char *file, int line,
					 const char *str)
{
	((void)level);

	mbedtls_fprintf((FILE *)ctx, "%s:%04d: %s", file, line, str);
	fflush((FILE *)ctx);
}

static unsigned char srv_cert[] =
	"-----BEGIN CERTIFICATE-----\r\n"
	"MIICjjCCAjOgAwIBAgIBBjAKBggqhkjOPQQDAjCBlzELMAkGA1UEBhMCS1IxLjAs\r\n"
	"BgNVBAoMJVNhbXN1bmcgRWxlY3Ryb25pY3MgRGlnaXRhbCBBcHBsaWFuY2UxITAf\r\n"
	"BgNVBAsMGERBIFRlc3QgT0NGIFNlcnZlciBTdWJDQTE1MDMGA1UEAwwsREEgVGVz\r\n"
	"dCBTYW1zdW5nIEVsZWN0cm9uaWNzIE9DRiBTZXJ2ZXIgU3ViQ0EwHhcNMjEwODEz\r\n"
	"MDYxNzEyWhcNMjUwODEyMDYxNzEyWjCBrDELMAkGA1UEBhMCS1IxLjAsBgNVBAoM\r\n"
	"JVNhbXN1bmcgRWxlY3Ryb25pY3MgRGlnaXRhbCBBcHBsaWFuY2UxMjAwBgNVBAsM\r\n"
	"KXV1aWQ6NmJlMzEyZjUtNzkwNS00NWZlLTg3NGMtZDZjNjczNzZmYWNhMTkwNwYD\r\n"
	"VQQDDDBEQSBUZXN0IFNhbXN1bmcgRWxlY3Ryb25pY3MgT0NGIFNlcnZlciBMZWFm\r\n"
	"IENlcnQwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAAT0fz3ykPzTpUiX+rdhM7cQ\r\n"
	"w5YLCIy9zwIhLkLza00KgFKEkzVApOyhz5CdFVAedFe1yMhiSNihd+ZoZVsrcq99\r\n"
	"o1kwVzAJBgNVHRMEAjAAMAsGA1UdDwQEAwIFoDAdBgNVHQ4EFgQU/DLAd/kGgIa2\r\n"
	"YFVydkYJBvwrLAowCQYDVR0jBAIwADATBgNVHSUEDDAKBggrBgEFBQcDATAKBggq\r\n"
	"hkjOPQQDAgNJADBGAiEAhqjNxGegNweqE9CaVB6CmpJzsH+d1zU1x+wKMyIU9ncC\r\n"
	"IQDK0SYF16hEAQMeRQIQriAIAcbTgP+wZQeC1y8avBQ/wA==\r\n"
	"-----END CERTIFICATE-----\r\n";
static int srv_cert_len = sizeof(srv_cert);

static unsigned char srv_key[] =
	"-----BEGIN EC PRIVATE KEY-----\r\n"
	"MHcCAQEEIDxkbi9CHy4EEjrqwaindd5NfyDbJBWBquaP/2MEg0p7oAoGCCqGSM49\r\n"
	"AwEHoUQDQgAE9H898pD806VIl/q3YTO3EMOWCwiMvc8CIS5C82tNCoBShJM1QKTs\r\n"
	"oc+QnRVQHnRXtcjIYkjYoXfmaGVbK3KvfQ==\r\n"
	"-----END EC PRIVATE KEY-----\r\n";
static int srv_key_len = sizeof(srv_key);

static unsigned char subca[] =
	"-----BEGIN CERTIFICATE-----\r\n"
	"MIICNjCCAd2gAwIBAgIBAzAKBggqhkjOPQQDAjCBjTELMAkGA1UEBhMCS1IxLjAs\r\n"
	"BgNVBAoMJVNhbXN1bmcgRWxlY3Ryb25pY3MgRGlnaXRhbCBBcHBsaWFuY2UxHDAa\r\n"
	"BgNVBAsME0RBIFRlc3QgT0NGIFJvb3QgQ0ExMDAuBgNVBAMMJ0RBIFRlc3QgU2Ft\r\n"
	"c3VuZyBFbGVjdHJvbmljcyBPQ0YgUm9vdCBDQTAgFw0yMTA4MTMwNDQ4MDlaGA8y\r\n"
	"MDY5MTIzMTA0NDgwOVowgZcxCzAJBgNVBAYTAktSMS4wLAYDVQQKDCVTYW1zdW5n\r\n"
	"IEVsZWN0cm9uaWNzIERpZ2l0YWwgQXBwbGlhbmNlMSEwHwYDVQQLDBhEQSBUZXN0\r\n"
	"IE9DRiBTZXJ2ZXIgU3ViQ0ExNTAzBgNVBAMMLERBIFRlc3QgU2Ftc3VuZyBFbGVj\r\n"
	"dHJvbmljcyBPQ0YgU2VydmVyIFN1YkNBMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcD\r\n"
	"QgAEhSxR60C1VQZwmNJUiyzD6z6Bqjzv0PuA2RbW/7HwCQDWWga6NLelKTz2vQOC\r\n"
	"Z1tfNm+OBKa4ZKXRvgtJsJszY6MgMB4wDwYDVR0TAQH/BAUwAwEB/zALBgNVHQ8E\r\n"
	"BAMCAsQwCgYIKoZIzj0EAwIDRwAwRAIgJIXw6mJe1GKKMmR2w4L9Qq7D6SqlRvk1\r\n"
	"h/U5irGB01gCIHkMoPlmskexdv1nYmeZV74K8SVdxdrK9fOHbX9Ogytg\r\n"
	"-----END CERTIFICATE-----\r\n";

static int subca_len = sizeof(subca);

static unsigned char rootca[] =
	"-----BEGIN CERTIFICATE-----\r\n"
	"MIICLTCCAdOgAwIBAgIBATAKBggqhkjOPQQDAjCBjTELMAkGA1UEBhMCS1IxLjAs\r\n"
	"BgNVBAoMJVNhbXN1bmcgRWxlY3Ryb25pY3MgRGlnaXRhbCBBcHBsaWFuY2UxHDAa\r\n"
	"BgNVBAsME0RBIFRlc3QgT0NGIFJvb3QgQ0ExMDAuBgNVBAMMJ0RBIFRlc3QgU2Ft\r\n"
	"c3VuZyBFbGVjdHJvbmljcyBPQ0YgUm9vdCBDQTAgFw0yMTA4MTMwNDIzMzBaGA8y\r\n"
	"MDY5MTIzMTA0MjMzMFowgY0xCzAJBgNVBAYTAktSMS4wLAYDVQQKDCVTYW1zdW5n\r\n"
	"IEVsZWN0cm9uaWNzIERpZ2l0YWwgQXBwbGlhbmNlMRwwGgYDVQQLDBNEQSBUZXN0\r\n"
	"IE9DRiBSb290IENBMTAwLgYDVQQDDCdEQSBUZXN0IFNhbXN1bmcgRWxlY3Ryb25p\r\n"
	"Y3MgT0NGIFJvb3QgQ0EwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAAS2H+PCRt/X\r\n"
	"7HDgY4inei2uCKsYTq5mcVsaumMHtXqNrs1LF6M0T73hcbjwdFvGhHRPYLBrJRZy\r\n"
	"UmQwhYTy0KKuoyAwHjAPBgNVHRMBAf8EBTADAQH/MAsGA1UdDwQEAwICxDAKBggq\r\n"
	"hkjOPQQDAgNIADBFAiBlSErIUCMyKg75TSXQt47WctpwO57cFy398AMl1b+RpAIh\r\n"
	"AOh+ajEBIKgHNSm6amXOCTBg40J97MBfJflm2DEHLP6v\r\n"
	"-----END CERTIFICATE-----\r\n";
static int rootca_len = sizeof(rootca);

int tls_handshake_server(void)
{
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
#if defined(MBEDTLS_SSL_CACHE_C)
	mbedtls_ssl_cache_context cache;
#endif

	mbedtls_net_init(&listen_fd);
	mbedtls_net_init(&client_fd);
	mbedtls_ssl_init(&ssl);
	mbedtls_ssl_config_init(&conf);
#if defined(MBEDTLS_SSL_CACHE_C)
	mbedtls_ssl_cache_init(&cache);
#endif
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
	mbedtls_printf("\n  . Loading the server cert. and key...");
	fflush(stdout);

	/*
     * This demonstration program uses embedded test certificates.
     * Instead, you may want to use mbedtls_x509_crt_parse_file() to read the
     * server and CA certificates, as well as mbedtls_pk_parse_keyfile().
     */

	ret = mbedtls_x509_crt_parse(&srvcert, (const unsigned char *)srv_cert,
								 srv_cert_len);
	if (ret != 0) {
		mbedtls_printf(" failed\n  !  mbedtls_x509_crt_parse returned %d\n\n", ret);
		goto exit;
	}

	ret = mbedtls_x509_crt_parse(&srvcert, (const unsigned char *)subca,
								 subca_len);
	if (ret != 0) {
		mbedtls_printf(" failed\n  !  mbedtls_x509_crt_parse returned %d\n\n", ret);
		goto exit;
	}

	ret = mbedtls_x509_crt_parse(&srvcert, (const unsigned char *)rootca,
								 rootca_len);
	if (ret != 0) {
		mbedtls_printf(" failed\n  !  mbedtls_x509_crt_parse returned %d\n\n", ret);
		goto exit;
	}

	ret = mbedtls_pk_parse_key(&pkey, (const unsigned char *)srv_key,
							   srv_key_len, NULL, 0);
	if (ret != 0) {
		mbedtls_printf(" failed\n  !  mbedtls_pk_parse_key returned %d\n\n", ret);
		goto exit;
	}
	mbedtls_printf(" ok\n");

	/*
     * 2. Setup the listening TCP socket
     */
	mbedtls_printf("  . Bind on https://localhost:4433/ ...");
	fflush(stdout);

	if ((ret = mbedtls_net_bind(&listen_fd, NULL, "4433", MBEDTLS_NET_PROTO_TCP)) != 0) {
		mbedtls_printf(" failed\n  ! mbedtls_net_bind returned %d\n\n", ret);
		goto exit;
	}

	mbedtls_printf(" ok\n");

	/*
     * 3. Seed the RNG
     */
	mbedtls_printf("  . Seeding the random number generator...");
	fflush(stdout);

	if ((ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
									 (const unsigned char *)pers,
									 strlen(pers))) != 0) {
		mbedtls_printf(" failed\n  ! mbedtls_ctr_drbg_seed returned %d\n", ret);
		goto exit;
	}

	mbedtls_printf(" ok\n");

	/*
     * 4. Setup stuff
     */
	mbedtls_printf("  . Setting up the SSL data....");
	fflush(stdout);

	if ((ret = mbedtls_ssl_config_defaults(&conf,
										   MBEDTLS_SSL_IS_SERVER,
										   MBEDTLS_SSL_TRANSPORT_STREAM,
										   MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
		mbedtls_printf(" failed\n  ! mbedtls_ssl_config_defaults returned %d\n\n", ret);
		goto exit;
	}

	mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);
	mbedtls_ssl_conf_dbg(&conf, my_debug, stdout);

#if defined(MBEDTLS_SSL_CACHE_C)
	mbedtls_ssl_conf_session_cache(&conf, &cache,
								   mbedtls_ssl_cache_get,
								   mbedtls_ssl_cache_set);
#endif

	mbedtls_ssl_conf_ca_chain(&conf, srvcert.next, NULL);
	if ((ret = mbedtls_ssl_conf_own_cert(&conf, &srvcert, &pkey)) != 0) {
		mbedtls_printf(" failed\n  ! mbedtls_ssl_conf_own_cert returned %d\n\n", ret);
		goto exit;
	}

	static mbedtls_ecp_group_id server_curves[2] = {
		MBEDTLS_ECP_DP_SECP256R1,
		MBEDTLS_ECP_DP_NONE};
	mbedtls_ssl_conf_curves(&conf, (mbedtls_ecp_group_id *)server_curves);

	int my_ciphersuite[2] = {MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_CCM, 0};
	mbedtls_ssl_conf_ciphersuites(&conf, my_ciphersuite);

	if ((ret = mbedtls_ssl_setup(&ssl, &conf)) != 0) {
		mbedtls_printf(" failed\n  ! mbedtls_ssl_setup returned %d\n\n", ret);
		goto exit;
	}

	mbedtls_printf(" ok\n");

reset:
#ifdef MBEDTLS_ERROR_C
	if (ret != 0) {
		char error_buf[100];
		mbedtls_strerror(ret, error_buf, 100);
		mbedtls_printf("Last error was: %d - %s\n\n", ret, error_buf);
	}
#endif

	mbedtls_net_free(&client_fd);

	mbedtls_ssl_session_reset(&ssl);

	/*
     * 3. Wait until a client connects
     */
	mbedtls_printf("  . Waiting for a remote connection ...");
	fflush(stdout);

	if ((ret = mbedtls_net_accept(&listen_fd, &client_fd,
								  NULL, 0, NULL)) != 0) {
		mbedtls_printf(" failed\n  ! mbedtls_net_accept returned %d\n\n", ret);
		goto exit;
	}

	mbedtls_ssl_set_bio(&ssl, &client_fd, mbedtls_net_send, mbedtls_net_recv, NULL);

	mbedtls_printf(" ok\n");

	/*
     * 5. Handshake
     */
	mbedtls_printf("  . Performing the SSL/TLS handshake...");
	fflush(stdout);

	while ((ret = mbedtls_ssl_handshake(&ssl)) != 0) {
		if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
			mbedtls_printf(" failed\n  ! mbedtls_ssl_handshake returned %d\n\n", ret);
			goto reset;
		}
	}

	mbedtls_printf(" ok\n");

	/*
     * 6. Read the HTTP Request
     */
	mbedtls_printf("  < Read from client:");
	fflush(stdout);

	do {
		len = sizeof(buf) - 1;
		memset(buf, 0, sizeof(buf));
		ret = mbedtls_ssl_read(&ssl, buf, len);

		if (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE)
			continue;

		if (ret <= 0) {
			switch (ret) {
			case MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY:
				mbedtls_printf(" connection was closed gracefully\n");
				break;

			case MBEDTLS_ERR_NET_CONN_RESET:
				mbedtls_printf(" connection was reset by peer\n");
				break;

			default:
				mbedtls_printf(" mbedtls_ssl_read returned -0x%x\n", (unsigned int)-ret);
				break;
			}

			break;
		}

		len = ret;
		mbedtls_printf(" %d bytes read\n\n%s", len, (char *)buf);

		if (ret > 0)
			break;
	} while (1);

	/*
     * 7. Write the 200 Response
     */
	mbedtls_printf("  > Write to client:");
	fflush(stdout);

	len = sprintf((char *)buf, HTTP_RESPONSE,
				  mbedtls_ssl_get_ciphersuite(&ssl));

	while ((ret = mbedtls_ssl_write(&ssl, buf, len)) <= 0) {
		if (ret == MBEDTLS_ERR_NET_CONN_RESET) {
			mbedtls_printf(" failed\n  ! peer closed the connection\n\n");
			goto reset;
		}

		if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
			mbedtls_printf(" failed\n  ! mbedtls_ssl_write returned %d\n\n", ret);
			goto exit;
		}
	}

	len = ret;
	mbedtls_printf(" %d bytes written\n\n%s\n", len, (char *)buf);

	/*  7-1 write 1MiB */
	unsigned int remain = 1024 * 1024;
	memset(buf, 0x01, sizeof(buf));
	while (remain) {
		size_t buf_size = remain > sizeof(buf) ? sizeof(buf) : remain;
		mbedtls_printf("[pkbuild] write %ld size\n", buf_size);
		ret = mbedtls_ssl_write(&ssl, buf, buf_size);
		if (ret == MBEDTLS_ERR_NET_CONN_RESET) {
			mbedtls_printf(" failed\n  ! peer closed the connection\n\n");
			goto reset;
		}
		if (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE) {
			continue;
		}
		if (ret < 0) {
			mbedtls_printf(" failed\n  ! mbedtls_ssl_write returned %d\n\n", ret);
			goto reset;
		}

		mbedtls_printf(" %d bytes sent\n", ret);
		remain -= ret;
	}

	mbedtls_printf("  . Closing the connection...");

	while ((ret = mbedtls_ssl_close_notify(&ssl)) < 0) {
		if (ret != MBEDTLS_ERR_SSL_WANT_READ &&
			ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
			mbedtls_printf(" failed\n  ! mbedtls_ssl_close_notify returned %d\n\n", ret);
			goto reset;
		}
	}

	mbedtls_printf(" ok\n");

	ret = 0;
	goto reset;

exit:

#ifdef MBEDTLS_ERROR_C
	if (ret != 0) {
		char error_buf[100];
		mbedtls_strerror(ret, error_buf, 100);
		mbedtls_printf("Last error was: %d - %s\n\n", ret, error_buf);
	}
#endif

	mbedtls_net_free(&client_fd);
	mbedtls_net_free(&listen_fd);

	mbedtls_x509_crt_free(&srvcert);
	mbedtls_pk_free(&pkey);
	mbedtls_ssl_free(&ssl);
	mbedtls_ssl_config_free(&conf);
#if defined(MBEDTLS_SSL_CACHE_C)
	mbedtls_ssl_cache_free(&cache);
#endif
	mbedtls_ctr_drbg_free(&ctr_drbg);
	mbedtls_entropy_free(&entropy);

#if defined(_WIN32)
	mbedtls_printf("  Press Enter to exit this program.\n");
	fflush(stdout);
	getchar();
#endif

	mbedtls_exit(ret);
}
#endif /* MBEDTLS_BIGNUM_C && MBEDTLS_CERTS_C && MBEDTLS_ENTROPY_C &&     \
		  MBEDTLS_SSL_TLS_C && MBEDTLS_SSL_SRV_C && MBEDTLS_NET_C &&      \
		  MBEDTLS_RSA_C && MBEDTLS_CTR_DRBG_C && MBEDTLS_X509_CRT_PARSE_C \
		  && MBEDTLS_FS_IO && MBEDTLS_PEM_PARSE_C */
