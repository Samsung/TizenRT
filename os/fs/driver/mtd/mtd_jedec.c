/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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
  * drivers/mtd/mtd_jedec.c
  *
  *   Copyright (C) 2009, 2011-2012, 2016 Gregory Nutt. All rights reserved.
  *   Author: Gregory Nutt <gnutt@nuttx.org>
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
  ****************************************************************************/

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/kmalloc.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/spi/spi.h>
#include <tinyara/fs/mtd.h>

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

#ifndef CONFIG_JEDEC_SPI_MODE
#define CONFIG_JEDEC_SPI_MODE	SPIDEV_MODE0
#endif

#ifndef CONFIG_JEDEC_SPI_FREQUENCY
#define CONFIG_JEDEC_SPI_FREQUENCY	(20000000)
#endif

/* Instructions */
/*      Command        Value	   N Description             Addr Dummy Data   */
#define JEDEC_WREN      0x06	/* 1 Write Enable              0   0     0     */
#define JEDEC_WRDI      0x04	/* 1 Write Disable             0   0     0     */
#define JEDEC_RDID      0x9f	/* 1 Read Identification       0   0     1-3   */
#define JEDEC_RDSR      0x05	/* 1 Read Status Register      0   0     >=1   */
#define JEDEC_WRSR      0x01	/* 1 Write Status Register     0   0     1     */
#define JEDEC_READ      0x03	/* 1 Read Data Bytes           3   0     >=1   */
#define JEDEC_FAST_READ 0x0b	/* 1 Higher speed read         3   1     >=1   */
#define JEDEC_PP        0x02	/* 1 Page Program              3   0     1-256 */
#define JEDEC_SE        0xd8	/* 1 Sector Erase              3   0     0     */
#define JEDEC_BE        0xc7	/* 1 Bulk Erase                0   0     0     */
#define JEDEC_DP        0xb9	/* 2 Deep power down           0   0     0     */
#define JEDEC_RES       0xab	/* 2 Read Electronic Signature 0   3     >=1   */
#define JEDEC_SSE       0x20	/* 3 Sub-Sector Erase          0   0     0     */

/* Status register bit definitions */
#define JEDEC_SR_WIP            (1 << 0)	/* Bit 0: Write in progress bit */
#define JEDEC_SR_WEL            (1 << 1)	/* Bit 1: Write enable latch bit */
#define JEDEC_SR_BP_SHIFT       (2)			/* Bits 2-4: Block protect bits */
#define JEDEC_SR_BP_MASK        (7 << JEDEC_SR_BP_SHIFT)
#define JEDEC_SR_BP_NONE        (0 << JEDEC_SR_BP_SHIFT)	/* Unprotected */
#define JEDEC_SR_BP_UPPER64th   (1 << JEDEC_SR_BP_SHIFT)	/* Upper 64th */
#define JEDEC_SR_BP_UPPER32nd   (2 << JEDEC_SR_BP_SHIFT)	/* Upper 32nd */
#define JEDEC_SR_BP_UPPER16th   (3 << JEDEC_SR_BP_SHIFT)	/* Upper 16th */
#define JEDEC_SR_BP_UPPER8th    (4 << JEDEC_SR_BP_SHIFT)	/* Upper 8th */
#define JEDEC_SR_BP_UPPERQTR    (5 << JEDEC_SR_BP_SHIFT)	/* Upper quarter */
#define JEDEC_SR_BP_UPPERHALF   (6 << JEDEC_SR_BP_SHIFT)	/* Upper half */
#define JEDEC_SR_BP_ALL         (7 << JEDEC_SR_BP_SHIFT)	/* All sectors */
/* Bits 5-6:  Unused, read zero */
#define JEDEC_SR_SRWD           (1 << 7)	/* Bit 7: Status register write protect */

#define JEDEC_DUMMY		(0xa5)

/************************************************************************************
 * Private Types
 ************************************************************************************/

