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
 * drivers/input/ist415_update.h
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

#ifndef __DRIVERS_INPUT_IST415_UPDATE_H
#define __DRIVERS_INPUT_IST415_UPDATE_H

#include "ist415.h"

// Flash size
#define IST415_ROM_BASE_ADDR		0
#define IST415_ROM_MAIN_SIZE		0x1F000
#define IST415_ROM_INFO_BASE_ADDR	0x1F000
#define IST415_ROM_INFO_SIZE		0x1000
#define IST415_ROM_PAGE_SIZE		0x400
#define IST415_ROM_SECTOR_SIZE		0x800

// ISP CMD
#define ISP_TARGET_MAIN				0x33
#define ISP_TARGET_INFO				0x55
#define ISP_TARGET_DTC				0x57
#define ISP_TARGET_RED				0x59
#define ISP_TARGET_LDT0				0x5A
#define ISP_TARGET_LDT1				0x5C
#define ISP_TARGET_TESTREG			0x5F
#define ISP_TARGET_STATUS			0x2F
#define ISP_TARGET_CHIP_VER			0x3F
#define ISP_TARGET_ENTER			0x27
#define ISP_TARGET_EXIT				0x51

#define ISP_CMD_PAGE_ERASE			0x1C
#define ISP_CMD_SECTOR_ERASE		0xCC
#define ISP_CMD_MASS_ERASE			0xFF
#define ISP_CMD_PROG				0x00
#define ISP_CMD_BURST_PROG			0xAA
#define ISP_CMD_READ				0xF0
#define ISP_CMD_RUN_CRC				0x73
#define ISP_CMD_READ_CRC			0x55
#define ISP_CMD_ENTER				0x27
#define ISP_CMD_EXIT				0x51

#define ISP_STATUS_BUSY				0x96
#define ISP_STATUS_DONE				0xAD

// Version flag
#define FLAG_MAIN					1
#define FLAG_RELEASE				2
#define FLAG_TEST					3
#define FLAG_CORE					4
#define FLAG_CONFIG					5

#define MAIN_VER_MASK           	0xFF000000

struct firmware {
	char *data;
	int size;
};

int ist415_check_fw(struct ist415_dev_s *dev);
int ist415_force_update(struct ist415_dev_s *dev, int argc, char **argv);

#endif				/* __DRIVERS_INPUT_IST415_UPDATE_H */
