/*
 * Copyright (c) 2015, Realtek Semiconductor Corp. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SPI_SHEIPA_H
#define SPI_SHEIPA_H

/* SPIC flash base */
#define SPIC_BASE		0x1fb09000
#define SPIC_SIZE		0x200
#define FLASH_BASE		0x14000000
#define FLASH_SIZE		0x100000
#define FLASH_FIP_OFFSET	0x100000
/* SPIC max slaves is 16(hw fixed value) */
#define MAX_SLAVES		16
/* FLASH base address for auto mode */
#define	FIFO_SIZE		64
#define	FIFO_HALF_SIZE	(FIFO_SIZE / 2)
/* Indicate num of data frames for read */
#define MAX_NUM_FRAME	0x10000

/* Soc tunning dummy cycle only. */
#define DEF_RD_TUNING_DUMMY_CYCLE  0x0
/* General flash opcode. */
#define WRSR		0x01
#define PP		0x02
#define NORM_READ	0x03
#define WRDI		0x04	/* write disable */
#define RDSR		0x05
#define WREN		0x06
#define FAST_READ       0x0b
#define RDID		0x9f
#define CE		0xc7	/* chip erase */
#define BE_4K		0x20	/* erase 4KiB Block */
#define SE		0xd8	/* sector erase(usually 64KiB) */


/*
 *  Used in conjunction with bitops.h to access register bitfields.
 *  They are defined as bit offset/mask pairs for each DMA register
 *  bitfield.
 */
#define BFO_SPI_FLASH_SR_BUSY                ((uint32_t)    0)
#define BFO_SPI_FLASH_SR_TXE                 ((uint32_t)    5)
#define BFO_SPI_FLASH_AUTO_LEN_DUM           ((uint32_t)     0)
#define BFW_SPI_FLASH_AUTO_LEN_DUM           ((uint32_t)    16)


/* Define macro */
#define DW_BIT_MASK_WIDTH(__bfws, __bits) ((uint32_t)((__bfws) == 32) ?  \
		0xFFFFFFFF : (((1 << (__bits)) - 1) << (__bfws)))
#define DW_BITS_SET_VAL(__datum, __bfws, __val, bit_num)	\
	((__datum) = ((uint32_t)(__datum) &	\
			~DW_BIT_MASK_WIDTH(__bfws, bit_num)) |	\
		((__val << (__bfws)) & DW_BIT_MASK_WIDTH(__bfws, bit_num)))

struct sheipa_spi {
	void	*regs;
	void	*auto_regs;
};

/*
 * This is the structure used for accessing the spi_flash register
 * portmap.
 */
struct spi_flash_portmap {
/* Channel registers
 * The offset address for each of the channel registers
 * is shown for channel 0. For other channel numbers
 * use the following equation.
 * offset = (channel_num * 0x058) + channel_0 offset
 */
	struct {
		volatile uint32_t ctrlr0;	/* Control Reg 0 (0x000) */
		volatile uint32_t ctrlr1;
		volatile uint32_t ssienr;	/* SPIC enable Reg1 (0x008) */
		volatile uint32_t mwcr;
		volatile uint32_t ser;		/* Slave enable Reg (0x010) */
		volatile uint32_t baudr;
		volatile uint32_t txftlr;	/* TX_FIFO th level (0x018) */
		volatile uint32_t rxftlr;
		volatile uint32_t txflr;	/* TX_FIFO th level (0x020) */
		volatile uint32_t rxflr;
		volatile uint32_t sr;		/* Dest Status Reg  (0x028) */
		volatile uint32_t imr;
		volatile uint32_t isr;		/* Intr Stauts Reg (0x030) */
		volatile uint32_t risr;
		volatile uint32_t txoicr;	/* TX_FIFO overflow (0x038) */
		volatile uint32_t rxoicr;
		volatile uint32_t rxuicr;	/* RX_FIFO underflow (0x040) */
		volatile uint32_t msticr;
		volatile uint32_t icr;		/* Intr clear Reg (0x048) */
		volatile uint32_t dmacr;
		volatile uint32_t dmatdlr;	/* DMA TX_data level (0x050) */
		volatile uint32_t dmardlr;
		volatile uint32_t idr;		/* Identify Scatter (0x058) */
		volatile uint32_t spi_flash_version;
		union{
			volatile uint8_t  byte;
			volatile uint16_t half;
			volatile uint32_t word;
		} dr[32];
		volatile uint32_t rd_fast_single;
		volatile uint32_t rd_dual_o;	/* Read dual data (0x0e4) */
		volatile uint32_t rd_dual_io;
		volatile uint32_t rd_quad_o;	/* Read quad data (0x0ec) */
		volatile uint32_t rd_quad_io;
		volatile uint32_t wr_single;	/* write single (0x0f4) */
		volatile uint32_t wr_dual_i;
		volatile uint32_t wr_dual_ii;	/* write dual addr/data(0x0fc)*/
		volatile uint32_t wr_quad_i;
		volatile uint32_t wr_quad_ii;	/* write quad addr/data(0x104)*/
		volatile uint32_t wr_enable;
		volatile uint32_t rd_status;	/* read status (0x10c) */
		volatile uint32_t ctrlr2;
		volatile uint32_t fbaudr;	/* fast baud rate (0x114) */
		volatile uint32_t addr_length;
		volatile uint32_t auto_length;	/* Auto addr length (0x11c) */
		volatile uint32_t valid_cmd;
		volatile uint32_t flash_size;	/* Flash size (0x124) */
		volatile uint32_t flush_fifo;
	};
};

struct spi_flash_param {
	uint32_t num_slaves;	/* slaves number */
	uint32_t fifo_size;	/* TX fifo depth number */
	uint32_t baudr_div;
};
#endif
