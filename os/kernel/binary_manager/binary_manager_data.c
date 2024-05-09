/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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
#include <debug.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#ifdef CONFIG_APP_BINARY_SEPARATION
#include <queue.h>
#include <stdint.h>
#ifdef CONFIG_VERSION_STRING
#include <stdlib.h>
#endif
#include <tinyara/sched.h>
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
#include <tinyara/binfmt/binfmt.h>
#endif
#endif
#include <tinyara/mm/mm.h>
#include <tinyara/binary_manager.h>
#include <tinyara/reboot_reason.h>
#ifdef CONFIG_BINARY_SIGNING
#include <tinyara/signature.h>
#endif

#ifdef CONFIG_APP_BINARY_SEPARATION
#include "sched/sched.h"
#endif
#include "binary_manager/binary_manager.h"

#ifdef CONFIG_BINARY_SIGNING
#include <tinyara/signature.h>
#endif

/****************************************************************************
 * Private Definitions
 ****************************************************************************/
#ifdef CONFIG_APP_BINARY_SEPARATION
/* Kernel version (not implemented, it will be modified) */
#ifdef CONFIG_VERSION_STRING
#define KERNEL_VER               atof(CONFIG_VERSION_STRING)
#else
#define KERNEL_VER               3.0
#endif

/* Table for User binaries */
/* Binary table, the first data [0] is for Common Library. */
static binmgr_uinfo_t bin_table[USER_BIN_COUNT + 1];
static uint32_t g_bin_count;
#endif

/* Data for Kernel partitions */
static binmgr_kinfo_t kernel_info;

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: partitions_get_kcount
 *
 * Description:
 *	 This function gets the number of registered partitions for kernel.
 *
 ****************************************************************************/
uint32_t binary_manager_get_kcount(void)
{
	return kernel_info.part_count;
}

/****************************************************************************	
 * Name: binary_manager_get_kdata	
 *	
 * Description:	
 *	 This function gets a kernel data.	
 *	
 ****************************************************************************/	
binmgr_kinfo_t *binary_manager_get_kdata(void)	
{	
	return &kernel_info;	
}

void binary_manager_register_kpart(int part_num, int part_size, int part_offset)
{
	if (part_num < 0 || part_size <= 0 || part_offset < 0 || kernel_info.part_count >= KERNEL_BIN_COUNT) {
		bmdbg("Invalid kernel partition : num %d, size %d, offset %d\n", part_num, part_size, part_offset);
		return;
	}

	kernel_info.part_info[kernel_info.part_count].size = part_size;
	kernel_info.part_info[kernel_info.part_count].devnum = part_num;
	kernel_info.part_info[kernel_info.part_count].address = CONFIG_FLASH_START_ADDR + part_offset;

	bmvdbg("[KERNEL %d] part num %d size %d, address 0x%x\n", kernel_info.part_count, part_num, part_size, CONFIG_FLASH_START_ADDR + part_offset);

	kernel_info.part_count++;
}

/****************************************************************************
 * Name: binary_manager_scan_kbin
 *
 * Description:
 *	 This function scans kernel binaries and update information.
 *
 ****************************************************************************/
bool binary_manager_scan_kbin(void)
{
	int ret;
	kernel_binary_header_t header_data;
	char filepath[BINARY_PATH_LEN];

#ifdef CONFIG_USE_BP
	binmgr_bpdata_t *bp_data;
	bp_data = binary_manager_get_bpdata();
	/* Verify running kernel binary based on bootparam */
	snprintf(filepath, BINARY_PATH_LEN, BINMGR_DEVNAME_FMT, kernel_info.part_info[bp_data->active_idx].devnum);
	ret = binary_manager_read_header(BINARY_KERNEL, filepath, &header_data, false);
	if (ret == OK) {
		/* Update inuse index and kernel version */
		kernel_info.inuse_idx = bp_data->active_idx;
		kernel_info.version = header_data.version;
		bmvdbg("Kernel version [%u] %u\n", kernel_info.inuse_idx, kernel_info.version);
		return true;
	}
#else
	uint32_t latest_ver = 0;

	for (int part_idx = 0; part_idx < kernel_info.part_count; part_idx++) {
		snprintf(filepath, BINARY_PATH_LEN, BINMGR_DEVNAME_FMT, kernel_info.part_info[part_idx].devnum);
		ret = binary_manager_read_header(BINARY_KERNEL, filepath, &header_data, true); /* do we need to do crc check? */
		if (ret == OK && latest_ver < header_data.version) {
			/* Update latest version and inuse index */
			kernel_info.version = header_data.version;
			kernel_info.inuse_idx = part_idx;
			latest_ver = header_data.version;
		}
	}

	bmvdbg("Kernel version [%u] %u\n", kernel_info.inuse_idx, kernel_info.version);

	/* Found valid binary */
	if (kernel_info.version != 0) {
		return true;
	}
#endif

	return false;
}

