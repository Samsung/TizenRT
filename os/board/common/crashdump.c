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
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <arch/board/board.h>
#include "up_internal.h"
#include "up_watchdog.h"
#include <sys/boardctl.h>
#include <sys/stat.h>
#include <limits.h>

#include <tinyara/mm/heap_regioninfo.h>
#include <tinyara/mm/mm.h>

/****************************************************************************
 * Definitions
 ****************************************************************************/
#if defined(CONFIG_BOARD_DUMP_UART)
#define RAMDUMP_HANDSHAKE_STRING	"TIZENRTRMDUMP"
#define FSDUMP_HANDSHAKE_STRING	"TIZENRTFSDUMP"
#define EXTFSDUMP_HANDSHAKE_STRING	"TIZENRTEXTFSD"
#define TARGET_REBOOT_STRING		"TIZENRTREBOOT"
#define USERFS_FDUMP_HANDSHAKE_STRING	"TIZENRTFILDUM"
#define HANDSHAKE_STR_LEN_MAX		(13)
#define USERFS_PART_NAME		"user"
#define USERFS_PART_NAME_LEN		(4)
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

#if defined(CONFIG_BOARD_DUMP_UART)
static int get_userfs_address_size(uint32_t *userfs_start, size_t *userfs_size)
{
	char flash_list[] = CONFIG_FLASH_PART_NAME;
	char flash_sizes[] = CONFIG_FLASH_PART_SIZE;
	char tmp_part_name[USERFS_PART_NAME_LEN + 1] = { '\0' };
	size_t tmp_part_size = 0;
	int i = 0;
	int x = 0;
	int c = 0;
	*userfs_start = CONFIG_FLASH_START_ADDR;

	while (flash_list[i] != '\0') {
		if (flash_list[i] != 'u') {
			while (flash_list[i] != ',') {
				i++;
			}
			c++;
		} else {
			while ((x < USERFS_PART_NAME_LEN) && (flash_list[i] != ',')) {
				tmp_part_name[x] = flash_list[i];
				x++;
				i++;
			}
			tmp_part_name[x] = '\0';
			if ((x == USERFS_PART_NAME_LEN) && (strncmp(tmp_part_name, USERFS_PART_NAME, USERFS_PART_NAME_LEN) == 0)) {
				break;
			}
			if (flash_list[i] == ',') {
				c++;
			}
		}
		x = 0;
		i++;
	}

	if (flash_list[i] == '\0') {
		return -1;
	}

	i = 0;
	while (flash_sizes[i] != '\0' && c) {
		while (flash_sizes[i] != ',') {
			tmp_part_size *= 10;
			tmp_part_size += (flash_sizes[i] - 48);
			i++;
		}
		*userfs_start += (tmp_part_size * 1024);
		tmp_part_size = 0;
		c--;
		i++;
	}

	if (flash_sizes[i] == '\0') {
		return -1;
	}

	*userfs_size = 0;
	while (flash_sizes[i] != ',' && flash_sizes[i] != '\0') {
		*userfs_size *= 10;
		*userfs_size += (flash_sizes[i] - 48);
		i++;
	}
	*userfs_size *= 1024;
	return 0;
}

