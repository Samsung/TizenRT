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
#include <tinyara/config.h>
#include <tinyara/compression.h>

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/clock.h>
#include <tinyara/crashlog_writer/crashlog_writer.h>
#include <dirent.h>
#include <ctype.h>
#include <fcntl.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define DIR_PATH "/mnt/crashlog"
#define MAX_FILENAME_LEN 64

/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/
typedef struct {
    int number;
    char name[MAX_FILENAME_LEN];
} FileEntry;

/****************************************************************************
 * Public Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables
 ****************************************************************************/
static unsigned char g_crashlog[CONFIG_CLWR_BUF_SIZE];
static int g_start_to_save = 0;
static int g_crashlog_index = 0;
static char g_filename[MAX_FILENAME_LEN];
#ifndef REBOOT_AFTER_LOG_SAVE
static int g_retry_count = 0;
#endif
/****************************************************************************
 * Private Functions
 ****************************************************************************/
/*************************************************************************************
* Name: crashlog_writer_init
*
* Description:
*   init global variables.
*
*************************************************************************************/
static void crashlog_writer_init()
{
	int i;
	for (i = 0; i < CONFIG_CLWR_BUF_SIZE; i++) {
		g_crashlog[i] ='\0';
	}
	for (i = 0; i < MAX_FILENAME_LEN; i++) { 
		g_filename[i] = '\0';
	}
	g_start_to_save = 0;
	g_crashlog_index = 0;
}

/*************************************************************************************
* Name: crashlog_writer_is_valid_filename
*
* Description:
*   This function checks whether a file in mnt/crashlog/ is a log save file based on its filename.
*
*************************************************************************************/
static int crashlog_writer_is_valid_filename(const char *name, int *out_number) 
{
    char expected_prefix[16];
    snprintf(expected_prefix, sizeof(expected_prefix), "crashlog_");
	int prefix_length = strlen(expected_prefix);
    if (strncmp(name, expected_prefix, prefix_length) != 0) {
        return 0;
    }
	if (name[prefix_length] != 'c' && name[prefix_length] != 'u') {
		return 0;
    }
    for (int i = prefix_length + 1; i < strlen(name); i++) {
        if (!isdigit(name[i])) {
            return 0;
		}
    }

    if (out_number) {
        *out_number = atoi(name + prefix_length + 1);
	}
    return 1;
}

/*************************************************************************************
* Name: crashlog_writer_get_next_filename
*
* Description:
*   This function generates the filename for a crash log file
*   ex)
*   if compression is enabled -> crashlog_c001
*   if compression is disabled -> crashlog_u002
*************************************************************************************/
static int crashlog_writer_get_next_filename(char *new_filename, int compressed) 
{
    DIR *dir = opendir(DIR_PATH);
    if (!dir) {
		if (errno == ENOENT) {
			if (mkdir(DIR_PATH, 0755) != 0) {
				lldbg("mkdir failed\n");
				return -1;
			}
			else {
				dir = opendir(DIR_PATH);
				if (!dir) {
					lldbg("opendir failed\n");
					return -1;
				}
			}
		}
		lldbg("opendir failed\n");
		return -1;
    }
    FileEntry entries[CONFIG_CLWR_MAX_FILES];
    int count = 0;
    int max_number = -1;
	int min_number = 999999999;
    char prefix = compressed ? 'c' : 'u';
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        int num;
        if (crashlog_writer_is_valid_filename(entry->d_name, &num)) {
            strncpy(entries[count].name, entry->d_name, sizeof(entries[count].name));
            entries[count].number = num;
            count++;
            if (num > max_number) {
				max_number = num;
			}
			if (num < min_number) {
				min_number = num;
			}
            if (count >= CONFIG_CLWR_MAX_FILES) {
				break;
            }
        }
    }
    closedir(dir);
    // If there are more than CONFIG_CLWR_MAX_FILES files, delete the file with the smallest index.
    if (count >= CONFIG_CLWR_MAX_FILES) {
        for (int i = 0; i < count; i++) {
            if (entries[i].number == min_number) {
                char fullpath[512];
                snprintf(fullpath, sizeof(fullpath), "%s/%s", DIR_PATH, entries[i].name);
                unlink(fullpath);
                break;
            }
        }
    }
	char pre_part[20];
	snprintf(new_filename, MAX_FILENAME_LEN, "%s/", DIR_PATH);
	int path_len = strlen(new_filename);
	snprintf(pre_part, MAX_FILENAME_LEN - path_len - 1, "crashlog_%c", prefix);
	strncat(new_filename, pre_part, MAX_FILENAME_LEN - path_len - 1);
    char number_part[16];
    snprintf(number_part, sizeof(number_part), "%d", max_number + 1);
    strncat(new_filename, number_part, MAX_FILENAME_LEN - strlen(new_filename) - 1);
    return 1;
}

