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
 * board/sidk_s5jt200/src/s5jt200_eeprom.c
 *
 *   Copyright (C) 2014-2015 Gregory Nutt. All rights reserved.
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/spi/spi.h>
#include <debug.h>
#include <assert.h>

#include <tinyara/board.h>
#include <arch/board/board.h>

#include <tinyara/config.h>

#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <debug.h>

#include <tinyara/configdata.h>

#include "sidk_s5jt200.h"

#include <chip.h>

#include <errno.h>

#include <tinyara/kmalloc.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/*
 * To use this logic, you should provide a C file that does the following:
 *
 * - Defines SPI_SETSCK and SPI_CLRSCK to set and clear the SCK signal
 * - Defines SPI_SETMOSI and SPI_CLRMOSI to set and clear the MISO signal
 * - Defines SPI_GETMISO to sample the MISO state
 * - Defines SPI_PERBIT_NSEC which is the minimum time to transfer one bit.
 *   This determines the maximum frequency.
 * - Other configuration options:
 *   SPI_BITBAND_LOOPSPERMSEC - Delay loop calibration
 *   SPI_BITBANG_DISABLEMODE0 - Define to disable Mode 0 support
 *   SPI_BITBANG_DISABLEMODE1 - Define to disable Mode 1 support
 *   SPI_BITBANG_DISABLEMODE2 - Define to disable Mode 2 support
 *   SPI_BITBANG_DISABLEMODE3 - Define to disable Mode 3 support
 * - Provide implementations of spi_select(), spi_status(), and spi_cmddata().
 * - Then include this file
 * - Provide an initialization function that initializes the GPIO pins used
 *   in the bit bang interface and calls spi_create_bitbang().
 */

#define SPI_SETSCK  s5j_gpiowrite(gpio_clk, 1)
#define SPI_CLRSCK  s5j_gpiowrite(gpio_clk, 0)

#define SPI_SETMOSI  s5j_gpiowrite(gpio_mosi, 1)
#define SPI_CLRMOSI  s5j_gpiowrite(gpio_mosi, 0)

#define SPI_GETMISO  s5j_gpioread(gpio_miso)

#define SPI_PERBIT_NSEC 1000000

#define SPI_BITBAND_LOOPSPERMSEC 30000000

#define SPI_BITBANG_DISABLEMODE1 1
#define SPI_BITBANG_DISABLEMODE2 1
#define SPI_BITBANG_DISABLEMODE3 1

/* EEPROM commands */
#define EE_CMD_WRITE 0x5
#define EE_CMD_READ  0x6
#define EE_CMD_WRDIS 0x4
#define EE_CMD_WREN  0x4

/****************************************************************************
 * Private Types
 ****************************************************************************/
/* Supported device geometries */

typedef enum {
	M93C46_1B,
	M93C56_1B,
	M93C66_1B,
	M93C76_1B,
	M93C86_1B,
	M93C46_2B,
	M93C56_2B,
	M93C66_2B,
	M93C76_2B,
	M93C86_2B
} ee_dev_type;

/****************************************************************************
 * Private Data
 ****************************************************************************/
static const uint16_t gpio_clk  = (GPIO_OUTPUT|GPIO_FLOAT|GPIO_PORTP4|GPIO_PIN0);
static const uint16_t gpio_ss   = (GPIO_OUTPUT|GPIO_FLOAT|GPIO_PORTP4|GPIO_PIN1);
static const uint16_t gpio_miso = (GPIO_INPUT |GPIO_FLOAT|GPIO_PORTP4|GPIO_PIN2);
static const uint16_t gpio_mosi = (GPIO_OUTPUT|GPIO_FLOAT|GPIO_PORTP4|GPIO_PIN3);

/* Device geometry description, compact form (2 bytes per entry) */

struct ee_geom_s {
	uint8_t bytes:4;		/* Power of two of 128 bytes (0:128 1:256 2:512 etc) */
	uint8_t org:4;			/* Organization, bytes */
	uint8_t addrlen:4;		/* Number of bytes in command address field */
	uint8_t flags:4;		/* Special Features ??? */
};

