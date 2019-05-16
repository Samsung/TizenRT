/******************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

/* WiFi station Example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <tinyara/config.h>

#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event_loop.h"
#include "esp_wifi_internal.h"

/*
 The examples use WiFi configuration that you can set via 'make menuconfig'.
*/

static const char *TAG = "station";
static int s_retry_num = 0;

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
	switch (event->event_id) {
	case SYSTEM_EVENT_STA_START:
		ESP_LOGI(TAG, "SYSTEM_EVENT_STA_START");
		if (esp_wifi_connect()) {
			ESP_LOGI(TAG, "esp_wifi_connect failed\n");
		}
		break;
	case SYSTEM_EVENT_STA_GOT_IP:
		ESP_LOGI(TAG, "SYSTEM_EVENT_STA_GOT_IP");
		ESP_LOGI(TAG, "Got IP: %s\n", ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
		break;
	case SYSTEM_EVENT_STA_DISCONNECTED:
		ESP_LOGI(TAG, "SYSTEM_EVENT_STA_DISCONNECTED");
		if (s_retry_num < CONFIG_ESP_MAXIMUM_RETRY) {
			esp_wifi_connect();
			s_retry_num++;
			ESP_LOGI(TAG, "retry to connect to the AP");
		}
		ESP_LOGI(TAG, "connect to the AP fail\n");
		break;
	default:
		break;
	}
	return ESP_OK;
}

/* Initialize Wi-Fi as sta*/
static void do_wifi_scan(void)
{
	esp_err_t ret;
	tcpip_adapter_init();
	ret = esp_event_loop_init(event_handler, NULL);
	if (ret) {
		ESP_LOGI(TAG, "esp_event_loop_init failed, %d\n", ret);
		return;
	}

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ret = esp_wifi_init(&cfg);
	if (ret) {
		ESP_LOGI(TAG, "esp_wifi_init failed, %d\n", ret);
		return;
	}
	esp_wifi_internal_set_log_level(WIFI_LOG_VERBOSE);
	esp_wifi_internal_set_log_mod(WIFI_LOG_MODULE_ALL, WIFI_LOG_MODULE_ALL | WIFI_LOG_SUBMODULE_INIT | WIFI_LOG_SUBMODULE_IOCTL | WIFI_LOG_SUBMODULE_CONN | WIFI_LOG_SUBMODULE_SCAN, true);

	wifi_config_t wifi_config = {
		.sta = {
			.ssid = CONFIG_ESP_WIFI_SSID,
			.password = CONFIG_ESP_WIFI_PASSWORD,
		}
		,
	};
	ret = esp_wifi_set_mode(WIFI_MODE_STA);
	if (ret) {
		ESP_LOGI(TAG, "esp_wifi_set_mode failed, %d\n", ret);
		return;
	}
	ret = esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
	if (ret) {
		ESP_LOGI(TAG, "esp_wifi_set_config failed, %d\n", ret);
		return;
	}
	ret = esp_wifi_start();
	if (ret) {
		return;
	}
}

void wifi_station_entry(void)
{
	ESP_LOGI(TAG, "ESP_WIFI_MODE_STATION");
	do_wifi_scan();
}
