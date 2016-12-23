/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
#if defined(CONFIG_BOARD_COREDUMP_FLASH) || defined(CONFIG_BOARD_RAMDUMP_FLASH)
#include <arch/board/s5jt200_partitions.h>
#endif

/****************************************************************************
 * Definitions
 ****************************************************************************/
#if defined(CONFIG_BOARD_COREDUMP_FLASH) || defined(CONFIG_BOARD_RAMDUMP_FLASH)
#define SFLASH_BLOCK_ERASE_LARGE_CMD    0xD8
#define SFLASH_SECTOR_ERASE_CMD         0x20
#endif

#if defined(CONFIG_BOARD_COREDUMP_FLASH)
#define COREDUMP_BUF_SIZE               (4096)
#endif

#define HANDSHAKE_STRING                "RAMDUMP"
#define HANDSHAKE_STR_LEN_MAX           (7)

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/
#if defined(CONFIG_BOARD_COREDUMP_FLASH) || defined(CONFIG_BOARD_RAMDUMP_FLASH)
/* Prototypes for flash API's, just to keep compiler happy */
extern int s5j_sflash_erase(void *dev, uint8_t cmd, uint32_t addr);
extern int s5j_sflash_write(void *dev, uint32_t addr, uint8_t *buf, uint32_t size);
extern int s5j_sflash_read(void *dev, uint32_t addr, uint8_t *buf, uint32_t size);
#endif

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
#if defined(CONFIG_BOARD_COREDUMP_FLASH)
static int coredump_open(FAR struct file *filep);
static int coredump_close(FAR struct file *filep);
static ssize_t coredump_read(FAR struct file *filep, FAR char *buffer, size_t buflen);
static void coredump_seek_reset(void);
static int read_coredump_from_flash(char *buffer, int size);
#endif

/****************************************************************************
 * External Variables
 ****************************************************************************/
#if defined(CONFIG_BOARD_COREDUMP_FLASH) || defined(CONFIG_BOARD_RAMDUMP_FLASH)
extern volatile bool g_sflash_nonsleep_mode;
#endif

/****************************************************************************
 * Private Variables
 ****************************************************************************/
#if defined(CONFIG_BOARD_COREDUMP_FLASH)
static sem_t coredump_readsem;
static int32_t coredump_dev_opened;
static int32_t read_flash_done;
static int32_t size_transferred;
static int32_t coredump_size = 0;
static uint8_t coredump_buf[COREDUMP_BUF_SIZE];

