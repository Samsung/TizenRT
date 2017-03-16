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
 * arch/arm/src/sidk_s5jt200/src/s5jt200_boot.c
 *
 *   Copyright (C) 2009, 2011, 2013, 2015 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *           Laurent Latil <laurent@latil.nom.fr>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ************************************************************************************/

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <tinyara/config.h>
#include <tinyara/spi/spi.h>
#ifdef CONFIG_MTD_PARTITION
#include <tinyara/fs/mtd.h>
#include <tinyara/fs/ioctl.h>
#endif
#include <debug.h>
#include <assert.h>

#include <tinyara/board.h>
#include <arch/board/board.h>
#include <arch/board/s5jt200_fota.h>
#include <arch/board/s5jt200_partitions.h>

#include <sys/mount.h>
#ifdef CONFIG_SCSC_WLAN
#include <net/lwip/opt.h>
#include <net/lwip/netif.h>
#include <net/lwip/tcpip.h>
#endif
#include "up_arch.h"

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/
#define PHY_ADDR(offset)  (MTD_SFLASH_ADDR + (offset))

/************************************************************************************
 * Public Functions Prototypes
 ************************************************************************************/
#if defined(CONFIG_BOARD_COREDUMP_FLASH) || defined(CONFIG_BOARD_RAMDUMP_FLASH)
extern FAR struct spi_dev_s *up_spiflashinitialize(void);
#endif

#if defined(CONFIG_BOARD_COREDUMP_FLASH)
extern void up_coredump_init(void);
#endif

/************************************************************************************
 * Private Data
 ************************************************************************************/

/************************************************************************************
 * Private Functions
 ************************************************************************************/
static void sflash_partitionmap_dump(void)
{
	int32_t size;
	int32_t part_no;
	uint32_t end_addr;
	uint32_t start_addr;

	dbg("*****************************************************************************\n");
	dbg("*PARTITION NO\tPARTITION NAME\tSTART ADDRESS\tEND ADDRESS\t      SIZE\n");
	dbg("*****************************************************************************\n");

	/* Bootloader Partition */
	part_no = 1;
	start_addr = PHY_ADDR(MTD_BLK_BOOTLDR_START);
	size = MTD_BLK_BOOTLDR_SIZE;
	end_addr = start_addr + size;
	dbg("*%12d\t%14s\t%#13X\t%#11X\t%10d\n", part_no, "BOOTLOADER", start_addr, end_addr, size);

	/* OTA0 Partition */
	part_no = 2;
	start_addr = PHY_ADDR(MTD_BLK_OTA0_START);
	size = MTD_BLK_OTA0_SIZE;
	end_addr = start_addr + size;
	dbg("*%12d\t%14s\t%#13X\t%#11X\t%10d\n", part_no, "OTA0", start_addr, end_addr, size);

	/* LED F/W Partition */
	part_no = 3;
	start_addr = PHY_ADDR(MTD_BLK_LEDFW_START);
	size = MTD_BLK_LEDFW_SIZE;
	end_addr = start_addr + size;
	dbg("*%12d\t%14s\t%#13X\t%#11X\t%10d\n", part_no, "LED F/W", start_addr, end_addr, size);

	/* SSS F/W Partition */
	part_no = 4;
	start_addr = PHY_ADDR(MTD_BLK_SSSFW_START);
	size = MTD_BLK_SSSFW_SIZE;
	end_addr = start_addr + size;
	dbg("*%12d\t%14s\t%#13X\t%#11X\t%10d\n", part_no, "SSS F/W", start_addr, end_addr, size);

	/* WLAN F/W Partition */
	part_no = 5;
	start_addr = PHY_ADDR(MTD_BLK_WLANFW_START);
	size = MTD_BLK_WLANFW_SIZE;
	end_addr = start_addr + size;
	dbg("*%12d\t%14s\t%#13X\t%#11X\t%10d\n", part_no, "WLAN F/W", start_addr, end_addr, size);

	/* SSS KEY Partition */
	part_no = 6;
	start_addr = PHY_ADDR(MTD_BLK_SSSKEY_START);
	size = MTD_BLK_SSSKEY_SIZE;
	end_addr = start_addr + size;
	dbg("*%12d\t%14s\t%#13X\t%#11X\t%10d\n", part_no, "SSS KEY", start_addr, end_addr, size);

	/* OTA1 Partition */
	part_no = 7;
	start_addr = PHY_ADDR(MTD_BLK_OTA1_START);
	size = MTD_BLK_OTA1_SIZE;
	end_addr = start_addr + size;
	dbg("*%12d\t%14s\t%#13X\t%#11X\t%10d\n", part_no, "OTA1", start_addr, end_addr, size);

	/* ROMFS Partition */
	part_no = 8;
	start_addr = PHY_ADDR(MTD_BLK_ROMFS_START);
	size = MTD_BLK_ROMFS_SIZE;
	end_addr = start_addr + size;
	dbg("*%12d\t%14s\t%#13X\t%#11X\t%10d\n", part_no, "ROMFS", start_addr, end_addr, size);

	/* SMARTFS Partition */
	part_no = 9;
	start_addr = PHY_ADDR(MTD_BLK_SMARTFS_START);
	size = MTD_BLK_SMARTFS_SIZE;
	end_addr = start_addr + size;
	dbg("*%12d\t%14s\t%#13X\t%#11X\t%10d\n", part_no, "SMARTFS", start_addr, end_addr, size);

	/* RAMDUMP Partition */
	part_no = 10;
	start_addr = PHY_ADDR(MTD_BLK_RAMDUMP_START);
	size = MTD_BLK_RAMDUMP_SIZE;
	end_addr = start_addr + size;
	dbg("*%12d\t%14s\t%#13X\t%#11X\t%10d\n", part_no, "RAMDUMP", start_addr, end_addr, size);

	/* COREDUMP Partition */
	part_no = 11;
	start_addr = PHY_ADDR(MTD_BLK_COREDUMP_START);
	size = MTD_BLK_COREDUMP_SIZE;
	end_addr = start_addr + size;
	dbg("*%12d\t%14s\t%#13X\t%#11X\t%10d\n", part_no, "COREDUMP", start_addr, end_addr, size);

	/* NV Partition */
	part_no = 12;
	start_addr = PHY_ADDR(MTD_BLK_NV_START);
	size = MTD_BLK_NV_SIZE;
	end_addr = start_addr + size;
	dbg("*%12d\t%14s\t%#13X\t%#11X\t%10d\n", part_no, "NV", start_addr, end_addr, size);

	/* BOOTPARAM Partition */
	part_no = 13;
	start_addr = PHY_ADDR(MTD_BLK_BOOTPARAM_START);
	size = MTD_BLK_BOOTPARAM_SIZE;
	end_addr = start_addr + size;
	dbg("*%12d\t%14s\t%#13X\t%#11X\t%10d\n", part_no, "BOOTPARAM", start_addr, end_addr, size);

	dbg("*****************************************************************************\n");

}

