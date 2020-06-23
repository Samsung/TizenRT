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

#include <stdio.h>
#include <errno.h>
#include <tinyara/seclink.h>
#include <tinyara/seclink_drv.h>
#include "seclink_drv_req.h"
#include "seclink_drv_utils.h"

/*  Debugging */
#ifdef SLDRV_TAG
#undef SLDRV_TAG
#endif
#define SLDRV_TAG "[SECLINK_DRV_COM]"


int hd_handle_common_request(int cmd, unsigned long arg, void *lower)
{
	SLDRV_ENTER;

	hal_init_param *params = NULL;
	struct seclink_req *req = (struct seclink_req *)arg;
	if (!req) {
		return -EINVAL;
	}

#ifdef CONFIG_ARM_TRUSTZONE
	struct seclink_comm_info *info = (struct seclink_comm_info *)req->req_type.comm;
	if (!info && (cmd == SECLINKIOC_ALLOC || cmd == SECLINKIOC_FREE)) {
		return -EINVAL;
	}
#endif
	struct sec_lowerhalf_s *se = (struct sec_lowerhalf_s *)lower;
	if (!se | !(se->ops)) {
		return -EINVAL;
	}

	switch (cmd) {
	case SECLINKIOC_INIT:
		params = (hal_init_param *)req->params;
		req->res = se->ops->init(params);
		break;
	case SECLINKIOC_DEINIT:
		req->res = se->ops->deinit();
		break;
#ifdef CONFIG_ARM_TRUSTZONE
	case SECLINKIOC_ALLOC:
		if (se->ops->allocate_scontext != NULL) {
			req->res = se->ops->allocate_scontext(info->s_size);
		} else {
			req->res = -ENOSYS;
		}
		break;
	case SECLINKIOC_FREE:
		if (se->ops->free_scontext != NULL) {
			req->res = se->ops->free_scontext(info->s_context);
		} else {
			req->res = -ENOSYS;
		}
		break;
#endif
	}

	return 0;
}
