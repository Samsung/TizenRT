/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
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
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <tinyara/fs/ioctl.h>
#include <wifi_manager/wifi_manager.h>

#define QT_DEVPATH "/dev/quicktrack"

static void _qt_sta_connected(wifi_manager_cb_msg_s msg, void *arg)
{
	(void)msg;
	(void)arg;
}

static void _qt_sta_disconnected(wifi_manager_cb_msg_s msg, void *arg)
{
	(void)msg;
	(void)arg;
}

static void _qt_softap_sta_joined(wifi_manager_cb_msg_s msg, void *arg)
{
	(void)msg;
	(void)arg;
}

static void _qt_softap_sta_left(wifi_manager_cb_msg_s msg, void *arg)
{
	(void)msg;
	(void)arg;
}

static void _qt_scan_done(wifi_manager_cb_msg_s msg, void *arg)
{
	(void)msg;
	(void)arg;
}

static wifi_manager_cb_s g_qt_wifi_callbacks = {
	_qt_sta_connected,
	_qt_sta_disconnected,
	_qt_softap_sta_joined,
	_qt_softap_sta_left,
	_qt_scan_done,
};

static int _qt_wifi_bringup(void)
{
	wifi_manager_info_s info;
	wifi_manager_result_e res;

	memset(&info, 0, sizeof(info));
	info.mode = WIFI_NONE;
	res = wifi_manager_get_info(&info);
	if (res == WIFI_MANAGER_SUCCESS &&
		info.mode != WIFI_NONE &&
		info.mode != WIFI_FAILURE &&
		info.mode != WIFI_INITIALIZING &&
		info.mode != WIFI_DEINITIALIZING) {
		printf("[QT] wifi manager already up (mode=%d)\n", info.mode);
		return 0;
	}

	res = wifi_manager_init(&g_qt_wifi_callbacks);
	if (res != WIFI_MANAGER_SUCCESS) {
		printf("[QT] wifi_manager_init failed (%d)\n", res);
		return -1;
	}

	printf("[QT] wifi_manager_init ok\n");
	return 0;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int quicktrack_test_main(int argc, char *argv[])
#endif
{
	if (_qt_wifi_bringup() != 0) {
		return -1;
	}

	printf("[QT] starting controlappc (uart port %d)\n", CONFIG_QUICK_TRACK_UART_PORT);

	int fd = open(QT_DEVPATH, O_RDONLY);
	if (fd < 0) {
		printf("[QT] open %s failed (%d)\n", QT_DEVPATH, errno);
		return -1;
	}

	int ret = ioctl(fd, QTIOC_START, 0);
	close(fd);

	if (ret < 0) {
		printf("[QT] QTIOC_START failed (%d)\n", errno);
		return -1;
	}

	printf("[QT] controlappc started\n");
	return 0;
}
