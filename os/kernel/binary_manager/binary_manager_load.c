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
#include <sys/types.h>

#include <tinyara/mm/mm.h>
#include <tinyara/sched.h>
#include <tinyara/init.h>

#include "task/task.h"
#include "binary_manager.h"

/****************************************************************************
 * Private Definitions
 ****************************************************************************/

struct binary_header_s {
	uint16_t header_size;
	uint16_t bin_type;
	uint32_t bin_size;
	char bin_name[BIN_NAME_MAX];
	char bin_ver[BIN_VER_MAX];
	uint32_t ram_size;
	char kernel_ver[KERNEL_VER_MAX];
	uint32_t jump_addr;
};
typedef struct binary_header_s binary_header_t;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int binary_manager_verify_checksum(int fd, int file_size, uint32_t crc_hash)
{
	int read_size;
	uint32_t check_crc = 0;
	uint8_t crc_buffer[CRC_BUFFER_SIZE];

	if (lseek(fd, CHECKSUM_SIZE, 0) < 0) {
		bmdbg("Failed to lseek : errno %d\n", errno);
		return ERROR;
	}

	while (file_size > 0) {
		if (file_size > CRC_BUFFER_SIZE) {
			read_size = read(fd, (FAR uint8_t *)crc_buffer, CRC_BUFFER_SIZE);
		} else {
			read_size = read(fd, (FAR uint8_t *)crc_buffer, file_size);
		}

		if (read_size == ERROR) {
			bmdbg("Failed to read : %d, errno %d\n", read_size, errno);
			return ERROR;
		}

		check_crc = crc32part(crc_buffer, read_size, check_crc);
		file_size -= read_size;
	}

	if (check_crc != crc_hash) {
		bmdbg("Failed to crc check : %u != %u\n", check_crc, crc_hash);
		return ERROR;
	}

	return OK;
}

static int binary_manager_verify_header(binary_header_t *header_data)
{
	if (header_data == NULL) {
		bmdbg("Header data is NULL\n");
		return ERROR;
	}

	if (header_data->header_size < 0 || header_data->bin_size < 0 \
	|| header_data->ram_size < 0 || header_data->bin_type != BIN_TYPE_ELF) {
		bmdbg("Invalid header data : headersize %d, binsize %d, ramsize %d, bintype %d\n", header_data->header_size, header_data->bin_size, header_data->ram_size, header_data->bin_type);
		return ERROR;
	}

	// calculate and check checksum value

	bmvdbg("Binary header : %d %d %d %s %s %s %d %d\n", header_data->header_size, header_data->bin_type, header_data->bin_size, header_data->bin_name, header_data->bin_ver, header_data->kernel_ver, header_data->ram_size, header_data->jump_addr);

	return OK;
}

/* Read header and Check whether header data is valid or not */
static int binary_manager_read_header(int bin_idx, int part_idx, binary_header_t *header_data)
{
	int fd;
	int ret;
	int file_size;
	char devname[BINMGR_DEVNAME_LEN];
	uint32_t crc_hash = 0;

	memset(header_data, 0, sizeof(binary_header_t));

	snprintf(devname, BINMGR_DEVNAME_LEN, BINMGR_DEVNAME_FMT, BIN_PARTNUM(bin_idx, part_idx));
	fd = open(devname, O_RDONLY);
	if (fd < 0) {
		bmdbg("Failed to open %s: %d, errno %d\n", devname, ret, errno);
		return ERROR;
	}

	/* Read the checksum */
	ret = read(fd, (FAR uint8_t *)&crc_hash, CHECKSUM_SIZE);
	if (ret != CHECKSUM_SIZE) {
		bmdbg("Failed to read %s: %d, errno %d\n", devname, ret, errno);
		close(fd);
		return ERROR;
	}

	/* Read the binary header */
	ret = read(fd, (FAR uint8_t *)header_data, sizeof(binary_header_t));
	if (ret != sizeof(binary_header_t)) {
		bmdbg("Failed to read %s: %d, errno %d\n", devname, ret, errno);
		close(fd);
		return ERROR;
	}

	/* Verify header data */
	if (binary_manager_verify_header(header_data) == ERROR) {
		close(fd);
		return ERROR;
	}

	file_size = header_data->header_size + header_data->bin_size;

	if (binary_manager_verify_checksum(fd, file_size, crc_hash) == ERROR) {
		close(fd);
		return ERROR;
	}

	close(fd);

	return OK;
}

