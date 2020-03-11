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
#include "up_watchdog.h"

#include <tinyara/mm/heap_regioninfo.h>
#include <tinyara/mm/mm.h>

/****************************************************************************
 * Definitions
 ****************************************************************************/
#if defined(CONFIG_BOARD_RAMDUMP_UART)
#define HANDSHAKE_STRING                "RAMDUMP"
#define HANDSHAKE_STR_LEN_MAX           (7)
#endif

#if defined(CONFIG_PM_METRICS)
#define RAMDUMP_CONFIG_PM_METRICS 0x0001
#endif

#if defined(CONFIG_BINMGR_RECOVERY)
#define RAMDUMP_CONFIG_BINMGR_RECOVERY 0x0002
#endif

#if defined(CONFIG_PRIORITY_INHERITANCE)
#define RAMDUMP_CONFIG_PRIORITY_INHERITANCE 0x0004
#endif

#if defined(CONFIG_SEM_PREALLOCHOLDERS)
#define RAMDUMP_CONFIG_SEM_PREALLOCHOLDERS 0x00f8 & (CONFIG_SEM_PREALLOCHOLDERS << 3)
#endif

#if defined(CONFIG_SYSTEM_TIME64)
#define RAMDUMP_CONFIG_SYSTEM_TIME64 0x0100
#endif

#if defined(CONFIG_PM_NDOMAINS)
#define RAMDUMP_CONFIG_PM_NDOMAINS 0x1e00 & (CONFIG_PM_NDOMAINS << 9)
#endif

#if defined(CONFIG_PM_MEMORY)
#define RAMDUMP_CONFIG_PM_MEMORY 0xe000 & (CONFIG_PM_MEMORY << 13)
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
	int target_region;
	size_t size;
	uint8_t *ptr;
	char host_reg[1] = "";
	char *target_str = HANDSHAKE_STRING;
	char host_buf[HANDSHAKE_STR_LEN_MAX] = "";
	uint8_t config_info_cnt = 0;
#if defined(CONFIG_PM_METRICS)
	uint32_t pm_config_info = 0;
	config_info_cnt += 1;

	/* Turn on the first bit if CONFIG_PM_METRICS is defined */
	pm_config_info |= RAMDUMP_CONFIG_PM_METRICS;

#if defined(CONFIG_BINMGR_RECOVERY)
	/* Turn on the second bit if CONFIG_BINMGR_RECOVERY is defined */
	pm_config_info |= RAMDUMP_CONFIG_BINMGR_RECOVERY;
#endif

#if defined(CONFIG_PRIORITY_INHERITANCE)
	/* Turn on the third bit if CONFIG_PRIORITY_INHERITANCE is defined */
	pm_config_info |= RAMDUMP_CONFIG_PRIORITY_INHERITANCE;
#endif

#if defined(CONFIG_SEM_PREALLOCHOLDERS)
	/* If CONFIG_SEM_PREALLOCHOLDERS is defined, the value is stored in 4 to 8 bit */
	pm_config_info |= RAMDUMP_CONFIG_SEM_PREALLOCHOLDERS;
#endif

#if defined(CONFIG_SYSTEM_TIME64)
	/* Turn on the 9th bit if CONFIG_SYSTEM_TIME64 is defined */
	pm_config_info |= RAMDUMP_CONFIG_SYSTEM_TIME64;
#endif

#if defined(CONFIG_PM_NDOMAINS)
	/* If CONFIG_PM_NDOMAINS is defined, the value is stored in 10 to 13 bit */
	pm_config_info |= RAMDUMP_CONFIG_PM_NDOMAINS;
#endif

#if defined(CONFIG_PM_MEMORY)
	/* If CONFIG_PM_MEMORY is defined, the value is stored in 14 to 16 bit */
	pm_config_info |= RAMDUMP_CONFIG_PM_MEMORY;
#endif
#endif

#if !defined(CONFIG_ARCH_LOWPUTC)
	/* If lowlevel serial is not available, ramdump is not possible */
	return -1;
#endif

	/* Inform the terminal user */
	up_puts("\n************************************************************************************************\n");
	up_puts("\t\tDisconnect this serial terminal and Run Ramdump Tool\n");
	up_puts("************************************************************************************************\n");

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

	/* Send number of memory regions to HOST */
	up_lowputc(CONFIG_MM_REGIONS);

