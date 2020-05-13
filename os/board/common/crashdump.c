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

#define HANDSHAKE_STRING                "RAMDUMP"
#define HANDSHAKE_STR_LEN_MAX           (7)

#if defined(CONFIG_BOARD_COREDUMP_FLASH)
#define MTD_PARTNAME_LEN		15
#define COREDUMP_BUF_SIZE		(4096)
#endif

#if defined(CONFIG_BOARD_RAMDUMP_FLASH)
#define MTD_PARTNAME_LEN		15
#define MTD_1K_SIZE			(1024)
#define MTD_FLASH_WRITE_SIZE		(32 * MTD_1K_SIZE)
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/
extern uint32_t up_dumptoflash(uint32_t offset, uint32_t *buf, uint32_t bytes);

/****************************************************************************
 * External Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables
 ****************************************************************************/
#if defined(CONFIG_BOARD_RAMDUMP_FLASH)
static char ramdump_partname[MTD_PARTNAME_LEN + 1];
static uint32_t ramdump_partoffset = -1;
static uint32_t ramdump_partsize = 0;
#endif

#if defined(CONFIG_BOARD_COREDUMP_FLASH)
static char coredump_partname[MTD_PARTNAME_LEN + 1];
static uint32_t coredump_partoffset = -1;
static uint32_t coredump_partsize = 0;
#endif

enum {
	RAMDUMP_UART = 0,
	COREDUMP_FLASH,
	RAMDUMP_FLASH,
	DUMP_ALL
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

#if defined(CONFIG_BOARD_COREDUMP_FLASH)
static int coredump_to_flash(uint32_t cur_sp, void *tcb)
{
	uint32_t *ptr;
	int32_t i;
	uint32_t j;
	uint32_t len = 0;
	char pbuf[COREDUMP_BUF_SIZE];
	int ret = -1;
	int32_t coredump_size = 0;

	sched_lock();

	/* Add dummy coredump_size at the beginning */
	len += sprintf(pbuf + len, "%08x\n", coredump_size);

	/* space for '\0' character added by sprintf */
	len += 1;

	/* Process and format the stack to readable format */
	len += sprintf(pbuf + len, "up_dumpstate:   size: %08x\n", \
				((struct tcb_s *)tcb)->adj_stack_size);

	ptr = (uint32_t *)cur_sp;
	for (i = 0; i < 8; i++) {
		len += sprintf(pbuf + len, "up_stackdump: %08x:", ptr);
		for (j = 0; j < 8; j++) {
			len += sprintf(pbuf + len, " %08x", ptr[j]);
		}
		len += sprintf(pbuf + len, "\n");
		ptr += 8;
	}

	ptr = (uint32_t *)current_regs;
	for (i = 0; i < 2; i++) {
		len += sprintf(pbuf + len, "up_registerdump: R%d:", (i * 8));
		for (j = 0; j < 8; j++) {
			len += sprintf(pbuf + len, " %08x", ptr[j]);
		}
		len += sprintf(pbuf + len, "\n");
		ptr += 8;
	}

	if (coredump_partsize < len) {
		lldbg("Partition size 0x%x is less coredump size 0x%x\n", coredump_partsize, len);
		goto err ;
	}

	/* calculate total formated coredump_size, without size field(10) bytes */
	coredump_size = len - 10;

	/* Add the actual crashlog_size at the beginning */
	sprintf(pbuf, "%08x", coredump_size);

	ret = up_dumptoflash(coredump_partoffset, (uint32_t *)pbuf, len);
	if (ret) {
		lldbg("Failed to write %s: len %d, errno %d\n", coredump_partname, len, errno);
	}

err:
	sched_unlock();
	lldbg("core dump of %d bytes dumped to partition %s\n", len, coredump_partname);
	return ret;
}
#endif

#if defined(CONFIG_BOARD_RAMDUMP_FLASH)
static int ramdump_to_flash(uint32_t ram_address, uint32_t ram_size)
{
	uint32_t offset = 0;
	uint32_t total_size = 0;
	uint32_t size;
	int ret = -1;

	lldbg("ramdump size = %d bytes\n", size);

	sched_lock();

	if (ramdump_partsize < ram_size) {
		lldbg("Partition size 0x%x is less ramdump size 0x%x\n", ramdump_partsize, ram_size);
		goto err ;
	}

	/* Maximum ramsize allowed */
	size = (ram_size < ramdump_partsize) ? ram_size : ramdump_partsize;

	while (size) {
		if (size >= MTD_FLASH_WRITE_SIZE) {
			ret = up_dumptoflash(ramdump_partoffset, (uint32_t *)ram_address + offset, MTD_FLASH_WRITE_SIZE);
			if (ret) {
				lldbg("Failed to write %s: len %d, errno %d\n", ramdump_partname, MTD_FLASH_WRITE_SIZE, errno);
				goto err ;
			}
			offset += MTD_FLASH_WRITE_SIZE;
			size -= MTD_FLASH_WRITE_SIZE;
			total_size += MTD_FLASH_WRITE_SIZE;
		} else {
			ret = up_dumptoflash(ramdump_partoffset, (uint32_t *)ram_address + offset, size);
			if (ret) {
				lldbg("Failed to write %s: len %d, errno %d\n", ramdump_partname, size, errno);
				goto err ;
			}
			offset += size;
			total_size += size;
			size -= size;
		}
		lldbg(" %d bytes dumped to flash\n", total_size);
	}
	lldbg(" total %d bytes dumped successfully\n", total_size);

err:
	sched_unlock();
	return ret;
}
#endif

static int handshake_on_uart(void)
{
	int i;
	int ch;
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
		return -1;
	}