/*************************************************************************************
* Name: crashlog_writer_get_crashlog_size
*
* Description: 
*   Gets the size (in bytes) of the log currently stored in the buffer.
*
*************************************************************************************/
static int crashlog_writer_get_crashlog_size()
{
	return g_crashlog_index;
}

/*************************************************************************************
* Name: crashlog_writer_make_big_logs
*
* Description:
*   Prints a large random log for testing purposes.
*	random 100*rows bytes be printed.
*************************************************************************************/
static void crashlog_writer_make_big_logs(int rows)
{
	srand((unsigned int)time(NULL));
	char temp[100];
	for (int i = 0; i < rows; i++) {
		for (int k = 0; k < 98; k++) {
			temp[k] = 32 + rand() % (126 - 32 + 1);
		}
		temp[98] = '\n';
		temp[99] = '\0';
		lldbg("%s", temp);
	}
}

/*************************************************************************************
* Name: crashlog_writer_crashlog_to_file
*
* Description:
*   Saves the log stored in the buffer to a file.
*************************************************************************************/
static int crashlog_writer_crashlog_to_file()
{
	lldbg("\n\n\n===================================crash log file saving start==========================================\n\n\n");
	lldbg("\nlog_save_size is %d\n", crashlog_writer_get_crashlog_size());
	int ret = crashlog_writer_get_next_filename(g_filename, 0);
	lldbg("filename is %s\n", g_filename);
	if (ret < 0) {
		lldbg("get_next_filename() failed\n");
		return -1;
	}
	int fd = open(g_filename, O_WRONLY | O_CREAT | O_TRUNC);
	if (fd < 0) {
		lldbg("file open failed\n");
		return -1;
	}
	lldbg("file open success\n");
	int log_size = write(fd,g_crashlog, g_crashlog_index);
	close(fd);
	if (log_size < 0){
		lldbg("file write failed\n");
		return -1;
	}
	lldbg("file write success, count:%d\n", log_size);
	lldbg("\n\n\n===========================================crash log file saving end=====================================================\n\n\n");
	return log_size;
}

/*************************************************************************************
* Name: crashlog_writer_compressed_crashlog_to_file
*
* Description: 
*   Saves the compressed log from the buffer to a file.
*************************************************************************************/
static int crashlog_writer_compressed_crashlog_to_file(unsigned char *compressed_buf,unsigned int compressed_buf_size)
{
	lldbg("\n\n================================================================compress file write start===========================================================\n\n");
	if (compressed_buf == NULL) {
		return -1;
	}
	int ret = crashlog_writer_get_next_filename(g_filename, 1);
	lldbg("filename is %s\n", g_filename);
	if (ret < 0) {
		lldbg("get_next_filename() failed\n");
		return -1;
	}
	int fd = open(g_filename, O_WRONLY | O_CREAT | O_TRUNC);
	if (fd < 0) {
		lldbg("file open failed\n");
		return -1;
	}
	lldbg("file open success\n");
	ret = write(fd, compressed_buf, compressed_buf_size);
	if (ret < 0) {
		lldbg("file write failed\n");
		return -1;
	}
	close(fd);
	lldbg("write count: %d\n", ret);
	lldbg("\n\n==========================================================compress file write end===========================================================\n\n");
	return ret;
}