/* Read binary header and update binary table */
static int binary_manager_load_bininfo(int bin_idx)
{
	int ret;
	int version;
	int part_idx;
	int latest_ver;
	int latest_idx;
	binary_header_t header_data[PARTS_PER_BIN];

	latest_ver = -1;
	latest_idx = -1;

	/* Read header data of binary partitions */
	for (part_idx = 0; part_idx < PARTS_PER_BIN; part_idx++) {
		ret = binary_manager_read_header(bin_idx, part_idx, &header_data[part_idx]);
		if (ret == OK) {
			version = (int)atoi(header_data[part_idx].bin_ver);
			bmvdbg("Found valid header in part %d, version %d\n", part_idx, version);
			if (version > latest_ver) {
				latest_ver = version;
				latest_idx = part_idx;
			}
		}
	}

	/* Failed to find valid binary */
	if (latest_ver < 0) {
		bmdbg("Failed to find loadable binary %d\n", bin_idx);
		return ERROR;
	}

	/* Set the data in table from header */
	BIN_USEIDX(bin_idx) = latest_idx;
	BIN_SIZE(bin_idx) = header_data[latest_idx].bin_size;
	BIN_RAMSIZE(bin_idx) = header_data[latest_idx].ram_size;
	BIN_OFFSET(bin_idx) = CHECKSUM_SIZE + header_data[latest_idx].header_size;
	strncpy(BIN_VER(bin_idx), header_data[latest_idx].bin_ver, BIN_VER_MAX);
	strncpy(BIN_KERNEL_VER(bin_idx), header_data[latest_idx].kernel_ver, KERNEL_VER_MAX);
	strncpy(BIN_NAME(bin_idx), header_data[latest_idx].bin_name, BIN_NAME_MAX);

	bmvdbg("BIN TABLE[%d] %d %d %s %s %s\n", bin_idx, BIN_SIZE(bin_idx), BIN_RAMSIZE(bin_idx), BIN_VER(bin_idx), BIN_KERNEL_VER(bin_idx), BIN_NAME(bin_idx));

	return OK;
}

/* Load binary with index in binary table */
int binary_manager_load_binary(int bin_idx)
{
	int ret;
	pid_t pid;
	char devname[BINMGR_DEVNAME_LEN];

	if (bin_idx < 0) {
		bmdbg("Invalid bin idx %d\n", bin_idx);
		return ERROR;
	}

	/* Load binary info */
	ret = binary_manager_load_bininfo(bin_idx);
	if (ret < 0) {
		bmdbg("Failed to get loadable binary %d\n", bin_idx);
		return ERROR;
	}

	/* Load binary */
	snprintf(devname, BINMGR_DEVNAME_LEN, BINMGR_DEVNAME_FMT, BIN_PARTNUM(bin_idx, BIN_USEIDX(bin_idx)));
	bmvdbg("BIN[%d] %s %d %d\n", bin_idx, devname, BIN_SIZE(bin_idx), BIN_OFFSET(bin_idx));
	pid = load_binary(devname, BIN_SIZE(bin_idx), BIN_OFFSET(bin_idx), BIN_RAMSIZE(bin_idx));
	if (pid <= 0) {
		bmdbg("Load '%s' fail, errno %d\n", BIN_NAME(bin_idx), errno);
		return ERROR;
	}

	BIN_ID(bin_idx) = pid;
	bmvdbg("Load '%s' success! pid = %d\n", BIN_NAME(bin_idx), BIN_ID(bin_idx));

	return OK;
}

static int binary_manager_load_all(void)
{
	int ret;
	int bin_idx;
	int load_cnt;
	int bin_count;

	load_cnt = 0;
	bin_count = binary_manager_get_binary_count();

	for (bin_idx = 1; bin_idx <= bin_count; bin_idx++) {
		ret = binary_manager_load_binary(bin_idx);
		if (ret == OK) {
			load_cnt++;
		}
	}

	return load_cnt;
}

