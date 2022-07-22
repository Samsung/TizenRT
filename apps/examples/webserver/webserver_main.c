/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

/**
* @testcase		lwip_http_01 (server)
* @brief		To send HTTP request messages and receive response messages.
*			By default, it starts HTTP server with port 80 and HTTPS server with port 443.
*			Entity and encoding are needed only for POST and PUT methods. If an encoding is not for "C", it always uses the content-length.
* @scenario		1. Start webserver at TASH using the command "webserver start".
*			2. Start webclient at TASH using the command "webclient GET http://[serverip]/". Refer to webclient_main.c to run HTTP client.
* @apicovered
* @precondition		Connect to Wi-Fi. Both ARTIK05x server and ARTIK05x client should be in the same network.
* @postcondition
*/

/**
* @testcase		http_ws_01 (server)
* @brief		To run HTTP server and websocket client. Test packet size and number can be modified as parameters.
* @scenario		1. Start webserver at TASH using the command "webserver start".
*			2. Start websocket client at TASH using the command "websocket client [serverip] 80 \ 0 128 10". Refer to websocket_main.c to run websocket client.
* @apicovered
* @precondition		Connect to Wi-Fi. Both ARTIK05x server and ARTIK05x client should be in the same network.
* @postcondition
*/

/**
* @testcase		http_wss_01 (server)
* @brief		To run HTTPS server and secure websocket client. Test packet size and number can be modified as parameters.
* @scenario		1. Start webserver at TASH using the command "webserver start".
*			2. Start websocket client at TASH using the command "websocket client [serverip] 443 \ 1 128 10". Refer to websocket_main.c to run websocket client.
* @apicovered
* @precondition		Connect to Wi-Fi. Both ARTIK05x server and ARTIK05x client should be in the same network.
* @postcondition
*/

#include <tinyara/config.h>

#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <debug.h>
#include <pthread.h>

#include <net/if.h>
#include <netinet/in.h>
#include <tinyara/net/ethernet.h>
#include <netutils/netlib.h>
#include <protocols/webserver/http_server.h>
#include <protocols/webserver/http_keyvalue_list.h>
#include "mbedtls/certs.h"

#define MAX_DATA_SIZE 50
#define MAX_BUF_SIZE 100
#define MAX_DATA_COUNT 5
#define HTTP_200_RESPONSE 200
#define WEBSERVER_STACK_SIZE   (1024 * 8)
#define WEBSERVER_SCHED_PRI    100
#define WEBSERVER_SCHED_POLICY SCHED_RR
#define WEBSERVER_FREE_INPUT(node, size) \
	do { \
		int m = 0; \
		for (; m < size; m++) { \
			free(node->argv[m]); \
		} \
		free(node->argv); \
		free(node); \
	} while (0)

struct webserver_input {
	int argc;
	char **argv;
};


static const char *root_url = "/";
static const char *busy_url = "/busy";

static const char g_httpcontype[] = "Content-type";
static const char g_httpconhtml[] = "text/html";
static const char g_httpcontsize[] = "Content-Length";
static const char g_httpconnect[] = "Connection";
static const char g_httpcnlost[] = "close";

struct http_server_t *http_server = NULL;
struct http_server_t *https_server = NULL;

static int http_send_chunk(struct http_client_t *client)
{
	char msg_body[MAX_DATA_SIZE + 1] = {0, };
	int ret = 0;

	//add data
	memset(msg_body, 'a', MAX_DATA_SIZE);
	msg_body[MAX_DATA_SIZE] = 0;

	ret = http_send_response_chunk(client, HTTP_200_RESPONSE, "OK", msg_body, MAX_DATA_SIZE, NULL, FIRST_DATA);
	if (ret < 0) {
		printf("Error: Fail to send FIRST_DATA \n");
		return ret;
	}

	for (int i = 0; i < MAX_DATA_COUNT; i++) {
		ret = http_send_response_chunk(client, HTTP_200_RESPONSE, "OK", msg_body, MAX_DATA_SIZE, NULL, NEXT_DATA);
		if (ret < 0) {
			printf("Error: Fail to send NEXT_DATA \n");
			return ret;
		}
	}

	ret = http_send_response_chunk(client, HTTP_200_RESPONSE, "OK", msg_body, MAX_DATA_SIZE, NULL, LAST_DATA);
	if (ret < 0) {
		printf("Error: Fail to send LAST_DATA \n");
		return ret;
	}

	return 0;
}

