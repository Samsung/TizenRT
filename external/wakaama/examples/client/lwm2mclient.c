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

#include "lwm2mclient.h"
#include <protocols/liblwm2m.h>
#include "commandline.h"
#ifdef WITH_TINYDTLS
#include "dtlsconnection.h"
#else
#include "connection.h"
#endif

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
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

#define MAX_PACKET_SIZE 1024
#define DEFAULT_SERVER_IPV6 "[::1]"
#define DEFAULT_SERVER_IPV4 "127.0.0.1"
#define IPADDRLEN_MAX  32
#define PORTLEN_MAX 6
#ifndef STDIN_FILENO
#  define STDIN_FILENO 0
#endif

int g_reboot = 0;
static int g_quit = 0;

static char g_serverAddr[IPADDRLEN_MAX];
static char g_serverPort[PORTLEN_MAX];
static char g_bootstrapserverAddr[IPADDRLEN_MAX];
static char g_bootstrapserverPort[PORTLEN_MAX];
static uint16_t g_lifetime;
bool g_bootstrapRequested;

#define OBJ_COUNT 9
lwm2m_object_t *objArray[OBJ_COUNT];

// only backup security and server objects
# define BACKUP_OBJECT_COUNT 2
lwm2m_object_t *backupObjectArray[BACKUP_OBJECT_COUNT];

typedef struct {
	lwm2m_object_t *securityObjP;
	lwm2m_object_t *serverObject;
	int sock;
#ifdef WITH_TINYDTLS
	dtls_connection_t *connList;
	lwm2m_context_t *lwm2mH;
#else
	connection_t *connList;
#endif
	int addressFamily;
} client_data_t;

lwm2m_context_t *lwm2mH;
client_data_t data;

static void clear_client_globals(void)
{
	memset(g_bootstrapserverAddr, 0x00, IPADDRLEN_MAX);
	memset(g_bootstrapserverPort, 0x00, 6);
	if (lwm2mH->state == STATE_REGISTER_REQUIRED        ||
		lwm2mH->state == STATE_REGISTERING              ||
		lwm2mH->state == STATE_READY) {
		printf("g_bootstrapRequested = false\n");
		g_bootstrapRequested = false;
	}
}

