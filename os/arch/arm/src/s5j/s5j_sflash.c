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
 * arch/arm/src/s5j/s5j_sflash.c
 *
 *   Copyright (C) 2015 Gregory Nutt. All rights reserved.
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
 * 3. Neither the name tinyara nor the names of its contributors may be
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
 *****************************************************************************/

/******************************************************************************
 * Included Files
 *****************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <debug.h>
#include <string.h>
#include <semaphore.h>
#include <tinyara/kmalloc.h>
#include <tinyara/spi/spi.h>
#include <arpa/inet.h>

#include "up_arch.h"
#include "cache.h"
#include <chip/s5j_gpio.h>
#include "s5j_qspi.h"

/******************************************************************************
 * Pre-processor Definitions
 *****************************************************************************/
#define S5J_SFLASH_MAPPED_ADDR   (CONFIG_S5J_FLASH_BASE)
#undef  S5J_SFLASH_USE_DIRECT_RW

#define SFLASH_SECTOR_SIZE       (CONFIG_S5J_FLASH_SECTOR_SIZE)
#define SECTOR_ALIGN_SIZE(x)     (((x)/SFLASH_SECTOR_SIZE)*SFLASH_SECTOR_SIZE)

#define MIN(a, b) (a > b) ? b : a

#if defined(CONFIG_BOARD_COREDUMP_FLASH) || defined(CONFIG_BOARD_RAMDUMP_FLASH) || \
	defined(CONFIG_BOARD_FOTA_SUPPORT)
volatile bool g_sflash_nonsleep_mode = false;

#define usleep(x) \
	do { \
		if (g_sflash_nonsleep_mode) { \
			up_udelay(x); \
		} else { \
			usleep(x); \
		} \
	} while (0)
#endif

/******************************************************************************
 * Private Types
 *****************************************************************************/
/* Command definitions */

enum s5j_sflash_command_e {
	SFLASH_WRITE_STATUS_REGISTER = 0x01,/* WRSR                   */
	SFLASH_WRITE = 0x02,
	SFLASH_READ = 0x03,
	SFLASH_WRITE_DISABLE = 0x04,		/* WRDI                   */
	SFLASH_READ_STATUS_REGISTER = 0x05,	/* RDSR                   */
	SFLASH_WRITE_ENABLE = 0x06,			/* WREN                   */
	SFLASH_SECTOR_ERASE = 0x20,			/* SE                     */
	SFLASH_READ_STATUS_REGISTER2 = 0x35,/* RDSR-2 - Winbond only  */
	SFLASH_BLOCK_ERASE_MID = 0x52,		/* SE                     */
	SFLASH_BLOCK_ERASE_LARGE = 0xD8,	/* SE                     */
	SFLASH_READ_ID1 = 0x90,				/* data size varies       */
	SFLASH_READ_ID2 = 0xAB,				/* data size varies       */
	SFLASH_READ_JEDEC_ID = 0x9F,		/* RDID                   */
	SFLASH_CHIP_ERASE1 = 0x60,			/* CE                     */
	SFLASH_CHIP_ERASE2 = 0xC7,			/* CE                     */
	SFLASH_DUMMY = 0xA5,				/* Dummy */
};

struct s5j_sflash_priv_s {
	uint32_t addr;
	sem_t sem_excl;				/* mutex */
};

struct s5j_sflash_dev_s {
	FAR const struct spi_ops_s *ops;
	struct s5j_sflash_priv_s priv;
};

/* this ptr can be used in nsh_cmd */

FAR struct s5j_sflash_dev_s *sflashdev;

/******************************************************************************
 * Private Function Prototypes
 *****************************************************************************/
static int s5j_sflash_init(struct s5j_sflash_dev_s *dev);
static int s5j_sflash_write_enable(struct s5j_sflash_dev_s *dev);
static int s5j_sflash_lock(FAR struct spi_dev_s *spidev, bool lock);
static void s5j_sflash_select(FAR struct spi_dev_s *spidev, enum spi_dev_e devid, bool selected);
static uint32_t s5j_sflash_setfrequency(FAR struct spi_dev_s *spidev, uint32_t frequency);
static int s5j_sflash_setmode(FAR struct spi_dev_s *spidev, enum spi_mode_e mode);
static int s5j_sflash_setbits(FAR struct spi_dev_s *spidev, int nbits);
#ifdef CONFIG_SPI_HWFEATURES
static int s5j_sflash_hwfeatures(FAR struct spi_dev_s *spidev, spi_hwfeatures_t features);
#endif
static uint16_t s5j_sflash_send(FAR struct spi_dev_s *spidev, uint16_t wd);
static void s5j_sflash_exchange(FAR struct spi_dev_s *spidev, FAR const void *txbuffer, FAR void *rxbuffer, size_t nwords);
static void s5j_sflash_sndblock(FAR struct spi_dev_s *spidev, FAR const void *buffer, size_t nwords);
static void s5j_sflash_recvblock(FAR struct spi_dev_s *spidev, FAR void *buffer, size_t nwords);

