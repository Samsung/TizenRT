/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
 * @file artik_onboarding_rest.c
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sched.h>
#include <pthread.h>
#include <cJSON.h>
#include <artik_error.h>
#include <artik_security.h>

#include <apps/netutils/webserver/http_server.h>
#include <apps/netutils/webserver/http_keyvalue_list.h>
#include <tls/see_api.h>

#include "artik_onboarding.h"

#define API_VERSION                 "/v1.0"
#define API_WIFI                    API_VERSION"/wifi"
#define API_WIFI_ACCESS_POINTS      API_WIFI"/accesspoints"
#define API_WIFI_CONFIG             API_WIFI"/config"
#define API_WIFI_CONNECTION         API_WIFI"/connection"
#define API_CLOUD                   API_VERSION"/artikcloud"
#define API_CLOUD_REGISTRATION      API_VERSION"/artikcloud/registration"

#define RESP_ERROR_INVALID_JSON_TPL   RESP_ERROR(API_ERROR_INVALID_JSON, "Invalid JSON")
#define RESP_ERROR_INVALID_PARAMS_TPL RESP_ERROR(API_ERROR_INVALID_PARAMS, "Invalid Parameters")
#define RESP_ERROR_INVALID_UUID_TPL   RESP_ERROR(API_ERROR_INVALID_UUID, "Invalid UUID")
#define RESP_ERROR_MISSING_PARAM_TPL  RESP_ERROR(API_ERROR_MISSING_PARAM, "Input argument missing in JSON")

/*
 * Uncomment the following variable to disable HTTPS and use unsecure
 * HTTP instead. Note that doing so will drop encryption of the packets
 * exchanged between the mobile app and the onboarding service, exposing
 * critical information such as the Wifi access point passphrase over the
 * air.
 */
//#define WEBSERVER_DISABLE_HTTPS

static struct http_server_t *https_server;

static pthread_addr_t start_websocket(pthread_addr_t arg)
{
	StartMDNSService(false);
	StartWebServer(false, API_SET_CLOUD);
	if (StartCloudWebsocket(true) == S_OK)
		printf("ARTIK Cloud connection started\n");
	if (StartLwm2m(true) == S_OK)
		printf("LWM2M connection started\n");

	return NULL;
}

static void get_akc_callback(struct http_client_t *client, struct http_req_message *req)
{
	struct http_keyvalue_list_t headers;
	char *body = NULL;
	cJSON *resp = cJSON_CreateObject();
	char length[32];

	printf("GET %s\n", req->url);

	cJSON_AddStringToObject(resp, "dtid", cloud_config.device_type_id);
	cJSON_AddStringToObject(resp, "did", cloud_config.device_id);
	body = cJSON_PrintUnformatted(resp);
	itoa(strlen(body), length, 10);

	http_keyvalue_list_init(&headers);
	http_keyvalue_list_add(&headers, "Content-type", "application/json");
	http_keyvalue_list_add(&headers, "Content-Length", length);
	http_keyvalue_list_add(&headers, "Connection", "close");

	if (http_send_response(client, 200, body, &headers) < 0) {
		printf("Failed to send response\n");
	}

	if (body)
		free(body);
	cJSON_Delete(resp);
}

static void set_akc_callback(struct http_client_t *client, struct http_req_message *msg)
{
	struct http_keyvalue_list_t headers;
	int status = 200;
	cJSON *config, *tmp;
	char resp[128];
	char length[32];

	printf("POST %s\n", msg->url);

	config = cJSON_Parse(msg->entity);
	if (!config) {
		strncpy(resp, RESP_ERROR_INVALID_JSON_TPL, sizeof(resp)-1);
		goto exit;
	}

	tmp = cJSON_GetObjectItem(config, "did");
	if (tmp) {
		if (strlen(tmp->valuestring) != AKC_DID_LEN) {
			strncpy(resp, RESP_ERROR_INVALID_UUID_TPL, sizeof(resp)-1);
			goto exit;
		}
		strncpy(cloud_config.device_id, tmp->valuestring, AKC_DID_LEN);
	}

	tmp = cJSON_GetObjectItem(config, "token");
	if (tmp) {
		if (strlen(tmp->valuestring) != AKC_TOKEN_LEN) {
			strncpy(resp, RESP_ERROR_INVALID_UUID_TPL, sizeof(resp)-1);
			goto exit;
		}
		strncpy(cloud_config.device_token, tmp->valuestring, AKC_TOKEN_LEN);
	}

	SaveConfiguration();

	/* Start connection to ARTIK Cloud */
	pthread_t tid;

	pthread_create(&tid, NULL, start_websocket, NULL);
	pthread_detach(tid);

	strncpy(resp, RESP_ERROR_OK, sizeof(resp)-1);

exit:
	itoa(strlen(resp), length, 10);
	http_keyvalue_list_init(&headers);
	http_keyvalue_list_add(&headers, "Content-type", "application/json");
	http_keyvalue_list_add(&headers, "Content-Length", length);
	http_keyvalue_list_add(&headers, "Connection", "close");

	if (http_send_response(client, status, resp, &headers) < 0) {
		printf("Failed to send response\n");
	}

	if (config)
		cJSON_Delete(config);
}

