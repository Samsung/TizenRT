/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License\n");
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
#include <stdlib.h>
#include <sys/types.h>
#include "connect_test_log.h"
#include "connect_test_data.h"
#include "mbedtls/config.h"
#include "mbedtls/net.h"
#include "mbedtls/debug.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "mbedtls/certs.h"
#include "mbedtls/timing.h"

#define TAG "[CT_DTLS_CLIENT]"
#define DEBUG_LEVEL 0
#define READ_TIMEOUT_MS 30000

extern unsigned char g_mac_buf_client[MAC_BUF_SIZE];
extern unsigned char g_recv_mac_buf_client[MAC_BUF_SIZE];

static unsigned char rootca[] =
"-----BEGIN CERTIFICATE-----\r\n"
"MIICUjCCAdegAwIBAgIJAMFD4n5iQ8zoMAoGCCqGSM49BAMCMD4xCzAJBgNVBAYT\r\n"
"Ak5MMREwDwYDVQQKEwhQb2xhclNTTDEcMBoGA1UEAxMTUG9sYXJzc2wgVGVzdCBF\r\n"
"QyBDQTAeFw0xMzA5MjQxNTQ5NDhaFw0yMzA5MjIxNTQ5NDhaMD4xCzAJBgNVBAYT\r\n"
"Ak5MMREwDwYDVQQKEwhQb2xhclNTTDEcMBoGA1UEAxMTUG9sYXJzc2wgVGVzdCBF\r\n"
"QyBDQTB2MBAGByqGSM49AgEGBSuBBAAiA2IABMPaKzRBN1gvh1b+/Im6KUNLTuBu\r\n"
"ww5XUzM5WNRStJGVOQsj318XJGJI/BqVKc4sLYfCiFKAr9ZqqyHduNMcbli4yuiy\r\n"
"aY7zQa0pw7RfdadHb9UZKVVpmlM7ILRmFmAzHqOBoDCBnTAdBgNVHQ4EFgQUnW0g\r\n"
"JEkBPyvLeLUZvH4kydv7NnwwbgYDVR0jBGcwZYAUnW0gJEkBPyvLeLUZvH4kydv7\r\n"
"NnyhQqRAMD4xCzAJBgNVBAYTAk5MMREwDwYDVQQKEwhQb2xhclNTTDEcMBoGA1UE\r\n"
"AxMTUG9sYXJzc2wgVGVzdCBFQyBDQYIJAMFD4n5iQ8zoMAwGA1UdEwQFMAMBAf8w\r\n"
"CgYIKoZIzj0EAwIDaQAwZgIxAMO0YnNWKJUAfXgSJtJxexn4ipg+kv4znuR50v56\r\n"
"t4d0PCu412mUC6Nnd7izvtE2MgIxAP1nnJQjZ8BWukszFQDG48wxCCyci9qpdSMv\r\n"
"uCjn8pwUOkABXK8Mss90fzCfCEOtIA==\r\n"
"-----END CERTIFICATE-----\r\n";
static int rootca_len = sizeof(rootca);

static unsigned char buf[BUF_SIZE];

static void my_debug(void* ctx, int level, const char* file, int line, const char* str)
{
	((void)ctx);
	((void)level);
	printf("%s:%04d: %s", file, line, str);
	fflush((FILE*)ctx);
}