/* Structure for static mapping of different flash's */
struct jedec_geometry_info_s {
	uint16_t manufacturer;		/* Manufacturer ID */
	uint16_t memory;			/* Memory Type */
	uint16_t capacity;			/* Memory Capacity */
	char *manufacturer_name;	/* Manufacturer Name */
	char *flash_size;			/* Flash Size string */
	uint8_t sectorshift;		/* Size of erasable block */
	uint16_t nsectors;			/* Number of erasable blocks */
	uint8_t pageshift;			/* Size of programmable page */
	uint32_t npages;			/* Number of pages */
	uint8_t subsectorshift;		/* Size of subsector(Ex: 12 for 4K) */
};

typedef struct jedec_geometry_info_s jedec_geometry_t;

/* Update this list for new spi based flash */
jedec_geometry_t g_jedec_device_info_list[] = {
	{0xA1, 0x28, 0x16, "Fudan", "4MB", 16, 64, 8, 16384, 12},
	{0xEF, 0x40, 0x16, "Winbond", "4MB", 16, 64, 8, 16384, 12},
	{0xEF, 0x40, 0x17, "Winbond", "8MB", 16, 128, 8, 32768, 12},
	{0x9D, 0x60, 0x16, "ISSI",    "4MB", 16, 64,  8, 16384, 12},
};

/* This type represents the state of the MTD device.  The struct mtd_dev_s
 * must appear at the beginning of the definition so that you can freely
 * cast between pointers to struct mtd_dev_s and struct jedec_dev_s.
 */

struct jedec_dev_s {
	struct mtd_dev_s mtd;		/* MTD interface */
	FAR struct spi_dev_s *dev;	/* Saved SPI interface instance */
	jedec_geometry_t geo;
};


/************************************************************************************
 * Private Function Prototypes
 ************************************************************************************/

/* Helpers */

static void jedec_lock(FAR struct spi_dev_s *dev);
static inline void jedec_unlock(FAR struct spi_dev_s *dev);
static inline int jedec_readid(struct jedec_dev_s *priv);
static void jedec_waitwritecomplete(struct jedec_dev_s *priv);
static void jedec_writeenable(struct jedec_dev_s *priv);
static inline void jedec_sectorerase(struct jedec_dev_s *priv, off_t offset, uint8_t type);
static inline int jedec_bulkerase(struct jedec_dev_s *priv);
static inline void jedec_pagewrite(struct jedec_dev_s *priv, FAR const uint8_t *buffer, off_t offset);

/* MTD driver methods */

static int jedec_erase(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks);
static ssize_t jedec_bread(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks, FAR uint8_t *buf);
static ssize_t jedec_bwrite(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks, FAR const uint8_t *buf);
static ssize_t jedec_read(FAR struct mtd_dev_s *dev, off_t offset, size_t nbytes, FAR uint8_t *buffer);
#ifdef CONFIG_MTD_BYTE_WRITE
static ssize_t jedec_write(FAR struct mtd_dev_s *dev, off_t offset, size_t nbytes, FAR const uint8_t *buffer);
#endif
static int jedec_ioctl(FAR struct mtd_dev_s *dev, int cmd, unsigned long arg);

/************************************************************************************
 * Private Data
 ************************************************************************************/

/************************************************************************************
 * Private Functions
 ************************************************************************************/
static int jedec_set_geometry(struct jedec_dev_s *priv, uint16_t manufacturer,
					uint16_t memory, uint16_t capacity)
{
	int i;

	for (i = 0; i < sizeof(g_jedec_device_info_list) / sizeof(g_jedec_device_info_list[0]); i++) {

		if ((manufacturer == g_jedec_device_info_list[i].manufacturer) &&
			(memory == g_jedec_device_info_list[i].memory) &&
			(capacity == g_jedec_device_info_list[i].capacity)) {

			/* Save the FLASH geometry */

			priv->geo.sectorshift = g_jedec_device_info_list[i].sectorshift;
			priv->geo.nsectors = g_jedec_device_info_list[i].nsectors;
			priv->geo.pageshift = g_jedec_device_info_list[i].pageshift;
			priv->geo.npages = g_jedec_device_info_list[i].npages;
			priv->geo.subsectorshift = g_jedec_device_info_list[i].subsectorshift;

			fdbg("Vendor : %s, Size = %s\n", g_jedec_device_info_list[i].manufacturer_name,
				g_jedec_device_info_list[i].flash_size);

			return OK;
		}

	}

	return ERROR;
}