static void get_ap_callback(struct http_client_t *client, struct http_req_message *req)
{
	struct http_keyvalue_list_t headers;
	int status = 200;
	char *body = WifiScanResult();

	printf("GET %s\n", req->url);

	http_keyvalue_list_init(&headers);
	http_keyvalue_list_add(&headers, "Content-type", "application/json");
	http_keyvalue_list_add(&headers, "Connection", "close");

	if (!body) {
		printf("Failed to scan WiFi networks\n");
		status = 500;
	} else {
		char length[32];

		itoa(strlen(body), length, 10);
		http_keyvalue_list_add(&headers, "Content-Length", length);
	}

	if (http_send_response(client, status, body, &headers) < 0) {
		printf("Failed to send response\n");
	}

	if (body) {
		free(body);
	}
}

static pthread_addr_t cloud_onboarding_start(pthread_addr_t arg)
{
	StartWebServer(false, API_SET_WIFI);
	printf("Start station connection\n");
	if (StartStationConnection(true) != S_OK) {
		printf("Failed to start connection to AP, returning to AP mode\n");
		if (StartSoftAP(true) != S_OK) {
			printf("Failed to start AP mode\n");
			return NULL;
		}

		if (StartWebServer(true, API_SET_WIFI) != S_OK) {
			StartSoftAP(false);
			printf("Failed to start Web server\n");
		}

		return NULL;
	}

	printf("Start webserver cloud API\n");
	if (StartWebServer(true, API_SET_CLOUD) != S_OK) {
		StartSoftAP(false);
		printf("Failed to start Web server\n");
		return NULL;
	}

	printf("Start MDNS service\n");
	StartMDNSService(true);

	return 0;
}

