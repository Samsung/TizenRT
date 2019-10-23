/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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

#include <tinyara/config.h>
#include <errno.h>
#include <sys/types.h>
#ifndef CONFIG_NET_NETMGR
#include <tinyara/lwnl/lwnl80211.h>

static inline int _lwnl_message_fetch(const char *input, uint8_t *name, lwnl_req *req, void **data, uint32_t *data_len)
{
	if (!input) {
		return -1;
	}

	lwnl80211_msg *msg = (lwnl80211_msg *)input;
	name = msg->name;
	*req = msg->req_type;
	*data = msg->data;
	*data_len = msg->data_len;

	return 0;
}

int lwnl_message_handle(struct lwnl80211_lowerhalf_s* lower, const char *msg, int msg_len)
{
	uint8_t *name = NULL;
	void *data = NULL;
	lwnl_req req = LWNL80211_REQ_UNKNOWN;
	uint32_t data_len = 0;
	lwnl80211_result_e res = LWNL80211_SUCCESS;

	int ret = _lwnl_message_fetch(msg, name, &req, &data, &data_len);
	if (ret < 0) {
		ndbg("fetch lwnl message fail\n");
		return -ENOSYS;
	}

	switch (req) {
	case LWNL80211_INIT:
	{
		res = lower->ops->init(NULL);
	}
	break;
	case LWNL80211_DEINIT:
	{
		res = lower->ops->deinit();
	}
	break;
	case LWNL80211_GET_INFO:
	{
		lwnl80211_info *info = (lwnl80211_info *)data;
		res = lower->ops->get_info(info);
	}
	break;
	case LWNL80211_SET_AUTOCONNECT:
	{
		uint8_t *check = (uint8_t *)data;
		res = lower->ops->set_autoconnect(*check);
	}
	break;
	case LWNL80211_START_STA:
	{
		res = lower->ops->start_sta();
	}
	break;
	case LWNL80211_CONNECT_AP:
	{
		lwnl80211_ap_config_s *config = (lwnl80211_ap_config_s *)data;
		res = lower->ops->connect_ap(config, NULL);
	}
	break;
	case LWNL80211_DISCONNECT_AP:
	{
		res = lower->ops->disconnect_ap(NULL);
	}
	break;
	case LWNL80211_START_SOFTAP:
	{
		lwnl80211_softap_config_s *config = (lwnl80211_softap_config_s *)data;
		res = lower->ops->start_softap(config);
	}
	break;
	case LWNL80211_STOP_SOFTAP:
	{
		res = lower->ops->stop_softap();
	}
	break;
	case LWNL80211_SCAN_AP:
	{
		lwnl80211_ap_config_s *config = (lwnl80211_ap_config_s *)data;
		res = lower->ops->scan_ap(config);
	}
	break;
	default:
		ret = -ENOSYS;
		break;
	}

	if (res != LWNL80211_SUCCESS) {
		ret = -ENOSYS;
	}

	return ret;
}

#endif // CONFIG_NET_NETMGR