static int get_ext_userfs_part(size_t *size, int *index)
{
#if defined(CONFIG_SECOND_FLASH_PART_NAME) && defined(CONFIG_SECOND_FLASH_PART_SIZE)
	int i = 0;
	int c = 0;
	int x = 0;
	size_t ext_userfs_size;
	char second_flash_part_list[] = CONFIG_SECOND_FLASH_PART_NAME;
	char second_flash_part_sizes[] = CONFIG_SECOND_FLASH_PART_SIZE;
	char tmp_part_name[USERFS_PART_NAME_LEN + 1] = { '0' };

	while (second_flash_part_list[i] != '\0') {
		if (second_flash_part_list[i] != 'u') {
			while (second_flash_part_list[i] != ',') {
				i++;
			}
			c++;
		} else {
			while ((x < USERFS_PART_NAME_LEN) && (second_flash_part_list[i] != ',')) {
				tmp_part_name[x] = second_flash_part_list[i];
				x++;
				i++;
			}
			tmp_part_name[x] = '\0';
			if ((x == USERFS_PART_NAME_LEN) && (strncmp(tmp_part_name, USERFS_PART_NAME, USERFS_PART_NAME_LEN) == 0)) {
				break;
			}
			if (second_flash_part_list[i] == ',') {
				c++;
			}
		}

		x = 0;
		i++;
	}

	if (second_flash_part_list[i] == '\0') {
		*size = 0;
		*index = -1;
		return ERROR;
	}

	*index = c;
	i = 0;
	while (second_flash_part_sizes[i] != '\0' && c) {
		while (second_flash_part_sizes[i] != ',') {
			i++;
		}
		c--;
		i++;
	}

	if (second_flash_part_sizes[i] == '\0') {
		*size = 0;
		*index = -1;
		return ERROR;
	}

	ext_userfs_size = 0;
	while (second_flash_part_sizes[i] != ',' && second_flash_part_sizes[i] != '\0') {
		ext_userfs_size *= 10;
		ext_userfs_size += (second_flash_part_sizes[i] - 48);
		i++;
	}
	ext_userfs_size *= 1024;
	*size = ext_userfs_size;

#else
	*size = 0;
	*index = -1;
#endif

	return 0;
}

static int send_file(void)
{
	int i;
	int fd;
	int ret;
	struct stat st;
	off_t size;
	char buf;
	char filename[CONFIG_SMARTFS_MAXNAMLEN + 1] = {'\0'};
	char char_filesize;

	/* Send acknowledgement for USERFS FILE DUMP Handshake */
	up_lowputc('A');

	/* Receive the file name to be sent to the host */
	for (i = 0; i < CONFIG_SMARTFS_MAXNAMLEN + 1; i++) {
		filename[i] = up_getc();
	}
	if (filename[i - 1] != '\0') {
		return -ENAMETOOLONG;
	}

	ret = stat(filename, &st);
	if (ret != OK) {
		/* We are not able to stat the file for some reason */
		return ERROR;
	}
	size = st.st_size;

	/* Send the size of the file we are dumping to the host so that it knows how much data is coming in
	 * It is a 64 bit data type so has to be transmitted accordingly
	 * This also acts as a confirmation of the presence of the file in itself.
	 */
	for (i = 0; i < 8; i++) {
		char_filesize = (size >> ((8 - 1 - i) * 8));
		up_lowputc(char_filesize);
	}

	fd = open(filename, O_RDOK);
	if (fd < 0) {
		//Unable to open file for transmitting data
		return ERROR;
	}

	while (size) {
		ret = read(fd, &buf, 1);
		if (ret != 1) {
			//Unable to read from file
			close(fd);
			return ERROR;
		}
		up_lowputc(buf);
		size--;
	}

	close(fd);
	return 0;
}

static int send_ramdump(void)
{
	int i;
	int x;
	int ch;
	int regions_to_dump;
	int target_region;
	size_t size;
	uint8_t *ptr;
	char host_reg[1] = "";

	/* Send acknowledgement for RAMDUMP Handshake */
	up_lowputc('A');
	/* Send number of memory regions to HOST */
	up_lowputc(CONFIG_KMM_REGIONS);

	/* Send memory region address, size & heap index to HOST */
	for (x = 0; x < CONFIG_KMM_REGIONS; x++) {

		/* Send RAM address */
		ptr = (uint8_t *)&kregionx_start[x];
		for (i = 0; i < sizeof(kregionx_start[x]); i++) {
			up_lowputc((uint8_t)*ptr);
			ptr++;
		}

		/* Send RAM size */
		ptr = (uint8_t *)&kregionx_size[x];
		for (i = 0; i < sizeof(kregionx_size[x]); i++) {
			up_lowputc((uint8_t)*ptr);
			ptr++;
		}

		/* Send Heap Index */
		up_lowputc(kregionx_heap_idx[x]);
	}

	/* Receive number of memory regions to be dumped from HOST */
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

		/* Send memory dump of size bytes */
		ptr = (uint8_t *)kregionx_start[target_region];
		size = kregionx_size[target_region];
		while (size) {
			up_lowputc((uint8_t)*ptr);
			ptr++;
			size--;
		}
	}

	return 0;
}

