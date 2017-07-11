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

#include "panicmon.h"
#include "scsc_mif_abs.h"
#include "mxman.h"
#include "debug_scsc.h"

#ifdef CONFIG_SLSI_WLAN_STATS
int firmware_triggered_panic = 0;
#endif

static void panicmon_isr(int irq, void *data)
{
	struct panicmon *panicmon = (struct panicmon *)data;

#ifdef CONFIG_SLSI_WLAN_STATS
	firmware_triggered_panic++;
#endif

	SLSI_INFO_NODEV("panicmon=%p panicmon->mx=%p mxman=%p\n", panicmon, panicmon->mx, scsc_mx_get_mxman(panicmon->mx));
	/* Avoid unused parameter error */
	(void)irq;
	mxman_fail(scsc_mx_get_mxman(panicmon->mx));
}

void panicmon_init(struct panicmon *panicmon, struct scsc_mx *mx)
{
	struct scsc_mif_abs *mif;

	panicmon->mx = mx;
	mif = scsc_mx_get_mif_abs(mx);
	/* register isr with mif abstraction */
	mif->irq_reg_reset_request_handler(mif, panicmon_isr, (void *)panicmon);
}

void panicmon_deinit(struct panicmon *panicmon)
{
	struct scsc_mif_abs *mif;

	mif = scsc_mx_get_mif_abs(panicmon->mx);
	mif->irq_unreg_reset_request_handler(mif);
}
