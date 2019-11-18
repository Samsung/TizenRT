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
#ifndef CONFIG_NET_NETMGR
#include <stdint.h>
#include <net/if.h>
#include <tinyara/lwnl/lwnl.h>

#define LWNL_NETDEV_MAX 3
static struct netdev *g_lwnl_dev[LWNL_NETDEV_MAX];
static int g_lwnl_dev_idx = 0;

void lwnl_initialize_dev(void)
{
	for (int i = 0; i < LWNL_NETDEV_MAX; i++) {
		g_lwnl_dev[i] = NULL;
	}

	g_lwnl_dev_idx = 0;
}

int lwnl_register_dev(struct netdev *dev)
{
	if (g_lwnl_dev_idx >= LWNL_NETDEV_MAX) {
		return -1;
	}
	g_lwnl_dev[g_lwnl_dev_idx++] = dev;
	return 0;
}

struct netdev *lwnl_get_dev(uint8_t *name)
{
	// g_lwnl_dev wouldn't changed while system is running.
	// so array protection doesn't be needed.
	for (int i = 0; i < g_lwnl_dev_idx; i++) {
		struct netdev *dev = g_lwnl_dev[i];
		if (name[0] == dev->ifname[0] && name[1] == dev->ifname[1]
			&& name[2] == dev->ifname[2]) {
			return dev;
		}
	}
	return NULL;
}

#endif //CONFIG_NET_NETMGR
