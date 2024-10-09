/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/kmalloc.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/spi/spi.h>
#include <tinyara/fs/mtd.h>

#include <tinyara/mtd/nand_raw.h>
#include <tinyara/mtd/nand_scheme.h>
#include <debug.h>

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/* SPI bus operation Mode 0 (0,0) and 3 (1,1) are supported. */

#ifndef CONFIG_W25N_SPIMODE
#define CONFIG_W25N_SPIMODE SPIDEV_MODE0
#endif

/* SPI clock frequencies of up to 104MHz are supported, also supports Dual and Quad SPI interface */

#ifndef CONFIG_W25N_SPIFREQUENCY
#define CONFIG_W25N_SPIFREQUENCY 48000000
#endif

/* W25N Instructions *********************************************************************/
/*      Command                    Value      Description                                */
/*                                                                                       */
#define W25N_RESET                  0xff	/* Reset device                          */
#define W25N_WREN                   0x06	/* Write enable                          */
#define W25N_WRDI                   0x04	/* Write Disable                         */
#define W25N_RDSR                   0x05	/* Read status register                  */
#define W25N_SR1_ADDR               0xA0	/* Status register 1 address             */
#define W25N_SR2_ADDR               0xB0	/* Status register 2 address             */
#define W25N_SR3_ADDR               0xC0	/* Status register 3 address             */
#define W25N_WRSR                   0x01	/* Write Status Register                 */
#define W25N_RDDATA                 0x03	/* Read data bytes                       */
#define W25N_READ_PAGE              0x13	/* Read data bytes(4bytes address)       */
#define W25N_FRD                    0x0b	/* Higher speed read                     */
#define W25N_FRD4BYTE               0x0c	/* Higher speed read(4bytes address)     */
#define W25N_FRDD                   0x3b	/* Fast read, dual output                */
#define W25N_FRDD4BYTE              0x3c	/* Fast read, dual output(4bytes address) */
#define W25N_PP                     0x02	/* Program page                          */
#define W25N_PE                     0x10	/* Program execute                       */
#define W25N_PP4BYTE                0x12	/* Program page(4bytes address)          */
#define W25N_BE                     0xd8	/* Block Erase (64KB)                    */
#define W25N_BE32                   0x52	/* Block Erase (32KB)                    */
#define W25N_CE                     0xc7	/* Chip erase                            */
#define W25N_PD                     0xb9	/* Power down                            */
#define W25N_PURDID                 0xab	/* Release PD, Device ID                 */
#define W25N_RDMFID                 0x90	/* Read Manufacturer / Device            */
#define W25N_JEDEC_ID               0x9f	/* JEDEC ID read                         */
#define W25N_ADDR4BYTE              0xb7	/* 4-byte Address mode                   */

/* W25N Registers ********************************************************************/
/* Read ID (RDID) register values */

/* JEDEC Read ID register values */

#define W25N_JEDEC_MANUFACTURER		0xef	/* SST manufacturer ID */

#define W25NQ_JEDEC_MEMORY_TYPE		0xAA	/* W25NQ memory type */

#define W25N_JEDEC_CAPACITY_2048MBIT	0x22	/* 65536x4096 = 2048Mbit memory capacity */

#define NBLOCKS_2048MBIT		512	/* 512 sectors x 65536 bytes/sector = 32Mb */

/* Status register bit definitions */

#define W25N_SR_BUSY                (1 << 0)	/* Bit 0: Write in progress */
#define W25N_SR_WEL                 (1 << 1)	/* Bit 1: Write enable latch bit */

/* Status register 1 layout */

/* |s7  |s6 |s5 |s4 |s3 |s2|s1 |s0  |  */
/* |SRP0|BP3|BP2|BP1|BP0|TB|WPE|SRP1|  */

/* TODO add block protection regions so we can use them easily, like protect first half of flash etc. */

#define W25N_SR_BP_SHIFT            (2)	/* Bits 2-5: Block protect bits */
#define W25N_SR_BP_MASK             (31 << W25N_SR_BP_SHIFT)	/* s2 to s6, 5 bits for block protection */
#define W25N_SR_BP_NONE             (0 << W25N_SR_BP_SHIFT)	/* Unprotected */

