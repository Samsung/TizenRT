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

#ifndef __INCLUDE_TINYARA_INPUT_TOUCHSCREEN_H
#define __INCLUDE_TINYARA_INPUT_TOUCHSCREEN_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <tinyara/config.h>

#if defined(CONFIG_TOUCH)
#include <tinyara/fs/ioctl.h>
#include <tinyara/i2c.h>
#include <semaphore.h>

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/* IOCTL Commands *******************************************************************/
/* Common TSC IOCTL commands */

#define TSIOC_SETCALIB       _TSIOC(0x0001)  /* arg: Pointer to int calibration value */
#define TSIOC_GETCALIB       _TSIOC(0x0002)  /* arg: Pointer to int calibration value */
#define TSIOC_SETFREQUENCY   _TSIOC(0x0003)  /* arg: Pointer to uint32_t frequency value */
#define TSIOC_GETFREQUENCY   _TSIOC(0x0004)  /* arg: Pointer to uint32_t frequency value */
#define TSIOC_DISABLE        _TSIOC(0x0005)  /* Disable touch interrupt */
#define TSIOC_ENABLE         _TSIOC(0x0006)  /* Enable touch interrupt */
#define TSIOC_SETAPPNOTIFY   _TSIOC(0x0007)  /* arg: Pointer to struct touch_set_callback_s. Support available only when CONFIG_TOUCH_CALLBACK is enabled */
#define TSIOC_CMD            _TSIOC(0x0008)  /* arg: Pointer to struct touchscreen_cmd_s */
#define TSC_FIRST            0x0001          /* First common command */
#define TSC_NCMDS            4               /* Four common commands */

#define TOUCH_DEV_PATH "/dev/touch0"	// Touch driver node path

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

/* These definitions provide the meaning of all of the bits that may be
 * reported in the struct touch_point_s flags.
 */

#define TOUCH_DOWN           (1 << 0) /* A new touch contact is established */
#define TOUCH_MOVE           (1 << 1) /* Movement occurred with previously reported contact */
#define TOUCH_UP             (1 << 2) /* The touch contact was lost */
#define TOUCH_ID_VALID       (1 << 3) /* Touch ID is certain */
#define TOUCH_POS_VALID      (1 << 4) /* Hardware provided a valid X/Y position */
#define TOUCH_PRESSURE_VALID (1 << 5) /* Hardware provided a valid pressure */
#define TOUCH_SIZE_VALID     (1 << 6) /* Hardware provided a valid H/W contact size */

#if defined(CONFIG_TOUCH_IST415)
#define TOUCH_MAX_POINTS 	15    /* Maximum number of simultaneous touch point supported */
#endif
/************************************************************************************
 * Public Types
 ************************************************************************************/

/* This structure contains information about a single touch point.
 * Positional units are device specific.
 */

struct touch_point_s {
	uint8_t  id;       /* Unique identifies contact; Same in all reports for the contact */
	uint8_t  flags;    /* See TOUCH_* definitions above */
	int16_t  x;        /* X coordinate of the touch point (uncalibrated) */
	int16_t  y;        /* Y coordinate of the touch point (uncalibrated) */
	int16_t  h;        /* Height of touch point (uncalibrated) */
	int16_t  w;        /* Width of touch point (uncalibrated) */
	uint16_t pressure; /* Touch pressure */
};

/* The typical touchscreen driver is a read-only, input character device driver.
 * the driver write() method is not supported and any attempt to open the
 * driver in any mode other than read-only will fail.
 *
 * Data read from the touchscreen device consists only of touch events and
 * touch sample data.  This is reflected by struct touch_sample_s.  This
 * structure is returned by either the driver read method.
 *
 * On some devices, multiple touchpoints may be supported. So this top level
 * data structure is a struct touch_sample_s that "contains" a set of touch
 * points.  Each touch point is managed individually using an ID that identifies
 * a touch from first contact until the end of the contact.
 */

/*
 * This structure contains the array that have information about each simultaneous touch point.
 */
struct touch_sample_s {
	int npoints;                   /* The number of touch points in point[] */
	struct touch_point_s point[TOUCH_MAX_POINTS]; /* Actual dimension is npoints */
};

struct touchscreen_cmd_s {
	int argc;
	char **argv;
};

#if defined(CONFIG_TOUCH_CALLBACK)

struct touch_set_callback_s {
	struct touch_sample_s *touch_points;
	void (*is_touch_detected)(int);
};

#endif /* CONFIG_TOUCH_CALLBACK */

struct touch_sample_buffer_s {
	sem_t sem;								/* Used to control exclusive access to the buffer */
	volatile int16_t head;					/* Index to the head [IN] index in the buffer */
	volatile int16_t tail;					/* Index to the tail [OUT] index in the buffer */
	int16_t size;							/* The allocated size of the buffer */
	struct touch_sample_s *buffer;			/* Pointer to the allocated buffer memory */
};

/*
 * This structure is upper level driver operations which will use lower level calls internally
 */
struct touchscreen_ops_s {
	void (*touch_enable)(struct touchscreen_s *upper);			/* Enable touch */
	void (*touch_disable)(struct touchscreen_s *upper);			/* Disable touch */
	int (*cmd)(struct touchscreen_s *upper, int argc, char **argv);
};

/*
 * This structure is upper level driver. This provides information about
 * Memory is provided by caller. It is not copied by the driver and is presumed to persist
 * while the driver is active.
 */
struct touchscreen_s {
	sem_t sem;
	uint8_t crefs;
#if !defined(CONFIG_DISABLE_POLL)
	sem_t pollsem;
	struct pollfd *fds[CONFIG_TOUCH_NPOLLWAITERS];
#endif

	struct touch_sample_buffer_s tp_buf;
	sem_t waitsem;

	const struct touchscreen_ops_s *ops;	/* Arch-specific operations */
	void *priv;		/* Used by the TSP-specific logic */

#if defined(CONFIG_TOUCH_CALLBACK)
	/* Below variables are set by UI using IOCTL during initialization */
	struct touch_sample_s *app_touch_point_buffer;		/* Buffer to store touch point allocated by UI */
	void (*is_touch_detected)(int);	/* Callback function to notify touch event to application */
#endif /* CONFIG_TOUCH_CALLBACK */
};

#define SIZEOF_TOUCH_SAMPLE_S(n) (sizeof(struct touch_sample_s) + ((n) - 1) * sizeof(struct touch_point_s))

/************************************************************************************
 * Public Function Prototypes
 ************************************************************************************/

void touch_report(struct touchscreen_s *dev, struct touch_sample_s *data);

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

#endif /* CONFIG_TOUCH */
#endif /* __INCLUDE_TINYARA_INPUT_TOUCHSCREEN_H */

