/******************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/
#include <tinyara/config.h>
#include <pthread.h>
#include <debug.h>
#include <net/if.h>
#include <tinyara/lwnl/lwnl.h>
#include <tinyara/net/if/wifi.h>
#include <tinyara/netmgr/netdev_mgr.h>
#include <tinyara/net/if/wifi.h>
#include <include/modules/wifi.h>
#include <bk_netmgr.h>
#include <components/event.h>
#include <bk_wifi_adapter.h>
#include <bk_wifi.h>
#include <components/log.h>

//wifi csi
#if CONFIG_BK_WIFI_CSI_ADAPTER
#include <tinyara/wifi_csi/wifi_csi.h>

#define BK_MAX_CSI_BUFF_LEN (536)

struct bk_csi_dev_s {
	struct wifi_csi_lowerhalf_s dev;
	beken_semaphore_t devsem;
	wifi_csi_receive_mode_config_t config_param;
	unsigned int interval_ms;
	unsigned int accuracy;
	int csi_data_reported_idx;
};

struct bk_csi_rept_flag_s {
	uint32_t data_received_flag;
	uint32_t data_reported_flag;
	uint32_t data_store_buff_ptr;
};


static int bk_wifi_csi_ioctl(int cmd, unsigned long arg);
static int bk_wifi_csi_getmacaddr(char *mac_addr);
static int bk_wifi_csi_getcsidata(unsigned char *buffer, size_t buflen);


static const struct wifi_csi_ops_s g_wificsiops = {
	NULL,                           /* read           */
	bk_wifi_csi_ioctl,             /* ioctl          */
	bk_wifi_csi_getmacaddr,        /* getmacaddr     */
	bk_wifi_csi_getcsidata,        /* getcsidata     */
};


static FAR struct bk_csi_dev_s *g_bk_drv;

struct bk_csi_rept_flag_s g_bk_csi_rept_data = {0};



static inline void bk_wifi_csi_takesem(void)
{
	int ret = 0;
	if(g_bk_drv->devsem != NULL)
	{
		ret = rtos_get_semaphore(&g_bk_drv->devsem, 10000);
		if(ret == kNoErr)
		{
			return;
		}
	}
}

static inline int bk_wifi_csi_givesem(void)
{
	int ret = 0;
	if (g_bk_drv->devsem) {
		ret = rtos_set_semaphore(&g_bk_drv->devsem);
		if(kNoErr !=ret)
			csidbg("bk_wifi_csi_givesem: set sema failed\r\n");
	}
	return ret;
}

static void bk_wifi_csi_report_event(void)
{
	if(g_bk_drv == NULL)
	{
		csidbg("ERROR: priv null\n");
		return;
	}

	csivdbg("csi_report_event\n");
	
	struct wifi_csi_lowerhalf_s *dev = &g_bk_drv->dev;
	int len = 0;
	// NON_HT_CSI_DATA
	if(g_bk_drv->config_param.filter_config.proto_type_bmp == 0x1)
	{
		len += 52 * (g_bk_drv->accuracy+1)*2;
	}
	// HT_CSI_DATA
	else if(g_bk_drv->config_param.filter_config.proto_type_bmp == 0x2)
	{
		len += 56 * (g_bk_drv->accuracy+1)*2;
	}
	// TODO VHT_CSI_DATA, HE_CSI_DATA
	
	g_bk_csi_rept_data.data_reported_flag = 1;
	/* buf_len -> data len with header */
	dev->upper_cb(dev->priv, CSI_CALLBACK_DATA_READY, NULL, len);
}

