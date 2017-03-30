/****************************************************************************
 *
 *   Copyright (c) 2016 Samsung Electronics Co., Ltd. All rights reserved.
 *
 ****************************************************************************/

#ifndef _PANICMON_H
#define _PANICMON_H
#include "mxman.h"

struct panicmon;

void panicmon_init(struct panicmon *panicmon, struct scsc_mx *mx);
void panicmon_deinit(struct panicmon *panicmon);

struct panicmon {
	struct scsc_mx *mx;
};

#endif /* _PANICMON_H */
