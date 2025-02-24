/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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
 * include/tinyara/sensors/sensor.h
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

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
#define SENSOR_VERIFY _SNIOC(9)

struct sensor_ops_s {
	int (*sensor_read)(struct sensor_upperhalf_s *priv, FAR void *buffer);
	void (*sensor_set_mclk)(struct sensor_upperhalf_s *priv, int mclk);
	void (*sensor_set_bclk)(struct sensor_upperhalf_s *priv, int bclk);
	void (*sensor_start)(struct sensor_upperhalf_s *priv);
	void (*sensor_stop)(struct sensor_upperhalf_s *priv);
	void (*sensor_setchannel_count)(struct sensor_upperhalf_s *priv, int channel_count);
	void (*sensor_setbit_perchannel)(struct sensor_upperhalf_s *priv, int bit_per_channel);
	void (*sensor_set_samprate)(struct sensor_upperhalf_s *priv, int samp_rate);
	void (*sensor_verify)();
};

#define CONFIG_SENSOR_NPOLLWAITERS 2

/* This structure describes the state of the upper half driver */
struct sensor_upperhalf_s {
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

typedef struct sensor_data_s {
	float x;
	float y;
	float z;
} sensor_data_s;

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

