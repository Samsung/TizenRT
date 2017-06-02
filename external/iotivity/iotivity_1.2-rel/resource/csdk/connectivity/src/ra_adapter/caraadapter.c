//*****************************************************************
//
// Copyright 2015 Intel Mobile Communications GmbH All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//****************************************************************

#include "caraadapter.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "caadapterutils.h"
#include "octhread.h"
#include "uarraylist.h"
#include "logger.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "caremotehandler.h"
#include "cacommon.h"

#ifdef RA_ADAPTER_IBB
#include "caprotocolmessage.h"
#include "xmpp_helper.h"
#include "xmpp_utils.h"
#include "xmpp_ibb.h"
#include "xmpp_utils.h"
#else
#include "ra_xmpp.h"
#endif

#ifdef RA_ADAPTER_IBB
#define SET_BUT_NOT_USED(x) (void) x
/**
 * Logging tag for module name.
 */
#define RA_ADAPTER_TAG "OIC_RA_ADAP_IBB"

/**
 * Network Packet Received Callback to CA.
 */
static CANetworkPacketReceivedCallback g_networkPacketCallback = NULL;

/**
 * Network Changed Callback to CA.
 */
static CAAdapterChangeCallback g_networkChangeCallback = NULL;

/**
 * Holds XMPP data information.
 */
#define RA_MAX_HOSTNAME_LENGTH 256
#define RA_MAX_PASSWORD_LENGTH 64
typedef struct
{
    xmpp_t     *xmpp;
    int         port;
    char        hostName[RA_MAX_HOSTNAME_LENGTH];
    char        password[RA_MAX_PASSWORD_LENGTH];
    char        jid[CA_RAJABBERID_SIZE];
    CANetworkStatus_t connectionStatus;
    CAJidBoundCallback jidBoundCallback;
} CARAXmppData_t;

static oc_mutex g_raadapterMutex = NULL;

static CARAXmppData_t g_xmppData = {.xmpp = NULL, .port = 5222, .hostName = {0},
    .password = {0}, .jid = {0}, .connectionStatus = CA_INTERFACE_DOWN,
    .jidBoundCallback = NULL};

static void CARANotifyNetworkChange(const char *address, CANetworkStatus_t status);

void CARANotifyNetworkChange(const char *address, CANetworkStatus_t status)
{
    OIC_LOG(DEBUG, RA_ADAPTER_TAG, "CARANotifyNetworkChange IN");

    g_xmppData.connectionStatus = status;

    CAAdapterChangeCallback networkChangeCallback = g_networkChangeCallback;
    if (networkChangeCallback)
    {
        networkChangeCallback(CA_ADAPTER_REMOTE_ACCESS, status);
    }
    else
    {
        OIC_LOG(ERROR, RA_ADAPTER_TAG, "g_networkChangeCallback is NULL");
    }

    OIC_LOG(DEBUG, RA_ADAPTER_TAG, "CARANotifyNetworkChange OUT");
}

#define MAX_IBB_SESSION_ID_LENGTH 32
/* Ref. octypes.h */
#define OBSERVE_REGISTER    0
#define OBSERVE_DEREGISTER  1
/* Ref. octypes.h */

static ilist_t * g_observerList = NULL;

typedef struct _obs_item_t
{
    char sessid[MAX_IBB_SESSION_ID_LENGTH + 1];
    int  option;
} obs_item_t;

