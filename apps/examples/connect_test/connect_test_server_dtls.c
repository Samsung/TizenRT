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
#include "connect_test_server.h"
#include "connect_test_data.h"
#include "mbedtls/config.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/certs.h"
#include "mbedtls/x509.h"
#include "mbedtls/ssl.h"
#include "mbedtls/ssl_cookie.h"
#include "mbedtls/net.h"
#include "mbedtls/error.h"
#include "mbedtls/debug.h"
#include "mbedtls/timing.h"
#if defined(MBEDTLS_SSL_CACHE_C)
#include "mbedtls/ssl_cache.h"
#endif

#define TAG "[CT_DTLS_SERVER]"
#define DEBUG_LEVEL 0
#define READ_TIMEOUT_MS 30000
#define MAX_RETRY 5

extern unsigned char g_mac_buf_server[MAC_BUF_SIZE];
extern unsigned char g_recv_mac_buf_server[MAC_BUF_SIZE];

static unsigned char srv_crt[] =
	"-----BEGIN CERTIFICATE-----\r\n"
	"MIICHzCCAaWgAwIBAgIBCTAKBggqhkjOPQQDAjA+MQswCQYDVQQGEwJOTDERMA8G\r\n"
	"A1UEChMIUG9sYXJTU0wxHDAaBgNVBAMTE1BvbGFyc3NsIFRlc3QgRUMgQ0EwHhcN\r\n"
	"MTMwOTI0MTU1MjA0WhcNMjMwOTIyMTU1MjA0WjA0MQswCQYDVQQGEwJOTDERMA8G\r\n"
	"A1UEChMIUG9sYXJTU0wxEjAQBgNVBAMTCWxvY2FsaG9zdDBZMBMGByqGSM49AgEG\r\n"
	"CCqGSM49AwEHA0IABDfMVtl2CR5acj7HWS3/IG7ufPkGkXTQrRS192giWWKSTuUA\r\n"
	"2CMR/+ov0jRdXRa9iojCa3cNVc2KKg76Aci07f+jgZ0wgZowCQYDVR0TBAIwADAd\r\n"
	"BgNVHQ4EFgQUUGGlj9QH2deCAQzlZX+MY0anE74wbgYDVR0jBGcwZYAUnW0gJEkB\r\n"
	"PyvLeLUZvH4kydv7NnyhQqRAMD4xCzAJBgNVBAYTAk5MMREwDwYDVQQKEwhQb2xh\r\n"
	"clNTTDEcMBoGA1UEAxMTUG9sYXJzc2wgVGVzdCBFQyBDQYIJAMFD4n5iQ8zoMAoG\r\n"
	"CCqGSM49BAMCA2gAMGUCMQCaLFzXptui5WQN8LlO3ddh1hMxx6tzgLvT03MTVK2S\r\n"
	"C12r0Lz3ri/moSEpNZWqPjkCMCE2f53GXcYLqyfyJR078c/xNSUU5+Xxl7VZ414V\r\n"
	"fGa5kHvHARBPc8YAIVIqDvHH1Q==\r\n"
	"-----END CERTIFICATE-----\r\n";
static int srv_crt_len = sizeof(srv_crt);

static unsigned char ca_crt[] =
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
static int ca_crt_len = sizeof(ca_crt);

static unsigned char srv_key[] =
	"-----BEGIN EC PRIVATE KEY-----\r\n"
	"MHcCAQEEIPEqEyB2AnCoPL/9U/YDHvdqXYbIogTywwyp6/UfDw6noAoGCCqGSM49\r\n"
	"AwEHoUQDQgAEN8xW2XYJHlpyPsdZLf8gbu58+QaRdNCtFLX3aCJZYpJO5QDYIxH/\r\n"
	"6i/SNF1dFr2KiMJrdw1VzYoqDvoByLTt/w==\r\n"
	"-----END EC PRIVATE KEY-----\r\n";
static int srv_key_len = sizeof(srv_key);

static unsigned char buf[BUF_SIZE];

static void my_debug(void *ctx, int level, const char *file, int line, const char *str)
{
	((void)ctx);
	((void)level);
	CT_LOG(TAG, "%s:%04d: %s", file, line, str);
	fflush((FILE *)ctx);
}