/* GET callbacks */
void http_get_root(struct http_client_t *client, struct http_req_message *req)
{
	struct http_keyvalue_list_t response_headers;
	const char *msg = "This is a root page";
	char contlen[6] = { 0, };
	int resp_type = 0; // 0 - Normal response, 1 - Chunk response

	http_keyvalue_list_init(&response_headers);

	http_keyvalue_list_add(&response_headers, g_httpcontype, g_httpconhtml);
	snprintf(contlen, sizeof(contlen), "%d", strlen(msg));
	http_keyvalue_list_add(&response_headers, g_httpcontsize, contlen);
	http_keyvalue_list_add(&response_headers, g_httpconnect, g_httpcnlost);

	printf("===== GET_ROOT CALLBACK url : %s =====\n", req->url);

	if (resp_type == 1) { // this is temporary change
		if (http_send_chunk(client) < 0) {
			printf("Error: Fail to send response\n");
		}
	} else {
		if (http_send_response(client, 200, msg, &response_headers) < 0) {
			printf("Error: Fail to send response\n");
		}
	}

	http_keyvalue_list_release(&response_headers);
}

void http_get_callback(struct http_client_t *client, struct http_req_message *req)
{
	printf("===== GET CALLBACK url : %s =====\n", req->url);

	if (http_send_response(client, 200, "GET SUCCESS", NULL) < 0) {
		printf("Error: Fail to send response\n");
	}
}

/* PUT callback */
void http_put_callback(struct http_client_t *client,  struct http_req_message *req)
{
	printf("===== PUT CALLBACK url : %s entity size : %d =====\n", req->url, strlen(req->entity));

	/*
	 * in callback for POST and PUT request,
	 * entity must be checked it is null when received chunked entity.
	 * if it has chunked encoding entity, it must not send response.
	 */
	if (req->encoding == HTTP_CHUNKED_ENCODING && req->entity[0] != '\0') {
		printf("chunk : %s\n", req->entity);
		return;
	}

	if (http_send_response(client, 200, "PUT SUCCESS", NULL) < 0) {
		printf("Error: Fail to send response\n");
	}
}

void http_put_busy(struct http_client_t *client, struct http_req_message *req)
{
	int i;

	printf("===== PUT BUSY CALLBACK url : %s entity size : %d =====\n", req->url, strlen(req->entity));
	for (i = 0; i < 100; i++) sleep(1);

	if (http_send_response(client, 200, "PUT BUSY Success", NULL) < 0) {
		printf("Error: Fail to send response\n");
	}
}

/* POST callback */
void http_post_callback(struct http_client_t *client, struct http_req_message *req)
{
	printf("===== POST CALLBACK url : %s =====\n", req->url);

	/*
	 * in callback for POST and PUT request,
	 * entity must be checked it is null when received chunked entity.
	 * if it has chunked encoding entity, it must not send response.
	 */
	if (req->encoding == HTTP_CHUNKED_ENCODING && req->entity[0] != '\0') {
		printf("chunk : %s\n", req->entity);
		return;
	}

	if (http_send_response(client, 200, "POST SUCCESS", NULL) < 0) {
		printf("Error: Fail to send response\n");
	}
}

/* DELETE callback */
void http_delete_callback(struct http_client_t *client,  struct http_req_message *req)
{
	printf("===== DELETE CALLBACK url : %s =====\n", req->url);

	if (http_send_response(client, 200, "DELETE SUCCESS", NULL) < 0) {
		printf("Error: Fail to send response\n");
	}
}

#ifdef CONFIG_NETUTILS_WEBSOCKET
/* receive packets from TCP socket */
ssize_t ws_recv_cb(websocket_context_ptr ctx, uint8_t *buf, size_t len, int flags, void *user_data)
{
	ssize_t r;
	int fd;
	int retry_cnt = 3;
	struct websocket_info_t *info = user_data;

	fd = info->data->fd;
RECV_RETRY:
	if (info->data->tls_enabled) {
		r = mbedtls_ssl_read(info->data->tls_ssl, buf, len);
	} else {
		r = recv(fd, buf, len, 0);
	}

	if (r == 0) {
		websocket_set_error(info->data, WEBSOCKET_ERR_CALLBACK_FAILURE);
	} else if (r < 0) {
		printf("websocket recv_cb err : %d\n", errno);
		if (retry_cnt == 0) {
			websocket_set_error(info->data, WEBSOCKET_ERR_CALLBACK_FAILURE);
			return r;
		}
		retry_cnt--;
		goto RECV_RETRY;
	}

	return r;
}

