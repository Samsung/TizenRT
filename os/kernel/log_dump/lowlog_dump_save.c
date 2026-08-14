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
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Variables
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/
/*************************************************************************************

/*************************************************************************************
* Name: lowlog_dump_is_valid_filename
*
* Description:
*   This function checks whether a file in mnt/crashlog/ is a log save file based on its filename.
*
*************************************************************************************/
int lowlog_dump_is_valid_filename(const char *name, int *out_number) 
{
    int pre_filename_comp_len = strlen(PRE_FILENAME_COMP);
    int pre_filename_uncomp_len = strlen(PRE_FILENAME_UNCOMP);
    int name_len = strlen(name);
    if (name_len > pre_filename_comp_len && !strncmp(name, PRE_FILENAME_COMP, pre_filename_comp_len)) {
        for (int i = pre_filename_comp_len; i < name_len; i++) {
            if (!isdigit(name[i])) {
                return ERROR;
            }
        }
        *out_number = atoi(name + pre_filename_comp_len);
        return OK;
    }
    else if (name_len > pre_filename_uncomp_len && !strncmp(name, PRE_FILENAME_UNCOMP, pre_filename_uncomp_len)) {
        for (int i = pre_filename_uncomp_len; i < name_len; i++) {
            if (!isdigit(name[i])) {
                return ERROR;
            }
        }
        *out_number = atoi(name + pre_filename_uncomp_len);
        return OK;
    }
    return ERROR;
}

/*************************************************************************************
* Name: lowlog_dump_get_next_filename
*
* Description:
*   This function generates the filename for a lowlog file
*   ex)
*   if compression is enabled -> lowlog_dumpXXX
*   if compression is disabled -> lowlog_dump_compXXX
*************************************************************************************/
static int lowlog_dump_get_next_filename(char *new_filename) 
{
	//TODO : Directory is not being created when it does not exist.
	DIR *dir = opendir(DIR_PATH);
	if (!dir) {
		if (errno == ENOENT) {
			if (mkdir(DIR_PATH, 0755) != 0) {
				ldpllvdbg("mkdir failed\n");
				return ERROR;
			}
			else {
				dir = opendir(DIR_PATH);
				if (!dir) {
					ldpllvdbg("opendir failed\n");
					return ERROR;
				}
			}
		}
		ldpllvdbg("opendir failed\n");
		return ERROR;
    }
    FileEntry entries[CONFIG_LOWLOG_DUMP_MAX_FILES];
    int count = 0;
    int max_number = -1;
	int min_number = 999999999;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        int num;
        if (lowlog_dump_is_valid_filename(entry->d_name, &num) == 0K) {
            strncpy(entries[count].name, entry->d_name, sizeof(entries[count].name));
            entries[count].number = num;
            count++;
            if (num > max_number) {
				max_number = num;
			}
			if (num < min_number) {
				min_number = num;
			}
            if (count >= CONFIG_LOWLOG_DUMP_MAX_FILES) {
				break;
            }
        }
    }
    closedir(dir);
    // If there are more than CONFIG_LOWLOG_DUMP_MAX_FILES files, not add a new file.
    if (max_number + 1 >= CONFIG_LOWLOG_DUMP_MAX_FILES) {
		return ERROR;
	}
	char tmp[64];
	snprintf(new_filename, MAX_FILENAME_LEN, "%s/", DIR_PATH);

	int path_len = strlen(new_filename);
#ifdef CONFIG_LOWLOG_DUMP_COMPRESS
	snprintf(tmp, MAX_FILENAME_LEN - path_len - 1, "%s%d", PRE_FILENAME_COMP, max_number + 1);
#else
	snprintf(tmp, MAX_FILENAME_LEN - path_len - 1, "%s%d", PRE_FILENAME_UNCOMP, max_number + 1);
#endif
	strncat(new_filename, tmp, MAX_FILENAME_LEN - path_len - 1);
    return OK;
}

/*************************************************************************************
* Name: lowlog_dump_save_uncomp
*
* Description:
*   Saves the log stored in the buffer to a file.
*************************************************************************************/
static int lowlog_dump_save_uncomp(char *filenm)
{
	ldpllvdbg("\n\n\n===================================crash log file saving start==========================================\n\n\n");
	char new_filename[MAX_FILENAME_LEN];
	int ret = lowlog_dump_get_next_filename(new_filename);
	if (ret < 0) {
		ldpllvdbg("get_next_filename() failed\n");
		return ERROR;
	}
	int fd = open(new_filename, O_WRONLY | O_CREAT | O_TRUNC);
	if (fd < 0) {
		ldpllvdbg("file open failed\n");
		return ERROR;
	}
	int log_size = write(fd, lowlog_dump_get_buf(), lowlog_dump_get_size());
	close(fd);
	if (log_size < 0) {
		ldpllvdbg("file write failed\n");
		return ERROR;
	}
	snprintf(filenm, MAX_FILENAME_LEN, "%s", new_filename);
	ldpllvdbg("file write success, count:%d\n", log_size);
	ldpllvdbg("\n\n\n===========================================crash log file saving end=====================================================\n\n\n");
	return log_size;
}

