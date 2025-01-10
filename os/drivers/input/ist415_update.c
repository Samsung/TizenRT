/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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
 * drivers/input/ist415_update.c
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>
#include <fcntl.h>
#include <unistd.h>

#include <tinyara/config.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>

#include <tinyara/input/ist415.h>
#include "ist415_misc.h"
#include "ist415_update.h"

/****************************************************************************
 * Pre-Processor Definitions
 ****************************************************************************/
#define FIRMWARE_PATH_WASHING_MACHINE	"/res/241128_AV_Display_V02060103.bin"

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int ist415_i2c_wait_isp(struct ist415_dev_s *dev)
{
	struct i2c_dev_s *i2c = dev->i2c;
	struct i2c_config_s *config = dev->i2c_config;
	uint8_t cmd = HCOM_ISP;
	uint8_t sub1 = ISP_TARGET_STATUS;
	uint8_t sub2 = ISP_CMD_READ;
	uint32_t addr = 0;
	int wsize = sizeof(cmd) + sizeof(sub1) + sizeof(sub2) + sizeof(addr);
	uint8_t *buf;
	uint32_t buf32 = ISP_STATUS_BUSY;
	uint32_t status;
	int ret = 0;
	int retry = 200;

	buf = (uint8_t *)kmm_malloc(wsize);
	if (!buf) {
		return -ENOMEM;
	}

	buf[0] = cmd;
	buf[1] = sub1;
	buf[2] = sub2;
	put_unaligned_be32(addr, buf + sizeof(cmd) + sizeof(sub1) + sizeof(sub2));

	do {
		retry--;
		if (retry == 0) {
			kmm_free(buf);
			ist415dbg("Fail to wait ISP\n");
			return ERROR;
		}

		msleep(1);
		ret = i2c_write(i2c, config, (uint8_t *)buf, wsize);
		if (ret != wsize) {
			continue;
		}
		ret = i2c_read(i2c, config, (uint8_t *)&buf32, sizeof(status));
		if (ret != sizeof(status)) {
			continue;
		}

		status = cpu_to_be32(buf32);
	} while (status != ISP_STATUS_DONE);

	kmm_free(buf);

	return OK;
}

/****************************************************************************
 * Name: ist415_i2c_write_isp
 ****************************************************************************/

int ist415_i2c_write_isp(struct ist415_dev_s *dev, uint8_t sub1, uint8_t sub2, uint32_t addr, uint32_t *data, int size)
{
	struct i2c_dev_s *i2c = dev->i2c;
	struct i2c_config_s *config = dev->i2c_config;
	uint8_t cmd = HCOM_ISP;
	int hsize = sizeof(cmd) + sizeof(sub1) + sizeof(sub2) + sizeof(addr);
	int wsize = hsize + (size * sizeof(uint32_t));
	uint8_t *buf = NULL;
	int ret = 0;

	buf = (uint8_t *)kmm_malloc(wsize);
	if (!buf) {
		return -ENOMEM;
	}


	buf[0] = cmd;
	buf[1] = sub1;
	buf[2] = sub2;
	put_unaligned_be32(addr, buf + sizeof(cmd) + sizeof(sub1) + sizeof(sub2));
	for (int i = 0; i < size; i++) {
		put_unaligned_be32(data[i], buf + (i * sizeof(uint32_t)) + hsize);
	}

	ret = i2c_write(i2c, config, (uint8_t *) buf, wsize);
	if (ret != wsize) {
		kmm_free(buf);
		ist415dbg("Fail to write ISP\n");
		return ret;
	}

	ret = ist415_i2c_wait_isp(dev);
	if (ret < 0) {
		kmm_free(buf);
		ist415dbg("Fail to write ISP\n");
		return ret;
	}

	kmm_free(buf);

	return OK;
}

/****************************************************************************
 * Name: ist415_parse_ver
 ****************************************************************************/

