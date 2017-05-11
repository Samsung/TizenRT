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
 * @file artik_onboarding_config.c
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <artik_error.h>

#include "artik_onboarding.h"

static char config_file[16] = "/mnt/config";

void PrintConfiguration(void)
{
	printf("Wifi:\n");
	printf("\tssid: %s\n", wifi_config.ssid);
	printf("\tpassphrase: %s\n", wifi_config.passphrase);
	printf("\tsecure: %s\n", wifi_config.secure ? "true" : "false");
	printf("\tNTP server: %s\n", wifi_config.ntp_server);

	printf("Cloud:\n");
	printf("\tdevice_id: %s\n", cloud_config.device_id);
	printf("\tdevice_token: %s\n", cloud_config.device_token);
	printf("\tdevice_type_id: %s\n", cloud_config.device_type_id);

	printf("Lwm2m:\n");
	printf("\tis_ota_update: %d\n", lwm2m_state.is_ota_update);
}

artik_error InitConfiguration(void)
{
	struct stat st;
	int fd = 0;
	artik_error err = S_OK;

	stat(config_file, &st);

	if (st.st_size != (sizeof(wifi_config) + sizeof(cloud_config) + sizeof(lwm2m_state))) {
		printf("Invalid configuration, creating default one\n");

		err = ResetConfiguration(true);
	} else {

		fd = open(config_file, O_RDONLY);
		if (fd == -1) {
			printf("Unable to open configuration file (errno=%d)\n", errno);
			err = E_ACCESS_DENIED;
			goto exit;
		}

		lseek(fd, 0, SEEK_SET);
		read(fd, (void *)&wifi_config, sizeof(wifi_config));
		read(fd, (void *)&cloud_config, sizeof(cloud_config));
		read(fd, (void *)&lwm2m_state, sizeof(lwm2m_state));

		close(fd);
	}

exit:
	return err;
}

artik_error SaveConfiguration(void)
{
	int ret = 0;
	int fd = 0;

	fd = open(config_file, O_WRONLY|O_CREAT);
	if (fd == -1) {
		printf("Unable to open configuration file (errno=%d)\n", errno);
		return E_ACCESS_DENIED;
	}

	lseek(fd, 0, SEEK_SET);
	ret = write(fd, (const void *)&wifi_config, sizeof(wifi_config));
	if (ret != sizeof(wifi_config))
		printf("Failed to write wifi config (%d - errno=%d)\n", ret, errno);
	ret = write(fd, (const void *)&cloud_config, sizeof(cloud_config));
	if (ret != sizeof(cloud_config))
		printf("Failed to write wifi config (%d - errno=%d)\n", ret, errno);
	ret = write(fd, (const void *)&lwm2m_state, sizeof(lwm2m_state));
	if (ret != sizeof(lwm2m_state))
		printf("Failed to write lwm2m state (%d - errno=%d)\n", ret, errno);

	close(fd);

	return S_OK;
}

artik_error ResetConfiguration(bool force)
{
	WifiResetConfig(force);
	CloudResetConfig(force);
	Lwm2mResetConfig();

	return SaveConfiguration();
}