static int send_fsdump(void)
{
	int i = 0;
	uint8_t *ptr;
	uint32_t userfs_start;
	size_t size;

	/* Send ACK for FSDUMP Handshake */
	up_lowputc('A');

	i = get_userfs_address_size(&userfs_start, &size);
	if (i < 0) {
		return -1;
	}

	/* Send address of userfs partition in flash to host */
	ptr = (uint8_t *)&userfs_start;
	for (i = 0; i < sizeof(userfs_start); i++) {
		up_lowputc((uint8_t)*ptr);
		ptr++;
	}

	/* Send size of userfs partition in flash to host */
	ptr = (uint8_t *)&size;
	for (i = 0; i < sizeof(userfs_start); i++) {
		up_lowputc((uint8_t)*ptr);
		ptr++;
	}

	/* Send contents of userfs partition in flash to host */
	ptr = (uint8_t *)userfs_start;
	while (size) {
		up_lowputc((uint8_t)*ptr);
		ptr++;
		size--;
	}
	return 0;
}

static int send_extfsdump(void)
{
	int i = 0;
	int fd;
	int index;
	uint8_t *ptr;
	size_t size;
	char ext_userfs_partition[CONFIG_PATH_MAX] = { '\0' };
	char ext_fsdump_buf[CONFIG_MTD_SMART_SECTOR_SIZE + 1];

	/*Send ACK for EXTERNAL FSDUMP Handshake */
	up_lowputc('A');

	i = get_ext_userfs_part(&size, &index);
	if (i < 0) {
		return ERROR;
	}

	/* Send external userfs partition size to host */
	ptr = (uint8_t *)&size;
	for (i = 0; i < sizeof(size); i++) {
		up_lowputc((uint8_t)*ptr);
		ptr++;
	}

	/* If extracted size is 0, it means there is no partition available to dump */
	if (size == 0) {
		return 0;
	}

	//TODO:- Currently using blockproxy to create a temporary BCH driver instance to
	//access the block device. Should be changed to create a permanent BCH instance
	//on device initialization which can also be used for other use cases.
	snprintf(ext_userfs_partition, CONFIG_PATH_MAX, "/dev/smart1p%d", index);
	fd = open(ext_userfs_partition, O_RDONLY);
	if (fd < 0) {
		return errno;
	}

	while (size) {
		// ToDo: Revert to the flexible logic that searches sectors and
		//       reads sector sizes stored in the sectors instead of
		//               using CONFIG_MTD_SMART_SECTOR_SIZE.
		memset(ext_fsdump_buf, 0, CONFIG_MTD_SMART_SECTOR_SIZE);
		i = read(fd, ext_fsdump_buf, CONFIG_MTD_SMART_SECTOR_SIZE);
		if (i < CONFIG_MTD_SMART_SECTOR_SIZE) {
			return ERROR;
		}

		ptr = (uint8_t *)ext_fsdump_buf;
		for (i = CONFIG_MTD_SMART_SECTOR_SIZE; i > 0; i--) {
			up_lowputc((uint8_t)*ptr);
			ptr++;
		}
		size -= CONFIG_MTD_SMART_SECTOR_SIZE;
	}

	return 0;
}

