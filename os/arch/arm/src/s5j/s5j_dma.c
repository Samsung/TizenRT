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

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <errno.h>
#include <assert.h>
#include <queue.h>
#include <debug.h>

#include <arch/board/board.h>

#include <tinyara/irq.h>
#include <tinyara/arch.h>
#include <tinyara/kmalloc.h>
#include <tinyara/wdog.h>
#include <tinyara/wqueue.h>

#include "up_internal.h"
#include "up_arch.h"
#include "cache.h"

#include "chip.h"
#include "s5j_dma.h"

/****************************************************************************
 * Private Types
 ****************************************************************************/
int pdma_irq_handler(int irq, FAR void *context, FAR void *arg);

static struct dma_drvdata dma_dev[] = {
	[0] = {
		.name = "pdma",
		.isr_num = IRQ_PDMA_U1,	/* IRQ_PDMA, */
		.isr_handler = pdma_irq_handler,
		.base = (void *)S5J_PDMA_BASE,
	},
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static u32 dma_get_intstatus(struct dma_drvdata *dma)
{
	return getreg32(dma->base + (u32)(DMAC_INTMIS));
}

static void dma_enable_ch_interrupt(struct dma_drvdata *dma, DMA_CHANNEL_NUM ch_num)
{
	volatile u32 inten_reg;
	inten_reg = getreg32(dma->base + (u32)(DMAC_INTEN));
	putreg32((inten_reg | (1 << ch_num)), (dma->base + (u32) DMAC_INTEN));
}

static void dma_disable_ch_interrupt(struct dma_drvdata *dma, DMA_CHANNEL_NUM ch_num)
{
	volatile u32 inten_reg;
	inten_reg = getreg32(dma->base + (u32)(DMAC_INTEN));
	putreg32((inten_reg & ~(1 << ch_num)), (dma->base + (u32) DMAC_INTEN));
}

static u32 dma_clear_ch_interrupt(struct dma_drvdata *dma, DMA_CHANNEL_NUM ch_num)
{

	volatile u32 reg = getreg32(dma->base + (u32)(DMAC_INTCLR));
	putreg32((reg | (1 << ch_num)), (dma->base + (u32) DMAC_INTCLR));
	return getreg32(dma->base + (u32)(DMAC_INTCLR));
}

static void dma_ch_kill(DMA_CH_CONTEXT *dma_ch)
{
	struct dma_drvdata *dma = dma_ch->dma;
	int debug_inst0;

	/* Debug status busy?  */
	while ((getreg32(dma->base + DMAC_DBG_STATUS) & 0x1)) ;

	debug_inst0 = (0 << 24) | ((0x01) << 16) | (dma_ch->dma_chan_num << 8) | (1 << 0);

	putreg32(debug_inst0, dma->base + (u32) DMAC_DBG_INTST0);
	putreg32(0, dma->base + DMAC_DBG_CMD);
}

int pdma_irq_handler(int irq, FAR void *context, FAR void *arg)
{

	struct dma_drvdata *dma = (struct dma_drvdata *)arg;
	u32 intstatus;
	int i;
	volatile u32 reg;

	intstatus = dma_get_intstatus(dma);

	for (i = 0; i < dma->max_ch_num; i++) {
		if ((intstatus & (1 << i)) && (dma->dma_ch[i].task->callback)) {
			dma_clear_ch_interrupt(dma, i);
			dma_disable_ch_interrupt(dma, i);
			dma->dma_ch[i].task->callback(&dma->dma_ch[i], dma->dma_ch[i].task->arg, 0);
		}
	}

	/* Check faults here */
	reg = getreg32(dma->base + (u32)(DMAC_FSRC));
	if (reg) {
		for (i = 0; i < dma->max_ch_num; i++) {
			if (reg & (1 << i)) {
				dmalldbg("\nChannel %d failt FTRD = 0x%x CH_FTR = 0x%x \n", i, getreg32(dma->base + (u32)(DMAC_FTRD)), getreg32(dma->base + (u32)(DMAC_CH_FTR(i))));

				dmalldbg("- CSR 0x%x, CPC 0x%x, SAR 0x%x, DAR 0x%x,\
							CCR 0x%x, LC0 0x%x, LC1 0x%x\n", getreg32(dma->base + (u32)(DMAC_CH_CSR(i))), getreg32(dma->base + (u32)(DMAC_CH_CPC(i))), getreg32(dma->base + (u32)(DMAC_CH_SAR(i))), getreg32(dma->base + (u32)(DMAC_CH_DAR(i))), getreg32(dma->base + (u32)(DMAC_CH_CCR(i))), getreg32(dma->base + (u32)(DMAC_CH_LC0(i))), getreg32(dma->base + (u32)(DMAC_CH_LC1(i))));

				dmalldbg("- INTEN 0x%x, RIS 0x%x, INTMIS 0x%x,\
							FSRD 0x%x, FSRC 0x%x, FTRD 0x%x \n", getreg32(dma->base + (u32)(DMAC_INTEN)), getreg32(dma->base + (u32)(DMAC_INT_EVENT_RIS)), getreg32(dma->base + (u32)(DMAC_INTMIS)), getreg32(dma->base + (u32)(DMAC_FSRD)), getreg32(dma->base + (u32)(DMAC_FSRC)), getreg32(dma->base + (u32)(DMAC_FTRD)));

				dma_ch_kill(&dma->dma_ch[i]);
				dma_disable_ch_interrupt(dma, i);
				if (dma->dma_ch[i].task->callback) {
					dma->dma_ch[i].task->callback(&dma->dma_ch[i], dma->dma_ch[i].task->arg, -1);
				}

			}
		}
	}
	return 0;
}

static struct dma_drvdata *get_dma_drvdata(char *name)
{
	int i;

