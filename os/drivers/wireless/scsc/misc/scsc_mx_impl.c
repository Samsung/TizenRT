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
#include "scsc_mx_impl.h"
#include "mifintrbit.h"
#include "miframman.h"
#include "mifmboxman.h"
#include "mxman.h"
#include "srvman.h"
#include "mxmgmt_transport.h"
#include "gdb_transport.h"
#include "mxlog.h"
#include "panicmon.h"
#include "mxlog_transport.h"
#include "suspendmon.h"
#include "debug_scsc.h"

#include <tinyara/kmalloc.h>

struct scsc_mx {
	struct scsc_mif_abs *mif_abs;
	struct mifintrbit intr;
	struct miframman ram;
	struct mifmboxman mbox;
	struct mxman mxman;
	struct srvman srvman;
	struct mxmgmt_transport mxmgmt_transport;
	struct gdb_transport gdb_transport_r4;
	struct gdb_transport gdb_transport_m4;
	int users;
	struct mxlog mxlog;
	struct panicmon panicmon;
	struct mxlog_transport mxlog_transport;
	struct suspendmon suspendmon;
};

struct scsc_mx *scsc_mx_create(struct scsc_mif_abs *mif)
{
	struct scsc_mx *mx;

	mx = kmm_zalloc(sizeof(*mx));
	if (!mx) {
		return NULL;
	}

	mx->mif_abs = mif;

	mifintrbit_init(&mx->intr, mif);

	mifmboxman_init(&mx->mbox);
	panicmon_init(&mx->panicmon, mx);
	suspendmon_init(&mx->suspendmon, mx);
	mxman_init(&mx->mxman, mx);
	srvman_init(&mx->srvman, mx);
	SLSI_INFO_NODEV("Hurray Maxwell is here with %p\n", mx);
	return mx;
}

void scsc_mx_destroy(struct scsc_mx *mx)
{
	SLSI_INFO_NODEV("\n");
	//BUG_ON(mx == NULL);
	mifintrbit_deinit(&mx->intr);
	mifmboxman_deinit(scsc_mx_get_mboxman(mx));
	suspendmon_deinit(scsc_mx_get_suspendmon(mx));
	panicmon_deinit(scsc_mx_get_panicmon(mx));
	srvman_deinit(&mx->srvman);
	mxman_deinit(&mx->mxman);
	kmm_free(mx);
}

struct scsc_mif_abs *scsc_mx_get_mif_abs(struct scsc_mx *mx)
{
	return mx->mif_abs;
}

struct mifintrbit *scsc_mx_get_intrbit(struct scsc_mx *mx)
{
	return &mx->intr;
}

struct miframman *scsc_mx_get_ramman(struct scsc_mx *mx)
{
	return &mx->ram;
}

struct mifmboxman *scsc_mx_get_mboxman(struct scsc_mx *mx)
{
	return &mx->mbox;
}

struct mxman *scsc_mx_get_mxman(struct scsc_mx *mx)
{
	return &mx->mxman;
}

struct srvman *scsc_mx_get_srvman(struct scsc_mx *mx)
{
	return &mx->srvman;
}

struct mxmgmt_transport *scsc_mx_get_mxmgmt_transport(struct scsc_mx *mx)
{
	return &mx->mxmgmt_transport;
}

struct gdb_transport *scsc_mx_get_gdb_transport_r4(struct scsc_mx *mx)
{
	return &mx->gdb_transport_r4;
}

struct mxlog *scsc_mx_get_mxlog(struct scsc_mx *mx)
{
	return &mx->mxlog;
}

struct panicmon *scsc_mx_get_panicmon(struct scsc_mx *mx)
{
	return &mx->panicmon;
}

struct mxlog_transport *scsc_mx_get_mxlog_transport(struct scsc_mx *mx)
{
	return &mx->mxlog_transport;
}

struct suspendmon *scsc_mx_get_suspendmon(struct scsc_mx *mx)
{
	return &mx->suspendmon;
}