#ifdef CONFIG_BOARD_ASSERT_AUTORESET
static int do_reboot(void)
{
	/* Invoke the BOARDIOC_RESET board control to reset the board. If
	 * the board_reset() function returns, then it was not possible to
	 * reset the board due to some constraints.
	 */
	boardctl(BOARDIOC_RESET, EXIT_SUCCESS);
	/* boarctl() will not return in this case. It if does, it means that
	 * there was a problem with the reset operaion.
	 */
	return ERROR;
}
#endif

static int dump_via_uart(void)
{
	int i;
	int ch;
	char *target_str = RAMDUMP_HANDSHAKE_STRING;
	char host_buf[HANDSHAKE_STR_LEN_MAX] = "";

#if !defined(CONFIG_ARCH_LOWPUTC)
	/* If lowlevel serial is not available, ramdump is not possible */
	up_puts("\nLow level serial APIs not available, getting dump from TARGET not possible\n");
	return -1;
#endif

	/* Inform the terminal user */
	up_puts("\n************************************************************************************************\n");
	up_puts("\t\tDisconnect this serial terminal and run Dump Tool\n");
	up_puts("************************************************************************************************\n");

	if (CONFIG_FLASH_START_ADDR == 0xFFFFFFFF) {
		up_puts("\nFLASH start address is incorrect, Userfs Dump will not work. Please check CONFIG_FLASH_START_ADDR\n");
	}

	while (1) {
		/* Receive handshake string from HOST */
		do {
			host_buf[0] = up_getc();
		} while (host_buf[0] != target_str[0]);

		for (i = 1; i < HANDSHAKE_STR_LEN_MAX; ) {
			if ((ch = up_getc()) != -1) {
				host_buf[i] = ch;
				i++;
			}
		}

		/* Check handshake string against RAMDUMP HANDSHAKE string */
		if (strncmp(host_buf, RAMDUMP_HANDSHAKE_STRING, strlen(RAMDUMP_HANDSHAKE_STRING)) == 0) {
			send_ramdump();
		/* Check handshake string against FSDUMP HANDSHAKE string */
		} else if (strncmp(host_buf, FSDUMP_HANDSHAKE_STRING, strlen(FSDUMP_HANDSHAKE_STRING)) == 0) {
			i = send_fsdump();
			if (i != 0) {
				break;
			}
		/* Check handshake string for TARGET REBOOT signal */
		} else if (strncmp(host_buf, EXTFSDUMP_HANDSHAKE_STRING, strlen(EXTFSDUMP_HANDSHAKE_STRING)) == 0) {
			i = send_extfsdump();
			if (i != 0) {
				break;
			}
		} else if (strncmp(host_buf, TARGET_REBOOT_STRING, strlen(TARGET_REBOOT_STRING)) == 0) {
			/* Acknowledge the reboot signal, then reboot device if CONFIG is enabled, otherwise simply exit the function */
			up_lowputc('A');
#ifdef CONFIG_BOARD_ASSERT_AUTORESET
			ch = do_reboot();
			lldbg("Unable to reset device\n");
#endif
			break;
		/* Check handshake string for USERFS FILE DUMP signal */
		} else if (strncmp(host_buf, USERFS_FDUMP_HANDSHAKE_STRING, strlen(USERFS_FDUMP_HANDSHAKE_STRING)) == 0) {
			i = send_file();
			if (i != 0) {
				break;
			}
		} else {
			/* Send NAK */
			up_lowputc('N');
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

#if defined(CONFIG_WATCHDOG)
	/* system under panic and we are dumping system state.
	 * watchdog reset might stall the system, causing crashdump hang.
	 * Hence let's disable watchdog reset during crashdump process.
	 */
	up_watchdog_disable();
#endif

#if defined(CONFIG_BOARD_DUMP_UART)
	ret = dump_via_uart();
	if (ret != OK) {
		lldbg("Dumping contents via uart failed, ret = %d\n", ret);
	}
#endif

	if (ret == OK) {
		lldbg(" Successfull\n");
	}
}
#endif
