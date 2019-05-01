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

const char ca_crt_rsa[] =
	"-----BEGIN CERTIFICATE-----\r\n"
	"MIIDhzCCAm+gAwIBAgIBADANBgkqhkiG9w0BAQUFADA7MQswCQYDVQQGEwJOTDER\r\n"
	"MA8GA1UEChMIUG9sYXJTU0wxGTAXBgNVBAMTEFBvbGFyU1NMIFRlc3QgQ0EwHhcN\r\n"
	"MTEwMjEyMTQ0NDAwWhcNMjEwMjEyMTQ0NDAwWjA7MQswCQYDVQQGEwJOTDERMA8G\r\n"
	"A1UEChMIUG9sYXJTU0wxGTAXBgNVBAMTEFBvbGFyU1NMIFRlc3QgQ0EwggEiMA0G\r\n"
	"CSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDA3zf8F7vglp0/ht6WMn1EpRagzSHx\r\n"
	"mdTs6st8GFgIlKXsm8WL3xoemTiZhx57wI053zhdcHgH057Zk+i5clHFzqMwUqny\r\n"
	"50BwFMtEonILwuVA+T7lpg6z+exKY8C4KQB0nFc7qKUEkHHxvYPZP9al4jwqj+8n\r\n"
	"YMPGn8u67GB9t+aEMr5P+1gmIgNb1LTV+/Xjli5wwOQuvfwu7uJBVcA0Ln0kcmnL\r\n"
	"R7EUQIN9Z/SG9jGr8XmksrUuEvmEF/Bibyc+E1ixVA0hmnM3oTDPb5Lc9un8rNsu\r\n"
	"KNF+AksjoBXyOGVkCeoMbo4bF6BxyLObyavpw/LPh5aPgAIynplYb6LVAgMBAAGj\r\n"
	"gZUwgZIwDAYDVR0TBAUwAwEB/zAdBgNVHQ4EFgQUtFrkpbPe0lL2udWmlQ/rPrzH\r\n"
	"/f8wYwYDVR0jBFwwWoAUtFrkpbPe0lL2udWmlQ/rPrzH/f+hP6Q9MDsxCzAJBgNV\r\n"
	"BAYTAk5MMREwDwYDVQQKEwhQb2xhclNTTDEZMBcGA1UEAxMQUG9sYXJTU0wgVGVz\r\n"
	"dCBDQYIBADANBgkqhkiG9w0BAQUFAAOCAQEAuP1U2ABUkIslsCfdlc2i94QHHYeJ\r\n"
	"SsR4EdgHtdciUI5I62J6Mom+Y0dT/7a+8S6MVMCZP6C5NyNyXw1GWY/YR82XTJ8H\r\n"
	"DBJiCTok5DbZ6SzaONBzdWHXwWwmi5vg1dxn7YxrM9d0IjxM27WNKs4sDQhZBQkF\r\n"
	"pjmfs2cb4oPl4Y9T9meTx/lvdkRYEug61Jfn6cA+qHpyPYdTH+UshITnmp5/Ztkf\r\n"
	"m/UTSLBNFNHesiTZeH31NcxYGdHSme9Nc/gfidRa0FLOCfWxRlFqAI47zG9jAQCZ\r\n"
	"7Z2mCGDNMhjQc+BYcdnl0lPXjdDK6V0qCg1dVewhUBcW5gZKzV7e9+DpVA==\r\n"
	"-----END CERTIFICATE-----\r\n";

