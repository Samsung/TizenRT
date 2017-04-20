/****************************************************************************
 *
 * Copyright (c) 2014 - 2016 Samsung Electronics Co., Ltd. All rights reserved
 *
 ****************************************************************************/

#ifndef _MXLOG_H
#define _MXLOG_H

#define MXLOG_BUFFER_SIZE 16
struct mxlog;

void mxlog_init(struct mxlog *mxlog, struct scsc_mx *mx);
void mxlog_release(struct mxlog *mxlog);

struct mxlog {
	struct scsc_mx *mx;
	u8 buffer[MXLOG_BUFFER_SIZE];
	u16 index;
};

#endif
