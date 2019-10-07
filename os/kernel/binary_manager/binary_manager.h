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
#include <queue.h>

#include <tinyara/binary_manager.h>

#ifdef CONFIG_BINARY_MANAGER

/****************************************************************************
 * Pre-Processor Definitions
 ****************************************************************************/
#define BINARY_MANAGER_NAME        "binary_manager"           /* Binary manager thread name */
#define BINARY_MANAGER_STACKSIZE   2048                       /* Binary manager thread stack size */
#define BINARY_MANAGER_PRIORITY    250                        /* Binary manager thread priority */

/* Loading Thread information */
#define LOADINGTHD_NAME            "bm_loader"                 /* Loading thread name */
#define LOADINGTHD_STACKSIZE       2048                        /* Loading thread stack size */
#define LOADINGTHD_PRIORITY        200                         /* Loading thread priority */

/* Supported binary types */
#define BIN_TYPE_BIN               0                          /* 'bin' type for kernel binary */
#define BIN_TYPE_ELF               1                          /* 'elf' type for user binary */

/* Binary information configuration */
#define PARTS_PER_BIN              2                          /* The number of partitions per binary */
#define BIN_VER_MAX                16                         /* The maximum length of binary version */
#define KERNEL_VER_MAX             8                          /* The maximum length of kernel version */

#define CHECKSUM_SIZE              4
#define CRC_BUFFER_SIZE            512

/* The number of arguments for loading thread */
#define LOADTHD_ARGC               2

#define BINMGR_DEVNAME_FMT         "/dev/mtdblock%d"

#if (defined(CONFIG_BUILD_PROTECTED) || defined(CONFIG_BUILD_KERNEL)) && defined(CONFIG_MM_KERNEL_HEAP)
#define MAX_WAIT_COUNT             3                          /* The maximum number of times you can wait to process a delayed free */
#endif
#define BINMGR_LOADING_TRYCNT      2

/* Loading thread cmd types */
enum loading_thread_cmd {
	LOADCMD_LOAD = 0,
	LOADCMD_LOAD_ALL = 1,
	LOADCMD_RELOAD = 2,
	LOADCMD_UPDATE = 3,          /* Reload on update request */
	LOADCMD_LOAD_MAX,
};

/* Binary states */
enum binary_state_e {
	BINARY_UNREGISTERED = 0,     /* Partition is unregistered */
	BINARY_INACTIVE = 1,         /* Partition is registered, but binary is not loaded yet */
	BINARY_LOADING_DONE = 2,     /* Loading binary is done */
	BINARY_RUNNING = 3,          /* Loaded binary gets scheduling */
	BINARY_WAITUNLOAD = 4,       /* Loaded binary would be unloaded */
	BINARY_FAULT = 5,            /* Binary is excluded from scheduling and would be reloaded */
	BINARY_STATE_MAX,
};

/* Binary data type in binary table */
struct binmgr_bininfo_s {
	pid_t bin_id;
	uint8_t state;
	uint8_t inuse_idx;
	load_attr_t load_attr;
	part_info_t part_info[PARTS_PER_BIN];
	char bin_ver[BIN_VER_MAX];
	char kernel_ver[KERNEL_VER_MAX];
	sq_queue_t cb_list; // list node type : statecb_node_t
};
typedef struct binmgr_bininfo_s binmgr_bininfo_t;

struct statecb_node_s {
	struct statecb_node_s *flink;
	int pid;
	binmgr_cb_t *cb_info;
};
typedef struct statecb_node_s statecb_node_t;

binmgr_bininfo_t *binary_manager_get_binary_data(uint32_t bin_idx);
#define BIN_ID(bin_idx)                                 binary_manager_get_binary_data(bin_idx)->bin_id
#define BIN_STATE(bin_idx)                              binary_manager_get_binary_data(bin_idx)->state
#define BIN_USEIDX(bin_idx)                             binary_manager_get_binary_data(bin_idx)->inuse_idx
#define BIN_PARTSIZE(bin_idx, part_idx)                 binary_manager_get_binary_data(bin_idx)->part_info[part_idx].part_size
#define BIN_PARTNUM(bin_idx, part_idx)                  binary_manager_get_binary_data(bin_idx)->part_info[part_idx].part_num

#define BIN_VER(bin_idx)                                binary_manager_get_binary_data(bin_idx)->bin_ver
#define BIN_KERNEL_VER(bin_idx)                         binary_manager_get_binary_data(bin_idx)->kernel_ver
#define BIN_CBLIST(bin_idx)                             binary_manager_get_binary_data(bin_idx)->cb_list

#define BIN_LOAD_ATTR(bin_idx)                          binary_manager_get_binary_data(bin_idx)->load_attr
#define BIN_NAME(bin_idx)                               binary_manager_get_binary_data(bin_idx)->load_attr.bin_name
#define BIN_SIZE(bin_idx)                               binary_manager_get_binary_data(bin_idx)->load_attr.bin_size
#define BIN_RAMSIZE(bin_idx)                            binary_manager_get_binary_data(bin_idx)->load_attr.ram_size
#define BIN_OFFSET(bin_idx)                             binary_manager_get_binary_data(bin_idx)->load_attr.offset
#define BIN_STACKSIZE(bin_idx)                          binary_manager_get_binary_data(bin_idx)->load_attr.stack_size
#define BIN_PRIORITY(bin_idx)                           binary_manager_get_binary_data(bin_idx)->load_attr.priority
#define BIN_COMPRESSION_TYPE(bin_idx)                   binary_manager_get_binary_data(bin_idx)->load_attr.compression_type

/****************************************************************************
 * Function Prototypes
 ****************************************************************************/
#ifdef CONFIG_BINMGR_RECOVERY
/****************************************************************************
 * Name: binary_manager_recovery
 *
 * Description:
 *   This function will receive the faulty pid and check if its binary id is one
 *   of the registered binary with binary manager.
 *   If the binary is registered, it excludes its children from scheduling
 *   and creates loading thread which will terminate them and load binary again.
 *   Otherwise, board will be rebooted.
 *
 * Input parameters:
 *   pid   -   The pid of recovery message
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void binary_manager_recovery(int pid);
mqd_t binary_manager_get_mqfd(void);
#endif

void binary_manager_register_statecb(int pid, binmgr_cb_t *cb_info);
void binary_manager_unregister_statecb(int pid);
void binary_manager_clear_bin_statecb(int bin_idx);
int binary_manager_send_statecb_msg(int recv_binidx, char *bin_name, uint8_t state, bool need_response);
void binary_manager_notify_state_changed(int bin_idx, uint8_t state);
int binary_manager_load_binary(int bin_idx);
int binary_manager_loading(char *loading_data[]);
uint32_t binary_manager_get_binary_count(void);
int binary_manager_get_index_with_binid(int bin_id);
void binary_manager_get_info_with_name(int request_pid, char *bin_name);
void binary_manager_get_info_all(int request_pid);

void binary_manager_send_response(char *q_name, void *response_msg, int msg_size);

/****************************************************************************
 * Binary Manager Main Thread
 ****************************************************************************/
int binary_manager(int argc, char *argv[]);

#endif							/* CONFIG_BINARY_MANAGER */
#endif							/* __SCHED_BINARY_MANAGER_BINARY_MANAGER_H */