/************************************************************************************
 * Public Functions
 ************************************************************************************/
/****************************************************************************
 * Name: up_create_romdevice
 *
 * Description:
 *  It creates a smart device using ROMFS MTD Partition
 ****************************************************************************/

#ifdef CONFIG_FS_ROMFS
int up_create_romdevice(void)
{
	int ret = 0;
	int minor = MTD_ROMFS;		/* Use MTD_ROMFS as minor number */
#ifdef CONFIG_MTD
	FAR struct mtd_dev_s *rom_mtd = NULL;

	rom_mtd = get_mtd_partition(MTD_ROMFS);
	if (!rom_mtd) {
		dbg("ERROR: Failed to get ROMFS partition\n");
		ret = 1;
		goto error_out;
	}
#ifdef CONFIG_MTD_SMART
	ret = smart_initialize(minor, rom_mtd, NULL);
	if (ret != OK) {
		dbg("ERROR: SMART initialization failed: %d\n", -ret);
		goto error_out;
	}
#endif
#endif
error_out:
	return ret > 0 ? -ret : ret;
}
#endif
/****************************************************************************
 * Name: up_flashinitialize
 *
 * Description:
 *   Create an initialized MTD device instance for internal flash.
 *   Initialize MTD master & each of partitions here.
 *   Each of partition classfied according to TAG number
 *
 ****************************************************************************/
