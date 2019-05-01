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

#ifndef __MIFMBOXMAN_H
#define __MIFMBOXMAN_H

#include <sys/types.h>
#include <semaphore.h>
#include "utils_misc.h"

struct mifmboxman;
struct scsc_mif_abs;

int mifmboxman_init(struct mifmboxman *mbox);
bool mifmboxman_alloc_mboxes(struct mifmboxman *mbox, int n, int *first_mbox_index);
void mifmboxman_free_mboxes(struct mifmboxman *mbox, int first_mbox_index, int n);
u32 *mifmboxman_get_mbox_ptr(struct mifmboxman *mbox, struct scsc_mif_abs *mif_abs, int mbox_index);
int mifmboxman_deinit(struct mifmboxman *mbox);

#define MIFMBOX_NUM    8

/* Inclusion in core.c treat it as opaque */
struct mifmboxman {
	bool in_use;
	u32 mbox_free;
	unsigned long bitmap[(((MIFMBOX_NUM) + (64) - 1) / (64))];
	pthread_mutex_t lock;
};
#endif
