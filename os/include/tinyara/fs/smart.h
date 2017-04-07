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
 * Sector Mapped Allocation for Really Tiny (SMART) FLASH interface
 *
 *   Copyright (C) 2013 Ken Pettit. All rights reserved.
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

#ifndef __INCLUDE_SMART_H
#define __INCLUDE_SMART_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <sys/types.h>
#include <stdint.h>
#include <tinyara/fs/fs.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Macros to hide implementation */

#define SMART_FMT_ISFORMATTED   0x01
#define SMART_FMT_HASBYTEWRITE  0x02

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* The following defines the format information for the device.  This
 * information is retrieved via the BIOC_GETFORMAT ioctl.
 */

struct smart_format_s {
	uint16_t sectorsize;		/* Size of one read/write sector */
	uint16_t availbytes;		/* Number of bytes available in each sector */
	uint16_t nsectors;			/* Total number of sectors on device */
	uint16_t nfreesectors;		/* Number of free sectors on device */
	uint8_t flags;				/* Format flags (see above) */
	uint8_t namesize;			/* Size of filenames on this volume */
#ifdef CONFIG_SMARTFS_MULTI_ROOT_DIRS
	uint8_t nrootdirentries;	/* Number of root directories on this device */
	uint8_t rootdirnum;			/* Root directory number for this dev entry */
#endif
};

/* The following defines the information for writing a logical sector
 * to the device.
 */

struct smart_read_write_s {
	uint16_t logsector;			/* The logical sector number */
	uint16_t offset;			/* Offset within the sector to write to */
	uint16_t count;				/* Number of bytes to write */
	const uint8_t *buffer;		/* Pointer to the data to write */
};

/* The following defines the procfs data exchange interface between the
 * SMART MTD and FS layers.
 */

#if defined(CONFIG_FS_PROCFS) && !defined(CONFIG_FS_PROCFS_EXCLUDE_SMARTFS)
struct smart_procfs_data_s {
#ifdef CONFIG_MTD_SMART_ERASE_DEBUG
	const uint16_t *erasecounts;	/* Pointer to the erase counts array */
	uint16_t erasesize;		/* Number of entries in the erase counts array */
#endif
};
#endif

/****************************************************************************
 * Public Data
 ****************************************************************************/
#ifdef CONFIG_SMARTFS_DYNAMIC_HEADER
uint16_t chunk_shift;
uint16_t used_block_divident;
size_t smart_sect_header_size;
#endif

#ifndef __ASSEMBLY__

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Description:
 *   This is used to format the flash in smartFs
 *
 ****************************************************************************/
void smart_set_format_status(bool value);

/****************************************************************************
 * Description:
 *   This is used to know if flash is already formated by smartFs
 *
 ****************************************************************************/
bool smart_get_format_status(void);


#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* __ASSEMBLY__ */
#endif							/* __INCLUDE_SMART_H */
