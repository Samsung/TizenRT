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
#include <debug.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <tinyara/lwnl/lwnl80211.h>
#include <tinyara/wifi/wifi_utils.h>
#include "wifi_event_listener.h"

#define LWNL_TAG "[WU]"

#define LWNL_ENTER									\
	do {											\
		ndbg(LWNL_TAG"%s:%d\n", __FILE__, __LINE__);	\
	} while (0)

#define LWNL_ERR										\
	do {											\
		ndbg(LWNL_TAG"[ERR] %s:%d code(%s)\n",		\
			 __FILE__, __LINE__, strerror(errno));	\
	} while (0)

#define LWNL_CALL(fname, func)					\
	do {										\
		if (g_cbk.fname) {						\
			g.cbk.func;							\
		}										\
	} while (0)

static wifi_utils_cb_s g_cbk = {NULL, NULL, NULL, NULL, NULL};
static sem_t g_lwnl_signal;

static void _close_cb_handler(void)
{
	int sd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sd < 0) {
		LWNL_ERR;
		return;
	}

	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = 9098;
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	lwnl80211_cb_data data_s = {LWNL80211_EXIT, .u.data = NULL, 0, 0};
	int res = sendto(sd, (const char *)&data_s, sizeof(data_s), 0,
					 (const struct sockaddr *)&saddr, sizeof(saddr));
	if (res < 0) {
		LWNL_ERR;
		return;
	}

	nvdbg("wait terminate signal\n");
	sem_wait(&g_lwnl_signal);

	return;
}

static void _free_scan_data(wifi_utils_scan_list_s *scan_list)
{
	wifi_utils_scan_list_s *cur = scan_list;
	wifi_utils_scan_list_s *prev = NULL;
	while (cur) {
		prev = cur;
		cur = cur->next;
		free(prev);
	}
	scan_list = NULL;
}

static wifi_utils_result_e _receive_scan_data(int fd, wifi_utils_scan_list_s *scan_list)
{
	int ret;
	int nbytes;
	int cnt = 0;
	int msglen = sizeof(lwnl80211_cb_data);
	wifi_utils_scan_list_s *prev = scan_list;

	while (true) {
		lwnl80211_cb_data msg;
		wifi_utils_scan_list_s *cur = NULL;
		nbytes = read(fd, (char *)&msg, msglen);
		if (nbytes < 0 || nbytes != msglen) {
			ndbg("Failed to receive (nbytes=%d, msglen=%d)\n", nbytes, msglen);
			LWNL_ERR;
			ret = WIFI_UTILS_FAIL;
			break;
		}

		if (msg.status != LWNL80211_SCAN_DONE) {
			LWNL_ERR;
			ret =  WIFI_UTILS_FAIL;
			break;
		}

		cur = (wifi_utils_scan_list_s *)malloc(sizeof(wifi_utils_scan_list_s));
		if (!cur) {
			_free_scan_data(scan_list);
			ret = WIFI_UTILS_FAIL;
			break;
		}
		cur->next = NULL;
		memcpy(&(cur->ap_info), &(msg.u.ap_info), sizeof(wifi_utils_ap_scan_info_s));
		cnt++;

		prev->next = cur;
		prev = cur;
	}

	return ret;
}

static int _wifi_utils_convert_scan(wifi_utils_scan_list_s **scan_list, void *input, int len)
{
	nvdbg("[WU] T%d %s len(%d)\n", getpid(), __FUNCTION__, len);
	int remain = len;
	wifi_utils_scan_list_s *prev = NULL;

	while (remain > 0) {
		wifi_utils_scan_list_s *item = (wifi_utils_scan_list_s *)malloc(sizeof(wifi_utils_scan_list_s));
		if (!item) {
			// To Do
			return -1;
		}
		// definition of wifi_utils_scan_list_s and lwnl80211_scan_list shoud be same
		memcpy(&item->ap_info, input, sizeof(wifi_utils_ap_scan_info_s));
		item->next = NULL;

		remain -= sizeof(wifi_utils_ap_scan_info_s);
		input = input + sizeof(wifi_utils_ap_scan_info_s);
		if (!prev) {
			prev = item;
			*scan_list = item;
			continue;
		}
		prev->next = item;
		prev = item;
	}

	return 0;
}

static wifi_utils_scan_list_s *_handle_scan(int fd, int len)
{
	char *buf = (char *)malloc(len);
	if (!buf) {
		return NULL;
	}

	int res = read(fd, buf, len);
	if (res != len) {
		ndbg("read error\n");
		free(buf);
		return NULL;
	}

	wifi_utils_scan_list_s *scan_list = NULL;
	res = _wifi_utils_convert_scan(&scan_list, buf, len);
	free(buf);
	if (res < 0) {
		return NULL;
	}
	return scan_list;
}

