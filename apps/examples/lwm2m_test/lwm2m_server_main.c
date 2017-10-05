/*******************************************************************************
 *
 * Copyright (c) 2013, 2014 Intel Corporation and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * The Eclipse Distribution License is available at
 *    http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    David Navarro, Intel Corporation - initial API and implementation
 *    domedambrosio - Please refer to git log
 *    Simon Bernard - Please refer to git log
 *    Toby Jaffey - Please refer to git log
 *    Julien Vermillard - Please refer to git log
 *    Bosch Software Innovations GmbH - Please refer to git log
 *    Christian Renz - Please refer to git log
 *
 *******************************************************************************/

/*
 Copyright (c) 2013, 2014 Intel Corporation

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

     * Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
     * Neither the name of Intel Corporation nor the names of its contributors
       may be used to endorse or promote products derived from this software
       without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 THE POSSIBILITY OF SUCH DAMAGE.

 David Navarro <david.navarro@intel.com>

*/

#include "lwm2m_server_main.h"

/*
 * Definition for handling pthread
 */
#define LWM2M_SERVER_PRIORITY     100
#define LWM2M_SERVER_STACK_SIZE   16384
#define LWM2M_SERVER_SCHED_POLICY SCHED_RR

static int g_quit = 0;
static char g_coap_protocol[COAP_PROTOCOL_MAX][COAP_MAX_URI_PREFIX_SIZE] = {
	"UDP",
	"UDP+DTLS",
	"TCP",
	"TCP+TLS"
};

static int read_input_command_line(char *buf)
{
	char buffer[MAX_PACKET_SIZE] = {0,};

	int nbytes = 0;
	int pos = 0;
	int char_idx = 0;
	int bufsize = MAX_PACKET_SIZE;

	do {
		nbytes = read(STDIN_FILENO, &buffer[pos], (bufsize - pos));
		if (nbytes <= 0) {
			fprintf(stderr, "cannot read command\n");
			return 0;
		}

		for (char_idx = 0; char_idx < nbytes; char_idx++) {
			if ((buffer[pos] == ASCII_BS) || (buffer[pos] == ASCII_DEL)) {
				int valid_char_pos = pos + 1;

				if (pos > 0) {
					pos--;
					/* update screen */
					if (write(STDOUT_FILENO, "\b \b", 3) <= 0) {
						fprintf(stderr, "write failed (errno = %d)\n", get_errno());
					}
				}

				if (buffer[valid_char_pos] != 0x0 && (valid_char_pos < MAX_PACKET_SIZE)) {
					memcpy(&buffer[pos], &buffer[valid_char_pos], (bufsize - valid_char_pos));
				}
			} else {
				if (buffer[pos] == ASCII_CR) {
					buffer[pos] = ASCII_LF;
				}

				/* echo */
				if (write(STDOUT_FILENO, &buffer[pos], 1) <= 0) {
					fprintf(stderr, "failed to write (errno = %d)\n", get_errno());
				}

				if (buffer[pos] == ASCII_LF) {
					pos++;
					break;
				}

				pos++;
				if (pos >= MAX_PACKET_SIZE) {
					fprintf(stderr, "out of range : command is too long, maximum length %d\n", MAX_PACKET_SIZE);
					memset(buf, 0x0, MAX_PACKET_SIZE);
					return 0;
				}
			}
		}
	} while (buffer[pos - 1] != ASCII_LF);

	memcpy(buf, buffer, pos);

	return pos;
}

static void prv_quit(char *buffer,
					 void *user_data)
{
	g_quit = 1;
}

void handle_sigint(int signum)
{
	g_quit = 2;
}

static void prv_print_error(uint8_t status)
{
	fprintf(stdout, "Error: ");
	print_status(stdout, status);
	fprintf(stdout, "\r\n");
}

static char *prv_dump_binding(lwm2m_binding_t binding)
{
	switch (binding) {
	case BINDING_UNKNOWN:
		return "Not specified";
	case BINDING_U:
		return "UDP";
	case BINDING_UQ:
		return "UDP queue mode";
	case BINDING_S:
		return "SMS";
	case BINDING_SQ:
		return "SMS queue mode";
	case BINDING_US:
		return "UDP plus SMS";
	case BINDING_UQS:
		return "UDP queue mode plus SMS";
	default:
		return "";
	}
}

static void prv_dump_client(lwm2m_client_t *targetP)
{
	lwm2m_client_object_t *objectP;

	fprintf(stdout, "Client #%d:\r\n", targetP->internalID);
	fprintf(stdout, "\tname: \"%s\"\r\n", targetP->name);
	fprintf(stdout, "\tbinding: \"%s\"\r\n", prv_dump_binding(targetP->binding));
	if (targetP->msisdn) {
		fprintf(stdout, "\tmsisdn: \"%s\"\r\n", targetP->msisdn);
	}
	if (targetP->altPath) {
		fprintf(stdout, "\talternative path: \"%s\"\r\n", targetP->altPath);
	}
	fprintf(stdout, "\tlifetime: %d sec\r\n", targetP->lifetime);
	fprintf(stdout, "\tobjects: ");
	for (objectP = targetP->objectList; objectP != NULL ; objectP = objectP->next) {
		if (objectP->instanceList == NULL) {
			fprintf(stdout, "/%d, ", objectP->id);
		} else {
			lwm2m_list_t *instanceP;

			for (instanceP = objectP->instanceList; instanceP != NULL ; instanceP = instanceP->next) {
				fprintf(stdout, "/%d/%d, ", objectP->id, instanceP->id);
			}
		}
	}
	fprintf(stdout, "\r\n");
}