static void prv_update_server(client_data_t *dataP,
							  uint16_t secObjInstID)
{
	char *uri;
	char *server_host;
	char *server_port;

	uri = get_server_uri(dataP->securityObjP, secObjInstID);

	if (uri == NULL) {
		return;
	}

	// parse uri in the form "coaps://[server_host]:[port]"
	if (0 == strncmp(uri, "coaps://", strlen("coaps://"))) {
		server_host = uri + strlen("coaps://");
	} else if (0 == strncmp(uri, "coap://",  strlen("coap://"))) {
		server_host = uri + strlen("coap://");
	} else {
		return;
	}
	server_port = strrchr(server_host, ':');
	if (server_port == NULL) {
		return;
	}
	// remove brackets
	if (server_host[0] == '[') {
		server_host++;
		if (*(server_port - 1) == ']') {
			*(server_port - 1) = 0;
		} else {
			return;
		}
	}
	// split strings
	*server_port = 0;
	server_port++;

	client_set_serverAddr(server_host, false);
	client_set_serverPort(server_port, false);
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

void handle_value_changed(lwm2m_context_t *lwm2mP,
						  lwm2m_uri_t *uri,
						  const char *value,
						  size_t valueLength)
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

#ifdef WITH_TINYDTLS
void *lwm2m_connect_server(uint16_t secObjInstID,
						   void *userData)
{
	client_data_t *dataP;
	lwm2m_list_t *instance;
	dtls_connection_t *newConnP = NULL;
	dataP = (client_data_t *)userData;
	lwm2m_object_t   *securityObj = dataP->securityObjP;

	instance = LWM2M_LIST_FIND(dataP->securityObjP->instanceList, secObjInstID);
	if (instance == NULL) {
		return NULL;
	}


	newConnP = connection_create(COAP_UDP, dataP->connList, dataP->sock, securityObj, instance->id, dataP->lwm2mH, dataP->addressFamily);
	if (newConnP == NULL) {
		fprintf(stderr, "Connection creation failed.\n");
		return NULL;
	}

	dataP->connList = newConnP;
	return (void *)newConnP;
}
#else /*  WITH_TINYDTLS */
void *lwm2m_connect_server(uint16_t secObjInstID,
						   void *userData)
{
	client_data_t *dataP;
	char *uri;
	char *host;
	char *port;
	connection_t *newConnP = NULL;

	dataP = (client_data_t *)userData;

	uri = get_server_uri(dataP->securityObjP, secObjInstID);

	if (uri == NULL) {
		return NULL;
	}

	// parse uri in the form "coaps://[host]:[port]"
	if (0 == strncmp(uri, "coaps://", strlen("coaps://"))) {
		host = uri + strlen("coaps://");
	} else if (0 == strncmp(uri, "coap://",  strlen("coap://"))) {
		host = uri + strlen("coap://");
	} else {
		goto exit;
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

	fprintf(stderr, "Opening connection to server at %s:%s\r\n", host, port);
	newConnP = connection_create(COAP_UDP, dataP->connList, dataP->sock, host, port, dataP->addressFamily);
	if (newConnP == NULL) {
		fprintf(stderr, "Connection creation failed.\r\n");
	} else {
		dataP->connList = newConnP;
	}

exit:
	lwm2m_free(uri);
	return (void *)newConnP;
}
#endif /*  WITH_TINYDTLS */

void lwm2m_close_connection(void *sessionH,
							void *userData)
{
	client_data_t *app_data;
#ifdef WITH_TINYDTLS
	dtls_connection_t *targetP;
#else
	connection_t *targetP;
#endif

	app_data = (client_data_t *)userData;
#ifdef WITH_TINYDTLS
	targetP = (dtls_connection_t *)sessionH;
#else
	targetP = (connection_t *)sessionH;
#endif

	if (targetP == app_data->connList) {
		app_data->connList = targetP->next;
		lwm2m_free(targetP);
	} else {
#ifdef WITH_TINYDTLS
		dtls_connection_t *parentP;
#else
		connection_t *parentP;
#endif

		parentP = app_data->connList;
		while (parentP != NULL && parentP->next != targetP) {
			parentP = parentP->next;
		}
		if (parentP != NULL) {
			parentP->next = targetP->next;
			lwm2m_free(targetP);
		}
	}
}

static void prv_output_servers(char *buffer,
							   void *user_data)
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

static void prv_object_list(char *buffer,
							void *user_data)
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

static void prv_instance_dump(lwm2m_object_t *objectP,
							  uint16_t id)
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


static void prv_object_dump(char *buffer,
							void *user_data)
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

static void prv_update(char *buffer,
					   void *user_data)
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

static void prv_add(char *buffer,
					void *user_data)
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
	} else {
		fprintf(stdout, "Object 1024 added.\r\n");
	}
	return;
}

static void prv_remove(char *buffer,
					   void *user_data)
{
	lwm2mH = (lwm2m_context_t *)user_data;
	int res;

	res = lwm2m_remove_object(lwm2mH, 1024);
	if (res != 0) {
		fprintf(stdout, "Removing object 1024 failed: ");
		print_status(stdout, res);
		fprintf(stdout, "\r\n");
	} else {
		fprintf(stdout, "Object 1024 removed.\r\n");
	}
	return;
}

#ifdef LWM2M_BOOTSTRAP

static void prv_initiate_bootstrap(char *buffer,
								   void *user_data)
{
	lwm2mH = (lwm2m_context_t *)user_data;
	lwm2m_server_t *targetP;

	// HACK !!!
	lwm2mH->state = STATE_BOOTSTRAP_REQUIRED;
	targetP = lwm2mH->bootstrapServerList;
	while (targetP != NULL) {
		targetP->lifetime = 0;
		targetP = targetP->next;
	}
}