/* Status register 2 layout */

/* |s7  |s6  |s5  |s4  |s3 |s2  |s1  |s0  |  */
/* |OTPL|OTPE|SR1L|ECCE|BUF|ODS1|ODS0|HDIS|  */

#define W25N_SR_ECC_SHIFT           (4)
#define W25N_SR_ECC_MASK	    (1 << W25N_SR_ECC_SHIFT)	/* Bit 4 is for ECC enable */
#define W25N_SR_ECC_ENABLE	    (1 << W25N_SR_ECC_SHIFT)	/* Enable ECC */
#define W25N_SR_ECC_DISABLE         (0 << W25N_SR_ECC_SHIFT)	/* Disable ECC */

/* Status Register 3 layout */

/* |s7 |s6 |s5  |s4  |s3   |s2   |s1 |s0  |  */
/* |(R)|(R)|ECC1|ECC0|PFAIL|EFAIL|WEL|BUSY|  */

#define W25N_SR_EFAIL_SHIFT           (2)
#define W25N_SR_EFAIL		      (1 << W25N_SR_EFAIL_SHIFT)
#define W25N_SR_PFAIL_SHIFT           (3)
#define W25N_SR_PFAIL		      (1 << W25N_SR_PFAIL_SHIFT)
#define W25N_ECC_STATUS_SHIFT	      (4)
#define W25N_ECC_STATUS_MASK          (3 << W25N_ECC_STATUS_SHIFT)
#define W25N_ECC_STATUS(status)	      (status & ~(W25N_ECC_STATUS_MASK)) >> W25N_ECC_STATUS_SHIFT

#define W25N_DUMMY                  0x00

/* Chip Geometries ******************************************************************/
/* 0~11, column address in page, 12 ~ 17, page address in block, 18 ~ 28 block address */

#define W25N_BLOCK_SHIFT                18
#define W25N_PAGE_SHIFT                 12
#define W25N_PAGE_SIZE                  2048
#define W25N_PAGES_PER_BLOCK		64
#define W25N_NBLOCKS                    2048
#define W25N_ERASED_STATE               0xff	/* State of FLASH when erased */

/************************************************************************************
 * Private Types
 ************************************************************************************/

/* This type represents the state of the MTD device.  The struct mtd_dev_s must
 * appear at the beginning of the definition so that you can freely cast between
 * pointers to struct mtd_dev_s and struct w25n_dev_s.
 */

struct w25n_dev_s {
	struct nand_raw_s raw;		/* MTD NAND lowerhalf interface */
	FAR struct spi_dev_s *spi;	/* Saved SPI interface instance */
	uint16_t nblocks;			/* Number of erase sectors */
	uint8_t prev_instr;			/* Previous executed instruction */
};

/************************************************************************************
 * Private Function Prototypes
 ************************************************************************************/

/* Helpers */

static void w25n_lock(FAR struct spi_dev_s *spi);
static inline void w25n_unlock(FAR struct spi_dev_s *spi);
static inline int w25n_readid(FAR struct w25n_dev_s *priv);
#ifndef CONFIG_W25N_READONLY
static void w25n_unprotect(FAR struct w25n_dev_s *priv);
#endif
static void w25n_eccenable(FAR struct w25n_dev_s *priv);
static uint8_t w25n_waitoperationcomplete(FAR struct w25n_dev_s *priv);
static inline void w25n_wren(FAR struct w25n_dev_s *priv);
static inline void w25n_wrdi(FAR struct w25n_dev_s *priv);

/* MTD NAND raw methods */
static int w25n_blockerase(FAR struct nand_raw_s *raw, off_t block);
static int w25n_pageread(FAR struct nand_raw_s *raw, off_t block, unsigned int page, FAR void *data, FAR void *spare);
static int w25n_pagewrite(FAR struct nand_raw_s *raw, off_t block, unsigned int page, FAR const void *data, FAR const void *spare);

/************************************************************************************
 * Private Data
 ************************************************************************************/

/************************************************************************************
 * Private Functions
 ************************************************************************************/

/************************************************************************************
 * Name: w25n_lock
 ************************************************************************************/