static void prv_output_clients(char *buffer,
							   void *user_data)
{
	lwm2m_context_t *lwm2mH = (lwm2m_context_t *) user_data;
	lwm2m_client_t *targetP;

	targetP = lwm2mH->clientList;

	if (targetP == NULL) {
		fprintf(stdout, "No client.\r\n");
		return;
	}

	for (targetP = lwm2mH->clientList ; targetP != NULL ; targetP = targetP->next) {
		prv_dump_client(targetP);
	}
}

static int prv_read_id(char *buffer,
					   uint16_t *idP)
{
	int nb;
	int value;

	nb = sscanf(buffer, "%d", &value);
	if (nb == 1) {
		if (value < 0 || value > LWM2M_MAX_ID) {
			nb = 0;
		} else {
			*idP = value;
		}
	}

	return nb;
}


static void prv_result_callback(uint16_t clientID,
								lwm2m_uri_t *uriP,
								int status,
								lwm2m_media_type_t format,
								uint8_t *data,
								int dataLength,
								void *userData)
{
	fprintf(stdout, "\r\nClient #%d /%d", clientID, uriP->objectId);
	if (LWM2M_URI_IS_SET_INSTANCE(uriP)) {
		fprintf(stdout, "/%d", uriP->instanceId);
	} else if (LWM2M_URI_IS_SET_RESOURCE(uriP)) {
		fprintf(stdout, "/");
	}
	if (LWM2M_URI_IS_SET_RESOURCE(uriP)) {
		fprintf(stdout, "/%d", uriP->resourceId);
	}
	fprintf(stdout, " : ");
	print_status(stdout, status);
	fprintf(stdout, "\r\n");

	output_data(stdout, format, data, dataLength, 1);

	fprintf(stdout, "\r\n> ");
	fflush(stdout);
}

static void prv_notify_callback(uint16_t clientID,
								lwm2m_uri_t *uriP,
								int count,
								lwm2m_media_type_t format,
								uint8_t *data,
								int dataLength,
								void *userData)
{
	fprintf(stdout, "\r\nNotify from client #%d /%d", clientID, uriP->objectId);
	if (LWM2M_URI_IS_SET_INSTANCE(uriP)) {
		fprintf(stdout, "/%d", uriP->instanceId);
	} else if (LWM2M_URI_IS_SET_RESOURCE(uriP)) {
		fprintf(stdout, "/");
	}
	if (LWM2M_URI_IS_SET_RESOURCE(uriP)) {
		fprintf(stdout, "/%d", uriP->resourceId);
	}
	fprintf(stdout, " number %d\r\n", count);

	output_data(stdout, format, data, dataLength, 1);

	fprintf(stdout, "\r\n> ");
	fflush(stdout);
}

static void prv_read_client(char *buffer,
							void *user_data)
{
	lwm2m_context_t *lwm2mH = (lwm2m_context_t *) user_data;
	uint16_t clientId;
	lwm2m_uri_t uri;
	char *end = NULL;
	int result;

	result = prv_read_id(buffer, &clientId);
	if (result != 1) {
		goto syntax_error;
	}

	buffer = get_next_arg(buffer, &end);
	if (buffer[0] == 0) {
		goto syntax_error;
	}

	result = lwm2m_stringToUri(buffer, end - buffer, &uri);
	if (result == 0) {
		goto syntax_error;
	}

	if (!check_end_of_args(end)) {
		goto syntax_error;
	}

	result = lwm2m_dm_read(lwm2mH, clientId, &uri, prv_result_callback, NULL);

	if (result == 0) {
		fprintf(stdout, "OK");
	} else {
		prv_print_error(result);
	}
	return;

syntax_error:
	fprintf(stdout, "Syntax error !");
}

static void prv_discover_client(char *buffer,
								void *user_data)
{
	lwm2m_context_t *lwm2mH = (lwm2m_context_t *) user_data;
	uint16_t clientId;
	lwm2m_uri_t uri;
	char *end = NULL;
	int result;

	result = prv_read_id(buffer, &clientId);
	if (result != 1) {
		goto syntax_error;
	}

	buffer = get_next_arg(buffer, &end);
	if (buffer[0] == 0) {
		goto syntax_error;
	}

	result = lwm2m_stringToUri(buffer, end - buffer, &uri);
	if (result == 0) {
		goto syntax_error;
	}

	if (!check_end_of_args(end)) {
		goto syntax_error;
	}

	result = lwm2m_dm_discover(lwm2mH, clientId, &uri, prv_result_callback, NULL);

	if (result == 0) {
		fprintf(stdout, "OK");
	} else {
		prv_print_error(result);
	}
	return;

syntax_error:
	fprintf(stdout, "Syntax error !");
}

