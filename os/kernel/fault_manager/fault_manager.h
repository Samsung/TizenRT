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

#ifndef __KERNEL_FAULT_MANAGER_FAULT_MANAGER_H
#define __KERNEL_FAULT_MANAGER_FAULT_MANAGER_H

#include <tinyara/config.h>
/****************************************************************************
 * Included Files
 ****************************************************************************/

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define PATH_LEN		128		/* Binary path length */
#define FAULTMGR_STACK_SIZE	2048		/* Stack size of fault manager */
#define FAULTMGR_PRIORITY	255		/* Fault manager priority */
#define FAULT_MGR_MQ		"fault_queue"	/* message queue name of fault manager */
#define CONFIG_NUM_BINARIES	CONFIG_NUM_APPS /* Total number of binaries available */
#define MAX_MSG			32		/* Maximum number of messages on queue */

/* This structure is used to store the mapping between binary id and binary path
 * Its value is filled at time of registering the binary with fault manager
 */
struct fm_binary_table_s {
	pid_t binid;
	char bin_path[PATH_LEN];
};

typedef struct fm_binary_table_s fm_binary_table_t;
/****************************************************************************
 * Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: fm_register_binary
 *
 * Description:
 *   This function is used to register binary with the fault manager.
 *   This function will map binary pid with its binary path.
 *
 * Input parameters:
 *   pid    - The pid of the binary to be registered
 *   result - Binary path to be registered with the fault manager
 *
 * Returned Value:
 *   Zero (OK) on success; otherwise a -1 (ERROR) value is returned.
 *
 ****************************************************************************/
int fm_register_bianry(pid_t pid, const char *path);

/****************************************************************************
 * Name: fm_deregister_binary
 *
 * Description:
 *   This function is used to deregister binary from the fault manager.
 *   This function will remove mapping of binary pid with its binary path
 *   in binary table.
 *   It will be called in case if binary is not able to start or it is being
 *   killed by the fault manager.
 *
 * Input parameters:
 *   pid    - The pid of the binary to be deregistered from binary table
 *
 * Returned Value:
 *   Zero (OK) on success; otherwise -1 (ERROR) value is returned.
 *
 ****************************************************************************/
int fm_deregister_binary(pid_t pid);

/****************************************************************************
 * Name: fm_printbinarytable
 *
 * Description:
 *   This function prints binary pid and its corresponding binary path
 *   which are being registered with fault manager.
 *
 * Input parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void fm_printbinarytable(void);

/****************************************************************************
 * Name: get_binid
 *
 * Description:
 *   This function will traverse the task/thread tree and return the root bin pid.
 *   If fault happened in any of the thread/task, it will call this function to get
 *   root binary pid for the same.
 *
 * Input parameters:
 *   pid   -   The pid of the task/thread, whose root bin pid is required
 *
 * Returned Value:
 *   pid   -   pid of the main binary of input pid
 *
 ****************************************************************************/
pid_t get_binid(pid_t pid);

/****************************************************************************
 * Name: kill_binary
 *
 * Description:
 *   This function will kill all the task/thread created by the binary
 *   i.e input pid.
 *   If any fault occurs in the system, fault manager will call this function to
 *   kill all the thread/task created by the input binary.
 *
 * Input parameters:
 *   pid   -   The pid of the binary, whoes all childs to be killed
 *
 * Returned Value:
 *   Zero (OK) on success; otherwise -1 (ERROR) value is returned.
 *
 ****************************************************************************/
int kill_binary(pid_t pid);

/****************************************************************************
 * Name: fault_manager
 *
 * Description:
 *   This function will receive the faulty binary's pid and check if it is one
 *   of the registered binary with fault manager.
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
 *    All the binaries of the system will be registered with the fault manager
 *
 ****************************************************************************/
int fault_manager(int argc, char *argv[]);

#endif							/* __KERNEL_FAULT_MANAGER_FAULT_MANAGER_H */
