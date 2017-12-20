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

struct light_resource {
	OCResourceHandle handle;
	bool value;
	char *uri;
	int gpio;
};

static struct light_resource _light = {
	.handle = NULL,
	.value = false,
	.uri = "/light",
	.gpio = 45
};

static void gpio_write(int port, int value)
{
	char tmp[20];
	int fd;

	snprintf(tmp, 16, "/dev/gpio%d", port);
	fd = open(tmp, O_RDWR);
	if (fd < 0) {
		DBG("open(%s) failed.", tmp);
		return;
	}

	MSG("GPIO: '%s' value '%d'", tmp, value);

	ioctl(fd, GPIOIOC_SET_DIRECTION, GPIO_DIRECTION_OUT);

	snprintf(tmp, 2, "%1d", value);
	write(fd, tmp, 2);

	close(fd);
}

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
	OCRepPayloadAddResourceType(payload, "oic.r.switch.binary");
	OCRepPayloadAddInterface(payload, "oic.if.baseline");
	OCRepPayloadAddInterface(payload, "oic.if.a");

	OCRepPayloadSetPropBool(payload, "value", _light.value);

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
	bool value;
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
	if (OCRepPayloadGetPropBool(payload, "value", &value))
		ocf_mylight_light_set_status(value);

	payload = OCRepPayloadCreate();
	OCRepPayloadAddResourceType(payload, "oic.r.switch.binary");
	OCRepPayloadAddInterface(payload, "oic.if.baseline");
	OCRepPayloadAddInterface(payload, "oic.if.a");

	OCRepPayloadSetPropBool(payload, "value", _light.value);

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

	ocf_mylight_notify_add(0, req->obsInfo.obsId);

	return OC_EH_OK;
}

static OCEntityHandlerResult on_deregister_observe(
		OCEntityHandlerFlag flag _UNUSED_, OCEntityHandlerRequest *req,
		void *user_data _UNUSED_)
{
	DBG("De-registration request for observation Id %d",
			req->obsInfo.obsId);

	ocf_mylight_notify_del(0, req->obsInfo.obsId);

	return OC_EH_OK;
}

static struct ocf_ops light_ops = {
	.get = on_get,
	.put = on_put_post,
	.post = on_put_post,
	.del = on_del,
	.register_observe = on_register_observe,
	.deregister_observe = on_deregister_observe
};

int ocf_mylight_light_get_handle(OCResourceHandle *handle)
{
	if (!handle)
		return -1;

	*handle = _light.handle;

	return 0;
}

const char *ocf_mylight_light_peek_uri(void)
{
	return _light.uri;
}

int ocf_mylight_light_set_status(bool status)
{
	if (_light.value == status)
		return 0;

	_light.value = status;

	DBG("Light value changed to %d", status);

	gpio_write(_light.gpio, _light.value);

	ocf_mylight_notify_emit(0);

	return 0;
}

int ocf_mylight_light_get_status(bool *status)
{
	if (!status)
		return -1;

	*status = _light.value;

	return 0;
}

int ocf_mylight_light_init(void)
{
	OCStackResult ret;

	ret = OCCreateResource(&(_light.handle),
			"oic.r.switch.binary", "oic.if.a",
			_light.uri, ocf_mylight_handler, &light_ops,
			OC_DISCOVERABLE | OC_OBSERVABLE | OC_SECURE);
	if (ret != OC_STACK_OK) {
		DBG("OCCreateResource() failed. (ret=%d)", ret);
		return -1;
	}

	MSG("Light resource created.");
	MSG(" - resource: 0x%p", _light.handle);
	MSG(" - uri: '%s'", _light.uri);

	return 0;
}

void ocf_mylight_light_exit(void)
{
}
