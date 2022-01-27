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
#include <stdint.h>
#ifdef CONFIG_BINMGR_RECOVERY
#include <mqueue.h>
#endif

#include <tinyara/sched.h>
#include <tinyara/binary_manager.h>

#ifdef CONFIG_BINARY_MANAGER

/****************************************************************************
 * Pre-Processor Definitions
 ****************************************************************************/
/* Priority Range of Binary Manager Modules */
#ifdef CONFIG_BM_PRIORITY_MAX
#define BM_PRIORITY_MAX            CONFIG_BM_PRIORITY_MAX     /* The maximum priority of BM module */
#else
#define BM_PRIORITY_MAX            205                        /* The maximum priority of BM module */
#endif
#ifdef CONFIG_BM_PRIORITY_MIN
#define BM_PRIORITY_MIN            CONFIG_BM_PRIORITY_MIN     /* The minimum priority of BM module */
#else
#define BM_PRIORITY_MIN            200                        /* The minimum priority of BM module */
#endif

/* Fault Message Sender Thread information */
#define FAULTMSGSENDER_NAME        "bm_faultmsgsender"        /* Fault Message Sender thread name */
#define FAULTMSGSENDER_STACKSIZE   1024                       /* Fault Message Sender thread stack size */
#define FAULTMSGSENDER_PRIORITY    BM_PRIORITY_MAX            /* Fault Message Sender thread priority */

/* Binary Manager Core Thread information */
#define BINARY_MANAGER_NAME        "binary_manager"           /* Binary manager thread name */
#define BINARY_MANAGER_STACKSIZE   8192                       /* Binary manager thread stack size */
#define BINARY_MANAGER_PRIORITY    203                        /* Binary manager thread priority */

/* Loading Thread information */
#define LOADER_NAME                "bm_loader"                 /* Loading thread name */
#define LOADER_STACKSIZE           4096                        /* Loading thread stack size */

#define LOADER_PRIORITY_HIGH       200                         /* Loading thread priority for binary with high priority */
#define LOADER_PRIORITY_MID        150                         /* Loading thread priority for binary with middle priority */
#define LOADER_PRIORITY_LOW        90                          /* Loading thread priority for binary with low priority */

/* Supported binary types */
#define BIN_TYPE_BIN               0                          /* 'bin' type for kernel binary */
#define BIN_TYPE_ELF               1                          /* 'elf' type for user binary */

#define PARTS_PER_BIN              2                          /* The number of partitions per binary */

/* Bootparam information */
#define BOOTPARAM_COUNT            2                          /* The number of boot parameters */
#define BOOTPARAM_SIZE             4096                       /* The size of boot parameter : 4K */
#define BOOTPARAM_PARTSIZE         (BOOTPARAM_SIZE * 2)       /* The size of partition for dual boot parameters : 8K */

#define CHECKSUM_SIZE              4

/* Index of 'Common Library' data in binary table. */
#define BM_CMNLIB_IDX              0

#define BM_CMNLIB_NAME             "common"

#define BM_VERSION_DATE_MIN        101
#define BM_VERSION_DATE_MAX        991231

/* The number of arguments for loader */
#define LOADER_ARGC                1

#define BINMGR_DEVNAME_FMT         "/dev/mtdblock%d"

#if (defined(CONFIG_BUILD_PROTECTED) || defined(CONFIG_BUILD_KERNEL)) && defined(CONFIG_MM_KERNEL_HEAP)
#define MAX_WAIT_COUNT             3                          /* The maximum number of times you can wait to process a delayed free */
#endif
#define BINMGR_LOADING_TRYCNT      2

/* Loading thread cmd types */
enum loading_thread_cmd {
	LOADCMD_LOAD = 0,
	LOADCMD_LOAD_ALL = 1,
	LOADCMD_UPDATE = 2,          /* Reload on update request */
#ifdef CONFIG_BINMGR_RECOVERY
	LOADCMD_RELOAD = 3,          /* Reload on recovery request */
#endif
	LOADCMD_LOAD_MAX,
};

