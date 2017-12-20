/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "ocf_mylight.h"
#include <doxmresource.h>
#include <srmutility.h>

#define SPEC_VERSION "ocf.1.0.0"
#define DM_VERSION "ocf.res.1.3.0,ocf.sh.1.3.0"
#define DEV_TYPE "oic.d.light"

/**
 * Protocol Independent Unique Identifier for device (UUID)
 *
 * A unique and immutable Device identifier. A Client can detect that a
 * single Device supports multiple communication protocols if it discovers
 * that the Device uses a single Protocol Independent ID value for all the
 * protocols it supports.
 */
#define PIID "C0FFEE00-BAB0-FACE-B00C-000000000000"

static OCEntityHandlerResult on_get(OCEntityHandlerFlag flag _UNUSED_,
		OCEntityHandlerRequest *req _UNUSED_, char *uri _UNUSED_,
		void *user_data _UNUSED_)
{
	DBG("dev on_get");

	return OC_EH_OK;
}

static OCEntityHandlerResult on_put_post(OCEntityHandlerFlag flag _UNUSED_,
		OCEntityHandlerRequest *req, char *uri _UNUSED_,
		void *user_data _UNUSED_)
{
	if (req->method == OC_REST_PUT)
		DBG("dev on_put");
	else if (req->method == OC_REST_POST)
		DBG("dev on_post");

	/* All properties are READONLY */

	return OC_EH_FORBIDDEN;
}

static OCEntityHandlerResult on_del(OCEntityHandlerFlag flag _UNUSED_,
		OCEntityHandlerRequest *req _UNUSED_, char *uri _UNUSED_,
		void *user_data _UNUSED_)
{
	DBG("dev on_del");

	/* All properties are READONLY */

	return OC_EH_FORBIDDEN;
}

static void _display_default_device_id(void)
{
	OicUuid_t uuid;
	char *device_id = NULL;
	int ret;
	int i;

	memset(&uuid, 0, sizeof(OicUuid_t));
	ret = GetDoxmDeviceID(&uuid);
	if (ret != OC_STACK_OK) {
		DBG("GetDoxmDeviceID() failed");
		return;
	}

	for (i = 0; i < UUID_LENGTH; i++)
		printf("%02X ", uuid.id[i]);
	printf("\n");

	ret = ConvertUuidToStr(&uuid, &device_id);
	if (ret != OC_STACK_OK) {
		DBG("ConvertUuidToStr() failed");
		return;
	}

	DBG("default device_id = %s", device_id);

	if (device_id)
		free(device_id);
}

struct ocf_dev_ops dev_ops = {
	.get = on_get,
	.put = on_put_post,
	.post = on_put_post,
	.del = on_del
};

int ocf_mylight_device_init(void)
{
	OCResourceHandle handle;

	OCSetDefaultDeviceEntityHandler(ocf_mylight_dev_handler, &dev_ops);

	handle = OCGetResourceHandleAtUri(OC_RSRVD_DEVICE_URI);
	if (!handle) {
		DBG("OCGetResourceHandleAtUri() failed!");
		return -1;
	}

	_display_default_device_id();

#ifdef STATIC_DEVICE_UUID
	OCSetDeviceId(ocf_mylight_setup_get_uuid());
#endif

	OCBindResourceTypeToResource(handle, DEV_TYPE);

	/*
	 * Mandatory fields (n)
	 * 'n' can be changed by 'POST' /oic/con/ oic.wk.con {'n':'xxx'} request
	 */
	OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_DEVICE_NAME,
			ocf_mylight_setup_get_name());

	/* Mandatory READONLY fields (n, icv, dmv, piid) */
	DBG("ID = %s", OCGetServerInstanceIDString());
	OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_SPEC_VERSION,
			SPEC_VERSION);
	OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_DATA_MODEL_VERSION,
			DM_VERSION);
	OCSetPropertyValue(PAYLOAD_TYPE_DEVICE,
			OC_RSRVD_PROTOCOL_INDEPENDENT_ID, PIID);

	printf("type: %s\n", DEV_TYPE);
	printf("version: %s\n", SPEC_VERSION);
	printf("dmv: %s\n", DM_VERSION);

	return 0;
}
