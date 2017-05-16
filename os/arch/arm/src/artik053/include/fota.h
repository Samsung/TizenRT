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

#ifndef __ARCH_ARM_SRC_ARTIK053_INCLUDE_FOTA_H__
#define __ARCH_ARM_SRC_ARTIK053_INCLUDE_FOTA_H__

/******************************************************************************
 * Included Files
 *****************************************************************************/

#include <tinyara/config.h>
#include <arch/board/partitions.h>

/******************************************************************************
 * Pre-processor Definitions
 *****************************************************************************/

/****************************************************************************
* Public Types
****************************************************************************/
#ifndef __ASSEMBLY__
enum fota_partition_id_e {
	FOTA_PARTITION_OTA0 = 0,
	FOTA_PARTITION_OTA1,
	FOTA_PARTITION_MAX,
};

typedef enum fota_partition_id_e fota_partition_id_t;

enum fota_bin_id_e {
	FOTA_BIN_ID_TINYARA = 0,
	FOTA_BIN_ID_LEDFW,
	FOTA_BIN_ID_SSSFW,
	FOTA_BIN_ID_WLANFW,
	FOTA_BIN_ID_SSSKEY,
	FOTA_BIN_ID_MAX,
};

typedef enum fota_bin_id_e fota_bin_id_t;

enum boot_mode_status_e {
	BOOTMODE_NORMALBOOT = 0,
	BOOTMODE_REBOOT,
	BOOTMODE_WATCHDOG_RESET,
	BOOTMODE_FOTA_UPDATE,
	BOOTMODE_FOTA_RESET,
	BOOTMODE_MAX,
};

typedef enum boot_mode_status_e boot_mode_status_t;

enum fota_state_e {
	FOTA_STATE_COMPLETED = 0,
	FOTA_STATE_PROGRESSING,
	FOTA_STATE_FAILED,
	FOTA_STATE_MAX,
};

typedef enum fota_state_e fota_state_t;

enum fota_binstate_e {
	FOTA_BINSTATE_NORMAL = 0,
	FOTA_BINSTATE_UPDATED,
	FOTA_BINSTATE_COPYING,
	FOTA_BINSTATE_COPIED,
	FOTA_BINSTATE_MAX,
};

typedef enum fota_binstate_e fota_binstate_t;

struct boot_param_s {
	boot_mode_status_t boot_mode;
	fota_state_t fota_state;
	uint32_t cur_partition;
	uint32_t fota_try_count;
	uint32_t fota_max_try;
	fota_binstate_t fota_bins_state[FOTA_BIN_ID_MAX];
};

typedef struct boot_param_s boot_param_t;

/****************************************************************************
* Public Data
****************************************************************************/
#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif
/****************************************************************************
* Inline Functions
****************************************************************************/

/****************************************************************************
* Public Functions
****************************************************************************/
/****************************************************************************
 * Name: s5jt200_fota_init
 *
 * Description:
 *   Intialize FOTA for s5jt200
 *
 ****************************************************************************/
int s5jt200_fota_init(void);

/****************************************************************************
 * Name: s5jt200_fota_update_notify
 *
 * Description:
 *   Notify if fota update done in previous boot
 *
 ****************************************************************************/
int s5jt200_fota_update_notify(void);

#undef EXTERN
#ifdef __cplusplus
}
#endif
#endif

#endif /* __ARCH_ARM_SRC_ARTIK053_INCLUDE_FOTA_H__ */