static void prv_write_client(char *buffer,
							 void *user_data)
{
	lwm2m_context_t *lwm2mH = (lwm2m_context_t *) user_data;
	uint16_t clientId;
	lwm2m_uri_t uri;
	char *end = NULL;
	int result;

	result = prv_read_id(buffer, &clientId);
	if (result != 1) {
		goto syntax_error;
	}

	buffer = get_next_arg(buffer, &end);
	if (buffer[0] == 0) {
		goto syntax_error;
	}

	result = lwm2m_stringToUri(buffer, end - buffer, &uri);
	if (result == 0) {
		goto syntax_error;
	}

	buffer = get_next_arg(end, &end);
	if (buffer[0] == 0) {
		goto syntax_error;
	}

	if (!check_end_of_args(end)) {
		goto syntax_error;
	}

	result = lwm2m_dm_write(lwm2mH, clientId, &uri, LWM2M_CONTENT_TEXT, (uint8_t *)buffer, end - buffer, prv_result_callback, NULL);

	if (result == 0) {
		fprintf(stdout, "OK");
	} else {
		prv_print_error(result);
	}
	return;

syntax_error:
	fprintf(stdout, "Syntax error !");
}


static void prv_time_client(char *buffer,
							void *user_data)
{
	lwm2m_context_t *lwm2mH = (lwm2m_context_t *) user_data;
	uint16_t clientId;
	lwm2m_uri_t uri;
	char *end = NULL;
	int result;
	lwm2m_attributes_t attr;
	int nb;
	int value;

	result = prv_read_id(buffer, &clientId);
	if (result != 1) {
		goto syntax_error;
	}

	buffer = get_next_arg(buffer, &end);
	if (buffer[0] == 0) {
		goto syntax_error;
	}

	result = lwm2m_stringToUri(buffer, end - buffer, &uri);
	if (result == 0) {
		goto syntax_error;
	}

	memset(&attr, 0, sizeof(lwm2m_attributes_t));
	attr.toSet = LWM2M_ATTR_FLAG_MIN_PERIOD | LWM2M_ATTR_FLAG_MAX_PERIOD;

	buffer = get_next_arg(end, &end);
	if (buffer[0] == 0) {
		goto syntax_error;
	}

	nb = sscanf(buffer, "%d", &value);
	if (nb != 1) {
		goto syntax_error;
	}
	if (value < 0) {
		goto syntax_error;
	}
	attr.minPeriod = value;

	buffer = get_next_arg(end, &end);
	if (buffer[0] == 0) {
		goto syntax_error;
	}

	nb = sscanf(buffer, "%d", &value);
	if (nb != 1) {
		goto syntax_error;
	}
	if (value < 0) {
		goto syntax_error;
	}
	attr.maxPeriod = value;

	if (!check_end_of_args(end)) {
		goto syntax_error;
	}

	result = lwm2m_dm_write_attributes(lwm2mH, clientId, &uri, &attr, prv_result_callback, NULL);

	if (result == 0) {
		fprintf(stdout, "OK");
	} else {
		prv_print_error(result);
	}
	return;

syntax_error:
	fprintf(stdout, "Syntax error !");
}


static void prv_attr_client(char *buffer,
							void *user_data)
{
	lwm2m_context_t *lwm2mH = (lwm2m_context_t *) user_data;
	uint16_t clientId;
	lwm2m_uri_t uri;
	char *end = NULL;
	int result;
	lwm2m_attributes_t attr;
	int nb;
	float value;

	result = prv_read_id(buffer, &clientId);
	if (result != 1) {
		goto syntax_error;
	}

	buffer = get_next_arg(buffer, &end);
	if (buffer[0] == 0) {
		goto syntax_error;
	}

	result = lwm2m_stringToUri(buffer, end - buffer, &uri);
	if (result == 0) {
		goto syntax_error;
	}

	memset(&attr, 0, sizeof(lwm2m_attributes_t));
	attr.toSet = LWM2M_ATTR_FLAG_LESS_THAN | LWM2M_ATTR_FLAG_GREATER_THAN;

	buffer = get_next_arg(end, &end);
	if (buffer[0] == 0) {
		goto syntax_error;
	}

	nb = sscanf(buffer, "%f", &value);
	if (nb != 1) {
		goto syntax_error;
	}
	attr.lessThan = value;

	buffer = get_next_arg(end, &end);
	if (buffer[0] == 0) {
		goto syntax_error;
	}

	nb = sscanf(buffer, "%f", &value);
	if (nb != 1) {
		goto syntax_error;
	}
	attr.greaterThan = value;

	buffer = get_next_arg(end, &end);
	if (buffer[0] != 0) {
		nb = sscanf(buffer, "%f", &value);
		if (nb != 1) {
			goto syntax_error;
		}
		attr.step = value;

		attr.toSet |= LWM2M_ATTR_FLAG_STEP;
	}

	if (!check_end_of_args(end)) {
		goto syntax_error;
	}

	result = lwm2m_dm_write_attributes(lwm2mH, clientId, &uri, &attr, prv_result_callback, NULL);

	if (result == 0) {
		fprintf(stdout, "OK");
	} else {
		prv_print_error(result);
	}
	return;

syntax_error:
	fprintf(stdout, "Syntax error !");
}


