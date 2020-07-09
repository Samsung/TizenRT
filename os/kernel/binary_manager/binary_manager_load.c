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
			bmvdbg("BIN TABLE[%d] %d %d %s %s %s\n", bin_idx, BIN_SIZE(bin_idx), BIN_RAMSIZE(bin_idx), BIN_VER(bin_idx), BIN_KERNEL_VER(bin_idx), BIN_NAME(bin_idx));
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
	DIR *dirp;
	int name_len;
	char *bin_name;
	int file_idx;
	int latest_ver;
	int latest_idx;
	int version;
	int valid_bin_count;
	load_attr_t load_attr;
	char filepath[FILES_PER_BIN][CONFIG_PATH_MAX];
	binary_header_t header_data[FILES_PER_BIN];
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
		valid_bin_count = 1;
		latest_idx = 0;
		snprintf(filepath[latest_idx], CONFIG_PATH_MAX, "%s/%s_%s", BINARY_DIR_PATH, BIN_NAME(bin_idx), BIN_VER(bin_idx));
	} else
#endif
	{
		bin_name = BIN_NAME(bin_idx);
		name_len = strlen(bin_name);

		latest_ver = -1;
		latest_idx = -1;
		valid_bin_count = 0;

		/* Open a directory for user binaries, BINARY_DIR_PATH */
		dirp = opendir(BINARY_DIR_PATH);
		if (!dirp) {
			bmdbg("Failed to open a directory, %s\n", BINARY_DIR_PATH);
			return ERROR;
		}

		file_idx = 0;

		bmvdbg("Open a directory, %s\n", BINARY_DIR_PATH);

		/* Read each directory entry */
		for (;;) {
			struct dirent *entryp = readdir(dirp);
			if (!entryp) {
				/* Finished with this directory */
				break;
			}
			/* Read and Verify a binary file */
			if (DIRENT_ISFILE(entryp->d_type) \
				&& !strncmp(entryp->d_name, bin_name, name_len) && entryp->d_name[name_len] == '_') {
				snprintf(filepath[file_idx], CONFIG_PATH_MAX, "%s/%s", BINARY_DIR_PATH, entryp->d_name);
				ret = binary_manager_read_header(filepath[file_idx], &header_data[file_idx]);
				if (ret == OK) {
					valid_bin_count++;
					version = (int)atoi(header_data[file_idx].bin_ver);
					bmvdbg("Found valid header in version %d\n", version);
					if (version > latest_ver) {
						latest_ver = version;
						latest_idx = file_idx;
						bmvdbg("Latest version %d, idx %d\n", latest_ver, latest_idx);
					}
				}
				file_idx++;
			}
		}
		closedir(dirp);

		if (valid_bin_count == 0 || latest_idx < 0) {
			bmdbg("No valid binary in fs : %s\n", BIN_NAME(bin_idx));
			return ERROR;
		}
	}
	do {
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
		if (!binp)
#endif
		{
			strncpy(load_attr.bin_name, header_data[latest_idx].bin_name, BIN_NAME_MAX);
			load_attr.bin_size = header_data[latest_idx].bin_size;
			load_attr.compression_type = header_data[latest_idx].compression_type;
			load_attr.ram_size = header_data[latest_idx].bin_ramsize;
			load_attr.stack_size = header_data[latest_idx].bin_stacksize;
			load_attr.priority = header_data[latest_idx].bin_priority;
			load_attr.offset = CHECKSUM_SIZE + header_data[latest_idx].header_size;
			strncpy(load_attr.bin_ver, header_data[latest_idx].bin_ver, BIN_VER_MAX);
		}
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
		else {
			load_attr = BIN_LOAD_ATTR(bin_idx);
		}
		load_attr.binp = binp;
#endif
		ret = binary_manager_load_binary(bin_idx, filepath[latest_idx], &load_attr);
		if (ret == OK) {			
			strncpy(BIN_KERNEL_VER(bin_idx), header_data[latest_idx].kernel_ver, KERNEL_VER_MAX);
			strncpy(BIN_NAME(bin_idx), header_data[latest_idx].bin_name, BIN_NAME_MAX);
			return BINMGR_OK;
		}
		if (--valid_bin_count > 0) {
			/* Change index 0 to 1 and 1 to 0. */
			latest_idx ^= 1;
		}
	} while (valid_bin_count > 0);

	return ERROR;
}

