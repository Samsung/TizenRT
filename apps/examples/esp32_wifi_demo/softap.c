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

/* WiFi SoftAP Example
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

static const char *TAG = "AP";

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
	switch (event->event_id) {
	case SYSTEM_EVENT_AP_STACONNECTED:
		ESP_LOGI(TAG, "station:" MACSTR " join, AID=%d", MAC2STR(event->event_info.sta_connected.mac), event->event_info.sta_connected.aid);
		break;
	case SYSTEM_EVENT_AP_STADISCONNECTED:
		ESP_LOGI(TAG, "station:" MACSTR "leave, AID=%d", MAC2STR(event->event_info.sta_disconnected.mac), event->event_info.sta_disconnected.aid);
		break;
	default:
		break;
	}
	return ESP_OK;
}

/* Initialize Wi-Fi as sotfap mode */
static void wifi_init_softap(void)
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
	wifi_config_t wifi_config = {
		.ap = {
			.ssid = CONFIG_ESP_WIFI_SSID,
			.ssid_len = strlen(CONFIG_ESP_WIFI_SSID),
			.password = CONFIG_ESP_WIFI_PASSWORD,
			.max_connection = CONFIG_ESP_MAX_STA_CONN,
			.authmode = WIFI_AUTH_WPA_WPA2_PSK
		}
		,
	};
	if (strlen(CONFIG_ESP_WIFI_PASSWORD) == 0) {
		wifi_config.ap.authmode = WIFI_AUTH_OPEN;
	}
	ret = esp_wifi_set_mode(WIFI_MODE_AP);
	if (ret) {
		ESP_LOGI(TAG, "esp_wifi_set_mode failed, %d\n", ret);
		return;
	}
	ret = esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config);
	if (ret) {
		ESP_LOGI(TAG, "esp_wifi_set_config failed, %d\n", ret);
		return;
	}
	ret = esp_wifi_start();
	if (ret) {
		ESP_LOGI(TAG, "esp_wifi_set_start failed, %d\n", ret);
		return;
	}
	ESP_LOGI(TAG, "wifi_init_softap finished.SSID:%s password:%s", CONFIG_ESP_WIFI_SSID, CONFIG_ESP_WIFI_PASSWORD);
}

void wifi_softap_entry(void)
{
	ESP_LOGI(TAG, "ESP_WIFI_MODE_AP");
	wifi_init_softap();
}
