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
 * @file artik_onboarding_cloud.c
 */


#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <artik_module.h>
#include <artik_error.h>
#include <artik_cloud.h>
#include <artik_gpio.h>
#include <artik_platform.h>
#include <artik_security.h>
#include <cJSON.h>

#include "artik_onboarding.h"

#define AKC_DEFAULT_DTID    "dt2d93bdb9c8fa446eb4a35544e66150f7"
#define LED_ID              ARTIK_A053_XGPIO20

#define RESP_ERROR_TPL      RESP_ERROR("%d", "%s")
#define RESP_INVALID_TPL    RESP_ERROR(API_ERROR_COMMUNICATION, "Device Communication Error")
#define RESP_UNAVAIL_TPL    RESP_ERROR(API_ERROR_INTERNAL, "Internal Software Module error")
#define RESP_PIN_TPL        RESP_ERROR_EXTRA(API_ERROR_OK, "none", "\"pin\":\"%s\"")

#define WEBSOCKET_REGISTER_TIMEOUT  30  /* seconds */

struct ArtikCloudConfig cloud_config;

static artik_websocket_handle g_ws_handle;
static sem_t g_sem_ws_registered;
static bool g_ws_registration_result;
bool cloud_secure_dt;

void CloudResetConfig(bool reset_dtid)
{
	strncpy(cloud_config.device_id, "null", AKC_DID_LEN);
	strncpy(cloud_config.device_token, "null", AKC_TOKEN_LEN);
	strncpy(cloud_config.reg_id, "", AKC_REG_ID_LEN);
	strncpy(cloud_config.reg_nonce, "", AKC_REG_NONCE_LEN);

	if (reset_dtid)
		strncpy(cloud_config.device_type_id, AKC_DEFAULT_DTID, AKC_DTID_LEN);
}

static void set_led_state(bool state)
{
	artik_gpio_config config;
	artik_gpio_handle handle;
	artik_gpio_module *gpio = (artik_gpio_module *)artik_request_api_module("gpio");
	char name[16] = "";

	if (!gpio)
		return;

	memset(&config, 0, sizeof(config));
	config.dir = GPIO_OUT;
	config.id = LED_ID;
	snprintf(name, 16, "gpio%d", config.id);
	config.name = name;

	if (gpio->request(&handle, &config) != S_OK)
		return;

	gpio->write(handle, state ? 1 : 0);
	gpio->release(handle);

	artik_release_api_module(gpio);
}

static pthread_addr_t wifi_onboarding_start(pthread_addr_t arg)
{
	StartCloudWebsocket(false);
	StartLwm2m(false);

	if (StartSoftAP(true) != S_OK) {
		return NULL;
	}

	if (StartWebServer(true, API_SET_WIFI) != S_OK) {
		StartSoftAP(false);
		return NULL;
	}

	printf("ARTIK Onboarding Service started\n");
	current_service_state = STATE_ONBOARDING;

	return NULL;
}

