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
/************************************************************************************
 * include/tinyara/input/touchscreen.h
 *
 *   Copyright (C) 2011-2012, 2017 Gregory Nutt. All rights reserved.
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
 ************************************************************************************/

/* The TOUCHSCREEN driver exports a standard character driver interface. By
 * convention, the touchscreen driver is registers as an input device at
 * /dev/inputN where N uniquely identifies the driver instance.
 *
 * This header file documents the generic interface that all tinyara
 * touchscreen devices must conform.  It adds standards and conventions on
 * top of the standard character driver interface.
 */

#ifndef __INCLUDE_TINYARA_MMWAVE_DEV_H
#define __INCLUDE_TINYARA_MMWAVE_DEV_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <tinyara/config.h>

#include <tinyara/fs/ioctl.h>
#include <semaphore.h>

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/* IOCTL Commands *******************************************************************/
#define MMWAVE_PAUSE       _SNIOC(0)
#define MMWAVE_RESUME        _SNIOC(1)

			  /* Four common commands */

#define MMWAVE_DEV_PATH "/dev/mmwave0" // Mmwave driver node path

/* User defined ioctl commands are also supported.  However, the TSC driver must
 * reserve a block of commands as follows in order prevent IOCTL command numbers
 * from overlapping.
 *
 * This is generally done as follows.  The first reservation for TSC driver A would
 * look like:
 *
 *   TSC_A_FIRST                 (TSC_FIRST + TSC_NCMDS)     <- First command
 *   TSC_A_NCMDS                 42                          <- Number of commands
 *
 * IOCTL commands for TSC driver A would then be defined in a TSC A header file like:
 *
 *   TSCIOC_A_CMD1               _TSIOC(TSC_A_FIRST+0)
 *   TSCIOC_A_CMD2               _TSIOC(TSC_A_FIRST+1)
 *   TSCIOC_A_CMD3               _TSIOC(TSC_A_FIRST+2)
 *   ...
 *   TSCIOC_A_CMD42              _TSIOC(TSC_A_FIRST+41)
 *
 * The next reservation would look like:
 *
 *   TSC_B_FIRST                 (TSC_A_FIRST + TSC_A_NCMDS) <- Next command
 *   TSC_B_NCMDS                 77                          <- Number of commands
 */

/************************************************************************************
 * Public Types
 ************************************************************************************/

/*
 * This structure is upper level driver. This provides information about
 * Memory is provided by caller. It is not copied by the driver and is presumed to persist
 * while the driver is active.
 */
struct mmwave_dev_s {
	sem_t sem;
	uint8_t crefs;
	sem_t pollsem;
	sem_t read_sem;
	struct pollfd *fds[CONFIG_MMWAVE_NPOLLWAITERS];
	void *priv;					 /* Used by the TSP-specific logic */
	struct mmwave_ops_s *ops;
};

struct mmwave_ops_s {
	CODE void (*mmwave_pause)(FAR struct mmwave_dev_s *dev);
	CODE void (*mmwave_resume)(FAR struct mmwave_dev_s *dev);
	CODE void (*mmwave_copy_buffer)(FAR struct mmwave_dev_s *dev, uint8_t **buffer);
};
#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* __INCLUDE_TINYARA_MMWAVE_DEV_H */
