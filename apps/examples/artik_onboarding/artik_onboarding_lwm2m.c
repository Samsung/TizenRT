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
 * @file artik_onboarding_lwm2m.c
 */

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/boardctl.h>

#include <artik_module.h>
#include <artik_http.h>
#include <artik_lwm2m.h>

#include "artik_onboarding.h"

static const char akc_root_ca[] =
	"-----BEGIN CERTIFICATE-----\r\n"
	"MIIE0zCCA7ugAwIBAgIQGNrRniZ96LtKIVjNzGs7SjANBgkqhkiG9w0BAQUFADCB\r\n"
	"yjELMAkGA1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMR8wHQYDVQQL\r\n"
	"ExZWZXJpU2lnbiBUcnVzdCBOZXR3b3JrMTowOAYDVQQLEzEoYykgMjAwNiBWZXJp\r\n"
	"U2lnbiwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5MUUwQwYDVQQDEzxW\r\n"
	"ZXJpU2lnbiBDbGFzcyAzIFB1YmxpYyBQcmltYXJ5IENlcnRpZmljYXRpb24gQXV0\r\n"
	"aG9yaXR5IC0gRzUwHhcNMDYxMTA4MDAwMDAwWhcNMzYwNzE2MjM1OTU5WjCByjEL\r\n"
	"MAkGA1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMR8wHQYDVQQLExZW\r\n"
	"ZXJpU2lnbiBUcnVzdCBOZXR3b3JrMTowOAYDVQQLEzEoYykgMjAwNiBWZXJpU2ln\r\n"
	"biwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5MUUwQwYDVQQDEzxWZXJp\r\n"
	"U2lnbiBDbGFzcyAzIFB1YmxpYyBQcmltYXJ5IENlcnRpZmljYXRpb24gQXV0aG9y\r\n"
	"aXR5IC0gRzUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCvJAgIKXo1\r\n"
	"nmAMqudLO07cfLw8RRy7K+D+KQL5VwijZIUVJ/XxrcgxiV0i6CqqpkKzj/i5Vbex\r\n"
	"t0uz/o9+B1fs70PbZmIVYc9gDaTY3vjgw2IIPVQT60nKWVSFJuUrjxuf6/WhkcIz\r\n"
	"SdhDY2pSS9KP6HBRTdGJaXvHcPaz3BJ023tdS1bTlr8Vd6Gw9KIl8q8ckmcY5fQG\r\n"
	"BO+QueQA5N06tRn/Arr0PO7gi+s3i+z016zy9vA9r911kTMZHRxAy3QkGSGT2RT+\r\n"
	"rCpSx4/VBEnkjWNHiDxpg8v+R70rfk/Fla4OndTRQ8Bnc+MUCH7lP59zuDMKz10/\r\n"
	"NIeWiu5T6CUVAgMBAAGjgbIwga8wDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8E\r\n"
	"BAMCAQYwbQYIKwYBBQUHAQwEYTBfoV2gWzBZMFcwVRYJaW1hZ2UvZ2lmMCEwHzAH\r\n"
	"BgUrDgMCGgQUj+XTGoasjY5rw8+AatRIGCx7GS4wJRYjaHR0cDovL2xvZ28udmVy\r\n"
	"aXNpZ24uY29tL3ZzbG9nby5naWYwHQYDVR0OBBYEFH/TZafC3ey78DAJ80M5+gKv\r\n"
	"MzEzMA0GCSqGSIb3DQEBBQUAA4IBAQCTJEowX2LP2BqYLz3q3JktvXf2pXkiOOzE\r\n"
	"p6B4Eq1iDkVwZMXnl2YtmAl+X6/WzChl8gGqCBpH3vn5fJJaCGkgDdk+bW48DW7Y\r\n"
	"5gaRQBi5+MHt39tBquCWIMnNZBU4gcmU7qKEKQsTb47bDN0lAtukixlE0kF6BWlK\r\n"
	"WE9gyn6CagsCqiUXObXbf+eEZSqVir2G3l6BFoMtEMze/aiCKm0oHw0LxOXnGiYZ\r\n"
	"4fQRbxC1lfznQgUy286dUV4otp6F01vvpX1FQHKOtw5rDgb7MzVIcbidJ4vEZV8N\r\n"
	"hnacRHr2lVz2XTIIM6RUthg/aFzyQkqFOFSDX9HoLPKsEdao7WNq\r\n"
	"-----END CERTIFICATE-----\r\n";


