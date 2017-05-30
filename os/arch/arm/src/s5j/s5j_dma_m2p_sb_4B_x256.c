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

/****************************************************************************
 * Included Files
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

typedef struct d_dma_task_priv {
	struct dma_task task;
	int mc_array_size;
	int mc_size;
	int chflags;
	void *SAR;
	void *DAR;
	void *CCR;
	void *LOOP1;
	void *LOOP0;
	void *EVENT_CH;
} t_dma_task_priv;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: s5j_dma_priv_setup
 *
 * Description:
 *   Private method to prepare mocrocode for DMA M2P operation.
 *
 * Input Parameters
 *  handle - DMA channel handle structure;
 *  task - DMA task structure with microcode to execute
 *
 * Returned Value:
 *  OK;
 *
 ****************************************************************************/
static int s5j_dma_priv_setup(DMA_HANDLE handle, dma_task *task)
{
	t_dma_task_priv *priv_task = (t_dma_task_priv *) task;
	DMA_CH_CONTEXT *ch;
	ch = handle;

	priv_task->chflags = CCR_M2P_DFLT |
						 SRC_BURST_SIZE(BS_1) |
						 DST_BURST_SIZE(BS_4);

	DMA_MC_4B_SET(priv_task->SAR, task->src);
	DMA_MC_4B_SET(priv_task->DAR, task->dst);
	DMA_MC_4B_SET(priv_task->CCR, priv_task->chflags);
	DMA_MC_1B_SET(priv_task->LOOP1, (((task->size / 4 - 1) >> 8) & 0xFF));
	DMA_MC_1B_SET(priv_task->LOOP0, (task->size / 4 - 1) & 0xFF);

	DMA_MC_EV_SET(priv_task->EVENT_CH, ch->dma_chan_num);

	arch_clean_dcache((uintptr_t)task->microcode,
					  (uintptr_t)(task->microcode + priv_task->mc_size));

	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: dma_task_m2p_sb_4B_x256_alloc
 *
 * Description:
 *   This function allocated task structure for M2P DMA transfers microcode.
 *   It prepares microcode and place holders for loop counters and addresses.
 *
 * Input Parameters
 *   d_ph_ch - destination peripheral DMA shannel number
 *
 * Returned Value:
 *   Pointer on dma_task structure
 *
 ****************************************************************************/
dma_task *dma_task_m2p_sb_4B_x256_alloc(DMA_REQ_MAP d_ph_ch)
{
	dma_task *task;
	t_dma_task_priv *priv_task;
	void *mc_base;
	void *loop_offs0;
	void *loop_offs1;

	task = zalloc(sizeof(t_dma_task_priv));
	if (task == NULL) {
		dmadbg("ERROR: Failed to allocate microcode memory\n");
		return NULL;
	}

	task->microcode = zalloc(64);
	if (task->microcode == NULL) {
		dmadbg("ERROR: Failed to allocate microcode memory\n");
		free(task);
		return NULL;
	}

	priv_task = (t_dma_task_priv *) task;
	priv_task->mc_array_size = 64;

	mc_base = task->microcode;

	mc_base += DMA_Encode_DMAMOV(mc_base, &(priv_task->SAR), DMA_SAR);
	mc_base += DMA_Encode_DMAMOV(mc_base, &(priv_task->DAR), DMA_DAR);
	mc_base += DMA_Encode_DMAMOV(mc_base, &(priv_task->CCR), DMA_CCR);

	mc_base += DMA_Encode_DMAFLUSHP(mc_base, d_ph_ch);

	/*loop start */
	mc_base += DMA_Encode_DMALP(mc_base, LC1, &(priv_task->LOOP1));
	loop_offs1 = mc_base;
	mc_base += DMA_Encode_DMALP(mc_base, LC0, &(priv_task->LOOP0));
	loop_offs0 = mc_base;

	/* 0 - wait for either Sinnle/Busrs requests */
	mc_base += DMA_Encode_DMAWFP(mc_base, 1, d_ph_ch);

	/* 0 - LD does not depend on burst L/S settings */
	mc_base += DMA_Encode_DMALD(mc_base, 0);
	mc_base += DMA_Encode_DMALD(mc_base, 0);
	mc_base += DMA_Encode_DMALD(mc_base, 0);
	mc_base += DMA_Encode_DMALD(mc_base, 0);

	/* 0 - STP singles / NOP burst */
	mc_base += DMA_Encode_DMASTP(mc_base, 0, d_ph_ch);

	/* 2 - NOP singles / STP burst */
	mc_base += DMA_Encode_DMASTP(mc_base, 2, d_ph_ch);

	mc_base += DMA_Encode_DMAFLUSHP(mc_base, d_ph_ch);

	/*loop end */
	mc_base += DMA_Encode_DMALPEND(mc_base, LC0_END, mc_base - loop_offs0);
	mc_base += DMA_Encode_DMALPEND(mc_base, LC1_END, mc_base - loop_offs1);

	mc_base += DMA_Encode_DMAWMB(mc_base);
	mc_base += DMA_Encode_DMASEV(mc_base, &(priv_task->EVENT_CH));
	mc_base += DMA_Encode_DMAEND(mc_base);

	priv_task->mc_size = mc_base - task->microcode;

	task->setup = s5j_dma_priv_setup;
	return task;
}