/************************************************************************************
 * Name: jedec_lock
 ************************************************************************************/

static void jedec_lock(FAR struct spi_dev_s *dev)
{
	/* On SPI busses where there are multiple devices, it will be necessary to
	 * lock SPI to have exclusive access to the busses for a sequence of
	 * transfers.  The bus should be locked before the chip is selected.
	 *
	 * This is a blocking call and will not return until we have exclusiv access to
	 * the SPI buss.  We will retain that exclusive access until the bus is unlocked.
	 */

	(void)SPI_LOCK(dev, true);

	/* After locking the SPI bus, the we also need call the setfrequency, setbits, and
	 * setmode methods to make sure that the SPI is properly configured for the device.
	 * If the SPI buss is being shared, then it may have been left in an incompatible
	 * state.
	 */

	SPI_SETMODE(dev, CONFIG_JEDEC_SPI_MODE);
	SPI_SETBITS(dev, 8);
	(void)SPI_SETFREQUENCY(dev, CONFIG_JEDEC_SPI_FREQUENCY);
}

/************************************************************************************
 * Name: jedec_unlock
 ************************************************************************************/

static inline void jedec_unlock(FAR struct spi_dev_s *dev)
{
	(void)SPI_LOCK(dev, false);
}

/************************************************************************************
 * Name: jedec_readid
 ************************************************************************************/

static inline int jedec_readid(struct jedec_dev_s *priv)
{
	uint16_t manufacturer;
	uint16_t memory;
	uint16_t capacity;

	fvdbg("priv: %p\n", priv);

	/* Lock the SPI bus, configure the bus, and select this FLASH part. */

	jedec_lock(priv->dev);
	SPI_SELECT(priv->dev, SPIDEV_FLASH, true);

	/* Send the "Read ID (RDID)" command and read the first three ID bytes */

	(void)SPI_SEND(priv->dev, JEDEC_RDID);
	manufacturer = SPI_SEND(priv->dev, JEDEC_DUMMY);
	memory = SPI_SEND(priv->dev, JEDEC_DUMMY);
	capacity = SPI_SEND(priv->dev, JEDEC_DUMMY);

	/* Deselect the FLASH and unlock the bus */

	SPI_SELECT(priv->dev, SPIDEV_FLASH, false);
	jedec_unlock(priv->dev);

	fvdbg("manufacturer: %02x memory: %02x capacity: %02x\n", manufacturer, memory, capacity);

	if (jedec_set_geometry(priv, manufacturer, memory, capacity) != OK) {
		fdbg("can't find such kind of driver\n");
		return -ENODEV;
	}

	return OK;
}

/************************************************************************************
 * Name: jedec_waitwritecomplete
 ************************************************************************************/

static void jedec_waitwritecomplete(struct jedec_dev_s *priv)
{
	uint8_t status;

	/* Loop as long as the memory is busy with a write cycle */

	do {
		/* Select this FLASH part */

		SPI_SELECT(priv->dev, SPIDEV_FLASH, true);

		/* Send "Read Status Register (RDSR)" command */

		(void)SPI_SEND(priv->dev, JEDEC_RDSR);

		/* Send a dummy byte to generate the clock needed to shift out the status */

		status = SPI_SEND(priv->dev, JEDEC_DUMMY);

		/* Deselect the FLASH */

		SPI_SELECT(priv->dev, SPIDEV_FLASH, false);

		/* Given that writing could take up to few tens of milliseconds, and erasing
		 * could take more.  The following short delay in the "busy" case will allow
		 * other peripherals to access the SPI bus.
		 */

		if ((status & JEDEC_SR_WIP) != 0) {
			jedec_unlock(priv->dev);
			usleep(1000);
			jedec_lock(priv->dev);
		}
	} while ((status & JEDEC_SR_WIP) != 0);

	fvdbg("Complete\n");
}

/************************************************************************************
 * Name:  jedec_writeenable
 ************************************************************************************/

static void jedec_writeenable(struct jedec_dev_s *priv)
{
	/* Select this FLASH part */

	SPI_SELECT(priv->dev, SPIDEV_FLASH, true);

	/* Send "Write Enable (WREN)" command */

	(void)SPI_SEND(priv->dev, JEDEC_WREN);

	/* Deselect the FLASH */

	SPI_SELECT(priv->dev, SPIDEV_FLASH, false);
	fvdbg("Enabled\n");
}

