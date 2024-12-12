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
 * drivers/input/ist415_misc.h
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

#ifndef __DRIVERS_INPUT_IST415_MISC_H
#define __DRIVERS_INPUT_IST415_MISC_H

#include "ist415.h"

// Recording
#define REC_START_SCAN              	2
#define REC_ENABLE                  	1
#define REC_DISABLE                 	0

// SelfTest
//#define USE_SELFTEST_CS

#define CMCS_FLAG_CM                	(1 << 0)
#define CMCS_FLAG_JITTER            	(1 << 2)

#define CM_MSG_VALID                	0xC3
#define JITTER_MSG_VALID            	0x71

#define SELFTEST_CM_MSG					(0xC3)
#define SELFTEST_JITTER_MSG				(0x71)

#define SELFTEST_PASS					(0x10)
#define SELFTEST_FAIL					(0x80)

// RawData
#define MTL_CDC_REGISTER				0x40000000
#define SLF_CDC_REGISTER				0x40001768

#define MTL_CPC_REGISTER				0x300505A0
#define SLF_CPC_REGISTER				0x30040290

// Calibration
#define CALIB_MSG_MASK                  0xF0000FFF
#define CALIB_MSG_VALID                 0x80000CAB
#define CALIB_TO_GAP(n)                 ((n >> 16) & 0xFFF)
#define CALIB_TO_STATUS(n)              ((n >> 12) & 0xF)


void ist415_calibrate(struct ist415_dev_s *dev, uint8_t retry);
int ist415_read_info(struct ist415_dev_s *dev);
int ist415_get_info(struct ist415_dev_s *dev);
void ist415_autocalibration(struct ist415_dev_s *dev);
int ist415_set_intr_debug(struct ist415_dev_s *dev, int argc, char **argv);
void ist415_run_intr_debug(struct ist415_dev_s *dev);
void ist415_display_cpc(struct ist415_dev_s *dev);
void ist415_display_rawdata(struct ist415_dev_s *dev);
int ist415_selftest(struct ist415_dev_s *dev);

#endif				/* __DRIVERS_INPUT_IST415_MISC_H */
