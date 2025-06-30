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

#ifndef __DRIVERS_SENSOR_AIS25BA_H
#define __DRIVERS_SENSOR_AIS25BA_H
#include <tinyara/config.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>

#include <tinyara/i2c.h>
#include <tinyara/audio/audio.h>
#include <tinyara/audio/i2s.h>

#include <tinyara/sensors/sensor.h>
#include <tinyara/sensors/ais25ba.h>

#define AIS25BA_WHOAMI_REGISTER 0xFF
#define AIS25BA_WHOAMI_VALUE 0x20
#define AIS25BA_TEST_REG 0x0B          /* Enable Self Test mode */
#define AIS25BA_TDM_CTRL_REG 0X2E      /* Control register */
#define AIS25BA_CTRL_REG_1 0x26        /* Control register, 0: Normal mode, 1: Disabled mode */      
#define AIS25BA_CTRL_REG_2 0X2F        /* Control register */
#define AIS25BA_CTRL_REG_FS 0x30       /* Accelerometer full-scale selection */
#endif /* __DRIVERS_SENSOR_AIS25BA_H */

