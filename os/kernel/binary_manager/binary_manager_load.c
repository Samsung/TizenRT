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
#include <debug.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>
#include <dirent.h>
#include <stdint.h>
#include <string.h>
#include <queue.h>
#include <stdbool.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/boardctl.h>

#include <tinyara/irq.h>
#include <tinyara/mm/mm.h>
#include <tinyara/sched.h>
#include <tinyara/init.h>
#include <tinyara/kthread.h>
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
#include <tinyara/binfmt/binfmt.h>
#endif
#ifdef CONFIG_BINMGR_RELOAD_REBOOT
#include <tinyara/reboot_reason.h>
#endif
#ifdef CONFIG_BINARY_SIGNING
#include <tinyara/signature.h>
#endif

#include "sched/sched.h"
#include "task/task.h"
#include "semaphore/semaphore.h"

#include "binary_manager.h"

/****************************************************************************
 * Private Definitions
 ****************************************************************************/
extern sq_queue_t g_sem_list;
extern struct binary_s *g_lib_binp;

#ifdef CONFIG_COMPRESSED_BINARY
#define BINARY_COMP_TYPE "[Compressed Binary]"
#else
#define BINARY_COMP_TYPE "[Un-compressed Binary]"
#endif

/* Partition Name - first partition : "A", second partition : "B" */
#define GET_PARTNAME(part_idx)  ((part_idx == 0) ? "A" : "B")

/****************************************************************************
 * Private Functions
 ****************************************************************************/
/****************************************************************************
 * Name: binary_manager_get_loader_priority
 *
 * Description:
 *   This function gets a priority of loader according to loading priority of binary.
 *
 ****************************************************************************/
static uint8_t binary_manager_get_loader_priority(uint8_t load_priority)
{
	uint8_t loader_priority;

	loader_priority = 0;

	switch (load_priority) {
	case BINARY_LOADPRIO_LOW:
		loader_priority = LOADER_PRIORITY_LOW;
		break;
	case BINARY_LOADPRIO_MID:
		loader_priority = LOADER_PRIORITY_MID;
		break;
	case BINARY_LOADPRIO_HIGH:
		loader_priority = LOADER_PRIORITY_HIGH;
		break;
	default:
		bmdbg("Invalid loading priority : %u\n", load_priority);
		break;
	}
	bmvdbg("Loader priority : %u\n", loader_priority);

	return loader_priority;
}

/****************************************************************************
 * Name: binary_manager_load_binary
 *
 * Description:
 *	 This function loads and executes binary.
 *
 ****************************************************************************/
static int binary_manager_load_binary(int bin_idx, char *path, load_attr_t *load_attr)
{
	int ret;
	int retry_count;

	retry_count = 0;
	while (retry_count < BINMGR_LOADING_TRYCNT) {
		ret = load_binary(bin_idx, path, load_attr);
		if (ret >= 0) {
			/* Set the data in table from header */
			BIN_LOAD_ATTR(bin_idx) = *load_attr;
			strncpy(BIN_NAME(bin_idx), load_attr->bin_name, BIN_NAME_MAX);
			bmdbg("Load success! [Name: %s] [Version: %d] [Partition: %s] [Text start : 0x%08x] %s\n", BIN_NAME(bin_idx), 
					BIN_LOADVER(bin_idx), GET_PARTNAME(BIN_USEIDX(bin_idx)), elf_find_text_section_addr(bin_idx), BINARY_COMP_TYPE);
			return OK;
		} else if (errno == ENOMEM) {
			/* Sleep for a moment to get available memory */
			usleep(1000);
		}
		retry_count++;
		bmdbg("Load '%s' %dth fail, errno %d\n", BIN_NAME(bin_idx), retry_count, errno);
	}
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
	struct binary_s *binp = load_attr->binp;
	if (binp) {
		/* If we are here it means that reload has failed. So perform cleanup */
		((struct binary_s *)binp)->reload = false;
		binfmt_exit(binp);
		BIN_LOADINFO(bin_idx) = NULL;
	}
#endif
	return ERROR;
}

/****************************************************************************
 * Name: binary_manager_load
 *
 * Description:
 *	 This function loads user binary with binary index.
 *
 ****************************************************************************/
