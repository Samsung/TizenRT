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

/*****************************************************************************
 * Definitions
 *****************************************************************************/
#define SPI_FILE_OPT 1

#define SPI0_BASE S5J_SPI0_BASE
#define SPI1_BASE S5J_SPI1_BASE
#define SPI2_BASE S5J_SPI2_BASE
#define SPI3_BASE S5J_SPI3_BASE

#define SPI_BUF_SIZE    (0x1000)	/* 1024 Byte */

/*****************************************************************************
 * Private Types
 *****************************************************************************/

struct s5jt200_spidev_s {
	struct spi_dev_s spidev;

#ifndef CONFIG_SPI_POLLWAIT
	sem_t xfrsem;				/* Wait for transfer to complete */
#endif
	void *txbuffer;				/* Source buffer */
	void *rxbuffer;				/* Destination buffer */

	/* Do we need it ???
	   void  (*txword)(struct s5jt200_spidev_s *priv);
	   void  (*rxword)(struct s5jt200_spidev_s *priv);
	 */

	uint32_t base;
	uint8_t spiirq;				/* SPI IRQ number */

#ifdef CONFIG_SPI_DMA
	/* Put some here if we will decide to use DMA some day */
#endif

#ifndef CONFIG_SPI_OWNBUS
	sem_t exclsem;
	uint32_t frequency;
	uint32_t actual;
	uint8_t nbits;
	uint8_t mode;
#endif
	uint8_t port;

};

/*****************************************************************************
 * Private Function Prototypes
 *****************************************************************************/

static int spi_lock(struct spi_dev_s *dev, bool lock);
static void spi_select(struct spi_dev_s *dev, enum spi_dev_e devid, bool selected);
static uint32_t spi_setfrequency(struct spi_dev_s *dev, uint32_t frequency);
static void spi_setmode(struct spi_dev_s *dev, enum spi_mode_e mode);
static void spi_setbits(struct spi_dev_s *dev, int nbits);
static uint8_t spi_status(struct spi_dev_s *dev, enum spi_dev_e devid);
static uint16_t spi_send(struct spi_dev_s *dev, uint16_t word);
static void spi_exchange(struct spi_dev_s *dev, const void *txbuffer, void *rxbuffer, size_t nwords);
#ifndef CONFIG_SPI_EXCHANGE
static void spi_sndblock(struct spi_dev_s *dev, const void *txbuffer, size_t nwords);
static void spi_recvblock(struct spi_dev_s *dev, void *rxbuffer, size_t nwords);
#endif

/*****************************************************************************
 * Private Data
 *****************************************************************************/
#define GSIO_0_IF                   S5J_SPI0_BASE
#define GSIO_0_CLKDIV               0x180000F0
#define GSIO_1_IF                   S5J_SPI0_BASE
#define GSIO_1_CLKDIV               0x1800048C

struct gsio_s5jt200_ctrl_regs_s {
	uint32_t ctrl;
	uint32_t address;
	uint32_t data;
	/* clockdiv outside of this struct */
};

struct gsio_s5jt200_regs_s {
	volatile struct gsio_s5jt200_ctrl_regs_s *interface;
	volatile uint32_t *clkdiv;
};

static const struct gsio_s5jt200_regs_s gsio_regs[2] = {
	{
		.interface = (volatile struct gsio_s5jt200_ctrl_regs_s *)GSIO_0_IF,
		.clkdiv = (volatile uint32_t *)GSIO_0_CLKDIV,
	},
	{
		.interface = (volatile struct gsio_s5jt200_ctrl_regs_s *)GSIO_1_IF,
		.clkdiv = (volatile uint32_t *)GSIO_1_CLKDIV,
	},
};

