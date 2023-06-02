/*****************************************************************************
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

/* Uses */

#include <stdio.h>
#include "scsc_mif_abs.h"
#include "utils_misc.h"
#include <errno.h>
#include <tinyara/irq.h>
/* Implements */
#include "mifintrbit.h"

/* default handler just logs a warning and clears the bit */
static void mifintrbit_default_handler(int irq, void *data)
{
	struct mifintrbit *intr = (struct mifintrbit *)data;
	unsigned long flags;

	spin_lock_enter_critical_section(&intr->spinlock, flags);
	intr->mif->irq_bit_clear(intr->mif, irq);
	spin_unlock_leave_critical_section(&intr->spinlock, flags);
}

static void mifiintrman_isr(int irq, void *data)
{
	struct mifintrbit *intr = (struct mifintrbit *)data;
	unsigned long flags;
	int irq_reg[1] = { 0 };;
	int bit;

	/* Avoid unused parameter error */
	(void)irq;

#define MITIGATE_INTERRUPT_ISSUE	/* Temporary fix to mitigate an issue of interrupt not being serviced */
#ifdef MITIGATE_INTERRUPT_ISSUE
	do {
		spin_lock_enter_critical_section(&intr->spinlock, flags);
		while ((irq_reg[0] = intr->mif->irq_get(intr->mif)) != 0) {
			for (bit = 0; bit < MIFINTRBIT_NUM_INT; bit++) {
				if ((irq_reg[0] & (1 << bit)) && (intr->mifintrbit_irq_handler[bit] != mifintrbit_default_handler)) {
					intr->mifintrbit_irq_handler[bit](bit, intr->irq_data[bit]);
				}
			}
		}
		spin_unlock_leave_critical_section(&intr->spinlock, flags);
	} while (intr->mif->irq_get(intr->mif) != 0);
#else
	spin_lock_enter_critical_section(&intr->spinlock, flags);
	irq_reg[0] = intr->mif->irq_get(intr->mif);
	for (bit = 0; bit < MIFINTRBIT_NUM_INT; bit++) {
		if ((irq_reg[0] & (1 << bit)) && (intr->mifintrbit_irq_handler[bit] != mifintrbit_default_handler)) {
			intr->mifintrbit_irq_handler[bit](bit, intr->irq_data[bit]);
		}
	}
	spin_unlock_leave_critical_section(&intr->spinlock, flags);
#endif
}

/* Public functions */
int mifintrbit_alloc_tohost(struct mifintrbit *intr, mifintrbit_handler handler, void *data)
{
	unsigned long flags;
	int which_bit = 0;

	spin_lock_enter_critical_section(&intr->spinlock, flags);

	/* Search for free slots */
	which_bit = find_first_zero_bit(intr->bitmap_tohost, MIFINTRBIT_NUM_INT);

	if (which_bit == MIFINTRBIT_NUM_INT) {
		goto error;
	}

	if (intr->mifintrbit_irq_handler[which_bit] != mifintrbit_default_handler) {
		spin_unlock_leave_critical_section(&intr->spinlock, flags);
		goto error;
	}

	intr->mifintrbit_irq_handler[which_bit] = handler;
	intr->irq_data[which_bit] = data;

	/* Update bit mask */
	set_bit(which_bit, intr->bitmap_tohost);

	spin_unlock_leave_critical_section(&intr->spinlock, flags);

	return which_bit;

error:
	spin_unlock_leave_critical_section(&intr->spinlock, flags);
	pr_err("%s: Error registering irq , which bit = %d\n", __func__, which_bit);
	return -EIO;
}

int mifintrbit_free_tohost(struct mifintrbit *intr, int which_bit)
{
	unsigned long flags;

	if (which_bit >= MIFINTRBIT_NUM_INT) {
		goto error;
	}

	spin_lock_enter_critical_section(&intr->spinlock, flags);
	intr->mifintrbit_irq_handler[which_bit] = mifintrbit_default_handler;
	intr->irq_data[which_bit] = NULL;
	/* Update bit mask */
	clear_bit(which_bit, intr->bitmap_tohost);
	spin_unlock_leave_critical_section(&intr->spinlock, flags);

	return 0;

error:
	pr_err("%s: Error unregistering irq\n", __func__);
	return -EIO;
}

int mifintrbit_alloc_fromhost(struct mifintrbit *intr, enum scsc_mif_abs_target target)
{
	unsigned long flags;
	int which_bit = 0;
	unsigned long *p;

	spin_lock_enter_critical_section(&intr->spinlock, flags);

	if (target == SCSC_MIF_ABS_TARGET_R4) {
		p = intr->bitmap_fromhost_r4;
	} else if (target == SCSC_MIF_ABS_TARGET_M4) {
		p = intr->bitmap_fromhost_m4;
	} else {
		goto error;
	}

	/* Search for free slots */
	which_bit = find_first_zero_bit(p, MIFINTRBIT_NUM_INT);

	if (which_bit == MIFINTRBIT_NUM_INT) {
		goto error;
	}

	/* Update bit mask */
	set_bit(which_bit, p);

	spin_unlock_leave_critical_section(&intr->spinlock, flags);

	return which_bit;
error:
	spin_unlock_leave_critical_section(&intr->spinlock, flags);
	pr_err("%s: Error allocating bit %d on %s\n", __func__, which_bit, target ? "M4" : "R4");
	return -EIO;
}

int mifintrbit_free_fromhost(struct mifintrbit *intr, int which_bit, enum scsc_mif_abs_target target)
{
	unsigned long flags;
	unsigned long *p;

	spin_lock_enter_critical_section(&intr->spinlock, flags);

	if (which_bit >= MIFINTRBIT_NUM_INT) {
		goto error;
	}

	if (target == SCSC_MIF_ABS_TARGET_R4) {
		p = intr->bitmap_fromhost_r4;
	} else if (target == SCSC_MIF_ABS_TARGET_M4) {
		p = intr->bitmap_fromhost_m4;
	} else {
		goto error;
	}

	/* Clear bit mask */
	clear_bit(which_bit, p);
	spin_unlock_leave_critical_section(&intr->spinlock, flags);

	return 0;
error:
	spin_unlock_leave_critical_section(&intr->spinlock, flags);
	pr_err("%s: Error freeing bit %d on %s\n", __func__, which_bit, target ? "M4" : "R4");
	return -EIO;
}

/* core API */
void mifintrbit_deinit(struct mifintrbit *intr)
{
	int i;

	/* Set all handlers to default before unregistering the handler */
	for (i = 0; i < MIFINTRBIT_NUM_INT; i++) {
		intr->mifintrbit_irq_handler[i] = mifintrbit_default_handler;
	}
	intr->mif->irq_unreg_handler(intr->mif);
}

void mifintrbit_init(struct mifintrbit *intr, struct scsc_mif_abs *mif)
{
	int i;

	spin_lock_init(&intr->spinlock);
	/* Set all handlersd to default before hooking the hardware interrupt */
	for (i = 0; i < MIFINTRBIT_NUM_INT; i++) {
		intr->mifintrbit_irq_handler[i] = mifintrbit_default_handler;
	}

	/* reset bitmaps */
	bitmap_zero(intr->bitmap_tohost, MIFINTRBIT_NUM_INT);
	bitmap_zero(intr->bitmap_fromhost_r4, MIFINTRBIT_NUM_INT);
	bitmap_zero(intr->bitmap_fromhost_m4, MIFINTRBIT_NUM_INT);

	/* register isr with mif abstraction */
	mif->irq_reg_handler(mif, mifiintrman_isr, (void *)intr);

	/* cache mif */
	intr->mif = mif;
}
