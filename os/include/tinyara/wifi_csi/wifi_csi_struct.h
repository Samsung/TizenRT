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

typedef enum CSI_GROUP_NUM_T{
	CSI_GROUP_NUM_1 = 0,  /**< per tone */
	CSI_GROUP_NUM_2,         /**< per 2tone */
	CSI_GROUP_NUM_4,        /**< per 4tone */
	CSI_GROUP_NUM_16,     /**< per 16tone */
	CSI_GROUP_NUM_MAX
} csi_group_num_t;

typedef enum CSI_MODE_T{
	CSI_MODE_NORMAL = 0,   ///<   normal mode
	CSI_MODE_NDP,                ///<   ndp mode
	CSI_MODE_RX_RESP,         ///<    rx rsp mode
	CSI_MODE_MAX,                 ///<    max mode
} csi_mode_t;

typedef enum CSI_ACTION_T{
	CSI_ACT_EN,    /**< enable or disable csi func */
	CSI_ACT_CFG,  /**< config csi parameters */
	CSI_ACT_MAX
} csi_action_t;

typedef enum CSI_ACCURACY_T{
	CSI_ACCU_1BYTE = 0, /**< CSI_ACCU_1BYTE: S(8,4) */
	CSI_ACCU_2BYTES,  /**< CSI_ACCU_2BYTE: S(16,12) */
	CSI_ACCU_MAX
} csi_accuracy_t;

typedef enum CSI_CH_OPT_T{
	CSI_CH_LEGACY = 0, /**< legacy part(L-LTF) channel estmation result */
	CSI_CH_NON_LEGACY,  /**< non-legacy(HT-LTF) part */
	CSI_CH_MAX
} csi_ch_opt_t;

typedef enum {
	CSI_ALG_LS = 0,
	CSI_ALG_SMOTHING,
	CSI_ALG_MAX
} rtw_csi_alg_opt;

typedef struct CSI_ACTION_PARAM_T{
	csi_group_num_t group_num;
	csi_mode_t mode;
	csi_action_t act;
	csi_accuracy_t accuracy;
	rtw_csi_alg_opt alg_opt;
	csi_ch_opt_t ch_opt;
	unsigned char enable;
	unsigned char trig_period;  /* unit:ms*/
	unsigned char data_rate;
} csi_action_param_t;

#ifdef __cplusplus /* If this is a C++ compiler, end C linkage */
	}
#endif
#endif  /* __INCLUDE_TINYARA_WIFI_CSI_STRUCT_H */