static int binary_manager_load(int bin_idx)
{
	int ret;
	int bin_count;
	load_attr_t load_attr;
	char devpath[BINARY_PATH_LEN];
	user_binary_header_t user_header_data;
#ifdef CONFIG_SUPPORT_COMMON_BINARY
	common_binary_header_t common_header_data;
#endif
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
	struct binary_s *binp;
#endif
#ifdef CONFIG_USE_BP
	bool need_update_bp = false;
#endif

	if (bin_idx < 0) {
		bmdbg("Invalid bin idx %d\n", bin_idx);
		return ERROR;
	}

	/* Check binary state */
	if (BIN_STATE(bin_idx) != BINARY_INACTIVE) {
		bmdbg("Invalid binary state %d\n", BIN_STATE(bin_idx));
		return ERROR;
	}

#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
	binp = BIN_LOADINFO(bin_idx);
	if (binp) {
		bin_count = 1;
		snprintf(devpath, BINARY_PATH_LEN, BINMGR_DEVNAME_FMT, BIN_PARTNUM(bin_idx, (BIN_USEIDX(bin_idx))));
	} else
#endif
	{
		bin_count = BIN_COUNT(bin_idx);
	}

	do {
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
		if (!binp)
#endif
		{
#ifdef CONFIG_BINARY_SIGNING
			/* Check signature */
			ret = up_verify_usersignature(BIN_PARTADDR(bin_idx, (BIN_USEIDX(bin_idx))));
			if (ret == OK) {
				bmdbg("%s Signature Checking Success\n", BIN_NAME(bin_idx));
			} else {
				bmdbg("Invalid Signature, name : %s, address : %p\n", BIN_NAME(bin_idx), BIN_PARTADDR(bin_idx, (BIN_USEIDX(bin_idx))));
				if (--bin_count > 0) {
					BIN_USEIDX(bin_idx) ^= 1;
#ifdef CONFIG_USE_BP
					need_update_bp = true;
#endif
					bmdbg("Try to read another partition %s\n", GET_PARTNAME(BIN_USEIDX(bin_idx)));
					continue;
				} else {
					bmdbg("No valid binary %s\n", BIN_NAME(bin_idx));
					break;
				}
			}
#endif

			/* Read header data and Check crc */
			snprintf(devpath, BINARY_PATH_LEN, BINMGR_DEVNAME_FMT, BIN_PARTNUM(bin_idx, (BIN_USEIDX(bin_idx))));
#ifdef CONFIG_SUPPORT_COMMON_BINARY
			if (bin_idx == BM_CMNLIB_IDX) {
				ret = binary_manager_read_header(BINARY_COMMON, devpath, &common_header_data, true);
				BIN_VER(bin_idx, BIN_USEIDX(bin_idx)) = common_header_data.version;
			} else
#endif
			{
				ret = binary_manager_read_header(BINARY_USERAPP, devpath, &user_header_data, true);
				BIN_VER(bin_idx, BIN_USEIDX(bin_idx)) = user_header_data.bin_ver;
			}
			if (ret == BINMGR_OK) {
				bmdbg("%s Header Checking Success\n", BIN_NAME(bin_idx));
			} else {
				/* Clear version because of invalid binary */
				BIN_VER(bin_idx, BIN_USEIDX(bin_idx)) = 0;
				bmdbg("Invalid Header data, name : %s, devpath : %p\n", BIN_NAME(bin_idx), devpath);
				if (--bin_count > 0) {
					BIN_USEIDX(bin_idx) ^= 1;
#ifdef CONFIG_USE_BP
					need_update_bp = true;
#endif
					bmdbg("Try to read another partition %s\n", GET_PARTNAME(BIN_USEIDX(bin_idx)));
					continue;
				} else {
					bmdbg("No valid binary %s\n", BIN_NAME(bin_idx));
					break;
				}
			}
#ifdef CONFIG_SUPPORT_COMMON_BINARY
			if (bin_idx == BM_CMNLIB_IDX) {
				strncpy(load_attr.bin_name, BM_CMNLIB_NAME, sizeof(BM_CMNLIB_NAME));
				load_attr.offset = CHECKSUM_SIZE + common_header_data.header_size;
				load_attr.bin_size = common_header_data.bin_size;
				load_attr.bin_ver = common_header_data.version;
#ifdef CONFIG_BINARY_SIGNING
				load_attr.offset += USER_SIGN_PREPEND_SIZE;
#endif

			} else
#endif
			{
				strncpy(load_attr.bin_name, user_header_data.bin_name, BIN_NAME_MAX - 1);
				load_attr.bin_name[BIN_NAME_MAX - 1] = '\0';
				load_attr.bin_size = user_header_data.bin_size;
				load_attr.ram_size = user_header_data.bin_ramsize;
				load_attr.stack_size = user_header_data.bin_stacksize;
				load_attr.priority = user_header_data.bin_priority;
				load_attr.offset = CHECKSUM_SIZE + user_header_data.header_size;
#ifdef CONFIG_BINARY_SIGNING
				load_attr.offset += USER_SIGN_PREPEND_SIZE;
#endif
				load_attr.bin_ver = user_header_data.bin_ver;
			}
		}
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
		else {
			load_attr = BIN_LOAD_ATTR(bin_idx);
		}
		load_attr.binp = binp;
#endif

		ret = binary_manager_load_binary(bin_idx, devpath, &load_attr);
		if (ret == OK) {
#ifdef CONFIG_USE_BP
			if (need_update_bp) {
				/* Update boot param data because the binary not written to bootparam is loaded */
				binmgr_bpdata_t update_bp_data;
				memcpy(&update_bp_data, binary_manager_get_bpdata(), sizeof(binmgr_bpdata_t));
				update_bp_data.version++;
				update_bp_data.app_data[BIN_BPIDX(bin_idx)].useidx ^= 1;
				ret = binary_manager_write_bootparam(&update_bp_data);
				if (ret == BINMGR_OK) {
					binary_manager_set_bpdata(&update_bp_data);
					bmvdbg("Update bootparam SUCCESS\n");
				} else {
					bmdbg("Fail to update bootparam to recover, %d\n", ret);
				}
			}
#endif
			return BINMGR_OK;
		}
		if (--bin_count > 0) {
			/* Change index 0 to 1 and 1 to 0. */
			BIN_USEIDX(bin_idx) ^= 1;
#ifdef CONFIG_USE_BP
			need_update_bp = true;
#endif
			bmdbg("Try to read another partition %s\n", GET_PARTNAME(BIN_USEIDX(bin_idx)));
		} else {
			bmdbg("No valid binary %s\n", BIN_NAME(bin_idx));
		}
	} while (bin_count > 0);

	return ERROR;
}

