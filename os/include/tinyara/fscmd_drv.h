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

#ifndef __INCLUDE_TINYARA_FSCMD_DRV_H
#define __INCLUDE_TINYARA_FSCMD_DRV_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/fs/fs.h>

#define FSCMD_DRVPATH			"/dev/fscmd"

struct foreach_mountpoint_s {
	foreach_mountpoint_t handler;
	FAR void *arg;
};

struct ramdisk_info_s {
	int minor;
	int sectsize;
	uint8_t *buffer;
	uint8_t rdflags;
	uint32_t nsectors;
};

struct mksmartfs_info_s {
	char *pathname;
	uint8_t nrootdirs;
	bool force;
};

void fscmd_drv_register(void);

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

#endif /* __INCLUDE_TINYARA_FSCMD_DRV_H */
