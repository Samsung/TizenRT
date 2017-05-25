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
 * arch/arm/src/s5j/s5j_pwr.c
 *
 *   Copyright (C) 2011 Uros Platise. All rights reserved.
 *   Copyright (C) 2013, 2015 Gregory Nutt. All rights reserved.
 *   Authors: Uros Platise <uros.platise@isotel.eu>
 *            Gregory Nutt <gnutt@nuttx.org>
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

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>

#include <tinyara/arch.h>

#include <stdint.h>
#include <stdbool.h>
#include <errno.h>

#include "up_arch.h"
#include "s5j_pwr.h"
#include "cache.h"

#if defined(CONFIG_S5J_PWR)

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
typedef struct __attribute__((__packed__))
{
	unsigned CR4_DBGRSTREQ:1;
	unsigned RSVD4:3;
	unsigned CM0_SYSRESETREQ:1;
	unsigned RSVD3:11;
	unsigned PINRESET:1;
	unsigned RSVD2:6;
	unsigned CR4_WDTRESET:1;
	unsigned RSVD1:4;
	unsigned WRESET:1;
	unsigned SWRESET:1;
	unsigned RSVD0:2;
} RST_STAT_SFR;

typedef struct __attribute__((__packed__))
{
	unsigned EINT:1;
	unsigned RSVD2:2;
	unsigned TRTC_ALARM:1;
	unsigned TRTC_TICK:1;
	unsigned RSVD1:7;
	unsigned UART0_RXD:1;
	unsigned I2C0_SDA:1;
	unsigned I2C1_SDA:1;
	unsigned RSVD0:16;
	unsigned WAKEUP:1;
} WAKEUP_STAT_SFR;

typedef struct __attribute__((__packed__))
{
	unsigned EINT0:1;
	unsigned EINT1:1;
	unsigned EINT2:1;
	unsigned RSCD0:29;
} EINT_WAKEUP_MASK_SFR;

typedef struct __attribute__((__packed__))
{
	unsigned RSVD3:3;
	unsigned TRTC_ALARM:1;
	unsigned TRTC_TICK:1;
	unsigned RSVD2:7;
	unsigned UART0_RXD:1;
	unsigned I2C0_SDA:1;
	unsigned I2C1_SDA:1;
	unsigned RSVD1:13;
	unsigned SFR:1;
	unsigned RSVD0:1;
	unsigned USE_LEVEL_TRIGGER:1;
	unsigned ENABLE_WAKEUP_SW:1;
} WAKEUP_MASK_SFR;

typedef struct __attribute__((__packed__))
{
	unsigned RSVD3:3;
	unsigned CLR_CR4:1;
	unsigned RSVD2:3;
	unsigned STATUS_CR4:1;
	unsigned RSVD1:3;
	unsigned DIRECT_INTR_CR4:1;
	unsigned RSVD0:19;
	unsigned ENABLE_CR4:1;
} WAKEUP_INTERRUPT_SFR;

/****************************************************************************
 * Private Data
 ****************************************************************************/
struct _PMU_SYS_PWR_CONF {
	unsigned int addr;
	unsigned char base;
	unsigned char mask;
	unsigned int dstop;
	unsigned int sleep;
	unsigned int stop;
};

int pmu_open(FAR struct file *filep);
static int pmu_ioctl(FAR struct file *filep, int cmd, unsigned long arg);
static ssize_t pmu_read(FAR struct file *filep, FAR char *buffer, size_t buflen);
static ssize_t pmu_write(FAR struct file *filep, FAR const char *buffer, size_t buflen);

static const struct file_operations pmu_fops = {
	pmu_open,				/* open */
	0,						/* close */
	pmu_read,				/* read */
	pmu_write,				/* write */
	0,						/* seek */
	pmu_ioctl				/* ioctl */
#ifndef CONFIG_DISABLE_POLL
	, 0						/* poll */
#endif
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/
enum _PMU_CMD_ {
	PMU_CMD_RESET = 1,
	PMU_CMD_LOCAL,
	PMU_CMD_SET_DBG,
	PMU_CMD_READ,
	PMU_CMD_PWR,
	PMU_CMD_SET_EINT,
	PMU_CMD_I2S,
};

int pmu_open(FAR struct file *filep)
{
	return true;
}

static ssize_t pmu_read(FAR struct file *filep, FAR char *buffer, size_t buflen)
{
	return true;
}

static ssize_t pmu_write(FAR struct file *filep, FAR const char *buffer, size_t buflen)
{
	return true;
}

static int pmu_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	switch (cmd) {
	case PMU_CMD_RESET:
		break;
	case PMU_CMD_LOCAL:
		break;
	case PMU_CMD_SET_DBG:
		break;
	case PMU_CMD_READ:
		break;
	case PMU_CMD_PWR:
		break;
	case PMU_CMD_SET_EINT:
		break;
	case PMU_CMD_I2S:
		break;
	default:
		break;
	}
	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
void s5j_pwr_mode(PWR_MODE emode)
{
	switch (emode) {
	case PWR_DSTOP:
		break;
	case PWR_STOP:
		break;
	}
}

void s5j_pwr_local_power(bool poweron)
{
	if (poweron) {
	} else {
	}
}
#endif /* CONFIG_S5J_PWR */
