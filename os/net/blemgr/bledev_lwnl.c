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
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <tinyara/lwnl/lwnl.h>
#include <tinyara/net/if/ble.h>
#include <tinyara/ble/ble_manager.h>

extern int bledev_handle(struct bledev *dev, lwnl_req cmd, void *data, uint32_t data_len);

int bledev_req_handle(const char *msg, size_t msg_len)
{
	lwnl_msg *lmsg = (lwnl_msg *)msg;
	struct bledev *dev = NULL;

	dev = (struct bledev *)net_get_bledev(lmsg->name);
	if (!dev) {
		return -ENOSYS;
	}

	trble_result_e *res = (trble_result_e *)lmsg->result;
	*res = bledev_handle(dev, lmsg->req_type, lmsg->data, lmsg->data_len);

	return 0;
}
