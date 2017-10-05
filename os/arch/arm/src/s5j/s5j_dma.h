/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

#ifndef __ARCH_ARM_SRC_S5J_DMA_H
#define __ARCH_ARM_SRC_S5J_DMA_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/compiler.h>

#include <stdint.h>
#include <stdbool.h>
#include <debug.h>

#include <tinyara/irq.h>

#include <arch/s5j/dma.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define DMAC_MAX_CH                  8

#define DMAC_DSR                     (0x000)
#define DMAC_DPC                     (0x004)

#define DMAC_INTEN                   (0x020)
#define DMAC_INT_EVENT_RIS           (0x024)
#define DMAC_INTMIS                  (0x028)
#define DMAC_INTCLR                  (0x02C)
#define DMAC_FSRD                    (0x030)
#define DMAC_FSRC                    (0x034)
#define DMAC_FTRD                    (0x038)

#define DMAC_CH_FTR(n)               0x040 + 0x04 * (n)
#define DMAC_CH_CSR(n)               (u32)(0x100 + 0x08 * (n))
#define DMAC_CH_CPC(n)               0x104 + 0x08 * (n)
#define DMAC_CH_SAR(n)               0x400 + 0x20 * (n)
#define DMAC_CH_DAR(n)               0x404 + 0x20 * (n)
#define DMAC_CH_CCR(n)               0x408 + 0x20 * (n)
#define DMAC_CH_LC0(n)               0x40C + 0x20 * (n)
#define DMAC_CH_LC1(n)               0x410 + 0x20 * (n)

#define DMAC_DBG_STATUS              0xD00
#define DMAC_DBG_CMD                 0xD04
#define DMAC_DBG_INTST0              0xD08
#define DMAC_DBG_INTST1              0xD0C

#define DMAC_CR0                     0xE00
#define DMAC_CR1                     0xE04
#define DMAC_CR2                     0xE08
#define DMAC_CR3                     0xE0C
#define DMAC_CR4                     0xE10
#define DMAC_CRD                     0xE14
#define DMAC_WD                      0xE80

#define DMA_BYTE(inst)               (u8)((inst) & 0xFF)
#define DMA_LOOP_MAX                 256

/* CCR BITFIELDS */

#define NOSWAP                  (0)
#define ENDIAN_SWAP_SIZE_MASK   (7 << 28)
#define ENDIAN_SWAP_SIZE(x)     ((x & 7) << 28)

#define AWCACHE0            (1)
#define AWCACHE1            (2)
#define AWCACHE2            (4)
#define DST_CACHE_CTRL_MASK (7 << 25)
#define DST_CACHE_CTRL(x)   ((x & 7) << 25)

#define AWPROT0             (1)
#define AWPROT1             (2)
#define AWPROT2             (4)
#define DST_PROT_CTRL_MASK  (7 << 22)
#define DST_PROT_CTRL(x)    ((x & 7) << 22)

/* Burst Len absolute value */
#define DST_BURST_LEN_MASK  (0xf << 18)
#define DST_BURST_LEN(x)    (((x - 1) & 0xf) << 18)

#define BS_1                0
#define BS_2                1
#define BS_4                2
#define BS_8                3
#define DST_BURST_SIZE_MASK (7 << 15)
#define DST_BURST_SIZE(x)   ((x & 7) << 15)

#define DST_INC             (1 << 14)

#define ARCACHE0            (1)
#define ARCACHE1            (2)
#define ARCACHE2            (4)
#define SRC_CACHE_CTRL_MASK (7 << 11)
#define SRC_CACHE_CTRL(x)   ((x & 7) << 11)

#define ARPROT0             (1)
#define ARPROT1             (2)
#define ARPROT2             (4)
#define SRC_PROT_CTRL_MASK  (7 << 8)
#define SRC_PROT_CTRL(x)    ((x & 7) << 8)

/* Burst Len absolute value */
#define SRC_BURST_LENGTH_MASK   (0xf << 4)
#define SRC_BURST_LENGTH(x)     (((x - 1) & 0xf) << 4)

/* Burst Size absolute value */
#define SRC_BURST_SIZE_MASK     (7 << 1)
#define SRC_BURST_SIZE(x)       ((x & 7) << 1)

#define SRC_INC     (1 << 0)

#define LC0         0x0
#define LC0_END     0x10
#define LC1         0x2
#define LC1_END     0x14

#define CCR_M2M_DFLT (ENDIAN_SWAP_SIZE(NOSWAP) | \
			DST_CACHE_CTRL(AWCACHE1) | \
			DST_INC | \
			SRC_CACHE_CTRL(ARCACHE1) | \
			SRC_INC | \
			SRC_PROT_CTRL(2) | \
			DST_PROT_CTRL(2))

#define CCR_P2P_DFLT (ENDIAN_SWAP_SIZE(NOSWAP) | \
			DST_CACHE_CTRL(AWCACHE1) | \
			SRC_CACHE_CTRL(ARCACHE1))