static void set_config_callback(struct http_client_t *client, struct http_req_message *msg)
{
	struct http_keyvalue_list_t headers;
	int status = 200;
	cJSON *config, *tmp;
	char resp[128];
	char length[32];
	bool start_connection = false;

	printf("POST %s\n", msg->url);

	config = cJSON_Parse(msg->entity);
	if (!config) {
		status = 400;
		strncpy(resp, RESP_ERROR_INVALID_JSON_TPL, sizeof(resp)-1);
		goto exit;
	}

	tmp = cJSON_GetObjectItem(config, "ssid");
	if (tmp) {
		if (tmp->type != cJSON_String) {
			status = 400;
			strncpy(resp, RESP_ERROR_INVALID_PARAMS_TPL, sizeof(resp)-1);
			goto exit;
		}
		WifiResetConfig(false);
		strncpy(wifi_config.ssid, tmp->valuestring, SSID_MAX_LEN);
	}

	tmp = cJSON_GetObjectItem(config, "security");
	if (tmp) {
		if (tmp->type != cJSON_String) {
			status = 400;
			strncpy(resp, RESP_ERROR_INVALID_PARAMS_TPL, sizeof(resp)-1);
			goto exit;
		}

		if (strcmp(tmp->valuestring, "Secure") && strcmp(tmp->valuestring, "Open")) {
			status = 400;
			strncpy(resp, RESP_ERROR_INVALID_PARAMS_TPL, sizeof(resp)-1);
			goto exit;
		}

		wifi_config.secure = (strcmp(tmp->valuestring, "Secure") == 0);
	}

	if (wifi_config.secure) {
		tmp = cJSON_GetObjectItem(config, "passphrase");
		if (tmp) {
			if (tmp->type != cJSON_String) {
				status = 400;
				strncpy(resp, RESP_ERROR_INVALID_PARAMS_TPL, sizeof(resp)-1);
				goto exit;
			}

			strncpy(wifi_config.passphrase, tmp->valuestring, PASSPHRASE_MAX_LEN);
		}
	}

	tmp = cJSON_GetObjectItem(config, "connect");
	if (tmp) {
		if ((tmp->type != cJSON_True) && (tmp->type != cJSON_False)) {
			status = 400;
			strncpy(resp, RESP_ERROR_INVALID_PARAMS_TPL, sizeof(resp)-1);
			goto exit;
		}

		if (tmp->type == cJSON_True) {
			/* Check if proper config has been provided */
			if (!(wifi_config.ssid[0])) {
				status = 400;
				strncpy(resp, RESP_ERROR_MISSING_PARAM_TPL, sizeof(resp)-1);
				goto exit;
			}

			if (wifi_config.secure && !(wifi_config.passphrase[0])) {
				status = 400;
				strncpy(resp, RESP_ERROR_MISSING_PARAM_TPL, sizeof(resp)-1);
				goto exit;
			}

			start_connection = true;
		}
	}

	SaveConfiguration();

	strncpy(resp, RESP_ERROR_OK, sizeof(resp)-1);

exit:

	itoa(strlen(resp), length, 10);
	http_keyvalue_list_init(&headers);
	http_keyvalue_list_add(&headers, "Content-type", "application/json");
	http_keyvalue_list_add(&headers, "Content-Length", length);
	http_keyvalue_list_add(&headers, "Connection", "close");

	if (http_send_response(client, status, resp, &headers) < 0) {
		printf("Failed to send response\n");
	}

	if (start_connection) {
		pthread_t tid;
		pthread_attr_t attr;

		pthread_attr_init(&attr);
		pthread_attr_setschedpolicy(&attr, SCHED_RR);
		pthread_attr_setstacksize(&attr, 8 * 1024);
		pthread_create(&tid, &attr, cloud_onboarding_start, NULL);
		pthread_detach(tid);
	}

	if (config)
		cJSON_Delete(config);
}

static void get_akc_registration_callback(struct http_client_t *client, struct http_req_message *msg)
{
	struct http_keyvalue_list_t headers;
	int status = 200;
	char *resp = NULL;
	char length[32];

	printf("GET %s\n", msg->url);

	/* Call start registration Cloud API and get the response */
	status = StartSDRRegistration(&resp);

	itoa(strlen(resp), length, 10);
	http_keyvalue_list_init(&headers);
	http_keyvalue_list_add(&headers, "Content-type", "application/json");
	http_keyvalue_list_add(&headers, "Content-Length", length);
	http_keyvalue_list_add(&headers, "Connection", "close");

	if (http_send_response(client, 200, resp, &headers) < 0)
		printf("Failed to send response\n");

	if (resp)
		free(resp);
}

static void put_akc_registration_callback(struct http_client_t *client, struct http_req_message *msg)
{
	struct http_keyvalue_list_t headers;
	int status = 200;
	char *resp = NULL;
	char length[32];

	printf("PUT %s\n", msg->url);

	/* Call complete registration Cloud API and get the response */
	status = CompleteSDRRegistration(&resp);

	itoa(strlen(resp), length, 10);
	http_keyvalue_list_init(&headers);
	http_keyvalue_list_add(&headers, "Content-type", "application/json");
	http_keyvalue_list_add(&headers, "Content-Length", length);
	http_keyvalue_list_add(&headers, "Connection", "close");

	if (http_send_response(client, 200, resp, &headers) < 0) {
		printf("Failed to send response\n");
	}

	if (resp)
		free(resp);

	/*
	 * If onboarding went well, save configuration
	 * and launch connection to cloud.
	 */
	if (status == 200) {
		memset(cloud_config.reg_id, 0, AKC_REG_ID_LEN + 1);
		memset(cloud_config.reg_nonce, 0, AKC_REG_NONCE_LEN + 1);
		SaveConfiguration();

		pthread_t tid;

		pthread_create(&tid, NULL, start_websocket, NULL);
		pthread_detach(tid);
	}
}