static const struct spi_ops_s g_spiops = {
#ifndef CONFIG_SPI_OWNBUS
	.lock = spi_lock,			//done
#endif
	.select = spi_select,		//done
	.setfrequency = spi_setfrequency,	//more
	.setmode = (void *)spi_setmode,	//done
	.setbits = (void *)spi_setbits,	//done
	.status = spi_status,		//done
#ifdef CONFIG_SPI_CMDDATA
	.cmddata = MISSING FUNCTION;
#endif
	.send = spi_send,
#ifdef CONFIG_SPI_EXCHANGE
	.exchange = spi_exchange,	//more
#else
	.sndblock = spi_sndblock,	//more
	.recvblock = spi_recvblock,	//more
#endif
	.registercallback = 0,
};

static struct s5jt200_spidev_s g_spidev[] = {
	/*Port 0 */
	{
		.spidev = {
			.ops = &g_spiops,
		},
		.base = S5J_SPI0_BASE,
		.port = SPI_PORT0,
	},
	/*Port 1 */
	{
		.spidev = {
			.ops = &g_spiops,
		},
		.base = S5J_SPI1_BASE,
		.port = SPI_PORT1,
	},
	/*Port 2 */
	{
		.spidev = {
			.ops = &g_spiops,
		},
		.base = S5J_SPI2_BASE,
		.port = SPI_PORT2,
	},
	/*Port 3 */
	{
		.spidev = {
			.ops = &g_spiops,
		},
		.base = S5J_SPI3_BASE,
		.port = SPI_PORT3,
	},
};

/*****************************************************************************
 * Public Data
 *****************************************************************************/

/*****************************************************************************
 * Private Functions
 *****************************************************************************/

static void spi_set_gpio(SPI_PORT ePort, u32 DrvStrength)
{
	s32 gpio_clk, gpio_nss, gpio_miso, gpio_mosi;
	if (ePort == SPI_PORT0) {
		gpio_clk = s5j_gpio(GPP0, 0);
		gpio_nss = s5j_gpio(GPP0, 1);
		gpio_miso = s5j_gpio(GPP0, 2);
		gpio_mosi = s5j_gpio(GPP0, 3);

		gpio_cfg_pin(gpio_clk, GPIO_FUNC(2));
		gpio_set_pull(gpio_clk, GPIO_PULL_NONE);
		gpio_cfg_pin(gpio_nss, GPIO_FUNC(2));
		gpio_set_pull(gpio_nss, GPIO_PULL_NONE);
		gpio_cfg_pin(gpio_miso, GPIO_FUNC(2));
		gpio_set_pull(gpio_miso, GPIO_PULL_NONE);
		gpio_cfg_pin(gpio_mosi, GPIO_FUNC(2));
		gpio_set_pull(gpio_mosi, GPIO_PULL_NONE);
	} else if (ePort == SPI_PORT1) {
		gpio_clk = s5j_gpio(GPP4, 0);
		gpio_nss = s5j_gpio(GPP4, 1);
		gpio_miso = s5j_gpio(GPP4, 2);
		gpio_mosi = s5j_gpio(GPP4, 3);

		gpio_cfg_pin(gpio_clk, GPIO_FUNC(2));
		gpio_set_pull(gpio_clk, GPIO_PULL_NONE);
		gpio_cfg_pin(gpio_nss, GPIO_FUNC(2));
		gpio_set_pull(gpio_nss, GPIO_PULL_NONE);
		gpio_cfg_pin(gpio_miso, GPIO_FUNC(2));
		gpio_set_pull(gpio_miso, GPIO_PULL_NONE);
		gpio_cfg_pin(gpio_mosi, GPIO_FUNC(2));
		gpio_set_pull(gpio_mosi, GPIO_PULL_NONE);
	} else if (ePort == SPI_PORT2) {
		gpio_clk = s5j_gpio(GPG0, 4);
		gpio_nss = s5j_gpio(GPG0, 5);
		gpio_miso = s5j_gpio(GPG0, 6);
		gpio_mosi = s5j_gpio(GPG0, 7);

		gpio_cfg_pin(gpio_clk, GPIO_FUNC(2));
		gpio_set_pull(gpio_clk, GPIO_PULL_NONE);
		gpio_cfg_pin(gpio_nss, GPIO_FUNC(2));
		gpio_set_pull(gpio_nss, GPIO_PULL_NONE);
		gpio_cfg_pin(gpio_miso, GPIO_FUNC(2));
		gpio_set_pull(gpio_miso, GPIO_PULL_NONE);
		gpio_cfg_pin(gpio_mosi, GPIO_FUNC(2));
		gpio_set_pull(gpio_mosi, GPIO_PULL_NONE);
	} else if (ePort == SPI_PORT3) {
		gpio_clk = s5j_gpio(GPG1, 0);
		gpio_nss = s5j_gpio(GPG1, 1);
		gpio_miso = s5j_gpio(GPG1, 2);
		gpio_mosi = s5j_gpio(GPG1, 3);

		gpio_cfg_pin(gpio_clk, GPIO_FUNC(2));
		gpio_set_pull(gpio_clk, GPIO_PULL_NONE);
		gpio_cfg_pin(gpio_nss, GPIO_FUNC(2));
		gpio_set_pull(gpio_nss, GPIO_PULL_NONE);
		gpio_cfg_pin(gpio_miso, GPIO_FUNC(2));
		gpio_set_pull(gpio_miso, GPIO_PULL_NONE);
		gpio_cfg_pin(gpio_mosi, GPIO_FUNC(2));
		gpio_set_pull(gpio_mosi, GPIO_PULL_NONE);
	} else {
		return;
	}
}