static bool CARAFindSessID(obs_item_t *item, char *key)
{
    if (item == NULL || key == NULL)
    {
        return false;
    }
    if (strcmp(item->sessid, key) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

static bool CARAPDUIsRequest(uint32_t x)
{
    return (x == CA_GET || x == CA_POST || x == CA_PUT || x == CA_DELETE);
}

static void CARAUpdateObsList(int option, char *sid)
{
    if (option == OBSERVE_REGISTER)
    {
        obs_item_t *item = (obs_item_t *) OICMalloc(sizeof(*item));
        OICStrcpy(item->sessid, sizeof(item->sessid), sid);
        item->option = OBSERVE_REGISTER;
        ilist_add(g_observerList, item);
    }
    else if (option == OBSERVE_DEREGISTER)
    {
        obs_item_t *item = ilist_finditem_func(g_observerList, (find_fp) CARAFindSessID, sid);
        if (item != NULL)
        {
            item->option = OBSERVE_DEREGISTER;
        }
    }
}

static int CARAGetReqObsOption(coap_pdu_t *pdu, const CAEndpoint_t *endPoint)
{
    uint32_t obsopt = -1;

    CARequestInfo_t *reqInfo = (CARequestInfo_t *) OICMalloc(sizeof(*reqInfo));
    VERIFY_NON_NULL_RET(reqInfo, RA_ADAPTER_TAG, "Memory alloc of CARequestInfo_t failed!", -1);

    CAResult_t result = CAGetRequestInfoFromPDU(pdu, endPoint, reqInfo);
    if (CA_STATUS_OK != result)
    {
        OICFree(reqInfo);
        OIC_LOG(ERROR, RA_ADAPTER_TAG, "Get Request Info failed!");
        return -1;
    }
    if (!CARAPDUIsRequest(reqInfo->method))
    {
        OICFree(reqInfo);
        OIC_LOG(DEBUG, RA_ADAPTER_TAG, "It is not a request data.");
        return -1;
    }

    uint8_t numOpt = reqInfo->info.numOptions;
    CAHeaderOption_t *options = reqInfo->info.options;
    for (uint8_t i = 0; i < numOpt; i++)
    {
        if(options[i].protocolID == CA_COAP_ID &&
                options[i].optionID == COAP_OPTION_OBSERVE)
        {
            obsopt = options[i].optionData[0];
            break;
        }
    }
    OICFree(reqInfo);
    return obsopt;
}

static int CARAErrorCB(xmpp_ibb_session_t *sess, xmpperror_t *xerr)
{
    OIC_LOG_V(ERROR, RA_ADAPTER_TAG, "%s(): code(%d) tyep'%s' mesg'%s'",
            __FUNCTION__, xerr->code, xerr->type, xerr->mesg);
    SET_BUT_NOT_USED(sess);
    SET_BUT_NOT_USED(xerr);
    return 0;
}

static int CARAOpenCB(xmpp_ibb_session_t *sess, char *type)
{
    OIC_LOG_V(DEBUG, RA_ADAPTER_TAG, "%s(): set type '%s'", __FUNCTION__, type);
    SET_BUT_NOT_USED(sess);
    SET_BUT_NOT_USED(type);
    return 0;
}

static int CARACloseCB(xmpp_ibb_session_t *sess, char *type)
{
    OIC_LOG_V(DEBUG, RA_ADAPTER_TAG, "%s(): set type '%s'", __FUNCTION__, type);
    char *sid = xmpp_ibb_get_sid(sess);
    obs_item_t *item = ilist_finditem_func(g_observerList, (find_fp) CARAFindSessID, sid);
    if (item != NULL)
    {
        ilist_remove(g_observerList, item);
        OICFree(item);
    }
    SET_BUT_NOT_USED(type);
    return 0;
}

static char *CARAGetSIDFromPDU(coap_pdu_t *pdu)
{
    static char s_sid[MAX_IBB_SESSION_ID_LENGTH + 1] = {0};

    VERIFY_NON_NULL_RET(pdu, RA_ADAPTER_TAG, "Invalid parameter!", NULL);

    if (pdu->hdr->token_length * 2 > MAX_IBB_SESSION_ID_LENGTH)
    {
        OIC_LOG(ERROR, RA_ADAPTER_TAG, "Token length more than expected!");
        return NULL;
    }

    char hex[3] = {0};
    for (int i = 0; i < pdu->hdr->token_length; i++)
    {
        snprintf(hex, 3, "%02x", pdu->hdr->token[i]);
        OICStrcat(s_sid, sizeof(s_sid), hex);
    }

    return s_sid;
}

static int CARARecvCB(xmpp_ibb_session_t *sess, xmppdata_t *xdata)
{
    if (xdata == NULL)
    {
        /* xdata == NULL, send ack result */
        return 0;
    }

    char *msg = xdata->data;
    char *from = xmpp_ibb_get_remote_jid(sess);
    if (g_networkPacketCallback)
    {
        VERIFY_NON_NULL_RET(from, RA_ADAPTER_TAG, "from sender is NULL", -1);
        VERIFY_NON_NULL_RET(msg, RA_ADAPTER_TAG, "message is NULL", -1);

        OIC_LOG_V (DEBUG, RA_ADAPTER_TAG, "Message received from %s", from);

        CAEndpoint_t *endPoint = CACreateEndpointObject(CA_DEFAULT_FLAGS,
                        CA_ADAPTER_REMOTE_ACCESS, from, 0);
        if (!endPoint)
        {
            OIC_LOG(ERROR, RA_ADAPTER_TAG, "EndPoint creation failed!");
            return -1;
        }
        uint32_t code = CA_NOT_FOUND;
        coap_pdu_t *pdu = (coap_pdu_t *) CAParsePDU(xdata->data, xdata->size, &code,
            endPoint);
        char *sid = CARAGetSIDFromPDU(pdu);
        int obsopt = CARAGetReqObsOption(pdu, endPoint);
        coap_delete_pdu(pdu);

        if (CARAPDUIsRequest(code))
        {
            OIC_LOG(DEBUG, RA_ADAPTER_TAG, "this is a request data");
            if (obsopt == OBSERVE_DEREGISTER || obsopt == OBSERVE_REGISTER)
            {
                CARAUpdateObsList(obsopt, sid);
            }
        }
        else
        {
            OIC_LOG(DEBUG, RA_ADAPTER_TAG, "this is a response data");
            obs_item_t *item = ilist_finditem_func(g_observerList, (find_fp) CARAFindSessID, sid);
            if (item != NULL)
            {
                if (item->option == OBSERVE_DEREGISTER)
                {
                    xmpp_ibb_close(sess);
                    ilist_remove(g_observerList, item);
                    OICFree(item);
                }
            }
            else
            {
                xmpp_ibb_close(sess);
            }
        }

        void *buf = NULL;
        xmpp_ibb_userdata_alloc(sess, &buf, xdata->size);
        if (!buf)
        {
            OIC_LOG(ERROR, RA_ADAPTER_TAG, "Memory alloc of message failed!");
            CAFreeEndpoint(endPoint);
            return -1;
        }
        memcpy(buf, xdata->data, xdata->size);
        CASecureEndpoint_t sep =
        {.endpoint = {.adapter = CA_ADAPTER_IP, .flags = CA_DEFAULT_FLAGS}};
        memcpy(&sep.endpoint, endPoint, sizeof(sep.endpoint));
        g_networkPacketCallback(&sep, buf, xdata->size);

        CAFreeEndpoint (endPoint);
    }
    else
    {
        OIC_LOG(ERROR, RA_ADAPTER_TAG, "No callback for RA received message found");
    }
    return 0;
}

static int CARAConnHandler(xmpp_t *xmpp, xmppconn_info_t *conninfo, void *udata)
{
    if (conninfo->connevent != 0)
    {
        OIC_LOG_V(ERROR, RA_ADAPTER_TAG, " status(%d) error(%d) errorType(%d) errorText '%s'\n",
                conninfo->connevent, conninfo->error, conninfo->errortype,
                conninfo->errortext);
        CARANotifyNetworkChange(g_xmppData.jid, CA_INTERFACE_DOWN);
        return -1;
    }
    OIC_LOG_V(DEBUG, RA_ADAPTER_TAG, "Bound JID: '%s'", xmpphelper_get_bound_jid(xmpp));
    if (g_xmppData.jidBoundCallback != NULL)
    {
        g_xmppData.jidBoundCallback((char *) xmpphelper_get_bound_jid(xmpp));
    }
    CARANotifyNetworkChange(xmpphelper_get_bound_jid(xmpp), CA_INTERFACE_UP);
    VERIFY_NON_NULL_RET(udata, RA_ADAPTER_TAG, "Invalid parameter!", 0);
    return 0;
}

CAResult_t CAInitializeRA(CARegisterConnectivityCallback registerCallback,
                          CANetworkPacketReceivedCallback networkPacketCallback,
                          CAAdapterChangeCallback netCallback, ca_thread_pool_t handle)
{
    OIC_LOG(DEBUG, RA_ADAPTER_TAG, "CAInitializeRA IN");
    if (!registerCallback || !networkPacketCallback || !netCallback || !handle)
    {
        OIC_LOG(ERROR, RA_ADAPTER_TAG, "Invalid parameter!");
        return CA_STATUS_INVALID_PARAM;
    }

    g_networkChangeCallback = netCallback;
    g_networkPacketCallback = networkPacketCallback;

    CAConnectivityHandler_t raHandler = {
        .startAdapter = CAStartRA,
        .stopAdapter = CAStopRA,
        .startListenServer = CAStartRAListeningServer,
        .startDiscoveryServer = CAStartRADiscoveryServer,
        .sendData = CASendRAUnicastData,
        .sendDataToAll = CASendRAMulticastData,
        .GetnetInfo = CAGetRAInterfaceInformation,
        .readData = CAReadRAData,
        .terminate = CATerminateRA,
        .cType = CA_ADAPTER_REMOTE_ACCESS};

    registerCallback(raHandler);
#ifdef NDEBUG
    xmpp_log_t *log = xmpp_get_default_logger(XMPP_LEVEL_ERROR);
#else
    xmpp_log_t *log = xmpp_get_default_logger(XMPP_LEVEL_DEBUG);
#endif
    g_xmppData.xmpp = xmpphelper_new(CARAConnHandler, NULL, log, NULL);
    xmpphelper_force_tls(g_xmppData.xmpp);
    g_observerList = ilist_new();

    return CA_STATUS_OK;
}

CAResult_t CASetRAInfo(const CARAInfo_t *caraInfo)
{
    if (!caraInfo)
    {
        OIC_LOG(ERROR, RA_ADAPTER_TAG, "Invalid parameter!");
        return CA_STATUS_INVALID_PARAM;
    }
    if (caraInfo->hostName != NULL)
    {
        OICStrcpy(g_xmppData.hostName, sizeof(g_xmppData.hostName), caraInfo->hostName);
    }
    else
    {
        OIC_LOG(ERROR, RA_ADAPTER_TAG, "Invalid parameter!");
        return CA_STATUS_INVALID_PARAM;
    }
    if (caraInfo->userName != NULL && strlen(caraInfo->userName) != 0)
    {
        OICStrcpy(g_xmppData.jid, sizeof(g_xmppData.jid), caraInfo->userName);
    }
    else
    {
        OIC_LOG(ERROR, RA_ADAPTER_TAG, "Invalid parameter!");
        return CA_STATUS_INVALID_PARAM;
    }
    if (caraInfo->xmppDomain != NULL && strlen(caraInfo->xmppDomain) != 0)
    {
        OICStrcat(g_xmppData.jid, sizeof(g_xmppData.jid), "@");
        OICStrcat(g_xmppData.jid, sizeof(g_xmppData.jid), caraInfo->xmppDomain);
        if (caraInfo->resource != NULL && strlen(caraInfo->resource) != 0)
        {
            OICStrcat(g_xmppData.jid, sizeof(g_xmppData.jid), "/");
            OICStrcat(g_xmppData.jid, sizeof(g_xmppData.jid), caraInfo->resource);
        }
    }
    if (caraInfo->password != NULL)
    {
        OICStrcpy(g_xmppData.password, sizeof(g_xmppData.password), caraInfo->password);
    }
    g_xmppData.port = caraInfo->port;
    g_xmppData.jidBoundCallback = caraInfo->jidBoundCallback;

    return CA_STATUS_OK;
}

void CATerminateRA()
{
    CAStopRA();
    ilist_destroy(g_observerList);
    xmpphelper_join(g_xmppData.xmpp);
    xmpphelper_release(g_xmppData.xmpp);
    g_xmppData.xmpp = NULL;
}

CAResult_t CAStartRA()
{
    OIC_LOG(DEBUG, RA_ADAPTER_TAG, PCF("Starting RA adapter"));

    if (!g_xmppData.xmpp)
    {
        OIC_LOG (ERROR, RA_ADAPTER_TAG, "CAStartRA(): g_xmppData.xmpp == NULL");
        return CA_STATUS_FAILED;
    }

    g_raadapterMutex = oc_mutex_new ();
    if (!g_raadapterMutex)
    {
        OIC_LOG (ERROR, RA_ADAPTER_TAG, PCF("Memory allocation for mutex failed."));
        return CA_MEMORY_ALLOC_FAILED;
    }

    oc_mutex_lock (g_raadapterMutex);

    xmpphelper_connect(g_xmppData.xmpp, g_xmppData.hostName, g_xmppData.port,
                    g_xmppData.jid, g_xmppData.password);
    xmpp_ibb_reg_funcs_t regfuncs;
    regfuncs.open_cb = CARAOpenCB;
    regfuncs.close_cb = CARACloseCB;
    regfuncs.recv_cb = CARARecvCB;
    regfuncs.error_cb = CARAErrorCB;
    xmpp_ibb_register(xmpphelper_get_conn(g_xmppData.xmpp), &regfuncs);

    xmpphelper_run(g_xmppData.xmpp);

    oc_mutex_unlock (g_raadapterMutex);

    OIC_LOG(DEBUG, RA_ADAPTER_TAG, "RA adapter started succesfully");
    return CA_STATUS_OK;
}

CAResult_t CAStopRA()
{
    OIC_LOG(DEBUG, RA_ADAPTER_TAG, PCF("Stopping RA adapter"));

    xmpphelper_stop(g_xmppData.xmpp);
    xmpp_ibb_unregister(xmpphelper_get_conn(g_xmppData.xmpp));
    if (!g_raadapterMutex)
    {
        oc_mutex_free (g_raadapterMutex);
        g_raadapterMutex = NULL;
    }
    OIC_LOG(DEBUG, RA_ADAPTER_TAG, PCF("Stopped RA adapter successfully"));
    return CA_STATUS_OK;
}

int32_t CASendRAUnicastData(const CAEndpoint_t *remoteEndpoint, const void *data,
                            uint32_t dataLength, CADataType_t dataType)
{
    SET_BUT_NOT_USED(dataType);

    if (!remoteEndpoint || !data)
    {
        OIC_LOG(ERROR, RA_ADAPTER_TAG, "Invalid parameter!");
        return -1;
    }

    if (0 == dataLength)
    {
        OIC_LOG(ERROR, RA_ADAPTER_TAG, "Data length is 0!");
        return 0;
    }
    OIC_LOG_V(DEBUG, RA_ADAPTER_TAG, "Sending unicast data to %s", remoteEndpoint->addr);

    uint32_t code = CA_NOT_FOUND;
    coap_pdu_t *pdu = (coap_pdu_t *) CAParsePDU(data, dataLength, &code, remoteEndpoint);
    char *sid = CARAGetSIDFromPDU(pdu);
    int obsopt = CARAGetReqObsOption(pdu, remoteEndpoint);
    coap_delete_pdu(pdu);

    oc_mutex_lock (g_raadapterMutex);
    if (CA_INTERFACE_UP != g_xmppData.connectionStatus)
    {
        OIC_LOG(ERROR, RA_ADAPTER_TAG, "Unable to send XMPP message, RA not connected");
        oc_mutex_unlock (g_raadapterMutex);
        return -1;
    }

    xmpp_ibb_session_t *sess = xmpp_ibb_get_session_by_sid(sid);
    if (sess == NULL)
    {
        sess = xmpp_ibb_open(xmpphelper_get_conn(g_xmppData.xmpp), (char * const) remoteEndpoint->addr, sid);
        if (sess == NULL)
        {
            OIC_LOG(ERROR, RA_ADAPTER_TAG, "IBB session establish failed!");
            oc_mutex_unlock (g_raadapterMutex);
            return -1;
        }
    }
    if (CARAPDUIsRequest(code))
    {
        if (obsopt == OBSERVE_REGISTER || obsopt == OBSERVE_DEREGISTER)
        {
            CARAUpdateObsList(obsopt, sid);
        }
    }
    xmppdata_t xdata = {.data = (char *) data, .size = dataLength};
    int rc = xmpp_ibb_send_data(sess, &xdata);
    oc_mutex_unlock (g_raadapterMutex);
    if (rc < 0)
    {
        OIC_LOG(ERROR, RA_ADAPTER_TAG, "IBB send data failed!");
        return -1;
    }

    OIC_LOG_V(INFO, RA_ADAPTER_TAG, "Successfully dispatched bytes[%d] to addr[%s]",
            dataLength, remoteEndpoint->addr);

    return dataLength;
}

CAResult_t CAGetRAInterfaceInformation(CAEndpoint_t **info, uint32_t *size)
{
    VERIFY_NON_NULL(info, RA_ADAPTER_TAG, "info is NULL");
    VERIFY_NON_NULL(size, RA_ADAPTER_TAG, "size is NULL");
    return CA_STATUS_OK;
}

int32_t CASendRAMulticastData(const CAEndpoint_t *endpoint,
                              const void *data, uint32_t dataLength,
                              CADataType_t dataType)
{
    OIC_LOG(INFO, RA_ADAPTER_TAG, "RA adapter does not support sending multicast data");
    SET_BUT_NOT_USED(endpoint);
    SET_BUT_NOT_USED(data);
    SET_BUT_NOT_USED(dataLength);
    SET_BUT_NOT_USED(dataType);
    return 0;
}

CAResult_t CAStartRAListeningServer()
{
    OIC_LOG(INFO, RA_ADAPTER_TAG, "RA adapter does not support listening for multicast data");
    return CA_NOT_SUPPORTED;
}

CAResult_t CAStartRADiscoveryServer()
{
    OIC_LOG(INFO, RA_ADAPTER_TAG, "RA adapter does not support discovery of multicast servers");
    return CA_NOT_SUPPORTED;
}

CAResult_t CAReadRAData()
{
    OIC_LOG(INFO, RA_ADAPTER_TAG, "Read data is not implemented for the RA adapter");
    return CA_NOT_SUPPORTED;
}

#else /* #ifdef RA_ADAPTER_IBB */

/**
 * Logging tag for module name.
 */
#define RA_ADAPTER_TAG "RA_ADAP"

/**
 * Network Packet Received Callback to CA.
 */
static CANetworkPacketReceivedCallback g_networkPacketCallback = NULL;

/**
 * Network Changed Callback to CA.
 */
static CAAdapterChangeCallback g_networkChangeCallback = NULL;

/**
 * Holds XMPP data information.
 */
typedef struct
{
    xmpp_context_t context;
    xmpp_handle_t handle;
    xmpp_connection_callback_t connection_callback;
    xmpp_connection_handle_t connection_handle;
    xmpp_message_context_t message_context;
    xmpp_message_callback_t message_callback;
    CANetworkStatus_t connection_status;
    xmpp_host_t     g_host;
    xmpp_identity_t g_identity;
    char jabberID[CA_RAJABBERID_SIZE];
} CARAXmppData_t;

static oc_mutex g_raadapterMutex = NULL;

static CARAXmppData_t g_xmppData = {};

static void CARANotifyNetworkChange(const char *address, CANetworkStatus_t status);

static void CARAXmppConnectedCB(void * const param, xmpp_error_code_t result,
        const char *const bound_jid,
        xmpp_connection_handle_t connection);

static void CARAXmppDisonnectedCB(void * const param, xmpp_error_code_t result,
        xmpp_connection_handle_t connection);

static void CARAXmppMessageSentCB(void * const param, xmpp_error_code_t result,
        const void *const recipient, const void *const msg, size_t messageOctets);

static void CARAXmppMessageReceivedCB(void * const param, xmpp_error_code_t result,
        const void *const sender, const void *const msg, size_t messageOctets);

void CARANotifyNetworkChange(const char *address, CANetworkStatus_t status)
{
    OIC_LOG(DEBUG, RA_ADAPTER_TAG, "CARANotifyNetworkChange IN");

    CAAdapterChangeCallback networkChangeCallback = g_networkChangeCallback;
    if (networkChangeCallback)
    {
        networkChangeCallback(CA_ADAPTER_REMOTE_ACCESS, status);
    }
    else
    {
        OIC_LOG(ERROR, RA_ADAPTER_TAG, "g_networkChangeCallback is NULL");
    }

    OIC_LOG(DEBUG, RA_ADAPTER_TAG, "CARANotifyNetworkChange OUT");
}

void CARAXmppConnectedCB(void * const param, xmpp_error_code_t result,
        const char *const bound_jid,
        xmpp_connection_handle_t connection)
{
    OIC_LOG(DEBUG, RA_ADAPTER_TAG, "CARAXmppConnectedCB IN");
    CANetworkStatus_t connection_status;
    if (XMPP_ERR_OK == result)
    {
        printf("\n\n\t\t===>your jid: %s\n\n", bound_jid);

        oc_mutex_lock (g_raadapterMutex);
        OICStrcpy (g_xmppData.jabberID, CA_RAJABBERID_SIZE, bound_jid);

        g_xmppData.connection_status = CA_INTERFACE_UP;
        connection_status = CA_INTERFACE_UP;
        g_xmppData.connection_handle = connection;
        g_xmppData.message_callback.on_received = CARAXmppMessageReceivedCB;
        g_xmppData.message_callback.on_sent = CARAXmppMessageSentCB;
        g_xmppData.message_context = xmpp_message_context_create(g_xmppData.connection_handle,
                g_xmppData.message_callback);
    }
    else
    {
        g_xmppData.connection_status = CA_INTERFACE_DOWN;
        connection_status = CA_INTERFACE_DOWN;
        OIC_LOG_V(ERROR, RA_ADAPTER_TAG, "XMPP connected callback status: %d", result);
    }

    oc_mutex_unlock (g_raadapterMutex);
    // Notify network change to CA
    CARANotifyNetworkChange(bound_jid, connection_status);

    OIC_LOG(DEBUG, RA_ADAPTER_TAG, "CARAXmppConnectedCB OUT");
}

void CARAXmppDisonnectedCB(void * const param, xmpp_error_code_t result,
        xmpp_connection_handle_t connection)
{
    OIC_LOG(DEBUG, RA_ADAPTER_TAG, "CARAXmppDisonnectedCB IN");
    char jabberID[CA_RAJABBERID_SIZE];
    oc_mutex_lock (g_raadapterMutex);

    g_xmppData.connection_status = CA_INTERFACE_DOWN;
    xmpp_message_context_destroy(g_xmppData.message_context);
    OICStrcpy (jabberID, CA_RAJABBERID_SIZE, g_xmppData.jabberID);

    oc_mutex_unlock (g_raadapterMutex);

    // Notify network change to CA
    CARANotifyNetworkChange(jabberID, CA_INTERFACE_DOWN);

    OIC_LOG(DEBUG, RA_ADAPTER_TAG, "CARAXmppDisonnectedCB OUT");
}

void CARAXmppMessageSentCB(void * const param, xmpp_error_code_t result,
        const void *const recipient, const void *const msg, size_t messageOctets)
{
    OIC_LOG_V(DEBUG, RA_ADAPTER_TAG, "Sending message to %s has result %d",
        recipient, result);
}

void CARAXmppMessageReceivedCB(void * const param, xmpp_error_code_t result,
        const void *const sender, const void *const msg, size_t messageOctets)
{
    if (g_networkPacketCallback)
    {
        VERIFY_NON_NULL_VOID(sender, RA_ADAPTER_TAG, "sender is NULL");
        VERIFY_NON_NULL_VOID(msg,    RA_ADAPTER_TAG, "message is NULL");

        OIC_LOG_V (ERROR, RA_ADAPTER_TAG, "Message received from %s", sender);
        OIC_LOG_V (ERROR, RA_ADAPTER_TAG, "Message reception result %d", result);

        CAEndpoint_t *endPoint = CACreateEndpointObject(CA_DEFAULT_FLAGS,
                        CA_ADAPTER_REMOTE_ACCESS, sender, 0);
        if (!endPoint)
        {
            OIC_LOG(ERROR, RA_ADAPTER_TAG, "EndPoint creation failed!");
            return;
        }

        void *buf = OICMalloc(messageOctets);
        if (!buf)
        {
            OIC_LOG(ERROR, RA_ADAPTER_TAG, "Memory alloc of message failed!");
            CAFreeEndpoint(endPoint);
            return;
        }
        memcpy(buf, msg, messageOctets);
        CANetworkPacketReceivedCallback networkPacketCallback = g_networkPacketCallback;
        if (networkPacketCallback)
        {
            g_networkPacketCallback(endPoint, buf, messageOctets);
        }

        CAFreeEndpoint (endPoint);
    }
    else
    {
        OIC_LOG_V (ERROR, RA_ADAPTER_TAG, "No callback for RA  received message found");
    }
}

CAResult_t CAInitializeRA(CARegisterConnectivityCallback registerCallback,
                          CANetworkPacketReceivedCallback networkPacketCallback,
                          CAAdapterChangeCallback netCallback, ca_thread_pool_t handle)
{
    OIC_LOG(DEBUG, RA_ADAPTER_TAG, "CAInitializeRA IN");
    if (!registerCallback || !networkPacketCallback || !netCallback || !handle)
    {
        return CA_STATUS_INVALID_PARAM;
    }

    g_networkChangeCallback = netCallback;
    g_networkPacketCallback = networkPacketCallback;

    CAConnectivityHandler_t raHandler = {
        .startAdapter = CAStartRA,
        .stopAdapter = CAStopRA,
        .startListenServer = CAStartRAListeningServer,
        .stopListenServer = CAStopRAListeningServer,
        .startDiscoveryServer = CAStartRADiscoveryServer,
        .sendData = CASendRAUnicastData,
        .sendDataToAll = CASendRAMulticastData,
        .GetnetInfo = CAGetRAInterfaceInformation,
        .readData = CAReadRAData,
        .terminate = CATerminateRA,
        .cType = CA_ADAPTER_REMOTE_ACCESS};
    registerCallback(raHandler);

    return CA_STATUS_OK;
}

CAResult_t CASetRAInfo(const CARAInfo_t *caraInfo)
{
    if (!caraInfo)
    {
        return CA_STATUS_INVALID_PARAM;
    }
    xmpp_identity_init(&g_xmppData.g_identity, caraInfo->username, caraInfo->password,
            caraInfo->user_jid, XMPP_TRY_IN_BAND_REGISTER);
    xmpp_host_init(&g_xmppData.g_host, caraInfo->hostname, caraInfo->port,
            caraInfo->xmpp_domain, XMPP_PROTOCOL_XMPP);
    return CA_STATUS_OK;
}

void CATerminateRA()
{
    CAStopRA();
}

CAResult_t CAStartRA()
{
    if (g_xmppData.handle.abstract_handle)
    {
        OIC_LOG(WARNING, RA_ADAPTER_TAG, "RA adapter already started");
        return CA_STATUS_OK;
    }

    OIC_LOG(DEBUG, RA_ADAPTER_TAG, PCF("Starting RA adapter"));

    g_raadapterMutex = oc_mutex_new ();
    if (!g_raadapterMutex)
    {
        OIC_LOG (ERROR, RA_ADAPTER_TAG, PCF("Memory allocation for mutex failed."));
        return CA_MEMORY_ALLOC_FAILED;
    }

    oc_mutex_lock (g_raadapterMutex);

    xmpp_context_init(&g_xmppData.context);
    g_xmppData.handle = xmpp_startup(&g_xmppData.context);

    // Wire up connection callbacks and call API to connect to XMPP server
    g_xmppData.connection_callback.on_connected = CARAXmppConnectedCB;
    g_xmppData.connection_callback.on_disconnected = CARAXmppDisonnectedCB;

    xmpp_error_code_t ret = xmpp_connect(g_xmppData.handle, &g_xmppData.g_host,
            &g_xmppData.g_identity, g_xmppData.connection_callback);

    // Destroy host and identity structures as they are only
    // required to establish initial connection
    xmpp_identity_destroy(&g_xmppData.g_identity);
    xmpp_host_destroy(&g_xmppData.g_host);

    oc_mutex_unlock (g_raadapterMutex);

    if (XMPP_ERR_OK != ret)
    {
        OIC_LOG_V(ERROR, RA_ADAPTER_TAG, "Failed to init XMPP connection status: %d",
            ret);
        return CA_STATUS_FAILED;
    }

    OIC_LOG(DEBUG, RA_ADAPTER_TAG, "RA adapter started succesfully");
    return CA_STATUS_OK;
}

CAResult_t CAStopRA()
{
    OIC_LOG(DEBUG, RA_ADAPTER_TAG, PCF("Stopping RA adapter"));

    xmpp_error_code_t ret = xmpp_close(g_xmppData.connection_handle);
    if (XMPP_ERR_OK != ret)
    {
        OIC_LOG_V(ERROR, RA_ADAPTER_TAG, "Failed to close XMPP connection, status: %d",
            ret);
        return CA_STATUS_FAILED;
    }

    xmpp_shutdown_xmpp(g_xmppData.handle);
    xmpp_context_destroy(&g_xmppData.context);
    oc_mutex_free (g_raadapterMutex);
    g_raadapterMutex = NULL;

    OIC_LOG(DEBUG, RA_ADAPTER_TAG, PCF("Stopped RA adapter successfully"));
    return CA_STATUS_OK;
}

int32_t CASendRAUnicastData(const CAEndpoint_t *remoteEndpoint, const void *data,
                            uint32_t dataLength, CADataType_t dataType)
{
    (void)dataType;
    if (!remoteEndpoint || !data)
    {
        OIC_LOG(ERROR, RA_ADAPTER_TAG, "Invalid parameter!");
        return -1;
    }

    if (0 == dataLength)
    {
        OIC_LOG(ERROR, RA_ADAPTER_TAG, "Data length is 0!");
        return 0;
    }

    OIC_LOG_V(ERROR, RA_ADAPTER_TAG, "Sending unicast data to %s", remoteEndpoint->addr);
    oc_mutex_lock (g_raadapterMutex);

    if (CA_INTERFACE_UP != g_xmppData.connection_status)
    {
        OIC_LOG(ERROR, RA_ADAPTER_TAG, "Unable to send XMPP message, RA not connected");
        oc_mutex_unlock (g_raadapterMutex);
        return -1;
    }

    xmpp_error_code_t res = xmpp_send_message(g_xmppData.message_context,
            remoteEndpoint->addr, data, dataLength,
            XMPP_MESSAGE_TRANSMIT_DEFAULT);
    if (XMPP_ERR_OK != res)
    {
        OIC_LOG_V(ERROR, RA_ADAPTER_TAG, "Unable to send XMPP message, status: %d", res);
        oc_mutex_unlock (g_raadapterMutex);
        return -1;
    }
    oc_mutex_unlock (g_raadapterMutex);

    OIC_LOG_V(INFO, RA_ADAPTER_TAG, "Successfully dispatched bytes[%d] to addr[%s]",
            dataLength, remoteEndpoint->addr);

    return dataLength;
}

CAResult_t CAGetRAInterfaceInformation(CAEndpoint_t **info, uint32_t *size)
{
    VERIFY_NON_NULL(info, RA_ADAPTER_TAG, "info is NULL");
    VERIFY_NON_NULL(size, RA_ADAPTER_TAG, "size is NULL");

    oc_mutex_lock (g_raadapterMutex);

    if (CA_INTERFACE_UP != g_xmppData.connection_status)
    {
        OIC_LOG(ERROR, RA_ADAPTER_TAG, "Failed to get interface info, RA not Connected");
        oc_mutex_unlock (g_raadapterMutex);
        return CA_ADAPTER_NOT_ENABLED;
    }

    oc_mutex_unlock (g_raadapterMutex);

    CAEndpoint_t *localEndpoint = CACreateEndpointObject(CA_DEFAULT_FLAGS,
                                 CA_ADAPTER_REMOTE_ACCESS,
                                 g_xmppData.jabberID, 0);

    *size = 1;
    *info = localEndpoint;

    return CA_STATUS_OK;
}

int32_t CASendRAMulticastData(const CAEndpoint_t *endpoint,
                              const void *data, uint32_t dataLength,
                              CADataType_t dataType)
{
    OIC_LOG(INFO, RA_ADAPTER_TAG, "RA adapter does not support sending multicast data");
    return 0;
}

CAResult_t CAStartRAListeningServer()
{
    OIC_LOG(INFO, RA_ADAPTER_TAG, "RA adapter does not support listening for multicast data");
    return CA_NOT_SUPPORTED;
}

CAResult_t CAStopRAListeningServer()
{
    OIC_LOG(INFO, RA_ADAPTER_TAG, "RA adapter does not support listening for multicast data");
    return CA_NOT_SUPPORTED;
}

CAResult_t CAStartRADiscoveryServer()
{
    OIC_LOG(INFO, RA_ADAPTER_TAG, "RA adapter does not support discovery of multicast servers");
    return CA_NOT_SUPPORTED;
}

CAResult_t CAReadRAData()
{
    OIC_LOG(INFO, RA_ADAPTER_TAG, "Read data is not implemented for the RA adapter");
    return CA_NOT_SUPPORTED;
}
#endif