static void cloud_websocket_rx_cb(void *user_data, void *result)
{
	cJSON *msg, *error, *code, *type, *data, *actions, *action;

	if (!result)
		return;

	/* Parse JSON and look for actions */
	msg = cJSON_Parse((const char *)result);
	if (!msg)
		goto out;

	/* Check for registration response */
	data = cJSON_GetObjectItem(msg, "data");
	if (data && (data->type == cJSON_Object)) {
		code = cJSON_GetObjectItem(data, "code");
		if (code && (code->type == cJSON_String)) {
			if (!strncmp(code->valuestring, "200", strlen("200"))) {
				g_ws_registration_result = true;
				sem_post(&g_sem_ws_registered);
				goto exit;
			}
		}
	}

	/* Check if error */
	error = cJSON_GetObjectItem(msg, "error");
	if (error && (error->type == cJSON_Object)) {
		code = cJSON_GetObjectItem(error, "code");
		if (code && (code->type == cJSON_Number)) {
			data = cJSON_GetObjectItem(error, "message");
			if (data && (data->type == cJSON_String)) {
				printf("Websocket error %d - %s\n", code->valueint,
				       data->valuestring);
				if (code->valueint == 404) {
					g_ws_registration_result = false;
					sem_post(&g_sem_ws_registered);
				}
				goto exit;
			}
		}
	}

	/* Check if we received an action */
	type = cJSON_GetObjectItem(msg, "type");
	if (!type || (type->type != cJSON_String)) {
		goto exit;
	}

	if (strncmp(type->valuestring, "action", strlen("action"))) {
		goto exit;
	}

	data = cJSON_GetObjectItem(msg, "data");
	if (!data || (data->type != cJSON_Object)) {
		goto exit;
	}

	actions = cJSON_GetObjectItem(data, "actions");
	if (!actions || actions->type != cJSON_Array) {
		goto exit;
	}

	for (action = (actions)->child; action != NULL; action = action->next) {
		cJSON *name;

		if (action->type != cJSON_Object) {
			continue;
		}

		name = cJSON_GetObjectItem(action, "name");
		if (!name || (name->type != cJSON_String)) {
			continue;
		}

		if (!strncmp(name->valuestring, "setOn", strlen("setOn"))) {
			fprintf(stderr, "CLOUD ACTION: SetOn\n");
			SendMessageToCloud("{\"state\":true}");
			set_led_state(true);
		} else if (!strncmp(name->valuestring, "setOff", strlen("setOff"))) {
			fprintf(stderr, "CLOUD ACTION: SetOff\n");
			SendMessageToCloud("{\"state\":false}");
			set_led_state(false);
		}
	}
exit:
	cJSON_Delete(msg);
out:
	free(result);
}

static pthread_addr_t websocket_start_cb(void *arg)
{
	artik_cloud_module *cloud = (artik_cloud_module *)artik_request_api_module("cloud");
	artik_error ret = S_OK;
	artik_ssl_config ssl;

	if (!cloud) {
		printf("Cloud module is not available\n");
		ret = E_NOT_SUPPORTED;
		pthread_exit((void *)ret);
		return NULL;
	}

	printf("Start websocket to ARTIK Cloud\n");

	memset(&ssl, 0, sizeof(ssl));
	ssl.use_se = cloud_secure_dt;

	ret = cloud->websocket_open_stream(&g_ws_handle, cloud_config.device_token,
					   cloud_config.device_id, &ssl);
	if (ret != S_OK) {
		printf("Failed to open websocket to cloud (err=%d)\n", ret);
		goto exit;
	}

	cloud->websocket_set_receive_callback(g_ws_handle, cloud_websocket_rx_cb, NULL);
	current_service_state = STATE_CONNECTED;

exit:
	pthread_exit((void *)ret);
	artik_release_api_module(cloud);
	return NULL;
}

static pthread_addr_t websocket_stop_cb(void *arg)
{
	int ret = 0;
	artik_cloud_module *cloud = (artik_cloud_module *)artik_request_api_module("cloud");

	if (!cloud) {
		printf("Cloud module is not available\n");
		ret = E_NOT_SUPPORTED;
		goto exit;
	}

	cloud->websocket_set_receive_callback(g_ws_handle, NULL, NULL);

	ret = cloud->websocket_close_stream(g_ws_handle);
	if (ret != S_OK) {
		artik_release_api_module(cloud);
		printf("Failed to close websocket to cloud (err=%d)\n", ret);
		goto exit;
	}

	g_ws_handle = NULL;
	artik_release_api_module(cloud);
	current_service_state = STATE_IDLE;

exit:
	return NULL;
}