static void prv_display_objects(char *buffer,
								void *user_data)
{
	lwm2mH = (lwm2m_context_t *)user_data;
	lwm2m_object_t *object;

	for (object = lwm2mH->objectList; object != NULL; object = object->next) {
		if (NULL != object) {
			switch (object->objID) {
			case LWM2M_SECURITY_OBJECT_ID:
				display_security_object(object);
				break;
			case LWM2M_SERVER_OBJECT_ID:
				display_server_object(object);
				break;
			case LWM2M_ACL_OBJECT_ID:
				break;
			case LWM2M_DEVICE_OBJECT_ID:
				display_device_object(object);
				break;
			case LWM2M_CONN_MONITOR_OBJECT_ID:
				break;
			case LWM2M_FIRMWARE_UPDATE_OBJECT_ID:
				display_firmware_object(object);
				break;
			case LWM2M_LOCATION_OBJECT_ID:
				display_location_object(object);
				break;
			case LWM2M_CONN_STATS_OBJECT_ID:
				break;
			case TEST_OBJECT_ID:
				display_test_object(object);
				break;
			}
		}
	}
}

static void prv_display_backup(char *buffer,
							   void *user_data)
{
		int i;
		for (i = 0 ; i < BACKUP_OBJECT_COUNT ; i++) {
			lwm2m_object_t *object = backupObjectArray[i];
			if (NULL != object) {
				switch (object->objID) {
				case LWM2M_SECURITY_OBJECT_ID:
					display_security_object(object);
					break;
				case LWM2M_SERVER_OBJECT_ID:
					display_server_object(object);
					break;
				default:
					break;
				}
			}
		}
	}

static void prv_backup_objects(lwm2m_context_t *context)
{
	uint16_t i;

	for (i = 0; i < BACKUP_OBJECT_COUNT; i++) {
		if (NULL != backupObjectArray[i]) {
			switch (backupObjectArray[i]->objID) {
			case LWM2M_SECURITY_OBJECT_ID:
				clean_security_object(backupObjectArray[i]);
				lwm2m_free(backupObjectArray[i]);
				break;
			case LWM2M_SERVER_OBJECT_ID:
				clean_server_object(backupObjectArray[i]);
				lwm2m_free(backupObjectArray[i]);
				break;
			default:
				break;
			}
		}
		backupObjectArray[i] = (lwm2m_object_t *)lwm2m_malloc(sizeof(lwm2m_object_t));
		memset(backupObjectArray[i], 0, sizeof(lwm2m_object_t));
	}

	/*
	 * Backup content of objects 0 (security) and 1 (server)
	 */
	copy_security_object(backupObjectArray[0], (lwm2m_object_t *)LWM2M_LIST_FIND(context->objectList, LWM2M_SECURITY_OBJECT_ID));
	copy_server_object(backupObjectArray[1], (lwm2m_object_t *)LWM2M_LIST_FIND(context->objectList, LWM2M_SERVER_OBJECT_ID));
}

static void prv_restore_objects(lwm2m_context_t *context)
{
	lwm2m_object_t *targetP;

	/*
	 * Restore content  of objects 0 (security) and 1 (server)
	 */
	targetP = (lwm2m_object_t *)LWM2M_LIST_FIND(context->objectList, LWM2M_SECURITY_OBJECT_ID);
	// first delete internal content
	clean_security_object(targetP);
	// then restore previous object
	copy_security_object(targetP, backupObjectArray[0]);

	targetP = (lwm2m_object_t *)LWM2M_LIST_FIND(context->objectList, LWM2M_SERVER_OBJECT_ID);
	// first delete internal content
	clean_server_object(targetP);
	// then restore previous object
	copy_server_object(targetP, backupObjectArray[1]);

	// restart the old servers
	fprintf(stdout, "[BOOTSTRAP] ObjectList restored\r\n");
}

static void update_bootstrap_info(lwm2m_client_state_t *previousBootstrapState,
								  lwm2m_context_t *context)
{
	if (*previousBootstrapState != context->state) {
		*previousBootstrapState = context->state;
		switch (context->state) {
		case STATE_BOOTSTRAPPING:
#ifdef WITH_LOGS
			fprintf(stdout, "[BOOTSTRAP] backup security and server objects\r\n");
#endif
			prv_backup_objects(context);
			break;
		default:
			break;
		}
	}
}

