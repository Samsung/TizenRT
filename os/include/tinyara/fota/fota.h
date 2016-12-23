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

#ifndef __INCLUDE_FOTA_FOTA_H
#define __INCLUDE_FOTA_FOTA_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>
#include <tinyara/fs/fs.h>

/************************************************************************************
 * Public Types
 ************************************************************************************/
typedef enum fota_bootparam_type_e {
	FOTA_BOOTPARAM_TYPE_BOOTMODE = 0,
	FOTA_BOOTPARAM_TYPE_BINSTATE,
	FOTA_BOOTPARAM_TYPE_MAX,
} fota_bootparam_type_t;

struct fota_dev_s {
	/* Get current OTA partiton */
	CODE int (*fota_get_partition)(void);

	/* Set OTA partiton for FOTA update */
	CODE int (*fota_set_partition)(uint32_t part_id);

	/* Set Binary Type for FOTA update */
	CODE int (*fota_set_binary)(uint32_t bin_id);

	/* Erase selected partition */
	CODE int (*fota_set_bootparam)(fota_bootparam_type_t bootparam_type);

	/* Read buflen charachter's  the selected bin partition, to buffer */
	CODE int (*fota_read)(FAR const char *buffer, size_t buflen);

	/* Write buflen charachter in buffer to the selected bin partition */
	CODE int (*fota_write)(FAR const char *buffer, size_t buflen);

	/* Flush any left-out write */
	CODE int (*fota_write_flush)(void);

	/* Erase selected partition */
	CODE int (*fota_erase)(void);
};

typedef struct fota_dev_s fota_dev_t;

/************************************************************************************
 * Public Data
 ************************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: fota_register
 *
 * Description:
 *   Register FOTA Driver
 *
 ************************************************************************************/

int fota_register(FAR fota_dev_t *dev);

#undef EXTERN
#if defined(__cplusplus)
}
#endif
#endif							/* __INCLUDE_FOTA_FOTA_H */
