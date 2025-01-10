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
 * drivers/input/ist415_misc.c
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
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>
#include <unistd.h>

#include <tinyara/config.h>

#include <tinyara/input/ist415.h>
#include "ist415_misc.h"

/****************************************************************************
 * Pre-Processor Definitions
 ****************************************************************************/

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

/****************************************************************************
 * Name: ist415_print_info
 ****************************************************************************/

static void ist415_print_info(struct ist415_dev_s *dev)
{
	ist415vdbg("*** TSP info ***\n");
	ist415vdbg(" swap_xy: %d\n", dev->swap_xy);
	ist415vdbg(" tx: %d, rx: %d\n", dev->tx_num, dev->rx_num);
	ist415vdbg(" touch width: %d, height: %d\n", dev->width, dev->height);
	ist415vdbg(" mutual node len: %d\n", dev->mtl_node_len);
	ist415vdbg(" self node len: %d\n", dev->slf_node_len);
	ist415vdbg("Main Ver: %X, Test Ver: %X\n", dev->fw.cur.main_ver, dev->fw.cur.test_ver);
	ist415vdbg("Core Ver: %X, Config Ver: %X, Release Ver: %X\n", dev->fw.cur.core_ver, dev->fw.cur.config_ver, dev->fw.cur.release_ver);
}

/****************************************************************************
 * Name: ist415_wait_calibrate
 ****************************************************************************/

static int ist415_wait_calibrate(struct ist415_dev_s *dev)
{
	int wait = IST415_WAIT_TIME;

	dev->calib_result = 0;
	memset(dev->calib_msg, 0, sizeof(dev->calib_msg));

	while (wait-- > 0) {
		msleep(100);

		if ((dev->calib_result == 0x10) || (dev->calib_result == 0x80)) {
			ist415_i2c_read(dev, HCOM_GET_CAL_RESULT, (uint8_t *) dev->calib_msg, sizeof(dev->calib_msg));
			ist415vdbg("MTL Calibration: %d, Max gap : %d - (%08x)\n", CALIB_TO_STATUS(dev->calib_msg[0]), CALIB_TO_GAP(dev->calib_msg[0]), dev->calib_msg[0]);
			ist415vdbg("SLF Calibration: %d, Max gap : %d - (%08x)\n", CALIB_TO_STATUS(dev->calib_msg[1]), CALIB_TO_GAP(dev->calib_msg[1]), dev->calib_msg[1]);
		}

		if (dev->calib_result == 0x10) {
			ist415vdbg("Calibration success\n");
			return OK;
		} else if (dev->calib_result == 0x80) {
			ist415vdbg("Calibration fail\n");
			return -EAGAIN;
		}
	}

	ist415dbg("Calibration time out\n");

	return -ETIME;
}

/****************************************************************************
 * Name: ist415_run_selftest
 ****************************************************************************/

static int ist415_run_selftest(struct ist415_dev_s *dev, uint8_t flag, int16_t *buf16)
{
	int wait = IST415_WAIT_TIME;
	int size = dev->mtl_node_len * sizeof(int16_t);
	int ret = 0;

	ist415_enable(dev);
	dev->event_mode = false;

	dev->cmcs = 0;
	dev->cmcs_msg = 0;

	ret = ist415_i2c_write(dev, HCOM_RUN_SELFTEST, (uint8_t *)&flag, 1);
	if (ret) {
		ist415dbg("Fail to write selftest command\n");
		return ret;
	}

	while (wait-- > 0) {
		msleep(100);

		if ((dev->cmcs == SELFTEST_PASS) || (dev->cmcs == SELFTEST_FAIL)) {
			if (flag == CMCS_FLAG_CM) {
				if (dev->cmcs_msg == CM_MSG_VALID) {
					if (dev->cmcs == SELFTEST_PASS) {
						dev->cm_result = 0;
					} else {
						dev->cm_result = 1;
					}
					break;
				}
			} else if (flag == CMCS_FLAG_JITTER) {
				if (dev->cmcs_msg == JITTER_MSG_VALID) {
					if (dev->cmcs == SELFTEST_PASS) {
						dev->jitter_result = 0;
					} else {
						dev->jitter_result = 1;
					}
					break;
				}
			} else {
				ist415wdbg("Not support selftest flag (0x%02X)\n", flag);
			}
		}
	}

	if (wait == 0) {
		ret = -EPERM;
		ist415dbg("Self Test time out\n");
	} else {
		ret = ist415_i2c_read(dev, HCOM_GET_SELFTEST_DATA, (uint8_t *)buf16, size);
		if (ret) {
			ist415vdbg("Fail to read selftest result data\n");
		} else {
			ret = 0;
			ist415vdbg("Test end\n");
		}
	}

	ist415_disable(dev);

	return ret;
}