/****************************************************************************
 * Name: binary_manager_load_all
 *
 * Description:
 *	 This function loads all user binaries in binary table.
 *
 ****************************************************************************/
static int binary_manager_load_all(void)
{
	int ret;
	int bin_idx;
	int load_cnt;
	uint32_t bin_count;

#ifdef CONFIG_SUPPORT_COMMON_BINARY
	char libname[CONFIG_NAME_MAX];
	snprintf(libname, CONFIG_NAME_MAX, "%s%s", CONFIG_COMMON_BINARY_PATH, CONFIG_COMMON_BINARY_NAME);
	ret = load_binary(COMMLIB_IDX, libname, NULL);
	if (ret < 0) {
		return BINMGR_OPERATION_FAIL;
	}
#endif

	load_cnt = 0;
	bin_count = binary_manager_get_ucount();

	for (bin_idx = 1; bin_idx <= bin_count; bin_idx++) {
		ret = binary_manager_load(bin_idx);
		if (ret == OK) {
			load_cnt++;
		}
	}

	if (load_cnt > 0) {
		return load_cnt;
	}

	return BINMGR_OPERATION_FAIL;
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

	if (BIN_STATE(bin_idx) == BINARY_RUNNING) {
		need_recovery = true;
		/* Waits until some callbacks for cleanup are done if registered callbacks exist */
		BIN_STATE(bin_idx) = BINARY_WAITUNLOAD;
		ret = binary_manager_send_statecb_msg(bin_idx, BIN_NAME(bin_idx), BINARY_READYTOUNLOAD, true);
		if (ret != OK) {
			return ERROR;
		}
		/* Release all kernel semaphores held by the threads in binary */
		binary_manager_release_binary_sem(bin_idx);
		BIN_STATE(bin_idx) = BINARY_RUNNING;
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
	if (BIN_STATE(bin_idx) == BINARY_FAULT) {
		binp = BIN_LOADINFO(bin_idx);
		binp->reload = true;
	}
#endif

	/* Finally, unload binary */
	ret = task_terminate_unloaded(btcb);
	if (ret < 0) {
		bmdbg("Failed to unload binary %s\n", BIN_NAME(bin_idx));
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
	ret = binary_manager_load(bin_idx);
	if (ret != OK) {
		bmdbg("Failed to load binary, bin_idx %d\n", bin_idx);
		return BINMGR_OPERATION_FAIL;
	}
	return BINMGR_OK;
}
#endif

/****************************************************************************
 * Name: binary_manager_update
 *
 * Description:
 *   This function will terminate all the task/thread created by the binary
 *   i.e input binary name.
 *   If the binary is registered, it checks current running version and
 *   executes registered callback for unloading.
 *   And it terminates its children, unloads binary and loads the binary.
 *
 * Input parameters:
 *   bin_name   -   The name of binary to be reload
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static int binary_manager_update(char *bin_name)
{
	int ret;
	int bin_idx;

	bin_idx = binary_manager_get_index_with_name(bin_name);
	if (bin_idx < 0) {
		bmdbg("binary %s is not registered\n", bin_name);
		return BINMGR_NOT_FOUND;
	}

	if (BIN_STATE(bin_idx) < BINARY_INACTIVE || BIN_STATE(bin_idx) > BINARY_RUNNING) {
		bmdbg("Invalid binary state %d\n", BIN_STATE(bin_idx));
		return BINMGR_OPERATION_FAIL;
	}

	/* Terminate binary if binary is already loaded */
	if (BIN_STATE(bin_idx) == BINARY_LOADING_DONE || BIN_STATE(bin_idx) == BINARY_RUNNING) {
		ret = binary_manager_terminate_binary(bin_idx);
		if (ret != OK) {
			bmdbg("Failed to terminate binary %s\n", BIN_NAME(bin_idx));
			return BINMGR_OPERATION_FAIL;
		}
	}

	/* Load binary */
	ret = binary_manager_load(bin_idx);
	if (ret != OK) {
		bmdbg("Failed to load binary, bin_idx %d\n", bin_idx);
		return BINMGR_OPERATION_FAIL;
	}
	return BINMGR_OK;
}

/****************************************************************************
 * Name: loading_thread
 *
 * Description:
 *	 This thread loads or reloads binaries accroding to request type.
 *
 ****************************************************************************/
static int loading_thread(int argc, char *argv[])
{
	int ret;
	int load_cmd;

	if (argc <= 1) {
		bmdbg("Invalid arguments\n");
		return -1;
	}

	/* Arguments : [1] type */
	load_cmd = (int)atoi(argv[1]);

	ret = BINMGR_INVALID_PARAM;
	switch (load_cmd) {
	case LOADCMD_LOAD_ALL:
		ret = binary_manager_load_all();
		break;
	case LOADCMD_UPDATE:
		if (argc <= 2) {
			bmdbg("Invalid arguments for reloading, argc %d\n", argc);
			break;
		}
		/* [2] binary name for reloading */
		ret = binary_manager_update(argv[2]);
		break;
#ifdef CONFIG_BINMGR_RECOVERY
	case LOADCMD_RELOAD:
		if (argc <= 2) {
			bmdbg("Invalid arguments for reloading, argc %d\n", argc);
			break;
		}
		/* [2] binary id for reloading */
		int bin_idx = (int)atoi(argv[2]);
#ifdef CONFIG_SUPPORT_COMMON_BINARY
		if (bin_idx == BM_BINID_LIBRARY) {
			/* Reload common library and all binaries */
			char libname[CONFIG_NAME_MAX];
			snprintf(libname, CONFIG_NAME_MAX, "%s%s", CONFIG_COMMON_BINARY_PATH, CONFIG_COMMON_BINARY_NAME);
			ret = load_binary(COMMLIB_IDX, libname, NULL);
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
		} else
#endif
			ret = binary_manager_reload(bin_idx);
		break;
#endif
	default:
		bmdbg("Invalid loading command %d\n", load_cmd);
	}
	bmvdbg("Loading result %d\n", ret);

	return ret;
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
					sem_freeholder(sem, holder);
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
int binary_manager_read_header(char *path, binary_header_t *header_data)
{
	int fd;
	int ret;
	int read_size;
	int file_size;
	bool need_unlink;
	uint32_t check_crc = 0;
	uint8_t crc_buffer[CRC_BUFFER_SIZE];

	memset(header_data, 0, sizeof(binary_header_t));

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
	if (header_data->bin_type != BIN_TYPE_ELF || atoi(header_data->bin_ver) <= 0) {
		need_unlink = true;
		bmdbg("Invalid header data : headersize %d, binsize %d, ramsize %d, bintype %d\n", header_data->header_size, header_data->bin_size, header_data->bin_ramsize, header_data->bin_type);
		goto errout_with_fd;
	}

	/* Calculate checksum and Verify it */
	check_crc = crc32part((uint8_t *)header_data + CHECKSUM_SIZE, header_data->header_size, check_crc);
	file_size = header_data->bin_size;
	while (file_size > 0) {
		read_size = file_size < CRC_BUFFER_SIZE ? file_size : CRC_BUFFER_SIZE;
		ret = read(fd, (FAR uint8_t *)crc_buffer, read_size);
		if (ret < 0 || ret != read_size) {
			bmdbg("Failed to read : %d, errno %d\n", ret, errno);
			goto errout_with_fd;
		}
		check_crc = crc32part(crc_buffer, read_size, check_crc);
		file_size -= read_size;
	}

	if (check_crc != header_data->crc_hash) {
		need_unlink = true;
		bmdbg("Failed to crc check : %u != %u\n", check_crc, header_data->crc_hash);
		goto errout_with_fd;
	}

	bmvdbg("Binary header : %d %d %d %s %s %d %s %d\n", header_data->header_size, header_data->bin_type, header_data->bin_size, header_data->bin_name, header_data->bin_ver, header_data->bin_ramsize, header_data->kernel_ver, header_data->jump_addr);
	close(fd);

	return OK;

errout_with_fd:
	close(fd);
	if (need_unlink) {
		unlink(path);
	}
	return ERROR;
}

/****************************************************************************
 * Name: binary_manager_loading
 *
 * Description:
 *   This function create loading thread to load/unload binary.
 *
 ****************************************************************************/
int binary_manager_loading(char *loading_data[])
{
	int ret;

	ret = kernel_thread(LOADINGTHD_NAME, LOADINGTHD_PRIORITY, LOADINGTHD_STACKSIZE, loading_thread, (char * const *)loading_data);
	if (ret > 0) {
		bmvdbg("Execute loading thread with pid %d\n", ret);
	} else {
		bmdbg("Loading Fail\n");
	}

	return ret;
}
