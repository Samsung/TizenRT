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
#include "liblwm2m.h"
#include "commandline.h"
#include "connection.h"

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

#if defined (__TINYARA__)
#include <tinyara/ascii.h>
#endif

#ifdef WITH_MBEDTLS
#include "tls/certs.h"
#define LWM2M_CIPHERSUIT "TLS-PSK-WITH-AES-128-CBC-SHA"
#endif

#define MAX_PACKET_SIZE 1024
#define DEFAULT_SERVER_IPV6 "[::1]"
#define DEFAULT_SERVER_IPV4 "coap://127.0.0.1"

/****************************************************************************
 * TINYARA
 *   - definition and global variables
 *   - APIs used for dm frameworks
 ****************************************************************************/
#if defined(__TINYARA__)

/* Definitions*/
#ifndef FD_SETSIZE
#define FD_SETSIZE	(CONFIG_NFILE_DESCRIPTORS + CONFIG_NSOCKET_DESCRIPTORS)
#endif

#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif
#ifndef STDOUT_FILENO
#define STDOUT_FILENO 1
#endif
#ifndef STDERR_FILENO
#define STDERR_FILENO 2
#endif

#define SET_PARAM_BY_GLOBAL_VALUE -10

#define DEFAULT_CLIENT_NAME "myawesomeclient"
#define IPADDRLEN_MAX  32
#define PORTLEN_MAX 6

/* Global variables */
static char g_serverAddr[IPADDRLEN_MAX] = {0,};
static char g_serverPort[PORTLEN_MAX] = {0,};
static char g_bootstrapserverAddr[IPADDRLEN_MAX] = {0,};
static char g_bootstrapserverPort[PORTLEN_MAX] = {0,};
static uint16_t g_lifetime = 300;
bool g_bootstrapRequested = false;

/*
 * Definition for handling pthread
 */
#define LWM2M_CLIENT_PRIORITY     100
#define LWM2M_CLIENT_STACK_SIZE   16384
#define LWM2M_CLIENT_SCHED_POLICY SCHED_RR

struct pthread_arg {
    int argc;
    char **argv;
};

#endif /* __TINYARA__ */

int g_reboot = 0;
static int g_quit = 0;

static char coap_uri_prefix [COAP_PROTOCOL_MAX][COAP_MAX_URI_PREFIX_SIZE] = {
  "coap://",
  "coaps://",
  "coap+tcp://",
  "coaps+tcp://"
};

lwm2m_context_t * lwm2mH;

#define OBJ_COUNT 9
lwm2m_object_t * objArray[OBJ_COUNT];

// only backup security and server objects
# define BACKUP_OBJECT_COUNT 2
lwm2m_object_t * backupObjectArray[BACKUP_OBJECT_COUNT];

typedef struct
{
    lwm2m_object_t * securityObjP;
    lwm2m_object_t * serverObject;
    int sock;
    struct sockaddr_storage server_addr;
    size_t server_addrLen;
    int addressFamily;
    connection_t * connList;
#ifdef WITH_MBEDTLS
    tls_ctx	* tls_context;
    tls_opt * tls_opt;
#endif
} client_data_t;

#if defined (__TINYARA__)
/* Private Functions */
static void clear_client_globals(void);
static void prv_close_sock(void);
static void prv_update_server(client_data_t *dataP, uint16_t secObjInstID);
static void process_udpconn(int sockfd, fd_set *readfds, client_data_t data);

static int read_input_command_line(char *buf);
#endif /*__TINYARA__*/

static void prv_quit(char * buffer,
                     void * user_data)
{
    g_quit = 1;
}

void handle_sigint(int signum)
{
    g_quit = 2;
}

void handle_value_changed(lwm2m_context_t * lwm2mP,
                          lwm2m_uri_t * uri,
                          const char * value,
                          size_t valueLength)
{
    lwm2m_object_t * object = (lwm2m_object_t *)LWM2M_LIST_FIND(lwm2mP->objectList, uri->objectId);

    if (NULL != object)
    {
        if (object->writeFunc != NULL)
        {
            lwm2m_data_t * dataP;
            int result;

            dataP = lwm2m_data_new(1);
            if (dataP == NULL)
            {
                fprintf(stderr, "Internal allocation failure !\n");
                return;
            }
            dataP->id = uri->resourceId;
            lwm2m_data_encode_nstring(value, valueLength, dataP);

            result = object->writeFunc(uri->instanceId, 1, dataP, object);
            if (COAP_405_METHOD_NOT_ALLOWED == result)
            {
                switch (uri->objectId)
                {
                case LWM2M_DEVICE_OBJECT_ID:
                    result = device_change(dataP, object);
                    break;
                default:
                    break;
                }
            }

            if (COAP_204_CHANGED != result)
            {
                fprintf(stderr, "Failed to change value!\n");
            }
            else
            {
                fprintf(stderr, "value changed!\n");
                lwm2m_resource_value_changed(lwm2mP, uri);
            }
            lwm2m_data_free(1, dataP);
            return;
        }
        else
        {
            fprintf(stderr, "write not supported for specified resource!\n");
        }
        return;
    }
    else
    {
        fprintf(stderr, "Object not found !\n");
    }
}