static const struct ee_geom_s g_ee_devices[] = {
	{0, 0, 7, 0},		/* M93C46     128  1byte org */
	{1, 0, 9, 0},		/* M93C56     256  1byte org */
	{2, 0, 9, 0},		/* M93C66     512  1byte org */
	{3, 0, 11, 0},		/* M93C76    1024  1byte org */
	{4, 0, 11, 0},		/* M93C86    2048  1byte org */

	{0, 1, 6, 0},		/* M93C46     128  2bytes org */
	{1, 1, 8, 0},		/* M93C56     256  2bytes org */
	{2, 1, 8, 0},		/* M93C66     512  2bytes org */
	{3, 1, 10, 0},		/* M93C76    1024  2bytes org */
	{4, 1, 10, 0},		/* M93C86    2048  2bytes org */
};

/* Private data attached to the inode */

struct ee_dev_s {
	struct spi_dev_s *spi;	/* SPI device where the EEPROM is attached */
	uint32_t size;		/* in bytes, expanded from geometry */
	uint16_t org_size;	/* access block size, power of 2, expanded from geometry */
	uint16_t addrlen;	/* number of BITS in data addresses */
	sem_t sem;		/* file access serialization */
	uint8_t refs;		/* The number of times the device has been opened */
	uint8_t readonly;	/* Flags */
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

#include <tinyara/spi/spi_bitbang.c>

/****************************************************************************
* Name: spi_select
*
* Description:
*   This function is used to selectdesired Slave on SPI bus.
*
* Input Parameters:
*   priv - Private structure
*   devid - Device ID.
*   selected - Select/Deselect argument
*
*
* Returned Value:
*   None
*
****************************************************************************/
static void spi_select(FAR struct spi_bitbang_s *priv, enum spi_dev_e devid, bool selected)
{
	s5j_gpiowrite(gpio_ss, selected);
}

/****************************************************************************
* Name: spi_ststus
*
* Description:
*   This function is used to determine, if  WRITE/ERASE procedure is over.
*   Within hardcoded time interval (5uS), write procedure should be over.
*   If after 5uS Status is not BUSY, function returns 1.
*
* Input Parameters:
*   priv - Private structure
*   devid - Device ID.
*
*
* Returned Value:
*   0 - BUSY
*   1 - FREE
*
****************************************************************************/
static uint8_t spi_status(FAR struct spi_bitbang_s *priv, enum spi_dev_e devid)
{
	s5j_gpiowrite(gpio_ss, true);

	usleep(5000);
	if (s5j_gpioread(gpio_miso) != 0) {
		goto OK_Exit;
	}

	usleep(5000);
	if (s5j_gpioread(gpio_miso) != 0) {
		goto OK_Exit;
	}

	s5j_gpiowrite(gpio_ss, false);
	return 0;

OK_Exit:
	s5j_gpiowrite(gpio_ss, false);
	return 1;
}

/****************************************************************************
* Name: spi_cmddata
*
* Description:
*   This is placeholder function. Empty.
*
* Input Parameters:
*   priv - Private structure
*   devid - Device ID.
*   bool - command
*
*
* Returned Value:
*   0
*
****************************************************************************/
static int spi_cmddata(FAR struct spi_bitbang_s *priv, enum spi_dev_e devid, bool cmd)
{
	return 0;
}

/****************************************************************************
 * Private Stuff
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int ee_fopen(FAR struct file *filep);
static int ee_fclose(FAR struct file *filep);
static off_t ee_fseek(FAR struct file *filep, off_t offset, int whence);
static ssize_t ee_fread(FAR struct file *filep, FAR char *buffer, size_t buflen);
static ssize_t ee_fwrite(FAR struct file *filep, FAR const char *buffer, size_t buflen);
static int ee_fioctl(FAR struct file *filep, int cmd, unsigned long arg);

/* Driver operations */

static const struct file_operations ee_fops = {
	ee_fopen,	/* open */
	ee_fclose,	/* close */
	ee_fread,	/* read */
	ee_fwrite,	/* write */
	ee_fseek,	/* seek */
	ee_fioctl,	/* ioctl */
#ifndef CONFIG_DISABLE_POLL
	0,		/* poll */
#endif
};

/****************************************************************************
 * Name: ee_lock
 ****************************************************************************/

static void ee_lock(FAR struct spi_dev_s *dev)
{
	/*
	 * On SPI buses where there are multiple devices, it will be necessary
	 * to lock SPI to have exclusive access to the buses for a sequence of
	 * transfers.  The bus should be locked before the chip is selected.
	 *
	 * This is a blocking call and will not return until we have exclusive
	 * access to the SPI bus.  We will retain that exclusive access until
	 * the bus is unlocked.
	 */

	(void)SPI_LOCK(dev, true);

	/* After locking the SPI bus, the we also need call the setfrequency,
	 * setbits, and setmode methods to make sure that the SPI is properly
	 * configured for the device.  If the SPI bus is being shared, then
	 * it may have been left in an incompatible state.
	 */
}

/****************************************************************************
 * Name: ee25xx_unlock
 ****************************************************************************/

static inline void ee_unlock(FAR struct spi_dev_s *dev)
{
	(void)SPI_LOCK(dev, false);
}

/****************************************************************************
 * Name: ee_writeenable
 *
 * Description: Enable or disable write operations.
 * This is required before any write, since a lot of operations automatically
 * disables the write latch.
 *
 ****************************************************************************/

static void ee_writeenable(FAR struct ee_dev_s *eedev, int enable)
{
	unsigned int read_cmd;

	SPI_SELECT(eedev->spi, SPIDEV_EEPROM, true);
	SPI_SETBITS(eedev->spi, 11);
	read_cmd = (EE_CMD_WREN << 8) | 0xC0;
	SPI_SNDBLOCK(eedev->spi, &read_cmd, 1);
	SPI_SELECT(eedev->spi, SPIDEV_EEPROM, false);
}

/****************************************************************************
 * Name: ee_write
 *
 * Description: Write data to the EEPROM.
 *
 ****************************************************************************/

static ssize_t ee_write(FAR struct ee_dev_s *eedev, uint32_t devaddr, FAR const char *data, size_t len)
{
	unsigned int write_cmd;
	ssize_t rc = 0;

	uint16_t org_size = 1 << eedev->org_size; /* access block size, in bytes, expanded from geometry */
	uint16_t addrlen = eedev->addrlen;	  /* number of BITS in data addresses */
	uint16_t data_bits = 8 * org_size;
	uint16_t cmd_bits = 3 + addrlen;

	devaddr >>= eedev->org_size;

	while (len > (org_size - 1)) {
		SPI_SELECT(eedev->spi, SPIDEV_EEPROM, true);

		SPI_SETBITS(eedev->spi, cmd_bits);
		write_cmd = (EE_CMD_WRITE << addrlen) | devaddr;
		SPI_SNDBLOCK(eedev->spi, &write_cmd, 1);

		SPI_SETBITS(eedev->spi, data_bits);
		SPI_SNDBLOCK(eedev->spi, data, 1);

		SPI_SELECT(eedev->spi, SPIDEV_EEPROM, false);

		if (SPI_STATUS(eedev->spi, SPIDEV_EEPROM) == 0) {
			printf("\nEEPROM_Write Failed\n");
		}

		len -= org_size;
		data += org_size;
		rc += org_size;
		devaddr++;
	}

	return rc;
}

/****************************************************************************
 * Name: ee_read
 *
 * Description: Read data from EEPROM.
 *
 ****************************************************************************/
static ssize_t ee_read(FAR struct ee_dev_s *eedev, uint32_t devaddr, FAR char *data, size_t len)
{
	unsigned int read_cmd;
	size_t rc = 0;

	uint16_t org_size = 1 << eedev->org_size; /* access block size, in bytes, expanded from geometry */
	uint16_t addrlen = eedev->addrlen;	  /* number of BITS in data addresses */
	uint16_t data_bits = 8 * org_size;
	uint16_t cmd_bits = 3 + addrlen;

	devaddr >>= eedev->org_size;
	while (len > (org_size - 1)) {
		SPI_SELECT(eedev->spi, SPIDEV_EEPROM, true);

		SPI_SETBITS(eedev->spi, cmd_bits);
		read_cmd = (EE_CMD_READ << addrlen) | devaddr;
		SPI_SNDBLOCK(eedev->spi, &read_cmd, 1);

		SPI_SETBITS(eedev->spi, data_bits);
		SPI_RECVBLOCK(eedev->spi, data, 1);
		SPI_SELECT(eedev->spi, SPIDEV_EEPROM, false);

		len -= org_size;
		data += org_size;
		rc += org_size;
		devaddr++;
	}

	return rc;
}

/****************************************************************************
 * Name: ee_semtake
 *
 * Acquire a resource to access the device.
 * The purpose of the semaphore is to block tasks that try to access the
 * EEPROM while another task is actively using it.
 *
 ****************************************************************************/

static void ee_semtake(FAR struct ee_dev_s *eedev)
{
	/* Take the semaphore (perhaps waiting) */

	while (sem_wait(&eedev->sem) != 0) {
		/*
		 * The only case that an error should occur here is if
		 * the wait was awakened by a signal.
		 */

		ASSERT(errno == EINTR);
	}
}

/****************************************************************************
 * Name: ee25xx_semgive
 *
 * Release a resource to access the device.
 *
 ****************************************************************************/

static inline void ee_semgive(FAR struct ee_dev_s *eedev)
{
	sem_post(&eedev->sem);
}

/****************************************************************************
 * Driver Functions
 ****************************************************************************/

/****************************************************************************
 * Name: ee_open
 *
 * Description: Open the block device
 *
 ****************************************************************************/

static int ee_fopen(FAR struct file *filep)
{
	FAR struct inode *inode = filep->f_inode;
	FAR struct ee_dev_s *eedev;
	int ret = OK;

	DEBUGASSERT(inode && inode->i_private);
	eedev = (FAR struct ee_dev_s *)inode->i_private;
	ee_semtake(eedev);

	/* Increment the reference count */

	if ((eedev->refs + 1) == 0) {
		ret = -EMFILE;
	} else {
		eedev->refs += 1;
	}

	ee_semgive(eedev);
	return ret;
}

/****************************************************************************
 * Name: ee_close
 *
 * Description: Close the block device
 *
 ****************************************************************************/

static int ee_fclose(FAR struct file *filep)
{
	FAR struct inode *inode = filep->f_inode;
	FAR struct ee_dev_s *eedev;
	int ret = OK;

	DEBUGASSERT(inode && inode->i_private);
	eedev = (FAR struct ee_dev_s *)inode->i_private;
	ee_semtake(eedev);

	/*
	 * Decrement the reference count. I want the entire close operation
	 * to be atomic wrt other driver operations.
	 */

	if (eedev->refs == 0) {
		ret = -EIO;
	} else {
		eedev->refs -= 1;
	}

	ee_semgive(eedev);
	return ret;
}

/****************************************************************************
 * Name: ee_seek
 *
 * Remark: Copied from bchlib
 *
 ****************************************************************************/

static off_t ee_fseek(FAR struct file *filep, off_t offset, int whence)
{
	FAR struct ee_dev_s *eedev;
	off_t newpos;
	int ret;
	FAR struct inode *inode = filep->f_inode;

	DEBUGASSERT(inode && inode->i_private);
	eedev = (FAR struct ee_dev_s *)inode->i_private;
	ee_semtake(eedev);

	/* Determine the new, requested file position */

	switch (whence) {
	case SEEK_CUR:
		newpos = filep->f_pos + offset;
		break;

	case SEEK_SET:
		newpos = offset;
		break;

	case SEEK_END:
		newpos = eedev->size + offset;
		break;

	default:
		/* Return EINVAL if the whence argument is invalid */

		ee_semgive(eedev);
		return -EINVAL;
	}

	/*
	 * Opengroup.org:
	 *
	 *  "The lseek() function shall allow the file offset to be set
	 *  beyond the end of the existing data in the file. If data is later
	 *  written at this point, subsequent reads of data in the gap shall
	 *  return bytes with the value 0 until data is actually written into
	 *  the gap."
	 *
	 * We can conform to the first part, but not the second. But return
	 * EINVAL if
	 *
	 *  "...the resulting file offset would be negative for a regular
	 *  file, block special file, or directory."
	 */

	if (newpos >= 0) {
		filep->f_pos = newpos;
		ret = newpos;
	} else {
		ret = -EINVAL;
	}

	ee_semgive(eedev);
	return ret;
}

/****************************************************************************
 * Name: ee_read
 ****************************************************************************/

static ssize_t ee_fread(FAR struct file *filep, FAR char *buffer, size_t len)
{
	FAR struct ee_dev_s *eedev;
	FAR struct inode *inode = filep->f_inode;
	size_t rc_len = 0;
	size_t rd_len;
	uint16_t buff_2;

	DEBUGASSERT(inode && inode->i_private);
	eedev = (FAR struct ee_dev_s *)inode->i_private;

	if (filep->f_pos >= eedev->size) {
		return EOF;
	}

	ee_semtake(eedev);

	/* trim len if read would go beyond end of device */

	if ((filep->f_pos + len) > eedev->size) {
		len = eedev->size - filep->f_pos;
	}

	ee_lock(eedev->spi);
	if (eedev->org_size == 1) {
		if (filep->f_pos & 1) {
			rd_len = ee_read(eedev, filep->f_pos & ~1, (char *)&buff_2, 2);
			buffer[0] = buff_2 >> 8;

			buffer += 1;
			filep->f_pos += 1;
			rc_len += 1;
			len = len - 1;
		}

		if (len > 1) {
			rd_len = ee_read(eedev, filep->f_pos, buffer, len);

			buffer += rd_len;
			filep->f_pos += rd_len;
			rc_len += rd_len;
			len = len - rd_len;
		}

		if (len == 1) {
			rd_len = ee_read(eedev, filep->f_pos, (char *)&buff_2, 2);
			buffer[0] = (char)buff_2;

			buffer += 1;
			filep->f_pos += 1;
			rc_len += 1;
		}
	} else {
		rc_len = ee_read(eedev, filep->f_pos, buffer, len);

		filep->f_pos += rc_len;
	}

	ee_unlock(eedev->spi);

	/* Update the file position */
	ee_semgive(eedev);
	return rc_len;
}

/****************************************************************************
 * Name: ee_write
 ****************************************************************************/

static ssize_t ee_fwrite(FAR struct file *filep, FAR const char *buffer, size_t len)
{
	FAR struct ee_dev_s *eedev;
	FAR struct inode *inode = filep->f_inode;
	int ret = -EACCES;
	uint16_t buff_2;
	size_t wr_len;

	DEBUGASSERT(inode && inode->i_private);
	eedev = (FAR struct ee_dev_s *)inode->i_private;

	if (eedev->readonly) {
		return ret;
	}

	/* Forbid writes past the end of the device */

	if (filep->f_pos >= eedev->size) {
		return -EFBIG;
	}

	/* Clamp len to avoid crossing the end of the memory */

	if ((len + filep->f_pos) > eedev->size) {
		len = eedev->size - filep->f_pos;
	}

	/*
	 * From this point no failure cannot be detected anymore.
	 * The user should verify the write by rereading memory.
	 */

	ret = len; /* save number of bytes written */

	ee_semtake(eedev);

	/* First, write some page-unaligned data */
	ee_lock(eedev->spi);

	ee_writeenable(eedev, true);

	if (eedev->org_size == 1) {
		if (filep->f_pos & 1) {
			ee_read(eedev, filep->f_pos & ~1, (char *)&buff_2, 2);
			buff_2 = (buff_2 & 0x00FF) | ((((uint16_t)*buffer) << 8) & 0xFF00);
			ee_write(eedev, filep->f_pos & ~1, (char *)&buff_2, 2);

			len--;
			buffer++;
			filep->f_pos++;
		}

		if (len > 1) {
			wr_len = ee_write(eedev, filep->f_pos, buffer, len);

			len -= wr_len;
			buffer += wr_len;
			filep->f_pos += wr_len;
		}

		if (len == 1) {
			ee_read(eedev, filep->f_pos, (char *)&buff_2, 2);
			buff_2 = (buff_2 & 0xFF00) | (((uint16_t)*buffer) & 0x00FF);
			ee_write(eedev, filep->f_pos, (char *)&buff_2, 2);

			len--;
			buffer++;
			filep->f_pos++;
		}
	} else {
		wr_len = ee_write(eedev, filep->f_pos, buffer, len);
		filep->f_pos += wr_len;
	}
	ee_writeenable(eedev, false);
	ee_unlock(eedev->spi);

	/* Then, write remaining bytes at page-aligned addresses */

	ee_semgive(eedev);

	return ret;
}

/****************************************************************************
 * Name: ee25xx_ioctl
 *
 * Description: TODO: Erase a sector/page/device or read device ID.
 * This is completely optional and only applies to bigger devices.
 *
 ****************************************************************************/

static int ee_fioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	FAR struct ee_dev_s *eedev;
	FAR struct inode *inode = filep->f_inode;
	int ret = 0;