/****************************************************************************
 * Name: ist415_selftest_result
 ****************************************************************************/

static void ist415_selftest_result(struct ist415_dev_s *dev, int16_t *buf16)
{
	int16_t value;
	int tx, rx;

	for (tx = 0; tx < dev->tx_num; tx++) {
		for (rx = 0; rx < dev->rx_num; rx++) {
			value = buf16[(tx * dev->rx_num) + rx];
			ist415vdbg(" %4d", value);
		}
		ist415vdbg("\n");
	}
	ist415vdbg("\n");
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: ist415_calibrate
 ****************************************************************************/

void ist415_calibrate(struct ist415_dev_s *dev, uint8_t retry)
{
	int ret = 0;

	ist415dbg("*** Calibrate %ds ***\n", IST415_WAIT_TIME / 10);

	while (retry-- > 0) {
		ist415_disable(dev);
		ist415_reset(dev, false);
		ist415_enable(dev);
		dev->event_mode = false;

		ret = ist415_i2c_write(dev, HCOM_CALIBRATION, NULL, 0);
		if (ret) {
			ist415dbg("Fail to write calibration commnad\n");
			continue;
		}
		ret = ist415_wait_calibrate(dev);
		if (ret == OK) {
			break;
		}
	}

	ist415_reset(dev, false);
}

/****************************************************************************
 * Name: ist415_read_info
 ****************************************************************************/

int ist415_read_info(struct ist415_dev_s *dev)
{
	int ret = 0;
	uint8_t panel_info[11];
	uint8_t version[6];
	uint8_t tsp_swap;
	uint8_t recording_info[6];

	ret = ist415_i2c_read(dev, HCOM_GET_PANEL_INFO, panel_info, sizeof(panel_info));
	if (ret) {
		ist415dbg("Fail to get panel info\n");
		return ret;
	}

	dev->width = ((uint16_t) panel_info[0] << 8) | (uint16_t) panel_info[1];
	dev->height = ((uint16_t) panel_info[2] << 8) | (uint16_t) panel_info[3];
	dev->tx_num = panel_info[8];
	dev->rx_num = panel_info[9];
	dev->slf_node_len = dev->tx_num + dev->rx_num;
	dev->mtl_node_len = dev->tx_num * dev->rx_num;

	ret = ist415_i2c_read(dev, HCOM_GET_FW_VER, version, sizeof(version));
	if (ret) {
		ist415dbg("Fail to get version\n");
		return ret;
	}

	dev->fw.cur.core_ver = ((uint16_t) version[0] << 8) | (uint16_t) version[1];
	dev->fw.cur.config_ver = ((uint16_t) version[2] << 8) | (uint16_t) version[3];
	dev->fw.cur.release_ver = ((uint16_t) version[4] << 8) | (uint16_t) version[5];

	ret = ist415_i2c_read(dev, HCOM_GET_SWAP_INFO, &tsp_swap, sizeof(tsp_swap));
	if (ret) {
		ist415dbg("Fail to get swap info\n");
		return ret;
	}

	dev->swap_xy = (tsp_swap & TSP_INFO_SWAP_XY) ? true : false;

	ret = ist415_i2c_read(dev, HCOM_GET_VER_MAIN, (uint8_t *)&dev->fw.cur.main_ver, sizeof(dev->fw.cur.main_ver));
	if (ret) {
		ist415dbg("Fail to get main version\n");
		return ret;
	}

	ret = ist415_i2c_read(dev, HCOM_GET_VER_TEST, (uint8_t *)&dev->fw.cur.test_ver, sizeof(dev->fw.cur.test_ver));
	if (ret) {
		ist415dbg("Fail to get test version\n");
		return ret;
	}

	ret = ist415_i2c_read(dev, HCOM_GET_ALGO_STATUS, recording_info, sizeof(recording_info));
	if (ret) {
		ist415dbg("Fail to get recording info\n");
		return ret;
	}

	dev->rec_addr = ((uint32_t) recording_info[3] << 24) | ((uint32_t) recording_info[2] << 16) | ((uint32_t) recording_info[1] << 8) | (uint32_t) recording_info[0];
	dev->rec_size = ((uint16_t) recording_info[5] << 8) | (uint16_t) recording_info[4];

	return ret;
}

/****************************************************************************
 * Name: ist415_get_info
 ****************************************************************************/

int ist415_get_info(struct ist415_dev_s *dev)
{
	int ret = 0;

	ret = ist415_read_info(dev);
	if (ret) {
		return ret;
	}

	ist415_print_info(dev);

	return 0;
}

/****************************************************************************
 * Name: ist415_autocalibration
 *
 * Description
 *   This function is called by Uart Command.
 ****************************************************************************/

void ist415_autocalibration(struct ist415_dev_s *dev)
{
	sem_wait(&dev->sem);

	ist415_calibrate(dev, 1);
	ist415_start(dev);
	ist415_enable(dev);

	sem_post(&dev->sem);
}

/****************************************************************************
 * Name: ist415_set_intr_debug
 *
 * Description
 *   This function is called by Uart Command.
 ****************************************************************************/

int ist415_set_intr_debug(struct ist415_dev_s *dev, int argc, char **argv)
{
	if (argc < 4) {
		return -EINVAL;
	}

	dev->intr_debug_addr = strtoul(argv[2], NULL, 16);
	dev->intr_debug_size = atoi(argv[3]);

	ist415vdbg("intr debug addr: 0x%x, size: %d\n", dev->intr_debug_addr, dev->intr_debug_size);

	return OK;
}

/****************************************************************************
 * Name: ist415_run_intr_debug
 ****************************************************************************/

void ist415_run_intr_debug(struct ist415_dev_s *dev)
{
	uint32_t *buf32;
	int ret = 0;

	if (dev->intr_debug_size <= 0) {
		return;
	}

	buf32 = (uint32_t *)kmm_malloc(dev->intr_debug_size);
	if (!buf32) {
		return;
	}

	ret = ist415_i2c_read_da(dev, dev->intr_debug_addr, (uint32_t *) buf32, dev->intr_debug_size);
	if (ret) {
		kmm_free(buf32);
		ist415dbg("Fail to read intr debug\n");
		return;
	}

	ist415vdbg("Intr Debug (Addr : 0x%08X)\n", dev->intr_debug_addr);
	for (int i = 0; i < dev->intr_debug_size; i++) {
		ist415vdbg(" %08X\n", buf32[i]);
	}

	kmm_free(buf32);
}

/****************************************************************************
 * Name: ist415_display_rawdata
 *
 * Description
 *   This function is called by Uart Command.
 ****************************************************************************/

void ist415_display_rawdata(struct ist415_dev_s *dev)
{
	uint32_t *bufslf;
	uint32_t *bufmtl;
	int tx, rx;
	uint32_t cdc, baseline, diff;
	int ret = 0;

	bufslf = (uint32_t *)kmm_malloc((int)dev->slf_node_len * sizeof(uint32_t));
	if (!bufslf) {
		return;
	}
	bufmtl = (uint32_t *)kmm_malloc((int)dev->mtl_node_len * sizeof(uint32_t));
	if (!bufmtl) {
		kmm_free(bufslf);
		return;
	}

	ist415_sensor(dev, false);

	ist415vdbg("SLF addr: %08X, size : %d\n", SLF_CDC_REGISTER, (int)dev->slf_node_len);
	ret = ist415_i2c_read_da(dev, SLF_CDC_REGISTER, (uint32_t *) bufslf, (int)dev->slf_node_len);
	if (ret) {
		kmm_free(bufslf);
		kmm_free(bufmtl);
		ist415dbg("Fail to read self raw data\n");
		return;
	}

	ist415vdbg("MTL addr: %08X, size : %d\n", MTL_CDC_REGISTER, (int)dev->mtl_node_len);
	ret = ist415_i2c_read_da(dev, MTL_CDC_REGISTER, (uint32_t *) bufmtl, (int)dev->mtl_node_len);
	if (ret) {
		kmm_free(bufslf);
		kmm_free(bufmtl);
		ist415dbg("Fail to read mutual raw data\n");
		return;
	}

	ist415_sensor(dev, true);

	ist415vdbg("[CDC]\n");
	if (dev->swap_xy) {
		for (rx = 0; rx < dev->rx_num; rx++) {
			if (rx == 0) {
				ist415vdbg(" %4s", " ");
				ist415vdbg(" %4s", " ");
				for (tx = 0; tx < dev->tx_num; tx++) {
					cdc = bufslf[tx] & 0xFFF;
					ist415vdbg(" %4d", cdc);
				}
				ist415vdbg("\n\n");
			}

			for (tx = 0; tx < dev->tx_num; tx++) {
				if (tx == 0) {
					cdc = bufslf[dev->tx_num + rx] & 0xFFF;
					ist415vdbg(" %4d", cdc);
					ist415vdbg(" %4s", " ");
				}

				cdc = bufmtl[(tx * dev->rx_num) + rx] & 0xFFF;
				ist415vdbg(" %4d", cdc);
			}
			ist415vdbg("\n");
		}
		ist415vdbg("\n");
	} else {
		for (tx = 0; tx < dev->tx_num; tx++) {
			if (tx == 0) {
				ist415vdbg(" %4s", " ");
				ist415vdbg(" %4s", " ");
				for (rx = 0; rx < dev->rx_num; rx++) {
					cdc = bufslf[dev->tx_num + rx] & 0xFFF;
					ist415vdbg(" %4d", cdc);
				}
				ist415vdbg("\n\n");
			}

			for (rx = 0; rx < dev->rx_num; rx++) {
				if (rx == 0) {
					cdc = bufslf[tx] & 0xFFF;
					ist415vdbg(" %4d", cdc);
					ist415vdbg(" %4s", " ");
				}

				cdc = bufmtl[(tx * dev->rx_num) + rx] & 0xFFF;
				ist415vdbg(" %4d", cdc);
			}
			ist415vdbg("\n");
		}
		ist415vdbg("\n");
	}

	ist415vdbg("[BASELINE]\n");
	if (dev->swap_xy) {
		for (rx = 0; rx < dev->rx_num; rx++) {
			if (rx == 0) {
				ist415vdbg(" %4s", " ");
				ist415vdbg(" %4s", " ");
				for (tx = 0; tx < dev->tx_num; tx++) {
					baseline = (bufslf[tx] >> 16) & 0xFFF;
					ist415vdbg(" %4d", baseline);
				}
				ist415vdbg("\n\n");
			}

			for (tx = 0; tx < dev->tx_num; tx++) {
				if (tx == 0) {
					baseline = (bufslf[dev->tx_num + rx] >> 16) & 0xFFF;
					ist415vdbg(" %4d", baseline);
					ist415vdbg(" %4s", " ");
				}

				baseline = (bufmtl[(tx * dev->rx_num) + rx] >> 16) & 0xFFF;
				ist415vdbg(" %4d", baseline);
			}
			ist415vdbg("\n");
		}
		ist415vdbg("\n");
	} else {
		for (tx = 0; tx < dev->tx_num; tx++) {
			if (tx == 0) {
				ist415vdbg(" %4s", " ");
				ist415vdbg(" %4s", " ");
				for (rx = 0; rx < dev->rx_num; rx++) {
					baseline = (bufslf[dev->tx_num + rx] >> 16) & 0xFFF;
					ist415vdbg(" %4d", baseline);
				}
				ist415vdbg("\n\n");
			}

			for (rx = 0; rx < dev->rx_num; rx++) {
				if (rx == 0) {
					baseline = (bufslf[tx] >> 16) & 0xFFF;
					ist415vdbg(" %4d", baseline);
					ist415vdbg(" %4s", " ");
				}

				baseline = (bufmtl[(tx * dev->rx_num) + rx] >> 16) & 0xFFF;
				ist415vdbg(" %4d", baseline);
			}
			ist415vdbg("\n");
		}
		ist415vdbg("\n");
	}

	ist415vdbg("[DIFF]\n");
	if (dev->swap_xy) {
		for (rx = 0; rx < dev->rx_num; rx++) {
			if (rx == 0) {
				ist415vdbg(" %4s", " ");
				ist415vdbg(" %4s", " ");
				for (tx = 0; tx < dev->tx_num; tx++) {
					cdc = bufslf[tx] & 0xFFF;
					baseline = (bufslf[tx] >> 16) & 0xFFF;
					diff = cdc - baseline;
					ist415vdbg(" %4d", diff);
				}
				ist415vdbg("\n\n");
			}

			for (tx = 0; tx < dev->tx_num; tx++) {
				if (tx == 0) {
					cdc = bufslf[dev->tx_num + rx] & 0xFFF;
					baseline = (bufslf[dev->tx_num + rx] >> 16) & 0xFFF;
					diff = cdc - baseline;
					ist415vdbg(" %4d", diff);
					ist415vdbg(" %4s", " ");
				}

				cdc = bufmtl[(tx * dev->rx_num) + rx] & 0xFFF;
				baseline = (bufmtl[(tx * dev->rx_num) + rx] >> 16) & 0xFFF;
				diff = cdc - baseline;
				ist415vdbg(" %4d", diff);
			}
			ist415vdbg("\n");
		}
		ist415vdbg("\n");
	} else {
		for (tx = 0; tx < dev->tx_num; tx++) {
			if (tx == 0) {
				ist415vdbg(" %4s", " ");
				ist415vdbg(" %4s", " ");
				for (rx = 0; rx < dev->rx_num; rx++) {
					cdc = bufslf[dev->tx_num + rx] & 0xFFF;
					baseline = (bufslf[dev->tx_num + rx] >> 16) & 0xFFF;
					diff = cdc - baseline;
					ist415vdbg(" %4d", diff);
				}
				ist415vdbg("\n\n");
			}

			for (rx = 0; rx < dev->rx_num; rx++) {
				if (rx == 0) {
					cdc = bufslf[tx] & 0xFFF;
					baseline = (bufslf[tx] >> 16) & 0xFFF;
					diff = cdc - baseline;
					ist415vdbg(" %4d", diff);
					ist415vdbg(" %4s", " ");
				}

				cdc = bufmtl[(tx * dev->rx_num) + rx] & 0xFFF;
				baseline = (bufmtl[(tx * dev->rx_num) + rx] >> 16) & 0xFFF;
				diff = cdc - baseline;
				ist415vdbg(" %4d", diff);
			}
			ist415vdbg("\n");
		}
		ist415vdbg("\n");
	}

	kmm_free(bufslf);
	kmm_free(bufmtl);
}

/****************************************************************************
 * Name: ist415_display_cpc
 *
 * Description
 *   This function is called by Uart Command.
 ****************************************************************************/

void ist415_display_cpc(struct ist415_dev_s *dev)
{
	uint32_t *bufslf;
	uint32_t *bufmtl;
	int tx, rx;
	uint32_t cpc;
	int ret = 0;

	bufslf = (uint32_t *)kmm_malloc(dev->slf_node_len * sizeof(uint32_t));
	if (!bufslf) {
		return;
	}
	bufmtl = (uint32_t *)kmm_malloc(dev->mtl_node_len * sizeof(uint32_t));
	if (!bufmtl) {
		kmm_free(bufslf);
		return;
	}

	ist415_sensor(dev, false);

	ist415vdbg("SLF addr: %08X, size : %d\n", SLF_CPC_REGISTER, dev->slf_node_len);
	ret = ist415_i2c_read_da(dev, SLF_CPC_REGISTER, (uint32_t *) bufslf, dev->slf_node_len);
	if (ret) {
		kmm_free(bufslf);
		kmm_free(bufmtl);
		ist415dbg("Fail to read self cpc data\n");
		return;
	}

	ist415vdbg("MTL addr: %08X, size : %d\n", MTL_CPC_REGISTER, dev->mtl_node_len);
	ret = ist415_i2c_read_da(dev, MTL_CPC_REGISTER, (uint32_t *) bufmtl, dev->mtl_node_len);
	if (ret) {
		kmm_free(bufslf);
		kmm_free(bufmtl);
		ist415dbg("Fail to read mutual cpc data\n");
		return;
	}

	ist415_sensor(dev, true);

	ist415vdbg("[CPC]\n");
	if (dev->swap_xy) {
		for (rx = 0; rx < dev->rx_num; rx++) {
			if (rx == 0) {
				ist415vdbg(" %4s", " ");
				ist415vdbg(" %4s", " ");
				for (tx = 0; tx < dev->tx_num; tx++) {
					cpc = bufslf[tx] & 0xFFF;
					ist415vdbg(" %4d", cpc);
				}
				ist415vdbg("\n\n");
			}

			for (tx = 0; tx < dev->tx_num; tx++) {
				if (tx == 0) {
					cpc = bufslf[dev->tx_num + rx] & 0xFFF;
					ist415vdbg(" %4d", cpc);
					ist415vdbg(" %4s", " ");
				}

				cpc = bufmtl[(tx * dev->rx_num) + rx] & 0xFFF;
				ist415vdbg(" %4d", cpc);
			}
			ist415vdbg("\n");
		}
		ist415vdbg("\n");
	} else {
		for (tx = 0; tx < dev->tx_num; tx++) {
			if (tx == 0) {
				ist415vdbg(" %4s", " ");
				ist415vdbg(" %4s", " ");
				for (rx = 0; rx < dev->rx_num; rx++) {
					cpc = bufslf[dev->tx_num + rx] & 0xFFF;
					ist415vdbg(" %4d", cpc);
				}
				ist415vdbg("\n\n");
			}

			for (rx = 0; rx < dev->rx_num; rx++) {
				if (rx == 0) {
					cpc = bufslf[tx] & 0xFFF;
					ist415vdbg(" %4d", cpc);
					ist415vdbg(" %4s", " ");
				}

				cpc = bufmtl[(tx * dev->rx_num) + rx] & 0xFFF;
				ist415vdbg(" %4d", cpc);
			}
			ist415vdbg("\n");
		}
		ist415vdbg("\n");
	}

	kmm_free(bufslf);
	kmm_free(bufmtl);
}

/****************************************************************************
 * Name: ist415_selftest
 *
 * Description
 *   This function is called by Uart Command.
 ****************************************************************************/

int ist415_selftest(struct ist415_dev_s *dev)
{
	int16_t *buf16;
	int ret = 0;

	ist415vdbg("*** SelfTest Start ***\n");

	ist415_disable(dev);
	ist415_sensor(dev, false);

	buf16 = (int16_t *)kmm_malloc(dev->mtl_node_len * sizeof(int16_t));
	if (!buf16) {
		kmm_free(buf16);
		ist415_reset(dev, false);
		ist415_start(dev);
		ist415_enable(dev);
		ist415dbg("*** SelfTest Stop ***\n");
		return -ENOMEM;;
	}

	ist415vdbg("* CM test *\n");
	ret = ist415_run_selftest(dev, CMCS_FLAG_CM, (int16_t *)buf16);
	if (ret) {
		kmm_free(buf16);
		ist415_reset(dev, false);
		ist415_start(dev);
		ist415_enable(dev);
		ist415dbg("*** SelfTest Stop ***\n");
		return ret;
	}
	ist415_selftest_result(dev, (int16_t *)buf16);

	ist415vdbg("* Jitter test *\n");
	ret = ist415_run_selftest(dev, CMCS_FLAG_JITTER, (int16_t *)buf16);
	if (ret) {
		kmm_free(buf16);
		ist415_reset(dev, false);
		ist415_start(dev);
		ist415_enable(dev);
		ist415dbg("*** SelfTest Stop ***\n");
		return ret;
	}
	ist415_selftest_result(dev, (int16_t *)buf16);

	ist415vdbg("SelfTest Result : %s;%s\n", (dev->cm_result == 0) ? "PASS" : "FAIL", (dev->jitter_result == 0) ? "PASS" : "FAIL");

	ist415vdbg("*** SelfTest Complete ***\n");

	kmm_free(buf16);
	ist415_reset(dev, false);
	ist415_start(dev);
	ist415_enable(dev);

	return ret;
}

/****************************************************************************
 * Name: ist415_rec_mode
 *
 * Description
 *   This function is called by Uart Command.
 ****************************************************************************/

int ist415_rec_mode(struct ist415_dev_s *dev, int argc, char **argv)
{
	int mode;
	uint8_t data;
	char header[32];
	int ret = 0;

	if (argc < 4) {
		return -EINVAL;
	}

	mode = atoi(argv[2]);
	ist415vdbg("rec mode  : %s\n", mode ? "start" : "stop");

	ret = ist415_i2c_write(dev, HCOM_SET_REC_MODE, (uint8_t *)&mode, 1);
	if (ret) {
		return -ENODEV;
	}

	dev->rec_mode = mode;
	if (dev->rec_mode) {
		dev->rec_delay = atoi(argv[3]);
		ist415vdbg("rec delay : %d\n", dev->rec_delay);

		snprintf(header, sizeof(header), "%s2 2 %d, %d, %d, %d\n", RECS_TAG, dev->slf_node_len, dev->tx_num, dev->rx_num, dev->rec_size);
		ist415vdbg("%s", header);

		msleep(100);

		data = REC_START_SCAN;
		ret = ist415_i2c_write(dev, HCOM_SET_REC_MODE, (uint8_t *)&data, 1);
		if (ret) {
			return -ENODEV;
		}
	}

	return OK;
}

void ist415_recording(struct ist415_dev_s *dev)
{
	uint32_t *buf32 = NULL;
	uint32_t fb = 0;
	uint32_t scancnt = 0;
	uint8_t data = 0;
	int size = 0;
	char logbuf[512] = { 0, };
	int offset = 0;
	int i;
	int ret = 0;

	if (dev->rec_idle && (dev->rec_mode > REC_ENABLE)) {
		goto rec_end;
	}

	msleep(dev->rec_delay);

	ret = ist415_i2c_read_da(dev, IST415_TOUCH_STATUS, (uint32_t *)&scancnt, 1);
	if (ret) {
		ist415dbg("Fail to read scan count\n");
		goto rec_end;
	}

	if (dev->rec_scancnt == scancnt) {
		ist415dbg("Same rec scan count\n");
		goto rec_end;
	}

	dev->rec_scancnt = scancnt;

	buf32 = (uint32_t *)kmm_malloc(dev->rec_size + ((dev->slf_node_len + dev->mtl_node_len) * sizeof(uint32_t)));
	if (!buf32) {
		ist415dbg("Fail to allocate buf32\n");
		goto rec_end;
	}

	if (dev->rec_size > 0) {
		ret = ist415_i2c_read_da(dev, dev->rec_addr, (uint32_t *)buf32,	dev->rec_size / sizeof(uint32_t));
		if (ret) {
			ist415dbg("Fail to reaqd rec data\n");
			goto rec_end;
		}
	}
	
	ret = ist415_i2c_read_da(dev, IST415_FB_SEL, (uint32_t *)&fb, 1);
	if (ret) {
		ist415dbg("Fail to read fb\n");
		goto rec_end;
	}

	size = dev->rec_size / sizeof(uint32_t);
	ret = ist415_i2c_read_da(dev, IST415_FB_SLF_ADDR(fb), (uint32_t *)(buf32 + size),
					dev->slf_node_len);
	if (ret) {
		ist415dbg("Fail to read self data\n");
		goto rec_end;
	}

	size += dev->slf_node_len;
	ret = ist415_i2c_read_da(dev, IST415_FB_MTL_ADDR(fb), (uint32_t *)(buf32 + size), dev->mtl_node_len);
	if (ret) {
		ist415dbg("Fail to read mutual data\n");
		goto rec_end;
	}

	size += dev->mtl_node_len;

	offset = snprintf(logbuf, sizeof(logbuf), "%s", RECS_TAG);
	for (i = 1; i <= size; i++) {
		offset += snprintf(logbuf + offset, sizeof(logbuf) - offset, "%08X ", buf32[i]);		
		if ((i % REC_PRINT_SIZE) == 0) {
			ist415vdbg("%s", logbuf);
			offset = snprintf(logbuf, sizeof(logbuf), "%s", RECC_TAG);
		}
	}
	snprintf(logbuf + offset, sizeof(logbuf) - offset, "\n");
	ist415vdbg("%s", logbuf);

rec_end:
	if (buf32) {
		kmm_free(buf32);
	}

	data = REC_START_SCAN;
	ret = ist415_i2c_write(dev, HCOM_SET_REC_MODE, (uint8_t *)&data, 1);
	if (ret) {
		ist415dbg("Fail to write rec start scan\n");
	}
}