static void prv_clear_client(char *buffer,
							 void *user_data)
{
	lwm2m_context_t *lwm2mH = (lwm2m_context_t *) user_data;
	uint16_t clientId;
	lwm2m_uri_t uri;
	char *end = NULL;
	int result;
	lwm2m_attributes_t attr;

	result = prv_read_id(buffer, &clientId);
	if (result != 1) {
		goto syntax_error;
	}

	buffer = get_next_arg(buffer, &end);
	if (buffer[0] == 0) {
		goto syntax_error;
	}

	result = lwm2m_stringToUri(buffer, end - buffer, &uri);
	if (result == 0) {
		goto syntax_error;
	}

	memset(&attr, 0, sizeof(lwm2m_attributes_t));
	attr.toClear = LWM2M_ATTR_FLAG_LESS_THAN | LWM2M_ATTR_FLAG_GREATER_THAN | LWM2M_ATTR_FLAG_STEP | LWM2M_ATTR_FLAG_MIN_PERIOD | LWM2M_ATTR_FLAG_MAX_PERIOD ;

	buffer = get_next_arg(end, &end);
	if (!check_end_of_args(end)) {
		goto syntax_error;
	}

	result = lwm2m_dm_write_attributes(lwm2mH, clientId, &uri, &attr, prv_result_callback, NULL);

	if (result == 0) {
		fprintf(stdout, "OK");
	} else {
		prv_print_error(result);
	}
	return;

syntax_error:
	fprintf(stdout, "Syntax error !");
}


static void prv_exec_client(char *buffer,
							void *user_data)
{
	lwm2m_context_t *lwm2mH = (lwm2m_context_t *) user_data;
	uint16_t clientId;
	lwm2m_uri_t uri;
	char *end = NULL;
	int result;

	result = prv_read_id(buffer, &clientId);
	if (result != 1) {
		goto syntax_error;
	}

	buffer = get_next_arg(buffer, &end);
	if (buffer[0] == 0) {
		goto syntax_error;
	}

	result = lwm2m_stringToUri(buffer, end - buffer, &uri);
	if (result == 0) {
		goto syntax_error;
	}

	buffer = get_next_arg(end, &end);


	if (buffer[0] == 0) {
		result = lwm2m_dm_execute(lwm2mH, clientId, &uri, 0, NULL, 0, prv_result_callback, NULL);
	} else {
		if (!check_end_of_args(end)) {
			goto syntax_error;
		}

		result = lwm2m_dm_execute(lwm2mH, clientId, &uri, LWM2M_CONTENT_TEXT, (uint8_t *)buffer, end - buffer, prv_result_callback, NULL);
	}

	if (result == 0) {
		fprintf(stdout, "OK");
	} else {
		prv_print_error(result);
	}
	return;

syntax_error:
	fprintf(stdout, "Syntax error !");
}

static void prv_create_client(char *buffer,
							  void *user_data)
{
	lwm2m_context_t *lwm2mH = (lwm2m_context_t *) user_data;
	uint16_t clientId;
	lwm2m_uri_t uri;
	char *end = NULL;
	int result;
	int64_t value;
	uint8_t *temp_buffer = NULL;
	int temp_length = 0;
	lwm2m_media_type_t format = LWM2M_CONTENT_TEXT;

	//Get Client ID
	result = prv_read_id(buffer, &clientId);
	if (result != 1) {
		goto syntax_error;
	}

	//Get Uri
	buffer = get_next_arg(buffer, &end);
	if (buffer[0] == 0) {
		goto syntax_error;
	}

	result = lwm2m_stringToUri(buffer, end - buffer, &uri);
	if (result == 0) {
		goto syntax_error;
	}

	//Get Data to Post
	buffer = get_next_arg(end, &end);
	if (buffer[0] == 0) {
		goto syntax_error;
	}

	if (!check_end_of_args(end)) {
		goto syntax_error;
	}

	// TLV

	/* Client dependent part   */

	if (uri.objectId == 1024) {
		lwm2m_data_t *dataP;

		value = 0;
		if (1 != sscanf(buffer, "%"PRId64, &value)) {
			fprintf(stdout, "Invalid value !");
			return;
		}

		dataP = lwm2m_data_new(1);
		if (dataP == NULL) {
			fprintf(stdout, "Allocation error !");
			return;
		}
		lwm2m_data_encode_int(value, dataP);
		dataP->id = 1;

		format = LWM2M_CONTENT_TLV;
		temp_length = lwm2m_data_serialize(NULL, 1, dataP, &format, &temp_buffer);
		lwm2m_data_free(1, dataP);
	}
	/* End Client dependent part*/

	//Create
	result = lwm2m_dm_create(lwm2mH, clientId, &uri, format, temp_buffer, temp_length, prv_result_callback, NULL);

	if (temp_buffer != NULL) {
		lwm2m_free(temp_buffer);
	}

	if (result == 0) {
		fprintf(stdout, "OK");
	} else {
		prv_print_error(result);
	}
	return;

syntax_error:
	fprintf(stdout, "Syntax error !");
}

