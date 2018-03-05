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
#ifndef __APPS_INCLUDE_SYSTEM_FOTA_HAL_H
#define __APPS_INCLUDE_SYSTEM_FOTA_HAL_H
/****************************************************************************
* Included Files
****************************************************************************/
#include <tinyara/config.h>
#include <sys/types.h>

/****************************************************************************
* Pre-processor Definitions
****************************************************************************/

/****************************************************************************
* Public Types
****************************************************************************/
#ifndef __ASSEMBLY__
enum fota_partition_id_s {
	FOTA_PARTITION_OTA0 = 0,
	FOTA_PARTITION_OTA1,
	FOTA_PARTITION_MAX,
};

typedef enum fota_partition_id_s fota_partition_id_t;

struct fotahal_bin_header_s {
	uint32_t fotahal_magic;
	uint32_t fotahal_bin_id;
	uint32_t fotahal_bin_partaddr;
	uint32_t fotahal_bin_size;
	uint32_t fotahal_bin_reserved[3];
	uint32_t fotahal_checksum;
};

typedef struct fotahal_bin_header_s fotahal_bin_header_t;

enum fotahal_return_e {
	FOTAHAL_RETURN_SUCCESS = 0,
	FOTAHAL_RETURN_ERROR,
	FOTAHAL_RETURN_OPENFAIL,
	FOTAHAL_RETURN_WRITEFAIL,
	FOTAHAL_RETURN_ERASEFAIL,
	FOTAHAL_RETURN_CLOSEFAIL,
	FOTAHAL_RETURN_CHECKSUMFAIL,
	FOTAHAL_RETURN_WRONGMAGIC,
	FOTAHAL_RETURN_WRONGBIN,
	FOTAHAL_RETURN_WRONGSIZE,
	FOTAHAL_RETURN_WRONGHANDLE,
	FOTAHAL_RETURN_PART_NOTSET,
	FOTAHAL_RETURN_BIN_NOTSET,
	FOTAHAL_RETURN_PARAM_NOTSET,
	FOTAHAL_RETURN_MAX,
};

typedef enum fotahal_return_e fotahal_return_t;

typedef void *fotahal_handle_t;

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
* Public Function Prototypes
****************************************************************************/
/****************************************************************************
 * Name: fotahal_open
 *
 * Description:
 *   open fotahal layer
 ****************************************************************************/
fotahal_handle_t fotahal_open(void);

#ifdef CONFIG_SYSTEM_FOTA_SET_SPECIFIC
/****************************************************************************
 * Name: fotahal_get_partition
 *
 * Description:
 *   Get current running partition id
 *   Return -1 on failure, on Success return part_id
 ****************************************************************************/
fota_partition_id_t fotahal_get_partition(fotahal_handle_t handle);

/****************************************************************************
 * Name: fotahal_set_partition
 *
 * Description:
 *   Set a new partition for fota write
 ****************************************************************************/
fotahal_return_t fotahal_set_partition(fotahal_handle_t handle, fota_partition_id_t part_id);

/****************************************************************************
 * Name: fotahal_set_binary
 *
 * Description:
 *   Set a binary type for fota write
 ****************************************************************************/
fotahal_return_t fotahal_set_binary(fotahal_handle_t handle, uint32_t bin_id);
#endif

/****************************************************************************
 * Name: fotahal_write
 *
 * Description:
 *   write binary chunck in buffer to fota partition
 ****************************************************************************/
fotahal_return_t fotahal_write(fotahal_handle_t handle, const char *buffer, uint32_t bin_size);

/****************************************************************************
 * Name: fotahal_erase
 *
 * Description:
 *   erase fota partition
 ****************************************************************************/
fotahal_return_t fotahal_erase(fotahal_handle_t handle);

/****************************************************************************
 * Name: fotahal_update_bootparam
 *
 * Description:
 *   Update boot param
 ****************************************************************************/
fotahal_return_t fotahal_update_bootparam(fotahal_handle_t handle);

/****************************************************************************
 * Name: fotahal_close
 *
 * Description:
 *   close fotahal
 ****************************************************************************/
fotahal_return_t fotahal_close(fotahal_handle_t handle);

/****************************************************************************
 * Name: fotahal_update
 *
 * Description:
 *   update to new
 ****************************************************************************/
#ifdef CONFIG_BOARDCTL_RESET
int fotahal_update(void);
#endif
#undef EXTERN
#ifdef __cplusplus
}
#endif	/* __cplusplus  */
#endif	/* __ASSEMBLY__ */
#endif	/* __APPS_INCLUDE_SYSTEM_FOTA_HAL_H */