static char *coap_get_port_from_proto(coap_protocol_t proto)
{
    char *port = NULL;

    switch(proto) {
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

void * lwm2m_connect_server(uint16_t secObjInstID,
                            void * userData)
{
    client_data_t * dataP;
    char * uri;
    char * host;
    char * port;
    connection_t * newConnP = NULL;
    coap_protocol_t proto = COAP_UDP;

    dataP = (client_data_t *)userData;

    uri = get_server_uri(dataP->securityObjP, secObjInstID);

    if (uri == NULL) return NULL;

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
    if (port == NULL) goto exit;
    // remove brackets
    if (host[0] == '[')
    {
        host++;
        if (*(port - 1) == ']')
        {
            *(port - 1) = 0;
        }
        else goto exit;
    }
    // split strings
    *port = 0;
    port++;

    fprintf(stderr, "Opening connection to server at %s:%s\r\n", host, port);
    fprintf(stderr, "Connection protocol type : %d\r\n", proto);
    newConnP = connection_create(proto, dataP->connList, dataP->sock, host, port, dataP->addressFamily);
    if (newConnP == NULL) {
        fprintf(stderr, "Connection creation failed.\r\n");
    } else {
        memcpy(&dataP->server_addr, &newConnP->addr, newConnP->addrLen);
        dataP->server_addrLen = newConnP->addrLen;
        dataP->connList = newConnP;
    }

#ifdef WITH_MBEDTLS
    if (proto == COAP_TCP_TLS || proto == COAP_UDP_DTLS) {
        newConnP->session = TLSSession(dataP->sock, dataP->tls_context, dataP->tls_opt);
        if (newConnP->session == NULL) {
            fprintf(stderr, "Failed to create secure session. \r\n");
            goto exit;
        }
        fprintf(stderr, "successfully create secure session. \r\n");
    }
#endif

exit:
    lwm2m_free(uri);
    return (void *)newConnP;
}

void lwm2m_close_connection(void * sessionH,
                            void * userData)
{
    client_data_t * app_data;
    connection_t * targetP;
    app_data = (client_data_t *)userData;
    targetP = (connection_t *)sessionH;

    if (targetP == app_data->connList)
    {
        app_data->connList = targetP->next;
        lwm2m_free(targetP);
    }
    else
    {
        connection_t * parentP;

        parentP = app_data->connList;
        while (parentP != NULL && parentP->next != targetP)
        {
            parentP = parentP->next;
        }
        if (parentP != NULL)
        {
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

static void prv_output_servers(char * buffer,
                               void * user_data)
{
    lwm2mH = (lwm2m_context_t *) user_data;
    lwm2m_server_t * targetP;

    targetP = lwm2mH->bootstrapServerList;

    if (lwm2mH->bootstrapServerList == NULL)
    {
        fprintf(stdout, "No Bootstrap Server.\r\n");
    }
    else
    {
        fprintf(stdout, "Bootstrap Servers:\r\n");
        for (targetP = lwm2mH->bootstrapServerList ; targetP != NULL ; targetP = targetP->next)
        {
            fprintf(stdout, " - Security Object ID %d", targetP->secObjInstID);
            fprintf(stdout, "\tHold Off Time: %lu s", (unsigned long)targetP->lifetime);
            fprintf(stdout, "\tstatus: ");
            switch(targetP->status)
            {
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

    if (lwm2mH->serverList == NULL)
    {
        fprintf(stdout, "No LWM2M Server.\r\n");
    }
    else
    {
        fprintf(stdout, "LWM2M Servers:\r\n");
        for (targetP = lwm2mH->serverList ; targetP != NULL ; targetP = targetP->next)
        {
            fprintf(stdout, " - Server ID %d", targetP->shortID);
            fprintf(stdout, "\tstatus: ");
            switch(targetP->status)
            {
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

static void prv_change(char * buffer,
                       void * user_data)
{
    lwm2mH = (lwm2m_context_t *) user_data;
    lwm2m_uri_t uri;
    char * end = NULL;
    int result;

    end = get_end_of_arg(buffer);
    if (end[0] == 0) goto syntax_error;

    result = lwm2m_stringToUri(buffer, end - buffer, &uri);
    if (result == 0) goto syntax_error;

    buffer = get_next_arg(end, &end);

    if (buffer[0] == 0)
    {
        fprintf(stderr, "report change!\n");
        lwm2m_resource_value_changed(lwm2mH, &uri);
    }
    else
    {
        handle_value_changed(lwm2mH, &uri, buffer, end - buffer);
    }
    return;

syntax_error:
    fprintf(stdout, "Syntax error !\n");
}

static void prv_object_list(char * buffer,
                            void * user_data)
{
    lwm2mH = (lwm2m_context_t *)user_data;
    lwm2m_object_t * objectP;

    for (objectP = lwm2mH->objectList; objectP != NULL; objectP = objectP->next)
    {
        if (objectP->instanceList == NULL)
        {
            fprintf(stdout, "/%d ", objectP->objID);
        }
        else
        {
            lwm2m_list_t * instanceP;

            for (instanceP = objectP->instanceList; instanceP != NULL ; instanceP = instanceP->next)
            {
                fprintf(stdout, "/%d/%d  ", objectP->objID, instanceP->id);
            }
        }
        fprintf(stdout, "\r\n");
    }
}

static void prv_instance_dump(lwm2m_object_t * objectP,
                              uint16_t id)
{
    int numData;
    lwm2m_data_t * dataArray;
    uint16_t res;

    numData = 0;
    res = objectP->readFunc(id, &numData, &dataArray, objectP);
    if (res != COAP_205_CONTENT)
    {
        printf("Error ");
        print_status(stdout, res);
        printf("\r\n");
        return;
    }

    dump_tlv(stdout, numData, dataArray, 0);
}


static void prv_object_dump(char * buffer,
                            void * user_data)
{
    lwm2mH = (lwm2m_context_t *) user_data;
    lwm2m_uri_t uri;
    char * end = NULL;
    int result;
    lwm2m_object_t * objectP;

    end = get_end_of_arg(buffer);
    if (end[0] == 0) goto syntax_error;

    result = lwm2m_stringToUri(buffer, end - buffer, &uri);
    if (result == 0) goto syntax_error;
    if (uri.flag & LWM2M_URI_FLAG_RESOURCE_ID) goto syntax_error;

    objectP = (lwm2m_object_t *)LWM2M_LIST_FIND(lwm2mH->objectList, uri.objectId);
    if (objectP == NULL)
    {
        fprintf(stdout, "Object not found.\n");
        return;
    }

    if (uri.flag & LWM2M_URI_FLAG_INSTANCE_ID)
    {
        prv_instance_dump(objectP, uri.instanceId);
    }
    else
    {
        lwm2m_list_t * instanceP;

        for (instanceP = objectP->instanceList; instanceP != NULL ; instanceP = instanceP->next)
        {
            fprintf(stdout, "Instance %d:\r\n", instanceP->id);
            prv_instance_dump(objectP, instanceP->id);
            fprintf(stdout, "\r\n");
        }
    }

    return;

syntax_error:
    fprintf(stdout, "Syntax error !\n");
}

static void prv_update(char * buffer,
                       void * user_data)
{
    lwm2mH = (lwm2m_context_t *)user_data;
    if (buffer[0] == 0) goto syntax_error;

    uint16_t serverId = (uint16_t) atoi(buffer);
    int res = lwm2m_update_registration(lwm2mH, serverId, false);
    if (res != 0)
    {
        fprintf(stdout, "Registration update error: ");
        print_status(stdout, res);
        fprintf(stdout, "\r\n");
    }
    return;

syntax_error:
    fprintf(stdout, "Syntax error !\n");
}

static void update_battery_level(lwm2m_context_t * context)
{
    static time_t next_change_time = 0;
    time_t tv_sec;

    tv_sec = lwm2m_gettime();
    if (tv_sec < 0) return;

    if (next_change_time < tv_sec)
    {
        char value[15];
        int valueLength;
        lwm2m_uri_t uri;
        int level = rand() % 100;

        if (0 > level) level = -level;
        if (lwm2m_stringToUri("/3/0/9", 6, &uri))
        {
            valueLength = sprintf(value, "%d", level);
            fprintf(stderr, "New Battery Level: %d\n", level);
            handle_value_changed(context, &uri, value, valueLength);
        }
        level = rand() % 20;
        if (0 > level) level = -level;
        next_change_time = tv_sec + level + 10;
    }
}

static void prv_add(char * buffer,
                    void * user_data)
{
    lwm2mH = (lwm2m_context_t *)user_data;
    lwm2m_object_t * objectP;
    int res;

    objectP = get_test_object();
    if (objectP == NULL)
    {
        fprintf(stdout, "Creating object 1024 failed.\r\n");
        return;
    }
    res = lwm2m_add_object(lwm2mH, objectP);
    if (res != 0)
    {
        fprintf(stdout, "Adding object 1024 failed: ");
        print_status(stdout, res);
        fprintf(stdout, "\r\n");
    }
    else
    {
        fprintf(stdout, "Object 1024 added.\r\n");
    }
    return;
}

static void prv_remove(char * buffer,
                       void * user_data)
{
    lwm2mH = (lwm2m_context_t *)user_data;
    int res;

    res = lwm2m_remove_object(lwm2mH, 1024);
    if (res != 0)
    {
        fprintf(stdout, "Removing object 1024 failed: ");
        print_status(stdout, res);
        fprintf(stdout, "\r\n");
    }
    else
    {
        fprintf(stdout, "Object 1024 removed.\r\n");
    }
    return;
}

#ifdef LWM2M_BOOTSTRAP

static void prv_initiate_bootstrap(char * buffer,
                                   void * user_data)
{
    lwm2mH = (lwm2m_context_t *)user_data;
    lwm2m_server_t * targetP;

    // HACK !!!
    lwm2mH->state = STATE_BOOTSTRAP_REQUIRED;
    targetP = lwm2mH->bootstrapServerList;
    while (targetP != NULL)
    {
        targetP->lifetime = 0;
        targetP = targetP->next;
    }
}

static void prv_display_objects(char * buffer,
                                void * user_data)
{
    lwm2mH = (lwm2m_context_t *)user_data;
    lwm2m_object_t * object;

    for (object = lwm2mH->objectList; object != NULL; object = object->next){
        if (NULL != object) {
            switch (object->objID)
            {
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

static void prv_display_backup(char * buffer,
        void * user_data)
{
   int i;
   for (i = 0 ; i < BACKUP_OBJECT_COUNT ; i++) {
       lwm2m_object_t * object = backupObjectArray[i];
       if (NULL != object) {
           switch (object->objID)
           {
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

static void prv_backup_objects(lwm2m_context_t * context)
{
    uint16_t i;

    for (i = 0; i < BACKUP_OBJECT_COUNT; i++) {
        if (NULL != backupObjectArray[i]) {
            switch (backupObjectArray[i]->objID)
            {
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

static void prv_restore_objects(lwm2m_context_t * context)
{
    lwm2m_object_t * targetP;

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

static void update_bootstrap_info(lwm2m_client_state_t * previousBootstrapState,
        lwm2m_context_t * context)
{
    if (*previousBootstrapState != context->state)
    {
        *previousBootstrapState = context->state;
        switch(context->state)
        {
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
        switch (backupObjectArray[i]->objID)
        {
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
#endif /* LWM2M_BOOTSTRAP */


#ifdef WITH_MBEDTLS

#define HEX2NUM(c)                  \
    if (c >= '0' && c <= '9')       \
        c -= '0';                   \
    else if (c >= 'a' && c <= 'f')  \
        c -= 'a' - 10;              \
    else if (c >= 'A' && c <= 'F')  \
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
#ifdef WITH_MBEDTLS
    fprintf(stdout, "  -i STRING\t Set PSK identity. If not set use none secure mode\r\n");
    fprintf(stdout, "  -s HEXSTRING\t Set Pre-Shared-Key. If not set use none secure mode\r\n");
#endif
    fprintf(stdout, "Examples:\r\n");
    fprintf(stdout, "  lwm2mclient -h coap://127.0.0.1 -4\r\n");
    fprintf(stdout, "  lwm2mclient -h coaps+tcp://127.0.0.1 -4 -i PSK_identity -s 4938271\r\n");
    fprintf(stdout, "\r\n");
}

#ifdef __TINYARA__
int lwm2m_client_cb(void *args)
{
	int argc;
	char **argv;

	argc = ((struct pthread_arg *)args)->argc;
	argv = ((struct pthread_arg *)args)->argv;
	g_quit = 0;
#else
int lwm2m_client_main(int argc, char *argv[])
{
#endif /* __TINYARA__ */
    client_data_t data;
    int result;
    lwm2mH = NULL;
    int i;
    const char * localPort = "56830";
    const char * server = NULL;
    const char * serverPort = LWM2M_STANDARD_PORT_STR;
    char * name = "testlwm2mclient";
    int lifetime = 300;
    int batterylevelchanging = 0;
    time_t reboot_time = 0;
    int opt;
    bool bootstrapRequested = false;
    bool serverPortChanged = false;

#ifdef LWM2M_BOOTSTRAP
    lwm2m_client_state_t previousState = STATE_INITIAL;
#endif

    char * pskId = NULL;
    uint16_t pskLen = -1;
    char * pskBuffer = NULL;
#ifdef WITH_MBEDTLS
    unsigned char psk[MBEDTLS_PSK_MAX_LEN];

    /* set default tls option */

    /*
     * Note that, currently auth_mode is set to 0 (for connecting artikcloud)
     * if you want to change auth_mode, please change 3rd parameter of tls_opt structure
     * - auth_mode can be configured  among (2: always verify, 1: optional, 0: not verify)
     */
    tls_opt option = {MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_DATAGRAM, 0, 5, NULL, };
    /* set cipher suite to all*/
    option.force_ciphersuites[0] = 0;
    option.force_ciphersuites[1] = 0;
#endif

    struct timeval tv = {60, 0};

    struct sockaddr_storage addr;
    socklen_t addrLen;

    coap_protocol_t proto = COAP_UDP;

    /*
     * The function start by setting up the command line interface (which may or not be useful depending on your project)
     *
     * This is an array of commands describes as { name, description, long description, callback, userdata }.
     * The firsts tree are easy to understand, the callback is the function that will be called when this command is typed
     * and in the last one will be stored the lwm2m context (allowing access to the server settings and the objects).
     */
    command_desc_t commands[] =
    {
            {"list", "List known servers.", NULL, prv_output_servers, NULL},
            {"change", "Change the value of resource.", " change URI [DATA]\r\n"
                                                        "   URI: uri of the resource such as /3/0, /3/0/2\r\n"
                                                        "   DATA: (optional) new value\r\n", prv_change, NULL},
            {"update", "Trigger a registration update", " update SERVER\r\n"
                                                        "   SERVER: short server id such as 123\r\n", prv_update, NULL},
#ifdef LWM2M_BOOTSTRAP
            {"bootstrap", "Initiate a DI bootstrap process", NULL, prv_initiate_bootstrap, NULL},
            {"dispb", "Display current backup of objects/instances/resources\r\n"
                    "\t(only security and server objects are backupped)", NULL, prv_display_backup, NULL},

            {"disp", "Display current objects/instances/resources", NULL, prv_display_objects, NULL},
#endif
            {"ls", "List Objects and Instances", NULL, prv_object_list, NULL},
            {"dump", "Dump an Object", "dump URI"
                                       "URI: uri of the Object or Instance such as /3/0, /1\r\n", prv_object_dump, NULL},
            {"add", "Add support of object 1024", NULL, prv_add, NULL},
            {"rm", "Remove support of object 1024", NULL, prv_remove, NULL},
            {"quit", "Quit the client gracefully.", NULL, prv_quit, NULL},
            {"^C", "Quit the client abruptly (without sending a de-register message).", NULL, NULL, NULL},

            COMMAND_END_LIST
    };

    memset(&data, 0, sizeof(client_data_t));
    data.addressFamily = AF_INET;

    opt = 1;
    while (opt < argc)
    {
        if (argv[opt] == NULL
            || argv[opt][0] != '-'
            || argv[opt][2] != 0)
        {
            print_usage();
            return 0;
        }
        switch (argv[opt][1])
        {
        case 'b':
            bootstrapRequested = true;
            if (!serverPortChanged) serverPort = LWM2M_BSSERVER_PORT_STR;
            break;
        case 'c':
            batterylevelchanging = 1;
            break;
        case 't':
            opt++;
            if (opt >= argc)
            {
                print_usage();
                return 0;
            }
            if (1 != sscanf(argv[opt], "%d", &lifetime))
            {
                print_usage();
                return 0;
            }
            break;
#ifdef WITH_MBEDTLS
        case 'i':
            opt++;
            if (opt >= argc)
            {
                print_usage();
                return 0;
            }
            pskId = argv[opt];
            break;
        case 's':
            opt++;
            if (opt >= argc)
            {
                print_usage();
                return 0;
            }
            pskBuffer = argv[opt];
            break;
#endif
        case 'n':
            opt++;
            if (opt >= argc)
            {
                print_usage();
                return 0;
            }
            name = argv[opt];
            break;
        case 'l':
            opt++;
            if (opt >= argc)
            {
                print_usage();
                return 0;
            }
            localPort = argv[opt];
            break;
        case 'h':
            opt++;
            if (opt >= argc)
            {
                print_usage();
                return 0;
            }
            server = argv[opt];
            break;
        case 'p':
            opt++;
            if (opt >= argc)
            {
                print_usage();
                return 0;
            }
            serverPort = argv[opt];
            serverPortChanged = true;
            break;
        case '4':
            data.addressFamily = AF_INET;
            break;
        default:
            print_usage();
            return 0;
        }
        opt += 1;
    }
#if defined (__TINYARA__)
    if (argc == SET_PARAM_BY_GLOBAL_VALUE) {
        /* To handling parameters set through dm frameworks */
        name = DEFAULT_CLIENT_NAME;
        lifetime = g_lifetime;

        bootstrapRequested = g_bootstrapRequested;

        if (bootstrapRequested) {
            server = g_bootstrapserverAddr;
            serverPort = g_bootstrapserverPort;
        } else {
            server = g_serverAddr;
            serverPort = g_serverPort;
        }
    }
#endif /* __TINYARA__ */

    if (!server)
    {
        proto = COAP_UDP;
        server = (AF_INET == data.addressFamily ? DEFAULT_SERVER_IPV4 : DEFAULT_SERVER_IPV6);
    } else {
        /*
         * Parse server URI to distinguish protocol and server address
         */
        proto = coap_get_protocol_from_uri(server);
        if (proto >= COAP_PROTOCOL_MAX) {
            fprintf(stderr, "Not supported protocol : %d\n", proto);
            return -1;
        } else {
            /* move pointer to address field */
            server += strlen(coap_uri_prefix[proto]);
        }
        if (!serverPortChanged)
            serverPort = coap_get_port_from_proto(proto);
    }

    /*
     * This call an internal function that create an IPV6 socket on the port 5683.
     */
    fprintf(stderr, "Trying to bind LWM2M Client to port %s\r\n", serverPort);
    data.sock = create_socket(proto, serverPort, data.addressFamily);
    if (data.sock < 0)
    {
        fprintf(stderr, "Failed to open socket: %d %s\r\n", errno, strerror(errno));
        return -1;
    }

    /*
     * Now the main function fill an array with each object, this list will be later passed to liblwm2m.
     * Those functions are located in their respective object file.
     */

    char serverUri[50];
    int serverId = 123;
    sprintf (serverUri, "%s%s:%s", coap_uri_prefix[proto], server, serverPort);
#ifdef LWM2M_BOOTSTRAP
    objArray[0] = get_security_object(serverId, serverUri, pskId, pskBuffer, pskLen, bootstrapRequested);
#else
    objArray[0] = get_security_object(serverId, serverUri, pskId, pskBuffer, pskLen, false);
#endif
    if (NULL == objArray[0])
    {
        fprintf(stderr, "Failed to create security object\r\n");
		close(data.sock);
        return -1;
    }
    data.securityObjP = objArray[0];
    /*
     * Bind Accordingly Protocol (e.g., TCP, UDP)
     * get_server_object(serverId, "T", lifetime, false);
     */
    switch(proto) {
        case COAP_TCP:
        case COAP_TCP_TLS:
            objArray[1] = get_server_object(serverId, "T", lifetime, false);
            break;
        case COAP_UDP:
        case COAP_UDP_DTLS:
            objArray[1] = get_server_object(serverId, "U", lifetime, false);
            break;
        default:
            fprintf(stderr, "Cannot get_server_object with protocol %d\n", proto);
            break;
    }

    if (NULL == objArray[1])
    {
        fprintf(stderr, "Failed to create server object\r\n");
		close(data.sock);
        return -1;
    }

    objArray[2] = get_object_device();
    if (NULL == objArray[2])
    {
        fprintf(stderr, "Failed to create Device object\r\n");
		close(data.sock);
        return -1;
    }

    objArray[3] = get_object_firmware();
    if (NULL == objArray[3])
    {
        fprintf(stderr, "Failed to create Firmware object\r\n");
		close(data.sock);
        return -1;
    }

    objArray[4] = get_object_location();
    if (NULL == objArray[4])
    {
        fprintf(stderr, "Failed to create location object\r\n");
		close(data.sock);
        return -1;
    }

    objArray[5] = get_test_object();
    if (NULL == objArray[5])
    {
        fprintf(stderr, "Failed to create test object\r\n");
		close(data.sock);
        return -1;
    }

    objArray[6] = get_object_conn_m();
    if (NULL == objArray[6])
    {
        fprintf(stderr, "Failed to create connectivity monitoring object\r\n");
		close(data.sock);
        return -1;
    }

    objArray[7] = get_object_conn_s();
    if (NULL == objArray[7])
    {
        fprintf(stderr, "Failed to create connectivity statistics object\r\n");
		close(data.sock);
        return -1;
    }

    int instId = 0;
    objArray[8] = acc_ctrl_create_object();
    if (NULL == objArray[8])
    {
        fprintf(stderr, "Failed to create Access Control object\r\n");
		close(data.sock);
        return -1;
    }
    else if (acc_ctrl_obj_add_inst(objArray[8], instId, 3, 0, serverId)==false)
    {
        fprintf(stderr, "Failed to create Access Control object instance\r\n");
		close(data.sock);
        return -1;
    }
    else if (acc_ctrl_oi_add_ac_val(objArray[8], instId, 0, 0b000000000001111)==false)
    {
        fprintf(stderr, "Failed to create Access Control ACL default resource\r\n");
		close(data.sock);
        return -1;
    }
    else if (acc_ctrl_oi_add_ac_val(objArray[8], instId, 999, 0b000000000000001)==false)
    {
        fprintf(stderr, "Failed to create Access Control ACL resource for serverId: 999\r\n");
		close(data.sock);
        return -1;
    }

#ifdef WITH_MBEDTLS
    if (proto == COAP_TCP_TLS || proto == COAP_UDP_DTLS) {

        /* Set Transport layer (TCP or UDP */
        switch(proto) {
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

        if (pskBuffer) {
            if (lwm2m_unhexify(psk, pskBuffer, &cred.psk_len) == 0) {
                if (pskId) {
                    cred.psk_identity = pskId;
                    cred.psk = psk;
                }
            }
            if (cred.psk_identity == NULL && cred.psk == NULL) {
                fprintf(stdout, "failed to set psk info\r\n");
				goto lwm2mclient_err_exit;
            }
        }

        data.tls_context = TLSCtx(&cred);
        if (data.tls_context == NULL) {
            fprintf(stdout, "TLS context initialize failed\r\n");
			goto lwm2mclient_err_exit;
        }
    }
#endif
    /*
     * The liblwm2m library is now initialized with the functions that will be in
     * charge of communication
     */
    /* Use new API to set protocol type */
    lwm2mH = lwm2m_init2(&data, proto);
    if (NULL == lwm2mH)
    {
        fprintf(stderr, "lwm2m_init2() failed\r\n");
		goto lwm2mclient_err_exit;
    }
	
    /*
     * We configure the liblwm2m library with the name of the client - which shall be unique for each client -
     * the number of objects we will be passing through and the objects array
     */
    result = lwm2m_configure(lwm2mH, name, NULL, NULL, OBJ_COUNT, objArray);
    if (result != 0)
    {
        fprintf(stderr, "lwm2m_configure() failed: 0x%X\r\n", result);
		goto lwm2mclient_err_exit;
    }

    //signal(SIGINT, handle_sigint);

    /**
     * Initialize value changed callback.
     */
    init_value_change(lwm2mH);

    /*
     * As you now have your lwm2m context complete you can pass it as an argument to all the command line functions
     * precedently viewed (first point)
     */
    for (i = 0 ; commands[i].name != NULL ; i++)
    {
        commands[i].userData = (void *)lwm2mH;
    }
    fprintf(stdout, "LWM2M Client \"%s\" started on port %s\r\n", name, serverPort);
    fprintf(stdout, "> "); fflush(stdout);
    /*
     * We now enter in a while loop that will handle the communications from the server
     */

    while (0 == g_quit)
    {
        fd_set readfds;

        if (g_reboot)
        {
            time_t tv_sec;

            tv_sec = lwm2m_gettime();

            if (0 == reboot_time)
            {
                reboot_time = tv_sec + 5;
            }
            if (reboot_time < tv_sec)
            {
                /*
                 * Message should normally be lost with reboot ...
                 */
                fprintf(stderr, "reboot time expired, rebooting ...");
                system_reboot();
            }
            else
            {
                tv.tv_sec = reboot_time - tv_sec;
            }
        }
        else if (batterylevelchanging) 
        {
            update_battery_level(lwm2mH);
            tv.tv_sec = 5;
        }
        else 
        {
            tv.tv_sec = 60;
        }
        tv.tv_usec = 0;

        /*
         * This function does two things:
         *  - first it does the work needed by liblwm2m (eg. (re)sending some packets).
         *  - Secondly it adjusts the timeout value (default 60s) depending on the state of the transaction
         *    (eg. retransmission) and the time between the next operation
         */
        result = lwm2m_step(lwm2mH, &(tv.tv_sec));
        fprintf(stdout, " -> State: ");
        switch (lwm2mH->state)
        {
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
        if (result != 0)
        {
            fprintf(stderr, "lwm2m_step() failed: 0x%X\r\n", result);
#ifdef LWM2M_BOOTSTRAP
            if(previousState == STATE_BOOTSTRAPPING)
            {
#ifdef WITH_LOGS
                fprintf(stdout, "[BOOTSTRAP] restore security and server objects\r\n");
#endif
                prv_restore_objects(lwm2mH);
                lwm2mH->state = STATE_INITIAL;
            }
            else
#endif /* LWM2M_BOOTSTRAP */
				goto lwm2mclient_err_exit;
        }
#ifdef LWM2M_BOOTSTRAP
        update_bootstrap_info(&previousState, lwm2mH);
#endif

        FD_ZERO(&readfds);
        FD_SET(data.sock, &readfds);
#if defined (__TINYARA__)
        FD_SET(STDIN_FILENO, &readfds);
#else
        FD_SET(stdin, &readfds);
#endif
        /*
         * This part will set up an interruption until an event happen on SDTIN or the socket until "tv" timed out (set
         * with the precedent function)
         */
        result = select(FD_SETSIZE, &readfds, NULL, NULL, &tv);

        if (result < 0)
        {
            if (errno != EINTR)
            {
              fprintf(stderr, "Error in select(): %d %s\r\n", errno, strerror(errno));
            }
        }
        else if (result > 0)
        {
            uint8_t buffer[MAX_PACKET_SIZE];
            int numBytes;

            /*
             * If an event happens on the socket
             */
            if (FD_ISSET(data.sock, &readfds))
            {
                addrLen = sizeof(addr);
                client_data_t *user_data = lwm2mH->userData;

                /*
                 * We retrieve the data received
                 */
                numBytes = connection_read(proto, user_data->connList,data.sock, buffer, MAX_PACKET_SIZE, &addr, &addrLen);
                memcpy(&addr, &data.server_addr, data.server_addrLen);
                addrLen = data.server_addrLen;

                if (0 > numBytes)
                {
                    fprintf(stderr, "Error in recvfrom(): %d %s\r\n", errno, strerror(errno));
					if (errno == ENOTCONN) {
					/* To exit when TCP session has been unexpectedly closed*/
						goto lwm2mclient_err_exit;
					}
                }
                else if (0 < numBytes)
                {
                    char s[INET6_ADDRSTRLEN];
                    in_port_t port = 0;
                    connection_t * connP;
                    if (AF_INET == addr.ss_family)
                    {
                        struct sockaddr_in *saddr = (struct sockaddr_in *)&addr;
                        inet_ntop(saddr->sin_family, &saddr->sin_addr, s, INET6_ADDRSTRLEN);
                        port = saddr->sin_port;
                    }
                    else if (AF_INET6 == addr.ss_family)
                    {
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
                    if (connP != NULL)
                    {
                        /*
                         * Let liblwm2m respond to the query depending on the context
                         */
                        lwm2m_handle_packet(lwm2mH, buffer, numBytes, connP);
                        conn_s_updateRxStatistic(objArray[7], numBytes, false);
                    }
                    else
                    {
                        fprintf(stderr, "received bytes ignored!\r\n");
                    }
                }
            }

            /*
             * If the event happened on the SDTIN
             */
#if defined (__TINYARA__)
            else if (FD_ISSET(STDIN_FILENO, &readfds))
#else
            else if (FD_ISSET(stdin, &readfds))
#endif
            {
#if defined (__TINYARA__)
                numBytes = read_input_command_line((char *)buffer);
#else
                numBytes = read(stdin, buffer, MAX_PACKET_SIZE - 1);
#endif

                if (numBytes > 1)
                {
                    buffer[numBytes] = 0;
                    /*
                     * We call the corresponding callback of the typed command passing it the buffer for further arguments
                     */
                    handle_command(commands, (char*)buffer);
                }
                if (g_quit == 0)
                {
                    fprintf(stdout, "\r\n> ");
                    fflush(stdout);
                }
                else
                {
                    fprintf(stdout, "\r\n");
                }
            }
        }
    }
    /*
     * Finally when the loop is left smoothly - asked by user in the command line interface - we unregister our client from it
     */
    if (g_quit == 1)
    {
#ifdef LWM2M_BOOTSTRAP
        close_backup_object();
#endif
        lwm2m_close(lwm2mH);
		lwm2mH = NULL;
    }

lwm2mclient_err_exit:
	if (lwm2mH != NULL)
		lwm2m_close(lwm2mH);

    close(data.sock);
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

#ifdef WITH_MBEDTLS
    TLSCtx_free(data.tls_context);
#endif

#ifdef MEMORY_TRACE
    if (g_quit == 1)
    {
        trace_print(0, 1);
    }
#endif

    return 0;
}
/****************************************************************************
 * TINYARA
 * DM (Device Management) Frameworks supporting APIs
 ****************************************************************************/

#if defined (__TINYARA__)

/* Public APIs */

pthread_addr_t lwm2m_client_run(void *arg)
{
	struct lwm2mclient_input *input;

	if (arg == NULL) {
		lwm2m_client_main(SET_PARAM_BY_GLOBAL_VALUE, NULL);
	} else {
		input = arg;
		lwm2m_client_main(input->argc, input->argv);
	}
	return 0;
}

char *client_set_serverAddr(FAR const char *serverAddr, bool isbootstrap)
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

char *client_set_serverPort(FAR const char *serverPort, bool isbootstrap)
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

static void process_udpconn(int sockfd, fd_set *readfds, client_data_t data)
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
			in_port_t port;
			connection_t *connP;
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
				lwm2m_handle_packet(lwm2mH, buffer, numBytes, connP);
				conn_s_updateRxStatistic(objArray[7], numBytes, false);
			} else {
				fprintf(stderr, "received bytes ignored!\r\n");
			}
		}
	}
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

/* Private APIs */

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
					if (write(STDOUT_FILENO, "\b \b",3) <= 0) {
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
	} while(buffer[pos - 1] != ASCII_LF);

	memcpy(buf, buffer, pos);

	return pos;
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

static void prv_update_server(client_data_t *dataP, uint16_t secObjInstID)
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
#endif /* __TINYARA__ */