/****************************************************************************
 * Name: binary_manager_terminate_binary
 *
 * Description:
 *   This function executes registered callbacks for 'unload' state,
 *   terminates all task/threads of binary, and unloads binary.
 *
 ****************************************************************************/
static int binary_manager_terminate_binary(int bin_idx)
{
	int ret;
	int binid;
	int state;
	bool need_recovery;
	struct tcb_s *btcb;
	struct tcb_s *tcb;
	struct tcb_s *ntcb;
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
	struct binary_s *binp = NULL;
#endif

#ifdef CONFIG_SUPPORT_COMMON_BINARY
	if (bin_idx == BM_CMNLIB_IDX) {
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
		if (BIN_STATE(bin_idx) == BINARY_FAULT) {
			/* Set the flag to reload binary without freeing memory in unload_module for app reloading optimization. */
			binp = BIN_LOADINFO(bin_idx);
			binp->reload = true;
		}
#endif
		ret = unload_module(g_lib_binp);
		if (ret != OK) {
			bmdbg("Fail to unload common binary %d\n", ret);
			return BINMGR_OPERATION_FAIL;
		}
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
		if (binp == NULL || binp->reload != true) {
			g_lib_binp = NULL;
			BIN_LOADINFO(bin_idx) = NULL;
		}
#else
		g_lib_binp = NULL;
#endif
		BIN_STATE(bin_idx) = BINARY_INACTIVE;
		bmvdbg("Unload common binary Done!!\n");
		return BINMGR_OK;
	}
#endif

	need_recovery = false;

	binid = BIN_ID(bin_idx);
	btcb = (struct tcb_s *)sched_gettcb(binid);
	if (btcb == NULL) {
		bmdbg("Fail to get main task of binary %d\n", binid);
		return BINMGR_OPERATION_FAIL;
	}

	/* Save a current state and change a state to BINARY_UNLOADING to avoid duplicated function call */
	state = BIN_STATE(bin_idx);
	BIN_STATE(bin_idx) = BINARY_UNLOADING;

	if (state == BINARY_RUNNING) {
		need_recovery = true;
		/* Waits until some callbacks for cleanup are done if registered callbacks exist */
		ret = binary_manager_send_statecb_msg(bin_idx, BIN_NAME(bin_idx), BINARY_READYTOUNLOAD, true);
		if (ret != OK) {
			bmdbg("Fail to execute callbacks for unloading %s\n", BIN_NAME(bin_idx));
			/* Recover binary state on failure */
			BIN_STATE(bin_idx) = state;
			return ERROR;
		}
		/* Release all kernel semaphores held by the threads in binary */
		binary_manager_release_binary_sem(bin_idx);
	}

	/* Terminate all children created by a binary */
	ntcb = tcb = BIN_RTLIST(bin_idx);
	while (tcb) {
		ntcb = tcb->bin_flink;
		if (need_recovery) {
			task_recover(tcb);
		}
		if (tcb != btcb) {
			ret = task_terminate_unloaded(tcb);
			if (ret < 0) {
				bmdbg("Fail to terminate task of binary %d\n", binid);
			}
		}
		tcb = ntcb;
	}

	ntcb = tcb = BIN_NRTLIST(bin_idx);
	while (tcb) {
		ntcb = tcb->bin_flink;
		if (need_recovery) {
			task_recover(tcb);
		}
		if (tcb != btcb) {
			ret = task_terminate_unloaded(tcb);
			if (ret < 0) {
				bmdbg("Fail to terminate task of binary %d\n", binid);
			}
		}
		tcb = ntcb;
	}

#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
	if (state == BINARY_FAULT) {
		binp = BIN_LOADINFO(bin_idx);
		binp->reload = true;
	}
#endif

	/* Finally, unload binary */
	ret = task_terminate_unloaded(btcb);
	if (ret < 0) {
		bmdbg("Fail to unload binary %s\n", BIN_NAME(bin_idx));
		/* Recover binary state on failure */
		BIN_STATE(bin_idx) = state;
		return ERROR;
	}
	bmvdbg("Unload binary %s\n", BIN_NAME(bin_idx));

	BIN_STATE(bin_idx) = BINARY_INACTIVE;
	BIN_RTLIST(bin_idx) = NULL;
	BIN_NRTLIST(bin_idx) = NULL;

	/* Notify 'Unloaded' state to other binaries */
	binary_manager_notify_state_changed(bin_idx, BINARY_UNLOADED);

	return OK;
}