static void bk_wifi_csi_report_data_transfer(uint32_t buf,uint8_t *csi_buf, uint32_t *len)
{
	struct wifi_csi_info_t *buf_info = (struct wifi_csi_info_t *)buf;
	int len_offset = sizeof(struct wifi_rx_pkt_t);

	// NON_HT_CSI_DATA
	if(g_bk_drv->config_param.filter_config.proto_type_bmp == 0x1)
	{
		memcpy(csi_buf, buf_info, len_offset);
		for(int i = 0; i < buf_info->csi_info[0].sc_num; i++)
		{
			if(i != 26)
			{
				if(g_bk_drv->accuracy == 0)
				{
					int8_t csi_real = (int8_t)(((int16_t)buf_info->csi_info[0].csi_data[i].real)>>8);
					int8_t csi_imag = (int8_t)(((int16_t)buf_info->csi_info[0].csi_data[i].imag)>>8);
					csi_buf[len_offset] = csi_real;
					len_offset++;
					csi_buf[len_offset] = csi_imag;
					len_offset++;
				}
				else if(g_bk_drv->accuracy == 1)
				{
					int16_t csi_real = (int16_t)(buf_info->csi_info[0].csi_data[i].real);
					int16_t csi_imag = (int16_t)(buf_info->csi_info[0].csi_data[i].imag);
					memcpy((void *)(csi_buf+len_offset), &csi_real, 2);
					len_offset += 2;
					memcpy((void *)(csi_buf+len_offset), &csi_imag, 2);
					len_offset += 2;
				}
			}
		}
	}
	// HT_CSI_DATA
	else if(g_bk_drv->config_param.filter_config.proto_type_bmp == 0x2)
	{
		memcpy(csi_buf, buf_info, len_offset);
		for(int i = 0; i < buf_info->csi_info[0].sc_num; i++)
		{
			if(i != 28)
			{
				if(g_bk_drv->accuracy == 0)
				{
					int8_t csi_real = (int8_t)(((int16_t)buf_info->csi_info[0].csi_data[i].real)>>8);
					int8_t csi_imag = (int8_t)(((int16_t)buf_info->csi_info[0].csi_data[i].imag)>>8);
					csi_buf[len_offset] = csi_real;
					len_offset++;
					csi_buf[len_offset] = csi_imag;
					len_offset++;
				}
				else if(g_bk_drv->accuracy == 1)
				{
					int16_t csi_real = (int16_t)(buf_info->csi_info[0].csi_data[i].real);
					int16_t csi_imag = (int16_t)(buf_info->csi_info[0].csi_data[i].imag);
					memcpy((void *)(csi_buf+len_offset), &csi_real, 2);
					len_offset += 2;
					memcpy((void *)(csi_buf+len_offset), &csi_imag, 2);
					len_offset += 2;
				}
			}
		}
	}
	// TODO VHT_CSI_DATA, HE_CSI_DATA
}

static int bk_wifi_csi_get(uint32_t buf_len, uint8_t *csi_buf, uint32_t *len)
{
	if(g_bk_csi_rept_data.data_reported_flag == 1)
	{
		bk_wifi_csi_report_data_transfer(g_bk_csi_rept_data.data_store_buff_ptr, csi_buf, len);
		g_bk_csi_rept_data.data_reported_flag = 0;
		g_bk_csi_rept_data.data_received_flag = 0;
	}
	else
	{
		csidbg("ERROR: no data to report");
		return -ENODATA;
	}
	return 0;
}