/************************************************************************************
 * Name:  jedec_sectorerase
 ************************************************************************************/

static void jedec_sectorerase(struct jedec_dev_s *priv, off_t sector, uint8_t type)
{
	off_t offset;

	if (priv->geo.subsectorshift > 0) {
		offset = sector << priv->geo.subsectorshift;
	} else {
		offset = sector << priv->geo.sectorshift;
	}

	fvdbg("sector: %08lx\n", (long)sector);

	/* Wait for any preceding write to complete.  We could simplify things by
	 * perform this wait at the end of each write operation (rather than at
	 * the beginning of ALL operations), but have the wait first will slightly
	 * improve performance.
	 */

	jedec_waitwritecomplete(priv);

	/* Send write enable instruction */

	jedec_writeenable(priv);

	/* Select this FLASH part */

	SPI_SELECT(priv->dev, SPIDEV_FLASH, true);

	/* Send the "Sector Erase (SE)" or Sub-Sector Erase (SSE) instruction
	 * that was passed in as the erase type.
	 */

	(void)SPI_SEND(priv->dev, type);

	/* Send the sector offset high byte first.  For all of the supported
	 * parts, the sector number is completely contained in the first byte
	 * and the values used in the following two bytes don't really matter.
	 */

	(void)SPI_SEND(priv->dev, (offset >> 16) & 0xff);
	(void)SPI_SEND(priv->dev, (offset >> 8) & 0xff);
	(void)SPI_SEND(priv->dev, offset & 0xff);

	/* Deselect the FLASH */

	SPI_SELECT(priv->dev, SPIDEV_FLASH, false);
	fvdbg("Erased\n");
}

/************************************************************************************
 * Name:  jedec_bulkerase
 ************************************************************************************/

static inline int jedec_bulkerase(struct jedec_dev_s *priv)
{
	fvdbg("priv: %p\n", priv);

	/* Wait for any preceding write to complete.  We could simplify things by
	 * perform this wait at the end of each write operation (rather than at
	 * the beginning of ALL operations), but have the wait first will slightly
	 * improve performance.
	 */

	jedec_waitwritecomplete(priv);

	/* Send write enable instruction */

	jedec_writeenable(priv);

	/* Select this FLASH part */

	SPI_SELECT(priv->dev, SPIDEV_FLASH, true);

	/* Send the "Bulk Erase (BE)" instruction */

	(void)SPI_SEND(priv->dev, JEDEC_BE);

	/* Deselect the FLASH */

	SPI_SELECT(priv->dev, SPIDEV_FLASH, false);
	fvdbg("Return: OK\n");
	return OK;
}

/************************************************************************************
 * Name:  jedec_pagewrite
 ************************************************************************************/

static inline void jedec_pagewrite(struct jedec_dev_s *priv, FAR const uint8_t *buffer, off_t page)
{
	off_t offset = page << priv->geo.pageshift;

	fvdbg("page: %08lx offset: %08lx\n", (long)page, (long)offset);

	/* Wait for any preceding write to complete.  We could simplify things by
	 * perform this wait at the end of each write operation (rather than at
	 * the beginning of ALL operations), but have the wait first will slightly
	 * improve performance.
	 */

	jedec_waitwritecomplete(priv);

	/* Enable the write access to the FLASH */

	jedec_writeenable(priv);

	/* Select this FLASH part */

	SPI_SELECT(priv->dev, SPIDEV_FLASH, true);

	/* Send "Page Program (PP)" command */

	(void)SPI_SEND(priv->dev, JEDEC_PP);

	/* Send the page offset high byte first. */

	(void)SPI_SEND(priv->dev, (offset >> 16) & 0xff);
	(void)SPI_SEND(priv->dev, (offset >> 8) & 0xff);
	(void)SPI_SEND(priv->dev, offset & 0xff);

	/* Then write the specified number of bytes */

	SPI_SNDBLOCK(priv->dev, buffer, 1 << priv->geo.pageshift);

	/* Deselect the FLASH: Chip Select high */

	SPI_SELECT(priv->dev, SPIDEV_FLASH, false);
	fvdbg("Written\n");
}

