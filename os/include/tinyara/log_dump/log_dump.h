/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
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
#include <stdio.h>
#include <fcntl.h>

/********************************************************************************
 * Definitions
 ********************************************************************************/

/* Log Dump Thread information */
#define LOG_DUMP_NAME        "log_dump"		/* Log dump thread name */
#define LOG_DUMP_STACKSIZE   16384		/* Log dump thread stack size */
#define LOG_DUMP_PRIORITY    100		/* Log dump thread priority */

#define LOGDUMP_SAVE_START	"1"
#define LOGDUMP_SAVE_STOP	"2"

/********************************************************************************
 * Public Types
 ********************************************************************************/
/********************************************************************************
 * Public Functions
 ********************************************************************************/

#define OPEN_LOGDUMP()			open("/proc/logsave", O_RDWR)
#define START_LOGDUMP_SAVE(fd)		write(fd, LOGDUMP_SAVE_START, strlen(LOGDUMP_SAVE_START) + 1)
#define STOP_LOGDUMP_SAVE(fd)		write(fd, LOGDUMP_SAVE_STOP, strlen(LOGDUMP_SAVE_STOP) + 1)
#define READ_LOGDUMP(fd, buf, bufsize)	read(fd, buf, bufsize)
#define CLOSE_LOGDUMP(fd)		close(fd)