/* Binary loading priority */
enum binary_loading_priority_e {
	BINARY_LOADPRIO_LOW = 1,
	BINARY_LOADPRIO_MID = 2,
	BINARY_LOADPRIO_HIGH = 3,
	BINARY_LOADPRIO_MAX,
};

#ifdef CONFIG_BINMGR_RECOVERY
struct faultmsg_s {
	struct faultmsg_s *flink;	/* Implements singly linked list */
	int binidx;
};
typedef struct faultmsg_s faultmsg_t;
#endif

/* Partition information : partition size, devnum */
struct part_info_s {
	uint32_t size;
	uint8_t devnum;
	uint32_t address;
};
typedef struct part_info_s part_info_t;

/* User binary data type in binary table */
struct binmgr_uinfo_s {
	pid_t bin_id;
	uint8_t state;
	uint8_t useidx;
	uint8_t bp_idx;
	uint8_t bin_count;
	load_attr_t load_attr;
	part_info_t part_info[PARTS_PER_BIN];
	uint8_t load_priority[PARTS_PER_BIN];
	uint32_t bin_ver[PARTS_PER_BIN];
	float available_kernel_ver;
	struct tcb_s *rt_list;
	struct tcb_s *nrt_list;
	sq_queue_t cb_list; // list node type : statecb_node_t
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
	struct binary_s *binp;
#endif
};
typedef struct binmgr_uinfo_s binmgr_uinfo_t;

/* Kernel binary data type in kernel table */
struct binmgr_kinfo_s {
	uint8_t inuse_idx;
	uint32_t part_count;
	part_info_t part_info[KERNEL_BIN_COUNT];
	uint32_t version;
};
typedef struct binmgr_kinfo_s binmgr_kinfo_t;

/* User bootparam data */
struct binmgr_userbp_s {
	char name[BIN_NAME_MAX];
	uint8_t useidx;
};
typedef struct binmgr_userbp_s binmgr_userbp_t;

/* Boot parameter data */
struct binmgr_bpdata_s {
	uint32_t crc_hash;
	uint32_t version;
	uint32_t format_ver;
	uint8_t active_idx;
	uint32_t address[BOOTPARAM_COUNT];
#ifdef CONFIG_APP_BINARY_SEPARATION
	uint8_t app_count;
	binmgr_userbp_t app_data[CONFIG_NUM_APPS + 1];
#endif
} __attribute__((__packed__));
typedef struct binmgr_bpdata_s binmgr_bpdata_t;

struct binmgr_bpinfo_s {
	uint8_t inuse_idx;
	int part_num;
	binmgr_bpdata_t bp_data;
};
typedef struct binmgr_bpinfo_s binmgr_bpinfo_t;

struct statecb_node_s {
	struct statecb_node_s *flink;
	int pid;
	binmgr_cb_t *cb_info;
};
typedef struct statecb_node_s statecb_node_t;

binmgr_uinfo_t *binary_manager_get_udata(uint32_t bin_idx);
#define BIN_ID(bin_idx)                                 binary_manager_get_udata(bin_idx)->bin_id
#define BIN_STATE(bin_idx)                              binary_manager_get_udata(bin_idx)->state
#define BIN_USEIDX(bin_idx)                             binary_manager_get_udata(bin_idx)->useidx
#define BIN_BPIDX(bin_idx)                              binary_manager_get_udata(bin_idx)->bp_idx
#define BIN_COUNT(bin_idx)                              binary_manager_get_udata(bin_idx)->bin_count

#define BIN_RTLIST(bin_idx)                             binary_manager_get_udata(bin_idx)->rt_list
#define BIN_NRTLIST(bin_idx)                            binary_manager_get_udata(bin_idx)->nrt_list
#define BIN_PARTSIZE(bin_idx, part_idx)                 binary_manager_get_udata(bin_idx)->part_info[part_idx].size
#define BIN_PARTNUM(bin_idx, part_idx)                  binary_manager_get_udata(bin_idx)->part_info[part_idx].devnum
#define BIN_PARTADDR(bin_idx, part_idx)                 binary_manager_get_udata(bin_idx)->part_info[part_idx].address

