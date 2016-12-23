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
#include <errno.h>
#include <debug.h>
#include <arch/board/board.h>
#include "up_internal.h"
#include <tinyara/fota/fota.h>
#include <arch/board/s5jt200_fota.h>
#include <arch/board/s5jt200_partitions.h>

/****************************************************************************
 * Definitions
 ****************************************************************************/
#define SFLASH_ERASE_SECTOR_SIZE  (4 * 1024)	//4K
#define SFLASH_ERASE_SECTOR_CMD   (0x20)		//4K Erase CMD

/****************************************************************************
 * Private Data
 ****************************************************************************/
static uint32_t g_cur_part_id;
static uint32_t g_cur_bin_id;
static uint32_t g_cur_ota_part_offset;
static uint32_t g_cur_bin_part_offset;
static uint32_t g_cur_bin_write_offset;
static uint32_t g_cur_bin_read_offset;
static uint32_t g_sector_buf_index;
static bool g_sector_buffered = false;
static uint8_t sector_buf[SFLASH_ERASE_SECTOR_SIZE];

/* Nearly 2608KB of partition size required */
static uint32_t fota_binpart_offset_array[FOTA_BIN_ID_MAX] = {
	/* [FOTA_BIN_ID_TINYARA] */ 0x00000000,
	/* 1.5MB */
	/* [FOTA_BIN_ID_LEDFW]   */ 0x00180000,
	/* 32KB  */
	/* [FOTA_BIN_ID_SSSFW]   */ 0x00188000,
	/* 32KB  */
	/* [FOTA_BIN_ID_WLANFW]  */ 0x00190000,
	/* 752KB */
	/* [FOTA_BIN_ID_SSSKEY]  */ 0x0024C000,
	/* 256KB */
};

