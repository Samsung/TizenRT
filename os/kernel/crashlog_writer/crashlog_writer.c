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

/****************************************************************************
 * Public Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables
 ****************************************************************************/
typedef struct {
    int number;
    char name[MAX_FILENAME_LEN];
} FileEntry;

static unsigned char *compressed_buf=NULL;
static unsigned int compressed_buf_size;
static char crash_log[CONFIG_CRASH_LOG_BUF_SIZE];
static int start_to_save=0;
static int crash_log_index=0;
#ifndef REBOOT_AFTER_LOG_SAVE
static int retry_count=0;
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/
/*************************************************************************************
* Name: is_valid_filename
*
* Description:
*   This function checks whether a file in mnt/crashlog/ is a log save file based on its filename.
*
*************************************************************************************/
static int is_valid_filename(const char *name, int *out_number) {
    char expected_prefix[16];
    snprintf(expected_prefix, sizeof(expected_prefix), "crashlog_");
	int prefix_length=strlen(expected_prefix);
    if (strncmp(name, expected_prefix, prefix_length) != 0) {
        return 0;
    }
	if (name[prefix_length]!='c' && name[prefix_length]!='u') {
		return 0;
    }
    const char *number_str = name + prefix_length+1;
    if ((int)strlen(number_str) != CONFIG_MAX_DIGITS)
        return 0;
    for (int i = 0; i < CONFIG_MAX_DIGITS; i++) {
        if (!isdigit(number_str[i]))
            return 0;
    }

    if (out_number)
        *out_number = atoi(number_str);
    return 1;
}

/*************************************************************************************
* Name: get_next_filename
*
* Description:
*   This function generates the filename for a crash log file
*   ex)
*   if compression is enabled -> crashlog_c001
*   if compression is disabled -> crashlog_u002
*************************************************************************************/
static int get_next_filename(char *new_filename, int compressed) {
    DIR *dir = opendir(DIR_PATH);
    if (!dir) {
		if(errno == ENOENT){
			if (mkdir(DIR_PATH, 0755)!=0){
				lldbg("mkdir failed\n");
				return -1;
			}
			else{
				dir=opendir(DIR_PATH);
				if(!dir) {
					lldbg("opendir failed\n");
					return -1;
				}
			}
		}
		lldbg("opendir failed\n");
		return -1;
    }
    FileEntry entries[CONFIG_MAX_FILES + 1];
    int count = 0;
    int total_matching = 0;
    int max_number = -1;
    int min_number = 99999999;
    char prefix = compressed ? 'c' : 'u';
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        int num;
        if (is_valid_filename(entry->d_name, &num)) {
            if (count < CONFIG_MAX_FILES + 1) {
                strncpy(entries[count].name, entry->d_name, sizeof(entries[count].name));
                entries[count].number = num;
                count++;
            }
            if (num > max_number) max_number = num;
            if (num < min_number) min_number = num;
            total_matching++;
        }
		else{
			lldbg("%s is not valid_filename\n",entry->d_name);
		}
    }
    closedir(dir);
    // If there are more than CONFIG_CONFIG_MAX_FILES files, delete the file with the smallest index.
    if (total_matching >= CONFIG_MAX_FILES) {
        for (int i = 0; i < count; i++) {
            if (entries[i].number == min_number) {
                char fullpath[512];
                snprintf(fullpath, sizeof(fullpath), "%s/%s", DIR_PATH, entries[i].name);
                unlink(fullpath);
                break;
            }
        }
    }
    // Wraparound
    int limit = 1;
    for (int i = 0; i < CONFIG_MAX_DIGITS; i++) {
        limit *= 10;
    }
    int next_number = (max_number + 1) % limit;
    // Create format string like "%05d"
	char pre_part[20];
    char fmt[8];
    snprintf(fmt, sizeof(fmt), "%%0%dd", CONFIG_MAX_DIGITS);  // → "%0xd"
	snprintf(new_filename, MAX_FILENAME_LEN,"%s/",DIR_PATH);\
	int path_len=strlen(new_filename);
	snprintf(pre_part, MAX_FILENAME_LEN-path_len-1, "crashlog_%c", prefix);
	strncat(new_filename, pre_part,MAX_FILENAME_LEN-path_len-1);
    char number_part[16];
    snprintf(number_part, sizeof(number_part), fmt, next_number);
    strncat(new_filename, number_part, MAX_FILENAME_LEN - strlen(new_filename) - 1);
    return 1;
}

/*************************************************************************************
* Name: get_crash_log_save_size
*
* Description: 
*   Gets the size (in bytes) of the log currently stored in the buffer.
*
*************************************************************************************/
static int get_crash_log_save_size(){
	return crash_log_index;
}

/*************************************************************************************
* Name: make_big_logs
*
* Description:
*   Prints a large random log for testing purposes.
*************************************************************************************/
static void make_big_logs(int rows){ //random 100*rows bytes be printed.
	srand((unsigned int)time(NULL));
	char temp[100];
	for(int i=0;i<rows;i++){
		for(int k=0;k<98;k++) temp[k]=32+rand()%(126-32+1);
		temp[98]='\n';
		temp[99]='\0';
		lldbg("%s",temp);
	}
}