const char srv_crt_rsa[] =
	"-----BEGIN CERTIFICATE-----\r\n"
	"MIIDNzCCAh+gAwIBAgIBAjANBgkqhkiG9w0BAQUFADA7MQswCQYDVQQGEwJOTDER\r\n"
	"MA8GA1UEChMIUG9sYXJTU0wxGTAXBgNVBAMTEFBvbGFyU1NMIFRlc3QgQ0EwHhcN\r\n"
	"MTEwMjEyMTQ0NDA2WhcNMjEwMjEyMTQ0NDA2WjA0MQswCQYDVQQGEwJOTDERMA8G\r\n"
	"A1UEChMIUG9sYXJTU0wxEjAQBgNVBAMTCWxvY2FsaG9zdDCCASIwDQYJKoZIhvcN\r\n"
	"AQEBBQADggEPADCCAQoCggEBAMFNo93nzR3RBNdJcriZrA545Do8Ss86ExbQWuTN\r\n"
	"owCIp+4ea5anUrSQ7y1yej4kmvy2NKwk9XfgJmSMnLAofaHa6ozmyRyWvP7BBFKz\r\n"
	"NtSj+uGxdtiQwWG0ZlI2oiZTqqt0Xgd9GYLbKtgfoNkNHC1JZvdbJXNG6AuKT2kM\r\n"
	"tQCQ4dqCEGZ9rlQri2V5kaHiYcPNQEkI7mgM8YuG0ka/0LiqEQMef1aoGh5EGA8P\r\n"
	"hYvai0Re4hjGYi/HZo36Xdh98yeJKQHFkA4/J/EwyEoO79bex8cna8cFPXrEAjya\r\n"
	"HT4P6DSYW8tzS1KW2BGiLICIaTla0w+w3lkvEcf36hIBMJcCAwEAAaNNMEswCQYD\r\n"
	"VR0TBAIwADAdBgNVHQ4EFgQUpQXoZLjc32APUBJNYKhkr02LQ5MwHwYDVR0jBBgw\r\n"
	"FoAUtFrkpbPe0lL2udWmlQ/rPrzH/f8wDQYJKoZIhvcNAQEFBQADggEBAJxnXClY\r\n"
	"oHkbp70cqBrsGXLybA74czbO5RdLEgFs7rHVS9r+c293luS/KdliLScZqAzYVylw\r\n"
	"UfRWvKMoWhHYKp3dEIS4xTXk6/5zXxhv9Rw8SGc8qn6vITHk1S1mPevtekgasY5Y\r\n"
	"iWQuM3h4YVlRH3HHEMAD1TnAexfXHHDFQGe+Bd1iAbz1/sH9H8l4StwX6egvTK3M\r\n"
	"wXRwkKkvjKaEDA9ATbZx0mI8LGsxSuCqe9r9dyjmttd47J1p1Rulz3CLzaRcVIuS\r\n"
	"RRQfaD8neM9c1S/iJ/amTVqJxA1KOdOS5780WhPfSArA+g4qAmSjelc3p4wWpha8\r\n"
	"zhuYwjVuX6JHG0c=\r\n"
	"-----END CERTIFICATE-----\r\n";

