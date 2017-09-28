/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
 *    Benjamin Cabé - Please refer to git log
 *    Fabien Fleutot - Please refer to git log
 *    Simon Bernard - Please refer to git log
 *    Julien Vermillard - Please refer to git log
 *    Axel Lorente - Please refer to git log
 *    Toby Jaffey - Please refer to git log
 *    Bosch Software Innovations GmbH - Please refer to git log
 *    Pascal Rieux - Please refer to git log
 *    Christian Renz - Please refer to git log
 *    Ricky Liu - Please refer to git log
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
 Bosch Software Innovations GmbH - Please refer to git log

*/

#include <tinyara/config.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/stat.h>
#include <errno.h>
#include <signal.h>
#include <tinyara/ascii.h>

#include "lwm2m_client_main.h"

/*
 * Definition for handling pthread
 */
#define LWM2M_CLIENT_PRIORITY     100
#define LWM2M_CLIENT_STACK_SIZE   16384
#define LWM2M_CLIENT_SCHED_POLICY SCHED_RR

#define OBJ_COUNT 9
# define BACKUP_OBJECT_COUNT 2

int g_reboot = 0;
static int g_quit = 0;

static char coap_uri_prefix[COAP_PROTOCOL_MAX][COAP_MAX_URI_PREFIX_SIZE] = {
	"coap://",
	"coaps://",
	"coap+tcp://",
	"coaps+tcp://"
};

int g_lifetime = 300;

/* PSK for example. Please do not use it in commercial version */
char *g_pskId = "testpsk";
uint16_t g_pskLen = 8;
char *g_pskBuffer = "12345678";

char *g_step[6] = {"STATE_INITIAL", "STATE_BOOTSTRAP_REQUIRED", "STATE_BOOTSTRAPPING", \
				   "STATE_REGISTER_REQUIRED", "STATE_REGISTERING", "STATE_READY"
				  };
const char *server = DEFAULT_SERVER_IPV4;
const char *serverPort = LWM2M_STANDARD_PORT_STR;
coap_protocol_t g_proto = COAP_UDP;
client_data_t data;
lwm2m_context_t *lwm2mH;
lwm2m_object_t *objArray[OBJ_COUNT];
lwm2m_object_t *backupObjectArray[BACKUP_OBJECT_COUNT];

static void prv_quit(char *buffer, void *user_data)
{
	g_quit = 1;
}

void handle_sigint(int signum)
{
	g_quit = 2;
}

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

void handle_value_changed(lwm2m_context_t *lwm2mP, lwm2m_uri_t *uri, const char *value, size_t valueLength)
{
	lwm2m_object_t *object = (lwm2m_object_t *)LWM2M_LIST_FIND(lwm2mP->objectList, uri->objectId);

	if (NULL != object) {
		if (object->writeFunc != NULL) {
			lwm2m_data_t *dataP;
			int result;

			dataP = lwm2m_data_new(1);
			if (dataP == NULL) {
				fprintf(stderr, "Internal allocation failure !\n");
				return;
			}
			dataP->id = uri->resourceId;
			lwm2m_data_encode_nstring(value, valueLength, dataP);

			result = object->writeFunc(uri->instanceId, 1, dataP, object);
			if (COAP_405_METHOD_NOT_ALLOWED == result) {
				switch (uri->objectId) {
				case LWM2M_DEVICE_OBJECT_ID:
					result = device_change(dataP, object);
					break;
				default:
					break;
				}
			}

			if (COAP_204_CHANGED != result) {
				fprintf(stderr, "Failed to change value!\n");
			} else {
				fprintf(stderr, "value changed!\n");
				lwm2m_resource_value_changed(lwm2mP, uri);
			}
			lwm2m_data_free(1, dataP);
			return;
		} else {
			fprintf(stderr, "write not supported for specified resource!\n");
		}
		return;
	} else {
		fprintf(stderr, "Object not found !\n");
	}
}

