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

#include <apps/netutils/webserver/http_server.h>
#include <apps/netutils/webserver/http_keyvalue_list.h>

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
 * We cannot enable HTTPs without a proper certificate, as iOS "AFNetworking"
 * library requires a non self-signed certificate to process HTTPs requests.
 */
//#define WEBSERVER_ENABLE_HTTPS

#ifdef WEBSERVER_ENABLE_HTTPS
/*
 * Self-signed certificate used when HTTPs webserver is enabled
 */
const char artik_srv_crt_rsa[] =
	"-----BEGIN CERTIFICATE-----\r\n"
	"MIICUzCCAbwCCQDhLm/KWqdqXzANBgkqhkiG9w0BAQUFADBuMQswCQYDVQQGEwJV\r\n"
	"UzETMBEGA1UECBMKQ2FsaWZvcm5pYTERMA8GA1UEBxMIU2FuIEpvc2UxEDAOBgNV\r\n"
	"BAoTB1NhbXN1bmcxDTALBgNVBAsTBFNTSUMxFjAUBgNVBAMTDVNhbXN1bmcgQVJU\r\n"
	"SUswHhcNMTcwMjI0MTczOTU3WhcNMTgwMjI0MTczOTU3WjBuMQswCQYDVQQGEwJV\r\n"
	"UzETMBEGA1UECBMKQ2FsaWZvcm5pYTERMA8GA1UEBxMIU2FuIEpvc2UxEDAOBgNV\r\n"
	"BAoTB1NhbXN1bmcxDTALBgNVBAsTBFNTSUMxFjAUBgNVBAMTDVNhbXN1bmcgQVJU\r\n"
	"SUswgZ8wDQYJKoZIhvcNAQEBBQADgY0AMIGJAoGBAPbypbbwrZ4+KZ79JjIHZOBW\r\n"
	"oxK5+0Cq+zsXdo09Ru1PYU7mGcJtfRsRNgg2TUhkMy6isK6YP+pINnkcPDW548iS\r\n"
	"0x1NsjF4oIocmRR0c/iKqnT3BALQYES3Av9vMnkK8Psee/T+kN8J2g4crIA/Oy6A\r\n"
	"b03g7jDpVgQQ8dDSpDTxAgMBAAEwDQYJKoZIhvcNAQEFBQADgYEAoFjdYkrRESsr\r\n"
	"YHNHSxu7xRY22dIpJ1s6cKGU8vVuxtR3iw0Rfb2oNZYvAFALm16wc9Q3BW5YICuD\r\n"
	"WRInSvLo3T8HXdSAh9EqBUlAWYcSu5+7859jAyfKXPX2kMIpr6FdZ/+YNaGhe2R+\r\n"
	"L1uHzH4cRPDdToa/wBN6POuFr2Uha88=\r\n"
	"-----END CERTIFICATE-----\r\n";

const char artik_srv_key_rsa[] =
	"-----BEGIN RSA PRIVATE KEY-----\r\n"
	"MIICXQIBAAKBgQD28qW28K2ePime/SYyB2TgVqMSuftAqvs7F3aNPUbtT2FO5hnC\r\n"
	"bX0bETYINk1IZDMuorCumD/qSDZ5HDw1uePIktMdTbIxeKCKHJkUdHP4iqp09wQC\r\n"
	"0GBEtwL/bzJ5CvD7Hnv0/pDfCdoOHKyAPzsugG9N4O4w6VYEEPHQ0qQ08QIDAQAB\r\n"
	"AoGBAKeAfODyrYEpfwfFhDPpCgaVHXWlSwRZNaGmT5aL2uE6t7FRqGVV6McETJcT\r\n"
	"ZiZVONZIKfmzcZ8TLkfF3B1BInvj6gcTbNHY3ThD2+SscV2UM7Kffg0fvRb45Bvo\r\n"
	"CN11N9dNhzrksFbHM/ugwM2W9kDIoDFT0/5f4XhlHTZ41O7FAkEA/NqsTR4IsQSf\r\n"
	"w6VIyjJaYmPOKezOLFUo82JBSPJ5JcJ7Jqs1se4g57pjDWS3MY6rspPfk4NlnxYx\r\n"
	"3Fkz2T778wJBAPoFKbFsYwbnliVxosNzhLqCig6yo+vKzPK2TiNduWIFazAVHUjx\r\n"
	"Y6KMYTiABNrnhhZfjTdNGZ+KFJcwzM6q+IsCQFoscf4ek+LtTzyqmUbipjEofsGc\r\n"
	"3foZdL03cSjesC2zKdAkhsWpsrpxUhnYrSxTQRAyhC1LUtZlR+rZNRBo0dkCQQD2\r\n"
	"FkLVabOGxkeZD2J/MqK/2WD92oNRwMk9VIdQdB3WBzNbnky20TGaesXukMM2m4/1\r\n"
	"UZNzjSXdTFG8BsJv4QTFAkBVuqG/aRs3yu0YEqP1WLmWLYeQPlCR851CZdcixafu\r\n"
	"LJq+FUtKgkzb7qctTd0eS3yL2hEQN1vZrYEolSJqC7EI\r\n"
	"-----END RSA PRIVATE KEY-----\r\n";

/*
 * Public key hash for SSL pinning:
 *
 * "sha256//w1qMo+N6/Njby2tpIc/5tG4CIMiAeC4hH/X9jgOEFMM"
 */
#endif

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

	printf("Start MDNS service\n");
	StartMDNSService(true);

	printf("Start webserver cloud API\n");
	if (StartWebServer(true, API_SET_CLOUD) != S_OK) {
		StartSoftAP(false);
		printf("Failed to start Web server\n");
		return NULL;
	}

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

		pthread_create(&tid, NULL, cloud_onboarding_start, NULL);
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

artik_error StartWebServer(bool start, enum ApiSet api_set)
{
	if (start) {
#ifdef WEBSERVER_ENABLE_HTTPS
		struct ssl_config_t ssl_config;

		https_server = http_server_init(443);
#else
		https_server = http_server_init(80);
#endif
		if (!https_server) {
			printf("Failed to initialize web server\n");
			return E_BUSY;
		}

#ifdef WEBSERVER_ENABLE_HTTPS
		memset(&ssl_config, 0, sizeof(ssl_config));
		ssl_config.auth_mode = MBEDTLS_SSL_VERIFY_NONE;
		ssl_config.dev_cert = (char *)artik_srv_crt_rsa;
		ssl_config.dev_cert_len  = sizeof(artik_srv_crt_rsa);
		ssl_config.private_key = (char *)artik_srv_key_rsa;
		ssl_config.private_key_len = sizeof(artik_srv_key_rsa);

		if (http_tls_init(https_server, &ssl_config)) {
			printf("Failed to initialize SSL configuration\n");
			http_server_release(&https_server);
			return E_BUSY;
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
