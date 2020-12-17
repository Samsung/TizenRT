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
#include <crc32.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>
#include <dirent.h>
#include <stdint.h>
#include <string.h>
#include <queue.h>
#include <semaphore.h>
#include <sys/types.h>

#include <tinyara/irq.h>
#include <tinyara/mm/mm.h>
#include <tinyara/sched.h>
#include <tinyara/init.h>
#include <tinyara/kthread.h>

#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
#include <tinyara/binfmt/binfmt.h>
#endif

#include "sched/sched.h"
#include "task/task.h"
#include "semaphore/semaphore.h"

#include "binary_manager.h"

extern sq_queue_t g_sem_list;

/****************************************************************************
 * Private Definitions
 ****************************************************************************/
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
		if (ret > 0) {
			bmvdbg("Load '%s' success! pid = %d\n", path, ret);
			/* Set the data in table from header */
			BIN_LOAD_ATTR(bin_idx) = *load_attr;
			strncpy(BIN_NAME(bin_idx), load_attr->bin_name, BIN_NAME_MAX);
			bmvdbg("BIN TABLE[%d] %d %d %d %.1f %s\n", bin_idx, BIN_SIZE(bin_idx), BIN_RAMSIZE(bin_idx), BIN_LOADVER(bin_idx), BIN_KERNEL_VER(bin_idx), BIN_NAME(bin_idx));
			return OK;
		} else if (errno == ENOMEM) {
			/* Sleep for a moment to get available memory */
			usleep(1000);
		}
		retry_count++;
		lldbg("Load '%s' %dth fail, errno %d\n", BIN_NAME(bin_idx), retry_count, errno);
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
	char filepath[CONFIG_PATH_MAX];
	binary_header_t header_data;
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
	struct binary_s *binp;
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
		snprintf(filepath, CONFIG_PATH_MAX, "%s/%s_%d", BINARY_DIR_PATH, BIN_NAME(bin_idx), BIN_LOADVER(bin_idx));
	} else
#endif
	{
		bin_count = BIN_FILECNT(bin_idx);
	}

	do {
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
		if (!binp)
#endif
		{
			/* Read header data and Check crc */
			snprintf(filepath, CONFIG_PATH_MAX, "%s/%s_%d", BINARY_DIR_PATH, BIN_NAME(bin_idx), BIN_VER(bin_idx, BIN_USEIDX(bin_idx)));
			ret = binary_manager_read_header(filepath, &header_data, true);
			if (ret != OK) {
				if (--bin_count > 0) {
					bmdbg("Failed to read header %s, try to read another file\n", filepath);
					BIN_USEIDX(bin_idx) ^= 1;
					continue;
				} else {
					bmdbg("No valid binary %s file\n", BIN_NAME(bin_idx));
					break;
				}
			}
			strncpy(load_attr.bin_name, header_data.bin_name, BIN_NAME_MAX - 1);
			load_attr.bin_name[BIN_NAME_MAX - 1] = '\0';
			load_attr.bin_size = header_data.bin_size;
			load_attr.compression_type = header_data.compression_type;
			load_attr.ram_size = header_data.bin_ramsize;
			load_attr.stack_size = header_data.bin_stacksize;
			load_attr.priority = header_data.bin_priority;
			load_attr.offset = CHECKSUM_SIZE + header_data.header_size;
			load_attr.bin_ver = header_data.bin_ver;
		}
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
		else {
			load_attr = BIN_LOAD_ATTR(bin_idx);
		}
		load_attr.binp = binp;
#endif
		ret = binary_manager_load_binary(bin_idx, filepath, &load_attr);
		if (ret == OK) {
			BIN_KERNEL_VER(bin_idx) = header_data.kernel_ver;
			return BINMGR_OK;
		}
		if (--bin_count > 0) {
			/* Change index 0 to 1 and 1 to 0. */
			BIN_USEIDX(bin_idx) ^= 1;
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

	need_recovery = false;

	binid = BIN_ID(bin_idx);
	btcb = (struct tcb_s *)sched_gettcb(binid);
	if (btcb == NULL) {
		bmdbg("Failed to get main task of binary %d\n", binid);
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
			bmdbg("Failed to execute callbacks for unloading %s\n", BIN_NAME(bin_idx));
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
				bmdbg("Failed to terminate task of binary %d\n", binid);
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
				bmdbg("Failed to terminate task of binary %d\n", binid);
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
		bmdbg("Failed to unload binary %s\n", BIN_NAME(bin_idx));
		/* Recover binary state on failure */
		BIN_STATE(bin_idx) = state;
		return ERROR;
	}
	bmvdbg("Unload binary %s\n", BIN_NAME(bin_idx));

	BIN_RTLIST(bin_idx) = NULL;
	BIN_NRTLIST(bin_idx) = NULL;

	/* Notify 'Unloaded' state to other binaries */
	binary_manager_notify_state_changed(bin_idx, BINARY_UNLOADED);

	return OK;
}

#ifdef CONFIG_BINMGR_RECOVERY
/****************************************************************************
 * Name: binary_manager_reload
 *
 * Description:
 *   This function will terminate all the task/thread created by the binary
 *   i.e input binary id.
 *   It is called after all children are excluded from scheduling by fault recovery.
 *   It terminates its children, unloads binary and then it will load the binary.
 *
 * Input parameters:
 *   binid   -   The pid of binary to be reload
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static int binary_manager_reload(int bin_idx)
{
	int ret;

	/* Terminate binary */
	ret = binary_manager_terminate_binary(bin_idx);
	if (ret != OK) {
		bmdbg("Failed to terminate binary %s\n", BIN_NAME(bin_idx));
		return BINMGR_OPERATION_FAIL;
	}

	/* Load binary */
	ret = binary_manager_execute_loader(LOADCMD_LOAD, bin_idx);
	if (ret != OK) {
		bmdbg("Failed to load binary, bin_idx %d\n", bin_idx);
		return BINMGR_OPERATION_FAIL;
	}
	return BINMGR_OK;
}
#endif

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

