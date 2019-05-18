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


/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/spi/spi.h>

#include <semaphore.h>

#define SPI_ENTRY spidbg("-->%s\t%s:%d\n", __FUNCTION__, __FILE__, __LINE__)
/******************************************************************************
 * Private Variables
 *****************************************************************************/
static int spi_uio_open(FAR struct file *filep);
static int spi_uio_close(FAR struct file *filep);
static int spi_uioctrl(FAR struct file *filep, int cmd, unsigned long arg);
static ssize_t spi_uioread(FAR struct file *filep, FAR char *buffer, size_t buflen);
static ssize_t spi_uiowrite(FAR struct file *filep, FAR const char *buffer, size_t buflen);

static const struct file_operations g_spiops = {
	spi_uio_open,				/* open */
	spi_uio_close,				/* close */
	spi_uioread,				/* read */
	spi_uiowrite,				/* write */
	NULL,						/* seek */
	spi_uioctrl,				/* ioctl */
#ifndef CONFIG_DISABLE_POLL
	NULL						/* poll */
#endif
};

/****************************************************************************
 * Public Functions
 ****************************************************************************/


/****************************************************************************
 * Name: spidrvr_open
 ****************************************************************************/

static int spi_uio_open(FAR struct file *filep)
{
	SPI_ENTRY;
	FAR struct inode *inode;
	FAR struct spi_dev_s *dev;

	/* Get our private data structure */
	DEBUGASSERT(filep != NULL && filep->f_inode != NULL);
	inode = filep->f_inode;

	dev = (FAR struct spi_dev_s *)inode->i_private;
	DEBUGASSERT(dev);

	return 0;
}

/****************************************************************************
 * Name: spidrvr_close
 ****************************************************************************/
static int spi_uio_close(FAR struct file *filep)
{
	SPI_ENTRY;
	FAR struct inode *inode;
	FAR struct spi_dev_s *dev;

	/* Get our private data structure */

	DEBUGASSERT(filep != NULL && filep->f_inode != NULL);
	inode = filep->f_inode;

	dev = (FAR struct spi_dev_s *)inode->i_private;
	DEBUGASSERT(dev);

	return 0;
}


/****************************************************************************
 * Name: spi_uioread
 *
 * Description:
 *   Read SPI Data. This function is automatically called when you're doing
 *   read() API on userspace.
 *
 ****************************************************************************/
static ssize_t spi_uioread(FAR struct file *filep, FAR char *buffer, size_t buflen)
{
	SPI_ENTRY;
	FAR struct inode *inode = filep->f_inode;
	FAR struct spi_dev_s *dev = inode->i_private;

	SPI_LOCK(dev, true);
	SPI_SETMODE(dev, dev->config.mode);
	SPI_SETBITS(dev, dev->config.bpw);
	SPI_SETFREQUENCY(dev, dev->config.freq);
	SPI_SELECT(dev, dev->config.cs, true);
	SPI_RECVBLOCK(dev, buffer, buflen); // return type is void
	SPI_SELECT(dev, dev->config.cs, false);
	SPI_LOCK(dev, false);

	return 0;
}

/****************************************************************************
 * Name: spi_uiowrite
 *
 * Description:
 *   Write SPI Data. This function is automatically called when you're doing
 *   write() API on userspace.
 *
 ****************************************************************************/
static ssize_t spi_uiowrite(FAR struct file *filep, FAR const char *buffer, size_t buflen)
{
	SPI_ENTRY;
	FAR struct inode *inode = filep->f_inode;
	FAR struct spi_dev_s *dev = inode->i_private;

	SPI_LOCK(dev, true);
	SPI_SETMODE(dev, dev->config.mode);
	SPI_SETBITS(dev, dev->config.bpw);
	SPI_SETFREQUENCY(dev, dev->config.freq);
	SPI_SELECT(dev, dev->config.cs, true);
	SPI_SNDBLOCK(dev, buffer, buflen); // return type is void
	SPI_SELECT(dev, dev->config.cs, false);
	SPI_LOCK(dev, false);

	return 0;
}

/****************************************************************************
 * Name: spi_uioctrl
 *
 * Description:
 *   Control SPI Data. This function is automatically called when you're doing
 *   ctrl() API on userspace.
 *
 ****************************************************************************/
static int spi_uioctrl(FAR struct file *filep, int cmd, unsigned long arg)
{
	SPI_ENTRY;

	FAR struct inode *inode = filep->f_inode;
	FAR struct spi_dev_s *dev = inode->i_private;

	if (cmd == SPIIOC_SET_CONFIG) {
		struct spi_io_config *user_config = (struct spi_io_config *)arg;
		dev->config = *user_config;
		spidbg("bpw:%d freq:%d cs:%d mode%d",
			   dev->config.bpw,
			   dev->config.freq,
			   dev->config.cs,
			   dev->config.mode);
		SPI_LOCK(dev, true);
		SPI_SETMODE(dev, dev->config.mode);
		SPI_SETBITS(dev, dev->config.bpw);
		SPI_SETFREQUENCY(dev, dev->config.freq);
		SPI_LOCK(dev, false);
	}
#ifdef CONFIG_SPI_EXCHANGE
	else if (cmd == SPIIOC_EXCHANGE) {
		struct spi_io_msg *msg = (struct spi_io_msg *)arg;
		spidbg("IO EXCHAnge %p %p %d", msg->txbuf, msg->rxbuf, msg->length);
		SPI_LOCK(dev, true);
		SPI_SETMODE(dev, dev->config.mode);
		SPI_SETBITS(dev, dev->config.bpw);
		SPI_SETFREQUENCY(dev, dev->config.freq);
		SPI_SELECT(dev, dev->config.cs, true);
		SPI_EXCHANGE(dev, msg->txbuf, msg->rxbuf, msg->length);
		SPI_SELECT(dev, dev->config.cs, false);
		SPI_LOCK(dev, false);
	}
#endif
	else {
		return -ENOTTY;
	}

	return 0;
}

/****************************************************************************
 * Name: spi_uioregister
 *
 * Description:
 *   register device.
 *
 ****************************************************************************/
int spi_uioregister(FAR int bus, FAR struct spi_dev_s *dev)
{
	SPI_ENTRY;
	if (!dev) {
		return -1;
	}
	char path[16] = {0,};
	snprintf(path, 16, "/dev/spi-%d", bus);

	spidbg("Registering %s\n", path);
	return register_driver(path, &g_spiops, 0666, dev);
}
