/****************************************************************************
 *
 *   Copyright (c) 2016 Samsung Electronics Co., Ltd. All rights reserved.
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