	/* Send ACK */
	up_lowputc('A');
	return OK;
}

#ifdef CONFIG_BOARD_COREDUMP_FLASH
static int coredump_from_flash()
{
	uint32_t len = 0;
	int ret;
	char pbuf[COREDUMP_BUF_SIZE];
	size_t size;
	uint8_t *ptr;

	if(coredump_partoffset < 0)
	{
		lldbg("coredump is not stored at partition offset \n", coredump_partoffset);
		return -1;
	}

	/* Read coredump size from 4 bytes */
	ret = up_dumpfromflash(coredump_partoffset, (uint8_t *)&len, 8);
	if (ret != OK)
		return -1;
	if (len > coredump_partsize) {
		lldbg("len: 0x%x is more than Partition size: 0x%x\n", len, coredump_partsize);
		return -1;
	}

	sprintf(pbuf, "%08x", len);
	ret = up_dumpfromflash(coredump_partoffset + 10, pbuf + 10, len);

	/* Sending the length */
	ptr = pbuf;
	size = 8;
	while (size) {
		up_lowputc((uint8_t)*ptr);
		ptr++;
		size--;
	}

	/* Sending the coredump */
	ptr = pbuf + 10;
	size = len;
	while (size) {
		up_lowputc((uint8_t)*ptr);
		ptr++;
		size--;
	}
}
#endif

