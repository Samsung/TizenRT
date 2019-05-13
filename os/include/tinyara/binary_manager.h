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

#ifndef __INCLUDE_TINYARA_BINARY_MANAGER_H
#define __INCLUDE_TINYARA_BINARY_MANAGER_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#ifdef CONFIG_BINARY_MANAGER

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define BINMGR_REQUEST_MQ                "bin_req_mq"
#define BINMGR_RESPONSE_MQ_PREFIX        "bin_res_mq"

/* Maximum number of messages on queue */
#define BINMGR_MAX_MSG                   32

/* Message priority for request type */
#define BINMGR_FAULT_PRIO                100
#define BINMGR_NORMAL_PRIO               50

/* Message queue name Length */
#define BIN_PRIVMQ_LEN                   16

/* The maximum length of binary name */
#define BIN_NAME_MAX                     16

/* The maximum length of version name */
#define BIN_VERSION_MAX                  16

/* The length of dev name */
#define BINMGR_DEVNAME_LEN               16

/* Default priority of task loaded by binary manager */
#define BINMGR_LOAD_PRIORITY_DEFAULT     120

/* The number of User binaries */
#ifdef CONFIG_NUM_APPS
#define USER_BIN_COUNT                   CONFIG_NUM_APPS
#else
#define USER_BIN_COUNT                   2
#endif
#define KERNEL_BIN_COUNT                 1

#define BINARY_COUNT                     (USER_BIN_COUNT + KERNEL_BIN_COUNT)


/* The types of partition for binary manager */
enum binmgr_partition_type {
	BINMGR_PART_KERNEL = 0,
	BINMGR_PART_USRBIN,
	BINMGR_PART_LOADPARAM,
	BINMGR_PART_MAX,
};

/* Message type for binary manager */
enum binmgr_request_msg_type {
	BINMGR_GET_INFO,
	BINMGR_GET_INFO_ALL,
	BINMGR_RELOAD,
#ifdef CONFIG_BINMGR_RECOVERY
	BINMGR_FAULT,
#endif
};

/* Result values of returned from binary manager. */
enum binmgr_response_result_type {
	BINMGR_OK = 0,
	BINMGR_COMMUNICATION_FAIL = -1,
	BINMGR_OPERATION_FAIL = -2,
	BINMGR_OUT_OF_MEMORY = -3,
	BINMGR_INVALID_PARAM = -4,
	BINMGR_BININFO_NOT_FOUND = -5,
};

/****************************************************************************
 * Public Data
 ****************************************************************************/
/* The structure of binary information */
struct binary_info_s {
	int part_size;
	char name[BIN_NAME_MAX];
	char version[BIN_VERSION_MAX];
	char dev_path[BINMGR_DEVNAME_LEN];
};
typedef struct binary_info_s binary_info_t;

/* The structure of binaries' information list */
struct binary_info_list_s {
	int bin_count;
	binary_info_t bin_info[BINARY_COUNT];
};
typedef struct binary_info_list_s binary_info_list_t;

struct binmgr_request_s {
	int cmd;
	int requester_pid;
	char bin_name[BIN_NAME_MAX];
};
typedef struct binmgr_request_s binmgr_request_t;

struct binmgr_reload_response_s {
	int result;
};
typedef struct binmgr_reload_response_s binmgr_reload_response_t;

struct binmgr_getinfo_response_s {
	int result;
	binary_info_t data;
};
typedef struct binmgr_getinfo_response_s binmgr_getinfo_response_t;

struct binmgr_getinfo_all_response_s {
	int result;
	binary_info_list_t data;
};
typedef struct binmgr_getinfo_all_response_s binmgr_getinfo_all_response_t;

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/
void binary_manager_register_partition(int part_num, int part_type, char *name, int part_size);

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

#endif							/* CONFIG_BINARY_MANAGER */
#endif							/* __INCLUDE_TINYARA_BINARY_MANAGER_H */