static void w25n_lock(FAR struct spi_dev_s *spi)
{
	/* On SPI busses where there are multiple devices, it will be necessary to
	 * lock SPI to have exclusive access to the busses for a sequence of
	 * transfers.  The bus should be locked before the chip is selected.
	 *
	 * This is a blocking call and will not return until we have exclusiv access to
	 * the SPI buss.  We will retain that exclusive access until the bus is unlocked.
	 */

	(void)SPI_LOCK(spi, true);

	/* After locking the SPI bus, the we also need call the setfrequency, setbits, and
	 * setmode methods to make sure that the SPI is properly configured for the device.
	 * If the SPI buss is being shared, then it may have been left in an incompatible
	 * state.
	 */

	SPI_SETMODE(spi, CONFIG_W25N_SPIMODE);
	SPI_SETBITS(spi, 8);
	(void)SPI_SETFREQUENCY(spi, CONFIG_W25N_SPIFREQUENCY);
}

/************************************************************************************
 * Name: w25n_unlock
 ************************************************************************************/

static inline void w25n_unlock(FAR struct spi_dev_s *spi)
{
	(void)SPI_LOCK(spi, false);
}

/************************************************************************************
 * Name: w25n_readid
 ************************************************************************************/

static inline int w25n_readid(struct w25n_dev_s *priv)
{
	uint16_t manufacturer;
	uint16_t memory;
	uint16_t capacity;

	fvdbg("priv: %p\n", priv);

	/* Lock and configure the SPI bus */

	w25n_lock(priv->spi);

	/* Wait for any preceding write or erase operation to complete. */

	(void)w25n_waitoperationcomplete(priv);

	/* Reset the flash */

	SPI_SELECT(priv->spi, SPIDEV_FLASH, true);

	(void)SPI_SEND(priv->spi, W25N_RESET);

	SPI_SELECT(priv->spi, SPIDEV_FLASH, false);

	/* wait for 500us */
	up_mdelay(1);

	/* Select this FLASH part. */

	SPI_SELECT(priv->spi, SPIDEV_FLASH, true);

	/* Send the "Read ID (RDID)" command and read the first three ID bytes */
	(void)SPI_SEND(priv->spi, W25N_JEDEC_ID);

	/* Send a dummy byte before reading the device ID and other details */
	(void)SPI_SEND(priv->spi, W25N_DUMMY);

	manufacturer = SPI_SEND(priv->spi, W25N_DUMMY);
	memory = SPI_SEND(priv->spi, W25N_DUMMY);
	capacity = SPI_SEND(priv->spi, W25N_DUMMY);

	/* Deselect the FLASH and unlock the bus */

	SPI_SELECT(priv->spi, SPIDEV_FLASH, false);
	w25n_unlock(priv->spi);

	fvdbg("manufacturer: %02x memory: %02x capacity: %02x\n", manufacturer, memory, capacity);

	/* Check for a valid manufacturer and memory type */

	if (manufacturer == W25N_JEDEC_MANUFACTURER && memory == W25NQ_JEDEC_MEMORY_TYPE) {

		/* 256 M-Byte
		 *
		 * W25N02KV
		 */

		if (capacity == W25N_JEDEC_CAPACITY_2048MBIT) {

			fvdbg("Winbond Flash Model : W25N02KVxxIR/U found\n");

			priv->nblocks = NBLOCKS_2048MBIT;
			priv->raw.model.devid = W25N_JEDEC_MANUFACTURER;	/* Identifier for the device */
			priv->raw.model.pagesize = 2048;	/* Size of the data area of a page in bytes */
			priv->raw.model.sparesize = 128;	/* Size of the spare area of a page in bytes */
			priv->raw.model.devsize = 256;	/* Size of the device in MB */
			priv->raw.model.blocksize = 128;	/* Size of one block in kilobytes */
			priv->raw.model.scheme = &g_nand_sparescheme2048;
		} else {
			/* Nope.. we don't understand this capacity. */

			return -ENODEV;
		}

		return OK;
	}

	/* We don't understand the manufacturer or the memory type */

	return -ENODEV;
}

