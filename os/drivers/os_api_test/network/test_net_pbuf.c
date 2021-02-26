/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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

#include <debug.h>
#include <string.h>
#include <tinyara/os_api_test_drv.h>
#include <lwip/pbuf.h>
#include <tinyara/kmalloc.h>
#include <sys/types.h>

typedef struct pbuf_test_args pbuf_test_args_t;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int test_pbuf(unsigned long arg)
{
	int ret = 0;
	struct pbuf* p = NULL;
	pbuf_test_args_t *args = (pbuf_test_args_t *)arg;

	p = pbuf_alloc(args->layer, args->len, args->type);
	if (p) {
		ret = 1;
	}

	pbuf_free(p);
	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int test_net_pbuf(int cmd, unsigned long arg)
{
	int ret = -1;

	switch (cmd) {
	case TESTIOC_NET_PBUF:
		ret = test_pbuf(arg);
		break;
	}
	return ret;
}