int _dtls_server(int msg_size, long* sum)
{
	int ret = 0;
	struct timeval start, end;
	const char *pers = "dtls_server";
	unsigned char client_ip[16] = { 0 };
	size_t cliip_len;
	int retry = MAX_RETRY;

	mbedtls_net_context listen_fd, client_fd;
	mbedtls_entropy_context entropy;
	mbedtls_ctr_drbg_context ctr_drbg;
	mbedtls_ssl_context ssl;
	mbedtls_ssl_config conf;
	mbedtls_x509_crt srvcert;
	mbedtls_pk_context pkey;
	mbedtls_timing_delay_context timer;
	mbedtls_ssl_cookie_ctx cookie_ctx;
#if defined(MBEDTLS_SSL_CACHE_C)
	mbedtls_ssl_cache_context cache;
#endif

	mbedtls_net_init(&listen_fd);
	mbedtls_net_init(&client_fd);
	mbedtls_ssl_init(&ssl);
	mbedtls_ssl_config_init(&conf);
	mbedtls_ssl_cookie_init(&cookie_ctx);
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

	if ((ret = mbedtls_x509_crt_parse(&srvcert, (const unsigned char *)srv_crt,
							      srv_crt_len)) != 0) {
		CT_LOG(TAG, "failed ! mbedtls_x509_crt_parse(srv_crt) returned %d", ret);
		goto exit;
	}

	if ((ret = mbedtls_x509_crt_parse(&srvcert, (const unsigned char *)ca_crt,
							      ca_crt_len)) != 0) {
		CT_LOG(TAG, "failed ! mbedtls_x509_crt_parse(ca_crt) returned %d", ret);
		goto exit;
	}

	if ((ret = mbedtls_pk_parse_key(&pkey, (const unsigned char *)srv_key,
						srv_key_len, NULL, 0)) != 0) {
		CT_LOG(TAG, "failed ! mbedtls_pk_parse_key returned %d", ret);
		goto exit;
	}

	CT_LOG(TAG, "Loading the server cert. and key. - ok");

	/* 2. Setup the "listening" UDP socket */
	CT_LOG(TAG, "Bind on udp/*/%s ...", CT_TLS_DTLS_PORT);
	fflush(stdout);

	if ((ret = mbedtls_net_bind(&listen_fd, NULL, CT_TLS_DTLS_PORT, MBEDTLS_NET_PROTO_UDP)) != 0) {
		CT_LOG(TAG, "failed ! mbedtls_net_bind returned %d", ret);
		goto exit;
	}

	CT_LOG(TAG, "Bind on udp/*/%s - ok", CT_TLS_DTLS_PORT);

	/* 3. Seed the RNG */
	CT_LOG(TAG, "Seeding the random number generator...");
	fflush(stdout);

	if ((ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
				(const unsigned char *)pers, strlen(pers))) != 0) {
		CT_LOG(TAG, "failed ! mbedtls_ctr_drbg_seed returned %d", ret);
		goto exit;
	}

	CT_LOG(TAG, "Seeding the random number generator - ok");

	/* 4. Setup stuff */
	CT_LOG(TAG, "Setting up the DTLS data...");
	fflush(stdout);

	if ((ret = mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_SERVER,
	MBEDTLS_SSL_TRANSPORT_DATAGRAM, MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
		CT_LOG(TAG, "failed ! mbedtls_ssl_config_defaults returned %d", ret);
		goto exit;
	}

	mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);
	mbedtls_ssl_conf_dbg(&conf, my_debug, stdout);

#if defined(MBEDTLS_SSL_CACHE_C)
	mbedtls_ssl_conf_session_cache(&conf, &cache, mbedtls_ssl_cache_get, mbedtls_ssl_cache_set);
#endif

	mbedtls_ssl_conf_ca_chain(&conf, srvcert.next, NULL);
	if ((ret = mbedtls_ssl_conf_own_cert(&conf, &srvcert, &pkey)) != 0) {
		CT_LOG(TAG, "failed ! mbedtls_ssl_conf_own_cert returned %d", ret);
		goto exit;
	}

	if ((ret = mbedtls_ssl_cookie_setup(&cookie_ctx, mbedtls_ctr_drbg_random, &ctr_drbg)) != 0) {
		CT_LOG(TAG, "failed ! mbedtls_ssl_cookie_setup returned %d", ret);
		goto exit;
	}

	mbedtls_ssl_conf_dtls_cookies(&conf, mbedtls_ssl_cookie_write, 
				mbedtls_ssl_cookie_check, &cookie_ctx);

	static mbedtls_ecp_group_id server_curves[2] = {MBEDTLS_ECP_DP_SECP256R1,
							    MBEDTLS_ECP_DP_NONE};
	mbedtls_ssl_conf_curves(&conf, (mbedtls_ecp_group_id *)server_curves);

	int my_ciphersuite[2] = {MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_CCM, 0};
	mbedtls_ssl_conf_ciphersuites(&conf, my_ciphersuite);	

	mbedtls_ssl_conf_read_timeout(&conf, READ_TIMEOUT_MS);
	if ((ret = mbedtls_ssl_setup(&ssl, &conf)) != 0) {
		CT_LOG(TAG, " failed ! mbedtls_ssl_setup returned %d", ret);
		goto exit;
	}

	mbedtls_ssl_set_timer_cb(&ssl, &timer, mbedtls_timing_set_delay, mbedtls_timing_get_delay);

	CT_LOG(TAG, "Setting up the DTLS data - ok");