artik_error StartCloudWebsocket(bool start)
{
	artik_error ret = S_OK;
	int num_retries = 5;

	if (start) {
		static pthread_t tid;
		pthread_attr_t attr;
		int status;
		struct sched_param sparam;
		struct timespec timeout;

		if (g_ws_handle) {
			printf("Websocket is already open, close it first\n");
			goto exit;
		}

		g_ws_registration_result = false;
		sem_init(&g_sem_ws_registered, 0, 0);

retry:
		pthread_attr_init(&attr);
		sparam.sched_priority = 100;
		status = pthread_attr_setschedparam(&attr, &sparam);
		status = pthread_attr_setschedpolicy(&attr, SCHED_RR);
		status = pthread_attr_setstacksize(&attr, 1024 * 8);
		status = pthread_create(&tid, &attr, websocket_start_cb, NULL);
		if (status) {
			sem_destroy(&g_sem_ws_registered);
			printf("Failed to create thread for websocket\n");
			ret = E_NO_MEM;
			goto exit;
		}

		pthread_attr_destroy(&attr);
		pthread_setname_np(tid, "cloud-websocket");
		pthread_join(tid, (void **)&ret);

		if ((ret != S_OK) && num_retries--) {
			printf("Failed to connect to websocket (%d), retrying...\n", ret);
			goto retry;
		}

		if (num_retries < 1) {
			sem_destroy(&g_sem_ws_registered);
			ret = E_WEBSOCKET_ERROR;
			goto exit;
		}

		/* Wait for registration status message */
		clock_gettime(CLOCK_REALTIME, &timeout);
		timeout.tv_sec += WEBSOCKET_REGISTER_TIMEOUT;
		if (sem_timedwait(&g_sem_ws_registered, &timeout) == -1) {
			sem_destroy(&g_sem_ws_registered);
			StartCloudWebsocket(false);
			printf("Failure while waiting websocket to register (err=%d)\n", errno);
			ret = E_ACCESS_DENIED;
			goto exit;
		}

		if (!g_ws_registration_result) {
			sem_destroy(&g_sem_ws_registered);
			StartCloudWebsocket(false);
			printf("Websocket failed to register AKC device\n");
			ret = E_ACCESS_DENIED;
			goto exit;
		}

		sem_destroy(&g_sem_ws_registered);

		printf("Websocket successfully connected\n");

	} else {
		static pthread_t tid;
		pthread_attr_t attr;
		int status;
		struct sched_param sparam;

		if (!g_ws_handle) {
			printf("Websocket is not open, cannot close it\n");
			goto exit;
		}

		pthread_attr_init(&attr);
		sparam.sched_priority = 100;
		status = pthread_attr_setschedparam(&attr, &sparam);
		status = pthread_attr_setschedpolicy(&attr, SCHED_RR);
		status = pthread_attr_setstacksize(&attr, 1024 * 4);
		status = pthread_create(&tid, &attr, websocket_stop_cb, NULL);
		if (status) {
			ret = E_ACCESS_DENIED;
			printf("Failed to create thread for closing websocket\n");
			goto exit;
		}

		pthread_join(tid, NULL);
		pthread_attr_destroy(&attr);
	}

exit:
	return ret;
}

artik_error SendMessageToCloud(char *message)
{
	artik_error ret = S_OK;
	artik_cloud_module *cloud = (artik_cloud_module *)artik_request_api_module("cloud");

	if (!cloud) {
		printf("Cloud module is not available\n");
		return E_NOT_SUPPORTED;
	}

	if (!g_ws_handle) {
		printf("Websocket is not open, cannot send message\n");
		goto exit;
	}

	ret = cloud->websocket_send_message(g_ws_handle, message);
	if (ret != S_OK) {
		printf("Failed to send message to cloud (err=%d)\n", ret);
		goto exit;
	}

exit:
	artik_release_api_module(cloud);
	return ret;
}

