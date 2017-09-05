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
#ifndef __ARCH_ARM_INCLUDE_S5J_DMA_H
#define __ARCH_ARM_INCLUDE_S5J_DMA_H

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define s5j_dmasetup(a, b)		\
do {					\
	if ((b)->setup) {		\
		(b)->setup(a, b);	\
	}				\
} while (0)

/************************************************************************************
 * Public Types
 ************************************************************************************/

typedef enum {
	DMA_UART0_RX = (0),
	DMA_UART0_TX = (1),
	DMA_UART1_RX = (2),
	DMA_UART1_TX = (3),
	DMA_UART2_RX = (4),
	DMA_UART2_TX = (5),
	DMA_UART3_RX = (6),
	DMA_UART3_TX = (7),
	DMA_HSI2C0_RX = (8),
	DMA_HSI2C0_TX = (9),
	DMA_HSI2C1_RX = (10),
	DMA_HSI2C1_TX = (11),
	DMA_HSI2C2_RX = (12),
	DMA_HSI2C2_TX = (13),
	DMA_HSI2C3_RX = (14),
	DMA_HSI2C3_TX = (15),
	DMA_SPI0_RX = (16),
	DMA_SPI0_TX = (17),
	DMA_SPI1_RX = (18),
	DMA_SPI1_TX = (19),
	DMA_SPI2_RX = (20),
	DMA_SPI2_TX = (21),
	DMA_SPI3_RX = (22),
	DMA_SPI3_TX = (23),
	DMA_I2S_RX = (24),
	DMA_I2S_TX = (25),
	DMA_I2S_TX_S = (26),
	DMA_UART_Debug_RX = (28),
	DMA_UART_Debug_TX = (29),
	DMA_PWM0 = (30),
	DMA_PWM1 = (31),
} DMA_REQ_MAP;

typedef FAR void *DMA_HANDLE;
typedef struct dma_task dma_task;

typedef void (*dma_callback_t)(DMA_HANDLE handle, void *arg, int result);
typedef int (*dma_setup_t)(DMA_HANDLE handle, dma_task *task);

struct dma_task {
	void *src;
	void *dst;
	u32 size;
	void *microcode;

	dma_setup_t setup;

	dma_callback_t callback;
	void *arg;
};

/************************************************************************************
 * Public Functions
 ************************************************************************************/
DMA_HANDLE s5j_dma_alloc(char *name);
DMA_HANDLE s5j_dmachannel(uint8_t dmacno, char *name);
void s5j_dmafree(DMA_HANDLE handle);

dma_task *dma_task_m2m_sb_4B_x256_alloc(void);
dma_task *dma_task_m2p_sb_4B_x256_alloc(DMA_REQ_MAP d_ph_ch);
dma_task *dma_task_p2m_sb_4B_x256_alloc(DMA_REQ_MAP d_ph_ch);
dma_task *dma_task_m2mp_sb_4B_x256_alloc(void);
dma_task *dma_task_mp2m_sb_4B_x256_alloc(void);

int s5j_dmatask_free(dma_task *task);

int s5j_dmastart(DMA_HANDLE handle, dma_task *task);
dma_task *s5j_dmastop(DMA_HANDLE handle);

void dma_clean_dcache(void *start, void *end);
void dma_invalidate_dcache(void *start, void *end);

#endif
