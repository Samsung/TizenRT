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

#ifndef __SCHED_BINARY_MANAGER_BINARY_MANAGER_H
#define __SCHED_BINARY_MANAGER_BINARY_MANAGER_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/binary_manager.h>

#ifdef CONFIG_BINARY_MANAGER

/****************************************************************************
 * Pre-Processor Definitions
 ****************************************************************************/
#define BINARY_MANAGER_NAME        "binary_manager"           /* Binary manager main thread name */
#define BINARY_MANAGER_STACKSIZE   2048                       /* Binary manager main thread stack size */
#define BINARY_MANAGER_PRIORITY    225                        /* Binary manager main thread priority */

#ifdef CONFIG_BINMGR_RECOVERY
/* Recovery Thread information */
#define RECOVERYTHD_NAME           "recovery_thread"          /* Recovery thread name */
#define RECOVERYTHD_STACKSIZE      2048                       /* Recovery thread stack size */
#define RECOVERYTHD_PRIORITY       255                        /* Recovery thread priority */
#endif

/* Loading Thread information */
#define LOADINGTHD_NAME           "loading_thread"            /* Loading thread name */
#define LOADINGTHD_STACKSIZE      2048                        /* Loading thread stack size */
#define LOADINGTHD_PRIORITY       220                         /* Loading thread priority */

/* Supported binary types */
#define BIN_TYPE_BIN               0                          /* 'bin' type for kernel binary */
#define BIN_TYPE_ELF               1                          /* 'elf' type for user binary */

/* Binary information configuration */
#define PARTS_PER_BIN              2                          /* The number of partitions per binary */
#define BIN_VER_MAX                16                         /* The maximum length of binary version */
#define KERNEL_VER_MAX             8                          /* The maximum length of kernel version */
#ifdef CONFIG_NUM_APPS
#define USER_BIN_COUNT             CONFIG_NUM_APPS
#else
#define USER_BIN_COUNT             2                          /* The number of User binaries */
#endif

#define CHECKSUM_SIZE              4
#define CRC_BUFFER_SIZE            512

/* Loading thread cmd types */
enum loading_thread_cmd {
	LOADCMD_LOAD_ALL = 0,
	LOADCMD_RELOAD = 1,
	LOADCMD_LOAD_MAX,
};

/* Binary data type in binary table */
struct binary_info_s {
	pid_t bin_id;
	uint32_t bin_size;
	uint32_t ram_size;
	uint16_t bin_offset;
	uint16_t inuse_idx;
	uint32_t part_size;
	uint16_t part_num[2];
	char name[BIN_NAME_MAX];
	char bin_ver[BIN_VER_MAX];
	char kernel_ver[KERNEL_VER_MAX];
};
typedef struct binary_info_s binary_info_t;

#define BIN_ID(bin_idx)                                 bin_table[bin_idx].bin_id
#define BIN_SIZE(bin_idx)                               bin_table[bin_idx].bin_size
#define BIN_RAMSIZE(bin_idx)                            bin_table[bin_idx].ram_size
#define BIN_OFFSET(bin_idx)                             bin_table[bin_idx].bin_offset
#define BIN_USEIDX(bin_idx)                             bin_table[bin_idx].inuse_idx

#define BIN_PARTSIZE(bin_idx)                           bin_table[bin_idx].part_size
#define BIN_PARTNUM(bin_idx, part_idx)                  bin_table[bin_idx].part_num[part_idx]

#define BIN_NAME(bin_idx)                               bin_table[bin_idx].name
#define BIN_VER(bin_idx)                                bin_table[bin_idx].bin_ver
#define BIN_KERNEL_VER(bin_idx)                         bin_table[bin_idx].kernel_ver

extern binary_info_t bin_table[USER_BIN_COUNT + 1];

/****************************************************************************
 * Function Prototypes
 ****************************************************************************/
#ifdef CONFIG_BINMGR_RECOVERY
/****************************************************************************
 * Name: recovery_thread
 *
 * Description:
 *   This function create recovery thread to handle fault with pid.
 *   Recovery thread will check if it is one of the registered binary with binary manager.
 *   If the binary is registered then it will be restarted after killing its
 *   child processes, and if it is not registered then board will be rebooted.
 *
 * Input parameters:
 *   None
 *
 * Returned Value:
 *   Zero (OK) on success; otherwise -1 (ERROR) value is returned.
 *
 * Assumptions:
 *    All the binaries of the system will be registered with the binary manager
 *
 ****************************************************************************/
void binary_manager_recovery(int pid);
#endif

int binary_manager_load_binary(int bin_idx);
int binary_manager_loading(int type, void *data);
int binary_manager_get_binary_count(void);
int binary_manager_get_index_with_binid(int bin_id);
int binary_manager_get_info_with_name(char *bin_name);
int binary_manager_get_info_all(void);

/****************************************************************************
 * Binary Manager Main Thread
 ****************************************************************************/
int binary_manager(int argc, char *argv[]);

#endif							/* CONFIG_BINARY_MANAGER */
#endif							/* __SCHED_BINARY_MANAGER_BINARY_MANAGER_H */
