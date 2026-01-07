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
#ifndef __INCLUDE_TINYARA_WIFI_CSI_STRUCT_H
#define __INCLUDE_TINYARA_WIFI_CSI_STRUCT_H

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
	extern "C" {
#endif

typedef struct {
	char mac_addr[6];
} csifw_mac_info;

typedef enum CSI_CONFIG_TYPE_T {
	MIN_CSI_CONFIG_TYPE = -1,       /* Invalid: config type should be greater than min */
	HT_CSI_DATA,                    /* 56 subcarriers each sub-carrier data is 2 bytes */
	NON_HT_CSI_DATA,                /* 52 subcarriers each sub-carrier data is 2 bytes */
	HT_CSI_DATA_ACC1,               /* 56 subcarriers each sub-carrier data is 4 bytes */
	NON_HT_CSI_DATA_ACC1,           /* 52 subcarriers each sub-carrier data is 4 bytes */
	MAX_CSI_CONFIG_TYPE             /* Invalid: config type should be less than max */
} csi_config_type_t;

typedef enum CSI_CONFIG_ACTION_T {
	CSI_CONFIG_ENABLE = 0,
	CSI_CONFIG_DISABLE = 1
} csi_config_action_t;

typedef struct {
	csi_config_action_t config_action;
	csi_config_type_t config_type;
	unsigned int interval;
} csi_config_args_t;

#ifdef __cplusplus /* If this is a C++ compiler, end C linkage */
	}
#endif
#endif  /* __INCLUDE_TINYARA_WIFI_CSI_STRUCT_H */

