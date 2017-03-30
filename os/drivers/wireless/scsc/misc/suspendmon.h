/****************************************************************************
 *
 *   Copyright (c) 2016 Samsung Electronics Co., Ltd. All rights reserved.
 *
 ****************************************************************************/

/* Baroque suspend/resume handler registration interface */

#ifndef _SUSPENDMON_H
#define _SUSPENDMON_H

#include "mxman.h"

struct suspendmon;

void suspendmon_init(struct suspendmon *suspendmon, struct scsc_mx *mx);
void suspendmon_deinit(struct suspendmon *suspendmon);

struct suspendmon {
	struct scsc_mx *mx;
};

#endif /* _SUSPENDMON_H */