#if defined(CONFIG_MM_KERNEL_HEAP)
	/* Send 1 to HOST if kernel heap exists */
	up_lowputc('1');

	/* Send number of memory regions to HOST */
	up_lowputc(CONFIG_KMM_REGIONS);

	/* Send number of memory regions to HOST */
	up_lowputc((CONFIG_MM_REGIONS + CONFIG_KMM_REGIONS));

	/* Send memory region address, size & heap index to HOST */
	for (x = 0; x < (CONFIG_MM_REGIONS + CONFIG_KMM_REGIONS); x++) {
#else
	/* Send 1 to HOST if kernel heap exists */
	up_lowputc('0');

	/* Send memory region address, size & heap index to HOST */
	for (x = 0; x < CONFIG_MM_REGIONS; x++) {
#endif

		if (x < CONFIG_MM_REGIONS) {

			/* Send RAM address */
			ptr = (uint8_t *)&regionx_start[x];
			for (i = 0; i < sizeof(regionx_start[x]); i++) {
				up_lowputc((uint8_t)*ptr);
				ptr++;
			}

			/* Send RAM size */
			ptr = (uint8_t *)&regionx_size[x];
			for (i = 0; i < sizeof(regionx_size[x]); i++) {
				up_lowputc((uint8_t)*ptr);
				ptr++;
			}

			/* Send Heap Index */
			up_lowputc(regionx_heap_idx[x]);
		}
#if defined(CONFIG_MM_KERNEL_HEAP)
		else if (x >= CONFIG_MM_REGIONS) {
			int y =  x - CONFIG_MM_REGIONS;

			/* Send Kernel region address */
			ptr = (uint8_t *)&kregionx_start[y];
			for (i = 0; i < sizeof(kregionx_start[y]); i++) {
				up_lowputc((uint8_t)*ptr);
				ptr++;
			}

			/* Send Kernel region size */
			ptr = (uint8_t *)&kregionx_size[y];
			for (i = 0; i < sizeof(kregionx_size[y]); i++) {
				up_lowputc((uint8_t)*ptr);
				ptr++;
			}

			/* Send Kernel Heap Index */
			up_lowputc(regionx_kheap_idx[y]);
		}
#endif
	}

	/* Receive number of user memory regions to be dumped from HOST */
	do {
		if ((ch = up_getc()) != -1) {
			host_reg[0] = ch;
		}
	} while (ch == -1);

	regions_to_dump = host_reg[0] - '0';

	/* Dump data region wise */

	for (x = 0; x < regions_to_dump; x++) {
		/* Receive region index from HOST */
		do {
			if ((ch = up_getc()) != -1) {
				host_reg[0] = ch;
			}
		} while (ch == -1);

		target_region = host_reg[0] - '0';

		if (target_region < CONFIG_MM_REGIONS) {

			/* Send User MM dump of size bytes */
			ptr = (uint8_t *)regionx_start[target_region];
			size = regionx_size[target_region];
			while (size) {
				up_lowputc((uint8_t)*ptr);
				ptr++;
				size--;
			}
		}
#if defined(CONFIG_MM_KERNEL_HEAP)
		else if (target_region >= CONFIG_MM_REGIONS) {
			target_region =  target_region - CONFIG_MM_REGIONS;

			/* Send Kernel MM dump of size bytes */
			ptr = (uint8_t *)kregionx_start[target_region];
			size = kregionx_size[target_region];
			while (size) {
				up_lowputc((uint8_t)*ptr);
				ptr++;
				size--;
			}

		}
#endif
		ptr = (uint8_t *)&config_info_cnt;
		up_lowputc((uint8_t)*ptr);

#ifdef CONFIG_PM_METRICS
		ptr = (uint8_t *)&pm_config_info;
		for (i = 0; i < sizeof(uint32_t); i++) {
			up_lowputc((uint8_t)*ptr);
			ptr++;
		}
#endif
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

#if defined(CONFIG_WATCHDOG)
	/* system under panic and we are dumping system state.
	 * watchdog reset might stall the system, causing crashdump hang.
	 * Hence let's disable watchdog reset during crashdump process.
	 */
	up_watchdog_disable();
#endif

#if defined(CONFIG_BOARD_RAMDUMP_UART)
	ret = ramdump_via_uart();
	if (ret != OK) {
		lldbg("ramdump via uart failed, ret = %d\n", ret);
	}
#endif

	if (ret == OK) {
		lldbg(" Successfull\n");
	}
}
#endif
