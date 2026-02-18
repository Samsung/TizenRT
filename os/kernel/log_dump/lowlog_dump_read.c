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
#include <dirent.h>
#include <ctype.h>
#include <fcntl.h>
#include "lowlog_dump.h"

/****************************************************************************
 * Public Variables
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/*************************************************************************************
* Name: lowlog_dump_read
*
* Description: 
* 	Reads a log from a file and stores it in a buffer. If the log is compresed, it is
*	decompressed before being stored.
*************************************************************************************/
int lowlog_dump_read(char *filename, char *buf, int buf_size) 
{
	int fd = open(filename, O_RDONLY);
	int ret;
	if (fd < 0) {
		ldpllvdbg("file open failed\n");
		return ERROR;
	}
	/* After reading the logs from the file, all files in DIRPATH should
	be deleted. Using lowlog_dump_is_valid_filename() will make this easier*/
	int dir_len = strlen(DIR_PATH);
	if (strlen(filename) - dir_len - 1 > strlen(PRE_FILENAME_COMP) && !strncmp(filename + dir_len + 1, PRE_FILENAME_COMP, strlen(PRE_FILENAME_COMP))) { //simple check
		unsigned int malloc_size = CONFIG_LOWLOG_DUMP_BUF_SIZE;
		unsigned char *compressed_buf = (unsigned char *)kmm_malloc(malloc_size);
		if (compressed_buf == NULL) {
			ldpllvdbg("kmm_malloc() failed\n");
            close(fd);
			return ERROR;
		}
  		unsigned int compressed_buf_size = malloc_size;
		ret = read(fd, compressed_buf, compressed_buf_size);
		if (ret < 0) {
			ldpllvdbg("read failed\n");
			free(compressed_buf);
            close(fd);
			return ERROR;
		}
		int tmp_buf_size = buf_size;
		decompress_block(buf, &tmp_buf_size, compressed_buf, &compressed_buf_size);
		free(compressed_buf);
        close(fd);
		return tmp_buf_size;
	}
	else if (strlen(filename) - dir_len - 1 > strlen(PRE_FILENAME_UNCOMP) && !strncmp(filename + dir_len + 1, PRE_FILENAME_UNCOMP, strlen(PRE_FILENAME_UNCOMP))) { //simple check
		ldpllvdbg("uncompressed file read start\n");
		ret = read(fd, buf, buf_size); //no add null
		if (ret < 0) {
			ldpllvdbg("read failed\n");
            close(fd);
			return ERROR;
		}
        close(fd);
		return ret;
	}
	return ERROR;
}

/*************************************************************************************
* Name: lowlog_dump_read_recent
*
* Description: 
* 	Reads the most recent log from a file and stores it in a buffer. If the log is compresed, it is
*	decompressed before being stored.
*************************************************************************************/
int lowlog_dump_read_recent(char *buf, int buf_size) 
{
	char most_recent_file_name[MAX_FILENAME_LEN];
	DIR *dir = opendir(DIR_PATH);
    if (!dir) {
		ldpllvdbg("opendir failed\n");
		return ERROR;
    }
	char temp_name[MAX_FILENAME_LEN];
    int count = 0;
    int max_number = -1;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        int num;
        if (lowlog_dump_is_valid_filename(entry->d_name, &num) == OK) {
            count++;
            if (num > max_number) {
				max_number = num;
            	strncpy(temp_name, entry->d_name, sizeof(temp_name));
			}
            if (count >= CONFIG_LOWLOG_DUMP_MAX_FILES) {
				break;
            }
        }
    }

    closedir(dir);

	snprintf(most_recent_file_name, MAX_FILENAME_LEN, "%s/%s", DIR_PATH, temp_name);
	ldpllvdbg("most_recent_file_name is %s\n",most_recent_file_name);
	lowlog_dump_read(most_recent_file_name, buf, buf_size);
    return OK;
}