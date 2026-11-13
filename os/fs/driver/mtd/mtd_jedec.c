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
#include <stdio.h>
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

#ifndef CONFIG_JEDEC_SPIMODE
#define CONFIG_JEDEC_SPIMODE	SPIDEV_MODE0
#endif

#ifndef CONFIG_JEDEC_SPI_FREQUENCY
#define CONFIG_JEDEC_SPI_FREQUENCY	(20000000)
#endif

/* Instructions */
/*      Command             	Value	   N Description             Addr Dummy Data   */
#define JEDEC_WREN           	0x06	/* 1 Write Enable              0   0     0     */
#define JEDEC_WRDI           	0x04	/* 1 Write Disable             0   0     0     */
#define JEDEC_RDID           	0x9f	/* 1 Read Identification       0   0     1-3   */
#define JEDEC_RDSR           	0x05	/* 1 Read Status Register      0   0     >=1   */
#define JEDEC_WRSR           	0x01	/* 1 Write Status Register     0   0     1     */
#define JEDEC_READ           	0x03	/* 1 Read Data Bytes           3   0     >=1   */
#define JEDEC_READ4BYTE      	0x13	/* 1 Read Data Bytes           4   0     >=1   */
#define JEDEC_FAST_READ      	0x0b	/* 1 Higher speed read         3   1     >=1   */
#define JEDEC_FAST_READ4BYTE 	0x0C	/* 1 Higher speed read         4   1     >=1   */
#define JEDEC_PP             	0x02	/* 1 Page Program              3   0     1-256 */
#define JEDEC_PP4BYTE        	0x12	/* 1 Page Program              4   0     1-256 */
#define JEDEC_SE             	0xd8	/* 1 Sector Erase              3   0     0     */
#define JEDEC_SE4BYTE        	0xdc	/* 1 Sector Erase              4   0     0     */
#define JEDEC_BE             	0xc7	/* 1 Bulk Erase                0   0     0     */
#define JEDEC_DP             	0xb9	/* 2 Deep power down           0   0     0     */
#define JEDEC_RES            	0xab	/* 2 Read Electronic Signature 0   3     >=1   */
#define JEDEC_SSE            	0x20	/* 3 Sub-Sector Erase          3   0     0     */
#define JEDEC_SSE4BYTE       	0x21	/* 3 Sub-Sector Erase          4   0     0     */
#define JEDEC_ADDR4BYTE      	0xb7	/* 3 4-byte Address mode       0   0     0     */
#define JEDEC_RDSFDP         	0x5a	/* - Read SFDP                 3   1     >=1   */

/* SFDP parameters */
#define JEDEC_SFDP_BFPT      	0xff00	/* Basic Flash Parameter Table ID */
#define JEDEC_SFDP_SECTORMAP 	0xff81	/* Sector Map Table ID */
#define JEDEC_SFDP_4BAIT     	0xff84	/* 4-Byte Address Instruction Table ID */
#define JEDEC_SFDP_MAX_HEADERS	32		/* Maximum number of parameter headers */
#define JEDEC_SFDP_ADDR_SPACE_SIZE 0x1000000u /* 3-byte SFDP address space size */

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

/* JEDEC identification and flash geometry obtained from SFDP. */
struct jedec_geometry_info_s {
	uint16_t manufacturer;		/* Manufacturer ID */
	uint16_t memory;			/* Memory Type */
	uint16_t capacity;			/* Memory Capacity ID */
	char *flash_size;			/* Flash Size string */
	char flash_size_buffer[16];	/* Storage for the flash size string */
	uint8_t sectorshift;		/* Size of erasable block */
	uint16_t nsectors;			/* Number of erasable blocks */
	uint8_t pageshift;			/* Size of programmable page */
	uint32_t npages;			/* Number of pages */
	uint8_t subsectorshift;		/* Size of subsector(Ex: 12 for 4K) */
};

typedef struct jedec_geometry_info_s jedec_geometry_t;