static void close_backup_object()
{
	int i;
	for (i = 0; i < BACKUP_OBJECT_COUNT; i++) {
		if (NULL != backupObjectArray[i]) {
			switch (backupObjectArray[i]->objID) {
			case LWM2M_SECURITY_OBJECT_ID:
				clean_security_object(backupObjectArray[i]);
				lwm2m_free(backupObjectArray[i]);
				break;
			case LWM2M_SERVER_OBJECT_ID:
				clean_server_object(backupObjectArray[i]);
				lwm2m_free(backupObjectArray[i]);
				break;
			default:
				break;
			}
		}
	}
}
#endif

void print_usage(void)
{
	fprintf(stdout, "Usage: lwm2mclient [OPTION]\r\n");
	fprintf(stdout, "Launch a LWM2M client.\r\n");
	fprintf(stdout, "Options:\r\n");
	fprintf(stdout, "  -n NAME\tSet the endpoint name of the Client. Default: testlwm2mclient\r\n");
	fprintf(stdout, "  -l PORT\tSet the local UDP port of the Client. Default: 56830\r\n");
	fprintf(stdout, "  -h HOST\tSet the hostname of the LWM2M Server to connect to. Default: localhost\r\n");
	fprintf(stdout, "  -p PORT\tSet the port of the LWM2M Server to connect to. Default: "LWM2M_STANDARD_PORT_STR"\r\n");
	fprintf(stdout, "  -4\t\tUse IPv4 connection. Default: IPv6 connection\r\n");
	fprintf(stdout, "  -t TIME\tSet the lifetime of the Client. Default: 300\r\n");
	fprintf(stdout, "  -b\t\tBootstrap requested.\r\n");
	fprintf(stdout, "  -c\t\tChange battery level over time.\r\n");
#ifdef WITH_TINYDTLS
	fprintf(stdout, "  -i STRING\tSet the device management or bootstrap server PSK identity. If not set use none secure mode\r\n");
	fprintf(stdout, "  -s HEXSTRING\tSet the device management or bootstrap server Pre-Shared-Key. If not set use none secure mode\r\n");
#endif
	fprintf(stdout, "\r\n");
}

/****************************************************************************
 * API support
 ****************************************************************************/
char *client_set_serverAddr(FAR const char *serverAddr,
							bool isbootstrap)
{
	if (serverAddr) {
		if (isbootstrap) {
			memset(g_bootstrapserverAddr, 0x00, IPADDRLEN_MAX);
			strncpy(g_bootstrapserverAddr, serverAddr, IPADDRLEN_MAX);
			return g_bootstrapserverAddr;
		} else {
			memset(g_serverAddr, 0x00, IPADDRLEN_MAX);
			strncpy(g_serverAddr, serverAddr, IPADDRLEN_MAX);
			return g_serverAddr;
		}
	} else {
		return NULL;
	}

}

char *client_get_serverAddr(void)
{
	return g_serverAddr;
}


char *client_set_serverPort(FAR const char *serverPort,
							bool isbootstrap)
{
	if (serverPort) {
		if (isbootstrap) {
			memset(g_bootstrapserverPort, 0x00, PORTLEN_MAX);
			strncpy(g_bootstrapserverPort, serverPort, PORTLEN_MAX);
			return g_bootstrapserverPort;
		} else {
			memset(g_serverPort, 0x00, PORTLEN_MAX);
			strncpy(g_serverPort, serverPort, PORTLEN_MAX);
			return g_serverPort;
		}
	} else {
		return NULL;
	}
}


char *client_get_serverPort(void)
{
	return g_serverPort;

}


int client_set_lifetime(int lifetime)
{
	g_lifetime = lifetime;
	return g_lifetime;
}

void client_close(void)
{
	prv_quit(NULL, NULL);
}

void client_initiate_bootstrap(void)
{
	g_bootstrapRequested = true;
}

int client_change_resource(char *buffer, void *user_data)
{
	prv_change(buffer, user_data);
	return 0;
}

