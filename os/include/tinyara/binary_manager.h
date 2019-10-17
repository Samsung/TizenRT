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
#include <stdint.h>

#ifdef CONFIG_BINARY_MANAGER

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define BINMGR_REQUEST_MQ                "bin_req_mq"
#define BINMGR_RESPONSE_MQ_PREFIX        "bin_res_mq"

/* Binary manager Callback Message Queue */
#define BINMGR_CBMSG_MQ                  "binmgr_cb"

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

/* The number of User binaries */
#ifdef CONFIG_NUM_APPS
#define USER_BIN_COUNT                   CONFIG_NUM_APPS
#else
#define USER_BIN_COUNT                   2
#endif
#define KERNEL_BIN_COUNT                 1

#define BINARY_COUNT                     (USER_BIN_COUNT + KERNEL_BIN_COUNT)

/* Binary states used in state callback */
enum binary_statecb_state_e {
	BINARY_STARTED = 0,           /* Binary is started */
	BINARY_UNLOADED = 1,          /* Binary is unloaded */
	BINARY_READYTOUNLOAD = 2,     /* Binary will be unloaded */
};

/* The types of partition for binary manager */
enum binmgr_partition_type {
	BINMGR_PART_KERNEL = 0,
	BINMGR_PART_USRBIN,
	BINMGR_PART_MAX,
};

/* Message type for binary manager */
enum binmgr_request_msg_type {
	BINMGR_GET_INFO,
	BINMGR_GET_INFO_ALL,
	BINMGR_UPDATE,
	BINMGR_NOTIFY_STARTED,
	BINMGR_REGISTER_STATECB,
	BINMGR_UNREGISTER_STATECB,
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
	BINMGR_NOT_FOUND = -5,
	BINMGR_ALREADY_REGISTERED = -6,
	BINMGR_ALREADY_UPDATED = -7,
};

/****************************************************************************
 * Public Data
 ****************************************************************************/
/* The structure of binary update information */
struct binary_update_info_s {
	int inactive_partsize;
	char name[BIN_NAME_MAX];
	char active_ver[BIN_VERSION_MAX];
	char active_dev[BINMGR_DEVNAME_LEN];
	char inactive_dev[BINMGR_DEVNAME_LEN];
};
typedef struct binary_update_info_s binary_update_info_t;

/* The sturcture of partition information */
struct part_info_s {
	uint32_t part_size;         /* Partition size */
	int8_t part_num;            /* Partition number */
};
typedef struct part_info_s part_info_t;

/* The structure of load attr configuration */
struct load_attr_s {
	char bin_name[BIN_NAME_MAX];
	uint32_t bin_size;			/* The size of ELF binary to be loaded */
	uint32_t ram_size;			/* The size of RAM partition required by binary */
	uint32_t stack_size;		/* Size of the stack allocated for binary */
	uint16_t offset;			/* The offset from which ELF binary has to be read in MTD partition */
	uint8_t priority;			/* Priority of the binary */
	uint8_t compression_type;	/* Binary compression type */
};
typedef struct load_attr_s load_attr_t;

/* The structure of binaries' information list */
struct binary_update_info_list_s {
	uint32_t bin_count;
	binary_update_info_t bin_info[BINARY_COUNT];
};
typedef struct binary_update_info_list_s binary_update_info_list_t;

typedef void (*binmgr_statecb_t)(char *bin_name, int state, void *cb_data);

struct binmgr_cb_s {
	binmgr_statecb_t func;
	void *data;
};
typedef struct binmgr_cb_s binmgr_cb_t;

struct binmgr_cb_msg_s {
	binmgr_cb_t *callback;
	bool need_response;
	int state;
	char bin_name[BIN_NAME_MAX];
};
typedef struct binmgr_cb_msg_s binmgr_cb_msg_t;

struct binmgr_request_s {
	int cmd;
	int requester_pid;
	union {
		char bin_name[BIN_NAME_MAX];
		binmgr_cb_t *cb_info;
	} data;
};
typedef struct binmgr_request_s binmgr_request_t;

struct binmgr_statecb_response_s {
	int result;
	binmgr_cb_t *cb_info;
};
typedef struct binmgr_statecb_response_s binmgr_statecb_response_t;

struct binmgr_getinfo_response_s {
	int result;
	binary_update_info_t data;
};
typedef struct binmgr_getinfo_response_s binmgr_getinfo_response_t;

struct binmgr_getinfo_all_response_s {
	int result;
	binary_update_info_list_t data;
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
