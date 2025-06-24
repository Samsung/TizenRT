 /*
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
 * include/tinyara/sensor/sensor.h
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

#ifndef __INCLUDE_TINYARA_SENSOR_H
#define __INCLUDE_TINYARA_SENSOR_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <tinyara/config.h>
#include <tinyara/fs/ioctl.h>


#define SENSOR_TEST _SNIOC(1)
#define SENSOR_SET_MCLK _SNIOC(2)
#define SENSOR_START _SNIOC(3)
#define SENSOR_STOP  _SNIOC(4)
#define SENSOR_SET_CHANNEL _SNIOC(5)
#define SENSOR_SET_DATABIT _SNIOC(6)
#define SENSOR_SET_SAMPRATE _SNIOC(7)
#define SENSOR_SET_BCLK _SNIOC(8)

struct sensor_ops_s {
	int (*sensor_read)(struct sensor_upperhalf_s *priv, FAR char *buffer);
	void (*sensor_set_mclk)(struct sensor_upperhalf_s *priv, int mclk);
	void (*sensor_set_bclk)(struct sensor_upperhalf_s *priv, int bclk);
	void (*sensor_start)(struct sensor_upperhalf_s *priv);
	void (*sensor_stop)(struct sensor_upperhalf_s *priv);
	void (*sensor_setchannel_count)(struct sensor_upperhalf_s *priv, int channel_count);
	void (*sensor_setbit_perchannel)(struct sensor_upperhalf_s *priv, int bit_per_channel);
	void (*sensor_set_samprate)(struct sensor_upperhalf_s *priv, int samp_rate);
};

#define CONFIG_SENSOR_NPOLLWAITERS 2
/* This structure describes the state of the upper half driver */

struct sensor_upperhalf_s
{
	sem_t sem;
	uint8_t crefs;
#ifndef CONFIG_DISABLE_POLL
	sem_t pollsem;
	struct pollfd *fds[CONFIG_SENSOR_NPOLLWAITERS];
#endif

	const struct sensor_ops_s *ops;	/* Arch-specific operations */
	void *priv;
	void (*func)(void);
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

#endif /* __INCLUDE_TINYARA_SENSOR_H */