/******************************************************************************
 * Private Data
 *****************************************************************************/
static FAR const struct spi_ops_s s5j_spi_ops = {
	.lock = s5j_sflash_lock,
	.select = s5j_sflash_select,
	.setfrequency = s5j_sflash_setfrequency,
	.setmode = s5j_sflash_setmode,
	.setbits = s5j_sflash_setbits,
#ifdef CONFIG_SPI_HWFEATURES
	.hwfeatures = s5j_sflash_hwfeatures,
#endif
	.status = NULL,
#ifdef CONFIG_SPI_CMDDATA
	.cmddata = NULL,
#endif
	.send = s5j_sflash_send,
#ifdef CONFIG_SPI_EXCHANGE
	.exchange = s5j_sflash_exchange,
#else
	.sndblock = s5j_sflash_sndblock,
	.recvblock = s5j_sflash_recvblock,
#endif
	.registercallback = NULL
};

/******************************************************************************
 * Public Data
 *****************************************************************************/

/******************************************************************************
 * Private Functions
 *****************************************************************************/

static void QSPI_Set_GPIOPort(void)
{
	s32 gpio_sf_clk;
	s32 gpio_sf_cs;
	s32 gpio_sf_si;
	s32 gpio_sf_so;
	s32 gpio_sf_wp;
	s32 gpio_sf_hld;

	gpio_sf_clk = s5j_gpio(GPP1, 0);
	gpio_sf_cs = s5j_gpio(GPP1, 1);
	gpio_sf_si = s5j_gpio(GPP1, 2);
	gpio_sf_so = s5j_gpio(GPP1, 3);
	gpio_sf_wp = s5j_gpio(GPP1, 4);
	gpio_sf_hld = s5j_gpio(GPP1, 5);

	gpio_cfg_pin(gpio_sf_clk, GPIO_FUNC(2));
	gpio_cfg_pin(gpio_sf_cs, GPIO_FUNC(2));
	gpio_cfg_pin(gpio_sf_si, GPIO_FUNC(2));
	gpio_cfg_pin(gpio_sf_so, GPIO_FUNC(2));
	gpio_cfg_pin(gpio_sf_wp, GPIO_FUNC(2));
	gpio_cfg_pin(gpio_sf_hld, GPIO_FUNC(2));

	gpio_set_pull(gpio_sf_clk, GPIO_PULL_UP);
	gpio_set_pull(gpio_sf_cs, GPIO_PULL_UP);
	gpio_set_pull(gpio_sf_si, GPIO_PULL_UP);
	gpio_set_pull(gpio_sf_so, GPIO_PULL_UP);
	gpio_set_pull(gpio_sf_wp, GPIO_PULL_UP);
	gpio_set_pull(gpio_sf_hld, GPIO_PULL_UP);

	return;
}

/******************************************************************************
 * Name: s5j_sflash_sem_post
 *
 * Description:
 *   Release the mutual exclusion semaphore
 *
 *****************************************************************************/

static inline void s5j_sflash_sem_post(struct s5j_sflash_dev_s *dev)
{
	sem_post(&dev->priv.sem_excl);
}

/******************************************************************************
 * Name: s5j_sflash_sem_init
 *
 * Description:
 *   Initialize semaphores
 *
 *****************************************************************************/

static inline void s5j_sflash_sem_init(struct s5j_sflash_dev_s *dev)
{
	sem_init(&dev->priv.sem_excl, 0, 1);
}

/******************************************************************************
 * Name: s5j_sflash_sem_wait
 *
 * Description:
 *   Take the exclusive access, waiting as necessary
 *
 *****************************************************************************/