/*************************************************************************************
* Name: lowlog_dump_save_comp
*
* Description: 
*   Saves the compressed log from the buffer to a file.
*************************************************************************************/
static int lowlog_dump_save_comp(char *filenm, unsigned char *compressed_buf, unsigned int compressed_buf_size)
{
	ldpllvdbg("\n\n================================================================compress file write start===========================================================\n\n");
	char new_filename[MAX_FILENAME_LEN];
	if (compressed_buf == NULL) {
		return ERROR;
	}
	int ret = lowlog_dump_get_next_filename(new_filename);
	ldpllvdbg("filename is %s\n", new_filename);
	if (ret < 0) {
		ldpllvdbg("get_next_filename() failed\n");
		return ERROR;
	}
	int fd = open(new_filename, O_WRONLY | O_CREAT | O_TRUNC);
	if (fd < 0) {
		ldpllvdbg("file open failed\n");
		return ERROR;
	}
	ret = write(fd, compressed_buf, compressed_buf_size);
	if (ret < 0) {
		ldpllvdbg("file write failed\n");
		close(fd);
		return ERROR;
	}
	close(fd);
	snprintf(filenm, MAX_FILENAME_LEN, "%s", new_filename);
	ldpllvdbg("write count: %d\n", ret);
	ldpllvdbg("\n\n==========================================================compress file write end===========================================================\n\n");
	return ret;
}

/*************************************************************************************
* Name: lowlog_dump_compress_and_save
*
* Description: 
*   Compresses the log stored in the buffer and save to file.
*************************************************************************************/
static int lowlog_dump_compress_and_save(char *filenm)
{
	ldpllvdbg("\n\n=====================================================compress start=====================================================\n\n");
    int malloc_size = lowlog_dump_get_size() / 3;
	if (kmm_get_largest_freenode_size() < malloc_size) {
		return ERROR;
	}
	unsigned char *compressed_buf = (unsigned char *)kmm_malloc(malloc_size);
	if (compressed_buf == NULL) {
		ldpllvdbg("kmm_malloc() failed\n");
		return ERROR;
	}
	if (kmm_get_largest_freenode_size() < REQUIRED_HEAP_FOR_COMPRESS) {
		return ERROR;
		/*TODO : A configuration option should be added, and when this option is enabled, 
		the system should save logs without compression
		if there is insufficient heap memory available for compression */
	}
    unsigned int compressed_buf_size = malloc_size;
	compress_block(compressed_buf, &compressed_buf_size, lowlog_dump_get_buf(), lowlog_dump_get_size());
	ldpllvdbg("compressed buf size: %d\n", compressed_buf_size);

	ldpllvdbg("\n==============================================================compress end===============================================================\n");
    
	lowlog_dump_save_comp(filenm, compressed_buf, compressed_buf_size);
	free(compressed_buf);
	return compressed_buf_size;
		
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/*************************************************************************************
* Name: lowlog_dump_save
*
* Description: 
*   Saves the crash log to a file. 
*************************************************************************************/
int lowlog_dump_save(void)
{
	lowlog_dump_pause();
	char filenm[MAX_FILENAME_LEN];
	int log_size = lowlog_dump_get_size();
	if (log_size == 0) {
		return ERROR;
	}
#ifdef CONFIG_LOWLOG_DUMP_RECORD_TIME
    clock_t start = clock();
#endif
    int ret;
#ifdef CONFIG_LOWLOG_DUMP_COMPRESS
	ret = lowlog_dump_compress_and_save(filenm);
#else
	ret = lowlog_dump_save_uncomp(filenm);
#endif
	if (ret < 0) {
		return ERROR;
	}
#ifdef CONFIG_LOWLOG_DUMP_RECORD_TIME
	clock_t end = clock();
	double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
	ldpllvdbg("\n\nElapsed time: %.6f seconds\n\n", elapsed_time);
	int fd = open("/mnt/lowlog_dump/result", O_CREAT | O_WRONLY | O_APPEND);
	if (fd < 0) {
		return ERROR;
	}
	char buf[100];
//TODO : The number of snprintf and write calls should be reduced.
//TODO : add result checking and error handling about snprintf and write.
#ifdef CONFIG_LOWLOG_DUMP_COMPRESS
	snprintf(buf, sizeof(buf), "C: %d -> %d ", log_size, ret);
	write(fd, buf, strlen(buf));
#else
	snprintf(buf, sizeof(buf), "U: %d ", ret);
	write(fd, buf, strlen(buf));
#endif
	ldpllvdbg("filenm is %s\n", filenm);
	snprintf(buf, sizeof(buf), "in %.6fs", elapsed_time);
	write(fd, buf, strlen(buf));
	snprintf(buf, sizeof(buf), " (%s)\n", filenm);
	write(fd, buf, strlen(buf));
	close(fd);
#endif
	return OK;
}