/****************************************************************************
 * Name: loading_thread
 *
 * Description:
 *   This thread loads binary with binary index.
 *
 ****************************************************************************/
static int loading_thread(int argc, char *argv[])
{
	if (argc <= 1) {
		bmdbg("Invalid arguments for loading, argc %d\n", argc);
		return ERROR;
	}

	/* argv[1] binary index for loading */
	return binary_manager_load((int)atoi(argv[1]));
}

/****************************************************************************
 * Name: loadingall_thread
 *
 * Description:
 *   This function loads all user binaries registered in binary table.
 *
 ****************************************************************************/
static int loadingall_thread(int argc, char *argv[])
{
	int ret;
	int bin_idx;
	int load_cnt;
	uint32_t bin_count;

	if (!binary_manager_scan_ubin_all()) {
		return BINMGR_OPERATION_FAIL;
	}

#ifdef CONFIG_SUPPORT_COMMON_BINARY
	ret = binary_manager_load(BM_CMNLIB_IDX);
	if (ret < 0) {
		return BINMGR_OPERATION_FAIL;
	}
#endif

	load_cnt = 0;
	bin_count = binary_manager_get_ucount();


	/* Load the binaries with high priority directly */
	for (bin_idx = 1; bin_idx <= bin_count; bin_idx++) {
		if (BIN_LOAD_PRIORITY(bin_idx, BIN_USEIDX(bin_idx)) == BINARY_LOADPRIO_HIGH) {
			ret = binary_manager_load(bin_idx);
			if (ret > 0) {
				load_cnt++;
			}
		}
	}

	/* Yield loading of other binaries to loader with lower priority */
	for (bin_idx = 1; bin_idx <= bin_count; bin_idx++) {
		if (BIN_LOAD_PRIORITY(bin_idx, BIN_USEIDX(bin_idx)) < BINARY_LOADPRIO_HIGH) {
			ret = binary_manager_execute_loader(LOADCMD_LOAD, bin_idx);
			if (ret == OK) {
				load_cnt++;
			}
		}
	}

	if (load_cnt > 0) {
		return load_cnt;
	}

	return BINMGR_OPERATION_FAIL;
}

