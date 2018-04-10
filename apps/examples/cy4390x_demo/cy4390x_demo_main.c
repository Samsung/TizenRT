/****************************************************************************
 *
 * Copyright 2018, Cypress Semiconductor Corporation or a subsidiary of
 * Cypress Semiconductor Corporation. All Rights Reserved.
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

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <tinyara/config.h>
#include <apps/shell/tash.h>

#include <wiced.h>

/****************************************************************************
 * Global variable
 ****************************************************************************/
extern uint8_t g_syslog_mask;

/****************************************************************************
 * Definitions
 ****************************************************************************/

static void show_wlan_firmware_version(void);
/****************************************************************************
 * External Function Protoype
 ****************************************************************************/
int read_wlan_chip_console_log(int argc, char *argv[]);
int log_level_change(int argc, char *argv[]);

static tash_cmdlist_t peripheral_cmds[] = {
	{"wlog", read_wlan_chip_console_log, TASH_EXECMD_ASYNC},
	{"loglevel", log_level_change, TASH_EXECMD_ASYNC},
	{NULL, NULL, 0}
};

extern wwd_result_t wwd_wifi_read_wlan_log(char *buffer, uint32_t buffer_size);
extern wwd_result_t wwd_wifi_get_wifi_version(char *version, uint8_t length);

/*!
 ******************************************************************************
 * Read WLAN chip console buffer and output to host console
 *
 * @return  0 for success, otherwise error
 */

int read_wlan_chip_console_log(int argc, char *argv[])
{
	const unsigned buffer_size = 200;
	int result = -1;

	char *buffer = (char *)malloc_named("console", buffer_size);
	if (buffer == NULL) {
		return result;
	}

	if (wwd_wifi_read_wlan_log(buffer, buffer_size) == WWD_SUCCESS) {
		result = 0;
	}

	free(buffer);

	return result;
}

int log_level_change(int argc, char *argv[])
{
	const unsigned buffer_size = 200;
	int result = -1;

	if (argc > 1) {
		g_syslog_mask = LOG_MASK(atoi(argv[1]));
	}

	printf("log level mask = 0x%x \n", g_syslog_mask);
	return result;
}

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void show_wlan_firmware_version(void)
{
	char fw_ver_buf[200];
	int fw_ver_result = 0;

	memset(fw_ver_buf, 0x0, sizeof(fw_ver_buf));
	fw_ver_result = wwd_wifi_get_wifi_version(fw_ver_buf, sizeof(fw_ver_buf));

	if (fw_ver_result == OK) {
		printf("WLAN Firmware Version: %s \n", fw_ver_buf);
	} else {
		printf("Failed to get WLAN FW version (err: %d)\n", fw_ver_result);
	}
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int cy4390x_demo_main(int argc, char *argv[])
#endif
{
	int i = 0;

	printf("\nApplication(%s) is launched@0x%08p\n", __func__, cy4390x_demo_main);

	/* Print WLAN firmware version */
	show_wlan_firmware_version();

#ifdef CONFIG_TASH
	/* add tash command */
	tash_cmdlist_install(peripheral_cmds);
#endif

	printf("CY4390X Demo \n");

	while (1) {
		sleep(30);
		printf("alive ! count : %d\n", i);
		i++;
	}

	return 0;
}
