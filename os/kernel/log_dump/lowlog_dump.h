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

#ifndef __SCHED_LOWLOG_DUMP_H
#define __SCHED_LOWLOG_DUMP_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#ifdef CONFIG_LOWLOG_DUMP

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/clock.h>
#include <tinyara/log_dump/lowlog_dump.h>
#include <dirent.h>
#include <ctype.h>
#include <fcntl.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define DIR_PATH "/mnt/lowlog_dump"
#define MAX_FILENAME_LEN 64
#define PRE_FILENAME_UNCOMP "lowlog_dump"
#define PRE_FILENAME_COMP "lowlog_dump_comp"
#define REQUIRED_HEAP_FOR_COMPRESS 400000
/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/
typedef struct {
    int number;
    char name[MAX_FILENAME_LEN];
} FileEntry;

/****************************************************************************
 * Function Prototypes
 ****************************************************************************/
void lowlog_dump_init(void);
void lowlog_dump_init_flag(void);
void lowlog_dump_init_buf(void);
char *lowlog_dump_get_buf(void);
int lowlog_dump_is_valid_filename(const char *name, int *out_number);
int lowlog_dump_get_size(void);
int lowlog_dump_set_store_flag(int flag);

#endif							/* CONFIG_LOWLOG_DUMP */
#endif							/* __SCHED_LOWLOG_DUMP_H */
