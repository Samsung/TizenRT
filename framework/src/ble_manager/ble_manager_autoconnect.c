/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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
#include <sched.h>
#include <pthread.h>
#include <fcntl.h>
#include <mqueue.h>
#include <string.h>
#include <errno.h>
#include "ble_manager_common.h"
#include "ble_manager_autoconnect.h"
#include "ble_manager_log.h"

static ble_client_ctx_internal *g_ctx = NULL;

static void ble_auto_scan_state_changed_cb(ble_scan_state_e scan_state)
{
	int status = 0;
	char buf[BLE_MQ_SIZE] = { 0, };
	
	if (scan_state == BLE_SCAN_STARTED) {
		buf[0] = (char)BLE_AUTOCON_EVT_SCAN_START;
	} else if (scan_state == BLE_SCAN_STOPPED) {
		buf[0] = (char)BLE_AUTOCON_EVT_SCAN_STOP;
	} else {
		return;
	}
	
	status = mq_send(g_ctx->mqfd, buf, BLE_MQ_SIZE, 0);
	if (status < 0) {
		BLE_LOG_ERROR("[BLEMGR][%s] fail to send mqueue[%d]\n", __FUNCTION__, errno);
	}
	return;
}

static void ble_auto_device_scanned_cb(ble_scanned_device *scanned_device)
{
	if (g_ctx == NULL) {
		return;
	}

	int status = 0;
	char buf[BLE_MQ_SIZE] = { 0, };

	if (scanned_device->adv_type == BLE_ADV_TYPE_DIRECT) {
		return;
	}
	if (memcmp(g_ctx->info.addr.mac, scanned_device->addr.mac, BLE_BD_ADDR_MAX_LEN) == 0) {
		buf[0] = (char)BLE_AUTOCON_EVT_MAC_SCANNED;
		status = mq_send(g_ctx->mqfd, buf, BLE_MQ_SIZE, 0);
		if (status < 0) {
			BLE_LOG_ERROR("[BLEMGR][%s] fail to send mqueue[%d]\n", __FUNCTION__, errno);
		}
	}
	return;
}

static ble_scan_callback_list scan_config = {
	ble_auto_scan_state_changed_cb,
	ble_auto_device_scanned_cb,
};