static void prv_delete_client(char *buffer,
							  void *user_data)
{
	lwm2m_context_t *lwm2mH = (lwm2m_context_t *) user_data;
	uint16_t clientId;
	lwm2m_uri_t uri;
	char *end = NULL;
	int result;

	result = prv_read_id(buffer, &clientId);
	if (result != 1) {
		goto syntax_error;
	}

	buffer = get_next_arg(buffer, &end);
	if (buffer[0] == 0) {
		goto syntax_error;
	}

	result = lwm2m_stringToUri(buffer, end - buffer, &uri);
	if (result == 0) {
		goto syntax_error;
	}

	if (!check_end_of_args(end)) {
		goto syntax_error;
	}

	result = lwm2m_dm_delete(lwm2mH, clientId, &uri, prv_result_callback, NULL);

	if (result == 0) {
		fprintf(stdout, "OK");
	} else {
		prv_print_error(result);
	}
	return;

syntax_error:
	fprintf(stdout, "Syntax error !");
}

static void prv_observe_client(char *buffer,
							   void *user_data)
{
	lwm2m_context_t *lwm2mH = (lwm2m_context_t *) user_data;
	uint16_t clientId;
	lwm2m_uri_t uri;
	char *end = NULL;
	int result;

	result = prv_read_id(buffer, &clientId);
	if (result != 1) {
		goto syntax_error;
	}

	buffer = get_next_arg(buffer, &end);
	if (buffer[0] == 0) {
		goto syntax_error;
	}

	result = lwm2m_stringToUri(buffer, end - buffer, &uri);
	if (result == 0) {
		goto syntax_error;
	}

	if (!check_end_of_args(end)) {
		goto syntax_error;
	}

	result = lwm2m_observe(lwm2mH, clientId, &uri, prv_notify_callback, NULL);

	if (result == 0) {
		fprintf(stdout, "OK");
	} else {
		prv_print_error(result);
	}
	return;

syntax_error:
	fprintf(stdout, "Syntax error !");
}

static void prv_cancel_client(char *buffer,
							  void *user_data)
{
	lwm2m_context_t *lwm2mH = (lwm2m_context_t *) user_data;
	uint16_t clientId;
	lwm2m_uri_t uri;
	char *end = NULL;
	int result;

	result = prv_read_id(buffer, &clientId);
	if (result != 1) {
		goto syntax_error;
	}

	buffer = get_next_arg(buffer, &end);
	if (buffer[0] == 0) {
		goto syntax_error;
	}

	result = lwm2m_stringToUri(buffer, end - buffer, &uri);
	if (result == 0) {
		goto syntax_error;
	}

	if (!check_end_of_args(end)) {
		goto syntax_error;
	}

	result = lwm2m_observe_cancel(lwm2mH, clientId, &uri, prv_result_callback, NULL);

	if (result == 0) {
		fprintf(stdout, "OK");
	} else {
		prv_print_error(result);
	}
	return;

syntax_error:
	fprintf(stdout, "Syntax error !");
}

static void prv_monitor_callback(uint16_t clientID,
								 lwm2m_uri_t *uriP,
								 int status,
								 lwm2m_media_type_t format,
								 uint8_t *data,
								 int dataLength,
								 void *userData)
{
	lwm2m_context_t *lwm2mH = (lwm2m_context_t *) userData;
	lwm2m_client_t *targetP;

	switch (status) {
	case COAP_201_CREATED:
		fprintf(stdout, "\r\nNew client #%d registered.\r\n", clientID);

		targetP = (lwm2m_client_t *)lwm2m_list_find((lwm2m_list_t *)lwm2mH->clientList, clientID);

		prv_dump_client(targetP);
		break;

	case COAP_202_DELETED:
		fprintf(stdout, "\r\nClient #%d unregistered.\r\n", clientID);
		break;

	case COAP_204_CHANGED:
		fprintf(stdout, "\r\nClient #%d updated.\r\n", clientID);

		targetP = (lwm2m_client_t *)lwm2m_list_find((lwm2m_list_t *)lwm2mH->clientList, clientID);

		prv_dump_client(targetP);
		break;

	default:
		fprintf(stdout, "\r\nMonitor callback called with an unknown status: %d.\r\n", status);
		break;
	}

	fprintf(stdout, "\r\n> ");
	fflush(stdout);
}


#ifdef WITH_MBEDTLS

#define HEX2NUM(c)                    \
	if (c >= '0' && c <= '9')      \
		c -= '0';                   \
	else if (c >= 'a' && c <= 'f') \
		c -= 'a' - 10;              \
	else if (c >= 'A' && c <= 'F') \
		c -= 'A' - 10;              \
	else                            \
		return(-1);

