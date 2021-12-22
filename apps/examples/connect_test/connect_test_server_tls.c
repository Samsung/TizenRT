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
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include "connect_test_server.h"
#include "connect_test_log.h"
#include "connect_test_data.h"
#include "mbedtls/config.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/debug.h"
#include "mbedtls/certs.h"
#include "mbedtls/x509.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/error.h"
#if defined(MBEDTLS_SSL_CACHE_C)
#include "mbedtls/ssl_cache.h"
#endif

#define TAG "[CT_TLS_SERVER]"
#define DEBUG_LEVEL 0
#define READ_TIMEOUT_MS 30000

extern bool g_tls_server_recvd_data;
extern bool g_tls_server_sent_data;

static int data_transferred = 0;
static int server_read_write_data = 0;
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

static unsigned char buf[BUF_SIZE];
extern volatile bool g_broadcast_flag;

static void my_debug(void *ctx, int level, const char *file, int line, const char *str)
{
	((void)ctx);
	((void)level);
	CT_LOG(TAG, "%s:%04d: %s", file, line, str);
	fflush((FILE *)ctx);
}

int _tls_server(int msg_size, long *sum)
{
	int ret = 0;
	struct timeval start, end;
	const char *pers = "ssl_server";
	
	mbedtls_net_context listen_fd, client_fd;
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

	/* 1. Load the certificates and private RSA key */
	CT_LOG(TAG, "Loading the server cert. and key...");
	fflush(stdout);
	
	if ((ret = mbedtls_x509_crt_parse(&srvcert, (const unsigned char *)srv_cert, 
							      srv_cert_len)) != 0) {
		CT_LOGE(TAG, "failed ! mbedtls_x509_crt_parse(srv_cert) returned %d", ret);
		goto exit;
	}

	if ((ret = mbedtls_x509_crt_parse(&srvcert, (const unsigned char *)subca, 
							      subca_len)) != 0) {
		CT_LOGE(TAG, "failed ! mbedtls_x509_crt_parse(subca) returned %d", ret);
		goto exit;
	}

	if ((ret = mbedtls_x509_crt_parse(&srvcert, (const unsigned char *)rootca, 
							      rootca_len)) != 0) {
		CT_LOGE(TAG, "failed ! mbedtls_x509_crt_parse(rootca) returned %d", ret);
		goto exit;
	}

	if ((ret = mbedtls_pk_parse_key(&pkey, (const unsigned char *)srv_key, srv_key_len,
									  NULL, 0)) != 0) {
		CT_LOGE(TAG, "failed ! mbedtls_pk_parse_key returned %d", ret);
		goto exit;
	}

	CT_LOG(TAG, "Loading the server cert. and key - ok");

	/* 2. Setup the listening TCP socket */
	CT_LOG(TAG, "Bind on https://localhost:%s/ ...", CT_TLS_DTLS_PORT);
	fflush(stdout);

	if ((ret = mbedtls_net_bind(&listen_fd, NULL, CT_TLS_DTLS_PORT, MBEDTLS_NET_PROTO_TCP)) != 0) {
		CT_LOGE(TAG, "failed ! mbedtls_net_bind returned %d", ret);
		goto exit;
	}

	CT_LOG(TAG, "Bind on https://localhost:%s/ - ok", CT_TLS_DTLS_PORT);

	/* 3. Seed the RNG */
	CT_LOG(TAG, "Seeding the random number generator...");
	fflush(stdout);

	if ((ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
			        (const unsigned char *)pers, strlen(pers))) != 0) {
		CT_LOGE(TAG, "failed ! mbedtls_ctr_drbg_seed returned %d", ret);
		goto exit;
	}

	CT_LOG(TAG, "Seeding the random number generator - ok");

	/* 4. Setup stuff */
	CT_LOG(TAG, "Setting up the SSL data....");
	fflush(stdout);

	if ((ret = mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_SERVER,
	  MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
		CT_LOGE(TAG, "failed ! mbedtls_ssl_config_defaults returned %d", ret);
		goto exit;
	}

	mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);
	mbedtls_ssl_conf_dbg(&conf, my_debug, stdout);

#if defined(MBEDTLS_SSL_CACHE_C)
	mbedtls_ssl_conf_session_cache(&conf, &cache, mbedtls_ssl_cache_get, mbedtls_ssl_cache_set);