static char *coap_get_port_from_proto(coap_protocol_t proto)
{
	char *port = NULL;

	switch (proto) {
	case COAP_TCP:
	case COAP_UDP:
		port = LWM2M_STANDARD_PORT_STR;
		break;
	case COAP_TCP_TLS:
	case COAP_UDP_DTLS:
		port = LWM2M_DTLS_PORT_STR;
		break;
	default:
		break;
	}

	return port;
}

static coap_protocol_t coap_get_protocol_from_uri(const char *uri)
{
	coap_protocol_t type;
	for (type = COAP_UDP; type < COAP_PROTOCOL_MAX; type++) {
		if (!strncmp(uri, coap_uri_prefix[type], strlen(coap_uri_prefix[type]))) {
			return type;
		}
	}
	return type;
}

void *lwm2m_connect_server(uint16_t secObjInstID, void *userData)
{
	client_data_t *dataP;
	char *uri;
	char *host;
	char *port;
	connection_t *newConnP = NULL;
	coap_protocol_t proto = COAP_UDP;

	dataP = (client_data_t *)userData;

	uri = get_server_uri(dataP->securityObjP, secObjInstID);

	if (uri == NULL) {
		return NULL;
	}

	// parse uri in the form "coaps://[host]:[port]"

	proto = coap_get_protocol_from_uri(uri);
	if (proto >= COAP_PROTOCOL_MAX) {
		fprintf(stderr, "Not supported protocol : %d\n", proto);
		goto exit;
	} else {
		/* move pointer to address field */
		host = uri + strlen(coap_uri_prefix[proto]);
	}

	port = strrchr(host, ':');
	if (port == NULL) {
		goto exit;
	}
	// remove brackets
	if (host[0] == '[') {
		host++;
		if (*(port - 1) == ']') {
			*(port - 1) = 0;
		} else {
			goto exit;
		}
	}
	// split strings
	*port = 0;
	port++;

	fprintf(stdout, "Opening connection to server at %s:%s\r\n", host, port);
	fprintf(stdout, "Connection protocol type : %d\r\n", proto);
	newConnP = connection_create(proto, dataP->connP, dataP->sock, host, port, dataP->addressFamily);
	if (newConnP == NULL) {
		fprintf(stderr, "Connection creation failed.\r\n");
		return NULL;
	} else {
		memcpy(&dataP->server_addr, &newConnP->addr, newConnP->addrLen);
		dataP->server_addrLen = newConnP->addrLen;
		dataP->connP = newConnP;

#ifdef WITH_MBEDTLS
		if (proto == COAP_TCP_TLS || proto == COAP_UDP_DTLS) {
			newConnP->session = TLSSession(newConnP->sock, dataP->tls_context, dataP->tls_opt);
			if (newConnP->session == NULL) {
				fprintf(stderr, "Failed to create secure session. \r\n");
				connection_free(newConnP);
				newConnP = NULL;
				goto exit;
			}
			dataP->sock = newConnP->session->net.fd;
			fprintf(stdout, "successfully create secure session. \r\n");
		}
#endif
	}

exit:
	lwm2m_free(uri);
	return (void *)newConnP;
}

void lwm2m_close_connection(void *sessionH, void *userData)
{
	client_data_t *app_data;
	connection_t *targetP;
	app_data = (client_data_t *)userData;
	targetP = (connection_t *)sessionH;

	if (targetP == app_data->connP) {
		app_data->connP = targetP->next;
		lwm2m_free(targetP);
	} else {
		connection_t *parentP;

		parentP = app_data->connP;
		while (parentP != NULL && parentP->next != targetP) {
			parentP = parentP->next;
		}
		if (parentP != NULL) {
			parentP->next = targetP->next;
			lwm2m_free(targetP);
		}
	}

#ifdef WITH_MBEDTLS
	if (targetP->session) {
		TLSSession_free(targetP->session);
	}
#endif
}

