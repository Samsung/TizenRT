/****************************************************************************
 *
 * Copyright (c) 2014 - 2016 Samsung Electronics Co., Ltd. All rights reserved
 *
 ****************************************************************************/

#ifndef __MIFMBOXMAN_H
#define __MIFMBOXMAN_H

#include <semaphore.h>
#include <pthread.h>
#include "utils_misc.h"

struct mifmboxman;
struct scsc_mif_abs;

int mifmboxman_init(struct mifmboxman *mbox);
bool mifmboxman_alloc_mboxes(struct mifmboxman *mbox, int n, int *first_mbox_index);
void mifmboxman_free_mboxes(struct mifmboxman *mbox, int first_mbox_index, int n);
u32 *mifmboxman_get_mbox_ptr(struct mifmboxman *mbox,  struct scsc_mif_abs *mif_abs, int mbox_index);
int mifmboxman_deinit(struct mifmboxman *mbox);

#define MIFMBOX_NUM    8

/* Inclusion in core.c treat it as opaque */
struct mifmboxman {
	bool            in_use;
	u32             mbox_free;
	unsigned long   bitmap[(((MIFMBOX_NUM)+(64) - 1) / (64))];
	pthread_mutex_t lock;
};
#endif
