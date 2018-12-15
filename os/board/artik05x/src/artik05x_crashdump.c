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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/board.h>
#include <tinyara/arch.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chip.h>
#include <arch/board/board.h>
#include "up_internal.h"

#include <tinyara/mm/heap_regioninfo.h>
#include <tinyara/mm/mm.h>
/****************************************************************************
 * Definitions
 ****************************************************************************/
#if defined(CONFIG_BOARD_RAMDUMP_UART)
#define HANDSHAKE_STRING                "RAMDUMP"
#define HANDSHAKE_STR_LEN_MAX           (7)
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * External Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

#if defined(CONFIG_BOARD_RAMDUMP_UART)
static int ramdump_via_uart(void)
{
	int i;
	int x;
	int ch;
	int regions_to_dump;
	int target_region = 0;
	size_t size;
	uint8_t *ptr;
	char host_reg[3] = "";
	char *target_str = HANDSHAKE_STRING;
	char host_buf[HANDSHAKE_STR_LEN_MAX] = "";

#if !defined(CONFIG_ARCH_LOWPUTC)
	/* If lowlevel serial is not available, ramdump is not possible */
	return -1;
#endif

	/* Inform the terminal user */
	up_puts("****************************************************\n");
	up_puts("Disconnect this serial terminal and Run Ramdump Tool\n");
	up_puts("****************************************************\n");

	/* Receive hanshake string from HOST */
	do {
		host_buf[0] = up_getc();
	} while (host_buf[0] != target_str[0]);

	for (i = 1; i < strlen(target_str);) {
		if ((ch = up_getc()) != -1) {
			host_buf[i] = ch;
			i++;
		}
	}

	if (strncmp(host_buf, target_str, strlen(target_str)) != 0) {
		/* Send NAK */
		up_lowputc('N');
	}

	/* Send ACK */
	up_lowputc('A');

	/* Receive number of regions to be dumped from HOST */
	do {
		host_reg[0] = up_getc();
	} while (host_reg[0] != 'r');

	do {
		ch = up_getc();
		if ((ch > 47) && (ch  < 58)) {
			host_reg[1] = ch;
		}
	} while (ch != 'r');

	regions_to_dump = host_reg[1] - '0';

	if (regions_to_dump > CONFIG_MM_REGIONS) {
		/* Send NAK */
		up_lowputc('N');
	}

	/* Send ACK */
	up_lowputc('A');

	if (regions_to_dump == CONFIG_MM_REGIONS) {
		target_region = 0;
	}

	/* Dump data region wise */

	for (x = 0; x < regions_to_dump; x++) {
		/* Receive region index from HOST */
		do {
			host_reg[0] = up_getc();
		} while (host_reg[0] != 'r');

		do {
			ch = up_getc();
			if ((ch > 47) && (ch  < 58)) {
				host_reg[1] = ch;
			}
		} while (ch != 'r');

		target_region = host_reg[1] - '0';

		if (target_region < 0 && target_region > (CONFIG_MM_REGIONS + 1)) {
			/* Send NAK */
			up_lowputc('N');
		}

		/* Send ACK */
		up_lowputc('A');

		/* Send RAM address */
		ptr = (uint8_t *)&regionx_start[target_region];
		for (i = 0; i < sizeof(regionx_start[target_region]); i++) {
			up_lowputc((uint8_t)*ptr);
			ptr++;
		}

		/* Send RAM size */
		ptr = (uint8_t *)&regionx_size[target_region];
		for (i = 0; i < sizeof(regionx_size[target_region]); i++) {
			up_lowputc((uint8_t)*ptr);
			ptr++;
		}

		/* Send RAMDUMP of ram_size bytes */
		ptr = (uint8_t *)regionx_start[target_region];
		size = regionx_size[target_region];
		while (size) {
			up_lowputc((uint8_t)*ptr);
			ptr++;
			size--;
		}
	}
	lldbg(" Successfull\n");

	return OK;
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#if defined(CONFIG_BOARD_CRASHDUMP)
/****************************************************************************
 * Name: board_crashdump
 *
 * Description:
 *   If CONFIG_BOARD_CRASHDUMP is selected then up_asseert will call out to
 *   board_crashdump prior to calling exit in the case of an assertion failure.
 *   Or in the case of a hardfault looping indefinitely. board_crashdump then
 *   has a chance to save the state of the machine. The provided
 *   board_crashdump should save as much information as it can about the cause
 *   of the fault and then most likely reset the system.
 *
 *   N.B. There are limited system resources that can be used by the provided
 *   board_crashdump function. The tems from the fact that most critical/fatal
 *   crashes are because of a hard fault or during interrupt processing.
 *   In these cases, up_assert is running from the context of an interrupt
 *   handlerand it is impossible to use any device driver in this context.
 *
 *   Also consider the following: Who knows what state the system is in? Is
 *   memory trashed? Is the Heap intact? Therefore all we can expect to do in
 *   board_crashdump is save the "machine state" in a place where on the next
 *   reset we can write it to more sophisticated storage in a sane operating
 *   environment.
 *
 ****************************************************************************/

void board_crashdump(uint32_t cur_sp, void *tcb, uint8_t *filename, int lineno)
{
	int ret = OK;

#if defined(CONFIG_S5J_WATCHDOG)
	/* system under panic and we are dumping system state.
	 * watchdog reset might stall the system, causing crashdump hang.
	 * Hence let's disable watchdog reset during crashdump process.
	 */
	s5j_watchdog_disable();
#endif

#if defined(CONFIG_BOARD_RAMDUMP_UART)
	ramdump_via_uart();
	if (ret != OK) {
		lldbg("ramdump via uart failed, ret = %d\n", ret);
	}
#endif

	if (ret == OK) {
		lldbg(" Successfull\n");
	}
}
#endif