static void prv_output_servers(char *buffer, void *user_data)
{
	lwm2mH = (lwm2m_context_t *) user_data;
	lwm2m_server_t *targetP;

	targetP = lwm2mH->bootstrapServerList;

	if (lwm2mH->bootstrapServerList == NULL) {
		fprintf(stdout, "No Bootstrap Server.\r\n");
	} else {
		fprintf(stdout, "Bootstrap Servers:\r\n");
		for (targetP = lwm2mH->bootstrapServerList ; targetP != NULL ; targetP = targetP->next) {
			fprintf(stdout, " - Security Object ID %d", targetP->secObjInstID);
			fprintf(stdout, "\tHold Off Time: %lu s", (unsigned long)targetP->lifetime);
			fprintf(stdout, "\tstatus: ");
			switch (targetP->status) {
			case STATE_DEREGISTERED:
				fprintf(stdout, "DEREGISTERED\r\n");
				break;
			case STATE_BS_HOLD_OFF:
				fprintf(stdout, "CLIENT HOLD OFF\r\n");
				break;
			case STATE_BS_INITIATED:
				fprintf(stdout, "BOOTSTRAP INITIATED\r\n");
				break;
			case STATE_BS_PENDING:
				fprintf(stdout, "BOOTSTRAP PENDING\r\n");
				break;
			case STATE_BS_FINISHED:
				fprintf(stdout, "BOOTSTRAP FINISHED\r\n");
				break;
			case STATE_BS_FAILED:
				fprintf(stdout, "BOOTSTRAP FAILED\r\n");
				break;
			default:
				fprintf(stdout, "INVALID (%d)\r\n", (int)targetP->status);
			}
		}
	}

	if (lwm2mH->serverList == NULL) {
		fprintf(stdout, "No LWM2M Server.\r\n");
	} else {
		fprintf(stdout, "LWM2M Servers:\r\n");
		for (targetP = lwm2mH->serverList ; targetP != NULL ; targetP = targetP->next) {
			fprintf(stdout, " - Server ID %d", targetP->shortID);
			fprintf(stdout, "\tstatus: ");
			switch (targetP->status) {
			case STATE_DEREGISTERED:
				fprintf(stdout, "DEREGISTERED\r\n");
				break;
			case STATE_REG_PENDING:
				fprintf(stdout, "REGISTRATION PENDING\r\n");
				break;
			case STATE_REGISTERED:
				fprintf(stdout, "REGISTERED\tlocation: \"%s\"\tLifetime: %lus\r\n", targetP->location, (unsigned long)targetP->lifetime);
				break;
			case STATE_REG_UPDATE_PENDING:
				fprintf(stdout, "REGISTRATION UPDATE PENDING\r\n");
				break;
			case STATE_DEREG_PENDING:
				fprintf(stdout, "DEREGISTRATION PENDING\r\n");
				break;
			case STATE_REG_FAILED:
				fprintf(stdout, "REGISTRATION FAILED\r\n");
				break;
			default:
				fprintf(stdout, "INVALID (%d)\r\n", (int)targetP->status);
			}
		}
	}
}

static void prv_change(char *buffer,
					   void *user_data)
{
	lwm2mH = (lwm2m_context_t *) user_data;
	lwm2m_uri_t uri;
	char *end = NULL;
	int result;

	end = get_end_of_arg(buffer);
	if (end[0] == 0) {
		goto syntax_error;
	}

	result = lwm2m_stringToUri(buffer, end - buffer, &uri);
	if (result == 0) {
		goto syntax_error;
	}

	buffer = get_next_arg(end, &end);

	if (buffer[0] == 0) {
		fprintf(stderr, "report change!\n");
		lwm2m_resource_value_changed(lwm2mH, &uri);
	} else {
		handle_value_changed(lwm2mH, &uri, buffer, end - buffer);
	}
	return;

syntax_error:
	fprintf(stdout, "Syntax error !\n");
}