#define OTA_FIRMWARE_HEADER_SIZE		4096
#define UUID_MAX_LEN					64
#define LWM2M_RES_DEVICE_REBOOT		"/3/0/4"
#define LWM2M_CONNECTION_MAX_RETRIES	5

struct ota_info {
	char header[OTA_FIRMWARE_HEADER_SIZE];
	size_t remaining_header_size;
	size_t offset;
	int fd;
};

static artik_lwm2m_handle g_lwm2m_handle;
static struct ota_info *g_ota_info;
struct Lwm2mState lwm2m_state;
static int g_lwm2m_connection_retries;

static void on_error(void *data, void *user_data)
{
	if (g_lwm2m_connection_retries++ < LWM2M_CONNECTION_MAX_RETRIES) {
		fprintf(stderr, "LWM2M error, reconnecting...\n");
		StartLwm2m(false);
		StartLwm2m(true);
	} else {
		fprintf(stderr, "Could not reconnect to LWM2M server, giving up\n");
	}
}

static pthread_addr_t delayed_reboot(pthread_addr_t arg)
{
	sleep(3);
	boardctl(BOARDIOC_RESET, EXIT_SUCCESS);

	return NULL;
}

static void reboot(void)
{
	pthread_t tid;

	pthread_create(&tid, NULL, delayed_reboot, NULL);
	pthread_detach(tid);
	fprintf(stderr, "Rebooting in 3 seconds\n");
}

static void on_execute_resource(void *data, void *user_data)
{
	char *uri = (char *)data;

	fprintf(stderr, "LWM2M resource execute: %s\n", uri);

	if (!strncmp(uri, LWM2M_RES_DEVICE_REBOOT, strlen(LWM2M_RES_DEVICE_REBOOT))) {
		reboot();
	}

	if (!strncmp(uri, ARTIK_LWM2M_URI_FIRMWARE_UPDATE, ARTIK_LWM2M_URI_LEN)) {
		artik_lwm2m_module *lwm2m = NULL;

		lwm2m = (artik_lwm2m_module *)artik_request_api_module("lwm2m");
		if (!lwm2m) {
			fprintf(stderr, "Failed to request LWM2M module\n");
			return;
		}
		lwm2m->client_write_resource(
			g_lwm2m_handle,
			ARTIK_LWM2M_URI_FIRMWARE_UPDATE_RES,
			(unsigned char *)ARTIK_LWM2M_FIRMWARE_UPD_RES_DEFAULT,
			strlen(ARTIK_LWM2M_FIRMWARE_UPD_RES_DEFAULT));
		lwm2m->client_write_resource(
			g_lwm2m_handle,
			ARTIK_LWM2M_URI_FIRMWARE_STATE,
			(unsigned char *)ARTIK_LWM2M_FIRMWARE_STATE_UPDATING,
			strlen(ARTIK_LWM2M_FIRMWARE_STATE_UPDATING));

		int fd =  open("/dev/mtdblock7", O_RDWR);

		write(fd, g_ota_info->header, OTA_FIRMWARE_HEADER_SIZE);
		close(fd);

		lwm2m_state.is_ota_update = 1;
		SaveConfiguration();
		reboot();
	}
}