#ifdef CONFIG_SUPPORT_COMMON_BINARY
	char libname[CONFIG_NAME_MAX];
	snprintf(libname, CONFIG_NAME_MAX, "%s%s", CONFIG_COMMON_BINARY_PATH, CONFIG_COMMON_BINARY_NAME);
	ret = load_binary(BM_CMNLIB_IDX, libname, NULL);
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
 *   This thread reloads binary with binary index.
 *
 ****************************************************************************/
static int reloading_thread(int argc, char *argv[])
{
	if (argc <= 1) {
		bmdbg("Invalid arguments for reloading, argc %d\n", argc);
		return ERROR;
	}

	/* argv[1] binary index for reloading */
	int bin_idx = (int)atoi(argv[1]);
#ifdef CONFIG_SUPPORT_COMMON_BINARY
	if (bin_idx == BM_CMNLIB_IDX) {
		int ret;

		/* Reload common library and all binaries */
		char libname[CONFIG_NAME_MAX];
		snprintf(libname, CONFIG_NAME_MAX, "%s%s", CONFIG_COMMON_BINARY_PATH, CONFIG_COMMON_BINARY_NAME);
		ret = load_binary(BM_CMNLIB_IDX, libname, NULL);
		if (ret < 0) {
			return BINMGR_OPERATION_FAIL;
		}

		int bidx;
		int bin_count = binary_manager_get_ucount();

		for (bidx = 1; bidx <= bin_count; bidx++) {
			ret = binary_manager_reload(bidx);
			if (ret < 0) {
				return ret;
			}
		}

		return ret;
	} else
#endif
	return binary_manager_reload(bin_idx);
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

	if (argc <= 1) {
		bmdbg("Invalid arguments for update, argc %d\n", argc);
		return ERROR;
	}

	/* argv[1] binary index for update */
	bin_idx = (int)atoi(argv[1]);
	if (bin_idx < 0 || bin_idx > USER_BIN_COUNT) {
		bmdbg("Invalid index %d\n", bin_idx);
		return ERROR;
	}

	if (BIN_STATE(bin_idx) < BINARY_INACTIVE || BIN_STATE(bin_idx) > BINARY_RUNNING) {
		bmdbg("Invalid binary state %d\n", BIN_STATE(bin_idx));
		return BINMGR_OPERATION_FAIL;
	}

	/* Scan binary files */
	ret = binary_manager_scan_ubin(bin_idx);
	if (ret != OK) {
		bmdbg("Failed to find binary file %s\n", BIN_NAME(bin_idx));
		return BINMGR_OPERATION_FAIL;
	}

	/* Terminate binary if binary is already loaded */
	if (BIN_STATE(bin_idx) == BINARY_LOADED || BIN_STATE(bin_idx) == BINARY_RUNNING) {
		ret = binary_manager_terminate_binary(bin_idx);
		if (ret != OK) {
			bmdbg("Failed to terminate binary %s\n", BIN_NAME(bin_idx));
			return BINMGR_OPERATION_FAIL;
		}
	}

	/* Load binary */
	ret = binary_manager_execute_loader(LOADCMD_LOAD, bin_idx);
	if (ret != OK) {
		bmdbg("Failed to load binary, bin_idx %d\n", bin_idx);
		return BINMGR_OPERATION_FAIL;
	}

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

	flags = irqsave();

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
	irqrestore(flags);
}