static void prv_object_list(char *buffer, void *user_data)
{
	lwm2mH = (lwm2m_context_t *)user_data;
	lwm2m_object_t *objectP;

	for (objectP = lwm2mH->objectList; objectP != NULL; objectP = objectP->next) {
		if (objectP->instanceList == NULL) {
			fprintf(stdout, "/%d ", objectP->objID);
		} else {
			lwm2m_list_t *instanceP;

			for (instanceP = objectP->instanceList; instanceP != NULL ; instanceP = instanceP->next) {
				fprintf(stdout, "/%d/%d  ", objectP->objID, instanceP->id);
			}
		}
		fprintf(stdout, "\r\n");
	}
}

static void prv_instance_dump(lwm2m_object_t *objectP, uint16_t id)
{
	int numData;
	lwm2m_data_t *dataArray;
	uint16_t res;

	numData = 0;
	res = objectP->readFunc(id, &numData, &dataArray, objectP);
	if (res != COAP_205_CONTENT) {
		printf("Error ");
		print_status(stdout, res);
		printf("\r\n");
		return;
	}

	dump_tlv(stdout, numData, dataArray, 0);
}


static void prv_object_dump(char *buffer, void *user_data)
{
	lwm2mH = (lwm2m_context_t *) user_data;
	lwm2m_uri_t uri;
	char *end = NULL;
	int result;
	lwm2m_object_t *objectP;

	end = get_end_of_arg(buffer);
	if (end[0] == 0) {
		goto syntax_error;
	}

	result = lwm2m_stringToUri(buffer, end - buffer, &uri);
	if (result == 0) {
		goto syntax_error;
	}
	if (uri.flag & LWM2M_URI_FLAG_RESOURCE_ID) {
		goto syntax_error;
	}

	objectP = (lwm2m_object_t *)LWM2M_LIST_FIND(lwm2mH->objectList, uri.objectId);
	if (objectP == NULL) {
		fprintf(stdout, "Object not found.\n");
		return;
	}

	if (uri.flag & LWM2M_URI_FLAG_INSTANCE_ID) {
		prv_instance_dump(objectP, uri.instanceId);
	} else {
		lwm2m_list_t *instanceP;

		for (instanceP = objectP->instanceList; instanceP != NULL ; instanceP = instanceP->next) {
			fprintf(stdout, "Instance %d:\r\n", instanceP->id);
			prv_instance_dump(objectP, instanceP->id);
			fprintf(stdout, "\r\n");
		}
	}

	return;

syntax_error:
	fprintf(stdout, "Syntax error !\n");
}

static void prv_update(char *buffer, void *user_data)
{
	lwm2mH = (lwm2m_context_t *)user_data;
	if (buffer[0] == 0) {
		goto syntax_error;
	}

	uint16_t serverId = (uint16_t) atoi(buffer);
	int res = lwm2m_update_registration(lwm2mH, serverId, false);
	if (res != 0) {
		fprintf(stdout, "Registration update error: ");
		print_status(stdout, res);
		fprintf(stdout, "\r\n");
	}
	return;

syntax_error:
	fprintf(stdout, "Syntax error !\n");
}

static void update_battery_level(lwm2m_context_t *context)
{
	static time_t next_change_time = 0;
	time_t tv_sec;

	tv_sec = lwm2m_gettime();
	if (tv_sec < 0) {
		return;
	}

	if (next_change_time < tv_sec) {
		char value[15];
		int valueLength;
		lwm2m_uri_t uri;
		int level = rand() % 100;

		if (0 > level) {
			level = -level;
		}
		if (lwm2m_stringToUri("/3/0/9", 6, &uri)) {
			valueLength = sprintf(value, "%d", level);
			fprintf(stderr, "New Battery Level: %d\n", level);
			handle_value_changed(context, &uri, value, valueLength);
		}
		level = rand() % 20;
		if (0 > level) {
			level = -level;
		}
		next_change_time = tv_sec + level + 10;
	}
}

static void prv_add(char *buffer, void *user_data)
{
	lwm2mH = (lwm2m_context_t *)user_data;
	lwm2m_object_t *objectP;
	int res;

	objectP = get_test_object();
	if (objectP == NULL) {
		fprintf(stdout, "Creating object 1024 failed.\r\n");
		return;
	}
	res = lwm2m_add_object(lwm2mH, objectP);
	if (res != 0) {
		fprintf(stdout, "Adding object 1024 failed: ");
		print_status(stdout, res);
		fprintf(stdout, "\r\n");
		free_test_object(objectP);
	} else {
		fprintf(stdout, "Object 1024 added.\r\n");
		objArray[5] = objectP;
	}
	return;
}

