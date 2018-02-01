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
 * include/sys/mount.h
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
 * @defgroup MOUNT_KERNEL MOUNT
 * @brief Provides APIs for Mount
 * @ingroup KERNEL
 *
 * @{
 */

/// @file sys/mount.h
/// @brief mount APIs

#ifndef __INCLUDE_SYS_MOUNT_H
#define __INCLUDE_SYS_MOUNT_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

/****************************************************************************
 * Pre-Processor Definitions
 ****************************************************************************/

/* Mount flags */

#define MS_RDONLY 1				/* Mount file system read-only */

/****************************************************************************
 * Public Type Definitions
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

/**
 * @ingroup MOUNT_KERNEL
 * @brief attaches filesystem to the root filesystem at the target path
 * @details @b #include <sys/mount.h> \n
 * SYSTEM CALL API \n
 * @param[in] source the name of block device to be attached
 * @param[in] target the root path of filesystem
 * @param[in] filesystemtype the type of filesystem
 * @param[in] mountflags the flags for mount
 * @param[in] data mountpoint private data
 * @return On success, Zero is returned. On failure, -1 is returned and errno is set appropriately.
 * @since TizenRT v1.0
 */
int mount(const char *source, const char *target, const char *filesystemtype, unsigned long mountflags, const void *data);
/**
 * @ingroup MOUNT_KERNEL
 * @brief detaches the filesystem mounted at the target path
 * @details @b #include <sys/mount.h> \n
 * SYSTEM CALL API \n
 * @param[in] target the root path of filesystem to be detached
 * @return On success, Zero is returned. On failure, -1 is returned and errno is set appropriately.
 * @since TizenRT v1.0
 */
int umount(const char *target);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif							/* __INCLUDE_SYS_MOUNT_H */
/**
 * @}
 */