reset:
#ifdef MBEDTLS_ERROR_C
	if (ret != 0) {
		char error_buf[100];
		mbedtls_strerror(ret, error_buf, 100);
		CT_LOGE(TAG, "Last error was: %d - %s", ret, error_buf);
	}
#endif

	mbedtls_net_free(&client_fd);
	mbedtls_ssl_session_reset(&ssl);

	/*5. Wait until a client connects */
	CT_LOG(TAG, "Waiting for a remote connection ...");
	fflush(stdout);

	if ((ret = mbedtls_net_accept(&listen_fd, &client_fd, client_ip, sizeof(client_ip),
								      &cliip_len)) != 0) {
		CT_LOG(TAG, "failed  ! mbedtls_net_accept returned %d", ret);
		goto exit;
	}

	/* For HelloVerifyRequest cookies */
	if ((ret = mbedtls_ssl_set_client_transport_id(&ssl, client_ip, cliip_len)) != 0) {
		CT_LOG(TAG, "failed !  mbedtls_ssl_set_client_transport_id() returned -0x%x", -ret);
		goto exit;
	}

	mbedtls_ssl_set_bio(&ssl, &client_fd, mbedtls_net_send, mbedtls_net_recv, 
							mbedtls_net_recv_timeout);

	CT_LOG(TAG, "Waiting for a remote connection - ok");

	/* 6. Handshake */
	CT_LOG(TAG, "Performing the DTLS handshake...");
	fflush(stdout);
	
	SET_START_TIME(TLS handshake);
	do {
		ret = mbedtls_ssl_handshake(&ssl);
	} while (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE);

	if (ret == MBEDTLS_ERR_SSL_HELLO_VERIFY_REQUIRED) {
		CT_LOGE(TAG, " hello verification requested");
		ret = 0;
		if(retry-- > 0) {
			goto reset;
		} else {
			goto exit;
		}
	} else if (ret != 0) {
		CT_LOGE(TAG, " failed ! mbedtls_ssl_handshake returned -0x%x", -ret);
		goto exit;
	}
	CALCULATE_ELAPSED_TIME(TLS handshake, *sum);
	CT_LOG(TAG, "Performing the DTLS handshake - ok");
	
	CT_LOG(TAG, "Receiving Device MAC from peer:");
	do {
		ret = mbedtls_ssl_read(&ssl, g_recv_mac_buf_server, MAC_BUF_SIZE*sizeof(char));
	} while (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE);
	if (ret <= 0) {
		switch (ret) {
		case MBEDTLS_ERR_SSL_TIMEOUT:
			CT_LOG(TAG, " timeout");
			goto exit;
		case MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY:
			CT_LOG(TAG, " connection was closed gracefully");
			goto exit;
		case MBEDTLS_ERR_NET_CONN_RESET:
			CT_LOG(TAG, " connection was reset by peer");
			goto exit;
		default:
			CT_LOG(TAG, " mbedtls_ssl_read returned -0x%x", (unsigned int)-ret);
			goto exit;
		}
	}
	CT_LOG(TAG, "Recieved %d bytes for Peer MAC", ret);
	CT_LOG(TAG, "Peer MAC is: %s", g_recv_mac_buf_server);

	CT_LOG(TAG, "Sending Device MAC to peer. mac %s", g_mac_buf_server);
	do {
		ret = mbedtls_ssl_write(&ssl, g_mac_buf_server, MAC_BUF_SIZE*sizeof(char));
	} while (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE);

	if (ret < 0) {
		CT_LOG(TAG, "failed ! mbedtls_ssl_write returned %d", ret);
		goto exit;
	}

	SET_START_TIME(DTLS Data Transfer);

	/*7. Read the echo Request */
	CT_LOG(TAG, "  < ReadFromClient:");
	fflush(stdout);	
	int remain = msg_size, done = 0;
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
		
	/*8. Write the echo request */
	CT_LOG(TAG, "	> WriteToClient:");
	fflush(stdout);	 
	int ack = 0;
	remain = msg_size;
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
	CALCULATE_ELAPSED_TIME(DTLS Data Transfer, *sum);

	CT_LOG(TAG, "sleep 1s -> before closing connection, allows receiving of data");
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
	mbedtls_ssl_cookie_free(&cookie_ctx);
#if defined(MBEDTLS_SSL_CACHE_C)
	mbedtls_ssl_cache_free(&cache);
#endif
	mbedtls_ctr_drbg_free(&ctr_drbg);
	mbedtls_entropy_free(&entropy);
	return ret;
}
