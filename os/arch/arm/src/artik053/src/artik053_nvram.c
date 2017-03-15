/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics Co., LTD. All Rights Reserved.
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
 * arch/arm/src/artik053/artik053_nvram.c
 *
 *   Copyright (C) 2015 Gregory Nutt. All rights reserved.
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
 * 3. Neither the name tinyara nor the names of its contributors may be
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

#include <tinyara/config.h>

#include <fcntl.h>
#include <stdbool.h>
#include <debug.h>

#include <tinyara/configdata.h>

#include "artik053.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/
unsigned int Nv_Write(unsigned int target_addr, unsigned int source_addr,
		unsigned int sizebyte)
{
#if defined(CONFIG_MTD_CONFIG)
	int fd;
	struct config_data_s config;

	fd = open("/dev/config", O_RDOK);
	if (fd < 0) {
		lldbg("Failed to open /dev/config\n");
		return false;
	}

	config.id = ARTIK053_CONFIGDATA_WIFI_NVRAM;
	config.instance = 0;
	config.configdata = (unsigned char *)source_addr;
	config.len = sizebyte;

	ioctl(fd, CFGDIOC_SETCONFIG, &config);
	close(fd);

	return true;
#else
	return false;
#endif
}

unsigned int Nv_Read(unsigned int target_addr, unsigned int source_addr,
		unsigned int sizebyte)
{
#if defined(CONFIG_MTD_CONFIG)
	int fd;
	struct config_data_s config;

	fd = open("/dev/config", O_RDOK);
	if (fd < 0) {
		lldbg("Failed to open /dev/config\n");
		return false;
	}

	config.id = ARTIK053_CONFIGDATA_WIFI_NVRAM;
	config.instance = 0;
	config.configdata = (unsigned char *)target_addr;
	config.len = sizebyte;

	ioctl(fd, CFGDIOC_GETCONFIG, &config);
	close(fd);

	return true;
#else
	return false;
#endif
}

unsigned int Nv_Erase(unsigned int target_addr, unsigned int sizebyte)
{
#if defined(CONFIG_MTD_CONFIG)
	return true;
#else
	return false;
#endif
}
