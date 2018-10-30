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
 * system/cu/cu.h
 *
 *   Copyright (C) 2014 sysmocom - s.f.m.c. GmbH. All rights reserved.
 *   Author: Harald Welte <hwelte@sysmocom.de>
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

#ifndef __APPS_SYSTEM_CU_CUTERM_H
#define __APPS_SYSTEM_CU_CUTERM_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Configuration ***********************************************************/

#ifndef CONFIG_SYSTEM_CUTERM_DEFAULT_DEVICE
#define CONFIG_SYSTEM_CUTERM_DEFAULT_DEVICE "/dev/ttyS0"
#endif

#ifndef CONFIG_SYSTEM_CUTERM_DEFAULT_BAUD
#define CONFIG_SYSTEM_CUTERM_DEFAULT_BAUD 115200
#endif

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* All terminal state data is packaged in a single structure to minimize
 * name conflicts with other global symbols -- a poor man's name space.
 */

struct cu_globals_s {
	int infd;					/* Incmoming data from serial port */
	int outfd;					/* Outgoing data to serial port */
	pthread_t listener;			/* Terminal listener thread */
};

/****************************************************************************
 * Public Variables
 ****************************************************************************/

/* terminal state data */

extern struct cu_globals_s g_cu;

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#endif							/* __APPS_SYSTEM_CU_CUTERM_H */