#endif

	mbedtls_ssl_conf_ca_chain(&conf, srvcert.next, NULL);
	if ((ret = mbedtls_ssl_conf_own_cert(&conf, &srvcert, &pkey)) != 0) {
		CT_LOGE(TAG, "failed ! mbedtls_ssl_conf_own_cert returned %d", ret);
		goto exit;
	}

	static mbedtls_ecp_group_id server_curves[2] = { MBEDTLS_ECP_DP_SECP256R1, 
							     MBEDTLS_ECP_DP_NONE};
	mbedtls_ssl_conf_curves(&conf, (mbedtls_ecp_group_id *)server_curves);

	int my_ciphersuite[2] = {MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_CCM, 0};
	mbedtls_ssl_conf_ciphersuites(&conf, my_ciphersuite);

	mbedtls_ssl_conf_read_timeout(&conf, READ_TIMEOUT_MS);

	if ((ret = mbedtls_ssl_setup(&ssl, &conf)) != 0) {
		CT_LOGE(TAG, " failed  ! mbedtls_ssl_setup returned %d", ret);
		goto exit;
	}

	CT_LOG(TAG, "Setting up the SSL data - ok");

	/* 5. Wait until a client connects */
	CT_LOG(TAG, "Waiting for a remote connection ...");
	fflush(stdout);

	if ((ret = mbedtls_net_accept(&listen_fd, &client_fd, NULL, 0, NULL)) != 0) {
		CT_LOGE(TAG, "failed  ! mbedtls_net_accept returned %d", ret);
		goto exit;
	}

	g_broadcast_flag = false;

	mbedtls_ssl_set_bio(&ssl, &client_fd, mbedtls_net_send, mbedtls_net_recv, 
						       mbedtls_net_recv_timeout);

	CT_LOG(TAG, "Waiting for a remote connection - ok");

	/* 6. Handshake */
	CT_LOG(TAG, "Performing the SSL/TLS handshake...");
	fflush(stdout);

	SET_START_TIME(TLS handshake);
	while ((ret = mbedtls_ssl_handshake(&ssl)) != 0) {
		if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
			CT_LOGE(TAG, " failed  ! mbedtls_ssl_handshake returned %d", ret);
			goto exit;
		}
	}

	CT_LOG(TAG, "Performing the SSL/TLS handshake - ok");
	CALCULATE_ELAPSED_TIME(TLS handshake, *sum);

	SET_START_TIME(TLS Data transfer);
	
	/* 7. Read the HTTP Request */
	CT_LOG(TAG, "  < Read from client:");
	fflush(stdout);
	// Send how much data received by Server before connection break;	
	do {
		memset(buf, '\0', 10);
		snprintf(buf, 10, "%d", data_transferred);
		ret = mbedtls_ssl_write(&ssl, buf, 10);
	} while (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE);
	if (ret < 0) {
		CT_LOGE(TAG, "failed ! mbedtls_ssl_write returned %d", ret);
		goto exit;
	}

	server_read_write_data = server_read_write_data + data_transferred;		
	int remain = msg_size - server_read_write_data;	
	int cnt = 1;
	int total_received = 0;
	CT_LOG(TAG, "_tls_recv_data start");	
	if (!g_tls_server_recvd_data) {
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
					CT_LOG(TAG, "Rcvd_Till_Now:_%d", server_read_write_data);
					goto exit;
				case MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY:
					CT_LOG(TAG, " connection was closed gracefully");	
					data_transferred = total_received;
					CT_LOG(TAG, "Rcvd_Till_Now:_%d", server_read_write_data);
					goto exit;
				case MBEDTLS_ERR_NET_CONN_RESET:
					CT_LOG(TAG, " connection was reset by peer");
					data_transferred = total_received;
					CT_LOG(TAG, "Rcvd_Till_Now:_%d", server_read_write_data);
					goto exit;
				default:
					CT_LOG(TAG, " mbedtls_ssl_read returned -0x%x", (unsigned int)-ret);
					data_transferred = total_received;
					CT_LOG(TAG, "Rcvd_Till_Now:_%d", server_read_write_data);
					goto exit;
				}
			}
			total_received += ret;
			remain -= ret;			
			cnt++;
		}		
		CT_LOG(TAG, "TOTAL_BYTES_RECEIVED:_%d", server_read_write_data + total_received);
		g_tls_server_recvd_data = true;
		server_read_write_data = 0;
		data_transferred = 0;		
	}	

	/* 8. Write the 200 Response*/
	CT_LOG(TAG, "  > Write to client:");
	fflush(stdout);

	// Get how much data received by Client before connection break
	do {
		ret = mbedtls_ssl_read(&ssl, buf, 10);
	} while (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE);
	if (ret < 0) {
		CT_LOGE(TAG, "failed ! mbedtls_ssl_write returned %d", ret);
		goto exit;
	}	
	server_read_write_data = server_read_write_data + atoi(buf);		
	remain = msg_size - server_read_write_data;
	cnt = 1;
	int total_sent = 0;	
	CT_LOG(TAG, "_tls_send_data start");	
	if (!g_tls_server_sent_data) {
		while (remain > 0) {
			int send_size = remain > BUF_SIZE ? BUF_SIZE : remain;
			do {
				ret = mbedtls_ssl_write(&ssl, buf, send_size);
			} while (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE);

			if (ret < 0) {				
				CT_LOGE(TAG, "failed ! mbedtls_ssl_write returned %d", ret);				
				CT_LOGE(TAG, "Sent_Till_Now:_%d", server_read_write_data);
				goto exit;
			}
			total_sent += ret;
			remain -= ret;			
			cnt++;
		}		
		CT_LOG(TAG, "TOTAL_BYTES_SEND:_%d", server_read_write_data + total_sent);
		CALCULATE_ELAPSED_TIME(TLS Data Transfer, *sum);
		g_tls_server_sent_data = true;
		server_read_write_data = 0;	
	}	

	CT_LOG(TAG, "1 second sleep -> before closing connection, allows receiving of data");
	sleep(1);

	/*9. Done, cleanly close the connection */
	CT_LOG(TAG, "Closing the connection...");
	ret = mbedtls_ssl_close_notify(&ssl);
	CT_LOG(TAG, "Closing the connection - ok");

	/*10. Final clean-ups and exit  */
exit:
#ifdef MBEDTLS_ERROR_C
	if (ret != 0) {
		char error_buf[100];
		mbedtls_strerror(ret, error_buf, 100);
		CT_LOG(TAG, "Last error was: %d - %s", ret, error_buf);
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
	return ret;
}