static const struct file_operations g_coredump_ops = {
	coredump_open,				/* open */
	coredump_close,				/* close */
	coredump_read,				/* read */
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static int coredump_open(FAR struct file *filep)
{
	if (coredump_dev_opened) {
		dbg("coredump device busy\n");
		return -1;
	}

	coredump_dev_opened = 1;

	return OK;
}

static int coredump_close(FAR struct file *filep)
{
	if (!coredump_dev_opened) {
		dbg("coredump device close failed\n");
		return -1;
	}

	coredump_dev_opened = 0;

	coredump_seek_reset();
	return OK;
}

static ssize_t coredump_read(FAR struct file *filep, FAR char *buffer, size_t buflen)
{
	int ret = 0;

	if (!coredump_dev_opened) {
		dbg("coredump device not opened\n");
		return -1;
	}

	/* take read semaphore */
	while (sem_wait(&coredump_readsem) != OK) ;

	ret = read_coredump_from_flash(buffer, buflen);

	/* give read semaphore */
	sem_post(&coredump_readsem);
	return ret;
}

static int read_coredump_from_flash(char *buffer, int size)
{
	int32_t len;
	char *pbuf = (char *)coredump_buf;
	uint32_t coredump_flash_base;

	coredump_flash_base = MTD_BLK_COREDUMP_START;

	if (!read_flash_done) {

		len = COREDUMP_BUF_SIZE;

		/* Maximum len allowed is MTD_BLK_COREDUMP_SIZE partition size */
		len = (len < MTD_BLK_COREDUMP_SIZE) ? len : MTD_BLK_COREDUMP_SIZE;

		/* clear coredump buffer */
		memset(coredump_buf, 0, COREDUMP_BUF_SIZE);

		s5j_sflash_read(NULL, coredump_flash_base, (uint8_t *)coredump_buf, len);

		read_flash_done = 1;
		coredump_size = strtol((char *)coredump_buf, NULL, 16);
	}

	/* seek after 10 byte header(size) field */
	pbuf += 10;

	if (coredump_size <= 0) {
		return 0;
	}

	/* adjust size */
	size = ((coredump_size - size) > 0) ? size : coredump_size;

	/* copy to user buffer */
	memcpy(buffer, pbuf + size_transferred, size);
	size_transferred += size;
	coredump_size -= size;

	return size;
}

static void coredump_seek_reset(void)
{
	read_flash_done = 0;
	size_transferred = 0;
}

static int coredump_to_flash(uint32_t cur_sp, void *tcb)
{
	int32_t i;
	int32_t j;
	int32_t len = 0;
	uint32_t *ptr;
	uint32_t stack;
	char *pbuf = (char *)coredump_buf;
	uint32_t coredump_flash_base;
	struct tcb_s *rtcb = (struct tcb_s *)tcb;

	/* MTD_BLK_DUMP partition is used for coredump(crashlog) */
	coredump_flash_base = MTD_BLK_COREDUMP_START;

	/* MTD_BLK_COREDUMP_SIZE */
	if (MTD_BLK_COREDUMP_SIZE < MTD_SFLASH_ERASE_SIZE) {
		lldbg("error :coredump partition is too less, exiting ...\n");
		return -1;
	}

	/* Add dummy coredump_size at the beginning */
	len += sprintf(pbuf + len, "%08x\n", coredump_size);

	/* space for '\0' character added by sprintf */
	len += 1;

	/* Process and format the stack to readable format */
	len += sprintf(pbuf + len, "up_dumpstate:   size: %08x\n", (rtcb->adj_stack_size));

	for (stack = cur_sp & ~0x1f; stack < (uint32_t)rtcb->adj_stack_ptr; stack += 32) {
		ptr = (uint32_t *)stack;
		len += sprintf(pbuf + len, "up_stackdump: ");
		len += sprintf(pbuf + len, "%08x: %08x %08x %08x %08x %08x %08x %08x %08x\n", stack, ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5], ptr[6], ptr[7]);
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

	len += sprintf(pbuf + len, "up_registerdump: CPSR: %08x\n", current_regs[REG_CPSR]);

	/* calculate total formated coredump_size, without size field(10) bytes */
	coredump_size = len - 10;

	/* Add the actual crashlog_size at the beginning */
	sprintf(pbuf, "%08x", coredump_size);

	/* Erase a block in sflash */
	s5j_sflash_erase(NULL, SFLASH_BLOCK_ERASE_LARGE_CMD, coredump_flash_base);

	/* Maximum len allowed is MTD_BLK_DUMP_SIZE partition size */
	len = (len < MTD_BLK_COREDUMP_SIZE) ? len : MTD_BLK_COREDUMP_SIZE;

	/* write formated crashlog to sflash */
	s5j_sflash_write(NULL, coredump_flash_base, (uint8_t *)pbuf, len);

	lldbg("core dump of %d bytes dumped to flash @ %08x\n", len, (MTD_SFLASH_ADDR + coredump_flash_base));

	return OK;
}
#endif

#if defined(CONFIG_BOARD_RAMDUMP_FLASH)
static int ramdump_to_flash(uint32_t ram_address, uint32_t ram_size)
{
	uint32_t offset = 0;
	uint32_t ramdump_flash_base;
	uint32_t total_size = 0;
	uint32_t size;

	/* Maximum ramsize allowed is MTD_BLK_RAMDUMP_SIZE partition size */
	size = (ram_size < MTD_BLK_RAMDUMP_SIZE) ? ram_size : MTD_BLK_RAMDUMP_SIZE;

	lldbg("ramdump size(requested = %d, truncated = %d) bytes\n", ram_size, size);

	/* ramdump base is MTD_BLK_RAMDUMP_START */
	ramdump_flash_base = MTD_BLK_RAMDUMP_START;

	/* validate MTD_BLK_RAMDUMP_SIZE */
	if (MTD_BLK_RAMDUMP_SIZE < MTD_SFLASH_ERASE_SIZE) {
		lldbg("ramdump partition is too less, exiting ...\n");
		return -1;
	}

	while (size) {
		if (size >= MTD_SFLASH_ERASE_SIZE) {
			s5j_sflash_erase(NULL, SFLASH_BLOCK_ERASE_LARGE_CMD, ramdump_flash_base + offset);
			s5j_sflash_write(NULL, ramdump_flash_base + offset, (uint8_t *)ram_address + offset, MTD_SFLASH_ERASE_SIZE);
			offset += MTD_SFLASH_ERASE_SIZE;
			size -= MTD_SFLASH_ERASE_SIZE;
			total_size += MTD_SFLASH_ERASE_SIZE;
		} else {
			s5j_sflash_erase(NULL, SFLASH_BLOCK_ERASE_LARGE_CMD, ramdump_flash_base + offset);
			s5j_sflash_write(NULL, ramdump_flash_base + offset, (uint8_t *)ram_address + offset, size);
			offset += size;
			total_size += size;
			size -= size;
		}

		lldbg(" %d bytes dumped to flash\n", total_size);
	}

	lldbg(" total %d bytes dumped successfully\n", total_size);

	return OK;
}
#endif

#if defined(CONFIG_BOARD_RAMDUMP_UART)
static int ramdump_via_uart(uint32_t address, uint32_t size)
{
	int i;
	int ch;
	uint8_t *ptr;
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

	/* Send RAM address */
	ptr = (uint8_t *)&address;
	for (i = 0; i < sizeof(address); i++) {
		up_lowputc((uint8_t)*ptr);
		ptr++;
	}

	/* Send RAM size */
	ptr = (uint8_t *)&size;
	for (i = 0; i < sizeof(size); i++) {
		up_lowputc((uint8_t)*ptr);
		ptr++;
	}

	/* Send RAMDUMP of ram_size bytes */
	ptr = (uint8_t *)address;
	while (size) {
		up_lowputc((uint8_t)*ptr);
		ptr++;
		size--;
	}

	lldbg(" Successfull\n");

	return OK;
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

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

#if defined(CONFIG_S5J_WATCHDOG) && defined(CONFIG_S5J_WATCHDOG_RESET)
	/* system under panic and we are dumping system state.
	 * watchdog reset might stall the system, causing crashdump hang.
	 * Hence let's disable watchdog reset during crashdump process.
	 */
	s5j_wdg_set_reset(WDT_CPUCL_BASE, 0);
#endif

#if defined(CONFIG_BOARD_COREDUMP_FLASH) || defined(CONFIG_BOARD_RAMDUMP_FLASH)
	/* Enable flash non sleep mode, will be used in s5j_sflash.c */
	g_sflash_nonsleep_mode = true;
#endif

#if defined(CONFIG_BOARD_COREDUMP_FLASH)
	ret = coredump_to_flash(cur_sp, tcb);
	if (ret != OK) {
		lldbg("coredump to flash failed, ret = %d\n", ret);
	}
#endif

#if defined(CONFIG_BOARD_RAMDUMP_FLASH)
	ret = ramdump_to_flash(CONFIG_RAM_START, CONFIG_RAM_SIZE);
	if (ret != OK) {
		lldbg("ramdump to flash failed, ret = %d\n", ret);
	}
#endif

#if defined(CONFIG_BOARD_RAMDUMP_UART)
	ret = ramdump_via_uart(CONFIG_RAM_START, CONFIG_RAM_SIZE);
	if (ret != OK) {
		lldbg("ramdump via uart failed, ret = %d\n", ret);
	}
#endif

#if defined(CONFIG_BOARD_COREDUMP_FLASH) || defined(CONFIG_BOARD_RAMDUMP_FLASH)
	/* Disable flash non sleep mode */
	g_sflash_nonsleep_mode = false;
#endif

#if defined(CONFIG_S5J_WATCHDOG) && defined(CONFIG_S5J_WATCHDOG_RESET)
	/* we are done !!, let's enable the watchdog reset */
	s5j_wdg_set_reset(WDT_CPUCL_BASE, 1);
#endif
	if (ret == OK) {
		lldbg(" Successfull\n");
	}
}

#if defined(CONFIG_BOARD_COREDUMP_FLASH)
/****************************************************************************
 * Name: up_coredump_init
 *
 * Description:
 *       Initialize coredump
 ****************************************************************************/
int up_coredump_init(void)
{
	char coredump_devname[20] = "/dev/coredump";

	sem_init(&coredump_readsem, 0, 1);

	if (register_driver(coredump_devname, &g_coredump_ops, 0444, NULL) != 0) {
		dbg("coredump driver register failed\n");
		return -1;
	}

	return OK;
}
#endif