/************************************************************************************
 * Name:  jedec_bytewrite
 ************************************************************************************/

#ifdef CONFIG_MTD_BYTE_WRITE
static inline void jedec_bytewrite(struct jedec_dev_s *priv, FAR const uint8_t *buffer, off_t offset, uint16_t count)
{
	fvdbg("offset: %08lx  count:%d\n", (long)offset, count);

	/* Wait for any preceding write to complete.  We could simplify things by
	 * perform this wait at the end of each write operation (rather than at
	 * the beginning of ALL operations), but have the wait first will slightly
	 * improve performance.
	 */

	jedec_waitwritecomplete(priv);

	/* Enable the write access to the FLASH */

	jedec_writeenable(priv);

	/* Select this FLASH part */

	SPI_SELECT(priv->dev, SPIDEV_FLASH, true);

	/* Send "Page Program (PP)" command */

	(void)SPI_SEND(priv->dev, JEDEC_PP);

	/* Send the page offset high byte first. */

	(void)SPI_SEND(priv->dev, (offset >> 16) & 0xff);
	(void)SPI_SEND(priv->dev, (offset >> 8) & 0xff);
	(void)SPI_SEND(priv->dev, offset & 0xff);

	/* Then write the specified number of bytes */

	SPI_SNDBLOCK(priv->dev, buffer, count);

	/* Deselect the FLASH: Chip Select high */

	SPI_SELECT(priv->dev, SPIDEV_FLASH, false);
	fvdbg("Written\n");
}
#endif

/************************************************************************************
 * Name: jedec_erase
 ************************************************************************************/

static int jedec_erase(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks)
{
	FAR struct jedec_dev_s *priv = (FAR struct jedec_dev_s *)dev;
	size_t blocksleft = nblocks;

	fvdbg("startblock: %08lx nblocks: %d\n", (long)startblock, (int)nblocks);

	/* Lock access to the SPI bus until we complete the erase */

	jedec_lock(priv->dev);
	while (blocksleft > 0) {
		size_t sectorboundry;
		size_t blkper;

		/* If we have a smaller erase size, then we will find as many full
		 * sector erase blocks as possible to speed up the process instead of
		 * erasing everything in smaller chunks.
		 */

		if (priv->geo.subsectorshift > 0) {
			blkper = 1 << (priv->geo.sectorshift - priv->geo.subsectorshift);
			sectorboundry = (startblock + blkper - 1) / blkper;
			sectorboundry *= blkper;

			/* If we are on a sector boundry and have at least a full sector
			 * of blocks left to erase, then we can do a full sector erase.
			 */

			if (startblock == sectorboundry && blocksleft >= blkper) {
				/* Do a full sector erase */

				jedec_sectorerase(priv, startblock, JEDEC_SE);
				startblock += blkper;
				blocksleft -= blkper;
				continue;
			} else {
				/* Just do a sub-sector erase */

				jedec_sectorerase(priv, startblock, JEDEC_SSE);
				startblock++;
				blocksleft--;
				continue;
			}
		}

		/* Not using sub-sector erase.  Erase each full sector */

		jedec_sectorerase(priv, startblock, JEDEC_SE);
		startblock++;
		blocksleft--;
	}

	jedec_unlock(priv->dev);
	return (int)nblocks;
}

/************************************************************************************
 * Name: jedec_bread
 ************************************************************************************/

static ssize_t jedec_bread(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks, FAR uint8_t *buffer)
{
	FAR struct jedec_dev_s *priv = (FAR struct jedec_dev_s *)dev;
	ssize_t nbytes;

	fvdbg("startblock: %08lx nblocks: %d\n", (long)startblock, (int)nblocks);

	/* On this device, we can handle the block read just like the byte-oriented read */

	nbytes = jedec_read(dev, startblock << priv->geo.pageshift, nblocks << priv->geo.pageshift, buffer);
	if (nbytes > 0) {
		return nbytes >> priv->geo.pageshift;
	}

	return (int)nbytes;
}

/************************************************************************************
 * Name: jedec_bwrite
 ************************************************************************************/

