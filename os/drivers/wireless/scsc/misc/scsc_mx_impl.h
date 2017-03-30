/****************************************************************************
 *
 * Copyright (c) 2014 - 2016 Samsung Electronics Co., Ltd. All rights reserved
 *
 ****************************************************************************/

#ifndef _CORE_H_
#define _CORE_H_

#include "scsc_mif_abs.h"

struct device;
struct scsc_mx;
struct mifintrbit;
struct miframman;
struct mifmboxman;
struct mxman;
struct srvman;
struct mxmgmt_transport;

struct scsc_mx          *scsc_mx_create(struct scsc_mif_abs *mif);
void scsc_mx_destroy(struct scsc_mx *mx);
struct scsc_mif_abs     *scsc_mx_get_mif_abs(struct scsc_mx *mx);
struct mifintrbit       *scsc_mx_get_intrbit(struct scsc_mx *mx);
struct mifmuxman        *scsc_mx_get_muxman(struct scsc_mx *mx);
struct miframman        *scsc_mx_get_ramman(struct scsc_mx *mx);
struct mifmboxman       *scsc_mx_get_mboxman(struct scsc_mx *mx);

struct mxman            *scsc_mx_get_mxman(struct scsc_mx *mx);
struct srvman           *scsc_mx_get_srvman(struct scsc_mx *mx);
struct mxmgmt_transport *scsc_mx_get_mxmgmt_transport(struct scsc_mx *mx);
struct gdb_transport    *scsc_mx_get_gdb_transport_r4(struct scsc_mx *mx);
struct mxlog            *scsc_mx_get_mxlog(struct scsc_mx *mx);
struct mxlog_transport  *scsc_mx_get_mxlog_transport(struct scsc_mx *mx);
struct panicmon         *scsc_mx_get_panicmon(struct scsc_mx *mx);
struct suspendmon       *scsc_mx_get_suspendmon(struct scsc_mx *mx);
#endif
