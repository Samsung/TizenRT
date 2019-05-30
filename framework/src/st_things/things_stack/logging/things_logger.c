/* ****************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include <unistd.h>

// if we have unistd.h, we're a Unix system
#include <time.h>
#include <sys/time.h>
#include <debug.h>

#include "things_logger.h"
#include "utils/things_malloc.h"
#include "string.h"

#include <sys/types.h>
#include <pthread.h>

#include "ocpayload.h"
#include "ocstack.h"

static const char *LEVEL[] __attribute__((unused)) = {
	"DEBUG", "INFOR", "WARNING", "ERROR", "FATAL"
};

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

char *p_version = NULL;

void things_log_init(void)
{
	/*
	 * Initialize p_version variable
	 */
	p_version = NULL;
}

void things_log_shutdown(void)
{
	/*
	 * Free the memory allocated for p_version
	 */
	things_free(p_version);
}

void things_log_set_version(char *version)
{
	if (NULL != p_version) {
		things_free(p_version);
	}

	/*
	 * Allocate memory for p_version and set the version information
	 */
	p_version = (char *)things_malloc(strlen(version) + 1);
	if (NULL == p_version) {
		dbg("Error while allocating memory for version");
		return;
	}
	memset(p_version, 0, strlen(version) + 1);
	things_strncpy(p_version, version, strlen(version));
}

/**
 * Output a log string with the specified priority level.
 * Only defined for Linux and Android
 *
 * @param level  - DEBUG, INFO, WARNING, ERROR, FATAL
 * @param tag    - Module name
 * @param logStr - log string
 */
void things_log(things_log_level_e level, const char *tag, const char *func_name, const int16_t line_num, const char *logStr)
{
	struct timeval tv;
	time_t nowtime = 0;
	struct tm *nowtm = NULL;
	char buf[64] = { 0 };

	gettimeofday(&tv, NULL);
	nowtime = tv.tv_sec;
	nowtm = localtime(&nowtime);
	strftime(buf, sizeof buf, "%m/%d %H:%M:%S", nowtm);

	/* If Version information is not NULL, display the Version Information
	 * Logger Format is given below
	 * [<MM/YY H:M:S.uS> <LOGGER_VERSION>] <LOG_LEVEL> <TAG> <API> <LINE_NO> <LOG_STR>
	 */
	pthread_mutex_lock(&lock);
	if (p_version) {
		dbg("[%s %s] %s %s %4d %s %s\n", buf, p_version, LEVEL[level], tag, line_num, func_name, logStr);
	} else {
		dbg("T%d [%s] %s %s %4d %s %s\n", getpid(), buf, LEVEL[level], tag, line_num, func_name, logStr);
	}
	pthread_mutex_unlock(&lock);
}

/**
 * Output a variable argument list log string with the specified priority level.
 * Only defined for Linux and Android
 *
 * @param level  - DEBUG, INFO, WARNING, ERROR, FATAL
 * @param tag    - Module name
 * @param format - variadic log string
 */
void things_logv(things_log_level_e level, const char *tag, const char *func_name, const int16_t line_num, const char *format, ...)
{
	if (!format || !tag) {
		return;
	}

	char buffer[MAX_LOG_V_BUFFER_SIZE] = { 0 };
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, sizeof buffer - 1, format, args);
	va_end(args);
	things_log(level, tag, func_name, line_num, buffer);
}

struct num_str {
	int number;
	char *str;
};

/**
 * CoAP Option Numbers
 * http://www.iana.org/assignments/core-parameters/core-parameters.xhtml
 */
static struct num_str g_coap_options[] = {
	{ 0, "Reserved" },
	{ 1, "If-Match" },
	{ 3, "Uri-Host" },
	{ 4, "ETag" },
	{ 5, "If-None-Match" },
	{ 6, "Observe" },
	{ 7, "Uri-Port" },
	{ 8, "Location-Path" },
	{ 11, "Uri-Path" },
	{ 12, "Content-Format" },
	{ 14, "Max-Age" },
	{ 15, "Uri-Query" },
	{ 17, "Accept" },
	{ 20, "Location-Query" },
	{ 23, "Block2" },
	{ 27, "Block1" },
	{ 28, "Size2" },
	{ 35, "Proxy-Uri" },
	{ 39, "Proxy-Scheme" },
	{ 60, "Size1" },
	{ 128, "Reserved" },
	{ 132, "Reserved" },
	{ 136, "Reserved" },
	{ 140, "Reserved" },
	{ 258, "No-Response" },
	{ 2049, "OCF-Accept-Content-Format-Version" },
	{ 2053, "OCF-Content-Format-Version" },
};