static ssize_t jedec_bwrite(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks, FAR const uint8_t *buffer)
{
	FAR struct jedec_dev_s *priv = (FAR struct jedec_dev_s *)dev;
	size_t blocksleft = nblocks;
	size_t pagesize = 1 << priv->geo.pageshift;

	fvdbg("startblock: %08lx nblocks: %d\n", (long)startblock, (int)nblocks);

	/* Lock the SPI bus and write each page to FLASH */

	jedec_lock(priv->dev);
	while (blocksleft-- > 0) {
		jedec_pagewrite(priv, buffer, startblock);
		buffer += pagesize;
		startblock++;
	}

	jedec_unlock(priv->dev);
	return nblocks;
}

/************************************************************************************
 * Name: jedec_read
 ************************************************************************************/

static ssize_t jedec_read(FAR struct mtd_dev_s *dev, off_t offset, size_t nbytes, FAR uint8_t *buffer)
{
	FAR struct jedec_dev_s *priv = (FAR struct jedec_dev_s *)dev;

	fvdbg("offset: %08lx nbytes: %d\n", (long)offset, (int)nbytes);

	/* Lock the SPI bus NOW because the following call must be executed with
	 * the bus locked.
	 */

	jedec_lock(priv->dev);

	/* Wait for any preceding write to complete.  We could simplify things by
	 * perform this wait at the end of each write operation (rather than at
	 * the beginning of ALL operations), but have the wait first will slightly
	 * improve performance.
	 */

	jedec_waitwritecomplete(priv);

	/* Select this FLASH part */

	SPI_SELECT(priv->dev, SPIDEV_FLASH, true);

	/* Send "Read from Memory" instruction */

	(void)SPI_SEND(priv->dev, JEDEC_READ);

	/* Send the page offset high byte first. */

	(void)SPI_SEND(priv->dev, (offset >> 16) & 0xff);
	(void)SPI_SEND(priv->dev, (offset >> 8) & 0xff);
	(void)SPI_SEND(priv->dev, offset & 0xff);

	/* Then read all of the requested bytes */

	SPI_RECVBLOCK(priv->dev, buffer, nbytes);

	/* Deselect the FLASH and unlock the SPI bus */

	SPI_SELECT(priv->dev, SPIDEV_FLASH, false);
	jedec_unlock(priv->dev);

	fvdbg("return nbytes: %d\n", (int)nbytes);
	return nbytes;
}

/************************************************************************************
 * Name: jedec_write
 ************************************************************************************/

#ifdef CONFIG_MTD_BYTE_WRITE
static ssize_t jedec_write(FAR struct mtd_dev_s *dev, off_t offset, size_t nbytes, FAR const uint8_t *buffer)
{
	FAR struct jedec_dev_s *priv = (FAR struct jedec_dev_s *)dev;
	int startpage;
	int endpage;
	int count;
	int index;
	int pagesize;
	int bytestowrite;

	fvdbg("offset: %08lx nbytes: %d\n", (long)offset, (int)nbytes);

	/* We must test if the offset + count crosses one or more pages
	 * and perform individual writes.  The devices can only write in
	 * page increments.
	 */

	startpage = offset / (1 << priv->geo.pageshift);
	endpage = (offset + nbytes) / (1 << priv->geo.pageshift);

	if (startpage == endpage) {
		/* All bytes within one programmable page.  Just do the write. */

		jedec_bytewrite(priv, buffer, offset, nbytes);
	} else {
		/* Write the 1st partial-page */

		count = nbytes;
		pagesize = (1 << priv->geo.pageshift);
		bytestowrite = pagesize - (offset & (pagesize - 1));
		jedec_bytewrite(priv, buffer, offset, bytestowrite);

		/* Update offset and count */

		offset += bytestowrite;
		count -= bytestowrite;
		index = bytestowrite;

		/* Write full pages */

		while (count >= pagesize) {
			jedec_bytewrite(priv, &buffer[index], offset, pagesize);

			/* Update offset and count */

			offset += pagesize;
			count -= pagesize;
			index += pagesize;
		}

		/* Now write any partial page at the end */

		if (count > 0) {
			jedec_bytewrite(priv, &buffer[index], offset, count);
		}
	}

	return nbytes;
}
#endif							/* CONFIG_MTD_BYTE_WRITE */