static uint32_t fota_otapart_offset_array[FOTA_PARTITION_MAX] = {
	/* [FOTA_PARTITION_OTA0] */ MTD_BLK_OTA0_START,
	/* [FOTA_PARTITION_OTA1] */ MTD_BLK_OTA1_START,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static int s5jt200_read_bootparam(boot_param_t *bootparam)
{
	int ret = ERROR;
	uint32_t boot_part_offset = MTD_BLK_BOOTPARAM_START;

	if (!bootparam) {
		return ret;
	}

	/* Read boot param */
	ret = s5j_sflash_read(NULL, boot_part_offset, bootparam, sizeof(boot_param_t));

	return ret;
}

static int s5jt200_fota_get_partition(void)
{
	boot_param_t bootparam;

	if (s5jt200_read_bootparam(&bootparam) != OK) {
		return ERROR;
	}

	if (bootparam.cur_partition < FOTA_PARTITION_OTA0 || bootparam.cur_partition >= FOTA_PARTITION_MAX) {
		bootparam.cur_partition = ERROR;
	}

	return bootparam.cur_partition;
}

static int s5jt200_fota_set_partition(uint32_t part_id)
{
	/* validate part_id */
	if ((part_id < FOTA_PARTITION_OTA0) || (part_id >= FOTA_PARTITION_MAX)) {
		return ERROR;
	}

	g_cur_part_id = part_id;
	g_cur_ota_part_offset = fota_otapart_offset_array[part_id];
	return OK;
}

static int s5jt200_fota_set_binary(uint32_t bin_id)
{
	/* validate bin_id */
	if ((bin_id < FOTA_BIN_ID_TINYARA) || (bin_id >= FOTA_BIN_ID_MAX)) {
		return ERROR;
	}

	g_cur_bin_id = bin_id;
	g_cur_bin_part_offset = fota_binpart_offset_array[bin_id];
	g_cur_bin_write_offset = 0;
	g_cur_bin_read_offset = 0;
	g_sector_buf_index = 0;
	g_sector_buffered = false;

	return OK;
}

static int s5jt200_fota_read(FAR const char *buffer, size_t buflen)
{
	/*TODO: Implement if required */

	return ERROR;
}

static int s5jt200_fota_write(FAR const char *buffer, size_t buflen)
{
	uint32_t len = 0;
	uint32_t size = buflen;
	uint8_t *p_sector_buf = (uint8_t *)buffer;
	uint32_t sector_len = SFLASH_ERASE_SECTOR_SIZE;
	uint32_t fota_part_offset = g_cur_ota_part_offset + g_cur_bin_part_offset;
	uint32_t fota_part_write_offset;

	/* validate the buffer */
	if (!buffer) {
		set_errno(EINVAL);
		return ERROR;
	}

	while (size > 0) {
		if ((size + g_sector_buf_index) != SFLASH_ERASE_SECTOR_SIZE) {
			while (g_sector_buf_index < SFLASH_ERASE_SECTOR_SIZE) {
				if (len >= buflen) {
					return buflen;
				}
				sector_buf[g_sector_buf_index] = buffer[len];
				g_sector_buf_index++;
				len++;
			}
			g_sector_buffered = true;
			p_sector_buf = sector_buf;
		} else {
			p_sector_buf = (uint8_t *)buffer + len;
		}

		fota_part_write_offset = fota_part_offset + g_cur_bin_write_offset;

		s5j_sflash_erase(NULL, SFLASH_ERASE_SECTOR_CMD, fota_part_write_offset);
		s5j_sflash_write(NULL, fota_part_write_offset, p_sector_buf, sector_len);

		g_cur_bin_write_offset += sector_len;
		g_sector_buf_index = 0;
		g_sector_buffered = false;
		size -= len;
	}

	return buflen;
}

static int s5jt200_fota_write_flush(void)
{
	uint32_t fota_part_offset = g_cur_ota_part_offset + g_cur_bin_part_offset;
	uint32_t fota_part_write_offset = fota_part_offset + g_cur_bin_write_offset;

	if (g_sector_buffered) {
		s5j_sflash_erase(NULL, SFLASH_ERASE_SECTOR_CMD, fota_part_write_offset);
		s5j_sflash_write(NULL, fota_part_write_offset, sector_buf, g_sector_buf_index);
	}

	g_sector_buffered = false;
	return OK;
}

static int s5jt200_fota_set_bootparam(fota_bootparam_type_t bootparam_type)
{
	int ret = OK;
	uint32_t boot_part_offset = MTD_BLK_BOOTPARAM_START;
	boot_param_t bootparam;

	switch (bootparam_type) {
	case FOTA_BOOTPARAM_TYPE_BOOTMODE:
		/* Read boot param */
		s5j_sflash_read(NULL, boot_part_offset, &bootparam, sizeof(bootparam));

		/* Modify boot param */
		bootparam.boot_mode = BOOTMODE_FOTA_UPDATE;

		/* Reset the fota_state */
		bootparam.fota_state = FOTA_STATE_COMPLETED;

		/* Erase */
		/* One 4K Erase Area will be sufficient for Bootparam */
		s5j_sflash_erase(NULL, SFLASH_ERASE_SECTOR_CMD, boot_part_offset);

		/* Write boot param */
		s5j_sflash_write(NULL, boot_part_offset, &bootparam, sizeof(bootparam));

		break;
	case FOTA_BOOTPARAM_TYPE_BINSTATE:
		/* Read boot param */
		s5j_sflash_read(NULL, boot_part_offset, &bootparam, sizeof(bootparam));

		/* Modify boot param */
		bootparam.fota_bins_state[g_cur_bin_id] = FOTA_BINSTATE_UPDATED;

		/* Erase */
		/* One 4K Erase Area will be sufficient for Bootparam */
		s5j_sflash_erase(NULL, SFLASH_ERASE_SECTOR_CMD, boot_part_offset);

		/* Write boot param */
		s5j_sflash_write(NULL, boot_part_offset, &bootparam, sizeof(bootparam));

		break;
	default:
		dbg(" unknown bootmode\n");
		ret = ERROR;
		break;
	}

	return ret;
}

static int s5jt200_fota_erase(void)
{
	/*TODO: Implement if required */

	return ERROR;
}

/* platform hooks for fota driver */
static fota_dev_t s5jt200_fota_dev = {
	s5jt200_fota_get_partition,
	s5jt200_fota_set_partition,
	s5jt200_fota_set_binary,
	s5jt200_fota_set_bootparam,
	s5jt200_fota_read,
	s5jt200_fota_write,
	s5jt200_fota_write_flush,
	s5jt200_fota_erase,
};

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: s5jt200_fota_init
 *
 * Description:
 *   Intialize FOTA for s5jt200
 *
 ****************************************************************************/
int s5jt200_fota_init(void)
{
	int ret = OK;

	if (fota_register(&s5jt200_fota_dev) != OK) {
		ret = ERROR;
	}

	return ret;
}

/****************************************************************************
 * Name: s5jt200_fota_update_notify
 *
 * Description:
 *   Notify if fota update done in previous boot
 *
 ****************************************************************************/
int s5jt200_fota_update_notify(void)
{
	boot_param_t bootparam;
	uint32_t boot_part_offset = MTD_BLK_BOOTPARAM_START;
	uint32_t ota0_phy_start = MTD_SFLASH_ADDR + MTD_BLK_OTA0_START;
	uint32_t ota1_phy_start = MTD_SFLASH_ADDR + MTD_BLK_OTA1_START;
	uint32_t ota0_phy_end = ota0_phy_start + MTD_BLK_OTA0_SIZE;
	uint32_t ota1_phy_end = ota1_phy_start + MTD_BLK_OTA1_SIZE;
	uint32_t start_text = (uint32_t)&_stext;

	/* Read boot param */
	s5j_sflash_read(NULL, boot_part_offset, &bootparam, sizeof(bootparam));

	/* FIX ME:
	 * Since bootloader has not modified boot_mode,
	 * temperarely using it to print fota status
	 * We should only use fota_state here
	 */
	if (bootparam.boot_mode == BOOTMODE_FOTA_UPDATE) {
		dbg("FOTA UPDATE SUCCESSFUL\n");

	} else {
		if (bootparam.fota_state == FOTA_STATE_FAILED) {
			dbg("FOTA UPDATE FAILED\n");
		} else if (bootparam.fota_state == FOTA_STATE_PROGRESSING) {
			dbg("FOTA UPDATE SUCCESSFUL\n");
		}
	}

	bootparam.fota_state = FOTA_STATE_COMPLETED;

	/* update boot param to complete fota update sequence */
	/* set boot_mode to normal */
	bootparam.boot_mode = BOOTMODE_NORMALBOOT;
	/* update fota state */
	bootparam.fota_state = FOTA_STATE_COMPLETED;

	/*FIXME:
	 * Get cur_partition(running partition)
	 * Below one might be crude way,
	 */

	//dbg("start = %08x, end = %08x, text = %08x\n", ota0_phy_start, ota0_phy_end, start_text);
	if (start_text >= ota0_phy_start && start_text <= ota0_phy_end) {
		bootparam.cur_partition = FOTA_PARTITION_OTA0;
	} else if (start_text >= ota1_phy_start && start_text <= ota1_phy_end) {
		bootparam.cur_partition = FOTA_PARTITION_OTA1;
	} else {
		bootparam.cur_partition = 255;
	}

	dbg("booted from [ OTA%d ] partition\n", bootparam.cur_partition);

	/* Erase */
	/* One 4K Erase Area will be sufficient for Bootparam */
	s5j_sflash_erase(NULL, SFLASH_ERASE_SECTOR_CMD, boot_part_offset);

	/* Write boot param */
	s5j_sflash_write(NULL, boot_part_offset, &bootparam, sizeof(bootparam));

	return OK;
}