/*************************************************************************************
* Name: crash_log_to_file
*
* Description:
*   Saves the log stored in the buffer to a file.
*************************************************************************************/
static int crash_log_to_file(){
	lldbg("\n\n\n===============crash log file saving start==================\n\n\n");
	lldbg("\nlog_save_size is %d\n",get_crash_log_save_size());
	char filename[MAX_FILENAME_LEN];
	int ret=get_next_filename(filename,0);
	lldbg("filename is %s\n",filename);
	if(ret<0){
		lldbg("get_next_filename() failed\n");
		return -1;
	}
	int fd=open(filename,O_WRONLY|O_CREAT|O_TRUNC);
	if(fd<0){
		lldbg("file open failed\n");
		return -1;
	}
	lldbg("file open success\n");
	int log_size=write(fd,crash_log,crash_log_index);
	close(fd);
	if(log_size<0){
		lldbg("file write failed\n");
		return -1;
	}
	lldbg("file write success, count:%d\n",log_size);
	lldbg("\n\n\n===============crash log file saving end=====================\n\n\n");
	return log_size;
}

/*************************************************************************************
* Name: crash_log_compress
*
* Description: 
*   Compresses the log stored in the buffer.
*************************************************************************************/
static int crash_log_compress(){

	lldbg("\n\n================compress start==============\n\n");
    int malloc_size=get_crash_log_save_size()/3;
	compressed_buf=(unsigned char*)kmm_malloc(malloc_size);
	if(compressed_buf==NULL){
		lldbg("kmm_malloc() failed\n");
		return -1;
	}
    compressed_buf_size=malloc_size;
	compress_block(compressed_buf,&compressed_buf_size,crash_log,CONFIG_CRASH_LOG_BUF_SIZE);
	lldbg("compressed buf size: %d\n",compressed_buf_size);

	lldbg("\n====================compress end =====================\n");
    return compressed_buf_size;
		
}

/*************************************************************************************
* Name: compressed_crash_log_to_file
*
* Description: 
*   Saves the compressed log from the buffer to a file.
*************************************************************************************/
static int compressed_crash_log_to_file(){
	lldbg("\n\n================compress file write start==============\n\n");
	if(compressed_buf==NULL){
		return -1;
	}
	char filename[MAX_FILENAME_LEN];
	int ret=get_next_filename(filename,1);
	lldbg("filename is %s\n",filename);
	if(ret<0){
		lldbg("get_next_filename() failed\n");
		return -1;
	}
	int fd=open(filename,O_WRONLY|O_CREAT|O_TRUNC);
	if(fd<0){
		lldbg("file open failed\n");
		return -1;
	}
	lldbg("file open success\n");
	ret=write(fd,compressed_buf,compressed_buf_size);
	if(ret<0){
		lldbg("file write failed\n");
		return -1;
	}
	close(fd);
	lldbg("write count: %d\n",ret);
	lldbg("\n\n=====================compress file write end ===================\n\n");
	return ret;
}

/*************************************************************************************
* Name: free_compressed_log
*
* Description:
*   Frees the heap memory allocated for storing the compressed log.
*************************************************************************************/
static void free_compressed_log(){
	free(compressed_buf);
	compressed_buf=NULL;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/*************************************************************************************
* Name: crash_log_to_buffer
*
* Description: 
*   Stores the crash log into the buffer.
*   This is used to store logs that are output via lldbg() during a user crash situation into a buffer.
*************************************************************************************/
void crash_log_to_buffer(char ch){
	if(start_to_save && crash_log_index<CONFIG_CRASH_LOG_BUF_SIZE) crash_log[crash_log_index++]=ch;
}

/*************************************************************************************
* Name: set_store_to_buffer_flag
*
* Description: 
*   Sets the flag to determine whether to store crash logs in the buffer. 
*   If the flag is 1, the log is saved; if it is 0, it is not saved.
*************************************************************************************/
void set_store_to_buffer_flag(int flag){
#ifdef CONFIG_CRASH_LOG_RETRY_COUNT
	if (retry_count>=CONFIG_CRASH_LOG_RETRU_COUNT)

		return -1;
#endif
	if (flag){
		lldbg("\n\n\n================================FROM THIS, lldbg LOG WILL BE SAVED!!================================\n\n\n\n");
		start_to_save=flag;
		make_big_logs(200);
	}
	else {
		lldbg("\n\n\n===========================================TO THIS==================================================\n\n\n\n");
	}
	start_to_save=flag;
}

/*************************************************************************************
* Name: save_crash_log
*
* Description: 
*   Saves the crash log to a file. 
*   If the flag is 1, the log is saved in compressed form; if it is 0, it is saved uncompressed.
*************************************************************************************/
void save_crash_log(int flag){
#ifdef CONFIG_CRASH_LOG_TIME_TEST
    clock_t start=clock();
#endif
#ifdef CONFIG_CRASH_LOG_SAVE_RETRY_COUNT
	if (retry_count>=CONFIG_CRASH_LOG_SAVE_RETRY_COUNT)
		return;
	retry_count++;
#endif
	if(flag){
		crash_log_compress();
		compressed_crash_log_to_file();
		free_compressed_log();
	}
	else{
		crash_log_to_file();
	}
#ifdef CONFIG_CRASH_LOG_TIME_TEST
    clock_t end=clock();
	lldbg("\n\nElapsed time: %.6f seconds\n\n",(double)(end-start)/CLOCKS_PER_SEC);
#endif
#ifndef CONFIG_REBOOT_AFTER_LOG_SAVE
	start_to_save=0;
	crash_log_index=0;
#endif
}