static void prv_remove(char *buffer, void *user_data)
{
	lwm2mH = (lwm2m_context_t *)user_data;
	int res;

	lwm2m_object_t * objectP;
	objectP = (lwm2m_object_t *)LWM2M_LIST_FIND(lwm2mH->objectList, 1024);
	res = lwm2m_remove_object(lwm2mH, 1024);
	if (res != 0) {
		fprintf(stdout, "Removing object 1024 failed: ");
		print_status(stdout, res);
		fprintf(stdout, "\r\n");
	} else {
		fprintf(stdout, "Object 1024 removed.\r\n");
		free_test_object(objectP);
		objArray[5] = NULL;
	}
	return;
}

#ifdef WITH_MBEDTLS

#define HEX2NUM(c)				\
if (c >= '0' && c <= '9')		\
	c -= '0';					\
else if (c >= 'a' && c <= 'f')	\
	c -= 'a' - 10;				\
else if (c >= 'A' && c <= 'F')	\
	c -= 'A' - 10;				\
else							\
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

int lwm2m_init_object(void)
{
	char serverUri[50];
	int serverId = 123;

	/* Fill an array with each object. */
	sprintf(serverUri, "%s%s:%s", coap_uri_prefix[g_proto], server, serverPort);
	objArray[0] = get_security_object(serverId, serverUri, g_pskId, g_pskBuffer, g_pskLen, false);
	if (NULL == objArray[0]) {
		fprintf(stderr, "Failed to create security object\r\n");
		return -1;
	}
	data.securityObjP = objArray[0];
	/*
	 * Bind Accordingly Protocol (e.g., TCP, UDP)
	 * get_server_object(serverId, "T", lifetime, false);
	 */
	switch (g_proto) {
	case COAP_TCP:
	case COAP_TCP_TLS:
		objArray[1] = get_server_object(serverId, "T", g_lifetime, false);
		break;
	case COAP_UDP:
	case COAP_UDP_DTLS:
		objArray[1] = get_server_object(serverId, "U", g_lifetime, false);
		break;
	default:
		fprintf(stderr, "Cannot get_server_object with protocol %d\n", g_proto);
		break;
	}

	if (NULL == objArray[1]) {
		fprintf(stderr, "Failed to create server object\r\n");
		return -1;
	}

	objArray[2] = get_object_device();
	if (NULL == objArray[2]) {
		fprintf(stderr, "Failed to create Device object\r\n");
		return -1;
	}

	objArray[3] = get_object_firmware();
	if (NULL == objArray[3]) {
		fprintf(stderr, "Failed to create Firmware object\r\n");
		return -1;
	}

	objArray[4] = get_object_location();
	if (NULL == objArray[4]) {
		fprintf(stderr, "Failed to create location object\r\n");
		return -1;
	}

	objArray[5] = get_test_object();
	if (NULL == objArray[5]) {
		fprintf(stderr, "Failed to create test object\r\n");
		return -1;
	}

	objArray[6] = get_object_conn_m();
	if (NULL == objArray[6]) {
		fprintf(stderr, "Failed to create connectivity monitoring object\r\n");
		return -1;
	}

	objArray[7] = get_object_conn_s();
	if (NULL == objArray[7]) {
		fprintf(stderr, "Failed to create connectivity statistics object\r\n");
		return -1;
	}

	int instId = 0;
	objArray[8] = acc_ctrl_create_object();
	if (NULL == objArray[8]) {
		fprintf(stderr, "Failed to create Access Control object\r\n");
		return -1;
	} else if (acc_ctrl_obj_add_inst(objArray[8], instId, 3, 0, serverId) == false) {
		fprintf(stderr, "Failed to create Access Control object instance\r\n");
		return -1;
	} else if (acc_ctrl_oi_add_ac_val(objArray[8], instId, 0, 0b000000000001111) == false) {
		fprintf(stderr, "Failed to create Access Control ACL default resource\r\n");
		return -1;
	} else if (acc_ctrl_oi_add_ac_val(objArray[8], instId, 999, 0b000000000000001) == false) {
		fprintf(stderr, "Failed to create Access Control ACL resource for serverId: 999\r\n");
		return -1;
	}
	return 0;
}


