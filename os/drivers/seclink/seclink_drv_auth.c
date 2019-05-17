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
#define SLDRV_TAG "[SECLINK_DRV_AUTH]"

int hd_handle_auth_request(int cmd, unsigned long arg, void *lower)
{
	SLDRV_ENTER;

	struct seclink_req *req = (struct seclink_req *)arg;
	if (!req) {
		return -EINVAL;
	}

	struct seclink_auth_info *info = req->req_type.auth;
	if (!info) {
		return -EINVAL;
	}

	struct sec_lowerhalf_s *se = (struct sec_lowerhalf_s *)lower;
	if (!se || !(se->ops)) {
		return -EINVAL;
	}

	switch (cmd) {
	case SECLINKIOC_GENERATERANDOM:
		req->res = se->ops->generate_random(info->auth_type.random_len, info->data);
		break;
	case SECLINKIOC_GETHASH:
		req->res = se->ops->get_hash(info->auth_type.hash_type, info->data, info->auth_data.data);
		break;
	case SECLINKIOC_GETHMAC:
		req->res = se->ops->get_hmac(info->auth_type.hmac_type, info->data, info->key_idx, info->auth_data.data);
		break;
	case SECLINKIOC_RSASIGNMD:
		req->res = se->ops->rsa_sign_md(info->auth_type.rsa_type, info->data, info->key_idx, info->auth_data.data);
		break;
	case SECLINKIOC_RSAVERIFYMD:
		req->res = se->ops->rsa_verify_md(info->auth_type.rsa_type, info->data, info->auth_data.data, info->key_idx);
		break;
	case SECLINKIOC_ECDSASIGNMD:
		req->res = se->ops->ecdsa_sign_md(info->auth_type.ecdsa_type, info->data, info->key_idx, info->auth_data.data);
		break;
	case SECLINKIOC_ECDSAVERIFYMD:
		req->res = se->ops->ecdsa_verify_md(info->auth_type.ecdsa_type, info->data, info->auth_data.data, info->key_idx);
		break;
	case SECLINKIOC_DHGENERATEPARAM:
		req->res = se->ops->dh_generate_param(info->key_idx, info->auth_data.dh_param);
		break;
	case SECLINKIOC_DHCOMPUTESHAREDSECRET:
		req->res = se->ops->dh_compute_shared_secret(info->auth_data.dh_param, info->key_idx, info->data);
		break;
	case SECLINKIOC_ECDHCOMPUTESHAREDSECRET:
		req->res = se->ops->ecdh_compute_shared_secret(info->auth_data.ecdh_param, info->key_idx, info->data);
		break;
	case SECLINKIOC_SETCERTIFICATE:
		req->res = se->ops->set_certificate(info->key_idx, info->data);
		break;
	case SECLINKIOC_GETCERTIFICATE:
		req->res = se->ops->get_certificate(info->key_idx, info->data);
		break;
	case SECLINKIOC_REMOVECERTIFICATE:
		req->res = se->ops->remove_certificate(info->key_idx);
		break;
	default:
		SLDRV_LOG("Invalid command error\n");
	}
	return 0;
}
