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
 * arch/arm/src/s5j/s5j_spi.c
 *
 *   Copyright (C) 2013 Gregory Nutt. All rights reserved.
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

/*****************************************************************************
 * Included Files
 *****************************************************************************/
#include <tinyara/config.h>

#include <stdio.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>
#include <errno.h>

#include <tinyara/arch.h>
#include <tinyara/spi/spi.h>

#include "up_arch.h"
#include <s5j_spi.h>
#include <string.h>
#include <assert.h>
#include <debug.h>
#include <tinyara/irq.h>
#include <arch/irq.h>
#include <arch/board/board.h>
#include <tinyara/kmalloc.h>
#include <poll.h>
#include <tinyara/fs/fs.h>
#include <stddef.h>
#include <chip.h>

#include "s5j_clock.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/
#define CH_CFG_TX_CH_ON			(1 << 0)
#define CH_CFG_TX_CH_OFF		(0 << 0)
#define CH_CFG_RX_CH_ON			(1 << 1)
#define CH_CFG_RX_CH_OFF		(0 << 1)
#define CH_CFG_MODE_MASK		(3 << 2)
#define CH_CFG_MODE(x)			((x & 3) << 2)
#define CH_CFG_SLAVE			(1 << 4)
#define CH_CFG_MASTER			(0 << 4)
#define CH_CFG_FIFO_FLUSH		(1 << 5)
#define CH_CFG_FIFO_FLUSH_OFF	(0 << 5)
#define CH_CFG_HIGH_SPEED_EN	(1 << 6)
#define CH_CFG_HIGH_SPEED_DIS	(0 << 6)

#define MODE_CFG_DMA_SINGLE		(0 << 0)
#define MODE_CFG_DMA_4BURST		(1 << 0)
#define MODE_CFG_DMA_TX_ON		(1 << 1)
#define MODE_CFG_DMA_TX_OFF		(0 << 1)
#define MODE_CFG_DMA_RX_ON		(1 << 2)
#define MODE_CFG_DMA_RX_OFF		(0 << 2)
#define MODE_CFG_TX_RDY_LVL(x)	((x & 0x3F) << 5)
#define MODE_CFG_RX_RDY_LVL(x)	((x & 0x3F) << 11)
#define MODE_CFG_BUS_WDT_MASK	(3 << 17)
#define MODE_CFG_BUS_WIDTH_8	(0 << 17)
#define MODE_CFG_BUS_WIDTH_16	(1 << 17)
#define MODE_CFG_BUS_WIDTH_32	(2 << 17)
#define MODE_CFG_TRLNG_CNT(x)	((x & 0x3FF) << 19)
#define MODE_CFG_CH_WDT_MASK	(3 << 29)
#define MODE_CFG_CH_WIDTH_8		(0 << 29)
#define MODE_CFG_CH_WIDTH_16	(1 << 29)
#define MODE_CFG_CH_WIDTH_32	(2 << 29)

#define CS_REG_nSS_ACTIVE		(0 << 0)
#define CS_REG_nSS_INACTIVE		(1 << 0)
#define CS_REG_nSS_AUTO			(1 << 1)
#define CS_REG_nSS_MANUAL		(0 << 1)
#define CS_REG_nSS_TIME_CNT(x)	((x & 0x3F) << 1)

#define INT_MASK_TRAILING		(1 << 6)
#define INT_MASK_RX_OVERRUN		(1 << 5)
#define INT_MASK_RX_UNDERRUN	(1 << 4)
#define INT_MASK_TX_OVERRUN		(1 << 3)
#define INT_MASK_TX_UNDERRUN	(1 << 2)
#define INT_MASK_RX_FIFO_RDY	(1 << 1)
#define INT_MASK_TX_FIFO_RDY	(1 << 0)

#define SPI_STAT_TX_FIFO_RDY(x)		((x >> 0) & 1)
#define SPI_STAT_RX_FIFO_RDY(x)		((x >> 1) & 1)
#define SPI_STAT_TX_UNDERRUN(x)		((x >> 2) & 1)
#define SPI_STAT_TX_OVERRUN(x)		((x >> 3) & 1)
#define SPI_STAT_RX_UNDERRUN(x)		((x >> 4) & 1)
#define SPI_STAT_RX_OVERRUN(x)		((x >> 5) & 1)
#define SPI_STAT_TX_FIFO_LVL(x)		((x >> 6) & 0x1FF)
#define SPI_STAT_RX_FIFO_LVL(x)		((x >> 15) & 0x1FF)
#define SPI_STAT_TRAILING_BYTE(x)	((x >> 24) & 1)
#define SPI_STAT_TX_DONE(x)			((x >> 25) & 1)