void print_usage(void)
{
	fprintf(stdout, "Usage: lwm2mclient [OPTION]\r\n");
	fprintf(stdout, "Launch a LWM2M client.\r\n");
	fprintf(stdout, "Options:\r\n");
	fprintf(stdout, "  -h HOST\tSet the hostname of the LWM2M Server to connect to. Default: localhost\r\n");
	fprintf(stdout, "  -t TIME\tSet the lifetime of the Client. Default: 300\r\n");
#ifdef WITH_MBEDTLS
	fprintf(stdout, "  -i STRING\t Set PSK identity.\r\n");
	fprintf(stdout, "  -s HEXSTRING\t Set Pre-Shared-Key. The input length should be even, such as 11, 1111.\r\n");
#endif
	fprintf(stdout, "Examples:\r\n");
	fprintf(stdout, "  lwm2mclient -h coap://127.0.0.1\r\n");
	fprintf(stdout, "  lwm2mclient -h coaps://127.0.0.1 -i PSK_identity -s 11111111\r\n");
	fprintf(stdout, "  lwm2mclient -h coap+tcp://127.0.0.1\r\n");
	fprintf(stdout, "  lwm2mclient -h coaps+tcp://127.0.0.1 -i PSK_identity -s 11111111\r\n");
	fprintf(stdout, "\r\n");
}