#ifdef CONFIG_BOARD_RAMDUMP_FLASH
static int ramdump_from_flash()
{
#if defined(CONFIG_BUILD_PROTECTED)
	uint32_t len = CONFIG_KSRAM_SIZE;
	char pbuf[CONFIG_KSRAM_SIZE];
#else
	uint32_t len = CONFIG_RAM_REGIONx_SIZE;
	char pbuf[CONFIG_RAM_REGIONx_SIZE];
#endif
	int ret;
	size_t size;
	uint8_t *ptr;

	if ( (ramdump_partsize < len) || (ramdump_partoffset < 0) ) {
		lldbg("Partition size 0x%x is less ramdump size 0x%x or partoffset not set\n", ramdump_partsize, len);
		return -1;
	}

	ret = up_dumpfromflash(ramdump_partoffset, pbuf, len);
	if (ret != OK)
		return -1;

	/* Sending the length */
	ptr = (uint8_t *)&len;
	size = sizeof(uint32_t);
	while (size) {
		up_lowputc((uint8_t)*ptr);
		ptr++;
		size--;
	}

	/* Sending the ramdump */
	ptr = pbuf;
	size = len;
	while (size) {
		up_lowputc((uint8_t)*ptr);
		ptr++;
		size--;
	}
}
#endif

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
	}

	lldbg(" Successfull\n");
	return OK;
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#if defined(CONFIG_BOARD_CRASHDUMP)
void crashdump_init(void)
{
	int index;

#if defined(CONFIG_BOARD_COREDUMP_FLASH)
	index = mtd_getpartitionindex("coredump");
	if (index >= 0) {
		snprintf(coredump_partname, MTD_PARTNAME_LEN + 1, "/dev/mtdblock%d", index);
		coredump_partsize = mtd_getpartitionsize(index, &coredump_partoffset);
		if (!coredump_partsize) {
			lldbg("partition size is wrong for coredump partition name\n");
		}
	} else {
		lldbg("partition index is wrong for coredump partition name\n");
	}
#endif

#if defined(CONFIG_BOARD_RAMDUMP_FLASH)
	index = mtd_getpartitionindex("ramdump");
	if (index >= 0) {
		snprintf(ramdump_partname, MTD_PARTNAME_LEN + 1, "/dev/mtdblock%d", index);
		ramdump_partsize = mtd_getpartitionsize(index, &ramdump_partoffset);
		if (!ramdump_partsize) {
			lldbg("partition size is wrong for ramdump partition name\n");
		}
	} else {
		lldbg("partition index is wrong for ramdump partition name\n");
	}
#endif
}

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
	int ch;

#if defined(CONFIG_WATCHDOG)
	/* system under panic and we are dumping system state.
	 * watchdog reset might stall the system, causing crashdump hang.
	 * Hence let's disable watchdog reset during crashdump process.
	 */
	up_watchdog_disable();
#endif

#if defined(CONFIG_BOARD_COREDUMP_FLASH)
	ret = coredump_to_flash(cur_sp, tcb);
	if (ret != OK) {
		lldbg("coredump to flash failed, ret = %d\n", ret);
	}
#endif

#if defined(CONFIG_BOARD_RAMDUMP_FLASH)
#if defined(CONFIG_BUILD_PROTECTED)
	ret = ramdump_to_flash(CONFIG_SRAM_START_ADDR, CONFIG_KSRAM_SIZE);
#else
	ret = ramdump_to_flash(CONFIG_RAM_REGIONx_START, CONFIG_RAM_REGIONx_SIZE);
	if (ret != OK) {
		lldbg("ramdump to flash failed, ret = %d\n", ret);
	}
#endif
#endif

#if defined(CONFIG_BOARD_COREDUMP_FLASH) || defined(CONFIG_BOARD_RAMDUMP_FLASH) || defined(CONFIG_BOARD_RAMDUMP_UART)
	ret = handshake_on_uart();
	if (ret != OK) {
		lldbg("handshake on uart failed\n");
		return;
	}

	/* Receive dump regions */
	while ((ch = up_getc()) == -1);
#endif

#if defined(CONFIG_BOARD_COREDUMP_FLASH)
	if ((ch == COREDUMP_FLASH) || (ch == DUMP_ALL)) {
		ret = coredump_from_flash();
		if (ret != OK) {
			lldbg("coredump from flash failed, ret = %d\n", ret);
		}
	}
#endif
#if defined(CONFIG_BOARD_RAMDUMP_FLASH)
	if ((ch == RAMDUMP_FLASH) || (ch == DUMP_ALL)) {
		ret = ramdump_from_flash();
		if (ret != OK) {
			lldbg("ramdump from uart failed, ret = %d\n", ret);
		}
	}
#endif
#if defined(CONFIG_BOARD_RAMDUMP_UART)
	if ((ch == RAMDUMP_UART) || (ch == DUMP_ALL)) {
		ret = ramdump_via_uart();
		if (ret != OK) {
			lldbg("ramdump via uart failed, ret = %d\n", ret);
		}
	}
#endif

	if (ret == OK) {
		lldbg(" Successfull\n");
	}
}
#endif
