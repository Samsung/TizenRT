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

/* uses */
#include "scsc_mif_abs.h"
#include <pthread.h>
#include <errno.h>
/* Implements */
#include "mifmboxman.h"

int mifmboxman_init(struct mifmboxman *mbox)
{
	if (mbox->in_use) {
		return -EBUSY;
	}

	pthread_mutex_init(&mbox->lock, NULL);
	mbox->mbox_free = MIFMBOX_NUM;
	mbox->in_use = true;
	bitmap_zero(mbox->bitmap, MIFMBOX_NUM);

	return 0;
}

bool mifmboxman_alloc_mboxes(struct mifmboxman *mbox, int n, int *first_mbox_index)
{
	unsigned int index = 0;
	unsigned int available;
	u8 i;

	pthread_mutex_lock(&mbox->lock);

	if ((n > MIFMBOX_NUM) || (n == 0) || !mbox->in_use) {
		goto error;
	}

	while (index <= (MIFMBOX_NUM - n)) {
		available = 0;

		/* Search consecutive blocks */
		for (i = 0; i < n; i++) {
			if (test_bit((i + index), mbox->bitmap)) {
				break;
			}
			available++;
		}

		if (available == n) {
			*first_mbox_index = index;

			for (i = 0; i < n; i++) {
				set_bit(index++, mbox->bitmap);
			}

			mbox->mbox_free -= n;
			goto exit;
		} else {
			index = index + available + 1;
		}
	}
error:
	pr_err("%s: Error allocating mbox\n", __func__);
	pthread_mutex_unlock(&mbox->lock);
	return false;
exit:
	pthread_mutex_unlock(&mbox->lock);
	return true;
}

void mifmboxman_free_mboxes(struct mifmboxman *mbox, int first_mbox_index, int n)
{
	int index = 0;
	int total_free = 0;

	if ((n > MIFMBOX_NUM) || ((n + first_mbox_index) > MIFMBOX_NUM) || (n == 0) || !mbox->in_use) {
		goto error;
	}

	pthread_mutex_lock(&mbox->lock);
	for (index = first_mbox_index; index < (first_mbox_index + n); index++)
		if (test_bit(index, mbox->bitmap)) {
			clear_bit(index, mbox->bitmap);
			total_free++;
		}

	mbox->mbox_free += total_free;
	pthread_mutex_unlock(&mbox->lock);

	return;
error:
	pr_err("%s: Error freeing mbox\n", __func__);
}

u32 *mifmboxman_get_mbox_ptr(struct mifmboxman *mbox, struct scsc_mif_abs *mif_abs, int mbox_index)
{
	/* Avoid unused parameter error */
	(void)mbox;

	return mif_abs->get_mbox_ptr(mif_abs, mbox_index);
}

int mifmboxman_deinit(struct mifmboxman *mbox)
{
	if (!mbox->in_use) {
		return -ENODEV;
	}
	mbox->in_use = false;
	return 0;
}
