/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
#include <tinyara/fs/mtd.h>

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/
#define PAGE_SHIFT (8)
#define FLASH_FS_START CONFIG_ESP_FLASH_BASE
#define ESP32_NSECTORS (CONFIG_ESP32_FLASH_CAPACITY / CONFIG_ESP32_FLASH_BLOCK_SIZE)
#define ESP32_START_SECOTR (FLASH_FS_START / CONFIG_ESP32_FLASH_BLOCK_SIZE)

/************************************************************************************
 * Private Types
 ************************************************************************************/

/* This type represents the state of the MTD device.  The struct mtd_dev_s must
 * appear at the beginning of the definition so that you can freely cast between
 * pointers to struct mtd_dev_s and struct esp32_dev_s.
 */
struct esp32_dev_s {
	struct mtd_dev_s mtd;		/* MTD interface */
    int nsectors; /* number of erase sectors */
};

/************************************************************************************
 * Private Function Prototypes
 ************************************************************************************/

/* MTD driver methods */
static int esp32_erase(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks);
static ssize_t esp32_bread(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks, FAR uint8_t *buf);
static ssize_t esp32_bwrite(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks, FAR const uint8_t *buf);
static ssize_t esp32_read(FAR struct mtd_dev_s *dev, off_t offset, size_t nbytes, FAR uint8_t *buffer);
static int esp32_ioctl(FAR struct mtd_dev_s *dev, int cmd, unsigned long arg);
#if defined(CONFIG_MTD_BYTE_WRITE) && !defined(CONFIG_W25_READONLY)
static ssize_t esp32_write(FAR struct mtd_dev_s *dev, off_t offset, size_t nbytes, FAR const uint8_t *buffer);
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/
/************************************************************************************
 * Name: esp32_erase
 ************************************************************************************/
static ssize_t esp32_erase_page(size_t page)
{
    irqstate_t irqs;
    int32_t result = 0;
    result = result;
    if (page > (ESP32_START_SECOTR + ESP32_NSECTORS)) {
        printf("Invalid page number\n");
        return -EFAULT;
    }
    /* Disable IRQs while erasing sector */
    irqs = irqsave();

    /* do erase */
    result = spi_flash_erase_sector(page);

    /* Restore IRQs */
    irqrestore(irqs);
    return result == 0 ? CONFIG_ESP32_FLASH_BLOCK_SIZE : -EIO;
}

static int esp32_erase(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks)
{
    ssize_t result;
    startblock += ESP32_START_SECOTR;
    /* Erase the specified blocks and return status (OK or a negated errno) */
    while (nblocks > 0) {
        result = esp32_erase_page(startblock);
        if (result < 0) {
            return (int)result;
        }
        startblock++;
        nblocks--;
    }
    return OK;
}

static ssize_t esp32_flash_write(size_t addr, const void *buf, size_t count)
{
   // ets_printf("up_progmem_read write addr = %d\n", addr);
    int32_t result = 0;
    ssize_t ret = 0;
    irqstate_t irqs;

    /* Disable IRQs while erasing sector */
    irqs = irqsave();
    if ((addr & 0xff) != 0) {
        //! value should be 256 byte alignment
        ret = -EFAULT;
    } else if ((count & 0x3) != 0) {
        ret = -EINVAL;
    } else {
        char *adpt_buf = (char *)kmm_malloc(count);
        if (adpt_buf == NULL) {
            printf("[esp32] up_progmem_write: fail to alloc memory\n");
            ret = -EPERM;
        } else {
            memcpy(adpt_buf, buf, count);
            result = spi_flash_write(addr, adpt_buf, count);
            if (result != 0) {
                ret = -EIO;
            } else {
                ret = count;
            }
            kmm_free(adpt_buf);
            adpt_buf = NULL;
        }
    }

    /* Restore IRQs */
    irqrestore(irqs);
    if (ret < 0) {
        printf("[esp32] up_progmem_write Errno: %d, addr %d, buf %p, count %d\n", ret, addr, buf, count);
    }
    return ret;
}

ssize_t esp32_flash_read(size_t addr, void *buf, size_t count)
{

    //ets_printf("up_progmem_read read addr = %d\n", addr); 
    int32_t result = 0;
    ssize_t ret = 0;
    irqstate_t irqs;

    /* Disable IRQs while erasing sector */
    irqs = irqsave();
        
    if ((addr & 0x3) == 0) {
            //! if addr is 4 bytes aligned
            result = spi_flash_read(addr, buf, count);

            if (result != 0) {
                ret = -EIO;
            } else {
                ret = count;
            }
        } else {
            //! if addr is not 4 bytes aligned
            uint32_t offset = addr & 0x3;
            int8_t *aligned_read_buf = (int8_t *)kmm_malloc(count + offset);
            if (aligned_read_buf == NULL) {
                printf("[esp32] up_progmem_read: fail to alloc memory\n");
                ret = -EPERM;
            } else {
                result = spi_flash_read((addr & 0xfffffffc), (char *)aligned_read_buf, count + offset);
                if (result != 0) {
                    ret = -EIO;
                } else {
                    ret = count;
                }
                memcpy(buf, aligned_read_buf + offset, count);
                kmm_free(aligned_read_buf);
                aligned_read_buf = NULL;
            }
        }
    /* Restore IRQs */
    irqrestore(irqs);

    if (ret < 0) {
        printf("[esp32] up_progmem_read Errno: %d, addr %d, buf %p, count %d\n", ret, addr, buf, count);
    }
    return ret;
}