#ifdef CONFIG_BINMGR_RECOVERY
/****************************************************************************
 * Name: reloading_thread
 *
 * Description:
 *   This function terminates all the task/thread created by the binary.
 *   If common binary is enabled, it terminates all user binaries and common binary.
 *   Otherwise, it terminates a faulty binary only.
 *   It is called after all children are excluded from scheduling by fault recovery.
 *   It terminates all children of binary, unloads binary and then it will load the binary.
 *
 ****************************************************************************/
static int reloading_thread(int argc, char *argv[])
{
	int ret;
	int load_cmd;

	if (argc <= 1) {
		bmdbg("Invalid arguments for reloading, argc %d\n", argc);
		return ERROR;
	}

	/* argv[1] binary index for reloading */
	int bin_idx = (int)atoi(argv[1]);
#ifdef CONFIG_SUPPORT_COMMON_BINARY
	int bidx;
	int bin_count = binary_manager_get_ucount();

	/*
	 * Unload all user binaries and common binary in sequence.
	 * The threads of common binary are linked to a list of threads of user binary.
	 * So the common binary should be unloaded after termination of all user binaries
	 * to clear used resources normally.
	 */
	load_cmd = LOADCMD_LOAD_ALL;

	/* Unload all user binaries and common binary */
	for (bidx = 0; bidx <= bin_count; bidx++) {
		ret = binary_manager_terminate_binary(bidx);
		if (ret != OK) {
			return BINMGR_OPERATION_FAIL;
		}
		bmvdbg("Terminated binary %d\n", bidx);
	}
#else
	load_cmd = LOADCMD_LOAD;

	/* Unload the faulty binary */
	ret = binary_manager_terminate_binary(bin_idx);
	if (ret != OK) {
		return BINMGR_OPERATION_FAIL;
	}
#endif

	/* Deinitialize modules in kernel */
	binary_manager_deinit_modules();

	/* Create a loader to reload binary */
	ret = binary_manager_execute_loader(load_cmd, bin_idx);
	if (ret != OK) {
		bmdbg("Fail to execute loader to reload binary, %d\n", ret);
		return BINMGR_OPERATION_FAIL;
	}

	return BINMGR_OK;
}
#endif

/****************************************************************************
 * Name: update_thread
 *
 * Description:
 *   This thread updates binary with binary index.
 *   It terminates all the task/thread created by the binary and loads the binary.
 *
 ****************************************************************************/
