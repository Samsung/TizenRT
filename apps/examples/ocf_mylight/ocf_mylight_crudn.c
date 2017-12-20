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
#include <time.h>

#include "ocf_mylight.h"

static void _timestamp(void)
{
	struct tm stamp;
	struct timespec tspec;
	char timebuf[32];

	clock_gettime(CLOCK_REALTIME, &tspec);
	localtime_r(&tspec.tv_sec, &stamp);

	strftime(timebuf, sizeof(timebuf), "%m-%d %H:%M:%S", &stamp);
	printf("%s.%03ld", timebuf, tspec.tv_nsec / 1000000);
}

OCEntityHandlerResult ocf_mylight_handler(OCEntityHandlerFlag flag,
		OCEntityHandlerRequest *req, void *user_data)
{
	struct ocf_ops *ops = user_data;
	OCEntityHandlerResult ret = OC_EH_METHOD_NOT_ALLOWED;

	printf("\n");
	_timestamp();
	MSG("\n<New request>");
	ocf_mylight_verbose_request(flag, req);

	if (!ops)
		return ret;

	if (flag & OC_REQUEST_FLAG) {
		if (req->method == OC_REST_GET && ops->get)
			ret = ops->get(flag, req, user_data);
		else if (req->method == OC_REST_PUT && ops->put)
			ret = ops->put(flag, req, user_data);
		else if (req->method == OC_REST_POST && ops->post)
			ret = ops->post(flag, req, user_data);
		else if (req->method == OC_REST_DELETE && ops->del)
			ret = ops->del(flag, req, user_data);
	}

	if (flag & OC_OBSERVE_FLAG) {
		if (req->obsInfo.action == OC_OBSERVE_REGISTER
				&& ops->register_observe)
			ret = ops->register_observe(flag, req, user_data);
		else if (req->obsInfo.action == OC_OBSERVE_DEREGISTER
				&& ops->deregister_observe)
			ret = ops->deregister_observe(flag, req, user_data);
	}

	MSG("  return code: %d", ret);
	MSG("</New request>\n");

	return ret;
}

OCEntityHandlerResult ocf_mylight_dev_handler(OCEntityHandlerFlag flag,
		OCEntityHandlerRequest *req, char *uri, void *user_data)
{
	struct ocf_dev_ops *ops = user_data;
	OCEntityHandlerResult ret = OC_EH_METHOD_NOT_ALLOWED;

	printf("\n");
	_timestamp();
	MSG("\n<New request for device>");
	ocf_mylight_verbose_request(flag, req);
	MSG("  - uri: %s", uri);

	if (!ops)
		return ret;

	if (uri) {
		if (strcmp(uri, "/oic/d") != 0) {
			DBG("invalid url");
			return OC_EH_RESOURCE_NOT_FOUND;
		}
	}

	if (flag & OC_REQUEST_FLAG) {
		if (req->method == OC_REST_GET && ops->get)
			ret = ops->get(flag, req, uri, user_data);
		else if (req->method == OC_REST_PUT && ops->put)
			ret = ops->put(flag, req, uri, user_data);
		else if (req->method == OC_REST_DELETE && ops->del)
			ret = ops->del(flag, req, uri, user_data);
	}

	MSG("  return code: %d", ret);
	MSG("</New request for device>\n");

	return ret;
}