static void process_udpconn(int sockfd, fd_set *readfds)
{
	uint8_t buffer[MAX_PACKET_SIZE];
	int numBytes;

	if (FD_ISSET(sockfd, readfds)) {
		struct sockaddr_storage addr;
		socklen_t addrLen;

		addrLen = sizeof(addr);
		printf("event happens, trying recvfrom\n");
		usleep(20000);

		/*
		* We retrieve the data received
		*/
		numBytes = recvfrom(sockfd, buffer, MAX_PACKET_SIZE, 0, (struct sockaddr *)&addr, &addrLen);
		printf("received %d bytes packet\n", numBytes);
		usleep(20000);
		if (0 > numBytes) {
			fprintf(stderr, "Error in recvfrom(): %d %s\r\n", errno, strerror(errno));
		} else if (0 < numBytes) {
			char s[INET6_ADDRSTRLEN];
			in_port_t port = 0;

#ifdef WITH_TINYDTLS
			dtls_connection_t *connP;
#else
			connection_t *connP;
#endif
			if (AF_INET == addr.ss_family) {
				struct sockaddr_in *saddr = (struct sockaddr_in *)&addr;
				inet_ntop(saddr->sin_family, &saddr->sin_addr, s, INET6_ADDRSTRLEN);
				port = saddr->sin_port;
			} else if (AF_INET6 == addr.ss_family) {
				struct sockaddr_in6 *saddr = (struct sockaddr_in6 *)&addr;
				inet_ntop(saddr->sin6_family, &saddr->sin6_addr, s, INET6_ADDRSTRLEN);
				port = saddr->sin6_port;
			}
			fprintf(stderr, "%d bytes received from [%s]:%hu\r\n", numBytes, s, ntohs(port));

			/*
			* Display it in the STDERR
			*/
			output_buffer(stderr, buffer, numBytes, 0);

			connP = connection_find(data.connList, &addr, addrLen);
			if (connP != NULL) {
				/*
				* Let liblwm2m respond to the query depending on the context
				*/
#ifdef WITH_TINYDTLS
				int result = connection_handle_packet(connP, buffer, numBytes);
				if (0 != result) {
					printf("error handling message %d\n", result);
				}
#else
				lwm2m_handle_packet(lwm2mH, buffer, numBytes, connP);
#endif
				conn_s_updateRxStatistic(objArray[7], numBytes, false);
			} else {
				fprintf(stderr, "received bytes ignored!\r\n");
			}
		}
	}
}

static void prv_close_sock(void)
{
	connection_t *connPtr = NULL;

	connPtr = ((client_data_t *)(lwm2mH->userData))->connList;

	while (connPtr != NULL) {
		close(connPtr->sock);
		connPtr = connPtr->next;
	}
}