static inline void s5j_sflash_sem_wait(struct s5j_sflash_dev_s *dev)
{
	while (sem_wait(&dev->priv.sem_excl) != OK) {
		ASSERT(errno == EINTR);
	}
}

/******************************************************************************
 * Name: s5j_sflash_erase
 *
 * Description:
 *   erase flash
 *
 *****************************************************************************/

int s5j_sflash_erase(struct s5j_sflash_dev_s *dev, uint8_t cmd, uint32_t addr)
{
	int ret;
	uint32_t val;
	int size = 0;

#ifdef S5J_SFLASH_USE_DIRECT_RW
	/* debug code */

	if (dev == NULL) {
		dev = sflashdev;
	}
#endif

	do {
		ret = s5j_sflash_write_enable(dev);
		if (OK != ret) {
			usleep(100);
		}
	} while (OK != ret);

	switch (cmd) {
	case SFLASH_CHIP_ERASE1:
		val = getreg32(rSF_CON) | (1 << 31) | (1 << 15);
		putreg32(val, rSF_CON);
		addr = 0;
		Outp8(rCE, QSPI_DUMMY_DATA);
		break;
	case SFLASH_CHIP_ERASE2:
		addr = 0;
		Outp8(rCE, QSPI_DUMMY_DATA);
		break;
	case SFLASH_SECTOR_ERASE:
		Outp32(rERASE_ADDRESS, addr);
		Outp8(rSE, QSPI_DUMMY_DATA);
		size = 4096; /* Sub-sector size */
		break;
	case SFLASH_BLOCK_ERASE_LARGE:
		size = 65536; /* block size */
		SetBits(rCOMMAND2, 16, 0xFF, COMMAND_ERASE_64KB);
		/* SetBits(rCOMMAND2, 16, 0xFF, COMMAND_ERASE_32KB); */

		Outp32(rERASE_ADDRESS, addr);
		Outp8(rBE, QSPI_DUMMY_DATA);

		break;
	default:
		break;
	}

	while (getreg8(rRDSR) & 0x1) ;

	if (cmd != SFLASH_CHIP_ERASE1) {
		arch_invalidate_dcache(addr + S5J_SFLASH_MAPPED_ADDR, (addr + S5J_SFLASH_MAPPED_ADDR + size));
	}

	return OK;
}

int s5j_sflash_write_enable(struct s5j_sflash_dev_s *dev)
{
	SetBits(rCOMMAND4, 8, 0xFF, 0x06);

	return OK;
}

/******************************************************************************
 * Name: s5j_sflash_read
 *
 * Description:
 *   Read data from flash.
 *
 *****************************************************************************/

int s5j_sflash_read(struct s5j_sflash_dev_s *dev, uint32_t addr, uint8_t *buf, uint32_t size)
{
	int32_t i;
	int32_t readsize;
	uint32_t offset;

	i = 0;

#ifdef S5J_SFLASH_USE_DIRECT_RW
	/* debug code */

	if (dev == NULL) {
		dev = sflashdev;
	}
#endif

	fvdbg("addr 0x%x, size %d\n", addr, size);
	while (size != 0) {
		/* first align */

		offset = addr % SFLASH_SECTOR_SIZE;
		readsize = MIN(SFLASH_SECTOR_SIZE - offset, size);

		memcpy((void *)(buf + i), (void *)(addr + S5J_SFLASH_MAPPED_ADDR), readsize);

		addr += readsize;
		i += readsize;
		size -= readsize;
	}

	fvdbg("buf[%d]\n", *buf);
	return OK;
}

/******************************************************************************
 * Name: s5j_sflash_write
 *
 * Description:
 *  Write data.
 *
 *****************************************************************************/

int s5j_sflash_write(struct s5j_sflash_dev_s *dev, uint32_t addr, uint8_t *buf, uint32_t size)
{
	int32_t i;
	int32_t readsize;
	uint32_t offset;

	i = 0;

	/* write is directly called from nshlib, so may allow it */

#ifdef S5J_SFLASH_USE_DIRECT_RW
	/* debug code */

	if (dev == NULL) {
		dev = sflashdev;
	}
#endif

	while (size != 0) {
		/* first align */

		offset = addr % SFLASH_SECTOR_SIZE;
		readsize = MIN(SFLASH_SECTOR_SIZE - offset, size);

		memcpy((void *)(addr + S5J_SFLASH_MAPPED_ADDR), (void *)(buf + i), readsize);
		arch_flush_dcache(addr + S5J_SFLASH_MAPPED_ADDR, (addr + S5J_SFLASH_MAPPED_ADDR + readsize));
		addr += readsize;
		i += readsize;
		size -= readsize;

		/* check write in progress */
		while (getreg8(rRDSR) & 0x1) ;
	}

	return OK;
}

