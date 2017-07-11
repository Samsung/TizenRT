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

struct scsc_mx *scsc_mx_create(struct scsc_mif_abs *mif);
void scsc_mx_destroy(struct scsc_mx *mx);
struct scsc_mif_abs *scsc_mx_get_mif_abs(struct scsc_mx *mx);
struct mifintrbit *scsc_mx_get_intrbit(struct scsc_mx *mx);
struct mifmuxman *scsc_mx_get_muxman(struct scsc_mx *mx);
struct miframman *scsc_mx_get_ramman(struct scsc_mx *mx);
struct mifmboxman *scsc_mx_get_mboxman(struct scsc_mx *mx);

struct mxman *scsc_mx_get_mxman(struct scsc_mx *mx);
struct srvman *scsc_mx_get_srvman(struct scsc_mx *mx);
struct mxmgmt_transport *scsc_mx_get_mxmgmt_transport(struct scsc_mx *mx);
struct gdb_transport *scsc_mx_get_gdb_transport_r4(struct scsc_mx *mx);
struct mxlog *scsc_mx_get_mxlog(struct scsc_mx *mx);
struct mxlog_transport *scsc_mx_get_mxlog_transport(struct scsc_mx *mx);
struct panicmon *scsc_mx_get_panicmon(struct scsc_mx *mx);
struct suspendmon *scsc_mx_get_suspendmon(struct scsc_mx *mx);
#endif
