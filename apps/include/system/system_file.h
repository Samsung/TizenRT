/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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

#ifndef __SYSTEM_FILE_H__
#define __SYSTEM_FILE_H__
#include "system.h"


#ifndef DOXYGEN
	#include <stdio.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file system_file.h
 * @brief simple file IO API
 */

/**
 * @defgroup file file
 * @brief <b> simple file IO API </b>
 *
 * 1. tizenrt file io need simple file functions, only open, close, read, write \n
 *    not need fseek (only sequence read and write)
 *
 * 2. file module have below functions
 *  - file system init, deinit
 *	- file open, close, read, write
 *
 * @brief <b> file module example </b>
 * @includelineno  ex_file.c
 *
 * @{
 */

#ifdef DOXYGEN
	/**
	 * @brief system_mqueue struct
	 */
	typedef vandor_file system_file;

#else
#define MAX_FILE_NAME_SIZE 64
	typedef struct {
		FILE *fp;
		char name[MAX_FILE_NAME_SIZE];
		char mode;
	} system_file;
#endif

#if !defined(CONFIG_DISABLE_MOUNTPOINT) && \
	!defined(CONFIG_DISABLE_PSEUDOFS_OPERATIONS) && defined(CONFIG_BCH)
enum fs_minor_num_e {
	FS_BLOCK_MINOR_PRIMARY = 0, // minor number primary block driver, usually for /mnt
	FS_BLOCK_MINOR_SECONDARY = 1 // usually for /ext
};

typedef enum fs_minor_num_e system_fs_minor_t;
#endif

system_result system_file_is_exist(char *file_name, bool *is_exist, int *file_size);


/**
 * @brief file open
 *
 * @param[in]  file	  :  tizenrt file
 * @param[in]  file_name  :  tizenrt file name
 * @param[in]  open_name  :  tizenrt file open mode "r","w" mode (only over write mode)
 *
 * @return SYSTEM_SUCCESS        :  success
 * @return SYSTEM_FAIL           :  fail
 * @return SYSTEM_INVALID_ARGS   :  input parameter invalid
 */
system_result system_file_open(system_file *file, char *file_name, char *open_mode);


/**
 * @brief file close
 *
 * @param[in]  file	  :  tizenrt file
 *
 * @return SYSTEM_SUCCESS        :  success
 * @return SYSTEM_FAIL           :  fail
 * @return SYSTEM_INVALID_ARGS   :  input parameter invalid
 * @return SYSTEM_RESOURCE_BUSY  :  another task using resource
 */
system_result system_file_close(system_file *file);



/**
 * @brief file read
 *
 * @param[in]   file	   :  tizenrt file
 * @param[out]  buffer     :  read buffer
 * @param[in]   read_size  :  read data size
 *
 * @return SYSTEM_SUCCESS       :  success
 * @return SYSTEM_FAIL          :  fail
 * @return SYSTEM_INVALID_ARGS  :  input parameter invalid
 */
system_result system_file_read(system_file *file, unsigned char *buffer, unsigned int read_size);


/**
 * @brief file write
 *
 * @param[in]   file	    :  tizenrt file
 * @param[out]  data        :  write data buffer pointer
 * @param[in]   write_size  :  write size (bytes)
 *
 * @return SYSTEM_SUCCESS       :  success
 * @return SYSTEM_FAIL          :  fail
 * @return SYSTEM_INVALID_ARGS  :  input parameter invalid
 */
system_result system_file_write(system_file *file, unsigned char *data, unsigned int write_size);


system_result system_file_delete(char *file_name);


/**
 * @brief set the file position
 *
 * @param[in]  file	  :  tizenrt file
 * @param[in]  file_size  :  file_size
 *
 * @return SYSTEM_SUCCESS       :  success
 * @return SYSTEM_FAIL          :  fail
 * @return SYSTEM_INVALID_ARGS  :  input parameter invalid
 */
system_result system_file_get_size(system_file *file, unsigned int *file_size);

system_result system_file_make_directory(char *directory_path);

system_result system_file_crc_check(char *file_name, bool *is_crc_correct);

#if !defined(CONFIG_DISABLE_MOUNTPOINT) && \
	!defined(CONFIG_DISABLE_PSEUDOFS_OPERATIONS) && defined(CONFIG_BCH)
/**
 * @brief low format target block driver which binded specific file system
 *
 * @param[in]  system_fs_minor_t   :  minor number of block driver
 *
 * @return SYSTEM_SUCCESS	   :  success
 * @return SYSTEM_FAIL		   :  fail
 * @return SYSTEM_INVALID_ARGS  :  input parameter invalid
 */
system_result system_file_format_filesystem(system_fs_minor_t minor);

/**
 * @brief low format target block drivers which binded specific file system
 *
 * @return SYSTEM_SUCCESS	   :  success
 * @return SYSTEM_FAIL		   :  fail
 * @return SYSTEM_INVALID_ARGS  :  input parameter invalid
 */
system_result system_file_format_filesystem_all(void);

/**
 * @brief Corrupt block driver to make file system error forcely before test format
 *
 * @param[in]  system_fs_minor_t   :  minor number of block driver
 *
 * @return SYSTEM_SUCCESS	   :  success
 * @return SYSTEM_FAIL		   :  fail
 * @return SYSTEM_INVALID_ARGS  :  input parameter invalid
 */
system_result system_file_corrupt_filesystem(system_fs_minor_t minor);

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

/**
 * @}
 */ // end file group
#endif