static uint32_t ist415_parse_ver(struct ist415_dev_s *dev, int flag, const uint8_t *buf)
{
	uint32_t ver = 0;
	uint32_t *buf32 = (uint32_t *) buf;

	if (flag == FLAG_MAIN) {
		ver = (uint32_t) buf32[(dev->tags.flag_addr + 0xFC) >> 2];
	} else if (flag == FLAG_RELEASE) {
		ver = (uint32_t) buf32[(dev->tags.flag_addr + 0xF0) >> 2];
	} else if (flag == FLAG_TEST) {
		ver = (uint32_t) buf32[(dev->tags.flag_addr + 0xF4) >> 2];
	} else if (flag == FLAG_CORE) {
		ver = ((uint32_t) buf32[(dev->tags.flag_addr + 0xEC) >> 2] >> 16) & 0xFFFF;
	} else if (flag == FLAG_CONFIG) {
		ver = (uint32_t) buf32[(dev->tags.flag_addr + 0xEC) >> 2] & 0xFFFF;
	} else {
		ist415dbg("Parsing ver's flag is not corrent\n");
	}

	return ver;
}

/****************************************************************************
 * Name: ist415_parse_tags
 ****************************************************************************/

static int ist415_parse_tags(struct ist415_dev_s *dev, const uint8_t *buf, const uint32_t size)
{
	int ret = -ENOENT;
	struct ist415_tags *tags;

	tags = (struct ist415_tags *)(&buf[size - sizeof(struct ist415_tags)]);

	if (!strncmp(tags->magic1, IST415_TAG_MAGIC, sizeof(tags->magic1)) && !strncmp(tags->magic2, IST415_TAG_MAGIC, sizeof(tags->magic2))) {
		dev->tags = *tags;

		dev->tags.fw_addr -= dev->tags.rom_base;
		dev->tags.cfg_addr -= dev->tags.rom_base;
		dev->tags.sensor_addr -= dev->tags.rom_base;
		dev->tags.flag_addr -= dev->tags.rom_base;

		dev->fw.index = dev->tags.fw_addr;
		dev->fw.size = tags->flag_addr - tags->fw_addr + tags->flag_size;
		dev->fw.chksum = tags->chksum;

		ist415vdbg("Tagts magic1: %s, magic2: %s\n", dev->tags.magic1, dev->tags.magic2);
		ist415vdbg(" rom: %x\n", dev->tags.rom_base);
		ist415vdbg(" ram: %x\n", dev->tags.ram_base);
		ist415vdbg(" fw: %x(%x)\n", dev->tags.fw_addr, dev->tags.fw_size);
		ist415vdbg(" cfg: %x(%x)\n", dev->tags.cfg_addr, dev->tags.cfg_size);
		ist415vdbg(" sensor: %x(%x)\n", dev->tags.sensor_addr, dev->tags.sensor_size);
		ist415vdbg(" flag: %x(%x)\n", dev->tags.flag_addr, dev->tags.flag_size);
		ist415vdbg(" chksum: %x\n", dev->tags.chksum);
		ist415vdbg(" chksum_all: %x\n", dev->tags.chksum_all);

		ret = OK;
	}

	return ret;
}

/****************************************************************************
 * Name: ist415_get_update_info
 ****************************************************************************/

static int ist415_get_update_info(struct ist415_dev_s *dev, const uint8_t *buf, const uint32_t size)
{
	int ret = 0;

	ret = ist415_parse_tags(dev, buf, size);
	if (ret) {
		ist415dbg("Can't find tags of firmware\n");
	}

	return ret;
}

/****************************************************************************
 * Name: request_firmware
 ****************************************************************************/

static int request_firmware(struct firmware *firmware, char *path, struct ist415_dev_s **dev)
{
	int fd;
	int ret = 0;
	struct stat file_stat;

	if (firmware == NULL || path == NULL) {
		return ERROR;
	}

	if (stat(path, &file_stat) != 0) {
		ist415dbg("Failed to stat file: %d\n", get_errno());
		return ERROR;
	}

	firmware->size = file_stat.st_size;
	firmware->data = (char *)kmm_malloc(firmware->size);
	if (!firmware->data) {
		return ERROR;
	}

	fd = open(path, O_RDONLY);
	if (fd < 0) {
		ist415dbg("Failed to open file: %d\n", get_errno());
		kmm_free(firmware->data);
		return ERROR;
	}

	ret = read(fd, firmware->data, firmware->size);
	if (ret != firmware->size) {
		ist415dbg("Failed to read file: %d\n", get_errno());
		kmm_free(firmware->data);
		close(fd);
		return ERROR;
	}
	close(fd);
	return OK;
}

static void release_firmware(struct firmware *firmware)
{
	firmware->size = 0;
	kmm_free(firmware->data);
}

/****************************************************************************
 * Name: ist415_isp_enable
 ****************************************************************************/