static int bk_wifi_csi_set_config(unsigned long arg)
{
	int ret = 0;
	csi_config_args_t *config_args = (csi_config_args_t *)arg; 
	csi_config_action_t config_action = config_args->config_action;
	csi_config_type_t config_type = config_args->config_type;
	if (config_type <= MIN_CSI_CONFIG_TYPE || config_type >= MAX_CSI_CONFIG_TYPE) {
		csidbg("ERROR: invalid config type");
		return -EINVAL;
	}
	/* 1. memset config zero */
	memset(&g_bk_drv->config_param, 0, sizeof(wifi_csi_receive_mode_config_t));
	
	/* 2. assign configs */
	g_bk_drv->interval_ms = config_args->interval;

	switch (config_type) {
	case HT_CSI_DATA:
		g_bk_drv->config_param.filter_config.proto_type_bmp = 0x2;
		g_bk_drv->config_param.filter_config.cbw_bmp = 0x1;
		g_bk_drv->config_param.filter_config.filter_mac_addr_type = 0;
		g_bk_drv->accuracy = 0;
		break;
	
	case HT_CSI_DATA_ACC1:
		g_bk_drv->config_param.filter_config.proto_type_bmp = 0x2;
		g_bk_drv->config_param.filter_config.cbw_bmp = 0x1;
		g_bk_drv->config_param.filter_config.filter_mac_addr_type = 0;
		g_bk_drv->accuracy = 1;
		break;
	
	case NON_HT_CSI_DATA:
		g_bk_drv->config_param.filter_config.proto_type_bmp = 0x1;
		g_bk_drv->config_param.filter_config.cbw_bmp = 0x1;
		g_bk_drv->config_param.filter_config.filter_mac_addr_type = 0;
		g_bk_drv->accuracy = 0;
		break;

	case NON_HT_CSI_DATA_ACC1:
		g_bk_drv->config_param.filter_config.proto_type_bmp = 0x1;
		g_bk_drv->config_param.filter_config.cbw_bmp = 0x1;
		g_bk_drv->config_param.filter_config.filter_mac_addr_type = 0;
		g_bk_drv->accuracy = 1;
		break;
	
	default:
		csidbg("ERROR: unknown config type: %d", config_type);
		return -EINVAL;
	}
	
	if(g_bk_csi_rept_data.data_store_buff_ptr == 0) 
	{
		g_bk_csi_rept_data.data_received_flag = 0;
		g_bk_csi_rept_data.data_reported_flag = 0;
		g_bk_csi_rept_data.data_store_buff_ptr = (uint32_t)os_malloc(BK_MAX_CSI_BUFF_LEN);
		if(g_bk_csi_rept_data.data_store_buff_ptr == 0) 
		{
			csidbg("ERROR: failed to allocate csi data buffer\n");
			return -ENOMEM;
		}
		os_memset(g_bk_csi_rept_data.data_store_buff_ptr, 0, BK_MAX_CSI_BUFF_LEN);
	}

	if(g_bk_drv->config_param.enable == 1) 
	{	
		g_bk_drv->config_param.enable = 0;
		ret = bk_wifi_csi_receive_mode_req(&g_bk_drv->config_param);
		if (ret != OK) 
		{
			csidbg("ERROR: wifi csi set config(enable) failed, ret: %d\n", ret);
			return ret;
		}
	}

	/* 3. call set config for enable/disable */
	if (config_action == CSI_CONFIG_ENABLE) 
	{
		csivdbg("wifi csi enable config requested\n");
		/* changes for enable */
		g_bk_drv->config_param.enable = 1;
		ret = bk_wifi_csi_receive_mode_req(&g_bk_drv->config_param);
		if (ret != OK) 
		{
			csidbg("ERROR: wifi csi set config(enable) failed, ret: %d\n", ret);
			return ret;
		}
	} 
	else if (config_action == CSI_CONFIG_DISABLE) 
	{
		/* changes for disable*/
		csivdbg("wifi csi disable config requested\n");
		g_bk_drv->config_param.enable = 0;
		ret = bk_wifi_csi_receive_mode_req(&g_bk_drv->config_param);
		if (ret != OK) {
			csidbg("ERROR: wifi csi set config(disable) failed, ret: %d\n", ret);
			return ret;
		}
	}

	return OK;
}