int lwm2m_unhexify(unsigned char *output, const char *input, size_t *olen)
{
	unsigned char c;
	size_t j;

	*olen = strlen(input);
	if (*olen % 2 != 0 || *olen / 2 > MBEDTLS_PSK_MAX_LEN) {
		return (-1);
	}
	*olen /= 2;

	for (j = 0; j < *olen * 2; j += 2) {
		c = input[j];
		HEX2NUM(c);
		output[j / 2] = c << 4;

		c = input[j + 1];
		HEX2NUM(c);
		output[j / 2] |= c;
	}

	return (0);
}
#endif

void print_usage(void)
{
	fprintf(stderr, "Usage: lwm2mserver [OPTION]\r\n");
	fprintf(stderr, "Launch a LWM2M server on localhost.\r\n\n");
	fprintf(stdout, "Options:\r\n");
	fprintf(stdout, "  -p protocol \tSet protocol type, (0: UDP, 1: UDP+DTLS, 2: TCP, 3: TCP+TLS)\r\n");
#ifdef WITH_MBEDTLS
	fprintf(stdout, "  -i STRING\t Set PSK identity.\r\n");
	fprintf(stdout, "  -s HEXSTRING\t Set Pre-Shared-Key. The input length should be even, such as 11, 1111.\r\n");
#endif
	fprintf(stdout, "Examples:\r\n");
	fprintf(stdout, "  lwm2mserver -p 0\r\n");
	fprintf(stdout, "  lwm2mserver -p 1 -i PSK_identity -s 11111111\r\n");
	fprintf(stdout, "  lwm2mserver -p 2\r\n");
	fprintf(stdout, "  lwm2mserver -p 3 -i PSK_identity -s 11111111\r\n");
	fprintf(stdout, "\r\n");
}