/* send packets from TCP socket */
ssize_t ws_send_cb(websocket_context_ptr ctx, const uint8_t *buf, size_t len, int flags, void *user_data)
{
	ssize_t r;
	int fd;
	int retry_cnt = 3;
	struct websocket_info_t *info = user_data;

	fd = info->data->fd;
SEND_RETRY:
	if (info->data->tls_enabled) {
		r = mbedtls_ssl_write(info->data->tls_ssl, buf, len);
	} else {
		r = send(fd, buf, len, flags);
	}

	if (r < 0) {
		printf("websocket send_cb err : %d\n", errno);
		if (retry_cnt == 0) {
			websocket_set_error(info->data, WEBSOCKET_ERR_CALLBACK_FAILURE);
			return r;
		}
		retry_cnt--;
		goto SEND_RETRY;
	}

	return r;
}

void ws_server_on_msg_cb(websocket_context_ptr ctx, const websocket_on_msg_arg *arg, void *user_data)
{
	struct websocket_info_t *info = user_data;
	websocket_frame_t msgarg = {
		arg->opcode, arg->msg, arg->msg_length
	};

	/* Echo back non-closing message */
	if (WEBSOCKET_CHECK_NOT_CTRL_FRAME(arg->opcode)) {
		websocket_queue_msg(info->data, &msgarg);
		//echo back the message
	} else if (WEBSOCKET_CHECK_CTRL_CLOSE(arg->opcode)) {
		printf("echoback_on_msg received close message\n");
	} else if (WEBSOCKET_CHECK_CTRL_PING(arg->opcode)) {
		printf("srv got ping\n");
	} else if (WEBSOCKET_CHECK_CTRL_PONG(arg->opcode)) {
		printf("srv got pong\n");
	}
}
#endif

void print_webserver_usage(void)
{
	printf("\n  webserver usage:\n");
	printf("   $ webserver OPERATION OPTION\n");
	printf("\n OPERATION   : %%s (webserver start or stop)\n");
	printf("\n OPTION      : %%s default:require (require, optional, none)\n");
	printf("\n example:\n");
	printf("  $ webserver start none\n");

}

void register_callbacks(struct http_server_t *server)
{
	http_server_register_cb(server, HTTP_METHOD_GET, NULL, http_get_callback);
	http_server_register_cb(server, HTTP_METHOD_GET, root_url, http_get_root);

	http_server_register_cb(server, HTTP_METHOD_PUT, NULL, http_put_callback);
	http_server_register_cb(server, HTTP_METHOD_PUT, busy_url, http_put_busy);

	http_server_register_cb(server, HTTP_METHOD_POST, NULL, http_post_callback);
	http_server_register_cb(server, HTTP_METHOD_DELETE, NULL, http_delete_callback);

#ifdef CONFIG_NETUTILS_WEBSOCKET
	server->ws_cb.recv_callback = ws_recv_cb;
	server->ws_cb.send_callback = ws_send_cb;
	server->ws_cb.on_msg_recv_callback = ws_server_on_msg_cb;
#endif
}

void deregister_callbacks(struct http_server_t *server)
{
	http_server_deregister_cb(server, HTTP_METHOD_GET, NULL);
	http_server_deregister_cb(server, HTTP_METHOD_GET, root_url);

	http_server_deregister_cb(server, HTTP_METHOD_PUT, NULL);
	http_server_deregister_cb(server, HTTP_METHOD_PUT, busy_url);

	http_server_deregister_cb(server, HTTP_METHOD_POST, NULL);
	http_server_deregister_cb(server, HTTP_METHOD_DELETE, NULL);
}