static int ist415_isp_enable(struct ist415_dev_s *dev, bool enable)
{
	int ret = 0;

	if (enable) {
		ret = ist415_i2c_write_isp(dev, ISP_TARGET_ENTER, ISP_CMD_ENTER, 0, NULL, 0);
	} else {
		ret = ist415_i2c_write_isp(dev, ISP_TARGET_EXIT, ISP_CMD_EXIT, 0, NULL, 0);
	}

	if (ret < 0) {
		ist415dbg("Fail to isp %s\n", enable ? "enable" : "disable");
		return ret;
	}

	return OK;
}

/****************************************************************************
 * Name: ist415_isp_erase
 ****************************************************************************/

static int ist415_isp_erase(struct ist415_dev_s *dev, uint8_t target, uint8_t area, uint32_t addr, int size)
{
	int count = 0;
	int unit = 0;
	int ret = 0;
	int i;

	ist415vdbg("%s\n", __func__);

	if (area == ISP_CMD_MASS_ERASE) {
		count = 1;
	} else if (area == ISP_CMD_SECTOR_ERASE) {
		count = size / IST415_ROM_SECTOR_SIZE;
		unit = IST415_ROM_SECTOR_SIZE;
	} else if (area == ISP_CMD_PAGE_ERASE) {
		count = size / IST415_ROM_PAGE_SIZE;
		unit = IST415_ROM_PAGE_SIZE;
	}

	for (i = 0; i < count; i++) {
		ret = ist415_i2c_write_isp(dev, target, area, addr + (i * unit), NULL, 0);
		if (ret < 0) {
			ist415dbg("Fail to erase ISP\n");
		}
	}

	return ret;
}

/****************************************************************************
 * Name: ist415_isp_program
 ****************************************************************************/

static int ist415_isp_program(struct ist415_dev_s *dev, uint8_t target, uint32_t addr, uint32_t *buf32, int size)
{
	int ret = 0;
	int i;

	ist415vdbg("%s\n", __func__);

	for (i = 0; i < (size / IST415_ROM_SECTOR_SIZE); i++) {
		ret = ist415_i2c_write_isp(dev, target, ISP_CMD_PROG, addr, buf32, IST415_ROM_SECTOR_SIZE / sizeof(uint32_t));
		if (ret < 0) {
			ist415dbg("Fail to write ISP (Sector %d)\n", i);
			break;
		}
		addr += IST415_ROM_SECTOR_SIZE;
		buf32 += (IST415_ROM_SECTOR_SIZE / sizeof(uint32_t));
	}

	return ret;
}

/****************************************************************************
 * Name: ist415_isp_fw_update
 ****************************************************************************/

static int ist415_isp_fw_update(struct ist415_dev_s *dev, const uint8_t *buf)
{
	int ret = 0;

	ist415vdbg("%s\n", __func__);

	ist415_reset(dev, true);

	ret = ist415_isp_enable(dev, true);
	if (ret < 0) {
		return ret;
	}

	ret = ist415_isp_erase(dev, ISP_TARGET_MAIN, ISP_CMD_SECTOR_ERASE, IST415_ROM_BASE_ADDR, IST415_ROM_MAIN_SIZE);
	if (ret < 0) {
		return ret;
	}

	msleep(1);

	ret = ist415_isp_program(dev, ISP_TARGET_MAIN, IST415_ROM_BASE_ADDR, (uint32_t *) buf, IST415_ROM_MAIN_SIZE);
	if (ret < 0) {
		ist415dbg("Fail to program ISP\n");
		return ret;
	}

	ist415_reset(dev, false);

	return ret;
}

/****************************************************************************
 * Name: ist415_check_auto_update
 ****************************************************************************/