static int write_firmware(char *data, size_t len, void *user_data)
{
	int header_size = 0;

	if (g_ota_info->remaining_header_size > 0) {
		header_size = len > g_ota_info->remaining_header_size ? g_ota_info->remaining_header_size : len;
		memcpy(g_ota_info->header + g_ota_info->offset, data, header_size);
		len -= header_size;
		g_ota_info->remaining_header_size -= header_size;
		g_ota_info->offset += header_size;
		printf("Skip OTA header (header_size %d, len %d, remaining_header_size %d)\n", header_size, len, g_ota_info->remaining_header_size);
	}

	if (len > 0)
		write(g_ota_info->fd, data+header_size, len);

	return len;
}

static int download_firmware(int argc, char *argv[])
{
	artik_lwm2m_module *lwm2m = (artik_lwm2m_module *)artik_request_api_module("lwm2m");
	artik_http_module *http = (artik_http_module *)artik_request_api_module("http");
	artik_ssl_config ssl_conf;
	int status = 0;

	artik_error ret;
	artik_http_headers headers;
	artik_http_header_field fields[] = {
		{ "User-Agent", "Artik Firmware Updater"}
	};

	g_ota_info = malloc(sizeof(struct ota_info));
	memset(g_ota_info, 0, sizeof(struct ota_info));
	g_ota_info->remaining_header_size = OTA_FIRMWARE_HEADER_SIZE;

	if (!lwm2m) {
		fprintf(stderr, "Failed to request LWM2M module\n");
		return 1;
	}

	if (!http) {
		lwm2m->client_write_resource(
			g_lwm2m_handle,
			ARTIK_LWM2M_URI_FIRMWARE_UPDATE_RES,
			(unsigned char *)ARTIK_LWM2M_FIRMWARE_UPD_RES_URI_ERR,
			strlen(ARTIK_LWM2M_FIRMWARE_UPD_RES_URI_ERR));

		lwm2m->client_write_resource(
			g_lwm2m_handle,
			ARTIK_LWM2M_URI_FIRMWARE_STATE,
			(unsigned char *)ARTIK_LWM2M_FIRMWARE_STATE_IDLE,
			strlen(ARTIK_LWM2M_FIRMWARE_STATE_IDLE));
		artik_release_api_module(lwm2m);
		return 1;
	}

	headers.fields = fields;
	headers.num_fields = ARRAY_SIZE(fields);
	memset(&ssl_conf, 0, sizeof(artik_ssl_config));
	ssl_conf.verify_cert = ARTIK_SSL_VERIFY_NONE;
	g_ota_info->fd = open("/dev/mtdblock7", O_RDWR);
	lseek(g_ota_info->fd, 4096, SEEK_SET);
	ret = http->get_stream(argv[1], &headers, &status, write_firmware, NULL, &ssl_conf);
	if (ret != S_OK) {
		lwm2m->client_write_resource(
			g_lwm2m_handle,
			ARTIK_LWM2M_URI_FIRMWARE_UPDATE_RES,
			(unsigned char *)ARTIK_LWM2M_FIRMWARE_UPD_RES_URI_ERR,
			strlen(ARTIK_LWM2M_FIRMWARE_UPD_RES_URI_ERR));

		lwm2m->client_write_resource(
			g_lwm2m_handle,
			ARTIK_LWM2M_URI_FIRMWARE_STATE,
			(unsigned char *)ARTIK_LWM2M_FIRMWARE_STATE_IDLE,
			strlen(ARTIK_LWM2M_FIRMWARE_STATE_IDLE));
		artik_release_api_module(lwm2m);
		artik_release_api_module(http);
		close(g_ota_info->fd);
		return 1;
	}

	lwm2m->client_write_resource(
		g_lwm2m_handle,
		ARTIK_LWM2M_URI_FIRMWARE_UPDATE_RES,
		(unsigned char *)ARTIK_LWM2M_FIRMWARE_UPD_RES_SUCCESS,
		strlen(ARTIK_LWM2M_FIRMWARE_UPD_RES_SUCCESS));

	lwm2m->client_write_resource(
		g_lwm2m_handle,
		ARTIK_LWM2M_URI_FIRMWARE_STATE,
		(unsigned char *)ARTIK_LWM2M_FIRMWARE_STATE_DOWNLOADED,
		strlen(ARTIK_LWM2M_FIRMWARE_STATE_DOWNLOADED));
	artik_release_api_module(lwm2m);
	artik_release_api_module(http);
	close(g_ota_info->fd);
	return 0;
}