FAR struct mtd_dev_s *up_flashinitialize(void)
{
	FAR struct mtd_dev_s *mtd = NULL;
	FAR struct spi_dev_s *spi;
#ifdef CONFIG_MTD_PARTITION
	int ret;
	FAR struct mtd_geometry_s geo;
#ifdef CONFIG_FS_ROMFS
	FAR struct mtd_dev_s *mtd_romfs;
#endif
#ifdef CONFIG_NV_MANAGER
	FAR struct mtd_dev_s *mtd_nvm;
#endif
	FAR struct mtd_dev_s *mtd_fs;
	uint32_t nblocks;
	uint32_t startblock;
	uint32_t blocksize;
#endif

	spi = (FAR struct spi_dev_s *)up_spiflashinitialize();
	if (!spi) {
		return NULL;
	}
#ifdef CONFIG_MTD_M25P
	mtd = (FAR struct mtd_dev_s *)m25p_initialize(spi);
	if (!mtd) {
		return NULL;
	}
#endif

#ifdef CONFIG_MTD_PARTITION

	DEBUGASSERT(mtd);

	/* Get the geometry of the FLASH device */

	ret = mtd->ioctl(mtd, MTDIOC_GEOMETRY, (unsigned long)((uintptr_t)&geo));
	if (ret < 0) {
		dbg("ERROR: mtd->ioctl failed: %d\n", ret);
		return NULL;
	}

	blocksize = geo.blocksize;

#ifdef CONFIG_FS_ROMFS
	/* ROMFS Partition */
	startblock = MTD_BLK_ROMFS_START / blocksize;
	nblocks = MTD_BLK_ROMFS_SIZE / blocksize;
	mtd_romfs = mtd_partition(mtd, startblock, nblocks, MTD_ROMFS);
	if (!mtd_romfs) {
		dbg("ERROR: mtd partition failed for romfs\n");
		return NULL;
	}
#endif

#ifdef CONFIG_NV_MANAGER
	startblock = MTD_BLK_NV_START / blocksize;
	nblocks = MTD_BLK_NV_SIZE / blocksize;
	mtd_nvm = mtd_partition(mtd, startblock, nblocks, MTD_NV);
	if (!mtd_nvm) {
		dbg("ERROR: mtd partition failed for nvm manager\n");
		return NULL;
	}
#endif

	/* FS Partition, SMARTFS will be mounted on it */
	startblock = MTD_BLK_SMARTFS_START / blocksize;
	nblocks = MTD_BLK_SMARTFS_SIZE / blocksize;
	mtd_fs = mtd_partition(mtd, startblock, nblocks, MTD_FS);
	if (!mtd_fs) {
		dbg("ERROR: mtd partition failed for smartfs\n");
		return NULL;
	}
#ifdef CONFIG_MTD_REGISTRATION
	/* register each of MTD here to get info through the procfs */
	if (mtd) {
		mtd_register(mtd, "master");
	}

	if (mtd_fs) {
		mtd_register(mtd_fs, "FS");
	}
#ifdef CONFIG_NV_MANAGER
	if (mtd_nvm) {
		mtd_register(mtd_nvm, "NVM");
	}
#endif
#ifdef CONFIG_FS_ROMFS
	if (mtd_romfs) {
		mtd_register(mtd_romfs, "ROMFS");
	}
#endif							/* CONFIG_FS_ROMFS */
#endif							/* CONFIG_MTD_REGISTRATION */
#endif							/* CONFIG_MTD_PARTITION */

	return mtd;
}

#ifdef CONFIG_SCSC_WLAN
#include <net/lwip/opt.h>
#include <net/lwip/netif.h>

err_t wlan_init(struct netif *netif);

struct netif g_wlanif;
struct netif *wlan_netif;
void wlan_initup(struct netif *dev)
{
	struct ip_addr ipaddr;
	struct ip_addr netmask;
	struct ip_addr gw;

	/* Start LWIP network thread */

//  memset(g_wlanif, 0, sizeof(struct netif));

	ipaddr.addr = inet_addr("0.0.0.0");
	netmask.addr = inet_addr("255.255.255.255");
	gw.addr = inet_addr("0.0.0.0");

	netif_set_default(dev);

	wlan_netif = netif_add(dev, &ipaddr, &netmask, &gw, NULL, wlan_init, tcpip_input);
	wlan_netif->flags |= NETIF_FLAG_IGMP;
}

#endif

/****************************************************************************
 * Name: board_initialize
 *
 * Description:
 *   If CONFIG_BOARD_INITIALIZE is selected, then an additional
 *   initialization call will be performed in the boot-up sequence to a
 *   function called board_initialize(). board_initialize() will be
 *   called immediately after up_intiialize() is called and just before the
 *   initial application is started.  This additional initialization phase
 *   may be used, for example, to initialize board-specific device drivers.
 *
 ****************************************************************************/

#ifdef CONFIG_BOARD_INITIALIZE
void board_initialize(void)
{
	unsigned int val;

	/* Perform app-specific initialization here instead of from the TASH. */
	board_app_initialize();

#ifdef CONFIG_SCSC_WLAN
	slldbg("Initialize SCSC driver\n");
	slsi_driver_initialize();
#endif

#if defined(CONFIG_BOARD_COREDUMP_FLASH) || defined(CONFIG_BOARD_RAMDUMP_FLASH)
	up_spiflashinitialize();
#endif

#if defined(CONFIG_BOARD_COREDUMP_FLASH)
	up_coredump_init();
#endif

	/* sflash_partitionmap_dump(); */
	/* temoprally disable watchdog reset */
#define WDT_EN        (1 << 5)
#define WDT_INT_EN    (1 << 2)
#define WDT_RESET_EN  (1 << 0)
	val = getreg32(0x80030000 + 0x0);
	val &= ~(WDT_EN);
	putreg32(val, 0x80030000 + 0x0);

#if 0
#ifdef CONFIG_FS_PROCFS
	int ret;
	ret = mount(NULL, "/proc", "procfs", 0, NULL);
	if (ret < 0) {
		syslog(LOG_ERR, "ERROR: Failed to mount the PROC filesystem: %d \n", ret);
		return;
	}
#endif
#endif
}
#endif /* CONFIG_BOARD_INITIALIZE */
