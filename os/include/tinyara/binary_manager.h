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
#define BINMGR_REQUEST_MQ               "bin_req_mq"
#define BINMGR_RESPONSE_MQ              "bin_res_mq"

/* Maximum number of messages on queue */
#define BINMGR_MAX_MSG                   32

/* Message priority for request type */
#define BINMGR_FAULT_PRIO                100
#define BINMGR_NORMAL_PRIO               50

/* Message queue name Length */
#define BIN_PRIVMQ_LEN                   16

/* The maximum length of binary name */
#define BIN_NAME_MAX                     16

/* Default priority of task loaded by binary manager */
#define BINMGR_LOAD_PRIORITY_DEFAULT     150

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

/****************************************************************************
 * Public Data
 ****************************************************************************/
struct binmgr_request_s {
	int cmd;
	int pid;
	char data[BIN_NAME_MAX];
	char res_qname[BIN_PRIVMQ_LEN];
};
typedef struct binmgr_request_s binmgr_request_t;

struct binmgr_response_s {
	int cmd;
	char data[BIN_NAME_MAX];
};
typedef struct binmgr_response_s binmgr_response_t;
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
