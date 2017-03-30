/****************************************************************************
 *
 *   Copyright (c) 2016 Samsung Electronics Co., Ltd. All rights reserved.
 *
 ****************************************************************************/

#include "suspendmon.h"
#include "scsc_mif_abs.h"
#include "mxman.h"
#include "debug_scsc.h"
#include <stdbool.h>

static int suspendmon_suspend(struct scsc_mif_abs *mif, void *data)
{
	struct suspendmon *suspendmon = (struct suspendmon *)data;

	SLSI_INFO_NODEV("suspendmon=%p suspendmon->mx=%p mxman=%p\n",
			suspendmon, suspendmon->mx, scsc_mx_get_mxman(suspendmon->mx));

	return mxman_suspend(scsc_mx_get_mxman(suspendmon->mx));
}

static void suspendmon_resume(struct scsc_mif_abs *mif, void *data)
{
	struct suspendmon *suspendmon = (struct suspendmon *)data;

	SLSI_INFO_NODEV("suspendmon=%p suspendmon->mx=%p mxman=%p\n",
			suspendmon, suspendmon->mx, scsc_mx_get_mxman(suspendmon->mx));

	mxman_resume(scsc_mx_get_mxman(suspendmon->mx));
}

void suspendmon_init(struct suspendmon *suspendmon, struct scsc_mx *mx)
{
	struct scsc_mif_abs *mif;

	suspendmon->mx = mx;
	mif = scsc_mx_get_mif_abs(mx);

	/* register callbacks with mif abstraction */
	if (mif->suspend_reg_handler)
		mif->suspend_reg_handler(mif, suspendmon_suspend, suspendmon_resume, (void *)suspendmon);
}

void suspendmon_deinit(struct suspendmon *suspendmon)
{
	struct scsc_mif_abs *mif;

	mif = scsc_mx_get_mif_abs(suspendmon->mx);

	if (mif->suspend_unreg_handler)
		mif->suspend_unreg_handler(mif);
}