static pthread_addr_t start_sdr_registration_cb(void *arg)
{
	int status = 200;
	char **resp = (char **)arg;
	char *response = NULL;
	cJSON *jresp = NULL;
	unsigned char serial_num[ARTIK_CERT_SN_MAXLEN];
	unsigned int sn_len = ARTIK_CERT_SN_MAXLEN;
	char vdid[AKC_VDID_LEN + 1] = "";
	int i = 0;
	artik_cloud_module *cloud = (artik_cloud_module *)artik_request_api_module("cloud");
	artik_security_module *security = (artik_security_module *)artik_request_api_module("security");
	artik_security_handle handle;
	artik_error ret = S_OK;

	if (!cloud || !security) {
		status = 500;
		*resp = strdup(RESP_UNAVAIL_TPL);
		goto exit;
	}

	/* Generate the vendor ID based on the certificate serial number */
	security->request(&handle);
	ret = security->get_certificate_sn(handle, serial_num, &sn_len);
	if ((ret != S_OK) || !sn_len) {
		strcpy(vdid, "vdid_default1234");
	} else {
		for (i = 0; i < sn_len; i++) {
			char tmp[3];

			snprintf(tmp, 3, "%02x", serial_num[i]);
			tmp[2] = '\0';
			strncat(vdid, tmp, 2);
		}
	}

	security->release(handle);

	/* Start SDR registration to ARTIK Cloud */
	ret = cloud->sdr_start_registration(cloud_config.device_type_id, vdid, &response);
	if (ret != S_OK) {
		if (response) {
			cJSON *error, *message, *code;

			/* We got an error from the cloud, parse the JSON to extract the error message */
			jresp = cJSON_Parse(response);
			if (!jresp) {
				goto fail_parse;
			}

			error = cJSON_GetObjectItem(jresp, "error");
			if (!error || (error->type != cJSON_Object)) {
				goto fail_parse;
			}

			message = cJSON_GetObjectItem(error, "message");
			if (!message || (message->type != cJSON_String)) {
				goto fail_parse;
			}

			code = cJSON_GetObjectItem(error, "code");
			if (!code || (code->type == cJSON_Number)) {
				goto fail_parse;
			}

			int len = strlen(RESP_ERROR_TPL) + 5 + strlen(message->valuestring);
			*resp = zalloc(len + 1);
			if (*resp == NULL) {
				goto fail_parse;
			}

			snprintf(*resp, len, RESP_ERROR_TPL, API_ERROR_CLOUD_BASE + code->valueint);
			goto exit;
		}
	} else {
		cJSON *data, *rid, *pin, *nonce;

		if (!response) {
			goto fail_parse;
		}

		jresp = cJSON_Parse(response);
		if (!jresp) {
			goto fail_parse;
		}

		data = cJSON_GetObjectItem(jresp, "data");
		if (!data) {
			goto fail_parse;
		}

		rid = cJSON_GetObjectItem(data, "rid");
		if (!rid || (rid->type != cJSON_String)) {
			goto fail_parse;
		}

		strncpy(cloud_config.reg_id, rid->valuestring, AKC_REG_ID_LEN);
		nonce = cJSON_GetObjectItem(data, "nonce");
		if (!nonce || (nonce->type != cJSON_String)) {
			goto fail_parse;
		}

		strncpy(cloud_config.reg_nonce, nonce->valuestring, AKC_REG_ID_LEN);
		pin = cJSON_GetObjectItem(data, "pin");
		if (!pin || (pin->type != cJSON_String)) {
			goto fail_parse;
		}

		int len = strlen(RESP_PIN_TPL) + strlen(pin->valuestring);
		*resp = zalloc(len + 1);
		if (*resp == NULL) {
			goto fail_parse;
		}

		snprintf(*resp, len, RESP_PIN_TPL, pin->valuestring);
		goto exit;
	}

fail_parse:
	/* Fallback here if we could not parse everything we were looking for */
	status = 500;
	*resp = strdup(RESP_INVALID_TPL);

exit:

	pthread_exit((void *)status);

	if (jresp)
		cJSON_Delete(jresp);

	if (response)
		free(response);

	if (cloud)
		artik_release_api_module(cloud);

	return NULL;
}

int StartSDRRegistration(char **resp)
{
	int status = 0;
	static pthread_t tid;
	pthread_attr_t attr;
	struct sched_param sparam;

	pthread_attr_init(&attr);
	sparam.sched_priority = 100;
	status = pthread_attr_setschedparam(&attr, &sparam);
	status = pthread_attr_setschedpolicy(&attr, SCHED_RR);
	status = pthread_attr_setstacksize(&attr, 1024 * 4);
	status = pthread_create(&tid, &attr, start_sdr_registration_cb, (void *)resp);
	if (status)
		goto exit;

	pthread_join(tid, (void **)&status);
	pthread_attr_destroy(&attr);

exit:
	return status;
}