static void reload_kill_each(FAR struct tcb_s *tcb, FAR void *arg)
{
	int ret;
	int binid;

	binid = (int)arg;
	if (binid < 0) {
		return;
	}

	if (tcb->group->tg_loadtask == binid && tcb->pid != binid) {
		bmdbg("KILL!! %d\n", tcb->pid);
		ret = task_terminate(tcb->pid, true);
		bmvdbg("Terminate %d ret %d\n", tcb->pid, ret);
	}
}

static int reload_kill_binary(int binid)
{
	int ret;

	if (binid < 0) {
		return ERROR;
	}

	/* Search all task/pthreads created by same loaded task */
	sched_foreach(reload_kill_each, (FAR void *)binid);

	/* Finally, unload binary */
	ret = task_terminate(binid, true);
	if (ret < 0) {
		bmdbg("Failed to unload binary %d, ret %d, errno %d\n", binid, ret, errno);
		return ERROR;
	}
	bmvdbg("Unload binary! pid %d\n", binid);

	return OK;
}

static int binary_manager_reload(char *name)
{
	int ret;
	int bin_id;
	int bin_idx;
	struct tcb_s *tcb;

	if (name == NULL) {
		return ERROR;
	}

	/* Check whether it is registered in binary manager */
	bin_idx = binary_manager_get_index_with_name(name);
	if (bin_idx < 0) {
		bmdbg("binary pid %d is not registered to binary manager\n", bin_id);
		return ERROR;
	}

	tcb = sched_gettcb(BIN_ID(bin_idx));
	if (tcb == NULL || tcb->group == NULL || tcb->group->tg_loadtask < 0) {
		bmdbg("Failed to get pid %d binary info\n", BIN_ID(bin_idx));
		return ERROR;
	}

	bin_id = tcb->group->tg_loadtask;
	bmvdbg("pid %d, binary id %d\n", BIN_ID(bin_idx), bin_id);

	/* Kill its children and restart binary if the binary is registered with the binary manager */
	ret = reload_kill_binary(bin_id);
	if (ret < 0) {
		bmdbg("Failed to kill binaries, binary pid %d, binid %d\n", BIN_ID(bin_idx), bin_id);
		return ERROR;
	}

	/* load binary and update binid */
	return binary_manager_load_binary(bin_idx);
}

static int loading_thread(int argc, char *argv[])
{
	int ret;
	int type;
	char *name;

	if (argc <= 1) {
		bmdbg("Invalid arguements\n");
		return -1;
	}

	type = (pid_t)atoi(argv[1]);

	ret = ERROR;
	switch (type) {
	case LOADCMD_LOAD_ALL:
		ret = binary_manager_load_all();
		break;
	case LOADCMD_RELOAD:
		if (argc > 2) {
			name = (char *)argv[2];
			ret = binary_manager_reload(name);
			break;
		}
	default:
		bmdbg("Invalid loading type %d\n", type);
		return ERROR;
	}
	bmvdbg("Loading result %d\n", ret);

	return 0;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: binary_manager_loading
 *
 * Description:
 *   This function create loading thread to load/unload binary.
 *
 ****************************************************************************/
int binary_manager_loading(int type, void *data)
{
	int ret;
	int argc;
	char **argv;
	char value[1];

	bmvdbg("Loading type %d\n", type);

	if (type < 0 || type >= LOADCMD_LOAD_MAX) {
		bmdbg("Invalid loading type %d\n", type);
		return ERROR;
	}

	argc = 2;
	argv = (char **)kmm_malloc(sizeof(char *) * (argc + 1));

	argv[0] = itoa(type, (char *)&value, 10);
	argv[1] = (char *)data;
	argv[2] = NULL;

	ret = kernel_thread(LOADINGTHD_NAME, LOADINGTHD_PRIORITY, LOADINGTHD_STACKSIZE, loading_thread, (char * const *)argv);
	free(argv);

	return ret;
}