/******************************************************************************
 * Name: s5j_sflash_init
 *
 * Description:
 *  initialize sflash controller.
 *
 *****************************************************************************/

static int s5j_sflash_init(struct s5j_sflash_dev_s *dev)
{
	/* initialize mutex */

	s5j_sflash_sem_init(dev);

	QSPI_Set_GPIOPort();
	HW_REG32(0x80310000, 0x04) = 0x8010001A;	/* disable WP */
	HW_REG32(0x80310000, 0x78) = 0x8;			//FLASH_IO_MODE
	HW_REG32(0x80310000, 0x74) = 0x4;			//QUAD_FAST_READ

	/* COMMNAD1 will be asserted by default value */
	cal_clk_setrate(d1_serialflash, 80000000);

	return OK;
}

/******************************************************************************
 * Name: s5j_sflash_lock
 *
 * Description:
 *  Support Lock/Unlock. This function s a part of spi_ops.
 *
 *****************************************************************************/

static int s5j_sflash_lock(FAR struct spi_dev_s *spidev, bool lock)
{
	struct s5j_sflash_dev_s *dev;

	dev = (struct s5j_sflash_dev_s *)spidev;

	if (lock) {
		s5j_sflash_sem_wait(dev);
	} else {
		s5j_sflash_sem_post(dev);
	}
	return OK;
}

/******************************************************************************
 * Name: s5j_sflash_select
 *
 * Description:
 *  Support SPI CS. But, the S5J only 1 sflash. We don't necessary.
 *  This function is a part of spi_ops.
 *
 *****************************************************************************/

static void s5j_sflash_select(FAR struct spi_dev_s *spidev, enum spi_dev_e devid, bool selected)
{
	return;
}

/******************************************************************************
 * Name: s5j_sflash_setfrequency
 *
 * Description:
 *  Support changing frequency.
 *
 *****************************************************************************/

static uint32_t s5j_sflash_setfrequency(FAR struct spi_dev_s *spidev, uint32_t frequency)
{
	return OK;
}

/******************************************************************************
 * Name: s5j_sflash_setmode
 *
 * Description:
 *  Support changing mode(CPHA/CPOL).
 *
 *****************************************************************************/

static int s5j_sflash_setmode(FAR struct spi_dev_s *spidev, enum spi_mode_e mode)
{
	return OK;
}

/******************************************************************************
 * Name: s5j_sflash_setbits
 *
 * Description:
 *  Support changing 1/4Bit. But we can't change bit.
 *  If you need to change bit, please ask the broadcom.
 *  This function is a part of spi_ops.
 *
 *****************************************************************************/

static int s5j_sflash_setbits(FAR struct spi_dev_s *spidev, int nbits)
{
	return OK;
}

#ifdef CONFIG_SPI_HWFEATURES

/******************************************************************************
 * Name: s5j_sflash_setbits
 *
 * Description:
 *  This function is a part of spi_ops
 *
 *****************************************************************************/

static int s5j_sflash_hwfeatures(FAR struct spi_dev_s *spidev, spi_hwfeatures_t features)
{
	return OK;
}
#endif

/******************************************************************************
 * Name: s5j_sflash_send
 *
 * Description:
 *  Send a slave address from upper-level driver to device.
 *  This function is a part of spi_ops.
 *
 *****************************************************************************/