/************************************************************************************
 * Name: w25n_unprotect
 ************************************************************************************/

#ifndef CONFIG_W25N_READONLY
static void w25n_unprotect(FAR struct w25n_dev_s *priv)
{
	uint8_t status_register;

	/* Lock and configure the SPI bus */

	w25n_lock(priv->spi);

	/* Wait for any preceding write or erase operation to complete. */

	(void)w25n_waitoperationcomplete(priv);

	/*
	 * We want to disable block protection so that we can write to the flash,
	 * As per my understanding, its the only thing this function is required for
	 */

	/* Read the SR1 register */

	SPI_SELECT(priv->spi, SPIDEV_FLASH, true);

	SPI_SEND(priv->spi, W25N_RDSR);

	/* Send Status Register 1 address */

	SPI_SEND(priv->spi, W25N_SR1_ADDR);

	/* Get value of Register */

	status_register = SPI_SEND(priv->spi, W25N_DUMMY);

	/* Deselect the FLASH and unlock the bus */

	SPI_SELECT(priv->spi, SPIDEV_FLASH, false);

	/* Now that we have the value of the register, we need to disable block protection */

	fvdbg("Value of SR1 : %02X\n", status_register);

	status_register = ((status_register & ~(W25N_SR_BP_MASK)) | W25N_SR_BP_NONE);

	fvdbg("Updated value of SR1 with no block protection : %02X\n", status_register);

	SPI_SELECT(priv->spi, SPIDEV_FLASH, true);

	SPI_SEND(priv->spi, W25N_WRSR);

	/* Send Status Register 1 address */

	SPI_SEND(priv->spi, W25N_SR1_ADDR);

	/* Send value of Register */

	SPI_SEND(priv->spi, status_register);

	/* Deselect the FLASH and unlock the bus */

	SPI_SELECT(priv->spi, SPIDEV_FLASH, false);
	w25n_unlock(priv->spi);
}
#endif

/************************************************************************************
 * Name: w25n_eccenable
 ************************************************************************************/
static void w25n_eccenable(struct w25n_dev_s *priv)
{
	uint8_t status_register;

	/* Lock and configure the SPI bus */

	w25n_lock(priv->spi);

	/* Wait for any preceding write or erase operation to complete. */

	(void)w25n_waitoperationcomplete(priv);

	/* Read the SR1 register */

	SPI_SELECT(priv->spi, SPIDEV_FLASH, true);

	SPI_SEND(priv->spi, W25N_RDSR);

	/* Send Status Register 2 address */

	SPI_SEND(priv->spi, W25N_SR2_ADDR);

	/* Get value of Register */

	status_register = SPI_SEND(priv->spi, W25N_DUMMY);

	/* Deselect the FLASH and unlock the bus */

	SPI_SELECT(priv->spi, SPIDEV_FLASH, false);

	/* Now that we have the value of the register, we need to set ECC enable bit */

	fvdbg("Value of SR2 : %02X\n", status_register);

	status_register = ((status_register & ~(W25N_SR_ECC_MASK)) | W25N_SR_ECC_ENABLE);

	fvdbg("Updated value of SR2 with ECC : %02X\n", status_register);

	SPI_SELECT(priv->spi, SPIDEV_FLASH, true);

	SPI_SEND(priv->spi, W25N_WRSR);

	/* Send Status Register 2 address */

	SPI_SEND(priv->spi, W25N_SR2_ADDR);

	/* Send value of Register */

	SPI_SEND(priv->spi, status_register);

	/* Deselect the FLASH and unlock the bus */

	SPI_SELECT(priv->spi, SPIDEV_FLASH, false);
	w25n_unlock(priv->spi);
}

/************************************************************************************
 * Name: w25n_waitoperationcomplete
 ************************************************************************************/