int lwm2m_client_cb(void *args)
{
	int argc;
	char **argv;

	int i;
	int result;
	int opt;
	char *name = "testlwm2mclient";

#ifdef WITH_MBEDTLS
	unsigned char psk[MBEDTLS_PSK_MAX_LEN];

	/* set default tls option */

	/*
	 * if you want to change auth_mode, please change 3rd parameter of tls_opt structure
	 * - auth_mode can be configured (2: mandatory 1: optional, 0: not verify)
	 */
	tls_opt option = {MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_DATAGRAM, 0, 1, NULL, };
	/* set cipher suite to all*/
	option.force_ciphersuites[0] = mbedtls_ssl_get_ciphersuite_id(LWM2M_CIPHERSUIT);
	option.force_ciphersuites[1] = 0;
#endif

	struct timeval tv = {60, 0};
	argc = ((struct pthread_arg *)args)->argc;
	argv = ((struct pthread_arg *)args)->argv;

	lwm2mH = NULL;
	g_quit = 0;
	server = DEFAULT_SERVER_IPV4;
	serverPort = LWM2M_STANDARD_PORT_STR;
	/* Setting up the command line interface. */
	command_desc_t commands[] = {
		{"list", "List known servers.", NULL, prv_output_servers, NULL},
		{
			"change", "Change the value of resource.", " change URI [DATA]\r\n"
			"   URI: uri of the resource such as /3/0, /3/0/2\r\n"
			"   DATA: (optional) new value\r\n", prv_change, NULL
		},
		{
			"update", "Trigger a registration update", " update SERVER\r\n"
			"   SERVER: short server id such as 123\r\n", prv_update, NULL
		},
		{"ls", "List Objects and Instances", NULL, prv_object_list, NULL},
		{
			"dump", "Dump an Object", "dump URI"
			"URI: uri of the Object or Instance such as /3/0, /1\r\n", prv_object_dump, NULL
		},
		{"add", "Add support of object 1024", NULL, prv_add, NULL},
		{"rm", "Remove support of object 1024", NULL, prv_remove, NULL},
		{"quit", "Quit the client gracefully.", NULL, prv_quit, NULL},
		{"^C", "Quit the client abruptly (without sending a de-register message).", NULL, NULL, NULL},
		COMMAND_END_LIST
	};

	memset(&data, 0, sizeof(client_data_t));
	data.addressFamily = AF_INET;

	opt = 1;
	while (opt < argc) {
		if (argv[opt] == NULL
			|| argv[opt][0] != '-'
			|| argv[opt][2] != 0) {
			print_usage();
			return 0;
		}
		switch (argv[opt][1]) {
		case 't':
			opt++;
			if (opt >= argc) {
				print_usage();
				return 0;
			}
			if (1 != sscanf(argv[opt], "%d", &g_lifetime)) {
				print_usage();
				return 0;
			}
			break;
#ifdef WITH_MBEDTLS
		case 'i':
			opt++;
			if (opt >= argc) {
				print_usage();
				return 0;
			}
			g_pskId = argv[opt];
			break;
		case 's':
			opt++;
			if (opt >= argc) {
				print_usage();
				return 0;
			}
			g_pskBuffer = argv[opt];
			break;
#endif
		case 'h':
			opt++;
			if (opt >= argc) {
				print_usage();
				return 0;
			}
			server = argv[opt];
			break;
		default:
			print_usage();
			return 0;
		}
		opt += 1;
	}

	/* Parse server URI to distinguish protocol and server address */
	g_proto = coap_get_protocol_from_uri(server);
	if (g_proto >= COAP_PROTOCOL_MAX) {
		printf("Not supported protocol : %d\n", g_proto);
		return -1;
	}

	/* Move pointer to address field */
	server += strlen(coap_uri_prefix[g_proto]);
	serverPort = coap_get_port_from_proto(g_proto);

	if (lwm2m_init_object() < 0) {
	}

	/* This call an internal function that create an socket. */
	printf("Trying to bind LWM2M Client to port %s\n", serverPort);
	data.sock = create_socket(g_proto, serverPort, data.addressFamily);
	if (data.sock < 0) {
		fprintf(stderr, "Failed to open socket: %d %s\r\n", errno, strerror(errno));
		return -1;
	}

#ifdef WITH_MBEDTLS
	if (g_proto == COAP_TCP_TLS || g_proto == COAP_UDP_DTLS) {

		/* Set Transport layer (TCP or UDP) */
		switch (g_proto) {
		case COAP_TCP_TLS:
			option.transport = MBEDTLS_SSL_TRANSPORT_STREAM;
			break;
		case COAP_UDP_DTLS:
			option.transport = MBEDTLS_SSL_TRANSPORT_DATAGRAM;
			break;
		default:
			break;
		}
		data.tls_opt = &option;

		/* Set credential information */
		tls_cred cred;
		memset(&cred, 0, sizeof(tls_cred));

		if (g_pskBuffer) {
			if (lwm2m_unhexify(psk, g_pskBuffer, &cred.psk_len) == 0) {
				if (g_pskId) {
					cred.psk_identity = g_pskId;
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

		data.tls_context = TLSCtx(&cred);
		if (data.tls_context == NULL) {
			printf("TLS context initialize failed\n");
			goto exit;
		}
	}
#endif
	if ((lwm2mH = lwm2m_init(&data)) == NULL) {
		printf("lwm2m_init2() failed\n");
		goto exit;
	}
	lwm2mH->protocol = g_proto;

	if ((result = lwm2m_configure(lwm2mH, name, NULL, NULL, OBJ_COUNT, objArray)) != 0) {
		printf("lwm2m_configure() failed: 0x%X\n", result);
		goto exit;
	}

	/* Register the command line command */
	for (i = 0 ; commands[i].name != NULL ; i++) {
		commands[i].userData = (void *)lwm2mH;
	}
	printf("LWM2M Client \"%s\" started on port %s\n>  ", name, serverPort);

	/* We now enter in a while loop that will handle the communications from the server */
	while (0 == g_quit) {
		tv.tv_sec = 60;
		tv.tv_usec = 0;

		if ((result = lwm2m_step(lwm2mH, &(tv.tv_sec))) != 0) {
			printf("lwm2m_step() failed: 0x%X\n", result);
			goto exit;
		} else {
			printf(" -> State: %s\n", g_step[lwm2mH->state]);
		}

		fd_set readfds;
		FD_ZERO(&readfds);
		FD_SET(data.sock, &readfds);
		FD_SET(STDIN_FILENO, &readfds);

		result = select(FD_SETSIZE, &readfds, NULL, NULL, &tv);
		if (result < 0) {
			if (errno != EINTR) {
				fprintf(stderr, "Error in select(): %d %s\r\n", errno, strerror(errno));
			}
		} else if (result > 0) {
			uint8_t buffer[MAX_PACKET_SIZE];
			int numBytes;

			if (FD_ISSET(data.sock, &readfds)) {
				client_data_t *user_data = lwm2mH->userData;

				numBytes = connection_read(g_proto, user_data->connP, data.sock, buffer, MAX_PACKET_SIZE, NULL, 0);

				if (numBytes > 0) {
					output_buffer(stderr, buffer, numBytes, 0);
					lwm2m_handle_packet(lwm2mH, buffer, numBytes, user_data->connP);
					conn_s_updateRxStatistic(objArray[7], numBytes, false);
				} else {
					printf("received bytes ignored!\n");
				}
				/* If the event happened on the SDTIN */
			} else if (FD_ISSET(STDIN_FILENO, &readfds)) {
				numBytes = read_input_command_line((char *)buffer);
				if (numBytes > 1) {
					buffer[numBytes] = 0;
					handle_command(commands, (char *)buffer);
				}
				if (g_quit == 0) {
					fprintf(stdout, "\r\n> ");
					fflush(stdout);
				}
			}
		}
	}

exit:
	if (g_quit && lwm2mH) {
		lwm2m_close(lwm2mH);
		sleep(1);
	}

#ifdef WITH_MBEDTLS
	if (data.tls_context) {
		TLSCtx_free(data.tls_context);
	}
#endif

	if (data.sock >= 0) {
		printf("Closing %d\n", data.sock);
		shutdown(data.sock, 3);
		if ((i = close(data.sock)) != 0) {
			printf("Fail to close %d\n", errno);
		}
	}

	if (data.connP) {
		connection_free(data.connP);
	}

	clean_security_object(objArray[0]);
	clean_server_object(objArray[1]);
	free_object_device(objArray[2]);
	free_object_firmware(objArray[3]);
	free_object_location(objArray[4]);
	free_test_object(objArray[5]);
	free_object_conn_m(objArray[6]);
	free_object_conn_s(objArray[7]);
	acl_ctrl_free_object(objArray[8]);
	lwm2m_free(objArray[0]);
	lwm2m_free(objArray[1]);

	return 0;
}

int lwm2m_client_main(int argc, char *argv[])
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
	sparam.sched_priority = LWM2M_CLIENT_PRIORITY;
	if ((r = pthread_attr_setschedparam(&attr, &sparam)) != 0) {
		printf("%s: pthread_attr_setschedparam failed, status=%d\n", __func__, r);
		return -1;
	}

	if ((r = pthread_attr_setschedpolicy(&attr, LWM2M_CLIENT_SCHED_POLICY)) != 0) {
		printf("%s: pthread_attr_setschedpolicy failed, status=%d\n", __func__, r);
		return -1;
	}

	/* 2. set a stacksize */
	if ((r = pthread_attr_setstacksize(&attr, LWM2M_CLIENT_STACK_SIZE)) != 0) {
		printf("%s: pthread_attr_setstacksize failed, status=%d\n", __func__, r);
		return -1;
	}

	/* 3. create pthread with entry function */
	if ((r = pthread_create(&tid, &attr, (pthread_startroutine_t)lwm2m_client_cb, (void *)&args)) != 0) {
		printf("%s: pthread_create failed, status=%d\n", __func__, r);
		return -1;
	}

	/* Wait for the threads to stop */
	pthread_join(tid, NULL);

	return 0;
}