static uint16_t s5j_sflash_send(FAR struct spi_dev_s *spidev, uint16_t wd)
{
	uint16_t data;
	static uint32_t val;
	static int32_t byteoffset;
	static uint8_t addrmode;
	static uint8_t erasemode;
	static uint8_t cmd;
	static int32_t addroffset;
	struct s5j_sflash_dev_s *dev;

	data = 0;

	dev = (struct s5j_sflash_dev_s *)spidev;

	if (addrmode) {
		dev->priv.addr |= (wd << addroffset);

		addroffset -= 8;

		if (addroffset < 0) {
			addrmode = 0;

			if (erasemode) {
				erasemode = 0;
				s5j_sflash_erase(dev, cmd, dev->priv.addr);
			}
		}

		return OK;
	}

	switch (wd) {
	case SFLASH_READ_JEDEC_ID:
		val = 0;
		byteoffset = 24;
		val = getreg32(rRDID);
		/* mtd driver needs to Big endian */
		val = htonl(val);
		break;
	case SFLASH_READ_STATUS_REGISTER:
		val = 0;
		byteoffset = 0;
		val = getreg8(rRDSR);
		//val = (val >> 8) & 0xFF;
		break;
	case SFLASH_DUMMY:
		/* need to touch flash ? */
		data = (val >> byteoffset) & 0xFF;
		byteoffset -= 8;
		break;
	case SFLASH_READ:
		addrmode = 1;
		addroffset = 16;
		dev->priv.addr = 0;
		break;
	case SFLASH_WRITE:
		addrmode = 1;
		addroffset = 16;
		dev->priv.addr = 0;
		break;
	case SFLASH_WRITE_ENABLE:
		s5j_sflash_write_enable(dev);
		break;
	case SFLASH_CHIP_ERASE1:
	case SFLASH_CHIP_ERASE2:
		s5j_sflash_erase(dev, SFLASH_CHIP_ERASE1, 0);
		break;
	case SFLASH_SECTOR_ERASE:
	case SFLASH_BLOCK_ERASE_LARGE:
		addrmode = 1;
		erasemode = 1;
		addroffset = 16;
		dev->priv.addr = 0;
		cmd = wd;
		break;
	default:
		break;
	}

	return data;
}

/******************************************************************************
 * Name: s5j_sflash_exchange
 *
 * Description:
 *  Send data from upper-level driver to device.
 *  This function is a part of spi_ops.
 *
 *****************************************************************************/

static void s5j_sflash_exchange(FAR struct spi_dev_s *spidev, FAR const void *txbuffer, FAR void *rxbuffer, size_t nwords)
{
	if (txbuffer) {
		s5j_sflash_sndblock(spidev, txbuffer, nwords);
	} else if (rxbuffer) {
		s5j_sflash_recvblock(spidev, rxbuffer, nwords);
	} else {
		ASSERT(0);
	}
}

/******************************************************************************
 * Name: s5j_sflash_sndblock
 *
 * Description:
 *  Send data from upper-level driver to device.
 *  This function is a part of spi_ops.
 *
 *****************************************************************************/

static void s5j_sflash_sndblock(FAR struct spi_dev_s *spidev, FAR const void *buffer, size_t nwords)
{
	struct s5j_sflash_dev_s *dev;

	dev = (struct s5j_sflash_dev_s *)spidev;

	/* it's not word, it's byte */

	s5j_sflash_write(dev, dev->priv.addr, (uint8_t *)buffer, nwords);

	return;
}

/******************************************************************************
 * Name: s5j_sflash_recvblock
 *
 * Description:
 *  Recv data from device to upper-level driver
 *  This function is a part of spi_ops.
 *
 *****************************************************************************/

static void s5j_sflash_recvblock(FAR struct spi_dev_s *spidev, FAR void *buffer, size_t nwords)
{
	struct s5j_sflash_dev_s *dev;

	dev = (struct s5j_sflash_dev_s *)spidev;

	/* it's not word, it's byte */

	s5j_sflash_read(dev, dev->priv.addr, buffer, nwords);
}

/******************************************************************************
 * Public Functions
 *****************************************************************************/

/*******************************************************************************
 * Name: up_spiflashinitialize
 *
 * Description:
 *   Initialize the selected SPI port
 *
 * Returned Value:
 *   Valid SPI device structure reference on success; a NULL on failure
 *
 ******************************************************************************/

FAR struct spi_dev_s *up_spiflashinitialize(void)
{
	static bool sflashdev_initialized = 0;
	/* Lets have single instance */
	if (sflashdev_initialized == 0) {
		if (!(sflashdev = kmm_malloc(sizeof(struct s5j_sflash_dev_s)))) {
			return NULL;
		}
		sflashdev->ops = (FAR const struct spi_ops_s *)&s5j_spi_ops;
		s5j_sflash_init(sflashdev);
		sflashdev_initialized = 1;
	}
	return (FAR struct spi_dev_s *)sflashdev;
}