int ist415_check_auto_update(struct ist415_dev_s *dev)
{
	int retry = IST415_RETRY_CNT;
	bool tsp_check = false;
	uint32_t chksum = 0;
	struct ist415_fw *fw = &dev->fw;
	int ret = 0;

	while (retry--) {
		ret = ist415_fw_integrity(dev);
		if (ret == FW_INTEGRITY_VALID) {
			tsp_check = true;
			break;
		} else if (ret == FW_INTEGRITY_INVALID) {
			break;
		}

		ist415_reset(dev, false);
	}

	if (!tsp_check) {
		return -EAGAIN;
	}

	ret = ist415_read_info(dev);
	if (ret < 0) {
		return -EAGAIN;
	}

	if ((fw->cur.release_ver > 0) && (fw->cur.release_ver < 0xFFFF)) {
		if (((fw->cur.main_ver & MAIN_VER_MASK) == MAIN_VER_MASK) || ((fw->cur.main_ver & MAIN_VER_MASK) == 0)) {
			return -EAGAIN;
		}

		ist415dbg("Version compare IC: %x(%x, %x), BIN: %x(%x, %x)\n", fw->cur.release_ver, fw->cur.core_ver, fw->cur.config_ver, fw->bin.release_ver, fw->bin.core_ver, fw->bin.config_ver);

		/* If FW version is same, check FW checksum */
		if ((fw->cur.core_ver == fw->bin.core_ver) && (fw->cur.config_ver == fw->bin.config_ver) && (fw->cur.release_ver == fw->bin.release_ver) && (fw->cur.test_ver == 0)) {
			ret = ist415_i2c_read(dev, HCOM_GET_CRC, (uint8_t *)&chksum, 2);
			if (ret || (chksum != fw->chksum)) {
				ist415dbg("Mismatch checksum IC: %x, Bin: %x (ret: %d)\n", chksum, fw->chksum, ret);
				return -EAGAIN;
			}
		}

		/* If the ver of binary is higher than ver of IC, FW update operate. */
		if ((fw->cur.core_ver >= fw->bin.core_ver) && (fw->cur.config_ver >= fw->bin.config_ver) && (fw->cur.release_ver >= fw->bin.release_ver)) {
			return OK;
		}
	}

	return -EAGAIN;
}

/****************************************************************************
 * Name: ist415_fw_update
 ****************************************************************************/