#define CCR_P2M_DFLT (ENDIAN_SWAP_SIZE(NOSWAP) | \
			DST_CACHE_CTRL(AWCACHE1) | \
			DST_INC | \
			SRC_CACHE_CTRL(ARCACHE1) | \
			SRC_PROT_CTRL(2) | \
			DST_PROT_CTRL(2))

#define CCR_M2P_DFLT (ENDIAN_SWAP_SIZE(NOSWAP) | \
			DST_CACHE_CTRL(AWCACHE1) | \
			SRC_CACHE_CTRL(ARCACHE1) | \
			SRC_INC | \
			SRC_PROT_CTRL(2) | \
			DST_PROT_CTRL(2))

#define CCR_M2MP_DFLT (ENDIAN_SWAP_SIZE(NOSWAP) | \
			DST_CACHE_CTRL(AWCACHE1) | \
			SRC_CACHE_CTRL(ARCACHE1) | \
			SRC_PROT_CTRL(2) | \
			DST_PROT_CTRL(2))

#define DMA_MC_4B_SET(a, b)     (*(unsigned int *)(a) = (unsigned int)(b))
#define DMA_MC_1B_SET(a, b)     (*(char *)(a) = (char)(b))
#define DMA_MC_EV_SET(a, b)     (*(char *)(a) = (char)(b << 3))

#define DMA_NOP		(0x18)

/************************************************************************************
 * Public Types
 ************************************************************************************/

typedef enum {
	DMA_SAR = 0,
	DMA_CCR,
	DMA_DAR,
} DMA_INST_REG;

typedef enum {
	DMA_LC0 = 0,
	DMA_LC1,
} DMA_LC;

typedef enum {
	DMA_CH0 = 0x0,
	DMA_CH1,
	DMA_CH2,
	DMA_CH3,
	DMA_CH4,
	DMA_CH5,
	DMA_CH6,
	DMA_CH7
} DMA_CHANNEL_NUM;

typedef struct dma_drvdata dma_drvdata;

typedef struct {
	dma_drvdata *dma;
	DMA_CHANNEL_NUM dma_chan_num;
	u8 channel_assigned;

	dma_task *task;

} DMA_CH_CONTEXT;

struct dma_drvdata {
	char *name;
	void *base;
	int isr_num;
	xcpt_t isr_handler;

	/* struct dma_device   *dev; */

	/* Add private data structures if it is needed */
	u32 max_ch_num;
	u32 max_burst_len;
	u32 max_burst_size;			/*  data_size */
	/*              bool dma_initialized; */
	DMA_CH_CONTEXT dma_ch[DMAC_MAX_CH];
};

typedef enum {
	m2m,
	m2p,
	p2m,
	p2p,
} mc_tmplt;

/************************************************************************************
 * Inline Functions
 ************************************************************************************/

/* Microcode functions */
static inline int DMA_Encode_DMAMOV(char *base, void **reg_addr, DMA_INST_REG rd)
{
	rd &= 0x07;

	base[0] = 0xBC;
	base[1] = rd;
	base[2] = 0xd;
	base[3] = 0xe;
	base[4] = 0xa;
	base[5] = 0xd;

	*reg_addr = base + 2;
	return 6;
}

static inline int DMA_Encode_DMALP(char *base, DMA_LC lc, void **iter)
{

	base[0] = 0x20 | lc;
	base[1] = 0xe;

	*iter = base + 1;
	return 2;
}

static inline int DMA_Encode_DMALPEND(char *base, u8 CTRL, char backwards_jump)
{
	base[0] = 0x28 | CTRL;
	base[1] = backwards_jump;
	return 2;
}

static inline int DMA_Encode_DMALD(char *base, u8 CTRL)
{
	base[0] = 0x04 | CTRL;
	return 1;
}

static inline int DMA_Encode_DMALDP(char *base, u8 CTRL, DMA_REQ_MAP periph)
{
	base[0] = 0x25 | CTRL;
	base[1] = periph << 3;
	return 2;
}

static inline int DMA_Encode_DMAST(char *base, u8 CTRL)
{
	base[0] = 0x08 | CTRL;
	return 1;
}

static inline int DMA_Encode_DMASTP(char *base, u8 CTRL, DMA_REQ_MAP periph)
{
	base[0] = 0x29 | CTRL;
	base[1] = periph << 3;
	return 2;
}

static inline int DMA_Encode_DMAWFP(char *base, u8 CTRL, DMA_REQ_MAP periph)
{
	base[0] = 0x30 | CTRL;
	base[1] = periph << 3;
	return 2;
}

static inline int DMA_Encode_DMASEV(char *base, void **event_num)
{
	base[0] = 0x34;
	base[1] = 0xe;
	*event_num = base + 1;
	return 2;
}

static inline int DMA_Encode_DMAEND(char *base)
{
	base[0] = 0x00;
	return 1;
}

static inline int DMA_Encode_DMAFLUSHP(char *base, u8 periph)
{
	base[0] = 0x35;
	base[1] = periph << 3;
	return 2;
}

static inline int DMA_Encode_DMAWMB(char *base)
{
	base[0] = 0x13;
	return 1;
}

#endif							/* __ARCH_ARM_SRC_S5J_DMA_H */