/************************************************************************************
 * Name: jedec_ioctl
 ************************************************************************************/

static int jedec_ioctl(FAR struct mtd_dev_s *dev, int cmd, unsigned long arg)
{
	FAR struct jedec_dev_s *priv = (FAR struct jedec_dev_s *)dev;
	int ret = -EINVAL;			/* Assume good command with bad parameters */

	fvdbg("cmd: %d \n", cmd);

	switch (cmd) {
	case MTDIOC_GEOMETRY: {
		FAR struct mtd_geometry_s *geo = (FAR struct mtd_geometry_s *)((uintptr_t) arg);
		if (geo) {
			/* Populate the geometry structure with information need to know
			 * the capacity and how to access the device.
			 *
			 * NOTE: that the device is treated as though it where just an array
			 * of fixed size blocks.  That is most likely not true, but the client
			 * will expect the device logic to do whatever is necessary to make it
			 * appear so.
			 */

			geo->blocksize = (1 << priv->geo.pageshift);
			if (priv->geo.subsectorshift > 0) {
				geo->erasesize = (1 << priv->geo.subsectorshift);
				geo->neraseblocks = priv->geo.nsectors * (1 << (priv->geo.sectorshift - priv->geo.subsectorshift));
			} else {
				geo->erasesize = (1 << priv->geo.sectorshift);
				geo->neraseblocks = priv->geo.nsectors;
			}

			ret = OK;

			fvdbg("blocksize: %d erasesize: %d neraseblocks: %d\n", geo->blocksize, geo->erasesize, geo->neraseblocks);
		}
	}
	break;

	case MTDIOC_BULKERASE: {
		/* Erase the entire device */

		jedec_lock(priv->dev);
		ret = jedec_bulkerase(priv);
		jedec_waitwritecomplete(priv);
		jedec_unlock(priv->dev);
	}
	break;

	case MTDIOC_XIPBASE:
	default:
		ret = -ENOTTY;			/* Bad command */
		break;
	}

	fvdbg("return %d\n", ret);
	return ret;
}

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: jedec_initialize
 *
 * Description:
 *   Create an initialize MTD device instance.  MTD devices are not registered
 *   in the file system, but are created as instances that can be bound to
 *   other functions (such as a block or character driver front end).
 *
 ************************************************************************************/

FAR struct mtd_dev_s *jedec_initialize(FAR struct spi_dev_s *dev)
{
	FAR struct jedec_dev_s *priv;
	int ret;

	fvdbg("dev: %p\n", dev);

	/* Allocate a state structure (we allocate the structure instead of using
	 * a fixed, static allocation so that we can handle multiple FLASH devices.
	 * The current implementation would handle only one FLASH part per SPI
	 * device (only because of the SPIDEV_FLASH definition) and so would have
	 * to be extended to handle multiple FLASH parts on the same SPI bus.
	 */

	priv = (FAR struct jedec_dev_s *)kmm_zalloc(sizeof(struct jedec_dev_s));
	if (priv) {
		/* Initialize the allocated structure. (unsupported methods were
		 * nullified by kmm_zalloc).
		 */

		priv->mtd.erase = jedec_erase;
		priv->mtd.bread = jedec_bread;
		priv->mtd.bwrite = jedec_bwrite;
		priv->mtd.read = jedec_read;
#ifdef CONFIG_MTD_BYTE_WRITE
		priv->mtd.write = jedec_write;
#endif
		priv->mtd.ioctl = jedec_ioctl;
		priv->dev = dev;

		/* Deselect the FLASH */

		SPI_SELECT(dev, SPIDEV_FLASH, false);

		/* Identify the FLASH chip and get its capacity */

		ret = jedec_readid(priv);
		if (ret != OK) {
			/* Unrecognized! Discard all of that work we just did and return NULL */

			fdbg("Unrecognized\n");
			kmm_free(priv);
			priv = NULL;
		}
	}

	/* Return the implementation-specific state structure as the MTD device */

	fvdbg("Return %p\n", priv);
	return (FAR struct mtd_dev_s *)priv;
}