static void *_autocon_process(void *param)
{
	BLE_LOG_INFO("[BLEMGR] Start auto connect\n");

	ble_autocon_state state = BLE_AUTOCON_STATE_DISCONNECT;
	ble_autocon_event evt;
	ble_client_ctx_internal *ctx = (ble_client_ctx_internal *)param;

	struct mq_attr attr;
	char mq_name[10] = { 0, };
	int nbytes;
	char buf[BLE_MQ_SIZE];
	ble_result_e ret;
	int conn_stop = 0;

	attr.mq_maxmsg = 10;
	attr.mq_msgsize = BLE_MQ_SIZE;
	attr.mq_flags = 0;

	sprintf(mq_name, "%p", ctx);
	ctx->mqfd = mq_open(mq_name, O_RDWR | O_CREAT, 0666, &attr);
	if (ctx->mqfd == (mqd_t)ERROR) {
		BLE_LOG_ERROR("[BLEMGR] fail to open mqueue(fd : %d , err : %d)", ctx->mqfd, errno);
		ctx->state = BLE_CLIENT_IDLE;
		return NULL;
	}
	g_ctx = ctx;

	while (ctx->state == BLE_CLIENT_AUTOCONNECTING) {
		if (state == BLE_AUTOCON_STATE_DISCONNECT) {
			ret = ble_client_start_scan(NULL, &scan_config);
			if (ret != BLE_MANAGER_SUCCESS) {
				BLE_LOG_ERROR("[BLEMGR] auto conn scan fail[%d]\n", ret);
				goto finish_auto;
			}
		}
		memset(buf, 0, sizeof(buf));
		for (;;) {
			nbytes = mq_receive(ctx->mqfd, buf, sizeof(buf), 0);
			if (nbytes < 0) {
				int err_no = get_errno();
				if (err_no == EINTR || err_no == EAGAIN) {
					continue;
				}
				BLE_LOG_ERROR("[BLEMGR] mq_receive fail(fd : %d , err : %d)", ctx->mqfd, errno);
				goto finish_auto;
			}
			break;
		}
		evt = (ble_autocon_event)(buf[0]);
		BLE_LOG_DEBUG("[BLEMGR] mqueue evt : %d\n", evt);

		/* This is finishing process */
		if (evt == BLE_AUTOCON_EVT_CANCEL || conn_stop == 1) {
			conn_stop = 1;
			if (state == BLE_AUTOCON_STATE_SCAN_STARTING) {
				state = BLE_AUTOCON_STATE_SCAN_STOPPING;
				ret = ble_client_stop_scan();
				if (ret != BLE_MANAGER_SUCCESS) {
					BLE_LOG_ERROR("[BLEMGR] auto conn scan stop fail[%d]\n", ret);
					goto finish_auto;
				}
			} else if (state == BLE_AUTOCON_STATE_SCAN_STOPPING) {
				if (evt == BLE_AUTOCON_EVT_SCAN_STOP) {
					state = BLE_AUTOCON_STATE_DONE;
				}
			} else if (state == BLE_AUTOCON_STATE_CONNECTING) {
				if (evt == BLE_AUTOCON_EVT_CONNECTED) {
					state = BLE_AUTOCON_STATE_CONNECTED;
					ctx->auto_connect = false;
					break;
				} else if (evt == BLE_AUTOCON_EVT_DISCONNECTED) {
					state = BLE_AUTOCON_STATE_DONE;
				}
			} else {
				state = BLE_AUTOCON_STATE_DONE;
			}

			if (state == BLE_AUTOCON_STATE_DONE) {
				ctx->state = BLE_CLIENT_IDLE;
				ctx->auto_connect = false;
				break;
			}

		/* This is normal process */
		} else if (state == BLE_AUTOCON_STATE_DISCONNECT) {
			if (evt == BLE_AUTOCON_EVT_SCAN_START) {
				state = BLE_AUTOCON_STATE_SCAN_STARTING;
			}
		} else if (state == BLE_AUTOCON_STATE_SCAN_STARTING) {
			if (evt == BLE_AUTOCON_EVT_MAC_SCANNED) {
				state = BLE_AUTOCON_STATE_SCAN_STOPPING;
				ret = ble_client_stop_scan();
				if (ret != BLE_MANAGER_SUCCESS) {
					BLE_LOG_ERROR("[BLEMGR] auto conn scan stop fail[%d]\n", ret);
					goto finish_auto;
				}
			}
		} else if (state == BLE_AUTOCON_STATE_SCAN_STOPPING) {
			if (evt == BLE_AUTOCON_EVT_SCAN_STOP) {
				state = BLE_AUTOCON_STATE_CONNECTING;
				ret = ble_client_reconnect((ble_client_ctx *)ctx);
				if (ret != BLE_MANAGER_SUCCESS) {
					BLE_LOG_ERROR("[BLEMGR] auto conn reconnect fail[%d]\n", ret);
					goto finish_auto;
				}
			}
		} else if (state == BLE_AUTOCON_STATE_CONNECTING) {
			if (evt == BLE_AUTOCON_EVT_CONNECTED) {
				state = BLE_AUTOCON_STATE_CONNECTED;
				break;
			} else if (evt == BLE_AUTOCON_EVT_DISCONNECTED) {
				state = BLE_AUTOCON_STATE_DISCONNECT;
			}
		}
		BLE_LOG_DEBUG("[BLEMGR] auto conn state : %d\n", state);
	}
finish_auto:
	mq_close(ctx->mqfd);
	ctx->mqfd = 0;
	mq_unlink(mq_name);
	g_ctx = NULL;

	BLE_LOG_INFO("[BLEMGR] Auto Connection Done\n");

	return NULL;
}

ble_autocon_result_e ble_manager_autoconnect(ble_client_ctx_internal *ctx)
{
	if (ctx->state != BLE_CLIENT_AUTOCONNECTING) {
		return BLE_AUTOCON_INVALID_STATE;
	}

	int status;
	pthread_t auto_thread = 0;

	/* create pthread with entry function */
	status = pthread_create(&auto_thread, NULL, _autocon_process, (pthread_addr_t)ctx);
	if (status != 0) {
		BLE_LOG_ERROR("[BLEMGR] pthread_create failed, status=%d\n", status);
		return BLE_AUTOCON_FAIL;
	}

	status = pthread_setname_np(auto_thread, "ble_autoconnect");
	if (status != 0) {
		BLE_LOG_ERROR("[BLEMGR] failed to set name for pthread, status=%d, errno=%d\n", status, errno);
		return BLE_AUTOCON_FAIL;
	}
	pthread_detach(auto_thread);

	return BLE_AUTOCON_SUCCESS;
}