static void on_changed_resource(void *data, void *user_data)
{
	artik_lwm2m_resource_t *res = (artik_lwm2m_resource_t *)data;

	fprintf(stderr, "LWM2M resource changed: %s\n", res->uri);
	if (!strncmp(res->uri, ARTIK_LWM2M_URI_FIRMWARE_PACKAGE_URI, ARTIK_LWM2M_URI_LEN)) {
		artik_lwm2m_module *lwm2m = NULL;
		char *firmware_uri;
		char *argv[2] = { NULL };

		lwm2m = (artik_lwm2m_module *)artik_request_api_module("lwm2m");
		if (!lwm2m) {
			fprintf(stderr, "Failed to request LWM2M module\n");
			return;
		}

		/* Initialize attribute "Update Result" */
		lwm2m->client_write_resource(
				g_lwm2m_handle,
				ARTIK_LWM2M_URI_FIRMWARE_UPDATE_RES,
				(unsigned char *)ARTIK_LWM2M_FIRMWARE_UPD_RES_DEFAULT,
				strlen(ARTIK_LWM2M_FIRMWARE_UPD_RES_DEFAULT));

		/* Change attribute "State" to "Downloading" */
		lwm2m->client_write_resource(
				g_lwm2m_handle,
				ARTIK_LWM2M_URI_FIRMWARE_STATE,
				(unsigned char *)ARTIK_LWM2M_FIRMWARE_STATE_DOWNLOADING,
				strlen(ARTIK_LWM2M_FIRMWARE_STATE_DOWNLOADING));

		/* The FW URI is empty, come back to IDLE */
		if (res->length == 0) {
			lwm2m->client_write_resource(
				g_lwm2m_handle,
				ARTIK_LWM2M_URI_FIRMWARE_STATE,
				(unsigned char *)ARTIK_LWM2M_FIRMWARE_STATE_IDLE,
				strlen(ARTIK_LWM2M_FIRMWARE_STATE_IDLE));
			return;
		}


		/* The FW URI is not valid */
		if (res->length > 255) {
			fprintf(stderr, "ERROR: Unable to retrieve firmware package uri\n");

			lwm2m->client_write_resource(
				g_lwm2m_handle,
				ARTIK_LWM2M_URI_FIRMWARE_UPDATE_RES,
				(unsigned char *)ARTIK_LWM2M_FIRMWARE_UPD_RES_URI_ERR,
				strlen(ARTIK_LWM2M_FIRMWARE_UPD_RES_URI_ERR));

			lwm2m->client_write_resource(
				g_lwm2m_handle,
				ARTIK_LWM2M_URI_FIRMWARE_STATE,
				(unsigned char *)ARTIK_LWM2M_FIRMWARE_STATE_IDLE,
				strlen(ARTIK_LWM2M_FIRMWARE_STATE_IDLE));

			return;
		}

		firmware_uri = strndup((char *)res->buffer, res->length);
		fprintf(stdout, "Downloading firmware from %s\n", firmware_uri);
		argv[0] = firmware_uri;
		task_create("download-firmware", SCHED_PRIORITY_DEFAULT, 16384, download_firmware, argv);
		artik_release_api_module(lwm2m);
	}
}