static const char *_coap_opt_name(int number)
{
	if (number < 0 || 2053 < number) {
		return "Unknown";
	}

	unsigned int i;

	for (i = 0; i < sizeof(g_coap_options) / sizeof(struct num_str); i++) {
		if (g_coap_options[i].number == number)
			return g_coap_options[i].str;
	}

	return "Unknown";
}

static void payload_ll(things_log_level_e level, const char *tag, const char *func_name, const int16_t line_num,
							OCStringLL *ll)
{
	OCStringLL *cur = ll;

	while (cur) {
		things_logv(level, tag, func_name, line_num, "\t\t  %s ", cur->value);
		cur = cur->next;
	}
}

static void payload_ll_rep_value(things_log_level_e level, const char *tag, const char *func_name, const int16_t line_num,
							OCRepPayloadValue *values)
{
	OCRepPayloadValue *cur = values;

	while (cur) {
		switch (cur->type) {
		case OCREP_PROP_INT:
			things_logv(level, tag, func_name, line_num, "\t  {\"%s\":  %zd} ", cur->name, cur->i);
			break;
		case OCREP_PROP_DOUBLE:
			things_logv(level, tag, func_name, line_num, "\t  {\"%s\":  %f} ", cur->name, cur->d);
			break;
		case OCREP_PROP_BOOL:
			things_logv(level, tag, func_name, line_num, "\t  {\"%s\":  %d} ", cur->name, cur->b);
			break;
		case OCREP_PROP_STRING:
			things_logv(level, tag, func_name, line_num, "\t  {\"%s\":  \"%s\"} ", cur->name, cur->str);
			break;
		case OCREP_PROP_BYTE_STRING:
			things_logv(level, tag, func_name, line_num, "\t  {\"%s\":  len=%zd [...]} ", cur->name, cur->ocByteStr.len);
			break;
		case OCREP_PROP_OBJECT:
			things_logv(level, tag, func_name, line_num, "\t  {\"%s\":  {0x%p}}", cur->name, cur->obj);
			break;
		case OCREP_PROP_ARRAY:
			things_logv(level, tag, func_name, line_num, "\t  {\"%s\":  [type=%d]}", cur->name, cur->arr.type);
			break;
		default:
			things_logv(level, tag, func_name, line_num, "\t  {\"%s\":  unknown} ");
			break;
		}

		cur = cur->next;
	}
}

static void things_log_payload_flag(things_log_level_e level, const char *tag, const char *func_name, const int16_t line_num,
							OCEntityHandlerFlag flag) {
	if (flag & OC_REQUEST_FLAG) {
		things_logv(level, tag, func_name, line_num, "\t\t\t  %s", "REQUEST ");
	}
	if (flag & OC_OBSERVE_FLAG) {
		things_logv(level, tag, func_name, line_num, "\t\t\t  %s", "OBSERVE ");
	}
}

static void things_log_payload_method(things_log_level_e level, const char *tag, const char *func_name, const int16_t line_num,
							OCMethod method) {
	if (method & OC_REST_GET) {
		things_logv(level, tag, func_name, line_num, "\t\t\t  %s", "GET ");
	}
	if (method & OC_REST_PUT) {
		things_logv(level, tag, func_name, line_num, "\t\t\t  %s", "PUT ");
	}
	if (method & OC_REST_POST) {
		things_logv(level, tag, func_name, line_num, "\t\t\t  %s", "POST ");
	}
	if (method & OC_REST_DELETE) {
		things_logv(level, tag, func_name, line_num, "\t\t\t  %s", "DELETE ");
	}
	if (method & OC_REST_OBSERVE) {
		things_logv(level, tag, func_name, line_num, "\t\t\t  %s", "OBSERVE ");
	}
	if (method & OC_REST_OBSERVE_ALL) {
		things_logv(level, tag, func_name, line_num, "\t\t\t  %s", "OBSERVE_ALL ");
	}
	if (method & OC_REST_PRESENCE) {
		things_logv(level, tag, func_name, line_num, "\t\t\t  %s", "PRESENCE ");
	}
	if (method & OC_REST_DISCOVER) {
		things_logv(level, tag, func_name, line_num, "\t\t\t  %s", "DISCOVERY ");
	}
}

