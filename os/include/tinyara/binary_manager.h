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
#include <limits.h>

#include <tinyara/fs/fs.h>

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

/* The length of dev name */
#define BINMGR_DEVNAME_LEN               16

/* The number of User binaries */
#ifdef CONFIG_NUM_APPS
#define USER_BIN_COUNT                   CONFIG_NUM_APPS
#else
#define USER_BIN_COUNT                   2
#endif
#define KERNEL_BIN_COUNT                 2

#define BINARY_COUNT                     (USER_BIN_COUNT + KERNEL_BIN_COUNT)

#define BINARY_MNT_PATH                  CONFIG_MOUNT_POINT

/* Mount point for User binaries */
#define BINARY_DIR_PATH                  CONFIG_MOUNT_POINT"bins"

/* The length of binary file or kernel partition path.
 * A path is same like below:
 *  - User filepath : "/<mount_path>/<binary_name>_<binary_version>"
 *         length : CONFIG_NAME_MAX + BIN_NAME_MAX + binary_version (32) + /,_(3) + '\0'(1)
 *  - Kernel partpath : "/dev/mtdblock#" defined as BINMGR_DEVNAME_FMT
 * So define binary path length as (CONFIG_NAME_MAX + BIN_NAME_MAX + 40) with some extra.
 */
#define BINARY_PATH_LEN                  (CONFIG_NAME_MAX + BIN_NAME_MAX + 40)

/* Binary states used in state callback */
enum binary_statecb_state_e {
	BINARY_STARTED = 0,           /* Binary is started */
	BINARY_UNLOADED = 1,          /* Binary is unloaded */
	BINARY_READYTOUNLOAD = 2,     /* Binary will be unloaded */
};

/* States for user binary */
enum binary_state {
	BINARY_UNREGISTERED = 0,     /* File is not existing */
	BINARY_INACTIVE = 1,         /* File is existing but binary is not loaded yet */
	BINARY_LOADED = 2,           /* Loading binary is done */
	BINARY_RUNNING = 3,          /* Loaded binary gets scheduling */
	BINARY_UNLOADING = 4,        /* Loaded binary would be unloaded */
	BINARY_FAULT = 5,            /* Binary is excluded from scheduling and would be reloaded */
	BINARY_STATE_MAX,
};
typedef enum binary_state binary_state_e;

/* Message type for binary manager */
enum binmgr_request_msg_type {
	BINMGR_GET_INFO,
	BINMGR_GET_INFO_ALL,
	BINMGR_UPDATE,
	BINMGR_GET_STATE,
	BINMGR_CREATE_BIN,
	BINMGR_NOTIFY_STARTED,
	BINMGR_REGISTER_STATECB,
	BINMGR_UNREGISTER_STATECB,
#ifdef CONFIG_BINMGR_RECOVERY
	BINMGR_FAULT,
#endif
};

/* Result values of returned from binary manager. */
enum binmgr_result_type {
	BINMGR_OK = 0,
	BINMGR_COMMUNICATION_FAIL = -1,
	BINMGR_OPERATION_FAIL = -2,
	BINMGR_OUT_OF_MEMORY = -3,
	BINMGR_INVALID_PARAM = -4,
	BINMGR_NOT_FOUND = -5,
	BINMGR_ALREADY_REGISTERED = -6,
	BINMGR_ALREADY_UPDATED = -7,
};
typedef enum binmgr_result_type binmgr_result_type_e;

/****************************************************************************
 * Public Data
 ****************************************************************************/
/* Binary header data */
struct binary_header_s {
	uint32_t crc_hash;
	uint16_t header_size;
	uint8_t bin_type;
	uint8_t compression_type;
	uint8_t bin_priority;
	uint8_t loading_priority;
	uint32_t bin_size;
	char bin_name[BIN_NAME_MAX];
	uint32_t bin_ver;
	uint32_t bin_ramsize;
	uint32_t bin_stacksize;
	float kernel_ver;
	uint32_t jump_addr;
} __attribute__((__packed__));
typedef struct binary_header_s binary_header_t;

/* The structure of binary update information for kernel or user binaries */
struct binary_update_info_s {
	int available_size;
	char name[BIN_NAME_MAX];
	/* A double type to cover kernel version as float type and user binary version as uint32_t type */
	double version;
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
	uint32_t bin_ver;			/* version of binary */
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
	void *binp;			/* Binary info pointer */
#endif
};
typedef struct load_attr_s load_attr_t;

/* The structure of binaries' information list */
struct binary_update_info_list_s {
	uint32_t bin_count;
	binary_update_info_t bin_info[BINARY_COUNT];
};
typedef struct binary_update_info_list_s binary_update_info_list_t;

typedef void (*binmgr_statecb_t)(char *bin_name, int state, void *cb_data);

struct binmgr_update_bin_s {
	char bin_name[BIN_NAME_MAX];
	int type;
	uint32_t version;
};
typedef struct binmgr_update_bin_s binmgr_update_bin_t;

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
		binmgr_update_bin_t update_bin;
	} data;
};
typedef struct binmgr_request_s binmgr_request_t;

struct binmgr_createbin_response_s {
	int result;
	char binpath[BINARY_PATH_LEN];
};
typedef struct binmgr_createbin_response_s binmgr_createbin_response_t;

struct binmgr_getstate_response_s {
	binmgr_result_type_e result;
	binary_state_e state;
};
typedef struct binmgr_getstate_response_s binmgr_getstate_response_t;

struct binmgr_statecb_response_s {
	binmgr_result_type_e result;
	binmgr_cb_t *cb_info;
};
typedef struct binmgr_statecb_response_s binmgr_statecb_response_t;

struct binmgr_getinfo_response_s {
	binmgr_result_type_e result;
	binary_update_info_t data;
};
typedef struct binmgr_getinfo_response_s binmgr_getinfo_response_t;

struct binmgr_getinfo_all_response_s {
	binmgr_result_type_e result;
	binary_update_info_list_t data;
};
typedef struct binmgr_getinfo_all_response_s binmgr_getinfo_all_response_t;

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/
void binary_manager_register_kpart(int part_num, int part_size);

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
