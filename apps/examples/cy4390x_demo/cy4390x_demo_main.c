/****************************************************************************
 *
 * Copyright 2017, Cypress Semiconductor Corporation or a subsidiary of 
 * Cypress Semiconductor Corporation. All Rights Reserved.
 * 
 * This software, associated documentation and materials ("Software"),
 * is owned by Cypress Semiconductor Corporation
 * or one of its subsidiaries ("Cypress") and is protected by and subject to
 * worldwide patent protection (United States and foreign),
 * United States copyright laws and international treaty provisions.
 * Therefore, you may use this Software only as provided in the license
 * agreement accompanying the software package from which you
 * obtained this Software ("EULA").
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software
 * source code solely for use in connection with Cypress's
 * integrated circuit products. Any reproduction, modification, translation,
 * compilation, or representation of this Software except as specified
 * above is prohibited without the express written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress's product in a High Risk Product, the manufacturer
 * of such system or application assumes all risk of such use and in doing
 * so agrees to indemnify Cypress against all liability.
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
 * Definitions
 ****************************************************************************/
#undef ndbg
#undef nvdbg
#define ndbg printf
#define nvdbg printf

static void show_wlan_firmware_version(void);
/****************************************************************************
 * External Function Protoype
 ****************************************************************************/
int read_wlan_chip_console_log(int argc, char* argv[]);

static tash_cmdlist_t peripheral_cmds[] = {
	{"wlog", read_wlan_chip_console_log, TASH_EXECMD_ASYNC},
	{NULL, NULL, 0}
};

extern wwd_result_t wwd_wifi_read_wlan_log(char* buffer, uint32_t buffer_size);
extern wwd_result_t wwd_wifi_get_wifi_version(char* version, uint8_t length);

/*!
 ******************************************************************************
 * Read WLAN chip console buffer and output to host console
 *
 * @return  0 for success, otherwise error
 */

int read_wlan_chip_console_log(int argc, char* argv[])
{
	const unsigned buffer_size = 200;
	int result = -1;

	char* buffer = (char *)malloc_named ("console",buffer_size);
	if (buffer == NULL) {
		return result;
	}

	if (wwd_wifi_read_wlan_log(buffer, buffer_size) == WWD_SUCCESS) {
		result = 0;
	}

	free(buffer);

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

	printf("CY4390X Demo n");

	while (1) {
		sleep(30);
		printf("alive ! count : %d\n", i);
		i++;
	}

	return 0;
}