bool bk_wifi_csi_info_copy(uint32_t buf_ptr, struct wifi_csi_info_t *info)
{
	bool is_data_valid = false;
	uint32_t len = sizeof(struct wifi_csi_info_t);
	uint32_t buf = buf_ptr;
	if(info == NULL) {
		return is_data_valid;
	}
	if(g_bk_drv == NULL)
	{
		return is_data_valid;
	}

	// NON_HT_CSI_DATA
	if(g_bk_drv->config_param.filter_config.proto_type_bmp == 0x1)
	{
		for(int i = 0; i < MAX_NUM; i++)
		{
			if(info->csi_info[i].ltf_type == 1)
			{
				if(info->csi_info[i].sc_num > 0 && info->csi_info[i].csi_data != NULL) 
				{
					memcpy((void *)buf, info, len);
					memcpy((void *)(buf+len), info->csi_info[i].csi_data, info->csi_info[i].sc_num * sizeof(struct csi_cfr_t));
					struct wifi_csi_info_t *buf_info = (struct wifi_csi_info_t *)buf;
					buf_info->csi_info[0].sc_num = info->csi_info[i].sc_num;
					buf_info->csi_info[0].ltf_type = info->csi_info[i].ltf_type;
					buf_info->csi_info[0].csi_data = (struct csi_cfr_t *)(buf+len);
					is_data_valid = true;
					break;
				}
				break;
			}
		}
	}
	// HT_CSI_DATA
	else if(g_bk_drv->config_param.filter_config.proto_type_bmp == 0x2)
	{
		for(int i = 0; i < MAX_NUM; i++)
		{
			if(info->csi_info[i].ltf_type == 2)
			{
				if(info->csi_info[i].sc_num > 0 && info->csi_info[i].csi_data != NULL) 
				{
					memcpy((void *)buf, info, len);
					memcpy((void *)(buf+len), info->csi_info[i].csi_data, info->csi_info[i].sc_num * sizeof(struct csi_cfr_t));
					struct wifi_csi_info_t *buf_info = (struct wifi_csi_info_t *)buf;
					buf_info->csi_info[0].sc_num = info->csi_info[i].sc_num;
					buf_info->csi_info[0].ltf_type = info->csi_info[i].ltf_type;
					buf_info->csi_info[0].csi_data = (struct csi_cfr_t *)(buf+len);
					is_data_valid = true;
					break;
				}
				break;
			}
		}
	}
	//TODO:VHT_CSI_DATA, HE_CSI_DATA
	
	return is_data_valid;
}
beken_time_t rtos_get_time( void );
beken_time_t g_bk_csi_last_report_time = 0;
void bk_wifi_csi_rx_cb(struct wifi_csi_info_t *info)
{
	if((info != NULL) && (g_bk_csi_rept_data.data_store_buff_ptr != 0))
	{
		bool is_report_data = false;
		if(g_bk_csi_last_report_time == 0)
		{
			g_bk_csi_last_report_time = rtos_get_time();
			is_report_data = true;
		}
		else
		{
			beken_time_t current_time = rtos_get_time();
			if(current_time > g_bk_csi_last_report_time)
			{
				if(current_time - g_bk_csi_last_report_time >= (g_bk_drv->interval_ms - 5))
				{
					g_bk_csi_last_report_time = current_time;
					is_report_data = true;
				}
			}
			else
			{
				g_bk_csi_last_report_time = current_time;
			}
		}

		if(is_report_data)
		{
			if(g_bk_csi_rept_data.data_reported_flag == 0)
			{
				if(bk_wifi_csi_info_copy(g_bk_csi_rept_data.data_store_buff_ptr, info))
				{
					g_bk_csi_rept_data.data_received_flag = 1;
					bk_wifi_csi_report_event();
				}
			}
		}
		
	}
}

