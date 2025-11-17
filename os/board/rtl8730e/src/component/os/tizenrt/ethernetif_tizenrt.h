/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/

#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__

#include "lwip/err.h"
#include "lwip/netif.h"
#include <tinyara/netmgr/netdev_mgr.h>

//----- ------------------------------------------------------------------
// Ethernet Buffer
//----- ------------------------------------------------------------------
struct eth_drv_sg {
	unsigned int buf;
	unsigned int len;
};

#define MAX_ETH_DRV_SG 32

err_t low_level_output(struct netdev *dev, uint8_t *data, uint16_t dlen);

#endif
