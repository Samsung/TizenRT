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
#include <tinyara/log_dump/log_dump_internal.h>
#include <tinyara/mm/mm.h>
#include <tinyara/config.h>

#include <semaphore.h>
#include <mqueue.h>

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

static int exception(void)
{
	if (IS_KMM_LOCKED()) {
		return LOG_DUMP_MEM_FAIL;
	}
	if (sched_self()->sched_priority > CONFIG_LOG_DUMP_MANAGER_PRIO) {
		return LOG_DUMP_FAIL;
	}
	if (sched_lockcount()) {
		return LOG_DUMP_FAIL;
	}
	
	return LOG_DUMP_OK;
}

static int request_compress(const int index)
{
	log_dump_msg_t comp_msg;
	comp_msg.uncomp_idx = index;

	int ret = mq_send(mq_fd, (const char *)&comp_msg, sizeof(log_dump_msg_t), 100);
	if (ret == ERROR) {
		lldbg("Fail to send message\n");
	}

	return ret;
}

static int put_spare_buf(const char ch)
{
	if (UNCOMP_SPARE_BUF.sparebytes == CONFIG_LOG_DUMP_SPARE_BUF_SIZE) {
		return LOG_DUMP_FAIL;
	}
		

	UNCOMP_SPARE_BUF.arr[UNCOMP_SPARE_BUF.sparebytes++] = ch;

	return LOG_DUMP_OK;
}

static void flush_spare_buf(void)
{
	int sparebytes = UNCOMP_SPARE_BUF.sparebytes;
	int curbytes = UNCOMP_TAIL.curbytes;

	memcpy(&UNCOMP_TAIL.arr[curbytes], UNCOMP_SPARE_BUF.arr, sparebytes);

	UNCOMP_TAIL.curbytes += sparebytes;
	UNCOMP_SPARE_BUF.sparebytes = 0;
}

static void flush_free_size(void)
{
	int curbytes = UNCOMP_TAIL.curbytes;
	char fsz_array[LOG_DUMP_STRLEN_FREEHEAP];	/* For Free heap memory string */

	snprintf(fsz_array, LOG_DUMP_STRLEN_FREEHEAP, "<fsz:%08d>", UNCOMP_TAIL.free_size);

	memcpy(&UNCOMP_TAIL.arr[curbytes], fsz_array, LOG_DUMP_STRLEN_FREEHEAP - 1);

	UNCOMP_TAIL.curbytes += LOG_DUMP_STRLEN_FREEHEAP - 1;
	UNCOMP_TAIL.free_size = -1;
}

static int request_tail_compress(int tail)
{
	sem_wait(&UNCOMP_BUF(tail).buf_sem);
	int ret = request_compress(tail);
	return ret;
}

static int move_tail_next(void)
{
	int next_idx = UNCOMP_NEXT(UNCOMP_TAIL_PTR);

	int prev_tail = UNCOMP_TAIL_PTR;

	sem_wait(&UNCOMP_BUF(next_idx).buf_sem);
	UNCOMP_TAIL_NEXT();
	sem_post(&UNCOMP_BUF(next_idx).buf_sem);
	return prev_tail;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int log_dump_save(char ch)
{
	if (!IS_POSSIBLE_TO_SAVE()) {
		return LOG_DUMP_OK;
	}
		

	sem_wait(&g_read_manager.read_sem);

	/**
	 * If uncomp ring is full
	 * This happens only after exception situation
	*/
	if (IS_UNCOMP_FULL()) {
		sem_post(&g_read_manager.read_sem);
		return put_spare_buf(ch);
	}

	/**
	 * If cur node is full
	 * This happens only after exception situation
	*/
	if (IS_UNCOMP_TAIL_FULL()) {
		log_dump_compress_and_move_tail();
	}
		

	if (UNCOMP_TAIL.free_size != -1) {
		flush_free_size();
	}
		

	if (UNCOMP_SPARE_BUF.sparebytes != 0) {
		flush_spare_buf();
	}
		

	UNCOMP_TAIL_WRITE(ch);

	if (IS_UNCOMP_TAIL_FULL()) {

		/**
		 * If exception situation
		 * just do nothing and return
		*/
		if (IS_UNCOMP_RING_FULL() && exception() != LOG_DUMP_OK) {
			sem_post(&g_read_manager.read_sem);
			return LOG_DUMP_OK;
		}

		log_dump_compress_and_move_tail();
	}

	sem_post(&g_read_manager.read_sem);
	return LOG_DUMP_OK;
}

int log_dump_compress_and_move_tail(void) {
	int cur_tail = move_tail_next();
	int ret = request_tail_compress(cur_tail);
	return ret;
}