/****************************************************************************
 * Private Types
 ****************************************************************************/
struct s5j_spidev_s {
	struct spi_dev_s spidev;
	uint32_t base;
	uint8_t port;
	unsigned int freqid;
	s32 gpio_clk;
	s32 gpio_nss;
	s32 gpio_miso;
	s32 gpio_mosi;

#ifndef CONFIG_SPI_POLLWAIT
	sem_t xfrsem;				/* Wait for transfer to complete */
#endif

#ifndef CONFIG_SPI_OWNBUS
	sem_t exclsem;
#endif
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/* SPI Driver Methods */
static int spi_lock(FAR struct spi_dev_s *dev, bool lock);
static void spi_select(FAR struct spi_dev_s *dev, enum spi_dev_e devid, bool selected);
static uint32_t spi_setfrequency(FAR struct spi_dev_s *dev, uint32_t frequency);
static void spi_setmode(FAR struct spi_dev_s *dev, enum spi_mode_e mode);
static void spi_setbits(FAR struct spi_dev_s *dev, int nbits);
static uint16_t spi_send(FAR struct spi_dev_s *dev, uint16_t wd);
static void spi_exchange(FAR struct spi_dev_s *dev, const void *txbuffer, void *rxbuffer, size_t nwords);

#ifndef CONFIG_SPI_EXCHANGE
static void spi_sndblock(FAR struct spi_dev_s *dev, const void *txbuffer, size_t nwords);
static void spi_recvblock(FAR struct spi_dev_s *dev, void *rxbuffer, size_t nwords);
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/
static const struct spi_ops_s g_spiops = {
#ifndef CONFIG_SPI_OWNBUS
	.lock = spi_lock,
#endif
	.select = spi_select,
	.setfrequency = spi_setfrequency,
	.setmode = (void *)spi_setmode,
	.setbits = (void *)spi_setbits,
	.status = 0,
#ifdef CONFIG_SPI_CMDDATA
	.cmddata = 0,
#endif
	.send = spi_send,
#ifdef CONFIG_SPI_EXCHANGE
	.exchange = spi_exchange,
#else
	.sndblock = spi_sndblock,
	.recvblock = spi_recvblock,
#endif
	.registercallback = 0,
};

static struct s5j_spidev_s g_spi0dev = {
	.spidev		= { .ops = &g_spiops },
	.base		= S5J_SPI0_BASE,
	.port		= SPI_PORT0,
	.freqid		= CLK_SPL_SPI0,
	.gpio_clk	= GPIO_SPI0_CLK,
	.gpio_nss	= GPIO_SPI0_CS,
	.gpio_miso	= GPIO_SPI0_MISO,
	.gpio_mosi	= GPIO_SPI0_MOSI,
};

static struct s5j_spidev_s g_spi1dev = {
	.spidev		= { .ops = &g_spiops },
	.base		= S5J_SPI1_BASE,
	.port		= SPI_PORT1,
	.freqid		= CLK_SPL_SPI1,
	.gpio_clk	= GPIO_SPI1_CLK,
	.gpio_nss	= GPIO_SPI1_CS,
	.gpio_miso	= GPIO_SPI1_MISO,
	.gpio_mosi	= GPIO_SPI1_MOSI,
};

static struct s5j_spidev_s g_spi2dev = {
	.spidev		= { .ops = &g_spiops },
	.base		= S5J_SPI2_BASE,
	.port		= SPI_PORT2,
	.freqid		= CLK_SPL_SPI2,
	.gpio_clk	= GPIO_SPI2_CLK,
	.gpio_nss	= GPIO_SPI2_CS,
	.gpio_miso	= GPIO_SPI2_MISO,
	.gpio_mosi	= GPIO_SPI2_MOSI,
};

static struct s5j_spidev_s g_spi3dev = {
	.spidev		= { .ops = &g_spiops },
	.base		= S5J_SPI3_BASE,
	.port		= SPI_PORT3,
	.freqid		= CLK_SPL_SPI3,
	.gpio_clk	= GPIO_SPI3_CLK,
	.gpio_nss	= GPIO_SPI3_CS,
	.gpio_miso	= GPIO_SPI3_MISO,
	.gpio_mosi	= GPIO_SPI3_MOSI,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: spi_lock
 *
 * Description:
 *   On SPI busses where there are multiple devices, it will be necessary to
 *   lock SPI to have exclusive access to the busses for a sequence of
 *   transfers.  The bus should be locked before the chip is selected. After
 *   locking the SPI bus, the caller should then also call the setfrequency,
 *   setbits, and setmode methods to make sure that the SPI is properly
 *   configured for the device.  If the SPI buss is being shared, then it
 *   may have been left in an incompatible state.
 *
 ****************************************************************************/
static int spi_lock(struct spi_dev_s *dev, bool lock)
{
	FAR struct s5j_spidev_s *priv = (FAR struct s5j_spidev_s *)dev->priv;

	if (lock) {
		while (sem_wait(&priv->exclsem) != 0) {
			DEBUGASSERT(errno == EINTR);
		}
	} else {
		(void)sem_post(&priv->exclsem);
	}

	return OK;
}

/****************************************************************************
 * Name: spi_setfrequency
 *
 * Description:
 *   Set the SPI frequency.
 *
 ****************************************************************************/
static uint32_t spi_setfrequency(struct spi_dev_s *dev, uint32_t frequency)
{
	FAR struct s5j_spidev_s *priv = (FAR struct s5j_spidev_s *)dev->priv;

	s5j_clk_set_rate(priv->freqid, (unsigned long)frequency);

	return OK;
}

/****************************************************************************
 * Name: spi_select
 *
 * Description:
 *   Enable/disable the SPI slave select.   The implementation of this method
 *   must include handshaking:  If a device is selected, it must hold off
 *   all other attempts to select the device until the device is deselecte.
 *
 ****************************************************************************/
static void spi_select(struct spi_dev_s *dev, enum spi_dev_e devid, bool selected)
{
	FAR struct s5j_spidev_s *priv = (FAR struct s5j_spidev_s *)dev->priv;

	SPI_SFR *pSPIRegs;
	pSPIRegs = (SPI_SFR *)priv->base;

	unsigned int cs_reg;
	cs_reg = getreg32(&pSPIRegs->CS_REG);
	cs_reg |= CS_REG_nSS_INACTIVE;

	if (selected == TRUE) {
		cs_reg &= ~CS_REG_nSS_INACTIVE;
	}

	putreg32(cs_reg, &pSPIRegs->CS_REG);
}

/****************************************************************************
 * Name: spi_setmode
 *
 * Description:
 *   Set the SPI mode.  see enum spi_mode_e for mode definitions
 *
 ****************************************************************************/
static void spi_setmode(struct spi_dev_s *dev, enum spi_mode_e mode)
{
	FAR struct s5j_spidev_s *priv = (FAR struct s5j_spidev_s *)dev->priv;

	SPI_SFR *pSPIRegs;
	pSPIRegs = (SPI_SFR *)priv->base;

	unsigned int ch_cfg;
	ch_cfg = getreg32(&pSPIRegs->CH_CFG);
	ch_cfg = (ch_cfg & (~CH_CFG_MODE_MASK)) | CH_CFG_MODE(mode);
	putreg32(ch_cfg, &pSPIRegs->CH_CFG);
}

/****************************************************************************
 * Name: spi_setbits
 *
 * Description:
 *   Set the number of bits per word.
 *
 ****************************************************************************/
static void spi_setbits(struct spi_dev_s *dev, int nbits)
{
	FAR struct s5j_spidev_s *priv = (FAR struct s5j_spidev_s *)dev->priv;

	SPI_SFR *pSPIRegs;
	pSPIRegs = (SPI_SFR *)priv->base;

	unsigned int mode_cfg;
	mode_cfg = getreg32(&pSPIRegs->MODE_CFG);
	mode_cfg = mode_cfg & (~(MODE_CFG_BUS_WDT_MASK | MODE_CFG_CH_WDT_MASK));

	switch (nbits) {
	case 8:
		mode_cfg = mode_cfg | MODE_CFG_BUS_WIDTH_8 | MODE_CFG_CH_WIDTH_8;
		break;

	case 16:
		mode_cfg = mode_cfg | MODE_CFG_BUS_WIDTH_16 | MODE_CFG_CH_WIDTH_16;
		break;

	case 32:
		mode_cfg = mode_cfg | MODE_CFG_BUS_WIDTH_32 | MODE_CFG_CH_WIDTH_32;
		break;

	default:
		DEBUGASSERT(0 == 1);
		break;
	}

	putreg32(mode_cfg, &pSPIRegs->MODE_CFG);
}

/****************************************************************************
 * Name: spi_send
 *
 * Description:
 *   Exchange one word on SPI. Currently support only byte transfers.
 *
 ****************************************************************************/
static uint16_t spi_send(struct spi_dev_s *dev, uint16_t wd)
{
	uint8_t txbyte;
	uint8_t rxbyte;

	txbyte = (uint8_t)wd;
	rxbyte = (uint8_t)0;
	spi_exchange(dev, &txbyte, &rxbyte, 1);

	return (uint16_t)rxbyte;
}

/****************************************************************************
 * Name: spi_exchange
 *
 * Description:
 *   Exchange a block data with the SPI device. Support only byte transfers.
 *
 ****************************************************************************/
static void spi_exchange(struct spi_dev_s *dev, const void *txbuffer, void *rxbuffer, size_t nwords)
{
	FAR struct s5j_spidev_s *priv = (FAR struct s5j_spidev_s *)dev->priv;

	size_t sent = 0;
	size_t received = 0;
	unsigned int dummy_rx;

	SPI_SFR *pSPIRegs;
	pSPIRegs = (SPI_SFR *)priv->base;

	/* SPI FIFO FLUSH */
	int ch_cfg = getreg32(&pSPIRegs->CH_CFG);
	putreg32(CH_CFG_FIFO_FLUSH, &pSPIRegs->CH_CFG);
	putreg32(ch_cfg, &pSPIRegs->CH_CFG);

	/* TX/RX */
	if ((rxbuffer == NULL) && (txbuffer == NULL)) {
		while (received < nwords) {
			if (sent < nwords)
				if (SPI_STAT_TX_FIFO_LVL(getreg32(&pSPIRegs->SPI_STATUS)) < 64) {
					putreg32(0, &pSPIRegs->SPI_TX_DATA);
					sent++;
				}

			if (SPI_STAT_RX_FIFO_LVL(getreg32(&pSPIRegs->SPI_STATUS)) > 0) {
				dummy_rx = getreg32(&pSPIRegs->SPI_RX_DATA);
				received++;
			}
		}
		return;
	}

	if (rxbuffer == NULL) {
		while (received < nwords) {
			if (sent < nwords)
				if (SPI_STAT_TX_FIFO_LVL(getreg32(&pSPIRegs->SPI_STATUS)) < 64) {
					putreg32(((unsigned char *)txbuffer)[sent++], &pSPIRegs->SPI_TX_DATA);
				}

			if (SPI_STAT_RX_FIFO_LVL(getreg32(&pSPIRegs->SPI_STATUS)) > 0) {
				dummy_rx = getreg32(&pSPIRegs->SPI_RX_DATA);
				received++;
			}
		}
		return;
	}

	if (txbuffer == NULL) {
		while (received < nwords) {
			if (sent < nwords)
				if (SPI_STAT_TX_FIFO_LVL(getreg32(&pSPIRegs->SPI_STATUS)) < 64) {
					putreg32(((unsigned char *)rxbuffer)[sent++], &pSPIRegs->SPI_TX_DATA);
				}

			if (SPI_STAT_RX_FIFO_LVL(getreg32(&pSPIRegs->SPI_STATUS)) > 0) {
				((unsigned char *)rxbuffer)[received++] = getreg32(&pSPIRegs->SPI_RX_DATA);
			}

		}
		return;
	}

	while (received < nwords) {
		if (sent < nwords)
			if (SPI_STAT_TX_FIFO_LVL(getreg32(&pSPIRegs->SPI_STATUS)) < 64) {
				putreg32(((unsigned char *)txbuffer)[sent++], &pSPIRegs->SPI_TX_DATA);
			}

		if (SPI_STAT_RX_FIFO_LVL(getreg32(&pSPIRegs->SPI_STATUS)) > 0) {
			((unsigned char *)rxbuffer)[received++] = getreg32(&pSPIRegs->SPI_RX_DATA);
		}

	}
}

/****************************************************************************
 * Name: spi_sndblock
 *
 * Description:
 *   Send a block of data on SPI. Support only byte transfers
 *
 ****************************************************************************/
static void spi_sndblock(struct spi_dev_s *dev, const void *txbuffer, size_t nwords)
{
	spi_exchange(dev, txbuffer, NULL, nwords);
}

/****************************************************************************
 * Name: spi_recvblock
 *
 * Description:
 *   Revice a block of data from SPI. Support only byte transfers.
 *
 ****************************************************************************/
static void spi_recvblock(struct spi_dev_s *dev, void *rxbuffer, size_t nwords)
{
	spi_exchange(dev, NULL, rxbuffer, nwords);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_spiinitialize
 *
 * Description:
 *   Initialize the selected SPI port
 *
 ****************************************************************************/
struct spi_dev_s *up_spiinitialize(int port)
{
	FAR struct s5j_spidev_s *priv = NULL;

	if (port < 0 || port >= SPI_PORT_MAX) {
		return NULL;
	}

	switch (port) {
	case 0:
		priv = &g_spi0dev;
		break;
	case 1:
		priv = &g_spi1dev;
		break;
	case 2:
		priv = &g_spi2dev;
		break;
	case 3:
		priv = &g_spi3dev;
		break;
	}
	lldbg("Prepare SPI%d for Master operation\n", priv->port);

	/* SET GPIO for the port */
	s5j_configgpio(priv->gpio_clk);
	s5j_configgpio(priv->gpio_nss);
	s5j_configgpio(priv->gpio_miso);
	s5j_configgpio(priv->gpio_mosi);

#ifndef CONFIG_SPI_POLLWAIT
	sem_init(&priv->xfrsem, 0, 0);
#endif

#ifndef CONFIG_SPI_OWNBUS
	sem_init(&priv->exclsem, 0, 1);
#endif

	/* SET SPI INITIAL */
	SPI_SFR *pSPIRegs;
	pSPIRegs = (SPI_SFR *)priv->base;

	/* TX/RX enable. Master.CPHA 00. */
	putreg32(CH_CFG_HIGH_SPEED_DIS | CH_CFG_FIFO_FLUSH_OFF | CH_CFG_MASTER | CH_CFG_MODE(SPIDEV_MODE0) | CH_CFG_RX_CH_ON | CH_CFG_TX_CH_ON, &pSPIRegs->CH_CFG);

	/*  No FIFO. N0 DMA. 8 bits */
	putreg32(MODE_CFG_CH_WIDTH_8 | MODE_CFG_TRLNG_CNT(0) | MODE_CFG_BUS_WIDTH_8 | MODE_CFG_RX_RDY_LVL(0) | MODE_CFG_TX_RDY_LVL(0) | MODE_CFG_DMA_RX_OFF | MODE_CFG_DMA_TX_OFF | MODE_CFG_DMA_SINGLE, &pSPIRegs->MODE_CFG);

	/* CS Manual Passive */
	putreg32(CS_REG_nSS_TIME_CNT(0) | CS_REG_nSS_MANUAL | CS_REG_nSS_INACTIVE, &pSPIRegs->CS_REG);

	/* Disable Interrupts */
	putreg32(0, &pSPIRegs->SPI_INT_EN);

	priv->spidev.priv = (void *)priv;

	return &priv->spidev;
}

/****************************************************************************
 * Name: s5j_spi_register
 *
 * Description:
 *   register SPI of s5j
 *
 ****************************************************************************/
void s5j_spi_register(int bus)
{
	FAR struct spi_dev_s *spi;
	spi = up_spiinitialize(bus);

#ifdef CONFIG_SPI_USERIO
	if (spi_uioregister(bus, spi) < 0) {
		lldbg("fail to register SPI");
	}
#endif
}