static uint8_t w25n_waitoperationcomplete(struct w25n_dev_s *priv)
{
	/*
	 * For NOR flash, just checking the In progress bit is enough.
	 * However, in NAND flash, the operations might fail if a block
	 * turns bad (erase block, write page can fail). In such cases
	 * we need to test the block and add it to bad blocks if requried.
	 * For this particular reason, we need to check the status at the
	 * end of each operation
	 */
	uint8_t status;

	do {
		SPI_SELECT(priv->spi, SPIDEV_FLASH, true);

		/* Send "Read Status Register (RDSR)" command */

		(void)SPI_SEND(priv->spi, W25N_RDSR);

		/* Send "Status Register 3 address" */

		(void)SPI_SEND(priv->spi, W25N_SR3_ADDR);

		/* Send a dummy byte to generate the clock needed to shift out the status */

		status = SPI_SEND(priv->spi, W25N_DUMMY);

		SPI_SELECT(priv->spi, SPIDEV_FLASH, false);

	} while ((status & W25N_SR_BUSY) != 0);

	return status;
}

/************************************************************************************
 * Name:  w25n_wren
 ************************************************************************************/

static inline void w25n_wren(struct w25n_dev_s *priv)
{
	/* Select this FLASH part */

	SPI_SELECT(priv->spi, SPIDEV_FLASH, true);

	/* Send "Write Enable (WREN)" command */

	(void)SPI_SEND(priv->spi, W25N_WREN);

	/* Deselect the FLASH */

	SPI_SELECT(priv->spi, SPIDEV_FLASH, false);
}

/************************************************************************************
 * Name:  w25n_wrdi
 ************************************************************************************/

static inline void w25n_wrdi(struct w25n_dev_s *priv)
{
	/* Select this FLASH part */

	SPI_SELECT(priv->spi, SPIDEV_FLASH, true);

	/* Send "Write Disable (WRDI)" command */

	(void)SPI_SEND(priv->spi, W25N_WRDI);

	/* Deselect the FLASH */

	SPI_SELECT(priv->spi, SPIDEV_FLASH, false);
}

/************************************************************************************
 * Name:  w25n_blockerase
 ************************************************************************************/

static int w25n_blockerase(FAR struct nand_raw_s *raw, off_t block)
{
	struct w25n_dev_s *priv = (struct w25n_dev_s *)raw;

	uint32_t address = block << W25N_BLOCK_SHIFT;

	fvdbg("block: %08lx\n", (long)block);

	fvdbg("24bit address sent : %02X %02X %02X\n", (address >> (16 + W25N_PAGE_SHIFT)) & 0xff, (address >> (8 + W25N_PAGE_SHIFT)) & 0xff, (address >> (W25N_PAGE_SHIFT)) & 0xff);

	w25n_lock(priv->spi);

	/* Wait for any preceding write or erase operation to complete. */

	(void)w25n_waitoperationcomplete(priv);

	/* Send write enable instruction */

	w25n_wren(priv);

	/* Select this FLASH part */

	SPI_SELECT(priv->spi, SPIDEV_FLASH, true);

	/* Send the "Block Erase" instruction */
	(void)SPI_SEND(priv->spi, W25N_BE);
	priv->prev_instr = W25N_BE;

	(void)SPI_SEND(priv->spi, (address >> (16 + W25N_PAGE_SHIFT)) & 0xff);
	(void)SPI_SEND(priv->spi, (address >> (8 + W25N_PAGE_SHIFT)) & 0xff);
	(void)SPI_SEND(priv->spi, (address >> (W25N_PAGE_SHIFT)) & 0xff);

	/* Deselect the FLASH */

	SPI_SELECT(priv->spi, SPIDEV_FLASH, false);

	/* check SR3 here to catch any errors */
	uint8_t status = w25n_waitoperationcomplete(priv);

	fvdbg("status register after operation complete : %02X\n", status);

	w25n_unlock(priv->spi);

	if (status & W25N_SR_EFAIL) {
		lldbg("erase failed, status : %02X\n", status);
		return ERROR;
	}

	return OK;
}

/************************************************************************************
 * Name:  w25n_pageread
 ************************************************************************************/