static int _wifi_utils_call_event(int fd, lwnl80211_cb_status status, int len)
{
	switch (status) {
	case LWNL80211_STA_CONNECTED:
		if (g_cbk.sta_connected) {
			g_cbk.sta_connected(WIFI_UTILS_SUCCESS, NULL);
		}
		break;
	case LWNL80211_STA_CONNECT_FAILED:
		if (g_cbk.sta_connected) {
			g_cbk.sta_connected(WIFI_UTILS_FAIL, NULL);
		}
		break;
	case LWNL80211_STA_DISCONNECTED:
		if (g_cbk.sta_disconnected) {
			g_cbk.sta_disconnected(NULL);
		}
		break;
	case LWNL80211_SOFTAP_STA_JOINED:
		if (g_cbk.softap_sta_joined) {
			g_cbk.softap_sta_joined(NULL);
		}
		break;
	case LWNL80211_SOFTAP_STA_LEFT:
		if (g_cbk.softap_sta_left) {
			g_cbk.softap_sta_left(NULL);
		}
		break;
	case LWNL80211_SCAN_FAILED:
		if (g_cbk.scan_done) {
			g_cbk.scan_done(WIFI_UTILS_FAIL, NULL, NULL);
		}
		break;
	case LWNL80211_SCAN_DONE:
	{
		if (!g_cbk.scan_done) {
			break;
		}
		wifi_utils_scan_list_s *scan_list = _handle_scan(fd, len);
		if (scan_list) {
			g_cbk.scan_done(WIFI_UTILS_SUCCESS, scan_list, 0);
		} else {
			g_cbk.scan_done(WIFI_UTILS_FAIL, NULL, NULL);
		}
		break;
	}
	default:
		ndbg("Bad status received (%d)\n", status);
		LWNL_ERR;
		return -1;
	}
	return 0;
}

static int _wifi_utils_fetch_event(int fd)
{
	lwnl80211_cb_status status;
	uint32_t len;
	char type_buf[8] = {0,};
	int nbytes = read(fd, (char *)type_buf, 8);

	if (nbytes < 0) {
		ndbg("Failed to receive (nbytes=%d)\n", nbytes);
		LWNL_ERR;
		return -1;
	}

	memcpy(&status, type_buf, sizeof(lwnl80211_cb_status));
	memcpy(&len, type_buf + sizeof(lwnl80211_cb_status), sizeof(uint32_t));

	ndbg("%d %d\n", status, len);
	(void)_wifi_utils_call_event(fd, status, len);

	return 0;
}

static int _wifi_utils_callback_handler(int argc, char *argv[])
{
	LWNL_ENTER;
	ndbg("run utils callback handler (should be moved to booting)\n");
	fd_set rfds, ofds;

	int nd = socket(AF_LWNL, SOCK_RAW, LWNL_ROUTE);
	if (nd < 0) {
		LWNL_ERR;
		return -1;
	}

	int sd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sd < 0) {
		LWNL_ERR;
		close(nd);
		return -1;
	}
	struct sockaddr_in saddr;
	memset(&saddr, 0, sizeof(struct sockaddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = 9098;

	int res = bind(sd, (struct sockaddr *)&saddr, sizeof(struct sockaddr));
	if (res < 0) {
		LWNL_ERR;
		close(nd);
		close(sd);
		return -1;
	}

	FD_ZERO(&ofds);
	FD_SET(nd, &ofds);
	FD_SET(sd, &ofds);

	int maxfd = (nd > sd) ? (nd + 1) : (sd + 1);

	// notify initialization of receive handler is done
	sem_post(&g_lwnl_signal);

	while (1) {
		rfds = ofds;
		res = select(maxfd, &rfds, NULL, NULL, NULL);
		if (res < 0) {
			LWNL_ERR;
			break;
		}

		if (res == 0) {
			LWNL_ERR;
			break;
		}

		if (FD_ISSET(nd, &rfds)) {
			// get events from netlink driver
			res = _wifi_utils_fetch_event(nd);
			if (res < 0) {
				ndbg("message currupted\n");
				break;
			}
		} else if (FD_ISSET(sd, &rfds)) {
			// get terminate event from application
			nvdbg("get terminate message\n");
			sem_post(&g_lwnl_signal);
			break;
		} else {
			// unknown error
			LWNL_ERR;
			break;
		}
	}

	close(nd);
	close(sd);
	return 0;
}

/*
 * Public
 */
void lwnl_start_monitor(void)
{
	int tid = task_create("lwnl8021 cb handler", 110, 4096, (main_t)_wifi_utils_callback_handler, NULL);
	if (tid < 0) {
		LWNL_ERR;
	}
}

void lwnl_stop_monitor(void)
{
	_close_cb_handler();
}

int lwnl_join_monitor(wifi_utils_cb_s *cbk)
{
	if (!cbk) {
		return WIFI_UTILS_INVALID_ARGS;
	}
	g_cbk = *cbk;

	return WIFI_UTILS_SUCCESS;
}


int lwnl_leave_monitor(wifi_utils_cb_s *cb)
{
	//TODO: g_cbk will support multiple callback from various applications.
	//This 'cb' then can be used to find the proper set of callbacks to be removed from the list
	(void)cb;
	g_cbk.sta_connected = NULL;
	g_cbk.sta_disconnected = NULL;
	g_cbk.softap_sta_joined = NULL;
	g_cbk.softap_sta_left = NULL;
	g_cbk.scan_done = NULL;

	return WIFI_UTILS_SUCCESS;
}
