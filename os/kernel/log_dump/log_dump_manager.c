/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "log_dump/log_dump.h"

#include <tinyara/log_dump/log_dump.h>
#include <tinyara/sched.h>

#include <semaphore.h>
#include <mqueue.h>
#include <stdbool.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/
/****************************************************************************
 * Public Variables
 ****************************************************************************/
/****************************************************************************
 * Private Variables
 ****************************************************************************/
/****************************************************************************
 * Private Functions
 ****************************************************************************/
/****************************************************************************
 * Public Functions
 ****************************************************************************/

int log_dump_manager(int argc, char *argv[])
{
	if (log_dump_initialize() != LOG_DUMP_OK) {
		lldbg("Fail to init log dump\n");
		return 0;
	}

	log_dump_msg_t recv_msg;
	int nbytes;
	int compressed_size;
	sem_t *prev_head_sem = NULL;

	while (true) {
		nbytes = mq_receive(mq_fd, (char *)&recv_msg, sizeof(log_dump_msg_t), NULL);
		if (nbytes <= 0) {
			continue;
		}

		int uncomp_size = UNCOMP_HEAD.curbytes;
		
		if (uncomp_size <= 0) {
			lldbg("No log to compress");
			UNCOMP_HEAD_NEXT();
			sem_post(&UNCOMP_HEAD.buf_sem);
			continue;
		}

		compressed_size = log_dump_compress(g_out_buf, UNCOMP_HEAD.arr, uncomp_size);
		if (compressed_size == LOG_DUMP_COMPRESS_FAIL) {
			lldbg("Fail to compress");
		}
		else {
			log_dump_append((const char*)g_out_buf, compressed_size);
		}

		UNCOMP_HEAD.curbytes = 0;
		UNCOMP_HEAD.free_size = log_dump_get_free_size();

		prev_head_sem = &UNCOMP_HEAD.buf_sem;

		UNCOMP_HEAD_NEXT();
		sem_post(prev_head_sem);
	}

	return LOG_DUMP_OK;
}
