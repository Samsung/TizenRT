/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
 * include/dirent.h
 *
 *   Copyright (C) 2007-2009 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/
/**
 * @defgroup DIRENT_KERNEL DIRENT
 * @brief Provides APIs for Directory Entries
 * @ingroup KERNEL
 *
 * @{
 */

/// @file dirent.h
/// @brief Directory entries APIs

#ifndef __INCLUDE_DIRENT_H
#define __INCLUDE_DIRENT_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <limits.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* File type code for the d_type field in dirent struct.
 * Note that because of the simplified filesystem organization
 * of TinyAra, an inode can be BOTH a file and a directory
 */

#define DTYPE_FILE      0x01
#define DTYPE_CHR       0x02
#define DTYPE_BLK       0x04
#define DTYPE_DIRECTORY 0x08

#define DIRENT_ISFILE(dtype)      (((dtype) & DTYPE_FILE) != 0)
#define DIRENT_ISCHR(dtype)       (((dtype) & DTYPE_CHR) != 0)
#define DIRENT_ISBLK(dtype)       (((dtype) & DTYPE_BLK) != 0)
#define DIRENT_ISDIRECTORY(dtype) (((dtype) & DTYPE_DIRECTORY) != 0)

/****************************************************************************
 * Public Type Definitions
 ****************************************************************************/

/* The POSIX specification requires that the caller of readdir_r provide
 * storage "large enough for a dirent with the d_name member and an array
 * of char containing at least {NAME_MAX} plus one elements.
 */

struct dirent {
	uint8_t d_type;				/* type of file */
	char d_name[NAME_MAX + 1];	/* filename */
};

typedef void DIR;

/****************************************************************************
 * Public Variables
 ****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/* POSIX-like File System Interfaces */

/**
 * @ingroup DIRENT_KERNEL
 * @brief close a directory stream
 * @details @b #include <dirent.h> \n
 * SYSTEM CALL API \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int closedir(FAR DIR *dirp);
/**
 * @ingroup DIRENT_KERNEL
 * @brief open directory associated with file descriptor
 * @details @b #include <dirent.h> \n
 * SYSTEM CALL API \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
FAR DIR *opendir(FAR const char *path);
/**
 * @ingroup DIRENT_KERNEL
 * @brief read a directory
 * @details @b #include <dirent.h> \n
 * SYSTEM CALL API \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
FAR struct dirent *readdir(FAR DIR *dirp);
/**
 * @ingroup DIRENT_KERNEL
 * @brief read a directory
 * @details @b #include <dirent.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int readdir_r(FAR DIR *dirp, FAR struct dirent *entry, FAR struct dirent **result);
/**
 * @ingroup DIRENT_KERNEL
 * @brief reset the position of a directory stream to the beginning of a directory
 * @details @b #include <dirent.h> \n
 * SYSTEM CALL API \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
void rewinddir(FAR DIR *dirp);
/**
 * @ingroup DIRENT_KERNEL
 * @brief sets the location in the directory stream from which the next readdir() call will start.
 * @details @b #include <dirent.h>
 * @param[in] dirp An instance of type DIR.
 * @param[in] loc offset to seek.
 * @since TizenRT v1.0
 */
void seekdir(FAR DIR *dirp, off_t loc);
/**
 * @ingroup DIRENT_KERNEL
 * @brief gets the current location associated with the directory stream
 * @details @b #include <dirent.h>
 * @param[in] dirp An instance of type DIR
 * @return On success, the current location in the directory stream is returned. On failure, -1 is returned and errno is set appropriately.
 * @since TizenRT v1.0
 */
off_t telldir(FAR DIR *dirp);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif							/* __INCLUDE_DIRENT_H */
/**
 * @}
 */
