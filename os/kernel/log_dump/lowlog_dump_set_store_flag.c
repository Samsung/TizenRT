/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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
#include <tinyara/config.h>
#include <tinyara/compression.h>

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/clock.h>
#include "lowlog_dump.h"
#include <dirent.h>
#include <ctype.h>
#include <fcntl.h>

/****************************************************************************
 * Public Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables
 ****************************************************************************/
static int g_start_to_save = 0;

/****************************************************************************
 * Private Functions
 ****************************************************************************/
/*************************************************************************************
* Name: lowlog_dump_init
*
* Description:
*   init global variables.
*
*************************************************************************************/
void lowlog_dump_init()
{
    lowlog_dump_init_buf();
    lowlog_dump_init_flag();
}

/*************************************************************************************
* Name: lowlog_dump_init_flag
*
* Description:
*   init global variables.
*
*************************************************************************************/
void lowlog_dump_init_flag()
{
	g_start_to_save = 0;
}

/*************************************************************************************
* Name: lowlog_dump_get_store_flag
*
* Description: 
* return store flag
*************************************************************************************/
int lowlog_dump_get_store_flag(void)
{
	return g_start_to_save;
}

/*************************************************************************************
* Name: lowlog_dump_set_store_to_buffer_flag
*
* Description: 
*   Sets the flag to determine whether to store crash logs in the buffer. 
*   If the flag is 1, the log is saved; if it is 0, it is not saved.
*************************************************************************************/
void lowlog_dump_set_store_flag(int flag)
{
	if (lowlog_dump_get_retry_count() >= CONFIG_LOWLOG_DUMP_RETRY_COUNT) {
		return;
	}
	if (flag) {
		if (!lowlog_dump_get_size()) {
			lowlog_dump_init();
		}
		lldbg("\n\n\n==========================================================FROM THIS, lldbg LOG WILL BE SAVED!!==========================================================\n\n\n\n");
		g_start_to_save = flag;
	}
	else {
		lldbg("\n\n\n==========================================================TO THIS==========================================================\n\n\n\n");
	}
	g_start_to_save = flag;
}