static int bk_wifi_csi_ioctl(int cmd, unsigned long arg)
{
	if (!g_bk_drv) {
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
		bk_wifi_csi_takesem();
		ret = bk_wifi_csi_set_config(arg);
		bk_wifi_csi_givesem();

		if (ret != OK) {
			csidbg("ERROR: wifi csi set config failed ret: %d\n", ret);
			break;
		}
		csivdbg("csi config has been set\n");
	}
	break;

	case CSIIOC_START_CSI: {
		csivdbg("CSIIOC_START_CSI\n");
		bk_wifi_csi_takesem();
		bk_wifi_csi_info_cb_register(bk_wifi_csi_rx_cb);
		bk_wifi_csi_givesem();
		csivdbg("event listener callback registered\n");
	}
	break;

	case CSIIOC_STOP_CSI: {
		csivdbg("CSIIOC_STOP_CSI\n");
		bk_wifi_csi_takesem();
		bk_wifi_csi_info_cb_register(NULL);
		bk_wifi_csi_givesem();
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

static int bk_wifi_csi_getmacaddr(char *mac_addr) {
	if (!g_bk_drv) {
		csidbg("ERROR: priv null\n");
		return -EINVAL;
	}
	//if (sta_ip_is_start()) 
	{
		wifi_link_status_t link_status = {0};
		os_memset(&link_status, 0x0, sizeof(link_status));
		BK_RETURN_ON_ERR(bk_wifi_sta_get_link_status(&link_status));

		bk_wifi_csi_takesem();
		csivdbg("mac addr: %02x:%02x:%02x:%02x:%02x:%02x\n", link_status.bssid[0], link_status.bssid[1], link_status.bssid[2], 
			link_status.bssid[3], link_status.bssid[4], link_status.bssid[5]);
		memcpy(mac_addr, link_status.bssid, 6);
		bk_wifi_csi_givesem();
	}
	//else
	//{
	//	csidbg("ERROR: not in station mode\n");
	//	return -EINVAL;
	//}
	
	return OK;
}

static int bk_wifi_csi_getcsidata(unsigned char *buffer, size_t buflen) {
	if (!g_bk_drv) {
		csidbg("ERROR: priv null\n");
		return -EINVAL;
	}
	uint32_t len;
	bk_wifi_csi_takesem();
	if (bk_wifi_csi_get(buflen, buffer, &len) == -1) {
		csidbg("ERROR: wifi csi report call failed\n");
		len = -EIO;
	}
	bk_wifi_csi_givesem();
	return len;
}

FAR struct wifi_csi_lowerhalf_s *bk_wifi_csi_initialize(void)
{
	int err = 0;
	/* Allocate a RTK_CSI device structure */
	g_bk_drv = (FAR struct bk_csi_dev_s *)os_malloc(sizeof(struct bk_csi_dev_s));
	if (g_bk_drv == NULL) {
		csidbg("ERROR: failed to allocate driver structure\n");
		return NULL;
	}
	g_bk_drv->dev.ops = &g_wificsiops;

	if(g_bk_drv->devsem == NULL) {
		err = rtos_init_semaphore(&g_bk_drv->devsem,1);
		if(err != kNoErr){
			csidbg("[BK] csi semaphore init failed!\r\n");
			return NULL;
		}
	}

	return &g_bk_drv->dev;
}

void bk_wifi_csi_deinitialize(void)
{
	if (!g_bk_drv) {
		csidbg("ERROR: invalid priv\n");
		return;
	}

	if(g_bk_drv->devsem != NULL) {
		rtos_deinit_semaphore(&g_bk_drv->devsem);
	}

	/* We should free g_rtk_drv->dev->priv here ? */
	free(g_bk_drv);
	g_bk_drv = NULL;
}

//bk_wifi_csi_init(0)
int bk_wifi_csi_init(int minor)
{
	FAR struct wifi_csi_lowerhalf_s *bkcsi;
	static bool bk_csi_initialized = false;
	char devname[12];
	int ret = 0;

	csivdbg("minor %d\n", minor);

	if (!bk_csi_initialized) {
		
		bkcsi = bk_wifi_csi_initialize();
		if (bkcsi == NULL) {
			csidbg("bk_wifi_csi_initialize failed \n");
			return -1;
		}

		snprintf(devname, sizeof(devname), "csi%u%u", (unsigned int)minor, (unsigned int)0);

		ret = wifi_csi_register(devname, bkcsi);
		
		if(ret != 0) {
			csidbg("wifi_csi_register failed \n");
			bk_wifi_csi_deinitialize();
			return ret;
		}
		bk_csi_initialized = true;
	}
	return ret;
}

#endif