/*****************************************************************************
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
 *****************************************************************************/

static int spi_lock(struct spi_dev_s *dev, bool lock)
{
	struct s5jt200_spidev_s *priv = (struct s5jt200_spidev_s *)dev;
	int result;

	if (lock) {
		while (sem_wait(&priv->exclsem) != 0) {
			DEBUGASSERT(errno == EINTR);
		}
		result = OK;
	} else {
		result = sem_post(&priv->exclsem);
		DEBUGASSERT(result == OK);
	}
	return result;

	return 0;
}

/*****************************************************************************
 * Name: spi_setfrequency
 *
 * Description:
 *   Set the SPI frequency.
 *
 *****************************************************************************/

static uint32_t spi_setfrequency(struct spi_dev_s *dev, uint32_t frequency)
{
	struct s5jt200_spidev_s *priv = (struct s5jt200_spidev_s *)dev;

	switch (priv->port) {
	case 0:
		cal_clk_setrate(d1_spi0, (unsigned long)frequency);
		break;
	case 1:
		cal_clk_setrate(d1_spi1, (unsigned long)frequency);
		break;
	case 2:
		cal_clk_setrate(d1_spi2, (unsigned long)frequency);
		break;
	case 3:
		cal_clk_setrate(d1_spi3, (unsigned long)frequency);
		break;
	}

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
	struct s5jt200_spidev_s *priv = (struct s5jt200_spidev_s *)dev;
	SPI_SFR *pSPIRegs;
	pSPIRegs = (SPI_SFR *)priv->base;
	unsigned int cs_reg;

	cs_reg = Inp32(&pSPIRegs->CS_REG);
	cs_reg = cs_reg | 1;

	if (selected == TRUE) {
		cs_reg = cs_reg & ~1;
	}

	Outp32(&pSPIRegs->CS_REG, cs_reg);
}

void spi_set_initial(struct spi_dev_s *dev)
{
	struct s5jt200_spidev_s *priv = (struct s5jt200_spidev_s *)dev;
	SPI_SFR *pSPIRegs;
	pSPIRegs = (SPI_SFR *)priv->base;

	Outp32(&pSPIRegs->CH_CFG, 0x03);	// TX/RX enable. Master.CPHA 00.
	Outp32(&pSPIRegs->MODE_CFG, 0);	// No FIFO. N0 DMA. 8 bits
	Outp32(&pSPIRegs->CS_REG, 1);	// CS Manual Passive
	Outp32(&pSPIRegs->SPI_INT_EN, 0);	// Disable Interrupts
}