/*************************************************************************************
* Name: crashlog_writer_compress_crashlog_and_save
*
* Description: 
*   Compresses the log stored in the buffer and save to file.
*************************************************************************************/
static int crashlog_writer_compress_crashlog_and_save()
{
	lldbg("\n\n=====================================================compress start=====================================================\n\n");
    int malloc_size = crashlog_writer_get_crashlog_size() / 3;
	unsigned char *compressed_buf = (unsigned char *)kmm_malloc(malloc_size);
	if (compressed_buf == NULL) {
		lldbg("kmm_malloc() failed\n");
		return -1;
	}
    unsigned int compressed_buf_size = malloc_size;
	compress_block(compressed_buf, &compressed_buf_size, g_crashlog, CONFIG_CLWR_BUF_SIZE);
	lldbg("compressed buf size: %d\n", compressed_buf_size);

	lldbg("\n==============================================================compress end===============================================================\n");
    
	crashlog_writer_compressed_crashlog_to_file(compressed_buf, compressed_buf_size);
	free(compressed_buf);
	compressed_buf = NULL;
	return compressed_buf_size;
		
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
void crashlog_writer_crashlog_to_buffer(char ch)
{
	if (g_start_to_save && g_crashlog_index < CONFIG_CLWR_BUF_SIZE) {
		g_crashlog[g_crashlog_index++] = ch;
	}
}

/*************************************************************************************
* Name: crashlog_writer_set_store_to_buffer_flag
*
* Description: 
*   Sets the flag to determine whether to store crash logs in the buffer. 
*   If the flag is 1, the log is saved; if it is 0, it is not saved.
*************************************************************************************/
void crashlog_writer_set_store_to_buffer_flag(int flag)
{
#ifdef CONFIG_CLWR_RETRY_COUNT
	if (g_retry_count >= CONFIG_CLWR_RETRY_COUNT) {
		return -1;
	}
#endif
	if (flag) {
		if (!g_crashlog_index) {
			crashlog_writer_init();
		}
		lldbg("\n\n\n==========================================================FROM THIS, lldbg LOG WILL BE SAVED!!==========================================================\n\n\n\n");
		g_start_to_save = flag;
		//make_big_logs(200);
	}
	else {
		lldbg("\n\n\n==========================================================TO THIS==========================================================\n\n\n\n");
	}
	g_start_to_save = flag;
}

/*************************************************************************************
* Name: crashlog_writer_save_crashlog
*
* Description: 
*   Saves the crash log to a file. 
*   If the flag is 1, the log is saved in compressed form; if it is 0, it is saved uncompressed.
*************************************************************************************/
char *crashlog_writer_save_crashlog(int flag)
{
#ifdef CONFIG_CLWR_RECORD_TIME
    clock_t start = clock();
	int log_size = crashlog_writer_get_crashlog_size();
#endif
#ifdef CONFIG_CLWR_RETRY_COUNT
	if (g_retry_count >= CONFIG_CLWR_RETRY_COUNT) {
		return NULL;
	}
	g_retry_count++;;
#endif
	int ret;
	if (flag) {
		ret = crashlog_writer_compress_crashlog_and_save();
	}
	else{
		ret = crashlog_writer_crashlog_to_file();
	}
#ifdef CONFIG_CLWR_RECORD_TIME
	if (ret > 0) {
		clock_t end = clock();
		double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
		lldbg("\n\nElapsed time: %.6f seconds\n\n", elapsed_time);
		int fd = open("/mnt/crashlog/result", O_CREAT | O_WRONLY | O_APPEND);
		if (fd < 0) {
			return g_filename;
		}
		char buf[100];
#ifdef CONFIG_CLWR_COMPRESS
		snprintf(buf, sizeof(buf), "C: %d -> %d ", log_size, ret);
		write(fd, buf, strlen(buf));
#else
		snprintf(buf, sizeof(buf), "U: %d ", ret);
		write(fd, buf, strlen(buf));
#endif
		lldbg("filename is %s\n", g_filename);
		snprintf(buf, sizeof(buf), "in %.6fs", elapsed_time);
		write(fd, buf, strlen(buf));
		snprintf(buf, sizeof(buf), " (%s)\n", g_filename);
		write(fd, buf, strlen(buf));
		close(fd);
#ifndef CONFIG_CLWR_REBOOT_AFTER_LOG_SAVE
		crashlog_writer_init();
#endif
	}
#endif
	return g_filename;
}

/*************************************************************************************
* Name: crashlog_writer_read_crashlog
*
* Description: 
* 	Reads a log from a file and stores it in a buffer. If the log is compresed, it is
*	decompressed before being stored.
*************************************************************************************/
int crashlog_writer_read_crashlog(char *filename, char *buf, int buf_size) 
{
	lldbg("filename : %s\n",filename);
	int fd = open(filename, O_RDONLY);
	int ret;
	if (fd < 0) {
		lldbg("file open failed\n");
		return -1;
	}
	if (filename[strlen(DIR_PATH) + 10] == 'u') { //simple check
		lldbg("uncompressed file read start\n");
		ret = read(fd, buf, buf_size); //no add null
		if (ret < 0) {
			lldbg("read failed\n");
			return -1;
		}
		return ret;
	}
	else if (filename[strlen(DIR_PATH) + 10] == 'c') { //simple check
		lldbg("compressed file read start\n");
		unsigned int malloc_size = CONFIG_CLWR_BUF_SIZE;
		unsigned char *compressed_buf = (unsigned char *)kmm_malloc(malloc_size);
		if (compressed_buf == NULL) {
			lldbg("kmm_malloc() failed\n");
			return -1;
		}
  		unsigned int compressed_buf_size = malloc_size;
		ret = read(fd, compressed_buf, compressed_buf_size);
		if (ret < 0) {
			lldbg("read failed\n");
			free(compressed_buf);
			return -1;
		}
		int tmp_buf_size = buf_size;
		decompress_block(buf, &tmp_buf_size, compressed_buf, &compressed_buf_size);
		free(compressed_buf);
		return tmp_buf_size;
	}
	close(fd);
}

/*************************************************************************************
* Name: crashlog_writer_read_recent_crashlog
*
* Description: 
* 	Reads the most recent log from a file and stores it in a buffer. If the log is compresed, it is
*	decompressed before being stored.
*************************************************************************************/
int crashlog_writer_read_recent_crashlog(char *buf, int buf_size) 
{
	char most_recent_file_name[MAX_FILENAME_LEN];
	DIR *dir = opendir(DIR_PATH);
    if (!dir) {
		lldbg("opendir failed\n");
		return -1;
    }
	char temp_name[MAX_FILENAME_LEN];
    int count = 0;
    int max_number = -1;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        int num;
        if (crashlog_writer_is_valid_filename(entry->d_name, &num)) {
            count++;
            if (num > max_number) {
				max_number = num;
            	strncpy(temp_name, entry->d_name, sizeof(temp_name));
			}
            if (count >= CONFIG_CLWR_MAX_FILES) {
				break;
            }
        }
    }
    closedir(dir);


	snprintf(most_recent_file_name, MAX_FILENAME_LEN, "%s/%s", DIR_PATH,temp_name);
	lldbg("most_recent_file_name is %s\n",most_recent_file_name);
	crashlog_writer_read_crashlog(most_recent_file_name, buf, buf_size);
    return 1;
}