static void things_log_payload_header_options(things_log_level_e level, const char *tag, const char *func_name, const int16_t line_num,
							OCHeaderOption *opt, uint8_t count)
{
	int i;
	int j;

	things_logv(level, tag, func_name, line_num, "  - vendor specific header count: %d", count);
	for (i = 0; i < count; i++) {
		things_logv(level, tag, func_name, line_num, "\t- [%d] ", i);

		if (opt[i].protocolID != 2) {
			things_logv(level, tag, func_name, line_num, "Invalid protocol(%d), option_id=0x%x, len=%d",
					opt[i].protocolID, opt[i].optionID,
					opt[i].optionLength);
			continue;
		}

		things_logv(level, tag, func_name, line_num, "CoAP option_id=0x%x (%s), option_len=%d",
				opt[i].optionID,
				_coap_opt_name(opt[i].optionID),
				opt[i].optionLength);

		if (opt[i].optionLength > 0) {
			things_logv(level, tag, func_name, line_num, "\t  Data: ");
			for (j = 0; j < opt[i].optionLength; j++)
				things_logv(level, tag, func_name, line_num, "%02X ", opt[i].optionData[j]);
		}
	}
}

static void things_log_payload_payload_representation(things_log_level_e level, const char *tag, const char *func_name, const int16_t line_num,
							OCRepPayload *payload)
{
	things_logv(level, tag, func_name, line_num, "  - payload-type: %d (%s)", payload->base.type,
			"Representation");
	things_logv(level, tag, func_name, line_num, "\t- uri: %s", payload->uri);

	if (payload->types) {
		things_logv(level, tag, func_name, line_num, "\t- types: ");
		payload_ll(level, tag, func_name, line_num, payload->types);
	}

	if (payload->interfaces) {
		things_logv(level, tag, func_name, line_num, "\t- interfaces: ");
		payload_ll(level, tag, func_name, line_num, payload->interfaces);
	}

	if (payload->values) {
		things_logv(level, tag, func_name, line_num, "\t- values: ");
		payload_ll_rep_value(level, tag, func_name, line_num, payload->values);
	}
}

void things_log_request(things_log_level_e level, const char *tag, const char *func_name, const int16_t line_num,
							OCEntityHandlerFlag flag, OCEntityHandlerRequest *req)
{
	if (!req) {
		return;
	}

	things_logv(level, tag, func_name, line_num, "IncomingRequest: (id: %d)", req->messageID);
	things_logv(level, tag, func_name, line_num, "  - resourceHandle: 0x%p", req->resource);
	things_logv(level, tag, func_name, line_num, "\t- uri: \"%s\"", OCGetResourceUri(req->resource));
	things_logv(level, tag, func_name, line_num, "\t- property: 0x%X",	OCGetResourceProperties(req->resource));
	things_logv(level, tag, func_name, line_num, "  - flag: 0x%X", flag);
	things_log_payload_flag(level, tag, func_name, line_num, flag);
	things_logv(level, tag, func_name, line_num, "  - method: %d", req->method);
	things_log_payload_method(level, tag, func_name, line_num, req->method);
	things_logv(level, tag, func_name, line_num, "  - query: '%s'", req->query);
	things_logv(level, tag, func_name, line_num, "  - dev_addr: adapter:%d, flags:0x%X, port:%d",
															req->devAddr.adapter, req->devAddr.flags,
															req->devAddr.port);

	if (req->numRcvdVendorSpecificHeaderOptions > 0) {
		things_log_payload_header_options(level, tag, func_name, line_num,
						req->rcvdVendorSpecificHeaderOptions,
						req->numRcvdVendorSpecificHeaderOptions);
	}

	if (req->payload) {
		if (req->payload->type == PAYLOAD_TYPE_REPRESENTATION) {
			things_log_payload_payload_representation(level, tag, func_name, line_num,
							(OCRepPayload *)req->payload);
		}
	}
}

void things_log_response(things_log_level_e level, const char *tag, const char *func_name, const int16_t line_num,
							OCEntityHandlerResponse *resp)
{
	things_logv(level, tag, func_name, line_num, "OutgoingResponse:");
	things_logv(level, tag, func_name, line_num, "  - resourceHandle: 0x%p", resp->resourceHandle);
	things_logv(level, tag, func_name, line_num, "  - result: %d", resp->ehResult);
	things_logv(level, tag, func_name, line_num, "  - resourceUri: '%s'", resp->resourceUri);
	things_logv(level, tag, func_name, line_num, "  - persistentBufferFlag: %d", resp->persistentBufferFlag);

	if (resp->numSendVendorSpecificHeaderOptions > 0) {
		things_log_payload_header_options(
						level, tag, func_name, line_num,
						resp->sendVendorSpecificHeaderOptions,
						resp->numSendVendorSpecificHeaderOptions);
	}

	if (resp->payload) {
		things_log_payload_payload_representation(level, tag, func_name, line_num,
							(OCRepPayload *)resp->payload);
	}
}