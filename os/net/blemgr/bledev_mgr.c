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
#include <stdlib.h>
#include <stdint.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <tinyara/kmalloc.h>
#include <tinyara/lwnl/lwnl.h>
#include <tinyara/net/if/ble.h>
#include <tinyara/ble/ble_manager.h>

#ifndef CONFIG_BLEDEV_NUM
#define CONFIG_BLEDEV_NUM 1
#endif

#define BLEDEV_MGR_BLE_NAME "ble"
#define TAG "[BLEMGR]"

static struct bledev g_bledev[CONFIG_BLEDEV_NUM] = {
	0,
};
static int g_bledev_idx = 0;

/*
 * external function
 */

struct bledev *net_get_bledev(uint8_t *ifname)
{
	for (int i = 0; i < g_bledev_idx; i++) {
		if (!strncmp((const char *)ifname, (const char *)g_bledev[i].ifname, strlen(g_bledev[i].ifname) + 1)) {
			return &g_bledev[i];
		}
	}
	return NULL;
}

struct bledev *bledev_register(struct trble_ops *ops)
{
	if (g_bledev_idx == CONFIG_BLEDEV_NUM) {
		ndbg("No available bledev slot %d %d\n", g_bledev_idx, CONFIG_BLEDEV_NUM);
		return NULL;
	}

	struct bledev *dev = &g_bledev[g_bledev_idx];

	char name[BLE_IF_NAME] = {
		0,
	};
	snprintf(name, BLE_IF_NAME, "%s%d", BLEDEV_MGR_BLE_NAME, g_bledev_idx);
	strncpy(dev->ifname, name, BLE_IF_NAME);
	dev->ops = ops;
	dev->priv = NULL;

	g_bledev_idx++;

	return dev;
}