#ifndef WEBSERVER_DISABLE_HTTPS

#define PEM_END_CRT             "-----END CERTIFICATE-----\n"

static void http_tls_debug(void *ctx, int level, const char *file, int line,
		const char *str)
{
	printf("%s:%04d: %s", file, line, str);
}

static artik_error ssl_init_context(struct http_server_t *server)
{
	char *server_cert = NULL;
	char *ca_chain = NULL;
	char *index = NULL;
	const mbedtls_pk_info_t *pk_info;
	artik_security_handle handle = NULL;
	artik_security_module *security = NULL;
	artik_error ret = S_OK;
	int err = 0;

	mbedtls_ssl_config_init(&server->tls_conf);
	mbedtls_x509_crt_init(&server->tls_srvcert);
	mbedtls_pk_init(&server->tls_pkey);
	mbedtls_entropy_init(&server->tls_entropy);
	mbedtls_ctr_drbg_init(&server->tls_ctr_drbg);
	mbedtls_net_init(&server->tls_ctx);
	mbedtls_ssl_cache_init(&server->tls_cache);

#ifdef MBEDTLS_DEBUG_C
	mbedtls_debug_set_threshold(0);
#endif

	/* Seed the Random Number Generator */
	err = mbedtls_ctr_drbg_seed(&server->tls_ctr_drbg,
			mbedtls_entropy_func, &server->tls_entropy, NULL, 0);
	if (err) {
		ret = E_BAD_ARGS;
		goto exit;
	}

	/* Setup default config */
	err = mbedtls_ssl_config_defaults(&server->tls_conf, MBEDTLS_SSL_IS_SERVER,
			MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT);
	if (err) {
		ret = E_BAD_ARGS;
		goto exit;
	}

	mbedtls_ssl_conf_dbg(&server->tls_conf, http_tls_debug, stdout);
	mbedtls_ssl_conf_rng(&server->tls_conf, mbedtls_ctr_drbg_random,
			&server->tls_ctr_drbg);
	mbedtls_ssl_conf_session_cache(&server->tls_conf, &server->tls_cache,
			mbedtls_ssl_cache_get, mbedtls_ssl_cache_set);

	security = (artik_security_module *)
			artik_request_api_module("security");
	if (!security) {
		printf("Failed to request security module\n");
		ret = E_BUSY;
		goto exit;
	}

	ret = security->request(&handle);
	if (ret != S_OK) {
		printf("Failed to initialize security module (err=%d)\n", ret);
		goto exit;
	}

	ret = security->get_certificate(handle, CERT_ID_ARTIK, &server_cert);
	if ((ret != S_OK) || !server_cert) {
		printf("Failed to get certificate from SE (err=%d)\n", ret);
		goto exit;
	}

	err = mbedtls_x509_crt_parse(&server->tls_srvcert,
			(const unsigned char *)server_cert, strlen(server_cert) + 1);
	if (err) {
		printf("Failed to parse server certificate (err=%d)\n", err);
		ret = E_BAD_ARGS;
		goto exit;
	}

	ret = security->get_ca_chain(handle, CERT_ID_ARTIK, &ca_chain);
	if ((ret != S_OK) || !ca_chain) {
		printf("Failed to get CA chain from SE (err=%d)\n", ret);
		ret = E_BAD_ARGS;
		goto exit;
	}

	/* Split after the first certificate to only keep the intermediate */
	index = strstr(ca_chain, PEM_END_CRT);
	if (!index) {
		printf("Wrong certificate format\n");
		ret = E_BAD_ARGS;
		goto exit;
	}

	index += strlen(PEM_END_CRT);
	*index = '\0';

	err = mbedtls_x509_crt_parse(&server->tls_srvcert,
			(const unsigned char *)ca_chain, strlen(ca_chain) + 1);
	if (err) {
		printf("Failed to parse server certificate (err=%d)\n", err);
		ret = E_BAD_ARGS;
		goto exit;
	}

	pk_info = mbedtls_pk_info_from_type(MBEDTLS_PK_ECKEY);
	if (!pk_info) {
		ret = E_BAD_ARGS;
		goto exit;
	}

	err = mbedtls_pk_setup(&server->tls_pkey, pk_info);
	if (err) {
		printf("Failed to setup private key (err=%d)\n", err);
		ret = E_BAD_ARGS;
		goto exit;
	}

	((mbedtls_ecdsa_context *)(server->tls_pkey.pk_ctx))->grp.id =
			MBEDTLS_ECP_DP_SECP256R1;
	((mbedtls_ecdsa_context *)(server->tls_pkey.pk_ctx))->key_index =
			FACTORYKEY_ARTIK_DEVICE;

	err = mbedtls_ssl_conf_own_cert(&server->tls_conf, &server->tls_srvcert,
			&server->tls_pkey);
	if (err) {
		printf("Failed to configure server certificate (err=%d)\n", err);
		ret = E_BAD_ARGS;
		goto exit;
	}

	mbedtls_ssl_conf_authmode(&server->tls_conf, MBEDTLS_SSL_VERIFY_NONE);
	server->tls_init = 1;

	if (http_tls_init(server, NULL)) {
		printf("Failed to initialize SSL configuration\n");
		http_server_release(&server);
		ret = E_BUSY;
		goto exit;
	}

exit:
	if (handle)
		security->release(handle);
	if (security)
		artik_release_api_module(security);
	if (server_cert)
		free(server_cert);
	if (ca_chain)
		free(ca_chain);

	return S_OK;
}
#endif