static pthread_addr_t complete_sdr_registration_cb(void *arg)
{
	int status = 200;
	char **resp = (char **)arg;
	char *response = NULL;
	cJSON *jresp = NULL;
	artik_cloud_module *cloud = (artik_cloud_module *)artik_request_api_module("cloud");
	artik_error ret = S_OK;

	if (!cloud) {
		status = 500;
		*resp = strdup(RESP_UNAVAIL_TPL);
		goto exit;
	}

	/* Start SDR registration to ARTIK Cloud */
	ret = cloud->sdr_complete_registration(cloud_config.reg_id, cloud_config.reg_nonce, &response);
	if (ret != S_OK) {
		if (response) {
			cJSON *error, *message, *code;

			/* We got an error from the cloud, parse the JSON to extract the error message */
			jresp = cJSON_Parse(response);
			if (!jresp) {
				goto fail_parse;
			}

			error = cJSON_GetObjectItem(jresp, "error");
			if (!error || (error->type != cJSON_Object)) {
				goto fail_parse;
			}

			message = cJSON_GetObjectItem(error, "message");
			if (!message || (message->type != cJSON_String)) {
				goto fail_parse;
			}

			code = cJSON_GetObjectItem(error, "code");
			if (!code || (code->type != cJSON_Number)) {
				goto fail_parse;
			}

			int len = strlen(RESP_ERROR_TPL) + 5 + strlen(message->valuestring);

			status = 400;
			*resp = zalloc(len + 1);
			if (*resp == NULL) {
				goto fail_parse;
			}

			snprintf(*resp, len, RESP_ERROR_TPL, API_ERROR_CLOUD_BASE + code->valueint,
						message->valuestring);
			goto exit;
		}
	} else {
		if (response) {
			cJSON *data, *token, *did;

			/* It's all good. Parse Device token and ID for later use */
			jresp = cJSON_Parse(response);
			if (!jresp) {
				goto fail_parse;
			}

			data = cJSON_GetObjectItem(jresp, "data");
			if (!data) {
				goto fail_parse;
			}

			token = cJSON_GetObjectItem(data, "accessToken");
			if (!token || (token->type != cJSON_String)) {
				goto fail_parse;
			}

			strncpy(cloud_config.device_token, token->valuestring, AKC_TOKEN_LEN);
			did = cJSON_GetObjectItem(data, "did");
			if (!did || (did->type != cJSON_String)) {
				goto fail_parse;
			}

			strncpy(cloud_config.device_id, did->valuestring, AKC_DID_LEN);
			*resp = strdup(RESP_ERROR_OK);
			goto exit;
		}
	}

fail_parse:
	/* Fallback here if we could not parse everything we were looking for */
	status = 500;
	*resp = strdup(RESP_INVALID_TPL);

exit:

	pthread_exit((void *)status);

	if (jresp)
		cJSON_Delete(jresp);

	if (response)
		free(response);

	if (cloud)
		artik_release_api_module(cloud);

	return NULL;
}

int CompleteSDRRegistration(char **resp)
{
	int status = 0;
	static pthread_t tid;
	pthread_attr_t attr;
	struct sched_param sparam;

	pthread_attr_init(&attr);
	sparam.sched_priority = 100;
	status = pthread_attr_setschedparam(&attr, &sparam);
	status = pthread_attr_setschedpolicy(&attr, SCHED_RR);
	status = pthread_attr_setstacksize(&attr, 1024 * 4);
	status = pthread_create(&tid, &attr, complete_sdr_registration_cb, (void *)resp);
	if (status)
		goto exit;

	pthread_join(tid, (void **)&status);
	pthread_attr_destroy(&attr);

exit:
	return status;
}

bool CloudIsSecureDeviceType(const char *dtid)
{
	/* It would be better to read the device type
	 * characteristics from the cloud to figure
	 * out if the DT is SDR-enabled. For now
	 * we just consider the default DTID is
	 * the only one considered as secure.
	 */
	return !strncmp(dtid, AKC_DEFAULT_DTID, AKC_DTID_LEN);
}