static int update_thread(int argc, char *argv[])
{
	int ret;
	int bin_idx;
	uint32_t bin_count = binary_manager_get_ucount();

	/* Check whether there is binary for update.
	 * If kernel binary to update exists, board will be rebooted.
	 * And if common or user biaries binary to update exist, it will be return BINMGR_OK.
	 */
	ret = binary_manager_check_update();
	if (ret < 0) {
		return ret;
	}

#ifdef CONFIG_BINMGR_RELOAD_REBOOT // Board Reset for binary reloading
	printf("==> [REBOOT] Board will be rebooted for new binary loading");
	binary_manager_reset_board(REBOOT_SYSTEM_BINARY_UPDATE);
#else
	/* Else, Reload all binaries */
	for (bin_idx = 1; bin_idx <= bin_count; bin_idx++) {
		if (BIN_STATE(bin_idx) == BINARY_LOADED || BIN_STATE(bin_idx) == BINARY_RUNNING) {
			ret = binary_manager_terminate_binary(bin_idx);
			if (ret != OK) {
				return BINMGR_OPERATION_FAIL;
			}
			bmvdbg("Terminated binary %d\n", bin_idx);
		}
	}

#ifdef CONFIG_SUPPORT_COMMON_BINARY
	/* Finally, Unload common library */
	ret = binary_manager_terminate_binary(BM_CMNLIB_IDX);
	if (ret != OK) {
		return BINMGR_OPERATION_FAIL;
	}
#endif

	/* Deinitialize modules in kernel */
	binary_manager_deinit_modules();

#ifdef CONFIG_USE_BP
	/* Update boot parameter data */
	binary_manager_update_bpinfo();
#endif

	/* Load binary */
	ret = binary_manager_execute_loader(LOADCMD_LOAD_ALL, bin_idx);
	if (ret != OK) {
		return BINMGR_OPERATION_FAIL;
	}
#endif
	return BINMGR_OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: binary_manager_release_binary_sem
 *
 * Description:
 *	 This function releases all kernel semaphores held by the threads in binary.
 *
 ****************************************************************************/
void binary_manager_release_binary_sem(int bin_idx)
{
	sem_t *sem;
	irqstate_t flags;
	FAR struct semholder_s *holder;

	flags = enter_critical_section();

	sem = (sem_t *)sq_peek(&g_sem_list);
	if (sem == NULL) {
		bmdbg("g_sem_list is empty.\n");
	} else {
		do {
#if CONFIG_SEM_PREALLOCHOLDERS > 0
			for (holder = sem->hhead; holder; holder = holder->flink)
#else
			holder = &sem->holder;
#endif
			{
				if (holder && holder->htcb && holder->htcb->group && holder->htcb->group->tg_binidx == bin_idx) {
					/* Increase semcount and release itself from holder */
					sem->semcount++;

					/* And after releasing the kernel sem, there can be a task which waits that sem. So unblock the waiting task. */
					sem_unblock_task(sem, holder->htcb);
				}
			}
			sem = sq_next(sem);
		} while (sem);
	}
	leave_critical_section(flags);
}

/****************************************************************************
 * Name: binary_manager_execute_loader
 *
 * Description:
 *	 This function creates loading thread to load/unload binary.
 *
 ****************************************************************************/
int binary_manager_execute_loader(int cmd, int bin_idx)
{
	int ret;
	uint8_t loader_priority;
	main_t loader_func;
	char data_str[2];
	char *loading_data[LOADER_ARGC + 1];

	if (bin_idx < 0 || bin_idx > USER_BIN_COUNT) {
		bmdbg("Invalid binary index %d\n", bin_idx);
		return ERROR;
	}

	memset(loading_data, 0, sizeof(char *) * (LOADER_ARGC + 1));
	loading_data[0] = itoa(bin_idx, data_str, 10);
	loading_data[1] = NULL;

	loader_priority = LOADER_PRIORITY_HIGH;
	switch (cmd) {
	case LOADCMD_LOAD:
		loader_priority = binary_manager_get_loader_priority(BIN_LOAD_PRIORITY(bin_idx, BIN_USEIDX(bin_idx)));
		if (loader_priority <= 0) {
			bmdbg("Invalid loading priority : %u\n", BIN_LOAD_PRIORITY(bin_idx, BIN_USEIDX(bin_idx)));
			return ERROR;
		}
		loader_func = loading_thread;
		break;
	case LOADCMD_LOAD_ALL:
		loader_func = loadingall_thread;
		break;
#ifdef CONFIG_BINMGR_RECOVERY
	case LOADCMD_RELOAD:
		loader_func = reloading_thread;
		break;
#endif
	case LOADCMD_UPDATE:
		loader_func = update_thread;
		break;
	default:
		bmdbg("Invalid loading cmd : %u\n", cmd);
		return ERROR;
	}

	/* Create a loader */
	ret = kernel_thread(LOADER_NAME, loader_priority, LOADER_STACKSIZE, loader_func, (char * const *)loading_data);
	if (ret > 0) {
		bmvdbg("Execute loading thread with pid %d\n", ret);
		ret = OK;
	} else {
		bmdbg("Fail to create loading thread for binary idx %d, errno %d\n", bin_idx, errno);
	}

	return ret;
}