static void finish_ota(void)
{
	artik_lwm2m_module *lwm2m = (artik_lwm2m_module *)artik_request_api_module("lwm2m");

	lwm2m->client_write_resource(
		g_lwm2m_handle,
		ARTIK_LWM2M_URI_FIRMWARE_UPDATE_RES,
		(unsigned char *)ARTIK_LWM2M_FIRMWARE_UPD_RES_SUCCESS,
		strlen(ARTIK_LWM2M_FIRMWARE_UPD_RES_SUCCESS));
	lwm2m->client_write_resource(
		g_lwm2m_handle,
		ARTIK_LWM2M_URI_FIRMWARE_STATE,
		(unsigned char *)ARTIK_LWM2M_FIRMWARE_STATE_IDLE,
		strlen(ARTIK_LWM2M_FIRMWARE_STATE_IDLE));
	lwm2m->client_write_resource(
		g_lwm2m_handle,
		ARTIK_LWM2M_URI_DEVICE_FW_VERSION,
		(unsigned char *)ONBOARDING_VERSION,
		strlen(ONBOARDING_VERSION));

	lwm2m_state.is_ota_update = 0;
	SaveConfiguration();
}

static pthread_addr_t lwm2m_start_cb(void *arg)
{
	artik_error ret = S_OK;
	artik_lwm2m_config config;
	artik_lwm2m_module *lwm2m = (artik_lwm2m_module *)artik_request_api_module("lwm2m");
	artik_ssl_config ssl_config;

	if (!lwm2m) {
		printf("Failed to request lwm2m module\n");
		ret = E_NOT_SUPPORTED;
		pthread_exit((void *)ret);
		return NULL;
	}

	printf("Start LWM2M connection to ARTIK Cloud\n");

	memset(&config, 0, sizeof(artik_lwm2m_config));
	memset(&ssl_config, 0, sizeof(artik_ssl_config));
	config.server_id = 123;
	config.server_uri = "coaps+tcp://coaps-api.artik.cloud:5689";
	config.lifetime = 30;
	config.name = strndup(cloud_config.device_id, UUID_MAX_LEN);
	config.tls_psk_identity = config.name;
	config.tls_psk_key = strndup(cloud_config.device_token, UUID_MAX_LEN);
	config.objects[ARTIK_LWM2M_OBJECT_DEVICE] =
		lwm2m->create_device_object("Samsung", "ARTIK05x", "1234567890", ONBOARDING_VERSION,
			"1.0", "1.0", "A05x", 0, 5000, 1500, 100, 1000000, 200000,
			"Europe/Paris", "+01:00", "U");

	ssl_config.use_se = cloud_secure_dt;
	ssl_config.ca_cert.data = (char *)akc_root_ca;
	ssl_config.ca_cert.len = sizeof(akc_root_ca);
	ssl_config.verify_cert = ARTIK_SSL_VERIFY_REQUIRED;
	config.ssl_config = &ssl_config;
	if (!config.objects[ARTIK_LWM2M_OBJECT_DEVICE]) {
		fprintf(stderr, "Failed to allocate memory for object device.");
		ret = E_NO_MEM;
		goto exit;
	}

	config.objects[ARTIK_LWM2M_OBJECT_FIRMWARE] =
		lwm2m->create_firmware_object(false, "Artik/TizenRT/OnBoarding", ONBOARDING_VERSION);
	if (!config.objects[ARTIK_LWM2M_OBJECT_FIRMWARE]) {
		fprintf(stderr, "Failed to allocate memory for object firmware.");
		ret = E_NO_MEM;
		goto exit;
	}

	ret = lwm2m->client_request(&g_lwm2m_handle, &config);
	if (ret != S_OK) {
		fprintf(stderr, "Failed to request LWM2M client handle (%d)\n", ret);
		goto exit;
	}

	ret = lwm2m->client_connect(g_lwm2m_handle);
	if (ret != S_OK) {
		fprintf(stderr, "Failed to connect to the DM server (%d)\n", ret);
		goto exit;
	}

	lwm2m->set_callback(g_lwm2m_handle, ARTIK_LWM2M_EVENT_ERROR,
			on_error, (void *)g_lwm2m_handle);
	lwm2m->set_callback(g_lwm2m_handle, ARTIK_LWM2M_EVENT_RESOURCE_EXECUTE,
			on_execute_resource, (void *)g_lwm2m_handle);
	lwm2m->set_callback(g_lwm2m_handle, ARTIK_LWM2M_EVENT_RESOURCE_CHANGED,
			on_changed_resource, (void *)g_lwm2m_handle);

	if (lwm2m_state.is_ota_update) {
		finish_ota();
	}

exit:
	if (config.objects[ARTIK_LWM2M_OBJECT_DEVICE])
		lwm2m->free_object(config.objects[ARTIK_LWM2M_OBJECT_DEVICE]);

	if (config.objects[ARTIK_LWM2M_OBJECT_FIRMWARE])
		lwm2m->free_object(config.objects[ARTIK_LWM2M_OBJECT_FIRMWARE]);

	if (config.name)
		free(config.name);

	if (config.tls_psk_key)
		free(config.tls_psk_key);

	if (g_lwm2m_handle && ret != S_OK) {
		lwm2m->client_release(g_lwm2m_handle);
		g_lwm2m_handle = NULL;
	}

	pthread_exit((void *)ret);

	return NULL;
}

