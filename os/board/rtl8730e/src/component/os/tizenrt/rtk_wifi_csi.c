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
#include "rtk_wifi_csi.h"

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int rtk_wifi_csi_ioctl(FAR struct wifi_csi_lowerhalf_s *dev, int cmd, unsigned long arg);
static int rtk_wifi_csi_getmacaddr(FAR struct wifi_csi_lowerhalf_s *dev, char *mac_addr);
static int rtk_wifi_csi_getcsidata(FAR struct wifi_csi_lowerhalf_s *dev, char *buffer, size_t buflen);

/****************************************************************************
 * Private Data
 ****************************************************************************/
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
	struct wifi_csi_lowerhalf_s *dev = (struct wifi_csi_lowerhalf_s *)userdata;
	FAR struct wifi_csi_dev_s *priv = (FAR struct wifi_csi_dev_s *)dev;
	// DEBUGASSERT(priv && priv->dev.upper && apb); //I think we should use this too.
	if (!dev) {
		csidbg("ERROR: dev null\n");
		return;
	}
	if (!priv) {
		csidbg("ERROR: priv null\n");
		return;
	}
	/* flags-> data len with header */
	priv->dev.upper(priv->dev.priv, CSI_CALLBACK_DATA_READY, NULL, flags);
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

static int rtk_wifi_csi_ioctl(FAR struct wifi_csi_lowerhalf_s *dev, int cmd, unsigned long arg)
{
	FAR struct wifi_csi_dev_s *priv = (FAR struct wifi_csi_dev_s *)dev;
	if (!priv) {
		csidbg("ERROR: priv null\n");
		return -EINVAL;
	}
	/* Deal with ioctls passed from the upper-half driver */
	int ret = 0;
	switch (cmd) {
	case CSIIOC_PARAM_SETZERO: {
		csivdbg("CSIIOC_PARAM_SETZERO\n");
		rtk_wifi_csi_takesem(&priv->devsem);
		memset(&priv->act_param, 0, sizeof(rtw_csi_action_parm_t));
		rtk_wifi_csi_givesem(&priv->devsem);
	}
	break;

	case CSIIOC_SET_CONFIG: {
		csivdbg("CSIIOC_SET_CONFIG\n");
		if (!arg) {
			csidbg("ERROR: invalid config arg\n");
			ret = -EINVAL;
			return ret;
		}
		rtk_wifi_csi_takesem(&priv->devsem);
		rtw_csi_action_parm_t *g_act_param_t = ((rtw_csi_action_parm_t*)(arg));
		priv->act_param.group_num = g_act_param_t->group_num;
		priv->act_param.mode = g_act_param_t->mode;
		priv->act_param.accuracy = g_act_param_t->accuracy;
		priv->act_param.ch_opt = g_act_param_t->ch_opt;
		priv->act_param.trig_period = g_act_param_t->trig_period;
		priv->act_param.data_rate = g_act_param_t->data_rate;
		priv->act_param.act = g_act_param_t->act;
		priv->act_param.enable = g_act_param_t->enable;		
		ret = wifi_csi_config(&priv->act_param);
		if (ret != 0) {
			csidbg("ERROR: wifi csi set config failed ret: %d\n", ret);
			ret = -EIO;
			rtk_wifi_csi_givesem(&priv->devsem);
			break;
		}
		rtk_wifi_csi_givesem(&priv->devsem);
		csivdbg("csi config has been set\n");
	}
	break;

	case CSIIOC_REGISTER_CALLBACK: {
		csivdbg("CSIIOC_REGISTER_CALLBACK\n");
		rtk_wifi_csi_takesem(&priv->devsem);
		wifi_reg_event_handler(WIFI_EVENT_CSI_DONE, data_ready_listener, dev);
		rtk_wifi_csi_givesem(&priv->devsem);
		csivdbg("event listener callback registered\n");
	}
	break;

	case CSIIOC_UNREGISTER_CALLBACK: {
		csivdbg("CSIIOC_UNREGISTER_CALLBACK\n");
		rtk_wifi_csi_takesem(&priv->devsem);
		wifi_unreg_event_handler(WIFI_EVENT_CSI_DONE, data_ready_listener);
		rtk_wifi_csi_givesem(&priv->devsem);
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

static int rtk_wifi_csi_getmacaddr(FAR struct wifi_csi_lowerhalf_s *dev, char *mac_addr) {
	FAR struct wifi_csi_dev_s *priv = (FAR struct wifi_csi_dev_s *)dev;
	if (!priv) {
		csidbg("ERROR: priv null\n");
		return -EINVAL;
	}
	rtk_wifi_csi_takesem(&priv->devsem);
	csivdbg("mac addr: %02x:%02x:%02x:%02x:%02x:%02x\n", priv->act_param.mac_addr[0], priv->act_param.mac_addr[1], priv->act_param.mac_addr[2], priv->act_param.mac_addr[3], priv->act_param.mac_addr[4], priv->act_param.mac_addr[5]);
	memcpy(mac_addr, priv->act_param.mac_addr, 6);
	rtk_wifi_csi_givesem(&priv->devsem);
	return 0;
}

static int rtk_wifi_csi_getcsidata(FAR struct wifi_csi_lowerhalf_s *dev, char *buffer, size_t buflen) {
	FAR struct wifi_csi_dev_s *priv = (FAR struct wifi_csi_dev_s *)dev;
	if (!priv) {
		csidbg("ERROR: priv null\n");
		return -EINVAL;
	}
	int len;
	if (wifi_csi_report(buflen, buffer, &len) == -1) {
		csidbg("ERROR: wifi csi report call failed\n");
		return -EIO;
	}
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
FAR struct wifi_csi_lowerhalf_s *rtk_csi_initialize()
{
	FAR struct wifi_csi_dev_s *priv;

	/* Allocate a RTK_CSI device structure */
	priv = (FAR struct wifi_csi_dev_s *)kmm_zalloc(sizeof(struct wifi_csi_dev_s));
	if (priv == NULL) {
		csidbg("ERROR: failed to allocate driver structure\n");
		return NULL;
	}
	priv->dev.ops = &g_wificsiops;
	sem_init(&priv->devsem, 0, 1);
	return &priv->dev;
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
void rtk_csi_deinitialize(FAR struct wifi_csi_lowerhalf_s *dev)
{
	if (!dev) {
		csidbg("ERROR: invalid parameter\n");
		return;
	}
	FAR struct wifi_csi_dev_s *priv = (struct wifi_csi_dev_s *)dev;
	sem_destroy(&priv->devsem);
	free(priv);
}