artik_error StartWebServer(bool start, enum ApiSet api_set)
{
	if (start) {
#ifndef WEBSERVER_DISABLE_HTTPS
		artik_error ret = S_OK;

		https_server = http_server_init(443);
#else
		https_server = http_server_init(80);
#endif
		if (!https_server) {
			printf("Failed to initialize web server\n");
			return E_BUSY;
		}

#ifndef WEBSERVER_DISABLE_HTTPS
		ret = ssl_init_context(https_server);
		if (ret != S_OK) {
			printf("Failed to initialize SSL context\n");
			http_server_release(&https_server);
			return ret;
		}
#endif
		if (api_set & API_SET_WIFI) {
			http_server_register_cb(https_server, HTTP_METHOD_GET, API_WIFI_ACCESS_POINTS, get_ap_callback);
			http_server_register_cb(https_server, HTTP_METHOD_POST, API_WIFI_CONFIG, set_config_callback);
		}
		if (api_set & API_SET_CLOUD) {
			http_server_register_cb(https_server, HTTP_METHOD_GET, API_CLOUD, get_akc_callback);
			http_server_register_cb(https_server, HTTP_METHOD_POST, API_CLOUD, set_akc_callback);
			http_server_register_cb(https_server, HTTP_METHOD_GET, API_CLOUD_REGISTRATION, get_akc_registration_callback);
			http_server_register_cb(https_server, HTTP_METHOD_PUT, API_CLOUD_REGISTRATION, put_akc_registration_callback);
		}

		if (http_server_start(https_server)) {
			printf("Failed to start web server\n");
			http_server_release(&https_server);
			return E_BUSY;
		}

		printf("Web server started\n");
	} else {
		if (!https_server) {
			printf("Web server is not started\n");
			return E_NOT_INITIALIZED;
		}

		if (api_set & API_SET_WIFI) {
			http_server_deregister_cb(https_server, HTTP_METHOD_GET, API_WIFI_ACCESS_POINTS);
			http_server_deregister_cb(https_server, HTTP_METHOD_POST, API_WIFI_CONFIG);
		}
		if (api_set & API_SET_CLOUD) {
			http_server_deregister_cb(https_server, HTTP_METHOD_GET, API_CLOUD);
			http_server_deregister_cb(https_server, HTTP_METHOD_POST, API_CLOUD);
			http_server_deregister_cb(https_server, HTTP_METHOD_GET, API_CLOUD_REGISTRATION);
			http_server_deregister_cb(https_server, HTTP_METHOD_PUT, API_CLOUD_REGISTRATION);
		}

		http_server_stop(https_server);
		http_server_release(&https_server);
		https_server = NULL;
		printf("Web server stopped\n");
	}

	return S_OK;
}