/*************************************************************************************
* Name: binary_manager_check_kernel_update
*
* Description:
*   This function checks that new kernel binary exists on inactive partition
*  and verifies the update is needed by comparing running version with new version.
*
*************************************************************************************/
int binary_manager_check_kernel_update(void)
{
	int ret;
	int inactive_partidx;
	char filepath[BINARY_PATH_LEN];
	kernel_binary_header_t header_data;

	inactive_partidx = kernel_info.inuse_idx ^ 1;

#ifdef CONFIG_BINARY_SIGNING
	/* Check signature */
	ret = up_verify_kernelsignature(kernel_info.part_info[inactive_partidx].address);
	if (ret == SIGNATURE_VAILD) {
		bmvdbg("Kernel Signature Checking Success\n");
	} else {
		bmdbg("Invalid Kernel Signature, address : 0x%x\n", kernel_info.part_info[inactive_partidx].address);
		return BINMGR_NOT_FOUND;
	}
#endif

	/* Verify kernel binary on the partition without running binary */
	snprintf(filepath, BINARY_PATH_LEN, BINMGR_DEVNAME_FMT, kernel_info.part_info[inactive_partidx].devnum);
	ret = binary_manager_read_header(BINARY_KERNEL, filepath, (void *)&header_data, true);
	if (ret == BINMGR_OK) {
#ifdef CONFIG_BINMGR_UPDATE_SAME_VERSION
		if (kernel_info.version <= header_data.version) {
#else
		if (kernel_info.version < header_data.version) {
#endif
			/* Need to update bootparam and reboot */
			bmvdbg("Found new kernel binary in inactive partition %d\n", kernel_info.part_info[inactive_partidx].devnum);
			return BINMGR_OK;
		} else {
			bmvdbg("Latest version is running, version %d\n", kernel_info.version);
			return BINMGR_ALREADY_UPDATED;
		}
	}
	return ret;
}

/*************************************************************************************
* Name: binary_manager_check_update
*
* Description:
*  This function checks whether there is update.
*  In case of boot parameters, it gets the latest bootparam and compares active index written in bootparam with current active index.
*  If they are different, reboot the board to boot kernel binary based on the latest bootparam.
*  Otherwise, it checks whether the binary to update exist in their own inactive partition.
*
*************************************************************************************/
int binary_manager_check_update(void)
{
	int ret;
#ifdef CONFIG_USE_BP
	binmgr_bpinfo_t bp_info;

	/* Get the latest bootparam */
	ret = binary_manager_scan_bootparam(&bp_info);
	if (ret < 0) {
		bmdbg("Fail to scan BP %d\n", ret);
		return ret;
	}

	/* Compare bootparam version with current running version */
	if (binary_manager_get_bpdata()->version >= bp_info.bp_data.version) {
		/* No bootparam update */
		bmdbg("All binaries are running based on BP\n");
		return BINMGR_NOT_FOUND;
	}

	/* Do kernel need to update? */
	if (binary_manager_get_kdata()->inuse_idx != bp_info.bp_data.active_idx) {
		/* Running partition and partition written in the latest BP are different.
		 * Reboot to switch kernel binary in another partition. */
		bmvdbg("Need to update to kernel binary in partition %d in the latest BP.\n", binary_manager_get_kdata()->part_info[bp_info.bp_data.active_idx].devnum);
		goto reboot;
	}
#else
	ret = binary_manager_check_kernel_update();
	if (ret == BINMGR_OK) {
		/* Reboot to switch kernel binary in another partition. */
		goto reboot;
	} else if (ret != BINMGR_ALREADY_UPDATED && ret != BINMGR_NOT_FOUND) {
		bmdbg("Failed to check for kernel update %d\n", ret);
		return ret;
	}

#ifdef CONFIG_APP_BINARY_SEPARATION
	/* Check if new common binary or user binaries exist */
	int bin_idx;
	uint32_t bin_count = binary_manager_get_ucount();
	bool need_update = false;

	/* Reload binaries if new binary is scanned */
#ifdef CONFIG_SUPPORT_COMMON_BINARY
	for (bin_idx = BM_CMNLIB_IDX; bin_idx <= bin_count; bin_idx++) {
#else
	for (bin_idx = 1; bin_idx <= bin_count; bin_idx++) {
#endif
		/* Scan binary files */
		ret = binary_manager_check_user_update(bin_idx);
		if (ret == BINMGR_OK) {
			/* Update index for inactive partition */
			BIN_USEIDX(bin_idx) ^= 1;
			need_update = true;
		} else {
			bmdbg("Fail to check user update: bin_idx %d, ret %d\n", bin_idx, ret);
		}
	}

	if (!need_update) {
		bmdbg("No App binaries to update\n");
		return BINMGR_ALREADY_UPDATED;
	}
#endif
#endif
	return BINMGR_OK;

reboot:
	/* No, Reboot for kernel update */
	printf("==> [REBOOT] Board will be rebooted for new binary loading");
	binary_manager_reset_board(REBOOT_SYSTEM_BINARY_UPDATE);

	return BINMGR_OK;
}


#ifdef CONFIG_APP_BINARY_SEPARATION
/****************************************************************************
 * Name: binary_manager_get_ucount
 *
 * Description:
 *	 This function gets the number of user binaries.
 *
 ****************************************************************************/
uint32_t binary_manager_get_ucount(void)
{
	return g_bin_count;
}

/****************************************************************************
 * Name: binary_manager_get_udata
 *
 * Description:
 *	 This function gets a row of binary table with bin_idx.
 *
 ****************************************************************************/
binmgr_uinfo_t *binary_manager_get_udata(uint32_t bin_idx)
{
	return &bin_table[bin_idx];
}

/****************************************************************************
 * Name: binary_manager_register_upart
 *
 * Description:
 *	 This function registers a partition of user binaries.
 *
 ****************************************************************************/
void binary_manager_register_upart(char *name, int part_num, int part_size, int part_offset)
{
	int bin_idx;

	if (part_num < 0 || part_size <= 0 || part_offset < 0) {
		bmdbg("Invalid user partition : num %d, size %d, offset : %d\n", part_num, part_size, part_offset);
		return;
	}

	for (bin_idx = 0; bin_idx <= g_bin_count; bin_idx++) {
		/* Already Registered */
		if (!strncmp(BIN_NAME(bin_idx), name, strlen(name) + 1)) {
			BIN_COUNT(bin_idx)++;
			BIN_PARTNUM(bin_idx, 1) = part_num;
			BIN_PARTSIZE(bin_idx, 1) = part_size;
			BIN_PARTADDR(bin_idx, 1) = CONFIG_FLASH_START_ADDR + part_offset;
			bmvdbg("[USER%d : 2] %s size %d num %d, address 0x%x\n", bin_idx, BIN_NAME(bin_idx), BIN_PARTSIZE(bin_idx, 1), BIN_PARTNUM(bin_idx, 1), BIN_PARTADDR(bin_idx, 1));
			return;
		}
	}
#ifdef CONFIG_SUPPORT_COMMON_BINARY
	if (!strncmp(name, BM_CMNLIB_NAME, sizeof(BM_CMNLIB_NAME))) {
		bin_idx = BM_CMNLIB_IDX;
	} else
#endif
	{
		/* Check whether new binary can be registered */
		if (g_bin_count >= USER_BIN_COUNT) {
			bmdbg("The maximum number of user binaries exceeded. registered user count: %u\n", g_bin_count);
			return;
		}
		bin_idx = ++g_bin_count;
	}

	/* Initialize binary table and register binary name, version and priority */
	BIN_ID(bin_idx) = -1;
	BIN_COUNT(bin_idx)++;
	BIN_RTLIST(bin_idx) = NULL;
	BIN_NRTLIST(bin_idx) = NULL;
	BIN_STATE(bin_idx) = BINARY_INACTIVE;
	BIN_PARTNUM(bin_idx, 0) = part_num;
	BIN_PARTSIZE(bin_idx, 0) = part_size;
	BIN_PARTADDR(bin_idx, 0) = CONFIG_FLASH_START_ADDR + part_offset;
	strncpy(BIN_NAME(bin_idx), name, BIN_NAME_MAX - 1);
	BIN_NAME(bin_idx)[BIN_NAME_MAX - 1] = '\0';
	sq_init(&BIN_CBLIST(bin_idx));
	bmvdbg("[USER%d : 1] %s size %d num %d, address 0x%x\n", bin_idx, BIN_NAME(bin_idx), BIN_PARTSIZE(bin_idx, 0), BIN_PARTNUM(bin_idx, 0), BIN_PARTADDR(bin_idx, 0));
}

/****************************************************************************
 * Name: binary_manager_scan_ubin_all
 *
 * Description:
 *	 This function scans user binaries and update information based on bootparam.
 *
 ****************************************************************************/
bool binary_manager_scan_ubin_all(void)
{
	int ret;
	int bin_idx;
	int part_idx;
	bool is_found;
#ifdef CONFIG_USE_BP
	int bp_app_idx;
	uint32_t version;
	binmgr_bpdata_t *bp_data;
#endif
#ifdef CONFIG_SUPPORT_COMMON_BINARY
	common_binary_header_t common_header_data;
#endif
	user_binary_header_t user_header_data;
	char devpath[BINARY_PATH_LEN];

	is_found = false;

#ifdef CONFIG_USE_BP
	bp_data = binary_manager_get_bpdata();
	/* Update user binary data based on bootparam */
	for (bp_app_idx = 0; bp_app_idx < bp_data->app_count; bp_app_idx++) {
		bin_idx = binary_manager_get_index_with_name(bp_data->app_data[bp_app_idx].name);
		if (bin_idx < 0) {
			bmdbg("Fail to find matched binary %s in binary table \n", bp_data->app_data[bp_app_idx].name);
			continue;
		}
		BIN_BPIDX(bin_idx) = bp_app_idx;
		part_idx = bp_data->app_data[bp_app_idx].useidx;
		snprintf(devpath, BINARY_PATH_LEN, BINMGR_DEVNAME_FMT, BIN_PARTNUM(bin_idx, part_idx));
#ifdef CONFIG_SUPPORT_COMMON_BINARY
		if (bin_idx == BM_CMNLIB_IDX) {
			ret = binary_manager_read_header(BINARY_COMMON, devpath, (void *)&common_header_data, false);			
			version = common_header_data.version;
		} else
#endif
		{
			ret = binary_manager_read_header(BINARY_USERAPP, devpath, (void *)&user_header_data, false);			
			version = user_header_data.bin_ver;
		}
		if (ret == OK) {
			/* Return true it there is at least one valid binary */
			BIN_USEIDX(bin_idx) = part_idx;
			BIN_VER(bin_idx, part_idx) = version;			
			bmdbg("Found binary %s in partition %d, version %d\n", BIN_NAME(bin_idx), BIN_PARTNUM(bin_idx, part_idx), version);
			if (bin_idx != BM_CMNLIB_IDX) {
				BIN_LOAD_PRIORITY(bin_idx, part_idx) = user_header_data.loading_priority;
			}
			/* Return true it there is at least one valid binary */
			is_found = true;
		} else {
			bmdbg("Fail to find valid binary %s based on BP. \n", BIN_NAME(bin_idx));
		}
	}
#else
	uint32_t latest_ver = 0;

#ifdef CONFIG_SUPPORT_COMMON_BINARY
	/* Scan Common binary on its own partitions */
	bin_idx = BM_CMNLIB_IDX;
	for (part_idx = 0; part_idx < PARTS_PER_BIN; part_idx++) {
		snprintf(devpath, BINARY_PATH_LEN, BINMGR_DEVNAME_FMT, BIN_PARTNUM(bin_idx, part_idx));
		ret = binary_manager_read_header(BINARY_COMMON, devpath, (void *)&common_header_data, false);
		if (ret == OK) {
			is_found = true;
			if (latest_ver < common_header_data.version) {
				/* Return true it there is at least one valid binary */
				BIN_USEIDX(bin_idx) = part_idx;
				BIN_VER(bin_idx, part_idx) = common_header_data.version;
				latest_ver = common_header_data.version;
			}
			bmdbg("Found binary %s in partition %d, version %d\n", BIN_NAME(bin_idx), BIN_PARTNUM(bin_idx, part_idx), common_header_data.version);
		} else {
			bmdbg("Fail to find valid binary %s in partition %d\n", BIN_NAME(bin_idx), BIN_PARTNUM(bin_idx, part_idx));
		}
	}
#endif
	/* Scan User binaries based on their own partitions */

	for (bin_idx = 1; bin_idx <= g_bin_count; bin_idx++) {
		latest_ver = 0;
		for (part_idx = 0; part_idx < PARTS_PER_BIN; part_idx++) {
			snprintf(devpath, BINARY_PATH_LEN, BINMGR_DEVNAME_FMT, BIN_PARTNUM(bin_idx, part_idx));
			ret = binary_manager_read_header(BINARY_USERAPP, devpath, (void *)&user_header_data, false);
			if (ret == OK) {
				is_found = true;
				if (latest_ver < user_header_data.bin_ver) {
					BIN_USEIDX(bin_idx) = part_idx;
					BIN_VER(bin_idx, part_idx) = user_header_data.bin_ver;
					BIN_LOAD_PRIORITY(bin_idx, part_idx) = user_header_data.loading_priority;
					latest_ver = user_header_data.bin_ver;
				}
				bmdbg("Found binary %s in partition %d, version %d\n", BIN_NAME(bin_idx), BIN_PARTNUM(bin_idx, part_idx), user_header_data.bin_ver);
			} else {
				bmdbg("Fail to find valid binary %s in partition %d\n", BIN_NAME(bin_idx), BIN_PARTNUM(bin_idx, part_idx));
			}
		}
	}
#endif

	return is_found;
}

/********************************************************************************************
 * Name: binary_manager_check_user_update
 *
 * Description:
 *	 This function checks that new user binary exists on inactive partition
 *	and verifies the update is needed by comparing running version with new version.
 *
 *******************************************************************************************/
int binary_manager_check_user_update(int bin_idx)
{
	int ret;
	int part_idx;
	uint32_t version;
	uint32_t running_ver;
	char devpath[BINARY_PATH_LEN];
	common_binary_header_t common_header_data;
	user_binary_header_t user_header_data;

	if (bin_idx < 0 || bin_idx > binary_manager_get_ucount()) {
		bmdbg("Invalid bin idx %d\n", bin_idx);
		return BINMGR_INVALID_PARAM;
	}
 
	running_ver = BIN_VER(bin_idx, BIN_USEIDX(bin_idx));
	part_idx = BIN_USEIDX(bin_idx) ^ 1;

	bmvdbg("Checking [%d] current version %d, part_idx %d\n", bin_idx, running_ver, part_idx);

#ifdef CONFIG_BINARY_SIGNING
	/* Check signature */
	ret = up_verify_usersignature(BIN_PARTADDR(bin_idx, part_idx));
	if (ret == SIGNATURE_VAILD) {
		bmvdbg("%s Signature Checking Success\n", BIN_NAME(bin_idx));
	} else {
		bmdbg("Invalid Signature, name : %s, address : 0x%x\n", BIN_NAME(bin_idx), BIN_PARTADDR(bin_idx, (BIN_USEIDX(bin_idx))));
		return BINMGR_NOT_FOUND;
	}
#endif
	snprintf(devpath, BINARY_PATH_LEN, BINMGR_DEVNAME_FMT, BIN_PARTNUM(bin_idx, part_idx));
	if (bin_idx == BM_CMNLIB_IDX) {
		ret = binary_manager_read_header(BINARY_COMMON, devpath, (void *)&common_header_data, true);
		version = common_header_data.version;
	} else {
		ret = binary_manager_read_header(BINARY_USERAPP, devpath, (void *)&user_header_data, true);
		version = user_header_data.bin_ver;
	}
	if (ret == BINMGR_OK) {		
		BIN_VER(bin_idx, part_idx) = version;
		if (bin_idx != BM_CMNLIB_IDX) {
			BIN_LOAD_PRIORITY(bin_idx, part_idx) = user_header_data.loading_priority;
		}
#ifdef CONFIG_BINMGR_UPDATE_SAME_VERSION
		if (running_ver <= version) {
#else
		if (running_ver < version) {
#endif
 			bmvdbg("Found Latest version %u in part %d\n", version, BIN_PARTNUM(bin_idx, part_idx));
			return BINMGR_OK;
		} else {
			bmdbg("No update! Latest version is running, version %d\n", running_ver);
			return BINMGR_ALREADY_UPDATED;
		}
	}

	return ret;
}

/****************************************************************************
 * Name: binary_manager_update_running_state
 *
 * Description:
 *	 This function update binary state to BINARY_RUNNING state.
 *   And notify other binaries changed state.
 *
 ****************************************************************************/
void binary_manager_update_running_state(int bin_id)
{
	int bin_idx;
	struct tcb_s *tcb;

	tcb = sched_gettcb(bin_id);
	if (tcb && tcb->group && tcb->group->tg_binidx > 0) {
		bin_idx = tcb->group->tg_binidx;
		BIN_STATE(bin_idx) = BINARY_RUNNING;
		bmvdbg("binary '%s' state is changed, state = %d.\n", BIN_NAME(bin_idx), BIN_STATE(bin_idx));

		/* Notify that binary is started. */
		binary_manager_notify_state_changed(bin_idx, BINARY_STARTED);
	}
}

/****************************************************************************
 * Name: binary_manager_add_binlist
 *
 * Description:
 *	 This function adds tcb to binary list.
 *
 ****************************************************************************/
void binary_manager_add_binlist(FAR struct tcb_s *tcb)
{
	int bin_idx;

	if (!tcb) {
		bmdbg("ERROR: tcb parameter is NULL\n");
		return;
	}

	if (!tcb->group) {
		/* It is possible that the task / pthread has already exited at the time of calling this function.
		 * In this case, the group member will be NULL and will cause an exception if we try to dereference
		 * the pointer. So, perform a null check here. If group is null, it is ok to not add the tcb to our
		 * list because, task has already exited.
		 */
		bmdbg("Fail to add pid %d to binlist. This task has already exited and group is NULL\n", tcb->pid);
		return;
	}

	bin_idx = tcb->group->tg_binidx;

	/* A binary index, bin_idx is greater than 0 only if tcb is a thread of user binary.
	 * Add tcb to the binary list for fault recovery in case of threads of user binary.
	 * Otherwise, it is unnecessary for idle or kernel threads. */
	if (bin_idx > 0) {
		/* Add a tcb to a head of list */
		if (tcb->sched_priority > BM_PRIORITY_MAX) {
			if (BIN_RTLIST(bin_idx)) {
				tcb->bin_flink = BIN_RTLIST(bin_idx);
				BIN_RTLIST(bin_idx)->bin_blink = tcb;
			}
			BIN_RTLIST(bin_idx) = tcb;
		} else {
			if (BIN_NRTLIST(bin_idx)) {
				tcb->bin_flink = BIN_NRTLIST(bin_idx);
				BIN_NRTLIST(bin_idx)->bin_blink = tcb;
			}
			BIN_NRTLIST(bin_idx) = tcb;
		}
	}
}

/****************************************************************************
 * Name: binary_manager_remove_binlist
 *
 * Description:
 *	 This function removes tcb from binary list.
 *
 ****************************************************************************/
void binary_manager_remove_binlist(FAR struct tcb_s *tcb)
{
	int bin_idx;
	struct tcb_s *prev;
	struct tcb_s *next;

	bin_idx = tcb->group->tg_binidx;
	if (bin_idx > 0) {
		/* Remove a tcb from the thread list of binary */
		prev = tcb->bin_blink;
		next = tcb->bin_flink;
		if (!prev) {
			if (tcb->sched_priority > BM_PRIORITY_MAX) {
				BIN_RTLIST(bin_idx) = next;
			} else {
				BIN_NRTLIST(bin_idx) = next;
			}
		} else {
			prev->bin_flink = next;
		}
		if (next) next->bin_blink = prev;
	}
}

/****************************************************************************
 * Name: binary_manager_clear_bindata
 *
 * Description:
 *	 This function clears data of binary with bin_idx in a binary table.
 *
 ****************************************************************************/
void binary_manager_clear_bindata(int bin_idx)
{
	if (bin_idx < 0 || bin_idx > USER_BIN_COUNT) {
		return;
	}

#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
	struct tcb_s *tcb;

	tcb = sched_gettcb(BIN_ID(bin_idx));
	/* If 'reload' flag is true, a binary manager uses loading data 'bininfo' when loading the binary later.
	 * So clear BIN_LOADINFO which points loading data if flag is false.
	 */
	if (tcb && ((struct task_tcb_s *)tcb)->bininfo && !((struct task_tcb_s *)tcb)->bininfo->reload) {
		BIN_LOADINFO(bin_idx) = NULL;
	}
#endif /* CONFIG_OPTIMIZE_APP_RELOAD_TIME */
	BIN_STATE(bin_idx) = BINARY_INACTIVE;
	BIN_ID(bin_idx) = -1;
	BIN_RTLIST(bin_idx) = NULL;
	BIN_NRTLIST(bin_idx) = NULL;
	/* Clear registered callbacks of binary */
	binary_manager_clear_bin_statecb(bin_idx);
}
#endif
