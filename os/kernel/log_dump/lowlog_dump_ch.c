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

/****************************************************************************
 * Public Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables
 ****************************************************************************/
static char g_lowlog[CONFIG_LOWLOG_DUMP_BUF_SIZE];
int g_lowlog_index = 0;

/****************************************************************************
 * Private Functions
 ****************************************************************************/
/*************************************************************************************
* Name: lowlog_dump_get_size
*
* Description: 
*   Gets the size (in bytes) of the log currently stored in the buffer.
*
*************************************************************************************/
int lowlog_dump_get_size()
{
	return g_lowlog_index;
}

/*************************************************************************************
* Name: lowlog_dump_init_buf
*
* Description:
*   init global buf
*
*************************************************************************************/
//TODO : It may be necessary to remove this function.
void lowlog_dump_init_buf()
{
	for (int i = 0; i < CONFIG_LOWLOG_DUMP_BUF_SIZE; i++) {
		g_lowlog[i] = '\0';
	}
	g_lowlog_index = 0;
}

/*************************************************************************************
* Name: lowlog_dump_get_buf
*
* Description:
*   return lowlog buf
*
*************************************************************************************/
char *lowlog_dump_get_buf()
{
	return g_lowlog;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/*************************************************************************************
* Name: crashlog_writer_crashlog_to_buffer
*
* Description: 
*   Stores the crash log into the buffer.
*   This is used to store logs that are output via lldbg() during a user crash situation into a buffer.
*************************************************************************************/
void lowlog_dump_save_ch(char ch)
{
	if (lowlog_dump_get_store_flag() && g_lowlog_index < CONFIG_LOWLOG_DUMP_BUF_SIZE) {
		g_lowlog[g_lowlog_index++] = ch;
	}
}