/*****************************************************************************
 * Name: spi_setmode
 *
 * Description:
 *   Set the SPI mode.  see enum spi_mode_e for mode definitions
 *
 *****************************************************************************/

static void spi_setmode(struct spi_dev_s *dev, enum spi_mode_e mode)
{
	struct s5jt200_spidev_s *priv = (struct s5jt200_spidev_s *)dev;
	SPI_SFR *pSPIRegs;
	pSPIRegs = (SPI_SFR *)priv->base;
	unsigned int ch_cfg;

	ch_cfg = Inp32(&pSPIRegs->CH_CFG);
	ch_cfg = (ch_cfg & (~0x000000C0)) | ((unsigned int)mode << 2);
	Outp32(&pSPIRegs->CH_CFG, ch_cfg);
}

inline void spi_fifo_flush(SPI_SFR *pSPIRegs)
{
	unsigned int ch_cfg;

	ch_cfg = Inp32(&pSPIRegs->CH_CFG);
	Outp32(&pSPIRegs->CH_CFG, 1 << 5);
	Outp32(&pSPIRegs->CH_CFG, ch_cfg);
}

/*****************************************************************************
 * Name: spi_setbits
 *
 * Description:
 *   Set the number of bits per word.
 *
 *****************************************************************************/

static void spi_setbits(struct spi_dev_s *dev, int nbits)
{
	struct s5jt200_spidev_s *priv = (struct s5jt200_spidev_s *)dev;
	SPI_SFR *pSPIRegs;
	pSPIRegs = (SPI_SFR *)priv->base;
	unsigned int mode_cfg;

	mode_cfg = Inp32(&pSPIRegs->MODE_CFG);
	mode_cfg = mode_cfg & (~((0x3 << 29) | (0x3 << 17)));

	switch (nbits) {

	case 8:
		mode_cfg = mode_cfg | (0 << 29) | (0 << 17);
		break;
	case 16:
		mode_cfg = mode_cfg | (1 << 29) | (1 << 17);
		break;

	case 32:
		mode_cfg = mode_cfg | (2 << 29) | (2 << 17);
		break;

	case -8:
	case -16:
	case -32:

	default:
		DEBUGASSERT(0 == 1);
	}

	Outp32(&pSPIRegs->MODE_CFG, mode_cfg);
}

/*****************************************************************************
 * Name: spi_status
 *
 * Description:
 *   Get SPI status
 *
 ****************************************************************************/

static uint8_t spi_status(struct spi_dev_s *dev, enum spi_dev_e devid)
{
	while (1) ;					/*STATUS? What status? */

	return SPI_STATUS_PRESENT;
}

/*****************************************************************************
 * Name: spi_send
 *
 * Description:
 *   Exchange one word on SPI
 *
 *****************************************************************************/

static uint16_t spi_send(struct spi_dev_s *dev, uint16_t word)
{
	uint8_t txbyte;
	uint8_t rxbyte;

	txbyte = (uint8_t)word;
	rxbyte = (uint8_t)0;
	spi_exchange(dev, &txbyte, &rxbyte, 1);

	return (uint16_t)rxbyte;
}

/*****************************************************************************
 * Name: spi_exchange
 *
 * Description:
 *   Exchange a block data with the SPI device
 *
 ****************************************************************************/
