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
 * @file artik_onboarding.c
 */

#include <stdio.h>
#include <sys/stat.h>
#include <tinyara/config.h>
#include <tinyara/fs/fs_utils.h>
#include <shell/tash.h>
#include <artik_error.h>
#include <artik_cloud.h>
#include <artik_wifi.h>
#include <errno.h>

#include "artik_onboarding.h"

enum ServiceState current_service_state = STATE_IDLE;

static pthread_addr_t start_onboarding(pthread_addr_t arg)
{
	printf("Onboarding service version %s\n", ONBOARDING_VERSION);

	current_service_state = STATE_ONBOARDING;

	/* Check if AKC device type requires secure communication */
	cloud_secure_dt = CloudIsSecureDeviceType(cloud_config.device_type_id);

	/* If we already have Wifi credentials, try to connect to the hotspot */
	if (strlen(wifi_config.ssid) > 0) {
		if (StartStationConnection(true) == S_OK) {
			/* Check if we have valid ARTIK Cloud credentials */
			if ((strlen(cloud_config.device_id) == AKC_DID_LEN) &&
			    (strlen(cloud_config.device_token) == AKC_TOKEN_LEN)) {
				if (StartCloudWebsocket(true) == S_OK) {
					if (StartLwm2m(true) == S_OK) {
						printf("ARTIK Cloud connection started\n");
						goto exit;
					} else {
						printf("Failed to start DM connection, switching back to onboarding mode\n");
					}
				} else {
					printf("Failed to start ARTIK Cloud connection, switching back to onboarding mode\n");
				}
			} else {
				printf("Invalid ARTIK Cloud credentials, switching back to onboarding mode\n");
			}
		} else {
			printf("Could not connect to access point, switching back to onboarding mode\n");
		}
	}

	/* If Cloud connection failed, start the onboarding service */
	if (StartSoftAP(true) != S_OK) {
		goto exit;
	}

	if (StartWebServer(true, API_SET_WIFI) != S_OK) {
		StartSoftAP(false);
		goto exit;
	}

	printf("ARTIK Onboarding Service started\n");
	current_service_state = STATE_ONBOARDING;

exit:
	return NULL;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int artik_onboarding_main(int argc, char *argv[])
#endif
{
	int ret = 0;
	pthread_t tid;

	if (argc > 1) {
		if (!strcmp(argv[1], "reset")) {
			ResetConfiguration(false);
			printf("Onboarding configuration was reset.\n"
				"Reboot the board to return to onboarding mode\n");
			goto exit;
		} else if (!strcmp(argv[1], "dtid")) {
			if (argc < 3) {
				printf("Missing parameter\n");
				printf("Usage: onboard dtid <device type ID>\n");
				goto exit;
			}

			strncpy(cloud_config.device_type_id, argv[2], AKC_DTID_LEN);
			SaveConfiguration();
			goto exit;
		} else if (!strcmp(argv[1], "config")) {
			PrintConfiguration();
			goto exit;
		}  else if (!strcmp(argv[1], "manual")) {
			if (argc < 4) {
				printf("Missing parameter\n");
				printf("Usage: onboard manual <device ID> <device token>\n");
				goto exit;
			}

			strncpy(cloud_config.device_id, argv[2], AKC_DID_LEN);
			strncpy(cloud_config.device_token, argv[3], AKC_TOKEN_LEN);
			SaveConfiguration();
			goto exit;
		}  else if (!strcmp(argv[1], "ntp")) {
			if (argc < 3) {
				printf("Missing parameter\n");
				printf("Usage: onboard ntp <NTP server URL>\n");
				goto exit;
			}

			strncpy(wifi_config.ntp_server, argv[2], NTP_SERVER_MAX_LEN);
			SaveConfiguration();
			goto exit;
		}
	}

	/* If already in onboarding mode or trying to connect, do nothing */
	if ((current_service_state == STATE_ONBOARDING) ||
			(current_service_state == STATE_CONNECTING))
		goto exit;

	if (current_service_state == STATE_CONNECTED) {
		printf("Device is currently connected to cloud. To return to\n"
		       "onboarding mode, delete the device from your ARTIK Cloud\n"
		       "account then reboot the board.\n");
		goto exit;
	}

	if (InitConfiguration() != S_OK) {
		ret = -1;
		goto exit;
	}

	pthread_create(&tid, NULL, start_onboarding, NULL);
	pthread_setname_np(tid, "onboarding start");
	pthread_join(tid, NULL);

exit:
	return ret;
}