struct jedec_manufacturer_s {
	uint8_t id;
	const char *name;
};

/* Used only for display; manufacturer IDs may be shared by vendors. */
static const struct jedec_manufacturer_s g_jedec_manufacturers[] = {
	{0x0b, "XTX"},
	{0x20, "XMC / ST / Micron"},
	{0x9d, "ISSI"},
	{0xa1, "Fudan"},
	{0xef, "Winbond"},
};

/* This type represents the state of the MTD device.  The struct mtd_dev_s
 * must appear at the beginning of the definition so that you can freely
 * cast between pointers to struct mtd_dev_s and struct jedec_dev_s.
 */

struct jedec_dev_s {
	struct mtd_dev_s mtd;		/* MTD interface */
	FAR struct spi_dev_s *dev;	/* Saved SPI interface instance */
	jedec_geometry_t geo;
	uint8_t addr_len;            /* Address length(3 or 4bytes) */
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
static int jedec_is_erased(FAR struct jedec_dev_s *dev, off_t block);
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

	SPI_SETMODE(dev, CONFIG_JEDEC_SPIMODE);
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

/* SFDP has its own 24-bit address space, regardless of flash address mode. */
static void jedec_read_sfdp(struct jedec_dev_s *priv, uint32_t addr, uint8_t *data, size_t len)
{
	size_t i;

	jedec_lock(priv->dev);
	jedec_waitwritecomplete(priv);
	SPI_SELECT(priv->dev, SPIDEV_FLASH, true);
	(void)SPI_SEND(priv->dev, JEDEC_RDSFDP);
	(void)SPI_SEND(priv->dev, (addr >> 16) & 0xff);
	(void)SPI_SEND(priv->dev, (addr >> 8) & 0xff);
	(void)SPI_SEND(priv->dev, addr & 0xff);
	(void)SPI_SEND(priv->dev, JEDEC_DUMMY);
	for (i = 0; i < len; i++) {
		data[i] = SPI_SEND(priv->dev, JEDEC_DUMMY);
	}
	SPI_SELECT(priv->dev, SPIDEV_FLASH, false);
	jedec_unlock(priv->dev);
}

static uint32_t jedec_sfdp_u32(const uint8_t *p)
{
	return (uint32_t)p[0] | ((uint32_t)p[1] << 8) |
		((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

static uint32_t jedec_sfdp_pointer(const uint8_t *header)
{
	return (uint32_t)header[4] | ((uint32_t)header[5] << 8) |
		((uint32_t)header[6] << 16);
}

static const char *jedec_manufacturer_name(uint16_t id)
{
	unsigned int i;

	for (i = 0; i < sizeof(g_jedec_manufacturers) / sizeof(g_jedec_manufacturers[0]); i++) {
		if (g_jedec_manufacturers[i].id == id) {
			return g_jedec_manufacturers[i].name;
		}
	}

	return "Unknown";
}

/* Decode the BFPT density field and validate the flash size in bytes. */
static int jedec_sfdp_get_flash_size(uint32_t density, uint64_t *size)
{
	uint64_t bits;
	uint64_t bytes;

	if (density & 0x80000000u) {
		uint32_t exponent = density & 0x7fffffffu;
		if (exponent > 31) {
			return -ENOTSUP;
		}
		bits = (uint64_t)1 << exponent;
	} else {
		bits = (uint64_t)density + 1;
	}
	bytes = bits >> 3;
	if ((bits & 7) || bytes < (1u << 20) || bytes > (1u << 28) || (bytes & (bytes - 1))) {
		fdbg("unsupported density\n");
		return -ENOTSUP;
	}
	*size = bytes;
	return OK;
}

/* Select only configurations supported by the existing MTD operations. */
static int jedec_set_geometry(struct jedec_dev_s *priv, jedec_geometry_t *info, uint8_t *address_length)
{
	uint8_t header[8];
	uint8_t bfpt[44];
	uint8_t bait[8];
	uint32_t bfpt_addr = 0;
	uint32_t bait_addr = 0;
	uint32_t bfpt_dword1;
	uint32_t bfpt_dword2;
	uint32_t bfpt_dword8;
	uint32_t bfpt_dword9;
	uint32_t bfpt_dword11;
	uint32_t flags;
	uint64_t size;
	uint8_t bfpt_minor = 0;
	uint8_t bfpt_words = 0;
	uint8_t bait_words = 0;
	uint8_t sectorshift = 0;
	uint8_t subsectorshift = 0;
	uint8_t pageshift;
	uint8_t addrmode;
	uint8_t addr_len;
	bool bfpt_found = false;
	bool sector_map_found = false;
	int sector_type = -1;
	int subsector_type = -1;
	unsigned int count;
	unsigned int i;

	/* Read and check the SFDP header. */
	jedec_read_sfdp(priv, 0, header, sizeof(header));
	if (header[0] != 'S' || header[1] != 'F' || header[2] != 'D' || header[3] != 'P') {
		fdbg("SFDP signature is missing or invalid\n");
		return -ENOTSUP;
	}
	if (header[5] != 1) {
		fdbg("Unsupported SFDP major revision: %u\n", (unsigned int)header[5]);
		return -ENOTSUP;
	}

	/* Read and check the parameter headers. */
	count = (unsigned int)header[6] + 1;
	for (i = 0; i < count; i++) {
		uint16_t id;
		jedec_read_sfdp(priv, 8 + i * 8, header, sizeof(header));
		id = ((uint16_t)header[7] << 8) | header[0];
		if (id == JEDEC_SFDP_SECTORMAP) {
			sector_map_found = true;
		} else if (id == JEDEC_SFDP_BFPT && header[2] == 1 &&
		    (!bfpt_found || header[1] > bfpt_minor ||
		     (header[1] == bfpt_minor && header[3] > bfpt_words))) {
			bfpt_found = true;
			bfpt_minor = header[1];
			bfpt_words = header[3];
			bfpt_addr = jedec_sfdp_pointer(header);
		} else if (id == JEDEC_SFDP_4BAIT && header[2] == 1 && header[3] >= 2) {
			bait_words = header[3];
			bait_addr = jedec_sfdp_pointer(header);
		}
	}

	/* The existing driver assumes one uniform erase layout. */
	if (sector_map_found) {
		fdbg("Sector map is not supported by this driver\n");
		return -ENOTSUP;
	}
	/* Require a BFPT containing the page-size field in DWORD11. */
	if (!bfpt_found || bfpt_words < 11) {
		fdbg("BFPT missing or shorter than 11 DWORDs; cannot read page size\n");
		return -ENOTSUP;
	}
	if ((bfpt_addr & 3) || bfpt_addr > JEDEC_SFDP_ADDR_SPACE_SIZE - sizeof(bfpt)) {
		fdbg("Invalid BFPT address\n");
		return -ENOTSUP;
	}
	jedec_read_sfdp(priv, bfpt_addr, bfpt, sizeof(bfpt));
	/* Decode the BFPT DWORDs used to configure priv->geo and priv->addr_len. */
	bfpt_dword1 = jedec_sfdp_u32(bfpt);
	bfpt_dword2 = jedec_sfdp_u32(bfpt + 4);
	bfpt_dword8 = jedec_sfdp_u32(bfpt + 28);
	bfpt_dword9 = jedec_sfdp_u32(bfpt + 32);
	bfpt_dword11 = jedec_sfdp_u32(bfpt + 40);

	/* DWORD2 supplies the flash size used to calculate sector and page counts. */
	if (jedec_sfdp_get_flash_size(bfpt_dword2, &size) < 0) {
		return -ENOTSUP;
	}

	/* DWORD8/9 describe four erase types; find the sector and subsector sizes. */
	for (i = 0; i < 4; i++) {
		uint32_t erase_dword = i < 2 ? bfpt_dword8 : bfpt_dword9;
		unsigned int bit_offset = (i % 2) * 16;
		uint8_t shift = (erase_dword >> bit_offset) & 0xff;
		uint8_t opcode = (erase_dword >> (bit_offset + 8)) & 0xff;
		if (opcode == JEDEC_SE && shift && !sectorshift) {
			sectorshift = shift;
			sector_type = i;
		} else if (opcode == JEDEC_SSE && shift && !subsectorshift) {
			subsectorshift = shift;
			subsector_type = i;
		}
	}
	/* DWORD11 supplies the page size used for reads and page programming. */
	pageshift = (bfpt_dword11 >> 4) & 0xf;
	if (pageshift < 8 || pageshift > 12 || sectorshift < pageshift || sectorshift > 20 ||
	    (subsectorshift && (subsectorshift < pageshift || subsectorshift >= sectorshift)) ||
	    (size >> sectorshift) == 0 || (size >> sectorshift) > 0xffff) {
		fdbg("unsupported page or erase geometry\n");
		return -ENOTSUP;
	}

	/* DWORD1 and the flash size determine the address length used by priv. */
	addrmode = (bfpt_dword1 >> 17) & 3;
	if (addrmode == 3 || (size > (1u << 24) && addrmode == 0)) {
		fdbg("unsupported address mode\n");
		return -ENOTSUP;
	}
	addr_len = (size > (1u << 24) || addrmode == 2) ? 4 : 3;
	if (addr_len == 4) {
		/* The existing read/program/erase paths use dedicated 4-byte opcodes. */
		if (!bait_words || (bait_addr & 3) || bait_addr > JEDEC_SFDP_ADDR_SPACE_SIZE - sizeof(bait)) {
			fdbg("missing 4-byte instruction table\n");
			return -ENOTSUP;
		}
		jedec_read_sfdp(priv, bait_addr, bait, sizeof(bait));
		flags = jedec_sfdp_u32(bait);
#ifdef CONFIG_JEDEC_SLOWREAD
		if (!(flags & (1u << 0))) {
#else
		if (!(flags & (1u << 1))) {
#endif
			fdbg("4-byte read instruction unavailable\n");
			return -ENOTSUP;
		}
		if (!(flags & (1u << 6)) || !(flags & (1u << (9 + sector_type))) ||
		    bait[4 + sector_type] != JEDEC_SE4BYTE ||
		    (subsector_type >= 0 && (!(flags & (1u << (9 + subsector_type))) ||
		     bait[4 + subsector_type] != JEDEC_SSE4BYTE))) {
			fdbg("required 4-byte program or erase instruction unavailable\n");
			return -ENOTSUP;
		}
	}

	/* Apply the validated geometry and address length to priv. */
	info->sectorshift = sectorshift;
	info->nsectors = (uint16_t)(size >> sectorshift);
	info->pageshift = pageshift;
	info->npages = (uint32_t)(size >> pageshift);
	info->subsectorshift = subsectorshift;
	(void)snprintf(info->flash_size_buffer, sizeof(info->flash_size_buffer),
		       "%u MiB", (unsigned int)(size >> 20));
	info->flash_size = info->flash_size_buffer;
	*address_length = addr_len;
	fdbg("Total memory size: %lu KiB, sector: %lu %s, subsector: %lu %s, page: %lu bytes, %u-byte address\n",
	     (unsigned long)(size >> 10),
	     (unsigned long)((1u << sectorshift) >> (sectorshift >= 10 ? 10 : 0)),
	     sectorshift >= 10 ? "KiB" : "bytes",
	     subsectorshift ? (unsigned long)((1u << subsectorshift) >> (subsectorshift >= 10 ? 10 : 0)) : 0ul,
	     subsectorshift ? (subsectorshift >= 10 ? "KiB" : "bytes") : "(not supported)",
	     (unsigned long)(1u << pageshift), addr_len);
	return OK;
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
	jedec_waitwritecomplete(priv);

	SPI_SELECT(priv->dev, SPIDEV_FLASH, true);

	/* Send the "Read ID (RDID)" command and read the first three ID bytes */

	(void)SPI_SEND(priv->dev, JEDEC_RDID);
	manufacturer = SPI_SEND(priv->dev, JEDEC_DUMMY);
	memory = SPI_SEND(priv->dev, JEDEC_DUMMY);
	capacity = SPI_SEND(priv->dev, JEDEC_DUMMY);

	/* Deselect the FLASH and unlock the bus */

	SPI_SELECT(priv->dev, SPIDEV_FLASH, false);
	jedec_unlock(priv->dev);

	/* Use the SFDP geometry and addressing requirements for every device. */
	if (jedec_set_geometry(priv, &priv->geo, &priv->addr_len) != OK) {
		fdbg("JEDEC ID %02x %02x %02x: no usable SFDP geometry\n", manufacturer, memory, capacity);
		return -ENODEV;
	}

	fdbg("manufacturer: %s (0x%02x), memory type: 0x%02x, capacity ID: 0x%02x\n",
	     jedec_manufacturer_name(manufacturer), manufacturer, memory, capacity);

	/* The JEDEC ID is obtained from the Read ID command. */
	priv->geo.manufacturer = manufacturer;
	priv->geo.memory = memory;
	priv->geo.capacity = capacity;

	return OK;
}

/************************************************************************************
 * Name: jedec_unprotect
 ************************************************************************************/

static void jedec_unprotect(FAR struct jedec_dev_s *priv)
{
	/* Lock and configure the SPI bus */

	jedec_lock(priv->dev);

	/* Wait for any preceding write or erase operation to complete. */

	(void)jedec_waitwritecomplete(priv);

	/* Send "Write enable (WREN)" */

	jedec_writeenable(priv);

	/* Select this FLASH part */

	SPI_SELECT(priv->dev, SPIDEV_FLASH, true);

	/* Send "Write enable status (EWSR)" */

	SPI_SEND(priv->dev, JEDEC_WRSR);

	/* Following by the new status value */

	SPI_SEND(priv->dev, 0);
	SPI_SEND(priv->dev, 0);

	/* Deselect the FLASH and unlock the bus */

	SPI_SELECT(priv->dev, SPIDEV_FLASH, false);
	jedec_unlock(priv->dev);
}

/************************************************************************************
 * Name: jedec_set_4byte_addr_mode
 ************************************************************************************/

static void jedec_set_4byte_addr_mode(FAR struct jedec_dev_s *priv)
{
	jedec_lock(priv->dev);

	/* Wait for any preceding write or erase operation to complete. */

	(void)jedec_waitwritecomplete(priv);

	/* Send "Write enable (WREN)" */

	jedec_writeenable(priv);

	/* Select this FLASH part */

	SPI_SELECT(priv->dev, SPIDEV_FLASH, true);

	/* Send "set 4byte address (JEDEC_ADDR4BYTE)" command */

	(void)SPI_SEND(priv->dev, JEDEC_ADDR4BYTE);

	/* Deselect the FLASH */

	SPI_SELECT(priv->dev, SPIDEV_FLASH, false);

	jedec_unlock(priv->dev);
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
//			usleep(1000);
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
 * Name:  w25_wrdi
 ************************************************************************************/

static inline void jedec_wrdi(struct jedec_dev_s *priv)
{
	/* Select this FLASH part */

	SPI_SELECT(priv->dev, SPIDEV_FLASH, true);

	/* Send "Write Disable (WRDI)" command */

	(void)SPI_SEND(priv->dev, JEDEC_WRDI);

	/* Deselect the FLASH */

	SPI_SELECT(priv->dev, SPIDEV_FLASH, false);
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
	if (priv->addr_len == 4) {
		(void)SPI_SEND(priv->dev, (offset >> 24) & 0xff);
	}
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
	if (priv->addr_len == 4) {
		(void)SPI_SEND(priv->dev, JEDEC_PP4BYTE);		
		(void)SPI_SEND(priv->dev, (offset >> 24) & 0xff);
	} else {
		(void)SPI_SEND(priv->dev, JEDEC_PP);
	}

	/* Send the page offset high byte first. */

	(void)SPI_SEND(priv->dev, (offset >> 16) & 0xff);
	(void)SPI_SEND(priv->dev, (offset >> 8) & 0xff);
	(void)SPI_SEND(priv->dev, offset & 0xff);

	/* Then write the specified number of bytes */

	SPI_SNDBLOCK(priv->dev, buffer, 1 << priv->geo.pageshift);

	/* Deselect the FLASH: Chip Select high */

	SPI_SELECT(priv->dev, SPIDEV_FLASH, false);

	jedec_wrdi(priv);
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

	if (priv->addr_len == 4) {
		(void)SPI_SEND(priv->dev, JEDEC_PP4BYTE);		
		(void)SPI_SEND(priv->dev, (offset >> 24) & 0xff);
	} else {
		(void)SPI_SEND(priv->dev, JEDEC_PP);
	}

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
 * Name: jedec_is_erased
 ************************************************************************************/
static int jedec_is_erased(FAR struct jedec_dev_s *priv, off_t block)
{
	size_t offset;
	size_t count;
	uint32_t *buffer;
	uint32_t erasesize;
	jedec_geometry_t geo;
	int ret = 1;
	
	DEBUGASSERT(priv);
	geo = priv->geo;

	if (geo.subsectorshift > 0) {
		erasesize = (1 << geo.subsectorshift);
	} else {
		erasesize = (1 << geo.sectorshift);
	}

	buffer = kmm_malloc(erasesize * sizeof(uint8_t));
	if (!buffer) {
		set_errno(ENOMEM);
		return 0;
	}
	offset = block * erasesize;

	jedec_waitwritecomplete(priv);
	
	/* Select this FLASH part */

	SPI_SELECT(priv->dev, SPIDEV_FLASH, true);

	/* Send "Read from Memory" instruction */
#ifdef CONFIG_JEDEC_SLOWREAD
	if (priv->addr_len == 4) {
		(void)SPI_SEND(priv->dev, JEDEC_READ4BYTE);		
		(void)SPI_SEND(priv->dev, (offset >> 24) & 0xff);
	} else {
		(void)SPI_SEND(priv->dev, JEDEC_READ);
	}
#else
	if (priv->addr_len == 4) {
		(void)SPI_SEND(priv->dev, JEDEC_FAST_READ4BYTE); 	
		(void)SPI_SEND(priv->dev, (offset >> 24) & 0xff);
	} else {
		(void)SPI_SEND(priv->dev, JEDEC_FAST_READ);
	}
#endif
	/* Send the page offset high byte first. */

	(void)SPI_SEND(priv->dev, (offset >> 16) & 0xff);
	(void)SPI_SEND(priv->dev, (offset >> 8) & 0xff);
	(void)SPI_SEND(priv->dev, offset & 0xff);

	/* Send a dummy byte */
#ifndef CONFIG_JEDEC_SLOWREAD
		(void)SPI_SEND(priv->dev, JEDEC_DUMMY);
#endif

	/* Then read all of the requested bytes */
	
	SPI_RECVBLOCK(priv->dev, (FAR uint8_t *)buffer, erasesize);

	/* Deselect the FLASH and unlock the SPI bus */

	SPI_SELECT(priv->dev, SPIDEV_FLASH, false);

	/* Because of sudden power off, some of bits still can be '0', verify here */
	for (count = 0; count < (erasesize / 4); count++) {
		if (buffer[count] != 0xffffffff) {
			fdbg("written wrongly block : %d data : %08lx\n", block, buffer);
			ret = 0;
			break;
		}
	}
	kmm_free(buffer);
	return ret;
}

/************************************************************************************
 * Name: jedec_erase
 ************************************************************************************/

static int jedec_erase(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks)
{
	FAR struct jedec_dev_s *priv = (FAR struct jedec_dev_s *)dev;
	size_t blocksleft = nblocks;
	int ret = 0;
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

			/* If we are on a sector boundary and have at least a full sector
			 * of blocks left to erase, then we can do a full sector erase.
			 */
			if (startblock == sectorboundry && blocksleft >= blkper) {
				if (priv->addr_len == 4) {
					jedec_sectorerase(priv, startblock, JEDEC_SE4BYTE);
				} else {
					jedec_sectorerase(priv, startblock, JEDEC_SE);
				}
				startblock += blkper;
				blocksleft -= blkper;
				continue;
			} else {
				/* Just do a sub-sector erase */
				if (priv->addr_len == 4) {
					jedec_sectorerase(priv, startblock, JEDEC_SSE4BYTE);
				} else {
					jedec_sectorerase(priv, startblock, JEDEC_SSE);
				}
				
				/* TODO we verify erased state only for sub-sector erase for now, let's think about more good idea */
				ret = jedec_is_erased(priv, startblock);
				if (!ret) {
					fdbg("erase failed nblocks : %d blocksleft : %d\n", nblocks, blocksleft);
					jedec_unlock(priv->dev);
					return nblocks - blocksleft;
				}
				startblock++;
				blocksleft--;
				continue;
			}
		}

		/* Not using sub-sector erase.  Erase each full sector */

		if (priv->addr_len == 4) {
			jedec_sectorerase(priv, startblock, JEDEC_SE4BYTE);
		} else {
			jedec_sectorerase(priv, startblock, JEDEC_SE);
		}
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

	jedec_wrdi(priv);

	/* Select this FLASH part */

	SPI_SELECT(priv->dev, SPIDEV_FLASH, true);

	/* Send "Read from Memory" instruction */
#ifdef CONFIG_JEDEC_SLOWREAD
	if (priv->addr_len == 4) {
		(void)SPI_SEND(priv->dev, JEDEC_READ4BYTE); 	
		(void)SPI_SEND(priv->dev, (offset >> 24) & 0xff);
	} else {
		(void)SPI_SEND(priv->dev, JEDEC_READ);
	}
#else
	if (priv->addr_len == 4) {
		(void)SPI_SEND(priv->dev, JEDEC_FAST_READ4BYTE);	
		(void)SPI_SEND(priv->dev, (offset >> 24) & 0xff);
	} else {
		(void)SPI_SEND(priv->dev, JEDEC_FAST_READ);
	}
#endif

	/* Send the page offset high byte first. */
	(void)SPI_SEND(priv->dev, (offset >> 16) & 0xff);
	(void)SPI_SEND(priv->dev, (offset >> 8) & 0xff);
	(void)SPI_SEND(priv->dev, offset & 0xff);

	/* Send a dummy byte */
#ifndef CONFIG_JEDEC_SLOWREAD
	(void)SPI_SEND(priv->dev, JEDEC_DUMMY);
#endif

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

	jedec_lock(priv->dev);

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

	jedec_unlock(priv->dev);

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

		/* Identify the FLASH chip and get its geometry from SFDP. */

		ret = jedec_readid(priv);
		if (ret != OK) {
			/* Unrecognized! Discard all of that work we just did and return NULL */
			fdbg("Unrecognized\n");
			kmm_free(priv);
			return NULL;
		}
		jedec_unprotect(priv);
		/* For 32MBit of flash, we will use 4byte address mode */
		if (priv->addr_len == 4) {
			jedec_set_4byte_addr_mode(priv);
		}
	}

	/* Return the implementation-specific state structure as the MTD device */

	fvdbg("Return %p\n", priv);
	return (FAR struct mtd_dev_s *)priv;
}
