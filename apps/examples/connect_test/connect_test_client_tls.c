/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
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
#include <time.h>
#include <stdlib.h>
#include "connect_test_log.h"
#include "connect_test_data.h"
#include "mbedtls/config.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/debug.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "mbedtls/certs.h"

#define TAG "[CT_TLS_CLIENT]"
#define DEBUG_LEVEL 0
#define READ_TIMEOUT_MS 30000

static int data_transferred = 0;
static int client_read_write_data = 0;
extern bool g_tls_client_recvd_data;
extern bool g_tls_client_sent_data;

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

static char* SERVER_ADDR = NULL;
static unsigned char buf[BUF_SIZE];

static void my_debug(void* ctx, int level, const char* file, int line, const char* str)
{
	((void)ctx);
	((void)level);
	CT_LOGE(TAG, "%s:%04d: %s", file, line, str);
	fflush((FILE*)ctx);
}

int _tls_client(char* ipaddr, int msg_size)
{
	SERVER_ADDR = ipaddr;
	uint32_t flags;
	const char* pers = "ssl_client";
	int ret = 1;	

	mbedtls_net_context server_fd;
	mbedtls_entropy_context entropy;
	mbedtls_ctr_drbg_context ctr_drbg;
	mbedtls_ssl_context ssl;
	mbedtls_ssl_config conf;
	mbedtls_x509_crt cacert;
#if defined(MBEDTLS_DEBUG_C)
	mbedtls_debug_set_threshold(DEBUG_LEVEL);
#endif

	int retrycount = 0;
retry_connection_tls:
	/* 1. Initialize the RNG and the session data */
	mbedtls_net_init(&server_fd);
	mbedtls_ssl_init(&ssl);
	mbedtls_ssl_config_init(&conf);
	mbedtls_x509_crt_init(&cacert);
	mbedtls_ctr_drbg_init(&ctr_drbg);

	CT_LOG(TAG, "Seeding the random number generator...");
	fflush(stdout);

	mbedtls_entropy_init(&entropy);
	if ((ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func,
		&entropy, (const unsigned char*)pers, strlen(pers))) != 0) {
		CT_LOG(TAG, " failed ! mbedtls_ctr_drbg_seed returned %d", ret);
		goto exit;
	}

	CT_LOG(TAG, "Seeding the random number generator - ok");

	/* 2. Initialize certificates */
	CT_LOG(TAG, "Loading the CA root certificate ...");
	fflush(stdout);

	ret = mbedtls_x509_crt_parse(&cacert, (const unsigned char*)rootca, rootca_len);
	if (ret < 0) {
		CT_LOGE(TAG, "failed !	mbedtls_x509_crt_parse returned -0x%x", (unsigned int)-ret);
		goto exit;
	}

	CT_LOG(TAG, "Loading the CA root certificate - ok (%d skipped)", ret);

	/* 3. Start the connection */
	CT_LOG(TAG, "Connecting to tls_server/%s/%s...", SERVER_ADDR, CT_TLS_DTLS_PORT);
	fflush(stdout);

	bool is_connected = false;
	retrycount++;

	if ((ret = mbedtls_net_connect(&server_fd, SERVER_ADDR,
		CT_TLS_DTLS_PORT, MBEDTLS_NET_PROTO_TCP)) != 0) {
		CT_LOGE(TAG, "failed ! mbedtls_net_connect returned %d", ret);			
	}
	else {
		CT_LOG(TAG, "connection OK");
		is_connected = true;
	}

	if (!is_connected) {
		if (retrycount <= 5) {
			CT_LOGE(TAG, "connect try again");
			mbedtls_net_free(&server_fd);
			mbedtls_ssl_free(&ssl);
			mbedtls_ssl_config_free(&conf);
			mbedtls_x509_crt_free(&cacert);
			mbedtls_ctr_drbg_free(&ctr_drbg);
			mbedtls_entropy_free(&entropy);
			sleep(3);
			goto retry_connection_tls;
		}
		else {
			CT_LOGE(TAG, "connect retry failed even after trying for 5 times");
			return ret;
		}
	}

	CT_LOG(TAG, "Connecting to tls_server/%s/%s - ok", SERVER_ADDR, CT_TLS_DTLS_PORT);

	/* 4. Setup stuff */
	CT_LOG(TAG, "Setting up the SSL/TLS structure...");
	fflush(stdout);

	if ((ret = mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT,
		MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
		CT_LOGE(TAG, " failed ! mbedtls_ssl_config_defaults returned %d", ret);
		goto exit;
	}

	mbedtls_ssl_conf_authmode(&conf, MBEDTLS_SSL_VERIFY_REQUIRED);
	mbedtls_ssl_conf_ca_chain(&conf, &cacert, NULL);
	mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);
	mbedtls_ssl_conf_dbg(&conf, my_debug, stdout);

	mbedtls_ssl_conf_read_timeout(&conf, READ_TIMEOUT_MS);

	if ((ret = mbedtls_ssl_setup(&ssl, &conf)) != 0) {
		CT_LOGE(TAG, " failed ! mbedtls_ssl_setup returned %d", ret);
		goto exit;
	}

	mbedtls_ssl_set_bio(&ssl, &server_fd, mbedtls_net_send, mbedtls_net_recv, mbedtls_net_recv_timeout);

	CT_LOG(TAG, " Setting up the SSL/TLS structure - ok");

	/*5. Handshake */
	CT_LOG(TAG, "Performing the SSL/TLS handshake...");
	fflush(stdout);

	while ((ret = mbedtls_ssl_handshake(&ssl)) != 0) {
		if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
			CT_LOGE(TAG, " failed ! mbedtls_ssl_handshake returned -0x%x", (unsigned int)-ret);
			goto exit;
		}
	}
	CT_LOG(TAG, "Performing the SSL/TLS handshake - ok");

	/* 6. Verify the server certificate */
	CT_LOG(TAG, "Verifying peer X.509 certificate...");
	if ((flags = mbedtls_ssl_get_verify_result(&ssl)) != 0) {
		char vrfy_buf[512];
		CT_LOGE(TAG, " failed\n");
		mbedtls_x509_crt_verify_info(vrfy_buf, sizeof(vrfy_buf), " ! ", flags);
		CT_LOGE(TAG, "%s", vrfy_buf);
	}
	else
		CT_LOG(TAG, "Verifying peer X.509 certificate - ok");

	/* 7. Write the GET request */
	CT_LOG(TAG, "	> Write to server:");
	fflush(stdout);

	// Get how much data received by Server before connection break
	do {
		ret = mbedtls_ssl_read(&ssl, buf, 10);
	} while (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE);
	if (ret < 0) {
		CT_LOG(TAG, "failed ! mbedtls_ssl_write returned %d", ret);
		goto exit;
	}
	client_read_write_data = client_read_write_data + atoi(buf);	
	int remain = msg_size - client_read_write_data;
	int cnt = 1;
	int total_sent = 0;

	CT_LOG(TAG, "_tls_send_data start");
	if (!g_tls_client_sent_data) {
		while (remain > 0) {
			int send_size = remain > BUF_SIZE ? BUF_SIZE : remain;
			do {
				ret = mbedtls_ssl_write(&ssl, buf, send_size);
			} while (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE);

			if (ret < 0) {
				CT_LOGE(TAG, "failed ! mbedtls_ssl_write returned %d", ret);				
				CT_LOGE(TAG, "Sent_Till_Now: %d", client_read_write_data);
				goto exit;
			}
			total_sent += ret;
			remain -= ret;
			cnt++;
		}		
		CT_LOG(TAG, "TOTAL_BYTES_SEND: %d", client_read_write_data + total_sent);
		g_tls_client_sent_data = true;
		client_read_write_data = 0;		
	}

	/* 8. Read the HTTP response */
	CT_LOG(TAG, "   < Read from server:");
	fflush(stdout);

	// Send how much data received by Client before connection break
	do {
		memset(buf, '\0', 10);
		snprintf(buf, 10, "%d", data_transferred);
		ret = mbedtls_ssl_write(&ssl, buf, 10);
	} while (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE);
	
	if (ret < 0) {
		CT_LOGE(TAG, "failed ! mbedtls_ssl_write returned %d", ret);
		goto exit;
	}

	client_read_write_data = client_read_write_data + data_transferred;		
	remain = msg_size - client_read_write_data;
	cnt = 1;
	int total_received = 0;	
	CT_LOG(TAG, "_tls_recv_data start");	
	if (!g_tls_client_recvd_data) {
		while (remain > 0) {
			int read_size = remain > BUF_SIZE ? BUF_SIZE : remain;
			do {
				ret = mbedtls_ssl_read(&ssl, buf, read_size);
			} while (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE);

			if (ret <= 0) {
				switch (ret) {
				case MBEDTLS_ERR_SSL_TIMEOUT:
					CT_LOG(TAG, " timeout");
					data_transferred = total_received;
					CT_LOG(TAG, "Rcvd_Till_Now: %d", client_read_write_data);
					goto exit;
				case MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY:
					CT_LOG(TAG, " connection was closed gracefully");
					data_transferred = total_received;
					CT_LOG(TAG, "Rcvd_Till_Now: %d", client_read_write_data);
					goto exit;
				case MBEDTLS_ERR_NET_CONN_RESET:
					CT_LOG(TAG, " connection was reset by peer");
					data_transferred = total_received;
					CT_LOG(TAG, "Rcvd_Till_Now: %d", client_read_write_data);
					goto exit;
				default:
					CT_LOG(TAG, " mbedtls_ssl_read returned -0x%x", (unsigned int)-ret);
					data_transferred = total_received;
					CT_LOG(TAG, "Rcvd_Till_Now: %d", client_read_write_data);
					goto exit;
				}
			}
			total_received += ret;
			remain -= ret;
			cnt++;
		}		
		CT_LOG(TAG, "TOTAL_BYTES_RECEIVED: %d", client_read_write_data + total_received);
		g_tls_client_recvd_data = true;
		client_read_write_data = 0;
		data_transferred = 0;
	}

	/* 9. Done, cleanly close the connection */
	CT_LOG(TAG, "Closing the connection...");
	ret = mbedtls_ssl_close_notify(&ssl);
	CT_LOG(TAG, "Closing the connection - ok");

	/*10. Final clean-ups and exit */
exit:
#ifdef MBEDTLS_ERROR_C
	if (ret != 0) {
		char error_buf[100];
		mbedtls_strerror(ret, error_buf, 100);
		CT_LOG(TAG, "Last error was: %d - %s", ret, error_buf);
	}
#endif

	mbedtls_net_free(&server_fd);
	mbedtls_x509_crt_free(&cacert);
	mbedtls_ssl_free(&ssl);
	mbedtls_ssl_config_free(&conf);
	mbedtls_ctr_drbg_free(&ctr_drbg);
	mbedtls_entropy_free(&entropy);
	return ret;
}