static int w25n_pageread(FAR struct nand_raw_s *raw, off_t block, unsigned int page, FAR void *data, FAR void *spare)
{
	/* in the current requirement, we either read from start for data or from spare, so offset needs to be set accordingly */

	struct w25n_dev_s *priv = (struct w25n_dev_s *)raw;

	uint32_t row_address = (block << W25N_BLOCK_SHIFT) + (page << W25N_PAGE_SHIFT);
	uint32_t column_address = 0;

	if ((data == NULL) && (spare != NULL)) {
		// get model details and set spare offset accordingly??
		column_address += W25N_PAGE_SIZE;
	}

	fvdbg("block: %08lx page : %08lx\n", (long)block, (long)page);
	fvdbg("24bit row address sent : %02X %02X %02X\n", (row_address >> (16 + W25N_PAGE_SHIFT)) & 0xff, (row_address >> (8 + W25N_PAGE_SHIFT)) & 0xff, (row_address >> (W25N_PAGE_SHIFT)) & 0xff);
	fvdbg("16bit col address sent : %02X %02X\n", (column_address >> 8) & 0xff, column_address & 0xff);

	w25n_lock(priv->spi);

	/* Wait for any preceding write or erase operation to complete. */

	(void)w25n_waitoperationcomplete(priv);

	/* Select this FLASH part */

	SPI_SELECT(priv->spi, SPIDEV_FLASH, true);

	/* Send the "Page Data Read" instruction */
	(void)SPI_SEND(priv->spi, W25N_READ_PAGE);
	priv->prev_instr = W25N_READ_PAGE;

	/* send page address */
	(void)SPI_SEND(priv->spi, (row_address >> (16 + W25N_PAGE_SHIFT)) & 0xff);
	(void)SPI_SEND(priv->spi, (row_address >> (8 + W25N_PAGE_SHIFT)) & 0xff);
	(void)SPI_SEND(priv->spi, (row_address >> (W25N_PAGE_SHIFT)) & 0xff);

	SPI_SELECT(priv->spi, SPIDEV_FLASH, false);

	uint8_t status = w25n_waitoperationcomplete(priv);

	fvdbg("status register after operation complete : %02X\n", status);

	if (W25N_ECC_STATUS(status) > 1) {
		lldbg("ecc recovery failed, status : %02x\n", status);
		w25n_unlock(priv->spi);
		return ERROR;
	}

	/*Now we can read the data */
	SPI_SELECT(priv->spi, SPIDEV_FLASH, true);

	(void)SPI_SEND(priv->spi, W25N_RDDATA);

	/* Send the column address */
	(void)SPI_SEND(priv->spi, (column_address >> 8) & 0xff);
	(void)SPI_SEND(priv->spi, column_address & 0xff);

	/* Send dummy byte */
	(void)SPI_SEND(priv->spi, W25N_DUMMY);

	/* send data */
	if (data != NULL) {
		SPI_RECVBLOCK(priv->spi, data, W25N_PAGE_SIZE);
	}

	/* send spare */
	if (spare != NULL) {
		SPI_RECVBLOCK(priv->spi, spare, 64);
	}

	SPI_SELECT(priv->spi, SPIDEV_FLASH, false);

	w25n_unlock(priv->spi);

	/* check SR3 here to catch any errors */

	return OK;
}

/************************************************************************************
 * Name:  w25n_pagewrite
 ************************************************************************************/