int lwm2m_server_cb(void *args)
{
	int argc;
	char **argv;

	argc = ((struct pthread_arg *)args)->argc;
	argv = ((struct pthread_arg *)args)->argv;
	g_quit = 0;
	int sock = -1;
	fd_set readfds;
	struct timeval tv;
	int result;
	lwm2m_context_t *lwm2mH = NULL;
	int i;
	connection_t *connP = NULL;
	int opt;
	const char *localPort = LWM2M_STANDARD_PORT_STR;

	coap_protocol_t proto = COAP_UDP;

	char *pskId = "testpsk";
	char *pskBuffer = "12345678";

#ifdef WITH_MBEDTLS
	unsigned char psk[MBEDTLS_PSK_MAX_LEN];

	/* set default tls option */
	tls_opt tls_option;
	memset(&tls_option, 0, sizeof(tls_opt));

	tls_option.server = MBEDTLS_SSL_IS_SERVER;
	tls_option.transport = MBEDTLS_SSL_TRANSPORT_DATAGRAM;
	tls_option.auth_mode = 2;
	tls_option.debug_mode = 1;

	tls_ctx *tls_context = NULL;
#endif

	struct sockaddr_storage addr;
	socklen_t addrLen = sizeof(addr);

	command_desc_t commands[] = {
		{"list", "List registered clients.", NULL, prv_output_clients, NULL},
		{
			"read", "Read from a client.", " read CLIENT# URI\r\n"
			"   CLIENT#: client number as returned by command 'list'\r\n"
			"   URI: uri to read such as /3, /3/0/2, /1024/11, /1024/0/1\r\n"
			"Result will be displayed asynchronously.", prv_read_client, NULL
		},
		{
			"disc", "Discover resources of a client.", " disc CLIENT# URI\r\n"
			"   CLIENT#: client number as returned by command 'list'\r\n"
			"   URI: uri to discover such as /3, /3/0/2, /1024/11, /1024/0/1\r\n"
			"Result will be displayed asynchronously.", prv_discover_client, NULL
		},
		{
			"write", "Write to a client.", " write CLIENT# URI DATA\r\n"
			"   CLIENT#: client number as returned by command 'list'\r\n"
			"   URI: uri to write to such as /3, /3/0/2, /1024/11, /1024/0/1\r\n"
			"   DATA: data to write\r\n"
			"Result will be displayed asynchronously.", prv_write_client, NULL
		},
		{
			"time", "Write time-related attributes to a client.", " time CLIENT# URI PMIN PMAX\r\n"
			"   CLIENT#: client number as returned by command 'list'\r\n"
			"   URI: uri to write attributes to such as /3, /3/0/2, /1024/11, /1024/0/1\r\n"
			"   PMIN: Minimum period\r\n"
			"   PMAX: Maximum period\r\n"
			"Result will be displayed asynchronously.", prv_time_client, NULL
		},
		{
			"attr", "Write value-related attributes to a client.", " attr CLIENT# URI LT GT [STEP]\r\n"
			"   CLIENT#: client number as returned by command 'list'\r\n"
			"   URI: uri to write attributes to such as /3, /3/0/2, /1024/11, /1024/0/1\r\n"
			"   LT: \"Less than\" value\r\n"
			"   GT: \"Greater than\" value\r\n"
			"   STEP: \"Step\" value\r\n"
			"Result will be displayed asynchronously.", prv_attr_client, NULL
		},
		{
			"clear", "Clear attributes of a client.", " clear CLIENT# URI\r\n"
			"   CLIENT#: client number as returned by command 'list'\r\n"
			"   URI: uri to clear attributes of such as /3, /3/0/2, /1024/11, /1024/0/1\r\n"
			"Result will be displayed asynchronously.", prv_clear_client, NULL
		},
		{
			"exec", "Execute a client resource.", " exec CLIENT# URI\r\n"
			"   CLIENT#: client number as returned by command 'list'\r\n"
			"   URI: uri of the resource to execute such as /3/0/2\r\n"
			"Result will be displayed asynchronously.", prv_exec_client, NULL
		},
		{
			"del", "Delete a client Object instance.", " del CLIENT# URI\r\n"
			"   CLIENT#: client number as returned by command 'list'\r\n"
			"   URI: uri of the instance to delete such as /1024/11\r\n"
			"Result will be displayed asynchronously.", prv_delete_client, NULL
		},
		{
			"create", "create an Object instance.", " create CLIENT# URI DATA\r\n"
			"   CLIENT#: client number as returned by command 'list'\r\n"
			"   URI: uri to which create the Object Instance such as /1024, /1024/45 \r\n"
			"   DATA: data to initialize the new Object Instance (0-255 for object 1024) \r\n"
			"Result will be displayed asynchronously.", prv_create_client, NULL
		},
		{
			"observe", "Observe from a client.", " observe CLIENT# URI\r\n"
			"   CLIENT#: client number as returned by command 'list'\r\n"
			"   URI: uri to observe such as /3, /3/0/2, /1024/11\r\n"
			"Result will be displayed asynchronously.", prv_observe_client, NULL
		},
		{
			"cancel", "Cancel an observe.", " cancel CLIENT# URI\r\n"
			"   CLIENT#: client number as returned by command 'list'\r\n"
			"   URI: uri on which to cancel an observe such as /3, /3/0/2, /1024/11\r\n"
			"Result will be displayed asynchronously.", prv_cancel_client, NULL
		},

		{"q", "Quit the server.", NULL, prv_quit, NULL},

		COMMAND_END_LIST
	};

	opt = 1;
	while (opt < argc) {
		if (argv[opt] == NULL
			|| argv[opt][0] != '-'
			|| argv[opt][2] != 0) {
			print_usage();
			return 0;
		}
		switch (argv[opt][1]) {
		case 'p':
			opt++;
			if (opt >= argc) {
				print_usage();
				return 0;
			}
			proto = (coap_protocol_t)atoi(argv[opt]);
			if (proto >= COAP_PROTOCOL_MAX || proto < 0) {
				printf("Error : not supported protocol\n");
				print_usage();
				return 0;
			} else {
				printf("INFO : selected protocol : %s\n", g_coap_protocol[proto]);
			}
			break;
#ifdef WITH_MBEDTLS
		case 'i':
			opt++;
			if (opt >= argc) {
				print_usage();
				return 0;
			}
			pskId = argv[opt];
			break;
		case 's':
			opt++;
			if (opt >= argc) {
				print_usage();
				return 0;
			}
			pskBuffer = argv[opt];
			break;
#endif
		default:
			print_usage();
			return 0;
		}
		opt += 1;
	}

	/* Step 0. Init context */
	lwm2mH = lwm2m_init(NULL);
	if (NULL == lwm2mH) {
		printf("lwm2m_init2() failed\n");
		goto exit;
	}
	lwm2mH->protocol = proto;

	for (i = 0 ; commands[i].name != NULL ; i++) {
		commands[i].userData = (void *)lwm2mH;
	}
	lwm2m_set_monitoring_callback(lwm2mH, prv_monitor_callback, lwm2mH);

#ifdef WITH_MBEDTLS
	if (proto == COAP_TCP_TLS || proto == COAP_UDP_DTLS) {
		tls_option.transport = (proto == COAP_TCP_TLS) ? (MBEDTLS_SSL_TRANSPORT_STREAM) : (MBEDTLS_SSL_TRANSPORT_DATAGRAM);
		tls_cred cred;
		memset(&cred, 0, sizeof(tls_cred));

		if (pskBuffer) {
			if (lwm2m_unhexify(psk, pskBuffer, &cred.psk_len) == 0) {
				if (pskId) {
					cred.psk_identity = pskId;
					cred.psk = psk;
				}
			}
			if (cred.psk_identity == NULL && cred.psk == NULL) {
				printf("failed to set psk info\n");
				goto exit;
			}
		} else {
			printf("Please set psk and psk_id\n");
			goto exit;
		}

		tls_context = TLSCtx(&cred);
		if (tls_context == NULL) {
			printf("TLS context initialize filaed\n");
			goto exit;
		}
		localPort = LWM2M_DTLS_PORT_STR;
	}
#endif

	/* Step 1. Create socket */
	sock = create_socket(proto, localPort, AF_INET);
	if (sock < 0) {
		printf("Error opening socket: %d\n", errno);
		goto exit;
	}

recon:
	/* Step 2. Waiting for a remote connection */
	printf("Waiting for a remote connection(port : %s) ...\n", localPort);
	if (proto == COAP_TCP_TLS || proto == COAP_UDP_DTLS) {
#ifdef WITH_MBEDTLS
		connP = create_session(sock, &addr, &addrLen, proto, tls_context, &tls_option);
#else
		printf("Platform is not supporting tls\n");
#endif
	} else if (proto == COAP_UDP || proto == COAP_TCP) {
		connP = create_session(sock, &addr, &addrLen, proto, NULL, NULL);
	} else {
		printf("Error: unsupported protocol proto : %d\n", proto);
	}

	if (connP == NULL) {
		printf("Error: create session\n");
		goto exit;
	}

	printf("session created : fd - %d port - %s\n", connP->sock, localPort);

	/* Step 3. Handling client request */
	while (0 == g_quit) {
		tv.tv_sec = 60;
		tv.tv_usec = 0;

		FD_ZERO(&readfds);
		FD_SET(connP->sock, &readfds);
		FD_SET(STDIN_FILENO, &readfds);

		result = lwm2m_step(lwm2mH, &(tv.tv_sec));
		if (result != 0) {
			printf("lwm2m_step() failed: 0x%X\n", result);
			goto exit;
		}

		printf("> ");
		result = select(FD_SETSIZE, &readfds, 0, 0, &tv);

		if (result < 0) {
			if (errno != EINTR) {
				printf("Error in select(): %d\n", errno);
			}
		} else if (result > 0) {
			uint8_t buffer[MAX_PACKET_SIZE];
			int numBytes = 0;
			if (FD_ISSET(connP->sock, &readfds)) {
				numBytes = connection_read(proto, connP, connP->sock, buffer, MAX_PACKET_SIZE, &addr, &addrLen);
				if (numBytes > 0) {
					char s[INET6_ADDRSTRLEN];
					struct sockaddr_in *saddr = (struct sockaddr_in *)&addr;
					inet_ntop(saddr->sin_family, &saddr->sin_addr, s, INET6_ADDRSTRLEN);
					printf("from : %s  fromLen : %d\n", s, addrLen);

					if (connP->addrLen != addrLen || memcmp(&(connP->addr), &addr, addrLen) != 0) {
						numBytes = -1;
					}
				}
				if (numBytes < 0) {
					if (errno == ENOTCONN) {
						close(connP->sock);
						connection_free(connP);
						goto recon;
					}
					printf("Error: connection_read(): %d\n", errno);

					goto exit;
				} else {
					output_buffer(stderr, buffer, numBytes, 0);
					lwm2m_handle_packet(lwm2mH, buffer, numBytes, connP);
				}
			} else if (FD_ISSET(STDIN_FILENO, &readfds)) {
				numBytes = read_input_command_line((char *)buffer);
				if (numBytes > 1) {
					buffer[numBytes] = 0;
					handle_command(commands, (char *)buffer);
				}
				if (g_quit) {
					fprintf(stdout, "\r\n");
				}
			}
		}
	}

	/* Step 4. release */
exit:
	if (lwm2mH) {
		lwm2m_close(lwm2mH);
	}

	if (connP->sock) {
		close(connP->sock);
	}

	if (connP) {
		connection_free(connP);
	}

#ifdef WITH_MBEDTLS
	if (tls_context) {
		TLSCtx_free(tls_context);
	}
#endif

	if (sock >= 0) {
		close(sock);
	}

	return 0;
}

