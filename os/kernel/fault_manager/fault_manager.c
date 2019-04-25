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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <tinyara/init.h>
#include <tinyara/board.h>
#include <debug.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <mqueue.h>
#include "fault_manager.h"
#ifdef CONFIG_BOARD_ASSERT_AUTORESET
#include <sys/boardctl.h>
#include <stdlib.h>
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Global Data
 ****************************************************************************/

/* This structure is used to store the mapping between binary id and binary path
 * Its value is filled at time of registering the binary with fault manager
 */
fm_binary_table_t reg_table[CONFIG_NUM_BINARIES];

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
int fm_register_binary(pid_t pid, const char *path)
{
	int i;

	for (i = 0; i < CONFIG_NUM_BINARIES; i++) {
		if (!(strncmp(reg_table[i].bin_path, path, strnlen(path, PATH_LEN) + 1))) {
			reg_table[i].binid = pid;
			return OK;
		}
	}

	for (i = 0; i < CONFIG_NUM_BINARIES; i++) {
		if (reg_table[i].binid == -1) {
			reg_table[i].binid = pid;
			strncpy(reg_table[i].bin_path, path, strnlen(path, PATH_LEN) + 1);
			return OK;
		}
	}

	fmlldbg("Total number of binaries exceeded\n");
	return ERROR;
}

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
int fm_deregister_binary(pid_t pid)
{
	int i;

	for (i = 0; i < CONFIG_NUM_BINARIES; i++) {
		if (reg_table[i].binid == pid) {
			reg_table[i].binid = -1;
			reg_table[i].bin_path[0] = '\0';
			return OK;
		}
	}

	fmlldbg("No such pid found in registered binaries\n");
	return ERROR;
}

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
void fm_printbinarytable(void)
{
	int i;

	lldbg("-------------------------------------\n");
	lldbg("BIN_ID	BIN_PATH\n");
	for (i = 0; i < CONFIG_NUM_BINARIES; i++) {
		lldbg("%d\t%s\n", reg_table[i].binid, reg_table[i].bin_path);
	}
	lldbg("-------------------------------------\n");

}

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
pid_t get_binid(pid_t pid)
{
	/* TODO: Implementation of traverser of the tree to get main binary pid */
	return pid;
}

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
int kill_binary(pid_t pid)
{
	int ret;

	/* TODO: Implementation of traverser of tree and kill all the task/thread
	 * of the main binary
	 */
	ret = kill(pid, SIGWORK);
	return OK;
}

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
int fault_manager(int argc, char *argv[])
{
	mqd_t mqdes;
	int ret;
	int i;
	int index = -1;
	pid_t pid;
	pid_t pid_restart;
	pid_t bin_id;
	struct mq_attr attr;
	char restart_bin[PATH_LEN] = " ";

	/* Initialize the binary table with default value */
	for (i = 0; i < CONFIG_NUM_BINARIES; i++) {
		reg_table[i].binid = -1;
		reg_table[i].bin_path[0] = '\0';
	}

	attr.mq_maxmsg = MAX_MSG;
	attr.mq_msgsize = sizeof(pid);
	attr.mq_flags = 0;

	mqdes = mq_open(FAULT_MGR_MQ, O_CREAT | O_RDONLY, 0666, &attr);
	DEBUGASSERT(mqdes != (mqd_t)-1);

	sleep(1);

	while (1) {
		/* Wait for the message from assert handler for the faulty pid */
		ret = mq_receive(mqdes, (char *)&pid, sizeof(pid), NULL);
		DEBUGASSERT(ret != -1 && ret == sizeof(pid));

		/* Get the binary pid from the pid got from assert handler */
		bin_id = get_binid(pid);
		DEBUGASSERT(bin_id != ERROR);

		fmlldbg("Faulty binary pid is %d\n", bin_id);

		for (i = 0; i < CONFIG_NUM_BINARIES; i++) {
			if (reg_table[i].binid == bin_id) {
				index = i;
				break;
			}
		}

		/* Check if the binary is registered with fault manager */
		if (index >= 0 && index < CONFIG_NUM_BINARIES) {
			strncpy(restart_bin, reg_table[index].bin_path, strnlen(reg_table[index].bin_path, PATH_LEN) + 1);

			/* Kill the binary and all its child processes */
			ret = kill_binary(bin_id);
			DEBUGASSERT(ret != ERROR);

			/* Deregister the binary from fault manager */
			ret = fm_deregister_binary(reg_table[index].binid);
			DEBUGASSERT(ret != ERROR);

			index = -1;
			sleep(1);

			/* Restart the killed binary */
			pid_restart = exec(restart_bin, NULL, NULL, 0);
			DEBUGASSERT(pid_restart > 0);
		} else {
			fmlldbg("Binary is not registered.\n");

			/* Reboot the board if the binary is not registered with the fault manager */
#ifdef CONFIG_BOARD_ASSERT_AUTORESET
			boardctl(BOARDIOC_RESET, EXIT_SUCCESS);
#else
			(void)irqsave();
			for (;;) {
#ifdef CONFIG_ARCH_LEDS
				board_autoled_on(LED_PANIC);
				up_mdelay(250);
				board_autoled_off(LED_PANIC);
				up_mdelay(250);
#endif
			}
#endif
		}
	}
}