static int w25n_pagewrite(FAR struct nand_raw_s *raw, off_t block, unsigned int page, FAR const void *data, FAR const void *spare)
{
	/* in the current requirement, we either write data or spare, so set the offset accordingly */
	struct w25n_dev_s *priv = (struct w25n_dev_s *)raw;

	uint32_t row_address = (block << W25N_BLOCK_SHIFT) + (page << W25N_PAGE_SHIFT);
	uint32_t column_address = 0;

	if ((data == NULL) && (spare != NULL)) {
		// get model details and set spare offset accordingly??
		column_address += W25N_PAGE_SIZE;
	}

	fvdbg("block: %08lx page : %08lx\n", (long)block, (long)page);
	fvdbg("24bit row address sent : %02X %02X %02X\n", (row_address >> (16 + W25N_PAGE_SHIFT)) & 0xff, (row_address >> (8 + W25N_PAGE_SHIFT)) & 0xff, (row_address >> (W25N_PAGE_SHIFT)) & 0xff);
	fvdbg("16bit col address sent : %02X %02X\n", (column_address >> 8) & 0xff, column_address & 0xff);

	w25n_lock(priv->spi);

	/* Wait for any preceding write or erase operation to complete. */

	(void)w25n_waitoperationcomplete(priv);

	/* Send write enable instruction */

	w25n_wren(priv);

	/* Select this FLASH part */

	SPI_SELECT(priv->spi, SPIDEV_FLASH, true);

	/* Send the "Page Program" instruction */
	(void)SPI_SEND(priv->spi, W25N_PP);
	priv->prev_instr = W25N_PP;

	/* Send the column address */
	(void)SPI_SEND(priv->spi, (column_address >> 8) & 0xff);
	(void)SPI_SEND(priv->spi, column_address & 0xff);

	/* send data */
	if (data != NULL) {
		SPI_SNDBLOCK(priv->spi, data, W25N_PAGE_SIZE);
	}

	/* send spare */
	if (spare != NULL) {
		SPI_SNDBLOCK(priv->spi, spare, 64);
	}

	SPI_SELECT(priv->spi, SPIDEV_FLASH, false);

	/* restart transfer */
	SPI_SELECT(priv->spi, SPIDEV_FLASH, true);

	/* start program execute */
	(void)SPI_SEND(priv->spi, W25N_PE);

	/* send page address */
	(void)SPI_SEND(priv->spi, (row_address >> (16 + W25N_PAGE_SHIFT)) & 0xff);
	(void)SPI_SEND(priv->spi, (row_address >> (8 + W25N_PAGE_SHIFT)) & 0xff);
	(void)SPI_SEND(priv->spi, (row_address >> (W25N_PAGE_SHIFT)) & 0xff);

	/* Deselect the FLASH */

	SPI_SELECT(priv->spi, SPIDEV_FLASH, false);

	uint8_t status = w25n_waitoperationcomplete(priv);

	fvdbg("status register after operation complete : %02X\n", status);

	w25n_unlock(priv->spi);

	if (status & W25N_SR_PFAIL) {
		lldbg("program failed, status : %02X\n", status);
		return ERROR;
	}

	return OK;
}

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: w25n_initialize
 *
 * Description:
 *   Create an initialize MTD device instance.  MTD devices are not registered
 *   in the file system, but are created as instances that can be bound to
 *   other functions (such as a block or character driver front end).
 *
 ************************************************************************************/

FAR struct mtd_dev_s *w25n_initialize(FAR struct spi_dev_s *spi)
{
	FAR struct w25n_dev_s *priv;
	int ret;

	fvdbg("spi: %p\n", spi);

	/* Allocate a state structure (we allocate the structure instead of using
	 * a fixed, static allocation so that we can handle multiple FLASH devices.
	 * The current implementation would handle only one FLASH part per SPI
	 * device (only because of the SPIDEV_FLASH definition) and so would have
	 * to be extended to handle multiple FLASH parts on the same SPI bus.
	 */

	priv = (FAR struct w25n_dev_s *)kmm_zalloc(sizeof(struct w25n_dev_s));
	if (priv) {
		/* Initialize the allocated structure (unsupported methods were
		 * nullified by kmm_zalloc).
		 */
		priv->raw.eraseblock = w25n_blockerase;
		priv->raw.rawread = w25n_pageread;
		priv->raw.rawwrite = w25n_pagewrite;

		priv->spi = spi;
		/* Deselect the FLASH */

		SPI_SELECT(spi, SPIDEV_FLASH, false);

		/* Identify the FLASH chip and get its capacity */

		ret = w25n_readid(priv);

		if (ret != OK) {
			/* Unrecognized! Discard all of that work we just did and return NULL */

			fdbg("ERROR: Unrecognized\n");
			kmm_free(priv);
			return NULL;
		}

		/* Make sure that the FLASH is unprotected so that we can write into it */

#ifndef CONFIG_W25N_READONLY
		fvdbg("Set Block Protection to none\n");
		w25n_unprotect(priv);
#endif

		fvdbg("Enable internal ECC for flash\n");
		w25n_eccenable(priv);

	}

	/* Return the implementation-specific state structure as the MTD device */

	fvdbg("Return %p\n", priv);
	return nand_initialize((struct nand_raw_s *)priv);
}