static int ist415_fw_update(struct ist415_dev_s *dev, const uint8_t *buf, int size)
{
	struct ist415_fw *fw = &dev->fw;
	uint32_t core_ver = ist415_parse_ver(dev, FLAG_CORE, buf);
	uint32_t config_ver = ist415_parse_ver(dev, FLAG_CONFIG, buf);
	uint32_t release_ver = ist415_parse_ver(dev, FLAG_RELEASE, buf);
	uint32_t test_ver = ist415_parse_ver(dev, FLAG_TEST, buf);
	uint32_t chksum = 0;
	int ret = 0;

	ist415dbg("*** Firmware update ***\n");
	ist415vdbg("core: %x, config: %x, release: %x, test: %x(addr: 0x%x ~ 0x%x)\n", core_ver, config_ver, release_ver, test_ver, fw->index, fw->index + fw->size);

	ret = ist415_isp_fw_update(dev, buf);
	if (ret < 0) {
		ist415dbg("Fail to update firmware\n");
		goto reset;
	}

	ret = ist415_i2c_read(dev, HCOM_GET_CRC, (uint8_t *)&chksum, 2);
	if (ret < 0) {
		ist415dbg("Fail to read checksum\n");
		goto reset;
	}

	if (chksum != fw->chksum) {
		ist415dbg("Mismatch checksum IC: %x, Bin: %x\n", chksum, fw->chksum);
		goto reset;
	}

	ret = ist415_read_info(dev);
	if (ret < 0) {
		ist415dbg("Fail to read info\n");
		goto reset;
	}

	ist415dbg("Firmware update success\n");

	return OK;

reset:

	ist415_reset(dev, false);

	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: ist415_check_fw
 ****************************************************************************/

int ist415_check_fw(struct ist415_dev_s *dev)
{
	struct firmware firmware;
	struct ist415_fw *fw = &dev->fw;
	int retry = 0;
	int ret = 0;

	ret = request_firmware(&firmware, FIRMWARE_PATH_WASHING_MACHINE, &dev);
	if (ret < 0) {
		ist415dbg("Don't request firmware\n");
		return -ENOENT;
	}

	fw->buf = (uint8_t *)kmm_malloc(firmware.size);
	if (!fw->buf) {
		release_firmware(&firmware);
		ist415dbg("Failed to allocate buffer\n");
		return -ENOMEM;
	} else {
		memcpy(fw->buf, firmware.data, firmware.size);
		fw->buf_size = firmware.size;
	}

	release_firmware(&firmware);

	ist415vdbg("Firmware loaded successfully.\n");

	ret = ist415_get_update_info(dev, fw->buf, fw->buf_size);
	if (ret < 0) {
		kmm_free(fw->buf);
		return ret;
	}

	fw->bin.main_ver = ist415_parse_ver(dev, FLAG_MAIN, fw->buf);
	fw->bin.release_ver = ist415_parse_ver(dev, FLAG_RELEASE, fw->buf);
	fw->bin.test_ver = ist415_parse_ver(dev, FLAG_TEST, fw->buf);
	fw->bin.core_ver = ist415_parse_ver(dev, FLAG_CORE, fw->buf);
	fw->bin.config_ver = ist415_parse_ver(dev, FLAG_CONFIG, fw->buf);

	ist415vdbg("Main : %08X Core: %x, Config %x, Release: %x, Test: %x\n", fw->bin.main_ver, fw->bin.core_ver, fw->bin.config_ver, fw->bin.release_ver, fw->bin.test_ver);

	while (sem_wait(&dev->sem) != OK) {
		ASSERT(get_errno() == EINTR);
	}
	ret = ist415_check_auto_update(dev);
	sem_post(&dev->sem);
	if (ret >= 0) {
		kmm_free(fw->buf);
		return OK;
	}

	ist415vdbg("Update version. release(core, config, test): %x(%x, %x, %x) -> %x(%x, %x, %x)\n", fw->cur.release_ver, fw->cur.core_ver, fw->cur.config_ver, fw->cur.test_ver, fw->bin.release_ver, fw->bin.core_ver, fw->bin.config_ver, fw->bin.test_ver);

	while (sem_wait(&dev->sem) != OK) {
		ASSERT(get_errno() == EINTR);
	}

	retry = IST415_RETRY_CNT;
	while (retry--) {
		ret = ist415_fw_update(dev, fw->buf, fw->buf_size);
		if (ret == OK) {
			ist415_calibrate(dev, IST415_RETRY_CNT);
			break;
		}
	}

	sem_post(&dev->sem);

	kmm_free(fw->buf);
	return ret;
}

/****************************************************************************
 * Name: ist415_force_update
 *
 * Description
 *   This function is called by Uart Command.
 ****************************************************************************/

int ist415_force_update(struct ist415_dev_s *dev, int argc, char **argv)
{
	struct firmware firmware;
	struct ist415_fw *fw = &dev->fw;

	char *path;
	bool cal;
	int ret;

	if (argc < 4) {
		return -EINVAL;
	}

	path = argv[2];
	cal = atoi(argv[3]);

	ret = request_firmware(&firmware, path, &dev);
	if (ret < 0) {
		ist415dbg("do not request firmware\n");
		return -ENOENT;
	}

	fw->buf = (uint8_t *)kmm_malloc(firmware.size);
	if (!fw->buf) {
		release_firmware(&firmware);
		ist415dbg("Fail to allocate memory for firmware\n");
		return -ENOMEM;
	} else {
		memcpy(fw->buf, firmware.data, firmware.size);
		fw->buf_size = firmware.size;
	}

	release_firmware(&firmware);

	ist415vdbg("Firmware loaded successfully.\n");

	ret = ist415_get_update_info(dev, fw->buf, fw->buf_size);
	if (ret < 0) {
		kmm_free(fw->buf);
		return ret;
	}

	fw->bin.main_ver = ist415_parse_ver(dev, FLAG_MAIN, fw->buf);
	fw->bin.release_ver = ist415_parse_ver(dev, FLAG_RELEASE, fw->buf);
	fw->bin.test_ver = ist415_parse_ver(dev, FLAG_TEST, fw->buf);
	fw->bin.core_ver = ist415_parse_ver(dev, FLAG_CORE, fw->buf);
	fw->bin.config_ver = ist415_parse_ver(dev, FLAG_CONFIG, fw->buf);

	ist415vdbg("Update version. release(core, config, test): %x(%x, %x, %x) -> %x(%x, %x, %x)\n", fw->cur.release_ver, fw->cur.core_ver, fw->cur.config_ver, fw->cur.test_ver, fw->bin.release_ver, fw->bin.core_ver, fw->bin.config_ver, fw->bin.test_ver);

	while (sem_wait(&dev->sem) != OK) {
		ASSERT(get_errno() == EINTR);
	}

	ist415_disable(dev);

	ret = ist415_fw_update(dev, fw->buf, fw->buf_size);
	if (ret == OK) {
		if (cal) {
			ist415_calibrate(dev, 1);
		}
	}

	ist415_start(dev);
	ist415_enable(dev);

	sem_post(&dev->sem);

	kmm_free(fw->buf);

	return ret;
}
