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
#include <tinyara/fs/mtd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
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
#define HANDSHAKE_STRING		"RAMDUMP"
#define HANDSHAKE_STR_LEN_MAX		(7)
#endif

#if defined(CONFIG_BOARD_SMARTFS_DUMP)
#define SDV_HANDSHAKE_STRING	"DUMPFS"
#define SDV_HANDSHAKE_STR_LEN	6
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

#if defined(CONFIG_BOARD_SMARTFS_DUMP)
static char smartfsdump_partname[MTD_PARTNAME_LEN + 1];
static uint32_t smartfsdump_partoffset = 0;
static uint32_t smartfsdump_partsize = 0;
#endif

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

	/* Send memory region address, size & heap index to HOST */
	for (x = 0; x < CONFIG_MM_REGIONS; x++) {

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

	/* Receive number of regions to be dumped from HOST */
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

		/* Send RAMDUMP of size bytes */
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

#if defined(CONFIG_BOARD_SMARTFS_DUMP)
static void print_sector(char *buf, int size) {
	for (register int i = 0; i < size / 16; i++) {
		for (register int j = 0; j < 16; j++) {
			printf("%02x ", buf[i * 16 + j]);
		}
		printf("\n");
	}
}

static int smartfs_dump(void)
{
	int i;
	int ch;
	char *handshake_str = SDV_HANDSHAKE_STRING;
	char handshake_msg_buf[SDV_HANDSHAKE_STR_LEN + 1] = "";
	int totalsectors = smartfsdump_partsize / CONFIG_MTD_SMART_SECTOR_SIZE;
	int offset = CONFIG_FLASH_START_ADDR + smartfsdump_partoffset;

#if !defined(CONFIG_ARCH_LOWPUTC)
	/* If low-level serial is not available, smartfs_dump is not possible */
	return -1;
#endif

	/* Inform the terminal user */
	up_puts("\n************************************************************************************************\n");
	up_puts("\t\tDisconnect this serial terminal and Run SDV Tool\n");
	up_puts("************************************************************************************************\n");

	/* Receive handshake string from HOST */
	do {
		handshake_msg_buf[0] = up_getc();
	} while (handshake_msg_buf[0] != handshake_str[0]);

	for (i = 1; i < strlen(handshake_str);) {
		ch = up_getc();
		if (ch != -1) {
			handshake_msg_buf[i] = ch;
			i++;
		}
	}
	handshake_msg_buf[SDV_HANDSHAKE_STR_LEN] = '\n';

	if (strncmp(handshake_msg_buf, handshake_str, strlen(handshake_str)) != 0) {
		lldbg("\tHandshake MSG is wrong... len = %d, %s\n", strlen(handshake_str), handshake_msg_buf);
		return -1;
	}
	llvdbg("\t\t\tHandshake msg is received.  Start transmission.. for %d sectors, offset = 0x%x\n", totalsectors, offset);

	for (register int s = 0; s < totalsectors; s++) {
		print_sector((char *)offset, CONFIG_MTD_SMART_SECTOR_SIZE);
		offset += CONFIG_MTD_SMART_SECTOR_SIZE;
	}

	while (1) {
		llvdbg("Waiting...\n");
	}
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#if defined(CONFIG_BOARD_CRASHDUMP)
#if defined(CONFIG_BOARD_SMARTFS_DUMP)
void smartfsdump_init(void)
{
	int index = mtd_getpartitionindex("userfs");
	lldbg("SmartFS partition index = %d\n", index);
	if (index >= 0) {
		snprintf(smartfsdump_partname, MTD_PARTNAME_LEN + 1, "/dev/mtdblock%d", index);
		smartfsdump_partsize = mtd_getpartitionsize(index, &smartfsdump_partoffset);
		if (!smartfsdump_partsize) {
			lldbg("SmartFS partition size is not obtained\n");
		}
	} else {
		lldbg("SmartFS partition index is wrong\n");
	}
}
#endif

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
	llvdbg("Crashdump is called.\n");
#if defined(CONFIG_WATCHDOG)
	/* system under panic and we are dumping system state.
	 * watchdog reset might stall the system, causing crashdump hang.
	 * Hence let's disable watchdog reset during crashdump process.
	 */
	up_watchdog_disable();
#endif

#if defined(CONFIG_BOARD_SMARTFS_DUMP)
	ret = smartfs_dump();
	if (ret != OK) {
		lldbg("SmartFS dump failed, ret = %d\n", ret);
	}
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
