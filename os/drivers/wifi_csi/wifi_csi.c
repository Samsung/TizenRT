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
#include <tinyara/wifi_csi/wifi_csi.h>
#include <tinyara/wifi_csi/wifi_csi_common.h>
#include <sys/types.h>
#include <tinyara/wifi/rtk/wifi_structures.h>
#include <debug.h>
#include <errno.h>

/**
 * TODOs 
 * - To implement the callback function in wifi_csi.c.
 * - The callback function will share data to CSI Platform using mq_send.
 * - next Phases Plan after this release
 *		Phase-1. Remove dependency from RTK headers. Separate local headers for config params to only share between driver, CSI Platform
 *		Phase-2: Multiple chip handling. Refactor implementation to upper and lower half as Suggested by Tj
*/

typedef void (*CSIRawDataReadyListener)(char *buf, int buf_len, int flags, void *userdata);

static int csi_open(FAR struct file *filep);
static int csi_close(FAR struct file *filep);
static ssize_t csi_read(FAR struct file *filep, FAR char *buffer, size_t buflen);
static int csi_ioctl(FAR struct file *filep, int cmd, unsigned long arg);

static const struct file_operations g_csiops = {
	csi_open,					/* open */
	csi_close,					/* close */
	csi_read,					/* read */
	0,							/* write */
	0,							/* seek */
	csi_ioctl					/* ioctl */
#ifndef CONFIG_DISABLE_POLL
	, 0							/* poll */
#endif
};

static rtw_csi_action_parm_t g_act_param __attribute__((aligned(64)))= {0,};

int wifi_csi_register(FAR const char *name)
{
	csidbg("Registering csi_driver %s\n", CONFIG_WIFI_CSI_DEVPATH);
	return register_driver(CONFIG_WIFI_CSI_DEVPATH, &g_csiops, 0666, NULL);
}

static int csi_open(FAR struct file *filep)
{
	csivdbg("DRIVER OPEN\n");
	return 0;
}

static int csi_close(FAR struct file *filep)
{
	csivdbg("DRIVER CLOSE\n");
	return 0;
}

static ssize_t csi_read(FAR struct file *filep, FAR char *buffer, size_t buflen)
{
	int len;
	if (wifi_csi_report(buflen, buffer, &len) == -1) {
		csidbg("ERROR: Wifi CSI report api\n");
		return -EIO;
	}
	return len;
}

static int csi_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	CSIRawDataReadyListener user_cb;
	int ret = 0;
	switch (cmd) {
	case CSIIOC_PARAM_SETZERO:
		csivdbg("CSIIOC_PARAM_SETZERO\n");
		memset(&g_act_param, 0, sizeof(rtw_csi_action_parm_t));
		break;

	case CSIIOC_SET_CONFIG:
		csivdbg("CSIIOC_SET_CONFIG\n");
		if (!arg) {
			csidbg("ERROR: Invalid config arg\n");
			ret = -EINVAL;
			break;
		}
		rtw_csi_action_parm_t *g_act_param_t = ((rtw_csi_action_parm_t*)(arg));
		g_act_param.group_num = g_act_param_t->group_num;
		g_act_param.mode = g_act_param_t->mode;
		g_act_param.accuracy = g_act_param_t->accuracy;
		g_act_param.ch_opt = g_act_param_t->ch_opt;
		g_act_param.trig_period = g_act_param_t->trig_period;
		g_act_param.data_rate = g_act_param_t->data_rate;
		g_act_param.act = g_act_param_t->act;
		g_act_param.enable = g_act_param_t->enable;
		
		ret = wifi_csi_config(&g_act_param);
		if (ret == -1) {
			csidbg("ERROR: Wifi csi setting config fail\n");
			ret = -EIO;
			break;
		}
		csivdbg("CSI config has been set\n");
		csivdbg("Logging configs\n");
		csivdbg("group_num %d\n", g_act_param_t->group_num);
		csivdbg("mode %d\n", g_act_param_t->mode);
		csivdbg("accuracy %d\n", g_act_param_t->accuracy);
		csivdbg("ch_opt %d\n", g_act_param_t->ch_opt);
		csivdbg("trig_period %d\n", g_act_param_t->trig_period);
		csivdbg("data_rate %d\n", g_act_param_t->data_rate); 
		csivdbg("act %d\n", g_act_param_t->act);
		csivdbg("enable %d\n", g_act_param_t->enable);
		csivdbg("alg_opt %d\n", g_act_param_t->alg_opt);
		csivdbg("data_bw %d\n", g_act_param_t->data_bw);
		csivdbg("mac_addr %d\n", g_act_param_t->mac_addr);
		csivdbg("multi_type %d\n", g_act_param_t->multi_type);
		break;

	case CSIIOC_REGISTER_CALLBACK:
		csivdbg("CSIIOC_REGISTER_CALLBACK\n");
		if (!arg) {
			csidbg("ERROR: Invalid callback pointer arg\n");
			ret = -EINVAL;
			break;
		}
		user_cb = (CSIRawDataReadyListener)(arg);
		ret = wifi_reg_event_handler(21, user_cb, NULL);
		if (ret == -1) {
			csidbg("ERROR: Registering wifi event handler fail\n");
			ret = -EIO;
			break;
		}
		csivdbg("Event listener callback registered\n");
		break;  

	case CSIIOC_UNREGISTER_CALLBACK:
		csivdbg("CSIIOC_UNREGISTER_CALLBACK\n");
		if (!arg) {
			csidbg("ERROR: Invalid callback pointer arg\n");
			ret = -EINVAL;
			break;
		}
		user_cb = (CSIRawDataReadyListener)(arg);
		ret = wifi_unreg_event_handler(21, user_cb, NULL);
		if (ret == -1) {
			csidbg("ERROR: Unregistring wifi event handler fail\n");
			ret = -EIO;
			break;
		}
		csivdbg("Callback unregistered \n");
		break;
	
	case CSIIOC_GET_MAC_ADDR:
		csivdbg("CSIIOC_GET_MAC_ADDR\n");
		csivdbg("Mac addr: %02x:%02x:%02x:%02x:%02x:%02x\n", g_act_param.mac_addr[0], g_act_param.mac_addr[1], g_act_param.mac_addr[2], g_act_param.mac_addr[3], g_act_param.mac_addr[4], g_act_param.mac_addr[5]);
		if (!arg) {
			csidbg("ERROR: Invalid mac info arg\n");
			ret = -EINVAL;
			break;
		}
		wifi_csi_mac_info *mac_info = (wifi_csi_mac_info*)arg;
		memcpy(mac_info->mac_addr, g_act_param.mac_addr, 6);
		break;
	default:
		csidbg("ERROR: Invalid CSIWIFI command received [%d]\n", cmd);
		ret = -EINVAL;
		break;
	}
	return ret;
}