int lwm2m_server_main(int argc, char *argv[])
{
	pthread_t tid;
	pthread_attr_t attr;
	struct sched_param sparam;
	int r;

	struct pthread_arg args;
	args.argc = argc;
	args.argv = argv;

	/* Initialize the attribute variable */
	if ((r = pthread_attr_init(&attr)) != 0) {
		printf("%s: pthread_attr_init failed, status=%d\n", __func__, r);
		return -1;
	}

	/* 1. set a priority */
	sparam.sched_priority = LWM2M_SERVER_PRIORITY;
	if ((r = pthread_attr_setschedparam(&attr, &sparam)) != 0) {
		printf("%s: pthread_attr_setschedparam failed, status=%d\n", __func__, r);
		return -1;
	}

	if ((r = pthread_attr_setschedpolicy(&attr, LWM2M_SERVER_SCHED_POLICY)) != 0) {
		printf("%s: pthread_attr_setschedpolicy failed, status=%d\n", __func__, r);
		return -1;
	}

	/* 2. set a stacksize */
	if ((r = pthread_attr_setstacksize(&attr, LWM2M_SERVER_STACK_SIZE)) != 0) {
		printf("%s: pthread_attr_setstacksize failed, status=%d\n", __func__, r);
		return -1;
	}

	/* 3. create pthread with entry function */
	if ((r = pthread_create(&tid, &attr, (pthread_startroutine_t)lwm2m_server_cb, (void *)&args)) != 0) {
		printf("%s: pthread_create failed, status=%d\n", __func__, r);
		return -1;
	}

	/* Wait for the threads to stop */
	pthread_join(tid, NULL);

	return 0;
}