pthread_addr_t httptest_cb(void *arg)
{
	int http_port = 80;

#ifdef CONFIG_NET_SECURITY_TLS
	int https_port = 443;
	struct ssl_config_t ssl_config;
	int auth_mode = MBEDTLS_SSL_VERIFY_REQUIRED;
#endif
	struct webserver_input *input = arg;
	if (!strncmp(input->argv[1], "start", 5)) {
#ifdef CONFIG_NET_SECURITY_TLS
		if (input->argc != 3) {
			print_webserver_usage();
			goto release;
		}
		if (strncmp(input->argv[2], "none", 4) == 0) {
			auth_mode = MBEDTLS_SSL_VERIFY_NONE;
		} else if (strncmp(input->argv[2], "required", 8) == 0) {
			auth_mode = MBEDTLS_SSL_VERIFY_REQUIRED;
		} else if (strncmp(input->argv[2], "optional", 8) == 0) {
			auth_mode = MBEDTLS_SSL_VERIFY_OPTIONAL;
		} else {
			print_webserver_usage();
			goto release;
		}
#endif
		goto start;
	} else if (!strncmp(input->argv[1], "stop", 4)) {
		if (input->argc != 2) {
			print_webserver_usage();
			goto release;
		}
		goto stop;
	} else {
		print_webserver_usage();
		goto release;
	}

start:
	if (http_server != NULL) {
		printf("Error: HTTP server is already run\n");
		goto release;
	}
#ifdef CONFIG_NET_SECURITY_TLS
	if (https_server != NULL) {
		printf("Error: HTTPS server is already run\n");
		goto release;
	}
	https_server = http_server_init(https_port);
	if (https_server == NULL) {
		printf("Error: Cannot allocate server structure!!\n");
		goto release;
	}
	ssl_config.root_ca = (char *)mbedtls_test_ca_crt;
	ssl_config.root_ca_len = mbedtls_test_ca_crt_len;
	ssl_config.dev_cert = (char *)mbedtls_test_srv_crt;
	ssl_config.dev_cert_len = mbedtls_test_srv_crt_len;
	ssl_config.private_key = (char *)mbedtls_test_srv_key;
	ssl_config.private_key_len = mbedtls_test_srv_key_len;
	ssl_config.auth_mode = auth_mode;

	if (http_tls_init(https_server, &ssl_config) != 0) {
		printf("ssl config Error\n");
		goto release;
	}
#endif
	http_server = http_server_init(http_port);

	if (http_server == NULL) {
		printf("Error: Cannot allocate server structure!!\n");
		goto release;
	}

	register_callbacks(http_server);
#ifdef CONFIG_NET_SECURITY_TLS
	register_callbacks(https_server);
#endif
	printf("Start Web server...\n");

#ifdef CONFIG_NET_SECURITY_TLS
	if (http_server_start(https_server) < 0) {
		printf("Fail to start HTTPS server\n");
	}
#endif
	if (http_server_start(http_server) < 0) {
		printf("Fail to start HTTP server\n");
	}
	goto release;

stop:
	http_server_stop(http_server);
	deregister_callbacks(http_server);
	http_server_release(&http_server);
#ifdef CONFIG_NET_SECURITY_TLS
	http_server_stop(https_server);
	deregister_callbacks(https_server);
	http_server_release(&https_server);
#endif
	printf("webserver end\n");

release:
	WEBSERVER_FREE_INPUT(input, input->argc);
	return NULL;
}

int webserver_main(int argc, char *argv[])
{
	pthread_attr_t attr;
	int status;
	struct sched_param sparam;
	pthread_t tid;
	struct webserver_input *input;

	input = (struct webserver_input *)malloc(sizeof(struct webserver_input));
	if (!input) {
		printf(" malloc fail\n");
		return -1;
	}

	input->argv = (char **)malloc(sizeof(char *) * argc);
	if (!input->argv) {
		printf(" malloc argv fail\n");
		free(input);
		return -1;
	}

	input->argc = argc;
	int i = 0;
	for (; i < argc; i++) {
		input->argv[i] = (char *)malloc(sizeof(char) * (strlen(argv[i]) + 1));
		if (!input->argv[i]) {
			WEBSERVER_FREE_INPUT(input, i);
			return -1;
		}
		strncpy(input->argv[i], argv[i], strlen(argv[i]));
	}
	status = pthread_attr_init(&attr);
	if (status != 0) {
		printf("fail to start webserver\n");
		return -1;
	}

	sparam.sched_priority = WEBSERVER_SCHED_PRI;
	status = pthread_attr_setschedparam(&attr, &sparam);
	status = pthread_attr_setschedpolicy(&attr, WEBSERVER_SCHED_POLICY);
	status = pthread_attr_setstacksize(&attr, WEBSERVER_STACK_SIZE);

	status = pthread_create(&tid, &attr, httptest_cb, input);
	if (status < 0) {
		printf("fail to start webserver\n");
		WEBSERVER_FREE_INPUT(input, input->argc);
		return -1;
	}
	pthread_setname_np(tid, "webserver");

	pthread_join(tid, NULL);

	return 0;
}