/****************************************************************************
 * Name: binary_manager_read_header
 *
 * Description:
 *	 This function reads header and checks whether it is valid or not.
 *
 ****************************************************************************/
int binary_manager_read_header(char *path, binary_header_t *header_data, bool crc_check)
{
	int fd;
	int ret;
	int read_size;
	int file_size;
	bool need_unlink;
	uint32_t crc_value = 0;
	uint8_t *crc_buffer;
	uint32_t crc_bufsize;

	memset(header_data, 0, sizeof(binary_header_t));
	crc_buffer = NULL;

	need_unlink = false;

	fd = open(path, O_RDONLY);
	if (fd < 0) {
		bmdbg("Failed to open %s: %d, errno %d\n", path, ret, errno);
		return ERROR;
	}

	/* Read the binary header */
	ret = read(fd, (FAR uint8_t *)header_data, sizeof(binary_header_t));
	if (ret != sizeof(binary_header_t)) {
		bmdbg("Failed to read %s: %d, errno %d\n", path, ret, errno);
		goto errout_with_fd;
	}

	/* Verify header data */
	if (header_data->bin_type != BIN_TYPE_ELF || header_data->bin_ver == 0 \
		|| header_data->loading_priority == 0 || header_data->loading_priority >= BINARY_LOADPRIO_MAX \
		|| header_data->bin_ramsize == 0) {
		need_unlink = true;
		bmdbg("Invalid header data : headersize %u, binsize %u, ramsize %u, bintype %u\n", header_data->header_size, header_data->bin_size, header_data->bin_ramsize, header_data->bin_type);
		goto errout_with_fd;
	}

	if (crc_check) {
		crc_bufsize = header_data->bin_ramsize;
		crc_buffer = (uint8_t *)kmm_malloc(crc_bufsize);
		if (!crc_buffer) {
			bmdbg("Failed to allocate buffer for checking crc, size %u\n", crc_bufsize);
			goto errout_with_fd;
		}
		/* Calculate checksum and Verify it */
		crc_value = crc32part((uint8_t *)header_data + CHECKSUM_SIZE, header_data->header_size, crc_value);
		file_size = header_data->bin_size;
		while (file_size > 0) {
			read_size = file_size < crc_bufsize ? file_size : crc_bufsize;
			ret = read(fd, (void *)crc_buffer, read_size);
			if (ret < 0 || ret != read_size) {
				bmdbg("Failed to read : %d, errno %d\n", ret, errno);
				goto errout_with_fd;
			}
			crc_value = crc32part(crc_buffer, read_size, crc_value);
			file_size -= read_size;
		}

		if (crc_value != header_data->crc_hash) {
			need_unlink = true;
			bmdbg("Failed to crc check : %u != %u\n", crc_value, header_data->crc_hash);
			goto errout_with_fd;
		}
		kmm_free(crc_buffer);
	}
	bmvdbg("Binary header : %d %d %d %d %s %d %d %.1f %d\n", header_data->header_size, header_data->bin_type, header_data->bin_size, header_data->loading_priority, header_data->bin_name, header_data->bin_ver, header_data->bin_ramsize, header_data->kernel_ver, header_data->jump_addr);
	close(fd);

	return OK;

errout_with_fd:
	close(fd);
	if (need_unlink) {
		unlink(path);
	}
	if (crc_buffer) {
		kmm_free(crc_buffer);
	}
	return ERROR;
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
	char data_str[1];
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
		bmdbg("Loading Fail, errno %d\n", errno);
	}

	return ret;
}
