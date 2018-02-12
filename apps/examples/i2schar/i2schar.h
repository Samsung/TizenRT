/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
 * examples/examples/i2schar/i2schar.h
 *
 *   Copyright (C) 2013 Gregory Nutt. All rights reserved.
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

#ifndef __APPS_EXAMPLES_I2SCHAR_I2SCHAR_H
#define __APPS_EXAMPLES_I2SCHAR_I2SCHAR_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <pthread.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Configuration ************************************************************/
/* CONFIG_BUILTIN_APPS - Build the I2SCHAR test as a built-in
 *   function. Default: Built as a standalone program
 * CONFIG_EXAMPLES_I2SCHAR_DEVPATH - The default path to the I2S character
 *   device. Default: /dev/i2schar0
 */

#ifndef CONFIG_AUDIO_I2SCHAR
#error "I2S character device support is not enabled (CONFIG_AUDIO_I2SCHAR)"
#endif

#ifndef CONFIG_EXAMPLES_I2SCHAR_DEVPATH
#define CONFIG_EXAMPLES_I2SCHAR_DEVPATH "/dev/i2schar0"
#endif

#ifdef CONFIG_EXAMPLES_I2SCHAR_TX
#ifndef CONFIG_EXAMPLES_I2SCHAR_TXBUFFERS
#define CONFIG_EXAMPLES_I2SCHAR_TXBUFFERS 4
#endif
#ifndef CONFIG_EXAMPLES_I2SCHAR_TXSTACKSIZE
#define CONFIG_EXAMPLES_I2SCHAR_TXSTACKSIZE 1536
#endif
#else
#undef CONFIG_EXAMPLES_I2SCHAR_TXBUFFERS
#define CONFIG_EXAMPLES_I2SCHAR_TXBUFFERS 0
#endif

#ifdef CONFIG_EXAMPLES_I2SCHAR_RX
#ifndef CONFIG_EXAMPLES_I2SCHAR_RXBUFFERS
#define CONFIG_EXAMPLES_I2SCHAR_RXBUFFERS 4
#endif
#ifndef CONFIG_EXAMPLES_I2SCHAR_RXSTACKSIZE
#define CONFIG_EXAMPLES_I2SCHAR_RXSTACKSIZE 1536
#endif
#else
#undef CONFIG_EXAMPLES_I2SCHAR_RXBUFFERS
#define CONFIG_EXAMPLES_I2SCHAR_RXBUFFERS 0
#endif

#ifndef CONFIG_EXAMPLES_I2SCHAR_BUFSIZE
#define CONFIG_EXAMPLES_I2SCHAR_BUFSIZE 256
#endif

/****************************************************************************
 * Public Types
 ****************************************************************************/

struct i2schar_state_s {
	bool initialized;

	FAR char *devpath;
#ifdef CONFIG_EXAMPLES_I2SCHAR_TX
	int txcount;
#endif
#ifdef CONFIG_EXAMPLES_I2SCHAR_RX
	int rxcount;
#endif
};

/****************************************************************************
 * Public Variables
 ****************************************************************************/

extern struct i2schar_state_s g_i2schar;

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: i2schar_devinit()
 *
 * Description:
 *   Perform architecuture-specific initialization of the I2s hardware and
 *   registration of the I2S character driver CHAR hardware.  If
 *   CONFIG_EXAMPLES_I2SCHAR_DEVINIT is defined, then board-specific logic
 *   must provide this function.
 *
 ****************************************************************************/

#ifdef CONFIG_EXAMPLES_I2SCHAR_DEVINIT
int i2schar_devinit(void);
#endif

/****************************************************************************
 * Name: i2schar_transmitter()
 *
 * Description:
 *   This is the entry point for the transmitter thread.
 *
 ****************************************************************************/

#ifdef CONFIG_EXAMPLES_I2SCHAR_TX
pthread_addr_t i2schar_transmitter(pthread_addr_t arg);
#endif

/****************************************************************************
 * Name: i2schar_receiver()
 *
 * Description:
 *   This is the entry point for the receiver thread.
 *
 ****************************************************************************/

#ifdef CONFIG_EXAMPLES_I2SCHAR_RX
pthread_addr_t i2schar_receiver(pthread_addr_t arg);
#endif

#endif							/* __APPS_EXAMPLES_I2SCHAR_I2SCHAR_H */