#define BIN_LOADVER(bin_idx)                            binary_manager_get_udata(bin_idx)->load_attr.bin_ver
#define BIN_KERNEL_VER(bin_idx)                         binary_manager_get_udata(bin_idx)->available_kernel_ver
#define BIN_CBLIST(bin_idx)                             binary_manager_get_udata(bin_idx)->cb_list
#define BIN_LOAD_PRIORITY(bin_idx, part_idx)            binary_manager_get_udata(bin_idx)->load_priority[part_idx]
#define BIN_VER(bin_idx, part_idx)                      binary_manager_get_udata(bin_idx)->bin_ver[part_idx]

#define BIN_LOAD_ATTR(bin_idx)                          binary_manager_get_udata(bin_idx)->load_attr
#define BIN_NAME(bin_idx)                               binary_manager_get_udata(bin_idx)->load_attr.bin_name
#define BIN_SIZE(bin_idx)                               binary_manager_get_udata(bin_idx)->load_attr.bin_size
#define BIN_RAMSIZE(bin_idx)                            binary_manager_get_udata(bin_idx)->load_attr.ram_size
#define BIN_OFFSET(bin_idx)                             binary_manager_get_udata(bin_idx)->load_attr.offset
#define BIN_STACKSIZE(bin_idx)                          binary_manager_get_udata(bin_idx)->load_attr.stack_size
#define BIN_PRIORITY(bin_idx)                           binary_manager_get_udata(bin_idx)->load_attr.priority
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
#define BIN_LOADINFO(bin_idx)                           binary_manager_get_udata(bin_idx)->binp
#endif

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
void binary_manager_deactivate_rtthreads(int bin_idx);
void binary_manager_set_faultmsg_sender(pid_t pid);
int binary_manager_faultmsg_sender(int argc, char *argv[]);
mqd_t binary_manager_get_mqfd(void);
void binary_manager_recover_userfault(void);
#endif

void binary_manager_add_binlist(FAR struct tcb_s *tcb);
void binary_manager_remove_binlist(FAR struct tcb_s *tcb);
void binary_manager_clear_bindata(int bin_idx);
void binary_manager_register_statecb(int pid, binmgr_cb_t *cb_info);
void binary_manager_unregister_statecb(int pid);
void binary_manager_clear_bin_statecb(int bin_idx);
int binary_manager_send_statecb_msg(int recv_binidx, char *bin_name, uint8_t state, bool need_response);
void binary_manager_notify_state_changed(int bin_idx, uint8_t state);
int binary_manager_execute_loader(int cmd, int bin_idx);
uint32_t binary_manager_get_ucount(void);
uint32_t binary_manager_get_kcount(void);
binmgr_kinfo_t *binary_manager_get_kdata(void);
void binary_manager_get_info_with_name(int request_pid, char *bin_name);
void binary_manager_get_info_all(int request_pid);
void binary_manager_get_state_with_name(int request_pid, char *bin_name);
void binary_manager_send_response(char *q_name, void *response_msg, int msg_size);
int binary_manager_read_header(int type, char *devpath, void *header_data, bool crc_check);
int binary_manager_create_entry(int requester_pid, char *bin_name, int version);
void binary_manager_release_binary_sem(int bin_idx);
void binary_manager_update_running_state(int bin_id);
int binary_manager_get_index_with_name(char *bin_name);
int binary_manager_scan_bootparam(binmgr_bpinfo_t *bp_info);
binmgr_bpdata_t *binary_manager_get_bpdata(void);
int binary_manager_get_inactive_path(int requester_pid, char *bin_name);
void binary_manager_update_bootparam(int requester_pid, uint8_t type);
void binary_manager_reset_board(int reboot_reason);
int binary_manager_update_kernel_binary(void);

/****************************************************************************
 * Binary Manager Main Thread
 ****************************************************************************/
int binary_manager(int argc, char *argv[]);

#endif							/* CONFIG_BINARY_MANAGER */
#endif							/* __SCHED_BINARY_MANAGER_BINARY_MANAGER_H */