	i = sizeof(dma_dev) / sizeof(struct dma_drvdata);

	while (i--)
		if (strcmp(name, dma_dev[i].name) == 0) {
			return &dma_dev[i];
		}
	return NULL;
}

static void dma_set_max_params(struct dma_drvdata *dma)
{
	volatile u32 reg_val;

	reg_val = getreg32(dma->base + (u32)(DMAC_CR0));
	dma->max_ch_num = ((reg_val >> 4) & 0x7) + 1;

	reg_val = getreg32(dma->base + (u32)(DMAC_CRD));
	dma->max_burst_size = ((reg_val) & 0x7) + 1;

	reg_val = getreg32(dma->base + (u32)(DMAC_CRD));
	dma->max_burst_len = ((reg_val >> 20) & 0x1FF) + 1;
}

static void dma_init(struct dma_drvdata *dma)
{
	int i;
	/*  INIT SOMETHING HERE before everything starts  */
	dma_set_max_params(dma);
	irq_attach(dma->isr_num, dma->isr_handler, dma);
	up_enable_irq(dma->isr_num);

	for (i = 0; i < 8; i++) {
		dma->dma_ch[i].dma = dma;
		dma->dma_ch[i].dma_chan_num = i;
		dma->dma_ch[i].channel_assigned = 0;
	}

	dmavdbg("DMA %s is configured\n", dma->name);
}

static void dma_ch_exec(DMA_CH_CONTEXT *dma_ch)
{
	struct dma_drvdata *dma = dma_ch->dma;
	int secure_status;
	int debug_inst0;

	while ((getreg32(dma->base + DMAC_DBG_STATUS) & 0x1)) ;	/* Debug status busy?  */

	secure_status = ((getreg32(dma->base + DMAC_CR0) >> 2) & 0x1);
	debug_inst0 = (dma_ch->dma_chan_num << 24) | ((0xA0 | secure_status << 1) << 16) | (dma_ch->dma_chan_num << 8) | (0 << 0);

	putreg32(debug_inst0, dma->base + (u32) DMAC_DBG_INTST0);
	putreg32((int)(dma_ch->task->microcode), dma->base + DMAC_DBG_INTST1);
	putreg32(0, dma->base + DMAC_DBG_CMD);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: s5j_dma_alloc
 *
 * Description:
 *   Allocate first avaliable DMA channel in DMA controller
 *
 * Input Parameters:
 *   name - Name of DMA controller
 *
 * Returned Value:
 *   allocted DMA channel handler, or NULL;
 *
 ****************************************************************************/
DMA_HANDLE s5j_dma_alloc(char *name)
{
	int i;
	struct dma_drvdata *dma;

	dma = get_dma_drvdata(name);

	if (dma == NULL) {
		return NULL;
	}

	for (i = 0; i < dma->max_ch_num; i++) {
		if (dma->dma_ch[i].channel_assigned == 0) {
			dma->dma_ch[i].channel_assigned = 1;
			return (DMA_HANDLE) & dma->dma_ch[i];
		}
	}
	return NULL;
}

/****************************************************************************
 * Name: s5j_dmachannel
 *
 * Description:
 *   Allocate specified DMA channel in DMA controller
 *
 * Input Parameters:
 *   dmacno - DMA channel number
 *   name - Name of DMA controller
 *
 * Returned Value:
 *   allocted DMA channel handler, or NULL;
 *
 ****************************************************************************/
DMA_HANDLE s5j_dmachannel(uint8_t dmacno, char *name)
{
	struct dma_drvdata *dma;

	dma = get_dma_drvdata(name);

	if (dma == NULL) {
		return NULL;
	}

	if (dmacno >= dma->max_ch_num) {
		return NULL;
	}

	if (dma->dma_ch[dmacno].channel_assigned != 0) {
		return NULL;
	}

	dma->dma_ch[dmacno].channel_assigned = 1;
	return (DMA_HANDLE) & dma->dma_ch[dmacno];
}

/****************************************************************************
 * Name: s5j_dmafree
 *
 * Description:
 *   Release Allocated DMA channel
 *
 * Input Parameters:
 *  handle - DMA handler
 *
 * Returned Value:
 *   none
 *
 ****************************************************************************/
void s5j_dmafree(DMA_HANDLE handle)
{
	DMA_CH_CONTEXT *ch;
	ch = (DMA_CH_CONTEXT *) handle;
	ch->channel_assigned = 0;
	ch->task = NULL;
	return;
}

/****************************************************************************
 * Name: s5j_dmastart
 *
 * Description:
 *   Start DMA operation
 *
 * Input Parameters:
 *  handle - DMA handler
 *  task - DMA task structure with microcode
 *
 * Returned Value:
 *   OK
 *
 ****************************************************************************/
int s5j_dmastart(DMA_HANDLE handle, dma_task *task)
{
	DMA_CH_CONTEXT *ch;
	ch = handle;

	dmavdbg("DMA ch %d start\n", ch->dma_chan_num);

	/* Somwhere here run DMA */
	ch->task = task;

	/* FIXME Here I should put task in DMA queue and start DMA */
	if (ch->task->callback) {
		dma_enable_ch_interrupt(ch->dma, ch->dma_chan_num);
	}
	dma_ch_exec(ch);

	return OK;
}

/****************************************************************************
 * Name: s5j_dmastop
 *
 * Description:
 *   Stop DMA operation
 *
 * Input Parameters:
 *  handle - DMA handler
 *
 * Returned Value:
 *  task structure, which was in prosess.
 *
 ****************************************************************************/
dma_task *s5j_dmastop(DMA_HANDLE handle)
{
	DMA_CH_CONTEXT *ch;
	dma_task *task;

	ch = handle;

	dma_ch_kill(ch);
	dma_disable_ch_interrupt(ch->dma, ch->dma_chan_num);

	task = ch->task;
	ch->task = NULL;

	return task;
}

/****************************************************************************
 * Name: s5j_dmatask_free
 *
 * Description:
 *   Free DMA task structure and microcode.
 *
 * Input Parameters:
 *  task srtucture to free
 *
 * Returned Value:
 *  OK
 *
 ****************************************************************************/
int s5j_dmatask_free(dma_task *task)
{
	free(task->microcode);
	free(task);
	return OK;
}

/****************************************************************************
 * Name: up_dmainitialize
 *
 * Description:
 *   Initialize the DMA subsystem
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void weak_function up_dmainitialize(void)
{
	struct dma_drvdata *dma;
	dmavdbg("Initialize PDMA 0\n");
	dma = get_dma_drvdata("pdma");

	if (dma != NULL) {
		dma_init(dma);
	} else {
		dmavdbg("There is NO PDMA 0\n");
	}
}

/****************************************************************************
 * Name: dma_clean_dcache
 *
 * Description:
 *   User application clean cache, used with data before transfer;
 *
 * Input Parameters:
 *  start - pointer on first byte to clean
 *  end - pointer on last byte to clean
 *
 * Returned Value:
 *  none
 *
 ****************************************************************************/
void dma_clean_dcache(void *start, void *end)
{
	arch_clean_dcache((uintptr_t) start, (uintptr_t) end);
}

/****************************************************************************
 * Name: dma_invalidate_dcache
 *
 * Description:
 *   User application invalidate cache, used with data before transfer;
 *
 * Input Parameters:
 *  start - pointer on first byte to clean
 *  end - pointer on last byte to clean
 *
 * Returned Value:
 *  none
 *
 ****************************************************************************/
void dma_invalidate_dcache(void *start, void *end)
{
	arch_invalidate_dcache((uintptr_t) start, (uintptr_t) end);
}