const char srv_key_rsa[] =
	"-----BEGIN RSA PRIVATE KEY-----\r\n"
	"MIIEpAIBAAKCAQEAwU2j3efNHdEE10lyuJmsDnjkOjxKzzoTFtBa5M2jAIin7h5r\r\n"
	"lqdStJDvLXJ6PiSa/LY0rCT1d+AmZIycsCh9odrqjObJHJa8/sEEUrM21KP64bF2\r\n"
	"2JDBYbRmUjaiJlOqq3ReB30Zgtsq2B+g2Q0cLUlm91slc0boC4pPaQy1AJDh2oIQ\r\n"
	"Zn2uVCuLZXmRoeJhw81ASQjuaAzxi4bSRr/QuKoRAx5/VqgaHkQYDw+Fi9qLRF7i\r\n"
	"GMZiL8dmjfpd2H3zJ4kpAcWQDj8n8TDISg7v1t7HxydrxwU9esQCPJodPg/oNJhb\r\n"
	"y3NLUpbYEaIsgIhpOVrTD7DeWS8Rx/fqEgEwlwIDAQABAoIBAQCXR0S8EIHFGORZ\r\n"
	"++AtOg6eENxD+xVs0f1IeGz57Tjo3QnXX7VBZNdj+p1ECvhCE/G7XnkgU5hLZX+G\r\n"
	"Z0jkz/tqJOI0vRSdLBbipHnWouyBQ4e/A1yIJdlBtqXxJ1KE/ituHRbNc4j4kL8Z\r\n"
	"/r6pvwnTI0PSx2Eqs048YdS92LT6qAv4flbNDxMn2uY7s4ycS4Q8w1JXnCeaAnYm\r\n"
	"WYI5wxO+bvRELR2Mcz5DmVnL8jRyml6l6582bSv5oufReFIbyPZbQWlXgYnpu6He\r\n"
	"GTc7E1zKYQGG/9+DQUl/1vQuCPqQwny0tQoX2w5tdYpdMdVm+zkLtbajzdTviJJa\r\n"
	"TWzL6lt5AoGBAN86+SVeJDcmQJcv4Eq6UhtRr4QGMiQMz0Sod6ettYxYzMgxtw28\r\n"
	"CIrgpozCc+UaZJLo7UxvC6an85r1b2nKPCLQFaggJ0H4Q0J/sZOhBIXaoBzWxveK\r\n"
	"nupceKdVxGsFi8CDy86DBfiyFivfBj+47BbaQzPBj7C4rK7UlLjab2rDAoGBAN2u\r\n"
	"AM2gchoFiu4v1HFL8D7lweEpi6ZnMJjnEu/dEgGQJFjwdpLnPbsj4c75odQ4Gz8g\r\n"
	"sw9lao9VVzbusoRE/JGI4aTdO0pATXyG7eG1Qu+5Yc1YGXcCrliA2xM9xx+d7f+s\r\n"
	"mPzN+WIEg5GJDYZDjAzHG5BNvi/FfM1C9dOtjv2dAoGAF0t5KmwbjWHBhcVqO4Ic\r\n"
	"BVvN3BIlc1ue2YRXEDlxY5b0r8N4XceMgKmW18OHApZxfl8uPDauWZLXOgl4uepv\r\n"
	"whZC3EuWrSyyICNhLY21Ah7hbIEBPF3L3ZsOwC+UErL+dXWLdB56Jgy3gZaBeW7b\r\n"
	"vDrEnocJbqCm7IukhXHOBK8CgYEAwqdHB0hqyNSzIOGY7v9abzB6pUdA3BZiQvEs\r\n"
	"3LjHVd4HPJ2x0N8CgrBIWOE0q8+0hSMmeE96WW/7jD3fPWwCR5zlXknxBQsfv0gP\r\n"
	"3BC5PR0Qdypz+d+9zfMf625kyit4T/hzwhDveZUzHnk1Cf+IG7Q+TOEnLnWAWBED\r\n"
	"ISOWmrUCgYAFEmRxgwAc/u+D6t0syCwAYh6POtscq9Y0i9GyWk89NzgC4NdwwbBH\r\n"
	"4AgahOxIxXx2gxJnq3yfkJfIjwf0s2DyP0kY2y6Ua1OeomPeY9mrIS4tCuDQ6LrE\r\n"
	"TB6l9VGoxJL4fyHnZb8L5gGvnB1bbD8cL6YPaDiOhcRseC9vBiEuVg==\r\n"
	"-----END RSA PRIVATE KEY-----\r\n";

static const char *root_url = "/";
static const char *busy_url = "/busy";

static const char g_httpcontype[] = "Content-type";
static const char g_httpconhtml[] = "text/html";
static const char g_httpcontsize[] = "Content-Length";
static const char g_httpconnect[] = "Connection";
static const char g_httpcnlost[] = "close";

struct http_server_t *http_server = NULL;
struct http_server_t *https_server = NULL;

/* GET callbacks */
void http_get_root(struct http_client_t *client, struct http_req_message *req)
{
	struct http_keyvalue_list_t response_headers;
	const char *msg = "This is a root page";
	char contlen[6] = { 0, };

	http_keyvalue_list_init(&response_headers);

	http_keyvalue_list_add(&response_headers, g_httpcontype, g_httpconhtml);
	snprintf(contlen, sizeof(contlen), "%d", strlen(msg));
	http_keyvalue_list_add(&response_headers, g_httpcontsize, contlen);
	http_keyvalue_list_add(&response_headers, g_httpconnect, g_httpcnlost);

	printf("===== GET_ROOT CALLBACK url : %s =====\n", req->url);
	if (http_send_response(client, 200, msg, &response_headers) < 0) {
		printf("Error: Fail to send response\n");
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
	ssl_config.root_ca = (char *)ca_crt_rsa;
	ssl_config.root_ca_len = sizeof(ca_crt_rsa);
	ssl_config.dev_cert = (char *)srv_crt_rsa;
	ssl_config.dev_cert_len = sizeof(srv_crt_rsa);
	ssl_config.private_key = (char *)srv_key_rsa;
	ssl_config.private_key_len = sizeof(srv_key_rsa);
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