int client_main(void)
{
	int result;
	int i;

	const char *localPort = "56830";
#if !defined(CONFIG_DM)
	const char *server = "192.168.0.4";
	const char *serverPort = LWM2M_STANDARD_PORT_STR;
	char *name = "testlwm2mclient";
	int lifetime = 300;
#else
	char *server = g_serverAddr;
	char *serverPort = g_serverPort;
	char *name = "myawesomeclient";
	int lifetime = g_lifetime;
#endif
	int batterylevelchanging = 0;
	time_t reboot_time = 0;
	bool bootstrapRequested = g_bootstrapRequested;

#if !defined(CONFIG_DM)
	int opt;
	bool serverPortChanged = false;
#endif

	if (bootstrapRequested) {
		server = g_bootstrapserverAddr;
		serverPort = g_bootstrapserverPort;
	} else {
		server = g_serverAddr;
		serverPort = g_serverPort;
	}

	lwm2m_client_state_t previousState = STATE_INITIAL;

	char *pskId = NULL;
	char *psk;
	uint16_t pskLen = -1;
	char *pskBuffer = NULL;

	psk = NULL;

	/*
	 * The function start by setting up the command line interface (which may or not be useful depending on your project)
	 *
	 * This is an array of commands describes as { name, description, long description, callback, userdata }.
	 * The firsts tree are easy to understand, the callback is the function that will be called when this command is typed
	 * and in the last one will be stored the lwm2m context (allowing access to the server settings and the objects).
	 */
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
#ifdef LWM2M_BOOTSTRAP
		{"bootstrap", "Initiate a DI bootstrap process", NULL, prv_initiate_bootstrap, NULL},
		{
			"dispb", "Display current backup of objects/instances/resources\r\n"
			"\t(only security and server objects are backupped)", NULL, prv_display_backup, NULL
		},
#endif
		{"ls", "List Objects and Instances", NULL, prv_object_list, NULL},
#ifdef LWM2M_BOOTSTRAP
		{"disp", "Display current objects/instances/resources", NULL, prv_display_objects, NULL},
#endif
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
#if defined(CONFIG_DM)
	printf("Inside %s check serial\n", __func__);
#else
	opt = 1;
	while (opt < argc) {
		if (argv[opt] == NULL
			|| argv[opt][0] != '-'
			|| argv[opt][2] != 0) {
			print_usage();
			return -1;
		}
		switch (argv[opt][1]) {
		case 'b':
			bootstrapRequested = true;
			if (!serverPortChanged) {
				serverPort = LWM2M_BSSERVER_PORT_STR;
			}
			break;
		case 'c':
			batterylevelchanging = 1;
			break;
		case 't':
			opt++;
			if (opt >= argc) {
				print_usage();
				return -1;
			}
			if (1 != sscanf(argv[opt], "%d", &lifetime)) {
				print_usage();
				return -1;
			}
			break;
#ifdef WITH_TINYDTLS
		case 'i':
			opt++;
			if (opt >= argc) {
				print_usage();
				return -1;
			}
			pskId = argv[opt];
			break;
		case 's':
			opt++;
			if (opt >= argc) {
				print_usage();
				return -1;
			}
			psk = argv[opt];
			break;
#endif
		case 'n':
			opt++;
			if (opt >= argc) {
				print_usage();
				return -1;
			}
			name = argv[opt];
			break;
		case 'l':
			opt++;
			if (opt >= argc) {
				print_usage();
				return -1;
			}
			localPort = argv[opt];
			break;
		case 'h':
			opt++;
			if (opt >= argc) {
				print_usage();
				return -1;
			}
			server = argv[opt];
			break;
		case 'p':
			opt++;
			if (opt >= argc) {
				print_usage();
				return -1;
			}
			serverPort = argv[opt];
			serverPortChanged = true;
			break;
		case '4':
			data.addressFamily = AF_INET;
			break;
		default:
			print_usage();
			return -1;
		}
		opt += 1;
	}
#endif /* CONFIG_DM */
	if (!server) {
		server = (AF_INET == data.addressFamily ? DEFAULT_SERVER_IPV4 : DEFAULT_SERVER_IPV6);
	}

	/*
	 *This call an internal function that create an IPV6 socket on the port 5683.
	 */
	fprintf(stderr, "Trying to bind LWM2M Client to port %s\r\n", localPort);
	data.sock = create_socket(COAP_UDP, localPort, data.addressFamily);
	if (data.sock < 0) {
		fprintf(stderr, "Failed to open socket: %d %s\r\n", errno, strerror(errno));
		return -1;
	}

	/*
	 * Now the main function fill an array with each object, this list will be later passed to liblwm2m.
	 * Those functions are located in their respective object file.
	 */
#ifdef WITH_TINYDTLS
	if (psk != NULL) {
		pskLen = strlen(psk) / 2;
		pskBuffer = malloc(pskLen);

		if (NULL == pskBuffer) {
			fprintf(stderr, "Failed to create PSK binary buffer\r\n");
			return -1;
		}
		// Hex string to binary
		char *h = psk;
		char *b = pskBuffer;
		char xlate[] = "0123456789ABCDEF";

		for (; *h; h += 2, ++b) {
			char *l = strchr(xlate, toupper(*h));
			char *r = strchr(xlate, toupper(*(h + 1)));

			if (!r || !l) {
				fprintf(stderr, "Failed to parse Pre-Shared-Key HEXSTRING\r\n");
				return -1;
			}

			*b = ((l - xlate) << 4) + (r - xlate);
		}
	}
#endif

	char serverUri[50];
	int serverId = 123;
	sprintf(serverUri, "coap://%s:%s", server, serverPort);
#ifdef LWM2M_BOOTSTRAP
	objArray[0] = get_security_object(serverId, serverUri, pskId, pskBuffer, pskLen, bootstrapRequested);
#else
	objArray[0] = get_security_object(serverId, serverUri, pskId, pskBuffer, pskLen, false);
#endif
	if (NULL == objArray[0]) {
		fprintf(stderr, "Failed to create security object\r\n");
		return -1;
	}
	data.securityObjP = objArray[0];

	objArray[1] = get_server_object(serverId, "U", lifetime, false);
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
	/*
	 * The liblwm2m library is now initialized with the functions that will be in
	 * charge of communication
	 */
	lwm2mH = lwm2m_init(&data);
	if (NULL == lwm2mH) {
		fprintf(stderr, "lwm2m_init() failed\r\n");
		return -1;
	}

#ifdef WITH_TINYDTLS
	data.lwm2mH = lwm2mH;
#endif

	/*
	 * We configure the liblwm2m library with the name of the client - which shall be unique for each client -
	 * the number of objects we will be passing through and the objects array
	 */
	result = lwm2m_configure(lwm2mH, name, NULL, NULL, OBJ_COUNT, objArray);
	if (result != 0) {
		fprintf(stderr, "lwm2m_configure() failed: 0x%X\r\n", result);
		return -1;
	}

	//signal(SIGINT, handle_sigint);
	printf("Instantiated objects\n");

	/**
	 * Initialize value changed callback.
	 */
	init_value_change(lwm2mH);

	/*
	 * As you now have your lwm2m context complete you can pass it as an argument to all the command line functions
	 * precedently viewed (first point)
	 */
	for (i = 0 ; commands[i].name != NULL ; i++) {
		commands[i].userData = (void *)lwm2mH;
	}
	fprintf(stdout, "LWM2M Client \"%s\" started on port %s\r\n", name, localPort);
	fprintf(stdout, "> ");
	fflush(stdout);
	/*
	 * We now enter in a while loop that will handle the communications from the server
	 */
	while (0 == g_quit) {
		struct timeval tv;
		fd_set readfds;
		connection_t *connPtr = NULL;

		struct timespec tv1;
		struct timespec tv2;
		int ret_timestamp;

		if (g_reboot) {
			time_t tv_sec;

			tv_sec = lwm2m_gettime();

			if (0 == reboot_time) {
				reboot_time = tv_sec + 5;
			}
			if (reboot_time < tv_sec) {
				/*
				 * Message should normally be lost with reboot ...
				 */
				fprintf(stderr, "reboot time expired, rebooting ...");
				system_reboot();
			} else {
				tv.tv_sec = reboot_time - tv_sec;
			}
		} else if (batterylevelchanging) {
			update_battery_level(lwm2mH);
			tv.tv_sec = 5;
		} else {
			tv.tv_sec = 60;
		}
		tv.tv_usec = 0;

		FD_ZERO(&readfds);
		connPtr = ((client_data_t *)(lwm2mH->userData))->connList;
		while (connPtr != NULL) {
			printf("setting %d\n", connPtr->sock);
			FD_SET(connPtr->sock, &readfds);
			connPtr = connPtr->next;
		}

		ret_timestamp = clock_gettime(CLOCK_REALTIME, &tv1);
		printf("checking lwm2m_step\n");
		usleep(20000);

		/*
		 * This function does two things:
		 *  - first it does the work needed by liblwm2m (eg. (re)sending some packets).
		 *  - Secondly it adjusts the timeout value (default 60s) depending on the state of the transaction
		 *    (eg. retransmission) and the time between the next operation
		 */
		result = lwm2m_step(lwm2mH, &(tv.tv_sec));
		fprintf(stdout, " -> State: ");
		switch (lwm2mH->state) {
		case STATE_INITIAL:
			fprintf(stdout, "STATE_INITIAL\r\n");
			break;
		case STATE_BOOTSTRAP_REQUIRED:
			fprintf(stdout, "STATE_BOOTSTRAP_REQUIRED\r\n");
			break;
		case STATE_BOOTSTRAPPING:
			fprintf(stdout, "STATE_BOOTSTRAPPING\r\n");
			break;
		case STATE_REGISTER_REQUIRED:
			fprintf(stdout, "STATE_REGISTER_REQUIRED\r\n");
			break;
		case STATE_REGISTERING:
			fprintf(stdout, "STATE_REGISTERING\r\n");
			break;
		case STATE_READY:
			fprintf(stdout, "STATE_READY\r\n");
			break;
		default:
			fprintf(stdout, "Unknown...\r\n");
			break;
		}
		if (result != 0) {
			fprintf(stderr, "lwm2m_step() failed: 0x%X\r\n", result);
			if (previousState == STATE_BOOTSTRAPPING) {
#ifdef WITH_LOGS
				fprintf(stdout, "[BOOTSTRAP] restore security and server objects\r\n");
#endif
				prv_restore_objects(lwm2mH);
				lwm2mH->state = STATE_INITIAL;
			} else {
				return -1;
			}
		}
#ifdef LWM2M_BOOTSTRAP
		update_bootstrap_info(&previousState, lwm2mH);
#endif
		printf("got connection, now trying select %d:%d, %lu:%lu\n", data.sock + 1, CONFIG_NSOCKET_DESCRIPTORS,
			   tv.tv_sec, tv.tv_usec);
		usleep(20000);
		/*
		 * This part will set up an interruption until an event happen on SDTIN or the socket until "tv" timed out (set
		 * with the precedent function)
		 */
		result = select(CONFIG_NFILE_DESCRIPTORS +
						CONFIG_NSOCKET_DESCRIPTORS, &readfds,
						NULL, NULL, &tv);
		ret_timestamp = clock_gettime(CLOCK_REALTIME, &tv2);
		printf("select exits with value %d\n", result);
		usleep(20000);
		printf("time diff %lu:%lu\n", tv2.tv_sec - tv1.tv_sec,
			   tv2.tv_nsec - tv1.tv_nsec);
		usleep(20000);
		if (result < 0) {
			if (errno != EINTR) {
				fprintf(stderr, "Error in select(): %d %s\r\n", errno, strerror(errno));
			}
		} else if (result > 0) {
			/*
			 * If an event happens on the socket
			 */
			connPtr = ((client_data_t *)(lwm2mH->userData))->connList;
			while (connPtr != NULL) {
				process_udpconn(connPtr->sock, &readfds);
				connPtr = connPtr->next;
			}
		}
	}

	/*
	 * Finally when the loop is left smoothly - asked by user in the command line interface - we unregister our client from it
	 */
	if (g_quit == 1) {
#ifdef LWM2M_BOOTSTRAP
		if (g_bootstrapRequested == true) {
			close_backup_object();
		}
#endif
		clear_client_globals();
		lwm2m_close(lwm2mH);
	}

	close(data.sock);
	prv_close_sock();
	connection_free(data.connList);

	clean_security_object(objArray[0]);
	lwm2m_free(objArray[0]);
	clean_server_object(objArray[1]);
	lwm2m_free(objArray[1]);
	free_object_device(objArray[2]);
	free_object_firmware(objArray[3]);
	free_object_location(objArray[4]);
	free_test_object(objArray[5]);
	free_object_conn_m(objArray[6]);
	free_object_conn_s(objArray[7]);
	acl_ctrl_free_object(objArray[8]);

#ifdef MEMORY_TRACE
	if (g_quit == 1) {
		trace_print(0, 1);
	}
#endif


	printf("Exiting wakaama client thread\n");
	g_quit = 0;
	return 0;
}

lwm2m_context_t *wrapper_get_context(void)
{
	return lwm2mH;
}

int client_object_dump(char *buffer)
{
	if (lwm2mH != NULL) {
		prv_object_dump(buffer, lwm2mH);
		return 0;
	} else {
		fprintf(stderr, "Error calling object dump\n");
	}
	return -1;
}
