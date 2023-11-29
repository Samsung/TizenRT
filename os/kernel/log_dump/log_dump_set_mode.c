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
#include <tinyara/config.h>
#include <tinyara/compiler.h>

#include <semaphore.h>

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

static void sleep_until_uncomp_queue_empty(void)
{
	while (!IS_UNCOMP_RING_EMPTY()) {
		usleep(10000);
	}
}

static void set_read_manager_to_head(void)
{
	g_read_manager.read_node = comp_chunk_peek();
	g_read_manager.readbytes = 0;
}

static int ready_to_read(void)
{
	if (!IS_POSSIBLE_TO_SAVE()) {
		return LOG_DUMP_FAIL;
	}

	sem_wait(&g_read_manager.read_sem);
	SAVE_STOP();

	sleep_until_uncomp_queue_empty();

	log_dump_compress_and_move_tail();

	sleep_until_uncomp_queue_empty();
		
	set_read_manager_to_head();
	return LOG_DUMP_OK;
}

static int finish_reading(void)
{
	if (IS_POSSIBLE_TO_SAVE()) {
		return LOG_DUMP_FAIL;
	}
		
	SAVE_START();
	sem_post(&g_read_manager.read_sem);
	return LOG_DUMP_OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int log_dump_set_mode(FAR const char *buffer, size_t buflen)
{
	(void)buflen;

	if (strncmp(buffer, LOGDUMP_SAVE_START, strlen(LOGDUMP_SAVE_START) + 1) == 0) {
		return finish_reading();
	} else if (strncmp(buffer, LOGDUMP_SAVE_STOP, strlen(LOGDUMP_SAVE_STOP) + 1) == 0) {
		return ready_to_read();
	} else {
		return LOG_DUMP_OPT_FAIL;
	}
	return LOG_DUMP_OK;
}