/************************************************************************************
 * Name: esp32_bread
 ************************************************************************************/
static ssize_t esp32_bread(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks, FAR uint8_t *buffer)
{
    ssize_t result;
    result = esp32_flash_read(CONFIG_ESP_FLASH_BASE + (startblock << PAGE_SHIFT), buffer, nblocks << PAGE_SHIFT);
    return result < 0 ? result : nblocks;
}

/************************************************************************************
 * Name: esp32_bwrite
 ************************************************************************************/
static ssize_t esp32_bwrite(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks, FAR const uint8_t *buffer)
{
    ssize_t result;

    /* Write the specified blocks from the provided user buffer and return status
     * (The positive, number of blocks actually written or a negated errno)
     */

    result = esp32_flash_write( CONFIG_ESP_FLASH_BASE + (startblock << PAGE_SHIFT), buffer, nblocks << PAGE_SHIFT);
    return result < 0 ? result : nblocks;

}

/************************************************************************************
 * Name: esp32_read
 ************************************************************************************/

static ssize_t esp32_read(FAR struct mtd_dev_s *dev, off_t offset, size_t nbytes, FAR uint8_t *buffer)
{
    ssize_t result;
    result = esp32_flash_read(CONFIG_ESP_FLASH_BASE + offset, buffer, nbytes);
    return result < 0 ? result : nbytes;
}

/************************************************************************************
 * Name: esp32_write
 ************************************************************************************/

#if defined(CONFIG_MTD_BYTE_WRITE) && !defined(CONFIG_W25_READONLY)
static ssize_t esp32_write(FAR struct mtd_dev_s *dev, off_t offset, size_t nbytes, FAR const uint8_t *buffer)
{
	FAR struct esp32_dev_s *priv = (FAR struct mtd_dev_s *)dev;
}
#endif							/* defined(CONFIG_MTD_BYTE_WRITE) && !defined(CONFIG_W25_READONLY) */

/************************************************************************************
 * Name: esp32_ioctl
 ************************************************************************************/

static int esp32_ioctl(FAR struct mtd_dev_s *dev, int cmd, unsigned long arg)
{
	int ret = -EINVAL;			/* Assume good command with bad parameters */
    FAR struct esp32_dev_s *priv = (FAR struct mtd_dev_s *)dev;
	fvdbg("cmd: %d \n", cmd);

	switch (cmd) {
	case MTDIOC_GEOMETRY: {
		FAR struct mtd_geometry_s *geo = (FAR struct mtd_geometry_s *)((uintptr_t) arg);
		if (geo) {
		    geo->blocksize = CONFIG_ESP32_FLASH_PAGE_SIZE;
			geo->erasesize = CONFIG_ESP32_FLASH_BLOCK_SIZE;
		    geo->neraseblocks = priv->nsectors;
			ret = OK;
			fvdbg("blocksize: %d erasesize: %d neraseblocks: %d\n", geo->blocksize, geo->erasesize, geo->neraseblocks);
		}
	}
	break;

	case MTDIOC_BULKERASE: {
		/* Erase the entire device */
        ret = esp32_erase(dev, 0, priv->nsectors);
	}
	break;

	case MTDIOC_XIPBASE:
	default:
		ret = -ENOTTY;			/* Bad command */
		break;
	}
    return ret;
}

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: esp32_initialize
 *
 * Description:
 *   Create an initialize MTD device instance.  MTD devices are not registered
 *   in the file system, but are created as instances that can be bound to
 *   other functions (such as a block or character driver front end).
 *
 ************************************************************************************/

FAR struct mtd_dev_s *up_flashinitialize(void)
{
	FAR struct esp32_dev_s *priv;
    priv = (FAR struct esp32_dev_s *)kmm_zalloc(sizeof(struct esp32_dev_s));
    if (priv) {
		/* Initialize the allocated structure (unsupported methods were
		 * nullified by kmm_zalloc).
		 */
		priv->mtd.erase = esp32_erase;
		priv->mtd.bread = esp32_bread;
		priv->mtd.bwrite = esp32_bwrite;
		priv->mtd.read = esp32_read;
		priv->mtd.ioctl = esp32_ioctl;
#if defined(CONFIG_MTD_BYTE_WRITE)
		priv->mtd.write = esp32_write;
#endif
        priv->nsectors = ESP32_NSECTORS;
		return (FAR struct mtd_dev_s *)priv;
    }
    return NULL;
}
