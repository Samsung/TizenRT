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
 * Included Files
 ****************************************************************************/

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <tinyara/kmalloc.h>
#include <tinyara/wifi_csi/wifi_csi.h>
#include "wifi_intf_drv_to_app_basic.h"
#include <semaphore.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define CSI_INTERVAL_TO_TRIG_PERIOD 320 /* Conversion factor to convert interval to trig period */

/****************************************************************************
 * Private Type Definitions
 ****************************************************************************/

/* This structure describes the state of the lower layer driver */

struct rtk_csi_dev_s {
	/* common parts */
	struct wifi_csi_lowerhalf_s dev; /* rtk wifi_csi lower half (this device) */

	sem_t devsem;			/* Protection for both pendq & dev */

	/* rtk_wifi_csi specific data */
	rtw_csi_action_parm_t act_param;
	unsigned char rtk_csi_buf[MAX_CSI_BUFF_LEN];
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int rtk_wifi_csi_ioctl(int cmd, unsigned long arg);
static int rtk_wifi_csi_getmacaddr(char *mac_addr);
static int rtk_wifi_csi_getcsidata(unsigned char *buffer, size_t buflen);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static FAR struct rtk_csi_dev_s *g_rtk_drv;

static const struct wifi_csi_ops_s g_wificsiops = {
	NULL,                           /* read           */
	rtk_wifi_csi_ioctl,             /* ioctl          */
	rtk_wifi_csi_getmacaddr,        /* getmacaddr     */
	rtk_wifi_csi_getcsidata,        /* getcsidata     */
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void data_ready_listener(char *buf, int buf_len, int flags, void *userdata)
{
	if (!g_rtk_drv) {
		csidbg("ERROR: priv null\n");
		return;
	}
	struct wifi_csi_lowerhalf_s *dev = &g_rtk_drv->dev;
	/* flags-> data len with header */
	dev->upper_cb(dev->priv, CSI_CALLBACK_DATA_READY, NULL, flags);
}

static void log_csi_config(void)
{
	if (!g_rtk_drv) {
		csidbg("ERROR: priv null\n");
		return;
	}
	csidbg("CSI config:\n");
	csidbg("group_num: %d\n", g_rtk_drv->act_param.group_num);
	csidbg("mode: %d\n", g_rtk_drv->act_param.mode);
	csidbg("accuracy: %d\n", g_rtk_drv->act_param.accuracy);
	csidbg("ch_opt: %d\n", g_rtk_drv->act_param.ch_opt);
	csidbg("trig_period: %d\n", g_rtk_drv->act_param.trig_period);
	csidbg("data_rate: %d\n", g_rtk_drv->act_param.data_rate);
	csidbg("act: %d\n", g_rtk_drv->act_param.act);
	csidbg("enable: %d\n", g_rtk_drv->act_param.enable);
}

static int rtk_wifi_csi_set_config(unsigned long arg)
{
	csi_config_args_t *config_args = (csi_config_args_t *)arg; 
	csi_config_action_t config_action = config_args->config_action;
	csi_config_type_t config_type = config_args->config_type;
	if (config_type <= MIN_CSI_CONFIG_TYPE || config_type >= MAX_CSI_CONFIG_TYPE) {
		csidbg("ERROR: invalid config type");
		return -EINVAL;
	}
	/* 1. memset config zero */
	memset(&g_rtk_drv->act_param, 0, sizeof(rtw_csi_action_parm_t));
	
	/* 2. assign configs */
	unsigned int interval_ms = config_args->interval;
	g_rtk_drv->act_param.group_num = CSI_GROUP_NUM_1;
	switch (config_type) {
	case HT_CSI_DATA:
		g_rtk_drv->act_param.mode = CSI_MODE_NORMAL;
		g_rtk_drv->act_param.ch_opt = CSI_CH_NON_LEGACY;
		g_rtk_drv->act_param.data_rate = 0x80;
		g_rtk_drv->act_param.accuracy = CSI_ACCU_1BYTE;
		g_rtk_drv->act_param.trig_period = 200;
		break;
	
	case HT_CSI_DATA_ACC1:
		g_rtk_drv->act_param.mode = CSI_MODE_NORMAL;
		g_rtk_drv->act_param.ch_opt = CSI_CH_NON_LEGACY;
		g_rtk_drv->act_param.data_rate = 0x80;
		g_rtk_drv->act_param.accuracy = CSI_ACCU_2BYTES;
		g_rtk_drv->act_param.trig_period = 200;
		break;
	
	case NON_HT_CSI_DATA:
		g_rtk_drv->act_param.mode = CSI_MODE_RX_RESP;
		g_rtk_drv->act_param.ch_opt = CSI_CH_LEGACY;
		g_rtk_drv->act_param.data_rate = 0xC;
		g_rtk_drv->act_param.accuracy = CSI_ACCU_1BYTE;
		g_rtk_drv->act_param.trig_period = (interval_ms * 1000) / CSI_INTERVAL_TO_TRIG_PERIOD;
		break;

	case NON_HT_CSI_DATA_ACC1:
		g_rtk_drv->act_param.mode = CSI_MODE_RX_RESP;
		g_rtk_drv->act_param.ch_opt = CSI_CH_LEGACY;
		g_rtk_drv->act_param.data_rate = 0xC;
		g_rtk_drv->act_param.accuracy = CSI_ACCU_2BYTES;
		g_rtk_drv->act_param.trig_period = (interval_ms * 1000) / CSI_INTERVAL_TO_TRIG_PERIOD;
		break;
	
	default:
		csidbg("ERROR: unknown config type: %d", config_type);
		return -EINVAL;
	}

	/* 3. call set config for init */
		/* set config */
	g_rtk_drv->act_param.act = 1;
	g_rtk_drv->act_param.enable = 0;
	int ret = wifi_csi_config(&g_rtk_drv->act_param); 
	if (ret != OK) {
		csidbg("ERROR: wifi csi set config(INIT) failed, ret: %d\n", ret);
		return ret;
	}
	/* 3. call set config for enable/disable */
	if (config_action == CSI_CONFIG_ENABLE) {
		csivdbg("wifi csi enable config requested\n");
		/* changes for enable */
		g_rtk_drv->act_param.act = 0;
		g_rtk_drv->act_param.enable = 1;
	} else if (config_action == CSI_CONFIG_DISABLE) {
		/* changes for disable*/
		csivdbg("wifi csi disable config requested\n");
		g_rtk_drv->act_param.act = 0;
		g_rtk_drv->act_param.enable = 0;
	}
	/* apply enable/disable */
	ret = wifi_csi_config(&g_rtk_drv->act_param); 
	if (ret != OK) {
		csidbg("ERROR: wifi csi set config(enable/disable) failed, ret: %d\n", ret);
		return ret;
	}
	return OK;
}

/****************************************************************************
 * rtk semaphore functions
 ****************************************************************************/

static inline void rtk_wifi_csi_takesem(sem_t *sem)
{
	int ret;

	do {
		ret = sem_wait(sem);
		DEBUGASSERT(ret == 0 || errno == EINTR);
	} while (ret < 0);
}

static inline int rtk_wifi_csi_givesem(sem_t *sem)
{
	return sem_post(sem);
}

static inline int rtk_wifi_csi_get_semvalue(sem_t *sem)
{
	int val;
	int ret;
	ret = sem_getvalue(sem, &val);
	if (ret < 0) {
		csidbg("ERROR: could not get semaphore value\n");
		return ret;
	}
	return val;
}

/****************************************************************************
 * rtk wificsi operations
 ****************************************************************************/

static int rtk_wifi_csi_ioctl(int cmd, unsigned long arg)
{
	if (!g_rtk_drv) {
		csidbg("ERROR: priv null\n");
		return -EINVAL;
	}
	/* Deal with ioctls passed from the upper-half driver */
	int ret = 0;
	switch (cmd) {
	case CSIIOC_SET_CONFIG: {
		csivdbg("CSIIOC_SET_CONFIG\n");
		if (!arg) {
			csidbg("ERROR: invalid config arg\n");
			ret = -EINVAL;
			return ret;
		}
		rtk_wifi_csi_takesem(&g_rtk_drv->devsem);
		ret = rtk_wifi_csi_set_config(arg);
		rtk_wifi_csi_givesem(&g_rtk_drv->devsem);
		log_csi_config();
		if (ret != OK) {
			csidbg("ERROR: wifi csi set config failed ret: %d\n", ret);
			break;
		}
		csivdbg("csi config has been set\n");
	}
	break;

	case CSIIOC_START_CSI: {
		csivdbg("CSIIOC_START_CSI\n");
		rtk_wifi_csi_takesem(&g_rtk_drv->devsem);
		wifi_reg_event_handler(WIFI_EVENT_CSI_DONE, data_ready_listener, g_rtk_drv);
		rtk_wifi_csi_givesem(&g_rtk_drv->devsem);
		csivdbg("event listener callback registered\n");
	}
	break;

	case CSIIOC_STOP_CSI: {
		csivdbg("CSIIOC_STOP_CSI\n");
		rtk_wifi_csi_takesem(&g_rtk_drv->devsem);
		wifi_unreg_event_handler(WIFI_EVENT_CSI_DONE, data_ready_listener);
		rtk_wifi_csi_givesem(&g_rtk_drv->devsem);
		csivdbg("callback unregistered \n");
	}
	break;

	default:
		csidbg("ERROR: invalid csiwifi command received [%d]\n", cmd);
		ret = -EINVAL;
		break;
	}
	return ret;
}

static int rtk_wifi_csi_getmacaddr(char *mac_addr) {
	if (!g_rtk_drv) {
		csidbg("ERROR: priv null\n");
		return -EINVAL;
	}
	rtk_wifi_csi_takesem(&g_rtk_drv->devsem);
	csivdbg("mac addr: %02x:%02x:%02x:%02x:%02x:%02x\n", g_rtk_drv->act_param.mac_addr[0], g_rtk_drv->act_param.mac_addr[1], g_rtk_drv->act_param.mac_addr[2], g_rtk_drv->act_param.mac_addr[3], g_rtk_drv->act_param.mac_addr[4], g_rtk_drv->act_param.mac_addr[5]);
	memcpy(mac_addr, g_rtk_drv->act_param.mac_addr, 6);
	rtk_wifi_csi_givesem(&g_rtk_drv->devsem);
	return OK;
}

static int rtk_wifi_csi_getcsidata(unsigned char *buffer, size_t buflen) {
	if (!g_rtk_drv) {
		csidbg("ERROR: priv null\n");
		return -EINVAL;
	}
	int len;
	rtk_wifi_csi_takesem(&g_rtk_drv->devsem);
	if (wifi_csi_report(buflen, buffer, &len) == -1) {
		csidbg("ERROR: wifi csi report call failed\n");
		len = -EIO;
	}
	rtk_wifi_csi_givesem(&g_rtk_drv->devsem);
	return len;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: rtk_csi_initialize
 *
 * Description:
 *   Initialize the rtk device.
 *
 * Input Parameters:
 *
 * Returned Value:
 *   A new lower half wificsi interface for the rtk device is returned on
 *   success; NULL is returned on failure.
 *
 ****************************************************************************/
FAR struct wifi_csi_lowerhalf_s *rtk_csi_initialize(void)
{
	/* Allocate a RTK_CSI device structure */
	g_rtk_drv = (FAR struct rtk_csi_dev_s *)kmm_zalloc(sizeof(struct rtk_csi_dev_s));
	if (g_rtk_drv == NULL) {
		csidbg("ERROR: failed to allocate driver structure\n");
		return NULL;
	}
	g_rtk_drv->dev.ops = &g_wificsiops;
	sem_init(&g_rtk_drv->devsem, 0, 1);
	return &g_rtk_drv->dev;
}

/****************************************************************************
 * Name: rtk_csi_deinitialize
 *
 * Description:
 *   Deinitialize the rtk device.
 *
 * Input Parameters:
 *
 * Returned Value:
 *
 ****************************************************************************/
void rtk_csi_deinitialize(void)
{
	if (!g_rtk_drv) {
		csidbg("ERROR: invalid priv\n");
		return;
	}
	sem_destroy(&g_rtk_drv->devsem);
	/* We should free g_rtk_drv->dev->priv here ? */
	free(g_rtk_drv);
	g_rtk_drv = NULL;
}