static pthread_addr_t lwm2m_stop_cb(void *arg)
{
	artik_error ret = S_OK;
	artik_lwm2m_module *lwm2m = (artik_lwm2m_module *)artik_request_api_module("lwm2m");

	if (!lwm2m) {
		printf("Failed to request lwm2m module\n");
		ret = E_NOT_SUPPORTED;
		pthread_exit((void *)ret);
		return NULL;
	}

	lwm2m->client_disconnect(g_lwm2m_handle);
	lwm2m->client_release(g_lwm2m_handle);
	g_lwm2m_handle = NULL;
	pthread_exit((void *)ret);

	return NULL;
}

artik_error StartLwm2m(bool start)
{
	artik_error ret = S_OK;

	if (start) {
		static pthread_t tid;
		pthread_attr_t attr;
		int status;
		struct sched_param sparam;

		if (g_lwm2m_handle) {
			fprintf(stderr, "LWM2M is already started, stop it first\n");
			ret = E_BUSY;
			goto exit;
		}

		pthread_attr_init(&attr);
		sparam.sched_priority = 100;
		status = pthread_attr_setschedparam(&attr, &sparam);
		status = pthread_attr_setschedpolicy(&attr, SCHED_RR);
		status = pthread_attr_setstacksize(&attr, 1024 * 8);
		status = pthread_create(&tid, &attr, lwm2m_start_cb, NULL);
		if (status) {
			printf("Failed to create thread for starting lwm2m\n");
			ret = E_NO_MEM;
			goto exit;
		}

		pthread_setname_np(tid, "onboarding-lwm2m");
		pthread_join(tid, (void **)ret);

		if (ret == S_OK)
			g_lwm2m_connection_retries = 0;

	} else {
		static pthread_t tid;
		pthread_attr_t attr;
		int status;
		struct sched_param sparam;

		if (!g_lwm2m_handle) {
			fprintf(stderr, "LWM2M is already stopped, start it first\n");
			ret = E_NOT_INITIALIZED;
			goto exit;
		}

		pthread_attr_init(&attr);
		sparam.sched_priority = 100;
		status = pthread_attr_setschedparam(&attr, &sparam);
		status = pthread_attr_setschedpolicy(&attr, SCHED_RR);
		status = pthread_attr_setstacksize(&attr, 1024 * 8);
		status = pthread_create(&tid, &attr, lwm2m_stop_cb, NULL);
		if (status) {
			printf("Failed to create thread for stopping lwm2m\n");
			ret = E_NO_MEM;
			goto exit;
		}

		pthread_setname_np(tid, "onboarding-lwm2m");
		pthread_join(tid, (void **)ret);
	}

exit:
	return ret;
}

void Lwm2mResetConfig(void)
{
	lwm2m_state.is_ota_update = 0;
}
