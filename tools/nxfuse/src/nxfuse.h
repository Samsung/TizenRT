/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
 * nxfuse - A FUSE filesystem for mounting NuttX FS natively under Linux.
 *
 * src/nxfuse.h:   Prototypes and definitions for nxfuse
 *
 *   Copyright (C) 2016 Ken Pettit. All rights reserved.
 *   Author: Ken Pettit <pettitkd@gmail.com>
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

#ifndef _SRC_NXFUSE_H
#define _SRC_NXFUSE_H

#define FIOUTIME        _FIOC(0x0007)  /* IN:  None
                                        * OUT: OK if successful, -ENOSYS if not
                                        */
#define FIOCHMOD        _FIOC(0x0008)  /* IN:  New mode flags (int)
                                        * OUT: OK if successful, -ENOSYS if not
                                        */

/* The logical sector number of the root directory. */

#define SMARTFS_ROOT_DIR_SECTOR   3

/* Defines the sector types */

#define SMARTFS_SECTOR_TYPE_DIR   1
#define SMARTFS_SECTOR_TYPE_FILE  2

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* nxfuse private data state control context */

struct nxfuse_state {
	const char *rootdir;
	struct inode *pinode;
};

/****************************************************************************
 * Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: vmount
 *
 * Description:
 *   This is called from the FUSE main routine to virtually mount a
 *   NuttX filesystem.
 *
 ****************************************************************************/
struct inode *vmount(const char *filename, const char *mount_point, const char *fs_type, int erasesize, int sectsize, int pagesize, char *generic);

/****************************************************************************
 * Name: mkfs
 *
 * Description:
 *   This is called from main when the -m 'mkfs' option is specified.
 *   It attempts to perform a mkfs on the specified source device using the
 *   specified NuttX filesystem type.
 *
 ****************************************************************************/
int mkfs(const char *filename, const char *fs_type, uint32_t erasesize, uint16_t sectsize, int pagesize, char *generic, int confirm);

#endif							/* _SRC_NXFUSE_H */