int _dtls_client(char* server_addr, int msg_size)
{
	int ret = 0;
	uint32_t flags;
	const char* pers = "dtls_client";

	mbedtls_net_context server_fd;
	mbedtls_entropy_context entropy;
	mbedtls_ctr_drbg_context ctr_drbg;
	mbedtls_ssl_context ssl;
	mbedtls_ssl_config conf;
	mbedtls_x509_crt cacert;
	mbedtls_timing_delay_context timer;
#if defined(MBEDTLS_DEBUG_C)
	mbedtls_debug_set_threshold(DEBUG_LEVEL);
#endif

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
		CT_LOGE(TAG, "failed ! mbedtls_ctr_drbg_seed returned %d", ret);
		goto exit;
	}

	CT_LOG(TAG, "Seeding the random number generator - ok");

	/* 2. Load certificates */
	CT_LOG(TAG, "Loading the CA root certificate ...");
	fflush(stdout);

	ret = mbedtls_x509_crt_parse(&cacert, (const unsigned char*)rootca, rootca_len);
	if (ret < 0) {
		CT_LOG(TAG, "failed ! mbedtls_x509_crt_parse returned -0x%x", -ret);
		goto exit;
	}

	CT_LOG(TAG, "Loading the CA root certificate - ok (%d skipped)", ret);

	/* 3. Start the connection */
	CT_LOG(TAG, "Connecting to udp/%s/%s...", server_addr, CT_TLS_DTLS_PORT);
	fflush(stdout);

	if ((ret = mbedtls_net_connect(&server_fd, server_addr,
		CT_TLS_DTLS_PORT, MBEDTLS_NET_PROTO_UDP)) != 0) {
		CT_LOG(TAG, "failed ! mbedtls_net_connect returned %d", ret);
		goto exit;
	}
	CT_LOG(TAG, "Connecting to udp/%s/%s - ok", server_addr, CT_TLS_DTLS_PORT);

	/*4. Setup stuff */
	CT_LOG(TAG, "Setting up the DTLS structure...");
	fflush(stdout);

	if ((ret = mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT,
		MBEDTLS_SSL_TRANSPORT_DATAGRAM, MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
		CT_LOG(TAG, " failed ! mbedtls_ssl_config_defaults returned %d", ret);
		goto exit;
	}

	mbedtls_ssl_conf_authmode(&conf, MBEDTLS_SSL_VERIFY_REQUIRED);
	mbedtls_ssl_conf_ca_chain(&conf, &cacert, NULL);
	mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);
	mbedtls_ssl_conf_dbg(&conf, my_debug, stdout);

	mbedtls_ssl_conf_read_timeout(&conf, READ_TIMEOUT_MS);
	if ((ret = mbedtls_ssl_setup(&ssl, &conf)) != 0) {
		CT_LOG(TAG, " failed ! mbedtls_ssl_setup returned %d", ret);
		goto exit;
	}

	if ((ret = mbedtls_ssl_set_hostname(&ssl, "localhost")) != 0) {
		CT_LOG(TAG, " failed ! mbedtls_ssl_set_hostname returned %d", ret);
		goto exit;
	}

	mbedtls_ssl_set_bio(&ssl, &server_fd, mbedtls_net_send, mbedtls_net_recv, mbedtls_net_recv_timeout);
	mbedtls_ssl_set_timer_cb(&ssl, &timer, mbedtls_timing_set_delay, mbedtls_timing_get_delay);

	CT_LOG(TAG, "Setting up the DTLS structure - ok");

	/* 5. Handshake */
	CT_LOG(TAG, "Performing the DTLS handshake...");
	fflush(stdout);

	do {
		ret = mbedtls_ssl_handshake(&ssl);
	} while (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE);

	if (ret != 0) {
		CT_LOG(TAG, "failed ! mbedtls_ssl_handshake returned -0x%x", -ret);
		goto exit;
	}
	CT_LOG(TAG, "Performing the DTLS handshake - ok");

	/* 6. Verify the server certificate */
	CT_LOG(TAG, "Verifying peer X.509 certificate...");
	if ((flags = mbedtls_ssl_get_verify_result(&ssl)) != 0) {
		char vrfy_buf[512];
		CT_LOG(TAG, "failed");
		mbedtls_x509_crt_verify_info(vrfy_buf, sizeof(vrfy_buf), "  ! ", flags);
		CT_LOG(TAG, "%s", vrfy_buf);
	}
	else {
		CT_LOG(TAG, "Verifying peer X.509 certificate - ok");
	}

	CT_LOG(TAG, "Sending Device MAC to peer. mac %s", g_mac_buf_client);
	do {
		ret = mbedtls_ssl_write(&ssl, g_mac_buf_client, MAC_BUF_SIZE * sizeof(char));
	} while (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE);

	if (ret < 0) {
		CT_LOG(TAG, "failed ! mbedtls_ssl_write returned %d", ret);
		goto exit;
	}

	CT_LOG(TAG, "Receiving Device MAC from peer:");
	do {
		ret = mbedtls_ssl_read(&ssl, g_recv_mac_buf_client, MAC_BUF_SIZE * sizeof(char));
	} while (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE);

	if (ret <= 0) {
		switch (ret) {
		case MBEDTLS_ERR_SSL_TIMEOUT:
			CT_LOG(TAG, "timeout");
			goto exit;
		case MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY:
			CT_LOG(TAG, "connection was closed gracefully");
			goto exit;
		case MBEDTLS_ERR_NET_CONN_RESET:
			CT_LOG(TAG, "connection was reset by peer");
			goto exit;
		default:
			CT_LOG(TAG, "mbedtls_ssl_read returned -0x%x", (unsigned int)-ret);
			goto exit;
		}
	}
	CT_LOG(TAG, "Peer MAC is: %s", g_recv_mac_buf_client);

	/*7. Write the echo request */
	CT_LOG(TAG, "	> WriteToServer:");
	fflush(stdout);
	int remain = msg_size, ack = 0;
	CT_LOG(TAG, "_dtls_send_data start");	
	while (remain > 0) {
		int size = remain > BUF_SIZE ? BUF_SIZE : remain;
		memset(buf, '\0', BUF_SIZE);
		do {
			ret = mbedtls_ssl_write(&ssl, buf, size);
		} while (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE);

		if (ret < 0) {
			CT_LOG(TAG, "failed ! mbedtls_ssl_write returned %d", ret);
			CT_LOG(TAG, "TOTAL_ACK_RECVD_FOR_SENT_%d", ack);
			goto exit;
		}		

		do {
			ret = mbedtls_ssl_read(&ssl, buf, size);
		} while (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE);

		if (ret <= 0) {
			switch (ret) {
			case MBEDTLS_ERR_SSL_TIMEOUT:
				CT_LOG(TAG, "timeout");
				CT_LOG(TAG, "TOTAL_ACK_RECVD_FOR_SENT_%d", ack);
				goto exit;
			case MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY:
				CT_LOG(TAG, "connection was closed gracefully");
				CT_LOG(TAG, "TOTAL_ACK_RECVD_FOR_SENT_%d", ack);
				goto exit;
			case MBEDTLS_ERR_NET_CONN_RESET:
				CT_LOG(TAG, "connection was reset by peer");
				CT_LOG(TAG, "TOTAL_ACK_RECVD_FOR_SENT_%d", ack);
				goto exit;
			default:
				CT_LOG(TAG, "mbedtls_ssl_read returned -0x%x", (unsigned int)-ret);
				CT_LOG(TAG, "TOTAL_ACK_RECVD_FOR_SENT_%d", ack);
				goto exit;
			}
		}
		ack = atoi(buf);
		remain = msg_size - ack;		
	}	
	CT_LOG(TAG, "TOTAL_ACK_RECVD_FOR_SENT_%d", ack);

	/*8. Read the echo Request */
	CT_LOG(TAG, "  < ReadFromServer:");
	fflush(stdout);	
	int done = 0;
	remain = msg_size;
	CT_LOG(TAG, "_dtls_recv_data start");	
	while (remain > 0) {
		int size = remain > BUF_SIZE ? BUF_SIZE : remain;
		do {
			ret = mbedtls_ssl_read(&ssl, buf, size);
		} while (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE);

		if (ret <= 0) {
			switch (ret) {
			case MBEDTLS_ERR_SSL_TIMEOUT:
				CT_LOG(TAG, "timeout");
				CT_LOG(TAG, "TOTAL_ACK_SENT_FOR_RECVD:%d", done);
				goto exit;
			case MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY:
				CT_LOG(TAG, "connection was closed gracefully");
				CT_LOG(TAG, "TOTAL_ACK_SENT_FOR_RECVD:%d", done);
				goto exit;
			case MBEDTLS_ERR_NET_CONN_RESET:
				CT_LOG(TAG, "connection was reset by peer");
				CT_LOG(TAG, "TOTAL_ACK_SENT_FOR_RECVD:%d", done);
				goto exit;
			default:
				CT_LOG(TAG, "mbedtls_ssl_read returned -0x%x", (unsigned int)-ret);
				CT_LOG(TAG, "TOTAL_ACK_SENT_FOR_RECVD:%d", done);
				goto exit;
			}
		}
		done += ret;
		remain -= ret;

		memset(buf, '\0', BUF_SIZE);
		snprintf(buf, BUF_SIZE, "%d", done);
		do {
			ret = mbedtls_ssl_write(&ssl, buf, size);
		} while (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE);

		if (ret < 0) {
			CT_LOG(TAG, "failed ! mbedtls_ssl_write returned %d", ret);
			CT_LOG(TAG, "TOTAL_ACK_SENT_FOR_RECVD:%d", done);
			goto exit;
		}		
	}	
	CT_LOG(TAG, "TOTAL_ACK_SENT_FOR_RECVD:%d", done);

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