	DEBUGASSERT(inode && inode->i_private);
	eedev = (FAR struct ee_dev_s *)inode->i_private;

	switch (cmd) {
	default:
		(void)eedev;
		ret = -EINVAL;
	}

	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

static struct spi_dev_s *eeprom_dev;

/****************************************************************************
 * Name: ee_initialize
 *
 * Description: Bind a EEPROM driver to an SPI bus. The user MUST provide
 * a description of the device geometry, since it is not possible to read
 * this information from the device (contrary to the SPI flash devices).
 *
 ****************************************************************************/
int ee_initialize(FAR struct spi_dev_s *dev, FAR char *devname, ee_dev_type devtype, int readonly)
{
	FAR struct ee_dev_s *eedev;

	/* Check device type early */

	eedev = (struct ee_dev_s *)kmm_zalloc(sizeof(struct ee_dev_s));

	if (!eedev) {
		return -ENOMEM;
	}

	sem_init(&eedev->sem, 0, 1);

	eedev->spi = dev;
	eedev->size = 128 << g_ee_devices[devtype].bytes;
	eedev->org_size = g_ee_devices[devtype].org;
	eedev->addrlen = g_ee_devices[devtype].addrlen;
	eedev->readonly = ! !readonly;

	return register_driver(devname, &ee_fops, 0666, eedev);
}

void sidk_s5jt200_eeprom_init(void)
{
	s5j_configgpio(GPIO_OUTPUT | GPIO_FLOAT | GPIO_PORTP4 | GPIO_PIN0); /* CLK  */
	s5j_configgpio(GPIO_OUTPUT | GPIO_FLOAT | GPIO_PORTP4 | GPIO_PIN1); /* CS   */
	s5j_configgpio(GPIO_INPUT  | GPIO_FLOAT | GPIO_PORTP4 | GPIO_PIN2); /* MISO */
	s5j_configgpio(GPIO_OUTPUT | GPIO_FLOAT | GPIO_PORTP4 | GPIO_PIN3); /* MOSI */

	eeprom_dev = spi_create_bitbang(&g_spiops);
	ee_initialize(eeprom_dev, "/dev/eeprom", M93C66_2B, false);
}