static void spi_exchange(struct spi_dev_s *dev, const void *txbuffer, void *rxbuffer, size_t nwords)
{
	size_t sent = 0;
	size_t received = 0;
	unsigned int dummy_rx;
	struct s5jt200_spidev_s *priv = (struct s5jt200_spidev_s *)dev;
	SPI_SFR *pSPIRegs;
	pSPIRegs = (SPI_SFR *)priv->base;

	spi_fifo_flush(pSPIRegs);

	if ((rxbuffer == NULL) && (txbuffer == NULL)) {
		while (received < nwords) {
			if (sent < nwords) {
				if (((Inp32(&pSPIRegs->SPI_STATUS) >> 6) & 0x7f) < 64) {
					Outp32(&pSPIRegs->SPI_TX_DATA, 0);
					sent++;
				}
			}

			if (((Inp32(&pSPIRegs->SPI_STATUS) >> 15) & 0x7f) > 0) {
				dummy_rx = Inp32(&pSPIRegs->SPI_RX_DATA);
				received++;
			}
		}
		return;
	}

	if (rxbuffer == NULL) {
		while (received < nwords) {
			if (sent < nwords) {
				if (((Inp32(&pSPIRegs->SPI_STATUS) >> 6) & 0x7f) < 64) {
					Outp32(&pSPIRegs->SPI_TX_DATA, ((unsigned char *)txbuffer)[sent++]);
				}
			}

			if (((Inp32(&pSPIRegs->SPI_STATUS) >> 15) & 0x7f) > 0) {
				dummy_rx = Inp32(&pSPIRegs->SPI_RX_DATA);
				received++;
			}
		}
		return;
	}

	if (txbuffer == NULL) {
		while (received < nwords) {
			if (sent < nwords) {
				if (((Inp32(&pSPIRegs->SPI_STATUS) >> 6) & 0x7f) < 64) {
					Outp32(&pSPIRegs->SPI_TX_DATA, ((unsigned char *)rxbuffer)[sent++]);
				}
			}

			if (((Inp32(&pSPIRegs->SPI_STATUS) >> 15) & 0x7f) > 0) {
				((unsigned char *)rxbuffer)[received++] = Inp32(&pSPIRegs->SPI_RX_DATA);
			}
		}
		return;
	}

	while (received < nwords) {
		if (sent < nwords) {
			if (((Inp32(&pSPIRegs->SPI_STATUS) >> 6) & 0x7f) < 64) {
				Outp32(&pSPIRegs->SPI_TX_DATA, ((unsigned char *)txbuffer)[sent++]);
			}
		}

		if (((Inp32(&pSPIRegs->SPI_STATUS) >> 15) & 0x7f) > 0) {
			((unsigned char *)rxbuffer)[received++] = Inp32(&pSPIRegs->SPI_RX_DATA);
		}
	}
}

/*****************************************************************************
 * Name: spi_sndblock
 *
 * Description:
 *   Send a block of data on SPI
 *
 *****************************************************************************/

static void spi_sndblock(struct spi_dev_s *dev, const void *txbuffer, size_t nwords)
{
	spi_exchange(dev, txbuffer, NULL, nwords);
}

/*****************************************************************************
 * Name: spi_recvblock
 *
 * Description:
 *   Revice a block of data from SPI
 *
 *****************************************************************************/

static void spi_recvblock(struct spi_dev_s *dev, void *rxbuffer, size_t nwords)
{
	spi_exchange(dev, NULL, rxbuffer, nwords);
}

/*****************************************************************************
 * Public Functions
 *****************************************************************************/

/*****************************************************************************
 * Name: up_spiinitialize
 *
 * Description:
 *   Initialize the selected SPI port
 *
 *****************************************************************************/

struct spi_dev_s *up_spiinitialize(int port)
{
	struct s5jt200_spidev_s *priv = NULL;

	if (port >= SPI_PORT_MAX) {
		return NULL;
	}

	priv = &g_spidev[port];
	lldbg("Prepare SPI0 for Master operation\n");
	//spi_init(&gpCtrlInfo);
	/*SET GPIO for the port */
	spi_set_gpio(port, 0xFFFF);

	priv->port = port;

#ifndef CONFIG_SPI_POLLWAIT
	sem_init(&priv->xfrsem, 0, 0);
#endif
#ifndef CONFIG_SPI_OWNBUS
	sem_init(&priv->exclsem, 0, 1);
#endif
	spi_set_initial((struct spi_dev_s *)priv);

	return (struct spi_dev_s *)priv;
}
