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
#include <tinyara/config.h>
#include <tinyara/gpio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "ocf_mylight.h"
#include <deviceonboardingstate.h>

struct dimmresource {
	OCResourceHandle handle;
	char *uri;
	int64_t dimmingSetting;
};

static struct dimmresource _dimm = {
	.handle = NULL,
	.uri = "/dimm",
	.dimmingSetting = 0
};

static OCEntityHandlerResult on_get(OCEntityHandlerFlag flag _UNUSED_,
		OCEntityHandlerRequest *req, void *user_data _UNUSED_)
{
	OCEntityHandlerResponse resp;
	OCRepPayload *payload = NULL;
	OicSecDostype_t dos;

	if (GetDos(&dos) != OC_STACK_OK) {
		DBG("GetDos() failed.");
		return OC_EH_ERROR;
	}

	if (dos.state != DOS_RFNOP) {
		MSG("DOS(0x%X) is not Ready-For-Normal-Operation", dos.state);
		return OC_EH_ERROR;
	}

	MSG("received GET request");

	payload = OCRepPayloadCreate();
	OCRepPayloadAddResourceType(payload, "oic.r.light.dimming");
	OCRepPayloadAddInterface(payload, "oic.if.baseline");
	OCRepPayloadAddInterface(payload, "oic.if.a");

	OCRepPayloadSetPropInt(payload, "dimmingSetting", _dimm.dimmingSetting);

	memset(&resp, 0, sizeof(OCEntityHandlerResponse));
	resp.requestHandle = req->requestHandle;
	resp.resourceHandle = req->resource;
	resp.ehResult = OC_EH_OK;
	resp.payload = (OCPayload *) payload;

	ocf_mylight_verbose_response(&resp);

	if (OCDoResponse(&resp) != OC_STACK_OK) {
		DBG("Error sending response");
		OCRepPayloadDestroy(payload);
		return OC_EH_ERROR;
	}

	OCRepPayloadDestroy(payload);

	return OC_EH_OK;
}

static OCEntityHandlerResult on_put_post(OCEntityHandlerFlag flag _UNUSED_,
		OCEntityHandlerRequest *req, void *user_data _UNUSED_)
{
	OCEntityHandlerResponse resp;
	OCRepPayload *payload = NULL;
	int64_t dimmingSetting;
	OicSecDostype_t dos;

	if (GetDos(&dos) != OC_STACK_OK) {
		DBG("GetDos() failed.");
		return OC_EH_ERROR;
	}

	if (dos.state != DOS_RFNOP) {
		MSG("DOS(0x%X) is not Ready-For-Normal-Operation", dos.state);
		return OC_EH_ERROR;
	}

	if (req->method == OC_REST_PUT)
		MSG("received PUT request");
	else if (req->method == OC_REST_POST)
		MSG("received POST request");

	payload = (OCRepPayload *) req->payload;
	if (OCRepPayloadGetPropInt(payload, "dimmingSetting", &dimmingSetting))
		ocf_mylight_dimm_set_level(dimmingSetting);

	payload = OCRepPayloadCreate();
	OCRepPayloadAddResourceType(payload, "oic.r.light.dimming");
	OCRepPayloadAddInterface(payload, "oic.if.baseline");
	OCRepPayloadAddInterface(payload, "oic.if.a");

	OCRepPayloadSetPropInt(payload, "dimmingSetting", _dimm.dimmingSetting);

	memset(&resp, 0, sizeof(OCEntityHandlerResponse));
	resp.requestHandle = req->requestHandle;
	resp.resourceHandle = req->resource;
	resp.ehResult = OC_EH_OK;
	resp.payload = (OCPayload *) payload;

	ocf_mylight_verbose_response(&resp);

	if (OCDoResponse(&resp) != OC_STACK_OK) {
		DBG("Sending response failed.");
		OCRepPayloadDestroy(payload);
		return OC_EH_ERROR;
	}

	OCRepPayloadDestroy(payload);

	return OC_EH_OK;
}

static OCEntityHandlerResult on_del(OCEntityHandlerFlag flag _UNUSED_,
		OCEntityHandlerRequest *req _UNUSED_, void *user_data _UNUSED_)
{
	DBG("Forbidden");

	return OC_EH_FORBIDDEN;
}

static OCEntityHandlerResult on_register_observe(
		OCEntityHandlerFlag flag _UNUSED_, OCEntityHandlerRequest *req,
		void *user_data _UNUSED_)
{
	DBG("Registration request with observation Id %d", req->obsInfo.obsId);

	ocf_mylight_notify_add(1, req->obsInfo.obsId);

	return OC_EH_OK;
}

static OCEntityHandlerResult on_deregister_observe(
		OCEntityHandlerFlag flag _UNUSED_, OCEntityHandlerRequest *req,
		void *user_data _UNUSED_)
{
	DBG("De-registration request for observation Id %d",
			req->obsInfo.obsId);

	ocf_mylight_notify_del(1, req->obsInfo.obsId);

	return OC_EH_OK;
}

static struct ocf_ops dimm_ops = {
	.get = on_get,
	.put = on_put_post,
	.post = on_put_post,
	.del = on_del,
	.register_observe = on_register_observe,
	.deregister_observe = on_deregister_observe
};

int ocf_mylight_dimm_get_handle(OCResourceHandle *handle)
{
	if (!handle)
		return -1;

	*handle = _dimm.handle;

	return 0;
}

const char *ocf_mylight_dimm_peek_uri(void)
{
	return _dimm.uri;
}

int ocf_mylight_dimm_set_level(int64_t value)
{
	if (_dimm.dimmingSetting == value)
		return 0;

	_dimm.dimmingSetting = value;

	DBG("Dimming value changed to %ld", value);

	ocf_mylight_notify_emit(1);

	return 0;
}

int ocf_mylight_dimm_get_level(int64_t *value)
{
	if (!value)
		return -1;

	*value = _dimm.dimmingSetting;

	return 0;
}

int ocf_mylight_dimm_init(void)
{
	OCStackResult ret;

	ret = OCCreateResource(&(_dimm.handle),
			"oic.r.light.dimming", "oic.if.a",
			_dimm.uri, ocf_mylight_handler, &dimm_ops,
			OC_DISCOVERABLE | OC_OBSERVABLE | OC_SECURE);
	if (ret != OC_STACK_OK) {
		DBG("OCCreateResource() failed. (ret=%d)", ret);
		return -1;
	}

	MSG("Dimming resource created.");
	MSG(" - resource: 0x%p", _dimm.handle);
	MSG(" - uri: '%s'", _dimm.uri);

	return 0;
}

void ocf_mylight_dimm_exit(void)
{
}
